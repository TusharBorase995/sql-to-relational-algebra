import React, { useState } from 'react';
import { Database, ChevronDown, ChevronRight, Key, ArrowUpRight } from 'lucide-react';

export default function SchemaBrowser({ schema, onInsertColumn }) {
  const [openTables, setOpenTables] = useState({ students: true, departments: true });

  const toggleTable = (name) => {
    setOpenTables(prev => ({ ...prev, [name]: !prev[name] }));
  };

  if (!schema || !schema.tables || schema.tables.length === 0) {
    return (
      <div style={{ padding: '20px', textAlign: 'center', color: 'var(--text-muted)' }}>
        Loading schema catalog...
      </div>
    );
  }

  return (
    <div style={{ height: '100%', overflowY: 'auto', padding: '12px' }}>
      <div style={{ display: 'flex', alignItems: 'center', gap: '8px', marginBottom: '12px', color: 'var(--text-secondary)', fontSize: '13px', fontWeight: '600' }}>
        <Database size={16} style={{ color: 'var(--primary)' }} />
        <span>Database Schema ({schema.tables.length} Tables)</span>
      </div>

      <div style={{ display: 'flex', flexDirection: 'column', gap: '8px' }}>
        {schema.tables.map(table => {
          const isOpen = !!openTables[table.name];
          return (
            <div
              key={table.name}
              style={{
                borderRadius: 'var(--radius-sm)',
                border: '1px solid var(--border-color)',
                background: 'rgba(30, 41, 59, 0.5)',
                overflow: 'hidden'
              }}
            >
              {/* Table Header */}
              <div
                onClick={() => toggleTable(table.name)}
                style={{
                  padding: '8px 12px',
                  display: 'flex',
                  alignItems: 'center',
                  justifyContent: 'space-between',
                  cursor: 'pointer',
                  userSelect: 'none',
                  background: isOpen ? 'rgba(51, 65, 85, 0.4)' : 'transparent',
                  transition: 'background 0.15s ease'
                }}
              >
                <div style={{ display: 'flex', alignItems: 'center', gap: '6px' }}>
                  {isOpen ? <ChevronDown size={14} color="#94a3b8" /> : <ChevronRight size={14} color="#94a3b8" />}
                  <span style={{ fontFamily: 'var(--font-mono)', fontWeight: '600', fontSize: '13px', color: '#f8fafc' }}>
                    {table.name}
                  </span>
                </div>
                <span style={{ fontSize: '11px', color: 'var(--text-muted)' }}>
                  {table.columns.length} cols
                </span>
              </div>

              {/* Columns Accordion */}
              {isOpen && (
                <div style={{ padding: '6px 12px 10px 24px', display: 'flex', flexDirection: 'column', gap: '4px' }}>
                  {table.columns.map(col => (
                    <div
                      key={col.name}
                      onClick={() => onInsertColumn && onInsertColumn(`${table.name}.${col.name}`)}
                      title="Click to insert column into SQL"
                      style={{
                        display: 'flex',
                        alignItems: 'center',
                        justifyContent: 'space-between',
                        padding: '4px 6px',
                        borderRadius: '4px',
                        fontSize: '12px',
                        cursor: 'pointer',
                        transition: 'background 0.15s ease'
                      }}
                      onMouseEnter={e => e.currentTarget.style.background = 'rgba(99, 102, 241, 0.15)'}
                      onMouseLeave={e => e.currentTarget.style.background = 'transparent'}
                    >
                      <div style={{ display: 'flex', alignItems: 'center', gap: '6px' }}>
                        <span style={{ fontFamily: 'var(--font-mono)', color: '#cbd5e1' }}>
                          {col.name}
                        </span>
                        {col.is_pk && (
                          <span
                            title="Primary Key"
                            style={{
                              fontSize: '9px',
                              fontWeight: 'bold',
                              padding: '1px 4px',
                              borderRadius: '3px',
                              background: 'rgba(245, 158, 11, 0.2)',
                              color: '#fbbf24',
                              border: '1px solid rgba(245, 158, 11, 0.4)'
                            }}
                          >
                            PK
                          </span>
                        )}
                        {col.is_fk && (
                          <span
                            title={`Foreign Key -> ${col.fk_target}`}
                            style={{
                              fontSize: '9px',
                              fontWeight: 'bold',
                              padding: '1px 4px',
                              borderRadius: '3px',
                              background: 'rgba(14, 165, 233, 0.2)',
                              color: '#38bdf8',
                              border: '1px solid rgba(14, 165, 233, 0.4)'
                            }}
                          >
                            FK
                          </span>
                        )}
                      </div>
                      <span style={{ fontFamily: 'var(--font-mono)', fontSize: '11px', color: 'var(--text-muted)' }}>
                        {col.type}
                      </span>
                    </div>
                  ))}
                </div>
              )}
            </div>
          );
        })}
      </div>
    </div>
  );
}
