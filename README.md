# SQL-to-Relational-Algebra (sql2ra)

[![Live Demo](https://img.shields.io/badge/Live%20Demo-Render-46E3B7?style=for-the-badge&logo=render&logoColor=white)](https://sql-to-relational-algebra.onrender.com/)
[![C++](https://img.shields.io/badge/C%2B%2B-14-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Flex & Bison](https://img.shields.io/badge/Parser-Flex%20%26%20Bison-orange?style=for-the-badge)](https://www.gnu.org/software/bison/)
[![Node.js](https://img.shields.io/badge/Backend-Node.js%20Express-339933?style=for-the-badge&logo=node.js&logoColor=white)](https://nodejs.org/)
[![React](https://img.shields.io/badge/Frontend-React%2019%20%2B%20Vite-61DAFB?style=for-the-badge&logo=react&logoColor=black)](https://react.dev/)
[![Docker](https://img.shields.io/badge/Deployment-Docker%20Multi--Stage-2496ED?style=for-the-badge&logo=docker&logoColor=white)](https://www.docker.com/)

A full-stack query compiler and visual optimizer that translates complex SQL queries into Relational Algebra expressions and interactive evaluation trees. Built with a native **Flex & Bison** LALR(1) compiler pipeline in C++, wrapped by an **Express API**, and rendered with **React 19**, **CodeMirror 6**, and **D3.js**.

🌐 **Live Application**: [https://sql-to-relational-algebra.onrender.com/](https://sql-to-relational-algebra.onrender.com/)

---

## Key Features

- **LALR(1) SQL Parsing**: Full lexical analysis and syntax checking via GNU Flex and Bison with exact line/column error tracking.
- **Relational Algebra Translator**: Converts parsed AST nodes into canonical algebraic operator trees ($\pi, \sigma, \bowtie, \times, \gamma, \tau, \delta, \cup, \cap, -$).
- **Rule-Based Query Optimizer**:
  - Pushes selections ($\sigma$) below Cartesian products and joins.
  - Converts Cartesian products ($R \times S$) into Theta Joins ($R \bowtie_\theta S$) when join conditions exist.
  - Merges cascaded selections ($\sigma_{c1}(\sigma_{c2}(R)) \to \sigma_{c1 \land c2}(R)$).
  - Pushes projections ($\pi$) downward to prune unnecessary attributes early.
- **Side-by-Side Tree Visualization**: Interactive D3.js hierarchical tree comparison between **Canonical (Unoptimized)** and **Optimized** execution plans.
- **Mathematical Formula Representation**: LaTeX-rendered and linear mathematical strings with one-click copy.
- **Custom Schema Manager & Browser**: Inspect preloaded tables or define custom relational schemas with live foreign-key validation.
- **Token Stream Inspector**: Full inspection of lexical tokens emitted by Flex with source position offsets.

---

## Architecture

```
                       ┌────────────────────────────────────────┐
                       │          Browser Client (SPA)          │
                       │   React 19 + CodeMirror 6 + D3.js      │
                       └───────────────────▲────────────────────┘
                                           │  HTTP / JSON
                                           ▼
                       ┌────────────────────────────────────────┐
                       │       Node.js / Express Backend        │
                       │     API Routing + Static Assets        │
                       └───────────────────▲────────────────────┘
                                           │  stdin (SQL) / stdout (JSON)
                                           ▼
┌────────────────────────────────────────────────────────────────────────────────────────┐
│                                 C++ Engine (sql2ra)                                    │
│                                                                                        │
│  ┌───────────────────────┐      ┌────────────────────────┐      ┌───────────────────┐  │
│  │      Flex Lexer       │ ───> │  Bison LALR(1) Parser  │ ───> │ Abstract Syntax   │  │
│  │ (tokens, line/col)    │      │  (Grammar validation)  │      │ Tree (AST) Nodes  │  │
│  └───────────────────────┘      └────────────────────────┘      └─────────┬─────────┘  │
│                                                                           │            │
│  ┌───────────────────────┐      ┌────────────────────────┐                │            │
│  │   Heuristic Query     │ <─── │   Canonical RA Tree    │ <──────────────┘            │
│  │  Optimizer (Pushdown) │      │  Generator (π, σ, ⋈)   │                             │
│  └──────────┬────────────┘      └────────────────────────┘                             │
│             │                                                                          │
│             ▼                                                                          │
│   JSON Serialization (AST, Token Stream, Canonical Plan, Optimized Plan, Metadata)     │
└────────────────────────────────────────────────────────────────────────────────────────┘
```

---

## Operator Mapping Reference

| SQL Clause | Operator | Math Symbol | Algebraic Representation | Description |
| :--- | :--- | :---: | :--- | :--- |
| `SELECT col1, col2` | Projection | $\pi$ | $\pi_{col1, col2}(R)$ | Eliminates unrequested columns |
| `WHERE condition` | Selection | $\sigma$ | $\sigma_{condition}(R)$ | Filters tuples by propositional predicates |
| `FROM t1, t2` | Cross Product | $\times$ | $R \times S$ | Cartesian product of relations |
| `JOIN ... ON` | Theta Join | $\bowtie$ | $R \bowtie_{\theta} S$ | Equi-join / conditional join |
| `GROUP BY ...` | Aggregation | $\gamma$ | $_{dept}\gamma_{AVG(gpa)}(R)$ | Tuple grouping and aggregate computation |
| `HAVING condition` | Post-Selection | $\sigma$ | $\sigma_{predicate}(\gamma(R))$ | Filters aggregated groups |
| `ORDER BY` | Sort | $\tau$ | $\tau_{age \text{ DESC}}(R)$ | Sorts output by specified ordering keys |
| `DISTINCT` | Deduplication | $\delta$ | $\delta(R)$ | Eliminates duplicate output records |
| `UNION` | Set Union | $\cup$ | $R \cup S$ | Set union of compatible relations |
| `INTERSECT` | Set Intersection | $\cap$ | $R \cap S$ | Set intersection |
| `EXCEPT` | Set Difference | $-$ | $R - S$ | Tuples in $R$ not present in $S$ |

---

## SQL Dialect & Grammar Coverage

The compiler parses ANSI-SQL queries across three tiered capability levels:

### Tier 1 — Core Relational Operators
- Attribute and wildcard projections (`SELECT *`, `SELECT id, name`)
- Complex conditional expressions (`=`, `<>`, `<`, `>`, `<=`, `>=`, `AND`, `OR`, `NOT`)
- Implicit cross joins via table lists (`FROM students, departments`)
- Explicit theta joins (`FROM students INNER JOIN departments ON students.dept_id = departments.id`)
- Duplicate elimination (`SELECT DISTINCT department FROM courses`)

### Tier 2 — Aggregates, Grouping & Ordering
- Aggregate functions: `COUNT(*)`, `COUNT(DISTINCT col)`, `SUM()`, `AVG()`, `MIN()`, `MAX()`
- Grouping: `GROUP BY col1, col2`
- Post-aggregation filters: `HAVING COUNT(*) > 5`
- Ordering: `ORDER BY gpa DESC, age ASC`
- Pattern matching & range filters: `LIKE`, `NOT LIKE`, `IN (...)`, `BETWEEN ... AND ...`

### Tier 3 — Multi-Query Set Operations
- Set Unions: `UNION`, `UNION ALL`, `UNION DISTINCT`
- Set Intersections: `INTERSECT`
- Set Differences: `EXCEPT`, `MINUS`

---

## Default Catalog Schema

The engine includes built-in semantic verification against a relational university catalog:

```sql
students    (id PK, name, age, dept_id FK -> departments.id, gpa)
departments (id PK, name, building)
courses     (id PK, code, title, dept_id FK -> departments.id, credits)
enrollments (student_id FK -> students.id, course_id FK -> courses.id, semester, grade)
```

Unqualified column names are automatically resolved, and ambiguous references across joins trigger semantic validation errors.

---

## Local Development & Setup

### Option 1: Docker (Recommended)

Requires Docker Desktop. Builds both C++ engine, backend, and frontend inside an isolated container:

```bash
# Clone the repository
git clone https://github.com/TusharBorase995/sql-to-relational-algebra.git
cd sql-to-relational-algebra

# Build and start the container
docker build -t sql2ra .
docker run -p 5000:5000 sql2ra
```

Open your browser at `http://localhost:5000`.

---

### Option 2: Native Setup (Windows / Linux)

#### Prerequisites
- **C++ Compiler**: MinGW-w64 `g++` (Windows) or GCC `g++` (Linux) with C++14 support
- **Flex & Bison**:
  - Windows: Portable binaries included in `tools/winflexbison/`
  - Linux: `sudo apt-get install -y flex bison build-essential`
- **Node.js**: v18 or higher & `npm`

#### 1. Compile the Core Binary
- **On Windows**:
  ```cmd
  cd compiler
  build.bat
  ```
- **On Linux / macOS**:
  ```bash
  cd compiler
  make
  ```
  The executable will be generated at `compiler/bin/sql2ra` (or `sql2ra.exe`).

#### 2. Start the Backend Service
```bash
cd backend
npm install
npm start
```
Runs at `http://localhost:5000`.

#### 3. Start the Frontend Development Server
```bash
cd frontend
npm install
npm run dev
```
Runs at `http://localhost:5173` (with `/api` automatically proxied to port 5000).

---

## API Documentation

### `POST /api/compile`
Compiles an arbitrary SQL query and returns the parse tree, token stream, canonical plan, and optimized plan.

**Request Body**:
```json
{
  "sql": "SELECT s.name, d.building FROM students s JOIN departments d ON s.dept_id = d.id WHERE s.age >= 21"
}
```

**Response**:
```json
{
  "success": true,
  "tokens": [
    { "type": "SELECT", "lexeme": "SELECT", "line": 1, "column": 1 },
    { "type": "IDENTIFIER", "lexeme": "s", "line": 1, "column": 8 }
  ],
  "ast": { "type": "SelectStmt", "selectList": [...] },
  "ra": {
    "canonical": {
      "type": "Project",
      "formula": "π[s.name, d.building](σ[s.age >= 21](students ⋈[s.dept_id = d.id] departments))",
      "tree": { ... }
    },
    "optimized": {
      "type": "Project",
      "formula": "π[s.name, d.building](σ[s.dept_id = d.id](σ[s.age >= 21](students) × departments))",
      "tree": { ... }
    }
  }
}
```

### `GET /api/schema`
Returns the active relational database schema catalog.

### `POST /api/schema`
Uploads a custom schema definition or resets to the default catalog.

---

## Directory Structure

```
sql-to-relational-algebra/
├── compiler/                  # Native C++ Compiler Engine
│   ├── src/
│   │   ├── lexer.l            # Flex lexer definitions (token rules & line/col tracking)
│   │   ├── parser.y           # Bison LALR(1) grammar specification
│   │   ├── ast.h / ast.cpp    # AST node class hierarchy
│   │   ├── schema.h / .cpp    # Catalog schema manager & semantic validation
│   │   ├── ra.h / ra.cpp      # AST -> Canonical RA conversion
│   │   ├── optimizer.h / .cpp # Relational algebra heuristic optimization rules
│   │   └── main.cpp           # CLI driver & JSON output pipeline
│   ├── Makefile               # Linux compilation target
│   └── build.bat              # Windows compilation target
├── backend/                   # Node.js Express API
│   ├── server.js              # HTTP server, process manager, and static asset host
│   └── package.json
├── frontend/                  # React Single-Page Application
│   ├── src/
│   │   ├── components/
│   │   │   ├── SqlEditor.jsx      # CodeMirror 6 editor with error squiggles
│   │   │   ├── RaTree.jsx         # D3.js interactive tree visualization
│   │   │   ├── RaFormula.jsx      # Mathematical formula bar with copy features
│   │   │   ├── TokenTable.jsx     # Lexical token stream display
│   │   │   ├── SchemaBrowser.jsx  # Interactive schema tree
│   │   │   └── NodeInspector.jsx  # Node properties drawer
│   │   ├── App.jsx                # Main interface & state container
│   │   └── index.css              # Dark theme design system
│   ├── vite.config.js
│   └── package.json
├── Dockerfile                 # Multi-stage production container
├── render.yaml                # Render cloud deployment blueprint
└── README.md
```

---

## License

Distributed under the [MIT License](LICENSE).
Built for educational and research purposes in Compiler Design and Database Management Systems.
