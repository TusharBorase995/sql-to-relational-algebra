# ==========================================
# Stage 1: Build the C++ Compiler Binary
# ==========================================
FROM debian:bookworm-slim AS compiler-builder

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    flex \
    bison \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build/compiler
COPY compiler/ .

# Direct build with flex, bison, and g++ (eliminates make tab issues)
RUN mkdir -p bin && \
    bison -d -v -o src/parser.tab.cpp src/parser.y && \
    flex -o src/lex.yy.cpp src/lexer.l && \
    g++ -std=gnu++14 -O2 -Isrc src/parser.tab.cpp src/lex.yy.cpp src/ast.cpp src/schema.cpp src/ra.cpp src/optimizer.cpp src/main.cpp -o bin/sql2ra

# ==========================================
# Stage 2: Build the React Frontend
# ==========================================
FROM node:22-slim AS frontend-builder

WORKDIR /build/frontend
COPY frontend/package*.json ./
RUN npm ci

COPY frontend/ .
ENV NODE_OPTIONS="--max-old-space-size=400"
RUN npm run build

# ==========================================
# Stage 3: Unified Production Runtime
# ==========================================
FROM node:22-slim AS runner

RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Install backend dependencies
COPY backend/package*.json ./backend/
RUN cd backend && npm ci --omit=dev

# Copy backend server code
COPY backend/ ./backend/

# Copy compiled Linux sql2ra binary from compiler-builder
COPY --from=compiler-builder /build/compiler/bin/sql2ra ./compiler/bin/sql2ra
RUN chmod +x ./compiler/bin/sql2ra

# Copy compiled frontend static assets from frontend-builder
COPY --from=frontend-builder /build/frontend/dist ./frontend/dist

ENV NODE_ENV=production
ENV PORT=5000
EXPOSE 5000

CMD ["node", "backend/server.js"]
