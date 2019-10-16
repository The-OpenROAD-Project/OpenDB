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

#include "dbTechLayer.h"
#include "dbDatabase.h"
#include "dbTech.h"
#include "dbTechLayerSpacingRule.h"
#include "dbTechMinCutOrAreaRule.h"
#include "dbTechLayerAntennaRule.h"
#include "lefout.h"
#include "dbTable.h"
#include "dbTable.hpp"
#include "db.h"


namespace odb {

template class dbTable<_dbTechLayer>;

bool _dbTechLayer::operator==( const _dbTechLayer & rhs ) const
{
    if( _flags._type != rhs._flags._type )
        return false;
    
    if( _flags._direction != rhs._flags._direction )
        return false;
    
    if( _flags._has_max_width != rhs._flags._has_max_width )
        return false;
    
    if( _flags._has_min_step != rhs._flags._has_min_step )
        return false;
    
    if( _flags._has_thickness != rhs._flags._has_thickness )
        return false;
    
    if( _flags._has_area != rhs._flags._has_area )
        return false;
    
    if( _flags._has_protrusion != rhs._flags._has_protrusion )
        return false;
    
    if( _flags._has_alias != rhs._flags._has_alias )
        return false;

    if( _pitch != rhs._pitch )
        return false;
    
    if( _width != rhs._width )
        return false;
    
    if( _spacing != rhs._spacing )
        return false;
    
    if( _resistance != rhs._resistance )
        return false;
    
    if( _capacitance != rhs._capacitance )
        return false;
    
    if( _edge_capacitance != rhs._edge_capacitance )
        return false;
    
    if( _wire_extension != rhs._wire_extension )
        return false;
    
    if( _number != rhs._number )
        return false;
    
    if( _rlevel != rhs._rlevel )
        return false;
    
    if( _area != rhs._area )
        return false;
    
    if( _thickness != rhs._thickness )
        return false;
    
    if( _min_step != rhs._min_step )
        return false;
    
    if( _max_width != rhs._max_width )
        return false;
    
    if( _pt._width != rhs._pt._width )
        return false;
    
    if( _pt._length != rhs._pt._length )
        return false;
    
    if( _pt._from_width != rhs._pt._from_width )
        return false;

    if ( _name && rhs._name )
    {
        if ( strcmp(_name, rhs._name) != 0 )
            return false;
    }
    else if ( _name || rhs._name )
            return false;

    if ( _alias && rhs._alias )
    {
        if ( strcmp(_alias, rhs._alias) != 0 )
            return false;
    }
    else if ( _alias || rhs._alias )
            return false;

    if( _upper != rhs._upper )
        return false;
    
    if( _lower != rhs._lower )
        return false;
    
    if( *_spacing_rules_tbl != *rhs._spacing_rules_tbl )
        return false;
    
    if( *_min_cut_rules_tbl != *rhs._min_cut_rules_tbl )
        return false;
    
    if( *_min_enc_rules_tbl != *rhs._min_enc_rules_tbl )
        return false;
    
    if( *_v55inf_tbl != *rhs._v55inf_tbl )
        return false;
    
    if( _v55sp_length_idx != rhs._v55sp_length_idx )
        return false;
    
    if( _v55sp_width_idx != rhs._v55sp_width_idx )
        return false;
    
    if( _v55sp_spacing != rhs._v55sp_spacing )
        return false;
    
    if( _oxide1 != rhs._oxide1 )
        return false;
    
    if( _oxide2 != rhs._oxide2 )
        return false;
 
    return true;
}

void _dbTechLayer::differences( dbDiff & diff, const char * field, const _dbTechLayer & rhs ) const
{
    DIFF_BEGIN
    DIFF_FIELD(_flags._type);
    DIFF_FIELD(_flags._direction);
    DIFF_FIELD(_flags._has_max_width);
    DIFF_FIELD(_flags._has_min_step);
    DIFF_FIELD(_flags._has_thickness);
    DIFF_FIELD(_flags._has_area);
    DIFF_FIELD(_flags._has_protrusion);
    DIFF_FIELD(_flags._has_alias);
    DIFF_FIELD(_pitch);
    DIFF_FIELD(_width);
    DIFF_FIELD(_spacing);
    DIFF_FIELD(_resistance);
    DIFF_FIELD(_capacitance);
    DIFF_FIELD(_edge_capacitance);
    DIFF_FIELD(_wire_extension);
    DIFF_FIELD(_number);
    DIFF_FIELD(_rlevel);
    DIFF_FIELD(_area);
    DIFF_FIELD(_thickness);
    DIFF_FIELD(_min_step);
    DIFF_FIELD(_max_width);
    DIFF_FIELD(_max_width);
    DIFF_FIELD(_pt._length);
    DIFF_FIELD(_pt._from_width);
    DIFF_FIELD(_name);
    DIFF_FIELD(_alias);
    DIFF_FIELD(_upper);
    DIFF_FIELD(_lower);
    DIFF_TABLE_NO_DEEP(_spacing_rules_tbl);
    DIFF_TABLE_NO_DEEP(_min_cut_rules_tbl);
    DIFF_TABLE_NO_DEEP(_min_enc_rules_tbl);
    DIFF_TABLE_NO_DEEP(_v55inf_tbl);
    DIFF_VECTOR(_v55sp_length_idx);
    DIFF_VECTOR(_v55sp_width_idx);
    DIFF_MATRIX(_v55sp_spacing);
    DIFF_FIELD(_oxide1);
    DIFF_FIELD(_oxide2);
    DIFF_END
}

void _dbTechLayer::out( dbDiff & diff, char side, const char * field ) const
{
    DIFF_OUT_BEGIN
    DIFF_OUT_FIELD(_flags._type);
    DIFF_OUT_FIELD(_flags._direction);
    DIFF_OUT_FIELD(_flags._has_max_width);
    DIFF_OUT_FIELD(_flags._has_min_step);
    DIFF_OUT_FIELD(_flags._has_thickness);
    DIFF_OUT_FIELD(_flags._has_area);
    DIFF_OUT_FIELD(_flags._has_protrusion);
    DIFF_OUT_FIELD(_flags._has_alias);
    DIFF_OUT_FIELD(_pitch);
    DIFF_OUT_FIELD(_width);
    DIFF_OUT_FIELD(_spacing);
    DIFF_OUT_FIELD(_resistance);
    DIFF_OUT_FIELD(_capacitance);
    DIFF_OUT_FIELD(_edge_capacitance);
    DIFF_OUT_FIELD(_wire_extension);
    DIFF_OUT_FIELD(_number);
    DIFF_OUT_FIELD(_rlevel);
    DIFF_OUT_FIELD(_area);
    DIFF_OUT_FIELD(_thickness);
    DIFF_OUT_FIELD(_min_step);
    DIFF_OUT_FIELD(_max_width);
    DIFF_OUT_FIELD(_max_width);
    DIFF_OUT_FIELD(_pt._length);
    DIFF_OUT_FIELD(_pt._from_width);
    DIFF_OUT_FIELD(_name);
    DIFF_OUT_FIELD(_alias);
    DIFF_OUT_FIELD(_upper);
    DIFF_OUT_FIELD(_lower);
    DIFF_OUT_TABLE_NO_DEEP(_spacing_rules_tbl);
    DIFF_OUT_TABLE_NO_DEEP(_min_cut_rules_tbl);
    DIFF_OUT_TABLE_NO_DEEP(_min_enc_rules_tbl);
    DIFF_OUT_TABLE_NO_DEEP(_v55inf_tbl);
    DIFF_OUT_VECTOR(_v55sp_length_idx);
    DIFF_OUT_VECTOR(_v55sp_width_idx);
    DIFF_OUT_MATRIX(_v55sp_spacing);
    DIFF_OUT_FIELD(_oxide1);
    DIFF_OUT_FIELD(_oxide2);
    DIFF_END
}

////////////////////////////////////////////////////////////////////
//
// _dbTechLayer - Methods
//
////////////////////////////////////////////////////////////////////

_dbTechLayer::_dbTechLayer( _dbDatabase * db )
{
    _flags._type = dbTechLayerType::ROUTING;
    _flags._direction = dbTechLayerDir::NONE;
    _flags._has_area = _flags._has_thickness = 0;
    _flags._has_min_step = _flags._has_max_width = 0;
    _flags._has_protrusion = 0;
    _flags._has_alias = 0;
    _flags._spare_bits = 0;
    _pitch = 0;
    _width = 0;
    _spacing  = 0;
    _resistance = 0.0;
    _capacitance = 0.0;
    _edge_capacitance = 0.0;
    _wire_extension = 0;
    _number = 0;
    _rlevel = 0;
    _area = 0.0;
    _thickness = _min_step = 0;
    _pt._width = _pt._length = _pt._from_width = 0;
    _max_width = MAX_INT;
    _v55sp_length_idx.clear();
    _v55sp_width_idx.clear();
    _v55sp_spacing.clear();
    _name = 0;
    _alias = 0;

    _spacing_rules_tbl = new dbTable<_dbTechLayerSpacingRule>(db, this, (GetObjTbl_t) &_dbTechLayer::getObjectTable, dbTechLayerSpacingRuleObj);
    ZALLOCATED(_spacing_rules_tbl);

    _min_cut_rules_tbl = new dbTable<_dbTechMinCutRule>(db, this,  (GetObjTbl_t) &_dbTechLayer::getObjectTable,dbTechMinCutRuleObj, 8, 3);
    ZALLOCATED(_min_cut_rules_tbl);

    _min_enc_rules_tbl = new dbTable<_dbTechMinEncRule>(db, this,  (GetObjTbl_t) &_dbTechLayer::getObjectTable,dbTechMinEncRuleObj, 8, 3);
    ZALLOCATED(_min_enc_rules_tbl);

    _v55inf_tbl = new dbTable<_dbTechV55InfluenceEntry>(db, this,  (GetObjTbl_t) &_dbTechLayer::getObjectTable,dbTechV55InfluenceEntryObj, 8, 3);
    ZALLOCATED(_v55inf_tbl);
}

_dbTechLayer::_dbTechLayer( _dbDatabase * db, const _dbTechLayer & l )
        : _flags(l._flags),
          _pitch(l._pitch),
          _width(l._width),
          _spacing(l._spacing),
          _resistance(l._resistance),
          _capacitance(l._capacitance),
          _edge_capacitance(l._edge_capacitance),
          _wire_extension(l._wire_extension),
          _number(l._number),
          _rlevel(l._rlevel),
          _area(l._area),
          _thickness(l._thickness),
          _min_step(l._min_step),
          _max_width(l._max_width),
          _pt(l._pt),
          _name(NULL),
          _alias(NULL),
          _upper(l._upper),
          _lower(l._lower),
          _v55sp_length_idx(l._v55sp_length_idx),
          _v55sp_width_idx(l._v55sp_width_idx),
          _v55sp_spacing(l._v55sp_spacing),
          _oxide1(l._oxide1),
          _oxide2(l._oxide2)
{
    if ( l._name )
    {
        _name = strdup(l._name);
        ZALLOCATED(_name);
    }

    if ( l._alias )
    {
        _alias = strdup(l._alias);
        ZALLOCATED(_alias);
    }

    _spacing_rules_tbl = new dbTable<_dbTechLayerSpacingRule>(db, this, *l._spacing_rules_tbl);
    ZALLOCATED(_spacing_rules_tbl);

    _min_cut_rules_tbl = new dbTable<_dbTechMinCutRule>(db, this, *l._min_cut_rules_tbl);
    ZALLOCATED(_min_cut_rules_tbl);

    _min_enc_rules_tbl = new dbTable<_dbTechMinEncRule>(db, this, *l._min_enc_rules_tbl);
    ZALLOCATED(_min_enc_rules_tbl);

    _v55inf_tbl = new dbTable<_dbTechV55InfluenceEntry>(db, this, *l._v55inf_tbl);
    ZALLOCATED(_v55inf_tbl);
}

_dbTechLayer::~_dbTechLayer()
{
  if ( _name )
    free( (void *) _name );

  if (_spacing_rules_tbl)
    delete _spacing_rules_tbl;

  if (_min_cut_rules_tbl)
    delete _min_cut_rules_tbl;

  if (_min_enc_rules_tbl)
    delete _min_enc_rules_tbl;

  if (_v55inf_tbl)
    delete _v55inf_tbl;
}

dbOStream & operator<<( dbOStream & stream, const _dbTechLayer & layer )
{
    uint * bit_field = (uint *) &layer._flags;
    stream << *bit_field;
    stream << layer._pitch;
    stream << layer._width;
    stream << layer._spacing;
    stream << layer._resistance;
    stream << layer._capacitance;
    stream << layer._edge_capacitance;
    stream << layer._wire_extension;
    stream << layer._number;
    stream << layer._rlevel;
    stream << layer._area;
    stream << layer._thickness;
    stream << layer._min_step;
    stream << layer._max_width;
    stream << layer._pt._width;
    stream << layer._pt._length;
    stream << layer._pt._from_width;
    stream << layer._name;
    stream << layer._alias;
    stream << layer._lower;
    stream << layer._upper;
    stream << *layer._spacing_rules_tbl;
    stream << *layer._min_cut_rules_tbl;
    stream << *layer._min_enc_rules_tbl;
    stream << *layer._v55inf_tbl;
    stream << layer._v55sp_length_idx;
    stream << layer._v55sp_width_idx;
    stream << layer._v55sp_spacing;
    stream << layer._oxide1;
    stream << layer._oxide2;

    return stream;
}

//  NOTE: Must pre-allocate pointer members that are in tables.
//  Will change this when partial specialization of templates is supported
//  in Windows.
dbIStream & operator>>( dbIStream & stream, _dbTechLayer & layer )
{
  //uint tparea;
    uint * bit_field = (uint *) &layer._flags;
    stream >> *bit_field;
    stream >> layer._pitch;
    stream >> layer._width;
    stream >> layer._spacing;
    stream >> layer._resistance;
    stream >> layer._capacitance;
    stream >> layer._edge_capacitance;
    stream >> layer._wire_extension;
    stream >> layer._number;
    stream >> layer._rlevel;
    stream >> layer._area;
    stream >> layer._thickness;
    stream >> layer._min_step;
    stream >> layer._max_width;
    stream >> layer._pt._width;
    stream >> layer._pt._length;
    stream >> layer._pt._from_width;
    stream >> layer._name;
    stream >> layer._alias;
    stream >> layer._lower;
    stream >> layer._upper;
    stream >> *layer._spacing_rules_tbl;
    stream >> *layer._min_cut_rules_tbl;
    stream >> *layer._min_enc_rules_tbl;
    stream >> *layer._v55inf_tbl;
    stream >> layer._v55sp_length_idx;
    stream >> layer._v55sp_width_idx;
    stream >> layer._v55sp_spacing;
    stream >> layer._oxide1;
    stream >> layer._oxide2;
    return stream;
}

dbObjectTable * _dbTechLayer::getObjectTable( dbObjectType type )
{
    switch( type )
    {
        case dbTechLayerSpacingRuleObj:
            return _spacing_rules_tbl;
            
        case dbTechMinCutRuleObj:
            return _min_cut_rules_tbl;

        case dbTechMinEncRuleObj:
            return _min_enc_rules_tbl;

        case dbTechV55InfluenceEntryObj:
            return _v55inf_tbl;
        default:
            break; //Wall
    }
    
    return getTable()->getObjectTable(type);
}

////////////////////////////////////////////////////////////////////
//
// dbTechLayer - Methods
//
////////////////////////////////////////////////////////////////////

dbString
dbTechLayer::getName()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    dbString n( layer->_name );
    return n;
}
const char *
dbTechLayer::getConstName()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return layer->_name;
}

bool
dbTechLayer::hasAlias()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return layer->_flags._has_alias == 1;
}

dbString
dbTechLayer::getAlias()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;

    if ( layer->_alias == NULL )
    {
        dbString n("");
        return n;
    }
        
    dbString n( layer->_alias );
    return n;
}

void
dbTechLayer::setAlias( const char * alias )
{
    _dbTechLayer * layer = (_dbTechLayer *) this;

    if ( layer->_alias )
        free( (void *) layer->_alias);

    layer->_flags._has_alias = 1;
    layer->_alias = strdup(alias);
    ZALLOCATED(layer->_alias);
}

uint
dbTechLayer::getWidth()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return layer->_width;
}

void
dbTechLayer::setWidth( int width )
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    layer->_width = width;
}

int
dbTechLayer::getSpacing()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return layer->_spacing;
}

void
dbTechLayer::setSpacing( int spacing )
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    layer->_spacing = spacing;
}

double
dbTechLayer::getEdgeCapacitance()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return layer->_edge_capacitance;
}

void 
dbTechLayer::setEdgeCapacitance( double cap )
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    layer->_edge_capacitance = cap;
}

uint 
dbTechLayer::getWireExtension()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return layer->_wire_extension;
}

void 
dbTechLayer::setWireExtension( uint ext )
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    layer->_wire_extension = ext;
}

int
dbTechLayer::getSpacing( int w, int l )
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  dbSet<dbTechLayerSpacingRule> v54rules;

  bool found_spacing = false;
  uint spacing = MAX_INT;

  bool found_over_spacing = false;
  uint over_spacing = MAX_INT;
  uint width = (uint) w;
  uint length = (uint) l;

  if (getV54SpacingRules(v54rules))
    {
      dbSet<dbTechLayerSpacingRule>::iterator  ritr;
      dbTechLayerSpacingRule * cur_rule;
      uint rmin, rmax;

      for (ritr = v54rules.begin(); ritr != v54rules.end(); ++ritr)
	{
	  cur_rule = *ritr;
	  if (cur_rule->getRange(rmin,rmax))
            {
              if ((width >= rmin) && (width <= rmax))
	        {
	          spacing = MIN(spacing, cur_rule->getSpacing());
	          found_spacing = true;
                }
	      if (width > rmax)
	        {
	          found_over_spacing = true;
	          over_spacing = MIN(over_spacing, cur_rule->getSpacing());
	        }
            }
	}
    }

  std::vector< std::vector<uint> > v55rules;
  uint i,j;
  if (getV55SpacingTable(v55rules))
    {
      for(i = 1; (i < layer->_v55sp_width_idx.size()) && (width > layer->_v55sp_width_idx[i]); i++);
      for(j = 1; (j < layer->_v55sp_length_idx.size()) && (length > layer->_v55sp_length_idx[j]); j++);
      found_spacing = true;
      spacing = v55rules[i-1][j-1];
    }

  if ((!found_spacing) && (found_over_spacing))
    {
      found_spacing = true;
      spacing = over_spacing;
    }

  return (found_spacing) ? spacing : layer->_spacing ;
}

//
// Get the low end of the uppermost range for wide wire design rules.
//
void
dbTechLayer::getMaxWideDRCRange( int & owidth, int & olength )
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  dbSet<dbTechLayerSpacingRule> v54rules;

  owidth = olength = getWidth();

  if (getV54SpacingRules(v54rules))
    {
      dbSet<dbTechLayerSpacingRule>::iterator  ritr;
      uint rmin, rmax;

      for (ritr = v54rules.begin(); ritr != v54rules.end(); ++ritr)
	{
	  if ((*ritr)->getRange(rmin,rmax))
	    {
	      if (rmin > (uint) owidth)
		owidth = olength = rmin;
	    }
	}
    }

  if (hasV55SpacingRules())
    {
      owidth =  layer->_v55sp_width_idx[layer->_v55sp_width_idx.size()-1];
      olength =  layer->_v55sp_length_idx[layer->_v55sp_length_idx.size()-1];
    }
}

//
// Get the low end of the lowermost range for wide wire design rules.
//
void
dbTechLayer::getMinWideDRCRange( int & owidth, int & olength )
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  dbSet<dbTechLayerSpacingRule> v54rules;

  owidth = olength = getWidth();

  if (getV54SpacingRules(v54rules))
    {
      dbSet<dbTechLayerSpacingRule>::iterator  ritr;
      uint rmin, rmax;
      bool range_found = false;

      for (ritr = v54rules.begin(); ritr != v54rules.end(); ++ritr)
	{
	  if ((*ritr)->getRange(rmin,rmax))
	    {
	      if ((rmin < (uint) owidth) || !range_found)
		owidth = olength = rmin;
	    }
	}
    }

  if (hasV55SpacingRules())
    {
      owidth =  layer->_v55sp_width_idx[1];
      olength =  layer->_v55sp_length_idx[1];
    }
}

bool
dbTechLayer::getV54SpacingRules( dbSet<dbTechLayerSpacingRule> & sp_rules ) const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  
  sp_rules = dbSet<dbTechLayerSpacingRule>(layer, layer->_spacing_rules_tbl);
  return true;
}

bool
dbTechLayer::hasV55SpacingRules() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  return ((layer->_v55sp_length_idx.size() > 0) && (layer->_v55sp_width_idx.size() > 0) && (layer->_v55sp_spacing.numElems() > 0));
}

void
dbTechLayer::printV55SpacingRules(lefout  & writer) const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
	
  fprintf(writer.out(), "SPACINGTABLE\n");
  fprintf(writer.out(), "  PARALLELRUNLENGTH");
  dbVector<uint>::const_iterator  v55_itr;
  uint wddx, lndx;

  for (v55_itr = layer->_v55sp_length_idx.begin(); v55_itr != layer->_v55sp_length_idx.end(); v55_itr++)
    fprintf(writer.out(), " %.3f", writer.lefdist(*v55_itr));

  for (wddx = 0, v55_itr = layer->_v55sp_width_idx.begin(); v55_itr != layer->_v55sp_width_idx.end(); wddx++, v55_itr++)
    {
      fprintf(writer.out(), "\n");
      fprintf(writer.out(), "  WIDTH %.3f\t", writer.lefdist(*v55_itr));
      for (lndx = 0; lndx < layer->_v55sp_spacing.numCols(); lndx++)
	fprintf(writer.out(), " %.3f", writer.lefdist(layer->_v55sp_spacing(wddx,lndx)));
    }

  fprintf(writer.out(), " ;\n");
}

bool
dbTechLayer::getV55SpacingTable( std::vector< std::vector<uint> > & sptbl ) const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;

  if (layer->_v55sp_spacing.numElems() == 0)
    return false;

  uint i,j;
  sptbl.clear();
  sptbl.resize(layer->_v55sp_spacing.numRows()); 
  std::vector<uint> tmpvec;
  tmpvec.reserve(layer->_v55sp_spacing.numCols());
  for (i = 0; i < layer->_v55sp_spacing.numRows(); i++)
    {
      tmpvec.clear();
      for (j = 0; j < layer->_v55sp_spacing.numCols(); j++)
	tmpvec.push_back(layer->_v55sp_spacing(i,j));
      sptbl[i] = tmpvec;
    }

  return true;
}

void
dbTechLayer::initV55LengthIndex( uint numelems )
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  layer->_v55sp_length_idx.reserve(numelems);
}

void
dbTechLayer::addV55LengthEntry( uint length )
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  layer->_v55sp_length_idx.push_back(length);
}

void
dbTechLayer::initV55WidthIndex( uint numelems )
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  layer->_v55sp_width_idx.reserve(numelems);
}

void
dbTechLayer::addV55WidthEntry( uint width )
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  layer->_v55sp_width_idx.push_back(width);
}

void
dbTechLayer::initV55SpacingTable( uint numrows, uint numcols )
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  layer->_v55sp_spacing.resize(numrows,numcols);
}


void
dbTechLayer::addV55SpacingTableEntry( uint inrow, uint incol, uint spacing )
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  layer->_v55sp_spacing(inrow,incol) = spacing;
}


bool
dbTechLayer::getV55InfluenceRules( std::vector<dbTechV55InfluenceEntry *> & inf_tbl ) 
{
    inf_tbl.clear();
    dbSet<dbTechV55InfluenceEntry> entries = getV55InfluenceEntries();

    if ( entries.size() == 0 )
        return false;
    
    dbSet<dbTechV55InfluenceEntry>::iterator itr;

    for( itr = entries.begin(); itr != entries.end(); ++itr )
        inf_tbl.push_back( *itr );
    
    return true;
}

bool
dbTechLayer::getMinimumCutRules( std::vector<dbTechMinCutRule *> & cut_rules )
{
    cut_rules.clear();
    dbSet<dbTechMinCutRule> rules = getMinCutRules();

    if ( rules.size() == 0 )
        return false;
    
    dbSet<dbTechMinCutRule>::iterator itr;

    for( itr = rules.begin(); itr != rules.end(); ++itr )
      cut_rules.push_back((dbTechMinCutRule *) *itr);
        
    return true;
}

dbSet<dbTechMinCutRule> dbTechLayer::getMinCutRules()
{
    dbSet<dbTechMinCutRule> rules;
    _dbTechLayer * layer = (_dbTechLayer *) this;
    rules = dbSet<dbTechMinCutRule>(layer, layer->_min_cut_rules_tbl);
    return rules;
}

dbSet<dbTechMinEncRule> dbTechLayer::getMinEncRules()
{
    dbSet<dbTechMinEncRule> rules;
    _dbTechLayer * layer = (_dbTechLayer *) this;
    rules = dbSet<dbTechMinEncRule>(layer, layer->_min_enc_rules_tbl);
    return rules;
}

dbSet<dbTechV55InfluenceEntry> dbTechLayer::getV55InfluenceEntries()
{
    dbSet<dbTechV55InfluenceEntry> rules;
    _dbTechLayer * layer = (_dbTechLayer *) this;
    rules = dbSet<dbTechV55InfluenceEntry>(layer, layer->_v55inf_tbl);
    return rules;
}

bool
dbTechLayer::getMinEnclosureRules( std::vector<dbTechMinEncRule *> & enc_rules )
{
    enc_rules.clear();
    
    dbSet<dbTechMinEncRule> rules = getMinEncRules();

    if ( rules.size() == 0 )
        return false;
  
    dbSet<dbTechMinEncRule>::iterator itr;

    for( itr = rules.begin(); itr != rules.end(); ++itr )
        enc_rules.push_back(*itr);
  
    return true;
}

dbTechLayerAntennaRule *
dbTechLayer::createDefaultAntennaRule()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    _dbTechLayerAntennaRule * r = (_dbTechLayerAntennaRule *) getDefaultAntennaRule();

    // Reinitialize the object to its default state...
    if ( r != NULL )
    {
        r->~_dbTechLayerAntennaRule();
        new(r) _dbTechLayerAntennaRule(getDatabase());
        r->_layer = getOID();
    }
    else
    {
        _dbTech * tech = (_dbTech *) getOwner();
        r = tech->_antenna_rule_tbl->create();
        layer->_oxide1 = r->getOID();
        r->_layer = getOID();
    }

    return (dbTechLayerAntennaRule *) r;
}

dbTechLayerAntennaRule *
dbTechLayer::createOxide2AntennaRule()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    _dbTechLayerAntennaRule * r = (_dbTechLayerAntennaRule *) getOxide2AntennaRule();

    // Reinitialize the object to its default state...
    if ( r != NULL )
    {
        r->~_dbTechLayerAntennaRule();
        new(r) _dbTechLayerAntennaRule(getDatabase());
        r->_layer = getOID();
    }
    else
    {
        _dbTech * tech = (_dbTech *) getOwner();
        r = tech->_antenna_rule_tbl->create();
        layer->_oxide2 = r->getOID();
        r->_layer = getOID();
    }

    return (dbTechLayerAntennaRule *) r;
}

bool
dbTechLayer::hasDefaultAntennaRule() const
{   
  _dbTechLayer * layer = (_dbTechLayer *) this;
  return (layer->_oxide1 != 0);
}

bool
dbTechLayer::hasOxide2AntennaRule() const
{   
  _dbTechLayer * layer = (_dbTechLayer *) this;
  return (layer->_oxide2 != 0);
}

dbTechLayerAntennaRule *
dbTechLayer::getDefaultAntennaRule() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  _dbTech * tech = (_dbTech*) getOwner();

  if ( layer->_oxide1 == 0 )
      return NULL;
  
  return (dbTechLayerAntennaRule *) tech->_antenna_rule_tbl->getPtr(layer->_oxide1);
}

dbTechLayerAntennaRule *
dbTechLayer::getOxide2AntennaRule() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  _dbTech * tech = (_dbTech*) getOwner();

  if ( layer->_oxide2 == 0 )
      return NULL;

  return (dbTechLayerAntennaRule *) tech->_antenna_rule_tbl->getPtr(layer->_oxide2);
}

void
dbTechLayer::writeAntennaRulesLef(lefout & writer) const
{
  bool prt_model = (hasDefaultAntennaRule() && hasOxide2AntennaRule());

  if (prt_model)
    fprintf(writer.out(),"    ANTENNAMODEL OXIDE1 ;\n");
  if (hasDefaultAntennaRule())
      getDefaultAntennaRule()->writeLef(writer);

  if (prt_model)
    fprintf(writer.out(),"    ANTENNAMODEL OXIDE2 ;\n");
  if (hasOxide2AntennaRule())
      getOxide2AntennaRule()->writeLef(writer);

}


bool
dbTechLayer::getThickness(uint & inthk) const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  if (layer->_flags._has_thickness)
    {
      inthk = layer->_thickness;
      return true;
    }

  return false;
}

void
dbTechLayer::setThickness(uint thickness)
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  layer->_flags._has_thickness = 1;
  layer->_thickness = thickness;
}

bool
dbTechLayer::hasArea() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  return (layer->_flags._has_area);
}

double   // Now denominated in squm
dbTechLayer::getArea() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  if (layer->_flags._has_area)
    return layer->_area;

  return 0.0;  // Default
}

void
dbTechLayer::setArea(double area)
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  layer->_flags._has_area = 1;
  layer->_area = area;
}

bool
dbTechLayer::hasMaxWidth() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  return (layer->_flags._has_max_width);
}

uint
dbTechLayer::getMaxWidth() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  if (layer->_flags._has_max_width)
    return layer->_max_width;

  return MAX_INT;  // Default
}

void
dbTechLayer::setMaxWidth(uint max_width)
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  layer->_flags._has_max_width = 1;
  layer->_max_width = max_width;
}


bool
dbTechLayer::hasMinStep() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  return (layer->_flags._has_min_step);
}

uint
dbTechLayer::getMinStep() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  if (layer->_flags._has_min_step)
    return layer->_min_step;

  return 0;  // Default
}

void
dbTechLayer::setMinStep(uint min_step)
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  layer->_flags._has_min_step = 1;
  layer->_min_step = min_step;
}

bool
dbTechLayer::hasProtrusion() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  return (layer->_flags._has_protrusion);
}

uint
dbTechLayer::getProtrusionWidth() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  if (layer->_flags._has_protrusion)
    return layer->_pt._width;

  return 0;  // Default
}

uint
dbTechLayer::getProtrusionLength() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  if (layer->_flags._has_protrusion)
    return layer->_pt._length;

  return 0;  // Default
}

uint
dbTechLayer::getProtrusionFromWidth() const
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  if (layer->_flags._has_protrusion)
    return layer->_pt._from_width;

  return 0;  // Default
}

void
dbTechLayer::setProtrusion(uint pt_width, uint pt_length, uint pt_from_width)
{
  _dbTechLayer * layer = (_dbTechLayer *) this;
  layer->_flags._has_protrusion = 1;
  layer->_pt._width = pt_width;
  layer->_pt._length = pt_length;
  layer->_pt._from_width = pt_from_width;
}


int
dbTechLayer::getPitch()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return layer->_pitch;
}

void
dbTechLayer::setPitch( int pitch )
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    layer->_pitch = pitch;
}

dbTechLayerDir
dbTechLayer::getDirection()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return dbTechLayerDir(layer->_flags._direction);
}

void
dbTechLayer::setDirection( dbTechLayerDir direction )
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    layer->_flags._direction = direction.getValue();
}

dbTechLayerType
dbTechLayer::getType()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return dbTechLayerType(layer->_flags._type);
}

double
dbTechLayer::getResistance()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return layer->_resistance;
}

void
dbTechLayer::setResistance( double resistance )
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    layer->_resistance = resistance;
}

double
dbTechLayer::getCapacitance()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return layer->_capacitance;
}

void
dbTechLayer::setCapacitance( double capacitance )
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    layer->_capacitance = capacitance;
}

int
dbTechLayer::getNumber() const
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return layer->_number;
}

int
dbTechLayer::getRoutingLevel()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    return layer->_rlevel;
}


dbTechLayer *
dbTechLayer::getLowerLayer()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    _dbTech * tech = (_dbTech *) getOwner();

    if ( layer->_lower == 0 )
        return NULL;

    return (dbTechLayer *) tech->_layer_tbl->getPtr(layer->_lower);
}

dbTechLayer *
dbTechLayer::getUpperLayer()
{
    _dbTechLayer * layer = (_dbTechLayer *) this;
    _dbTech * tech = (_dbTech *) getOwner();

    if ( layer->_upper == 0 )
        return NULL;

    return (dbTechLayer *) tech->_layer_tbl->getPtr(layer->_upper);
}

dbTech *
dbTechLayer::getTech()
{
    return (dbTech *) getOwner();
}

dbTechLayer *
dbTechLayer::create( dbTech * tech_, const char * name_, dbTechLayerType type )
{
    if ( type.getValue()== dbTechLayerType::NONE )
		return NULL;

    if ( tech_->findLayer(name_) )
        return NULL;

    _dbTech * tech = (_dbTech *) tech_;
    _dbTechLayer * layer = tech->_layer_tbl->create();
    layer->_name = strdup(name_);
    ZALLOCATED(layer->_name);
    layer->_number = tech->_layer_cnt++;
    layer->_flags._type = type.getValue();

    if ( type.getValue()== dbTechLayerType::ROUTING )
    {
        layer->_rlevel = ++tech->_rlayer_cnt;
    }

    if ( tech->_bottom == 0 )
    {
        tech->_bottom = layer->getOID();
        tech->_top = layer->getOID();
        return (dbTechLayer *) layer;
    }

    _dbTechLayer * top = tech->_layer_tbl->getPtr(tech->_top);
    top->_upper = layer->getOID();
    layer->_lower = top->getOID();
    tech->_top = layer->getOID();

    return (dbTechLayer *) layer;
}

dbTechLayer *
dbTechLayer::getTechLayer( dbTech * tech_, uint dbid_ )
{
    _dbTech * tech = (_dbTech *) tech_;
    return (dbTechLayer *) tech->_layer_tbl->getPtr( dbid_ );
}

} // namespace
