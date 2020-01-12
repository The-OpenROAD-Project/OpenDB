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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef ADS_RTREE_CORE_H
#define ADS_RTREE_CORE_H

#include "ads.h"
#include "adsAllocator.h"
#include "geom.h"

namespace odb {

class adsRTreeNode
{
 public:
  unsigned char  _level;  // leaf = 0, leaf-branch = 1, 1 > internal-branch
  unsigned short _count;  // branch child-count
  adsRect        _bbox;
  adsRTreeNode*  _next;

  adsRTreeNode()
  {
    _count = 0;
    _next  = NULL;
  }
};

class adsRTreeBranch : public adsRTreeNode
{
 public:
  adsRTreeNode* _children;

  adsRTreeBranch()
  {
    _children = NULL;
    _level    = 1;
  }
};

template <class T>
class adsRTreeLeaf : public adsRTreeNode
{
 public:
  T _value;
  adsRTreeLeaf() { _level = 0; }
};

class adsRTreeStats;

class adsRTreeCore
{
 protected:
  enum
  {
    SP_MAX = 64
  };
  int _MAX;       // maximum number of children per node.
  int _MIN;       // minimum number of children per node.
  int _REINSERT;  // number of children per node to reinsert.

  adsAllocator<adsRTreeBranch> _branch_alloc;
  adsRTreeBranch*              _root;

  adsRTreeCore(unsigned short max_children_per_node);
  ~adsRTreeCore();
  void          clear();
  adsRTreeNode* chooseChild(const adsRect&  ir,
                            adsRTreeBranch* node,
                            bool&           is_contained);
  adsRTreeNode* chooseSubTree(const adsRect& rect,
                              int            level,
                              adsRTreeNode*  path[]);
  void          update_bboxes(int level, adsRTreeNode* path[]);
  void          recompute_bboxes(int level, adsRTreeNode* path[]);
  void          adjustTree(adsRTreeBranch* new_child, adsRTreeNode* path[]);
  void          reinsert(adsRTreeBranch* node,
                         adsRTreeNode*   path[],
                         unsigned char*  overflow);
  uint64 chooseSplitYAxis(adsRTreeBranch* parent, adsRTreeNode* children[]);
  uint64 chooseSplitXAxis(adsRTreeBranch* parent, adsRTreeNode* children[]);
  int chooseSplitDistribution(adsRTreeBranch* parent, adsRTreeNode* children[]);
  adsRTreeBranch* split(adsRTreeBranch* node);
  void insert(adsRTreeNode* child, int level, unsigned char* overflow);
  void condense_tree(int level, adsRTreeNode* path[]);

  void checkBBoxes(adsRTreeNode* node);
  void checkBBoxes();
  void printStats();
  void printStats(adsRTreeNode* node, adsRTreeStats* stats);
};

}  // namespace odb

#endif
