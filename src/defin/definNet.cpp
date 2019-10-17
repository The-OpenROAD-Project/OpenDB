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
#include "definNet.h"
#include "dbWireCodec.h"
#include "dbShape.h"
#include "db.h"

namespace odb {

inline uint get_net_dbid( const char * name )
{
    if ( *name != 'N' )
        return 0;

    ++name;
    
    if ( *name == '\0' )
        return 0;

    char *end;
    uint dbid = strtoul( name, &end, 10 );

    if ( *end != '\0' )
        return 0;

    return dbid;
}

definNet::definNet()
{
    init();
    _skip_signal_connections = false;
    _skip_wires = false;
    _replace_wires = false;
    _names_are_ids = false;
    _assembly_mode = false;
}

definNet::~definNet()
{
}

void definNet::init()
{
    definBase::init();
    _found_new_routing = false;
    _net_cnt = 0;
    _net_iterm_cnt = 0;
    _cur_net = NULL;
    _cur_layer = NULL;
    _wire = NULL;
    _wire_type = dbWireType::NONE;
    _wire_shape_type = dbWireShapeType::NONE;
    _prev_x = 0;
    _prev_y = 0;
    _width = 0;
    _point_cnt = 0;
    _taper_rule = NULL;
    _non_default_rule = NULL;
    _rule_for_path = NULL;
    _rotated_vias.clear();
}

void definNet::begin( const char * name )
{
    assert( _cur_net == NULL);

    if ( _replace_wires == false )
    {
        _cur_net = _block->findNet( name );

        if ( _cur_net == NULL )
            _cur_net = dbNet::create( _block, name );

        _non_default_rule = NULL;
    }
    else
    {
        if ( _names_are_ids == false )
            _cur_net = _block->findNet( name );
        else
        {
            uint netid = get_net_dbid(name);

            if ( netid )
                _cur_net = dbNet::getNet( _block, netid );
        }
    
        if ( _cur_net == NULL )
        {
            notice(0,"net %s does not exist\n", name );
            ++_errors;
        }
        else
        {
             if ( ! _assembly_mode )
             {
                dbWire * wire = _cur_net->getWire();

                if ( wire )
                    dbWire::destroy(wire);
             }
        }

        // As per Glenn, in replace mode, use the current non-default-rule.
        // WRoute does not write the rule in the DEF.
        // This may cause problems with other routers.
        _non_default_rule = _cur_net->getNonDefaultRule();
    }

    _net_cnt++;
    _wire = NULL;
    _rule_for_path = NULL;
    _found_new_routing = false;
}

void
definNet::beginMustjoin( const char * iname, const char * tname )
{
    assert( _cur_net == NULL);

    if ( _replace_wires == false )
    {
        char buf[BUFSIZ];
        sprintf( buf, "__%d__mustjoin\n", _net_cnt );
    
        _cur_net = _block->findNet( buf );
    
        if ( _cur_net == NULL )
        {
            notice(0,"duplicate mustjoin net found (%s)\n", buf );
            ++_errors;
        }

        _cur_net = dbNet::create( _block, buf );
        connection( iname, tname );
    }
    
    _net_cnt++;
    _wire = NULL;
    _non_default_rule = NULL;
    _rule_for_path = NULL;
}

void
definNet::connection( const char * iname, const char * tname )
{
    if ( _skip_signal_connections == true )
        return;

    if ( (_cur_net == NULL) || (_replace_wires == true) )
        return;

    if ( iname[0] == 'P' || iname[0] == 'p' )
    {
        if ( iname[1] == 'I' || iname[1] == 'i' )
        {
            if ( iname[2] == 'N' || iname[2] == 'n' )
            {
                if ( iname[3] == 0 )
                {
                    if ( _block->findBTerm(tname) == NULL )
                    {
                        dbBTerm::create(_cur_net, tname);
                    }

                    return;
                }
            }
        }
    }
    
    dbInst * inst = _block->findInst(iname);
    
    if ( inst == NULL )
    {
        notice(0,"error: netlist component (%s) is not defined\n", iname);
        ++_errors;
        return;
    }

    dbMaster * master = inst->getMaster();
    dbMTerm * mterm = master->findMTerm( _block, tname );

    if ( mterm == NULL )
    {
        notice(0,"error: netlist component-pin (%s, %s) is not defined\n", iname, tname );
        ++_errors;
        return;
    }

    dbITerm::connect( inst, _cur_net, mterm );
    _net_iterm_cnt++;
}

dbTechNonDefaultRule * definNet::findNonDefaultRule( const char * name )
{
    dbTechNonDefaultRule * rule = _block->findNonDefaultRule( name );

    if ( rule )
        return rule;
    
    rule = _tech->findNonDefaultRule( name );
    return rule;
}

void
definNet::nonDefaultRule( const char * rule )
{
    if ( _cur_net == NULL )
        return;

    if ( _replace_wires == true )
    {
        // As per Glenn, in "replace" mode, ignore the
        // non default rule, because wroute does not write it (even if it exists).
        // Issue an error, if the rules do not match.
        dbTechNonDefaultRule * net_rule = _cur_net->getNonDefaultRule();
        dbTechNonDefaultRule * def_rule = findNonDefaultRule( rule );

        if ( def_rule == NULL )
        {
            notice(0,"error: undefined NONDEFAULTRULE (%s) referenced\n", rule );
            ++_errors;
        }

        else if ( net_rule != def_rule )
        {
            dbString net_name = _cur_net->getName();
            const char * net_rule_name = "(NULL)";
            dbString n;
            
            if ( net_rule != NULL )
            {
                n = net_rule->getName();
                net_rule_name = n.c_str();
            }
                
            notice(0,"error: NONDEFAULTRULE (%s) of net (%s) does not match DEF rule (%s).\n",
                   net_name.c_str(), net_rule_name, rule );
            ++_errors;
        }
    }
    else
    {
        _non_default_rule = findNonDefaultRule( rule );

        if ( _non_default_rule == NULL )
        {
            notice(0,"error: undefined NONDEFAULTRULE (%s) referenced\n", rule );
            ++_errors;
        }
        else
            _cur_net->setNonDefaultRule( _non_default_rule );
    }
}

void
definNet::use( defSigType type )
{
    if ( (_cur_net == NULL) || (_replace_wires == true) )
        return;

    switch( type )
    {
        case DEF_SIG_ANALOG:
            _cur_net->setSigType( dbSigType::ANALOG );
            break;
        case DEF_SIG_CLOCK:
            _cur_net->setSigType( dbSigType::CLOCK );
            break;
        case DEF_SIG_GROUND:
            _cur_net->setSigType( dbSigType::GROUND );
            break;
        case DEF_SIG_POWER:
            _cur_net->setSigType( dbSigType::POWER );
            break;
        case DEF_SIG_RESET:
            _cur_net->setSigType( dbSigType::RESET );
            break;
        case DEF_SIG_SCAN:
            _cur_net->setSigType( dbSigType::SCAN );
            break;
        case DEF_SIG_SIGNAL:
            _cur_net->setSigType( dbSigType::SIGNAL );
            break;
        case DEF_SIG_TIEOFF:
            _cur_net->setSigType( dbSigType::TIEOFF );
            break;
    }
}

void definNet::source( defSource source )
{
    if ( (_cur_net == NULL) || (_replace_wires == true) )
        return;

    switch( source)
    {
        case DEF_DIST:
            _cur_net->setSourceType( dbSourceType::DIST );
            break;
            
        case DEF_NETLIST:
            _cur_net->setSourceType( dbSourceType::NETLIST );
            break;
            
        case DEF_TEST:
            _cur_net->setSourceType( dbSourceType::TEST );
            break;
            
        case DEF_TIMING:
            _cur_net->setSourceType( dbSourceType::TIMING );
            break;
            
        case DEF_USER:
            _cur_net->setSourceType( dbSourceType::USER );
            break;
    }
}

void definNet::weight( int weight )
{
    if ( (_cur_net == NULL) || (_replace_wires == true) )
        return;

    _cur_net->setWeight( weight );
}

void definNet::fixedbump()
{
    if ( (_cur_net == NULL) || (_replace_wires == true) )
        return;

    _cur_net->setFixedBump( true );
}

void definNet::xtalk(int value)
{
    if ( (_cur_net == NULL) || (_replace_wires == true) )
        return;

    _cur_net->setXTalkClass( value );
}

void definNet::wire( defWireType type )
{
    if ( _skip_wires )
        return;
    
    if ( _wire == NULL )
    {
        if ( ! _assembly_mode )
            _wire = dbWire::create(_cur_net);
        else
        {
            _wire = _cur_net->getWire();

            if ( _wire == NULL )
                _wire = dbWire::create(_cur_net);
        }
        _wire_encoder.begin(_wire);
    }
    
    switch( type )
    {
        case DEF_WIRE_COVER:
            _wire_type = dbWireType::COVER;
            break;
        case DEF_WIRE_FIXED:
            _wire_type = dbWireType::FIXED;
            break;
        case DEF_WIRE_ROUTED:
            _wire_type = dbWireType::ROUTED;
            _found_new_routing = true;
            break;
        case DEF_WIRE_NOSHIELD:
            _wire_type = dbWireType::NOSHIELD;
            _found_new_routing = true;
            break;
        case DEF_WIRE_SHIELD:
            _wire_type = dbWireType::SHIELD;
            _found_new_routing = true;
            break;
    }

    _taper_rule = NULL;
}

void definNet::pathStyle( int style )
{
    notice(0,"error: styles are not supported on wires\n");
    ++_errors;
}

void definNet::path( const char * layer_name )
{
    _rule_for_path = _non_default_rule;
    pathBegin(layer_name);
}

void definNet::pathBegin( const char * layer_name )
{
    if ( _wire == NULL )
        return;

    _cur_layer = _tech->findLayer(layer_name);

    if ( _cur_layer == NULL )
    {
        notice(0,"error: undefined layer (%s) referenced\n", layer_name );
        ++_errors;
        dbWire::destroy(_wire);
        _wire = NULL;
        return;
    }

    _taper_rule = NULL;
    if ( _rule_for_path )
    {
        _taper_rule = _rule_for_path->getLayerRule( _cur_layer );

        if ( _taper_rule == NULL )
        {
            dbString lyr_name = _cur_layer->getName();
            dbString rule_name = _non_default_rule->getName();
            notice(0,"error: RULE (%s) referenced for layer (%s)\n",
                _rule_for_path->getName().c_str(), lyr_name.c_str() );
            ++_errors;
        }
    }

    if ( _taper_rule )
        _wire_encoder.newPath( _cur_layer, _wire_type, _taper_rule );
    else
        _wire_encoder.newPath( _cur_layer, _wire_type );
}

void definNet::pathTaper( const char * layer )
{
    _rule_for_path = NULL; // turn off non-default-rule for this path
    pathBegin(layer);
}

void definNet::pathTaperRule( const char * layer_name, const char * rule_name )
{
    _rule_for_path = findNonDefaultRule(rule_name);

    if ( _rule_for_path == NULL )
    {
        notice(0,"error: undefined TAPER RULE (%s) referenced\n", rule_name );
        ++_errors;
        path(layer_name);
        return;
    }

    pathBegin(layer_name);
}

void definNet::pathPoint( int x, int y )
{
    if ( _wire == NULL )
        return;

    _wire_encoder.addPoint( dbdist(x), dbdist(y) );
}

void definNet::pathPoint( int x, int y, int ext )
{
    if ( _wire == NULL )
        return;

    _wire_encoder.addPoint( dbdist(x), dbdist(y), dbdist(ext) );
}

void definNet::getUniqueViaName( std::string & viaName )
{
    if( (_tech->findVia(viaName.c_str()) == NULL) && (_block->findVia(viaName.c_str()) == NULL) )
        return;

    int cnt = 1;
    for( ;; ++cnt )
    {
        char buffer[16];
        snprintf( buffer, 15, "%d", cnt );
        std::string vn(viaName);
        vn += buffer;

        if( (_tech->findVia(vn.c_str()) == NULL) && (_block->findVia(vn.c_str()) == NULL) )
        {
            viaName = vn;
            return;
        }
    }
}
    
dbVia * definNet::getRotatedVia(const char * via_name, int orient )
{
    std::string viaName(via_name);
    dbOrientType::Value db_orient;

    switch( orient )
    {
        case DEF_ORIENT_N:
            viaName += "_N";
            db_orient = dbOrientType::R0;
            break;
            
        case DEF_ORIENT_S:
            viaName += "_S";
            db_orient = dbOrientType::R180;
            break;
            
        case DEF_ORIENT_E:
            viaName += "_E";
            db_orient = dbOrientType::R270;
            break;
            
        case DEF_ORIENT_W:
            viaName += "_W";
            db_orient = dbOrientType::R90;
            break;
            
        case DEF_ORIENT_FN:
            viaName += "_FN";
            db_orient = dbOrientType::MY;
            break;
            
        case DEF_ORIENT_FS:
            viaName += "_FS";
            db_orient = dbOrientType::MX;
            break;
            
        case DEF_ORIENT_FE:
            viaName += "_FE";
            db_orient = dbOrientType::MYR90;
            break;
            
        case DEF_ORIENT_FW:
            viaName += "_FW";
            db_orient = dbOrientType::MXR90;
            break;
    }

    dbVia * & via = _rotated_vias[viaName];

    if ( via != NULL )
        return via;

    getUniqueViaName(viaName);

    dbTechVia * tech_via = _tech->findVia(via_name);
  
    if ( tech_via )
    {
        via = dbVia::create( _block, viaName.c_str(), tech_via, db_orient );
    }
    
    else
    {
        dbVia * block_via = _block->findVia(via_name);

        if ( block_via == NULL )
        {
            notice(0,"error: undefined via (%s) referenced\n", via_name);
            ++_errors;
            return NULL;
        }

        via = dbVia::create( _block, viaName.c_str(), block_via, db_orient );
    }

    return via;
}

void definNet::pathVia( const char * via_name, int def_orient )
{
    if ( _wire == NULL )
        return;

    
    dbVia * via = getRotatedVia(via_name, def_orient);

    if ( via == NULL )
        return;
    
    _wire_encoder.addVia( via );
    dbTechLayer * top = via->getTopLayer();
    dbTechLayer * bot = via->getBottomLayer();
    
    if ( top == _cur_layer )
        _cur_layer = bot;
    else if ( bot == _cur_layer )
        _cur_layer = top;
    else
    {
        ++_errors;
        notice(0,"error: invalid VIA layers, cannot determine exit layer of path\n");
    }
}

void definNet::pathVia( const char * via_name )
{
    if ( _wire == NULL )
        return;

    dbTechLayer * top;
    dbTechLayer * bot;
    dbTechVia * tech_via = _tech->findVia(via_name);
  
    if ( tech_via != NULL )
    {
        _wire_encoder.addTechVia( tech_via );
        top = tech_via->getTopLayer();
        bot = tech_via->getBottomLayer();
    }
    else
    {
        dbVia * via = _block->findVia(via_name);

        if ( via == NULL )
        {
            notice(0,"error: undefined via (%s) referenced\n", via_name);
            ++_errors;
            return;
        }

        _wire_encoder.addVia( via );
        top = via->getTopLayer();
        bot = via->getBottomLayer();
    }
    
    if ( top == _cur_layer )
        _cur_layer = bot;
    else if ( bot == _cur_layer )
        _cur_layer = top;
    else
    {
        ++_errors;
        notice(0,"error: invalid VIA layers, cannot determine exit layer of path\n");
    }
}

void definNet::pathEnd()
{
    _cur_layer = NULL;
}

void definNet::wireEnd()
{
}

void definNet::property( const char * name, const char * value )
{
    if ( (_cur_net == NULL) || _replace_wires )
        return;

    dbProperty * p = dbProperty::find(_cur_net,name);
    if ( p )
        dbProperty::destroy(p);

    dbStringProperty::create(_cur_net,name,value);
}

void definNet::property( const char * name, int value )
{
    if ( (_cur_net == NULL) || _replace_wires )
        return;

    dbProperty * p = dbProperty::find(_cur_net,name);
    if ( p )
        dbProperty::destroy(p);

    dbIntProperty::create(_cur_net,name,value);
}

void definNet::property( const char * name, double value )
{
    if ( (_cur_net == NULL) || _replace_wires )
        return;

    dbProperty * p = dbProperty::find(_cur_net,name);

    if ( p )
        dbProperty::destroy(p);

    dbDoubleProperty::create(_cur_net,name,value);
}

void definNet::end()
{
    if ( _cur_net == NULL )
        return;
    
    if ( ! _replace_wires )
    {
        dbSet<dbITerm> iterms = _cur_net->getITerms();

        if ( iterms.reversible() && iterms.orderReversed() )
            iterms.reverse();

        dbSet<dbProperty> props = dbProperty::getProperties(_cur_net);

        if ( !props.empty() && props.orderReversed() )
            props.reverse();

    }

    if (_wire)
    {
        if (_assembly_mode && !_found_new_routing)
        {
//          notice(0,"CANCEL wiring for net %s ID: %d\n", _cur_net->getName().c_str(), _cur_net->getId());
            _wire_encoder.clear();
        }
        else
        {
//          notice(0,"Committing wiring for net %s ID: %d\n", _cur_net->getName().c_str(), _cur_net->getId());
            _wire_encoder.end();

            if ( _replace_wires )
                _cur_net->setWireAltered(true);
        }
    }
    else
    {
//    notice(0,"NO WIRE for net %s ID: %d\n", _cur_net->getName().c_str(), _cur_net->getId());
    }

    _cur_net = NULL;
}
    
} // namespace
