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

#ifndef ADS_GEOM_H
#define ADS_GEOM_H

#include "ads.h"

namespace odb {

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

class dbIStream;
class dbOStream;

class adsPoint
{
    int _x;
    int _y;

  public:
    adsPoint();
    adsPoint( const adsPoint & p );
    adsPoint( int x, int y );

    adsPoint & operator=( const adsPoint & p );
    bool operator==( const adsPoint & p ) const;
    bool operator!=( const adsPoint & p ) const;
    bool operator<( const adsPoint & p ) const;

    int getX() const;
    int getY() const;
    void setX( int x );
    void setY( int y );

    void rotate90();
    void rotate180();
    void rotate270();

    int & x() { return _x; }
    int & y() { return _y; }
    const int & x() const { return _x; }
    const int & y() const { return _y; }

    // compute cross product of the vectors <p0,p1> and <p0,p2>
    //
    //      p2
    //      +
    //      ^
    //      |
    //      | crossProduct(p0,p1,p2) > 0
    //      |
    //      +------------>+
    //     p0            p1
    //
    //      p1
    //      +
    //      ^
    //      |
    //      | crossProduct(p0,p1,p2) < 0
    //      |
    //      +------------>+
    //     p0            p2
    //
    // Returns 0 if the vectors are colinear
    // Returns > 0 if the vectors rotate counter clockwise
    // Returns < 0 if the vectors rotate clockwise
    static int64 crossProduct( adsPoint p0, adsPoint p1, adsPoint p2 );

    // compute the rotation direction of the vectors <p0,p1> and <p0,p2>
    // Returns 0 if the vectors are colinear
    // Returns 1 if the vectors rotate counter clockwise
    // Returns -1 if the vectors rotate clockwise
    //
    enum Rotation { COLINEAR=0, CW=-1, CCW=1 };
    static int rotation( adsPoint p0, adsPoint p1, adsPoint p2 );

    // compute the square distance between two points
    static uint64 squaredDistance( adsPoint p0, adsPoint p1 );

    // compute the manhattan distance between two points
    static uint64 manhattanDistance( adsPoint p0, adsPoint p1 );

    friend dbIStream & operator>>( dbIStream & stream, adsPoint & p );
    friend dbOStream & operator<<( dbOStream & stream, const adsPoint & p );
};

class adsRect
{
    int _xlo;
    int _ylo;
    int _xhi;
    int _yhi;

  public:

    adsRect();
    adsRect(const adsRect & r);
    adsRect(const adsPoint p1, const adsPoint p2 );
    adsRect(int x1, int y1, int x2, int y2);

    adsRect & operator=( const adsRect & r );
    bool operator==( const adsRect & r ) const;
    bool operator!=( const adsRect & r ) const;
    bool operator<( const adsRect & r ) const;
    
    // Reinitialize the rectangle
    void init(int x1, int y1, int x2, int y2);

    // Reinitialize the rectangle without normalization
    void reset(int x1, int y1, int x2, int y2);

    // Moves the rectangle to the new point.
    void moveTo(int x, int y);

    // Moves the rectangle by the offset amount
    void moveDelta(int dx, int dy);

    // Set the coordinates to: min(INT_MAX, INT_MAX) max(INT_MIN, INT_MIN)
    void mergeInit();

    uint minDXDY();
    uint maxDXDY();
    int getDir();

    void set_xlo(int x1);
    void set_xhi(int x1);
    void set_ylo(int x1);
    void set_yhi(int x1);

    int xMin() const;
    int yMin() const;
    int xMax() const;
    int yMax() const;
    uint dx() const;
    uint dy() const;
    adsPoint ll() const;
    adsPoint ul() const;
    adsPoint ur() const;
    adsPoint lr() const;

    // Returns the lower point (lower-left)
    adsPoint low() const;

    // Returns the upper point (upper-right)
    adsPoint high() const;

    // A point intersects any part of this rectangle.
    bool intersects( const adsPoint & p ) const;

    // A rectangle intersects any part of this rectangle.
    bool intersects( const adsRect & r ) const;

    // A point intersects the interior of this rectangle
    bool overlaps( const adsPoint & p ) const;

    // A rectangle intersects the interior of this rectangle
    bool overlaps( const adsRect & r ) const;

    //  A rectangle is contained in the interior of this rectangle
    bool contains( const adsRect & r ) const;

    //  A rectangle is completely contained in the interior of this rectangle,
    bool inside( const adsRect & r ) const;

    // Compute the union of these two rectangles.
    void merge( const adsRect & r, adsRect & result );

    // Compute the union of these two rectangles. The result is stored in this
    // rectangle.
    void merge( const adsRect & r );

    // Compute the intersection of these two rectangles.
    void intersection( const adsRect & r, adsRect & result );

    // Compute the intersection of these two rectangles.
    adsRect intersect( const adsRect & r );

    uint64 area();
    uint64 margin();

    void notice(const char * prefix="");
    void printf(FILE *fp, const char * prefix="");
    void print(const char * prefix="");

    friend dbIStream & operator>>( dbIStream & stream, adsRect & r );
    friend dbOStream & operator<<( dbOStream & stream, const adsRect & r );
};

inline adsPoint::adsPoint() 
{
    _x = 0;
    _y = 0;
}

inline adsPoint::adsPoint( const adsPoint & p ) 
{
    _x = p._x;
    _y = p._y;
}

inline adsPoint::adsPoint( int x, int y ) 
{
    _x = x;
    _y = y;
}

inline adsPoint & adsPoint::operator=( const adsPoint & p )
{
    _x = p._x;
    _y = p._y;
    return *this;
}

inline bool adsPoint::operator==( const adsPoint & p ) const
{
    return (_x == p._x) && (_y == p._y);
}

inline bool adsPoint::operator!=( const adsPoint & p ) const
{
    return (_x != p._x) || (_y != p._y);
}

inline int adsPoint::getX() const
{
    return _x;
}

inline int adsPoint::getY() const
{
    return _y;
}

inline void adsPoint::setX( int x ) 
{
    _x = x;
}

inline void adsPoint::setY( int y ) 
{
    _y = y;
}

inline void adsPoint::rotate90()
{
    int xp = -_y;
    int yp = _x;
    _x = xp;
    _y = yp;
}

inline void adsPoint::rotate180()
{
    int xp = -_x;
    int yp = -_y;
    _x = xp;
    _y = yp;
}

inline void adsPoint::rotate270()
{
    int xp = _y;
    int yp = -_x;
    _x = xp;
    _y = yp;
}  

inline int64 adsPoint::crossProduct( adsPoint p0, adsPoint p1, adsPoint p2 )
{
    // because the cross-product might overflow in an "int"
    // 64-bit arithmetic is used here
    int64 x0 = p0._x;
    int64 x1 = p1._x;
    int64 x2 = p2._x;
    int64 y0 = p0._y;
    int64 y1 = p1._y;
    int64 y2 = p2._y;
    return (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
}

inline int adsPoint::rotation( adsPoint p0, adsPoint p1, adsPoint p2 )
{
    int64 cp = crossProduct(p0, p1, p2);
    return ( cp == 0 ? 0 : cp < 0 ? -1 : 1 );
}

inline uint64 adsPoint::squaredDistance( adsPoint p0, adsPoint p1 )
{
    int64 x0 = p0._x;
    int64 x1 = p1._x;
    int64 dx = x1-x0;
    int64 y0 = p0._y;
    int64 y1 = p1._y;
    int64 dy = y1-y0;
    return (uint64) (dx * dx + dy * dy);
}

inline uint64 adsPoint::manhattanDistance( adsPoint p0, adsPoint p1 )
{
    int64 x0 = p0._x;
    int64 x1 = p1._x;
    int64 dx = x1-x0;
    if ( dx < 0 ) dx = -dx;
    int64 y0 = p0._y;
    int64 y1 = p1._y;
    int64 dy = y1-y0;
    if ( dy < 0 ) dy = -dy;
    return (uint64) (dx + dy);
}

inline bool adsPoint::operator<( const adsPoint & rhs ) const
{
    if ( _x < rhs._x )
        return true;

    if ( _x > rhs._x )
        return false;
    
    return _y < rhs._y;
}

inline bool adsRect::operator<( const adsRect & rhs ) const
{
    if ( _xlo < rhs._xlo )
        return true;

    if ( _xlo > rhs._xlo )
        return false;

    if ( _ylo < rhs._ylo )
        return true;

    if ( _ylo > rhs._ylo )
        return false;

    if ( _xhi < rhs._xhi )
        return true;

    if ( _xhi > rhs._xhi )
        return false;

    return _yhi < rhs._yhi;
}

inline adsRect::adsRect()
{
    _xlo = _ylo = _xhi = _yhi = 0;
}

inline adsRect::adsRect(const adsRect & r)
{
    _xlo = r._xlo;
    _ylo = r._ylo;
    _xhi = r._xhi;
    _yhi = r._yhi;
}

inline adsRect::adsRect(int x1, int y1, int x2, int y2)
{
    if ( x1 < x2 )
    {
        _xlo = x1;
        _xhi = x2;
    }
    else
    {
        _xlo = x2;
        _xhi = x1;
    }
    
    if ( y1 < y2 )
    {
        _ylo = y1;
        _yhi = y2;
    }
    else
    {
        _ylo = y2;
        _yhi = y1;
    }
}

inline adsRect::adsRect(const adsPoint p1, const adsPoint p2 )
{
    int x1 = p1.getX();
    int y1 = p1.getY();
    int x2 = p2.getX();
    int y2 = p2.getY();
    
    if ( x1 < x2 )
    {
        _xlo = x1;
        _xhi = x2;
    }
    else
    {
        _xlo = x2;
        _xhi = x1;
    }
    
    if ( y1 < y2 )
    {
        _ylo = y1;
        _yhi = y2;
    }
    else
    {
        _ylo = y2;
        _yhi = y1;
    }
}
inline void adsRect::set_xlo(int x1)
{
	_xlo = x1;
}
inline void adsRect::set_xhi(int x2)
{
	_xhi = x2;
}
inline void adsRect::set_ylo(int y1)
{
	_ylo = y1;
}
inline void adsRect::set_yhi(int y2)
{
	_yhi = y2;
}
inline void adsRect::reset(int x1, int y1, int x2, int y2)
{
	_xlo = x1;
	_xhi = x2;
	_ylo = y1;
	_yhi = y2;
}

inline void adsRect::init(int x1, int y1, int x2, int y2)
{
    if ( x1 < x2 )
    {
        _xlo = x1;
        _xhi = x2;
    }
    else
    {
        _xlo = x2;
        _xhi = x1;
    }
    
    if ( y1 < y2 )
    {
        _ylo = y1;
        _yhi = y2;
    }
    else
    {
        _ylo = y2;
        _yhi = y1;
    }
}

inline adsRect & adsRect::operator=( const adsRect & r )
{
    _xlo = r._xlo;
    _ylo = r._ylo;
    _xhi = r._xhi;
    _yhi = r._yhi;
    return *this;
}

inline bool adsRect::operator==( const adsRect & r ) const
{
    return (_xlo == r._xlo) && (_ylo == r._ylo) && (_xhi == r._xhi) && (_yhi == r._yhi);
}

inline bool adsRect::operator!=( const adsRect & r ) const
{
    return (_xlo != r._xlo) || (_ylo != r._ylo) || (_xhi != r._xhi) || (_yhi != r._yhi);
}

inline uint adsRect::minDXDY()
{
    uint DX = dx();
    uint DY = dy();
    if (DX<DY)
	return DX;
    else
	return DY;
}
inline uint adsRect::maxDXDY()
{
    uint DX = dx();
    uint DY = dy();
    if (DX>DY)
	return DX;
    else
	return DY;
}
inline int adsRect::getDir()
{
    uint DX = dx();
    uint DY = dy();
    if (DX<DY)
	return 0;
    else if (DX>DY)
	return 1;
    else
	return -1;
}
inline void adsRect::moveTo( int x, int y )
{
    uint DX = dx();
    uint DY = dy();
    _xlo = x;
    _ylo = y;
    _xhi = x + DX;
    _yhi = y + DY;
}

inline void adsRect::moveDelta( int dx, int dy )
{
    _xlo += dx;
    _ylo += dy;
    _xhi += dx;
    _yhi += dy;
}

inline int adsRect::xMin() const { return _xlo; }
inline int adsRect::yMin() const { return _ylo; }
inline int adsRect::xMax() const { return _xhi; }
inline int adsRect::yMax() const { return _yhi; }
inline adsPoint adsRect::ll() const { return adsPoint(_xlo, _ylo); }
inline adsPoint adsRect::ul() const { return adsPoint(_xlo, _yhi); }
inline adsPoint adsRect::ur() const { return adsPoint(_xhi, _yhi); }
inline adsPoint adsRect::lr() const { return adsPoint(_xhi, _ylo); }
inline uint adsRect::dx() const { return (uint) (_xhi - _xlo); }
inline uint adsRect::dy() const { return (uint) (_yhi - _ylo); }
inline adsPoint adsRect::low() const { return adsPoint(_xlo, _ylo); }
inline adsPoint adsRect::high() const { return adsPoint(_xhi, _yhi); }

inline bool adsRect::intersects( const adsPoint & p ) const
{
    return !(    (p.getX() < _xlo)
              || (p.getX() > _xhi) 
              || (p.getY() < _ylo) 
              || (p.getY() > _yhi)
                 );
}

inline bool adsRect::intersects( const adsRect & r ) const
{
    return !(    (r._xhi < _xlo)
              || (r._xlo > _xhi)
              || (r._yhi < _ylo)
              || (r._ylo > _yhi)
                 );
}

inline bool adsRect::overlaps( const adsPoint & p ) const
{
    return !(    (p.getX() <= _xlo)
              || (p.getX() >= _xhi) 
              || (p.getY() <= _ylo) 
              || (p.getY() >= _yhi)
                 );
}

inline bool adsRect::overlaps( const adsRect & r ) const
{
    return !(    (r._xhi <= _xlo)
              || (r._xlo >= _xhi)
              || (r._yhi <= _ylo)
              || (r._ylo >= _yhi)
                 );
}

inline bool adsRect::contains( const adsRect & r ) const
{
    return    (_xlo <= r._xlo)
           && (_ylo <= r._ylo)
           && (_xhi >= r._xhi)
           && (_yhi >= r._yhi);
}

inline bool adsRect::inside( const adsRect & r ) const
{
    return    (_xlo < r._xlo)
           && (_ylo < r._ylo)
           && (_xhi > r._xhi)
           && (_yhi > r._yhi);
}

// Compute the union of these two rectangles.
inline void adsRect::merge( const adsRect & r, adsRect & result )
{
    result._xlo = MIN( _xlo, r._xlo );
    result._ylo = MIN( _ylo, r._ylo );
    result._xhi = MAX( _xhi, r._xhi );
    result._yhi = MAX( _yhi, r._yhi );
}

// Compute the union of these two rectangles.
inline void adsRect::merge( const adsRect & r )
{
    _xlo = MIN( _xlo, r._xlo );
    _ylo = MIN( _ylo, r._ylo );
    _xhi = MAX( _xhi, r._xhi );
    _yhi = MAX( _yhi, r._yhi );
}

// Compute the intersection of these two rectangles.
inline void adsRect::intersection( const adsRect & r, adsRect & result )
{
    if ( ! intersects(r) )
    {
        result._xlo = 0;
        result._ylo = 0;
        result._xhi = 0;
        result._yhi = 0;
    }
    else
    {
        result._xlo = MAX( _xlo, r._xlo );
        result._ylo = MAX( _ylo, r._ylo );
        result._xhi = MIN( _xhi, r._xhi );
        result._yhi = MIN( _yhi, r._yhi );
    }
}

// Compute the intersection of these two rectangles.
inline adsRect adsRect::intersect( const adsRect & r )
{
    assert( intersects(r) );
    adsRect result;
    result._xlo = MAX( _xlo, r._xlo );
    result._ylo = MAX( _ylo, r._ylo );
    result._xhi = MIN( _xhi, r._xhi );
    result._yhi = MIN( _yhi, r._yhi );
    return result;
}

inline uint64 adsRect::area()
{
    uint64 a = dx();
    uint64 b = dy();
    return a * b;
}

inline uint64 adsRect::margin()
{
    uint64 DX = dx();
    uint64 DY = dy();
    return DX + DX + DY + DY;
}

inline void adsRect::mergeInit()
{
    _xlo = INT_MAX;
    _ylo = INT_MAX;
    _xhi = INT_MIN;
    _yhi = INT_MIN;
    
}
inline void adsRect::notice(const char *)
{
	;//notice(0, "%s%12d %12d - %12d %12d\n", prefix, _xlo, _ylo, dx, dy);
}
inline void adsRect::printf(FILE *fp, const char * prefix)
{
	fprintf(fp, "%s%12d %12d - %12d %12d\n", prefix, _xlo, _ylo, dx(), dy());
}
inline void adsRect::print(const char * prefix)
{
	fprintf(stdout,"%s%12d %12d - %12d %12d\n", prefix, _xlo, _ylo, dx(), dy());
}


} // namespace

#endif
