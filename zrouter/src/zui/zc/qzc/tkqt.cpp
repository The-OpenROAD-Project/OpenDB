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

#include "tkqt.h"
#include <stdlib.h>
#include <ctype.h>
#include "parse.h"


static char *alphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

unsigned char find_char(unsigned char c)
{
    unsigned char cnt=0;
    while( alphabet[cnt] != c )
    {
        cnt++;
    }
    return cnt;
}

int decode(char *in, char *out, int len, int final)
{
    char *ptr = in;
    char *end = in + len;

    int loop=0;
    if( final == 1)
    {
        // stop one sooner to catch any padding
        end -= 4;
        //fprintf(stderr,"Final: %s\n",end);
    }

    unsigned char b1, b2, b3, b4;
    unsigned char o1, o2, o3;
    while( ptr < end)
    {
        while (isspace(*ptr)) { ptr++; }
        b1 = find_char(*ptr++ & 0xff);

        while (isspace(*ptr)) { ptr++; }
        b2 = find_char(*ptr++ & 0xff);

        while (isspace(*ptr)) { ptr++; }
        b3 = find_char(*ptr++ & 0xff);

        while (isspace(*ptr)) { ptr++; }
        b4 = find_char(*ptr++ & 0xff);

        o1 = (b1 << 2) + ((b2 & 0x30) >> 4);
        o2 = (b2 << 4) + (b3 >> 2);
        o3 = (b3 << 6) + (b4);

        *out++ = o1;
        *out++ = o2;
        *out++ = o3;

        loop++;
    }
    //fprintf(stderr,"%d loops - %d bytes read, %d bytes written\n",loop,loop*4,loop*3);
    // see if we're done
    if( (end - ptr) != 0)
    {
        fprintf(stderr,"Error non-final: uneven # bytes: %d!\n",(int)(end-ptr));
        return -1;
    }
    else if (final == 1)
    {
        //fprintf(stderr,"Final check: %s\n",ptr);
        // check for padding;
        while (isspace(*ptr)) { ptr++; }
        b1 = find_char(*ptr++ & 0xff);

        while (isspace(*ptr)) { ptr++; }
        b2 = find_char(*ptr++ & 0xff);

        while (isspace(*ptr)) { ptr++; }
        b3 = find_char(*ptr++ & 0xff);

        while (isspace(*ptr)) { ptr++; }
        b4 = find_char(*ptr++ & 0xff);
        //fprintf(stderr,"Final padding check: %c %c %c %c!\n",b1,b2,b3,b4);

        // do NOT use '=' - = is position 64 in base64!!
        if( b4 == 64 )
        {
            // padding
            if( b3 == 64 )
            {
                //fprintf(stderr,"Two padding bytes - writing 1 char!\n");
                o1 = (b1 << 2) + ((b2 & 0x30) >> 4);
                *out++=o1;
                return (loop*3+1);
            }
            else
            {
                //fprintf(stderr,"One padding byte - writing 2 chars!\n");
                o1 = (b1 << 2) + ((b2 & 0x30) >> 4);
                o2 = (b2 << 4) + (b3 >> 2);
                *out++ = o1;
                *out++ = o2;
                return (loop*3+2);
            }
        }
        else
        {
            //fprintf(stderr,"No padding bytes - writing 3 chars!\n");
            o1 = (b1 << 2) + ((b2 & 0x30) >> 4);
            o2 = (b2 << 4) + (b3 >> 2);
            o3 = (b3 << 6) + (b4);

            *out++ = o1;
            *out++ = o2;
            *out++ = o3;
            return (loop*3+3);
        }

    }
    return (loop*3);
}

char buff[1024];
//Window-specific API 
void show(int ranum)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::show!\n",ranum); 
        return; 
    }

    qapp.ra[ranum]->show();
}

void newParent(int ranum, WId parent)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::newParent!\n",ranum); 
        return; 
    }
    //printf("New Parent: %x\n",parent);
    qapp.ra[ranum]->embedInto(parent);
}

void newWindow(int ranum, WId parent)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::newWindow!\n",ranum); 
        return; 
    }
    qapp.ra[ranum]->newWindow(parent);
}

void resize(int ranum, int x, int y)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::resize!\n",ranum); 
        return; 
    }

    //fprintf(stderr,"tkqt::resize for window %d!\n",ranum);
    qapp.ra[ranum]->resize(x,y);
}

void setChipBBox(int ranum, int x, int y, int width, int height)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::setBBox!\n",ranum); 
        return; 
    }
    qapp.ra[ranum]->setChipBBox(x,y,width,height);
}

void setBBox(int ranum, int x, int y, int width, int height)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::setBBox!\n",ranum); 
        return; 
    }
    qapp.ra[ranum]->setBBox(x,y,width,height);
}

void setSelection(int ranum, char *tag, int x, int y)
{
    qapp.ra[ranum]->setSelection(tag, x, y);
}

void refreshSelection(int ranum)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::refreshSelection!\n",ranum); 
        return; 
    }
    qapp.ra[ranum]->mkSelPixmap(qapp.datasets,qapp.images,qapp.tags);
}

void clear(int ranum)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::clear!\n",ranum); 
        return; 
    }

    qapp.ra[ranum]->clearWindow();
}

int refresh(int ranum)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::refresh!\n",ranum); 
        return 0; 
    }

#ifdef VIA_PTR
    qapp.ra[ranum]->setViaPtrs(qapp.tags);
#endif
    qapp.ra[ranum]->setSignalWirePtr(qapp.tags.getTagId("htype","signal/wire"));
    return qapp.ra[ranum]->mkPixmap(qapp.datasets,qapp.images, qapp.tags);
}


void paint(int ranum)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::paint!\n",ranum); 
        return; 
    }
    qapp.ra[ranum]->up2();
}

int newArea()
{
    qapp._racnt++;
    qapp.ra[qapp._racnt] = (RenderArea*)new RenderArea(qapp._racnt,0);

    return qapp._racnt;
}

void setScaleFactor(int ranum, double xsf, double ysf)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::setScaleFactor!\n",ranum); 
        return; 
    }
    qapp.ra[ranum]->setScaleFactor(xsf,ysf);
}

void setOverscanColor(int ranum, char *color)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::setOverscanColor!\n",ranum); 
        return; 
    }
    qapp.ra[ranum]->setOverscanColor(color);
}

void scroll(int ranum, int dx, int dy)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::scroll!\n",ranum); 
        return; 
    }
    qapp.ra[ranum]->scroll(dx, dy);
}

void resetScroll(int ranum)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::resetScroll!\n",ranum); 
        return; 
    }
    qapp.ra[ranum]->resetScroll();
}


void setLayerStatus(int ranum, int layer, int status)
{
    qapp.ra[ranum]->getLayerSet()->setLayerStatus(layer,status);
}

int getMaxLayers()
{
    return qapp.ils.getMaxLayer();
    //return qapp.ra[ranum]->getLayerSet()->getMaxLayer();
}

void world2Screen(int ranum, int x, int y)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::world2Screen!\n",ranum); 
        return; 
    }
    qapp.ra[ranum]->world2Screen(x,y);
    sprintf(buff,"%d %d",x,y);
    Tcl_SetResult(qapp.interp, buff, TCL_VOLATILE);
}
void screen2World(int ranum, int x, int y)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::screen2World!\n",ranum); 
        return; 
    }
    qapp.ra[ranum]->screen2World(x,y);
    sprintf(buff,"%d %d",x,y);
    Tcl_SetResult(qapp.interp, buff, TCL_VOLATILE);
}


//Dataset Specific API
void setBrush(int ranum, int layer, const char *color)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::setBrush!\n",ranum); 
        return; 
    }
    //qapp.ra[ranum]->getColorMap()->setBrush(layer,color);
    qapp.curCM->setBrush(layer,color);
}

void setPen(int ranum, int layer, const char *color)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::setPen!\n",ranum); 
        return; 
    }
    //qapp.ra[ranum]->getColorMap()->setPen(layer,color);
    qapp.curCM->setPen(layer,color);
}

void setSelBrush(int ranum, const char *color)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::setSelBrush!\n",ranum); 
        return; 
    }
    //qapp.ra[ranum]->getColorMap()->setSelBrush(color);
    qapp.curCM->setSelBrush(color);
}

void setSelPen(int ranum, int width, const char *color)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::setSelPen!\n",ranum); 
        return; 
    }
    //qapp.ra[ranum]->getColorMap()->setSelPen(width, color);
    qapp.curCM->setSelPen(width, color);
}

void selectColormap(char *tag)
{
    counter *c = qapp.cmaps.getTagId(tag);
    if( c->data != NULL )
    {
        qapp.curCM = (ColorMap *)c->data;
        //fprintf(stderr,"using existing Colormap: %p\n",qapp.curCM);
    }
    else
    {
        qapp.curCM = new ColorMap(tag);
        c->data = (void*)qapp.curCM;
        //fprintf(stderr,"using new Colormap: %p\n",qapp.curCM);
    }
}

void setSpecialColormap(int ranum, char *tag)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::setColormap!\n",ranum); 
        return; 
    }
    selectColormap(tag);
    qapp.ra[ranum]->setSpecialColorMap(qapp.curCM);
}


void setColormap(int ranum, char *tag)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::setColormap!\n",ranum); 
        return; 
    }
    selectColormap(tag);
    qapp.ra[ranum]->setColorMap(qapp.curCM);
}

void adjust(int ranum, int w, int l, int o, int y, int z)
{
    qapp.ra[ranum]->adjust(w,l,o,y,z);
}


void eraseDataset(char *tag)
{
    counter *c = qapp.datasets.getTagId(tag);
    if( c->data != NULL )
    {
        DataSet *ds = (DataSet *)c->data;
        delete ds;
        c->data = NULL;
    }
    qapp.datasets.erase(c->val);

    // get rid of marks on erase
    char buff[1024];
    sprintf(buff,"marks-%s",tag);
    qapp.tags.erase(buff);
}


DataSet *getDataset(char *tag)
{
    char buff[1024];
    counter *c = qapp.datasets.getTagId(tag);
    if( c->data == NULL )
    {
        c->data = (void*) new DataSet(tag);
        sprintf(buff,"marks-%s",tag);
        //fprintf(stderr,"New Dataset %s - %s\n",tag,buff);
        qapp.tags.getTagId(buff,"")->status = 1;
    }
    return (DataSet *)c->data;
}

Image *getImage(char *tag)
{
    counter *c = qapp.images.getTagId(tag);
    if( c->data == NULL )
    {
        //fprintf(stderr,"New Image %s\n",c->val);
        c->data = (void*) new Image(tag);
    }
    return (Image *)c->data;
}

void setDataset(int ranum, char *tag, int x, int y)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::setDataset!\n",ranum); 
        return; 
    }
    qapp.ra[ranum]->setDataset(tag,x,y);
}

void getDataSetData(char *tag)
{
    DataSet *ds = getDataset(tag);
    Tcl_SetObjResult(qapp.interp,ds->getDataSet(qapp.interp));
}

void getDataSetRange(char *tag, int start, int end)
{
    DataSet *ds = getDataset(tag);
    Tcl_SetObjResult(qapp.interp,ds->getDataRange(qapp.interp,start,end));
}

void getDataBBox(char *tag)
{
    DataSet *ds = getDataset(tag);
    ds->getBBox(buff);
    Tcl_SetResult(qapp.interp, buff, TCL_VOLATILE);
}

void getImageBBox(char *tag)
{
    Image *i = getImage(tag);
    i->getBBox(buff);
    Tcl_SetResult(qapp.interp, buff, TCL_VOLATILE);
}

void eraseImage(char *tag)
{
    counter *c = qapp.images.getTagId(tag);
    if( c->data != NULL )
    {
        Image *is = (Image *)c->data;
        delete is;
        c->data = NULL;
    }
    qapp.images.erase(c->val);
}


void setImage(int ranum, char *tag, int x, int y)
{
    qapp.ra[ranum]->setImage(tag,x,y);
}

void grabImage(int ranum, char *name)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::grabImage!\n",ranum); 
        return; 
    }

    Image *i = qapp.ra[ranum]->grabImage(name);
    counter *c = qapp.images.getTagId(name);

    if( c->data != NULL )
    {
        // was an image there
        delete (Image *)c->data;
    }
    c->data = (void*)i;

}

int getRenderStats(int ranum)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::getRenderStats!\n",ranum); 
        return -1; 
    }
    return qapp.ra[ranum]->getRenderCount();
}

int getSRenderStats(int ranum)
{
    if( qapp.ra[ranum] == NULL ) 
    { 
        qDebug("Window %d not initialized in tkqt::getSRenderStats!\n",ranum); 
        return -1; 
    }
    return qapp.ra[ranum]->getSRenderCount();
}

int getDatasetStats(char *tag)
{
    return getDataset(tag)->getObjectCount();
}

void setImageBBox(char *tag, int x0, int y0, int x1, int y1)
{
    Image *i = getImage(tag);
    i->getBBox()->setW(lower(x0,x1), lower(y0,y1),abs(x1-x0),abs(y1-y0));
}

void addImageChannel(char *tag, char *channel, int x0, int y0, int x1, int y1)
{
    Image *i = getImage(tag);
    i->getBBox()->setW(lower(x0,x1), lower(y0,y1),abs(x1-x0),abs(y1-y0));
    i->readChannel(channel,qapp.interp);
}

void addImage64(char *tag, char *data, int len, int x0, int y0, int x1, int y1, char *format = NULL)
{
    Image *i = getImage(tag);

    unsigned char *tmp = (unsigned char *)malloc(len);
    int l2 = decode(data,(char*)tmp,len,1);

    i->getBBox()->setW(lower(x0,x1), lower(y0,y1),abs(x1-x0),abs(y1-y0));
    i->loadData(tmp, l2, format);
    free(tmp);
}

void addImage(char *tag, unsigned char *bytearray, int len, int x0, int y0, int x1, int y1, char *format = NULL)
{
    Image *i = getImage(tag);

    i->getBBox()->setW(lower(x0,x1), lower(y0,y1),abs(x1-x0),abs(y1-y0));
    i->loadData(bytearray, len, format);
}

void updateLayers()
{
    for(int i=0; i<=qapp._racnt; i++)
    {
        //qDebug("Setting Layermax to: %d",qapp.ils.getMaxLayer());
        qapp.ra[i]->getLayerSet()->setMaxLayer(qapp.ils.getMaxLayer());
    }
}

void addObject(char *tag, char *line)
{
    DataSet *ds = getDataset(tag);
    init_idx(qapp.tags);
    addObject(line, ds, qapp.tags, qapp.ils);
    updateLayers();
}

void addObjects(char *tag, char *list)
{
    DataSet *ds = getDataset(tag);
    readList(list, ds, qapp.tags, qapp.ils);
    updateLayers();
}

int readFile(char *tag, char *channel)
{
    DataSet *ds = getDataset(tag);

    //readChannel(channel, qapp.interp, ds, qapp.tags, qapp.ils);
    readStream(channel, qapp.interp, ds, qapp.tags, qapp.ils);
    ds->getBBox(buff);
    updateLayers();
    return ds->getObjectCount();
    //qDebug("ds %s bbox: %s\n",ds->getName(),buff);
}

int getTagSize(char *type)
{
    return qapp.tags.getTagSize(type);
}

int getTagCount(char *type, int idx)
{
    return qapp.tags.getTagCount(type,idx);
}

/*
int getTypeCount(int idx)
{
    return qapp.tags.getTypeCount(idx);
}

char *getTypeName(int idx)
{
    return qapp.tags.getType(idx);
}
*/

char *getTagName(char *type, int idx)
{
    return qapp.tags.getTag(type,idx);
}

int getSliceSize()
{
    return qapp.tags.getTagSize("HType");
}

int getSliceCount(int slice)
{
    return qapp.tags.getTagCount("HType",slice);
}

char *getSliceName(int slice)
{
    return qapp.tags.getTag("HType",slice);
}

void setVisibility(char *tag, int status)
{
    qapp.tags.setTagStatus("HType",tag, status);
}

void setTypeStatus(char *type, int status)
{
    qapp.tags.setTypeStatus(type,status);
}

void setTagStatus(char *type, char *tag, int status)
{
    qapp.tags.setTagStatus(type,tag,status);
}

void dumpTags()
{
    qapp.tags.dump();
}
void dumpDSets()
{
    qapp.datasets.dump();
}


//General API


void QtEventSetup(ClientData data, int flags)
{

    // DUMMY
    flags=0;
    data=NULL;
    //

    static Tcl_Time maxDelay={0, 50000}; /* To process non-X events */

    Tcl_SetMaxBlockTime(&maxDelay);
}

int QtDoEvent(Tcl_Event *evPtr, int flags)
{
    // DUMMY
    flags=0;
    evPtr=NULL;
    //

    qapp.app->processEvents();
    return 1;
}


void QtEventCheck(ClientData data, int flags)
{
    // DUMMY
    flags=0;
    data=NULL;
    //
    Tcl_Event *event;

    if ( qapp.app->hasPendingEvents() )
    {
        event   = (Tcl_Event *)ckalloc(sizeof(Tcl_Event));
        event->proc = QtDoEvent;
        Tcl_QueueEvent(event, TCL_QUEUE_TAIL);
    }
}



int QtInit(Tcl_Interp *interp)
{
    qapp.interp = interp;

    Tcl_CreateEventSource(QtEventSetup, QtEventCheck, (ClientData)&qapp);
    pool_init();

    return TCL_OK;
}

