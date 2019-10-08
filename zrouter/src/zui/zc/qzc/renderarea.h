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

#ifndef _RENDERAREA_H
#define _RENDERAREA_H

#include <qbrush.h>
#include <qpen.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qptrlist.h>

#include <qlayout.h>
#include <qpixmap.h>
#include <qdatetime.h>

#include <X11/Xlib.h>
#include "debug.h"
#include "globals.h"
#include "colormap.h"
#include "dataset.h"
#include "tagManager.h"
#include "renderbox.h"
#include "layerset.h"
#include "image.h"
#include "tcl.h"

typedef void (*mCallback)(int, QMouseEvent*);

class RenderArea : public QWidget
{
    Q_OBJECT

public:

    RenderArea(int ranum, QWidget *parent = 0);

    ~RenderArea();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    //void box(RenderBox *x);

    void embedInto(WId wid);
    void newWindow(WId wid);

    void setSize(int x, int y);
    void scaleBBox(double scx, double scy);

    void setScreen(int x, int y, int width, int height);
    void setBBox(int x, int y, int width, int height);
    void setChipBBox(int x, int y, int width, int height);

    void showStats();

    void screen2World(int &x, int &y);
    void world2Screen(int &x, int &y);

    void setSignalWirePtr(counter *c);
#ifdef VIA_PTR
    void setViaPtrs(tagManager &tm);
#endif

    void getDataBBox(RenderBox &ds, tagManager& datasets, tagManager& images);
    int mkPixmap(tagManager& datasets, tagManager& images, tagManager& tags);
    void mkSelPixmap(tagManager& datasets, tagManager& images, tagManager& tags);
    void clearWindow();

    void setColorMap(ColorMap *cm);
    ColorMap* getColorMap() { return _cm; }

    void setSpecialColorMap(ColorMap *sm);
    ColorMap* getSpecialColorMap() { return _sm; }

    void scaleObjects();

    void resize(int x, int y);

    void up2();

    LayerSet* getLayerSet() { return &ls; };

    void setScaleFactor(double xsf, double ysf);
    void setOverscanColor(char *color);

    void scroll(int dx, int dy);
    void resetScroll();

    void setSelection(char *tag, int x, int y);
    void setDataset(char *tag, int x, int y);
    void setImage(char *tag, int x, int y);

    void adjust(int w, int l, int o, int y, int z);

    int getRenderCount() { return _rcnt + _vcnt + _pcnt; }
    int getSRenderCount() { return _srcnt + _svcnt + _spcnt; }

    Image *grabImage(char *name);

public slots:

//protected:
    //void paintEvent(QPaintEvent *event);

private:
    // private methods
    void initBoxList();
    void initLayers();
    void calcRes();
    void resizeCalc();

    void delPainters();
    void delBitmaps();
    void delPens();

    void allocPainters();
    void allocBitmaps();

    void selectBitmap();

    int checkStatus(RenderBox& r);
    int checkViaStatus(RenderBox& r);

    void checkColor(QPainter *pt, RenderBox *rb);

    void drawArrow(QPainter *pt, RenderBox *rb, int lr);
    void drawVia(QPainter *pt, RenderBox *rb);
    void checkLabel(QPainter *pt, RenderBox *rb, int xoff, int yoff);
    void checkPolyLabel(QPainter *pt, RenderBox *rb, int xoff, int yoff);

    void drawRect(QPainter *pt, int x, int y, int w, int h);
    void drawRect2(QPainter *pt, int x, int y, int w, int h);
    void drawLine(QPainter *pt, int x0, int y0, int x1, int y1);
    void drawLine2(QPainter *pt, int x0, int y0, int x1, int y1);
    void drawOutline(QPainter *pt, int x, int y, int w, int h);

    tagManager dlist;
    tagManager ilist;

    tagManager slist;

    ColorMap *_cm;
    ColorMap *_sm; // for non-standard colors

    // private variables

    QPixmap *_bitmap;
    QPixmap *_bitmap_uns;
    QPixmap *_bitmap_sel;
    QPainter *_painter;
    QPainter *_painter_sel;
    QPainter *_painter_draw;

    QPixmap *_bitmap_draw;

    QBrush *_brush_black;
    QPen *_pen_black;
    QPen *_pen_white;
    QPen *_pen_gray;
    QPen *_pen_red;

    QBrush *_brush_oscan;
    QPen *_pen_oscan;

    RenderBox _screen;
    RenderBox _oscreen;
    RenderBox _bbox;
    RenderBox _obbox;
    RenderBox _cbbox;

    LayerSet ls;

    double _xres;
    double _yres;

    double _oxres;
    double _oyres;

    counter *_signal_wire_ptr;
#ifdef VIA_PTR
    counter *_power_via_ptr;
    counter *_signal_via_ptr;
    counter *_tcut_ptr;
    counter *_bcut_ptr;
#endif

    int _ranum;

    int _dx;
    int _dy;

    double _xsf;
    double _ysf;


    QColor *_ocolor;


    // for counting stats
    int _rcnt;
    int _pcnt;
    int _vcnt;

    int _srcnt;
    int _spcnt;
    int _svcnt;
};

#endif
