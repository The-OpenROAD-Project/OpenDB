#ifndef ADS_H
#include "ads.h"
#endif

BEGIN_NAMESPACE_ADS

/* routines called by liberty_parser.y */

void sy_group_wrap();

void sy_simple_attr(char *name,liberty_attribute_value *v);

void sy_group_start(char *name,liberty_attribute_value *list);

void sy_complex(char *name,liberty_attribute_value *list);

void sy_define(char *name,char *str,char *value_type);

void sy_define_group(char *name,char *str);


/* routines called by sy_pi */

sy_template_enum sy_template_enum_lookup(char *name);

char *sy_template_enum_str(sy_template_enum ex);

sy_library *sy_library_create(char *name);

sy_bustype *sy_bustype_create(sy_library *lib,char *name);

sy_template *sy_template_create(sy_library *lib,char *name);

void sy_template_add_index(sy_template *tm,int index,double *x,int n);

sy_template *sy_template_copy(sy_library *lib,sy_template *tdummy,int lineno);

sy_cell *sy_cell_create(sy_library *lib,const char *name);

sy_pin *sy_pin_create(sy_cell *cell,const char *name);

sy_pin_pair *sy_pin_pair_create(sy_pin *from,sy_pin *to);

sy_table *sy_table_create(sy_template *tm);

void sy_table_destroy(sy_table *tab);

sy_tab8 *sy_tab8_create(void);

void sy_table_set_extrap(sy_table *td);

sy_pin *sy_copy_pin(sy_cell *cell,sy_pin *pin0,char *name);

END_NAMESPACE_ADS
