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

#ifndef ADS_DB_TMG_H
#define ADS_DB_TMG_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

#include "dbDatabase.h"

BEGIN_NAMESPACE_ADS

class dbIStream;
class dbOStream;
class dbDiff;

struct _dbTmgScenario
{ 
    uint _slew_rise : 8;
    uint _slew_fall : 8;
    uint _slack_rise : 8;
    uint _slack_fall : 8;
};

class _dbTmg : public dbObject
{
  public:
    float        _slew_rise; // payam: changed int to float
    float        _slew_fall; // payam: changed int to float
    float        _slack_rise; // payam: changed int to float
    float        _slack_fall; // payam: changed int to float
    // _dbTmgScenario _scenario;
    
    _dbTmg( _dbDatabase * );
    _dbTmg( _dbDatabase *, const _dbTmg & i );
    ~_dbTmg();
    int operator==( const _dbTmg & rhs ) const;
    int operator!=( const _dbTmg & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbTmg & rhs ) const;
    void differences( dbDiff & diff, const char * field, const _dbTmg & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
    bool updateSlacks(float wns[2], float tns[2], uint mode);
    static void merge( _dbTmg & dst, _dbTmg & src, int slew_op, int slack_op);
    static float min( float a,  float b, bool &secondNumber);
    static float max( float a,  float b, bool &secondNumber);
};

inline _dbTmg::_dbTmg( _dbDatabase * )
        : _slew_rise(2),
          _slew_fall(2),
          _slack_rise(2),
          _slack_fall(2)
{
        //_scenario._slew_rise = 0; 
        //_scenario._slew_fall = 0; 
        //_scenario._slack_rise = 0; 
        //_scenario._slack_fall = 0; 
}

inline _dbTmg::_dbTmg( _dbDatabase *, const _dbTmg & i )
        : _slew_rise(i._slew_rise),
          _slew_fall(i._slew_fall),
          _slack_rise(i._slack_rise),
          _slack_fall(i._slack_fall)
{
        //_scenario._slew_rise = 0; 
        //_scenario._slew_fall = 0; 
        //_scenario._slack_rise = 0; 
        //_scenario._slack_fall = 0; 
}

inline _dbTmg::~_dbTmg()
{
}

inline int _dbTmg::operator==( const _dbTmg & rhs ) const
{
    if ( _slew_rise != rhs._slew_rise )
        return false;

    if ( _slew_fall != rhs._slew_fall )
        return false;

    if ( _slack_rise != rhs._slack_rise )
        return false;

    if ( _slack_fall != rhs._slack_fall )
        return false;

//    if ( _scenario._slew_rise != rhs._scenario._slew_rise )
//        return false;
//
//    if ( _scenario._slew_fall != rhs._scenario._slew_fall )
//        return false;
//
//    if ( _scenario._slack_rise != rhs._scenario._slack_rise )
//        return false;
//
//    if ( _scenario._slack_fall != rhs._scenario._slack_fall )
//        return false;

    return true;
}

inline int _dbTmg::operator<( const _dbTmg & rhs ) const
{
    return getOID() < rhs.getOID();
}

inline dbOStream & operator<<( dbOStream & stream, const _dbTmg & tmg )
{ 
    stream << tmg._slew_rise;
    stream << tmg._slew_fall;
    stream << tmg._slack_rise;
    stream << tmg._slack_fall;
    
//    uint * bit_field = (uint *) &tmg._scenario;
//    stream << *bit_field; 

    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbTmg & tmg )
{
    stream >> tmg._slew_rise;
    stream >> tmg._slew_fall;
    stream >> tmg._slack_rise;
    stream >> tmg._slack_fall; 
    
//    if ( stream.getDatabase()->isSchema(ADS_DB_TMG_SCENARIO) ) { 
//        uint * bit_field = (uint *) &tmg._scenario; 
//        stream >> *bit_field;
//    }
//    else { 
//        tmg._scenario._slew_rise = 0; 
//        tmg._scenario._slew_fall = 0; 
//        tmg._scenario._slack_rise = 0; 
//        tmg._scenario._slack_fall = 0; 
//    }
    return stream;
}

END_NAMESPACE_ADS

#endif
