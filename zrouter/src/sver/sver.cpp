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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "db.h"
#include "sver.h"
#include "sver_names.h"

extern FILE * sver_parser_in;
extern int sver_parser_lineno;
extern int sver_parser_parse(void);

void sver_parser_error(char *s)
{
    ads::notice(0,"error at line %d,\n%s\n",sver_parser_lineno,s);
}

void sver_parser_unrec()
{
    ads::notice(0,"warning, unrecognized line %d\n",sver_parser_lineno);
}

BEGIN_NAMESPACE_ADS

extern void sver_parser_set_sver(sver *);

void sver::read(const char *file)
{
  FILE *fp = fopen(file,"r");
  if (!fp) {
    ads::notice(0,"cannot open %s\n",file);
    return;
  }
  sver_parser_set_sver(this);
  sver_parser_in = fp;
  sver_parser_lineno = 1;
  sver_parser_parse();
  fclose(fp);
  sver_parser_set_sver(NULL);

  set_default_top();

#if 0
  int nmod = 0;
  sver_module *mod;
  uint j;
  for (j=0;j<N_MODH;j++)
    for (mod = _modH[j]; mod; mod=mod->_next) nmod++;
  ads::notice(0,"%d modules\n",nmod);
  ads::notice(0,"strings %d kb\n",_names->get_used_kb());

  int idist[64];
  for (j=0;j<64;j++) idist[j] = 0;
  for (j=0;j<N_MODH;j++)
    for (mod = _modH[j]; mod; mod=mod->_next)
      idist[(mod->_instN < 64)? mod->_instN : 63] ++;
  for (j=0;j<63;j++) if (idist[j])
    ads::notice(0,"%d modules with instN = %d\n",idist[j],j);
  ads::notice(0,"%d modules with instN >= %d\n",idist[j],j);
#endif
}

void sver::set_default_top()
{
  int j;
  sver_module *mod;
  for (j=0;j<N_MODH;j++) {
    for (mod = _modH[j]; mod; mod=mod->_next) {
      if (mod->_ref == 0 && mod->_instN > 0) {
        _top = mod;
        break;
      }
    }
    if (mod) break;
  }
}

sver::sver()
{
  _names = new sver_names();
  int j;
  for (j=0;j<N_MODH;j++) _modH[j] = NULL;
  _top = NULL;
  _hi_ring_next = 0;
  _name_ring_next = 0;
  _name_ring2_next = 0;
  _cur_module = NULL;
  for (j=0;j<N_HI_RING;j++) {
    _name_ring[j].name = _name_ring_str + j*1024;
  }
}

sver::~sver()
{
  // ads::notice(0,"destroying sver\n");
}

void sver::start_module(const char *name)
{
  _cur_module = find_module(name);
  if (_cur_module) {
    _old_module = true;
  } else {
    _cur_module = create_module(name);
    _old_module = false;
  }
  return;
}

void sver::add_module_pin(const char *name)
{
  sver_term * t = _cur_module->find_or_create_term(name,_old_module,_names);
}

void sver::start_inst(const char *name,const char *module_name)
{
  // ads::notice(0,"inst %s master %s\n",name,module_name);
  sver_module *parent = _cur_module;
  _cur_inst_mod = find_or_create_module(module_name);
  _cur_inst_mod->_ref++;
  _old_inst_mod = (_cur_inst_mod->_terms != NULL);
  _cur_inst = new sver_inst;
  _cur_inst->name = _names->store(name);
  _cur_inst->module = _cur_inst_mod;
  _cur_inst->term_nets = NULL;
  parent->add_inst(_cur_inst);
}

void sver::add_inst_pin(const char *name,const char *net_name)
{
  if (name[0] != '.') {
    return;
  }
  const char *pin_name = name+1;
  // ads::notice(0,"  pin %s net %s\n",pin_name,net_name);
  sver_term *t = _cur_inst_mod->find_or_create_term(pin_name,_old_inst_mod,_names);
  
  // now add term_net to inst, and term_inst to net
  sver_inst *inst = _cur_inst;
  sver_net *net = _cur_module->find_or_create_net(net_name,_names);
  sver_term_inst *ti = new sver_term_inst;
  ti->term = t;
  ti->inst = inst;
  ti->next = net->term_insts;
  net->term_insts = ti;
  sver_term_net *tn = new sver_term_net;
  tn->term = t;
  tn->net = net;
  tn->next = inst->term_nets;
  inst->term_nets = tn;
}

void sver::stop_inst()
{
}

void sver::stop_module()
{
  // ads::notice(0,"stop module\n");
}

///////////////////////////////////////////////////////////////////

sver_hier_inst * sver::find_hier_inst(sver_name_list *nm,sver_module *top)
{
  // the hier_inst's are ring-alloced, so dont keep them for long
  if (!nm) return NULL;
  if (!top) top = get_top();
  sver_inst *inst = top->find_inst(nm->name);
  if (!inst) return NULL;
  sver_hier_inst *ret = new_hier_inst();
  sver_hier_inst *x = ret;
  x->inst = inst;
  while ((nm = nm->next)) {
    inst = inst->module->find_inst(nm->name);
    if (!inst) {
      // cannot descend
      return NULL;
    }
    x->next = new_hier_inst();
    x = x->next;
    x->inst = inst;
  }
  return ret;
}

sver_hier_pin * sver::find_hier_pin(sver_name_list *nm,sver_module *top)
{
  // use immediately
  sver_hier_pin *shp = &_tmp_hier_pin;
  if (!nm) return NULL;
  if (!top) top = get_top();
  if (!top) return NULL;
  if (!nm->next) {
    shp->hier_inst = NULL;
    shp->term = top->find_term(nm->name);
  } else {
    sver_inst *inst = top->find_inst(nm->name);
    if (!inst) return NULL;
    sver_hier_inst *x = shp->hier_inst = new_hier_inst();
    x->inst = top->find_inst(nm->name);
    if (!x->inst)
      return NULL;
    for (nm = nm->next; nm->next; nm = nm->next) {
      inst = inst->module->find_inst(nm->name);
      if (!inst) {
        return NULL;
      }
      x->next = new_hier_inst();
      x = x->next;
      x->inst = inst;
    }
    shp->term = inst->module->find_term(nm->name);
  }
  if (!shp->term)
    return NULL;
  return shp;
}

sver_hier_net * sver::find_hier_net(sver_name_list *nm,sver_module *top)
{
  // use immediately
  sver_hier_net *shn = &_tmp_hier_net;
  sver_name_list *nextlast = nm;
  sver_name_list *last = nextlast->next;
  if (!last) {
    shn->hier_inst = NULL;
    shn->net = top->find_net(nm->name);
  } else {
    while (last->next) { nextlast = last; last = last->next; }
    nextlast->next = NULL;
    shn->hier_inst = find_hier_inst(nm,top);
    nextlast->next = last;
    sver_hier_inst *x = shn->hier_inst;
    while (x->next) x = x->next;
    shn->net = x->inst->module->find_net(last->name);
  }

  return shn;
}

sver_net * sver::find_upper_net(sver_hier_inst *hi,sver_net *net,sver_hier_inst **at)
{
  // at is a pointer into hi
  sver_hier_inst *x, *y;
  y = hi; while (y->next) y = y->next;
  // check input : net should belong to module of last instance
  if (y->inst->module != net->module) {
    ads::notice(0,"error in find_upper_net\n");
    *at = NULL;
    return NULL;
  }
  *at = y;
  while (y != hi) {
    sver_term *t = get_mterm(net);
    if (! t) 
      break;  // normal
    sver_net *z = get_net(y->inst,t);
    if (!z)
      break;  // t is not connected above
    net = z;
    x = hi; while (x->next != y) x = x->next;
    *at = y = x;
  }
  return net;
}

sver_term * sver::get_mterm(sver_net *net)
{
  sver_term_inst *x;
  for (x = net->term_insts; x; x = x->next) 
    if (! x->inst)
      return (x->term);
  return NULL;
}
  
sver_net * sver::get_net(sver_inst *inst,sver_term *t)
{
  // return net connected to inst at term t
  sver_term_net *x;
  for (x = inst->term_nets; x; x = x->next) 
    if (x->term == t)
      return (x->net);
  return NULL;
}

///////////////////////////////////////////////////////////////////

static uint sver_hash_f(const char *name)
{
  register uint key = 0;
  const char *p;
  for (p=name; *p; p++)
    key = 17*key + (*p-'a');
  return key;
}

sver_module *sver::find_module(const char *name)
{
  sver_module *mod;
  uint key = sver_hash_f(name) % N_MODH;
  for (mod = _modH[key]; mod; mod = mod->_next) {
    if (!strcmp(mod->_name,name))
      return mod;
  }
  return NULL;
}

sver_module *sver::create_module(const char *name)
{
  sver_module *mod = new sver_module();
  mod->_name = _names->store(name);
  uint key = sver_hash_f(name) %  N_MODH;
  mod->_next = _modH[key];
  _modH[key] = mod;
  return mod;
}

sver_module *sver::find_or_create_module(const char *name)
{
  sver_module *mod;
  uint key = sver_hash_f(name) % N_MODH;
  for (mod = _modH[key]; mod; mod = mod->_next) {
    if (!strcmp(mod->_name,name))
      return mod;
  }
  mod = new sver_module();
  mod->_name = _names->store(name);
  mod->_next = _modH[key];
  _modH[key] = mod;
  return mod;
}

sver_hier_inst * sver::new_hier_inst()
{
  sver_hier_inst *hi = _hi_ring + _hi_ring_next;
  ++_hi_ring_next;
  if (_hi_ring_next == N_HI_RING) _hi_ring_next = 0;
  hi->next = NULL;
  return hi;
}

sver_name_list * sver::new_name_list()
{
  sver_name_list *nm = _name_ring + _name_ring_next;
  ++_name_ring_next;
  if (_name_ring_next == N_HI_RING) _name_ring_next = 0;
  return nm;
}

sver_name_list * sver::new_name_list_no_str()
{
  sver_name_list *nm = _name_ring2 + _name_ring2_next;
  ++_name_ring2_next;
  if (_name_ring2_next == N_HI_RING) _name_ring2_next = 0;
  return nm;
}

sver_module::sver_module()
{
  _name = NULL;
  _terms = NULL;
  _instN = 0;
  instH_n = 0;
  instH = NULL;
  instL = NULL;
  _netN = 0;
  netH_n = 0;
  netH = NULL;
  netL = NULL;
  _ref = 0;
}

sver_module::~sver_module()
{
  // not deleting name or insts here
  if (instH) free(instH);
}
  
void sver_module::add_inst(sver_inst *inst)
{
  ++_instN;
  if (instH_n) {
    uint key = sver_hash_f(inst->name) % instH_n;
    inst->next = instH[key]; instH[key] = inst;
    return;
  }
  inst->next = instL;
  instL = inst;
  if (_instN == 16) {
    instH_n = 32;
    instH = (sver_inst**)malloc(instH_n*sizeof(sver_inst*));
    uint key;
    for (key=0;key<instH_n;key++) instH[key] = NULL;
    sver_inst *next_inst = NULL;
    for (inst = instL; inst; inst = next_inst) {
      next_inst = inst->next;
      key = sver_hash_f(inst->name) % instH_n;
      inst->next = instH[key]; instH[key] = inst;
    }
    instL = NULL;
  }
}

sver_inst *sver_module::find_inst(const char *name)
{
  sver_inst *inst;
  if (instH) {
    uint key = sver_hash_f(name) % instH_n;
    inst = instH[key];
  } else {
    inst = instL;
  }
  for (; inst; inst = inst->next) {
    if (!strcmp(inst->name, name))
      return inst;
  }
  return NULL;
}

sver_net *sver_module::find_net(const char *name)
{
  sver_net *net;
  if (netH) {
    uint key = sver_hash_f(name) % netH_n;
    net = netH[key];
  } else {
    net = netL;
  }
  for (; net; net = net->next) {
    if (!strcmp(net->name, name))
      return net;
  }
  return NULL;
}

sver_term *sver_module::find_or_create_term(const char *name,bool old_mod,sver_names *names)
{
  sver_term *t = NULL;
  if (old_mod) {
    for (t = _terms; t; t = t->next)
      if (!strcmp(t->name, name)) break;
    // ads::notice(0,"warning, new pin for %s at %s\n",_name,name);
  }
  if (!t) {
    t = new sver_term;
    t->name = names->store(name);
    t->module = this;
    t->net = NULL;
    t->next = _terms;
    _terms = t;
  }
  return t;
}

sver_term *sver_module::find_term(const char *name)
{
  sver_term *t;
  for (t = _terms; t; t = t->next)
    if (!strcmp(t->name, name))
      return t;
  return NULL;
}


void sver_module::add_net(sver_net *net)
{
  ++_netN;
  if (netH_n) {
    uint key = sver_hash_f(net->name) % netH_n;
    net->next = netH[key]; netH[key] = net;
    return;
  }
  net->next = netL;
  netL = net;
  if (_netN == 16) {
    netH_n = 32;
    netH = (sver_net**)malloc(netH_n*sizeof(sver_net*));
    uint key;
    for (key=0;key<netH_n;key++) netH[key] = NULL;
    sver_net *next_net = NULL;
    for (net = netL; net; net = next_net) {
      next_net = net->next;
      key = sver_hash_f(net->name) % netH_n;
      net->next = netH[key]; netH[key] = net;
    }
    netL = NULL;
  }
}

sver_net *sver_module::find_or_create_net(const char *name,sver_names *names)
{
  sver_net *net;
  if (netH) {
    uint key = sver_hash_f(name) % netH_n;
    net = netH[key];
  } else {
    net = netL;
  }
  for (; net; net = net->next) {
    if (!strcmp(net->name, name))
      return net;
  }

  // net not found, creating new
  net = new sver_net;
  net->name = names->store(name);  // duplicates term name
  net->module = this;
  net->term_insts = NULL;
  add_net(net);

  // see if this new net matches a term
  sver_term *t;
  for (t = _terms; t; t=t->next) {
    if (!strcmp(t->name, name)) {
      t->net = net;
      sver_term_inst *ti = new sver_term_inst;
      ti->term = t;
      ti->inst = NULL;
      ti->next = NULL;
      net->term_insts = ti;
      break;
    }
  }

  return net;
}
  
///////////////////////////////////////////////////////


sver_name_list *sver::name_list(const char *name,const char delim)
{
  const char *p;
  char *q;
  sver_name_list *first,*last,*cur;
  first = NULL;
  p = name;
  while (*p) {
    cur = new_name_list();
    if (!first) first = cur;
    else last->next = cur;
    cur->next = NULL;
    q = cur->name;
    while (*p == '/') p++;
    while (*p && *p!='/') *(q++) = *(p++);
    *q = '\0';
    if (q == cur->name) {
      if (first==cur) return NULL;
      last->next = NULL; break;
    }
    last = cur;
  }
  return first;
}

const char *sver::name_list_str(sver_name_list *nm,const char delim,const char last_delim)
{
  name_list_print(_tmp_str,nm,delim,last_delim);
  return _tmp_str;
}

void sver::name_list_print(char *str,sver_name_list *nm,const char delim,const char last_delim)
{
  char *q = str;
  while (1) {
    const char *p = nm->name;
    while (*p) *(q++) = *(p++);
    nm = nm->next;
    if (!nm) break;
    if (nm->next) *(q++) = delim;
    else *(q++) = last_delim;
  }
  *q = '\0';
}

////////////////////////////////////////////////////////////////////

sver_name_list *sver::name(sver_hier_inst *hier_inst)
{
  sver_name_list *ret = new_name_list_no_str();
  sver_hier_inst *x = hier_inst;
  sver_name_list *y = ret;
  y->name = x->inst->name;
  for (x=x->next;x;x=x->next) {
    y->next = new_name_list_no_str();
    y = y->next;
    y->name = x->inst->name;
  }
  y->next = NULL;
  return ret;
}

sver_name_list *sver::name(sver_hier_net *hier_net)
{
  sver_name_list *ret = new_name_list_no_str();
  sver_hier_inst *x = hier_net->hier_inst;
  sver_name_list *y = ret;
  for (;x;x=x->next) {
    y->name = x->inst->name;
    y->next = new_name_list_no_str();
    y = y->next;
  }
  y->name = hier_net->net->name;
  y->next = NULL;
  return ret;
}

sver_name_list *sver::name(sver_hier_pin *hier_pin)
{
  sver_name_list *ret = new_name_list_no_str();
  sver_hier_inst *x = hier_pin->hier_inst;
  sver_name_list *y = ret;
  for (;x;x=x->next) {
    y->name = x->inst->name;
    y->next = new_name_list_no_str();
    y = y->next;
  }
  y->name = hier_pin->term->name;
  y->next = NULL;
  return ret;
}

////////////////////////////////////////////////////////////////////

sver_hier_pin *sver::get_any_leaf_pin(sver_hier_pin *hier_pin)
{
  sver_hier_pin *ret = &_tmp_hier_pin2;
  sver_hier_inst *x = ret->hier_inst = NULL;
  sver_hier_inst *y = hier_pin->hier_inst;
  if (y) {
    x = ret->hier_inst = new_hier_inst();
    x->inst = y->inst;
    while ((y=y->next)) {
      x = x->next = new_hier_inst();
      x->inst = y->inst;
    }
    x->next = NULL;
  }
  if (hier_pin->term->net == NULL) {
    // hier_pin is a leaf pin
    ret->term = hier_pin->term;
    return ret;
  }
  // descend from the pin
  sver_term *term = hier_pin->term;
  sver_net *net = term->net;
  while (net) {
    sver_term_inst *ti;
    for (ti = net->term_insts; ti; ti = ti->next)
      if (ti->inst) break;
    if (!ti)
      break;
    if (x)
      x = x->next = new_hier_inst();
    else
      x = ret->hier_inst = new_hier_inst();
    x->inst = ti->inst;
    // from ti->inst and ti->term get internal net
    term = ti->term;
    net = term->net;
  }
  ret->term = term;
  return ret;
}

END_NAMESPACE_ADS
