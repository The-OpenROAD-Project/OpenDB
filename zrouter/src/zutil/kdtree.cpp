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

/*-------------------------------------------------------------
////	AUTHOR: SANJEEV MAHAJAN 
---------------------------------------------------------------*/
#include "hash.h"
#include "kdtree.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include "ZInterface.h"
#define LARGE 0x7fffffff

static inline int mini(int a, int b) {
  int m = a;
  if(a>b)
    m = b;
  return m;
}
static inline int maxi(int a, int b) {
  int m = a;
  if(a<b)
    m = b;
  return m;
}
Kdtree::Kdtree() {
  _node = 0;
  _n = 0;
}
Kdtree::Kdtree(Darr<Rect*> *sites) {
	_node = (Node *) malloc(sizeof(Node));
	Darr<Rect*> *rar = new Darr<Rect*>;
	_n = sites->n(); 
	int i = 0;
	int minx= LARGE, miny=LARGE, maxx=-LARGE, maxy = -LARGE;
	for(i = 0; i<_n; i++) {
		Rect *site = sites->get(i);
		rar->insert(site);
		if(site->left[0]<minx)
			minx = site->left[0];
		if(site->right[0]>maxx)
			maxx = site->right[0];
		if(site->left[1]<miny)
			miny = site->left[1];
		if(site->right[1]>maxy)
			maxy = site->right[1];
	}
	_node->typ = X;
	_node->bbx.left[0] = minx;
	_node->bbx.right[0] = maxx;
	_node->bbx.left[1] = miny;
	_node->bbx.right[1] = maxy;
	create_tree_rec(_node, rar, 0); 
}

void Kdtree::init(Darr<Rect*> *sites) {
	_node = (Node *) malloc(sizeof(Node));
	Darr<Rect*> *rar = new Darr<Rect*>;
	_n = sites->n(); 
	int i = 0;
	int minx= LARGE, miny=LARGE, maxx=-LARGE, maxy = -LARGE;
	for(i = 0; i<_n; i++) {
		Rect *site = sites->get(i);
		rar->insert(site);
		if(site->left[0]<minx)
			minx = site->left[0];
		if(site->right[0]>maxx)
			maxx = site->right[0];
		if(site->left[1]<miny)
			miny = site->left[1];
		if(site->right[1]>maxy)
			maxy = site->right[1];
	}
	_node->typ = X;
	_node->bbx.left[0] = minx;
	_node->bbx.right[0] = maxx;
	_node->bbx.left[1] = miny;
	_node->bbx.right[1] = maxy;
	create_tree_rec(_node, rar, 0); 
}

void Kdtree::init(Rect *sites,int n) {
        _node = (Node *) malloc(sizeof(Node));
        Darr<Rect*> *rar = new Darr<Rect*>;
        _n = n;
        int i = 0;
        int minx= LARGE, miny=LARGE, maxx=-LARGE, maxy = -LARGE;
        for(i = 0; i<_n; i++) {
                Rect *site = sites+i;
                rar->insert(site);
                if(site->left[0]<minx)
                        minx = site->left[0];
                if(site->right[0]>maxx)
                        maxx = site->right[0];
                if(site->left[1]<miny)
                        miny = site->left[1];
                if(site->right[1]>maxy)
                        maxy = site->right[1];
        }
        _node->typ = X;
        _node->bbx.left[0] = minx;
        _node->bbx.right[0] = maxx;
        _node->bbx.left[1] = miny;
        _node->bbx.right[1] = maxy;
        create_tree_rec(_node, rar, 0);
}

void Kdtree::printTree(Node *nd) {
  printNodeRec(nd);
}
void Kdtree::printNodeRec(Node *nd) {

  ads::debug("KDTREE","a"," (%d,%d) (%d,%d)\n",nd->bbx.left[0],nd->bbx.left[1],nd->bbx.right[0],nd->bbx.right[1]);
  if(nd->typ == C) {
    Rect *site;
    int i;
    for(i=0;i<(nd->data).rectar->n();i++) {
      site = (nd->data).rectar->get(i);
      ads::debug("KDTREE","a","C (%d,%d) (%d,%d)\n",site->left[0],site->left[1],site->right[0],site->right[1]);
    }
    return;
  }
  Node *lft = ((nd->data).data)->left;
  if(lft) {
    ads::debug("KDTREE","a","L (%d,%d) (%d,%d)\n",lft->bbx.left[0],lft->bbx.left[1],lft->bbx.right[0],lft->bbx.right[1]);
    printNodeRec(lft);
  }
  Node *rgt = ((nd->data).data)->right;
  if(rgt) {
    ads::debug("KDTREE","a","R (%d,%d) (%d,%d)\n",rgt->bbx.left[0],rgt->bbx.left[1],rgt->bbx.right[0],rgt->bbx.right[1]);

    printNodeRec(rgt);
  }
}
/* ----------------------------------------------

   Debug method to create a tree and test the 
   various operations on the tree such as, insert, 
   delete, find and print tree
------------------------------------------------*/
void Kdtree::testTreeDbg() {
  int i;
  Rect *rect;
  Darr<Rect *> *arr = new Darr<Rect *>;
  int num;
  double m;
  int minx= LARGE, miny=LARGE, maxx=-LARGE, maxy = -LARGE;
  
  _node = (Node *) malloc(sizeof(Node));
  // create 30 random rectangles
#if 0
  for(i=0;i<30;i++) {
    rect = new Rect;
    m = (double)rand() /RAND_MAX;
    num =  m*55000;
    rect->left[0] = num;
    ads::notice(0,"rect->left[0]%d\n",rect->left[0]);

    m = (double)rand() /RAND_MAX;
    num =  m*55000;
    rect->left[1] = num;
    rect->right[0] = rect->left[0] + 5000;
    rect->right[1] = rect->left[1] + 3600;
    if(rect->left[0]<minx)
       minx = rect->left[0];
    if(rect->right[0]>maxx)
      maxx = rect->right[0];
    if(rect->left[1]<miny)
      miny = rect->left[1];
    if(rect->right[1]>maxy)
      maxy = rect->right[1];
    ads::notice(0,"inserting rect (%d,%d) (%d,%d) into tree\n",rect->left[0],rect->left[1],rect->right[0],rect->right[1]);
    arr->insert(rect);
  } 
#endif

//DONT DELETE these 2 arrays. they are needed for regression test
 int x1Arr[] = {52204, 40135, 40171,36711,17954,9797,4153,28207,7132,29734,16913,24771,25529,2500,29508,34228,40899,6322,54727,8026,10316,39744,45997,41804,32877,48010,5193,42406,43460,36090};

 int y1Arr[] = {2474,34609,35364,36955,22281,22062,28174,27334,19924,49879,43655,47616,25678,5367,38425,26713,19363,26071,43034,17681,17823,14469,12952,53130,16538,49790,17781,30723,44906,17968};

 for(i=0;i<28;i++) {
    rect = new Rect;
    rect->left[0] = x1Arr[i];
    rect->left[1] = y1Arr[i];

    rect->right[0] = x1Arr[i]+5000;
    rect->right[1] = y1Arr[i]+3600;
    if(rect->left[0]<minx)
       minx = rect->left[0];
    if(rect->right[0]>maxx)
      maxx = rect->right[0];
    if(rect->left[1]<miny)
      miny = rect->left[1];
    if(rect->right[1]>maxy)
      maxy = rect->right[1];
    arr->insert(rect);
 }
#if 0
 ads::notice(0,"Bounding Box for Tree: (%d,%d),(%d,%d)\n",minx,miny,maxx,maxy);
#endif
  _node->typ = X;
  _node->bbx.left[0] = minx;
  _node->bbx.right[0] = maxx;
  _node->bbx.left[1] = miny;
  _node->bbx.right[1] = maxy;
  create_tree_rec(_node, arr, 0);
 // printNodeRec(_node);
 for(i=28;i<30;i++) {
  Rect *krect = new Rect; 
  krect->left[0] = x1Arr[i];
  krect->left[1] = y1Arr[i];
  krect->right[0] = x1Arr[i] +5000;
  krect->right[1] = y1Arr[i] +3600;
  ads::notice(0,"inserting rect (%d,%d) (%d,%d) into tree\n",krect->left[0],krect->left[1],krect->right[0],krect->right[1]);
  insertRect(krect);
 }
 Rect krect ;
 krect.left[0] = x1Arr[28];
 krect.left[1] = y1Arr[28];
 krect.right[0] = x1Arr[28] +5000;
 krect.right[1] = y1Arr[28] +3600;
 removeRect(&krect);
}
void
Kdtree::destroy_rec(Node *nd) {
	if(nd->typ == C) {
		delete (nd->data).rectar;
		free(nd);
		return;
	}
	Node *lft = ((nd->data).data)->left;
	if(lft)
		destroy_rec(lft);
	Node *rgt = ((nd->data).data)->right;
	if(rgt)
		destroy_rec(rgt);
	free((nd->data).data);
	free(nd);
}
Kdtree:: ~Kdtree() {
  if (_node) destroy_rec(_node);
}
Rect bobx;
int cmpx(const void *a, const void *b) {
  Rect *ai = *(Rect **)a;
  Rect *bi = *(Rect **)b;
  int a1 = maxi(bobx.left[0], ai->left[0]);
  int a2 = mini(bobx.right[0], ai->right[0]);
  int b1 = maxi(bobx.left[0], bi->left[0]);
  int b2 = mini(bobx.right[0], bi->right[0]);
  if((a1+a2)/2<(b1+b2)/2)
    return -1;
  if((a1+a2)/2>(b1+b2)/2)
    return 1;
  return 0;
}
int cmpy(const void *a, const void *b) {
  Rect *ai = *(Rect **)a;
  Rect *bi = *(Rect **)b;
  int a1 = maxi(bobx.left[1], ai->left[1]);
  int a2 = mini(bobx.right[1], ai->right[1]);
  int b1 = maxi(bobx.left[1], bi->left[1]);
  int b2 = mini(bobx.right[1], bi->right[1]);
  
  if((a1+a2)/2<(b1+b2)/2)
    return -1;
  if((a1+a2)/2>(b1+b2)/2)
    return 1;
  return 0;
}
int
Kdtree::find_cutline(Cuttype typ, Darr<Rect*> *ar) {
  int med = ar->n()/2;
  if(typ == X) {
    ar->dsort(cmpx);
    int x1 = maxi(bobx.left[0], ar->get(med)->left[0]);
    int x2 = mini(bobx.right[0], ar->get(med)->right[0]);
    return (x1+x2)/2;
  }
  ar->dsort(cmpy);
    int x1 = maxi(bobx.left[1], ar->get(med)->left[1]);
    int x2 = mini(bobx.right[1], ar->get(med)->right[1]);
    return (x1+x2)/2;
}
void modify_box(Darr<Rect*> *ar, Rect *bbx) {
	int i;
	int xmin = LARGE, xmax = -LARGE, ymin=LARGE, ymax = -LARGE;
	for(i = 0; i<ar->n(); i++) {
		if(xmin>ar->get(i)->left[0])
			xmin = ar->get(i)->left[0];
		if(xmax<ar->get(i)->right[0])
			xmax = ar->get(i)->right[0];
		if(ymin>ar->get(i)->left[1])
			ymin = ar->get(i)->left[1];
		if(ymax<ar->get(i)->right[1])
			ymax = ar->get(i)->right[1];
	}
	if(xmin>bbx->left[0])
		bbx->left[0] = xmin;
	if(xmax<bbx->right[0])
		bbx->right[0] = xmax;
	if(ymin>bbx->left[1])
		bbx->left[1] = ymin;
	if(ymax<bbx->right[1])
		bbx->right[1] = ymax;
	
}
void 
Kdtree::create_tree_rec(Node *nd, Darr<Rect*> *rectar, int num) {
	int nu = rectar->n();
	int wi;
	Rect *bbx = &(nd->bbx);
	if(nd->typ == X)
		wi = bbx->right[0]-bbx->left[0];
	else
		wi = bbx->right[1]-bbx->left[1]; 
	if(wi<10000 || rectar->n() <= 10) {
        // leaf level rects start from now
		nd->typ = C;
		(nd->data).rectar = rectar;
		return; 
	} 
	int j = 0;
	(nd->data).data = (Idata *) malloc(sizeof(Idata));
	Cuttype typ;
	Darr<Rect*> *arl = new Darr<Rect*>;
	Darr<Rect*> *arr = new Darr<Rect*>;
	bobx = *bbx;
	//int cut = find_cutline(nd->typ, rectar);
	int cut;
	if(nd->typ == X) 
		cut = (bbx->left[0]+bbx->right[0])/2;
	else
		cut = (bbx->left[1]+bbx->right[1])/2;
	((nd->data).data)->cut = cut;
	int i;
	for(i = 0; i<rectar->n(); i++) {
		Rect *r = rectar->get(i);
		int low, high;
		if(nd->typ == X) {
			low = r->left[0];
			high = r->right[0];
		} else {
			low = r->left[1];
			high = r->right[1];
		}
		// For strict intersections 
		if(low<cut)
			arl->insert(r);
		if(high>cut)
			arr->insert(r);
		if((low == high) && (cut == low))
			arl->insert(r);
	}
	//if(((arl->n() == nu) && (arl->n() == num)) || ((arr->n() == nu) && (arr->n() == num))) {}
	if((nu==num)&&((arl->n() == num) || (arr->n() == num))) {
		delete arl;
		delete arr;
		nd->typ = C;
		free((nd->data).data);
		(nd->data).rectar = rectar;
		return; 
	} 
        // delete the original array, since u have split it into 
        // arl and arr
	delete rectar;
	Node *ndl = (Node *) malloc(sizeof(Node));
	Node *ndr = (Node *) malloc(sizeof(Node));
	((nd->data).data)->left = ndl;
	((nd->data).data)->right = ndr;
	if(nd->typ == X) {
		ndl->typ = Y;
		ndr->typ = Y;
	} else {
	if(nd->typ == Y) {
		ndl->typ = X;
		ndr->typ = X;
		}
	}
	ndl->bbx = *bbx;
	if(nd->typ == X)
		ndl->bbx.right[0] = cut;
	else
		ndl->bbx.right[1] = cut;
	if(arl->n() == 0) {
		free(ndl);
		delete arl;
		((nd->data).data)->left = NULL;
	} else { 
		//modify_box(arl, &(ndl->bbx));	
		create_tree_rec(ndl, arl, nu); 
	}
	ndr->bbx = *bbx;
	if(nd->typ == X)
		ndr->bbx.left[0] = cut;
	else
		ndr->bbx.left[1] = cut;
	if(arr->n() == 0) {
		free(ndr);
		delete arr;
		((nd->data).data)->right = NULL;
	} else {
		//modify_box(arr, &(ndl->bbx));	
		create_tree_rec(ndr, arr, nu); 
	}
}
int
rects_touch(Rect *r1, Rect *r2) {
	if(r1->left[0]>r2->right[0])
		return 0;
	if(r2->left[0]>r1->right[0])
		return 0;
	if(r1->left[1]>r2->right[1])
		return 0;
	if(r2->left[1]>r1->right[1])
		return 0;
	return 1;
}
int
rects_intersect(Rect *r1, Rect *r2) {
	if(r1->left[0]>=r2->right[0])
		return 0;
	if(r2->left[0]>=r1->right[0])
		return 0;
	if(r1->left[1]>=r2->right[1])
		return 0;
	if(r2->left[1]>=r1->right[1])
		return 0;
	return 1;
}
int
rect_inside(Rect *r1, Rect *r2) {
  return ((r1->left[0]>=r2->left[0])&&(r1->right[0]<=r2->right[0])&&(r1->left[1]>=r2->left[1])&&(r1->right[1]<=r2->right[1]));
}
int
rects_intersect_boundary(Rect *r1, Rect *r2) {
  if(!rects_intersect(r1, r2))
    return 0;
  if(rect_inside(r1, r2))
    return 0;
  if(rect_inside(r2, r1))
    return 0;
  return 1;
}
void
Kdtree::get_cut_rects_bottom(Rect *r, Darr<Rect*> *bar, Darr<Rect*> *ar, Hash<Rect*, int> *table, Itype itype) {
	int i;
	for(i = 0; i<bar->n(); i++) {
		if((itype==OVLAP) && rects_intersect((Rect *) bar->get(i), r)) {
			int val;
			if(table->find(bar->get(i), val))
				continue;
			table->insert(bar->get(i), 1);
			ar->insert(bar->get(i));
			continue;
		}
		if((itype==BOUNDARY) && rects_intersect_boundary((Rect *) bar->get(i), r)) {
			int val;
			if(table->find(bar->get(i), val))
				continue;
			ar->insert(bar->get(i));
			continue;
		}
		if((itype==TOUCH) && rects_touch((Rect *) bar->get(i), r)) {
			int val;
			if(table->find(bar->get(i), val))
				continue;
			ar->insert(bar->get(i));
			continue;
		}
	}
}
int
Kdtree::is_rect_cut_bottom(Rect *r, Darr<Rect*> *bar, Itype itype) {
	int i;
	for(i = 0; i<bar->n(); i++) {
		if((itype==OVLAP) && rects_intersect((Rect *) bar->get(i), r))
			return 1;
		if((itype==BOUNDARY) && rects_intersect_boundary((Rect *) bar->get(i), r))
			return 1;
		if((itype==TOUCH) && rects_touch((Rect *) bar->get(i), r))
			return 1;
	}
	return 0;
}
void
Kdtree::get_cut_rects_rec(Rect *r, Node *nd, Darr<Rect*> *ar, Hash<Rect*, int> *table,  Itype itype) {
	if(nd == NULL)
		return;
	if(!rects_touch(r, &(nd->bbx)))
		return; 
        //leaf level rectangles
	if(nd->typ == C) {
		get_cut_rects_bottom(r, (nd->data).rectar, ar, table, itype);
		return;
	}
	Cuttype typ = nd->typ;
	int lo, hi;
	if(typ == X) {
		lo = r->left[0];
		hi = r->right[0];
	} else {
		lo = r->left[1];
		hi = r->right[1];
	}
	int cut = ((nd->data).data)->cut;
	if(lo<cut)
		get_cut_rects_rec(r, ((nd->data).data)->left, ar, table, itype);
	if(hi>cut)
		get_cut_rects_rec(r, ((nd->data).data)->right, ar, table, itype);
}

Node* Kdtree::find_parent_node_rec(Rect *r, Node *nd,Darr<Node*> *ndArr) {

	if(nd == NULL)
		return nd;
  // disjoint rectangles
	if(!rects_touch(r, &(nd->bbx)))
		return NULL; 
  //leaf level rectangles
	if(nd->typ == C) {
#if 0
  ads::notice(0," rect is (%d,%d),(%d,%d)\n",r->left[0],r->left[1],r->right[0],r->right[1]);
  ads::notice(0," node's bbx is (%d,%d),(%d,%d)\n",nd->bbx.left[0],nd->bbx.left[1],nd->bbx.right[0],nd->bbx.right[1]);
#endif
              ndArr->insert(nd);
		return nd;
	}
	Cuttype typ = nd->typ;
	int lo, hi;
	if(typ == X) {
		lo = r->left[0];
		hi = r->right[0];
	} else {
		lo = r->left[1];
		hi = r->right[1];
	}
#if 0
  ads::notice(0," node's bbx is (%d,%d),(%d,%d)\n",nd->bbx.left[0],nd->bbx.left[1],nd->bbx.right[0],nd->bbx.right[1]);
#endif
	int cut = ((nd->data).data)->cut;
  Node *ret = NULL;
#if 0
    ads::notice(0,"rectangle is on the left side\n");
#endif
		ret = find_parent_node_rec(r, ((nd->data).data)->left,ndArr);
#if 0
    ads::notice(0,"rectangle is on the right side\n");
#endif
		ret = find_parent_node_rec(r, ((nd->data).data)->right,ndArr);
  return ret;
}
/*-----------------------------------------------------------------
  Given the root node of the tree, find a node in the tree
  which is the parent of the given rectangle.
------------------------------------------------------------------*/
Node* Kdtree::find_parent_nodes(Rect *r,Darr<Node*> *ndArr) {
  Node *node;
  node = find_parent_node_rec(r, _node,ndArr);
  return node;
}

/*--------------------------------------------------
  Function to insert a rectangle in to the tree.
  This is meant for inserting leaf level rectangles,
  ie. rectangles whose width is <10000
  All this function does is that, it finds out the
  tree node that corresponds to this rectangle and
  insert this rectangle in to the rectar of that node
  This function does not add tree nodes
 ---------------------------------------------------*/
void Kdtree::insertRect(Rect *r) {
  Darr<Node *> *ndArr = new Darr<Node *>;
  Node *pnd;
  int i;
  find_parent_nodes(r,ndArr);
  ads::debug("KDTREE","a","tree before insert\n");
  for(i=0;i<ndArr->n();i++) {
     pnd = ndArr->get(i); 
  printNodeRec(_node);
  ads::debug("KDTREE","a","Inserting rect (%d,%d) (%d,%d) into bin (%d,%d) (%d,%d)\n",r->left[0],r->left[1],r->right[0],r->right[1],pnd->bbx.left[0],pnd->bbx.left[1],pnd->bbx.right[0],pnd->bbx.right[1]); 

  (pnd->data).rectar->insert(r);

  ads::debug("KDTREE","a","number of elements in array for this node %d\n",(pnd->data).rectar->n());
  }
  ads::debug("KDTREE","a","tree after insert\n");
   printNodeRec(_node);
  delete(ndArr);
}

/*--------------------------------------------------
  Function to remove a rectangle from the tree.
  This is meant for deleting leaf level rectangles,
  ie. rectangles whose width is <10000
  All this function does is that, it finds out the
  tree nodes that corresponds to this rectangle and
  delete this rectangle from the rectar of that node
  This function does not delete tree nodes
 ---------------------------------------------------*/
void Kdtree::removeRect(Rect *r) {
  Darr<Node *> *ndArr = new Darr<Node *>;
  Node *pnd;
  int i,k;
  int ind = -1;
  find_parent_nodes(r,ndArr);
  ads::debug("KDTREE","a","tree before delete\n");
  for(i=0;i<ndArr->n();i++) {
     pnd = ndArr->get(i); 
  ads::debug("KDTREE","a","deleting rect (%d,%d) (%d,%d) from bin (%d,%d) (%d,%d)\n",r->left[0],r->left[1],r->right[0],r->right[1],pnd->bbx.left[0],pnd->bbx.left[1],pnd->bbx.right[0],pnd->bbx.right[1]); 
    for(k=0;k<(pnd->data).rectar->n();k++) {
      Rect *r1 = (pnd->data).rectar->get(k);
      if( (r1->left[0] == r->left[0]) && (r1->right[0] == r->right[0]) && (r1->left[1] == r->left[1]) && (r1->right[1] == r->right[1]) ) {
       ind = k;
       break;
      }
    }
    if( ind >= 0 ) 
      (pnd->data).rectar->remove(k);
  ads::debug("KDTREE","a","number of elements in array for this node %d\n",(pnd->data).rectar->n());
  }
  ads::debug("KDTREE","a","tree after delete\n");
   printNodeRec(_node);
}




void
Kdtree::get_cut_rects(Rect *r, Darr<Rect*> *ar, Itype itype) {
	Hash<Rect*, int> table;
	get_cut_rects_rec(r, _node, ar, &table, itype);
}
int
Kdtree::is_rect_cut(Rect *r, Itype itype) {
	return is_rect_cut_rec(r, _node, itype);
}
int
Kdtree::is_rect_cut_rec(Rect *r, Node *nd, Itype itype) {
	if(nd->typ == C) {
		if(is_rect_cut_bottom(r, (nd->data).rectar, itype))
			return 1;
		return 0;
	}
	Cuttype typ = nd->typ;
	int lo, hi;
	if(typ == X) {
		lo = r->left[0];
		hi = r->right[0];
	} else {
		lo = r->left[1];
		hi = r->right[1];
	}
	int cut = ((nd->data).data)->cut;
	if(lo<cut) {
		if(is_rect_cut_rec(r, ((nd->data).data)->left, itype))
			return 1;
	}
	if(hi>cut) {
		if(is_rect_cut_rec(r, ((nd->data).data)->right, itype))
			return 1;
	}
	return 0;
}

