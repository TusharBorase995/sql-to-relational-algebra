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

# Ensure clean build with Linux flex/bison and g++
RUN make clean || true
RUN make

# ==========================================
# Stage 2: Build the React Frontend
# ==========================================
FROM node:20-slim AS frontend-builder

WORKDIR /build/frontend
COPY frontend/package*.json ./
RUN npm ci

COPY frontend/ .
RUN npm run build

# ==========================================
# Stage 3: Unified Production Runtime
# ==========================================
FROM node:20-slim AS runner

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
