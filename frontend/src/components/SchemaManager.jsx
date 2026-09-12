import React, { useState } from 'react';
import {
  Plus,
  Trash2,
  Edit2,
  Check,
  X,
  Code,
  RotateCcw,
  Sparkles,
  Copy,
  Table as TableIcon
} from 'lucide-react';

const DATA_TYPES = ['INT', 'VARCHAR(100)', 'VARCHAR(255)', 'FLOAT', 'BOOLEAN', 'DATE', 'TEXT'];

export default function SchemaManager({
  schema,
  onUpdateSchema,
  onLoadMockData,
  onClearAllTables,
  onInsertSql
}) {
  const tables = schema?.tables || [];
  const [selectedTableName, setSelectedTableName] = useState(tables[0]?.name || '');

  // Table Renaming State
  const [editingTableName, setEditingTableName] = useState(false);
  const [tableRenameValue, setTableRenameValue] = useState('');

  // Column Editing State (columnName -> { name, type, constraint, fkTarget })
  const [editingColIdx, setEditingColIdx] = useState(null);
  const [editColData, setEditColData] = useState({ name: '', type: 'VARCHAR(100)', constraint: '-', fkTarget: '' });

  // New Table Modal State
  const [showNewTableModal, setShowNewTableModal] = useState(false);
  const [newTableName, setNewTableName] = useState('');
  const [newTableFirstCol, setNewTableFirstCol] = useState('id');
  const [newTableFirstColType, setNewTableFirstColType] = useState('INT');

  // Add Column Form State
  const [showAddCol, setShowAddCol] = useState(false);
  const [newColName, setNewColName] = useState('');
  const [newColType, setNewColType] = useState('VARCHAR(100)');
  const [newColConstraint, setNewColConstraint] = useState('-');
  const [newColFkTarget, setNewColFkTarget] = useState('');

  // Add Row Form State
  const [showAddRow, setShowAddRow] = useState(false);
  const [newRowValues, setNewRowValues] = useState({});

  // Row Cell Editing (rowIdx, colName) -> value
  const [editingRowIdx, setEditingRowIdx] = useState(null);
  const [editRowData, setEditRowData] = useState({});

  // JSON Raw Editor Modal
  const [showJsonModal, setShowJsonModal] = useState(false);
  const [jsonText, setJsonText] = useState('');
  const [jsonError, setJsonError] = useState(null);
  const [copied, setCopied] = useState(false);

  // Active table
  const activeTable = tables.find(t => t.name === selectedTableName) || tables[0] || null;

  // 1. Create New Table
  const handleCreateTable = (e) => {
    e?.preventDefault();
    const cleanName = newTableName.trim().toLowerCase().replace(/[^a-z0-9_]/g, '');
    if (!cleanName) return;

    if (tables.some(t => t.name.toLowerCase() === cleanName)) {
      alert(`Table '${cleanName}' already exists.`);
      return;
    }

    const newTable = {
      name: cleanName,
      columns: [
        {
          name: newTableFirstCol.trim().toLowerCase() || 'id',
          type: newTableFirstColType,
          is_pk: true,
          is_fk: false
        }
      ],
      rows: []
    };

    const updated = { ...schema, tables: [...tables, newTable] };
    onUpdateSchema(updated);
    setSelectedTableName(cleanName);
    setNewTableName('');
    setShowNewTableModal(false);
  };

  // 2. Rename Table
  const handleStartRenameTable = () => {
    if (!activeTable) return;
    setTableRenameValue(activeTable.name);
    setEditingTableName(true);
  };

  const handleSaveRenameTable = () => {
    const cleanName = tableRenameValue.trim().toLowerCase().replace(/[^a-z0-9_]/g, '');
    if (!cleanName || cleanName === activeTable.name) {
      setEditingTableName(false);
      return;
    }

    if (tables.some(t => t.name.toLowerCase() === cleanName && t.name !== activeTable.name)) {
      alert(`Table '${cleanName}' already exists.`);
      return;
    }

    const oldName = activeTable.name;
    const updatedTables = tables.map(t => {
      if (t.name === oldName) {
        return { ...t, name: cleanName };
      }
      return t;
    });

    onUpdateSchema({ ...schema, tables: updatedTables });
    setSelectedTableName(cleanName);
    setEditingTableName(false);
  };

  // 3. Delete Single Table (Remove one by one)
  const handleDeleteTable = (tableName, e) => {
    e?.stopPropagation();
    if (!confirm(`Delete table '${tableName}'?`)) return;

    const newTables = tables.filter(t => t.name !== tableName);
    onUpdateSchema({ ...schema, tables: newTables });
    if (selectedTableName === tableName) {
      setSelectedTableName(newTables[0]?.name || '');
    }
  };

  // 4. Delete All Tables At Once
  const handleClearAll = () => {
    if (!confirm('Are you sure you want to remove ALL tables from the catalog?')) return;
    if (onClearAllTables) {
      onClearAllTables();
    } else {
      onUpdateSchema({ ...schema, tables: [] });
    }
    setSelectedTableName('');
  };

  // 5. Add Column
  const handleAddColumn = () => {
    const cleanName = newColName.trim().toLowerCase().replace(/[^a-z0-9_]/g, '');
    if (!cleanName || !activeTable) return;

    if (activeTable.columns.some(c => c.name.toLowerCase() === cleanName)) {
      alert(`Column '${cleanName}' already exists in '${activeTable.name}'.`);
      return;
    }

    const isPk = newColConstraint === 'PRIMARY KEY';
    const isFk = newColConstraint === 'FOREIGN KEY';

    const newCol = {
      name: cleanName,
      type: newColType,
      is_pk: isPk,
      is_fk: isFk,
      ...(isFk && newColFkTarget ? { fk_target: newColFkTarget } : {})
    };

    const updatedTables = tables.map(t => {
      if (t.name === activeTable.name) {
        return { ...t, columns: [...t.columns, newCol] };
      }
      return t;
    });

    onUpdateSchema({ ...schema, tables: updatedTables });
    setNewColName('');
    setNewColConstraint('-');
    setNewColFkTarget('');
    setShowAddCol(false);
  };

  // 6. Edit Column Details (Name, Type, Constraint)
  const handleStartEditCol = (idx, col) => {
    setEditingColIdx(idx);
    let constraint = '-';
    if (col.is_pk) constraint = 'PRIMARY KEY';
    else if (col.is_fk) constraint = 'FOREIGN KEY';
    setEditColData({
      name: col.name,
      type: col.type || 'VARCHAR(100)',
      constraint: constraint,
      fkTarget: col.fk_target || ''
    });
  };

  const handleSaveEditCol = (idx) => {
    if (!activeTable) return;
    const cleanName = editColData.name.trim().toLowerCase().replace(/[^a-z0-9_]/g, '');
    if (!cleanName) return;

    const isPk = editColData.constraint === 'PRIMARY KEY';
    const isFk = editColData.constraint === 'FOREIGN KEY';
    const oldColName = activeTable.columns[idx].name;

    const updatedCols = activeTable.columns.map((c, i) => {
      if (i === idx) {
        return {
          name: cleanName,
          type: editColData.type,
          is_pk: isPk,
          is_fk: isFk,
          ...(isFk && editColData.fkTarget ? { fk_target: editColData.fkTarget } : {})
        };
      }
      return c;
    });

    // If column was renamed, update row keys as well
    let updatedRows = activeTable.rows || [];
    if (cleanName !== oldColName && updatedRows.length > 0) {
      updatedRows = updatedRows.map(r => {
        const copy = { ...r };
        if (copy[oldColName] !== undefined) {
          copy[cleanName] = copy[oldColName];
          delete copy[oldColName];
        }
        return copy;
      });
    }

    const updatedTables = tables.map(t => {
      if (t.name === activeTable.name) {
        return { ...t, columns: updatedCols, rows: updatedRows };
      }
      return t;
    });

    onUpdateSchema({ ...schema, tables: updatedTables });
    setEditingColIdx(null);
  };

  // 7. Delete Column
  const handleDeleteColumn = (colName) => {
    if (!activeTable) return;
    const updatedTables = tables.map(t => {
      if (t.name === activeTable.name) {
        return { ...t, columns: t.columns.filter(c => c.name !== colName) };
      }
      return t;
    });
    onUpdateSchema({ ...schema, tables: updatedTables });
  };

  // 8. Add Row
  const handleAddRow = () => {
    if (!activeTable) return;

    const rowObj = {};
    activeTable.columns.forEach(col => {
      const rawVal = newRowValues[col.name];
      if (col.type.startsWith('INT')) {
        rowObj[col.name] = rawVal ? parseInt(rawVal, 10) : 0;
      } else if (col.type.startsWith('FLOAT')) {
        rowObj[col.name] = rawVal ? parseFloat(rawVal) : 0.0;
      } else {
        rowObj[col.name] = rawVal !== undefined ? String(rawVal) : '';
      }
    });

    const currentRows = activeTable.rows || [];
    const updatedTables = tables.map(t => {
      if (t.name === activeTable.name) {
        return { ...t, rows: [...currentRows, rowObj] };
      }
      return t;
    });

    onUpdateSchema({ ...schema, tables: updatedTables });
    setNewRowValues({});
    setShowAddRow(false);
  };

  // 9. Edit Row Values
  const handleStartEditRow = (rowIdx, row) => {
    setEditingRowIdx(rowIdx);
    setEditRowData({ ...row });
  };

  const handleSaveEditRow = (rowIdx) => {
    if (!activeTable) return;
    const updatedRows = (activeTable.rows || []).map((r, i) => {
      if (i === rowIdx) {
        return { ...editRowData };
      }
      return r;
    });

    const updatedTables = tables.map(t => {
      if (t.name === activeTable.name) {
        return { ...t, rows: updatedRows };
      }
      return t;
    });

    onUpdateSchema({ ...schema, tables: updatedTables });
    setEditingRowIdx(null);
  };

  // 10. Delete Row
  const handleDeleteRow = (rowIndex) => {
    if (!activeTable) return;
    const updatedTables = tables.map(t => {
      if (t.name === activeTable.name) {
        const rows = [...(t.rows || [])];
        rows.splice(rowIndex, 1);
        return { ...t, rows };
      }
      return t;
    });
    onUpdateSchema({ ...schema, tables: updatedTables });
  };

  // 11. Clear All Rows in Table
  const handleClearTableRows = () => {
    if (!activeTable) return;
    if (!confirm(`Clear all sample data rows from table '${activeTable.name}'?`)) return;
    const updatedTables = tables.map(t => {
      if (t.name === activeTable.name) {
        return { ...t, rows: [] };
      }
      return t;
    });
    onUpdateSchema({ ...schema, tables: updatedTables });
  };

  // 12. JSON Editor
  const handleOpenJson = () => {
    setJsonText(JSON.stringify({ tables }, null, 2));
    setJsonError(null);
    setShowJsonModal(true);
  };

  const handleApplyJson = () => {
    try {
      const parsed = JSON.parse(jsonText);
      const newTables = parsed.tables || (Array.isArray(parsed) ? parsed : null);
      if (!Array.isArray(newTables)) {
        setJsonError('JSON must contain a "tables" array.');
        return;
      }
      onUpdateSchema({ ...schema, tables: newTables });
      setShowJsonModal(false);
      if (newTables.length > 0) setSelectedTableName(newTables[0].name);
    } catch (e) {
      setJsonError(e.message);
    }
  };

  return (
    <div style={{ height: '100%', display: 'flex', flexDirection: 'column', background: '#ffffff', overflow: 'hidden' }}>
      {/* 3-Column Split Layout */}
      <div style={{ flex: 1, display: 'grid', gridTemplateColumns: '240px 1.4fr 1.6fr', height: '100%', overflow: 'hidden' }}>
        
        {/* ======================================================== */}
        {/* COLUMN 1: Tables List */}
        {/* ======================================================== */}
        <div
          style={{
            borderRight: '1px solid var(--border-color)',
            display: 'flex',
            flexDirection: 'column',
            background: '#ffffff',
            overflowY: 'auto'
          }}
        >
          {/* Header */}
          <div
            style={{
              padding: '10px 12px',
              borderBottom: '1px solid var(--border-color)',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'space-between',
              background: '#f8fafc'
            }}
          >
            <span style={{ fontWeight: '600', fontSize: '13px', color: 'var(--text-primary)' }}>
              Tables ({tables.length})
            </span>

            <div style={{ display: 'flex', alignItems: 'center', gap: '4px' }}>
              <button
                onClick={() => setShowNewTableModal(true)}
                title="Create a new custom table"
                style={{
                  display: 'flex',
                  alignItems: 'center',
                  gap: '3px',
                  padding: '3px 7px',
                  borderRadius: 'var(--radius-sm)',
                  fontSize: '11px',
                  fontWeight: '600',
                  background: 'var(--primary)',
                  color: '#ffffff',
                  border: 'none',
                  cursor: 'pointer'
                }}
              >
                <Plus size={12} />
                <span>New Table</span>
              </button>

              {tables.length > 0 && (
                <button
                  onClick={handleClearAll}
                  title="Remove all tables at once"
                  style={{
                    display: 'flex',
                    alignItems: 'center',
                    gap: '2px',
                    padding: '3px 6px',
                    borderRadius: 'var(--radius-sm)',
                    fontSize: '11px',
                    background: '#ffffff',
                    border: '1px solid #fca5a5',
                    color: '#dc2626',
                    cursor: 'pointer'
                  }}
                  onMouseEnter={e => e.currentTarget.style.background = '#fef2f2'}
                  onMouseLeave={e => e.currentTarget.style.background = '#ffffff'}
                >
                  <Trash2 size={11} />
                  <span>Clear All</span>
                </button>
              )}
            </div>
          </div>

          {/* List of Tables or Empty State */}
          <div style={{ flex: 1, padding: '8px', display: 'flex', flexDirection: 'column', gap: '3px', overflowY: 'auto' }}>
            {tables.length === 0 ? (
              <div style={{ padding: '20px 10px', textAlign: 'center', display: 'flex', flexDirection: 'column', alignItems: 'center', gap: '8px' }}>
                <TableIcon size={24} color="#94a3b8" />
                <span style={{ fontSize: '12px', color: 'var(--text-muted)' }}>
                  No tables in catalog.
                </span>
                <button
                  onClick={onLoadMockData}
                  style={{
                    display: 'flex',
                    alignItems: 'center',
                    gap: '5px',
                    padding: '6px 12px',
                    borderRadius: 'var(--radius-sm)',
                    fontSize: '11px',
                    fontWeight: '600',
                    background: '#ecfdf5',
                    color: '#065f46',
                    border: '1px solid #a7f3d0',
                    cursor: 'pointer',
                    marginTop: '4px'
                  }}
                  onMouseEnter={e => e.currentTarget.style.background = '#d1fae5'}
                  onMouseLeave={e => e.currentTarget.style.background = '#ecfdf5'}
                >
                  <Sparkles size={12} />
                  <span>Load Mock Sample Data</span>
                </button>
              </div>
            ) : (
              tables.map(tbl => {
                const isSelected = activeTable && activeTable.name === tbl.name;
                return (
                  <div
                    key={tbl.name}
                    onClick={() => setSelectedTableName(tbl.name)}
                    style={{
                      padding: '7px 10px',
                      borderRadius: 'var(--radius-sm)',
                      fontSize: '13px',
                      cursor: 'pointer',
                      display: 'flex',
                      alignItems: 'center',
                      justifyContent: 'space-between',
                      fontFamily: 'var(--font-mono)',
                      background: isSelected ? 'var(--primary-light)' : 'transparent',
                      color: isSelected ? 'var(--primary)' : 'var(--text-primary)',
                      fontWeight: isSelected ? '600' : '400',
                      border: isSelected ? '1px solid var(--primary-border)' : '1px solid transparent',
                      transition: 'all 0.12s ease'
                    }}
                    onMouseEnter={e => !isSelected && (e.currentTarget.style.background = '#f8fafc')}
                    onMouseLeave={e => !isSelected && (e.currentTarget.style.background = 'transparent')}
                  >
                    <span style={{ overflow: 'hidden', textOverflow: 'ellipsis', whiteSpace: 'nowrap' }}>
                      {tbl.name}
                    </span>

                    <div style={{ display: 'flex', alignItems: 'center', gap: '6px' }}>
                      <span style={{ fontSize: '11px', color: isSelected ? 'var(--primary)' : 'var(--text-muted)' }}>
                        {tbl.columns?.length || 0}c
                      </span>
                      <button
                        onClick={(e) => handleDeleteTable(tbl.name, e)}
                        title={`Delete table ${tbl.name}`}
                        style={{
                          background: 'transparent',
                          border: 'none',
                          color: 'var(--text-muted)',
                          cursor: 'pointer',
                          padding: '2px',
                          display: 'flex',
                          alignItems: 'center'
                        }}
                        onMouseEnter={e => e.currentTarget.style.color = '#dc2626'}
                        onMouseLeave={e => e.currentTarget.style.color = 'var(--text-muted)'}
                      >
                        <Trash2 size={12} />
                      </button>
                    </div>
                  </div>
                );
              })
            )}
          </div>

          {/* Bottom Actions: Load Mock Data, JSON, Reset */}
          <div style={{ padding: '8px 10px', borderTop: '1px solid var(--border-color)', display: 'flex', flexDirection: 'column', gap: '4px', background: '#fafbfc' }}>
            <button
              onClick={onLoadMockData}
              title="Load demo sample tables (students, departments, courses, enrollments)"
              style={{
                width: '100%',
                padding: '4px 6px',
                borderRadius: '4px',
                fontSize: '11px',
                background: '#ffffff',
                border: '1px solid #a7f3d0',
                color: '#065f46',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
                gap: '4px',
                cursor: 'pointer',
                fontWeight: '500'
              }}
            >
              <Sparkles size={12} />
              <span>Load Mock Data</span>
            </button>

            <div style={{ display: 'flex', gap: '4px' }}>
              <button
                onClick={handleOpenJson}
                title="View or edit raw JSON schema"
                style={{
                  flex: 1,
                  padding: '3px 6px',
                  borderRadius: '4px',
                  fontSize: '11px',
                  background: '#ffffff',
                  border: '1px solid var(--border-color)',
                  color: 'var(--text-secondary)',
                  display: 'flex',
                  alignItems: 'center',
                  justifyContent: 'center',
                  gap: '4px',
                  cursor: 'pointer'
                }}
              >
                <Code size={11} />
                <span>JSON</span>
              </button>
              <button
                onClick={handleClearAll}
                title="Clear all tables"
                style={{
                  flex: 1,
                  padding: '3px 6px',
                  borderRadius: '4px',
                  fontSize: '11px',
                  background: '#ffffff',
                  border: '1px solid var(--border-color)',
                  color: 'var(--text-secondary)',
                  display: 'flex',
                  alignItems: 'center',
                  justifyContent: 'center',
                  gap: '4px',
                  cursor: 'pointer'
                }}
              >
                <RotateCcw size={11} />
                <span>Reset</span>
              </button>
            </div>
          </div>
        </div>

        {/* ======================================================== */}
        {/* COLUMN 2: Table Definition (Editable Name & Columns) */}
        {/* ======================================================== */}
        <div
          style={{
            borderRight: '1px solid var(--border-color)',
            display: 'flex',
            flexDirection: 'column',
            background: '#ffffff',
            overflowY: 'auto'
          }}
        >
          {activeTable ? (
            <>
              {/* Header with Editable Table Name */}
              <div
                style={{
                  padding: '10px 14px',
                  borderBottom: '1px solid var(--border-color)',
                  display: 'flex',
                  alignItems: 'center',
                  justifyContent: 'space-between',
                  background: '#ffffff'
                }}
              >
                <div style={{ display: 'flex', alignItems: 'center', gap: '8px' }}>
                  {editingTableName ? (
                    <div style={{ display: 'flex', alignItems: 'center', gap: '4px' }}>
                      <span style={{ fontWeight: '700', fontSize: '13px' }}>Table:</span>
                      <input
                        type="text"
                        value={tableRenameValue}
                        onChange={e => setTableRenameValue(e.target.value)}
                        style={{
                          padding: '2px 6px',
                          borderRadius: '4px',
                          border: '1px solid var(--primary)',
                          fontFamily: 'var(--font-mono)',
                          fontSize: '13px',
                          outline: 'none'
                        }}
                        autoFocus
                        onKeyDown={e => e.key === 'Enter' && handleSaveRenameTable()}
                      />
                      <button
                        onClick={handleSaveRenameTable}
                        style={{ background: 'var(--primary)', color: '#fff', border: 'none', borderRadius: '3px', padding: '2px 5px', cursor: 'pointer' }}
                      >
                        <Check size={12} />
                      </button>
                      <button
                        onClick={() => setEditingTableName(false)}
                        style={{ background: '#f1f5f9', color: '#475569', border: 'none', borderRadius: '3px', padding: '2px 5px', cursor: 'pointer' }}
                      >
                        <X size={12} />
                      </button>
                    </div>
                  ) : (
                    <div style={{ display: 'flex', alignItems: 'center', gap: '6px' }}>
                      <span style={{ fontWeight: '700', fontSize: '14px', color: 'var(--text-primary)' }}>
                        Table: <span style={{ fontFamily: 'var(--font-mono)' }}>{activeTable.name}</span>
                      </span>
                      <button
                        onClick={handleStartRenameTable}
                        title="Rename Table"
                        style={{
                          background: 'transparent',
                          border: 'none',
                          color: 'var(--text-muted)',
                          cursor: 'pointer',
                          display: 'flex',
                          padding: '2px'
                        }}
                        onMouseEnter={e => e.currentTarget.style.color = 'var(--text-primary)'}
                        onMouseLeave={e => e.currentTarget.style.color = 'var(--text-muted)'}
                      >
                        <Edit2 size={12} />
                      </button>
                    </div>
                  )}

                  <button
                    onClick={() => onInsertSql && onInsertSql(`SELECT * FROM ${activeTable.name};`)}
                    title="Insert query into SQL Editor"
                    style={{
                      padding: '2px 6px',
                      borderRadius: '3px',
                      fontSize: '10px',
                      background: '#f1f5f9',
                      border: '1px solid var(--border-color)',
                      color: 'var(--text-secondary)',
                      cursor: 'pointer'
                    }}
                  >
                    Query
                  </button>
                </div>

                <button
                  onClick={() => handleDeleteTable(activeTable.name)}
                  style={{
                    display: 'flex',
                    alignItems: 'center',
                    gap: '4px',
                    padding: '3px 8px',
                    borderRadius: 'var(--radius-sm)',
                    fontSize: '11px',
                    background: '#ffffff',
                    border: '1px solid #fca5a5',
                    color: '#dc2626',
                    cursor: 'pointer'
                  }}
                  onMouseEnter={e => e.currentTarget.style.background = '#fef2f2'}
                  onMouseLeave={e => e.currentTarget.style.background = '#ffffff'}
                >
                  <Trash2 size={12} />
                  <span>Delete Table</span>
                </button>
              </div>

              {/* Columns Table with Full Editing Support */}
              <div style={{ flex: 1, overflowY: 'auto', padding: '12px' }}>
                <table style={{ width: '100%', borderCollapse: 'collapse', fontSize: '12px', textAlign: 'left' }}>
                  <thead>
                    <tr style={{ borderBottom: '1px solid var(--border-color)', color: 'var(--text-muted)', fontSize: '11px', fontWeight: '600' }}>
                      <th style={{ padding: '6px 8px' }}>Column Name</th>
                      <th style={{ padding: '6px 8px' }}>Data Type</th>
                      <th style={{ padding: '6px 8px' }}>Constraints</th>
                      <th style={{ width: '50px', textAlign: 'right', paddingRight: '8px' }}>Action</th>
                    </tr>
                  </thead>
                  <tbody>
                    {activeTable.columns.map((col, idx) => {
                      const isEditingThisCol = editingColIdx === idx;

                      if (isEditingThisCol) {
                        return (
                          <tr key={idx} style={{ background: '#f8fafc', borderBottom: '1px solid var(--border-color)' }}>
                            <td style={{ padding: '5px 8px' }}>
                              <input
                                type="text"
                                value={editColData.name}
                                onChange={e => setEditColData({ ...editColData, name: e.target.value })}
                                style={{ width: '100%', padding: '2px 4px', border: '1px solid var(--primary)', borderRadius: '3px', fontSize: '11px', fontFamily: 'var(--font-mono)' }}
                                autoFocus
                              />
                            </td>
                            <td style={{ padding: '5px 8px' }}>
                              <select
                                value={editColData.type}
                                onChange={e => setEditColData({ ...editColData, type: e.target.value })}
                                style={{ width: '100%', padding: '2px 4px', border: '1px solid var(--border-color)', borderRadius: '3px', fontSize: '11px' }}
                              >
                                {DATA_TYPES.map(t => <option key={t} value={t}>{t}</option>)}
                              </select>
                            </td>
                            <td style={{ padding: '5px 8px' }}>
                              <select
                                value={editColData.constraint}
                                onChange={e => setEditColData({ ...editColData, constraint: e.target.value })}
                                style={{ width: '100%', padding: '2px 4px', border: '1px solid var(--border-color)', borderRadius: '3px', fontSize: '11px' }}
                              >
                                <option value="-">No Constraint</option>
                                <option value="PRIMARY KEY">PRIMARY KEY</option>
                                <option value="FOREIGN KEY">FOREIGN KEY</option>
                              </select>
                              {editColData.constraint === 'FOREIGN KEY' && (
                                <input
                                  type="text"
                                  placeholder="target (e.g. dept.id)"
                                  value={editColData.fkTarget}
                                  onChange={e => setEditColData({ ...editColData, fkTarget: e.target.value })}
                                  style={{ width: '100%', marginTop: '3px', padding: '2px 4px', border: '1px solid var(--border-color)', borderRadius: '3px', fontSize: '10px' }}
                                />
                              )}
                            </td>
                            <td style={{ padding: '5px 8px', textAlign: 'right' }}>
                              <div style={{ display: 'flex', justifyContent: 'flex-end', gap: '3px' }}>
                                <button
                                  onClick={() => handleSaveEditCol(idx)}
                                  title="Save Changes"
                                  style={{ background: 'var(--primary)', color: '#fff', border: 'none', borderRadius: '3px', padding: '2px 4px', cursor: 'pointer' }}
                                >
                                  <Check size={11} />
                                </button>
                                <button
                                  onClick={() => setEditingColIdx(null)}
                                  title="Cancel"
                                  style={{ background: '#e2e8f0', color: '#475569', border: 'none', borderRadius: '3px', padding: '2px 4px', cursor: 'pointer' }}
                                >
                                  <X size={11} />
                                </button>
                              </div>
                            </td>
                          </tr>
                        );
                      }

                      return (
                        <tr
                          key={col.name}
                          style={{
                            borderBottom: '1px solid #f1f5f9',
                            fontFamily: 'var(--font-mono)'
                          }}
                        >
                          <td style={{ padding: '7px 8px', color: 'var(--text-primary)', fontWeight: '500' }}>
                            {col.name}
                          </td>
                          <td style={{ padding: '7px 8px', color: 'var(--text-secondary)' }}>
                            {col.type}
                          </td>
                          <td style={{ padding: '7px 8px' }}>
                            {col.is_pk ? (
                              <span style={{ fontSize: '10px', padding: '1px 5px', borderRadius: '3px', background: '#fef3c7', color: '#b45309', fontWeight: '600' }}>
                                PRIMARY KEY
                              </span>
                            ) : col.is_fk ? (
                              <span style={{ fontSize: '10px', padding: '1px 5px', borderRadius: '3px', background: '#e0f2fe', color: '#0369a1', fontWeight: '600' }}>
                                FK &rarr; {col.fk_target}
                              </span>
                            ) : (
                              <span style={{ color: 'var(--text-muted)' }}>-</span>
                            )}
                          </td>
                          <td style={{ padding: '7px 8px', textAlign: 'right' }}>
                            <div style={{ display: 'flex', justifyContent: 'flex-end', gap: '4px' }}>
                              <button
                                onClick={() => handleStartEditCol(idx, col)}
                                title="Edit this column"
                                style={{
                                  background: 'transparent',
                                  border: 'none',
                                  color: 'var(--text-muted)',
                                  cursor: 'pointer',
                                  padding: '2px'
                                }}
                                onMouseEnter={e => e.currentTarget.style.color = 'var(--text-primary)'}
                                onMouseLeave={e => e.currentTarget.style.color = 'var(--text-muted)'}
                              >
                                <Edit2 size={11} />
                              </button>
                              {activeTable.columns.length > 1 && (
                                <button
                                  onClick={() => handleDeleteColumn(col.name)}
                                  title="Delete Column"
                                  style={{
                                    background: 'transparent',
                                    border: 'none',
                                    color: 'var(--text-muted)',
                                    cursor: 'pointer',
                                    padding: '2px'
                                  }}
                                  onMouseEnter={e => e.currentTarget.style.color = '#dc2626'}
                                  onMouseLeave={e => e.currentTarget.style.color = 'var(--text-muted)'}
                                >
                                  <Trash2 size={11} />
                                </button>
                              )}
                            </div>
                          </td>
                        </tr>
                      );
                    })}
                  </tbody>
                </table>

                {/* Inline Add Column Form */}
                {showAddCol ? (
                  <div
                    style={{
                      marginTop: '10px',
                      padding: '10px',
                      borderRadius: 'var(--radius-sm)',
                      background: '#f8fafc',
                      border: '1px solid var(--border-color)',
                      display: 'flex',
                      flexDirection: 'column',
                      gap: '8px'
                    }}
                  >
                    <div style={{ display: 'grid', gridTemplateColumns: '1.2fr 1fr 1fr', gap: '6px' }}>
                      <input
                        type="text"
                        placeholder="Column Name"
                        value={newColName}
                        onChange={e => setNewColName(e.target.value)}
                        style={{
                          padding: '4px 8px',
                          borderRadius: '4px',
                          border: '1px solid var(--border-color)',
                          fontSize: '11px',
                          outline: 'none'
                        }}
                        autoFocus
                      />
                      <select
                        value={newColType}
                        onChange={e => setNewColType(e.target.value)}
                        style={{
                          padding: '4px 6px',
                          borderRadius: '4px',
                          border: '1px solid var(--border-color)',
                          fontSize: '11px',
                          outline: 'none',
                          background: '#fff'
                        }}
                      >
                        {DATA_TYPES.map(t => <option key={t} value={t}>{t}</option>)}
                      </select>
                      <select
                        value={newColConstraint}
                        onChange={e => setNewColConstraint(e.target.value)}
                        style={{
                          padding: '4px 6px',
                          borderRadius: '4px',
                          border: '1px solid var(--border-color)',
                          fontSize: '11px',
                          outline: 'none',
                          background: '#fff'
                        }}
                      >
                        <option value="-">No Constraint</option>
                        <option value="PRIMARY KEY">PRIMARY KEY</option>
                        <option value="FOREIGN KEY">FOREIGN KEY</option>
                      </select>
                    </div>

                    {newColConstraint === 'FOREIGN KEY' && (
                      <input
                        type="text"
                        placeholder="Target (e.g. departments.id)"
                        value={newColFkTarget}
                        onChange={e => setNewColFkTarget(e.target.value)}
                        style={{
                          padding: '4px 8px',
                          borderRadius: '4px',
                          border: '1px solid var(--border-color)',
                          fontSize: '11px',
                          outline: 'none'
                        }}
                      />
                    )}

                    <div style={{ display: 'flex', justifyContent: 'flex-end', gap: '6px' }}>
                      <button
                        onClick={() => setShowAddCol(false)}
                        style={{
                          padding: '3px 8px',
                          borderRadius: '4px',
                          fontSize: '11px',
                          background: 'transparent',
                          border: '1px solid var(--border-color)',
                          cursor: 'pointer'
                        }}
                      >
                        Cancel
                      </button>
                      <button
                        onClick={handleAddColumn}
                        style={{
                          padding: '3px 12px',
                          borderRadius: '4px',
                          fontSize: '11px',
                          fontWeight: '600',
                          background: 'var(--primary)',
                          color: '#fff',
                          border: 'none',
                          cursor: 'pointer'
                        }}
                      >
                        Save Column
                      </button>
                    </div>
                  </div>
                ) : (
                  <button
                    onClick={() => setShowAddCol(true)}
                    style={{
                      marginTop: '10px',
                      display: 'flex',
                      alignItems: 'center',
                      gap: '4px',
                      padding: '5px 10px',
                      borderRadius: 'var(--radius-sm)',
                      background: 'transparent',
                      border: '1px solid var(--border-color)',
                      color: 'var(--text-secondary)',
                      fontSize: '11px',
                      fontWeight: '500',
                      cursor: 'pointer'
                    }}
                    onMouseEnter={e => {
                      e.currentTarget.style.borderColor = 'var(--primary)';
                      e.currentTarget.style.color = 'var(--primary)';
                    }}
                    onMouseLeave={e => {
                      e.currentTarget.style.borderColor = 'var(--border-color)';
                      e.currentTarget.style.color = 'var(--text-secondary)';
                    }}
                  >
                    <Plus size={12} />
                    <span>Add Column</span>
                  </button>
                )}
              </div>
            </>
          ) : (
            <div style={{ padding: '24px', textAlign: 'center', color: 'var(--text-muted)' }}>
              No table selected.
            </div>
          )}
        </div>

        {/* ======================================================== */}
        {/* COLUMN 3: Sample Rows (Optional & Fully Editable) */}
        {/* ======================================================== */}
        <div
          style={{
            display: 'flex',
            flexDirection: 'column',
            background: '#ffffff',
            overflowY: 'auto'
          }}
        >
          {activeTable ? (
            <>
              {/* Header */}
              <div
                style={{
                  padding: '10px 14px',
                  borderBottom: '1px solid var(--border-color)',
                  display: 'flex',
                  alignItems: 'center',
                  justifyContent: 'space-between'
                }}
              >
                <div style={{ display: 'flex', alignItems: 'center', gap: '8px' }}>
                  <span style={{ fontWeight: '600', fontSize: '13px', color: 'var(--text-primary)' }}>
                    Sample Rows (Optional)
                  </span>
                  <span style={{ fontSize: '11px', color: 'var(--text-muted)' }}>
                    ({activeTable.rows?.length || 0} rows)
                  </span>
                </div>

                <div style={{ display: 'flex', alignItems: 'center', gap: '6px' }}>
                  <button
                    onClick={() => setShowAddRow(true)}
                    style={{
                      display: 'flex',
                      alignItems: 'center',
                      gap: '4px',
                      padding: '3px 8px',
                      borderRadius: 'var(--radius-sm)',
                      fontSize: '11px',
                      fontWeight: '500',
                      background: '#ffffff',
                      color: 'var(--text-primary)',
                      border: '1px solid var(--border-color)',
                      cursor: 'pointer'
                    }}
                    onMouseEnter={e => e.currentTarget.style.borderColor = 'var(--primary)'}
                    onMouseLeave={e => e.currentTarget.style.borderColor = 'var(--border-color)'}
                  >
                    <Plus size={12} />
                    <span>Add Row</span>
                  </button>

                  {activeTable.rows && activeTable.rows.length > 0 && (
                    <button
                      onClick={handleClearTableRows}
                      title="Clear rows in this table"
                      style={{
                        padding: '3px 6px',
                        borderRadius: 'var(--radius-sm)',
                        fontSize: '11px',
                        background: '#ffffff',
                        border: '1px solid #fca5a5',
                        color: '#dc2626',
                        cursor: 'pointer'
                      }}
                    >
                      Clear Rows
                    </button>
                  )}
                </div>
              </div>

              {/* Data Table */}
              <div style={{ flex: 1, overflowY: 'auto', padding: '12px' }}>
                {(activeTable.rows && activeTable.rows.length > 0) ? (
                  <table style={{ width: '100%', borderCollapse: 'collapse', fontSize: '12px', textAlign: 'left' }}>
                    <thead>
                      <tr style={{ borderBottom: '1px solid var(--border-color)', color: 'var(--text-muted)', fontSize: '11px', fontWeight: '600' }}>
                        {activeTable.columns.map(col => (
                          <th key={col.name} style={{ padding: '6px 8px', fontFamily: 'var(--font-mono)' }}>
                            {col.name}
                          </th>
                        ))}
                        <th style={{ width: '50px', textAlign: 'right', paddingRight: '8px' }}>Action</th>
                      </tr>
                    </thead>
                    <tbody>
                      {activeTable.rows.map((row, rIdx) => {
                        const isEditingThisRow = editingRowIdx === rIdx;

                        if (isEditingThisRow) {
                          return (
                            <tr key={rIdx} style={{ background: '#f8fafc', borderBottom: '1px solid var(--border-color)' }}>
                              {activeTable.columns.map(col => (
                                <td key={col.name} style={{ padding: '4px 6px' }}>
                                  <input
                                    type={col.type.startsWith('INT') || col.type.startsWith('FLOAT') ? 'number' : 'text'}
                                    value={editRowData[col.name] !== undefined ? editRowData[col.name] : ''}
                                    onChange={e => setEditRowData({ ...editRowData, [col.name]: e.target.value })}
                                    style={{
                                      width: '100%',
                                      padding: '2px 4px',
                                      borderRadius: '3px',
                                      border: '1px solid var(--primary)',
                                      fontSize: '11px',
                                      fontFamily: 'var(--font-mono)'
                                    }}
                                  />
                                </td>
                              ))}
                              <td style={{ padding: '4px 6px', textAlign: 'right' }}>
                                <div style={{ display: 'flex', justifyContent: 'flex-end', gap: '3px' }}>
                                  <button
                                    onClick={() => handleSaveEditRow(rIdx)}
                                    title="Save Row"
                                    style={{ background: 'var(--primary)', color: '#fff', border: 'none', borderRadius: '3px', padding: '2px 4px', cursor: 'pointer' }}
                                  >
                                    <Check size={11} />
                                  </button>
                                  <button
                                    onClick={() => setEditingRowIdx(null)}
                                    title="Cancel"
                                    style={{ background: '#e2e8f0', color: '#475569', border: 'none', borderRadius: '3px', padding: '2px 4px', cursor: 'pointer' }}
                                  >
                                    <X size={11} />
                                  </button>
                                </div>
                              </td>
                            </tr>
                          );
                        }

                        return (
                          <tr
                            key={rIdx}
                            style={{
                              borderBottom: '1px solid #f1f5f9',
                              fontFamily: 'var(--font-mono)'
                            }}
                          >
                            {activeTable.columns.map(col => (
                              <td
                                key={col.name}
                                onClick={() => handleStartEditRow(rIdx, row)}
                                title="Click to edit row value"
                                style={{ padding: '7px 8px', color: 'var(--text-primary)', cursor: 'pointer' }}
                              >
                                {row[col.name] !== undefined ? String(row[col.name]) : '—'}
                              </td>
                            ))}
                            <td style={{ padding: '7px 8px', textAlign: 'right' }}>
                              <div style={{ display: 'flex', justifyContent: 'flex-end', gap: '4px' }}>
                                <button
                                  onClick={() => handleStartEditRow(rIdx, row)}
                                  title="Edit row"
                                  style={{
                                    background: 'transparent',
                                    border: 'none',
                                    color: 'var(--text-muted)',
                                    cursor: 'pointer',
                                    padding: '2px'
                                  }}
                                  onMouseEnter={e => e.currentTarget.style.color = 'var(--text-primary)'}
                                  onMouseLeave={e => e.currentTarget.style.color = 'var(--text-muted)'}
                                >
                                  <Edit2 size={11} />
                                </button>
                                <button
                                  onClick={() => handleDeleteRow(rIdx)}
                                  title="Delete Row"
                                  style={{
                                    background: 'transparent',
                                    border: 'none',
                                    color: 'var(--text-muted)',
                                    cursor: 'pointer',
                                    padding: '2px'
                                  }}
                                  onMouseEnter={e => e.currentTarget.style.color = '#dc2626'}
                                  onMouseLeave={e => e.currentTarget.style.color = 'var(--text-muted)'}
                                >
                                  <Trash2 size={11} />
                                </button>
                              </div>
                            </td>
                          </tr>
                        );
                      })}
                    </tbody>
                  </table>
                ) : (
                  <div style={{ padding: '24px', textAlign: 'center', color: 'var(--text-muted)', fontSize: '12px' }}>
                    No sample records in '{activeTable.name}'. Click "+ Add Row" to insert data.
                  </div>
                )}

                {/* Inline Add Row Form */}
                {showAddRow && (
                  <div
                    style={{
                      marginTop: '10px',
                      padding: '10px',
                      borderRadius: 'var(--radius-sm)',
                      background: '#f8fafc',
                      border: '1px solid var(--border-color)',
                      display: 'flex',
                      flexDirection: 'column',
                      gap: '8px'
                    }}
                  >
                    <div style={{ fontSize: '11px', fontWeight: '600', color: 'var(--text-secondary)' }}>
                      Insert Record
                    </div>
                    <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(110px, 1fr))', gap: '6px' }}>
                      {activeTable.columns.map(col => (
                        <div key={col.name}>
                          <label style={{ fontSize: '10px', color: 'var(--text-muted)', display: 'block', marginBottom: '2px' }}>
                            {col.name}
                          </label>
                          <input
                            type={col.type.startsWith('INT') || col.type.startsWith('FLOAT') ? 'number' : 'text'}
                            placeholder={col.name}
                            value={newRowValues[col.name] || ''}
                            onChange={e => setNewRowValues({ ...newRowValues, [col.name]: e.target.value })}
                            style={{
                              width: '100%',
                              padding: '4px 6px',
                              borderRadius: '3px',
                              border: '1px solid var(--border-color)',
                              fontSize: '11px',
                              outline: 'none'
                            }}
                          />
                        </div>
                      ))}
                    </div>
                    <div style={{ display: 'flex', justifyContent: 'flex-end', gap: '6px', marginTop: '4px' }}>
                      <button
                        onClick={() => setShowAddRow(false)}
                        style={{
                          padding: '3px 8px',
                          borderRadius: '4px',
                          fontSize: '11px',
                          background: 'transparent',
                          border: '1px solid var(--border-color)',
                          cursor: 'pointer'
                        }}
                      >
                        Cancel
                      </button>
                      <button
                        onClick={handleAddRow}
                        style={{
                          padding: '3px 12px',
                          borderRadius: '4px',
                          fontSize: '11px',
                          fontWeight: '600',
                          background: 'var(--primary)',
                          color: '#fff',
                          border: 'none',
                          cursor: 'pointer'
                        }}
                      >
                        Save Record
                      </button>
                    </div>
                  </div>
                )}
              </div>
            </>
          ) : (
            <div style={{ padding: '24px', textAlign: 'center', color: 'var(--text-muted)' }}>
              Select a table to view rows.
            </div>
          )}
        </div>
      </div>

      {/* New Table Modal */}
      {showNewTableModal && (
        <div
          style={{
            position: 'fixed',
            inset: 0,
            background: 'rgba(15, 23, 42, 0.4)',
            backdropFilter: 'blur(3px)',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            zIndex: 100
          }}
          onClick={() => setShowNewTableModal(false)}
        >
          <div
            style={{
              background: '#ffffff',
              borderRadius: 'var(--radius-md)',
              border: '1px solid var(--border-color)',
              padding: '18px',
              width: '380px',
              boxShadow: '0 10px 25px rgba(0, 0, 0, 0.1)'
            }}
            onClick={e => e.stopPropagation()}
          >
            <div style={{ fontWeight: '700', fontSize: '15px', color: 'var(--text-primary)', marginBottom: '12px' }}>
              Create New Table
            </div>
            <div style={{ display: 'flex', flexDirection: 'column', gap: '10px' }}>
              <div>
                <label style={{ fontSize: '11px', color: 'var(--text-secondary)', display: 'block', marginBottom: '3px' }}>
                  Table Name:
                </label>
                <input
                  type="text"
                  placeholder="e.g. employees, orders"
                  value={newTableName}
                  onChange={e => setNewTableName(e.target.value)}
                  style={{
                    width: '100%',
                    padding: '6px 8px',
                    borderRadius: '4px',
                    border: '1px solid var(--border-color)',
                    fontSize: '12px',
                    outline: 'none'
                  }}
                  autoFocus
                />
              </div>

              <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr', gap: '8px' }}>
                <div>
                  <label style={{ fontSize: '11px', color: 'var(--text-secondary)', display: 'block', marginBottom: '3px' }}>
                    Primary Key Column:
                  </label>
                  <input
                    type="text"
                    value={newTableFirstCol}
                    onChange={e => setNewTableFirstCol(e.target.value)}
                    style={{
                      width: '100%',
                      padding: '6px 8px',
                      borderRadius: '4px',
                      border: '1px solid var(--border-color)',
                      fontSize: '12px',
                      outline: 'none'
                    }}
                  />
                </div>
                <div>
                  <label style={{ fontSize: '11px', color: 'var(--text-secondary)', display: 'block', marginBottom: '3px' }}>
                    Type:
                  </label>
                  <select
                    value={newTableFirstColType}
                    onChange={e => setNewTableFirstColType(e.target.value)}
                    style={{
                      width: '100%',
                      padding: '6px 8px',
                      borderRadius: '4px',
                      border: '1px solid var(--border-color)',
                      fontSize: '12px',
                      outline: 'none',
                      background: '#fff'
                    }}
                  >
                    {DATA_TYPES.map(t => <option key={t} value={t}>{t}</option>)}
                  </select>
                </div>
              </div>
            </div>

            <div style={{ display: 'flex', justifyContent: 'flex-end', gap: '8px', marginTop: '16px' }}>
              <button
                onClick={() => setShowNewTableModal(false)}
                style={{
                  padding: '6px 12px',
                  borderRadius: '4px',
                  fontSize: '12px',
                  background: 'transparent',
                  border: '1px solid var(--border-color)',
                  cursor: 'pointer'
                }}
              >
                Cancel
              </button>
              <button
                onClick={handleCreateTable}
                style={{
                  padding: '6px 14px',
                  borderRadius: '4px',
                  fontSize: '12px',
                  fontWeight: '600',
                  background: 'var(--primary)',
                  color: '#fff',
                  border: 'none',
                  cursor: 'pointer'
                }}
              >
                Create Table
              </button>
            </div>
          </div>
        </div>
      )}

      {/* Raw JSON Modal */}
      {showJsonModal && (
        <div
          style={{
            position: 'fixed',
            inset: 0,
            background: 'rgba(15, 23, 42, 0.4)',
            backdropFilter: 'blur(3px)',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center',
            zIndex: 100
          }}
          onClick={() => setShowJsonModal(false)}
        >
          <div
            style={{
              background: '#ffffff',
              borderRadius: 'var(--radius-md)',
              border: '1px solid var(--border-color)',
              padding: '18px',
              width: '560px',
              height: '420px',
              display: 'flex',
              flexDirection: 'column',
              boxShadow: '0 10px 25px rgba(0, 0, 0, 0.1)'
            }}
            onClick={e => e.stopPropagation()}
          >
            <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between', marginBottom: '10px' }}>
              <span style={{ fontWeight: '700', fontSize: '15px', color: 'var(--text-primary)' }}>
                Raw JSON Schema
              </span>
              <button
                onClick={() => {
                  navigator.clipboard.writeText(jsonText);
                  setCopied(true);
                  setTimeout(() => setCopied(false), 2000);
                }}
                style={{
                  display: 'flex',
                  alignItems: 'center',
                  gap: '4px',
                  background: 'transparent',
                  border: 'none',
                  color: copied ? 'var(--emerald)' : 'var(--text-secondary)',
                  cursor: 'pointer',
                  fontSize: '12px'
                }}
              >
                {copied ? <Check size={12} /> : <Copy size={12} />}
                <span>{copied ? 'Copied' : 'Copy JSON'}</span>
              </button>
            </div>

            <textarea
              value={jsonText}
              onChange={e => setJsonText(e.target.value)}
              style={{
                flex: 1,
                fontFamily: 'var(--font-mono)',
                fontSize: '12px',
                padding: '10px',
                borderRadius: '4px',
                border: '1px solid var(--border-color)',
                outline: 'none',
                resize: 'none',
                background: '#f8fafc'
              }}
            />

            {jsonError && (
              <div style={{ color: 'var(--rose)', fontSize: '11px', marginTop: '6px' }}>
                {jsonError}
              </div>
            )}

            <div style={{ display: 'flex', justifyContent: 'flex-end', gap: '8px', marginTop: '12px' }}>
              <button
                onClick={() => setShowJsonModal(false)}
                style={{
                  padding: '5px 12px',
                  borderRadius: '4px',
                  fontSize: '12px',
                  background: 'transparent',
                  border: '1px solid var(--border-color)',
                  cursor: 'pointer'
                }}
              >
                Cancel
              </button>
              <button
                onClick={handleApplyJson}
                style={{
                  padding: '5px 14px',
                  borderRadius: '4px',
                  fontSize: '12px',
                  fontWeight: '600',
                  background: 'var(--primary)',
                  color: '#fff',
                  border: 'none',
                  cursor: 'pointer'
                }}
              >
                Apply Schema
              </button>
            </div>
          </div>
        </div>
      )}
    </div>
  );
}
