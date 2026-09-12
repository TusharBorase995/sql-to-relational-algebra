# Web-Based SQL-to-Relational-Algebra Compiler

A complete, end-to-end SQL-to-Relational-Algebra compiler suite for a **Compiler Design** course project. Built with classic **Flex (.l)** and **Bison (.y)** compiled to a high-performance C++ binary, wrapped by a **Node.js/Express** backend, and visualized through a modern **React + CodeMirror + D3.js** web interface.

---

## Architecture Overview

```
                   ┌────────────────────────────────────────┐
                   │          Web Browser (Client)          │
                   │  React + CodeMirror 6 + D3.js Tree UI  │
                   └───────────────────▲────────────────────┘
                                       │ HTTP / JSON
                                       ▼
                   ┌────────────────────────────────────────┐
                   │       Node.js / Express Backend        │
                   │    POST /api/compile  |  GET /api/...  │
                   └───────────────────▲────────────────────┘
                                       │ stdin (SQL) / stdout (JSON)
                                       ▼
                   ┌────────────────────────────────────────┐
                   │          C++ Compiler Binary           │
                   │           (sql2ra.exe)                 │
                   │  ┌──────────────────────────────────┐  │
                   │  │  Flex Lexer (Line/Col Tracking)   │  │
                   │  ├──────────────────────────────────┤  │
                   │  │  Bison LALR(1) Parser Grammar     │  │
                   │  ├──────────────────────────────────┤  │
                   │  │  C++ Abstract Syntax Tree (AST)   │  │
                   │  ├──────────────────────────────────┤  │
                   │  │  Schema Catalog & Validator       │  │
                   │  ├──────────────────────────────────┤  │
                   │  │  AST-to-RA Canonical Translator   │  │
                   │  ├──────────────────────────────────┤  │
                   │  │  Relational Algebra Optimizer     │  │
                   │  └──────────────────────────────────┘  │
                   └────────────────────────────────────────┘
```

---

## SQL Grammar Scope (14 Features Across 3 Tiers)

### Tier 1 — Core
1. **Basic SELECT**: `SELECT col1, col2 FROM table`
2. **SELECT \* (Wildcard projection)**: `SELECT * FROM table`
3. **WHERE clause**: with `=`, `<`, `>`, `<=`, `>=`, `<>`, `AND`, `OR`, `NOT`
4. **Multiple tables via comma**: implicit Cartesian join (`FROM t1, t2 WHERE t1.x = t2.y`)
5. **DISTINCT**: duplicate elimination (`SELECT DISTINCT ...`)
6. **INNER JOIN ... ON condition**: theta join (`FROM t1 INNER JOIN t2 ON ...`)

### Tier 2 — Extended
7. **LEFT JOIN / RIGHT JOIN ... ON condition**: outer theta joins tagged as LEFT/RIGHT
8. **GROUP BY with Aggregate functions**: `COUNT(*)`, `COUNT(DISTINCT col)`, `SUM(col)`, `AVG(col)`, `MIN(col)`, `MAX(col)`
9. **HAVING clause**: post-aggregation filter conditions
10. **ORDER BY**: sorting by multiple attributes with `ASC` / `DESC`
11. **LIKE, IN, BETWEEN**: pattern matching, set membership, and range expressions (with `NOT LIKE`, `NOT IN`, `NOT BETWEEN`)

### Tier 3 — Set Operations
12. **UNION**: set union (and `UNION ALL`, `UNION DISTINCT`)
13. **INTERSECT**: set intersection with standard SQL precedence over UNION/EXCEPT
14. **EXCEPT (MINUS)**: set difference

---

## Relational Algebra Operator Mapping

| SQL Clause / Feature | Relational Algebra Symbol | Operation Name | Description |
| :--- | :---: | :--- | :--- |
| `SELECT` columns | $\pi$ | Projection | Projects specified expressions/columns |
| `WHERE` condition | $\sigma$ | Selection | Filters tuples satisfying condition |
| `FROM t1, t2` | $\times$ | Cartesian Product | Cross product of unjoined tables |
| `JOIN ... ON` | $\bowtie$ | Theta Join | Join with condition; tagged INNER/LEFT/RIGHT |
| `GROUP BY` + Aggregates | $\gamma$ | Aggregation | Groups by attributes and computes aggregates |
| `HAVING` condition | $\sigma$ | Selection (HAVING) | Filters aggregated groups |
| `ORDER BY` | $\tau$ | Sort / Ordering | Orders tuples by specified keys |
| `DISTINCT` | $\delta$ | Duplicate Elimination | Eliminates duplicate tuples |
| `UNION` | $\cup$ | Set Union | Combines tuples from two queries |
| `INTERSECT` | $\cap$ | Set Intersection | Retains common tuples |
| `EXCEPT` / `MINUS` | $-$ | Set Difference | Tuples in left query not in right query |

---

## Query Optimization Engine

The optimizer implements classic heuristic query optimization rules:
1. **Merge Cascaded Selections**: $\sigma_{c1}(\sigma_{c2}(R)) \longrightarrow \sigma_{c1 \land c2}(R)$
2. **Push Selection Down**:
   - Pushes predicates as close to base relations as possible.
   - Pushes branch-specific predicates below joins to filter relations *before* joining.
   - **Cartesian Product to Theta Join Conversion**: Identifies join predicates over $R_1 \times R_2$ and converts them to $R_1 \bowtie_{cond} R_2$.
3. **Merge Cascaded Projections**: $\pi_{L1}(\pi_{L2}(R)) \longrightarrow \pi_{L1}(R)$
4. **Push Projection Down**: Eliminates unnecessary attributes early before joins, pushing early $\pi$ nodes below joins.

Both the **Canonical (Unoptimized)** and **Optimized** RA trees and linear formulas are produced side-by-side for comparison.

---

## Sample Schema Catalog

The system includes a pre-loaded relational database schema:
- **`students`** (`id` INT PK, `name` VARCHAR, `age` INT, `dept_id` INT FK $\to$ `departments.id`, `gpa` FLOAT)
- **`departments`** (`id` INT PK, `name` VARCHAR, `building` VARCHAR)
- **`courses`** (`id` INT PK, `code` VARCHAR, `title` VARCHAR, `dept_id` INT FK $\to$ `departments.id`, `credits` INT)
- **`enrollments`** (`student_id` INT FK $\to$ `students.id`, `course_id` INT FK $\to$ `courses.id`, `semester` VARCHAR, `grade` VARCHAR)

The schema validator checks table/column existence and flags ambiguous unqualified column references in multi-table queries.

---

## Project Structure

```
CD_project/
├── tools/
│   └── winflexbison/          # Portable Flex 2.6.4 & Bison 3.8.2 binaries
├── compiler/
│   ├── src/
│   │   ├── lexer.l            # Flex lexer with line/col tracking & token recorder
│   │   ├── parser.y           # Bison LALR(1) grammar for Tiers 1-3
│   │   ├── ast.h / ast.cpp    # C++ AST node hierarchy
│   │   ├── schema.h / schema.cpp # Schema catalog & semantic validation
│   │   ├── ra.h / ra.cpp      # AST -> Relational Algebra translator
│   │   ├── optimizer.h / optimizer.cpp # Selection/projection pushdown & merging
│   │   ├── json_helper.h      # Lightweight JSON string escaping
│   │   └── main.cpp           # CLI driver & error handler
│   ├── build.bat              # Windows compilation script
│   └── bin/
│       └── sql2ra.exe         # Compiled C++ executable
├── backend/
│   ├── package.json
│   └── server.js              # Express API (subprocess pipeline & endpoints)
├── frontend/
│   ├── package.json
│   ├── vite.config.js         # Vite configuration with /api proxy
│   ├── src/
│   │   ├── App.jsx            # Main app shell & state management
│   │   ├── index.css          # Design system & dark theme tokens
│   │   └── components/
│   │       ├── SqlEditor.jsx  # CodeMirror SQL editor with error markers
│   │       ├── RaTree.jsx     # Interactive D3.js node-link tree
│   │       ├── TokenTable.jsx # Filterable token stream panel
│   │       ├── SchemaBrowser.jsx # Database schema inspector
│   │       └── NodeInspector.jsx # Node details side drawer
└── README.md
```

---

## Getting Started

### Prerequisites
- **Flex & Bison**: Included in `tools/winflexbison/` (no global installation required).
- **C++ Compiler**: MinGW GCC 6.3+ (`g++`).
- **Node.js**: v18+ & npm.

---

### Step 1: Build the Compiler Binary

Open a terminal in `compiler/` and run the build script:

```bash
cd compiler
build.bat
```

This invokes `win_bison` and `win_flex`, then compiles `sql2ra.exe` into `compiler/bin/sql2ra.exe`.

#### Test the Binary Directly (CLI):
```bash
bin\sql2ra.exe -c "SELECT s.name, d.name FROM students s, departments d WHERE s.age > 20 AND s.dept_id = d.id"
```
Or view the schema catalog:
```bash
bin\sql2ra.exe --schema
```

---

### Step 2: Run the Backend Server

```bash
cd backend
npm install
npm start
```
The server will start at `http://localhost:5000`.

- `GET /api/health` — Check server and compiler status
- `GET /api/schema` — Retrieve catalog tables and columns
- `GET /api/samples` — Retrieve pre-loaded queries
- `POST /api/compile` — Compile SQL piped via stdin

---

### Step 3: Run the React Frontend

Open a new terminal:

```bash
cd frontend
npm install
npm run dev
```

Open your browser at:
```
http://localhost:5173
```

---

## User Interface Features

1. **CodeMirror SQL Editor**: Full syntax highlighting, line numbers, automatic bracket matching, and inline red squiggly underlines on syntax errors.
2. **Interactive D3.js RA Tree**:
   - Math operator symbols ($\pi, \sigma, \bowtie, \times, \gamma, \tau, \delta, \cup, \cap, -$).
   - Color-coded badges by operator type.
   - Smooth pan & zoom controls (Zoom in, Zoom out, Reset view).
   - Click any node to open the **Node Inspector** drawer showing condition, attributes, and subtree formula.
3. **Optimized vs. Unoptimized Toggle**: Switch views instantly to observe selection pushdowns, Cartesian-to-Theta-join conversions, and early projections.
4. **Linear RA Formula Bar**: Formatted mathematical string representation with a one-click copy button.
5. **Sample Queries Dropdown**: 16 pre-loaded queries covering every grammar feature and optimization pattern.
6. **Token Stream Table**: Searchable and filterable table displaying every token with type, lexeme, line, and column.
7. **Catalog Schema Browser**: Interactive tree of database tables, columns, data types, primary keys (PK), and foreign keys (FK).
