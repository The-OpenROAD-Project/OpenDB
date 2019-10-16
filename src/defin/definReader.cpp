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

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include "def.h"
#include "definReader.h"
#include "definBlockage.h"
#include "definComponent.h"
#include "definFill.h"
#include "definGCell.h"
#include "definNet.h"
#include "definPin.h"
#include "definRow.h"
#include "definSNet.h"
#include "definTracks.h"
#include "definVia.h"
#include "definRegion.h"
#include "definNonDefaultRule.h"
#include "definPropDefs.h"
#include "definPinProps.h"
#include "db.h"
#include "dbShape.h"

namespace odb {

class DefHeader
{
public:
    const char * _version;
    char         _hier_delimeter;
    char         _left_bus_delimeter;
    char         _right_bus_delimeter;
    const char * _design;

    DefHeader()
    {
        _version = NULL;
        _hier_delimeter = 0;
        _left_bus_delimeter = 0;
        _right_bus_delimeter = 0;
        _design = NULL;
    }
    
    ~DefHeader()
    {
        if ( _version )
            free( (void *) _version );

        if ( _design )
            free( (void *) _design );
    }

    static DefHeader * getDefHeader( const char * file );
};

definReader::definReader(dbDatabase * db)
{
    _db = db;
    _block_name = NULL;

    _blockageR = new definBlockage;
    _componentR = new definComponent;
    _fillR = new definFill;
    _gcellR = new definGCell;
    _netR = new definNet;
    _pinR = new definPin;
    _rowR = new definRow;
    _snetR = new definSNet;
    _tracksR = new definTracks;
    _viaR = new definVia;
    _regionR = new definRegion;
    _non_default_ruleR = new definNonDefaultRule;
    _prop_defsR = new definPropDefs;
    _pin_propsR = new definPinProps;

    _interfaces.push_back( _blockageR );
    _interfaces.push_back( _componentR );
    _interfaces.push_back( _fillR );
    _interfaces.push_back( _gcellR );
    _interfaces.push_back( _netR );
    _interfaces.push_back( _pinR );
    _interfaces.push_back( _rowR );
    _interfaces.push_back( _snetR );
    _interfaces.push_back( _tracksR );
    _interfaces.push_back( _viaR );
    _interfaces.push_back( _regionR );
    _interfaces.push_back( _non_default_ruleR );
    _interfaces.push_back( _prop_defsR );
    _interfaces.push_back( _pin_propsR );
    init();
}

definReader::~definReader()
{
    delete _blockageR;
    delete _componentR;
    delete _fillR;
    delete _gcellR;
    delete _netR;
    delete _pinR;
    delete _rowR;
    delete _snetR;
    delete _tracksR;
    delete _viaR;
    delete _regionR;
    delete _non_default_ruleR;
    delete _prop_defsR;
    delete _pin_propsR;

    if ( _block_name )
        free( (void *) _block_name );
}

int definReader::errors()
{
    int e = _errors;

    std::vector<definBase *>::iterator itr;
    for( itr = _interfaces.begin(); itr != _interfaces.end(); ++itr )
        e += (*itr)->_errors;
    
    return e;
}

void definReader::skipWires()
{
    _netR->skipWires();
}
void definReader::skipConnections()
{
    _netR->skipConnections();
}
void definReader::skipSpecialWires()
{ 
    _snetR->skipSpecialWires();
}

void definReader::skipShields()
{ 
    _snetR->skipShields();
}

void definReader::skipBlockWires()
{ 
    _snetR->skipBlockWires();
}

void definReader::skipFillWires()
{
    _snetR->skipFillWires();
}

void definReader::replaceWires()
{
    _netR->replaceWires();
    _snetR->replaceWires();
}

void definReader::namesAreDBIDs()
{
    _netR->namesAreDBIDs();
    _snetR->namesAreDBIDs();
}

void definReader::setAssemblyMode()
{
    _netR->setAssemblyMode();
}

void definReader::useBlockName( const char * name )
{
    if ( _block_name )
        free( (void *) _block_name );
    
    _block_name = strdup(name);
    assert(_block_name);
}

void definReader::init()
{
    std::vector<definBase *>::iterator itr;
    for( itr = _interfaces.begin(); itr != _interfaces.end(); ++itr )
        (*itr)->init();
    _update = false;
}

void definReader::setTech( dbTech * tech )
{
    definBase::setTech(tech);

    std::vector<definBase *>::iterator itr;
    for( itr = _interfaces.begin(); itr != _interfaces.end(); ++itr )
        (*itr)->setTech(tech);
}

void definReader::setBlock( dbBlock * block )
{
    definBase::setBlock(block);
    std::vector<definBase *>::iterator itr;
    for( itr = _interfaces.begin(); itr != _interfaces.end(); ++itr )
        (*itr)->setBlock(block);
}

void definReader::units( int d )
{
    // Truncation error
    if ( d > _tech->getDbUnitsPerMicron() )
    {
        notice(0, "error: The DEF UNITS DISTANCE MICRONS convert factor (%d) is greater than the database units per micron (%d) value.\n",
                d, _tech->getDbUnitsPerMicron() );
        ++_errors;

        d = _tech->getDbUnitsPerMicron();
    }

    definBase::units(d);

    std::vector<definBase *>::iterator itr;
    for( itr = _interfaces.begin(); itr != _interfaces.end(); ++itr )
        (*itr)->units(d);

    if ( ! _update )
        _block->setDefUnits(d);
}

void definReader::dieArea( const std::vector<defPoint> & points )
{
    if ( ! _update )
    {
        std::vector<adsPoint> P;
        translate(points,P);

        if ( P.size() < 2 )
        {
            notice(0, "error: Invalid DIEAREA statement, missing point(s)\n");
            ++_errors;
            return;
        }
        
        if ( P.size() == 2 )
        {
            adsPoint p0 = P[0];
            adsPoint p1 = P[1];
            adsRect r(p0.getX(),p0.getY(),p1.getX(),p1.getY());
            _block->setDieArea(r);
        }
        else
        {
            notice(0, "warning: Polygon DIEAREA statement not supported..\n");
            int xmin = INT_MAX;
            int ymin = INT_MAX;
            int xmax = INT_MIN;
            int ymax = INT_MIN;
            std::vector<adsPoint>::iterator itr;

            for( itr = P.begin(); itr != P.end(); ++itr )
            {
                adsPoint & p = *itr;
                int x = p.getX();
                int y = p.getY();
                
                if ( x < xmin )
                    xmin = x;

                if ( y < ymin )
                    ymin = y;

                if ( x > xmax )
                    xmax = x;

                if ( y > ymax )
                    ymax = y;
            }

            adsRect r(xmin, ymin, xmax, ymax);
            _block->setDieArea(r);
        }
    }
}

void definReader::line( int line_num )
{
    notice(0,"lines processed: %d\n", line_num );
}

void definReader::error( const char * msg )
{
    notice(0, "error: %s\n", msg );
    ++_errors;
}

void definReader::setLibs( std::vector<dbLib *> & libs )
{
    _componentR->setLibs(libs);
    _rowR->setLibs(libs);
}

DefHeader * DefHeader::getDefHeader( const char * file )
{
    FILE * f = fopen(file, "r");

    if ( f == NULL )
    {
        fprintf( stderr, "Cannot open DEF file %s\n", file );
        return NULL;
    }
    
    int l;
    char line[8192];
    DefHeader * hdr = new DefHeader();

    for( l = 1; fgets( line, 8192, f ); ++l )
    {
        const char * token = strtok( line, " \t\n" );

        if ( token == NULL )
            continue;
        
        if ( (strcmp( token, "VERSION" ) == 0) || (strcmp( token, "version") == 0) )
        {
            const char * version = strtok(NULL, " \t\n");

            if ( version == NULL )
            {
                fprintf( stderr, "Error: Cannot read VERSION statment at line %d\n", l );
                delete hdr;
                fclose(f);
                return NULL;
            }
            hdr->_version = strdup(version);
            assert( hdr->_version );
            continue;
        }
        
        if ( (strcmp( token, "DESIGN" ) == 0) || (strcmp( token, "design") == 0) )
        {
            const char * design = strtok(NULL, " \t\n");

            if ( design == NULL )
            {
                fprintf( stderr, "Error: Cannot read DESIGN statment at line %d\n", l );
                delete hdr;
                fclose(f);
                return NULL;
            }

            hdr->_design = strdup(design);
            assert( hdr->_design );
            break;
        }

        if ( (strcmp( token, "DIVIDERCHAR" ) == 0) || (strcmp( token, "dividerchar") == 0) )
        {
            const char * divider = strtok(NULL, " \t\"");

            if ( divider == NULL )
            {
                fprintf( stderr, "Error: Cannot read DIVIDERCHAR statment at line %d\n", l );
                delete hdr;
                fclose(f);
                return NULL;
            }

            hdr->_hier_delimeter = divider[0];

            if ( hdr->_hier_delimeter == 0 )
            {
                fprintf( stderr, "Error: Syntax error in DIVIDERCHAR statment at line %d\n", l );
                delete hdr;
                fclose(f);
                return NULL;
            }
            continue;
        }

        if ( (strcmp( token, "BUSBITCHARS" ) == 0) || (strcmp( token, "busbitchars") == 0) )
        {
            const char * busbitchars = strtok(NULL, " \t\"");

            if ( busbitchars == NULL )
            {
                fprintf( stderr, "Error: Cannot read BUSBITCHARS statment at line %d\n", l );
                delete hdr;
                fclose(f);
                return NULL;
            }

            hdr->_left_bus_delimeter = busbitchars[0];
            hdr->_right_bus_delimeter = busbitchars[1];

            if ( (hdr->_left_bus_delimeter == 0) || (hdr->_right_bus_delimeter == 0) )
            {
                fprintf( stderr, "Error: Syntax error in BUSBITCHARS statment at line %d\n", l );
                delete hdr;
                fclose(f);
                return NULL;
            }

            continue;
        }

        if ( (strcmp( token, "COMPONENTS" ) == 0) || (strcmp( token, "components") == 0) )
        {
            fprintf( stderr, "Error: DESIGN statement is missing\n");
            delete hdr;
            fclose(f);
            return NULL;
        }
    }
    
    fclose(f);
    return hdr;
}

dbChip *
definReader::createChip( std::vector<dbLib *> & libs, const char * file )
{
    init();
    setLibs(libs);

    DefHeader * hdr = DefHeader::getDefHeader( file );

    if ( hdr == NULL )
        return NULL;

    if ( _db->getChip() )
    {
        fprintf( stderr, "Error: Chip already exists\n");
        return NULL;
    }

    dbChip * chip = dbChip::create( _db );
    assert(chip);

    if ( _block_name )
        _block = dbBlock::create( chip, _block_name, hdr->_hier_delimeter );
    else
        _block = dbBlock::create( chip, hdr->_design, hdr->_hier_delimeter );

    assert(_block);
    setBlock(_block);
    setTech(_db->getTech());

    _block->setBusDelimeters( hdr->_left_bus_delimeter, hdr->_right_bus_delimeter );
    
    notice(0,"\nReading DEF file: %s\n", file );
    notice(0,"Design: %s\n", hdr->_design);

    if ( ! createBlock( file ) )
    {
        delete hdr;
        dbChip::destroy(chip);
        notice(0,"Error: Failed to read DEF file\n");
        return NULL;
    }

    if ( _pinR->_bterm_cnt )
        notice(0,"    Created %d pins.\n", _pinR->_bterm_cnt);

    if ( _componentR->_inst_cnt )
        notice(0,"    Created %d components and %d component-terminals.\n", _componentR->_inst_cnt, _componentR->_iterm_cnt);

    if ( _snetR->_snet_cnt )
        notice(0,"    Created %d special nets and %d connections.\n", _snetR->_snet_cnt, _snetR->_snet_iterm_cnt);

    if ( _netR->_net_cnt )
        notice(0,"    Created %d nets and %d connections.\n", _netR->_net_cnt, _netR->_net_iterm_cnt);
    
    notice(0,"Finished DEF file: %s\n", file );
    delete hdr;
    return chip;
}

static std::string renameBlock( dbBlock * parent, const char * old_name )
{
    int cnt = 1;

    for( ;; ++cnt )
    {
        char n[16];
        snprintf( n, 15, "_%d", cnt );
        std::string name(old_name);
        name += n;

        if ( ! parent->findChild(name.c_str()) )
            return name;
    }
}
    
dbBlock *
definReader::createBlock( dbBlock * parent, std::vector<dbLib *> & libs, const char * def_file )
{
    init();
    setLibs(libs);

    DefHeader * hdr = DefHeader::getDefHeader( def_file );

    if ( hdr == NULL )
    {
        fprintf( stderr, "Header information missing from DEF file.\n");
        return NULL;
    }

    std::string block_name;

    if ( _block_name )
        block_name = _block_name;
    else
        block_name = hdr->_design;

    if ( parent->findChild(block_name.c_str()) )
    {
        std::string new_name = renameBlock(parent, block_name.c_str() );
        fprintf( stderr, "Warning: Block with name \"%s\" already exists, renaming too \"%s\".\n", block_name.c_str(), new_name.c_str() );
        block_name = new_name;
    }

    _block = dbBlock::create( parent, block_name.c_str(), hdr->_hier_delimeter );

    if ( _block == NULL )
    {
        fprintf( stderr, "Error: Failed to create Block with name \"%s\".\n", block_name.c_str() );
        delete hdr;
        return NULL;
    }
    

    setBlock(_block);
    setTech(_db->getTech());
    
    _block->setBusDelimeters( hdr->_left_bus_delimeter, hdr->_right_bus_delimeter );

    notice(0,"\nReading DEF file: %s\n", def_file );
    notice(0,"Design: %s\n", hdr->_design);
    
    if ( ! createBlock( def_file ) )
    {
        dbBlock::destroy(_block);
        notice(0,"Error: Failed to read DEF file\n");
        delete hdr;
        return NULL;
    }

    if ( _pinR->_bterm_cnt )
        notice(0,"    Created %d pins.\n", _pinR->_bterm_cnt);

    if ( _componentR->_inst_cnt )
        notice(0,"    Created %d components and %d component-terminals.\n", _componentR->_inst_cnt, _componentR->_iterm_cnt);

    if ( _snetR->_snet_cnt )
        notice(0,"    Created %d special nets and %d connections.\n", _snetR->_snet_cnt, _snetR->_snet_iterm_cnt);

    if ( _netR->_net_cnt )
        notice(0,"    Created %d nets and %d connections.\n", _netR->_net_cnt, _netR->_net_iterm_cnt);

    notice(0,"Finished DEF file: %s\n", def_file );

    delete hdr;
    return _block;
}

bool
definReader::replaceWires( dbBlock * block, const char * def_file )
{
    init();
    setBlock(block);
    setTech(_db->getTech());

    notice(0,"\nReading DEF file: %s\n", def_file );
    
    if ( ! replaceWires( def_file ) )
    {
            //dbBlock::destroy(_block);
        notice(0,"Error: Failed to read DEF file\n");
        return false;
    }

    if ( _snetR->_snet_cnt )
        notice(0,"    Processed %d special nets.\n", _snetR->_snet_cnt );

    if ( _netR->_net_cnt )
        notice(0,"    Processed %d nets.\n", _netR->_net_cnt );

    notice(0,"Finished DEF file: %s\n", def_file );
    return errors() == 0;
}

bool definReader::createBlock( const char * file )
{
    FILE * f = fopen(file, "r");

    if ( f == NULL )
    {
        notice(0,"error: Cannot open DEF file %s\n", file );
        return false;
    }

    defparse_init(f);
    defin_set_IBlockage( _blockageR );
    defin_set_IComponent( _componentR );
    defin_set_IFill( _fillR );
    defin_set_IGCell( _gcellR );
    defin_set_INet( _netR );
    defin_set_IPin( _pinR );
    defin_set_IReader( this );
    defin_set_IRow( _rowR );
    defin_set_ISNet( _snetR );
    defin_set_ITracks( _tracksR );
    defin_set_IVia( _viaR );
    defin_set_IRegion( _regionR );
    defin_set_INonDefaultRule( _non_default_ruleR );
    defin_set_IPropDefs( _prop_defsR );
    defin_set_IPinProps( _pin_propsR );
    defparse();
    defparse_done();
    return true;
    // 1220 return errors() == 0;
}
    
bool definReader::replaceWires( const char * file )
{
    FILE * f = fopen(file, "r");

    if ( f == NULL )
    {
        notice(0,"error: Cannot open DEF file %s\n", file );
        return false;
    }

    replaceWires();
    defparse_init(f);
    defin_set_IReader( this );
    defin_set_INet( _netR );
    defin_set_ISNet( _snetR );
    defparse();
    defparse_done();
    return errors() == 0;
}

} // namespace
