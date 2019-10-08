#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "logger.h"
#include "liberty_structs.h"
#include "sy_lib.h"
#include "sy_pi.h"
#include "sy_palloc.h"
#include "sy_group_enum.h"
#include "sy_attr_enum.h"
#include "sy_lib.h"

extern int Sy_lineno;
extern char *Sy_filename;
extern FILE *liberty_parser2_in;
extern int liberty_parser_parse(void);

BEGIN_NAMESPACE_ADS

static const double pwr_01[12] = { 0.1, 0.01, 0.001, 0.0001, 0.00001,
                0.000001, 0.0000001, 0.00000001, 0.000000001,
                0.0000000001, 0.00000000001, 0.000000000001 };

double ath_strtod(char *p,char **endp)
{
  char *p0 = p;
  double x = 0.0;
  int n;
  int minus = 0;
  while (*p==' ' || *p=='\t') p++;
  if (*p=='+') p++;
  else if (*p=='-') { p++; minus=1; }
  if ((*p<'0' || *p>'9') && *p!='.') {
    if (endp) *endp=p0;
  } else {
    while (*p>='0' && *p<='9') x = 10.0*x + (*(p++) - '0');
    if (*p=='.') {
      p++;
      n = 0;
      while (*p>='0' && *p<='9') x += pwr_01[n++]*(*(p++) - '0');
    }
    if (*p=='e' || *p=='E') {
      return strtod(p0,endp);
    }
    if (endp) *endp = p;
    if (minus && x) x = -x;
  }
  return x;
}



typedef struct libGroupMap { char *name; sy_group_enum type; } libGroupMap;
typedef struct libAttrMap { char *name; sy_attr_enum type; } libAttrMap;

const struct libGroupMap *
lookup_group_name (register const char *str, register unsigned int len);

const struct libGroupMap *
lookup_attr_name (register const char *str, register unsigned int len);

static sy_library *Sy_lib = NULL;
static sy_bustype *Sy_bt = NULL;
static sy_template *Sy_tm = NULL;
static sy_template *Sy_tbl_tm = NULL;
static sy_cell *Sy_cell = NULL;
static sy_pin *Sy_pin = NULL;
static int     Sy_pin_nameN = 0;
static char  **Sy_pin_nameV = NULL;
static sy_pin *Sy_related_pin = NULL;
static sy_table *Sy_tbl = NULL;
static int Sy_test_cell = 0;
static double Sy_cap_unit = 1.0;

static int Sy_depth = 0;
static int Sy_bustype_depth = 0;
static int Sy_tm_depth = 0;
static int Sy_cell_depth = 0;
static int Sy_bus_depth = 0;
static int Sy_pin_depth = 0;
static int Sy_timing_depth = 0;
static int Sy_table_depth = 0;
static int Sy_test_cell_depth = 0;

static double *tmp_dbl[64];
static int     tmp_dbl_n[64];

static struct { sy_table *tbl; int grp_ty; } Sy_tblV[32];
static int Sy_tblN = 0;
static int Sy_unateness = 0;
static char Sy_timing_type[256];
static char Sy_bus[256];
static sy_bustype *Sy_bus_t;

static sy_template * sy_find_template(char *name);
static sy_bustype * sy_find_bustype(char *name);
static void sy_timing_wrap();

static struct { char *msg; int cnt; } Sy_err_reported[32];

void sy_read_init(void)
{
  Sy_lib = NULL;
  int j;
  for (j=0;j<32;j++) Sy_err_reported[j].cnt = 0;
}

sy_library *sy_get_last_lib()
{
  return Sy_lib;
}

int sy_suppress(char *msg)
{
  int j;
  for (j=0;j<32;j++) {
    if (Sy_err_reported[j].cnt==0) {
      Sy_err_reported[j].msg = (char*)malloc(1+strlen(msg));
      strcpy(Sy_err_reported[j].msg, msg);
      Sy_err_reported[j].cnt = 1;
      return 0;
    } else if (!strcmp(Sy_err_reported[j].msg, msg)) {
      Sy_err_reported[j].cnt ++;
      if (Sy_err_reported[j].cnt < 10) return 0;
      if (Sy_err_reported[j].cnt == 10)
        notice(0,"Error: %s suppressed\n",msg);
      return 1;
    }
  }
  return 1;
}

void sy_err(char *msg)
{
  if (!sy_suppress(msg))
  notice(0,"Error: %s  line %d %s\n",msg,Sy_lineno,Sy_filename);
}

void sy_err1(char *msg,char *str)
{
  if (!sy_suppress(msg))
  notice(0,"Error: %s %s  line %d %s\n",msg,str,Sy_lineno,Sy_filename);
}

int sy_lookupGroupType(char * group_type)
{
    const libGroupMap *lgm;
    lgm = lookup_group_name (group_type, strlen(group_type));
    if (!lgm) {
      sy_err1("unrecognized group type",group_type);
      return 0;
    }
    return (int)(lgm->type);
}

int sy_lookupAttrType(char * attr_type)
{
    const libGroupMap *lgm;
    lgm = lookup_attr_name (attr_type, (uint) strlen(attr_type));
    if (!lgm) return (int)(LIBERTY_ATTRENUM_revision);
    return (int)(lgm->type);
}


void sy_print_attr_val(liberty_attribute_value *v)
{
  if (v->type==LIBERTY__VAL_STRING)
    notice(0,"%s",v->u.string_val);
  else if (v->type==LIBERTY__VAL_INT || v->type==LIBERTY__VAL_BOOLEAN)
    notice(0,"%d",v->u.int_val);
  else if (v->type==LIBERTY__VAL_DOUBLE)
    notice(0,"%g",v->u.double_val);
}

static void  sy_read_values(liberty_attribute_value *v,int tmpdi)
{
  int n = tmp_dbl_n[tmpdi] = 0;
  int nmax;
  double *x = tmp_dbl[tmpdi] = sy_double_alloc(&nmax);
  char *pc,*pend;
  if (nmax<12) { sy_err("overflow doubles"); return; }
  if (v->type != LIBERTY__VAL_STRING) {
    return;
  }
  for (pc = v->u.string_val;*pc;pc++) if (*pc==',') *pc=' ';
  pc = v->u.string_val;
  while (n<nmax) {
#if 1
    x[n] = ath_strtod(pc,&pend);
#else
    x[n] = strtod(pc,&pend);
#endif
    if (pend==pc) break;
    pc = pend;
    n++;
  }
  if (n==nmax) {
    sy_err("overflow double");
  }
  sy_double_used(n);
  tmp_dbl_n[tmpdi] = n;
}

static void  sy_read_values(liberty_attribute_value *v,int tmpdi,double xmult)
{
  int n = tmp_dbl_n[tmpdi] = 0;
  int nmax;
  double *x = tmp_dbl[tmpdi] = sy_double_alloc(&nmax);
  char *pc,*pend;
  if (nmax<12) { sy_err("overflow doubles"); return; }
  if (v->type != LIBERTY__VAL_STRING) {
    return;
  }
  for (pc = v->u.string_val;*pc;pc++) if (*pc==',') *pc=' ';
  pc = v->u.string_val;
  while (n<nmax) {
    x[n] = xmult*ath_strtod(pc,&pend);
    if (pend==pc) break;
    pc = pend;
    n++;
  }
  if (n==nmax) {
    sy_err("overflow double");
  }
  sy_double_used(n);
  tmp_dbl_n[tmpdi] = n;
}


void sy_group_wrap()
{
  if (Sy_depth==Sy_tm_depth) { Sy_tm = NULL; Sy_tm_depth = 0; }
  if (Sy_depth==Sy_bustype_depth) { 
    if (Sy_bt->b1 < Sy_bt->b0) {
      int tmpi = Sy_bt->b0; Sy_bt->b0 = Sy_bt->b1; Sy_bt->b1 = tmpi;
    }
    Sy_bt = NULL; Sy_bustype_depth = 0;
  }
  if (Sy_depth==Sy_cell_depth) { Sy_cell = NULL; Sy_cell_depth = 0; }
  if (Sy_depth==Sy_bus_depth) {
    if (Sy_pin && Sy_bus_t) {
      int k;
      for (k=Sy_bus_t->b0+1;k<=Sy_bus_t->b1;k++) {
        char tmps[1024];
        sprintf(tmps,"%s[%d]",Sy_bus,k);
        sy_copy_pin(Sy_cell,Sy_pin,tmps);
      }
      Sy_pin = NULL;
    }
    Sy_bus[0]='\0'; Sy_bus_t = NULL; Sy_bus_depth = 0;
  }
  if (Sy_depth==Sy_pin_depth) { 
    if (Sy_pin_nameN) {
      int k;
      for (k=0;k<Sy_pin_nameN;k++)
        sy_copy_pin(Sy_cell,Sy_pin,Sy_pin_nameV[k]);
      while (Sy_pin_nameN) {
        --Sy_pin_nameN;
        free(Sy_pin_nameV[Sy_pin_nameN]);
      }
      free(Sy_pin_nameV);
    }
    Sy_pin = NULL; Sy_pin_depth = 0;
  }
  if (Sy_depth==Sy_timing_depth) {
    sy_timing_wrap();
    Sy_related_pin = NULL;
    Sy_unateness = 0;
    Sy_timing_type[0] = '\0';
    Sy_timing_depth = 0;
  }
  if (Sy_depth==Sy_table_depth) { Sy_tbl = NULL; Sy_table_depth = 0; Sy_tbl_tm = NULL; }
  if (Sy_depth==Sy_test_cell_depth) { Sy_test_cell = 0; Sy_test_cell_depth = 0; }
  --Sy_depth;
}

bool sy_bus_name(char *str,char *bname,int *b0,int *b1)
{
  char *p,*popen,*psep,*pclose;
  strcpy(bname,str);
  popen = psep = pclose = NULL;
  for (p=bname;*p;p++) {
    if (*p=='[') popen = p;
    if (*p==':') psep = p;
    if (*p==']') pclose = p;
  }
  if (!popen || !psep || !pclose) return false;
  if (popen>psep || psep>pclose) return false;
  *popen = *psep = *pclose = '\0';
  *b0 = atoi(popen+1);
  *b1 = atoi(psep+1);
  if (*b1 < *b0) { int tmp = *b0; *b0 = *b1; *b1 = tmp; }
  return true;
}

void sy_group_start(char *name,liberty_attribute_value *list)
{
  int grp_ty;
  char *valstr = NULL;
  Sy_depth++;
  if (list && list->type==LIBERTY__VAL_STRING) valstr = list->u.string_val;
  grp_ty = sy_lookupGroupType(name);

  if (grp_ty==LIBERTY_GROUPENUM_library) {
    if (Sy_depth!=1) {
      sy_err("library at wrong depth");
    } else {
      Sy_lib = sy_library_create(valstr);
      Sy_cell = NULL;
      Sy_pin = NULL;
      Sy_cap_unit = 1.0;
    }
    sy_attr_free_list(list);
    return;
  }
  if (!Sy_lib) {
    sy_attr_free_list(list);
    return;
  }

  if (grp_ty==LIBERTY_GROUPENUM_type) {
    if (Sy_depth!=2) {
      sy_err("bustype at wrong depth");
    } else {
      Sy_bustype_depth = Sy_depth;
      Sy_bt = sy_bustype_create(Sy_lib,valstr);
    }
  }
  else if (grp_ty==LIBERTY_GROUPENUM_lu_table_template) {
    if (Sy_depth!=2) {
      sy_err("template at wrong depth");
    } else {
      Sy_tm_depth = Sy_depth;
      Sy_tm = sy_template_create(Sy_lib,valstr);
    }
  }
  else if (grp_ty==LIBERTY_GROUPENUM_cell) {
    debug("LIBERTY","a","sy_lib cell %s\n",valstr);
    if (Sy_depth!=2) {
      sy_err("template at wrong depth");
    } else {
      Sy_cell = sy_cell_create(Sy_lib,valstr);
      Sy_cell_depth = Sy_depth;
    }
    if (list->next) sy_err("unexpected cell names");
    Sy_bus[0] = '\0';
    Sy_bus_t = NULL;
  }

  /* requiring cell from here */

  if (!Sy_cell) {
    sy_attr_free_list(list);
    return;
  }

  if (grp_ty==LIBERTY_GROUPENUM_test_cell) {
    Sy_test_cell = 1;
    Sy_test_cell_depth = Sy_depth;
  }

  if (Sy_test_cell) {
    sy_attr_free_list(list);
    return;
  }

  if (grp_ty==LIBERTY_GROUPENUM_bus) {
    strcpy(Sy_bus,valstr);
    Sy_bus_depth = Sy_depth;
    sy_attr_free_list(list);
    return;
  }

  if (grp_ty==LIBERTY_GROUPENUM_pin) {
    char bname[1024]; int b0,b1;
    if (Sy_depth!=3 && !(Sy_bus_depth>0 && Sy_depth==Sy_bus_depth+1)) {
      sy_err("pin at wrong depth");
    } else if (Sy_bus_depth>0 && sy_bus_name(valstr,bname,&b0,&b1)) {
      char tmps[1024];
      sprintf(tmps,"%s[%d]",bname,b0);
      if (Sy_pin)
        Sy_pin->name = strdup(tmps);
      else
        Sy_pin = sy_pin_create(Sy_cell,tmps);
      Sy_pin_depth = Sy_depth;
      Sy_pin_nameN = b1 - b0;
      if (Sy_pin_nameN) {
        Sy_pin_nameV = (char**)malloc(Sy_pin_nameN*sizeof(char*));
        for (int k=0;k<Sy_pin_nameN;k++) {
          Sy_pin_nameV[k] = (char*)malloc(6+strlen(bname));
          sprintf(Sy_pin_nameV[k],"%s[%d]",bname,b0+1+k);
        }
      }
    } else {
      Sy_pin = sy_pin_create(Sy_cell,valstr);
      Sy_pin_depth = Sy_depth;
    }
    if (list->next) {
      Sy_pin_nameN = 0;
      liberty_attribute_value *z;
      for (z = list->next; z; z = z->next) {
        if (z->type==LIBERTY__VAL_STRING) Sy_pin_nameN++;
        else
          sy_err("unexpected pin attribute");
      }
      if (Sy_pin_nameN) {
        Sy_pin_nameV = (char**)malloc(Sy_pin_nameN*sizeof(char*));
        Sy_pin_nameN = 0; z = list->next;
        for (z = list->next; z; z = z->next) {
          if (z->type==LIBERTY__VAL_STRING) {
            Sy_pin_nameV[Sy_pin_nameN] = (char*)
              malloc(1+strlen(z->u.string_val));
            strcpy(Sy_pin_nameV[Sy_pin_nameN], z->u.string_val);
            Sy_pin_nameN++;
          }
        }
      }
    }
  }

  /* requiring pin from here */

  if (grp_ty==LIBERTY_GROUPENUM_timing && !Sy_pin) {
    if (!Sy_bus[0] || !Sy_bus_t) {
      notice(0,"warning, timing table without pin\n");
    } else {
      notice(0,"timing table for pins %s[] %d %d\n",Sy_bus,Sy_bus_t->b0,Sy_bus_t->b1);
    }
  }

  if (!Sy_pin) {
    sy_attr_free_list(list);
    return;
  }

  if (grp_ty==LIBERTY_GROUPENUM_timing) {
    Sy_timing_depth = Sy_depth;
    Sy_tblN = 0;
    Sy_related_pin = NULL;
    Sy_unateness = 0;
    Sy_timing_type[0] = '\0';
  }

  /* requiring timing from here */

  if (!Sy_timing_depth) {
    sy_attr_free_list(list);
    return;
  }


  if (grp_ty>=LIBERTY_GROUPENUM_rise_transition && grp_ty<=LIBERTY_GROUPENUM_cell_degradation) {
    if (list->next) sy_err("unexpected template names for table");
    if (!strcmp(valstr,"scalar")) {
        Sy_table_depth = Sy_depth;
        Sy_tbl = Sy_tblV[Sy_tblN].tbl = sy_table_create(NULL);
        Sy_tblV[Sy_tblN++].grp_ty = grp_ty;
    } else {
      sy_template *tm = sy_find_template(valstr);
      if (!tm) {
        sy_err1("cannot find template",valstr);
      } else {
        Sy_table_depth = Sy_depth;
        Sy_tbl = Sy_tblV[Sy_tblN].tbl = sy_table_create(tm);
        Sy_tblV[Sy_tblN++].grp_ty = grp_ty;
      }
    }
  }
  sy_attr_free_list(list);
}
    
  
void sy_simple_attr(char *name,liberty_attribute_value *v)
{
  int attr_ty = sy_lookupAttrType(name);
  if (v->type == LIBERTY__VAL_STRING) {
    char *val = v->u.string_val;
    if (attr_ty==LIBERTY_ATTRENUM_variable_1 && Sy_tm) {
      Sy_tm->ex = sy_template_enum_lookup(val);
    } else if (attr_ty==LIBERTY_ATTRENUM_variable_2 && Sy_tm) {
      Sy_tm->ey = sy_template_enum_lookup(val);
    } else if (attr_ty==LIBERTY_ATTRENUM_direction && Sy_pin) {
      if (!strcmp(val,"input")) Sy_pin->dir = 0;
      else if (!strcmp(val,"output")) Sy_pin->dir = 1;
      else if (!strcmp(val,"inout")) Sy_pin->dir = 2;
      else if (!strcmp(val,"internal")) Sy_pin->dir = 3;
      else sy_err1("unrecognized direction",val);
    } else if (attr_ty==LIBERTY_ATTRENUM_function && Sy_pin) {
      Sy_pin->func = sy_func_read(val);
    } else if (Sy_pin && Sy_timing_depth) {
      if (attr_ty==LIBERTY_ATTRENUM_related_pin) {
         Sy_related_pin = sy_pin_create(Sy_cell,val);
      } else if (attr_ty==LIBERTY_ATTRENUM_timing_type) {
         strcpy(Sy_timing_type,val);
      } else if (attr_ty==LIBERTY_ATTRENUM_timing_sense) {
         if (!strcmp(val,"positive_unate")) Sy_unateness = 1;
         else if (!strcmp(val,"negative_unate")) Sy_unateness = 2;
         else if (!strcmp(val,"non_unate")) Sy_unateness = 0;
         else {
           Sy_unateness = 0;
           sy_err1("unrecognized timing_sense",val);
         }
      }
    } else if (Sy_bus[0] && attr_ty==LIBERTY_ATTRENUM_bus_type) {
      Sy_bus_t = sy_find_bustype(val);
      if (!Sy_bus_t) {
        sy_err1("unrecognized bus_type",val);
      } else {
        char tmps[1024];
        sprintf(tmps,"%s[%d]",Sy_bus,Sy_bus_t->b0);
        Sy_pin = sy_pin_create(Sy_cell,tmps);
      }
    }

  }
  else if (v->type == LIBERTY__VAL_DOUBLE) {
    if (attr_ty==LIBERTY_ATTRENUM_max_capacitance && Sy_pin) {
      Sy_pin->maxcap = Sy_cap_unit*v->u.double_val;
    } else if (attr_ty==LIBERTY_ATTRENUM_capacitance && Sy_pin) {
      Sy_pin->cap = Sy_cap_unit*v->u.double_val;
    } else if (attr_ty==LIBERTY_ATTRENUM_slew_derate_from_library) {
      Sy_lib->slew_derate = v->u.double_val;
      Sy_lib->slew_thresh_set++;
    } else if (attr_ty==LIBERTY_ATTRENUM_slew_upper_threshold_pct_rise) {
      Sy_lib->slew_hi_thresh_rise = 0.01*v->u.double_val;
      Sy_lib->slew_thresh_set++;
    } else if (attr_ty==LIBERTY_ATTRENUM_slew_upper_threshold_pct_fall) {
      Sy_lib->slew_hi_thresh_fall = 0.01*v->u.double_val;
      Sy_lib->slew_thresh_set++;
    } else if (attr_ty==LIBERTY_ATTRENUM_slew_lower_threshold_pct_rise) {
      Sy_lib->slew_lo_thresh_rise = 0.01*v->u.double_val;
      Sy_lib->slew_thresh_set++;
    } else if (attr_ty==LIBERTY_ATTRENUM_slew_lower_threshold_pct_fall) {
      Sy_lib->slew_lo_thresh_fall = 0.01*v->u.double_val;
      Sy_lib->slew_thresh_set++;
    } else if (attr_ty==LIBERTY_ATTRENUM_bit_from && Sy_bt) {
      Sy_bt->b0 = int(v->u.double_val);
    } else if (attr_ty==LIBERTY_ATTRENUM_bit_to && Sy_bt) {
      Sy_bt->b1 = int(v->u.double_val);
    }

  }
  else if (v->type == LIBERTY__VAL_BOOLEAN) {
    if (attr_ty==LIBERTY_ATTRENUM_dont_use && Sy_cell) {
      Sy_cell->dont_use = v->u.bool_val;
    } else if (attr_ty==LIBERTY_ATTRENUM_dont_touch && Sy_cell) {
      Sy_cell->dont_touch = v->u.bool_val;
    }
  }
  sy_attr_free(v);
}


void sy_complex(char *name,liberty_attribute_value *list)
{
  liberty_attribute_value *v;
  int attr_ty = sy_lookupAttrType(name);
  int n,j,k;
  if (attr_ty==LIBERTY_ATTRENUM_index_1 && Sy_tbl) {
    // template values inside timing table
    Sy_tbl_tm = sy_template_copy(Sy_lib,Sy_tbl->tm,Sy_lineno);
    if (Sy_tbl->tm->ex ==  SY_total_output_net_capacitance)
      sy_read_values(list,0,Sy_cap_unit);
    else
      sy_read_values(list,0);
    sy_template_add_index(Sy_tbl_tm,1,tmp_dbl[0],tmp_dbl_n[0]);
    sy_double_free_all();
    if (list->next) sy_err("too many values");

  } else if (attr_ty==LIBERTY_ATTRENUM_index_2 && Sy_tbl) {
    // template values inside timing table
    if (! Sy_tbl_tm) {
      // index1 was not done first, so copy it
      Sy_tbl_tm = sy_template_copy(Sy_lib,Sy_tbl->tm,Sy_lineno);
      sy_template_add_index(Sy_tbl_tm,1,Sy_tbl->tm->vx,Sy_tbl->tm->nx);
    } 
    if (Sy_tbl->tm->ey ==  SY_total_output_net_capacitance)
      sy_read_values(list,0,Sy_cap_unit);
    else
      sy_read_values(list,0);
    sy_template_add_index(Sy_tbl_tm,2,tmp_dbl[0],tmp_dbl_n[0]);
    sy_double_free_all();
    if (list->next) sy_err("too many values");

  } else if (attr_ty==LIBERTY_ATTRENUM_index_3 && Sy_tbl) {
    // template values inside timing table
    if (! Sy_tbl_tm) {
      // index1,2 not done first, so copy
      Sy_tbl_tm = sy_template_copy(Sy_lib,Sy_tbl->tm,Sy_lineno);
      sy_template_add_index(Sy_tbl_tm,1,Sy_tbl->tm->vx,Sy_tbl->tm->nx);
      sy_template_add_index(Sy_tbl_tm,2,Sy_tbl->tm->vy,Sy_tbl->tm->ny);
    }
    if (Sy_tbl->tm->ey ==  SY_total_output_net_capacitance)
      sy_read_values(list,0,Sy_cap_unit);
    else
      sy_read_values(list,0);
    sy_template_add_index(Sy_tbl_tm,3,tmp_dbl[0],tmp_dbl_n[0]);
    sy_double_free_all();
    if (list->next) sy_err("too many values");

  } else if (attr_ty==LIBERTY_ATTRENUM_values) {
    for (n=0,v=list;v;v=v->next,n++) {
      if (n==64) { sy_err("too many values"); break; }
      sy_read_values(v,n);
    }
    // process tmp_dbl
    if (Sy_tbl && ! Sy_tbl->tm) {
      // scalar
      if (n!=1 || tmp_dbl_n[0]!=1) {
         sy_err("values does not match template");
         Sy_tbl->t[0][0] = 0.0;
      } else {
         Sy_tbl->t[0][0] = tmp_dbl[0][0];
      }
    } else if (Sy_tbl && Sy_tbl->tm) {
      if (Sy_tbl_tm) {
        sy_table_replace_template(Sy_tbl,Sy_tbl_tm);
      }
      if (n==1 && Sy_tbl->tm->ny==0) {
        if (tmp_dbl_n[0] != Sy_tbl->tm->nx) {
           sy_err("values does not match template");
           for (k=0;k<Sy_tbl->tm->nx;k++) Sy_tbl->t[0][k] = 0.0;
        } else {
           for (k=0;k<Sy_tbl->tm->nx;k++)
             Sy_tbl->t[0][k] = tmp_dbl[0][k];
        }
      } else if (Sy_tbl->tm->three_dim &&
                 n == Sy_tbl->tm->nx*Sy_tbl->tm->ny) {
          for (j=0;j<Sy_tbl->tm->nx;j++)
            for (k=0;k<Sy_tbl->tm->ny;k++)
              Sy_tbl->t[j][k] = tmp_dbl[j*Sy_tbl->tm->ny+k][0];
      } else {
        if (n != Sy_tbl->tm->nx) {
          sy_err("values does not match template");
          for (j=0;j<Sy_tbl->tm->nx;j++)
            for (k=0;k<Sy_tbl->tm->ny;k++) Sy_tbl->t[j][k] = 0.0;
        } else {
          for (j=0;j<n;j++) {
            if (tmp_dbl_n[j] != Sy_tbl->tm->ny) {
              sy_err("values does not match template");
            } else {
              for (k=0;k<Sy_tbl->tm->ny;k++)
                Sy_tbl->t[j][k] = tmp_dbl[j][k];
            }
          }
        }
      }
    }
    sy_double_free_all();
  } else if (attr_ty==LIBERTY_ATTRENUM_index_1 && Sy_tm) {
    if (Sy_tm->ex == SY_total_output_net_capacitance)
      sy_read_values(list,0,Sy_cap_unit);
    else
      sy_read_values(list,0);
    sy_template_add_index(Sy_tm,1,tmp_dbl[0],tmp_dbl_n[0]);
    sy_double_free_all();
    if (list->next) sy_err("too many values");
  } else if (attr_ty==LIBERTY_ATTRENUM_index_2 && Sy_tm) {
    if (Sy_tm->ey == SY_total_output_net_capacitance)
      sy_read_values(list,0,Sy_cap_unit);
    else
      sy_read_values(list,0);
    sy_template_add_index(Sy_tm,2,tmp_dbl[0],tmp_dbl_n[0]);
    sy_double_free_all();
    if (list->next) sy_err("too many values");
  } else if (attr_ty==LIBERTY_ATTRENUM_index_3 && Sy_tm) {
    sy_read_values(list,0);
    sy_template_add_index(Sy_tm,3,tmp_dbl[0],tmp_dbl_n[0]);
    sy_double_free_all();
    if (list->next) sy_err("too many values");
  } else if (attr_ty==LIBERTY_ATTRENUM_capacitive_load_unit) {
    double uval = 0.0;
    char *ustr = NULL;
    if (list->type == LIBERTY__VAL_DOUBLE) uval = list->u.double_val;
    if (list->next && list->next->type == LIBERTY__VAL_STRING)
      ustr = list->next->u.string_val;
    if (uval==0.0 || ustr==NULL) {
      notice(0,"cannot read capacitive unit\n");
    }
    if (!strcmp(ustr,"ff")) uval *= 1e-3;
    else if (!strcmp(ustr,"pf")) {}
    else if (!strcmp(ustr,"f") || !strncmp(ustr,"farad",5)) uval *= 1e12;
    else {
      notice(0,"cannot recognized capacitive unit %s\n",ustr);
      uval = 0.0;
    }
    if (uval > 0.0) {
      // notice(0,"capacitive unit: %g pf\n",uval);
      Sy_cap_unit = uval;
    }
  }
  sy_attr_free_list(list);
}

void sy_define(char *name,char *str,char *value_type)
{
}

void sy_define_group(char *name,char *str)
{
}

static sy_template * sy_find_template(char *name)
{
  sy_template *tm;
  if (!Sy_lib) return NULL;
  for (tm = Sy_lib->tm_list; tm; tm=tm->next)
    if (!strcmp(name,tm->name)) return tm;
  return NULL;
}

static sy_bustype * sy_find_bustype(char *name)
{
  sy_bustype *bt;
  if (!Sy_lib) return NULL;
  for (bt = Sy_lib->bt_list; bt; bt=bt->next)
    if (!strcmp(name,bt->name)) return bt;
  return NULL;
}

static bool sy_add_tab_d(sy_tab8 *t8,int ii,int jj,sy_table *tbl)
{
  if (t8->tab_d[ii][jj]) {
    // sy_err("ignoring duplicate table");
    return false;
  } else {
    t8->tab_d[ii][jj] = tbl;
  }
  return true;
}

static bool sy_add_tab_s(sy_tab8 *t8,int ii,int jj,sy_table *tbl)
{
  if (t8->tab_s[ii][jj]) {
    // sy_err("ignoring duplicate table");
    return false;
  } else {
    t8->tab_s[ii][jj] = tbl;
  }
  return true;
}

static void sy_timing_wrap()
{
  int j,cnt,grp_ty,rf_in,match;
  sy_pin_pair *pp = NULL;
  sy_tab8 *tab8;
  if (!Sy_pin) notice(0,"no pin\n");
  if (!Sy_related_pin) notice(0,"no related pin\n");
  if (Sy_tblN>8) notice(0,"%d tables\n",Sy_tblN);
  if (!(Sy_pin && Sy_related_pin)) { Sy_tblN = 0; return; }

#if 0
  notice(0,"%d tables, pin=%s related_pin=%s\n",Sy_tblN,Sy_pin->name,Sy_related_pin->name);
  if (Sy_timing_type[0]) notice(0,"timing_type %s\n",Sy_timing_type);
#endif

  if (!Sy_timing_type[0] || !strcmp(Sy_timing_type,"combinational")) {
    cnt = 0;
    for (j=0;j<Sy_tblN;j++) {
      grp_ty = Sy_tblV[j].grp_ty;
      if (grp_ty==LIBERTY_GROUPENUM_cell_rise
         || grp_ty==LIBERTY_GROUPENUM_cell_fall
         || grp_ty==LIBERTY_GROUPENUM_rise_transition
         || grp_ty==LIBERTY_GROUPENUM_fall_transition) cnt++;
    }
    if (cnt) {
      // normal delay tables
      pp = sy_pin_pair_create(Sy_related_pin,Sy_pin);
      if (Sy_unateness == 0 && Sy_pin->func) {
        int unat;
        if (sy_func_get_unate(Sy_pin->func,Sy_related_pin->name,&unat))
          Sy_unateness = unat;
      }
      // may have already seen other unateness, so normal may exist
      for (tab8 = pp->normal; tab8; tab8=tab8->next) {
        // here, should be checking for matching attributes
        match = 1;
        if (match) break;
      }
      if (!tab8) {
        tab8 = sy_tab8_create();
        tab8->next = pp->normal;
        pp->normal = tab8;
      }
      for (j=0;j<Sy_tblN;j++) {
        grp_ty = Sy_tblV[j].grp_ty;
        bool add = false;
        if (grp_ty==LIBERTY_GROUPENUM_cell_rise) {
          sy_table_set_extrap(Sy_tblV[j].tbl);
          if (Sy_unateness!=2) add= sy_add_tab_d(tab8,1,1,Sy_tblV[j].tbl);
          if (Sy_unateness!=1) add= sy_add_tab_d(tab8,0,1,Sy_tblV[j].tbl);
        } else if (grp_ty==LIBERTY_GROUPENUM_cell_fall) {
          sy_table_set_extrap(Sy_tblV[j].tbl);
          if (Sy_unateness!=2) add= sy_add_tab_d(tab8,0,0,Sy_tblV[j].tbl);
          if (Sy_unateness!=1) add= sy_add_tab_d(tab8,1,0,Sy_tblV[j].tbl);
        } else if (grp_ty==LIBERTY_GROUPENUM_rise_transition) {
          sy_table_set_extrap(Sy_tblV[j].tbl);
          if (Sy_unateness!=2) add= sy_add_tab_s(tab8,1,1,Sy_tblV[j].tbl);
          if (Sy_unateness!=1) add= sy_add_tab_s(tab8,0,1,Sy_tblV[j].tbl);
        } else if (grp_ty==LIBERTY_GROUPENUM_fall_transition) {
          sy_table_set_extrap(Sy_tblV[j].tbl);
          if (Sy_unateness!=2) add= sy_add_tab_s(tab8,0,0,Sy_tblV[j].tbl);
          if (Sy_unateness!=1) add= sy_add_tab_s(tab8,1,0,Sy_tblV[j].tbl);
        }
        if (!add) {
          sy_table_destroy(Sy_tblV[j].tbl);
          Sy_tblV[j].tbl = NULL;
        }
      }
      if (cnt==Sy_tblN) {
        Sy_tblN = 0;
        return;
      }
    }
    sy_err("other tables, without timing type");
    Sy_tblN = 0;
    return;
  }
  
  if (!strcmp(Sy_timing_type,"rising_edge")
     || !strcmp(Sy_timing_type,"falling_edge")) {
    // we treat these as normal
    pp = sy_pin_pair_create(Sy_related_pin,Sy_pin);
    for (tab8 = pp->normal; tab8; tab8=tab8->next) {
      // here, should be checking for matching attributes
      match = 1;
      if (match) break;
    }
    if (!tab8) {
      tab8 = sy_tab8_create();
      tab8->next = pp->normal;
      pp->normal = tab8;
    }
    if (!strcmp(Sy_timing_type,"rising_edge")) rf_in = 1; else rf_in = 0;
    for (j=0;j<Sy_tblN;j++) {
      grp_ty = Sy_tblV[j].grp_ty;
      bool add = false;
      if (grp_ty==LIBERTY_GROUPENUM_cell_rise) {
        sy_table_set_extrap(Sy_tblV[j].tbl);
        add= sy_add_tab_d(tab8,rf_in,1,Sy_tblV[j].tbl);
      } else if (grp_ty==LIBERTY_GROUPENUM_cell_fall) {
        sy_table_set_extrap(Sy_tblV[j].tbl);
        add= sy_add_tab_d(tab8,rf_in,0,Sy_tblV[j].tbl);
      } else if (grp_ty==LIBERTY_GROUPENUM_rise_transition) {
        sy_table_set_extrap(Sy_tblV[j].tbl);
        add= sy_add_tab_s(tab8,rf_in,1,Sy_tblV[j].tbl);
      } else if (grp_ty==LIBERTY_GROUPENUM_fall_transition) {
        sy_table_set_extrap(Sy_tblV[j].tbl);
        add= sy_add_tab_s(tab8,rf_in,0,Sy_tblV[j].tbl);
      } else if (grp_ty==LIBERTY_GROUPENUM_retaining_rise) {
      } else if (grp_ty==LIBERTY_GROUPENUM_retain_rise_slew) {
      } else if (grp_ty==LIBERTY_GROUPENUM_retaining_fall) {
      } else if (grp_ty==LIBERTY_GROUPENUM_retain_fall_slew) {
      } else {
        sy_err("unexpected table with rising_edge or falling_edge");
      }
      if (!add) {
        sy_table_destroy(Sy_tblV[j].tbl);
        Sy_tblV[j].tbl = NULL;
      }
    }
    Sy_tblN = 0;
    return;
  }

  if (!strcmp(Sy_timing_type,"setup_rising")
      || !strcmp(Sy_timing_type,"setup_falling")) {
    // need to add setup tables
    Sy_cell->sequential = true;
  }
      
  // notice(0,"timing_type %s\n",Sy_timing_type);

  for (j=0;j<Sy_tblN;j++) {
    sy_table_destroy(Sy_tblV[j].tbl);
  }
  Sy_tblN = 0;
}

/********************************************************************/

void liberty_parser_report(void)
{
  if (!Sy_lib) return;
  notice(0,"ok\n");
#if 0
  for (tm = Sy_lib->tm_list; tm; tm=tm->next) {
    notice(0,"template %s\n",tm->name);
    if (tm->nx) {
      notice(0,"%s:",sy_template_enum_str(tm->ex));
      for (j=0;j<tm->nx;j++) notice(0," %g",tm->vx[j]); notice(0,"\n");
    }
    if (tm->ny) {
      notice(0,"%s:",sy_template_enum_str(tm->ey));
      for (j=0;j<tm->ny;j++) notice(0," %g",tm->vy[j]); notice(0,"\n");
    }
  }
#endif
#if 0
  for (c = Sy_lib->cell_list; c; c=c->next) {
    notice(0,"%s\n",c->name);
    for (pin=c->pin; pin; pin=pin->next) {
      for (pp = pin->from_pair; pp; pp=pp->next_for_from) {
        notice(0," %s to %s\n",pin->name,pp->to->name);
        if (pp->normal) {
          for (rf_in=0;rf_in<2;rf_in++) for (rf_out=0;rf_out<2;rf_out++)
          if (pp->normal->tab_d[rf_in][rf_out] && pp->normal->tab_s[rf_in][rf_out]) {
            notice(0,"%d->%d delay, slew\n",rf_in,rf_out);
          }
        }
      }
    }
  }
#endif
}

sy_library * liberty_read(FILE *fp,char *file)
{
  liberty_parser2_in = fp;
  Sy_filename = file;
  Sy_lineno = 0;
  sy_read_init();
  liberty_parser_parse();
  fclose(liberty_parser2_in);
  return sy_get_last_lib();
}


END_NAMESPACE_ADS
