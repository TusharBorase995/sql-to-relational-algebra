import React, { useEffect, useRef, useState } from 'react';
import * as d3 from 'd3';
import { ZoomIn, ZoomOut, RotateCcw } from 'lucide-react';

const OP_COLORS = {
  'π': { bg: '#ede9fe', border: '#c4b5fd', text: '#3b0764', label: '#4c1d95', badge: '#ddd6fe' }, // Projection (lavender)
  'σ': { bg: '#dcfce7', border: '#86efac', text: '#052e16', label: '#14532d', badge: '#bbf7d0' }, // Selection (mint)
  '⋈': { bg: '#f3e8ff', border: '#d8b4fe', text: '#3b0764', label: '#581c87', badge: '#e9d5ff' }, // Theta Join (soft purple)
  '×': { bg: '#fef3c7', border: '#fcd34d', text: '#451a03', label: '#78350f', badge: '#fde68a' }, // Cross Product (amber)
  'γ': { bg: '#ffe4e6', border: '#fecdd3', text: '#4c0519', label: '#881337', badge: '#fecdd3' }, // Aggregation (soft rose)
  'τ': { bg: '#fae8ff', border: '#f5d0fe', text: '#4a044e', label: '#701a75', badge: '#f0abfc' }, // Sort (soft pink)
  'δ': { bg: '#ccfbf1', border: '#99f6e4', text: '#042f2e', label: '#134e4a', badge: '#5eead4' }, // Distinct (teal)
  '∪': { bg: '#e0f2fe', border: '#bae6fd', text: '#082f49', label: '#0369a1', badge: '#7dd3fc' }, // Union
  '∩': { bg: '#e0f2fe', border: '#bae6fd', text: '#082f49', label: '#0369a1', badge: '#7dd3fc' }, // Intersect
  '−': { bg: '#fee2e2', border: '#fca5a5', text: '#450a0a', label: '#991b1b', badge: '#f87171' }, // Except
  'R': { bg: '#f8fafc', border: '#e2e8f0', text: '#0f172a', label: '#475569', badge: '#f1f5f9' }  // Base Relation
};

export default function RaTree({ data, onSelectNode, selectedNodeId }) {
  const svgRef = useRef(null);
  const containerRef = useRef(null);
  const zoomBehaviorRef = useRef(null);

  useEffect(() => {
    if (!data || !svgRef.current || !containerRef.current) return;

    const width = containerRef.current.clientWidth || 700;
    const height = containerRef.current.clientHeight || 450;

    const svg = d3.select(svgRef.current);
    svg.selectAll('*').remove();

    // Setup main zoomable container
    const g = svg.append('g').attr('class', 'main-tree-group');

    const zoom = d3.zoom()
      .scaleExtent([0.25, 2.5])
      .on('zoom', (event) => {
        g.attr('transform', event.transform);
      });

    zoomBehaviorRef.current = zoom;
    svg.call(zoom);

    // Setup hierarchy layout
    const root = d3.hierarchy(data, d => d.children);

    const nodeWidth = 170;
    const nodeHeight = 65;
    const treeLayout = d3.tree().nodeSize([nodeWidth + 35, nodeHeight + 45]);

    treeLayout(root);

    // Smooth curved links
    const linkGenerator = d3.linkVertical()
      .x(d => d.x)
      .y(d => d.y);

    g.selectAll('.tree-link')
      .data(root.links())
      .enter()
      .append('path')
      .attr('class', 'tree-link')
      .attr('d', linkGenerator)
      .attr('fill', 'none')
      .attr('stroke', '#94a3b8')
      .attr('stroke-width', 1.8)
      .attr('opacity', 0.85);

    // Node groups
    const node = g.selectAll('.tree-node')
      .data(root.descendants())
      .enter()
      .append('g')
      .attr('class', 'tree-node')
      .attr('transform', d => `translate(${d.x - nodeWidth / 2}, ${d.y - nodeHeight / 2})`)
      .style('cursor', 'pointer')
      .on('click', (event, d) => {
        event.stopPropagation();
        if (onSelectNode) onSelectNode(d.data);
      });

    // Node Card Background
    node.append('rect')
      .attr('width', nodeWidth)
      .attr('height', nodeHeight)
      .attr('rx', 8)
      .attr('ry', 8)
      .attr('fill', d => {
        const conf = OP_COLORS[d.data.op_symbol] || OP_COLORS['R'];
        return conf.bg;
      })
      .attr('stroke', d => {
        const conf = OP_COLORS[d.data.op_symbol] || OP_COLORS['R'];
        return d.data.id === selectedNodeId ? '#065f46' : conf.border;
      })
      .attr('stroke-width', d => (d.data.id === selectedNodeId ? 2.5 : 1.2))
      .style('filter', d => {
        return d.data.id === selectedNodeId
          ? 'drop-shadow(0 4px 12px rgba(6, 95, 70, 0.25))'
          : 'drop-shadow(0 1px 3px rgba(0, 0, 0, 0.05))';
      });

    // Operator Symbol text in center top
    node.append('text')
      .attr('x', nodeWidth / 2)
      .attr('y', 24)
      .attr('text-anchor', 'middle')
      .attr('fill', d => {
        const conf = OP_COLORS[d.data.op_symbol] || OP_COLORS['R'];
        return conf.text;
      })
      .attr('font-size', '17px')
      .attr('font-weight', '700')
      .text(d => {
        if (d.data.op_symbol === 'R') {
          return d.data.details || 'Relation';
        }
        return d.data.op_symbol;
      });

    // Subtitle / Expression details below symbol
    node.append('text')
      .attr('x', nodeWidth / 2)
      .attr('y', 44)
      .attr('text-anchor', 'middle')
      .attr('fill', d => {
        const conf = OP_COLORS[d.data.op_symbol] || OP_COLORS['R'];
        return conf.label;
      })
      .attr('font-size', '11px')
      .attr('font-family', 'var(--font-mono)')
      .text(d => {
        if (d.data.op_symbol === 'R') {
          // If there's an alias or condition
          return d.data.condition ? `AS ${d.data.condition}` : '';
        }
        const text = d.data.details || d.data.condition || '';
        return text.length > 22 ? text.substring(0, 20) + '…' : text;
      });

    // Initial centering of tree
    const bounds = g.node().getBBox();
    const midX = bounds.x + bounds.width / 2;
    const initialTransform = d3.zoomIdentity
      .translate(width / 2 - midX, 35)
      .scale(0.9);

    svg.call(zoom.transform, initialTransform);
  }, [data, selectedNodeId]);

  // Zoom controls
  const handleZoomIn = () => {
    if (svgRef.current && zoomBehaviorRef.current) {
      d3.select(svgRef.current).transition().duration(200).call(zoomBehaviorRef.current.scaleBy, 1.25);
    }
  };

  const handleZoomOut = () => {
    if (svgRef.current && zoomBehaviorRef.current) {
      d3.select(svgRef.current).transition().duration(200).call(zoomBehaviorRef.current.scaleBy, 0.8);
    }
  };

  const handleResetZoom = () => {
    if (svgRef.current && zoomBehaviorRef.current && containerRef.current) {
      const svg = d3.select(svgRef.current);
      const g = svg.select('.main-tree-group');
      if (!g.node()) return;
      const width = containerRef.current.clientWidth || 700;
      const bounds = g.node().getBBox();
      const midX = bounds.x + bounds.width / 2;
      const initialTransform = d3.zoomIdentity
        .translate(width / 2 - midX, 35)
        .scale(0.9);
      svg.transition().duration(250).call(zoomBehaviorRef.current.transform, initialTransform);
    }
  };

  return (
    <div ref={containerRef} style={{ width: '100%', height: '100%', position: 'relative', overflow: 'hidden' }}>
      <svg
        ref={svgRef}
        style={{ width: '100%', height: '100%', display: 'block', background: '#ffffff' }}
      />

      {/* Floating Zoom Controls at Bottom Right */}
      <div
        style={{
          position: 'absolute',
          bottom: '12px',
          right: '12px',
          display: 'flex',
          alignItems: 'center',
          gap: '4px',
          background: '#ffffff',
          padding: '4px 6px',
          borderRadius: 'var(--radius-sm)',
          border: '1px solid var(--border-color)',
          boxShadow: '0 2px 8px rgba(0, 0, 0, 0.06)'
        }}
      >
        <button
          onClick={handleZoomOut}
          title="Zoom Out"
          style={{
            background: 'transparent',
            border: 'none',
            color: 'var(--text-secondary)',
            cursor: 'pointer',
            padding: '4px',
            borderRadius: '3px',
            display: 'flex',
            alignItems: 'center'
          }}
          onMouseEnter={e => e.currentTarget.style.color = '#0f172a'}
          onMouseLeave={e => e.currentTarget.style.color = 'var(--text-secondary)'}
        >
          <ZoomOut size={14} />
        </button>
        <button
          onClick={handleZoomIn}
          title="Zoom In"
          style={{
            background: 'transparent',
            border: 'none',
            color: 'var(--text-secondary)',
            cursor: 'pointer',
            padding: '4px',
            borderRadius: '3px',
            display: 'flex',
            alignItems: 'center'
          }}
          onMouseEnter={e => e.currentTarget.style.color = '#0f172a'}
          onMouseLeave={e => e.currentTarget.style.color = 'var(--text-secondary)'}
        >
          <ZoomIn size={14} />
        </button>
        <button
          onClick={handleResetZoom}
          title="Reset View"
          style={{
            background: 'transparent',
            border: 'none',
            color: 'var(--text-secondary)',
            cursor: 'pointer',
            padding: '4px',
            borderRadius: '3px',
            display: 'flex',
            alignItems: 'center'
          }}
          onMouseEnter={e => e.currentTarget.style.color = '#0f172a'}
          onMouseLeave={e => e.currentTarget.style.color = 'var(--text-secondary)'}
        >
          <RotateCcw size={13} />
        </button>
      </div>
    </div>
  );
}
