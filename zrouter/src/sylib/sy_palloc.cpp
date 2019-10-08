/* allocation for the parser */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"

static char sy_token_strV[32768];
static char *sy_token_end = sy_token_strV+32768;
static char *sy_token_p  = sy_token_strV;

char *sy_lex_alloc(int size)
{
  char *x = sy_token_p;
  sy_token_p += size;
  if (sy_token_p < sy_token_end)
    return x;
  x = sy_token_strV;
  sy_token_p = x + size;
  return x;
}

char *sy_lex_dup(char *str)
{
  char *x = sy_lex_alloc(strlen(str)+3 /* just in case of insertions */);
  strcpy(x,str);
  return x;
}


/*****************************************************************************/
#include "liberty_structs.h"

static liberty_attribute_value attr_memV[1024];
static int                     attr_memN = 0;
static int                     attr_memU[1024];

/* expecting the top k elements to be freed, maybe in forward order */

liberty_attribute_value *sy_attr_calloc(void)
{
  if (attr_memN>=1024) {
    ads::notice(0,"Error, my_attr_calloc overflow\n");
    exit(-1);
  }
  attr_memU[attr_memN] = 1;
  liberty_attribute_value *v = attr_memV + attr_memN++;
  /* *v = (liberty_attribute_value)0; */
  v->type =  LIBERTY__VAL_UNDEFINED;
  v->u.string_val = NULL;
  v->next = NULL;
  return v;
}

void sy_attr_free(liberty_attribute_value *v)
{
  int j = v-attr_memV;
  if (j<0 || j>=attr_memN) {
    ads::notice(0,"Error, my_attr_free j=%d\n",j);
    exit(-1);
  }
  attr_memU[j]=0;

  if (j==attr_memN-1) {
    while (j>=0 && !attr_memU[j]) j--;
    attr_memN = j+1;
  }
}

void sy_attr_free_list(liberty_attribute_value *v)
{
  for (;v;v=v->next) sy_attr_free(v);
}

/*****************************************************************************/

static double d_memV[1024];
static int    d_memN = 0;

/* get a temp vector, fill it and report size used, get another, free all */
double *sy_double_alloc(int *nmax)
{
  *nmax = 1024-d_memN;
  return (d_memV+d_memN);
}

void sy_double_used(int n)
{
   d_memN += n;
   if (d_memN > 1024) {
     ads::notice(0,"Error, overflow of doubles\n");
     exit(-1);
   }
}

void sy_double_free_all(void)
{
   d_memN = 0;
}
