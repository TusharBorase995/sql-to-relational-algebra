import React, { useState, useEffect, useRef } from 'react';
import {
  Play,
  Copy,
  Check,
  Database,
  Code2,
  GitBranch,
  Layers,
  Sparkles,
  AlertTriangle,
  Server,
  RefreshCw,
  Table as TableIcon,
  GripHorizontal,
  GripVertical
} from 'lucide-react';
import SqlEditor from './components/SqlEditor';
import RaTree from './components/RaTree';
import TokenTable from './components/TokenTable';
import SchemaManager from './components/SchemaManager';
import NodeInspector from './components/NodeInspector';

const DEFAULT_SQL = `SELECT s.name, d.name
FROM students s, departments d
WHERE s.age > 20 AND s.dept_id = d.id;`;

const OPERATOR_LEGEND = [
  { symbol: 'π', name: 'Projection', color: '#6366f1' },
  { symbol: 'σ', name: 'Selection', color: '#10b981' },
  { symbol: '⋈', name: 'Join', color: '#a855f7' },
  { symbol: '×', name: 'Cross', color: '#f59e0b' },
  { symbol: 'γ', name: 'Aggregation', color: '#e11d48' },
  { symbol: 'δ', name: 'Distinct', color: '#0d9488' },
  { symbol: 'τ', name: 'Sort', color: '#d946ef' },
  { symbol: '∪/∩/−', name: 'Set', color: '#0284c7' }
];

export default function App() {
  const [sql, setSql] = useState(DEFAULT_SQL);
  const [samples, setSamples] = useState([]);
  const [schema, setSchema] = useState({ tables: [] });
  const [loading, setLoading] = useState(false);
  const [serverOk, setServerOk] = useState(true);

  // Compiler results
  const [result, setResult] = useState(null);
  const [syntaxError, setSyntaxError] = useState(null);

  // Active UI views
  const [leftTab, setLeftTab] = useState('editor'); // 'editor', 'tokens'
  const [viewMode, setViewMode] = useState('optimized'); // 'optimized', 'unoptimized'
  const [displayType, setDisplayType] = useState('tree'); // 'tree', 'plan'
  const [selectedNode, setSelectedNode] = useState(null);
  const [copied, setCopied] = useState(false);

  // Resizable Panes State
  const [topHeightPercent, setTopHeightPercent] = useState(56);
  const [leftWidthPx, setLeftWidthPx] = useState(440);
  const [isDraggingH, setIsDraggingH] = useState(false);
  const [isDraggingV, setIsDraggingV] = useState(false);

  useEffect(() => {
    fetchSamples();
    fetchSchema();
  }, []);

  const fetchSamples = async () => {
    try {
      const res = await fetch('/api/samples');
      if (res.ok) {
        const data = await res.json();
        setSamples(data);
      }
    } catch (e) {
      setServerOk(false);
    }
  };

  const fetchSchema = async () => {
    try {
      const res = await fetch('/api/schema');
      if (res.ok) {
        const data = await res.json();
        setSchema(data || { tables: [] });
        handleCompile(DEFAULT_SQL, data || { tables: [] });
      }
    } catch (e) {
      handleCompile(DEFAULT_SQL, { tables: [] });
    }
  };

  const handleCompile = async (overrideSql, overrideSchema) => {
    const queryToRun = typeof overrideSql === 'string' ? overrideSql : sql;
    const activeSchema = overrideSchema !== undefined ? overrideSchema : schema;

    setLoading(true);
    setSyntaxError(null);
    setSelectedNode(null);

    try {
      const res = await fetch('/api/compile', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          sql: queryToRun,
          schema: activeSchema
        })
      });

      const data = await res.json();
      setServerOk(true);

      if (!data.success && data.error) {
        setSyntaxError(data.error);
        setResult(data);
      } else {
        setResult(data);
        setSyntaxError(null);
      }
    } catch (err) {
      setServerOk(false);
      setSyntaxError({ message: 'Could not connect to compiler backend on :5000', line: 1, col: 1 });
    } finally {
      setLoading(false);
    }
  };

  const handleUpdateSchema = async (newSchema) => {
    setSchema(newSchema);
    try {
      await fetch('/api/schema', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ schema: newSchema })
      });
      handleCompile(sql, newSchema);
    } catch (e) {
      console.error(e);
    }
  };

  const handleLoadMockData = async () => {
    try {
      const res = await fetch('/api/schema', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ loadSample: true })
      });
      if (res.ok) {
        const data = await res.json();
        setSchema(data.schema);
        handleCompile(sql, data.schema);
      }
    } catch (e) {
      console.error(e);
    }
  };

  const handleClearAllTables = async () => {
    try {
      const res = await fetch('/api/schema', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ clear: true })
      });
      if (res.ok) {
        const empty = { tables: [] };
        setSchema(empty);
        handleCompile(sql, empty);
      }
    } catch (e) {
      console.error(e);
    }
  };

  const handleSelectSample = (e) => {
    const selectedId = e.target.value;
    if (!selectedId) return;
    const sample = samples.find(s => s.id === selectedId);
    if (sample) {
      setSql(sample.sql);
      handleCompile(sample.sql, schema);
    }
  };

  const handleCopyLinearFormula = () => {
    const formula = viewMode === 'optimized'
      ? result?.ra_string_optimized
      : result?.ra_string_unoptimized;

    if (formula) {
      navigator.clipboard.writeText(formula);
      setCopied(true);
      setTimeout(() => setCopied(false), 2000);
    }
  };

  // Vertical Drag (Left Panel / Right Panel)
  const handleStartDragV = (e) => {
    e.preventDefault();
    setIsDraggingV(true);
    const startX = e.clientX;
    const startWidth = leftWidthPx;

    const onMouseMove = (moveEvent) => {
      const delta = moveEvent.clientX - startX;
      const maxW = window.innerWidth - 350;
      const newW = Math.max(260, Math.min(maxW, startWidth + delta));
      setLeftWidthPx(newW);
    };

    const onMouseUp = () => {
      setIsDraggingV(false);
      window.removeEventListener('mousemove', onMouseMove);
      window.removeEventListener('mouseup', onMouseUp);
    };

    window.addEventListener('mousemove', onMouseMove);
    window.addEventListener('mouseup', onMouseUp);
  };

  // Horizontal Drag (Top Section / Bottom Schema Section)
  const handleStartDragH = (e) => {
    e.preventDefault();
    setIsDraggingH(true);
    const startY = e.clientY;
    const startPercent = topHeightPercent;
    const totalHeight = window.innerHeight - 56; // 56px navbar

    const onMouseMove = (moveEvent) => {
      const deltaY = moveEvent.clientY - startY;
      const deltaPercent = (deltaY / totalHeight) * 100;
      const newPercent = Math.max(20, Math.min(80, startPercent + deltaPercent));
      setTopHeightPercent(newPercent);
    };

    const onMouseUp = () => {
      setIsDraggingH(false);
      window.removeEventListener('mousemove', onMouseMove);
      window.removeEventListener('mouseup', onMouseUp);
    };

    window.addEventListener('mousemove', onMouseMove);
    window.addEventListener('mouseup', onMouseUp);
  };

  const currentRaTree = viewMode === 'optimized'
    ? result?.ra_optimized
    : result?.ra_unoptimized;

  const currentLinearStr = viewMode === 'optimized'
    ? result?.ra_string_optimized
    : result?.ra_string_unoptimized;

  // Render text-based indented execution plan
  const renderPlanList = (node, depth = 0) => {
    if (!node) return null;
    return (
      <div key={node.id} style={{ marginLeft: `${depth * 20}px`, marginTop: '6px', fontFamily: 'var(--font-mono)', fontSize: '12px' }}>
        <div style={{ display: 'flex', alignItems: 'center', gap: '8px' }}>
          <span style={{ color: 'var(--primary)', fontWeight: 'bold' }}>&rarr;</span>
          <span style={{ fontWeight: '600', color: 'var(--text-primary)' }}>{node.op_name} ({node.op_symbol}):</span>
          <span style={{ color: 'var(--text-secondary)' }}>{node.details || node.condition || ''}</span>
        </div>
        {node.children && node.children.map(child => renderPlanList(child, depth + 1))}
      </div>
    );
  };

  return (
    <div
      style={{
        display: 'flex',
        flexDirection: 'column',
        height: '100vh',
        background: 'var(--bg-app)',
        color: 'var(--text-primary)',
        userSelect: (isDraggingH || isDraggingV) ? 'none' : 'auto',
        cursor: isDraggingH ? 'row-resize' : (isDraggingV ? 'col-resize' : 'auto')
      }}
    >
      {/* 1. TOP BAR */}
      <header
        style={{
          height: '56px',
          background: '#ffffff',
          borderBottom: '1px solid var(--border-color)',
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'space-between',
          padding: '0 18px',
          zIndex: 20,
          flexShrink: 0
        }}
      >
        {/* Project Title (Left) */}
        <div style={{ display: 'flex', alignItems: 'center', gap: '12px' }}>
          <div
            style={{
              width: '32px',
              height: '32px',
              borderRadius: 'var(--radius-sm)',
              border: '1px solid var(--border-color)',
              background: '#f8fafc',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'center',
              color: 'var(--primary)'
            }}
          >
            <Database size={18} />
          </div>
          <div>
            <div style={{ fontWeight: '700', fontSize: '15px', color: 'var(--text-primary)', letterSpacing: '-0.2px' }}>
              SQL &rarr; Relational Algebra
            </div>
            <div style={{ fontSize: '11px', color: 'var(--text-muted)' }}>
              Write SQL. See the Relational Algebra.
            </div>
          </div>
        </div>

        {/* Top Bar Right: Sample Query + Compile + Online */}
        <div style={{ display: 'flex', alignItems: 'center', gap: '12px' }}>
          <select
            onChange={handleSelectSample}
            defaultValue=""
            style={{
              background: '#ffffff',
              color: 'var(--text-secondary)',
              border: '1px solid var(--border-color)',
              borderRadius: 'var(--radius-sm)',
              padding: '6px 12px',
              fontSize: '12px',
              outline: 'none',
              cursor: 'pointer',
              maxWidth: '280px'
            }}
          >
            <option value="" disabled>Load Sample Query (14 Features)...</option>
            {['Tier 1 — Core', 'Tier 2 — Extended', 'Tier 3 — Set Operations', 'Optimization Showcase'].map(cat => (
              <optgroup key={cat} label={cat}>
                {samples.filter(s => s.category === cat).map(s => (
                  <option key={s.id} value={s.id}>{s.title}</option>
                ))}
              </optgroup>
            ))}
          </select>

          {/* Top Compile Button */}
          <button
            onClick={() => handleCompile()}
            disabled={loading}
            style={{
              display: 'flex',
              alignItems: 'center',
              gap: '6px',
              background: 'var(--primary)',
              color: '#ffffff',
              border: 'none',
              borderRadius: 'var(--radius-sm)',
              padding: '7px 16px',
              fontSize: '13px',
              fontWeight: '600',
              cursor: loading ? 'not-allowed' : 'pointer',
              transition: 'background 0.15s ease'
            }}
            onMouseEnter={e => !loading && (e.currentTarget.style.background = 'var(--primary-hover)')}
            onMouseLeave={e => !loading && (e.currentTarget.style.background = 'var(--primary)')}
          >
            {loading ? <RefreshCw size={14} className="spin" /> : <Play size={14} fill="#ffffff" />}
            <span>Compile</span>
          </button>

          {/* Online Status Pill */}
          <div
            style={{
              display: 'flex',
              alignItems: 'center',
              gap: '6px',
              fontSize: '12px',
              fontWeight: '500',
              color: serverOk ? '#065f46' : '#b91c1c',
              background: serverOk ? '#ecfdf5' : '#fef2f2',
              padding: '5px 10px',
              borderRadius: '20px',
              border: `1px solid ${serverOk ? '#a7f3d0' : '#fecaca'}`
            }}
          >
            <span
              style={{
                width: '7px',
                height: '7px',
                borderRadius: '50%',
                background: serverOk ? '#059669' : '#dc2626'
              }}
            />
            <span>{serverOk ? 'Online' : 'Offline'}</span>
          </div>
        </div>
      </header>

      {/* 2. TOP SECTION: Resizable Left Panel (SQL Editor) and Right Panel (Output) */}
      <div
        style={{
          height: `${topHeightPercent}%`,
          display: 'flex',
          flexDirection: 'row',
          overflow: 'hidden',
          flexShrink: 0
        }}
      >
        {/* LEFT PANEL (SQL Editor + Token Stream) */}
        <div
          style={{
            width: `${leftWidthPx}px`,
            flexShrink: 0,
            display: 'flex',
            flexDirection: 'column',
            background: '#ffffff',
            height: '100%',
            overflow: 'hidden'
          }}
        >
          {/* Navigation Tabs */}
          <div style={{ display: 'flex', borderBottom: '1px solid var(--border-color)', background: '#f8fafc', padding: '0 8px' }}>
            <button
              onClick={() => setLeftTab('editor')}
              style={{
                display: 'flex',
                alignItems: 'center',
                gap: '6px',
                padding: '9px 14px',
                fontSize: '12px',
                fontWeight: '600',
                background: leftTab === 'editor' ? '#ffffff' : 'transparent',
                color: leftTab === 'editor' ? 'var(--text-primary)' : 'var(--text-muted)',
                border: '1px solid var(--border-color)',
                borderBottom: leftTab === 'editor' ? '1px solid #ffffff' : '1px solid var(--border-color)',
                marginBottom: '-1px',
                borderTopLeftRadius: '4px',
                borderTopRightRadius: '4px',
                cursor: 'pointer'
              }}
            >
              <Code2 size={13} />
              <span>SQL Editor</span>
            </button>

            <button
              onClick={() => setLeftTab('tokens')}
              style={{
                display: 'flex',
                alignItems: 'center',
                gap: '6px',
                padding: '9px 14px',
                fontSize: '12px',
                fontWeight: '600',
                background: leftTab === 'tokens' ? '#ffffff' : 'transparent',
                color: leftTab === 'tokens' ? 'var(--text-primary)' : 'var(--text-muted)',
                border: '1px solid var(--border-color)',
                borderBottom: leftTab === 'tokens' ? '1px solid #ffffff' : '1px solid var(--border-color)',
                marginBottom: '-1px',
                borderTopLeftRadius: '4px',
                borderTopRightRadius: '4px',
                cursor: 'pointer',
                marginLeft: '4px'
              }}
            >
              <Layers size={13} />
              <span>Token Stream</span>
              {result?.tokens && (
                <span
                  style={{
                    fontSize: '10px',
                    padding: '1px 5px',
                    borderRadius: '8px',
                    background: '#e2e8f0',
                    color: 'var(--text-secondary)'
                  }}
                >
                  {result.tokens.length}
                </span>
              )}
            </button>
          </div>

          {/* Editor / Tokens View Area */}
          <div style={{ flex: 1, overflow: 'hidden', padding: '10px 12px 6px 12px' }}>
            {leftTab === 'editor' ? (
              <SqlEditor
                value={sql}
                onChange={setSql}
                error={syntaxError}
                onCompile={() => handleCompile(sql, schema)}
              />
            ) : (
              <TokenTable tokens={result?.tokens} />
            )}
          </div>

          {/* Bottom Wide Green Compile Button */}
          <div style={{ padding: '8px 12px', borderTop: '1px solid var(--border-color)', background: '#ffffff' }}>
            <button
              onClick={() => handleCompile()}
              disabled={loading}
              style={{
                width: '100%',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
                gap: '8px',
                background: 'var(--primary)',
                color: '#ffffff',
                border: 'none',
                borderRadius: 'var(--radius-sm)',
                padding: '9px',
                fontSize: '13px',
                fontWeight: '600',
                cursor: loading ? 'not-allowed' : 'pointer',
                transition: 'background 0.15s ease'
              }}
              onMouseEnter={e => !loading && (e.currentTarget.style.background = 'var(--primary-hover)')}
              onMouseLeave={e => !loading && (e.currentTarget.style.background = 'var(--primary)')}
            >
              {loading ? <RefreshCw size={14} className="spin" /> : <Play size={14} fill="#ffffff" />}
              <span>Compile</span>
              <span style={{ fontSize: '11px', opacity: 0.7, marginLeft: '6px' }}>Ctrl + Enter</span>
            </button>
          </div>
        </div>

        {/* VERTICAL RESIZER BAR (Left/Right Splitter) */}
        <div
          onMouseDown={handleStartDragV}
          title="Drag to resize SQL Editor and Relational Algebra output"
          style={{
            width: '7px',
            cursor: 'col-resize',
            background: isDraggingV ? 'var(--primary-light)' : '#f8fafc',
            borderLeft: '1px solid var(--border-color)',
            borderRight: '1px solid var(--border-color)',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            transition: 'background 0.15s ease',
            flexShrink: 0,
            zIndex: 10
          }}
          onMouseEnter={e => !isDraggingV && (e.currentTarget.style.background = '#e2e8f0')}
          onMouseLeave={e => !isDraggingV && (e.currentTarget.style.background = '#f8fafc')}
        >
          <div
            style={{
              width: '2px',
              height: '24px',
              borderRadius: '2px',
              background: isDraggingV ? 'var(--primary)' : '#94a3b8'
            }}
          />
        </div>

        {/* RIGHT PANEL (Output: Relational Algebra, Trees & Legend) */}
        <div style={{ flex: 1, minWidth: '300px', display: 'flex', flexDirection: 'column', height: '100%', background: '#ffffff', overflow: 'hidden', position: 'relative' }}>
          {/* Top Sub-Bar: Optimized vs Canonical Switcher & Status */}
          <div
            style={{
              padding: '8px 14px',
              borderBottom: '1px solid var(--border-color)',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'space-between',
              background: '#f8fafc'
            }}
          >
            {/* Optimized vs Canonical Tabs */}
            <div style={{ display: 'flex', gap: '4px' }}>
              <button
                onClick={() => setViewMode('optimized')}
                style={{
                  display: 'flex',
                  alignItems: 'center',
                  gap: '6px',
                  padding: '5px 12px',
                  borderRadius: 'var(--radius-sm)',
                  fontSize: '12px',
                  fontWeight: '600',
                  border: '1px solid var(--border-color)',
                  cursor: 'pointer',
                  background: viewMode === 'optimized' ? '#ecfdf5' : '#ffffff',
                  color: viewMode === 'optimized' ? '#065f46' : 'var(--text-secondary)',
                  borderColor: viewMode === 'optimized' ? '#a7f3d0' : 'var(--border-color)'
                }}
              >
                <Sparkles size={13} />
                <span>Optimized RA Tree</span>
              </button>

              <button
                onClick={() => setViewMode('unoptimized')}
                style={{
                  display: 'flex',
                  alignItems: 'center',
                  gap: '6px',
                  padding: '5px 12px',
                  borderRadius: 'var(--radius-sm)',
                  fontSize: '12px',
                  fontWeight: '600',
                  border: '1px solid var(--border-color)',
                  cursor: 'pointer',
                  background: viewMode === 'unoptimized' ? '#f1f5f9' : '#ffffff',
                  color: viewMode === 'unoptimized' ? 'var(--text-primary)' : 'var(--text-secondary)'
                }}
              >
                <GitBranch size={13} />
                <span>Canonical (Unoptimized)</span>
              </button>
            </div>

            {/* Optimization Badge */}
            {result?.ra_string_optimized && result?.ra_string_unoptimized && (
              <div style={{ fontSize: '12px' }}>
                {result.ra_string_optimized !== result.ra_string_unoptimized ? (
                  <span style={{ color: '#059669', fontWeight: '600', display: 'flex', alignItems: 'center', gap: '4px' }}>
                    <Check size={14} /> Optimized (Pushdown / Join converted)
                  </span>
                ) : (
                  <span style={{ color: 'var(--text-muted)' }}>Canonical tree is already minimal</span>
                )}
              </div>
            )}
          </div>

          {/* Relational Algebra Formula Bar */}
          <div
            style={{
              padding: '10px 14px',
              borderBottom: '1px solid var(--border-color)',
              background: '#ffffff',
              display: 'flex',
              alignItems: 'flex-start',
              justifyContent: 'space-between',
              gap: '12px'
            }}
          >
            <div style={{ display: 'flex', alignItems: 'flex-start', gap: '8px', flex: 1, minWidth: 0 }}>
              <span style={{ fontWeight: '700', fontSize: '13px', color: 'var(--text-primary)', whiteSpace: 'nowrap', marginTop: '1px' }}>
                Relational Algebra:
              </span>
              <span
                style={{
                  fontFamily: 'var(--font-mono)',
                  fontSize: '13px',
                  color: '#1e293b',
                  lineHeight: '1.5',
                  wordBreak: 'break-word',
                  whiteSpace: 'normal',
                  flex: 1
                }}
              >
                {currentLinearStr || '—'}
              </span>
            </div>

            <button
              onClick={handleCopyLinearFormula}
              title="Copy linear formula"
              style={{
                display: 'flex',
                alignItems: 'center',
                gap: '4px',
                background: '#ffffff',
                border: '1px solid var(--border-color)',
                borderRadius: 'var(--radius-sm)',
                padding: '4px 10px',
                fontSize: '11px',
                fontWeight: '500',
                color: copied ? '#059669' : 'var(--text-secondary)',
                cursor: 'pointer',
                flexShrink: 0,
                marginTop: '1px'
              }}
            >
              {copied ? <Check size={12} /> : <Copy size={12} />}
              <span>{copied ? 'Copied' : 'Copy'}</span>
            </button>
          </div>

          {/* Operator Legend Ribbon + View Toggle */}
          <div
            style={{
              padding: '6px 14px',
              borderBottom: '1px solid var(--border-color)',
              background: '#fafbfc',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'space-between',
              fontSize: '11px'
            }}
          >
            {/* Operators Legend */}
            <div style={{ display: 'flex', alignItems: 'center', gap: '10px', flexWrap: 'wrap' }}>
              {OPERATOR_LEGEND.map(op => (
                <div key={op.name} style={{ display: 'flex', alignItems: 'center', gap: '4px' }}>
                  <span style={{ fontWeight: '700', color: op.color, fontFamily: 'var(--font-mono)' }}>{op.symbol}</span>
                  <span style={{ color: 'var(--text-secondary)' }}>{op.name}</span>
                </div>
              ))}
            </div>

            {/* View Toggle: Tree vs Plan */}
            <div style={{ display: 'flex', alignItems: 'center', gap: '6px' }}>
              <span style={{ color: 'var(--text-muted)' }}>View:</span>
              <div style={{ display: 'flex', border: '1px solid var(--border-color)', borderRadius: 'var(--radius-sm)', overflow: 'hidden' }}>
                <button
                  onClick={() => setDisplayType('tree')}
                  style={{
                    padding: '2px 8px',
                    fontSize: '11px',
                    border: 'none',
                    cursor: 'pointer',
                    background: displayType === 'tree' ? '#e2e8f0' : '#ffffff',
                    color: displayType === 'tree' ? '#0f172a' : 'var(--text-muted)',
                    fontWeight: displayType === 'tree' ? '600' : '400'
                  }}
                >
                  Tree
                </button>
                <button
                  onClick={() => setDisplayType('plan')}
                  style={{
                    padding: '2px 8px',
                    fontSize: '11px',
                    border: 'none',
                    cursor: 'pointer',
                    background: displayType === 'plan' ? '#e2e8f0' : '#ffffff',
                    color: displayType === 'plan' ? '#0f172a' : 'var(--text-muted)',
                    fontWeight: displayType === 'plan' ? '600' : '400'
                  }}
                >
                  Plan
                </button>
              </div>
            </div>
          </div>

          {/* Center Diagram Canvas or Plan */}
          <div style={{ flex: 1, position: 'relative', overflow: 'hidden' }}>
            {displayType === 'tree' ? (
              currentRaTree ? (
                <RaTree
                  data={currentRaTree}
                  onSelectNode={(node) => setSelectedNode(node)}
                  selectedNodeId={selectedNode?.id}
                />
              ) : (
                <div style={{ height: '100%', display: 'flex', alignItems: 'center', justifyContent: 'center', color: 'var(--text-muted)', fontSize: '13px' }}>
                  Enter a query and click Compile to render the tree.
                </div>
              )
            ) : (
              <div style={{ padding: '16px', overflowY: 'auto', height: '100%', background: '#ffffff' }}>
                <div style={{ fontWeight: '600', fontSize: '13px', marginBottom: '8px', color: 'var(--text-primary)' }}>
                  Indented Relational Algebra Execution Plan:
                </div>
                {currentRaTree ? renderPlanList(currentRaTree) : <div style={{ color: 'var(--text-muted)' }}>No execution plan.</div>}
              </div>
            )}

            {/* Selected Node Details Drawer */}
            {selectedNode && (
              <NodeInspector
                node={selectedNode}
                onClose={() => setSelectedNode(null)}
              />
            )}
          </div>
        </div>
      </div>

      {/* HORIZONTAL RESIZER BAR (Top/Bottom Splitter) */}
      <div
        onMouseDown={handleStartDragH}
        title="Drag up or down to adjust Catalog Schema height"
        style={{
          height: '7px',
          cursor: 'row-resize',
          background: isDraggingH ? 'var(--primary-light)' : '#f8fafc',
          borderTop: '1px solid var(--border-color)',
          borderBottom: '1px solid var(--border-color)',
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'center',
          transition: 'background 0.15s ease',
          zIndex: 15,
          flexShrink: 0
        }}
        onMouseEnter={e => !isDraggingH && (e.currentTarget.style.background = '#e2e8f0')}
        onMouseLeave={e => !isDraggingH && (e.currentTarget.style.background = '#f8fafc')}
      >
        <div
          style={{
            width: '32px',
            height: '2px',
            borderRadius: '2px',
            background: isDraggingH ? 'var(--primary)' : '#94a3b8'
          }}
        />
      </div>

      {/* 3. BOTTOM SECTION: Catalog Schema & Data Insertion (3-Column Layout) */}
      <div
        style={{
          flex: 1,
          height: `calc(${100 - topHeightPercent}% - 7px)`,
          display: 'flex',
          flexDirection: 'column',
          background: '#ffffff',
          overflow: 'hidden'
        }}
      >
        {/* Navigation Tab Header */}
        <div style={{ display: 'flex', borderBottom: '1px solid var(--border-color)', background: '#f8fafc', padding: '0 12px', flexShrink: 0 }}>
          <div
            style={{
              display: 'flex',
              alignItems: 'center',
              gap: '6px',
              padding: '8px 16px',
              fontSize: '13px',
              fontWeight: '600',
              background: '#ffffff',
              color: '#065f46',
              border: '1px solid var(--border-color)',
              borderBottom: '1px solid #ffffff',
              marginBottom: '-1px',
              borderTopLeftRadius: '4px',
              borderTopRightRadius: '4px'
            }}
          >
            <Database size={14} color="#059669" />
            <span>Catalog Schema</span>
            {schema?.tables && (
              <span
                style={{
                  fontSize: '11px',
                  padding: '1px 6px',
                  borderRadius: '10px',
                  background: '#ecfdf5',
                  color: '#065f46',
                  border: '1px solid #a7f3d0'
                }}
              >
                {schema.tables.length} tables
              </span>
            )}
          </div>
        </div>

        {/* 3-Column Manager Container */}
        <div style={{ flex: 1, overflow: 'hidden' }}>
          <SchemaManager
            schema={schema}
            onUpdateSchema={handleUpdateSchema}
            onLoadMockData={handleLoadMockData}
            onClearAllTables={handleClearAllTables}
            onInsertSql={(text) => {
              setSql(text);
              handleCompile(text, schema);
            }}
          />
        </div>
      </div>
    </div>
  );
}
