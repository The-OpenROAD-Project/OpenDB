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

#include "renderarea.h"
#include "math.h"

//#define SHOW_STATUS
//#define SHOW_TIME
// debug visibility of data sets being rendered
//#define SHOW_VISIBILITY


RenderArea::RenderArea(int ranum, QWidget *parent)
    : QWidget(parent)
{

#ifdef DEBUG_MEM
    fprintf(stderr,"Render Area Initialization!\n");
#endif
    _ranum = ranum;
    _bitmap_uns = NULL;
    _bitmap_sel = NULL;
    _bitmap = NULL;
    _painter = NULL;
    _painter_sel = NULL;
    _painter_draw = NULL;

    _bitmap_draw = NULL;

    _brush_black = new QBrush(Qt::black);
    _pen_black = new QPen(Qt::black);
    _pen_black->setWidth(2);

    _pen_white = new QPen(Qt::white);
    _pen_white->setWidth(2);

    _pen_red = new QPen(Qt::red);
    _pen_red->setWidth(2);

    _pen_gray = new QPen("gray30");
    _pen_gray->setWidth(2);

    _pen_oscan = NULL;
    _brush_oscan = NULL;
    _ocolor = NULL;
    setBackgroundMode(Qt::NoBackground);

    _dx = 0;
    _dy = 0;

    _xsf = 1.0;
    _ysf = 1.0;

    _cm = NULL;
    _sm = NULL;

    ilist.setAuto(0);
    dlist.setAuto(0);
    slist.setAuto(0);

    _oscreen._sx = 0;
    _oscreen._sy = 0;
#ifdef VIA_PTR
    _power_via_ptr = NULL;
    _signal_via_ptr = NULL;
#endif
#ifdef DEBUG_MEM
    fprintf(stderr,"End Render Area Initialization!\n");
#endif
#if 0
    setWFlags(Qt::WNoAutoErase);
    setWFlags(Qt::WNoMousePropagation);
#endif
}

RenderArea::~RenderArea()
{

#ifdef DEBUG_MEM
    fprintf(stderr,"RenderArea destructor for window %d\n",_ranum);
#endif
    setDataset(NULL,0,0); // clear DList
    setImage(NULL,0,0); // clear IList
    setSelection(NULL,0,0);

    delPainters();
    delBitmaps();
    delPens();

    // overscan color
    if( _ocolor != NULL )
    {
        delete _ocolor;
    }
}

void RenderArea::delPainters()
{
    // delete painter resources
    if( _painter != NULL)
    {
        delete _painter;
    }
    if( _painter_sel != NULL)
    {
        delete _painter_sel;
    }
    if( _painter_sel != NULL)
    {
        delete _painter_draw;
    }
}

void RenderArea::delBitmaps()
{
    // delete bitmap resources
    if( _bitmap_uns != NULL)
    {
        delete _bitmap_uns;
    }
    if( _bitmap_sel != NULL)
    {
        delete _bitmap_sel;
    }

    if( _bitmap_draw != NULL )
    {
        delete _bitmap_draw;
    }
}

void RenderArea::delPens()
{
    delete _pen_black;
    delete _pen_white;
    delete _pen_gray;
}



void RenderArea::showStats()
{
#ifdef SHOW_STATS
    qDebug("RenderArea Debug!");
    qDebug("BBox, OBBox, Screen, OScreen: ");
    _bbox.show();
    _obox.show();
    _screen.show();
    _oscreen.show();
    qDebug("oxyres: %f,%f\n",_oxres, _oyres);
    qDebug("xyres: %f,%f\n",_xres[_ranum], _yres[_ranum]);
#endif
}

void RenderArea::setColorMap(ColorMap *cm)
{
    //qDebug("Set Colormap to: %s\n",cm->getName());
    _cm = cm;
}

void RenderArea::setSpecialColorMap(ColorMap *sm)
{
    //qDebug("Set Special Colormap to: %s\n",sm->getName());
    _sm = sm;
}

void RenderArea::setDataset(char *tag, int x, int y)
{
    if( tag == NULL || strlen(tag) == 0)
    {
        // reset display list
        //qDebug("Win %d erasing Dlist!\n", _ranum);
        tmEraser<char> ted;
        ted.eraseTM(dlist);
        //qDebug("Win %d done erasing Dlist!\n", _ranum);
    }
    else
    {
        // add to display list
        // NEED to uncomment both current+1 AND current+3 for debugging
        counter *c = dlist.getTagId(tag);
        char buff[1024];
        sprintf(buff,"%d %d",x,y);
        c->data = (void*)strdup(buff);

        //qDebug("Win %d added set %s to Dlist: %p!\n",_ranum,c->val,c->data);
    }
}

void RenderArea::setSelection(char *tag, int x, int y)
{
    if( tag == NULL || strlen(tag) == 0)
    {
        // reset display list
        //qDebug("Win %d erasing Slist!\n", _ranum);
        tmEraser<char> ted;
        ted.eraseTM(slist);
        //qDebug("Win %d done erasing Slist!\n", _ranum);
    }
    else
    {
        // add to display list
        counter *c = slist.getTagId(tag);
        char buff[1024];
        sprintf(buff,"%d %d",x,y);
        c->data = (void*)strdup(buff);
        //qDebug("Win %d added set %s to Slist: %p!\n",_ranum,c->val,c->val);
    }
    selectBitmap();
}

void RenderArea::setImage(char *tag, int x, int y)
{
    if( tag == NULL || strlen(tag) == 0)
    {
        // reset display list
        //qDebug("Win %d erasing Ilist!\n", _ranum);
        tmEraser<char> ted;
        ted.eraseTM(ilist);
        //qDebug("Win %d done erasing Ilist!\n", _ranum);
    }
    else
    {
        // add to display list
        counter *c = 
        ilist.getTagId(tag);
        char buff[1024];
        sprintf(buff,"%d %d",x,y);
        c->data = (void*)strdup(buff);
        //qDebug("Win %d added image %s to Ilist: %p!\n",_ranum,c->val,c->data);
    }
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize RenderArea::sizeHint() const
{
    return QSize(800, 800);
}

void RenderArea::embedInto(WId id)
{
    switch (XReparentWindow(x11Display(), winId(), id, 0, 0)) {
        case BadWindow:
            fprintf(stderr,"Bad Window ID!\n");
            break;
        case BadMatch:
            fprintf(stderr,"Internal Error!\n");
            break;
        case Success:
        default:
            break;
    }
}


void RenderArea::setScaleFactor(double xsf, double ysf) 
{ 
    _xsf = xsf; 
    _ysf = ysf; 
}

void RenderArea::setOverscanColor(char *color)
{
    if( _ocolor != NULL )
        delete _ocolor;

    _ocolor = new QColor(color);
    //qDebug("Background color: %s\n",color);

    setBackgroundColor(*_ocolor);

    if( _pen_oscan != NULL )
        delete _pen_oscan;

    _pen_oscan = new QPen(*_ocolor);

    if( _brush_oscan != NULL )
        delete _brush_oscan;

    _brush_oscan = new QBrush(*_ocolor);
    
}

void RenderArea::scroll(int dx, int dy) 
{ 
    _dx -= dx; 
    _dy -= dy; 
    //qDebug("Scroll: %d, %d\n",_dx, _dy);
}

void RenderArea::resetScroll() 
{ 
    _dx = 0; 
    _dy = 0; 
}


void RenderArea::newWindow(WId wid)
{
    create(wid, TRUE, TRUE);
}


void RenderArea::world2Screen(int &x, int &y)
{
    x = (int)( (double)(x-_bbox._wx) * _oxres + 0.9999);
    y = (int)( (double)(y-_bbox._wy) * _oyres + 0.9999);
}

void RenderArea::screen2World(int &x, int &y)
{
    x = (int)( (double)x / _xres + _bbox._wx);
    // y co-ordinate is reversed
    y = _screen._sheight - y;
    y = (int)( (double)y / _yres + _bbox._wy);
}

Image *RenderArea::grabImage(char *name)
{
    Image *i = new Image(name);
    i->setPixmap(new QPixmap(*_bitmap_uns));
    return i;
}
void RenderArea::setChipBBox(int x, int y, int width, int height)
{
    //qDebug("setChipBBox: %d,%d to %d,%d\n",x,y,x+width,y+height);
    _cbbox.setW(x,y,width,height);
}

void RenderArea::setBBox(int x, int y, int width, int height)
{
    _bbox.setW(x,y,width,height);
    //qDebug("bbox: ");
    //_bbox.show();

    /*
    qDebug("obbox: ");
    _obbox.setW(_bbox);
    _obbox.show();
    _obbox.scaleWorld(_xsf,_ysf);
    _obbox.show();
    */

    calcRes();
}
void RenderArea::setScreen(int x, int y, int width, int height)
{
    _screen.setS(x,y,width,height);

    _oscreen.setS(_screen);
    _oscreen.scaleScreen(_xsf,_ysf);
}




//LOGIC OF Rendering/Scaling

void RenderArea::setSignalWirePtr(counter *c)
{
    _signal_wire_ptr = c;
}

#ifdef VIA_PTR
void RenderArea::setViaPtrs(tagManager &tm)
{
    _power_via_ptr = tm.getTagId("htype","power/via");
    _signal_via_ptr = tm.getTagId("htype","signal/via");
    _bcut_ptr = tm.getTagId("type","bcut");
    _tcut_ptr = tm.getTagId("type","tcut");

}
#endif

void RenderArea::resize(int x, int y)
{
    //qDebug("Resize Event! New size: %d by %d = %d!\n",x,y,x*y);
    _screen._swidth = x;
    _screen._sheight = y;

    _oscreen._swidth = (int)(x * _xsf + 0.5);
    _oscreen._sheight = (int)(y * _ysf + 0.5);

    _screen._sx = (_oscreen._swidth - _screen._swidth) / 2;
    _screen._sy = (_oscreen._sheight - _screen._sheight) / 2;
    resizeCalc();

    /*
    qDebug("Screen: ");
    _screen.show();
    qDebug("O Screen: ");
    _oscreen.show();
    */
}

void RenderArea::resizeCalc()
{

    //qDebug("Resize Event!");

    delPainters();
    delBitmaps();

    allocBitmaps();
    allocPainters();

    calcRes();

#ifdef SHOW_STATS
    showStats();
#endif
}

//#define DEBUG_RES
void RenderArea::calcRes()
{
    //qDebug("calcRes: %d!\n",_bbox._wwidth);
    //_bbox.show();
    if( _bbox._wwidth > 0.0 )
    {
#ifdef DEBUG_RES
        qDebug("Before xres (%p-%d): %f, yres: %f, oxy: %f, %f\n",ds,_ranum,_xres, _yres, _oxres, _oyres);
        qDebug("bbox (w/h), (w/h): (%d %d), (%d %d)\n",_bbox._wwidth, _bbox._wheight, _bbox._swidth, _bbox._sheight);
#endif

        _xres = (double)_screen._swidth / (double)_bbox._wwidth;
        _yres = (double)_screen._sheight / (double)_bbox._wheight;
        //_oxres = (double)_oscreen._swidth / (double)_obbox._wwidth;
        //_oyres = (double)_oscreen._sheight / (double)_obbox._wheight;

#ifdef DEBUG_RES
        qDebug("After xres (%p-%d): %f, yres: %f, oxy: %f, %f\n",ds,_ranum,_xres, _yres, _oxres, _oyres);

#endif
    }
}


void RenderArea::allocBitmaps()
{
    _bitmap_sel = new QPixmap(_oscreen._swidth, _oscreen._sheight, -1,QPixmap::BestOptim);
    _bitmap_uns = new QPixmap(_oscreen._swidth, _oscreen._sheight, -1,QPixmap::BestOptim);
    _bitmap_draw = new QPixmap(_screen._swidth,_screen._sheight, -1,QPixmap::BestOptim);

    selectBitmap();
}

void RenderArea::selectBitmap()
{
    if( slist.getTypeCount() > 0 )
    {
        _bitmap = _bitmap_sel;
    }
    else
    {
        _bitmap = _bitmap_uns;
    }
}

void RenderArea::allocPainters()
{
    _painter = new QPainter(_bitmap_uns);
    _painter_sel = new QPainter(_bitmap_sel);

    _painter_draw = new QPainter(_bitmap_draw);

}




int RenderArea::checkStatus(RenderBox& rb)
{
    int x =  (rb._htype->status & rb._marks->status & rb._tags->status);
#ifdef SHOW_STATUS
if (!x)
    qDebug("checkStatus: '%s':%d-%d-%d",rb._htype->val,rb._htype->status, rb._marks->status, rb._tags->status);
#endif
    return x;
}

int RenderArea::checkViaStatus(RenderBox& rb)
{
#ifdef VIA_PTR
    //qDebug("cvs: %d\n",(rb._marks->status & rb._tags->status));
    return (rb._marks->status & rb._tags->status);
#else
    //qDebug("cvs: %d\n",(rb._htype-.status & rb._marks->status & rb._tags->status));
    return (rb._htype->status & rb._marks->status & rb._tags->status);
#endif
}

void RenderArea::getDataBBox(RenderBox &db, tagManager &datasets, tagManager &images)
{
    DataSet *ds;
    counter *c;
    int dlen = dlist.getTypeCount();

    Image *is;
    int ilen = ilist.getTypeCount();

    RenderBox *rb;

    for(int ilidx=0; ilidx<ilen; ilidx++)
    {
        c = ilist.getTagId(ilidx);
        c = images.findTagId(c->val);
        if( (c == NULL) || (c->data == NULL) )
            continue;

        is = (Image*) c->data;

        if( is != NULL )
        {
            rb = is->getBBox();
            db.maxMin(*rb);
        }
    }
    for(int dlidx=0; dlidx < dlen; dlidx++)
    {

        c = dlist.getTagId(dlidx);

        c = datasets.findTagId(c->val);

        if( (c == NULL) || (c->data == NULL) )
            continue;

        ds = (DataSet*)c->data;

        //qDebug("BBox for dataset %s\n",ds->getName());
        rb = ds->getBBox();
        db.maxMin(*rb);
    }
}
// "thick" arrows
//int aw = 7; int al = 12; int aoff = 1; int yoff = -1; int lw = 3;
// "thin" arrows
int aw = 3; int al = 9; int aoff = 1; int yoff = 0; int lw = 1;

void RenderArea::adjust(int w, int l, int o, int y, int z)
{
    aw += w;
    al += l;
    aoff += o;
    yoff += y;
    lw += z;
    qDebug("Aw: %d, Al: %d, AO: %d AY: %d LW: %d\n",aw,al,aoff,yoff,lw);
}


void RenderArea::drawVia(QPainter *pt, RenderBox *rb)
{
#if 0
    // black/white method
    QPen pn = pt->pen();
    if( rb->_type == _bcut_ptr )
    {
        pt->setPen(*_pen_gray);
        drawOutline(pt, rb->_sx, rb->_sy, rb->_swidth, rb->_sheight);
        pt->setPen(*_pen_black);
        pt->drawLine(rb->_sx,rb->_sy,rb->_sx+rb->_swidth,rb->_sy+rb->_sheight);

        int x1 = rb->_sx + rb->_swidth;
        int y1 = rb->_sy + rb->_sheight;

        for(int i=rb->_sx; i<x1; i++)
        {
            pt->drawLine(i,y1,rb->_sx,rb->_sy);
        }
        
    }
    else if( rb->_type == _tcut_ptr )
    {
        pt->setPen(*_pen_gray);
        drawOutline(pt, rb->_sx, rb->_sy, rb->_swidth, rb->_sheight);
        pt->setPen(*_pen_white);
        pt->drawLine(rb->_sx,rb->_sy,rb->_sx+rb->_swidth,rb->_sy+rb->_sheight);

        int x1 = rb->_sx + rb->_swidth;
        int y1 = rb->_sy + rb->_sheight;

        for(int i=rb->_sx; i<x1; i++)
        {
            pt->drawLine(i,rb->_sy,x1,y1);
        }
    }
    else
    {
        drawRect(pt, rb->_sx, rb->_sy, rb->_swidth, rb->_sheight);
        pt->setPen(*_pen_gray);
        drawOutline(pt, rb->_sx, rb->_sy, rb->_swidth, rb->_sheight);
    }
    pt->setPen(pn);
#else
    // draw overall rectangle
    if( rb->_type == _bcut_ptr  || rb->_type == _tcut_ptr)
    {
        QPen pn = pt->pen();
        pt->setPen(*_pen_gray);
        drawOutline(pt, rb->_sx, rb->_sy, rb->_swidth, rb->_sheight);
        int dx = (int)(rb->_swidth * 0.5);
        int dy = (int)(rb->_sheight * 0.5);
        if( rb->_type == _tcut_ptr )
        {
            // top cut
            //pt->setPen(*_pen_red);
            drawLine(pt,rb->_sx,rb->_sy,rb->_sx+dx,rb->_sy+dy);
            drawLine(pt,rb->_sx+dx, rb->_sy+dy,rb->_sx+rb->_swidth,rb->_sy);
        }
        else
        {
            // bottom cut
            //pt->setPen(*_pen_oscan);
            drawLine(pt,rb->_sx,rb->_sy+rb->_sheight,rb->_sx+dx,
                    rb->_sy+rb->_sheight-dy);
            drawLine(pt,rb->_sx+dx, rb->_sy+rb->_sheight-dy,
                    rb->_sx+rb->_swidth,rb->_sy+rb->_sheight);

        }
        pt->setPen(pn);
    }
    else
    {
        drawRect(pt, rb->_sx, rb->_sy, rb->_swidth, rb->_sheight);
    }
#endif
}
void RenderArea::drawArrow(QPainter *pt, RenderBox *rb, int lr)
{

    if( (lr < 0) || (lr > 1) )
    {
        qDebug("Error - neither left nor right arrow!");
        return;
    }

    QPen pen = pt->pen();
    QPen np(pen);
    int width = pen.width();
    np.setWidth(lw);
    pt->setPen(np);
    
    int sx = 0,sy = 0;
    int ex = 0,ey = 0;
    int epx0 = 0, epx1 = 0;
    int epy00 = 0, epy01 = 0;
    int epy10 = 0, epy11 = 0;

    int dx, dy;

    //rb->show();
    if( lr == 0 )
    {
        // right arrow
        sx = rb->_sx;
        ey = rb->_sy;
        ex = rb->_sx+rb->_swidth;
        sy = rb->_sy+rb->_sheight;

    }
    else if( lr == 1 )
    {
        // left arrow
        ex = rb->_sx;
        sy = rb->_sy;
        sx = rb->_sx+rb->_swidth;
        ey = rb->_sy+rb->_sheight;

    }

    epx0 = -aoff;
    epx1 = -al;
    epy00 = -yoff;
    epy01 = yoff;
    epy10 =  - aw;
    epy11 = aw;

    //qDebug("sx,sy: %d, %d ex,ey: %d, %d, rx,ry: %d, %d\n",sx,sy,ex,ey,rx,ry);

    pt->drawLine(sx, sy, ex, ey);
    dx = ex - sx;
    dy = ey - sy;

    double arad = atan2((double)dy,(double)dx);

    pt->save();
    pt->translate(ex,ey);
    pt->rotate(arad * 57.29578);
    pt->drawLine(epx0,epy00,epx1, epy10);
    pt->drawLine(epx0,epy01,epx1, epy11);
    pt->restore();

    if( rb->_label != NULL )
    {
        QString qstr(rb->_label);
        QFontMetrics fm = pt->fontMetrics();
        QSize sz = fm.size(Qt::ShowPrefix,qstr);
        int mx = sx + (dx/2);
        int my = sy + (dy/2);

        pt->drawText(mx, my, sz.width(), sz.height(),
            Qt::AlignLeft || Qt::AlignBottom, qstr);

        // rotated text
#if 0
        int mx = sx;
        int my = sy;
        pt->save();
        if( dx >= 0)
        {
            if( sz.width() < dx )
            {
                mx = sx + (dx - sz.width()) / 2;
            }
            if ( dy >= 0)
            {
                //qDebug("case 1: %s\n",rb->_label);
                my = sy + dy/2-8;
            }
            else
            {
                //qDebug("case 2: %s\n",rb->_label);
                my = sy + dy/2+8;
            }
            pt->translate(mx,my);
            pt->rotate(arad * 57.29578);
        }
        else
        {
            if( sz.width() < (-dx) )
            {
                mx = sx + ( (dx) - sz.width()) / 2;
            }
            if ( dy >= 0)
            {
                //qDebug("case 3: %s\n",rb->_label);
                my = sy + dy/2+8;
            }
            else
            {
                //qDebug("case 4: %s\n",rb->_label);
                my = sy + dy/2-8;
            }
            pt->translate(mx,my);
            pt->rotate(180 + arad *57.29758);
        }


        pt->drawText(0, 0, sz.width(), sz.height(),
            Qt::AlignLeft || Qt::AlignBottom, qstr);
        pt->restore();
#endif
    }
        
    /* rx, ry dot.. - for debugging */
#if 0
    QColor c(Qt::red);
    QBrush b2;
    b2.setColor(c);
    QPen p2;
    p2.setColor(c);
    pt->setBrush(b2);
    pt->setPen(p2);
    pt->drawLine(rx,ry,rx,ry);
#endif

    pen.setWidth(width);
    pt->setPen(pen);
}

void RenderArea::checkPolyLabel(QPainter *pt, RenderBox *rb, int xoff, int yoff)
{
    checkLabel(pt,rb,xoff,yoff);
}

void RenderArea::checkLabel(QPainter *pt, RenderBox *rb, int xoff, int yoff)
{
    if( rb->_label == NULL )
        return;

    bool scale = false;
    QString qstr(rb->_label);
    QFontMetrics fm = pt->fontMetrics();
    QSize sz = fm.size(Qt::ShowPrefix,qstr);
    int oldPointsize = -1;
    if( rb->_lbbox != NULL )
    {
        // use given bbox
        rb->scaleLB( _xres,_yres, _bbox._wx, _bbox._wy, 
            _bbox._wheight, _screen._sx, _screen._sy, xoff, yoff);

        if( (sz.width() > rb->_swidth) || (sz.height() > rb->_sheight) )
        {
            QFont f = pt->font();
            oldPointsize = f.pointSize();
            for( int i = oldPointsize; i>= 4; i-- )
            {
                f.setPointSize(i);
                QFontMetrics fm2(f);
                QSize sz2 = fm2.size(Qt::ShowPrefix,qstr);

                if( (sz2.width() <= rb->_swidth) && (sz2.height() <= rb->_sheight) )
                    break;
            }
            pt->setFont(f);
            QFont f2 = pt->font();
            scale = true;
        }
    }
    else
    {
        // draw below and centered, or to right and centered
        // add 10% to y component, 15 pixels tall?
        // count number of '\n'
        int cnt = 1;
        char *ptr = rb->_label;
        while(*ptr != '\0')
        {
            if( *ptr++ == '\n')
                cnt++;
        }

        //qDebug("fs: %d,%d\n",sz.width(), sz.height());

        if( rb->_swidth > rb->_sheight)
        {
            rb->_sy += rb->_sheight;
            if( sz.width() < rb->_swidth )
            {
                // center horizontally
                rb->_sx += (rb->_swidth - sz.width()) / 2;
            }
            rb->_swidth = sz.width();
            //rb->_sheight = (fr.height() - fr.y())*cnt;
            rb->_sheight = sz.height();
        }
        else
        {
            rb->_sx += rb->_swidth+2;
            if( sz.height() < rb->_sheight)
            {
                // center vertically
                rb->_sy += (rb->_sheight - sz.height()) / 2;
            }
            rb->_swidth = sz.width();
            rb->_sheight = sz.height();
        }
    }

    pt->drawText(rb->_sx, rb->_sy, rb->_swidth, rb->_sheight, 
        Qt::AlignLeft || Qt::AlignBottom, qstr);

    if( scale == true )
    {
        QFont f = pt->font();
        f.setPointSize(oldPointsize);
        pt->setFont(f);
    }
}


void RenderArea::clearWindow()
{
    //qDebug("clearWindow for window %d !\n",_ranum);
    //qDebug("background bbox: ");

    // fill new canvas with background color
    //_oscreen.show();

    _painter->setPen(*_pen_oscan);
    _painter->setBrush(*_brush_oscan);
    drawRect(_painter,0,0,_oscreen._swidth, _oscreen._sheight);


    // fill canvas with required black color
    _painter->setPen(*_pen_black);
    //_painter->setBrush(*_brush_black);

    _cbbox.scaleAbsolute( _xres,_yres, _bbox._wx, _bbox._wy, 
            _bbox._wheight, _screen._sx, _screen._sy, 0,0);
    //qDebug("black bbox: ");
    //_cbbox.show();

    drawRect(_painter,_cbbox._sx-1,_cbbox._sy-1,_cbbox._swidth+1,_cbbox._sheight+1);
}

void RenderArea::up2()
{
    if ( _bitmap != NULL && _ocolor != NULL)
    {
        //_painter_draw->fillRect(0,0,_screen._swidth, _screen._sheight,*_ocolor);
#ifdef SHOW_VISIBILITY
        qDebug("RA Painting!");
        if( _bitmap == _bitmap_sel ) 
        {
            qDebug("With selection!");
        }
        else
        {
            qDebug("No selection!");
        }
#endif
        // assume bitmap is correct

        if( ( abs(_dx) > _screen._sx) || ( abs(_dy) > _screen._sy) )
        {
            // IF we have dragged more than the overscan covers, help it out
            // cover drawing canvas with background color


            _painter_draw->setPen(*_pen_oscan);
            //_painter_draw->setPen(*_pen_red);
            drawRect(_painter_draw,0,0,_screen._swidth, _screen._sheight);


            // draw "unfilled" chip background during scrolling
            _painter_draw->setPen(*_pen_black);
            //_painter_draw->setPen(*_pen_white);

            // scale relative to the dragged viewport
            _cbbox.scaleAbsolute( _xres,_yres, _bbox._wx, _bbox._wy, 
                    _bbox._wheight, -_dx,-_dy, 0,0);

            drawRect(_painter_draw,_cbbox._sx-1,_cbbox._sy-1,_cbbox._swidth+1, _cbbox._sheight+1);
        }

        // copy bitmap onto drawing canvas
        bitBlt(_bitmap_draw,0,0,_bitmap,_screen._sx+_dx,_screen._sy+_dy, -1, -1,Qt::CopyROP, false);

        // copy drawn canvas onto screen
        bitBlt(this,0,0,_bitmap_draw,0,0,-1,-1,Qt::CopyROP, false);
    }
}

void RenderArea::checkColor(QPainter *pt, RenderBox *rb)
{
    // set special pen/brush
    if( rb->_color != NULL )
    {
        if( isdigit(rb->_color->val[0]) )
        {
            int a = atoi(rb->_color->val);
            //qDebug("Special color %d\n",a);
            pt->setPen( *_sm->getPen(a) );
        }
        else
        {
            QPen   qp(rb->_color->val);
            pt->setPen( qp);
        }
    }
}

void RenderArea::drawOutline(QPainter *pt, int x, int y, int w, int h)
{
    drawLine(pt,x,y,x+w,y);
    drawLine(pt,x,y,x,y+h);
    drawLine(pt,x,y+h,x+w,y+h);
    drawLine(pt,x+w,y,x+w,y+h);
}

void RenderArea::drawLine(QPainter *pt, int x0, int y0, int x1, int y1)
{
    if( x0 > _oscreen._swidth )
        return;

    if( x1 < 0 )
        return;

    if( y0 > _oscreen._sheight)
        return;

    if( y1 < 0 )
        return;
            
    // do some clipping
    int rx0 = x0;
    int rx1 = x1;
    int ry0 = y0;
    int ry1 = y1;

    if( x0 < 0 )
        rx0 = 0;

    if( x1 > _oscreen._swidth )
        rx1 = _oscreen._swidth;

    if( y0 < 0 )
        ry0 = 0;

    if( y1 > _oscreen._sheight )
        ry1 = _oscreen._sheight;

    pt->drawLine(rx0,ry0,rx1,ry1);
}

void RenderArea::drawLine2(QPainter *pt, int x0, int y0, int x1, int y1)
{
    //qDebug("dl2: enter");
    if( x0 > _oscreen._swidth )
        return;

    if( x1 < 0 )
        return;

    if( y0 > _oscreen._sheight)
        return;

    if( y1 < 0 )
        return;
    //qDebug("dl2: survive!");
            
    // do some clipping
    int rx0 = x0;
    int rx1 = x1;
    int ry0 = y0;
    int ry1 = y1;

    if( x0 < 0 )
        rx0 = 0;

    if( x1 > _oscreen._swidth )
        rx1 = _oscreen._swidth;

    if( y0 < 0 )
        ry0 = 0;

    if( y1 > _oscreen._sheight )
        ry1 = _oscreen._sheight;

    //qDebug("dl2: draw %d %d %d %d!",rx0, ry0, rx1, ry1);
    pt->drawLine(rx0,ry0,rx1,ry1);
}

void RenderArea::drawRect(QPainter *pt, int x, int y, int w, int h)
{
    // ASSUME pen width == 1!!
    if( (w == 1) )
    {
        if (h == 1) 
        {
            if( (x >= 0) && (x <= _oscreen._swidth) )
                pt->drawPoint(x,y);
        }
        else
        {
            drawLine(pt,x,y,x,y+h-1);
        }
    }
    else if( h == 1)
    {
        drawLine(pt,x,y,x+w-1,y);
    }
    else
    {
        int y1=y+h-1;
        int x1=x+w-1;
        // generic rectangle
        if( h > w )
        {
            for(int i=x; i<=x1; i++)
            {
                drawLine(pt,i,y,i,y1);
            }
        }
        else
        {
            
            for(int i=y; i<=y1; i++)
            {
                drawLine(pt,x,i,x1,i);
            }
        }
    }
}

void RenderArea::drawRect2(QPainter *pt, int x, int y, int w, int h)
{
    // ASSUME pen width == 1!!
    if( (w == 1) )
    {
        if (h == 1) 
        {
            if( (x >= 0) && (x <= _oscreen._swidth) )
                pt->drawPoint(x,y);
        }
        else
        {
            drawLine(pt,x,y,x,y+h-1);
        }
    }
    else if( h == 1)
    {
        drawLine(pt,x,y,x+w-1,y);
    }
    else
    {
        //qDebug("dr2: Else!");
        int y1=y+h-1;
        int x1=x+w-1;
        // generic rectangle
        if( h > w )
        {
            for(int i=x; i<=x1; i++)
            {
                drawLine2(pt,i,y,i,y1);
                //pt->drawLine(i,y,i,y1);
                //qDebug("dr2: pt->drawLine %d %d %d %d!",i,y,i,y1);
            }
        }
        else
        {
            
            for(int i=y; i<=y1; i++)
            {
                //drawLine(pt,x,i,x1,i);
                //pt->drawLine(x,i,x1,i);
                drawLine2(pt,x,i,x1,i);
                //qDebug("dr2: drawLine %d %d %d %d!",x,i,x1,i);
            }
        }
    }
}

int RenderArea::mkPixmap(tagManager& datasets, tagManager& images, tagManager& tags)
{

    //qDebug("Window %d mk Pixmap!\n", _ranum);
    //QSize x = maximumSize();
    //qDebug("Max Size: %d, %d\n",x.width(), x.height());
    //qDebug("Current Size: %d, %d\n",width(), height());
    if( _cm == NULL )
    {
        qDebug("No ColorMap - nothing to do!\n");
        return 0;
    }

    DataSet *ds;
    counter *c;
    int dlen = dlist.getTypeCount();

    int xoff = 0;
    int yoff = 0;

    Image *is;
    int ilen = ilist.getTypeCount();

    _rcnt= 0, _vcnt=0, _pcnt=0;
#ifdef SHOW_TIME
    QTime timer;
    int elapsed;
    timer.start();
#endif

    QPixmap *pmap;
    RenderBox * rb;

    int sx = _screen._sx;
    int sy = _screen._sy;
    // fill "data" portion (if any) with black..
    /*
    RenderBox dbb(0,0,0,0);
    dbb.setMaxMin();
    getDataBBox(dbb, datasets, images);

    dbb.scaleAbsolute( _xres,_yres, _bbox._wx, 
            _bbox._wy, _bbox._wwidth, _bbox._wheight, sx, sy, 0, 0);

            */
    /*
    qDebug("Current BBox:");
    _bbox.show();

    qDebug("Data BBox for black: ");
    dbb.show();
    */


    counter *larrow = tags.findTagId("type","larrow");
    counter *rarrow = tags.findTagId("type","rarrow");

    /*
    _painter->setPen(*_pen_oscan);
    _painter->setBrush(*_brush_oscan);
    _painter->drawRect(0,0,_oscreen._swidth, _oscreen._sheight);

    _painter->setPen(*_pen_black);
    _painter->setBrush(*_brush_black);
    _painter->drawRect(dbb._sx,dbb._sy,dbb._swidth,dbb._sheight);
    */

    for(int ilidx=0; ilidx<ilen; ilidx++)
    {
        c = ilist.getTagId(ilidx);
        if( c->data != NULL )
        {
            sscanf((char*)(c->data),"%d %d",&xoff, &yoff);
            //qDebug("Xoff: %d, Yoff: %d\n", xoff, yoff);
        }
        else
        {
            xoff = 0;
            yoff = 0;
        }
        c = images.findTagId(c->val);
        if( (c == NULL) || (c->data == NULL) )
            continue;

        is = (Image*) c->data;
        //qDebug("Win %d drawing Image %s\n",_ranum, is->getName());

        if( is != NULL )
        {
            rb = is->getBBox();
            rb->scaleAbsolute( _xres,_yres, _bbox._wx, _bbox._wy, 
                    _bbox._wheight, sx, sy, xoff, yoff);

            is->scale(rb->_swidth, rb->_sheight);
            pmap = is->getPixmap();
            if( pmap != NULL )
            {
                //pmap = is->getImage()->scale(rb->_swidth, rb->_sheight);

                bitBlt(_bitmap_uns,rb->_sx,rb->_sy,pmap,xoff,yoff, rb->_swidth, rb->_sheight, Qt::CopyROP, false);
            }
            else
            {
                fprintf(stderr,"Unable to convert image in mkPixmap!\n");
            }
        }

    }
    //qDebug("After Rect: %d, %d, %d\n",width(), height(), _ranum);
    //int ts, hs;

    for(int i=0; i<=ls.getMaxLayer(); i++)
    {
        if ( ls.getLayerStatus(i) != 0 || i == 11)
        {
            if( i == 10 )
                continue;

#ifdef SHOW_VISIBILITY
            fprintf(stderr,"Layer %d active (%d)\n",i,dlen);
#endif
            for(int dlidx=0; dlidx < dlen; dlidx++)
            {

                c = dlist.getTagId(dlidx);
                if( c->data != NULL )
                {
                    sscanf((char*)(c->data),"%d %d",&xoff, &yoff);
                    //qDebug("Xoff: %d, Yoff: %d\n", xoff, yoff);
                }
                else
                {
                    xoff = 0;
                    yoff = 0;
                }

                c = datasets.findTagId(c->val);

                if( (c == NULL) || (c->data == NULL) )
                    continue;

                _painter->setPen( *_cm->getPen(i) );
#ifdef SHOW_VISIBILITY
                qDebug("Win %d, rendering Dataset %s\n",_ranum,c->val);
#endif
                ds = (DataSet*)c->data;

                QPtrListIterator<RenderBox> *box_it = ds->getBoxIt(i);
                while( (rb = box_it->current()) != 0 )
                {
                    // don't draw arrows here
                    if ( (rb->_type == larrow) || (rb->_type == rarrow) )
                    {
                        ++(*box_it);
                        continue;
                    }

                    if ( checkStatus(*rb) == 1 )
                    {
                        checkColor(_painter,rb);
                        // can't be an arrow
                        rb->scaleAbsolute( _xres,_yres, _bbox._wx, 
                         _bbox._wy, _bbox._wheight, sx, sy,
                         xoff, yoff);

#ifdef SHOW_VISIBILITY
                        rb->show();
                        qDebug("Type: %s\n",rb->_type->val);
#endif

                        drawRect(_painter,
                            rb->_sx, rb->_sy, rb->_swidth, rb->_sheight);
                        checkLabel(_painter,rb,xoff, yoff);

                        // reset original brush/pen
                        if( rb->_color != NULL )
                        {
                            _painter->setPen( *_cm->getPen(i) );
                        }
                        _rcnt++;
                    }
                    ++(*box_it);
                }
                if (box_it != NULL)
                    delete box_it;

                _painter->setPen( *_cm->getPen(i) );
                //_painter->setBrush( *_cm->getBrush(i) );
                if( _signal_wire_ptr->status == 1)
                {
                    QPtrListIterator<RenderBox> *swire_it = ds->getSWireIt(i);
                    while( (rb = swire_it->current()) != 0 )
                    {
                        if ( checkStatus(*rb) == 1 )
                        {
                            rb->scaleAbsolute( _xres,_yres, _bbox._wx, 
                             _bbox._wy, _bbox._wheight, sx, sy,
                             xoff, yoff);
#ifdef SHOW_VISIBILITY
                            rb->show();
#endif
                            drawRect(_painter,
                                 rb->_sx, rb->_sy, rb->_swidth, rb->_sheight);
                            _rcnt++;
                            checkLabel(_painter,rb,xoff, yoff);
                        }
                        ++(*swire_it);
                    }
                    if (swire_it != NULL)
                        delete swire_it;
                }

#ifdef VIA_PTR
                if ( _power_via_ptr->status == 1 )
                {
#endif
                    QPtrListIterator<RenderBox> *via_it = ds->getPViaIt(i);
                    while( (rb = via_it->current()) != 0 )
                    {
                        if ( checkViaStatus(*rb) == 1)
                        {
                            rb->scaleAbsolute( _xres,_yres, _bbox._wx, 
                             _bbox._wy, _bbox._wheight, sx, sy,
                             xoff, yoff);
                            drawVia(_painter,rb);

                            _vcnt++;
                            checkLabel(_painter,rb,xoff, yoff);
                        }
                        ++(*via_it);
                    }
                    if (via_it != NULL)
                        delete via_it;
#ifdef VIA_PTR
                }
#endif

#ifdef VIA_PTR
                if ( _signal_via_ptr->status == 1 )
                {
#endif
                    QPtrListIterator<RenderBox> *via_it = ds->getSViaIt(i);
                    while( (rb = via_it->current()) != 0 )
                    {
                        if ( checkViaStatus(*rb) == 1)
                        {

                            rb->scaleAbsolute( _xres,_yres, _bbox._wx, 
                             _bbox._wy, _bbox._wheight, sx, sy,
                             xoff, yoff);
                            drawVia(_painter,rb);

                            _vcnt++;
                            checkLabel(_painter,rb,xoff, yoff);
                        }
                        ++(*via_it);
                    }
                    if (via_it != NULL)
                        delete via_it;
#ifdef VIA_PTR
                }
#endif
                _painter->setPen(*_cm->getPen(i));
                _painter->setBrush( *_cm->getBrush(i) );
                QPtrListIterator<RenderBox>* poly_it = ds->getPolyIt(i);
                while( (rb = poly_it->current()) != 0 )
                {
#ifdef SHOW_VISIBILITY
                fprintf(stderr,"poly visibility: %d\n",checkStatus(*rb));
#endif
                    if ( checkStatus(*rb) == 1)
                    {

                        rb->scaleAbsolutePoly( _xres,_yres, _bbox._wx, 
                            _bbox._wy, _bbox._wheight, sx, sy,
                            xoff, yoff);

                        _painter->drawPolygon(*rb->_pscreen);
                        _pcnt++;
                        checkPolyLabel(_painter,rb, xoff, yoff);
                    }
                    ++(*poly_it);
                }
                if (poly_it != NULL)
                    delete poly_it;

                _painter->setPen( *_cm->getPen(i) );
                _painter->setBrush( *_cm->getBrush(i) );

                box_it = ds->getBoxIt(i);
                while( (rb = box_it->current()) != 0 )
                {
                    if ( checkStatus(*rb) == 1 )
                    {

                        checkColor(_painter,rb);
#ifdef SHOW_VISIBILITY
                        rb->show();
                        qDebug("Type: %s\n",rb->_type->val);
#endif
                        if( (rb->_type == larrow))
                        {
                            //qDebug("Left Arrow!\n");
                            rb->scaleArrow( _xres,_yres, _bbox._wx, 
                            _bbox._wy, _bbox._wheight, sx, sy,
                            xoff, yoff);
                            drawArrow(_painter,rb,1);
                            _rcnt++;
                        }
                        else if( (rb->_type==rarrow) )
                        {
                            //qDebug("Right Arrow!\n");
                            rb->scaleArrow( _xres,_yres, _bbox._wx, 
                            _bbox._wy, _bbox._wheight, sx, sy,
                            xoff, yoff);
                            drawArrow(_painter,rb,0);
                            _rcnt++;
                        }

                        // reset original brush/pen
                        if( rb->_color != NULL )
                        {
                            _painter->setPen( *_cm->getPen(i) );
                        }
                    }
                    ++(*box_it);
                }
                if( box_it != NULL)
                    delete box_it;
            }
        }
    }

#ifdef SHOW_TIME
    elapsed = timer.elapsed();
    qDebug("Elapsed time: %d, Objects: %d, Vias: %d, Polys: %d, Total: %d, Speed: %f\n",elapsed,_rcnt,_vcnt, _pcnt, _vcnt+_rcnt+_pcnt, ( (_vcnt + _rcnt + _pcnt)/ (float)elapsed)*1000);
    //qDebug("Dataset %s has %d objects\n",ds->getName(), ds->getObjectCount());
#endif

    return (_rcnt + _vcnt + _pcnt);
}

//#define SHOW_TIME_SEL
void RenderArea::mkSelPixmap(tagManager& datasets, tagManager& images, tagManager& tags)
{
    //qDebug("Window %d mk Sel Pixmap!\n", _ranum);
    if( _cm == NULL )
    {
        qDebug("No ColorMap - nothing to do!\n");
        return;
    }

#ifdef SHOW_TIME_SEL
    QTime timer;
    int elapsed;
    timer.start();
#endif
    // line here to see if we can suppress the "unused parameter" warning
    tagManager x = images;

    DataSet *ds = NULL;
    counter *c;
    int slen = slist.getTypeCount();

    int xoff = 0;
    int yoff = 0;

    //qDebug("mk Sel Pixmap - render!\n");
    int sx = _screen._sx;
    int sy = _screen._sy;

    bitBlt(_bitmap_sel,0,0,_bitmap_uns,0,0, -1, -1, Qt::CopyROP, false);
    _painter_sel->setPen(*_cm->getSelPen());
    QBrush *qb = _cm->getSelBrush();
    if( qb != NULL )
    {
        _painter_sel->setBrush(*qb);
    }
    else
    {
        _painter_sel->setBrush(Qt::NoBrush);
    }
    RenderBox * rb;

    // draw boxes
    counter *larrow = tags.findTagId("type","larrow");
    counter *rarrow = tags.findTagId("type","rarrow");

    _srcnt= 0, _svcnt=0, _spcnt=0;

    for(int i=0; i<MAX_LAYER; i++)
    {
        for(int slidx=0; slidx < slen; slidx++)
        {

            c = slist.getTagId(slidx);
            if( c->data != NULL )
            {
                sscanf((char*)(c->data),"%d %d",&xoff, &yoff);
            }
            else
            {
                xoff = 0;
                yoff = 0;
            }

            c = datasets.findTagId(c->val);

            if( (c == NULL) || (c->data == NULL) )
                continue;

            ds = (DataSet*)c->data;
#ifdef SHOW_VISIBILITY
            qDebug("Win %d, selecting Dataset %s, %d items\n",_ranum,c->val,ds->getObjectCount());
#endif


            QPtrListIterator<RenderBox> *via_it = ds->getPViaIt(i);
            while( (rb = via_it->current()) != 0 )
            {
                rb->scaleAbsolute( _xres,_yres, _bbox._wx, 
                 _bbox._wy, _bbox._wheight, sx, sy, xoff, yoff);

                if( (rb->_type == _bcut_ptr) || (rb->_type == _tcut_ptr) )
                {
                    drawVia(_painter_sel,rb);
                }
                else
                {
                    drawOutline(_painter_sel, rb->_sx, rb->_sy, rb->_swidth, rb->_sheight);
                }
                ++(*via_it);
                checkLabel(_painter_sel,rb, xoff, yoff);

                _svcnt++;
            }
            if (via_it != NULL)
                delete via_it;

            via_it = ds->getSViaIt(i);
            while( (rb = via_it->current()) != 0 )
            {
                rb->scaleAbsolute( _xres,_yres, _bbox._wx, 
                 _bbox._wy, _bbox._wheight, sx, sy, xoff, yoff);

                if( (rb->_type == _bcut_ptr) || (rb->_type == _tcut_ptr) )
                {
                    drawVia(_painter_sel,rb);
                }
                else
                {
                    drawOutline(_painter_sel, rb->_sx, rb->_sy, rb->_swidth, rb->_sheight);
                }
                ++(*via_it);
                checkLabel(_painter_sel,rb, xoff, yoff);
                _svcnt++;
            }
            if (via_it != NULL)
                delete via_it;

            QPtrListIterator<RenderBox>* poly_it = ds->getPolyIt(i);
            while( (rb = poly_it->current()) != 0 )
            {
                rb->scaleAbsolutePoly( _xres,_yres, _bbox._wx, 
                 _bbox._wy, _bbox._wheight, sx, sy, xoff, yoff);

                _painter_sel->drawPolygon(*rb->_pscreen);
                ++(*poly_it);
                checkPolyLabel(_painter_sel,rb, xoff, yoff);
                _spcnt++;
            }
            if (poly_it != NULL)
                delete poly_it;

            QPtrListIterator<RenderBox> *box_it = ds->getBoxIt(i);
            while( (rb = box_it->current()) != 0 )
            {
                //qDebug("Selection:");
                //rb->show();

                if( rb->_color != NULL )
                {
                    QBrush qb(rb->_color->val);
                    QPen   qp(rb->_color->val);
                    _painter->setPen( qp);
                    _painter->setBrush( qb);
                }

                if( rb->_type == larrow) 
                {
                    rb->scaleArrow( _xres, _yres, _bbox._wx, _bbox._wy, 
                        _bbox._wheight, sx, sy, xoff, yoff);

                    drawArrow(_painter,rb,1);
                }
                else if  (rb->_type==rarrow) 
                {
                    rb->scaleArrow( _xres, _yres, _bbox._wx, _bbox._wy, 
                        _bbox._wheight, sx, sy, xoff, yoff);

                    drawArrow(_painter,rb,0);
                }
                else
                {
                    rb->scaleAbsolute( _xres,_yres, _bbox._wx, _bbox._wy, 
                        _bbox._wheight, sx, sy, xoff, yoff);
                    drawOutline(_painter_sel, rb->_sx, rb->_sy, rb->_swidth, rb->_sheight);
                    checkLabel(_painter_sel,rb, xoff, yoff);
                }

                if( rb->_color != NULL )
                {
                    _painter_sel->setPen(*_cm->getSelPen());
                    QBrush *qb = _cm->getSelBrush();
                    if( qb != NULL )
                    {
                        _painter_sel->setBrush(*qb);
                    }
                    else
                    {
                        _painter_sel->setBrush(Qt::NoBrush);
                    }
                }
                ++(*box_it);
                _srcnt++;
            }
            if (box_it != NULL)
                delete box_it;
        }
    }
#ifdef SHOW_TIME_SEL
    elapsed = timer.elapsed();
    if( ds != NULL && ds->getObjectCount() > 0) 
        qDebug("Elapsed Selection time for ds %s: %d, Objects: %d, Speed: %f\n",
            ds->getName(),elapsed, ds->getObjectCount(), 
            ( ds->getObjectCount()/ (float)elapsed)*1000);
#endif

}
