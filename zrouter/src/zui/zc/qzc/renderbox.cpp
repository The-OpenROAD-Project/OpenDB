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

/* RenderBox.cpp
 * Author: Mattias Hembruch
 *
 * Purpose: RenderBox is the container for holding box-coordinates internally,
 *          suitable for rendering. Also holds object's world coordinates, so
 *          that all adjustments are from originals, not subject to round-off.
 *          -scaleAbsolute calculates based on world coordinates and current bbox.
 *          -scaleRelative recalculates based on current screen coordinates
 */

#include "renderbox.h"


void RenderBox::init()
{
    _tags = NULL;
    _marks = NULL;
    _type = NULL;
    _hierarchy = NULL;
    _htype = NULL;
    _color = NULL;

    _label = NULL;

    _layer = 0;
    _lbbox = NULL;

    _pworld = NULL;
    _pscreen = NULL;
}

void RenderBox::un_init()
{
    if( _pscreen != NULL )
    {
        delete _pscreen;
        _pscreen = NULL;
    }

    if( _pworld != NULL )
    {
        delete _pworld;
        _pworld = NULL;
    }

    if( _label != NULL )
    {
        free(_label);
        _label = NULL;
    }
    if( _lbbox != NULL )
    {
        free(_lbbox);
        _lbbox = NULL;
    }
}


void RenderBox::setW(int x, int y, int width, int height) 
{
    _wx = x; _wy = y; _wwidth = width; _wheight = height; 
}

void RenderBox::setS(int x, int y, int width, int height) 
{
    _sx = x; _sy = y; _swidth = width; _sheight = height;
}

void RenderBox::setW(RenderBox &rb)
{
    _wx = rb._wx; _wy = rb._wy; _wwidth = rb._wwidth; _wheight = rb._wheight; 
}

void RenderBox::setS(RenderBox &rb)
{
    _sx = rb._sx; _sy = rb._sy; _swidth = rb._swidth; _sheight = rb._sheight;
}

void RenderBox::setMaxMin()
{
    _wx = LONG_MAX;
    _wy = LONG_MAX;
    _wwidth = LONG_MIN;
    _wheight = LONG_MIN;

}

void RenderBox::scaleWorld(double xsf, double ysf)
{
    //qDebug("Scaling world by: %f, %f\n",xsf,ysf);
    double ndx = (_wwidth) * xsf;
    double ndy = (_wheight) * ysf;

    ndx -= _wwidth;
    ndy -= _wheight;

    ndx /= 2;
    ndy /= 2;

    _wx = (int)(_wx - ndx);
    _wy = (int)(_wy - ndy);

    _wwidth = (int)(_wwidth + ndx);
    _wheight = (int)(_wheight + ndy);
    
}

void RenderBox::scaleScreen(double xsf, double ysf)
{
    _sx = (int)(_sx * xsf + 0.5);
    _swidth = (int)(_swidth * xsf + 0.5);

    _sy = (int)(_sy * ysf + 0.5);
    _sheight = (int)(_sheight * ysf + 0.5);
}

void RenderBox::maxMin(RenderBox &od)
{
    if( od._wx < _wx )
    {
        _wwidth += (_wx - od._wx);
        _wx = od._wx;
    }

    if( (od._wx + od._wwidth) > (_wx + _wwidth) )
        _wwidth = (od._wx + od._wwidth) - _wx;

    if( od._wy < _wy )
    {
        _wheight += (_wy - od._wy);
        _wy = od._wy;
    }

    if( (od._wy + od._wheight) > (_wy + _wheight) )
        _wheight = (od._wy + od._wheight) - _wy;
}

RenderBox::RenderBox(int x, int y, int width, int height) 
{
    init();
    setW(x,y,width,height);
    setS(x,y,width,height);
#ifdef SHOW_CREATION
    //fprintf(stderr,"New RB 1: %p\n",this);
    //show();
#endif
}

RenderBox::RenderBox()
{
    init();
}

RenderBox::RenderBox(int x, int y, int width, int height, int layer, counter *htype) 
{
    init();
    setW(x,y,width,height);
    setS(x,y,width,height);
    _layer = layer;
    _htype = htype;

#ifdef SHOW_CREATION
    //fprintf(stderr,"New RB 2: %p\n",this);
    //show();
#endif
}


RenderBox::~RenderBox()
{
    un_init();
}

void RenderBox::getBBox(char *buff)
{
    sprintf(buff,"{%d %d %d %d}",_wx,_wy,(_wx+_wwidth),(_wy+_wheight));
}



void RenderBox::show() 
{
    fprintf(stderr,"World (x,y) (w,h): %d,%d %d,%d\n",_wx,_wy,_wwidth,_wheight);
    fprintf(stderr,"Screen (x,y) (w,h): %d,%d %d,%d\n",_sx,_sy,_swidth,_sheight);
}

#define ROUND_UP 0.999999

void RenderBox::scaleAbsolute(double xres, double yres, int wx0, int wy0, int height, int dx, int dy, int xoff, int yoff)
{
    //printf("SA: %f, %f, %d, %d, %d, %d, dxy: %d,%d\n",xres, yres, wx0, wy0, width, height, dx, dy);

    double tmpx = (double)(_wx-wx0 + xoff);
    tmpx *= xres;
    // no roundoff for points - a little larger, possibly..
    _sx = (int)(tmpx) + dx;
    //double xrem = tmpx - (double)_sx;

    if( _wwidth != 0 )
    {
        // convert x0+w to x1 to get identical treatment for width
        tmpx = (double)((_wx+_wwidth)-wx0 + xoff);
        tmpx *= xres;

        // round ANYTHING above 0 up by almost 1, a little larger possibly
        _swidth = (int)(tmpx+ROUND_UP) - _sx+dx;

        if( (abs(_swidth) < 1))
        {
            _swidth = 1;
        }
    }
    else
    {
        _swidth = 0;
    }

    // Y-coordinate is reversed

    double tmpy = (double)( (2 * wy0  + height - (_wy + _wheight)));
    tmpy = (double)((tmpy)-wy0 + yoff);
    tmpy *=yres;
    _sy = (int)(tmpy) + dy;

    if( _wheight != 0)
    {
        // convert y0+w to y1 to get identical treatment for width
        tmpy = (double)( (2 * wy0  + height - _wy) );
        tmpy = (double)((tmpy)-wy0 + yoff);
        tmpy *= yres;

        _sheight = (int)(tmpy + ROUND_UP) - _sy + dy;

        if( (abs(_sheight) < 1 ) )
        {
            _sheight = 1;
        }
    }
    else
    {
        _sheight = 0;
    }
    //show();
};

void RenderBox::scaleLB(double xres, double yres, int wx0, int wy0, int height, int dx, int dy, int xoff, int yoff)
{
    //printf("SA: %f, %f, %d, %d, %d, %d\n",xres, yres, wx0, wy0, width, height);
    if( _lbbox == NULL )
        return;

    int lwx, lwy, lwwidth, lwheight;

    if( 4 != sscanf(_lbbox,"%d %d %d %d",&lwx, &lwy, &lwwidth, &lwheight) )
    {
        return;
    }

    double tmpx = (double)(lwx-wx0 + xoff);
    tmpx *= xres;
    // no roundoff for points - a little larger, possibly..
    _sx = (int)(tmpx) + dx;

    if( lwwidth != 0 )
    {
        tmpx = ((double)lwwidth * xres);

        // round ANYTHING above 0 up by 1, a little larger possibly
        //_swidth = (int)(tmpx+0.99999);
        _swidth = (int)(tmpx+0.999999);

        if( (abs(_swidth) < 1))
        {
            _swidth = 1;
        }
    }
    else
    {
        _swidth = 0;
    }

    // Y-coordinate is reversed

    double tmpy = (double)( (2 * wy0  + height - (lwy + lwheight)));
    tmpy = (double)((tmpy)-wy0 + yoff);
    tmpy *=yres;
    _sy = (int)(tmpy) + dy;

    if( lwheight != 0)
    {
        tmpy = (double)(lwheight * yres);
        // round ANYTHING above 0 up by 1, a little larger possibly
        //_sheight = (int)(tmpy+0.99999);
        _sheight = (int)(tmpy + 0.99999);

        if( (abs(_sheight) < 1 ) )
        {
            _sheight = 1;
        }
    }
    else
    {
        _sheight = 0;
    }
};

#define ROUNDOFF(x,y) ( (x>0) ? y : -y )
void RenderBox::scaleArrow(double xres, double yres, int wx0, int wy0, int height, int dx, int dy, int xoff, int yoff)
{
    //printf("SA: %f, %f, %d, %d, %d, %d\n",xres, yres, wx0, wy0, width, height);

    double tmpx = (double)(_wx-wx0 + xoff);
    tmpx *= xres;
    // no roundoff for points - a little larger, possibly..
    _sx = (int)(tmpx + ROUNDOFF(tmpx,0.5)) + dx;

    if( _wwidth != 0 )
    {
        tmpx = ((double)_wwidth * xres);

        // round ANYTHING above 0 up by 1, a little larger possibly
        _swidth = (int)(tmpx+ROUNDOFF(tmpx,0.5));

        if( (abs(_swidth) < 1))
        {
            _swidth = 1;
        }
    }
    else
    {
        _swidth = 0;
    }

    // Y-coordinate is reversed

    double tmpy = (double)( (2 * wy0  + height - (_wy + _wheight)));
    tmpy = (double)((tmpy)-wy0 + yoff);
    tmpy *=yres;
    _sy = (int)(tmpy + ROUNDOFF(tmpy,0.5)) + dy;

    if( _wheight != 0)
    {
        tmpy = (double)(_wheight * yres);
        // round ANYTHING above 0 up by 1, a little larger possibly
        //_sheight = (int)(tmpy+0.99999);
        _sheight = (int)(tmpy + (ROUNDOFF(tmpy,0.5)));

        if( (abs(_sheight) < 1 ) )
        {
            _sheight = 1;
        }
    }
    else
    {
        _sheight = 0;
    }
};

void RenderBox::scaleAbsolutePoly(double xres, double yres, int wx0, int wy0, int height, int dx, int dy, int xoff, int yoff)
{

    if( _pworld == NULL )
    {
        fprintf(stderr,"SAP: NULL!\n");
        return;
    }
    unsigned int len = _pworld->count();

    if( _pscreen != NULL )
        _pscreen->resize(len);
    else
        _pscreen = new QPointArray(len);

    int x, y;
    double tmpx, tmpy;
    for(unsigned int i=0; i<len; i++)
    {
        _pworld->point(i,&x,&y);
        tmpx = (double)(x);
        tmpy = (double)(y);
        _pscreen->setPoint(i,
            (int)((tmpx-wx0) * xres + dx + xoff), 
            (int)(((double)(2 * wy0 + height - tmpy) - wy0 + yoff) * yres + dy));
        //fprintf(stderr,"Point before: %d, %d\n",x,y);
        _pscreen->point(i,&x,&y);
        //fprintf(stderr,"Point after: %d, %d\n",x,y);
    }
};
