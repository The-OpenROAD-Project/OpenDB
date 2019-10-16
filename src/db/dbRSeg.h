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

#ifndef ADS_DB_RSEG_H
#define ADS_DB_RSEG_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_DATABASE_H
#include "dbDatabase.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

#include "dbDatabase.h"

namespace odb {

class _dbNet;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

//struct _dbRSegFlags
//{
//    uint             _cnt          : 8;
//    uint             _path_dir     : 1; // 0 == low to hi coord
//    uint             _allocated_cap: 1; // 0, cap points to target node cap
//                                        // 1, cap is allocated
//    uint             _update_cap: 1;^M
//    uint             _spare_bits_21: 21;^M
//};

struct _dbRSegFlags
{
    uint             _path_dir     : 1; // 0 == low to hi coord
    uint             _allocated_cap: 1; // 0, cap points to target node cap
                                        // 1, cap is allocated
    uint             _update_cap: 1;
    uint             _spare_bits_29: 29;
};

class _dbRSeg : public dbObject
{
  public:
    enum Field  // dbJournal field names
    {
        FLAGS,
        SOURCE,
        TARGET,
        SHAPE_ID,
        RESISTANCE,
        CAPACITANCE,
        COORDINATES,
        ADDRSEGCAPACITANCE,
        ADDRSEGRESISTANCE
    };

    // PERSISTANT-MEMBERS
    _dbRSegFlags   _flags;
    uint           _source; // rc-network node-id
    uint           _target; // rc-network node-id
    union {
        uint           _shape_id; // DKF: shape-id to be combined with _net
        int            _xcoord;
    };
    int _ycoord;
    dbId<_dbRSeg>  _next;
    
    _dbRSeg( _dbDatabase *, const _dbRSeg & s );
    _dbRSeg( _dbDatabase * );
    ~_dbRSeg();

    int operator==( const _dbRSeg & rhs ) const;
    int operator!=( const _dbRSeg & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbRSeg & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
    int operator<( const _dbRSeg & rhs ) const
    {
        _dbRSeg * o1 = (_dbRSeg *) this;
        _dbRSeg * o2 = (_dbRSeg *) &rhs;
        return o1->getOID() < o2->getOID();
    }
};

inline _dbRSeg::_dbRSeg( _dbDatabase *, const _dbRSeg & s )
        : _flags(s._flags),
          _source(s._source),
          _target(s._target),
          _xcoord(s._xcoord),
          _ycoord(s._ycoord),
          _next(s._next)
{
}
    
inline _dbRSeg::_dbRSeg( _dbDatabase * )
{
    _flags._spare_bits_29 = 0;
    _flags._update_cap= 0;
    _flags._path_dir= 0;
    _flags._allocated_cap= 0;

    _source = 0;
    _target = 0;
    _xcoord = 0;
    _ycoord = 0;
}

inline _dbRSeg::~_dbRSeg()
{
    /*
    if ( _res )
        free( (void *) _res );

    if (( _cap ) && (_flags._allocated_cap>0))
        free( (void *) _cap );
    */
}

inline dbOStream & operator<<( dbOStream & stream, const _dbRSeg & seg )
{
    uint *bit_field = (uint *) &seg._flags;
    stream << *bit_field;
    stream << seg._source;
    stream << seg._target;
    if (stream.getDatabase()->isLessThanSchema(ADS_DB_EXT_RSEG_COORDS))
        stream << seg._shape_id;
    else
    {
        stream << seg._xcoord;
        stream << seg._ycoord;
    }
    stream << seg._next;
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbRSeg & seg )
{
    uint *bit_field = (uint *) &seg._flags;
    stream >> *bit_field;
    if (!stream.getDatabase()->isSchema(ADS_DB_INDEPENDENT_EXT_CORNERS))
        *bit_field = (*bit_field)>>8;
    stream >> seg._source;
    stream >> seg._target;
    if (stream.getDatabase()->isLessThanSchema(ADS_DB_EXT_RSEG_COORDS))
        stream >> seg._shape_id;
    else
    {
        stream >> seg._xcoord;
        stream >> seg._ycoord;
    }
    stream >> seg._next;
    return stream;
}

} // namespace

#endif
