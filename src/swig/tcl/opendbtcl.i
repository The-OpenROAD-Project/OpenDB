%module opendbtcl

%{
#define SWIG_FILE_WITH_INIT
#include "db.h"
#include "lefin.h"
#include "lefout.h"
#include "defin.h"
#include "defout.h"
#include "dbExtControl.h"
#include "dbViaParams.h"
#include "dbtable2.h"
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
#include "dbObject.h"
#include "dbSet.h"
#include "dbtable1.h"
using namespace odb;
%}

%include <stl.i>
%include <typemaps.i>
%include <std_string.i>
%include <std_vector.i>
%include <std_pair.i>
%typemap(cstype) unsigned long    "uint"
%include "dbtypes.i"


%include "db.h"
%include "lefin.h"
%include "lefout.h"
%include "defin.h"
%include "defout.h"
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
%include "dbObject.h"
%include "dbtable1.h"

