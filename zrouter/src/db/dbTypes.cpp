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

#include "dbTypes.h"
#include "dbId.h"
#include <string.h>
#include <ctype.h>

BEGIN_NAMESPACE_ADS

//
// dbIdValidation methods here
//
bool
dbIdValidation::isId(const char *inid)
{
  if (!inid)
    return false;

  for (; *inid; inid++)
    {
      if (isdigit(*inid) == 0)
	return false;
    }

  return true;
}


static bool streq( const char * s1, const char * s2 )
{
    if (! (s1 && s2))
       return !((s1 && *s1) || (s2 && *s2));

    while( *s1 && *s2 )
    {
        if ( toupper(*s1) != toupper(*s2) )
            return false;

        ++s1;
        ++s2;
    }

    return ( *s1 == *s2 );
}

dbOrientType::dbOrientType( const char * orient )
{
    if ( streq( orient, "R0" ) )
        _value = R0;

    else if ( streq( orient, "R90" ) )
        _value = R90;

    else if ( streq( orient, "R180" ) )
        _value = R180;
    
    else if ( streq( orient, "R270" ) )
        _value = R270;

    else if ( streq( orient, "MY" ) )
        _value = MY;

    else if ( streq( orient, "MYR90" ) )
        _value = MYR90;

    else if ( streq( orient, "MX" ) )
        _value = MX;

    else if ( streq( orient, "MXR90" ) )
        _value = MXR90;

    else
        _value = R0;
}
    
dbOrientType::dbOrientType( Value orient )
{
    _value = orient;
}

dbOrientType::dbOrientType()
{
    _value = R0;
}

dbOrientType::dbOrientType( const dbOrientType & orient )
{
    _value = orient._value;
}

const char * dbOrientType::getString() const
{
    const char * value = "";
    
    switch( _value )
    {
        case R0:
            value = "R0";
            break;
            
        case R90:
            value = "R90";
            break;
            
        case R180:
            value = "R180";
            break;
            
        case R270:
            value = "R270";
            break;
            
        case MY:
            value = "MY";
            break;
            
        case MYR90:
            value = "MYR90";
            break;
            
        case MX:
            value = "MX";
            break;
            
        case MXR90:
            value = "MXR90";
            break;
    }

    return value;
}

dbSigType::dbSigType( const char * value )
{
    if ( streq( value, "SIGNAL" ) )
        _value = SIGNAL;
    
    else if ( streq( value, "POWER" ) )
        _value = POWER;
    
    else if ( streq( value, "GROUND" ) )
        _value = GROUND;
    
    else if ( streq( value, "CLOCK" ) )
        _value = CLOCK;

    else if ( streq( value, "ANALOG" ) )
        _value = ANALOG;

    else if ( streq( value, "ANALOG" ) )
        _value = ANALOG;

    else if ( streq( value, "RESET" ) )
        _value = RESET;

    else if ( streq( value, "SCAN" ) )
        _value = SCAN;

    else if ( streq( value, "TIEOFF" ) )
        _value = TIEOFF;

    else
        _value = SIGNAL;
}

dbSigType::dbSigType( Value value )
{
    _value = value;
}

dbSigType::dbSigType()
{
    _value = SIGNAL;
}

dbSigType::dbSigType( const dbSigType & value )
{
    _value = value._value;
}

const char * dbSigType::getString() const
{
    const char * value = "";

    switch( _value )
    {
        case SIGNAL:
            value = "SIGNAL";
            break;
            
        case POWER:
            value = "POWER";
            break;
            
        case GROUND:
            value = "GROUND";
            break;
            
        case CLOCK:
            value = "CLOCK";
            break;

        case ANALOG:
            value = "ANALOG";
            break;

        case RESET:
            value = "RESET";
            break;

        case SCAN:
            value = "SCAN";
            break;

        case TIEOFF:
            value = "TIEOFF";
            break;
    }

    return value;
}

dbIoType::dbIoType( const char * value )
{
    if ( streq( value, "INPUT" ) )
        _value = INPUT;
    
    else if ( streq( value, "OUTPUT" ) )
        _value = OUTPUT;
    
    else if ( streq( value, "INOUT" ) )
        _value = INOUT;

    else if ( streq( value, "FEEDTHRU" ) )
        _value = FEEDTHRU;
    
    else
        _value = INPUT;
}

dbIoType::dbIoType( Value value )
{
    _value = value;
}

dbIoType::dbIoType()
{
    _value = INPUT;
}

dbIoType::dbIoType( const dbIoType & value )
{
    _value = value._value;
}

const char * dbIoType::getString() const
{
    const char * value = "";

    switch( _value )
    {
        case INPUT:
            value = "INPUT";
            break;
            
        case OUTPUT:
            value = "OUTPUT";
            break;
            
        case INOUT:
            value = "INOUT";
            break;

        case FEEDTHRU:
            value = "FEEDTHRU";
            break;
    }

    return value;
}

dbPlacementStatus::dbPlacementStatus( const char * value )
{
    if ( streq( value, "NONE" ) )
        _value = NONE;

    else if ( streq( value, "UNPLACED" ) )
        _value = UNPLACED;

    else if ( streq( value, "SUGGESTED" ) )
        _value = SUGGESTED;

    else if ( streq( value, "PLACED" ) )
        _value = PLACED;

    else if ( streq( value, "LOCKED" ) )
        _value = LOCKED;

    else if ( streq( value, "FIRM" ) )
        _value = FIRM;

    else if ( streq( value, "COVER" ) )
        _value = COVER;

    else
        _value = NONE;
}

dbPlacementStatus::dbPlacementStatus( Value value )
{
    _value = value;
}

dbPlacementStatus::dbPlacementStatus()
{
    _value = NONE;
}

dbPlacementStatus::dbPlacementStatus( const dbPlacementStatus & value )
{
    _value = value._value;
}

const char * dbPlacementStatus::getString() const
{
    const char * value = "";
    
    switch( _value )
    {
        case NONE:
            value = "NONE";
            break;
        
        case UNPLACED:
            value = "UNPLACED";
            break;
        
        case SUGGESTED:
            value = "SUGGESTED";
            break;
        
        case PLACED:
            value = "PLACED";
            break;
        
        case LOCKED:
            value = "LOCKED";
            break;
        
        case FIRM:
            value = "FIRM";
            break;
        
        case COVER:
            value = "COVER";
            break;
     }
    
    return value;
}

bool
dbPlacementStatus::isPlaced() const
{
  return ((_value == PLACED) || (_value == LOCKED) || (_value == FIRM) || 
	  (_value == COVER));
}


dbMasterType::dbMasterType( const char * value )
{
    _value = NONE;
    
    if ( streq( value, "NONE" ) )
        _value = NONE;
    
    else if ( streq( value, "COVER" ) )
        _value = COVER;
    
    else if ( streq( value, "RING" ) )
        _value = RING;
    
    else if ( streq( value, "BLOCK" ) )
        _value = BLOCK;
    
    else if ( streq( value, "PAD" ) )
        _value = PAD;
    
    else if ( streq( value, "PAD INPUT" ) )
        _value = PAD_INPUT;
    
    else if ( streq( value, "PAD OUTPUT" ) )
        _value = PAD_OUTPUT;
    
    else if ( streq( value, "PAD INOUT" ) )
        _value = PAD_INOUT;
    
    else if ( streq( value, "PAD POWER" ) )
        _value = PAD_POWER;
    
    else if ( streq( value, "PAD SPACER" ) )
        _value = PAD_SPACER;
    
    else if ( streq( value, "CORE" ) )
        _value = CORE;
    
    else if ( streq( value, "CORE FEEDTHRU" ) )
        _value = CORE_FEEDTHRU;
    
    else if ( streq( value, "CORE TIEHIGH" ) )
        _value = CORE_TIEHIGH;
    
    else if ( streq( value, "CORE TIELOW" ) )
        _value = CORE_TIELOW;
    
    else if ( streq( value, "CORE SPACER" ) )
        _value = CORE_SPACER;
    
    else if ( streq( value, "ENDCAP" ) )
        _value = ENDCAP;
    
    else if ( streq( value, "ENDCAP PRE" ) )
        _value = ENDCAP_PRE;
    
    else if ( streq( value, "ENDCAP POST" ) )
        _value = ENDCAP_POST;
    
    else if ( streq( value, "ENDCAP TOPLEFT" ) )
        _value = ENDCAP_TOPLEFT;
    
    else if ( streq( value, "ENDCAP TOPRIGHT" ) )
        _value = ENDCAP_TOPRIGHT;
    
    else if ( streq( value, "ENDCAP BOTTOMLEFT" ) )
        _value = ENDCAP_BOTTOMLEFT;
    
    else if ( streq( value, "ENDCAP BOTTOMRIGHT" ) )
        _value = ENDCAP_BOTTOMRIGHT;
}

dbMasterType::dbMasterType( Value value )
{
    _value = value;
}

dbMasterType::dbMasterType()
{
    _value = NONE;
}

dbMasterType::dbMasterType( const dbMasterType & value )
{
    _value = value._value;
}

const char * dbMasterType::getString() const
{
    const char * value = "";
    
    switch( _value )
    {
        case NONE:
            value = "NONE";
            break;
            
        case COVER:
            value = "COVER";
            break;
            
        case RING:
            value = "RING";
            break;
            
        case BLOCK:
            value = "BLOCK";
            break;
            
        case PAD:
            value = "PAD";
            break;
            
        case PAD_INPUT:
            value = "PAD INPUT";
            break;
            
        case PAD_OUTPUT:
            value = "PAD OUTPUT";
            break;
            
        case PAD_INOUT:
            value = "PAD INOUT";
            break;
            
        case PAD_POWER:
            value = "PAD POWER";
            break;
            
        case PAD_SPACER:
            value = "PAD SPACER";
            break;
            
        case CORE:
            value = "CORE";
            break;
            
        case CORE_FEEDTHRU:
            value = "CORE FEEDTHRU";
            break;
            
        case CORE_TIEHIGH:
            value = "CORE TIEHIGH";
            break;
            
        case CORE_TIELOW:
            value = "CORE TIELOW";
            break;
            
        case ENDCAP:
            value = "ENDCAP";
            break;
            
        case ENDCAP_PRE:
            value = "ENDCAP PRE";
            break;
            
        case ENDCAP_POST:
            value = "ENDCAP POST";
            break;
            
        case ENDCAP_TOPLEFT:
            value = "ENDCAP TOPLEFT";
            break;
            
        case ENDCAP_TOPRIGHT:
            value = "ENDCAP TOPRIGHT";
            break;
            
        case ENDCAP_BOTTOMLEFT:
            value = "ENDCAP BOTTOMLEFT";
            break;
            
        case ENDCAP_BOTTOMRIGHT:
            value = "ENDCAP BOTTOMRIGHT";
            break;
    }
    
    return value;
}

dbTechLayerType::dbTechLayerType( const char * value )
{
    if ( streq( value, "ROUTING" ) )
        _value = ROUTING;
    
    else if ( streq( value, "CUT" ) )
        _value = CUT;
    
    else if ( streq( value, "MASTERSLICE" ) )
        _value = MASTERSLICE;

    else if ( streq( value, "OVERLAP" ) )
        _value = OVERLAP;

    else if ( streq( value, "IMPLANT" ) )
        _value = IMPLANT;

    else
        _value = NONE;
}

dbTechLayerType::dbTechLayerType( Value value )
{
    _value = value;
}

dbTechLayerType::dbTechLayerType()
{
    _value = ROUTING;
}

dbTechLayerType::dbTechLayerType( const dbTechLayerType & value )
{
    _value = value._value;
}

const char * dbTechLayerType::getString() const
{
    const char * value = "";
    
    switch( _value )
    {
        case NONE:
            value = "NONE";
            break;

        case ROUTING:
            value = "ROUTING";
            break;
            
        case CUT:
            value = "CUT";
            break;
            
        case MASTERSLICE:
            value = "MASTERSLICE";
            break;

        case OVERLAP:
            value = "OVERLAP";
            break;

        case IMPLANT:
            value = "IMPLANT";
            break;
    }

    return value;
}

dbTechLayerDir::dbTechLayerDir( const char * value )
{
    if ( streq( value, "NONE" ) )
        _value = NONE;
    
    else if ( streq( value, "HORIZONTAL" ) )
        _value = HORIZONTAL;
    
    else if ( streq( value, "VERTICAL" ) )
        _value = VERTICAL;

    else
        _value = NONE;
}

dbTechLayerDir::dbTechLayerDir( Value value )
{
    _value = value;
}

dbTechLayerDir::dbTechLayerDir()
{
    _value = NONE;
}

dbTechLayerDir::dbTechLayerDir( const dbTechLayerDir & value )
{
    _value = value._value;
}

const char * dbTechLayerDir::getString() const
{
    const char * value = "";
    
    switch( _value )
    {
        case NONE:
            value = "NONE";
            break;
            
        case HORIZONTAL:
            value = "HORIZONTAL";
            break;
            
        case VERTICAL:
            value = "VERTICAL";
            break;
    }

    return value;
}

dbBoxOwner::dbBoxOwner( const char * value )
{
    if ( streq( value, "UNKNOWN" ) )
        _value = UNKNOWN;

    else if ( streq( value, "BLOCK" ) )
        _value = BLOCK;
    
    else if ( streq( value, "INST" ) )
        _value = INST;

    else if ( streq( value, "BTERM" ) )
        _value = BTERM;

    else if ( streq( value, "BPIN" ) )
        _value = BPIN;

    else if ( streq( value, "BPIN" ) )
        _value = BPIN;

    else if ( streq( value, "VIA" ) )
        _value = VIA;

    else if ( streq( value, "OBSTRUCTION" ) )
        _value = OBSTRUCTION;

    else if ( streq( value, "BLOCKAGE" ) )
        _value = BLOCKAGE;

    else if ( streq( value, "MASTER" ) )
        _value = MASTER;

    else if ( streq( value, "MPIN" ) )
        _value = MPIN;

    else if ( streq( value, "TECH VIA" ) )
        _value = TECH_VIA;

    else if ( streq( value, "SWIRE" ) )
        _value = SWIRE;
    else if ( streq( value, "REGION" ) )
        _value = REGION;
}

dbBoxOwner::dbBoxOwner( Value value )
{
    _value = value;
}

dbBoxOwner::dbBoxOwner()
{
    _value = BLOCK;
}

dbBoxOwner::dbBoxOwner( const dbBoxOwner & value )
{
    _value = value._value;
}

const char * dbBoxOwner::getString() const
{
    const char * value = "";

    switch( _value )
    {
        case UNKNOWN:
            value = "UNKNOWN";
            break;
        
        case BLOCK:
            value = "BLOCK";
            break;
        
        case INST:
            value = "INST";
            break;

        case BTERM:
            value = "BTERM";
            break;

        case BPIN:
            value = "BPIN";
            break;

        case VIA:
            value = "VIA";
            break;

        case OBSTRUCTION:
            value = "OBSTRUCTION";
            break;

        case BLOCKAGE:
            value = "BLOCKAGE";
            break;

        case MASTER:
            value = "MASTER";
            break;

        case MPIN:
            value = "MPIN";
            break;

        case TECH_VIA:
            value = "TECH_VIA";
            break;

        case SWIRE:
            value = "SWIRE";
            break;

        case REGION:
            value = "REGION";
            break;
    }

    return value;
}

dbWireType::dbWireType( const char * value )
{
    _value = NONE;
    if ( streq( value, "NONE" ) )
        _value = NONE;

    else if ( streq( value, "COVER" ) )
        _value = COVER;

    else if ( streq( value, "FIXED" ) )
        _value = FIXED;
    
    else if ( streq( value, "ROUTED" ) )
        _value = ROUTED;

    else if ( streq( value, "SHIELD" ) )
        _value = SHIELD;

    else if ( streq( value, "NOSHIELD" ) )
        _value = NOSHIELD;
}

dbWireType::dbWireType( Value value )
{
    _value = value;
}

dbWireType::dbWireType()
{
    _value = NONE;
}

dbWireType::dbWireType( const dbWireType & value )
{
    _value = value._value;
}

const char * dbWireType::getString() const
{
    const char * value = "";

    switch( _value )
    {
        case NONE:
            value = "NONE";
            break;

        case COVER:
            value = "COVER";
            break;
        
        case FIXED:
            value = "FIXED";
            break;
        
        case ROUTED:
            value = "ROUTED";
            break;

        case SHIELD:
            value = "SHIELD";
            break;

        case NOSHIELD:
            value = "NOSHIELD";
            break;
    }

    return value;
}

dbWireShapeType::dbWireShapeType( const char * value )
{
    if ( streq( value, "NONE" ) )
        _value = NONE;

    else if ( streq( value, "RING" ) )
        _value = RING;

    else if ( streq( value, "PADRING" ) )
        _value = PADRING;
    
    else if ( streq( value, "BLOCKRING" ) )
        _value = BLOCKRING;

    else if ( streq( value, "STRIPE" ) )
        _value = STRIPE;

    else if ( streq( value, "FOLLOWPIN" ) )
        _value = FOLLOWPIN;

    else if ( streq( value, "IOWIRE" ) )
        _value = IOWIRE;

    else if ( streq( value, "COREWIRE" ) )
        _value = COREWIRE;

    else if ( streq( value, "BLOCKWIRE" ) )
        _value = BLOCKWIRE;

    else if ( streq( value, "BLOCKAGEWIRE" ) )
        _value = BLOCKAGEWIRE;

    else if ( streq( value, "FILLWIRE" ) )
        _value = FILLWIRE;

    else if ( streq( value, "DRCFILL" ) )
        _value = DRCFILL;
}

dbWireShapeType::dbWireShapeType( Value value )
{
    _value = value;
}

dbWireShapeType::dbWireShapeType()
{
    _value = NONE;
}

dbWireShapeType::dbWireShapeType( const dbWireShapeType & value )
{
    _value = value._value;
}

const char * dbWireShapeType::getString() const
{
    const char * value = "";

    switch( _value )
    {
        case NONE:
            value = "NONE";
            break;
        
        case RING:
            value = "RING";
            break;
        
        case PADRING:
            value = "PADRING";
            break;
        
        case BLOCKRING:
            value = "BLOCKRING";
            break;

        case STRIPE:
            value = "STRIPE";
            break;
        
        case FOLLOWPIN:
            value = "FOLLOWPIN";
            break;
        
        case IOWIRE:
            value = "IOWIRE";
            break;
        
        case COREWIRE:
            value = "COREWIRE";
            break;
        
        case BLOCKWIRE:
            value = "BLOCKWIRE";
            break;
        
        case BLOCKAGEWIRE:
            value = "BLOCKAGEWIRE";
            break;
        
        case FILLWIRE:
            value = "FILLWIRE";
            break;
        
        case DRCFILL:
            value = "DRCFILL";
            break;
    }

    return value;
}

dbSiteClass::dbSiteClass( const char * value )
{
    if ( streq( value, "NONE" ) )
        _value = NONE;

    else if ( streq( value, "PAD" ) )
        _value = PAD;

    else if ( streq( value, "CORE" ) )
        _value = CORE;
}

dbSiteClass::dbSiteClass( Value value )
{
    _value = value;
}

dbSiteClass::dbSiteClass()
{
    _value = NONE;
}

dbSiteClass::dbSiteClass( const dbSiteClass & value )
{
    _value = value._value;
}

const char * dbSiteClass::getString() const
{
    const char * value = "";

    switch( _value )
    {
        case NONE:
            value = "NONE";
            break;
        
        case PAD:
            value = "PAD";
            break;
        
        case CORE:
            value = "CORE";
            break;
    }

    return value;
}

///
/// dbOnOffType methods here
///
dbOnOffType::dbOnOffType( const char * instr )
{
    if ( streq( instr, "ON" ) )
        _value = ON;
    else if ( streq( instr, "OFF" ) )
        _value = OFF;
    else
        _value = OFF;
}
    
dbOnOffType::dbOnOffType( Value inval )
{    _value = inval;  }

dbOnOffType::dbOnOffType( int innum )
{    _value = (innum == 0) ? OFF : ON ;  }

dbOnOffType::dbOnOffType( bool insw )
{    _value = (insw) ? ON : OFF ;  }

dbOnOffType::dbOnOffType()
{    _value = OFF;  }

const char * dbOnOffType::getString() const
{
    const char * value = "";
    
    switch( _value )
    {
        case OFF:
            value = "OFF";
            break;
            
        case ON:
            value = "ON";
            break;
    }

    return value;
}

int dbOnOffType::getAsInt() const
{  return (_value == ON) ? 1 : 0 ; }

bool dbOnOffType::isSet() const
{  return (_value == ON); }

///
/// dbClMeasureType methods here
///
dbClMeasureType::dbClMeasureType( const char * instr )
{
    if ( streq( instr, "MAXXY" ) )
        _value = MAXXY;
    else if ( streq( instr, "EUCLIDEAN" ) )
        _value = EUCLIDEAN;
    else
        _value = EUCLIDEAN;
}
    
const char * dbClMeasureType::getString() const
{
    const char * value = "";
    
    switch( _value )
    {
        case EUCLIDEAN:
            value = "EUCLIDEAN";
            break;
            
        case MAXXY:
            value = "MAXXY";
            break;
    }

    return value;
}

//
//  dbRowDir methods here
//
dbRowDir::dbRowDir( const char * value )
{
    if ( streq( value, "HORIZONTAL" ) )
        _value = HORIZONTAL;
    
    else
        _value = VERTICAL;

}

dbRowDir::dbRowDir( Value value )
{
    _value = value;
}

dbRowDir::dbRowDir()
{
    _value = HORIZONTAL;
}

dbRowDir::dbRowDir( const dbRowDir & value )
{
    _value = value._value;
}

const char * dbRowDir::getString() const
{
    const char * value = "";
    
    switch( _value )
    {
        case HORIZONTAL:
            value = "HORIZONTAL";
            break;
            
        case VERTICAL:
            value = "VERTICAL";
            break;
    }

    return value;
}

const char * dbRegionType::getString() const
{
    const char * value = "";
    
    switch( _value )
    {
        case INCLUSIVE:
            value = "INCLUSIVE";
            break;
            
        case EXCLUSIVE:
            value = "EXCLUSIVE";
            break;

        case SUGGESTED:
            value = "SUGGESTED";
            break;
    }

    return value;
}

const char * dbSourceType::getString() const
{
    const char * value = "";
    
    switch( _value )
    {
        case NONE:
            value = "NONE";
            break;
            
        case NETLIST:
            value = "NETLIST";
            break;

        case DIST:
            value = "DIST";
            break;

        case USER:
            value = "USER";
            break;

        case TIMING:
            value = "TIMING";
            break;

        case TEST:
            value = "TEST";
            break;
    }

    return value;
}

const char * TechLayerSpacingRule::getString( TechLayerSpacingRule::_RuleType v )
{
    const char * s = "";
    
    switch(v)
    {
        case TechLayerSpacingRule::DEFAULT:
            s = "DEFAULT";
            break;
            
        case TechLayerSpacingRule::RANGE_ONLY:
            s = "RANGE_ONLY";
            break;
            
        case TechLayerSpacingRule::RANGE_USELENGTH:
            s = "RANGE_USELENGTH";
            break;
            
        case TechLayerSpacingRule::RANGE_INFLUENCE:
            s = "RANGE_INFLUENCE";
            break;
            
        case TechLayerSpacingRule::RANGE_INFLUENCE_RANGE:
            s = "RANGE_INFLUENCE_RANGE";
            break;
            
        case TechLayerSpacingRule::RANGE_RANGE:
            s = "RANGE_RANGE";
            break;
            
        case TechLayerSpacingRule::LENGTHTHRESHOLD:
            s = "LENGTHTHRESHOLD";
            break;
            
        case TechLayerSpacingRule::LENGTHTHRESHOLD_RANGE:
            s = "LENGTHTHRESHOLD_RANGE";
            break;
            
        case TechLayerSpacingRule::CUT_LAYER_BELOW:
            s = "CUT_LAYER_BELOW";
            break;
            
        case TechLayerSpacingRule::ADJACENT_CUTS_INFLUENCE:
            s = "ADJACENT_CUTS_INFLUENCE";
            break;
    }

    return s;
}

const char * TechMinCutRule::getString( TechMinCutRule::_RuleType v )
{
    const char * s = "";
    switch(v)
    {
        case TechMinCutRule::NONE:
            s = "NONE";
            break;
            
        case TechMinCutRule::MINIMUM_CUT:
            s = "MINIMUM_CUT";
            break;
            
        case TechMinCutRule::MINIMUM_CUT_ABOVE:
            s = "MINIMUM_CUT_ABOVE";
            break;
            
        case TechMinCutRule::MINIMUM_CUT_BELOW:
            s = "MINIMUM_CUT_BELOW";
            break;
    }

    return s;
}

//
//  dbJournalEntryType methods here
//
const char *
dbJournalEntryType::getString() const
{
  const char * value = "";

  switch( _value )  {

    case NONE:
      value = "NONE";
      break;

    case OWNER:
      value = "OWNER";
      break;

    case ADD:
      value = "ADD";
      break;

    case DESTROY:
      value = "DESTROY";
      break;
  }
  
  return value;
}

//
//  dbDirection methods here
//
const char *
dbDirection::getString() const
{
  const char * value = "";

  switch( _value )  {

    case NONE:
      value = "NONE";
      break;

    case NORTH:
      value = "NORTH";
      break;

    case EAST:
      value = "EAST";
      break;

    case SOUTH:
      value = "SOUTH";
      break;

    case WEST:
      value = "WEST";
      break;
  }
  
  return value;
}

END_NAMESPACE_ADS
