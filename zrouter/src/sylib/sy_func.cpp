#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sy_lib.h>

BEGIN_NAMESPACE_ADS

extern void sy_err(char *);

//////////////////////////////////////////////////////////////////////////
// parsing

static char Sy_func_vars[32*256];
static char *Sy_func_var[32];

static void sy_func_set_unate(sy_func *f);
static sy_func *sy_fdb_add(sy_func *f);

int sy_func_var_cmp(const void *a,const void *b)
{
  int ai = *(int*)a;
  int bi = *(int*)b;
  return strcmp(Sy_func_var[ai],Sy_func_var[bi]);
}

static void  sy_func_parse_collapse(int **sV,int *opV,int k,int sN)
{
  int m;
  int *s = sV[k];
  int *x;
  while (*s != 3) s++;
  for (m=k+1;m<=sN;m++) {
    x = sV[m];
    while (*x != 3) *(s++) = *(x++);
  }
  for (m=sN-1;m>=k;--m) *(s++) = opV[m];
  *s = 3;
}

static int sy_func_parse(int* tok,int nt,int *t2,int *n2)
{
  int _sV[32*1024];
  int *sV[32],*s;
  int sN,tk,j,k;
  int *x;
  for (j=0;j<32;j++) sV[j] = _sV+1024*j;
  int opV[32];
  sN = 0;
  s = sV[sN];
  s[0] = 3;
  opV[0] = 0;
  for (j=0;j<nt;j++) {
    tk = tok[j];
    if (tk == '|') tk = '+';
    if (s[0]!=3 && (tk<=0 || tk=='!' || tk=='(')) {
      // implicit and
      j--; tk= '*';
    }
    if (tk<=0) {
      if (sN>0 && opV[sN-1]=='!') {
        s = sV[--sN];
        s[0] = tk;
        s[1] = '!';
        s[2] = 3;
        opV[sN] = 0;
      } else {
        s[0] = tk;
        s[1] = 3;
      }
    } else if (tk=='!') {
      if (sN>0 && opV[sN-1]=='!') {
        s = sV[--sN];
        s[0] = 3;
        opV[sN] = 0;
      } else {
        if (s[0]!=3)
          return 0;
        opV[sN] = tk;
        s = sV[++sN];
        s[0] = 3;
        opV[sN] = 0;
      }
    } else if (tk=='+') {
      k = sN;
      while (k>0 && (opV[k-1]=='+' || opV[k-1]=='*' || opV[k-1]=='^')) k--;
      if (k<sN) {
        sy_func_parse_collapse(sV,opV,k,sN);
        sN = k;
      }
      opV[sN] = '+';
      s = sV[++sN]; s[0] = 3; opV[sN] = 0;
    } else if (tk=='*') {
      k = sN;
      while (k>0 && (opV[k-1]=='*' || opV[k-1]=='^')) k--;
      if (k<sN) {
        sy_func_parse_collapse(sV,opV,k,sN);
        sN = k;
      }
      opV[sN] = '*';
      s = sV[++sN]; s[0] = 3; opV[sN] = 0;
    } else if (tk=='^') {
      k = sN;
      while (k>0 && opV[k-1]=='^') k--;
      if (k<sN) {
        sy_func_parse_collapse(sV,opV,k,sN);
        sN = k;
      }
      opV[sN] = '^';
      s = sV[++sN]; s[0] = 3; opV[sN] = 0;
    } else if (tk=='\'') {
      if (s[0]==3)
        return 0;
      x = s; while (*x!=3) x++;
      *(x++) = '!';
      *x = 3;
    } else if (tk==')') {
      k = sN;
      while (k>0 && opV[k-1]!='(') k--;
      if (k==0)  // no matching paren
        return 0;
      sy_func_parse_collapse(sV,opV,k,sN);
      s = sV[k-1]; x = sV[k]; while (*x != 3) *(s++) = *(x++); *s = 3;
      sN = k-1;
      s = sV[sN];
      opV[sN] = 0;
    } else if (tk=='(') {
      if (s[0]!=3)
        return 0;
      opV[sN] = '(';
      s = sV[++sN]; s[0] = 3; opV[sN] = 0;
    }

#if 0
    notice(0,"------------\n");
    for (k=0;k<=sN;k++) {
      for (m=0;sV[k][m]!=3;m++) {
        if (sV[k][m]<=0) notice(0," V%d",-sV[k][m]);
        else notice(0," %c",sV[k][m]);
      }
      notice(0,"  : %c\n",opV[k]?opV[k]:'-');
    }
    notice(0,"------------\n");
#endif
  } 
  if (sN>0) {
    sy_func_parse_collapse(sV,opV,0,sN);
    s = sV[0];
  }
  for (j=0;s[j]!=3;j++) t2[j] = s[j];
  *n2 = j;
  return 1;
}

sy_func * sy_func_read(char *line)
{
  char ch,*pc,*p2;
  int j,n2,nv=0,nt=0,err=0;
  int tok[256];
  int t2[256];
  int vord[32],vord2[32];
  int _unate[32];
  sy_func _ff, *f = &_ff, *rf;
  f->var = Sy_func_var;
  f->unate = _unate;
  f->tok = t2;
  f->orig = line;
  f->sta_func_expr = NULL;
  for (pc=f->orig;*pc;pc++) {
    ch = *pc;
    if ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z')) {
      Sy_func_var[nv] = Sy_func_vars+32*nv;
      p2 = Sy_func_var[nv++];
      *(p2++) = ch;
      ch = pc[1];
      while ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z')
             || (ch>='0' && ch<='9') || (ch=='_') ) {
        *(p2++) = ch;
        pc++;
        ch = pc[1];
      }
      *p2 = '\0';
      for (j=0;j<nv-1;j++) if (!strcmp(Sy_func_var[j],Sy_func_var[nv-1])) { --nv; break; }
      tok[nt++] = -j;
    } else {
      tok[nt++] = (int)ch;
    } 
  }
  if (err) {
    notice(0,"error parsing %s\n",line);
    nv = 0;
  }
  if (!nv) return NULL;
  f->nv = nv;

  for (j=0;j<nv;j++) vord[j] = j;
  qsort(vord,nv,sizeof(int),sy_func_var_cmp);
  for (j=0;j<nv;j++) Sy_func_var[j] = Sy_func_vars+256*vord[j];
  for (j=0;j<nv;j++) vord2[vord[j]]=j;
  for (j=0;j<nt;j++) if (tok[j]<=0) tok[j]= -vord2[-tok[j]];

  if (!sy_func_parse(tok,nt,t2,&n2)) {
    sy_err("error parsing function");
    return NULL;
  }

  f->nt = n2;
  sy_func_set_unate(f);
  rf = sy_fdb_add(f);
  return rf;
}

static void sy_func_set_unate(sy_func *f)
{
  struct xxx { uint pos; uint neg; } st[32], *cur;
  int stN,tk,j,isp,isn;
  uint tmp;
  stN = 0; cur = NULL;
  for (j=0;j<f->nt;j++) {
    tk = f->tok[j];
    if (tk<=0) {
      cur = st+stN++;
      cur->pos = (1<<(-tk));
      cur->neg = 0;
    } else if (tk=='!') {
      tmp = cur->pos; cur->pos = cur->neg; cur->neg = tmp;
    } else if (tk=='+' || tk=='*') {
      --stN; cur = st+(stN-1);
      cur->pos |= (cur+1)->pos;
      cur->neg |= (cur+1)->neg;
    } else if (tk=='^') {
      --stN; cur = st+(stN-1);
      cur->pos = (cur->pos|cur->neg|(cur+1)->pos|(cur+1)->neg);
      cur->neg = cur->pos;
    }
  }
  if (stN!=1) {
    notice(0,"error in sy_func_set_unate\n");
    for (j=0;j<f->nv;j++) f->unate[j]=2;
    return;
  }
  for (j=0;j<f->nv;j++) {
    isp = (cur->pos >> j)%2;
    isn = (cur->neg >> j)%2;
    if (!isp && !isn) notice(0,"no effect %s\n",f->var[j]);
    f->unate[j] = (isp&&isn) ? 0 : (1+isn);
    // 0 = non_unate, 1 = positive_unate, 2 = negative_unate
  }
}

int sy_func_eval(sy_func *f,uint ev,int *err)
{
  int stV[32];
  int j, tk, stN = -1;
  *err =0;
  for (j=0;j<f->nt;j++) {
    tk = f->tok[j];
    if (tk<=0) {
      tk= -tk;
      stV[++stN] = (ev>>tk)%2;
    } else if (tk=='!') {
      if (stN<0) { *err=1; return 0; }
      stV[stN] = (stV[stN]?0:1);
    } else {
      --stN;
      if (stN<0) { *err=1; return 0; }
      if (tk=='+') {
        stV[stN] = (stV[stN] || stV[stN+1]);
      } else if (tk=='*') {
        stV[stN] = (stV[stN] && stV[stN+1]);
      } else if (tk=='^') {
        stV[stN] = ((stV[stN] && !stV[stN+1]) || (!stV[stN] && stV[stN+1]));
      }
    }
  }
  if (stN!=0) { *err=1; return 0; }
  return stV[0];
}


static int sy_func_match(sy_func *f,sy_func *g)
{
  uint ev;
  uint evmax = 1<<(f->nv);
  int err,fe,ge;
  for (ev=0;ev<evmax;ev++) {
    fe = sy_func_eval(f,ev,&err);
    if (err) return 0;
    ge = sy_func_eval(g,ev,&err);
    if (err || fe!=ge) return 0;
  }
  return 1;
}

int sy_func_get_unate(sy_func *f,char *pin,int *unate)
{
  *unate = 0;
  if (!f) return 0;
  int j;
  for (j=0;j<f->nv;j++) if (!strcmp(pin,f->var[j])) {
    *unate = f->unate[j];
    return 1;
  }
  return 0;
}


//////////////////////////////////////////////////////////////////////////
// fdb

typedef struct sy_func_list sy_func_list;
struct sy_func_list {
  sy_func *f;
  sy_func_list *next;
};
typedef struct sy_func_list2 sy_func_list2;
struct sy_func_list2 {
  sy_func_list *list;
  sy_func_list2 *next;
};
typedef struct sy_fdb_iter sy_fdb_iter;
struct sy_fdb_iter {
  int nv;
  sy_func_list2 *y;
  sy_func_list *x;
};

static sy_func_list2 *Sy_fdb[32];
static sy_fdb_iter Sy_fdb_it = { 0, NULL, NULL };
static int Sy_fdb_initd = 0;

void sy_fdb_init(void)
{
  int j;
  if (Sy_fdb_initd) return;
  Sy_fdb_initd = 1;
  for (j=0;j<32;j++) Sy_fdb[j] = NULL;
}


static sy_func *sy_fdb_add(sy_func *fin)
{
  int nv = fin->nv;
  int j;
  sy_func *f;
  sy_func_list *x;
  sy_func_list2 *y = Sy_fdb[nv];
  if (!Sy_fdb_initd) sy_fdb_init();
  if (y) {
    for ( ;y;y=y->next) {
      f = y->list->f;
      for (j=0;j<nv;j++) {
        if (strcmp(fin->var[j],f->var[j])) break;
      }
      if (j==nv) break;
    }
    if (y) for (x=y->list; x; x=x->next) {
      f = x->f;
      for (j=0;j<nv;j++) if (f->unate[j]!=fin->unate[j]) break;
      if (j==nv)
        if (sy_func_match(fin,f))
          return f;
    }
  }
  if (!y) {
    y = (sy_func_list2 *)malloc(sizeof(sy_func_list2));
    y->list = NULL;
    y->next = Sy_fdb[nv];
    Sy_fdb[nv] = y;
  }
  x = (sy_func_list *)malloc(sizeof(sy_func_list));
  x->next = y->list;
  y->list = x;

  f = (sy_func*)malloc(sizeof(sy_func));
  f->nv = nv;
  f->var = (char**)malloc(nv*sizeof(char*));
  for (j=0;j<nv;j++) f->var[j] = strdup(fin->var[j]);
  f->unate = (int*)malloc(nv*sizeof(int));
  for (j=0;j<nv;j++) f->unate[j] = fin->unate[j];
  f->nt = fin->nt;
  f->tok = (int*)malloc(f->nt*sizeof(int));
  for (j=0;j<f->nt;j++) f->tok[j] = fin->tok[j];
  f->orig = strdup(fin->orig);
  f->sta_func_expr = NULL;

  x->f = f;
  return f;
}

void sy_fdb_start(void)
{
  Sy_fdb_it.nv = 0;
  Sy_fdb_it.y = Sy_fdb[0];
  if (Sy_fdb_it.y)
    Sy_fdb_it.x = Sy_fdb_it.y->list;
  else
    Sy_fdb_it.x = NULL;
}

sy_func *sy_fdb_next(void)
{
  sy_func *f = NULL;
  if (Sy_fdb_it.nv == 32)
    return NULL;
  if (!Sy_fdb_it.x) {
    Sy_fdb_it.nv ++;
    while (Sy_fdb_it.nv<32 && !Sy_fdb[Sy_fdb_it.nv])
      Sy_fdb_it.nv ++;
    if (Sy_fdb_it.nv == 32)
      return NULL;
    Sy_fdb_it.y = Sy_fdb[Sy_fdb_it.nv];
    Sy_fdb_it.x = Sy_fdb_it.y->list;
  }
  f = Sy_fdb_it.x->f;
  Sy_fdb_it.x = Sy_fdb_it.x->next;
  if (!Sy_fdb_it.x) {
    Sy_fdb_it.y = Sy_fdb_it.y->next;
    if (Sy_fdb_it.y)
      Sy_fdb_it.x = Sy_fdb_it.y->list;
  }
  return f;
}

END_NAMESPACE_ADS
