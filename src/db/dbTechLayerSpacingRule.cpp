///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019, Nefelus Inc
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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "dbTechLayerSpacingRule.h"
#include "dbDatabase.h"
#include "dbTech.h"
#include "dbTechLayer.h"
#include "dbTable.h"
#include "dbTable.hpp"
#include "db.h"
#include "lefout.h"

namespace odb {

template class dbTable<_dbTechLayerSpacingRule>;
template class dbTable<_dbTechV55InfluenceEntry>;

bool _dbTechLayerSpacingRule::operator==( const _dbTechLayerSpacingRule & rhs ) const
{
    if( _flags._rule != rhs._flags._rule )
        return false;
    
    if( _spacing != rhs._spacing )
        return false;
    
    if( _length_or_influence != rhs._length_or_influence )
        return false;
    
    if( _r1min != rhs._r1min )
        return false;
    
    if( _r1max != rhs._r1max )
        return false;
    
    if( _r2min != rhs._r2min )
        return false;
    
    if( _r2max != rhs._r2max )
        return false;
    
    if( _layer != rhs._layer )
        return false;
    
    if( _cut_layer_below != rhs._cut_layer_below )
        return false;
    
    return true;
}

void _dbTechLayerSpacingRule::differences( dbDiff & diff, const char * field, const _dbTechLayerSpacingRule & rhs ) const
{
    DIFF_BEGIN
    DIFF_FIELD(_flags._rule );
    DIFF_FIELD(_spacing);
    DIFF_FIELD(_length_or_influence );
    DIFF_FIELD(_r1min );
    DIFF_FIELD(_r1max );
    DIFF_FIELD(_r2min );
    DIFF_FIELD(_r2max );
    DIFF_FIELD(_layer );
    DIFF_FIELD(_cut_layer_below );
    DIFF_END
}

void _dbTechLayerSpacingRule::out( dbDiff & diff, char side, const char * field ) const
{
    DIFF_OUT_BEGIN
    DIFF_OUT_FIELD(_flags._rule );
    DIFF_OUT_FIELD(_spacing);
    DIFF_OUT_FIELD(_length_or_influence );
    DIFF_OUT_FIELD(_r1min );
    DIFF_OUT_FIELD(_r1max );
    DIFF_OUT_FIELD(_r2min );
    DIFF_OUT_FIELD(_r2max );
    DIFF_OUT_FIELD(_layer );
    DIFF_OUT_FIELD(_cut_layer_below );
    DIFF_END
}

bool _dbTechV55InfluenceEntry::operator==( const _dbTechV55InfluenceEntry & rhs ) const
{
    if( _width != rhs._width )
        return false;
    
    if( _within != rhs._within )
        return false;
    
    if( _spacing != rhs._spacing )
        return false;
    
    return true;
}

void _dbTechV55InfluenceEntry::differences( dbDiff & diff, const char * field, const _dbTechV55InfluenceEntry & rhs ) const
{
    DIFF_BEGIN
    DIFF_FIELD(_width);
    DIFF_FIELD(_within);
    DIFF_FIELD(_spacing);
    DIFF_END
}

void _dbTechV55InfluenceEntry::out( dbDiff & diff, char side, const char * field ) const
{
    DIFF_OUT_BEGIN
    DIFF_OUT_FIELD(_width);
    DIFF_OUT_FIELD(_within);
    DIFF_OUT_FIELD(_spacing);
    DIFF_END
}

////////////////////////////////////////////////////////////////////
//
// _dbTechLayerSpacingRule - Methods
// NOTE: Streaming methods work with pointers.
//
////////////////////////////////////////////////////////////////////

dbOStream & operator<<( dbOStream & stream, const _dbTechLayerSpacingRule &rule )
{
  uint * bit_field = (uint *) &rule._flags;
  stream << *bit_field;
  stream << rule._spacing;
  stream << rule._length_or_influence;
  stream << rule._r1min;
  stream << rule._r1max;
  stream << rule._r2min;
  stream << rule._r2max;
  stream << rule._layer;
  stream << rule._cut_layer_below;
  return stream;
}

dbIStream & operator>>( dbIStream & stream, _dbTechLayerSpacingRule & rule )
{
  uint * bit_field = (uint *) &rule._flags;
  stream >> *bit_field;
  stream >> rule._spacing;
  stream >> rule._length_or_influence;
  stream >> rule._r1min;
  stream >> rule._r1max;
  stream >> rule._r2min;
  stream >> rule._r2max;
  stream >> rule._layer;
  stream >> rule._cut_layer_below;

  return stream;
}

////////////////////////////////////////////////////////////////////
//
// _dbTechV55InfluenceEntry - Methods
//
////////////////////////////////////////////////////////////////////

dbOStream & operator<<( dbOStream & stream, const _dbTechV55InfluenceEntry & infitem )
{
    stream << infitem._width;
    stream << infitem._within;
    stream << infitem._spacing;
    return stream;
}

dbIStream & operator>>( dbIStream & stream, _dbTechV55InfluenceEntry & infitem )
{
  stream >> infitem._width;
  stream >> infitem._within;
  stream >> infitem._spacing;
  return stream;
}

////////////////////////////////////////////////////////////////////
//
// dbTechLayerSpacingRule - Methods
//
////////////////////////////////////////////////////////////////////

using namespace TechLayerSpacingRule;

uint
dbTechLayerSpacingRule::getSpacing() const
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  return _lsp->_spacing;
}

void
dbTechLayerSpacingRule::setSpacing( uint spacing )
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  _lsp->_spacing = spacing;
}

bool
dbTechLayerSpacingRule::isUnconditional() const
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  return (_lsp->_flags._rule == DEFAULT);
}

bool
dbTechLayerSpacingRule::getLengthThreshold( uint & threshold ) const
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  if ((_lsp->_flags._rule != LENGTHTHRESHOLD) && (_lsp->_flags._rule != LENGTHTHRESHOLD_RANGE))
    return false;

  threshold = _lsp->_length_or_influence;
  return true;
}

bool
dbTechLayerSpacingRule::getLengthThresholdRange( uint & rmin, uint & rmax ) const
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  if (_lsp->_flags._rule != LENGTHTHRESHOLD_RANGE)
    return false;

  rmin = _lsp->_r2min;
  rmax = _lsp->_r2max;
  return true;
}

bool
dbTechLayerSpacingRule::getRange( uint & rmin, uint & rmax ) const
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  if ((_lsp->_flags._rule != RANGE_ONLY) &&
      (_lsp->_flags._rule != RANGE_RANGE) &&
      (_lsp->_flags._rule != RANGE_USELENGTH) && 
      (_lsp->_flags._rule != RANGE_INFLUENCE) && 
      (_lsp->_flags._rule != RANGE_INFLUENCE_RANGE))
    return false;

  rmin = _lsp->_r1min;
  rmax = _lsp->_r1max;
  return true;
}

bool
dbTechLayerSpacingRule::hasUseLengthThreshold() const
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  return (_lsp->_flags._rule == RANGE_USELENGTH);
}

bool
dbTechLayerSpacingRule::getInfluence( uint & influence ) const
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  if ((_lsp->_flags._rule != RANGE_INFLUENCE) && (_lsp->_flags._rule != RANGE_INFLUENCE_RANGE))
    return false;

  influence = _lsp->_length_or_influence;
  return true;
}

bool
dbTechLayerSpacingRule::getInfluenceRange( uint & rmin, uint & rmax ) const
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  if (_lsp->_flags._rule != RANGE_INFLUENCE_RANGE)
    return false;

  rmin = _lsp->_r2min;
  rmax = _lsp->_r2max;
  return true;
}

bool
dbTechLayerSpacingRule::getRangeRange( uint & rmin, uint & rmax ) const
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  if (_lsp->_flags._rule != RANGE_RANGE)
    return false;

  rmin = _lsp->_r2min;
  rmax = _lsp->_r2max;
  return true;
}

bool
dbTechLayerSpacingRule::getAdjacentCuts( uint & numcuts, uint & within, uint & spacing ) const
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;

  if (_lsp->_flags._rule != ADJACENT_CUTS_INFLUENCE)
    return false;
  
  spacing = _lsp->_spacing;
  within = _lsp->_length_or_influence;
  numcuts = _lsp->_r1min;

  return true;
}

bool
dbTechLayerSpacingRule::getCutLayer4Spacing( dbTechLayer *&outly ) const
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  if (_lsp->_flags._rule != CUT_LAYER_BELOW)
    return false;

  dbTechLayer *tmply = (dbTechLayer *) getOwner();
  dbTech *tmptech = (dbTech *) tmply->getOwner();
  outly = tmply->getTechLayer(tmptech, _lsp->_cut_layer_below);
  return true;
}

void
dbTechLayerSpacingRule::setLengthThreshold( uint threshold )
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  assert((_lsp->_flags._rule != RANGE_ONLY) &&
	 (_lsp->_flags._rule != RANGE_RANGE) &&
	 (_lsp->_flags._rule != RANGE_USELENGTH) &&
	 (_lsp->_flags._rule != RANGE_INFLUENCE) &&
	 (_lsp->_flags._rule != RANGE_INFLUENCE_RANGE) &&
	 (_lsp->_flags._rule != CUT_LAYER_BELOW) &&
	 (_lsp->_flags._rule != ADJACENT_CUTS_INFLUENCE)
	 );

  // Not already LENGTHTHRESHOLD or LENGTHTHRESHOLD_RANGE
  if (_lsp->_flags._rule == DEFAULT)
    _lsp->_flags._rule = LENGTHTHRESHOLD;

  _lsp->_length_or_influence = threshold;
}

void
dbTechLayerSpacingRule::setLengthThresholdRange( uint rmin, uint rmax )
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  assert((_lsp->_flags._rule != RANGE_ONLY) &&
	 (_lsp->_flags._rule != RANGE_RANGE) &&
	 (_lsp->_flags._rule != RANGE_USELENGTH) && 
	 (_lsp->_flags._rule != RANGE_INFLUENCE) && 
	 (_lsp->_flags._rule != RANGE_INFLUENCE_RANGE));

  _lsp->_flags._rule = LENGTHTHRESHOLD_RANGE;
  _lsp->_r2min = rmin;
  _lsp->_r2max = rmax;
}

void
dbTechLayerSpacingRule::setRange( uint rmin, uint rmax )
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  assert((_lsp->_flags._rule != LENGTHTHRESHOLD) &&
	 (_lsp->_flags._rule != LENGTHTHRESHOLD_RANGE));

  if (_lsp->_flags._rule == DEFAULT)
    _lsp->_flags._rule = RANGE_ONLY;

  _lsp->_r1min = rmin;
  _lsp->_r1max = rmax;
}

void
dbTechLayerSpacingRule::setUseLengthThreshold()
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  assert((_lsp->_flags._rule != LENGTHTHRESHOLD) &&
	 (_lsp->_flags._rule != LENGTHTHRESHOLD_RANGE) &&
	 (_lsp->_flags._rule != RANGE_RANGE) &&
	 (_lsp->_flags._rule != RANGE_INFLUENCE_RANGE) &&
	 (_lsp->_flags._rule != RANGE_INFLUENCE));

  _lsp->_flags._rule = RANGE_USELENGTH;
}

void
dbTechLayerSpacingRule::setInfluence( uint influence )
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  assert((_lsp->_flags._rule != LENGTHTHRESHOLD) &&
	 (_lsp->_flags._rule != LENGTHTHRESHOLD_RANGE) &&
	 (_lsp->_flags._rule != RANGE_USELENGTH) &&
	 (_lsp->_flags._rule != CUT_LAYER_BELOW) &&
	 (_lsp->_flags._rule != ADJACENT_CUTS_INFLUENCE) &&
	 (_lsp->_flags._rule != RANGE_RANGE)
	 );

  // Not already RANGE_INFLUENCE or RANGE_INFLUENCE_RANGE
  if ((_lsp->_flags._rule == DEFAULT) || (_lsp->_flags._rule == RANGE_ONLY))
    _lsp->_flags._rule = RANGE_INFLUENCE;

  _lsp->_length_or_influence = influence;
}

void
dbTechLayerSpacingRule::setInfluenceRange( uint rmin, uint rmax )
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  assert((_lsp->_flags._rule != LENGTHTHRESHOLD) &&
	 (_lsp->_flags._rule != LENGTHTHRESHOLD_RANGE) &&
	 (_lsp->_flags._rule != RANGE_RANGE) &&
	 (_lsp->_flags._rule != RANGE_USELENGTH));

  _lsp->_flags._rule = RANGE_INFLUENCE_RANGE;
  _lsp->_r2min = rmin;
  _lsp->_r2max = rmax;
}

void
dbTechLayerSpacingRule::setRangeRange( uint rmin, uint rmax )
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  assert((_lsp->_flags._rule != LENGTHTHRESHOLD) &&
	 (_lsp->_flags._rule != LENGTHTHRESHOLD_RANGE) &&
	 (_lsp->_flags._rule != RANGE_INFLUENCE_RANGE) &&
	 (_lsp->_flags._rule != RANGE_INFLUENCE) &&
	 (_lsp->_flags._rule != RANGE_USELENGTH));

  _lsp->_flags._rule = RANGE_RANGE;
  _lsp->_r2min = rmin;
  _lsp->_r2max = rmax;
}

void
dbTechLayerSpacingRule::setAdjacentCuts( uint numcuts, uint within, uint spacing )
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;

  assert((_lsp->_flags._rule == DEFAULT) ||
	 (_lsp->_flags._rule == ADJACENT_CUTS_INFLUENCE));

  _lsp->_flags._rule = ADJACENT_CUTS_INFLUENCE;
  _lsp->_spacing = spacing;
  _lsp->_length_or_influence = within;
  _lsp->_r1min = numcuts;
}

void
dbTechLayerSpacingRule::setCutLayer4Spacing( dbTechLayer *cutly )
{
  _dbTechLayerSpacingRule * _lsp = (_dbTechLayerSpacingRule *) this;
  assert((_lsp->_flags._rule == DEFAULT) ||
	 (_lsp->_flags._rule == CUT_LAYER_BELOW));

  dbTechLayer *tmply = (dbTechLayer *) getOwner();
  assert(cutly->getNumber() < tmply->getNumber());
  _dbTechLayer *ct_ly = (_dbTechLayer *) cutly;

  _lsp->_flags._rule = CUT_LAYER_BELOW;
  _lsp->_cut_layer_below = ct_ly->getOID();
}

dbTechLayerSpacingRule *
dbTechLayerSpacingRule::create( dbTechLayer * inly )
{
  _dbTechLayer * layer = (_dbTechLayer *) inly;
  _dbTechLayerSpacingRule * newrule = layer->_spacing_rules_tbl->create();
  newrule->_layer = inly->getOID();

  return ((dbTechLayerSpacingRule *) newrule);
}

dbTechLayerSpacingRule *
dbTechLayerSpacingRule::getTechLayerSpacingRule( dbTechLayer * inly, uint dbid )
{
    _dbTechLayer * layer = (_dbTechLayer *) inly;
    return (dbTechLayerSpacingRule *) layer->_spacing_rules_tbl->getPtr(dbid);
}

void
dbTechLayerSpacingRule::writeLef( lefout & writer ) const
{
  uint rmin, rmax, length_or_influence, cut_spacing, numcuts;
  dbTechLayer  *rulely;

  fprintf(writer.out(), "    SPACING %g ", writer.lefdist(getSpacing()));
  if (getRange(rmin, rmax))
    {
      fprintf(writer.out(), "RANGE %g %g ", writer.lefdist(rmin), writer.lefdist(rmax));
      if (hasUseLengthThreshold())
	fprintf( writer.out(), "USELENGTHTHRESHOLD ");
      else if (getInfluence(length_or_influence))
	{
	  fprintf(writer.out(), "INFLUENCE %g ", writer.lefdist(length_or_influence));
	  if (getInfluenceRange(rmin, rmax))
	    fprintf(writer.out(), "RANGE %g %g ", writer.lefdist(rmin),
		    writer.lefdist(rmax));
	}
      else if (getRangeRange(rmin, rmax))
	{
	  fprintf(writer.out(), "RANGE %g %g ", writer.lefdist(rmin),
		  writer.lefdist(rmax));
	}
    }
  else if (getLengthThreshold(length_or_influence))
    {
      fprintf(writer.out(), "LENGTHTHRESHOLD %g ", writer.lefdist(length_or_influence));
      if (getLengthThresholdRange(rmin, rmax))
	fprintf(writer.out(), "RANGE %g %g ", writer.lefdist(rmin), 
		writer.lefdist(rmax));
    }
  else if (getCutLayer4Spacing(rulely))
    {
      fprintf(writer.out(), "LAYER %s ",(const char *) rulely->getName());
    }
  else if (getAdjacentCuts(numcuts, length_or_influence, cut_spacing))
    {
      fprintf(writer.out(), "ADJACENTCUTS %d WITHIN %g ", numcuts,
	      writer.lefdist(length_or_influence));
    }

  fprintf( writer.out(), " ;\n");
}


////////////////////////////////////////////////////////////////////
//
// dbTechV55InfluenceEntry - Methods
//
////////////////////////////////////////////////////////////////////

bool 
dbTechV55InfluenceEntry::getV55InfluenceEntry( uint & width, uint & within, uint & spacing ) const
{
  _dbTechV55InfluenceEntry * _v55ie = (_dbTechV55InfluenceEntry *) this;
  width = _v55ie->_width;
  within = _v55ie->_within;
  spacing = _v55ie->_spacing;
  return true;
}
 
void
dbTechV55InfluenceEntry::setV55InfluenceEntry( const uint & width, const uint & within, const uint & spacing )
{
  _dbTechV55InfluenceEntry * _v55ie = (_dbTechV55InfluenceEntry *) this;
  _v55ie->_width = width;
  _v55ie->_within = within;
  _v55ie->_spacing = spacing;
}

void
dbTechV55InfluenceEntry::writeLef(lefout & writer) const
{
  uint inf_width, inf_within, inf_spacing;
    
  getV55InfluenceEntry(inf_width, inf_within, inf_spacing);
  fprintf(writer.out(), "\n   WIDTH %g WITHIN %g SPACING %g",
	  writer.lefdist(inf_width), writer.lefdist(inf_within),
	  writer.lefdist(inf_spacing));
}

dbTechV55InfluenceEntry *
dbTechV55InfluenceEntry::create( dbTechLayer * inly )
{
  _dbTechLayer * layer = (_dbTechLayer *) inly;
  _dbTechV55InfluenceEntry * newitem = layer->_v55inf_tbl->create();
  return ((dbTechV55InfluenceEntry *) newitem);
}

dbTechV55InfluenceEntry *
dbTechV55InfluenceEntry::getV55InfluenceEntry( dbTechLayer * inly, uint oid )
{
  _dbTechLayer * layer = (_dbTechLayer *) inly;
  return (dbTechV55InfluenceEntry *) layer->_v55inf_tbl->getPtr(oid);
}

} // namespace
