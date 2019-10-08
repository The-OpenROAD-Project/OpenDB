///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019, Nefelus Inc
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <math.h>
#include <float.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include "adsRTreeCore.h"
#include "adsRTree.h"
#include "logger.h"

BEGIN_NAMESPACE_ADS

adsRTreeCore::adsRTreeCore( unsigned short max_children_per_node ) 
        : _branch_alloc(512)
{
    _MAX = max_children_per_node;
    _MIN = (int) ceil(((double) _MAX) * 0.4);
    _REINSERT = (int) ceil(((double) _MAX) * 0.3);
    _root = _branch_alloc.create();
    _root->_bbox.mergeInit();
}

adsRTreeCore::~adsRTreeCore()
{
}

void adsRTreeCore::clear()
{
    _branch_alloc.clear();
    _root = _branch_alloc.create();
    _root->_bbox.mergeInit();
}

///
/// chooseChild: This implementation only implements the "branch" choose algorithm.
//
inline adsRTreeNode * adsRTreeCore::chooseChild( const adsRect & ir, adsRTreeBranch * parent, bool & is_contained )
{
    register adsRTreeNode * child;
    register adsRTreeNode * min_child = NULL;
    register uint64 min_area = ~0;


    // Optimization to speed up insertion
    if ( is_contained )
    {
        for( child = parent->_children; child != NULL; child = child->_next )
        {
            if ( child->_bbox.contains(ir) )
            {
                register uint64 child_area = child->_bbox.area();
    
                if ( child_area < min_area )
                {
                    min_child = child;
                    min_area = child_area;
                }
            }
        }

        if ( min_child )
            return min_child;
    }
    
    is_contained = false;
    
    register uint64 min_cost = ~0;

    for( child = parent->_children; child != NULL; child = child->_next )
    {
        adsRect r = child->_bbox;
        r.merge(ir);
        
        register uint64 child_area = child->_bbox.area();
        register uint64 merged_area = r.area();
        register uint64 cost = merged_area - child_area;

        // take solution with least area-enlargement
        if ( cost < min_cost )
        {
            min_child = child;
            min_cost = cost;
            min_area = child_area;
        }
        else if ( cost == min_cost )
        {
            if ( child_area < min_area )
            {
                min_child = child;
                min_area = child_area;
            }
        }
    }

    return min_child;
}

inline adsRTreeNode * adsRTreeCore::chooseSubTree( const adsRect & ir, int level, adsRTreeNode * path[] )
{
    assert(level != 0);
    register adsRTreeNode * node = _root;
    path[node->_level] = node;
    bool is_contained = _root->_bbox.contains(ir);

    while( node->_level != level )
    {
        assert(((adsRTreeBranch *)node)->_children);
        node = chooseChild(ir, (adsRTreeBranch *) node, is_contained);
        path[node->_level] = node;
    }

    return node;
}

inline void adsRTreeCore::update_bboxes( int level, adsRTreeNode * path[] )
{
    adsRTreeNode * child = path[level];
    level++;

    for( ;; )
    {
        adsRTreeNode * parent = path[level];

        if ( parent->_bbox.contains( child->_bbox ) )
            break;

        parent->_bbox.merge( child->_bbox );

        if ( parent == _root )
            break;

        child = parent;
        parent = path[++level];
    }
}

void adsRTreeCore::recompute_bboxes( int level, adsRTreeNode * path[] )
{
    assert(level != 0);
    adsRTreeBranch * parent = (adsRTreeBranch *) path[level];

    for( ;; )
    {
        adsRect old_bbox = parent->_bbox;
        parent->_bbox.mergeInit();
        adsRTreeNode * child;

        for( child = parent->_children; child != NULL; child = child->_next )
            parent->_bbox.merge( child->_bbox );
        
        if ( parent == _root )
            return;

        parent = (adsRTreeBranch *) path[++level];

        if ( parent->_bbox.inside( old_bbox ) )
            return;
    }
}

inline void adsRTreeCore::adjustTree( adsRTreeBranch * new_child, adsRTreeNode * path[] )
{
    for( ; new_child != NULL ; )
    {
        adsRTreeBranch * parent = (adsRTreeBranch *) path[new_child->_level+1];
        parent->_count++;
        new_child->_next = parent->_children;
        parent->_children = new_child;
        
        if ( parent->_count <= _MAX )
            return;
        
        // overflowed parent... split again
        new_child = split( parent );
    }
}

struct adsRTreeNodeReinsertCmp
{
    adsRTreeNode * _node;
    uint64         _value;
    
    int operator<( const adsRTreeNodeReinsertCmp & n ) const
    {
        // sort from HIGH to LOW, hence; ">"
        return _value > n._value;
    }
};

void adsRTreeCore::reinsert( adsRTreeBranch * node, adsRTreeNode * path[],  unsigned char * overflow )
{
    assert( node->_level != 0 );
    
    adsRTreeNodeReinsertCmp * array = (adsRTreeNodeReinsertCmp *) malloc( sizeof(adsRTreeNodeReinsertCmp) * node->_count);

    int i = 0;
    int cx = node->_bbox.xMin() + (int) (node->_bbox.dx() >> 1);
    int cy = node->_bbox.yMin() + (int) (node->_bbox.dy() >> 1);
    adsRTreeNode * child;

    for( child = node->_children; child != NULL; child = child->_next, ++i )
    {
        int x = child->_bbox.xMin() + (int) (child->_bbox.dx() >> 1);
        int y = child->_bbox.yMin() + (int) (child->_bbox.dy() >> 1);
        int dx = x - cx;
        int dy = y - cy;
        array[i]._node = child;
        array[i]._value = dx*dx + dy*dy;
    }

    std::sort( array, &array[node->_count] );

    adsRect old_bbox = node->_bbox;
    // Insert the nodes array[REINSERT...child->_count) back into the "node", i.e.,
    // fix up the pointers...
    node->_bbox.mergeInit();
    int n = node->_count;
    node->_count = n -_REINSERT;
    node->_children = 0;

    for ( i = _REINSERT; i < n; ++i )
    {
        child = array[i]._node;
        child->_next = node->_children;
        node->_children = child;
        node->_bbox.merge( child->_bbox );
    }

    adsRTreeBranch * parent = (adsRTreeBranch *) path[node->_level+1];

    // Recompute the bboxes if needed
    if ( parent->_bbox.inside( old_bbox ) == false )
        recompute_bboxes( parent->_level, path );

    for( i = 0; i < _REINSERT; ++i )
    {
        child = array[i]._node;
        insert( child, node->_level, overflow );
    }

    free( (void *) array );
}

struct adsRTreeNodeSplitCmpX
{
    bool           _sort_min;
    
    bool operator()( adsRTreeNode * n1, adsRTreeNode * n2 )
    {
        if ( _sort_min )
            return n1->_bbox.xMin() < n2->_bbox.xMin();
        else
            return n1->_bbox.xMax() < n2->_bbox.xMax();
    }
};

struct adsRTreeNodeSplitCmpY
{
    bool           _sort_min;
    
    bool operator()( adsRTreeNode * n1, adsRTreeNode * n2 )
    {
        if ( _sort_min )
            return n1->_bbox.yMin() < n2->_bbox.yMin();
        else
            return n2->_bbox.yMax() < n2->_bbox.yMax();
    }
};

///
/// chooseSplitYAxis - Returns the margin sum of all distributions. The distribution with the min.
/// score is return as an idx-parameter in min_dist.
///
uint64 adsRTreeCore::chooseSplitYAxis( adsRTreeBranch * parent, adsRTreeNode * children[] )
{
    adsRTreeNodeSplitCmpX cmp;
    register int count = parent->_count;
    adsRTreeNode ** s = children;
    adsRTreeNode ** e = &children[count];
    cmp._sort_min = true;
    std::stable_sort( s, e, cmp );
    cmp._sort_min = false;
    std::stable_sort( s, e, cmp );

    register uint64 margin_sum = 0;
    register int k;
    register int n = _MAX - 2*_MIN + 2;

    for( k = 1; k <= n; ++k )
    {
        int i;
        int grp1 = _MIN - 1 + k;

        adsRect r1;
        r1.mergeInit();
        
        for( i = 0; i < grp1; ++i )
            r1.merge( children[i]->_bbox );

        adsRect r2;
        r2.mergeInit();

        for( i = grp1; i < count; ++i )
            r2.merge( children[i]->_bbox );

        margin_sum += r1.margin();;
        margin_sum += r2.margin();;
    }

    return margin_sum;
}

///
/// chooseSplitXAxis - Returns the margin sum of all distributions. The distribution with the min.
/// score is return as an idx-parameter in min_dist.
///
uint64 adsRTreeCore::chooseSplitXAxis( adsRTreeBranch * parent, adsRTreeNode * children[] )
{
    adsRTreeNodeSplitCmpY cmp;
    register int count = parent->_count;
    adsRTreeNode ** s = children;
    adsRTreeNode ** e = &children[count];
    cmp._sort_min = true;
    std::stable_sort( s, e, cmp );
    cmp._sort_min = false;
    std::stable_sort( s, e, cmp );

    register uint64 margin_sum = 0;
    register int k;
    register int n = _MAX - 2*_MIN + 2;

    for( k = 1; k <= n; ++k )
    {
        int i;
        int grp1 = _MIN - 1 + k;

        adsRect r1;
        r1.mergeInit();
        
        for( i = 0; i < grp1; ++i )
            r1.merge( children[i]->_bbox );

        adsRect r2;
        r2.mergeInit();

        for( i = grp1; i < count; ++i )
            r2.merge( children[i]->_bbox );

        margin_sum += r1.margin();;
        margin_sum += r2.margin();;
    }

    return margin_sum;
}

int adsRTreeCore::chooseSplitDistribution( adsRTreeBranch * parent, adsRTreeNode * children[] )
{
    register int count = parent->_count;
    register uint64 min_overlap = ~0;
    register uint64 min_area = ~0;
    register uint64 min_dist = 0;
    register int k;
    register int n = _MAX - 2*_MIN + 2;

    for( k = 1; k <= n; ++k )
    {
        int i;
        int grp1 = _MIN - 1 + k;

        adsRect r1;
        r1.mergeInit();
        
        for( i = 0; i < grp1; ++i )
            r1.merge( children[i]->_bbox );

        adsRect r2;
        r2.mergeInit();

        for( i = grp1; i < count; ++i )
            r2.merge( children[i]->_bbox );

        adsRect r3;
        r1.intersection(r2, r3);
        uint64 overlap = r3.area();
        uint64 area = r1.area() + r2.area();

        if ( overlap < min_overlap )
        {
            min_overlap = overlap;
            min_area = area;
            min_dist = k;
        }
        else if ( overlap == min_overlap )
        {
            if ( area < min_area )
            {
                min_area = area;
                min_dist = k;
            }
        }
    }

    return min_dist;
}

adsRTreeBranch * adsRTreeCore::split( adsRTreeBranch * node )
{
    assert( node->_level > 0 );
    adsRTreeNode ** arrayX = (adsRTreeNode **) malloc(sizeof(adsRTreeNode*) *node->_count);
    adsRTreeNode ** arrayY = (adsRTreeNode **) malloc(sizeof(adsRTreeNode*) *node->_count);

    int i;
    adsRTreeNode * child;

    for( i = 0, child = node->_children; child; child = child->_next, ++i )
    {
        arrayX[i] = child;
        arrayY[i] = child;
    }

    uint64 margin_y_split = chooseSplitYAxis( node, arrayX );
    uint64 margin_x_split = chooseSplitXAxis( node, arrayY );
    int k;
    adsRTreeNode ** array;
    
    if ( margin_y_split < margin_x_split )
    {
        k = chooseSplitDistribution( node, arrayX );
        array = arrayX;
    }
    else
    {
        k = chooseSplitDistribution( node, arrayY );
        array = arrayY;
    }

    adsRect old_bbox = node->_bbox;
    int grp1 = _MIN - 1 + k;
    int count = node->_count;
    node->_bbox.mergeInit();
    node->_count = grp1;
    node->_children = NULL;

    for( i = 0; i < grp1; ++i )
    {
        adsRTreeNode * child = array[i];
        child->_next = node->_children;
        node->_children = child;
        node->_bbox.merge( child->_bbox );
    }

    adsRTreeBranch * branch = _branch_alloc.create();
    branch->_level = node->_level;
    branch->_bbox.mergeInit();
    branch->_count = count - grp1;
    branch->_children = NULL;

    for( i = grp1; i < count; ++i )
    {
        adsRTreeNode * child = array[i];
        child->_next = branch->_children;
        branch->_children = child;
        branch->_bbox.merge( child->_bbox );
    }

    if ( node == _root ) // the root was split...
    {
        int level = _root->_level + 1;
        _root = _branch_alloc.create();
        _root->_level = level;
        _root->_bbox = old_bbox;
        _root->_count = 2;
        _root->_children = NULL;
        node->_next = _root->_children;
        _root->_children = node;
        branch->_next = _root->_children;
        _root->_children = branch;
        free( (void *) arrayX );
        free( (void *) arrayY );
        return NULL;
    }

    free( (void *) arrayX );
    free( (void *) arrayY );
    return branch;
}

void adsRTreeCore::insert( adsRTreeNode * child, int level, unsigned char * overflow )
{
    adsRTreeNode * path[SP_MAX+2];
    chooseSubTree( child->_bbox, level, path );
    adsRTreeBranch * parent = (adsRTreeBranch *) path[level];
    parent->_count++;
    child->_next = parent->_children;
    parent->_children = child;
    path[level-1] = child;
    update_bboxes( level-1, path );

    if ( parent->_count > _MAX )
    {
        // overflow...
        if ( (parent == _root) || overflow[parent->_level] )
            adjustTree( split(parent), path );
        else
        {
            overflow[level] = true;
            reinsert((adsRTreeBranch *) parent, path, overflow);
        }
    }
}

void adsRTreeCore::condense_tree( int level, adsRTreeNode * path[] )
{
    assert(level != 0);
    adsRTreeBranch * node = (adsRTreeBranch *) path[level];
    adsRTreeNode * reinsert_list = NULL;

    while( (node->_count < _MIN) && (node != _root) )
    {
        // remove "node" from parent...
        adsRTreeBranch * parent = (adsRTreeBranch *) path[++level];
        adsRTreeNode * c = parent->_children;
        adsRTreeNode * p = NULL;

        for( ; c ; p = c, c = c->_next )
        {
            if ( c == node )
            {
                if ( p == NULL )
                    parent->_children = c->_next;
                else
                    p->_next = c ->_next;
        
                break;
            }
        }

        parent->_count--;

        // recompute the bboxes if needed
        if ( parent->_bbox.inside( node->_bbox ) == false )
            recompute_bboxes( level, path );

        // add child-nodes to reinsert list
        c = node->_children;

        while( c )
        {
            adsRTreeNode * next = c->_next;
            c->_next = reinsert_list;
            reinsert_list = c;
            c = next;
        }

        assert(node->_level != 0);
        _branch_alloc.destroy(node);
        node = parent;
    }

    // reinsert orphans
    while( reinsert_list )
    {
        adsRTreeNode * n = reinsert_list;
        reinsert_list = n->_next;
        unsigned char overflow[SP_MAX+2];
        memset( overflow, 0, _root->_level+2 );
        adsRTreeCore::insert( n, n->_level+1, overflow );
    }

    if ( (_root->_count == 1) && (_root->_level > 1) )
    {
        adsRTreeBranch * n = (adsRTreeBranch *) _root;
        _root = (adsRTreeBranch *) n->_children;
        assert(n->_level != 0);
        _branch_alloc.destroy(n);
    }
}

void adsRTreeCore::checkBBoxes()
{
    checkBBoxes( _root );
}

void adsRTreeCore::checkBBoxes( adsRTreeNode * node )
{
    assert( node != 0 );
    adsRect bbox;
    bbox.mergeInit();
    
    adsRTreeBranch * b = (adsRTreeBranch *) node;
    adsRTreeNode * child;

    for( child = b->_children; child; child = child->_next )
        bbox.merge(child->_bbox);

    assert( bbox == b->_bbox );
    
    if ( node->_level > 1 )
        for( child = b->_children; child; child = child->_next )
            checkBBoxes(child);
}

class adsRTreeStats
{
public:
    std::vector<int> _level_count;
};

void adsRTreeCore::printStats()
{
    adsRTreeStats stats;
    stats._level_count.resize( _root->_level + 1 );
    printStats( _root, &stats );

    int i;

    for( i = _root->_level; i >= 0; --i )
        notice(0,"Level %d node count = %d\n", i, stats._level_count[i] );
}

void adsRTreeCore::printStats( adsRTreeNode * node, adsRTreeStats * stats )
{
    stats->_level_count[node->_level]++;

    if ( node->_level == 0 )
        return;
    
    if ( node != _root && node->_count < _MIN )
        notice(0,"Node at level (%d) has count(%d) < _MIN(%d)\n", node->_level,
               node->_count, _MIN);

    if ( node->_count > _MAX )
        notice(0,"Node at level (%d) has count(%d) > _MAX(%d)\n", node->_level,
               node->_count, _MAX);
        
    adsRTreeBranch * b = (adsRTreeBranch *) node;
    adsRTreeNode * child;

    for( child = b->_children; child; child = child->_next )
        printStats( child, stats );
}

END_NAMESPACE_ADS
