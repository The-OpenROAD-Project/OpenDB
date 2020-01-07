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

/*-------------------------------------------------------------
////	AUTHOR: SANJEEV MAHAJAN
---------------------------------------------------------------*/
#ifndef _KDTREE_H
#define _KDTREE_H
#include "darr.h"
enum Cuttype
{
  X,
  Y,
  C
};
typedef struct _Point
{
  int x;
  int y;
} Point;
enum Itype
{
  TOUCH,
  OVLAP,
  BOUNDARY
};
class Rect
{
 public:
  int left[2];
  int right[2];
};
int rects_intersect(Rect* r1, Rect* r2);
int rects_touch(Rect* r1, Rect* r2);
int rects_intersect_boundary(Rect* r1, Rect* r2);
typedef struct _Idata
{
  struct _Node* left;
  struct _Node* right;
  int           cut;
} Idata;
union Data
{
  Idata*       data;
  Darr<Rect*>* rectar;
};
typedef struct _Node
{
  Data    data;
  Cuttype typ;
  Rect    bbx;
} Node;
class Kdtree
{
 public:
  Kdtree();
  ~Kdtree();
  Kdtree(Darr<Rect*>* sites);
  void init(Darr<Rect*>* sites);
  void init(Rect* sites, int n);
  // int	insert(Rect *r);
  // int	insert_rect_array(Darr<Rect*> *ar);
  // int	remove(Rect *r);
  void  get_cut_rects(Rect* r, Darr<Rect*>* ar, Itype itype = OVLAP);
  int   is_rect_cut(Rect* r, Itype itype);
  Node* find_parent_nodes(Rect* r, Darr<Node*>* ar);
  void  testTreeDbg();
  void  insertRect(Rect* r);
  void  removeRect(Rect* r);
  Node* getNode() { return _node; }
  void  printTree(Node* nd);

 private:
  Node* _node;
  int   _n;
  int   find_cutline(Cuttype typ, Darr<Rect*>* ar);
  void  destroy_rec(Node* nd);
  void  create_tree_rec(Node* nd, Darr<Rect*>* rectar, int num);
  void  get_cut_rects_rec(Rect*             r,
                          Node*             nd,
                          Darr<Rect*>*      ar,
                          Hash<Rect*, int>* table,
                          Itype             itype = OVLAP);
  Node* find_parent_node_rec(Rect* r, Node* nd, Darr<Node*>* ar);
  void  get_cut_rects_bottom(Rect*             r,
                             Darr<Rect*>*      bar,
                             Darr<Rect*>*      ar,
                             Hash<Rect*, int>* table,
                             Itype             itype = OVLAP);
  int   is_rect_cut_rec(Rect* r, Node* nd, Itype itype);
  int   is_rect_cut_bottom(Rect* r, Darr<Rect*>* bar, Itype itype);
  void  printNodeRec(Node* nd);
};

#endif
