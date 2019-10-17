/*
 *     This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 *  Distribution,  Product Version 5.6, and is subject to the Cadence
 *  LEF/DEF Open Source License Agreement.   Your  continued  use  of
 *  this file constitutes your acceptance of the terms of the LEF/DEF
 *  Open Source License and an agreement to abide by its  terms.   If
 *  you  don't  agree  with  this, you must remove this and any other
 *  files which are part of the distribution and  destroy any  copies
 *  made.
 *
 *     For updates, support, or to become part of the LEF/DEF Community,
 *  check www.openeda.org for details.
 */

/* This is a high performance hash table, for large jobs.
 *
 * There are only three functions:
 *   table = lef_make_stringtab(store);
 *   bool = lef_present_in_stringtab(table, string, &val) 
 *   insert(table, string, ptr);
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef ibmrs
#include <strings.h>
#endif
#include <ctype.h>
#include "FEF_util.h"
#include "FEF_malloc.h"
#include "FEF_stringhash.h"

static int hashval(const char *str);
static int rehashval(const char *str);
static void stringtab_grow(HTABLE *);
static const char *stringtab_alloc_string(HTABLE *,const char *);

static const char DATA_STRING[] = "Hashtable (char *)";
static const char DATA_HTABLE[] = "Hashtable HTABLE";
static const char DATA_ENTRY[] = "Hashtable ENTRY";

#define INIT_TABLE_SIZE 32
#define POOL_ALLOC_SIZE (4*4096-16)
#define POOL_SIZE (POOL_ALLOC_SIZE - sizeof(POOL *))

typedef struct entry ENTRY;
struct entry {
   const char *string;
   void *value;
};

typedef struct pool POOL;
struct pool {
    POOL *next;
    char data[POOL_SIZE];
    };

/* already defined in the .h file: typedef struct htable HTABLE; */
struct htable {
   int size;		// size of the table.  Always a power of 2
   int mask;		// masking by this will do a MOD(x, size);
   int has;		// number of entries used
   int limit;		// don't let table get fuller than this
   ENTRY *table;	// pointer to the table itself
   POOL *pool_base;	// base of linked list of pool entries
   char *pool;		// points into the first block on the above list
   int nleft;		// bytes left in the pool.
   enum table_alloc alloc;	// who allocates memory
   int (*cmp_fn)(const char *, const char *);
   int (*hash_fn)(const char *);
   int (*rehash_fn)(const char *);
};

/* for keeping track of hash tables in progress */
#define MAX_TABLES 10
static HTABLE *tables[] = {0,0,0,0,0,0,0,0,0,0};
static char *table_names[] = {0,0,0,0,0,0,0,0,0,0};


/* The following functions come in two versions - one for exact match,
 * which is faster but case sensitive, and one for case insensitive. */

static int hashval(const char *str)
{
    const char *p;
    int val=0;
    for(p=str; *p; p++)
       val = ((val << 4) + val) + *p;
    return val;
}
static int ic_hashval(const char *str)
{
    const char *p;
    int val=0;
    for(p=str; *p; p++)
       val = ((val << 4) + val) + toupper(*p);
    return val;
}

/* NOTE: rehashval MUST return an odd number, or the search algorithm
   may infinite loop.  This is because the rehash value must be
   relatively prime to the table size to ensure all slots are searched
   before we loop back to the original.  Since the table size is a
   power of 2, we need rehash to be odd to guarentee no common factors */

static int rehashval(const char *str)
{
    const char *p;
    int val=0;
    for(p=str; *p; p++)
       val = ((val << 4) + val) ^ *p;
    if ((val & 1) == 0)
       val = val + 1;
    return val;
}

static int ic_rehashval(const char *str)
{
    const char *p;
    int val=0;
    for(p=str; *p; p++)
       val = ((val << 4) + val) ^ toupper(*p);
    if ((val & 1) == 0)
       val = val + 1;
    return val;
}

int lefcasecmp(const char *str1,const char *str2)
{
// NT doesn't have this function
// return 0 if the strings match (case insensitive)
int l=strlen(str1);
int i;
char ch1,ch2;
int diff = 'Z'-'z';
if((int)strlen(str2)!=l)
	return 1;
for(i=0; i<l; i++)
	{
	ch1 = str1[i];
	if((ch1>='a')&&(ch1<='z'))
		ch1 += diff;
	ch2 = str2[i];
	if((ch2>='a')&&(ch2<='z'))
		ch2 += diff;
	if(ch1 != ch2 )
		return 1;
	}
return 0;
}

HTABLE *
lef_make_stringtab(enum table_alloc we_alloc, const char *comment,
	    enum table_case ignore_case) {
    HTABLE *htab;
    int i;
    htab = (HTABLE *)FEF_malloc(sizeof(HTABLE),DATA_HTABLE);
    htab->size = INIT_TABLE_SIZE;
    htab->mask = htab->size-1;
    htab->limit = (3*htab->size)>>2;
    htab->has = 0;
    htab->table = (ENTRY *)FEF_calloc(1,sizeof(ENTRY)*htab->size,DATA_ENTRY);
    htab->pool_base = NULL;	// no space for strings
    htab->pool = NULL;		// hence no pointer into it
    htab->nleft = 0;		// and no bytes left
    htab->alloc = we_alloc;
    if (ignore_case == TABLE_IGNORE_CASE) {
#ifdef WIN32
	htab->cmp_fn = ( int (*)(const char *, const char *)) lefcasecmp;
#else
#ifdef NOSTRCASECMP
	htab->cmp_fn = ( int (*)(const char *, const char *)) lefcasecmp;
#else
	htab->cmp_fn = ( int (*)(const char *, const char *)) strcasecmp;
#endif
#endif
	htab->hash_fn = ic_hashval;
	htab->rehash_fn = ic_rehashval;
	}
    else {   /* we are case sensitive */
	htab->cmp_fn = ( int (*)(const char *, const char *)) strcmp;
	htab->hash_fn = hashval;
	htab->rehash_fn = rehashval;
	}
	
    /* remember this hashtable */
    for(i=0;i<MAX_TABLES; i++)
	if( ! tables[i] )
		break;
    if(i<MAX_TABLES)
	{
    	tables[i] = htab;
	table_names[i] = (char *)FEF_malloc(strlen(comment)+1,DATA_STRING);
	if(! table_names[i]) lefiNerr(37);
	strcpy(table_names[i],comment);
	}
    return htab;
}

int lef_present_in_stringtab(void *hdl,const char *str,int **ret)
{
    HTABLE *htab = (HTABLE *)(hdl);
    int h = (htab->hash_fn)(str) & (htab->mask);
    int r; 
    if (htab->table[h].string == NULL)
	return FALSE;
    if ((htab->cmp_fn)(str, htab->table[h].string) == 0)
	    {
	    *ret = (int*)(htab->table[h].value);
	    return TRUE;
	    }
    /* did not find it */
    r = (htab->rehash_fn)(str);
    for(h = (h+r)&htab->mask; htab->table[h].string; h = (h+r)&htab->mask)
	if ((htab->cmp_fn)(str, htab->table[h].string) == 0)
	    {
	    *ret = (int*)htab->table[h].value;
	    return TRUE;
	    }
    return FALSE;
}

int lef_modify_stringtab(void *hdl,const char *str,void *new_val)
{
    HTABLE *htab = (HTABLE *)(hdl);
    int h = (htab->hash_fn)(str) & (htab->mask);
    int r; 
    if (htab->table[h].string == NULL)
	return FALSE;
    if ((htab->cmp_fn)(str, htab->table[h].string) == 0)
	    {
	    htab->table[h].value = new_val;
	    return TRUE;
	    }
    /* did not find it */
    r = (htab->rehash_fn)(str);
    for(h = (h+r)&htab->mask; htab->table[h].string; h = (h+r)&htab->mask)
	if ((htab->cmp_fn)(str, htab->table[h].string) == 0)
	    {
	    htab->table[h].value = new_val;
	    return TRUE;
	    }
    return FALSE;
}


const char *lef_make_string_in_stringtab(HTABLE* htab,const char *str,void *val)
{
    int h;
    if (htab->has >= htab->limit) {   /* make table bigger */
	stringtab_grow(htab);
    }

    /* Now table is big enough.  Find an empty slot */
    h = (htab->hash_fn)(str)&htab->mask;
    if (htab->table[h].string) {
	int r = (htab->rehash_fn)(str);	// only compute the rehash if needed
	for(h = (h+r)&htab->mask; htab->table[h].string; h = (h+r)&htab->mask) {
	    if ((htab->cmp_fn)(str, htab->table[h].string) == 0) {
		htab->table[h].value = val;
		return htab->table[h].string;
	    }
	}
    }

    /* now entry h is empty. */
    htab->has++;
    htab->table[h].value = val;
    if (htab->alloc == TABLE_ITSELF_ALLOCS) { /* find an entry from the pool */
	str = stringtab_alloc_string(htab,str);
    }
    htab->table[h].string = str;
    return str;
}


void lef_add_to_stringtab(void *hdl, const char *str, void *val)
{
    HTABLE *htab = (HTABLE *)(hdl);
    int h;

    if (htab->has >= htab->limit) {   /* make table bigger */
	stringtab_grow(htab);
    }

/* Now table is big enough.  Find an empty slot */
    htab->has++;
    h = (htab->hash_fn)(str)&htab->mask;
    if (htab->table[h].string) {
	int r = (htab->rehash_fn)(str);	// only compute the rehash if needed
	do {
	    h = (h+r)&htab->mask;
	} while (htab->table[h].string);
    }
/* now entry h is empty. */
    htab->table[h].value = val;
    if (htab->alloc == TABLE_ITSELF_ALLOCS) { /* find an entry from the pool */
	str = stringtab_alloc_string(htab,str);
    }
    htab->table[h].string = str;
}

static void stringtab_grow(HTABLE *htab)
{
    int i;
    ENTRY *old_table = htab->table;
    int old_size = htab->size;
    int old_has = htab->has;
    enum table_alloc alloc_save;

    htab->size = htab->size << 2;	// 4 times bigger
    htab->mask = htab->size - 1;
    htab->limit = (3*htab->size)>>2;
    htab->has = 0;
    htab->table = (ENTRY *)FEF_calloc(1,sizeof(ENTRY)*htab->size,DATA_HTABLE);

    alloc_save = htab->alloc;
    htab->alloc = TABLE_CALLER_ALLOCS;	// when resizing table,don't
    for(i=0; i<old_size; i++)
	if(old_table[i].string != NULL)
	    lef_add_to_stringtab(htab, old_table[i].string, 
		    old_table[i].value);
    htab->alloc = alloc_save;
    FEF_free((char*)old_table,DATA_HTABLE);
    if (old_has != htab->has)
	    lefiNerr(61);
}

/* Allocate a string from the pool.  Grab another block if needed */
static const char *stringtab_alloc_string(HTABLE *htab, const char *str)
{
    char *addr;
    int need = strlen(str)+1;
    if (need > POOL_SIZE) {
	printf("String too long!");
	exit(1);
	}
    if (need > htab->nleft) {
	POOL *p = (POOL *)FEF_malloc(sizeof(POOL), "POOL");
	// chain these together so lef_destroy_stringab can free all blocks.
	p->next = htab->pool_base;
	htab->pool_base = p;
	htab->pool = &(p->data[0]);
	htab->nleft = POOL_SIZE;
    }
    addr = htab->pool;
    htab->pool += need;
    htab->nleft -= need;
    strcpy(addr, str);
    return addr;
}


void lef_destroy_stringtab(void *handle)
{
    HTABLE *htab = (HTABLE *)handle;
    /* remove this handle from list */
    int i;
    for(i=0; i<MAX_TABLES; i++)
        if( tables[i] == htab )
                break;
    if( i < MAX_TABLES )
        {
        tables[i] = 0;
        FEF_free(table_names[i],DATA_STRING);
        }
    /* now do the freeing of the table */
    FEF_free((char*)htab->table,DATA_ENTRY);
    /* free all the string blocks, if allocated */
    for(;htab->pool_base != NULL;) {
	POOL *next = htab->pool_base->next;
        FEF_free((char*)htab->pool_base, DATA_STRING);
	htab->pool_base = next;
	}
    FEF_free((char*)htab,DATA_HTABLE);
}

void lef_apply_stringtab(void *handle, void (*fn)(const char *, void *, void *), void *a)
{
    HTABLE *htab = (HTABLE *)handle;
    int i;
    for(i=0; i<htab->size; i++)
	if(htab->table[i].string != NULL)
	    (*fn)(htab->table[i].string, htab->table[i].value, a);
}

int lef_active_hashtables(const char *indent)
{
int i;
int count=0;
char str[100];
strcpy(str,indent);
strcat(str,"  ");// INDENT
printf("%s{ Hash Tables !\n",str);
for(i=0; i<MAX_TABLES; i++)
        if( tables[i] )
                {
                count += 1;
                printf("%s%s\n",str,table_names[i]);
                }
printf("%s} Hash Tables\n",str);
printf("\n");
return count;
}


extern void* alias_set;  /* from lex.cpph */


const char* aliasKey(int index) {
  HTABLE* h = (HTABLE*)alias_set;
  if (h == 0) return 0;
  return h->table[index].string;
}


const char* aliasData(int index) {
  HTABLE* h = (HTABLE*)alias_set;
  if (h == 0) return 0;
  return (char*)(h->table[index].value);
}


int aliasLimit() {
  HTABLE* h = (HTABLE*)alias_set;
  if (h == 0) return 0;
  return h->size;
}


int addAlias(const char* key, const char* data) {
  HTABLE* h = (HTABLE*)alias_set;
  if (alias_set == 0) {
      alias_set = lef_make_stringtab(TABLE_CALLER_ALLOCS,
	"&alias", TABLE_USE_CASE);
    h = (HTABLE*)alias_set;
  }
  lef_add_to_stringtab(alias_set, key, (void*)data);
  return 0;
}


