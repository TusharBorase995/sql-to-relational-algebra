import React, { useMemo } from 'react';
import CodeMirror from '@uiw/react-codemirror';
import { sql } from '@codemirror/lang-sql';
import { EditorView, Decoration } from '@codemirror/view';
import { AlertCircle } from 'lucide-react';

export default function SqlEditor({ value, onChange, error, onCompile }) {
  // Inline error decoration if error exists
  const errorExtension = useMemo(() => {
    if (!error || !error.line) return [];

    try {
      const lineNum = error.line;
      const colNum = Math.max(1, error.col || 1);

      const addErrorMarker = EditorView.decorations.compute([], (state) => {
        if (lineNum > state.doc.lines) return Decoration.none;
        const line = state.doc.line(lineNum);
        const from = Math.min(line.from + colNum - 1, line.to);
        const to = Math.min(from + 5, line.to);

        const deco = Decoration.mark({
          class: 'cm-error-text',
          attributes: { title: error.message }
        });

        return Decoration.set([deco.range(from, Math.max(from, to))]);
      });

      return [addErrorMarker];
    } catch (e) {
      return [];
    }
  }, [error]);

  const handleKeyDown = (e) => {
    if ((e.ctrlKey || e.metaKey) && e.key === 'Enter') {
      e.preventDefault();
      if (onCompile) onCompile();
    }
  };

  return (
    <div style={{ display: 'flex', flexDirection: 'column', height: '100%', position: 'relative' }}>
      <div
        style={{
          flex: 1,
          overflow: 'hidden',
          background: '#ffffff',
          borderRadius: 'var(--radius-sm)',
          border: error ? '1px solid var(--rose)' : '1px solid var(--border-color)',
          transition: 'border-color 0.2s ease'
        }}
        onKeyDown={handleKeyDown}
      >
        <CodeMirror
          value={value}
          height="100%"
          extensions={[sql(), ...errorExtension]}
          theme="light"
          onChange={onChange}
          basicSetup={{
            lineNumbers: true,
            highlightActiveLineGutter: true,
            highlightActiveLine: true,
            foldGutter: false,
            autocompletion: true,
            bracketMatching: true,
            closeBrackets: true
          }}
        />
      </div>

      {error && (
        <div
          className="fade-in"
          style={{
            marginTop: '6px',
            padding: '6px 10px',
            background: 'var(--rose-light)',
            border: '1px solid var(--rose-border)',
            borderRadius: 'var(--radius-sm)',
            display: 'flex',
            alignItems: 'center',
            gap: '8px',
            fontSize: '12px',
            color: '#dc2626'
          }}
        >
          <AlertCircle size={14} style={{ flexShrink: 0 }} />
          <span>
            <strong>Line {error.line}, Col {error.col}:</strong> {error.message}
          </span>
        </div>
      )}
    </div>
  );
}
