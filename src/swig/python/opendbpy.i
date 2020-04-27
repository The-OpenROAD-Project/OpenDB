%module opendbpy

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

%typemap(typecheck,precedence=SWIG_TYPECHECK_INTEGER) uint {
   $1 = PyInt_Check($input) ? 1 : 0;
}

%ignore odb::dbTechLayerAntennaRule::pwl_pair;
%ignore odb::dbTechLayerAntennaRule::getPAR_PWL() const;
%ignore odb::dbTechLayerAntennaRule::getCAR_PWL() const;
%ignore odb::dbTechLayerAntennaRule::getPSR_PWL() const;
%ignore odb::dbTechLayerAntennaRule::getCSR_PWL() const;
%ignore odb::dbTechLayerAntennaRule::getAreaDiffReduce_PWL() const;

%include "dbenums.i"
%include "parserenums.i"
%include "dbtypes.i"

%include "geom.h"
%include "db.h"

%include "dbhelpers.i"
%include "../tcl/dbdiff.i"

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
