import React, { useState, useMemo } from 'react';
import { Search } from 'lucide-react';

const TYPE_COLORS = {
  SELECT: '#4f46e5',
  FROM: '#4f46e5',
  WHERE: '#4f46e5',
  JOIN: '#0284c7',
  INNER: '#0284c7',
  LEFT: '#0284c7',
  RIGHT: '#0284c7',
  ON: '#0284c7',
  GROUP: '#9333ea',
  BY: '#9333ea',
  HAVING: '#9333ea',
  ORDER: '#c026d3',
  DISTINCT: '#db2777',
  UNION: '#2563eb',
  INTERSECT: '#0d9488',
  EXCEPT: '#e11d48',
  MINUS: '#e11d48',
  IDENTIFIER: '#0f172a',
  INT_LITERAL: '#d97706',
  FLOAT_LITERAL: '#d97706',
  STRING_LITERAL: '#059669',
  COMMENT: '#94a3b8'
};

export default function TokenTable({ tokens }) {
  const [filter, setFilter] = useState('');

  const filteredTokens = useMemo(() => {
    if (!tokens) return [];
    if (!filter.trim()) return tokens;
    const query = filter.toLowerCase();
    return tokens.filter(t =>
      t.type.toLowerCase().includes(query) ||
      t.lexeme.toLowerCase().includes(query)
    );
  }, [tokens, filter]);

  if (!tokens || tokens.length === 0) {
    return (
      <div style={{ padding: '24px', textAlign: 'center', color: 'var(--text-muted)' }}>
        No tokens generated yet. Run a compilation to view the token stream.
      </div>
    );
  }

  return (
    <div style={{ display: 'flex', flexDirection: 'column', height: '100%', background: '#ffffff' }}>
      {/* Search bar */}
      <div style={{ padding: '8px 12px', borderBottom: '1px solid var(--border-color)', display: 'flex', alignItems: 'center', gap: '8px' }}>
        <Search size={14} style={{ color: 'var(--text-muted)' }} />
        <input
          type="text"
          placeholder="Filter tokens by type or lexeme..."
          value={filter}
          onChange={e => setFilter(e.target.value)}
          style={{
            background: 'transparent',
            border: 'none',
            outline: 'none',
            color: 'var(--text-primary)',
            fontSize: '12px',
            width: '100%'
          }}
        />
        <span style={{ fontSize: '11px', color: 'var(--text-muted)', whiteSpace: 'nowrap' }}>
          {filteredTokens.length} / {tokens.length} tokens
        </span>
      </div>

      {/* Table */}
      <div style={{ flex: 1, overflowY: 'auto' }}>
        <table style={{ width: '100%', borderCollapse: 'collapse', fontSize: '12px', textAlign: 'left' }}>
          <thead>
            <tr style={{ background: '#f8fafc', position: 'sticky', top: 0, borderBottom: '1px solid var(--border-color)', color: 'var(--text-secondary)' }}>
              <th style={{ padding: '6px 10px', width: '40px' }}>#</th>
              <th style={{ padding: '6px 10px' }}>Token Type</th>
              <th style={{ padding: '6px 10px' }}>Lexeme</th>
              <th style={{ padding: '6px 10px', width: '50px' }}>Line</th>
              <th style={{ padding: '6px 10px', width: '50px' }}>Col</th>
            </tr>
          </thead>
          <tbody>
            {filteredTokens.map((t, idx) => {
              const color = TYPE_COLORS[t.type] || '#475569';
              return (
                <tr
                  key={idx}
                  style={{
                    borderBottom: '1px solid #f1f5f9',
                    background: idx % 2 === 0 ? '#ffffff' : '#fafbfc'
                  }}
                >
                  <td style={{ padding: '5px 10px', color: 'var(--text-muted)', fontFamily: 'var(--font-mono)', fontSize: '11px' }}>
                    {idx + 1}
                  </td>
                  <td style={{ padding: '5px 10px' }}>
                    <span
                      style={{
                        display: 'inline-block',
                        padding: '1px 5px',
                        borderRadius: '3px',
                        background: '#f1f5f9',
                        color: color,
                        fontFamily: 'var(--font-mono)',
                        fontWeight: '600',
                        fontSize: '11px'
                      }}
                    >
                      {t.type}
                    </span>
                  </td>
                  <td style={{ padding: '5px 10px', fontFamily: 'var(--font-mono)', color: 'var(--text-primary)' }}>
                    {t.lexeme}
                  </td>
                  <td style={{ padding: '5px 10px', color: 'var(--text-secondary)', fontFamily: 'var(--font-mono)', fontSize: '11px' }}>
                    {t.line}
                  </td>
                  <td style={{ padding: '5px 10px', color: 'var(--text-secondary)', fontFamily: 'var(--font-mono)', fontSize: '11px' }}>
                    {t.col}
                  </td>
                </tr>
              );
            })}
          </tbody>
        </table>
      </div>
    </div>
  );
}
