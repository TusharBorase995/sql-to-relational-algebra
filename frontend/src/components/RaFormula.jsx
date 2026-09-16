import React, { useMemo } from 'react';
import katex from 'katex';
import 'katex/dist/katex.min.css';

/**
 * Strips outer enclosing parentheses if they wrap the entire expression:
 * e.g. "(s.dept_id = d.id)" -> "s.dept_id = d.id"
 */
export function cleanCondition(cond) {
  if (!cond) return '';
  let c = cond.trim();
  if (c.startsWith('(') && c.endsWith(')')) {
    let depth = 0;
    let wraps = true;
    for (let i = 0; i < c.length - 1; i++) {
      if (c[i] === '(') depth++;
      else if (c[i] === ')') depth--;
      if (depth === 0) {
        wraps = false;
        break;
      }
    }
    if (wraps) c = c.slice(1, -1).trim();
  }
  return c;
}

/**
 * Escapes special characters for KaTeX math mode
 */
export function escapeLatex(str) {
  if (!str) return '';
  return str
    .replace(/\\/g, '\\\\')
    .replace(/_/g, '\\_')
    .replace(/%/g, '\\%')
    .replace(/#/g, '\\#')
    .replace(/&/g, '\\&')
    .replace(/<=/g, ' \\le ')
    .replace(/>=/g, ' \\ge ')
    .replace(/!=|<>/g, ' \\ne ')
    .replace(/\bAND\b/gi, ' \\land ')
    .replace(/\bOR\b/gi, ' \\lor ')
    .replace(/\bNOT\b/gi, ' \\neg ');
}

function isLeafNode(node) {
  return !node.children || node.children.length === 0 || node.op_symbol === 'R';
}

/**
 * Converts an AST node recursively into a horizontal mathematical expression.
 */
export function toHorizontalMath(node) {
  if (!node) return { latex: '', plain: '' };

  // Leaf Relation
  if (isLeafNode(node)) {
    const relName = node.details || node.op_name || 'Relation';
    return {
      latex: `\\text{${escapeLatex(relName)}}`,
      plain: relName
    };
  }

  // Unary Operators: π, σ, γ, δ, τ
  if (node.children.length === 1) {
    const child = node.children[0];
    const sym = node.op_symbol;
    const latexSym = sym === 'π' ? '\\pi'
      : sym === 'σ' ? '\\sigma'
      : sym === 'γ' ? '\\gamma'
      : sym === 'δ' ? '\\delta'
      : sym === 'τ' ? '\\tau'
      : sym;

    let sub = '';
    if (sym === 'σ') {
      sub = cleanCondition(node.condition || node.details);
    } else if (sym === 'δ') {
      sub = ''; // δ does not need a subscript if it is just DISTINCT
    } else {
      sub = node.details || cleanCondition(node.condition) || '';
    }

    const latexSub = sub ? `_{${escapeLatex(sub)}}` : '';
    const plainSub = sub ? `_{${sub}}` : '';

    const childRes = toHorizontalMath(child);
    return {
      latex: `${latexSym}${latexSub}(${childRes.latex})`,
      plain: `${sym}${plainSub}(${childRes.plain})`
    };
  }

  // Binary Operators: ⋈, ×, ∪, ∩, −
  if (node.children.length === 2) {
    const left = node.children[0];
    const right = node.children[1];

    const sym = node.op_symbol;
    const latexSym = sym === '⋈' ? '\\bowtie'
      : sym === '×' ? '\\times'
      : sym === '∪' ? '\\cup'
      : sym === '∩' ? '\\cap'
      : sym === '−' ? '-'
      : sym;

    const sub = cleanCondition(node.condition);
    const joinType = node.join_type && node.join_type !== 'INNER' ? `^{${escapeLatex(node.join_type)}}` : '';
    const plainJoinType = node.join_type && node.join_type !== 'INNER' ? `^{${node.join_type}}` : '';

    const latexSub = sub ? `_{${escapeLatex(sub)}}` : '';
    const plainSub = sub ? `_{${sub}}` : '';

    const leftRes = toHorizontalMath(left);
    const rightRes = toHorizontalMath(right);

    const leftLatex = left.children && left.children.length === 2 ? `(${leftRes.latex})` : leftRes.latex;
    const rightLatex = right.children && right.children.length === 2 ? `(${rightRes.latex})` : rightRes.latex;

    const leftPlain = left.children && left.children.length === 2 ? `(${leftRes.plain})` : leftRes.plain;
    const rightPlain = right.children && right.children.length === 2 ? `(${rightRes.plain})` : rightRes.plain;

    return {
      latex: `${leftLatex} \\; ${latexSym}${joinType}${latexSub} \\; ${rightLatex}`,
      plain: `${leftPlain} ${sym}${plainJoinType}${plainSub} ${rightPlain}`
    };
  }

  const raw = node.linear_str || '';
  return {
    latex: `\\text{${escapeLatex(raw)}}`,
    plain: raw
  };
}

/**
 * Fallback converter for raw linear strings like:
 * "π_{s.name, d.name}((σ_{s.age > 20}(students AS s) ⋈_{(s.dept_id = d.id)} π_{id, name}(departments AS d)))"
 */
export function convertRawStringToLatex(raw) {
  if (!raw) return { latex: '', plain: '' };

  let str = raw;
  str = str.replace(/_\{([^}]+)\}/g, (m, p1) => `_{${escapeLatex(cleanCondition(p1))}}`);
  str = str.replace(/([a-zA-Z0-9])_([a-zA-Z0-9])/g, '$1\\_$2');

  const latex = str
    .replace(/π/g, '\\pi ')
    .replace(/σ/g, '\\sigma ')
    .replace(/⋈/g, '\\bowtie ')
    .replace(/γ/g, '\\gamma ')
    .replace(/δ/g, '\\delta ')
    .replace(/τ/g, '\\tau ')
    .replace(/×/g, '\\times ')
    .replace(/∪/g, '\\cup ')
    .replace(/∩/g, '\\cap ')
    .replace(/−/g, '-');

  return { latex, plain: raw };
}

/**
 * Returns clean plain-text string for clipboard copying
 */
export function formatRaTreeToPlainText(tree, rawFallback) {
  if (tree) {
    const res = toHorizontalMath(tree);
    if (res.plain) return res.plain;
  }
  return rawFallback || '';
}

export default function RaFormula({ tree, rawString }) {
  const { html } = useMemo(() => {
    let mathObj = null;
    if (tree) {
      mathObj = toHorizontalMath(tree);
    }
    if ((!mathObj || !mathObj.latex) && rawString) {
      mathObj = convertRawStringToLatex(rawString);
    }

    if (!mathObj || !mathObj.latex) {
      return { html: null };
    }

    try {
      const rendered = katex.renderToString(mathObj.latex, {
        displayMode: false,
        throwOnError: false
      });
      return { html: rendered };
    } catch (e) {
      return { html: null };
    }
  }, [tree, rawString]);

  if (!html) {
    return (
      <span style={{ color: 'var(--text-muted)', fontSize: '13px' }}>
        —
      </span>
    );
  }

  return (
    <div
      className="ra-formula-wrap"
      style={{
        fontSize: '15px',
        lineHeight: '1.8',
        color: '#101828',
        whiteSpace: 'normal',
        wordBreak: 'break-word',
        overflowWrap: 'anywhere',
        padding: '2px 0'
      }}
    >
      <span dangerouslySetInnerHTML={{ __html: html }} />
    </div>
  );
}
