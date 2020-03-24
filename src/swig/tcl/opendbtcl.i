%module opendbtcl

%{
#define SWIG_FILE_WITH_INIT
#include "geom.h"
#include "db.h"
#include "dbShape.h"
#include "dbViaParams.h"
#include "dbRtEdge.h"
#include "dbWireCodec.h"
#include "dbBlockCallBackObj.h"
#include "dbIterator.h"
#include "dbRtNode.h"
#include "dbTransform.h"
#include "dbWireGraph.h"
#include "dbBlockSet.h"
#include "dbMap.h"
#include "dbRtTree.h"
#include "dbCCSegSet.h"
#include "dbSet.h"
#include "dbTypes.h"
#include "geom.h"
#include "wOrder.h"

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
%apply int* OUTPUT {int* x, int* y};

%include "dbenums.i"
%include "parserenums.i"
%include "dbtypes.i"

%include "geom.h"
%include "db.h"
%include "polygon.i"

%include "dbhelpers.i"  
%include "dbdiff.i"

%include "dbViaParams.h"
%include "dbRtEdge.h"
%include "dbWireCodec.h"
%include "dbBlockCallBackObj.h"
%include "dbIterator.h"
%include "dbRtNode.h"
%include "dbTransform.h"
%include "dbWireGraph.h"
%include "dbBlockSet.h"
%include "dbRtTree.h"
%include "dbCCSegSet.h"
%include "wOrder.h"
