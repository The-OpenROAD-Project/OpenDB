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

/*----------------------------------------------------------------
/
/////   AUTHOR: SANJEEV MAHAJAN
_________________________________________________________________*/

#include "dgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "logger.h"

Dgraph::Dgraph(int n)
{
  dist   = (double*) malloc(n * sizeof(double));
  _cycle = (int*) malloc(n * sizeof(int));
  _pred  = (int*) malloc(n * sizeof(int));
  _prede = (Dedge**) malloc(n * sizeof(Dedge*));
  _upper = (double*) malloc(n * sizeof(double));
  _vis   = (int*) malloc(n * sizeof(int));
  int i;
  for (i = 0; i < n; i++)
    _upper[i] = 1e99;
  _n = n;
}
Dgraph::~Dgraph()
{
  if (!_n)
    return;
  free(dist);
  free(_cycle);
  free(_pred);
  free(_prede);
  free(_vis);
  _neighbors.begin();
  int                i;
  Hash<Dedge*, int>* nei;
  while (_neighbors.next(i, nei)) {
    if (nei)
      delete nei;
  }
  for (i = 0; i < cycles.n(); i++)
    delete cycles.get(i);
}
Dedge* Dgraph::add_edge(int s, int d, double wt, bool is_hold, bool preserve)
{
  Dedge* e = (Dedge*) malloc(sizeof(Dedge));
  if (s >= _n) {
    printf("Cannot add this edge, as source is > %d\n", _n);
    return NULL;
  }
  if (d >= _n) {
    printf("Cannot add this edge, as destination is > %d\n", _n);
    return NULL;
  }

  e->s  = s;
  e->d  = d;
  e->wt = wt;
  if (wt > 0 && preserve)
    e->wt = 0;
  e->preserve = preserve;
  e->cwt      = wt;
  e->is_hold  = is_hold;
  Hash<Dedge*, int>* nei;
  if (!_neighbors.find(s, nei)) {
    nei = new Hash<Dedge*, int>;
    _neighbors.insert(s, nei);
  }
  nei->insert(e, 1);
  return NULL;
}
void Dgraph::remove_edge(Dedge* e)
{
  int                v;
  Hash<Dedge*, int>* nei;
  if (!_neighbors.find(e->s, nei)) {
    printf(
        "Edge %d-%d does not exist in the graph. Cannot remove\n", e->s, e->d);
    return;
  }
  nei->remove(e, v);
}
void Dgraph::remove_negative_edges()
{
  Darr<Dedge*> negative;
  Dedge*       e = NULL;
  int          i;
  for (i = 0; i < _n; i++) {
    if (!neighbors_begin(i))
      continue;
    while (next_neighbor(e)) {
      if (e->wt < 0)
        negative.insert(e);
    }
  }
  for (i = 0; i < negative.n(); i++)
    remove_edge(negative.get(i));
}
int Dgraph::neighbors_begin(int i)
{
  if (!_neighbors.find(i, _curnei))
    return 0;
  _curnei->begin();
  return 1;
}
bool Dgraph::next_neighbor(Dedge*& e)
{
  int i;
  return _curnei->next(e, i);
}
void Dgraph::modify(int i, double delta)
{
  Hash<int, int>       marked;
  Hash<Dedge*, double> sl;
  int                  l = 0;
  while (_pred[i] != i && delta > 0.001) {
    int    k   = i;
    int    n   = 0;
    double sum = 0;
    sl.clear();
    while (_pred[k] != k) {
      int vvv;
      if (!(_prede[k]->preserve) && !marked.find(k, vvv)) {
        Dedge* e = _prede[k];
        sum += e->wt - e->cwt;
        // sum += dist[e->d]-dist[e->s]-e->cwt;
        n++;
      }
      k = _pred[k];
    }
    if (n == 0)
      break;
    double a = (sum - delta) / n;
    k        = i;
    int nm   = 0;
    while (_pred[k] != k) {
      int vvv;
      if (_prede[k]->preserve || marked.find(k, vvv)) {
        k = _pred[k];
        continue;
      }
      Dedge* e = _prede[k];
      if (e->wt - e->cwt < a) {
        // if(dist[e->d]-dist[e->s]-e->cwt<a) {
        nm++;
        marked.insert(k, 1);
      } else
        sl.insert(e, e->wt - e->cwt - a);
      // sl.insert(e, dist[e->d]-dist[e->s]-e->cwt-a);
      k = _pred[k];
    }
    double vul;
    Dedge* e = _prede[i];
    if (!nm && sl.find(e, vul)) {
      double upe = _upper[i];
      if (l > 0)
        upe = 0;
      if (vul > 0 && e->wt - vul > upe + 0.001) {
        delta -= e->wt;
        e->wt = upe;
        i     = _pred[i];
        l++;
        continue;
      }
    }
    if (!nm)
      break;
  }
  double s;
  Dedge* e;
  sl.begin();
  while (sl.next(e, s))
    e->wt -= s;
}
bool Dgraph::relax(Dedge* e, bool* rest)
{
  if (dist[e->d] < dist[e->s] + e->wt - 1e-7) {
    _pred[e->d]  = e->s;
    _prede[e->d] = e;
    dist[e->d]   = dist[e->s] + e->wt;
    if (rest && dist[e->d] > _upper[e->d] * (1 + 1e-3) + 0.001)
      *rest = true;
    return true;
  }
  return false;
}
bool Dgraph::uppers_met()
{
  int i;
  for (i = 0; i < _n; i++) {
    if (dist[i] > _upper[i] + 1e-7)
      return false;
  }
  return true;
}
void Dgraph::set_upper_bound(int i, double j)
{
  _upper[i] = j;
}
double Dgraph::get_upper_bound(int i)
{
  return _upper[i];
}
bool Dgraph::has_cycle()
{
  int i;
  for (i = 0; i < _n; i++) {
    _cycle[i] = 0;
    _vis[i]   = 0;
  }
  for (i = 0; i < _n; i++) {
    if (_vis[i])
      continue;
    if (_pred[i] == i)
      continue;
    int val = dfs(i);
    if (!val)
      continue;
    printf("cycle found!\n");
    int           k   = val - 1;
    double        wt  = 0;
    int           n   = 0;
    Darr<Dedge*>* dar = new Darr<Dedge*>;
    cycles.insert(dar);
    while (_pred[k] != val - 1) {
      assert(_pred[k] != k);
      Dedge* e = _prede[k];
      dar->insert(e);
      if (!(e->preserve) || e->cwt < 0) {
        wt += _prede[k]->cwt;
        if (!(e->preserve))
          n++;
      }
      k = _pred[k];
    }
    Dedge* e = _prede[k];
    dar->insert(e);
    if (!(e->preserve || e->cwt < 0)) {
      wt += _prede[k]->cwt;
      if (!(e->preserve))
        n++;
    }
    if (n == 0)
      return true;
    printf("The weight is %g\n", wt);
    assert(wt > 0);
    double res = -wt / n;
    residues.insert(res);
    printf("The residue is %g\n", res);
    k = val - 1;
    while (_pred[k] != val - 1) {
      if (!(_prede[k]->preserve)) {
        double w = _prede[k]->cwt + res;
        if (w < _prede[k]->wt)
          _prede[k]->wt = w;
      }
      k = _pred[k];
    }
    double w = _prede[k]->cwt + res;
    if (!_prede[k]->preserve && w < _prede[k]->wt)
      _prede[k]->wt = w;
    return true;
  }
  return false;
}
void Dgraph::init()
{
  assert(_topo_sorted.n() == _n);
  int i;
  for (i = 0; i < _n; i++) {
    dist[i]   = 0;
    _pred[i]  = i;
    _prede[i] = NULL;
  }
}
void Dgraph::get_slacks()
{
  int    i;
  double wsb = 1e99, wsa = 1e99;
  double tnsb = 0, tnsa = 0;
  for (i = 0; i < _n; i++) {
    if (!neighbors_begin(i))
      continue;
    Dedge* e;
    while (next_neighbor(e)) {
      double sl = dist[e->d] - dist[e->s] - e->cwt;
      printf("The slack of edge %d-%d is %g\n", e->s, e->d, sl);
      if (wsb > -e->cwt)
        wsb = -e->cwt;
      if (wsa > sl)
        wsa = sl;
      if (e->cwt > 1e-7)
        tnsb -= e->cwt;
      if (sl < -1e-7)
        tnsa += sl;
    }
  }
  printf("The worst slack before is %g and after is %g\n", wsb, wsa);
  printf("The tns before is %g and after is %g\n", tnsb, tnsa);
}
bool Dgraph::bellmanford(bool* rest)
{
  int j;
  int i;
  init();
  Dedge* e     = NULL;
  double delta = 0;
  if (rest)
    *rest = false;
  for (j = _n - 1; j >= 0; j--) {
    bool changed = false;
    for (i = _n - 1; i >= 0; i--) {
      int k = _topo_sorted.get(i);
      if (!neighbors_begin(k))
        continue;
      while (next_neighbor(e)) {
        if (rest)
          *rest = false;
        if (relax(e, rest)) {
          changed = true;
          if (rest && *rest) {
            delta = dist[e->d] - _upper[e->d];
            assert(delta > 0);
            modify(e->d, delta);
            return false;
          }
        }
      }
    }
    if (!changed) {
      return true;
    }
    if (has_cycle()) {
      return false;
    }
  }
  return true;
}
bool Dgraph::find_longest_path()
{
  int i;
  init();
  for (i = _n - 1; i >= 0; i--) {
    int k = _topo_sorted.get(i);
    if (!neighbors_begin(k))
      continue;
    Dedge* e = NULL;
    while (next_neighbor(e)) {
      relax(e);
    }
  }
  return true;
}
int Dgraph::dfs(int i)
{
  if (_cycle[i])
    return i + 1;
  if (_vis[i])
    return 0;
  _vis[i] = 1;
  if (_pred[i] == i)
    return 0;
  _cycle[i] = 1;
  int val   = dfs(_pred[i]);
  _cycle[i] = 0;
  return val;
}
bool Dgraph::Dfs(int i)
{
  if (_cycle[i]) {
    printf("Has a cycle\n");
    return true;
  }
  if (_vis[i])
    return false;
  _vis[i] = 1;
  Hash<Dedge*, int>* ce;
  if (!_neighbors.find(i, ce)) {
    _topo_sorted.insert(i);
    return false;
  }
  ce->begin();
  _cycle[i]  = 1;
  Dedge* e   = NULL;
  int    val = false;
  int    vvv;
  while (ce->next(e, vvv)) {
    if (e->wt < 0)
      continue;
    if (Dfs(e->d))
      val = true;
  }
  _topo_sorted.insert(i);
  _cycle[i] = 0;
  return val;
}
int Dgraph::n()
{
  return _n;
}
bool Dgraph::topo_sort()
{
  int i;
  int val = false;
  _topo_sorted.reset();
  for (i = 0; i < _n; i++) {
    _vis[i]   = 0;
    _cycle[i] = 0;
  }
  for (i = 0; i < _n; i++) {
    if (_vis[i])
      continue;
    if (Dfs(i))
      val = true;
  }
  return val;
}
