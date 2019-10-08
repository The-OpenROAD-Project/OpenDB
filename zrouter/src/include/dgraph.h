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

/*----------------------------------------------------------------
/
/////   AUTHOR: SANJEEV MAHAJAN
_________________________________________________________________*/

#ifndef _DGRAPH_H
#define _DGRAPH_H
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#define MAXIT 1000
class Dedge {
  public:
         int s;
         int d;
         double wt;
	 double cwt;
         bool is_hold;
         bool preserve;
}; 
class Dgraph {
  public:
         Dgraph(int n = 0);
         ~Dgraph();
         Dedge* add_edge(int source, int dest, double weight, bool is_hold, bool preserve);
         void remove_edge(Dedge *edge); 
         int neighbors_begin(int v);
         bool next_neighbor(Dedge* &e);
         void init();
         bool relax(Dedge *e, bool *restr = NULL);
         void modify(int i, double delta); 
         bool has_cycle();
         bool bellmanford(bool *rest = NULL);
         int dfs(int i); 
         bool Dfs(int i);
         int n();
         bool topo_sort();
         bool find_longest_path();
         void remove_negative_edges();
         double *dist;
         void set_upper_bound(int i, double j); 
         bool uppers_met();
         double get_upper_bound(int i);
         Darr<int> _topo_sorted;
         void get_slacks();
         Darr<Darr<Dedge*>*> cycles;
         Darr<double> residues;
  private:
         int _n;   
         int *_vis;
         int *_cycle;
         int *_pred;
         double *_upper;
         Dedge** _prede;
         Darr<Dedge*> _cycle_edges;
         Hash<int, Hash<Dedge*, int>*> _neighbors;
         Hash<Dedge*, int> *_curnei;
         Darr<int> sources;
         
};
#endif
