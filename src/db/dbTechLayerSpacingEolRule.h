///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2020, OpenRoad Project
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// Generator Code Begin 1
#pragma once

#include "dbCore.h"
#include "odb.h"

// User Code Begin includes
// User Code End includes

namespace odb {

class dbIStream;
class dbOStream;
class dbDiff;
class _dbDatabase;
// User Code Begin Classes
class _dbTechLayer;
// User Code End Classes

struct dbTechLayerSpacingEolRuleFlags
{
  bool EXACTWIDTH : 1;
  bool WRONGDIRSPACING : 1;
  bool OPPOSITEWIDTH : 1;
  bool WITHIN : 1;
  bool WRONGDIRWITHIN : 1;
  bool SAMEMASK : 1;
  bool EXCEPTEXACTWIDTH : 1;
  bool FILLCONCAVECORNER : 1;
  bool WITHCUT : 1;
  bool CUTCLASS : 1;
  bool withCutABOVE : 1;
  bool ENCLOSUREEND : 1;
  bool ENCLOSUREENDWITHIN : 1;
  bool ENDPRLSPACING : 1;
  bool PRL : 1;
  bool ENDTOEND : 1;
  bool hasCutSpaces : 1;
  bool EXTENSION : 1;
  bool hasWrongDirExtension : 1;
  bool OTHERENDWIDTH : 1;
  bool MAXLENGTH : 1;
  bool MINLENGTH : 1;
  bool TWOSIDES : 1;
  bool EQUALRECTWIDTH : 1;
  bool PARALLELEDGE : 1;
  bool SUBTRACTEOLWIDTH : 1;
  bool PARPRL : 1;
  bool PARMINLENGTH : 1;
  bool TWOEDGES : 1;
  bool SAMEMETAL : 1;
  bool NONEOLCORNERONLY : 1;
  bool PARALLELSAMEMASK : 1;
  bool ENCLOSECUT : 1;
  bool BELOW : 1;
  bool ABOVE : 1;
  bool CUTSPACING : 1;
  bool ALLCUTS : 1;
  bool TOCONCAVECORNER : 1;
  bool MINADJACENTLENGTH : 1;
  bool hasTwoMinAdjLength : 1;
  bool TONOTCHLENGTH : 1;
  uint _spare_bits : 23;
};
// User Code Begin structs
// User Code End structs

class _dbTechLayerSpacingEolRule : public _dbObject
{
 public:
  // User Code Begin enums
  // User Code End enums

  dbTechLayerSpacingEolRuleFlags _flags;
  double                         eolSpace;
  double                         eolwidth;
  double                         wrongDirSpace;
  double                         oppositeWidth;
  double                         eolWithin;
  double                         wrongDirWithin;
  double                         exactWidth;
  double                         otherWidth;
  double                         fillTriangle;
  double                         cutClass;
  double                         withCutSpace;
  double                         enclosureEndWidth;
  double                         enclosureEndWithin;
  double                         endPrlSpace;
  double                         endPrl;
  double                         endToEndSpace;
  double                         oneCutSpace;
  double                         twoCutSpace;
  double                         extension;
  double                         wrongDirExtension;
  double                         otherEndWidth;
  double                         maxLength;
  double                         minLength;
  double                         parSpace;
  double                         parWithin;
  double                         parPrl;
  double                         parMinLength;
  double                         encloseDist;
  double                         cutToMetalSpace;
  double                         minAdjLength;
  double                         minAdjLength1;
  double                         minAdjLength2;
  double                         notchLength;

  // User Code Begin fields
  dbId<_dbTechLayer> _layer;
  // User Code End fields
  _dbTechLayerSpacingEolRule(_dbDatabase*, const _dbTechLayerSpacingEolRule& r);
  _dbTechLayerSpacingEolRule(_dbDatabase*);
  ~_dbTechLayerSpacingEolRule();
  bool operator==(const _dbTechLayerSpacingEolRule& rhs) const;
  bool operator!=(const _dbTechLayerSpacingEolRule& rhs) const
  {
    return !operator==(rhs);
  }
  bool operator<(const _dbTechLayerSpacingEolRule& rhs) const;
  void differences(dbDiff&                           diff,
                   const char*                       field,
                   const _dbTechLayerSpacingEolRule& rhs) const;
  void out(dbDiff& diff, char side, const char* field) const;
  // User Code Begin methods
  // User Code End methods
};
dbIStream& operator>>(dbIStream& stream, _dbTechLayerSpacingEolRule& obj);
dbOStream& operator<<(dbOStream& stream, const _dbTechLayerSpacingEolRule& obj);
// User Code Begin general
// User Code End general
}  // namespace odb
   // Generator Code End 1