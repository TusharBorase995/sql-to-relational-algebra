import React from 'react';
import { X, Layers, Code, CheckCircle, Tag } from 'lucide-react';

export default function NodeInspector({ node, onClose }) {
  if (!node) return null;

  return (
    <div
      className="fade-in"
      style={{
        position: 'absolute',
        top: '12px',
        right: '12px',
        width: '320px',
        maxHeight: 'calc(100% - 24px)',
        overflowY: 'auto',
        background: '#ffffff',
        border: '1px solid var(--border-color)',
        borderRadius: 'var(--radius-md)',
        padding: '16px',
        boxShadow: '0 8px 24px rgba(0, 0, 0, 0.08)',
        zIndex: 20
      }}
    >
      {/* Header */}
      <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between', marginBottom: '12px', borderBottom: '1px solid var(--border-color)', paddingBottom: '8px' }}>
        <div style={{ display: 'flex', alignItems: 'center', gap: '8px' }}>
          <span
            style={{
              display: 'inline-flex',
              alignItems: 'center',
              justifyContent: 'center',
              width: '28px',
              height: '28px',
              borderRadius: '50%',
              background: 'var(--primary-light)',
              color: 'var(--primary)',
              fontWeight: 'bold',
              fontSize: '15px'
            }}
          >
            {node.op_symbol}
          </span>
          <div>
            <div style={{ fontWeight: '600', fontSize: '14px', color: 'var(--text-primary)' }}>
              {node.op_name}
            </div>
            <div style={{ fontSize: '11px', color: 'var(--text-muted)' }}>
              Node ID #{node.id}
            </div>
          </div>
        </div>

        <button
          onClick={onClose}
          style={{ background: 'transparent', border: 'none', color: 'var(--text-muted)', cursor: 'pointer', display: 'flex' }}
          onMouseEnter={e => e.currentTarget.style.color = '#0f172a'}
          onMouseLeave={e => e.currentTarget.style.color = 'var(--text-muted)'}
        >
          <X size={16} />
        </button>
      </div>

      {/* Details & Predicates */}
      <div style={{ display: 'flex', flexDirection: 'column', gap: '10px' }}>
        {node.details && (
          <div>
            <div style={{ display: 'flex', alignItems: 'center', gap: '6px', fontSize: '11px', color: 'var(--text-muted)', marginBottom: '3px' }}>
              <Code size={12} />
              <span>Details / Projection List</span>
            </div>
            <div
              style={{
                fontFamily: 'var(--font-mono)',
                fontSize: '12px',
                padding: '6px 8px',
                background: '#f8fafc',
                border: '1px solid var(--border-color)',
                borderRadius: '4px',
                color: 'var(--text-primary)',
                wordBreak: 'break-all'
              }}
            >
              {node.details}
            </div>
          </div>
        )}

        {node.condition && (
          <div>
            <div style={{ display: 'flex', alignItems: 'center', gap: '6px', fontSize: '11px', color: 'var(--text-muted)', marginBottom: '3px' }}>
              <CheckCircle size={12} />
              <span>Condition Predicate</span>
            </div>
            <div
              style={{
                fontFamily: 'var(--font-mono)',
                fontSize: '12px',
                padding: '6px 8px',
                background: '#f8fafc',
                border: '1px solid var(--border-color)',
                borderRadius: '4px',
                color: 'var(--text-primary)',
                wordBreak: 'break-all'
              }}
            >
              {node.condition}
            </div>
          </div>
        )}

        {node.attributes && node.attributes.length > 0 && (
          <div>
            <div style={{ display: 'flex', alignItems: 'center', gap: '6px', fontSize: '11px', color: 'var(--text-muted)', marginBottom: '3px' }}>
              <Tag size={12} />
              <span>Attributes in Scope ({node.attributes.length})</span>
            </div>
            <div style={{ display: 'flex', flexWrap: 'wrap', gap: '4px' }}>
              {node.attributes.map((attr, idx) => (
                <span
                  key={idx}
                  style={{
                    fontFamily: 'var(--font-mono)',
                    fontSize: '11px',
                    padding: '2px 6px',
                    borderRadius: '3px',
                    background: '#f1f5f9',
                    border: '1px solid var(--border-color)',
                    color: 'var(--text-secondary)'
                  }}
                >
                  {attr}
                </span>
              ))}
            </div>
          </div>
        )}

        {/* Linear Subtree Formula */}
        {node.linear_str && (
          <div>
            <div style={{ display: 'flex', alignItems: 'center', gap: '6px', fontSize: '11px', color: 'var(--text-muted)', marginBottom: '3px' }}>
              <Layers size={12} />
              <span>Subtree Expression</span>
            </div>
            <div
              style={{
                fontFamily: 'var(--font-mono)',
                fontSize: '11px',
                padding: '6px 8px',
                background: '#f8fafc',
                border: '1px solid var(--border-color)',
                borderRadius: '4px',
                color: '#065f46',
                lineHeight: '1.4',
                wordBreak: 'break-all'
              }}
            >
              {node.linear_str}
            </div>
          </div>
        )}
      </div>
    </div>
  );
}
