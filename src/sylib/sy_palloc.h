/******************************************************************************
    Copyright (c) 1996-2000 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SYNOPSYS Open Source License Version 1.0  (the "License"); 
  you may not use this file except in compliance with such restrictions 
  and limitations. You may obtain instructions on how to receive a copy of 
  the License at

  http://www.synopsys.com/partners/tapin/tapinprogram.html. 

  Software distributed by Original Contributor under the License is 
  distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either 
  expressed or implied. See the License for the specific language governing 
  rights and limitations under the License.

******************************************************************************/

liberty_attribute_value *sy_attr_calloc(void);

void sy_attr_free(liberty_attribute_value *v);

void sy_attr_free_list(liberty_attribute_value *v);

char *sy_lex_dup(char *str);

char *sy_lex_alloc(int size);

double *sy_double_alloc(int *nmax);

void sy_double_used(int n);

void sy_double_free_all(void);
