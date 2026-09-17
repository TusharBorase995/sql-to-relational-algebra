const express = require('express');
const cors = require('cors');
const { spawn } = require('child_process');
const path = require('path');
const fs = require('fs');
const os = require('os');

const app = express();
const BINARY_FILENAME = process.platform === 'win32' ? 'sql2ra.exe' : 'sql2ra';
const BINARY_PATH = process.env.SQL2RA_BIN || path.resolve(__dirname, '../compiler/bin', BINARY_FILENAME);


app.use(cors());
app.use(express.json({ limit: '10mb' }));

const DEFAULT_ROWS = {
  students: [
    { id: 1, name: 'Alice Smith', age: 20, dept_id: 1, gpa: 3.8 },
    { id: 2, name: 'Bob Jones', age: 22, dept_id: 1, gpa: 3.2 },
    { id: 3, name: 'Charlie Brown', age: 21, dept_id: 2, gpa: 3.5 },
    { id: 4, name: 'Diana Prince', age: 19, dept_id: 3, gpa: 3.9 }
  ],
  departments: [
    { id: 1, name: 'Computer Science', building: 'Turing Hall' },
    { id: 2, name: 'Mathematics', building: 'Euler Tower' },
    { id: 3, name: 'Physics', building: 'Newton Wing' }
  ],
  courses: [
    { id: 101, code: 'CS101', title: 'Intro to Compilers', dept_id: 1, credits: 4 },
    { id: 102, code: 'CS102', title: 'Data Structures', dept_id: 1, credits: 4 },
    { id: 201, code: 'MATH201', title: 'Linear Algebra', dept_id: 2, credits: 3 }
  ],
  enrollments: [
    { student_id: 1, course_id: 101, semester: 'Fall 2026', grade: 'A' },
    { student_id: 2, course_id: 101, semester: 'Fall 2026', grade: 'B' },
    { student_id: 3, course_id: 102, semester: 'Spring 2026', grade: 'A' }
  ]
};

const SAMPLE_QUERIES = [
  {
    id: "basic-select",
    title: "Basic SELECT & Wildcard",
    category: "Tier 1 — Core",
    sql: "SELECT id, name, age FROM students"
  },
  {
    id: "wildcard",
    title: "SELECT * (All Columns)",
    category: "Tier 1 — Core",
    sql: "SELECT * FROM departments"
  },
  {
    id: "where-conditions",
    title: "WHERE with Comparisons & Logic",
    category: "Tier 1 — Core",
    sql: "SELECT name, age FROM students WHERE age >= 20 AND NOT age <> 25"
  },
  {
    id: "comma-join",
    title: "Multiple Tables via Comma (Implicit Join)",
    category: "Tier 1 — Core",
    sql: "SELECT s.name, d.name FROM students s, departments d WHERE s.dept_id = d.id"
  },
  {
    id: "distinct",
    title: "DISTINCT Projection",
    category: "Tier 1 — Core",
    sql: "SELECT DISTINCT dept_id FROM students"
  },
  {
    id: "inner-join",
    title: "INNER JOIN ... ON Condition",
    category: "Tier 1 — Core",
    sql: "SELECT s.name, e.grade FROM students s INNER JOIN enrollments e ON s.id = e.student_id"
  },
  {
    id: "left-right-join",
    title: "LEFT JOIN / RIGHT JOIN",
    category: "Tier 2 — Extended",
    sql: "SELECT s.name, e.grade FROM students s LEFT JOIN enrollments e ON s.id = e.student_id"
  },
  {
    id: "group-by-agg",
    title: "GROUP BY with Aggregates (COUNT, AVG, SUM)",
    category: "Tier 2 — Extended",
    sql: "SELECT dept_id, COUNT(*), AVG(age), SUM(age) FROM students GROUP BY dept_id"
  },
  {
    id: "having-clause",
    title: "HAVING Clause Filter",
    category: "Tier 2 — Extended",
    sql: "SELECT dept_id, COUNT(*) FROM students GROUP BY dept_id HAVING COUNT(*) > 1"
  },
  {
    id: "order-by",
    title: "ORDER BY (ASC / DESC)",
    category: "Tier 2 — Extended",
    sql: "SELECT name, age, gpa FROM students ORDER BY age DESC, name ASC"
  },
  {
    id: "like-in-between",
    title: "LIKE, IN, BETWEEN Operators",
    category: "Tier 2 — Extended",
    sql: "SELECT name FROM students WHERE name LIKE 'A%' AND dept_id IN (1, 2) AND age BETWEEN 18 AND 25"
  },
  {
    id: "union",
    title: "UNION Operation",
    category: "Tier 3 — Set Operations",
    sql: "SELECT name FROM students UNION SELECT name FROM departments"
  },
  {
    id: "intersect",
    title: "INTERSECT Operation",
    category: "Tier 3 — Set Operations",
    sql: "SELECT student_id FROM enrollments WHERE course_id = 101 INTERSECT SELECT student_id FROM enrollments WHERE course_id = 102"
  },
  {
    id: "except",
    title: "EXCEPT (MINUS) Operation",
    category: "Tier 3 — Set Operations",
    sql: "SELECT id FROM students EXCEPT SELECT student_id FROM enrollments"
  },
  {
    id: "optimizer-demo-1",
    title: "Optimizer Demo: Pushdown & Join Conversion",
    category: "Optimization Showcase",
    sql: "SELECT s.name, d.name FROM students s, departments d WHERE s.age > 20 AND s.dept_id = d.id"
  },
  {
    id: "optimizer-demo-2",
    title: "Optimizer Demo: Filter Pushdown to Both Branches",
    category: "Optimization Showcase",
    sql: "SELECT s.name, e.grade FROM students s INNER JOIN enrollments e ON s.id = e.student_id WHERE s.age >= 21 AND e.grade = 'A'"
  }
];

// Active custom schema in memory (empty by default so demo tables don't appear until requested)
let activeCustomSchema = { tables: [] };

// Helper to get compiler's default sample tables with mock data rows
function getSampleDatabase() {
  return new Promise((resolve, reject) => {
    if (!fs.existsSync(BINARY_PATH)) {
      return reject(new Error('Compiler binary not found'));
    }
    const child = spawn(BINARY_PATH, ['--schema']);
    let stdout = '';
    child.stdout.on('data', d => { stdout += d.toString(); });
    child.on('close', code => {
      try {
        const parsed = JSON.parse(stdout);
        if (parsed.tables && Array.isArray(parsed.tables)) {
          parsed.tables = parsed.tables.map(t => ({
            ...t,
            rows: DEFAULT_ROWS[t.name] || []
          }));
        }
        resolve(parsed);
      } catch (err) {
        reject(err);
      }
    });
    child.on('error', reject);
  });
}

// Health endpoint
app.get('/api/health', (req, res) => {
  res.json({
    status: 'ok',
    binaryExists: fs.existsSync(BINARY_PATH),
    binaryPath: BINARY_PATH
  });
});

// Sample queries endpoint
app.get('/api/samples', (req, res) => {
  res.json(SAMPLE_QUERIES);
});

// Endpoint to fetch mock sample database (when explicitly requested by user)
app.get('/api/schema/sample', async (req, res) => {
  try {
    const sample = await getSampleDatabase();
    res.json(sample);
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
});

// Current active schema endpoint
app.get('/api/schema', (req, res) => {
  res.json(activeCustomSchema || { tables: [] });
});

// Update, clear, or load mock schema endpoint
app.post('/api/schema', async (req, res) => {
  const { schema, clear, loadSample } = req.body;

  if (clear) {
    activeCustomSchema = { tables: [] };
    return res.json({ success: true, schema: activeCustomSchema, message: 'All tables cleared.' });
  }

  if (loadSample) {
    try {
      const sample = await getSampleDatabase();
      activeCustomSchema = sample;
      return res.json({ success: true, schema: activeCustomSchema, message: 'Sample mock database loaded.' });
    } catch (err) {
      return res.status(500).json({ success: false, error: err.message });
    }
  }

  if (!schema || !Array.isArray(schema.tables)) {
    return res.status(400).json({ success: false, error: 'Invalid schema format. Expected { tables: [...] }' });
  }

  activeCustomSchema = schema;
  res.json({ success: true, schema: activeCustomSchema });
});

// Compile SQL endpoint (supports custom schema)
app.post('/api/compile', (req, res) => {
  const sql = req.body.sql;
  const customSchema = req.body.schema !== undefined ? req.body.schema : activeCustomSchema;

  if (typeof sql !== 'string') {
    return res.status(400).json({
      success: false,
      error: { message: 'Request body must contain a "sql" string property.' }
    });
  }

  if (!fs.existsSync(BINARY_PATH)) {
    return res.status(500).json({
      success: false,
      error: { message: 'Compiler binary not found at ' + BINARY_PATH + '. Please build the compiler first.' }
    });
  }

  const args = [];
  let tempSchemaPath = null;

  if (customSchema && Array.isArray(customSchema.tables)) {
    tempSchemaPath = path.join(os.tmpdir(), `sql2ra_schema_${Date.now()}_${Math.random().toString(36).substr(2, 6)}.json`);
    try {
      fs.writeFileSync(tempSchemaPath, JSON.stringify(customSchema));
      args.push('--schema-file', tempSchemaPath);
    } catch (err) {
      console.error('Error writing temp schema file:', err);
    }
  }

  const child = spawn(BINARY_PATH, args);
  let stdout = '';
  let stderr = '';
  let timedOut = false;

  const cleanup = () => {
    if (tempSchemaPath && fs.existsSync(tempSchemaPath)) {
      try {
        fs.unlinkSync(tempSchemaPath);
      } catch (e) {}
    }
  };

  const timer = setTimeout(() => {
    timedOut = true;
    child.kill();
    cleanup();
    res.status(504).json({
      success: false,
      error: { message: 'Compilation timed out after 5000ms.' }
    });
  }, 5000);

  child.stdout.on('data', data => { stdout += data.toString(); });
  child.stderr.on('data', data => { stderr += data.toString(); });

  child.on('close', (code, signal) => {
    clearTimeout(timer);
    cleanup();
    if (timedOut) return;

    try {
      const result = JSON.parse(stdout);
      res.json(result);
    } catch (parseErr) {
      res.status(500).json({
        success: false,
        error: {
          message: 'Failed to parse compiler output as JSON.',
          details: stdout || stderr,
          exitCode: code
        }
      });
    }
  });

  child.on('error', err => {
    clearTimeout(timer);
    cleanup();
    if (timedOut) return;
    res.status(500).json({
      success: false,
      error: { message: 'Subprocess error: ' + err.message }
    });
  });

  // Write SQL to stdin and close pipe
  child.stdin.write(sql);
  child.stdin.end();
});

// Serve production static frontend if dist directory exists
const FRONTEND_DIST = path.resolve(__dirname, '../frontend/dist');
if (fs.existsSync(FRONTEND_DIST)) {
  app.use(express.static(FRONTEND_DIST));
  app.get('*', (req, res, next) => {
    if (req.path.startsWith('/api')) return next();
    res.sendFile(path.join(FRONTEND_DIST, 'index.html'));
  });
}

app.listen(PORT, () => {
  console.log(`SQL-to-RA Compiler Backend running on http://localhost:${PORT}`);
  console.log(`Using compiler binary at: ${BINARY_PATH}`);
});

