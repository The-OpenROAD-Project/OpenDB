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

#ifndef SY_LIB_H
#define SY_LIB_H

#include "ads.h"
#include "db.h"

namespace odb {

typedef enum {
 SY_template_none,
 SY_input_net_transition,
 SY_total_output_net_capacitance,
 SY_output_net_length,
 SY_output_net_wire_cap,
 SY_output_net_pin_cap,
 SY_constrained_pin_transition,
 SY_related_pin_transition,
 SY_connect_delay,
 SY_output_pin_transition,
 SY_related_out_total_output_net_capacitance,
 SY_related_out_output_net_length,
 SY_related_out_output_net_wire_cap,
 SY_related_out_output_net_pin_cap,
 SY_related_output_pin_capacitance,
 SY_fanout_number,
 SY_fanout_pin_capacitance,
 SY_driver_slew,
 SY_rc_product,
} sy_template_enum;

typedef struct sy_template sy_template;
typedef struct sy_bustype sy_bustype;
typedef struct sy_table sy_table;
typedef struct sy_tab8 sy_tab8;
typedef struct sy_pin sy_pin;
typedef struct sy_pin_pair sy_pin_pair;
typedef struct sy_func sy_func;
typedef struct sy_cell sy_cell;
typedef struct sy_library sy_library;

struct sy_template {
  char *name;
  int three_dim;
  sy_template_enum ex;
  int nx;
  double *vx;
  sy_template_enum ey;
  int ny;
  double *vy;
  sy_template *next;
  sy_template *copies;
};

struct sy_bustype {
  char *name;
  int b0;
  int b1;
  sy_bustype *next;
};

struct sy_table {
  sy_template *tm;
  double **t;
  double ddx, ddy;
};

struct sy_tab8 {
  sy_table *tab_d[2][2];
  sy_table *tab_s[2][2];
  sy_tab8 *next;
};

struct sy_pin_pair {
  sy_pin *from;
  sy_pin *to;
  sy_tab8 *normal;
  sy_tab8 *preset;
  sy_tab8 *setup;
  sy_tab8 *hold;
  sy_pin_pair *next_for_from;
  sy_pin_pair *next_for_to;
};

struct sy_func {
  int nv;
  char **var;
  int *unate;
  int nt;
  int *tok;
  char *orig;
  void *sta_func_expr;
};

struct sy_pin {
  char *name; // should be gname pointer
  dbMTerm *mterm;
  sy_pin_pair *from_pair;
  sy_pin_pair *to_pair;
  int dir; /* 0,1,2,3 = in, out, inout, internal */
  sy_func *func;
  double maxcap;
  double cap;
  sy_pin *next;
  void *sta_port;
};

struct sy_cell {
  char *name;
  dbMaster *master;
  sy_library *lib;
  sy_pin *pin;
  // rearrange by pins later
  sy_table *table;
  sy_cell *next;
  void *sta_cell;
  bool dont_use;
  bool dont_touch;
  bool sequential;
  int  power;
  // for equivalent cells
  sy_cell *feq; // first equivalent cell
  sy_cell *neq; // next equivalent cell
  double strength;
  int fid;
};

struct sy_library {
  char *name;
  sy_template *tm_list;
  sy_bustype *bt_list;
  sy_cell *cell_list;
  sy_library *next;
  void *sta_lib;
  double slew_hi_thresh_rise;
  double slew_hi_thresh_fall;
  double slew_lo_thresh_rise;
  double slew_lo_thresh_fall;
  double slew_derate;
  int    slew_thresh_set;
};

void sy_read_init(void);
sy_library *sy_get_last_lib(void);
void sy_add_lib(int min_max, sy_library *lib);

double sy_interp2(double **t,double *vx,int nx,double *vy,int ny,double x,double y);

double sy_interp1(double *t,double *vx,int nx,double x);

void sy_table_cap_range(sy_table *tab,double *cmin,double *cmax);

double sy_table_cap_slew(sy_table *tab,double cap,double in_slew);

int sy_normal_eval(sy_tab8 *x,int rf_in,int rf_out,
   double cap,double in_slew,double *delay,double *slew);

int sy_template_cap_slew(sy_template *tm,int *nc,double *cv,int *ns,double *sv);

int sy_template_equal(sy_template *tm,sy_template *tm2);

sy_tab8 *sy_get_normal_table(sy_pin *from,sy_pin *to);

sy_cell *sy_find_cell(const char *name,int min_max);

sy_pin *sy_find_pin(sy_cell *cell,char *name);

sy_func *sy_func_read(char *line);

void sy_fdb_start(void);

sy_func *sy_fdb_next(void);

int sy_func_eval(sy_func *f,uint ev,int *err);

int sy_func_get_unate(sy_func *f,char *pin,int *unate);

bool is_sequential(sy_cell *cell);

sy_template *sy_add_normal_template(sy_library *lib,char *name,int cn,double *cv,int sn,double *sv);

void sy_table_replace_template(sy_table *tbl,sy_template *new_tm);

void sy_write_start(sy_library *lib,char *file);
void sy_write_add_cell(sy_cell *cell);
void sy_write_end();

sy_library * liberty_read(FILE *fp,char *file);

void sy_lib_set_power(sy_library *lib,int power);

} // namespace
#endif
