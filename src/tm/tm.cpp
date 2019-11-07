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

#include <stdarg.h>

#ifdef WIN32
#include <stdlib.h>
#include <io.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif

#define ADS_TM_BIND_HPP

#include <string>
#include <errno.h>
#include "db.h"
#include "tm.h"
#include "tm_bind.h"
#include "tm.hpp"
#include "tm_bind.hpp"
#include "ZNamespace.h"
#include "logger.h"

//#define TCL_EVAL 

namespace odb {

Tcl_SavedResult savedResult[128];
int curSavedResult = 0;

ZArgs::Arg::Arg()
{
    _name = NULL;
    _value = NULL;
    _type = ZARG_UNKNOWN;
    _set = false;
}

ZArgs::Arg::~Arg()
{
    if ( _name )
        free( (void *) _name );

    if ( _value )
        free( (void *) _value );
}
    
ZArgs::ZArgs()
{
}

ZArgs::ZArgs( ZContext & context )
{
    _context = context;
}


ZArgs::ZArgs( const ZArgs & args )
{
    std::vector<Arg *>::const_iterator itr;

    for( itr = args._args.begin(); itr != args._args.end(); ++itr )
    {
        const Arg * a = *itr;
        addArg(a->_name, a->_value, a->_type, a->_set);
    }

    _context = args._context;
}

ZArgs::ZArgs( const std::vector<ZArg> & args, ZContext & context, const char * tcl_vname, const char * tcl_sname )
{
    _context = context;
    std::vector<ZArg>::const_iterator itr;

    for ( itr = args.begin(); itr != args.end(); ++itr )
    {
        const ZArg & a = *itr;
        const char * value = Tcl_GetVar2( _context._interp, (char *) tcl_vname, a._name, 0 );
        bool is_set = false;
        
        if ( (!tcl_sname && value) || Tcl_GetVar2( _context._interp, (char *) tcl_sname, a._name, 0 ) )
            is_set = true;
            
        addArg( a._name, value, a._type, is_set );
    }
}

ZArgs::ZArgs( const std::vector<ZArg> & args, ZContext & context )
{
    _context = context;
    std::vector<ZArg>::const_iterator itr;

    for ( itr = args.begin(); itr != args.end(); ++itr )
    {
        const ZArg & a = *itr;
        addArg( a._name, a._default, a._type, false);
    }
}

ZArgs & ZArgs::operator=( const ZArgs & args )
{
    if ( &args == this )
        return *this;

    _args.clear();
    
    std::vector<Arg *>::const_iterator citr;
    for( citr = args._args.begin(); citr != args._args.end(); ++citr )
    {
        const Arg * a = *citr;
        addArg(a->_name, a->_value, a->_type, a->_set);
    }

    _context = args._context;
    return *this;
}

ZArgs::~ZArgs()
{
    std::vector<Arg *>::iterator itr;
    for( itr = _args.begin(); itr !=  _args.end(); ++itr )
        delete *itr;
}

void ZArgs::addArg( const char * name, const char * value, ZArgType type, bool is_set )
{
    Arg * a = new Arg();
    a->_name = strdup(name);
    ZALLOCATED(a->_name);
    a->_value = strdup(value);
    ZALLOCATED(a->_value);
    a->_type = type;
    a->_set = is_set;
    _args.push_back(a);
}


void ZArgs::setValue( int i, const char * s )
{
    if ( s )
    {
        Arg * a = _args[i];
        free( (void *) a->_value );
        a->_value = strdup(s);
        ZALLOCATED(a->_value);
    }
}

void ZArgs::setValue( int i, int n )
{
    Arg * a = _args[i];
    free( (void *) a->_value );
    char buf[16];
    snprintf(buf, 16, "%d", n);
    a->_value = strdup(buf);
    ZALLOCATED(a->_value);
}

void ZArgs::setValue( int i, double n )
{
    Arg * a = _args[i];
    free( (void *) a->_value );
    char buf[128];
    snprintf(buf, 128, "%g", n);
    a->_value = strdup(buf);
    ZALLOCATED(a->_value);
}

void ZArgs::setValue( int i, bool b )
{
    Arg * a = _args[i];
    free( (void *) a->_value );

    if ( b )
        a->_value = strdup("1");
    else
        a->_value = strdup("0");
    ZALLOCATED(a->_value);
}

void ZArgs::setValue( int i, const adsRect & r )
{
    Arg * a = _args[i];
    free( (void *) a->_value );
    char buf[128];
    snprintf(buf, 128, "%d %d %d %d", r.xMin(), r.yMin(), r.xMax(), r.yMax());
    a->_value = strdup(buf);
    ZALLOCATED(a->_value);
}

void ZArgs::setValue( int i, adsPoint p )
{
    Arg * a = _args[i];
    free( (void *) a->_value );
    char buf[128];
    snprintf(buf, 128, "%d %d", p.getX(), p.getY() );
    a->_value = strdup(buf);
    ZALLOCATED(a->_value);
}

bool ZArgs::isSet( int i ) const
{
    Arg * a = _args[i];
    return a->_set;
}

std::string ZArgs::getString( int i ) const
{
    ZASSERT( getType(i) == ZARG_STRING );
    return std::string( getValue(i) );
}

bool ZArgs::getBool( int i ) const
{
    ZASSERT( getType(i) == ZARG_BOOL );
    const char * s = getValue(i);
    return s[0] == '1';
}

int ZArgs::getInt( int i ) const
{
    ZASSERT( getType(i) == ZARG_INT );
    return atoi( getValue(i) );
}

double ZArgs::getFloat( int i ) const
{
    ZASSERT( getType(i) == ZARG_FLOAT );
    return atof( getValue(i) );
}

ZObject * ZArgs::getZObject( int i ) const
{
    ZASSERT( getType(i) == ZARG_ZOBJECT );
    const char * s = getValue(i);

    if( *s == '\0' )
        return NULL;
    
    return _context._session->_ns->resolveZObject(s);
}

dbObject * ZArgs::getDbObject( int i ) const
{
    ZASSERT( getType(i) == ZARG_DBOBJECT );

    const char * s = getValue(i);

    if( *s == '\0' )
        return NULL;

    return _context._session->_ns->resolveDB(s);
}

const adsRect *ZArgs::getRect( int i ) const
{
    ZASSERT( getType(i) == ZARG_RECT );
    const char * s = getValue(i);
    static adsRect rtrct;
    int x1,y1,x2,y2;
    if ( sscanf( s, "%d %d %d %d", &x1, &y1, &x2, &y2 ) == 4)
      {
        rtrct.reset(x1,y1,x2,y2);
        return &rtrct;
      }

    return NULL;
}

const adsPoint *ZArgs::getPoint( int i ) const
{
    ZASSERT( getType(i) == ZARG_POINT );
    const char * s = getValue(i);
    static adsPoint rtrct;
    int x1,y1;
    if ( sscanf( s, "%d %d", &x1, &y1) == 2)
      {
        rtrct.setX(x1);
        rtrct.setY(y1);
        return &rtrct;
      }

    return NULL;
}

static FILE * getOpenFile( Tcl_Interp * interp, const char * channel_name, int mode )
{
    const char * fmode;
    
    if ( mode == TCL_READABLE )
        fmode = "r";
    else
        fmode = "w";

    Tcl_Channel channel;
    channel = Tcl_GetChannel( interp, channel_name, &mode );

    if ( channel == NULL )
        throw ZException( "failed to get Tcl_Channel for %s", channel_name);

    int fd;
    if ( Tcl_GetChannelHandle( channel, mode, (ClientData*) &fd ) != TCL_OK )
        throw ZException( "failed to get Tcl_Channel handle for %s", channel_name);

    int dfd = dup( fd );
/*
    //int dfd = fcntl( fd, F_DUPFD, 0 );
    int dfd;
	dup2(fd, dfd);
*/
    if ( dfd < 0 )
        throw ZException( "dup failed on fd (%d), because %s", fd, strerror(errno) );

    FILE * file = fdopen( dfd, fmode );

    if ( file == NULL )
        throw ZException( "fdopen failed on fd (%d), because %s", dfd, strerror(errno) );
    return file;
}

FILE * ZArgs::getIChannel( int i ) const
{
    const char * s = getValue(i);

    if( *s == '\0' )
        return NULL;

    int mode = TCL_READABLE;
    FILE * file = getOpenFile( _context._interp, s, mode );
    return file;
}

FILE * ZArgs::getOChannel( int i ) const
{
    const char * s = getValue(i);

    if( *s == '\0' )
        return NULL;

    int mode = TCL_WRITABLE;
    FILE * file = getOpenFile( _context._interp, s, mode );
    return file;
}

void ZArgs::getString( int i, std::list<std::string> & l ) const
{
    int argc;
    const char ** argv;
    if ( Tcl_SplitList( _context._interp, getValue(i), &argc, &argv ) != TCL_OK )
        throw ZException("Tcl_SplitList failed");

    int j;
    for( j = 0; j < argc; ++j )
        l.push_back( std::string( argv[j] ) );

    Tcl_Free( (char *) argv );
}

void ZArgs::getBool( int i, std::list<bool> & l ) const
{
    int argc;
    const char ** argv;
    if ( Tcl_SplitList( _context._interp, getValue(i), &argc, &argv ) != TCL_OK )
        throw ZException("Tcl_SplitList failed");

    int j;
    for( j = 0; j < argc; ++j )
        l.push_back( argv[j][0] == '1' );

    Tcl_Free( (char *) argv );
}

void ZArgs::getInt( int i, std::list<int> & l ) const
{
    int argc;
    const char ** argv;
    if ( Tcl_SplitList( _context._interp, getValue(i), &argc, &argv ) != TCL_OK )
        throw ZException("Tcl_SplitList failed");

    int j;
    for( j = 0; j < argc; ++j )
        l.push_back( atoi(argv[j]) );
    
    Tcl_Free( (char *) argv );
}

void ZArgs::getFloat( int i, std::list<double> & l ) const
{
    int argc;
    const char ** argv;
    if ( Tcl_SplitList( _context._interp, getValue(i), &argc, &argv ) != TCL_OK )
        throw ZException("Tcl_SplitList failed");

    int j;
    for( j = 0; j < argc; ++j )
        l.push_back( atof(argv[j]) );
    
    Tcl_Free( (char *) argv );
}

void ZArgs::getZObject( int i, std::list<ZObject *> & l ) const
{
    int argc;
    const char ** argv;
    if ( Tcl_SplitList( _context._interp, getValue(i), &argc, &argv ) != TCL_OK )
        throw ZException("Tcl_SplitList failed");

    int j;
    for( j = 0; j < argc; ++j )
    {
        const char * s = argv[j];

        if( *s == '\0' )
            continue;

        l.push_back( _context._session->_ns->resolveZObject( argv[j] ) );
    }
    
    
    Tcl_Free( (char *) argv );
}

void ZArgs::getDbObject( int i, std::list<dbObject *> & l ) const
{
    int argc;
    const char ** argv;
    if ( Tcl_SplitList( _context._interp, getValue(i), &argc, &argv ) != TCL_OK )
        throw ZException("Tcl_SplitList failed");

    int j;
    for( j = 0; j < argc; ++j )
    {
        const char * s = argv[j];

        if( *s == '\0' )
            continue;

        l.push_back( _context._session->_ns->resolveDB( argv[j] ) );
    }
    
    Tcl_Free( (char *) argv );
}

void ZArgs::getIChannel( int i, std::list<FILE *> & l ) const
{
    int argc;
    const char ** argv;
    if ( Tcl_SplitList( _context._interp, getValue(i), &argc, &argv ) != TCL_OK )
        throw ZException("Tcl_SplitList failed");

    int j;
    for( j = 0; j < argc; ++j )
    {
        if ( argv[j][0] == '\0' )
        {
            FILE * file;
            int mode = TCL_READABLE;
            file = getOpenFile( _context._interp, argv[j], mode );
            l.push_back( file );
      }
    }
    Tcl_Free( (char *) argv );
}

void ZArgs::getOChannel( int i, std::list<FILE *> & l ) const
{
    int argc;
    const char ** argv;
    if ( Tcl_SplitList( _context._interp, getValue(i), &argc, &argv ) != TCL_OK )
        throw ZException("Tcl_SplitList failed");

    int j;
    for( j = 0; j < argc; ++j )
    {
        if ( argv[j][0] == '\0' )
        {
            FILE * file;
            int mode = TCL_WRITABLE;
            file = getOpenFile( _context._interp, argv[j], mode );
            l.push_back( file );
        }
    }
    Tcl_Free( (char *) argv );
}

void ZArgs::getRect( int i, std::list<adsRect> & l ) const
{
    int argc;
    const char ** argv;
    if ( Tcl_SplitList( _context._interp, getValue(i), &argc, &argv ) != TCL_OK )
        throw ZException("Tcl_SplitList failed");

    int j;
    for( j = 0; j < argc; ++j )
    {
        const char * r = argv[j];
        int x1, y1, x2, y2;
        sscanf( r, "%d %d %d %d", &x1, &y1, &x2, &y2 );
        l.push_back( adsRect(x1,y1,x2,y2) );
    }
    
    Tcl_Free( (char *) argv );
}

void ZArgs::getPoint( int i, std::list<adsPoint> & l ) const
{
    int argc;
    const char ** argv;
    if ( Tcl_SplitList( _context._interp, getValue(i), &argc, &argv ) != TCL_OK )
        throw ZException("Tcl_SplitList failed");

    int j;
    for( j = 0; j < argc; ++j )
    {
        const char * p = argv[j];
        int x, y;
        sscanf( p, "%d %d", &x, &y);
        l.push_back( adsPoint(x,y) );
    }
    
    Tcl_Free( (char *) argv );
}

void ZArgs::setString( int i, const char * s )
{
    setValue(i,s);
}

void ZArgs::setString( int i, const std::string & s )
{
    setValue(i,s.c_str());
}

void ZArgs::setBool( int i, bool b )
{
    setValue(i, b);
}

void ZArgs::setInt( int i, int n )
{
    setValue(i, n);
}

void ZArgs::setFloat( int i, double n )
{
    setValue(i, n);
}

void ZArgs::setZObject( int i, ZObject * z )
{
    if ( z == NULL )
        setValue(i, "" );
    else
        setValue(i, _context._session->_ns->addZObject(z) );
}

void ZArgs::setDbObject( int i, dbObject * d )
{
    if ( d == NULL )
        setValue(i, "" );
    else
    {
        char name[256];
        d->getDbName(name);
        setValue(i, name);
    }
}

void ZArgs::setRect( int i, const adsRect & r )
{
    setValue(i, r);
}

void ZArgs::setPoint( int i, adsPoint p )
{
    setValue(i, p);
}

void ZArgs::setString( int i, const std::list<std::string> & l )
{
    std::list<std::string>::const_iterator itr;
    std::string s;
    s.append("{");
    for( itr = l.begin(); itr != l.end(); ++itr )
    {
        const std::string & e = *itr;
        s.append(" {");
        s.append(e);
        s.append("}");
    }
    s.append(" }");
    setValue(i, s.c_str());
}

void ZArgs::setBool( int i, const std::list<bool> & l )
{
    std::list<bool>::const_iterator itr;
    std::string s;
    s.append("{");
    for( itr = l.begin(); itr != l.end(); ++itr )
    {
        bool e = *itr;
        if ( e )
            s.append(" 1");
        else
            s.append(" 0");
    }
    s.append(" }");
    setValue(i, s.c_str());
}

void ZArgs::setInt( int i, const std::list<int> & l )
{
    std::list<int>::const_iterator itr;
    std::string s;
    s.append("{");
    for( itr = l.begin(); itr != l.end(); ++itr )
    {
        int e = *itr;
        char buf[16];
        snprintf(buf, 16, " %d", e );
        s.append(buf);
    }
    s.append(" }");
    setValue(i, s.c_str());
}

void ZArgs::setFloat( int i, const std::list<double> & l )
{
    std::list<double>::const_iterator itr;
    std::string s;
    s.append("{");
    for( itr = l.begin(); itr != l.end(); ++itr )
    {
        double e = *itr;
        char buf[128];
        snprintf(buf, 128, " %g", e );
        s.append(buf);
    }
    s.append(" }");
    setValue(i, s.c_str());
}

void ZArgs::setRect( int i, const std::list<adsRect> & l )
{
    std::list<adsRect>::const_iterator itr;
    std::string s;
    s.append("{");
    for( itr = l.begin(); itr != l.end(); ++itr )
    {
        const adsRect & r = *itr;
        char buf[128];
        snprintf(buf, 128, " \"%d %d %d %d\"", r.xMin(), r.xMin(), r.xMax(), r.yMax() );
        s.append(buf);
    }
    s.append(" }");
    setValue(i, s.c_str());
}

void ZArgs::setPoint( int i, const std::list<adsPoint> & l )
{
    std::list<adsPoint>::const_iterator itr;
    std::string s;
    s.append("{");
    for( itr = l.begin(); itr != l.end(); ++itr )
    {
        adsPoint p = *itr;
        char buf[128];
        snprintf(buf, 128, " \"%d %d\"", p.getX(), p.getY() );
        s.append(buf);
    }
    s.append(" }");
    setValue(i, s.c_str());
}

void ZArgs::setZObject( int i, const std::list<ZObject *> & l )
{
    std::list<ZObject *>::const_iterator itr;
    std::string s;
    s.append("{");
    for( itr = l.begin(); itr != l.end(); ++itr )
    {
        ZObject * e = *itr;
        if ( e )
        {
            s.append(" ");
            s.append( _context._session->_ns->addZObject(e) );
        }
    }
    s.append(" }");
    setValue(i, s.c_str());
}

void ZArgs::setDbObject( int i, const std::list<dbObject *> & l )
{
    std::list<dbObject *>::const_iterator itr;
    std::string s;
    s.append("{");
    for( itr = l.begin(); itr != l.end(); ++itr )
    {
        dbObject * e = *itr;
        if ( e )
        {
            char name[256];
            e->getDbName(name);
            s.append(" ");
            s.append(name);
        }
    }
    s.append(" }");
    setValue(i, s.c_str());
}

ZEvent::Attr::Attr( const char * name )
{
    _name = strdup(name);
    ZALLOCATED(_name);
}

ZEvent::Attr::~Attr()
{
    if ( _name )
        free( (void *) _name );

    if ( (_type == Z_STRING) && (_value._str != NULL) )
        free( (void *) _value._str );
}

void ZEvent::Attr::clearValue()
{
    if ( (_type == Z_STRING) && (_value._str != NULL) )
        free( (void *) _value._str );
}

ZEvent::ZEvent( const char * name )
{
    _name = strdup(name);
    ZALLOCATED(_name);
}

ZEvent::~ZEvent()
{
    if ( _name )
        free( (void *) _name );

    std::vector<Attr *>::iterator itr;

    for( itr = _attrs.begin(); itr != _attrs.end(); ++itr )
        delete *itr;
}

ZEvent::Attr * ZEvent::find( const char * attr )
{
    std::vector<Attr *>::iterator itr;

    for( itr = _attrs.begin(); itr != _attrs.end(); ++itr )
    {
        Attr * a = *itr;

        if ( strcmp(a->_name, attr) == 0 )
            return a;
    }

    return NULL;
}

void ZEvent::add( const char * attr, bool value )
{
    ZASSERT( find(attr) != NULL );
    Attr * a = new Attr(attr);
    ZALLOCATED(a);
    a->_type = Z_BOOL;
    a->_value._int = value;
    _attrs.push_back(a);
}

void ZEvent::add( const char * attr, char * value )
{
    ZASSERT( find(attr) != NULL );
    Attr * a = new Attr(attr);
    ZALLOCATED(a);
    a->_type = Z_STRING;
    a->_value._str = strdup(value);
    ZALLOCATED(a->_value._str);
    _attrs.push_back(a);
}

void ZEvent::add( const char * attr, char value )
{
    ZASSERT( find(attr) != NULL );
    Attr * a = new Attr(attr);
    ZALLOCATED(a);
    a->_type = Z_CHAR;
    a->_value._int = value;
    _attrs.push_back(a);
}

void ZEvent::add( const char * attr, short value )
{
    ZASSERT( find(attr) != NULL );
    Attr * a = new Attr(attr);
    ZALLOCATED(a);
    a->_type = Z_SHORT;
    a->_value._int = value;
    _attrs.push_back(a);
}

void ZEvent::add( const char * attr, int value )
{
    ZASSERT( find(attr) != NULL );
    Attr * a = new Attr(attr);
    ZALLOCATED(a);
    a->_type = Z_INT;
    a->_value._int = value;
    _attrs.push_back(a);
}

void ZEvent::add( const char * attr, unsigned char value )
{
    ZASSERT( find(attr) != NULL );
    Attr * a = new Attr(attr);
    ZALLOCATED(a);
    a->_type = Z_UCHAR;
    a->_value._int = value;
    _attrs.push_back(a);
}

void ZEvent::add( const char * attr, unsigned short value )
{
    ZASSERT( find(attr) != NULL );
    Attr * a = new Attr(attr);
    ZALLOCATED(a);
    a->_type = Z_USHORT;
    a->_value._int = value;
    _attrs.push_back(a);
}

void ZEvent::add( const char * attr, unsigned int value )
{
    ZASSERT( find(attr) != NULL );
    Attr * a = new Attr(attr);
    ZALLOCATED(a);
    a->_type = Z_UINT;
    a->_value._uint = value;
    _attrs.push_back(a);
}

void ZEvent::add( const char * attr, float value )
{
    ZASSERT( find(attr) != NULL );
    Attr * a = new Attr(attr);
    ZALLOCATED(a);
    a->_type = Z_FLOAT;
    a->_value._dbl = value;
    _attrs.push_back(a);
}

void ZEvent::add( const char * attr, double value )
{
    ZASSERT( find(attr) != NULL );
    Attr * a = new Attr(attr);
    ZALLOCATED(a);
    a->_type = Z_DOUBLE;
    a->_value._dbl = value;
    _attrs.push_back(a);
}

void ZEvent::add( const char * attr, ZObject * value )
{
    ZASSERT( find(attr) != NULL );
    Attr * a = new Attr(attr);
    ZALLOCATED(a);
    a->_type = Z_ZOBJECT;
    a->_value._zobj = value;
    _attrs.push_back(a);
}

void ZEvent::add( const char * attr, dbObject * value )
{
    ZASSERT( find(attr) != NULL );
    Attr * a = new Attr(attr);
    ZALLOCATED(a);
    a->_type = Z_DBOBJECT;
    a->_value._dbobj = value;
    _attrs.push_back(a);
}

void ZEvent::change( const char * attr, bool value )
{
    Attr * a = find(attr);
    ZASSERT(a);
    a->_type = Z_BOOL;
    a->_value._int = value;
}

void ZEvent::change( const char * attr, char * value )
{
    Attr * a = find(attr);
    ZASSERT(a);
    a->clearValue();
    a->_type = Z_STRING;
    a->_value._str = strdup(value);
    ZALLOCATED(a->_value._str);
}

void ZEvent::change( const char * attr, char value )
{
    Attr * a = find(attr);
    ZASSERT(a);
    a->clearValue();
    a->_type = Z_CHAR;
    a->_value._int = value;
}

void ZEvent::change( const char * attr, short value )
{
    Attr * a = find(attr);
    ZASSERT(a);
    a->clearValue();
    a->_type = Z_SHORT;
    a->_value._int = value;
}

void ZEvent::change( const char * attr, int value )
{
    Attr * a = find(attr);
    ZASSERT(a);
    a->clearValue();
    a->_type = Z_INT;
    a->_value._int = value;
}

void ZEvent::change( const char * attr, unsigned char value )
{
    Attr * a = find(attr);
    ZASSERT(a);
    a->clearValue();
    a->_type = Z_UCHAR;
    a->_value._int = value;
}

void ZEvent::change( const char * attr, unsigned short value )
{
    Attr * a = find(attr);
    ZASSERT(a);
    a->clearValue();
    a->_type = Z_USHORT;
    a->_value._int = value;
}

void ZEvent::change( const char * attr, unsigned int value )
{
    Attr * a = find(attr);
    ZASSERT(a);
    a->clearValue();
    a->_type = Z_UINT;
    a->_value._uint = value;
}

void ZEvent::change( const char * attr, float value )
{
    Attr * a = find(attr);
    ZASSERT(a);
    a->clearValue();
    a->_type = Z_FLOAT;
    a->_value._dbl = value;
}

void ZEvent::change( const char * attr, double value )
{
    Attr * a = find(attr);
    ZASSERT(a);
    a->clearValue();
    a->_type = Z_DOUBLE;
    a->_value._dbl = value;
}

void ZEvent::change( const char * attr, ZObject * value )
{
    Attr * a = find(attr);
    ZASSERT(a);
    a->clearValue();
    a->_type = Z_ZOBJECT;
    a->_value._zobj = value;
}

void ZEvent::change( const char * attr, dbObject * value )
{
    Attr * a = find(attr);
    ZASSERT(a);
    a->clearValue();
    a->_type = Z_DBOBJECT;
    a->_value._dbobj = value;
}

ZInterface::ZInterface()
{
}

ZInterface::~ZInterface()
{
}

int ZInterface::event( const char * name, const char * attr1, const char * val1, ... )
{
    const char * attr = attr1;
    const char * val = val1;
    std::string event_str;
    event_str = "zevent";
    event_str.append(" ");
    event_str.append( name );
    
    va_list args;
    va_start(args, val1);

    for( ;; )
    {
        event_str.append(" ");
        event_str.append(attr);
        event_str.append(" {\"");
        event_str.append(val);
        event_str.append("\"}");

        attr = va_arg(args, const char *);

        if ( attr == NULL )
            break;

        val = va_arg(args, const char *);

        if ( val == NULL )
            error(0, "Missing value argument to ZInterface::event()");
    }
    
    va_end(args);
    //fprintf(stderr,"Event1: %s\n",event_str.c_str() );


    Tcl_SaveResult(_context._interp,&savedResult[curSavedResult++]);

#ifdef TCL_EVAL
    fprintf(stderr,"TCL EVAL 1: %s\n",(char*)event_str.c_str());
#endif
    int rc = Tcl_Eval( _context._interp, (char *) event_str.c_str() );
#ifdef TCL_EVAL
    fprintf(stderr,"TCL EVAL 1 (%s) result: %d\n",event_str.c_str(),rc);
#endif
    if ( rc != TCL_OK )
    {
#ifdef TCL_EVAL
        fprintf(stderr,"TCL EVAL 1b error branch!\n");
#endif
        if ( strcasecmp(name,"error") == 0 ) 
        {
#ifdef TCL_EVAL
            fprintf(stderr,"TCL EVAL 1b error branch 1!\n");
#endif
            error(0, "error");
        }
        else
        {
#ifdef TCL_EVAL
            fprintf(stderr,"TCL EVAL 1b error branch 2!\n");
#endif
            error(0, "TCL evaluation 1b failed on (%s - %d).\n", event_str.c_str(),rc );
        }
    }

    const char * result = Tcl_GetStringResult(_context._interp );
    int x = (int)strtol(result,NULL,10);

    Tcl_RestoreResult(_context._interp,&savedResult[--curSavedResult]);

    return x;
}

int ZInterface::event( const char * name, const char * attr1, int type, ... )
{
    const char * attr = attr1;
    std::string event_str;
    event_str = "zevent";
    event_str.append(" ");
    event_str.append( name );
    
    va_list args;
    va_start(args, type);

    for( ;; )
    {
        event_str.append(" ");
        event_str.append(attr);
        event_str.append(" {");

        switch( type )
        {
            case Z_CHAR:
            case Z_UCHAR:
            case Z_SHORT:
            case Z_USHORT:
            case Z_INT:
            {
                int val = va_arg(args, int);
                char buffer[16];
                snprintf( buffer, 16, "%d", val);
                event_str.append(buffer);
                break;
            }
            
            case Z_UINT:
            {
                unsigned int val = va_arg(args, unsigned int);
                char buffer[16];
                snprintf( buffer, 16, "%u", val);
                event_str.append(buffer);
                break;
            }
            
            case Z_FLOAT:
            case Z_DOUBLE:
            {
                double val = va_arg(args, double);
                char buffer[128];
                snprintf( buffer, 128, "%g", val);
                event_str.append(buffer);
                break;
            }
            
            case Z_STRING:
            {
                char * val = va_arg(args, char *);
                event_str.append(val);
                break;
            }
            
            case Z_BOOL:
            {
                int val = va_arg(args, int);

                if ( val )
                    event_str.append("1");
                else
                    event_str.append("0");
                break;
            }
            
            case Z_ZOBJECT:
            {
                ZObject * val = va_arg(args, ZObject *);
                const char * name = _context._session->_ns->addZObject(val);
                event_str.append(name);
                break;
            }
            
            case Z_DBOBJECT:
            {
                dbObject * val = va_arg(args, dbObject *);
                char buffer[256];
                val->getDbName(buffer);
                event_str.append(buffer);
                break;
            }

            default:
                ZASSERT(0);
        }
        
        event_str.append("}");

        attr = va_arg(args, const char *);

        if ( attr == NULL )
            break;

        type = (ZValueType) va_arg(args, int);
    }
    
    va_end(args);
    //fprintf(stderr,"Event2: %s\n",event_str.c_str() );

    Tcl_SaveResult(_context._interp,&savedResult[curSavedResult++]);

#ifdef TCL_EVAL
    fprintf(stderr,"TCL EVAL 2: %s\n",(char*)event_str.c_str());
#endif
    int rc = Tcl_Eval( _context._interp, (char *) event_str.c_str() );
#ifdef TCL_EVAL
    fprintf(stderr,"TCL EVAL 2 result: %d\n",rc);
#endif
    if ( rc != TCL_OK )
        error(0, "TCL evaluation 2 failed on (%s - %d).\n", event_str.c_str(),rc );

    const char * result = Tcl_GetStringResult(_context._interp );
    int x = (int)strtol(result,NULL,10);

    Tcl_RestoreResult(_context._interp,&savedResult[--curSavedResult]);

    return x;
}

int ZInterface::event( const ZEvent & event )
{
    std::string event_str;
    event_str = "zevent";
    event_str.append(" ");
    event_str.append( event._name );
    
    std::vector<ZEvent::Attr *>::const_iterator itr;
    
    for( itr = event._attrs.begin(); itr != event._attrs.end(); ++itr )
    {
        ZEvent::Attr * attr = *itr;
        event_str.append(" ");
        event_str.append(attr->_name);
        event_str.append(" {");

        switch( attr->_type )
        {
            case Z_CHAR:
            case Z_UCHAR:
            case Z_SHORT:
            case Z_USHORT:
            case Z_INT:
            {
                char buffer[16];
                snprintf( buffer, 16, "%d", attr->_value._int);
                event_str.append(buffer);
                break;
            }
            
            case Z_UINT:
            {
                char buffer[16];
                snprintf( buffer, 16, "%u", attr->_value._uint);
                event_str.append(buffer);
                break;
            }
            
            case Z_FLOAT:
            case Z_DOUBLE:
            {
                char buffer[128];
                snprintf( buffer, 128, "%g", attr->_value._dbl);
                event_str.append(buffer);
                break;
            }
            
            case Z_STRING:
            {
                event_str.append(attr->_value._str);
                break;
            }
            
            case Z_BOOL:
            {
                if ( attr->_value._int )
                    event_str.append("1");
                else
                    event_str.append("0");
                break;
            }
            
            case Z_ZOBJECT:
            {
                const char * name = _context._session->_ns->addZObject(attr->_value._zobj);
                event_str.append(name);
                break;
            }
            
            case Z_DBOBJECT:
            {
                char buffer[256];
                attr->_value._dbobj->getDbName(buffer);
                event_str.append(buffer);
                break;
            }

            default:
                ZASSERT(0);
        }
        
        event_str.append("}");
    }
    
    //fprintf(stderr,"Event3: %s\n",event_str.c_str() );
    Tcl_SaveResult(_context._interp,&savedResult[curSavedResult++]);

#ifdef TCL_EVAL
    fprintf(stderr,"TCL EVAL 3: %s\n",(char*)event_str.c_str());
#endif
    int rc = Tcl_Eval( _context._interp, (char *) event_str.c_str() );
#ifdef TCL_EVAL
    fprintf(stderr,"TCL EVAL 3 result: %d\n",rc);
#endif
    if ( rc != TCL_OK )
        error(0, "TCL evaluation 3 failed on (%s - %d).\n", event_str.c_str(),rc );

    const char * result = Tcl_GetStringResult(_context._interp );
    int x = (int)strtol(result,NULL,10);

    Tcl_RestoreResult(_context._interp,&savedResult[--curSavedResult]);

    return x;
}

int ZInterface::idle( int level = 0)
{
    char idle_id[128];
    snprintf(idle_id, 128, "%s:%d", getModuleName(), level );
    return event( "idle", idle_id, "", 0);
}

/* ********************************************************************
   ********************************************************************
                                  IMPORTANT!!!
             If you change the methods below, update logger.cpp to 
              reflect the same behavior, or there will be trouble.
   ********************************************************************
   ******************************************************************** */

int ZInterface::milos( int code, const char *msg, ... )
{
    char buffer[8192];
    va_list args;
    va_start(args, msg);
    vsnprintf(buffer, 8192, msg, args);
    va_end(args);
    char milos_id[128];
    snprintf(milos_id, 128, "%s:%d", getModuleName(), code );
    return event( "milos", milos_id, buffer, 0 );
}

int ZInterface::ade( int code, const char *msg, ... )
{
    char buffer[8192];
    va_list args;
    va_start(args, msg);
    vsnprintf(buffer, 8192, msg, args);
    va_end(args);
    char ade_id[128];
    snprintf(ade_id, 128, "%s:%d", getModuleName(), code );
    return event( "ade", ade_id, buffer, 0 );
}



int ZInterface::warning( int code, const char * msg, ... )
{
    if( checkWarning(msg) == 1 )
        return TCL_OK;

    char buffer[8192];
    va_list args;
    va_start(args, msg);
    vsnprintf(buffer, 8192, msg, args);
    va_end(args);
    char warning_id[128];
    snprintf(warning_id, 128, "%s:%d", getModuleName(), code );
    return event( "warning", warning_id, buffer, 0 );
}

int ZInterface::info( int code, const char * msg, ... )
{
    if( checkWarning(msg) == 1 )
        return TCL_OK;

    char buffer[8192];
    va_list args;
    va_start(args, msg);
    vsnprintf(buffer, 8192, msg, args);
    va_end(args);
    char info_id[128];
    snprintf(info_id, 128, "%s:%d", getModuleName(), code );
    return event( "info", info_id, buffer, 0 );
}


int ZInterface::notice( int code, const char * msg, ... )
{
    char buffer[8192];
    va_list args;
    va_start(args, msg);
    vsnprintf(buffer, 8192, msg, args);
    va_end(args);
    char notice_id[128];
    snprintf(notice_id, 128, "%s:%d", getModuleName(), code );
    return event( "notice", notice_id, buffer, 0 );
}

extern int verboseStatus;

int ZInterface::verbose( int code, const char * msg, ... )
{
    if( verboseStatus != 1 ) return TCL_OK;

    char buffer[8192];
    va_list args;
    va_start(args, msg);
    vsnprintf(buffer, 8192, msg, args);
    va_end(args);
    char verbose_id[128];
    snprintf(verbose_id, 128, "%s:%d", getModuleName(), code );
    return event( "verbose", verbose_id, buffer, 0 );
}

void ZInterface::error( int code, const char * msg, ... )
{
    char buffer[8192];
    va_list args;
    va_start(args, msg);
    vsnprintf(buffer, 8192, msg, args);
    va_end(args);
    //char error_id[128];
    //snprintf(error_id, 128, "%s:%d", getModuleName(), code );
    //event( "error", error_id, buffer, 0 );
    //debug("ERROR","A","error contents: %s\n",buffer);
    throw( ZException("%s", buffer ) );
}

}
