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

#ifdef WIN32
#pragma warning (disable : 4786)
#endif

#include <stdio.h>
#include <ctype.h>

#ifndef WIN32
#   include <unistd.h>
#else
#   include <windows.h>
#endif

#include "lef/lefrReader.hpp"
#include "lef/lefiDebug.hpp"
#include "lef/lefiUtil.hpp"
#include "db.h"
#include "dbTransform.h"
#include "geom.h"
#include "poly_decomp.h"
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include "lefin.h"
#include "logger.h"

extern int lefrRelaxMode; // This variable turns off strick checking in the lef 5.6 parser.

namespace odb {

extern bool lefin_parse( lefin *, const char * );

static bool streq( const char * s1, const char * s2 )
{
    while( *s1 && *s2 )
    {
        if ( toupper(*s1) != toupper(*s2) )
            return false;

        ++s1;
        ++s2;
    }

    return ( *s1 == *s2 );
}

lefin::lefin( dbDatabase * db, bool ignore_non_routing_layers )
        : _db(db),
          _tech(NULL),
          _master(NULL),
          _create_tech(false),
          _create_lib(false),
          _skip_obstructions(false),
          _left_bus_delimeter(0),
          _right_bus_delimeter(0),
          _hier_delimeter(0),
          _layer_cnt(0),
          _master_cnt(0),
          _via_cnt(0),
          _errors(0),
          _lef_units(0),
          _lib_name(NULL),
          _dist_factor(1000.0),
          _area_factor(1000000.0),
          _dbu_per_micron(1000),
          _override_lef_dbu(false),
          _ignore_non_routing_layers(ignore_non_routing_layers)
{
}

void lefin::init()
{
    _tech = NULL;
    _lib = NULL;
    _master = NULL;
    _create_tech = false;
    _create_lib = false;
    _left_bus_delimeter = 0;
    _right_bus_delimeter = 0;
    _hier_delimeter = 0;
    _layer_cnt = 0;
    _master_cnt = 0;
    _via_cnt = 0;
    _errors = 0;

    if ( ! _override_lef_dbu )
    {
        _lef_units = 0;
        _dist_factor = 1000.0;
        _area_factor = 1000000.0;
        _dbu_per_micron = 1000;
    }
}

lefin::~lefin()
{
}

void lefin::createLibrary()
{
    _lib = dbLib::create( _db, _lib_name, _hier_delimeter );
    _lib->setLefUnits(_lef_units);
    if ( _left_bus_delimeter )
        _lib->setBusDelimeters( _left_bus_delimeter, _right_bus_delimeter );
}

static void create_path_box( dbObject * obj, bool is_pin, dbTechLayer * layer,
                             int dw,
                             int prev_x, int prev_y,
                             int cur_x, int cur_y )
{
    int x1, x2, y1, y2;
    
    if ( (cur_x == prev_x) && (cur_y == prev_y) ) // single point
    {
        x1 = cur_x - dw;
        y1 = cur_y - dw;
        x2 = cur_x + dw;
        y2 = cur_y + dw;

        if( is_pin )
            dbBox::create( (dbMPin *) obj, layer, x1, y1, x2, y2);
        else
            dbBox::create( (dbMaster *) obj, layer, x1, y1, x2, y2);
    }
    else if ( cur_x == prev_x ) // vert. path
    {
        x1 = cur_x - dw;
        x2 = cur_x + dw;

        if ( cur_y > prev_y )
        {
            y1 = prev_y - dw;
            y2 = cur_y + dw;
        }
        else
        {
            y1 = cur_y - dw;
            y2 = prev_y + dw;
        }
        
        if( is_pin )
            dbBox::create( (dbMPin *) obj, layer, x1, y1, x2, y2);
        else
            dbBox::create( (dbMaster *) obj, layer, x1, y1, x2, y2);
    }
    else if ( cur_y == prev_y ) // horiz. path
    {
        y1 = cur_y - dw;
        y2 = cur_y + dw;

        if ( cur_x > prev_x )
        {
            x1 = prev_x - dw;
            x2 = cur_x + dw;
        }
        else
        {
            x1 = cur_x - dw;
            x2 = prev_x + dw;
        }

        if( is_pin )
            dbBox::create( (dbMPin *) obj, layer, x1, y1, x2, y2);
        else
            dbBox::create( (dbMaster *) obj, layer, x1, y1, x2, y2);
    }
    else
    {
        assert(0); // illegal: non-orthogonal-path
    }
}

//
// add geoms to master or terminal
//
bool lefin::addGeoms( dbObject * object, bool is_pin, lefiGeometries * geometry)
{
    int count = geometry->numItems();
    int i;
    dbTechLayer * layer = NULL;
    int dw = 0;

    for( i = 0; i < count; i++ )
    {
        _master_modified = true;

        switch( geometry->itemType(i) )
        {
            case lefiGeomLayerE:
            {
                layer = _tech->findLayer( geometry->getLayer(i) );

                if ( layer == NULL )
                {
                    notice(0,"error: undefined layer (%s) referenced\n", geometry->getLayer(i) );
                    return false;
                }

                dw = dbdist(layer->getWidth()) >> 1;
                break;
            }
            case lefiGeomWidthE:
            {
                dw = dbdist(geometry->getWidth(i)) >> 1;
                break;
            }
            case lefiGeomPathE:
            {
                lefiGeomPath * path = geometry->getPath(i);

                if ( path->numPoints == 1 )
                {
                    int x = dbdist(path->x[0]);
                    int y = dbdist(path->y[0]);
                    create_path_box( object, is_pin, layer, dw, x, y, x, y );
                    break;
                }
                
                int prev_x = dbdist(path->x[0]);
                int prev_y = dbdist(path->y[0]);
                int j;

                for( j = 1; j < path->numPoints; j++ )
                {
                    int cur_x = dbdist(path->x[j]);
                    int cur_y = dbdist(path->y[j]);
                    create_path_box( object, is_pin, layer, dw, prev_x, prev_y, cur_x, cur_y );
                    prev_x = cur_x;
                    prev_y = cur_y;
                }
                break;
            }
            case lefiGeomPathIterE:
            {
                lefiGeomPathIter * pathItr = geometry->getPathIter(i);
                int j;
                std::vector<adsPoint> points;

                for( j = 0; j < pathItr->numPoints; j++ )
                {
                    int x = dbdist(pathItr->x[j]);
                    int y = dbdist(pathItr->y[j]);
                    points.push_back( adsPoint( x, y ) );
                }

                int numX = round(pathItr->xStart);
                int numY = round(pathItr->yStart);
                int stepX = dbdist(pathItr->xStep);
                int stepY = dbdist(pathItr->yStep);
                int dx, dy, x_idx, y_idx;

                for( dx = 0, x_idx = 0; x_idx < numX; ++x_idx, dx += stepX )
                {
                    for( dy = 0, y_idx = 0; y_idx < numY; ++y_idx, dy += stepY )
                    {
                        if ( points.size() == 1 )
                        {
                            adsPoint p = points[0];
                            int x = p.getX() + dx;
                            int y = p.getY() + dy;
                            create_path_box( object, is_pin, layer, dw, x, y, x, y );
                            continue;
                        }

                        std::vector<adsPoint>::iterator itr = points.begin();
                        adsPoint p = *itr;
                        int prev_x = p.getX() + dx;
                        int prev_y = p.getY() + dy;

                        for( ++itr; itr != points.end(); ++itr )
                        {
                            adsPoint c = *itr;
                            int cur_x = c.getX() + dx;
                            int cur_y = c.getY() + dy;
                            create_path_box( object, is_pin, layer, dw, cur_x, cur_y, prev_x, prev_y );
                            prev_x = cur_x;
                            prev_y = cur_y;
                        }
                    }
                }
                break;
            }
            case lefiGeomRectE:
            {
                lefiGeomRect * rect = geometry->getRect(i);
                int x1 = dbdist(rect->xl);
                int y1 = dbdist(rect->yl);
                int x2 = dbdist(rect->xh);
                int y2 = dbdist(rect->yh);

                if( is_pin )
                    dbBox::create( (dbMPin *) object, layer, x1, y1, x2, y2);
                else
                    dbBox::create( (dbMaster *) object, layer, x1, y1, x2, y2);
                break;
            }
            case lefiGeomRectIterE:
            {
                lefiGeomRectIter * rectItr = geometry->getRectIter(i);
                int x1 = dbdist(rectItr->xl);
                int y1 = dbdist(rectItr->yl);
                int x2 = dbdist(rectItr->xh);
                int y2 = dbdist(rectItr->yh);
                int numX = round(rectItr->xStart);
                int numY = round(rectItr->yStart);
                int stepX = dbdist(rectItr->xStep);
                int stepY = dbdist(rectItr->yStep);
                int dx, dy, x_idx, y_idx;

                for( dx = 0, x_idx = 0; x_idx < numX; ++x_idx, dx += stepX )
                {
                    for( dy = 0, y_idx = 0; y_idx < numY; ++y_idx, dy += stepY )
                    {
                        if( is_pin )
                            dbBox::create( (dbMPin *) object, layer, x1 + dx, y1 + dy, x2 + dx, y2 + dy);
                        else
                            dbBox::create( (dbMaster *) object, layer, x1 + dx, y1 + dy, x2 + dx, y2 + dy);
                    }
                }
                break;
            }
            case lefiGeomPolygonE:
            {
                createPolygon( object, is_pin, layer, geometry->getPolygon(i) );
                break;
            }
            case lefiGeomPolygonIterE:
            {
                lefiGeomPolygonIter * pItr = geometry->getPolygonIter(i);
                lefiGeomPolygon p;
                double x;
                double y;

                p.numPoints = pItr->numPoints;
                p.x = pItr->x;
                p.y = pItr->y;

                for (y = 0; y < pItr->yStart; y++)
                    for (x = 0; x < pItr->xStart; x++)
                        createPolygon(object, is_pin, layer, &p, x * pItr->xStep, y * pItr->yStep);
                break;
            }
            case lefiGeomViaE:
            {
                lefiGeomVia * via = geometry->getVia(i);
                dbTechVia * dbvia = _tech->findVia( via->name );

                if ( dbvia == NULL )
                {
                    notice(0,"error: undefined via (%s) referenced\n", via->name );
                    return false;
                }

                int x = dbdist(via->x);
                int y = dbdist(via->y);

                if ( is_pin )
                    dbBox::create( (dbMPin *) object, dbvia, x, y );
                else
                    dbBox::create( (dbMaster *) object, dbvia, x, y );
                
                break;
            }
            case lefiGeomViaIterE:
            {
                lefiGeomViaIter * viaItr = geometry->getViaIter(i);
                dbTechVia * dbvia = _tech->findVia( viaItr->name );

                if ( dbvia == NULL )
                {
                    notice(0,"error: undefined via (%s) referenced\n", viaItr->name );
                    return false;
                }

                int x = dbdist(viaItr->x);
                int y = dbdist(viaItr->y);
                int numX = round(viaItr->xStart);
                int numY = round(viaItr->yStart);
                int stepX = dbdist(viaItr->xStep);
                int stepY = dbdist(viaItr->yStep);
                int dx, dy, x_idx, y_idx;

                for( dx = 0, x_idx = 0; x_idx < numX; ++x_idx, dx += stepX )
                {
                    for( dy = 0, y_idx = 0; y_idx < numY; ++y_idx, dy += stepY )
                    {
                        if ( is_pin )
                            dbBox::create( (dbMPin *) object, dbvia, x + dx, y + dy );
                        else
                            dbBox::create( (dbMaster *) object, dbvia, x + dx, y + dy );
                    }
                }
            }

            default:
                break;
        }
    }

    return true;
}

void lefin::createPolygon( dbObject * object, bool is_pin, dbTechLayer * layer, lefiGeomPolygon * p, double offset_x, double offset_y )
{
    std::vector<adsPoint> P;
    int j;

    for( j = 0; j < p->numPoints; ++j )
    {
        int x = dbdist(p->x[j] + offset_x);
        int y = dbdist(p->y[j] + offset_y);
        P.push_back( adsPoint(x,y) );
    }

    if ( p->numPoints < 4 )
        return;

    if ( P[0] == P[P.size()-1] )
        P.pop_back();

    if ( p->numPoints < 4 )
        return;

    if ( ! polygon_is_clockwise(P) )
        std::reverse(P.begin(), P.end());
    
    std::vector<adsRect> R;
    decompose_polygon( P, R );

    std::vector<adsRect>::iterator itr;

    for( itr = R.begin(); itr != R.end(); ++itr )
    {
        adsRect & r = *itr;

        if ( is_pin )
            dbBox::create( (dbMPin *) object, layer, r.xMin(), r.yMin(), r.xMax(), r.yMax() );
        else
            dbBox::create( (dbMaster *) object, layer, r.xMin(), r.yMin(), r.xMax(), r.yMax() );
    }
}

void lefin::antenna( lefin::AntennaType type, double value )
{
}

void lefin::arrayBegin( const char * name )
{
        /* Gate arrays not supported */
}

void lefin::array( lefiArray * a )
{
        /* Gate arrays not supported */
}

void lefin::arrayEnd( const char * name )
{
        /* Gate arrays not supported */
}

void lefin::busBitChars( const char * busBit )
{
    if ( (busBit[0] != '\0') && (busBit[1] != '\0') )
    {
        _left_bus_delimeter = busBit[0];
        _right_bus_delimeter = busBit[1];
        if ( _lib )
            _lib->setBusDelimeters( _left_bus_delimeter, _right_bus_delimeter );
    }
}

void lefin::caseSense( int caseSense )
{
  _tech->setNamesCaseSensitive(dbOnOffType(caseSense));
}

void lefin::clearance(const char * name )
{
  _tech->setClearanceMeasure(dbClMeasureType(name));
}

void lefin::divider( const char * div )
{
    if ( div[0] != '\0' )
        _hier_delimeter = div[0];
}

void lefin::noWireExt( const char * name )
{
  _tech->setNoWireExtAtPin(dbOnOffType(name));
}

void lefin::noiseMargin( lefiNoiseMargin * noise )
{
}

void lefin::edge1( double name )
{
}

void lefin::edge2( double name )
{
}

void lefin::edgeScale( double name )
{
}

void lefin::noiseTable( lefiNoiseTable * noise )
{
}

void lefin::correction( lefiCorrectionTable * corr )
{
}

void lefin::dielectric( double dielectric )
{
}

void lefin::irdropBegin( void * ptr )
{
}

void lefin::irdrop( lefiIRDrop * irdrop )
{
}

void lefin::irdropEnd( void * ptr )
{
}

void lefin::layer( lefiLayer * layer )
{
    if ( ! _create_tech )
        return;

    if ( _tech->findLayer( layer->name() ) )
    {
        notice(0,"duplicate LAYER (%s) ignored\n", layer->name() );
        return;
    }

    dbTechLayerType type( dbTechLayerType::ROUTING );

    if (layer->hasType())
        type = dbTechLayerType( layer->type() );
	
	if (_ignore_non_routing_layers && (
		(type!=dbTechLayerType::ROUTING) &&
		(type!=dbTechLayerType::CUT) &&
		(type!=dbTechLayerType::MASTERSLICE) &&
		(type!=dbTechLayerType::OVERLAP))
		){
		
        notice(0,"Skipping LAYER (%s) ; Non Routing or Cut type\n", layer->name() );
		return;
	}
	

    dbTechLayer * l = dbTechLayer::create(_tech, layer->name(), type );
	if (l==NULL) {
        notice(0,"Skipping LAYER (%s) ; cannot understand type\n", layer->name() );
		return;
	}
    
    if ( layer->hasWidth() )
        l->setWidth( dbdist(layer->width() ) );

    if ( layer->hasPitch() )
        l->setPitch( dbdist(layer->pitch() ) );
    
    int j;
    dbTechLayerSpacingRule  *cur_rule;
    if (layer->hasSpacingNumber())
      {
        for ( j = 0; j < layer->numSpacing(); j++)
	  {
	    cur_rule = dbTechLayerSpacingRule::create(l);
	    cur_rule->setSpacing(dbdist(layer->spacing(j)));
	    if (layer->hasSpacingRange(j))
	      {
		cur_rule->setRange(dbdist(layer->spacingRangeMin(j)),
				   dbdist(layer->spacingRangeMax(j)));
		if (layer->hasSpacingRangeUseLengthThreshold(j))
		  cur_rule->setUseLengthThreshold();
		else if (layer->hasSpacingRangeInfluence(j))
		  {
		    cur_rule->setInfluence(dbdist(layer->spacingRangeInfluence(j)));
		    if (layer->hasSpacingRangeInfluenceRange(j))
		      cur_rule->setInfluenceRange(dbdist(layer->spacingRangeInfluenceMin(j)),dbdist(layer->spacingRangeInfluenceMax(j)));
		  }
		else if (layer->hasSpacingRangeRange(j))
		  cur_rule->setRangeRange(dbdist(layer->spacingRangeRangeMin(j)),dbdist(layer->spacingRangeRangeMax(j)));
	      }
	    else if (layer->hasSpacingLengthThreshold(j))
	      {
		cur_rule->setLengthThreshold(dbdist(layer->spacingLengthThreshold(j)));
		if (layer->hasSpacingLengthThresholdRange(j))
		  cur_rule->setLengthThresholdRange(dbdist(layer->spacingLengthThresholdRangeMin(j)), dbdist(layer->spacingLengthThresholdRangeMax(j)));
	      }
	    else if (layer->hasSpacingAdjacent(j))
	      {
		cur_rule->setAdjacentCuts(layer->spacingAdjacentCuts(j),
					  dbdist(layer->spacingAdjacentWithin(j)),
					  dbdist(layer->spacing(j)));
	      }
	    else if (layer->hasSpacingName(j))
	      {
		dbTechLayer *tmply = NULL;
		assert(tmply = _tech->findLayer(layer->spacingName(j)));
		cur_rule->setCutLayer4Spacing(tmply);
	      }
	    else
	      l->setSpacing( dbdist(layer->spacing(j)) );
	  }
      }

    lefiSpacingTable  *cur_sptbl;
    for (j = 0; j < layer->numSpacingTable(); j++)
      {
	cur_sptbl = layer->spacingTable(j);
	if (cur_sptbl->isInfluence())
	  {
	    lefiInfluence *cur_ifl = cur_sptbl->influence();
	    int iflidx;
	    dbTechV55InfluenceEntry  *iflitem;
	    for (iflidx = 0; iflidx < cur_ifl->numInfluenceEntry(); iflidx++)
	      {
		iflitem = dbTechV55InfluenceEntry::create(l);
		iflitem->setV55InfluenceEntry(dbdist(cur_ifl->width(iflidx)),
					      dbdist(cur_ifl->distance(iflidx)),
					      dbdist(cur_ifl->spacing(iflidx)));
	      }
	  }
	else  // Assume parallel run length spacing table
	  {
	    lefiParallel *cur_ipl = cur_sptbl->parallel();
	    int wddx,lndx;

	    l->initV55LengthIndex(cur_ipl->numLength());
	    for (lndx = 0; lndx < cur_ipl->numLength(); lndx++)
	      l->addV55LengthEntry(dbdist(cur_ipl->length(lndx)));

	    l->initV55WidthIndex(cur_ipl->numWidth());
	    l->initV55SpacingTable(cur_ipl->numWidth(),
				   cur_ipl->numLength());
	    for (wddx = 0; wddx < cur_ipl->numWidth(); wddx++)
	      {
		l->addV55WidthEntry(dbdist(cur_ipl->width(wddx)));
		for (lndx = 0; lndx < cur_ipl->numLength(); lndx++)
                  {
		    l->addV55SpacingTableEntry(wddx,lndx,dbdist(cur_ipl->widthSpacing(wddx,lndx)));
		    if ((wddx == 0) && (lndx == 0))
			l->setSpacing(dbdist(cur_ipl->widthSpacing(wddx,lndx)));
		  }
	      }
	  }
      }

    dbTechMinCutRule  *cur_cut_rule;
    bool from_above, from_below;
    for ( j = 0; j < layer->numMinimumcut(); j++)
      {
	cur_cut_rule = dbTechMinCutRule::create(l);
	from_above = from_below = false;
	if (layer->hasMinimumcutConnection(j))
	  {
	    from_above = streq(layer->minimumcutConnection(j),"FROMABOVE");
	    from_below = streq(layer->minimumcutConnection(j),"FROMBELOW");
	  }

	cur_cut_rule->setMinimumCuts( layer->minimumcut(j),
				      dbdist(layer->minimumcutWidth(j)),
				      from_above, from_below );
	
	if (layer->hasMinimumcutNumCuts(j))
	  cur_cut_rule->setLengthForCuts(dbdist(layer->minimumcutLength(j)),
					 dbdist(layer->minimumcutDistance(j)));
      }

    dbTechMinEncRule  *cur_enc_rule;
    for ( j = 0; j < layer->numMinenclosedarea(); j++)
      {
	cur_enc_rule = dbTechMinEncRule::create(l);
	cur_enc_rule->setEnclosure(dbarea(layer->minenclosedarea(j)));
	if (layer->hasMinenclosedareaWidth(j))
	  cur_enc_rule->setEnclosureWidth(dbdist(layer->minenclosedareaWidth(j)));
      }

    dbTechLayerAntennaRule *cur_ant_rule;
    lefiAntennaModel *cur_model;
    lefiAntennaPWL  *cur_pwl;
    std::vector<double>  dffdx, dffratio;
    int k;

    if (layer->numAntennaModel() > 0)
      {
	for (j = 0; j < MIN(layer->numAntennaModel(),2); j++)
	  {
	    cur_ant_rule = (j == 1) ? l->createOxide2AntennaRule() : l->createDefaultAntennaRule();
	    cur_model = layer->antennaModel(j);
	    if (cur_model->hasAntennaAreaFactor())
	      {
		if (cur_model->hasAntennaAreaFactorDUO())
		  cur_ant_rule->setAreaFactor(-1.0,
					      cur_model->antennaAreaFactor());
		else
		  cur_ant_rule->setAreaFactor(cur_model->antennaAreaFactor());
	      }

	    if (cur_model->hasAntennaSideAreaFactor())
	      {
		if (cur_model->hasAntennaSideAreaFactorDUO())
		  cur_ant_rule->setSideAreaFactor(-1.0,
						  cur_model->antennaSideAreaFactor());
		else
		  cur_ant_rule->setSideAreaFactor(cur_model->antennaSideAreaFactor());
	      }

	    if (cur_model->hasAntennaAreaRatio())
	      {
		if (cur_model->hasAntennaDiffAreaRatio())
		  cur_ant_rule->setPAR(cur_model->antennaAreaRatio(),
				       cur_model->antennaDiffAreaRatio());
		else
		  cur_ant_rule->setPAR(cur_model->antennaAreaRatio());

		if (cur_model->hasAntennaDiffAreaRatioPWL())
		  {
		    dffdx.clear(); dffratio.clear();
		    cur_pwl = cur_model->antennaDiffAreaRatioPWL();
		    for (k = 0; k < cur_pwl->numPWL(); k++)
		      {
			dffdx.push_back(cur_pwl->PWLdiffusion(k));
			dffratio.push_back(cur_pwl->PWLratio(k));
		      }
		    cur_ant_rule->setPAR_PWL(dffdx, dffratio);
		  }
	      }

	    if (cur_model->hasAntennaCumAreaRatio())
	      {
		if (cur_model->hasAntennaCumDiffAreaRatio())
		  cur_ant_rule->setCAR(cur_model->antennaCumAreaRatio(),
				       cur_model->antennaCumDiffAreaRatio());
		else
		  cur_ant_rule->setCAR(cur_model->antennaCumAreaRatio());

		if (cur_model->hasAntennaCumDiffAreaRatioPWL())
		  {
		    dffdx.clear(); dffratio.clear();
		    cur_pwl = cur_model->antennaCumDiffAreaRatioPWL();
		    for (k = 0; k < cur_pwl->numPWL(); k++)
		      {
			dffdx.push_back(cur_pwl->PWLdiffusion(k));
			dffratio.push_back(cur_pwl->PWLratio(k));
		      }
		    cur_ant_rule->setCAR_PWL(dffdx, dffratio);
		  }
	      }

	    if (cur_model->hasAntennaSideAreaRatio())
	      {
		if (cur_model->hasAntennaDiffSideAreaRatio())
		  cur_ant_rule->setPSR(cur_model->antennaSideAreaRatio(),
				       cur_model->antennaDiffSideAreaRatio());
		else
		  cur_ant_rule->setPSR(cur_model->antennaSideAreaRatio());

		if (cur_model->hasAntennaDiffSideAreaRatioPWL())
		  {
		    dffdx.clear(); dffratio.clear();
		    cur_pwl = cur_model->antennaDiffSideAreaRatioPWL();
		    for (k = 0; k < cur_pwl->numPWL(); k++)
		      {
			dffdx.push_back(cur_pwl->PWLdiffusion(k));
			dffratio.push_back(cur_pwl->PWLratio(k));
		      }
		    cur_ant_rule->setPSR_PWL(dffdx, dffratio);
		  }
	      }

	    if (cur_model->hasAntennaCumSideAreaRatio())
	      {
		if (cur_model->hasAntennaCumDiffSideAreaRatio())
		  cur_ant_rule->setCSR(cur_model->antennaCumSideAreaRatio(),
				       cur_model->antennaCumDiffSideAreaRatio());
		else
		  cur_ant_rule->setCSR(cur_model->antennaCumSideAreaRatio());

		if (cur_model->hasAntennaCumDiffSideAreaRatioPWL())
		  {
		    dffdx.clear(); dffratio.clear();
		    cur_pwl = cur_model->antennaCumDiffSideAreaRatioPWL();
		    for (k = 0; k < cur_pwl->numPWL(); k++)
		      {
			dffdx.push_back(cur_pwl->PWLdiffusion(k));
			dffratio.push_back(cur_pwl->PWLratio(k));
		      }
		    cur_ant_rule->setCSR_PWL(dffdx, dffratio);
		  }
	      }
	  }
      }

    if (layer->hasArea())
      l->setArea(layer->area());

    if (layer->hasThickness())
      l->setThickness(dbdist(layer->thickness()));

    if (layer->hasMaxwidth())
      l->setMaxWidth(dbdist(layer->maxwidth()));

    if (layer->hasMinstep())
      l->setMinStep(dbdist(layer->minstep(0)));

    if (layer->hasProtrusion())
      l->setProtrusion(dbdist(layer->protrusionWidth1()),
		       dbdist(layer->protrusionLength()),
		       dbdist(layer->protrusionWidth2())); 

    if (layer->hasDirection())
    {
        dbTechLayerDir direction( layer->direction() );
        l->setDirection( direction );
    }
    
    if (layer->hasResistance())
        l->setResistance( layer->resistance() );

    if (layer->hasCapacitance())
        l->setCapacitance( layer->capacitance() );

    if (layer->hasEdgeCap())
        l->setEdgeCapacitance( layer->edgeCap() );

    if (layer->hasWireExtension())
        l->setWireExtension( dbdist(layer->wireExtension()) );

    _layer_cnt++;
}

void lefin::macroBegin( const char * name )
{
    _master = NULL;

    if ( _create_lib )
    {
        if ( _lib == NULL )
            createLibrary();

        _master = _lib->findMaster( name );
        
        if ( _master == NULL )
            _master = dbMaster::create( _lib, name );
    }

    _master_modified = false;
}

void lefin::macro( lefiMacro * macro )
{
    if ( _master == NULL )
        return;

    if (macro->hasClass())
    {
        dbMasterType type( macro->macroClass() );
        _master->setType(type);
    }
    
    if (macro->hasEEQ())
    {
        dbMaster * eeq = _lib->findMaster( macro->EEQ() );
        _master->setEEQ(eeq);
    }

    if (macro->hasLEQ())
    {
        dbMaster * leq = _lib->findMaster( macro->LEQ() );
        _master->setLEQ(leq);
    }
    
    if (macro->hasSize())
    {
        int w = dbdist( macro->sizeX() );
        int h = dbdist( macro->sizeY() );
        _master->setWidth( w );
        _master->setHeight( h );
    }
    
    if (macro->hasOrigin())
    {
       
        int x = dbdist( macro->originX() );
        int y = dbdist( macro->originY() );
        _master->setOrigin(x,y);
    }

    if (macro->hasSiteName())
    {
        dbSite * site = _lib->findSite( macro->siteName() );

        if ( site == NULL )
            notice(0,"warning: macro %s references unkown site %s\n", macro->name(), macro->siteName() );
        else
            _master->setSite(site);
    }

    if ( macro->hasXSymmetry() )
        _master->setSymmetryX();

    if ( macro->hasYSymmetry() )
        _master->setSymmetryY();

    if ( macro->has90Symmetry() )
        _master->setSymmetryR90();
}

void lefin::macroEnd( const char * macroName )
{
    if ( _master )
    {

        if ( _master_modified )
        {
            int x, y;
            _master->getOrigin(x,y);
    
            if ( x != 0 || y != 0 )
            {
                dbTransform t( adsPoint(x,y) );
                _master->transform(t);
            }
        }
        
        _master->setFrozen();
        _master = NULL;
        _master_cnt++;
    }
}

void lefin::manufacturing( double num )
{
  _tech->setManufacturingGrid(dbdist(num));
}

void lefin::maxStackVia( lefiMaxStackVia * maxStack )
{
}

void lefin::minFeature( lefiMinFeature * min )
{
}

void lefin::nonDefault( lefiNonDefault * rule )
{
    if ( ! _create_tech )
        return;
    
    dbTechNonDefaultRule * dbrule = dbTechNonDefaultRule::create( _tech, rule->name() );

    if ( dbrule == NULL )
    {
        notice(0,"duplicate NON DEFAULT RULE (%s)\n", rule->name() );
        return;
    }

    int i;

    for( i = 0; i < rule->numLayers(); ++i )
    {
        dbTechLayer * dblayer = _tech->findLayer( rule->layerName(i) );

        if ( dblayer == NULL )
        {
            notice(0,"Invalid layer name %s in NON DEFAULT RULE %s\n", rule->layerName(i), rule->name() );
            continue;
        }
        
        dbTechLayerRule * lr = dbTechLayerRule::create(dbrule, dblayer);

        if ( rule->hasLayerWidth(i) )
            lr->setWidth( dbdist(rule->layerWidth(i)) );

        if ( rule->hasLayerSpacing(i) )
            lr->setSpacing( dbdist(rule->layerSpacing(i)) );

        if ( rule->hasLayerWireExtension(i) )
            lr->setWireExtension( dbdist(rule->layerWireExtension(i)) );

        if ( rule->hasLayerResistance(i) )
            lr->setResistance( rule->layerResistance(i) );

        if ( rule->hasLayerCapacitance(i) )
            lr->setCapacitance( rule->layerCapacitance(i) );

        if ( rule->hasLayerEdgeCap(i) )
            lr->setEdgeCapacitance( rule->layerEdgeCap(i) );
    }

    for( i = 0; i < rule->numVias(); ++i )
    {
        via( rule->viaRule(i), dbrule );
    }

    for( i = 0; i < rule->numSpacingRules(); ++i )
    {
        lefiSpacing * spacing = rule->spacingRule(i);
        dbTechLayer * l1 = _tech->findLayer( spacing->name1() );
        dbTechLayer * l2 = _tech->findLayer( spacing->name2() );
        dbTechSameNetRule * srule = dbTechSameNetRule::create( dbrule, l1, l2 );

        if ( spacing->hasStack() )
            srule->setAllowStackedVias(true);

        srule->setSpacing( dbdist(spacing->distance()) );
    }

    // 5.6 additions
    if ( rule->hasHardspacing() )
        dbrule->setHardSpacing(true);

    for( i = 0; i < rule->numUseVia(); ++i )
    {
        const char * vname = rule->viaName(i);
        dbTechVia * via = _tech->findVia(vname);

        if ( via == NULL )
        {
            notice(0,"error: undefined VIA %s\n", vname );
            ++_errors;
            continue;
        }

        dbrule->addUseVia(via);
    }

    for( i = 0; i < rule->numUseViaRule(); ++i )
    {
        const char * rname = rule->viaRuleName(i);
        dbTechViaGenerateRule * genrule = _tech->findViaGenerateRule(rname);

        if ( genrule == NULL )
        {
            notice(0,"error: undefined VIA GENERATE RULE %s\n", rname );
            ++_errors;
            continue;
        }

        dbrule->addUseViaRule(genrule);
    }

    for( i = 0; i < rule->numMinCuts(); ++i )
    {
        const char * lname = rule->cutLayerName(i);
        dbTechLayer * layer = _tech->findLayer(lname);

        if ( layer == NULL )
        {
            notice(0,"error: undefined LAYER %s\n", lname );
            ++_errors;
            continue;
        }

        dbrule->setMinCuts(layer, rule->numCuts(i));
    }
}

void lefin::obstruction( lefiObstruction * obs )
{
    if ( (_master == NULL) || (_skip_obstructions == true) )
        return;

    lefiGeometries * geometries = obs->geometries();

    if ( geometries->numItems() )
    {
        addGeoms(_master, false, geometries);
        dbSet<dbBox> obstructions = _master->getObstructions();

        // Reverse the stored order, too match the created order.
        if ( obstructions.reversible() && obstructions.orderReversed() )
            obstructions.reverse();
    }
}

void lefin::pin( lefiPin * pin )
{
    if (_master == NULL)
       return;

    dbIoType io_type;
    
    if ( pin->hasDirection() )
    {
        if( streq( pin->direction(), "OUTPUT TRISTATE" ) )
            io_type = dbIoType( dbIoType::OUTPUT );
        else
            io_type = dbIoType( pin->direction() );
    }

    dbSigType sig_type;
    
    if (pin->lefiPin::hasUse())
        sig_type = dbSigType( pin->use() );
    
    dbMTerm * term = _master->findMTerm( pin->name() );

    if ( term == NULL )
    {
        if ( _master->isFrozen() )
        {
            dbString n = _master->getName();
            notice(0,"Cannot add a new PIN (%s) to MACRO (%s), because the pins have already been defined. \n",
                   pin->name(), n.c_str() );
            return;
        }
        
        term = dbMTerm::create( _master, pin->name(), io_type, sig_type );
    }
    
    //
    // Parse and install antenna info
    //
    int i;
    dbTechLayer *tply;

    if (pin->lefiPin::hasAntennaPartialMetalArea())
      for (i = 0; i < pin->lefiPin::numAntennaPartialMetalArea(); i++)
	{
	  tply = NULL;
	  if (pin->lefiPin::antennaPartialMetalAreaLayer(i))
	    {
	      tply = _tech->findLayer(pin->lefiPin::antennaPartialMetalAreaLayer(i));
	      if (!tply)
		notice(0,"Invalid layer name %s in antenna info for term %s\n", pin->lefiPin::antennaPartialMetalAreaLayer(i), term->getName().c_str());
	    }

	  term->addPartialMetalAreaEntry( pin->lefiPin::antennaPartialMetalArea(i), tply);
	}

    if (pin->lefiPin::hasAntennaPartialMetalSideArea())
      for (i = 0; i < pin->lefiPin::numAntennaPartialMetalSideArea(); i++)
	{
	  tply = NULL;
	  if (pin->lefiPin::antennaPartialMetalSideAreaLayer(i))
	    {
	      tply = _tech->findLayer(pin->lefiPin::antennaPartialMetalSideAreaLayer(i));
	      if (!tply)
		notice(0,"Invalid layer name %s in antenna info for term %s\n", pin->lefiPin::antennaPartialMetalSideAreaLayer(i), term->getName().c_str());
	    }

	  term->addPartialMetalSideAreaEntry( pin->lefiPin::antennaPartialMetalSideArea(i), tply);
	}

    if (pin->lefiPin::hasAntennaPartialCutArea())
      for (i = 0; i < pin->lefiPin::numAntennaPartialCutArea(); i++)
	{
	  tply = NULL;
	  if (pin->lefiPin::antennaPartialCutAreaLayer(i))
	    {
	      tply = _tech->findLayer(pin->lefiPin::antennaPartialCutAreaLayer(i));
	      if (!tply)
		notice(0,"Invalid layer name %s in antenna info for term %s\n", pin->lefiPin::antennaPartialCutAreaLayer(i), term->getName().c_str());
	    }

	  term->addPartialCutAreaEntry( pin->lefiPin::antennaPartialCutArea(i), tply);
	}

    if (pin->lefiPin::hasAntennaDiffArea())
      for (i = 0; i < pin->lefiPin::numAntennaDiffArea(); i++)
	{
	  tply = NULL;
	  if (pin->lefiPin::antennaDiffAreaLayer(i))
	    {
	      tply = _tech->findLayer(pin->lefiPin::antennaDiffAreaLayer(i));
	      if (!tply)
		notice(0,"Invalid layer name %s in antenna info for term %s\n", pin->lefiPin::antennaDiffAreaLayer(i), term->getName().c_str());
	    }

	  term->addDiffAreaEntry( pin->lefiPin::antennaDiffArea(i), tply);
	}

    int j;
    dbTechAntennaPinModel *curmodel;
    lefiPinAntennaModel   *curlefmodel;
    if (pin->lefiPin::numAntennaModel() > 0)
      {
	// NOTE: Only two different oxides supported for now!
	for (i = 0; (i < pin->lefiPin::numAntennaModel()) && (i < 2); i++)
	  {
	    curmodel = (i == 1) ? term->createOxide2AntennaModel() : term->createDefaultAntennaModel();
	    curlefmodel = pin->lefiPin::antennaModel(i);

	    if (curlefmodel->hasAntennaGateArea())
	      {
		for (j = 0; j < curlefmodel->numAntennaGateArea(); j++)
		  {
		    tply = NULL;
		    if (curlefmodel->antennaGateAreaLayer(j))
		      {
			tply = _tech->findLayer(curlefmodel->antennaGateAreaLayer(j));
			if (!tply)
			  notice(0,"Invalid layer name %s in antenna info for term %s\n", curlefmodel->antennaGateAreaLayer(j), term->getName().c_str());
		      }
		    curmodel->addGateAreaEntry( curlefmodel->antennaGateArea(j), tply);
		  }
	      }

	    if (curlefmodel->hasAntennaMaxAreaCar())
	      {
		for (j = 0; j < curlefmodel->numAntennaMaxAreaCar(); j++)
		  {
		    tply = NULL;
		    if (curlefmodel->antennaMaxAreaCarLayer(j))
		      {
			tply = _tech->findLayer(curlefmodel->antennaMaxAreaCarLayer(j));
			if (!tply)
			  notice(0,"Invalid layer name %s in antenna info for term %s\n", curlefmodel->antennaMaxAreaCarLayer(j), term->getName().c_str());
		      }
		    curmodel->addMaxAreaCAREntry( curlefmodel->antennaMaxAreaCar(j), tply);
		  }
	      }

	    if (curlefmodel->hasAntennaMaxSideAreaCar())
	      {
		for (j = 0; j < curlefmodel->numAntennaMaxSideAreaCar(); j++)
		  {
		    tply = NULL;
		    if (curlefmodel->antennaMaxSideAreaCarLayer(j))
		      {
			tply = _tech->findLayer(curlefmodel->antennaMaxSideAreaCarLayer(j));
			if (!tply)
			  notice(0,"Invalid layer name %s in antenna info for term %s\n", curlefmodel->antennaMaxSideAreaCarLayer(j), term->getName().c_str());
		      }
		    curmodel->addMaxSideAreaCAREntry( curlefmodel->antennaMaxSideAreaCar(j), tply);
		  }
	      }

	    if (curlefmodel->hasAntennaMaxCutCar())
	      {
		for (j = 0; j < curlefmodel->numAntennaMaxCutCar(); j++)
		  {
		    tply = NULL;
		    if (curlefmodel->antennaMaxCutCarLayer(j))
		      {
			tply = _tech->findLayer(curlefmodel->antennaMaxCutCarLayer(j));
			if (!tply)
			  notice(0,"Invalid layer name %s in antenna info for term %s\n", curlefmodel->antennaMaxCutCarLayer(j), term->getName().c_str());
		      }
		    curmodel->addMaxCutCAREntry( curlefmodel->antennaMaxCutCar(j), tply);
		  }
	      }

	  }
      }

    bool created_mpins = false;
    int numPorts = pin->lefiPin::numPorts();
    for (i = 0; i < numPorts; i++)
    {
        lefiGeometries * geometries = pin->lefiPin::port(i);
        if ( geometries->numItems() )
        {
            dbMPin * dbpin = dbMPin::create(term);
            created_mpins = true;
            addGeoms(dbpin, true, geometries);

            dbSet<dbBox> geoms = dbpin->getGeometry();
            if ( geoms.reversible() && geoms.orderReversed() )
                geoms.reverse();
        }
    }

    if ( created_mpins ) // created pins
    {
        dbSet<dbMPin> pins = term->getMPins();
        if ( pins.reversible() && pins.orderReversed() )
            pins.reverse();
    }
}

void lefin::propDefBegin( void* ptr )
{
}

void lefin::propDef( lefiProp * prop )
{
}

void lefin::propDefEnd( void * ptr )
{
}

void lefin::site( lefiSite * lefsite )
{
    if ( ! _create_lib )
        return;
    
    if ( _lib == NULL )
        createLibrary();
    
    dbSite * site = _lib->findSite( lefsite->name() );

    if ( site )
        return;

    site = dbSite::create( _lib, lefsite->name() );

    if ( lefsite->hasSize() )
    {
        site->setWidth( dbdist( lefsite->sizeX() ) );
        site->setHeight( dbdist( lefsite->sizeY() ) );
    }

    if ( lefsite->hasXSymmetry() )
        site->setSymmetryX();

    if ( lefsite->hasYSymmetry() )
        site->setSymmetryY();

    if ( lefsite->has90Symmetry() )
        site->setSymmetryR90();

    if ( lefsite->hasClass() )
        site->setClass( dbSiteClass( lefsite->siteClass() ) );
}

void lefin::spacingBegin( void * ptr )
{
}

void lefin::spacing( lefiSpacing * spacing )
{
    if ( _create_tech == false )
        return;

    dbTechLayer * l1 = _tech->findLayer( spacing->name1() );
    dbTechLayer * l2 = _tech->findLayer( spacing->name2() );
    dbTechSameNetRule * rule = dbTechSameNetRule::create( l1, l2 );

    if ( rule == NULL )
        return;

    if ( spacing->hasStack() )
        rule->setAllowStackedVias(true);

    rule->setSpacing( dbdist(spacing->distance()) );
}

void lefin::spacingEnd( void * ptr )
{
}

void lefin::timing( lefiTiming * timing )
{
}

void lefin::units( lefiUnits * unit )
{
    if ( unit->hasDatabase() )
    {
        _lef_units = (int) unit->databaseNumber();
        
        if ( _override_lef_dbu == false )
        {
            if ( _create_tech )
            {
                if ( _lef_units < 1000 ) // historically the database was always stored in nm
                    setDBUPerMicron(1000);
                else
                    setDBUPerMicron(_lef_units);
            
                _tech->setDbUnitsPerMicron(_dbu_per_micron);
                _tech->setLefUnits(_lef_units);
            }
        }

        if ( _lef_units > _dbu_per_micron )
        {
            ++_errors;
            notice(0,"The LEF UNITS DATABASE MICRON convert factor (%d) is greater than the database units per micron (%d) of the current technology.\n",
                   _lef_units, _dbu_per_micron );
        }
    }
}

void lefin::setDBUPerMicron( int dbu )
{
    switch( dbu )
    {
        case 100:
        {
            _dist_factor = 100.0;
            _area_factor = 10000.0;
            _dbu_per_micron = 100;
            break;
        }

        case 200:
        {
            _dist_factor = 200.0;
            _area_factor = 40000.0;
            _dbu_per_micron = 200;
            break;
        }

        case 1000:
        {
            _dist_factor = 1000.0;
            _area_factor = 1000000.0;
            _dbu_per_micron = 1000;
            break;
        }
        
        case 2000:
        {
            _dist_factor = 2000.0;
            _area_factor = 4000000.0;
            _dbu_per_micron = 2000;
            break;
        }
        
        case 10000:
        {
            _dist_factor = 10000.0;
            _area_factor = 100000000.0;
            _dbu_per_micron = 10000;
            break;
        }
        
        case 20000:
        {
            _dist_factor = 20000.0;
            _area_factor = 400000000.0;
            _dbu_per_micron = 20000;
            break;
        }
        
        default:
        {
            ++_errors;
            notice(0,"error: invalid dbu-per-micron value %d; valid units (100, 200, 1000, 2000, 10000, 20000)\n", _lef_units );
        }
    }
}

#ifdef _WIN32
#define strncasecmp strncmp
#endif

void lefin::useMinSpacing( lefiUseMinSpacing * spacing )
{
  if (! strncasecmp(spacing->name(),"PIN",3))
    {
      _tech->setUseMinSpacingPin(dbOnOffType(spacing->value()));
    }
  else if (! strncasecmp(spacing->name(),"OBS",3))
    {
      _tech->setUseMinSpacingObs(dbOnOffType(spacing->value()));
    }
  else
    {
      fprintf(stderr,"Unknown object type for USEMINSPACING: %s\n",spacing->name());
    }
}

void lefin::version( double num )
{
   _tech->setLefVersion(num);
}

void lefin::via( lefiVia * via, dbTechNonDefaultRule * rule )
{
    if ( ! _create_tech )
        return;
    
    if ( _tech->findVia( via->name() ) )
    {
        notice(0,"VIA: duplicate VIA (%s) ignored...\n", via->name() );
        return;
    }

    dbTechVia * v;
    
    if ( rule )
        v = dbTechVia::create( rule, via->name() );
    else
        v = dbTechVia::create( _tech, via->name() );
    
    if ( via->hasDefault() )
        v->setDefault();

    if ( via->hasTopOfStack() )
        v->setTopOfStack();
    
    if ( via->hasResistance() )
        v->setResistance( via->resistance() );

    if ( via->numLayers() > 0 )
    {
        int i;
        int j;
        
        for (i = 0; i < via->lefiVia::numLayers(); i++)
        {

            dbTechLayer * l = _tech->findLayer( via->layerName(i) );

            if ( l == NULL )
            {
                notice(0,"VIA: undefined layer (%s) in VIA (%s)\n",
                       via->layerName(i), via->name() );

                continue;
            }
            
            for (j = 0; j < via->numRects(i); j++)
            {
                int xlo = dbdist(via->xl(i,j));
                int xhi = dbdist(via->xh(i,j));
                int ylo = dbdist(via->yl(i,j));
                int yhi = dbdist(via->yh(i,j));
                dbBox::create( v, l, xlo, ylo, xhi, yhi );
            }
        }

        dbSet<dbBox> boxes = v->getBoxes();
        // Reverse the stored order, too match the created order.
        if ( boxes.reversible() && boxes.orderReversed() )
            boxes.reverse();
     }

    // 5.6 VIA RULE
    if ( via->hasViaRule() )
    {
        dbTechViaGenerateRule * gen_rule = _tech->findViaGenerateRule( via->viaRuleName() );

        if ( rule == NULL )
        {
            notice(0,"error: missing VIA GENERATE rule %s\n", via->viaRuleName() );
            ++_errors;
            return;
        }

        v->setViaGenerateRule(gen_rule);
        dbViaParams P;
        P.setXCutSize( dbdist(via->xCutSize()) );
        P.setYCutSize( dbdist(via->yCutSize()) );

        dbTechLayer * bot = _tech->findLayer( via->botMetalLayer() );

        if ( bot == NULL )
        {
            notice(0,"error: missing LAYER %s\n", via->botMetalLayer() );
            ++_errors;
            return;
        }

        dbTechLayer * cut = _tech->findLayer( via->cutLayer() );

        if ( cut == NULL )
        {
            notice(0,"error: missing LAYER %s\n", via->cutLayer() );
            ++_errors;
            return;
        }

        dbTechLayer * top = _tech->findLayer( via->topMetalLayer() );

        if ( top == NULL )
        {
            notice(0,"error: missing LAYER %s\n", via->topMetalLayer() );
            ++_errors;
            return;
        }

        P.setTopLayer(top);
        P.setCutLayer(cut);
        P.setBottomLayer(bot);
        P.setXCutSpacing( dbdist(via->xCutSpacing()) );
        P.setYCutSpacing( dbdist(via->yCutSpacing()) );
        P.setXBottomEnclosure( dbdist(via->xBotEnc()) );
        P.setYBottomEnclosure( dbdist(via->yBotEnc()) );
        P.setXTopEnclosure( dbdist(via->xTopEnc()) );
        P.setYTopEnclosure( dbdist(via->yTopEnc()) );

        if ( via->hasRowCol() )
        {
            P.setNumCutRows( via->numCutRows() );
            P.setNumCutCols( via->numCutCols() );
        }

        if ( via->hasOrigin() )
        {
            P.setXOrigin( dbdist(via->xOffset()) );
            P.setYOrigin( dbdist(via->yOffset()) );
        }

        if ( via->hasOffset() )
        {
            P.setXBottomEnclosure( dbdist(via->xBotOffset()) );
            P.setYBottomEnclosure( dbdist(via->yBotOffset()) );
            P.setXTopEnclosure( dbdist(via->xTopOffset()) );
            P.setYTopEnclosure( dbdist(via->yTopOffset()) );
        }

        if ( via->hasCutPattern() )
            v->setPattern( via->cutPattern() );
    }

     _via_cnt++;
}

void lefin::viaRule( lefiViaRule * viaRule )
{
    if ( viaRule->hasGenerate() )
    {
        viaGenerateRule(viaRule);
        return;
    }

    const char * name = viaRule->name();

    dbTechViaRule * rule = dbTechViaRule::create(_tech, name );
    
    if ( rule == NULL )
    {
        notice(0,"warning: duplicate VIARULE (%s) ignoring...", name );
        return;
    }

    int idx;
    for( idx = 0; idx < viaRule->numLayers(); ++idx )
    {
        lefiViaRuleLayer * leflay = viaRule->layer(idx);
        dbTechLayer * layer = _tech->findLayer(leflay->name());

        if ( layer == NULL )
        {
            notice(0,"error: VIARULE (%s) undefined layer %s\n", name, leflay->name());
            ++_errors;
            return;
        }

        dbTechViaLayerRule * layrule = dbTechViaLayerRule::create( _tech,  rule, layer );

        if ( viaRule->layer(idx)->hasDirection() )
        {
            if ( viaRule->layer(idx)->isVertical() )
                layrule->setDirection( dbTechLayerDir::VERTICAL );
            else if ( viaRule->layer(idx)->isHorizontal() )
                layrule->setDirection( dbTechLayerDir::HORIZONTAL );
        }

        if ( viaRule->layer(idx)->hasWidth() )
        {
            int minW = dbdist( viaRule->layer(idx)->widthMin() );
            int maxW = dbdist( viaRule->layer(idx)->widthMax() );
            layrule->setWidth(minW, maxW);
        }
    }

    for( idx = 0; idx < viaRule->numVias(); ++idx )
    {
        dbTechVia * via = _tech->findVia(viaRule->viaName(idx));

        if ( via == NULL )
        {
            notice(0,"error: undefined VIA %s in VIARULE %s\n", viaRule->viaName(idx), name );
            ++_errors;
        }
        else
        {
            rule->addVia(via);
        }
    }
}

void lefin::viaGenerateRule( lefiViaRule * viaRule )
{
    const char * name = viaRule->name();
    dbTechViaGenerateRule * rule = dbTechViaGenerateRule::create(_tech, name, viaRule->hasDefault() );

    if ( rule == NULL )
    {
        notice(0,"warning: duplicate VIARULE (%s) ignoring...", name );
        return;
    }

    int idx;
    for( idx = 0; idx < viaRule->numLayers(); ++idx )
    {
        lefiViaRuleLayer * leflay = viaRule->layer(idx);
        dbTechLayer * layer = _tech->findLayer(leflay->name());

        if ( layer == NULL )
        {
            notice(0,"error: VIARULE (%s) undefined layer %s\n", name, leflay->name());
            ++_errors;
            return;
        }

        dbTechViaLayerRule * layrule = dbTechViaLayerRule::create( _tech,  rule, layer );

        if ( viaRule->layer(idx)->hasDirection() )
        {
            if ( viaRule->layer(idx)->isVertical() )
                layrule->setDirection( dbTechLayerDir::VERTICAL );
            else if ( viaRule->layer(idx)->isHorizontal() )
                layrule->setDirection( dbTechLayerDir::HORIZONTAL );
        }

        if ( viaRule->layer(idx)->hasEnclosure() )
        {
            int overhang1 = dbdist( viaRule->layer(idx)->enclosureOverhang1() );
            int overhang2 = dbdist( viaRule->layer(idx)->enclosureOverhang2() );
            layrule->setEnclosure(overhang1, overhang2);
        }

        if ( viaRule->layer(idx)->hasWidth() )
        {
            int minW = dbdist( viaRule->layer(idx)->widthMin() );
            int maxW = dbdist( viaRule->layer(idx)->widthMax() );
            layrule->setWidth(minW, maxW);
        }

        if ( viaRule->layer(idx)->hasOverhang() )
        {
            int overhang = dbdist( viaRule->layer(idx)->overhang() );
            layrule->setOverhang(overhang);
        }

        if ( viaRule->layer(idx)->hasMetalOverhang() )
        {
            int overhang = dbdist( viaRule->layer(idx)->metalOverhang() );
            layrule->setMetalOverhang(overhang);
        }
    
        if ( viaRule->layer(idx)->hasRect() )
        {
            int xMin = dbdist( viaRule->layer(idx)->xl() );
            int yMin = dbdist( viaRule->layer(idx)->yl() );
            int xMax = dbdist( viaRule->layer(idx)->xh() );
            int yMax = dbdist( viaRule->layer(idx)->yh() );
            adsRect r(xMin, yMin, xMax, yMax);
            layrule->setRect(r);
        }

        if ( viaRule->layer(idx)->hasSpacing() )
        {
            int x_spacing = dbdist( viaRule->layer(idx)->spacingStepX() );
            int y_spacing = dbdist( viaRule->layer(idx)->spacingStepY() );
            layrule->setSpacing(x_spacing,y_spacing);
        }

        if ( viaRule->layer(idx)->hasResistance() )
        {
            layrule->setResistance(viaRule->layer(idx)->resistance() );
        }
    }
}

void lefin::done( void * ptr )
{
}
 
void lefin::error(const char* msg)
{
    notice(0,"Error: %s\n", msg);
    ++_errors;
}

void lefin::warning(const char* msg)
{
    notice(0,"Warning: %s\n", msg);
}

void lefin::lineNumber(int lineNo)
{
    notice(0,"%d lines parsed!\n", lineNo);
}

bool lefin::readLef( const char * lef_file )
{
    notice(0,"Reading LEF file:  %s\n", lef_file );

    bool r =  lefin_parse( this, lef_file );

    if ( _layer_cnt )
        notice(0,"    Created %d technology layers\n", _layer_cnt);

    if ( _via_cnt )
        notice(0,"    Created %d technology vias\n", _via_cnt);

    if ( _master_cnt )
        notice(0,"    Created %d library cells\n", _master_cnt);
    
    notice(0,"Finished LEF file:  %s\n", lef_file );

    return r;
    
}

dbTech * lefin::createTech( const char * lef_file )
{
    lefrRelaxMode = true;
    init();
    
    if ( _db->getTech() )
    {
        notice(0,"Error: technology already exists\n" );
        return NULL;
    };

    _tech = dbTech::create( _db, _dbu_per_micron );
    _create_tech = true;

    if ( ! readLef(lef_file) )
    {
        dbTech::destroy(_tech);
        return NULL;
    }

    if ( _errors != 0 )
    {
        dbTech::destroy(_tech);
        return NULL;
    }

    return _tech;
}

dbLib * lefin::createLib( const char * name, const char * lef_file )
{
    lefrRelaxMode = true;
    init();
    
    _tech = _db->getTech();

    if ( _tech == NULL )
    {
        notice(0,"Error: technolgy does not exists\n");
        return NULL;
    }
    
    if ( _db->findLib(name) )
    {
        notice(0,"Error: library (%s) already exists\n", name );
        return NULL;
    };

    setDBUPerMicron( _tech->getDbUnitsPerMicron() );
    _lib_name = name;
    _create_lib = true;

    if ( ! readLef(lef_file) )
    {
        if ( _lib )
            dbLib::destroy(_lib);
        return NULL;
    }

    if ( _errors != 0 )
    {
        if ( _lib )
            dbLib::destroy(_lib);
        return NULL;
    }
    
    return _lib;
}

dbLib * lefin::createTechAndLib( const char * lib_name, const char * lef_file )
{
    lefrRelaxMode = true;
    init();
    
    if ( _db->findLib(lib_name) )
    {
        notice(0,"Error: library (%s) already exists\n", lib_name );
        return NULL;
    };
    
    if ( _db->getTech() )
    {
        notice(0,"Error: technology already exists\n" );
        ++_errors;
        return NULL;
    };

    _tech = dbTech::create( _db, _dbu_per_micron);
    _lib_name = lib_name;
    _create_lib = true;
    _create_tech = true;

    if ( ! readLef(lef_file) )
    {
        if ( _lib )
            dbLib::destroy(_lib);
        dbTech::destroy(_tech);
        return NULL;
    }

    if ( _errors != 0 )
    {
        if ( _lib )
            dbLib::destroy(_lib);
        dbTech::destroy(_tech);
        return NULL;
    }

    dbSet<dbTechNonDefaultRule> rules = _tech->getNonDefaultRules();

    if ( rules.orderReversed() )
        rules.reverse();
    
    return _lib;
}

dbLib * lefin::createTechAndLib( const char *lib_name, std::list<std::string> &file_list )
{
    lefrRelaxMode = true;
    init();
    
    if ( _db->findLib(lib_name) )
    {
        notice(0,"Error: library (%s) already exists\n", lib_name );
        return NULL;
    };
    
    if ( _db->getTech() )
    {
        notice(0,"Error: technology already exists\n" );
        ++_errors;
        return NULL;
    };

    _tech = dbTech::create( _db, _dbu_per_micron );
    assert(_tech);
    
    _lib_name = lib_name;
    _create_lib = true;
    _create_tech = true;

    std::list<std::string>::iterator it;
    for (it = file_list.begin(); it != file_list.end(); ++it ) {
      std::string str = *it;
      const char *lef_file = str.c_str(); 
      notice(0,"Reading LEF file:  %s ...\n", lef_file );
      if (! lefin_parse( this, lef_file )) {
        notice(0,"Error reading %s\n",lef_file);

        if ( _lib )
            dbLib::destroy(_lib);
        dbTech::destroy(_tech);
        return NULL;
      }
      notice(0,"Finished LEF file:  %s\n", lef_file );
    }
                                                                                
    if ( _layer_cnt )
        notice(0,"    Created %d technology layers\n", _layer_cnt);
                                                                                
    if ( _via_cnt )
        notice(0,"    Created %d technology vias\n", _via_cnt);
                                                                                
    if ( _master_cnt )
        notice(0,"    Created %d library cells\n", _master_cnt);
                                                                                
    if ( _errors != 0 )
    {
        if ( _lib )
            dbLib::destroy(_lib);
        dbTech::destroy(_tech);
        return NULL;
    }

    dbSet<dbTechNonDefaultRule> rules = _tech->getNonDefaultRules();

    if ( rules.orderReversed() )
        rules.reverse();
    
    return _lib;
}

bool lefin::updateLib( dbLib * lib,
                       const char * lef_file )
{
    lefrRelaxMode = true;
    init();
    _tech = lib->getTech();
    _lib = lib;
    _create_lib = true;
    setDBUPerMicron( _tech->getDbUnitsPerMicron() );

    if ( ! readLef(lef_file) )
        return false;
    
    return _errors == 0;
}

//
// TODO: Recover gracefully from any update errors
//
bool lefin::updateTechAndLib( dbLib * lib,
                              const char * lef_file )
{
    lefrRelaxMode = true;
    
    init();
    _lib = lib;
    _tech = lib->getTech();
    _create_lib = true;
    _create_tech = true;
    dbu_per_micron( _tech->getDbUnitsPerMicron() ); // set override-flag, because the tech is being updated.

    if ( ! readLef(lef_file) )
        return false;

    return _errors == 0;
}

bool lefin::updateTech( dbTech * tech,
                        const char * lef_file )
{
    lefrRelaxMode = true;
    init();
    _tech = tech;
    _create_tech = true;
    dbu_per_micron( _tech->getDbUnitsPerMicron() ); // set override-flag, because the tech is being updated.

    if ( ! readLef(lef_file) )
        return false;
    
    return _errors == 0;
}

} // namespace
