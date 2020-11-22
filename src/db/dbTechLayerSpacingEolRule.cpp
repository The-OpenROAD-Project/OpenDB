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
#include "dbTechLayerSpacingEolRule.h"

#include "db.h"
#include "dbDatabase.h"
#include "dbDiff.hpp"
#include "dbTable.h"
#include "dbTable.hpp"

// User Code Begin includes
#include "dbTech.h"
#include "dbTechLayer.h"
// User Code End includes
namespace odb {

// User Code Begin definitions
// User Code End definitions
template class dbTable<_dbTechLayerSpacingEolRule>;

bool _dbTechLayerSpacingEolRule::operator==(
    const _dbTechLayerSpacingEolRule& rhs) const
{
  if (_flags.EXACTWIDTH != rhs._flags.EXACTWIDTH)
    return false;

  if (_flags.WRONGDIRSPACING != rhs._flags.WRONGDIRSPACING)
    return false;

  if (_flags.OPPOSITEWIDTH != rhs._flags.OPPOSITEWIDTH)
    return false;

  if (_flags.WITHIN != rhs._flags.WITHIN)
    return false;

  if (_flags.WRONGDIRWITHIN != rhs._flags.WRONGDIRWITHIN)
    return false;

  if (_flags.SAMEMASK != rhs._flags.SAMEMASK)
    return false;

  if (_flags.EXCEPTEXACTWIDTH != rhs._flags.EXCEPTEXACTWIDTH)
    return false;

  if (_flags.FILLCONCAVECORNER != rhs._flags.FILLCONCAVECORNER)
    return false;

  if (_flags.WITHCUT != rhs._flags.WITHCUT)
    return false;

  if (_flags.CUTCLASS != rhs._flags.CUTCLASS)
    return false;

  if (_flags.withCutABOVE != rhs._flags.withCutABOVE)
    return false;

  if (_flags.ENCLOSUREEND != rhs._flags.ENCLOSUREEND)
    return false;

  if (_flags.ENCLOSUREENDWITHIN != rhs._flags.ENCLOSUREENDWITHIN)
    return false;

  if (_flags.ENDPRLSPACING != rhs._flags.ENDPRLSPACING)
    return false;

  if (_flags.PRL != rhs._flags.PRL)
    return false;

  if (_flags.ENDTOEND != rhs._flags.ENDTOEND)
    return false;

  if (_flags.hasCutSpaces != rhs._flags.hasCutSpaces)
    return false;

  if (_flags.EXTENSION != rhs._flags.EXTENSION)
    return false;

  if (_flags.hasWrongDirExtension != rhs._flags.hasWrongDirExtension)
    return false;

  if (_flags.OTHERENDWIDTH != rhs._flags.OTHERENDWIDTH)
    return false;

  if (_flags.MAXLENGTH != rhs._flags.MAXLENGTH)
    return false;

  if (_flags.MINLENGTH != rhs._flags.MINLENGTH)
    return false;

  if (_flags.TWOSIDES != rhs._flags.TWOSIDES)
    return false;

  if (_flags.EQUALRECTWIDTH != rhs._flags.EQUALRECTWIDTH)
    return false;

  if (_flags.PARALLELEDGE != rhs._flags.PARALLELEDGE)
    return false;

  if (_flags.SUBTRACTEOLWIDTH != rhs._flags.SUBTRACTEOLWIDTH)
    return false;

  if (_flags.PARPRL != rhs._flags.PARPRL)
    return false;

  if (_flags.PARMINLENGTH != rhs._flags.PARMINLENGTH)
    return false;

  if (_flags.TWOEDGES != rhs._flags.TWOEDGES)
    return false;

  if (_flags.SAMEMETAL != rhs._flags.SAMEMETAL)
    return false;

  if (_flags.NONEOLCORNERONLY != rhs._flags.NONEOLCORNERONLY)
    return false;

  if (_flags.PARALLELSAMEMASK != rhs._flags.PARALLELSAMEMASK)
    return false;

  if (_flags.ENCLOSECUT != rhs._flags.ENCLOSECUT)
    return false;

  if (_flags.BELOW != rhs._flags.BELOW)
    return false;

  if (_flags.ABOVE != rhs._flags.ABOVE)
    return false;

  if (_flags.CUTSPACING != rhs._flags.CUTSPACING)
    return false;

  if (_flags.ALLCUTS != rhs._flags.ALLCUTS)
    return false;

  if (_flags.TOCONCAVECORNER != rhs._flags.TOCONCAVECORNER)
    return false;

  if (_flags.MINADJACENTLENGTH != rhs._flags.MINADJACENTLENGTH)
    return false;

  if (_flags.hasTwoMinAdjLength != rhs._flags.hasTwoMinAdjLength)
    return false;

  if (_flags.TONOTCHLENGTH != rhs._flags.TONOTCHLENGTH)
    return false;

  if (eolSpace != rhs.eolSpace)
    return false;

  if (eolwidth != rhs.eolwidth)
    return false;

  if (wrongDirSpace != rhs.wrongDirSpace)
    return false;

  if (oppositeWidth != rhs.oppositeWidth)
    return false;

  if (eolWithin != rhs.eolWithin)
    return false;

  if (wrongDirWithin != rhs.wrongDirWithin)
    return false;

  if (exactWidth != rhs.exactWidth)
    return false;

  if (otherWidth != rhs.otherWidth)
    return false;

  if (fillTriangle != rhs.fillTriangle)
    return false;

  if (cutClass != rhs.cutClass)
    return false;

  if (withCutSpace != rhs.withCutSpace)
    return false;

  if (enclosureEndWidth != rhs.enclosureEndWidth)
    return false;

  if (enclosureEndWithin != rhs.enclosureEndWithin)
    return false;

  if (endPrlSpace != rhs.endPrlSpace)
    return false;

  if (endPrl != rhs.endPrl)
    return false;

  if (endToEndSpace != rhs.endToEndSpace)
    return false;

  if (oneCutSpace != rhs.oneCutSpace)
    return false;

  if (twoCutSpace != rhs.twoCutSpace)
    return false;

  if (extension != rhs.extension)
    return false;

  if (wrongDirExtension != rhs.wrongDirExtension)
    return false;

  if (otherEndWidth != rhs.otherEndWidth)
    return false;

  if (maxLength != rhs.maxLength)
    return false;

  if (minLength != rhs.minLength)
    return false;

  if (parSpace != rhs.parSpace)
    return false;

  if (parWithin != rhs.parWithin)
    return false;

  if (parPrl != rhs.parPrl)
    return false;

  if (parMinLength != rhs.parMinLength)
    return false;

  if (encloseDist != rhs.encloseDist)
    return false;

  if (cutToMetalSpace != rhs.cutToMetalSpace)
    return false;

  if (minAdjLength != rhs.minAdjLength)
    return false;

  if (minAdjLength1 != rhs.minAdjLength1)
    return false;

  if (minAdjLength2 != rhs.minAdjLength2)
    return false;

  if (notchLength != rhs.notchLength)
    return false;

  // User Code Begin ==
  // User Code End ==
  return true;
}
bool _dbTechLayerSpacingEolRule::operator<(
    const _dbTechLayerSpacingEolRule& rhs) const
{
  // User Code Begin <
  // User Code End <
  return true;
}
void _dbTechLayerSpacingEolRule::differences(
    dbDiff&                           diff,
    const char*                       field,
    const _dbTechLayerSpacingEolRule& rhs) const
{
  DIFF_BEGIN

  DIFF_FIELD(_flags.EXACTWIDTH);

  DIFF_FIELD(_flags.WRONGDIRSPACING);

  DIFF_FIELD(_flags.OPPOSITEWIDTH);

  DIFF_FIELD(_flags.WITHIN);

  DIFF_FIELD(_flags.WRONGDIRWITHIN);

  DIFF_FIELD(_flags.SAMEMASK);

  DIFF_FIELD(_flags.EXCEPTEXACTWIDTH);

  DIFF_FIELD(_flags.FILLCONCAVECORNER);

  DIFF_FIELD(_flags.WITHCUT);

  DIFF_FIELD(_flags.CUTCLASS);

  DIFF_FIELD(_flags.withCutABOVE);

  DIFF_FIELD(_flags.ENCLOSUREEND);

  DIFF_FIELD(_flags.ENCLOSUREENDWITHIN);

  DIFF_FIELD(_flags.ENDPRLSPACING);

  DIFF_FIELD(_flags.PRL);

  DIFF_FIELD(_flags.ENDTOEND);

  DIFF_FIELD(_flags.hasCutSpaces);

  DIFF_FIELD(_flags.EXTENSION);

  DIFF_FIELD(_flags.hasWrongDirExtension);

  DIFF_FIELD(_flags.OTHERENDWIDTH);

  DIFF_FIELD(_flags.MAXLENGTH);

  DIFF_FIELD(_flags.MINLENGTH);

  DIFF_FIELD(_flags.TWOSIDES);

  DIFF_FIELD(_flags.EQUALRECTWIDTH);

  DIFF_FIELD(_flags.PARALLELEDGE);

  DIFF_FIELD(_flags.SUBTRACTEOLWIDTH);

  DIFF_FIELD(_flags.PARPRL);

  DIFF_FIELD(_flags.PARMINLENGTH);

  DIFF_FIELD(_flags.TWOEDGES);

  DIFF_FIELD(_flags.SAMEMETAL);

  DIFF_FIELD(_flags.NONEOLCORNERONLY);

  DIFF_FIELD(_flags.PARALLELSAMEMASK);

  DIFF_FIELD(_flags.ENCLOSECUT);

  DIFF_FIELD(_flags.BELOW);

  DIFF_FIELD(_flags.ABOVE);

  DIFF_FIELD(_flags.CUTSPACING);

  DIFF_FIELD(_flags.ALLCUTS);

  DIFF_FIELD(_flags.TOCONCAVECORNER);

  DIFF_FIELD(_flags.MINADJACENTLENGTH);

  DIFF_FIELD(_flags.hasTwoMinAdjLength);

  DIFF_FIELD(_flags.TONOTCHLENGTH);

  DIFF_FIELD(eolSpace);

  DIFF_FIELD(eolwidth);

  DIFF_FIELD(wrongDirSpace);

  DIFF_FIELD(oppositeWidth);

  DIFF_FIELD(eolWithin);

  DIFF_FIELD(wrongDirWithin);

  DIFF_FIELD(exactWidth);

  DIFF_FIELD(otherWidth);

  DIFF_FIELD(fillTriangle);

  DIFF_FIELD(cutClass);

  DIFF_FIELD(withCutSpace);

  DIFF_FIELD(enclosureEndWidth);

  DIFF_FIELD(enclosureEndWithin);

  DIFF_FIELD(endPrlSpace);

  DIFF_FIELD(endPrl);

  DIFF_FIELD(endToEndSpace);

  DIFF_FIELD(oneCutSpace);

  DIFF_FIELD(twoCutSpace);

  DIFF_FIELD(extension);

  DIFF_FIELD(wrongDirExtension);

  DIFF_FIELD(otherEndWidth);

  DIFF_FIELD(maxLength);

  DIFF_FIELD(minLength);

  DIFF_FIELD(parSpace);

  DIFF_FIELD(parWithin);

  DIFF_FIELD(parPrl);

  DIFF_FIELD(parMinLength);

  DIFF_FIELD(encloseDist);

  DIFF_FIELD(cutToMetalSpace);

  DIFF_FIELD(minAdjLength);

  DIFF_FIELD(minAdjLength1);

  DIFF_FIELD(minAdjLength2);

  DIFF_FIELD(notchLength);

  // User Code Begin differences
  // User Code End differences
  DIFF_END
}
void _dbTechLayerSpacingEolRule::out(dbDiff&     diff,
                                     char        side,
                                     const char* field) const
{
  DIFF_OUT_BEGIN

  DIFF_OUT_FIELD(_flags.EXACTWIDTH);

  DIFF_OUT_FIELD(_flags.WRONGDIRSPACING);

  DIFF_OUT_FIELD(_flags.OPPOSITEWIDTH);

  DIFF_OUT_FIELD(_flags.WITHIN);

  DIFF_OUT_FIELD(_flags.WRONGDIRWITHIN);

  DIFF_OUT_FIELD(_flags.SAMEMASK);

  DIFF_OUT_FIELD(_flags.EXCEPTEXACTWIDTH);

  DIFF_OUT_FIELD(_flags.FILLCONCAVECORNER);

  DIFF_OUT_FIELD(_flags.WITHCUT);

  DIFF_OUT_FIELD(_flags.CUTCLASS);

  DIFF_OUT_FIELD(_flags.withCutABOVE);

  DIFF_OUT_FIELD(_flags.ENCLOSUREEND);

  DIFF_OUT_FIELD(_flags.ENCLOSUREENDWITHIN);

  DIFF_OUT_FIELD(_flags.ENDPRLSPACING);

  DIFF_OUT_FIELD(_flags.PRL);

  DIFF_OUT_FIELD(_flags.ENDTOEND);

  DIFF_OUT_FIELD(_flags.hasCutSpaces);

  DIFF_OUT_FIELD(_flags.EXTENSION);

  DIFF_OUT_FIELD(_flags.hasWrongDirExtension);

  DIFF_OUT_FIELD(_flags.OTHERENDWIDTH);

  DIFF_OUT_FIELD(_flags.MAXLENGTH);

  DIFF_OUT_FIELD(_flags.MINLENGTH);

  DIFF_OUT_FIELD(_flags.TWOSIDES);

  DIFF_OUT_FIELD(_flags.EQUALRECTWIDTH);

  DIFF_OUT_FIELD(_flags.PARALLELEDGE);

  DIFF_OUT_FIELD(_flags.SUBTRACTEOLWIDTH);

  DIFF_OUT_FIELD(_flags.PARPRL);

  DIFF_OUT_FIELD(_flags.PARMINLENGTH);

  DIFF_OUT_FIELD(_flags.TWOEDGES);

  DIFF_OUT_FIELD(_flags.SAMEMETAL);

  DIFF_OUT_FIELD(_flags.NONEOLCORNERONLY);

  DIFF_OUT_FIELD(_flags.PARALLELSAMEMASK);

  DIFF_OUT_FIELD(_flags.ENCLOSECUT);

  DIFF_OUT_FIELD(_flags.BELOW);

  DIFF_OUT_FIELD(_flags.ABOVE);

  DIFF_OUT_FIELD(_flags.CUTSPACING);

  DIFF_OUT_FIELD(_flags.ALLCUTS);

  DIFF_OUT_FIELD(_flags.TOCONCAVECORNER);

  DIFF_OUT_FIELD(_flags.MINADJACENTLENGTH);

  DIFF_OUT_FIELD(_flags.hasTwoMinAdjLength);

  DIFF_OUT_FIELD(_flags.TONOTCHLENGTH);

  DIFF_OUT_FIELD(eolSpace);

  DIFF_OUT_FIELD(eolwidth);

  DIFF_OUT_FIELD(wrongDirSpace);

  DIFF_OUT_FIELD(oppositeWidth);

  DIFF_OUT_FIELD(eolWithin);

  DIFF_OUT_FIELD(wrongDirWithin);

  DIFF_OUT_FIELD(exactWidth);

  DIFF_OUT_FIELD(otherWidth);

  DIFF_OUT_FIELD(fillTriangle);

  DIFF_OUT_FIELD(cutClass);

  DIFF_OUT_FIELD(withCutSpace);

  DIFF_OUT_FIELD(enclosureEndWidth);

  DIFF_OUT_FIELD(enclosureEndWithin);

  DIFF_OUT_FIELD(endPrlSpace);

  DIFF_OUT_FIELD(endPrl);

  DIFF_OUT_FIELD(endToEndSpace);

  DIFF_OUT_FIELD(oneCutSpace);

  DIFF_OUT_FIELD(twoCutSpace);

  DIFF_OUT_FIELD(extension);

  DIFF_OUT_FIELD(wrongDirExtension);

  DIFF_OUT_FIELD(otherEndWidth);

  DIFF_OUT_FIELD(maxLength);

  DIFF_OUT_FIELD(minLength);

  DIFF_OUT_FIELD(parSpace);

  DIFF_OUT_FIELD(parWithin);

  DIFF_OUT_FIELD(parPrl);

  DIFF_OUT_FIELD(parMinLength);

  DIFF_OUT_FIELD(encloseDist);

  DIFF_OUT_FIELD(cutToMetalSpace);

  DIFF_OUT_FIELD(minAdjLength);

  DIFF_OUT_FIELD(minAdjLength1);

  DIFF_OUT_FIELD(minAdjLength2);

  DIFF_OUT_FIELD(notchLength);

  // User Code Begin out
  // User Code End out
  DIFF_END
}
_dbTechLayerSpacingEolRule::_dbTechLayerSpacingEolRule(_dbDatabase* db)
{
  // User Code Begin constructor
  _flags.EXACTWIDTH           = 0;
  _flags.WRONGDIRSPACING      = 0;
  _flags.OPPOSITEWIDTH        = 0;
  _flags.WITHIN               = 0;
  _flags.WRONGDIRWITHIN       = 0;
  _flags.SAMEMASK             = 0;
  _flags.EXCEPTEXACTWIDTH     = 0;
  _flags.FILLCONCAVECORNER    = 0;
  _flags.WITHCUT              = 0;
  _flags.CUTCLASS             = 0;
  _flags.withCutABOVE         = 0;
  _flags.ENCLOSUREEND         = 0;
  _flags.ENCLOSUREENDWITHIN   = 0;
  _flags.ENDPRLSPACING        = 0;
  _flags.PRL                  = 0;
  _flags.ENDTOEND             = 0;
  _flags.hasCutSpaces         = 0;
  _flags.EXTENSION            = 0;
  _flags.hasWrongDirExtension = 0;
  _flags.OTHERENDWIDTH        = 0;
  _flags.MAXLENGTH            = 0;
  _flags.MINLENGTH            = 0;
  _flags.TWOSIDES             = 0;
  _flags.EQUALRECTWIDTH       = 0;
  _flags.PARALLELEDGE         = 0;
  _flags.SUBTRACTEOLWIDTH     = 0;
  _flags.PARPRL               = 0;
  _flags.PARMINLENGTH         = 0;
  _flags.TWOEDGES             = 0;
  _flags.SAMEMETAL            = 0;
  _flags.NONEOLCORNERONLY     = 0;
  _flags.PARALLELSAMEMASK     = 0;
  _flags.ENCLOSECUT           = 0;
  _flags.BELOW                = 0;
  _flags.ABOVE                = 0;
  _flags.CUTSPACING           = 0;
  _flags.ALLCUTS              = 0;
  _flags.TOCONCAVECORNER      = 0;
  _flags.MINADJACENTLENGTH    = 0;
  _flags.hasTwoMinAdjLength   = 0;
  _flags.TONOTCHLENGTH        = 0;
  _flags._spare_bits          = 0;
  eolSpace                    = 0;
  eolwidth                    = 0;
  wrongDirSpace               = 0;
  oppositeWidth               = 0;
  eolWithin                   = 0;
  wrongDirWithin              = 0;
  exactWidth                  = 0;
  otherWidth                  = 0;
  fillTriangle                = 0;
  cutClass                    = 0;
  withCutSpace                = 0;
  enclosureEndWidth           = 0;
  enclosureEndWithin          = 0;
  endPrlSpace                 = 0;
  endPrl                      = 0;
  endToEndSpace               = 0;
  oneCutSpace                 = 0;
  twoCutSpace                 = 0;
  extension                   = 0;
  wrongDirExtension           = 0;
  otherEndWidth               = 0;
  maxLength                   = 0;
  minLength                   = 0;
  parSpace                    = 0;
  parWithin                   = 0;
  parPrl                      = 0;
  parMinLength                = 0;
  encloseDist                 = 0;
  cutToMetalSpace             = 0;
  minAdjLength                = 0;
  minAdjLength1               = 0;
  minAdjLength2               = 0;
  notchLength                 = 0;
  // User Code End constructor
}
_dbTechLayerSpacingEolRule::_dbTechLayerSpacingEolRule(
    _dbDatabase*                      db,
    const _dbTechLayerSpacingEolRule& r)
{
  _flags.EXACTWIDTH           = r._flags.EXACTWIDTH;
  _flags.WRONGDIRSPACING      = r._flags.WRONGDIRSPACING;
  _flags.OPPOSITEWIDTH        = r._flags.OPPOSITEWIDTH;
  _flags.WITHIN               = r._flags.WITHIN;
  _flags.WRONGDIRWITHIN       = r._flags.WRONGDIRWITHIN;
  _flags.SAMEMASK             = r._flags.SAMEMASK;
  _flags.EXCEPTEXACTWIDTH     = r._flags.EXCEPTEXACTWIDTH;
  _flags.FILLCONCAVECORNER    = r._flags.FILLCONCAVECORNER;
  _flags.WITHCUT              = r._flags.WITHCUT;
  _flags.CUTCLASS             = r._flags.CUTCLASS;
  _flags.withCutABOVE         = r._flags.withCutABOVE;
  _flags.ENCLOSUREEND         = r._flags.ENCLOSUREEND;
  _flags.ENCLOSUREENDWITHIN   = r._flags.ENCLOSUREENDWITHIN;
  _flags.ENDPRLSPACING        = r._flags.ENDPRLSPACING;
  _flags.PRL                  = r._flags.PRL;
  _flags.ENDTOEND             = r._flags.ENDTOEND;
  _flags.hasCutSpaces         = r._flags.hasCutSpaces;
  _flags.EXTENSION            = r._flags.EXTENSION;
  _flags.hasWrongDirExtension = r._flags.hasWrongDirExtension;
  _flags.OTHERENDWIDTH        = r._flags.OTHERENDWIDTH;
  _flags.MAXLENGTH            = r._flags.MAXLENGTH;
  _flags.MINLENGTH            = r._flags.MINLENGTH;
  _flags.TWOSIDES             = r._flags.TWOSIDES;
  _flags.EQUALRECTWIDTH       = r._flags.EQUALRECTWIDTH;
  _flags.PARALLELEDGE         = r._flags.PARALLELEDGE;
  _flags.SUBTRACTEOLWIDTH     = r._flags.SUBTRACTEOLWIDTH;
  _flags.PARPRL               = r._flags.PARPRL;
  _flags.PARMINLENGTH         = r._flags.PARMINLENGTH;
  _flags.TWOEDGES             = r._flags.TWOEDGES;
  _flags.SAMEMETAL            = r._flags.SAMEMETAL;
  _flags.NONEOLCORNERONLY     = r._flags.NONEOLCORNERONLY;
  _flags.PARALLELSAMEMASK     = r._flags.PARALLELSAMEMASK;
  _flags.ENCLOSECUT           = r._flags.ENCLOSECUT;
  _flags.BELOW                = r._flags.BELOW;
  _flags.ABOVE                = r._flags.ABOVE;
  _flags.CUTSPACING           = r._flags.CUTSPACING;
  _flags.ALLCUTS              = r._flags.ALLCUTS;
  _flags.TOCONCAVECORNER      = r._flags.TOCONCAVECORNER;
  _flags.MINADJACENTLENGTH    = r._flags.MINADJACENTLENGTH;
  _flags.hasTwoMinAdjLength   = r._flags.hasTwoMinAdjLength;
  _flags.TONOTCHLENGTH        = r._flags.TONOTCHLENGTH;
  _flags._spare_bits          = r._flags._spare_bits;
  eolSpace                    = r.eolSpace;
  eolwidth                    = r.eolwidth;
  wrongDirSpace               = r.wrongDirSpace;
  oppositeWidth               = r.oppositeWidth;
  eolWithin                   = r.eolWithin;
  wrongDirWithin              = r.wrongDirWithin;
  exactWidth                  = r.exactWidth;
  otherWidth                  = r.otherWidth;
  fillTriangle                = r.fillTriangle;
  cutClass                    = r.cutClass;
  withCutSpace                = r.withCutSpace;
  enclosureEndWidth           = r.enclosureEndWidth;
  enclosureEndWithin          = r.enclosureEndWithin;
  endPrlSpace                 = r.endPrlSpace;
  endPrl                      = r.endPrl;
  endToEndSpace               = r.endToEndSpace;
  oneCutSpace                 = r.oneCutSpace;
  twoCutSpace                 = r.twoCutSpace;
  extension                   = r.extension;
  wrongDirExtension           = r.wrongDirExtension;
  otherEndWidth               = r.otherEndWidth;
  maxLength                   = r.maxLength;
  minLength                   = r.minLength;
  parSpace                    = r.parSpace;
  parWithin                   = r.parWithin;
  parPrl                      = r.parPrl;
  parMinLength                = r.parMinLength;
  encloseDist                 = r.encloseDist;
  cutToMetalSpace             = r.cutToMetalSpace;
  minAdjLength                = r.minAdjLength;
  minAdjLength1               = r.minAdjLength1;
  minAdjLength2               = r.minAdjLength2;
  notchLength                 = r.notchLength;
  // User Code Begin CopyConstructor
  // User Code End CopyConstructor
}

dbIStream& operator>>(dbIStream& stream, _dbTechLayerSpacingEolRule& obj)
{
  uint* _flags_bit_field = (uint*) &obj._flags;
  stream >> *_flags_bit_field;
  stream >> obj.eolSpace;
  stream >> obj.eolwidth;
  stream >> obj.wrongDirSpace;
  stream >> obj.oppositeWidth;
  stream >> obj.eolWithin;
  stream >> obj.wrongDirWithin;
  stream >> obj.exactWidth;
  stream >> obj.otherWidth;
  stream >> obj.fillTriangle;
  stream >> obj.cutClass;
  stream >> obj.withCutSpace;
  stream >> obj.enclosureEndWidth;
  stream >> obj.enclosureEndWithin;
  stream >> obj.endPrlSpace;
  stream >> obj.endPrl;
  stream >> obj.endToEndSpace;
  stream >> obj.oneCutSpace;
  stream >> obj.twoCutSpace;
  stream >> obj.extension;
  stream >> obj.wrongDirExtension;
  stream >> obj.otherEndWidth;
  stream >> obj.maxLength;
  stream >> obj.minLength;
  stream >> obj.parSpace;
  stream >> obj.parWithin;
  stream >> obj.parPrl;
  stream >> obj.parMinLength;
  stream >> obj.encloseDist;
  stream >> obj.cutToMetalSpace;
  stream >> obj.minAdjLength;
  stream >> obj.minAdjLength1;
  stream >> obj.minAdjLength2;
  stream >> obj.notchLength;
  // User Code Begin >>
  // User Code End >>
  return stream;
}
dbOStream& operator<<(dbOStream& stream, const _dbTechLayerSpacingEolRule& obj)
{
  uint* _flags_bit_field = (uint*) &obj._flags;
  stream << *_flags_bit_field;
  stream << obj.eolSpace;
  stream << obj.eolwidth;
  stream << obj.wrongDirSpace;
  stream << obj.oppositeWidth;
  stream << obj.eolWithin;
  stream << obj.wrongDirWithin;
  stream << obj.exactWidth;
  stream << obj.otherWidth;
  stream << obj.fillTriangle;
  stream << obj.cutClass;
  stream << obj.withCutSpace;
  stream << obj.enclosureEndWidth;
  stream << obj.enclosureEndWithin;
  stream << obj.endPrlSpace;
  stream << obj.endPrl;
  stream << obj.endToEndSpace;
  stream << obj.oneCutSpace;
  stream << obj.twoCutSpace;
  stream << obj.extension;
  stream << obj.wrongDirExtension;
  stream << obj.otherEndWidth;
  stream << obj.maxLength;
  stream << obj.minLength;
  stream << obj.parSpace;
  stream << obj.parWithin;
  stream << obj.parPrl;
  stream << obj.parMinLength;
  stream << obj.encloseDist;
  stream << obj.cutToMetalSpace;
  stream << obj.minAdjLength;
  stream << obj.minAdjLength1;
  stream << obj.minAdjLength2;
  stream << obj.notchLength;
  // User Code Begin <<
  // User Code End <<
  return stream;
}
dbObjectTable* _dbTechLayerSpacingEolRule::getObjectTable(dbObjectType type)
{
  switch (type) {
      // User Code Begin getObjectTable
    // User Code End getObjectTable
    default:
      break;
  }
  return getTable()->getObjectTable(type);
}
_dbTechLayerSpacingEolRule::~_dbTechLayerSpacingEolRule()
{
}
////////////////////////////////////////////////////////////////////
//
// dbTechLayerSpacingEolRule - Methods
//
////////////////////////////////////////////////////////////////////

void dbTechLayerSpacingEolRule::seteolSpace(double eolSpace)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->eolSpace = eolSpace;
}

double dbTechLayerSpacingEolRule::geteolSpace() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->eolSpace;
}

void dbTechLayerSpacingEolRule::setEolwidth(double eolwidth)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->eolwidth = eolwidth;
}

double dbTechLayerSpacingEolRule::getEolwidth() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->eolwidth;
}

void dbTechLayerSpacingEolRule::setwrongDirSpace(double wrongDirSpace)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->wrongDirSpace = wrongDirSpace;
}

double dbTechLayerSpacingEolRule::getwrongDirSpace() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->wrongDirSpace;
}

void dbTechLayerSpacingEolRule::setoppositeWidth(double oppositeWidth)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->oppositeWidth = oppositeWidth;
}

double dbTechLayerSpacingEolRule::getoppositeWidth() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->oppositeWidth;
}

void dbTechLayerSpacingEolRule::seteolWithin(double eolWithin)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->eolWithin = eolWithin;
}

double dbTechLayerSpacingEolRule::geteolWithin() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->eolWithin;
}

void dbTechLayerSpacingEolRule::setwrongDirWithin(double wrongDirWithin)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->wrongDirWithin = wrongDirWithin;
}

double dbTechLayerSpacingEolRule::getwrongDirWithin() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->wrongDirWithin;
}

void dbTechLayerSpacingEolRule::setexactWidth(double exactWidth)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->exactWidth = exactWidth;
}

double dbTechLayerSpacingEolRule::getexactWidth() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->exactWidth;
}

void dbTechLayerSpacingEolRule::setotherWidth(double otherWidth)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->otherWidth = otherWidth;
}

double dbTechLayerSpacingEolRule::getotherWidth() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->otherWidth;
}

void dbTechLayerSpacingEolRule::setfillTriangle(double fillTriangle)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->fillTriangle = fillTriangle;
}

double dbTechLayerSpacingEolRule::getfillTriangle() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->fillTriangle;
}

void dbTechLayerSpacingEolRule::setcutClass(double cutClass)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->cutClass = cutClass;
}

double dbTechLayerSpacingEolRule::getcutClass() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->cutClass;
}

void dbTechLayerSpacingEolRule::setwithCutSpace(double withCutSpace)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->withCutSpace = withCutSpace;
}

double dbTechLayerSpacingEolRule::getwithCutSpace() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->withCutSpace;
}

void dbTechLayerSpacingEolRule::setenclosureEndWidth(double enclosureEndWidth)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->enclosureEndWidth = enclosureEndWidth;
}

double dbTechLayerSpacingEolRule::getenclosureEndWidth() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->enclosureEndWidth;
}

void dbTechLayerSpacingEolRule::setenclosureEndWithin(double enclosureEndWithin)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->enclosureEndWithin = enclosureEndWithin;
}

double dbTechLayerSpacingEolRule::getenclosureEndWithin() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->enclosureEndWithin;
}

void dbTechLayerSpacingEolRule::setendPrlSpace(double endPrlSpace)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->endPrlSpace = endPrlSpace;
}

double dbTechLayerSpacingEolRule::getendPrlSpace() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->endPrlSpace;
}

void dbTechLayerSpacingEolRule::setendPrl(double endPrl)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->endPrl = endPrl;
}

double dbTechLayerSpacingEolRule::getendPrl() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->endPrl;
}

void dbTechLayerSpacingEolRule::setendToEndSpace(double endToEndSpace)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->endToEndSpace = endToEndSpace;
}

double dbTechLayerSpacingEolRule::getendToEndSpace() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->endToEndSpace;
}

void dbTechLayerSpacingEolRule::setoneCutSpace(double oneCutSpace)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->oneCutSpace = oneCutSpace;
}

double dbTechLayerSpacingEolRule::getoneCutSpace() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->oneCutSpace;
}

void dbTechLayerSpacingEolRule::settwoCutSpace(double twoCutSpace)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->twoCutSpace = twoCutSpace;
}

double dbTechLayerSpacingEolRule::gettwoCutSpace() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->twoCutSpace;
}

void dbTechLayerSpacingEolRule::setExtension(double extension)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->extension = extension;
}

double dbTechLayerSpacingEolRule::getExtension() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->extension;
}

void dbTechLayerSpacingEolRule::setwrongDirExtension(double wrongDirExtension)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->wrongDirExtension = wrongDirExtension;
}

double dbTechLayerSpacingEolRule::getwrongDirExtension() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->wrongDirExtension;
}

void dbTechLayerSpacingEolRule::setotherEndWidth(double otherEndWidth)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->otherEndWidth = otherEndWidth;
}

double dbTechLayerSpacingEolRule::getotherEndWidth() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->otherEndWidth;
}

void dbTechLayerSpacingEolRule::setmaxLength(double maxLength)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->maxLength = maxLength;
}

double dbTechLayerSpacingEolRule::getmaxLength() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->maxLength;
}

void dbTechLayerSpacingEolRule::setminLength(double minLength)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->minLength = minLength;
}

double dbTechLayerSpacingEolRule::getminLength() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->minLength;
}

void dbTechLayerSpacingEolRule::setparSpace(double parSpace)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->parSpace = parSpace;
}

double dbTechLayerSpacingEolRule::getparSpace() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->parSpace;
}

void dbTechLayerSpacingEolRule::setparWithin(double parWithin)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->parWithin = parWithin;
}

double dbTechLayerSpacingEolRule::getparWithin() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->parWithin;
}

void dbTechLayerSpacingEolRule::setparPrl(double parPrl)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->parPrl = parPrl;
}

double dbTechLayerSpacingEolRule::getparPrl() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->parPrl;
}

void dbTechLayerSpacingEolRule::setparMinLength(double parMinLength)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->parMinLength = parMinLength;
}

double dbTechLayerSpacingEolRule::getparMinLength() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->parMinLength;
}

void dbTechLayerSpacingEolRule::setencloseDist(double encloseDist)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->encloseDist = encloseDist;
}

double dbTechLayerSpacingEolRule::getencloseDist() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->encloseDist;
}

void dbTechLayerSpacingEolRule::setcutToMetalSpace(double cutToMetalSpace)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->cutToMetalSpace = cutToMetalSpace;
}

double dbTechLayerSpacingEolRule::getcutToMetalSpace() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->cutToMetalSpace;
}

void dbTechLayerSpacingEolRule::setminAdjLength(double minAdjLength)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->minAdjLength = minAdjLength;
}

double dbTechLayerSpacingEolRule::getminAdjLength() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->minAdjLength;
}

void dbTechLayerSpacingEolRule::setminAdjLength1(double minAdjLength1)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->minAdjLength1 = minAdjLength1;
}

double dbTechLayerSpacingEolRule::getminAdjLength1() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->minAdjLength1;
}

void dbTechLayerSpacingEolRule::setminAdjLength2(double minAdjLength2)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->minAdjLength2 = minAdjLength2;
}

double dbTechLayerSpacingEolRule::getminAdjLength2() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->minAdjLength2;
}

void dbTechLayerSpacingEolRule::setnotchLength(double notchLength)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->notchLength = notchLength;
}

double dbTechLayerSpacingEolRule::getnotchLength() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->notchLength;
}

void dbTechLayerSpacingEolRule::setEXACTWIDTH(bool EXACTWIDTH)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.EXACTWIDTH = EXACTWIDTH;
}

bool dbTechLayerSpacingEolRule::isEXACTWIDTH() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.EXACTWIDTH;
}

void dbTechLayerSpacingEolRule::setWRONGDIRSPACING(bool WRONGDIRSPACING)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.WRONGDIRSPACING = WRONGDIRSPACING;
}

bool dbTechLayerSpacingEolRule::isWRONGDIRSPACING() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.WRONGDIRSPACING;
}

void dbTechLayerSpacingEolRule::setOPPOSITEWIDTH(bool OPPOSITEWIDTH)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.OPPOSITEWIDTH = OPPOSITEWIDTH;
}

bool dbTechLayerSpacingEolRule::isOPPOSITEWIDTH() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.OPPOSITEWIDTH;
}

void dbTechLayerSpacingEolRule::setWITHIN(bool WITHIN)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.WITHIN = WITHIN;
}

bool dbTechLayerSpacingEolRule::isWITHIN() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.WITHIN;
}

void dbTechLayerSpacingEolRule::setWRONGDIRWITHIN(bool WRONGDIRWITHIN)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.WRONGDIRWITHIN = WRONGDIRWITHIN;
}

bool dbTechLayerSpacingEolRule::isWRONGDIRWITHIN() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.WRONGDIRWITHIN;
}

void dbTechLayerSpacingEolRule::setSAMEMASK(bool SAMEMASK)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.SAMEMASK = SAMEMASK;
}

bool dbTechLayerSpacingEolRule::isSAMEMASK() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.SAMEMASK;
}

void dbTechLayerSpacingEolRule::setEXCEPTEXACTWIDTH(bool EXCEPTEXACTWIDTH)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.EXCEPTEXACTWIDTH = EXCEPTEXACTWIDTH;
}

bool dbTechLayerSpacingEolRule::isEXCEPTEXACTWIDTH() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.EXCEPTEXACTWIDTH;
}

void dbTechLayerSpacingEolRule::setFILLCONCAVECORNER(bool FILLCONCAVECORNER)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.FILLCONCAVECORNER = FILLCONCAVECORNER;
}

bool dbTechLayerSpacingEolRule::isFILLCONCAVECORNER() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.FILLCONCAVECORNER;
}

void dbTechLayerSpacingEolRule::setWITHCUT(bool WITHCUT)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.WITHCUT = WITHCUT;
}

bool dbTechLayerSpacingEolRule::isWITHCUT() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.WITHCUT;
}

void dbTechLayerSpacingEolRule::setCUTCLASS(bool CUTCLASS)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.CUTCLASS = CUTCLASS;
}

bool dbTechLayerSpacingEolRule::isCUTCLASS() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.CUTCLASS;
}

void dbTechLayerSpacingEolRule::setwithCutABOVE(bool withCutABOVE)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.withCutABOVE = withCutABOVE;
}

bool dbTechLayerSpacingEolRule::iswithCutABOVE() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.withCutABOVE;
}

void dbTechLayerSpacingEolRule::setENCLOSUREEND(bool ENCLOSUREEND)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.ENCLOSUREEND = ENCLOSUREEND;
}

bool dbTechLayerSpacingEolRule::isENCLOSUREEND() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.ENCLOSUREEND;
}

void dbTechLayerSpacingEolRule::setENCLOSUREENDWITHIN(bool ENCLOSUREENDWITHIN)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.ENCLOSUREENDWITHIN = ENCLOSUREENDWITHIN;
}

bool dbTechLayerSpacingEolRule::isENCLOSUREENDWITHIN() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.ENCLOSUREENDWITHIN;
}

void dbTechLayerSpacingEolRule::setENDPRLSPACING(bool ENDPRLSPACING)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.ENDPRLSPACING = ENDPRLSPACING;
}

bool dbTechLayerSpacingEolRule::isENDPRLSPACING() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.ENDPRLSPACING;
}

void dbTechLayerSpacingEolRule::setPRL(bool PRL)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.PRL = PRL;
}

bool dbTechLayerSpacingEolRule::isPRL() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.PRL;
}

void dbTechLayerSpacingEolRule::setENDTOEND(bool ENDTOEND)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.ENDTOEND = ENDTOEND;
}

bool dbTechLayerSpacingEolRule::isENDTOEND() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.ENDTOEND;
}

void dbTechLayerSpacingEolRule::sethasCutSpaces(bool hasCutSpaces)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.hasCutSpaces = hasCutSpaces;
}

bool dbTechLayerSpacingEolRule::ishasCutSpaces() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.hasCutSpaces;
}

void dbTechLayerSpacingEolRule::setEXTENSION(bool EXTENSION)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.EXTENSION = EXTENSION;
}

bool dbTechLayerSpacingEolRule::isEXTENSION() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.EXTENSION;
}

void dbTechLayerSpacingEolRule::sethasWrongDirExtension(
    bool hasWrongDirExtension)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.hasWrongDirExtension = hasWrongDirExtension;
}

bool dbTechLayerSpacingEolRule::ishasWrongDirExtension() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.hasWrongDirExtension;
}

void dbTechLayerSpacingEolRule::setOTHERENDWIDTH(bool OTHERENDWIDTH)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.OTHERENDWIDTH = OTHERENDWIDTH;
}

bool dbTechLayerSpacingEolRule::isOTHERENDWIDTH() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.OTHERENDWIDTH;
}

void dbTechLayerSpacingEolRule::setMAXLENGTH(bool MAXLENGTH)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.MAXLENGTH = MAXLENGTH;
}

bool dbTechLayerSpacingEolRule::isMAXLENGTH() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.MAXLENGTH;
}

void dbTechLayerSpacingEolRule::setMINLENGTH(bool MINLENGTH)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.MINLENGTH = MINLENGTH;
}

bool dbTechLayerSpacingEolRule::isMINLENGTH() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.MINLENGTH;
}

void dbTechLayerSpacingEolRule::setTWOSIDES(bool TWOSIDES)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.TWOSIDES = TWOSIDES;
}

bool dbTechLayerSpacingEolRule::isTWOSIDES() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.TWOSIDES;
}

void dbTechLayerSpacingEolRule::setEQUALRECTWIDTH(bool EQUALRECTWIDTH)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.EQUALRECTWIDTH = EQUALRECTWIDTH;
}

bool dbTechLayerSpacingEolRule::isEQUALRECTWIDTH() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.EQUALRECTWIDTH;
}

void dbTechLayerSpacingEolRule::setPARALLELEDGE(bool PARALLELEDGE)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.PARALLELEDGE = PARALLELEDGE;
}

bool dbTechLayerSpacingEolRule::isPARALLELEDGE() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.PARALLELEDGE;
}

void dbTechLayerSpacingEolRule::setSUBTRACTEOLWIDTH(bool SUBTRACTEOLWIDTH)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.SUBTRACTEOLWIDTH = SUBTRACTEOLWIDTH;
}

bool dbTechLayerSpacingEolRule::isSUBTRACTEOLWIDTH() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.SUBTRACTEOLWIDTH;
}

void dbTechLayerSpacingEolRule::setPARPRL(bool PARPRL)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.PARPRL = PARPRL;
}

bool dbTechLayerSpacingEolRule::isPARPRL() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.PARPRL;
}

void dbTechLayerSpacingEolRule::setPARMINLENGTH(bool PARMINLENGTH)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.PARMINLENGTH = PARMINLENGTH;
}

bool dbTechLayerSpacingEolRule::isPARMINLENGTH() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.PARMINLENGTH;
}

void dbTechLayerSpacingEolRule::setTWOEDGES(bool TWOEDGES)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.TWOEDGES = TWOEDGES;
}

bool dbTechLayerSpacingEolRule::isTWOEDGES() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.TWOEDGES;
}

void dbTechLayerSpacingEolRule::setSAMEMETAL(bool SAMEMETAL)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.SAMEMETAL = SAMEMETAL;
}

bool dbTechLayerSpacingEolRule::isSAMEMETAL() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.SAMEMETAL;
}

void dbTechLayerSpacingEolRule::setNONEOLCORNERONLY(bool NONEOLCORNERONLY)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.NONEOLCORNERONLY = NONEOLCORNERONLY;
}

bool dbTechLayerSpacingEolRule::isNONEOLCORNERONLY() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.NONEOLCORNERONLY;
}

void dbTechLayerSpacingEolRule::setPARALLELSAMEMASK(bool PARALLELSAMEMASK)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.PARALLELSAMEMASK = PARALLELSAMEMASK;
}

bool dbTechLayerSpacingEolRule::isPARALLELSAMEMASK() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.PARALLELSAMEMASK;
}

void dbTechLayerSpacingEolRule::setENCLOSECUT(bool ENCLOSECUT)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.ENCLOSECUT = ENCLOSECUT;
}

bool dbTechLayerSpacingEolRule::isENCLOSECUT() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.ENCLOSECUT;
}

void dbTechLayerSpacingEolRule::setBELOW(bool BELOW)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.BELOW = BELOW;
}

bool dbTechLayerSpacingEolRule::isBELOW() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.BELOW;
}

void dbTechLayerSpacingEolRule::setABOVE(bool ABOVE)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.ABOVE = ABOVE;
}

bool dbTechLayerSpacingEolRule::isABOVE() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.ABOVE;
}

void dbTechLayerSpacingEolRule::setCUTSPACING(bool CUTSPACING)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.CUTSPACING = CUTSPACING;
}

bool dbTechLayerSpacingEolRule::isCUTSPACING() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.CUTSPACING;
}

void dbTechLayerSpacingEolRule::setALLCUTS(bool ALLCUTS)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.ALLCUTS = ALLCUTS;
}

bool dbTechLayerSpacingEolRule::isALLCUTS() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.ALLCUTS;
}

void dbTechLayerSpacingEolRule::setTOCONCAVECORNER(bool TOCONCAVECORNER)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.TOCONCAVECORNER = TOCONCAVECORNER;
}

bool dbTechLayerSpacingEolRule::isTOCONCAVECORNER() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.TOCONCAVECORNER;
}

void dbTechLayerSpacingEolRule::setMINADJACENTLENGTH(bool MINADJACENTLENGTH)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.MINADJACENTLENGTH = MINADJACENTLENGTH;
}

bool dbTechLayerSpacingEolRule::isMINADJACENTLENGTH() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.MINADJACENTLENGTH;
}

void dbTechLayerSpacingEolRule::sethasTwoMinAdjLength(bool hasTwoMinAdjLength)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.hasTwoMinAdjLength = hasTwoMinAdjLength;
}

bool dbTechLayerSpacingEolRule::ishasTwoMinAdjLength() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.hasTwoMinAdjLength;
}

void dbTechLayerSpacingEolRule::setTONOTCHLENGTH(bool TONOTCHLENGTH)
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  obj->_flags.TONOTCHLENGTH = TONOTCHLENGTH;
}

bool dbTechLayerSpacingEolRule::isTONOTCHLENGTH() const
{
  _dbTechLayerSpacingEolRule* obj = (_dbTechLayerSpacingEolRule*) this;

  return obj->_flags.TONOTCHLENGTH;
}

// User Code Begin dbTechLayerSpacingEolRulePublicMethods
dbTechLayerSpacingEolRule* dbTechLayerSpacingEolRule::create(dbTechLayer* inly)
{
  _dbTechLayer*            layer   = (_dbTechLayer*) inly;
  _dbTechLayerSpacingEolRule* newrule = layer->_spacing_eol_rules_tbl->create();
  newrule->_layer                  = inly->getImpl()->getOID();

  return ((dbTechLayerSpacingEolRule*) newrule);
}

dbTechLayerSpacingEolRule* dbTechLayerSpacingEolRule::getTechLayerSpacingEolRule(
    dbTechLayer* inly,
    uint         dbid)
{
  _dbTechLayer* layer = (_dbTechLayer*) inly;
  return (dbTechLayerSpacingEolRule*) layer->_spacing_eol_rules_tbl->getPtr(dbid);
}

// User Code End dbTechLayerSpacingEolRulePublicMethods
}  // namespace odb
   // Generator Code End 1