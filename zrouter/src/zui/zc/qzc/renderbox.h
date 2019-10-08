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

/* RenderBox.h 
 * Author: Mattias Hembruch
 *
 * Purpose: RenderBox is the container for holding box-coordinates internally,
 *          suitable for rendering. Also holds object's world coordinates, so
 *          that all adjustments are from originals, not subject to round-off.
 *          -scaleAbsolute calculates based on world coordinates and current bbox.
 *          -scaleRelative recalculates based on current screen coordinates
 */

#ifndef _RENDERBOX_H
#define _RENDERBOX_H
#include <stdio.h>
#include <stdlib.h>
#include <qpointarray.h>
#include <qptrlist.h>
#include <math.h>
#include "debug.h"
#include "tagManager.h"


//#define SHOW_CREATION
#define ROUNDOFF(x,y) ( (x>0) ? y : -y )

class RenderBox
{
    private:
    public:
    // Original (world) coordinates
    int _wx; 
    int _wy;
    int _wwidth;
    int _wheight;

    // Adjusted (screen) coordinates
    int _sx; 
    int _sy;
    int _swidth;
    int _sheight;

    // Other data
    int _layer; // layer

    counter *_hierarchy; // hierarchy tag
    counter *_type; // type tag
    counter *_htype; // combined hierarchy and layer tag

    counter *_tags;
    counter *_marks;

    counter *_color; // color of item - for selection

    QPointArray *_pworld;
    QPointArray *_pscreen;

    char * _label;
    char * _lbbox;

    RenderBox();
    RenderBox(int x, int y = 0, int width = 0, int height = 0);
    RenderBox(int x, int y, int width, int height, int layer, counter *htype);
    ~RenderBox();

    void init();
    void un_init();

    void setW(int x, int y, int width, int height);
    void setS(int x, int y, int width, int height);
    void setW(RenderBox &rb);
    void setS(RenderBox &rb);

    void scaleWorld(double xsf, double ysf);
    void scaleScreen(double xsf, double ysf);

    void setMaxMin();
    void maxMin(RenderBox &od);


    void getBBox(char *buff);

    void show();

    void scaleAbsolute(double xres, double yres, int wx0, int wy0, int height, int dx, int dy, int xoff, int yoff);

    void scaleLB(double xres, double yres, int wx0, int wy0, int height, int dx, int dy, int xoff, int yoff);

    void scaleArrow(double xres, double yres, int wx0, int wy0, int height, int dx, int dy, int xoff, int yoff);

    void scaleAbsolutePoly(double xres, double yres, int wx0, int wy0, int height, int dx, int dy, int xoff, int yoff);

}; 
#endif
