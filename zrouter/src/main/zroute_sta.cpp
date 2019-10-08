// Copyright (c) 2001, 2002, 2003, 2004, 2005
// Parallax Software, Inc.
// All rights reserved.
// 
// No part of this document may be copied, transmitted or
// disclosed in any form or fashion without the express 
// written consent of Parallax Software, Inc.
// orig Id: StaMain.cc 3111 2005-01-12 05:17:06Z  

#include "ads.h"
#include <tcl.h>
#include <StringUtil.hh>
#include <ReportTcl.hh>
#include <StaZroute.h>
#include <Error.hh>

extern "C" void ZRouter_InitSta(Tcl_Interp *);
extern "C" void Sta_Init(Tcl_Interp *);
void ZRouter_Sta_evalTclInit(Tcl_Interp *interp);

namespace sta
{
extern const char *sta_tcl_inits[];
}


using namespace sta;

extern int sta_swig_init(Tcl_Interp *);

// Mattias Nov 9/06
void setZRSta(Tcl_Interp *sta_interp)
{
    //old instance delete by "sta::delete_all_memory"
    StaZroute *staz = new StaZroute;
    StaZroute::setStaZroute(staz);

    Sta *sta = (Sta *)staz;
    sta->makeComponents();
    Sta::setSta(sta);

    sta->setReport(sta::makeTclReport(sta_interp));
    sta->updateComponentsState();
}

Tcl_Interp *mysta_interp;
void resetZRSta()
{
    setZRSta(mysta_interp);
}

void ZRouter_InitSta(Tcl_Interp *interp)
{
  Tcl_Interp *sta_interp = Tcl_CreateSlave(interp,"sta_interp",0);
  if (! sta_interp) {
     printf("failed to create sta_interp\n");
     return;
  }
  Sta_Init(sta_interp);
  mysta_interp = sta_interp;

  setZRSta(sta_interp);

  ZRouter_Sta_evalTclInit(sta_interp);

const char *stap = "proc sta { cmd args } {\n"
    "    if { (\[info exists ::env(DRYRUN)]) && ($::env(DRYRUN) == 1) } {\n"
    "        return 0;\n"
    "    }\n"
    "    set y [concat \\{ $cmd $args \\}]\n"
    "    debug ZTM:A \"ZTM:A sta $y\"\n"
    "    debug ZTC:A \"ZTC:A sta $y\"\n"
    "    uplevel sta_interp eval $y\n}";
  if (Tcl_Eval(interp, stap) != TCL_OK) {
    printf("failed to eval this:\n%s\n",stap);
  }
  const char *put = "interp alias sta_interp puts \"\" puts\n";
  if (Tcl_Eval(interp, put) != TCL_OK) {
    printf("failed to eval this:\n%s\n",put);
  }

}
/*
static void my_print_u(char *u)
{
  char line[256];
  char *p;
  while (*u) {
    p = line;
    while (p<line+80 && *u) {
      *(p++) = *(u++);
      if (p[-1]=='\n') break;
    }
    *p = '\0';
    printf("%s\n",line);
  }
}
*/

// TCL init files are encoded into the string init using the three
// digit decimal equivalent for each ascii character.  This function
// unencodes the string and evals it.  This packages the TCL init
// files as part of the executable so they don't have to be shipped as
// separate files that have to be located and loaded at run time.
void
ZRouter_Sta_evalTclInit(Tcl_Interp *interp)
{
  const char **inits = sta::sta_tcl_inits;
  size_t length = 0;
  for (const char **e = inits; *e; e++) {
    const char *init = *e;
    length += strlen(init);
  }
  char *unencoded = new char[length / 3 + 1];
  char *u = unencoded;
  for (const char **e = inits; *e; e++) {
    const char *init = *e;
    size_t init_length = strlen(init);
    for (const char *s = init; s < &init[init_length]; s += 3) {
      char code[4] = {s[0], s[1], s[2], '\0'};
      char ch = atoi(code);
      *u++ = ch;
    }
  }
  *u = '\0';

  // my_print_u(unencoded);

  if (Tcl_Eval(interp, unencoded) != TCL_OK) {
    const char *tcl_err = Tcl_GetStringResult(interp);
    fprintf(stderr, "Error: sta TCL init script: %s.\n", tcl_err);
  } else {
    // printf("Tcl_Eval(interp, unencoded) returned TCL_OK\n");
  }
  delete [] unencoded;
}


