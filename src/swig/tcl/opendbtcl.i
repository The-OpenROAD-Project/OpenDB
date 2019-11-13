%module opendbtcl

%{
#define SWIG_FILE_WITH_INIT
#include <stdio.h>
#include "db.h"
#include "dbShape.h"
#include "lefin.h"
#include "lefout.h"
#include "defin.h"
#include "defout.h"
#include "dbExtControl.h"
#include "dbViaParams.h"
#include "dbId.h"
#include "dbRtEdge.h"
#include "dbStream.h"
#include "dbWireCodec.h"
#include "dbBlockCallBackObj.h"
#include "dbIterator.h"
#include "dbRtNode.h"
#include "dbTransform.h"
#include "dbWireGraph.h"
#include "dbBlockSet.h"
#include "dbMap.h"
#include "dbRtTree.h"
#include "dbgdefines.h"
#include "dbCCSegSet.h"
#include "dbSet.h"
#include "dbTypes.h"
#include "geom.h"
using namespace odb;
%}

%include <stl.i>
%include <typemaps.i>
%include <std_string.i>
%include <std_vector.i>
%include <std_pair.i>


%typemap(in) (uint) = (int);
%typemap(out) (uint) = (int);
%typemap(out) (uint64) = (long);

%include "dbenums.i"
%include "parserenums.i"
%include "dbtypes.i"


%include "geom.h"
%include "db.h"


%include "lefin.h"
%include "lefout.h"
%include "defin.h"
%include "defout.h"
%include "dbhelpers.i"  
%include "dbExtControl.h"
%include "dbViaParams.h"

%include "dbtable2.h"
%include "dbId.h"
%include "dbRtEdge.h"
%include "dbStream.h"
%include "dbWireCodec.h"
%include "dbBlockCallBackObj.h"
%include "dbIterator.h"
%include "dbRtNode.h"
%include "dbTransform.h"
%include "dbWireGraph.h"
%include "dbBlockSet.h"
%include "dbMap.h"
%include "dbRtTree.h"
%include "dbgdefines.h"
%include "dbCCSegSet.h"
// Support file operations
FILE *fopen(const char *name, const char *mode);
int fclose(FILE *);
int fgetc(FILE *);