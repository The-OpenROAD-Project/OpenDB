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

#ifndef ADS_ZINTERFACE_H
#define ADS_ZINTERFACE_H

#include <vector>

#ifndef _TCL
#include <tcl.h>
#endif

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_ZEXCEPTION_H
#include "ZException.h"
#endif

namespace odb {

class dbDatabase;
class ZSession;
class ZObject;
class dbObject;

/////////////////////////////////
/// Event value types
/////////////////////////////////
enum ZValueType
{
    Z_CHAR,
    Z_UCHAR,
    Z_SHORT,
    Z_USHORT,
    Z_INT,
    Z_UINT,
    Z_FLOAT,
    Z_DOUBLE,
    Z_STRING,
    Z_BOOL,
    Z_ZOBJECT,
    Z_DBOBJECT
};

///////////////////////////////////////////////////////
// ZEvent - Alternative interface for generating events
///////////////////////////////////////////////////////
class ZEvent
{
    struct Attr
    {
        const char * _name;
        ZValueType   _type;
        union 
        {
            int          _int;
            unsigned int _uint;
            double       _dbl;
            const char * _str;
            dbObject *   _dbobj;
            ZObject *    _zobj;
        } _value;
        
        Attr( const char * name );
        ~Attr();
        void clearValue();
    };
        
    const char *         _name;
    std::vector<Attr *>  _attrs;
    Attr * find( const char * attr );

    ZEvent & operator=( const ZEvent & e );
    ZEvent( const ZEvent & e);

  public:
    ZEvent( const char * name );
    ~ZEvent();

    // Add a new attribute value pair.
    // Return false, if the attr already exists
    void add( const char * attr, bool value );
    void add( const char * attr, char * value );
    void add( const char * attr, char value );
    void add( const char * attr, short value );
    void add( const char * attr, int value );
    void add( const char * attr, unsigned char value );
    void add( const char * attr, unsigned short value );
    void add( const char * attr, unsigned int value );
    void add( const char * attr, float value );
    void add( const char * attr, double value );
    void add( const char * attr, ZObject * value );
    void add( const char * attr, dbObject * value );
    void add( const char * attr, const adsRect & value );
    void add( const char * attr, adsPoint value );

    // Change an existing attribute value pair.
    // Return false, if the attr does not exists
    void change( const char * attr, bool value );
    void change( const char * attr, char * value );
    void change( const char * attr, char value );
    void change( const char * attr, short value );
    void change( const char * attr, int value );
    void change( const char * attr, unsigned char value );
    void change( const char * attr, unsigned short value );
    void change( const char * attr, unsigned int value );
    void change( const char * attr, float value );
    void change( const char * attr, double value );
    void change( const char * attr, ZObject * value );
    void change( const char * attr, dbObject * value );
    void change( const char * attr, const adsRect & value );
    void change( const char * attr, adsPoint value );

    friend class ZInterface;
};

/////////////////////////////////////////////////////
/// ZContext - Context the software is running in.
/////////////////////////////////////////////////////
class ZContext
{
  public:
    
    Tcl_Interp * _interp;
    ZSession *   _session;
};

//////////////////////////////////////////////
/// Interface to zroute software architecture
//////////////////////////////////////////////
class ZInterface
{
public:
    ZInterface();
    virtual ~ZInterface();
    
    //
    // Post an event. This method specifies the event as a series of attribute value pairs. There
    // must be a at least one attribute value pair. Furthermore the series of attribute value pairs must
    // be terminated with a zero. For examples:
    //    int x, y;
    //    ...
    //    z->event("foo", "x", Z_INT, x, "y", Z_INT, y, 0 );
    //
    // Throws ZException
    int event( const char * name, const char * attr1, ZValueType type, ... );

    //
    // Post an event. This method specifies the event as a series of attribute value pairs. There
    // must be a at least one attribute value pair. Furthermore the series of attribute value pairs must
    // be terminated with a zero. For examples:
    //    int x, y;
    //    ...
    //    z->event("foo", "x", "10", "y", "11", 0 );
    // Throws ZException
    int event( const char * name, const char * attr1, const char * val, ... );

    // Post an event.
    // Throws ZException
    int event( const ZEvent & e );

    // idle event
    int idle( int level);

    // ade event
    int ade( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // milos event
    int milos( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // warning message
    int warning( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // informational message
    int info( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // informational message
    int notice( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // verbose/debugging message
    int verbose( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // debug message
    //void debug(const char *mod, const char *tag, const char * fmt, ... ) ADS_FORMAT_PRINTF(4,5);

    // error, Throws a ZException
    void error( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // Get the name of this module
    virtual const char * getModuleName() { return ""; }

public:
    ZContext     _context;
};

} // namespace

#endif
