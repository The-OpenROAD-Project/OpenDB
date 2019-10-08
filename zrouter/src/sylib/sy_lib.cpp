#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sy_lib.h"
#include "liberty_structs.h"
#include "sy_pi.h"

BEGIN_NAMESPACE_ADS

sy_template_enum sy_template_enum_lookup(char *name)
{
 if (!strcmp(name,"input_net_transition")) return  SY_input_net_transition;
 if (!strcmp(name,"total_output_net_capacitance")) return  SY_total_output_net_capacitance;
 if (!strcmp(name,"output_net_length")) return  SY_output_net_length;
 if (!strcmp(name,"output_net_wire_cap")) return  SY_output_net_wire_cap;
 if (!strcmp(name,"output_net_pin_cap")) return  SY_output_net_pin_cap;
 if (!strcmp(name,"constrained_pin_transition")) return  SY_constrained_pin_transition;
 if (!strcmp(name,"related_pin_transition")) return  SY_related_pin_transition;
 if (!strcmp(name,"connect_delay")) return  SY_connect_delay;
 if (!strcmp(name,"output_pin_transition")) return  SY_output_pin_transition;
 if (!strcmp(name,"related_out_total_output_net_capacitance")) return  SY_related_out_total_output_net_capacitance;
 if (!strcmp(name,"related_out_output_net_length")) return  SY_related_out_output_net_length;
 if (!strcmp(name,"related_out_output_net_wire_cap")) return  SY_related_out_output_net_wire_cap;
 if (!strcmp(name,"related_out_output_net_pin_cap")) return  SY_related_out_output_net_pin_cap;
 if (!strcmp(name,"related_output_pin_capacitance")) return  SY_related_output_pin_capacitance;
 if (!strcmp(name,"fanout_number")) return  SY_fanout_number;
 if (!strcmp(name,"fanout_pin_capacitance")) return  SY_fanout_pin_capacitance;
 if (!strcmp(name,"driver_slew")) return  SY_driver_slew;
 if (!strcmp(name,"rc_product")) return  SY_rc_product;
 return SY_template_none;
}

char *sy_template_enum_str(sy_template_enum ex)
{
 if (ex==SY_template_none) return "template_none";
 if (ex==SY_input_net_transition) return "input_net_transition";
 if (ex==SY_total_output_net_capacitance) return "total_output_net_capacitance";
 if (ex==SY_output_net_length) return "output_net_length";
 if (ex==SY_output_net_wire_cap) return "output_net_wire_cap";
 if (ex==SY_output_net_pin_cap) return "output_net_pin_cap";
 if (ex==SY_constrained_pin_transition) return "constrained_pin_transition";
 if (ex==SY_related_pin_transition) return "related_pin_transition";
 if (ex==SY_connect_delay) return "connect_delay";
 if (ex==SY_output_pin_transition) return "output_pin_transition";
 if (ex==SY_related_out_total_output_net_capacitance) return "related_out_total_output_net_capacitance";
 if (ex==SY_related_out_output_net_length) return "related_out_output_net_length";
 if (ex==SY_related_out_output_net_wire_cap) return "related_out_output_net_wire_cap";
 if (ex==SY_related_out_output_net_pin_cap) return "related_out_output_net_pin_cap";
 if (ex==SY_related_output_pin_capacitance) return "related_output_pin_capacitance";
 if (ex==SY_fanout_number) return "fanout_number";
 if (ex==SY_fanout_pin_capacitance) return "fanout_pin_capacitance";
 if (ex==SY_driver_slew) return "driver_slew";
 if (ex==SY_rc_product) return "rc_product";
 return "";
}

#define MAL(tt) (tt*)malloc(sizeof(tt))

sy_library *sy_library_create(char *name)
{
  sy_library *lib = MAL(sy_library);
  lib->name = strdup(name);
  lib->tm_list = NULL;
  lib->bt_list = NULL;
  lib->cell_list = NULL;
  lib->next = NULL;
  lib->sta_lib = NULL;
  lib->slew_hi_thresh_rise = 0.8;
  lib->slew_hi_thresh_fall = 0.8;
  lib->slew_lo_thresh_rise = 0.2;
  lib->slew_lo_thresh_fall = 0.2;
  lib->slew_derate = 1.0;
  lib->slew_thresh_set = 0;
  return lib;
}

sy_template *sy_template_create0(char *name)
{
  sy_template *tm = MAL(sy_template);
  tm->name = strdup(name);
  tm->three_dim = 0;
  tm->ex = SY_template_none;
  tm->nx = 0;
  tm->vx = NULL;
  tm->ey = SY_template_none;
  tm->ny = 0;
  tm->vy = NULL;
  tm->next = NULL;
  tm->copies = NULL;
  return tm;
}

sy_template *sy_template_create(sy_library *lib,char *name)
{
  sy_template *tm = sy_template_create0(name);
  tm->next = lib->tm_list; lib->tm_list = tm;
  tm->copies = NULL;
  return tm;
}

sy_bustype *sy_bustype_create(sy_library *lib,char *name)
{
  sy_bustype *bt = MAL(sy_bustype);
  bt->name = strdup(name);
  bt->b0 = 0;
  bt->b1 = 0;
  bt->next = NULL;
  bt->next = lib->bt_list; lib->bt_list = bt;
  return bt;
}

void sy_template_add_index(sy_template *tm,int index,double *x,int n)
{
  int j;
  if (index==1) {
    tm->nx = n;
    tm->vx = (double *)malloc(n*sizeof(double));
    for (j=0;j<n;j++) tm->vx[j] = x[j];
  }
  else if (index==2) {
    tm->ny = n;
    tm->vy = (double *)malloc(n*sizeof(double));
    for (j=0;j<n;j++) tm->vy[j] = x[j];
  } else if (index==3) {
    tm->three_dim = 1;
  }
  return;
}

sy_template *sy_add_normal_template(sy_library *lib,char *name,int cn,double *cv,int sn,double *sv)
{
  sy_template *tm = sy_template_create(lib,name);
  sy_template_add_index(tm,1,cv,cn);
  tm->ex = SY_total_output_net_capacitance;
  sy_template_add_index(tm,2,sv,sn);
  tm->ey = SY_input_net_transition;
  return tm;
}


sy_template *sy_find_or_add_normal_template(sy_library *lib,int cn,double *cv,int sn,double *sv, int cnt)
{
  //int cnt = 0;
  char name[256];
  sy_template *tm;
  //for (tm = lib->tm_list; tm; tm=tm->next) cnt++;
  sprintf(name,"tm%d",cnt);
// TODO: find
  tm = sy_add_normal_template(lib,name,cn,cv,sn,sv);
  return tm; 
}

sy_template *sy_template_copy(sy_library *lib,sy_template *tdummy,int lineno)
{
  char name[256];
  sprintf(name,"%s_%d",tdummy->name,lineno);
  sy_template *tm;
#if 0
  for (tm = lib->tm_list; tm; tm=tm->next)
    if (!strcmp(name,tm->name)) break;
  if (tm) {
    notice(0,"error, duplicate name, sy_template_copy\n");
  }
#endif
  tm = sy_template_create0(name);
  tm->ex = tdummy->ex;
  tm->ey = tdummy->ey;
  tm->nx = 0;
  tm->ny = 0;
  tm->next = tdummy->copies;
  tdummy->copies = tm;
  return tm;
}

sy_cell *sy_cell_create(sy_library *lib,const char *name)
{
  sy_cell *c = MAL(sy_cell);
  c->name = strdup(name);
  c->master = NULL;
  c->lib = lib;
  c->pin = NULL;
  c->table = NULL;
  c->next = lib->cell_list; lib->cell_list = c;
  c->sta_cell = NULL;
  c->dont_use = c->dont_touch = c->sequential = false;
  c->power = 0;
  c->feq = NULL;
  c->neq = NULL;
  c->strength = -1.0;
  c->fid = 0;
  return c;
}

sy_pin *sy_pin_create(sy_cell *cell,const char *name)
{
  // find or create
  sy_pin *pin;
  for (pin = cell->pin; pin; pin=pin->next)
    if (!strcmp(name,pin->name)) return pin;
  pin = MAL(sy_pin);
  pin->name = strdup(name);
  pin->mterm = NULL;
  pin->from_pair = NULL;
  pin->to_pair = NULL;
  pin->dir = 0;
  pin->func = NULL;
  pin->maxcap = 0.0;
  pin->cap = 0.0;
  pin->next = cell->pin; cell->pin = pin;
  pin->sta_port = NULL;
  return pin;
}

sy_pin_pair *sy_pin_pair_create(sy_pin *from,sy_pin *to)
{
  // find or create
  sy_pin_pair *x = from->from_pair;
  while (x && x->to != to) x = x->next_for_from;
  if (x) return x;
  x = MAL(sy_pin_pair);
  x->from = from;
  x->to = to;
  x->normal = NULL;
  x->preset = NULL;
  x->setup = NULL;
  x->hold = NULL;
  x->next_for_from = from->from_pair;
  from->from_pair = x;
  x->next_for_to = to->to_pair;
  to->to_pair = x;
  return x;
}

sy_table *sy_table_create(sy_template *tm)
{
  int j;
  sy_table *tbl = MAL(sy_table);
  tbl->tm = tm;
  if (tm == NULL) {
    // scalar
    tbl->t = (double**)malloc(sizeof(double*));
    tbl->t[0] = (double*)malloc(sizeof(double));
  } else if (tm->ny==0) {
    // one-dim
    tbl->t = (double**)malloc(sizeof(double*));
    tbl->t[0] = (double*)malloc(tm->nx*sizeof(double));
  } else {
    // two-dim
    tbl->t = (double**)malloc(tm->nx*sizeof(double*));
    for (j=0;j<tm->nx;j++)
      tbl->t[j] = (double*)malloc(tm->ny*sizeof(double));
  }
  tbl->ddx = 0.0;
  tbl->ddy = 0.0;
  return tbl;
}

void sy_table_destroy(sy_table *tbl)
{
  if (tbl->tm && tbl->t) {
    int j,n;
    n =  (tbl->tm->ny ? tbl->tm->nx : 1);
    for (j=0;j<n;j++) free(tbl->t[j]);
    free(tbl->t);
  }
  free(tbl);
}

void sy_table_replace_template(sy_table *tbl,sy_template *new_tm)
{
  int j,n;
  n = (tbl->tm->ny ? tbl->tm->nx : 1);
  for (j=0;j<n;j++) free(tbl->t[j]);
  free(tbl->t);
  tbl->tm = new_tm;
  if (new_tm->ny==0) {
    tbl->t = (double**)malloc(sizeof(double*));
    tbl->t[0] = (double*)malloc(new_tm->nx*sizeof(double));
  } else {
    tbl->t = (double**)malloc(new_tm->nx*sizeof(double*));
    for (j=0;j<new_tm->nx;j++)
      tbl->t[j] = (double*)malloc(new_tm->ny*sizeof(double));
  }
}

sy_tab8 *sy_tab8_create(void)
{
  sy_tab8 *tab8 = MAL(sy_tab8);
  tab8->tab_d[0][0] = NULL;
  tab8->tab_d[0][1] = NULL;
  tab8->tab_d[1][0] = NULL;
  tab8->tab_d[1][1] = NULL;
  tab8->tab_s[0][0] = NULL;
  tab8->tab_s[0][1] = NULL;
  tab8->tab_s[1][0] = NULL;
  tab8->tab_s[1][1] = NULL;
  tab8->next = NULL;
  return tab8;
}

/***************************************************************************************/

double sy_interp2(double **t,double *vx,int nx,double *vy,int ny,double x,double y)
{
  int jx,jy;
  double ax,ay,val0,val1,val;
  double *tx0,*tx1;
  
  for (jx=0;jx<nx;jx++) if (x<vx[jx]) break;
  for (jy=0;jy<ny;jy++) if (y<vy[jy]) break;
  if (jx>0 && jx<nx && jy>0 && jy<ny) {
    ax = (x-vx[jx-1])/(vx[jx]-vx[jx-1]);
    ay = (y-vy[jy-1])/(vy[jy]-vy[jy-1]);
    tx0 = t[jx-1];
    tx1 = t[jx];
    val0 = tx0[jy-1]+ay*(tx0[jy]-tx0[jy-1]);
    val1 = tx1[jy-1]+ay*(tx1[jy]-tx1[jy-1]);
    val = val0 + ax*(val1-val0);
  } else if (jy>0 && jy<ny) {
    if (jx) --jx;
    tx0 = t[jx];
    ay = (y-vy[jy-1])/(vy[jy]-vy[jy-1]);
    val = tx0[jy-1]+ay*(tx0[jy]-tx0[jy-1]);
  } else if (jx>0 && jx<nx) {
    if (jy) --jy;
    val0 = t[jx-1][jy];
    val1 = t[jx][jy];
    ax = (x-vx[jx-1])/(vx[jx]-vx[jx-1]);
    val = val0 + ax*(val1-val0);
  } else {
    if (jx) --jx;
    if (jy) --jy;
    val = t[jx][jy];
  }
  return val;
}

double sy_interp2sc(double **t,double *vx,int nx,double *vy,int ny,double x,double y)
{
  int jx,jy;
  double ax,ay,val0,val1,val;
  double *tx0,*tx1;

  for (jx=1;jx<nx-1;jx++) if (x<vx[jx]) break;
  for (jy=1;jy<ny-1;jy++) if (y<vy[jy]) break;

  ax = (x-vx[jx-1])/(vx[jx]-vx[jx-1]);
  ay = (y-vy[jy-1])/(vy[jy]-vy[jy-1]);

  // forbid extrap in s
  if (ax>1.0) ax=1.0;  // or, ++jx and ax=0

  tx0 = t[jx-1];
  tx1 = t[jx];
  val0 = tx0[jy-1]+ay*(tx0[jy]-tx0[jy-1]);
  val1 = tx1[jy-1]+ay*(tx1[jy]-tx1[jy-1]);
  val = val0 + ax*(val1-val0);
  if (val < 0.0) val = 0.0;

  return val;
}

double sy_interp2cs(double **t,double *vx,int nx,double *vy,int ny,double x,double y)
{
  int jx,jy;
  double ax,ay,val0,val1,val;
  double *tx0,*tx1;

  for (jx=1;jx<nx-1;jx++) if (x<vx[jx]) break;
  for (jy=1;jy<ny-1;jy++) if (y<vy[jy]) break;

  ax = (x-vx[jx-1])/(vx[jx]-vx[jx-1]);
  ay = (y-vy[jy-1])/(vy[jy]-vy[jy-1]);

  // forbid extrap in s
  if (ay>1.0) ay=1.0;  // or, ++jy and ay=0

  tx0 = t[jx-1];
  tx1 = t[jx];
  val0 = tx0[jy-1]+ay*(tx0[jy]-tx0[jy-1]);
  val1 = tx1[jy-1]+ay*(tx1[jy]-tx1[jy-1]);
  val = val0 + ax*(val1-val0);
  if (val < 0.0) val = 0.0;

  return val;
}



double sy_interp1(double *t,double *vx,int nx,double x)
{
  int jx;
  double ax,val;
  for (jx=0;jx<nx;jx++) if (x<vx[jx]) break;
  if (jx>0 && jx<nx) {
    ax = (x-vx[jx-1])/(vx[jx]-vx[jx-1]);
    val = vx[jx-1]+ax*(vx[jx]-vx[jx-1]);
  } else {
    val = jx?vx[jx-1]:(vx[0]);
  }
  return val;
}

void sy_table_cap_range(sy_table *tab,double *cmin,double *cmax)
{
  *cmin = *cmax = 0.0;
  sy_template *tm = tab->tm;
  if (! tm) {
    *cmin = 0.0; *cmax = 1e3;
  } else if (tm->ex==SY_total_output_net_capacitance) {
    *cmin = tm->vx[0];
    *cmax = tm->vx[tm->nx-1];
  } else if (tm->ey==SY_total_output_net_capacitance) {
    *cmin = tm->vy[0];
    *cmax = tm->vy[tm->ny-1];
  }
}
  

double sy_table_cap_slew(sy_table *tab,double cap,double in_slew)
{
  sy_template *tm = tab->tm;
  double val;
  if (!tm) {
    val = tab->t[0][0];
  } else if (tm->ex==SY_total_output_net_capacitance && tm->ey==SY_input_net_transition) {
    val = sy_interp2cs(tab->t,tm->vx,tm->nx,tm->vy,tm->ny,cap,in_slew);
#if 0
    val = sy_interp2(tab->t,tm->vx,tm->nx,tm->vy,tm->ny,cap,in_slew);
    cap1 = tm->vx[tm->nx-1];
    if (cap > cap1)
      val += tab->ddx*(cap-cap1);
#endif
  }
  else if (tm->ey==SY_total_output_net_capacitance && tm->ex==SY_input_net_transition)
    val = sy_interp2sc(tab->t,tm->vx,tm->nx,tm->vy,tm->ny,in_slew,cap);
#if 0
    val = sy_interp2(tab->t,tm->vx,tm->nx,tm->vy,tm->ny,in_slew,cap);
#endif
  else if (tm->ny==0 && tm->ex==SY_total_output_net_capacitance)
    val = sy_interp1(tab->t[0],tm->vx,tm->nx,cap);
  else if (tm->ny==0 && tm->ex==SY_input_net_transition)
    val = sy_interp1(tab->t[0],tm->vx,tm->nx,in_slew);
  else {
    val = 0.0;
  }
  return val;
}

int sy_normal_eval(sy_tab8 *x,int rf_in,int rf_out,double cap,double in_slew,double *delay,double *slew)
{
  sy_table *td = x->tab_d[rf_in][rf_out];
  sy_table *ts = x->tab_s[rf_in][rf_out];
  if (!td || !ts) return 0;
  *delay = sy_table_cap_slew(td,cap,in_slew);
  *slew  = sy_table_cap_slew(ts,cap,in_slew);
  return 1;
}

sy_tab8 *sy_get_normal_table(sy_pin *from,sy_pin *to)
{
  sy_pin_pair *x = from->from_pair;
  while (x && x->to != to) x = x->next_for_from;
  if (x) return x->normal;
  return NULL;
}

int sy_template_cap_slew(sy_template *tm,int *nc,double *cv,int *ns,double *sv)
{
  int j;
  if (tm->ex==SY_total_output_net_capacitance && tm->ey==SY_input_net_transition) {
    *nc = tm->nx;
    for (j=0;j<tm->nx;j++) cv[j] = tm->vx[j];
    *ns = tm->ny;
    for (j=0;j<tm->ny;j++) sv[j] = tm->vy[j];
  }
  else if (tm->ey==SY_total_output_net_capacitance && tm->ex==SY_input_net_transition) {
    *nc = tm->ny;
    for (j=0;j<tm->ny;j++) cv[j] = tm->vy[j];
    *ns = tm->nx;
    for (j=0;j<tm->nx;j++) sv[j] = tm->vx[j];
  }
  else if (tm->ny==0 && tm->ex==SY_total_output_net_capacitance) {
    *nc = tm->nx;
    for (j=0;j<tm->nx;j++) cv[j] = tm->vx[j];
    *ns = 1;
    sv[0] = 0.1;
  }
  else if (tm->ny==0 && tm->ex==SY_input_net_transition) {
    *ns = tm->nx;
    for (j=0;j<tm->nx;j++) sv[j] = tm->vx[j];
    *nc = 1;
    cv[0] = 0.1;
  } else {
    return 0;
  }
  return 1;
}

int sy_template_equal(sy_template *tm,sy_template *tm2)
{
  int j;
  if (tm->ex != tm2->ex) return 0;
  if (tm->nx != tm2->nx) return 0;
  if (tm->ey != tm2->ey) return 0;
  if (tm->ny != tm2->ny) return 0;
  for (j=0;j<tm->nx;j++) if (tm->vx[j] != tm2->vx[j]) return 0;
  for (j=0;j<tm->ny;j++) if (tm->vy[j] != tm2->vy[j]) return 0;
  return 1;
}


/***************************************************************************************/

static sy_library *sy_lib_max = NULL;
static sy_library *sy_lib_min = NULL;

void sy_add_lib(int min_max, sy_library *lib)
{
  sy_library *x;
  if (min_max) {
    if (!sy_lib_max) {
      sy_lib_max = lib;
    } else {
      x = sy_lib_max;
      while (x->next) x=x->next;
      x->next = lib;
    }
  } else {
    if (!sy_lib_min) {
      sy_lib_min = lib;
    } else {
      x = sy_lib_min;
      while (x->next) x=x->next;
      x->next = lib;
    }
  }
}

sy_cell *sy_find_cell(const char *name,int min_max)
{
  sy_library *lib;
  sy_cell *c;
  for (lib= (min_max?sy_lib_max:sy_lib_min);lib;lib=lib->next) {
    for (c=lib->cell_list;c;c=c->next)
      if (!strcmp(name,c->name)) return c;
  }
  return NULL;
}

sy_pin *sy_find_pin(sy_cell *cell,char *name)
{
  sy_pin *x;
  for (x=cell->pin;x;x=x->next)
    if (!strcmp(name,x->name))
      return x;
  return NULL;
}


/***************************************************************************************/

void report_cell_delay(int min_max,const char *cell_nm,const char *in_nm,const char *out_nm,
                const char *rf_in_nm,const char *rf_out_nm,const char *in_slew_nm,const char *cload_nm)
{
  sy_cell *cell = sy_find_cell(cell_nm, min_max);
  sy_pin *in,*out;
  sy_pin_pair *pp;
  sy_tab8 *tab8;
  int rf_in_arg,rf_out_arg,rf_in,rf_out,first_tab;
  double in_slew,cload,delay,out_slew;
  if (!cell) {
    notice(0,"cannot find cell %s\n",cell_nm);
    return;
  }
  rf_in_arg = 2; if (rf_in_nm[0]) sscanf(rf_in_nm,"%d",&rf_in_arg);
  rf_out_arg = 2; if (rf_out_nm[0]) sscanf(rf_out_nm,"%d",&rf_out_arg);
  in_slew = 0.1; if (in_slew_nm[0]) sscanf(in_slew_nm,"%lf",&in_slew);
  cload = 0.1; if (cload_nm[0]) sscanf(cload_nm,"%lf",&cload);
  notice(0,"cell %s   (%s)\n",cell->name,(min_max?"max":"min"));
  for (in=cell->pin;in;in=in->next) {
    if (in_nm[0] && strcmp(in_nm,in->name)) continue;
    for (pp=in->from_pair; pp; pp=pp->next_for_from) {
      out = pp->to;
      if (out_nm[0] && strcmp(out_nm,out->name)) continue;
      if (!pp->normal) continue;
      for (rf_in=0;rf_in<2;rf_in++) for (rf_out=0;rf_out<2;rf_out++) {
        if (rf_in_arg<2 && rf_in!=rf_in_arg) continue;
        if (rf_out_arg<2 && rf_out!=rf_out_arg) continue;
        if (!pp->normal->tab_d[rf_in][rf_out]) continue;
        first_tab = 1;
        for (tab8 = pp->normal; tab8; tab8=tab8->next) if (tab8->tab_d[rf_in][rf_out]) {
          if (first_tab) {
            first_tab = 0;
            notice(0,"%s->%s %d->%d",in->name,out->name,rf_in,rf_out);
          }
          sy_normal_eval(tab8,rf_in,rf_out,cload,in_slew,&delay,&out_slew);
          notice(0," in_slew=%g cload=%g delay=%g out_slew=%g\n",
            in_slew,cload,delay,out_slew);
        }
      }
    }
  }
}

bool is_sequential(sy_cell *cell) {
  return cell->sequential;
}

void sy_table_set_extrap(sy_table *td)
{
  td->ddx = 0.0;
  td->ddy = 0.0;
  if (!td->tm) return; // scalar
  int n;
  double d0,d1,delc;
  if (td->tm->ex == SY_total_output_net_capacitance && td->tm->nx>1) {
    n = td->tm->nx;
    if (td->tm->ny==0) {
      // notice(0,"ny=0\n");    
      d0 = td->t[0][n-2];
      d1 = td->t[0][n-1];
    } else {
      d0 = td->t[n-2][0];
      d1 = td->t[n-1][0];
    }
    delc = td->tm->vx[n-1] - td->tm->vx[n-2];
    td->ddx = (d1-d0)/delc;
    td->ddy = 0.0;
  } else
  if (td->tm->ey == SY_total_output_net_capacitance && td->tm->ny>1) {
    n = td->tm->ny;
    d0 = td->t[0][n-2];
    d1 = td->t[0][n-1];
    delc = td->tm->vy[n-1] - td->tm->vy[n-2];
    td->ddy = (d1-d0)/delc;
    td->ddx = 0.0;
  }
}

sy_pin *sy_copy_pin(sy_cell *cell,sy_pin *pin0,char *name)
{
  sy_pin *pin = sy_pin_create(cell,name);
  // notice(0,"copying pin %s to %s in %s\n",pin0->name,pin->name,cell->name);
  pin->dir = pin0->dir;
  pin->func = pin0->func;
  pin->maxcap = pin0->maxcap;
  pin->cap = pin0->cap;
  sy_pin_pair *pp0, *pp;
  // sy_tab8 *tab80, *tab8;
  for (pp0 = pin0->from_pair; pp0; pp0 = pp0->next_for_from) {
    pp = sy_pin_pair_create(pin, pp0->to);
    pp->normal = pp0->normal;
  }
  for (pp0 = pin0->to_pair; pp0; pp0 = pp0->next_for_to) {
    pp = sy_pin_pair_create(pp0->from, pin);
    pp->normal = pp0->normal;
  }
  return pin;
}

void sy_lib_set_power(sy_library *lib,int power)
{
  sy_cell *cell;
  for (cell=lib->cell_list;cell;cell=cell->next)
    cell->power = power;
}

END_NAMESPACE_ADS
