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

#ifndef ADS_DEFIN_READER_H
#define ADS_DEFIN_READER_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DEFIN_IREADER_H
#include "definIReader.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

BEGIN_NAMESPACE_ADS

class definBlockage;
class definComponent;
class definFill;
class definGCell;
class definNet;
class definPin;
class definRow;
class definSNet;
class definTracks;
class definVia;
class definRegion;
class definNonDefaultRule;
class definPropDefs;
class definPinProps;

class definReader : public definIReader, public definBase
{
    dbDatabase *             _db;
    definBlockage *          _blockageR;
    definComponent *         _componentR;
    definFill *              _fillR;
    definGCell *             _gcellR;
    definNet *               _netR;
    definPin *               _pinR;
    definRow *               _rowR;
    definSNet *              _snetR;
    definTracks *            _tracksR;
    definVia *               _viaR;
    definRegion *            _regionR;
    definNonDefaultRule *    _non_default_ruleR;
    definPropDefs *          _prop_defsR;
    definPinProps *          _pin_propsR;
    std::vector<definBase *> _interfaces;
    bool             _update;
    const char *     _block_name;

    void init();
    void setLibs( std::vector<dbLib *> & lib_names );
    
    virtual void dieArea( const std::vector<defPoint> & points );
    virtual void error( const char * msg );
    virtual void units( int d );
    virtual void line( int line_num );

    void setTech( dbTech * tech );
    void setBlock( dbBlock * block );

    bool createBlock( const char * file );
    bool replaceWires( const char * file );
    void replaceWires();
    int errors();
    
  public:
    definReader( dbDatabase * db );
    virtual ~definReader();
    
    void skipConnections();
    void skipWires();
    void skipSpecialWires();
    void skipShields();
    void skipBlockWires();
    void skipFillWires();
    void useBlockName( const char * name );
    void namesAreDBIDs();
    void setAssemblyMode();
    
    dbChip * createChip( std::vector<dbLib *> & search_libs, const char * def_file );
    dbBlock * createBlock( dbBlock * parent, std::vector<dbLib *> & search_libs, const char * def_file );
    bool replaceWires( dbBlock * block, const char * def_file );
};
    
END_NAMESPACE_ADS

#endif
