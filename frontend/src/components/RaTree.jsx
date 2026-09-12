import React, { useEffect, useRef } from 'react';
import * as d3 from 'd3';
import { ZoomIn, ZoomOut, RotateCcw } from 'lucide-react';

const OP_COLORS = {
  'π': { bg: '#F3F1FF', border: '#DDD6FE', symbol: '#5B4BDB', label: '#475467' }, // Projection
  'σ': { bg: '#ECFDF5', border: '#A7E3CF', symbol: '#087F5B', label: '#475467' }, // Selection
  '⋈': { bg: '#F5F3FF', border: '#DDD6FE', symbol: '#7C3AED', label: '#475467' }, // Join
  '×': { bg: '#FFF7ED', border: '#FED7AA', symbol: '#C2410C', label: '#475467' }, // Cross
  'γ': { bg: '#FFFBEB', border: '#FDE68A', symbol: '#B45309', label: '#475467' }, // Aggregation
  'δ': { bg: '#F0FDFA', border: '#99F6E4', symbol: '#0F766E', label: '#475467' }, // Distinct
  'τ': { bg: '#F5F3FF', border: '#DDD6FE', symbol: '#7C3AED', label: '#475467' }, // Sort
  '∪': { bg: '#F0F9FF', border: '#BAE6FD', symbol: '#0284C7', label: '#475467' }, // Union
  '∩': { bg: '#F0F9FF', border: '#BAE6FD', symbol: '#0284C7', label: '#475467' }, // Intersect
  '−': { bg: '#FEF3F2', border: '#FECDCA', symbol: '#D92D20', label: '#475467' }, // Except
  'R': { bg: '#F8FAFC', border: '#E4E7EC', symbol: '#101828', label: '#475467' }  // Base Relation
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

    const nodeWidth = 168;
    const nodeHeight = 64;
    const treeLayout = d3.tree().nodeSize([nodeWidth + 36, nodeHeight + 46]);

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
      .attr('stroke', '#D0D5DD')
      .attr('stroke-width', 1.6)
      .attr('opacity', 0.95);

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
      .attr('rx', 7)
      .attr('ry', 7)
      .attr('fill', d => {
        const conf = OP_COLORS[d.data.op_symbol] || OP_COLORS['R'];
        return conf.bg;
      })
      .attr('stroke', d => {
        const conf = OP_COLORS[d.data.op_symbol] || OP_COLORS['R'];
        return d.data.id === selectedNodeId ? '#087F5B' : conf.border;
      })
      .attr('stroke-width', d => (d.data.id === selectedNodeId ? 2 : 1))
      .style('filter', 'drop-shadow(0 1px 2px rgba(16, 24, 40, 0.04))');

    // Operator Symbol text in center top
    node.append('text')
      .attr('x', nodeWidth / 2)
      .attr('y', 25)
      .attr('text-anchor', 'middle')
      .attr('fill', d => {
        const conf = OP_COLORS[d.data.op_symbol] || OP_COLORS['R'];
        return conf.symbol;
      })
      .attr('font-size', '17px')
      .attr('font-weight', '600')
      .attr('font-family', 'var(--font-mono)')
      .text(d => {
        if (d.data.op_symbol === 'R') {
          return d.data.details || 'Relation';
        }
        return d.data.op_symbol;
      });

    // Subtitle / Expression details below symbol
    node.append('text')
      .attr('x', nodeWidth / 2)
      .attr('y', 45)
      .attr('text-anchor', 'middle')
      .attr('fill', d => {
        const conf = OP_COLORS[d.data.op_symbol] || OP_COLORS['R'];
        return conf.label;
      })
      .attr('font-size', '11.5px')
      .attr('font-weight', '500')
      .attr('font-family', 'var(--font-mono)')
      .text(d => {
        if (d.data.op_symbol === 'R') {
          return d.data.condition ? `AS ${d.data.condition}` : '';
        }
        const text = d.data.details || d.data.condition || '';
        return text.length > 22 ? text.substring(0, 20) + '…' : text;
      });

    // Initial centering of tree with comfortable vertical headroom
    const bounds = g.node().getBBox();
    const midX = bounds.x + bounds.width / 2;
    const initialTransform = d3.zoomIdentity
      .translate(width / 2 - midX, 32)
      .scale(0.88);

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
        .translate(width / 2 - midX, 32)
        .scale(0.88);
      svg.transition().duration(250).call(zoomBehaviorRef.current.transform, initialTransform);
    }
  };

  return (
    <div ref={containerRef} style={{ width: '100%', height: '100%', position: 'relative', overflow: 'hidden' }}>
      <svg
        ref={svgRef}
        style={{ width: '100%', height: '100%', display: 'block', background: '#ffffff' }}
      />

      {/* Floating Zoom Controls: Compact Control Group */}
      <div
        style={{
          position: 'absolute',
          bottom: '12px',
          right: '12px',
          display: 'flex',
          alignItems: 'center',
          height: '32px',
          background: '#ffffff',
          borderRadius: '6px',
          border: '1px solid var(--border-color)',
          boxShadow: 'var(--shadow-subtle)',
          overflow: 'hidden'
        }}
      >
        <button
          onClick={handleZoomOut}
          title="Zoom Out"
          style={{
            background: 'transparent',
            border: 'none',
            borderRight: '1px solid var(--border-color)',
            color: 'var(--text-muted)',
            cursor: 'pointer',
            padding: '0 8px',
            height: '100%',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center'
          }}
          onMouseEnter={e => e.currentTarget.style.color = 'var(--text-primary)'}
          onMouseLeave={e => e.currentTarget.style.color = 'var(--text-muted)'}
        >
          <ZoomOut size={13} />
        </button>
        <button
          onClick={handleZoomIn}
          title="Zoom In"
          style={{
            background: 'transparent',
            border: 'none',
            borderRight: '1px solid var(--border-color)',
            color: 'var(--text-muted)',
            cursor: 'pointer',
            padding: '0 8px',
            height: '100%',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center'
          }}
          onMouseEnter={e => e.currentTarget.style.color = 'var(--text-primary)'}
          onMouseLeave={e => e.currentTarget.style.color = 'var(--text-muted)'}
        >
          <ZoomIn size={13} />
        </button>
        <button
          onClick={handleResetZoom}
          title="Reset View"
          style={{
            background: 'transparent',
            border: 'none',
            color: 'var(--text-muted)',
            cursor: 'pointer',
            padding: '0 8px',
            height: '100%',
            display: 'flex',
            alignItems: 'center',
            justifyContent: 'center'
          }}
          onMouseEnter={e => e.currentTarget.style.color = 'var(--text-primary)'}
          onMouseLeave={e => e.currentTarget.style.color = 'var(--text-muted)'}
        >
          <RotateCcw size={12} />
        </button>
      </div>
    </div>
  );
}
