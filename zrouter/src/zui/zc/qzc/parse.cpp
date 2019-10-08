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

#include "parse.h"
#include "util.h"
#include "zlib.h"
#include <qdatetime.h>
#include "../../../dcr/asa.h"

#define SHOW_TIME
//#define SHOW_PARSING
//#define SHOW_EXACT

AthPool<RenderBox> *_pool;

void appendRB(RenderBox *rb,DataSet *ds);

void pool_free(RenderBox *rb)
{
    rb->un_init();
    _pool->free(rb);
}

RenderBox *pool_alloc()
{
    RenderBox *rb = _pool->alloc();
    rb->init();
    return rb;
}

char *hier[128];
char *tlist[128];

#define PREINIT 1000000
void pool_init()
{
    _pool = new AthPool<RenderBox>(false,PREINIT);
    RenderBox *rb;
    for(int i=0; i<PREINIT; i++)
    {
        rb = new RenderBox();
        pool_free(rb);
    }
    //qDebug("Pool init finished!");


    for( int i=0; i<128; i++)
    {
        hier[i] = tlist[i] = NULL;
    }
    hier['b'] = strdup("block");
    hier['i'] = strdup("inst");
    hier['s'] = strdup("signal");
    hier['p'] = strdup("power");

    tlist['a'] = strdup("rarrow");
    tlist['b'] = strdup("bbox");
    tlist['c'] = strdup("bcut");
    tlist['l'] = strdup("larrow");
    tlist['o'] = strdup("obs");
    tlist['p'] = strdup("pin");
    tlist['r'] = strdup("track");
    tlist['t'] = strdup("tcut");
    tlist['v'] = strdup("via");
    tlist['w'] = strdup("wire");



}


void init_idx(tagManager &tm);

char * skipval(char *s)
{
    char *ptr = s;
    // skip val
    int brace=0;
    while(*ptr != '\0')
    {
        if ( *ptr == '{' )
        {
            brace++;
        }
        else if ( *ptr == '}' )
        {
            brace--;
        }

        if ( (brace==0) && (*ptr == ' ') )
        {
            *ptr = '\0';
            return ++ptr;
        }
        ptr++;
    }
    return ptr;
}

char * skiplist(char *s)
{
    char *ptr = s;
    // skip tag
    // skip val
    int brace=0;
    while(*ptr != '\0')
    {
        if ( *ptr == '{' )
        {
            brace++;
        }
        else if ( *ptr == '}' )
        {
            brace--;
        }

        if ( (brace==0) && (*ptr == ' ' || (*ptr == '\n' )) )
        {
            *ptr = '\0';
            return ++ptr;
        }
        ptr++;
    }
    return ptr;
}

// find the first space past a starting point, 
// and then skip all consecutive spaces
char *pastspc(char *s)
{
    char *ptr = s;

    // find first space
    while(*ptr != ' ')
        ptr++;

    // skip all spaces
    while(*ptr == ' ')
        ptr++;

    return ptr;
}

char *pastspcn(char *s)
{
    char *ptr = s;

    // find first space
    while(*ptr != '\0' && *ptr != ' ')
        ptr++;

    // skip all spaces
    while(*ptr != '\0' && *ptr == ' ')
        ptr++;

    return ptr;
}


void readList(char *list, DataSet *ds, tagManager &tm, LayerSet &ls)
{

    int len = strlen(list);
    char *end = (list+len);
    char * ptr = list;
    char * cur;
    init_idx(tm);
    while( ptr < end )
    {
        cur = skiplist(ptr);
        ptr++;
        *(cur-2) = '\0';
        //fprintf(stderr,"Parsing: %s\n",ptr);
        addObject(ptr,ds,tm,ls);
        ptr = cur;
    }
}


void readStream2(char *channel, Tcl_Interp *interp, DataSet *ds, tagManager &tm, LayerSet &ls)
{
    int mode;
    int len = 1;
    Tcl_Channel ch = Tcl_GetChannel(interp, channel, &mode);

    Tcl_DString dstring;
    Tcl_DStringInit(&dstring);
    char *buff;
    buff = Tcl_DStringValue(&dstring);

    //qDebug("readStream: dataset: %s\n",ds->getName());
    init_idx(tm);
    while( (len = Tcl_Gets(ch,&dstring)) >= 0 )
    {
        if (len == 0 ) continue ;

        //strcpy(buff,Tcl_DStringValue(&dstring));
        if( len <= 3 && buff[0] == '.' )
        {
            //qDebug("break!\n");
            break;
        }
        char *ptr = buff;
        while( *ptr != '\0' && isspace(*ptr) )
        {
            ptr++;
        }
        char *sptr = ptr;

        if( *ptr != '\0' && *ptr == '{')
        {
            // 
            while( buff[--len] != '}');

            buff[len]='\0';

            sptr = ptr+1;
        }

        //qDebug("read line len: %d, <%s>\n",len,buff);
        addObjectFixed(sptr,ds,tm,ls);
        Tcl_DStringSetLength(&dstring,0);
    }
    Tcl_DStringFree(&dstring);

}

int linenum;
void readStream(char *channel, Tcl_Interp *interp, DataSet *ds, tagManager &tm, LayerSet &ls)
{
#ifdef SHOW_TIME
    int elapsed;
    QTime timer;
    timer.start();
#endif
    int mode;
    int len = 1;
    Tcl_Channel ch = Tcl_GetChannel(interp, channel, &mode);

    int fd, dfd;

    if( Tcl_GetChannelHandle(ch, mode, (ClientData*) &fd) != TCL_OK)
    {
        qDebug("Error getting channel!\n");
        return;
    }

    dfd = dup(fd);
    FILE *fp = fdopen(dfd, "r");
    //setlinebuf(fp);

    if( fp == NULL )
    {
        qDebug("Can't open file!\n");
        return;
    }
            
    char buff[32768];

    init_idx(tm);
    linenum=0;
    while( fgets(buff,10239, fp) != NULL )
    {
        //qDebug("readStream: %s!",buff);
        len = strlen(buff);
        if (len == 0 ) continue ;
        if( len <= 3 && buff[0] == '.' )
        {
            //qDebug("break!\n");
            break;
        }

        // strip off {}
        
        /*
        char *ptr = buff;
        while( *ptr != '\0' && isspace(*ptr) )
        {
            ptr++;
        }
        char *sptr = ptr;

        if( *ptr != '\0' && *ptr == '{')
        {
            // 
            while( buff[--len] != '}');

            buff[len]='\0';

            sptr = ptr+1;
        }
        */

        //qDebug("read line len: %d, <%s>\n",len,buff);
        //addObjectFixed(sptr,ds,tm,ls);
        addObjectFixedI(buff,ds,tm,ls);
        //addObject(sptr,ds,tm,ls);
        //addObject2(buff+1,ds,tm,ls);
        linenum++;
    }

    //qDebug("Read %d lines!\n",linenum);

    close(dfd);

    int idx=0;
    int c;
    while ((c = fgetc(fp)) != EOF )
    {
        buff[idx++] = c;
    }
    buff[idx]='\0';
    fclose(fp);
    c = Tcl_Ungets(ch,buff,idx,0);
#ifdef SHOW_TIME
    elapsed = timer.elapsed();
    int oc = ds->getObjectCount();
    float cnt = (float)oc/(float)elapsed;
    cnt *= 1000.0;

    qDebug("Parsed %d objects, %d millis, %f obj/s\n",oc,elapsed,cnt);
#endif
    //qDebug("After Streaming: dataset %s has %d objects\n",ds->getName(),ds->getObjectCount());
}
void readStreamz(char *channel, Tcl_Interp *interp, DataSet *ds, tagManager &tm, LayerSet &ls)
{
    int mode;
    int len = 1;
    Tcl_Channel ch = Tcl_GetChannel(interp, channel, &mode);

    int fd, dfd;

    if( Tcl_GetChannelHandle(ch, mode, (ClientData*) &fd) != TCL_OK)
    {
        qDebug("Error getting channel!\n");
        return;
    }

    dfd = dup(fd);
    gzFile fp = gzdopen(dfd, "r");
    //setlinebuf(fp);

    if( fp == NULL )
    {
        qDebug("Can't open file!\n");
        return;
    }
            
    char buff[32768];

    init_idx(tm);
    while( gzgets(fp,buff,10239) != NULL )
    {
        //qDebug("readStream: %s!",buff);
        len = strlen(buff);
        if (len == 0 ) continue ;
        if( len <= 3 && buff[0] == '.' )
        {
            //qDebug("break!\n");
            break;
        }

        // strip off {}
        
        char *ptr = buff;
        while( *ptr != '\0' && isspace(*ptr) )
        {
            ptr++;
        }
        char *sptr = ptr;

        if( *ptr != '\0' && *ptr == '{')
        {
            // 
            while( buff[--len] != '}');

            buff[len]='\0';

            sptr = ptr+1;
        }

        //qDebug("read line len: %d, <%s>\n",len,buff);
        addObjectFixed(sptr,ds,tm,ls);
        //addObject(sptr,ds,tm,ls);
        //addObject2(buff+1,ds,tm,ls);
    }

    close(dfd);

    int idx=0;
    int c;
    while ((c = gzgetc(fp)) != EOF )
    {
        buff[idx++] = c;
    }
    buff[idx]='\0';
    gzclose(fp);
    c = Tcl_Ungets(ch,buff,idx,0);
}

//define PARSE_DEBUG

int type_idx=-1;
int hier_idx=-1;
int htype_idx=-1;
int colors_idx=-1;
int tags_idx=-1;
int marks_idx=-1;

RenderBox cache;

counter *via_ptr;
counter *tcut_ptr;
counter *bcut_ptr;
counter *swire_ptr;
counter *hpower_ptr;
counter *hsignal_ptr;

counter *larrow_ptr;
counter *rarrow_ptr;

void init_idx(tagManager &tm)
{
    //qDebug("init idx!\n");

    type_idx=tm.getTagId("type")->id;
    hier_idx=tm.getTagId("hierarchy")->id;
    htype_idx=tm.getTagId("HType")->id;
    colors_idx=tm.getTagId("colors")->id;
    tags_idx=tm.getTagId("tags")->id;
    marks_idx=tm.getTagId("marks")->id;

    via_ptr = tm.getTagId("type","via");
    bcut_ptr = tm.getTagId("type","bcut");
    tcut_ptr = tm.getTagId("type","tcut");
    swire_ptr = tm.getTagId("htype","signal/wire");
    hsignal_ptr = tm.getTagId("hierarchy","signal");
    hpower_ptr = tm.getTagId("hierarchy","power");

    larrow_ptr = tm.getTagId("type","larrow");
    rarrow_ptr = tm.getTagId("type","rarrow");
    tm.getTagId("htype","inst/larrow")->status = 1;
    tm.getTagId("htype","inst/rarrow")->status = 1;


    cache._type = via_ptr;
    cache._hierarchy = hsignal_ptr;
    cache._htype = swire_ptr;

    cache._color = tm.getTagId("type");
    cache._tags = tm.getTagId("type");
    cache._marks = tm.getTagId("type");

    //qDebug("id summary: %d, %d, %d, %d, %d, %d",type_idx, hier_idx, htype_idx, colors_idx, tags_idx, marks_idx);
}

void addObject(char *line, DataSet *ds, tagManager& tm, LayerSet &ls)
{
    char *sptr = line;

    int llen = strlen(line);

    // trim leading spaces
    
    if( llen <= 1) return;

    int x0 = -1;
    int y0 = -1;
    int x1 = -1;
    int y1 = -1;;
    int layer = 0;
    char *layerptr = NULL;
    char *type = NULL;
    char *hierarchy = NULL;
    char *color = NULL;

    char tags[2048];
    char marks[2048];
    char label[2048];

    char *tagptr = NULL;
    char *markptr = NULL;
    char *labelptr = NULL;
    char *lbbptr = NULL;

    char *ddptr = NULL;;

    tags[0] = '\0';
    marks[0] = '\0';
    label[0] = '\0';


    while( *sptr != '\0' )
    {
        if( isspace(*sptr) ) 
        {
            sptr++;
            continue;
        }

        //fprintf(stderr,"Inner Parsing: %s\n",sptr);

        switch( *sptr )
        {
            case 'b':
                // bbox
#ifdef PARSE_DEBUG
                qDebug("bbox: %s\n",sptr);
#endif
                //sptr = pastspc(sptr+1);
                sptr += 3;
#ifdef PARSE_DEBUG
                qDebug("bbox 2: %s\n",sptr);
#endif
                if ( 4 != sscanf(sptr,"{%d %d %d %d}",&x0,&y0,&x1,&y1))
                {
                    fprintf(stderr,"bb: could not scan %s\n",sptr);
                }
                sptr = skipval(sptr);
                break;

            case 'c':
                // color
#ifdef PARSE_DEBUG
                qDebug("color: %s\n",sptr);
#endif
                //color = pastspc(sptr+5);
                color = sptr + 6;
                sptr = skipval(color);
                break;

            case 'd':
#ifdef PARSE_DEBUG
                qDebug("dd: %s\n",sptr);
#endif
                //ddptr = pastspc(sptr+2);
                ddptr = sptr + 3;
                sptr = skipval(ddptr);
                break;

            case 'h':
#ifdef PARSE_DEBUG
                qDebug("hierarchy: %s\n",sptr);
#endif
                // hierarchy
                //hierarchy = pastspc(sptr+9);
                hierarchy = sptr + 10;
                sptr = skipval(hierarchy);
                break;
                
            case 'i':
#ifdef PARSE_DEBUG
                qDebug("id: %s\n",sptr);
#endif
                // skip id
                //sptr = pastspc(sptr+2);
                sptr += 3;
                sptr = skipval(sptr);
                break;

            case 'l':
#ifdef PARSE_DEBUG
                qDebug("layer/label/lbbox: %s\n",sptr);
#endif
                if( strncmp(sptr,"layer",5) == 0)
                {
                    // layer
                    //layerptr = pastspc(sptr+5);
                    layerptr = sptr + 6;
                    layer = atoi(layerptr);
                    sptr = skipval(layerptr);
                }
                else if (strncmp(sptr,"label",5) == 0)
                {
                    labelptr = pastspcn(sptr+4);
                    //fprintf(stderr,"labelptr: %s\n",labelptr);
                    sptr = skiplist(labelptr);
                    //fprintf(stderr,"sptr: %s\n",sptr);
                    if( labelptr[0] == '{' )
                    {
                        labelptr++;
                        char *tptr = sptr;
                        while( *--tptr != '}' );
                        *(tptr)='\0';
                    }
                }
                else if(strncmp(sptr,"lbbox",5) == 0)
                {
                    // lbbox
                    lbbptr = pastspcn(sptr+4);
                    sptr = skiplist(lbbptr);

                    if( lbbptr[0] == '{' )
                    {
                        lbbptr++;
                        char *tptr = sptr;
                        while( *--tptr != '}' );
                        *(tptr)='\0';
                    }
                }
                else
                {
                    fprintf(stderr,"Unknown 'l' token: %s\n",sptr);
                    sptr = pastspc(pastspc(sptr+1)+1);
                }
                break;

            case 'm':
#ifdef PARSE_DEBUG
                qDebug("mark: %s\n",sptr);
#endif
                // mark
                markptr = pastspc(sptr+4);
                sptr = skipval(markptr);
                break;

            case 't':
                //tag or type
#ifdef PARSE_DEBUG
                qDebug("tag/type: %s\n",sptr);
#endif
                switch(*(sptr+1))
                {
                    case 'a':
                        // tag
                        tagptr = pastspc((sptr+3));
                        sptr = skipval(tagptr);
                        break;

                    case 'y':
                        // type
                        type = pastspc((sptr+4));
                        sptr = skipval(type);
                        break;

                    default:
                        fprintf(stderr,"Unknown 't' token: %s\n",sptr);
                        sptr = pastspc(pastspc(sptr+1)+1);
                        break;
                }
                break;

            default:
                char buf[1024]; int n;
                for(n=0;n<100;n++) 
                {
                    buf[n] = sptr[n];
                    if (sptr[n] == 0) break;
                }
                fprintf(stderr,"No match for attribute: %s...\n",buf);
                sptr = skipval(sptr);
                break;;
        }

        // catchup from last "skiptoken"
        if ( tagptr != NULL )
        {
            if (strlen(tags) != 0 )
            {
                strcat(tags," ");
            }
            strcat(tags,tagptr);
            tagptr=NULL;
        }
        if ( markptr != NULL )
        {
            if (strlen(marks) != 0 )
            {
                strcat(marks," ");
            }
            strcat(marks,markptr);
            markptr=NULL;
        }
        if ( labelptr != NULL )
        {
            if (strlen(label) != 0 )
            {
                strcat(label,"\n");
            }
            strcat(label,labelptr);
            labelptr=NULL;
        }
    }

    //fprintf(stderr,"Total single parsing: type: %s, hierarchy: %s, layer: %d, bbox: %d,%d %d,%d tag(s): %s, marks(s): %s, color: %s, label: %s, lbbox: %s\n",type, hierarchy, layer, x0,y0,x1,y1,tags,marks, color, label, lbbptr);

    ls.setMaxLayer(layer);

    RenderBox *rb;

    counter *type_ptr; 
    if( strcmp(cache._type->val,type) == 0 )
    {
        type_ptr = cache._type;
    }
    else
    {
        type_ptr = tm.getTagId(type_idx,type);
        cache._type = type_ptr;
    }
    type_ptr->cnt++;

    rb = pool_alloc();
    rb->_layer = layer;

    if( x0 != -1)
    {
        //if( (strcasecmp(type,"larrow") == 0) || (strcasecmp(type,"rarrow") == 0) )
        if( (type_ptr == larrow_ptr) || (type_ptr == rarrow_ptr) )
        {
            //rb = new RenderBox(x0,y0, x1-x0, y1-y0, layer, NULL);
            rb->setW(x0,y0,x1-x0,y1-y0);
        }
        else
        {
            //rb = new RenderBox(lower(x0,x1),lower(y0,y1), abs(x1-x0), abs(y1-y0), layer, NULL);
            rb->setW(lower(x0,x1), lower(y0,y1), abs(x1-x0), abs(y1-y0));
        }
    }
    else if ( ddptr != NULL )
    {
        //qDebug("Poly box!\n");
        //rb = new RenderBox(0,0,0,0);
        //rb->_layer = layer;

        int pcount = 0;
        char * ptr = ddptr+1;
        long pt, pt2;
        char *eptr;
        //fprintf(stderr,"Before poly: %s\n",ddptr);
        while( *ptr != '\0' )
        {
            pt = strtol(ptr, &eptr, 10);
            pcount++;
            //fprintf(stderr,"Found poly points: %d, %d\n", pcount, pt);
            //fprintf(stderr,"poly: %p, %p, %c, %c\n", ptr, eptr, *ptr, *eptr);
            if( *eptr == '}' )
            {
                break;
            }
            ptr = eptr;

        }
        //fprintf(stderr,"%d poly points\n", pcount);
        if ( (pcount % 2) == 0 )
        {
            // found an even number of points
            rb->_pworld = new QPointArray( pcount / 2);
            uint pidx = 0;
            ptr = ddptr+1;
            while( *ptr != '\0' )
            {
                pt = strtol(ptr, &eptr, 10);
                if( *eptr == '}' )
                {
                    break;
                }
                ptr = eptr;

                pt2 = strtol(ptr, &eptr, 10);
                //qDebug("Setting points: %d, %d\n",pt,pt2);
                rb->_pworld->setPoint(pidx,pt,pt2);
                pidx ++;
                if( *eptr == '}' )
                {
                    break;
                }
                ptr = eptr;
            }
        }
    }
    else
    {
        fprintf(stderr,"Unknown data type:%s!\n",line);
        fprintf(stderr,"Coords (%s): %d,%d %d,%d, Layer: %d\n",ddptr,x0,y0,x1,y0,layer);
        fprintf(stderr,"type: %s\n",type);
        fprintf(stderr,"hierarchy: %s\n",hierarchy);
        fprintf(stderr,"tags: %s\n",tagptr);
        fprintf(stderr,"marks: %s\n",markptr);
        return;

    }

    rb->_type = type_ptr;

    if( strcmp(cache._hierarchy->val,hierarchy) == 0 )
    {
        rb->_hierarchy = cache._hierarchy;
    }
    else
    {
        rb->_hierarchy = tm.getTagId(hier_idx,hierarchy);
        cache._hierarchy =  rb->_hierarchy;
    }
    rb->_hierarchy->cnt++;

    char buff[1024];
    sprintf(buff,"%s/%s",hierarchy,type);

    //qDebug("PO: ht: %s\n",buff);
    if( strcmp(cache._htype->val, buff) == 0 )
    {
        rb->_htype = cache._htype;
    }
    else
    {
        rb->_htype = tm.getTagId(htype_idx,buff);
        cache._htype = rb->_htype;
    }
    rb->_htype->cnt++;

    if( strcmp(cache._tags->val,tags) == 0 )
    {
        rb->_tags = cache._tags;
    }
    else
    {
        rb->_tags = tm.getTagId(tags_idx,tags);
        cache._tags = rb->_tags;
    }
    rb->_tags->cnt++;

    if( color != NULL )
    {
        if( strcmp(cache._color->val,color) == 0)
        {
            rb->_color = cache._color;
        }
        else
        {
            rb->_color = tm.getTagId(colors_idx,color);
            cache._color = rb->_color;
        }
        rb->_color->cnt++;
    }
    
    sprintf(buff,"%s-%s","marks",ds->getName());
    if( strcmp(cache._marks->val,marks) == 0 )
    {
        rb->_marks = cache._marks;
    }
    else
    {
        rb->_marks = tm.getTagId(buff,marks);
        cache._marks = rb->_marks;
    }
    rb->_marks->cnt++;


    if( strlen(tags) >= 1 )
    {
        rb->_tags->status = 1;
    }
    if( strlen(marks) >= 1 )
    {
        rb->_marks->status = 1;
    }

    if( label[0] != '\0' )
    {
        rb->_label = strdup(label);

        if( lbbptr != NULL )
        {
            rb->_lbbox = strdup(lbbptr);
        }
    }
    appendRB(rb,ds);
}

int myatoi(char *s)
{
    //qDebug("Input: %s\n",s);
    int val=0;
    int sign=0;
    if (*s == '-') 
    {
        sign = 1;
        s++;
    }
    while( 1 )
    {
        val *=10;
        int n = (*s++)&15;
        //qDebug("new: %d\n",n);
        val += n;
        if( *s == ' ' || *s == 10 || *s == '\0')
            break;
    }

    if ( sign )
        return -val;
    else
    {
        //qDebug("Return: %d\n",val);
        return val;
    }
}

void addObjectFixed(char *line, DataSet *ds, tagManager& tm, LayerSet &ls)
{
    char *sptr = line;

    //int llen = strlen(line);

    // trim leading spaces
    
    //if( llen <= 1) return;
    if( sptr[0] == '\0' || sptr[1] == '\0') return;

    int x0 = INT_MIN;
    int y0 = INT_MIN;
    int x1 = INT_MIN;
    int y1 = INT_MIN;

    // layer does not need initialization
    int layer;

    char *type = NULL;
    char *hierarchy = NULL;
    char *color = NULL;

    char tags[2048];
    char marks[2048];
    char label[2048];

    char *tagptr = NULL;
    char *markptr = NULL;
    char *labelptr = NULL;
    char *lbbptr = NULL;

    char *ddptr = NULL;;

    tags[0] = '\0';
    marks[0] = '\0';
    label[0] = '\0';


    while( *sptr != '\0' && *sptr == ' ')
    {
        sptr++;
    }


    // order: layer, hierarchy, type bbox
    layer = myatoi(sptr);
    hierarchy = pastspc(sptr+1);
    type = pastspc(hierarchy+1);
    *(type-1)='\0';

    // bb {x0 y0 x1 y1};
    sptr = pastspc(type+1);
    *(sptr-1)='\0';

    x0 = myatoi(sptr);
    sptr = pastspc(sptr+1);
    y0 = myatoi(sptr);
    sptr = pastspc(sptr+1);
    x1 = myatoi(sptr);
    sptr = pastspc(sptr+1);
    y1 = myatoi(sptr);
    sptr = pastspcn(sptr+1);

    // handle rest of input
    while( *sptr != '\0' )
    {
        if( isspace(*sptr) ) 
        {
            sptr++;
            continue;
        }

        //fprintf(stderr,"Parsing: %s\n",sptr);

        switch( *sptr )
        {
            case 'c':
                // color
#ifdef PARSE_DEBUG
                qDebug("color: %s\n",sptr);
#endif
                color = sptr + 6;
                sptr = skipval(color);
                break;

            case 'd':
#ifdef PARSE_DEBUG
                qDebug("dd: %s\n",sptr);
#endif
                ddptr = sptr + 3;
                sptr = skipval(ddptr);
                break;

            case 'l':
#ifdef PARSE_DEBUG
                qDebug("label: %s\n",sptr);
#endif
                if( sptr[1] == 'a' )
                {
                    // label
                    labelptr = pastspcn(sptr+4);
                    sptr = skiplist(labelptr);

                    if( labelptr[0] == '{' )
                    {
                        labelptr++;
                        char *tptr = sptr;
                        while( *--tptr != '}' );
                        *(tptr)='\0';

                    }
                }
                else if( sptr[1] == 'b' )
                {
                    // lbbox
                    lbbptr = pastspcn(sptr+4);
                    sptr = skiplist(lbbptr);

                    if( lbbptr[0] == '{' )
                    {
                        lbbptr++;
                        char *tptr = sptr;
                        while( *--tptr != '}' );
                        *(tptr)='\0';
                    }
                }

                break;

            case 'm':
#ifdef PARSE_DEBUG
                qDebug("mark: %s\n",sptr);
#endif
                // mark
                markptr = pastspcn(sptr+4);
                sptr = skipval(markptr);
                break;

            case 't':
#ifdef PARSE_DEBUG
                qDebug("tag: %s\n",sptr);
#endif
                tagptr = pastspcn(sptr+3);
                sptr = skipval(tagptr);
                break;

            default:
                char buf[1024]; int n;
                for(n=0;n<100;n++) 
                {
                    buf[n] = sptr[n];
                    if (sptr[n] == 0) break;
                }
                fprintf(stderr,"No match for attribute (%c): <%s>...\n",*sptr,buf);
                sptr = skipval(sptr);
                break;;
        }

        // catchup from last "skiptoken"
        if ( tagptr != NULL )
        {
            if (strlen(tags) != 0 )
            {
                strcat(tags," ");
            }
            strcat(tags,tagptr);
            tagptr=NULL;
        }
        if ( markptr != NULL )
        {
            if (strlen(marks) != 0 )
            {
                strcat(marks," ");
            }
            strcat(marks,markptr);
            markptr=NULL;
        }
        if ( labelptr != NULL )
        {
            if (strlen(label) != 0 )
            {
                strcat(label,"\n");
            }
            strcat(label,labelptr);
            labelptr=NULL;
        }
    }

    //fprintf(stderr,"Total parsing: type: %s, hierarchy: %s, layer: %d, bbox: %d,%d %d,%d tag(s): %s, marks(s): %s, color: %s label: %s\n",type, hierarchy, layer, x0,y0,x1,y1,tags,marks, color, label);

    RenderBox *rb;

    rb = pool_alloc();

    ls.setMaxLayer(layer);

    rb->_layer = layer;

    if( ddptr == NULL)
    {
        rb->setW(x0,y0,x1,y1);
    }
    else if ( ddptr != NULL )
    {
        //qDebug("Poly box!\n");
        //rb = new RenderBox(0,0,0,0);
        //rb->_layer = layer;

        int pcount = 0;
        char * ptr = ddptr+1;
        long pt, pt2;
        char *eptr;
        //fprintf(stderr,"Before poly: %s\n",ddptr);
        while( *ptr != '\0' )
        {
            pt = strtol(ptr, &eptr, 10);
            pcount++;
            //fprintf(stderr,"Found poly points: %d, %d\n", pcount, pt);
            //fprintf(stderr,"poly: %p, %p, %c, %c\n", ptr, eptr, *ptr, *eptr);
            if( *eptr == '}' )
            {
                break;
            }
            ptr = eptr;

        }
        //fprintf(stderr,"%d poly points\n", pcount);
        if ( (pcount % 2) == 0 )
        {
            // found an even number of points
            rb->_pworld = new QPointArray( pcount / 2);
            uint pidx = 0;
            ptr = ddptr+1;
            while( *ptr != '\0' )
            {
                pt = strtol(ptr, &eptr, 10);
                if( *eptr == '}' )
                {
                    break;
                }
                ptr = eptr;

                pt2 = strtol(ptr, &eptr, 10);
                //qDebug("Setting points: %d, %d\n",pt,pt2);
                rb->_pworld->setPoint(pidx,pt,pt2);
                pidx ++;
                if( *eptr == '}' )
                {
                    break;
                }
                ptr = eptr;
            }
        }
    }
    else
    {
        fprintf(stderr,"Unknown data type:%s!\n",line);
        fprintf(stderr,"Coords (%s): %d,%d %d,%d, Layer: %d\n",ddptr,x0,y0,x1,y0,layer);
        fprintf(stderr,"type: %s\n",type);
        fprintf(stderr,"hierarchy: %s\n",hierarchy);
        fprintf(stderr,"tags: %s\n",tagptr);
        fprintf(stderr,"marks: %s\n",markptr);
        return;

    }
    if( strcmp(cache._hierarchy->val,hierarchy) == 0 )
    {
        rb->_hierarchy = cache._hierarchy;
    }
    else
    {
        rb->_hierarchy = tm.getTagId(hier_idx,hierarchy);
        cache._hierarchy =  rb->_hierarchy;
    }
    rb->_hierarchy->cnt++;

    if( strcmp(cache._type->val,type) == 0 )
    {
        rb->_type = cache._type;
    }
    else
    {
        rb->_type = tm.getTagId(type_idx,type);
        cache._type = rb->_type;
    }
    rb->_type->cnt++;


    // do the equivalent of: sprintf(buff,"%s/%s",hierarchy,type);
    char buff[1024];
    /*
    char *ptr = buff;
    char *eptr = hierarchy;
    */

    *(type-1)='/';

    //qDebug("PO: ht: %s\n",buff);
    if( strcmp(cache._htype->val, hierarchy) == 0 )
    {
        rb->_htype = cache._htype;
    }
    else
    {
        rb->_htype = tm.getTagId(htype_idx,hierarchy);
        cache._htype = rb->_htype;
    }
    rb->_htype->cnt++;

    if( strcmp(cache._tags->val,tags) == 0 )
    {
        rb->_tags = cache._tags;
    }
    else
    {
        rb->_tags = tm.getTagId(tags_idx,tags);
        rb->_tags->status = 1;
        cache._tags = rb->_tags;
    }
    rb->_tags->cnt++;

    if( strcmp(cache._marks->val,marks) == 0 )
    {
        rb->_marks = cache._marks;
    }
    else
    {
        // sprintf is slow, but this branch isn't taken very often..
        sprintf(buff,"%s-%s","marks",ds->getName());
        rb->_marks = tm.getTagId(buff,marks);
        rb->_marks->status = 1;
        cache._marks = rb->_marks;
    }
    rb->_marks->cnt++;

    if( color != NULL )
    {
        if( strcmp(cache._color->val,color) == 0)
        {
            rb->_color = cache._color;
        }
        else
        {
            rb->_color = tm.getTagId(colors_idx,color);
            cache._color = rb->_color;
        }
        rb->_color->cnt++;
    }
    
    if( label[0] != '\0' )
    {
        rb->_label = strdup(label);

        if( lbbptr != NULL )
        {
            rb->_lbbox = strdup(lbbptr);
        }
    }

    appendRB(rb,ds);

}

void addObjectFixedI(char *line, DataSet *ds, tagManager& tm, LayerSet &ls)
{
    static int xs = 0;
    static int ys = 0;
    static int xe = 0;
    static int ye = 0;


    char *sptr = line;

    while( (*sptr == '\r') || (*sptr == '\n') )
    {
        sptr++;
    }

    if( *sptr == '\0' || *(sptr+1) == '\0') return;

    int x0 = INT_MIN;
    int y0 = INT_MIN;
    int x1 = INT_MIN;
    int y1 = INT_MIN;

    char *color = NULL;

    char tags[2048];
    char marks[2048];
    char label[2048];

    char *tagptr = NULL;
    char *markptr = NULL;
    char *labelptr = NULL;
    char *lbbptr = NULL;

    char *ddptr = NULL;;

    tags[0] = '\0';
    marks[0] = '\0';
    label[0] = '\0';

    char ht[1024];


    static int layer = -1;
    static char *hierarchy = NULL;
    static char *type = NULL;

    //fprintf(stderr,"New Line: %s\n",sptr);
    switch(*sptr)
    {
        case ASA_LAYER:
            layer = atoi(sptr+1);
            //if( layer == 4)
                //layer = 11;
#ifdef SHOW_PARSING
            fprintf(stderr,"New Layer: %d\n",layer);
#endif
            return;

        case ASA_HIER:
            hierarchy = hier[*(sptr+1)];
#ifdef SHOW_PARSING
            fprintf(stderr,"New Hierarchy: %s\n",hierarchy);
#endif
            if( type != NULL )
            {
                strcpy(ht,hierarchy);
                strcat(ht,"/");
                strcat(ht,type);
            }
            return;

        case ASA_TYPE:
            type = tlist[*(sptr+1)];
#ifdef SHOW_PARSING
            fprintf(stderr,"New Type: %s\n",type);
#endif
            if( hierarchy != NULL )
            {
                strcpy(ht,hierarchy);
                strcat(ht,"/");
                strcat(ht,type);
            }
            return;

        case ASA_EQ:
#ifdef SHOW_PARSING
            fprintf(stderr,"Reset coords to 0!\n");
#endif
            xs = 0;
            ys = 0;
            xe = 0;
            ye = 0;
            sptr++;
            break;
    }

//#define NOB100
#ifdef SHOW_PARSING
    int oxs = xs;
    int oys = ys;
    int oxe = xe;
    int oye = ye;

    int dxs, dys, dxe, dye;
#endif

#ifdef NOB100
    x0 = myatoi(sptr)+xs;
    sptr = pastspc(sptr+1);
    y0 = myatoi(sptr)+ys;
    sptr = pastspc(sptr+1);
    x1 = myatoi(sptr)+xe;
    sptr = pastspc(sptr+1);
    y1 = myatoi(sptr)+ye;
    sptr = pastspcn(sptr+1);
#else

    sptr = unb100(sptr,&x0);
#ifdef SHOW_PARSING
    dxs = x0;
#endif
    x0 += xs;
    sptr = unb100(sptr+1,&y0);
#ifdef SHOW_PARSING
    dys = y0;
#endif
    y0 += ys;

    sptr = unb100(sptr+1,&x1);
#ifdef SHOW_PARSING
    dxe = x1;
#endif
    x1 += xe;
    sptr = unb100(sptr+1,&y1);
#ifdef SHOW_PARSING
    dye = y1;
#endif
    y1 += ye;
    sptr++;
#endif


    xs = x0;
    ys = y0;
    xe = x1;
    ye = y1;

    // handle rest of input
    while( *sptr != '\0' )
    {
        if( isspace(*sptr) ) 
        {
            sptr++;
            continue;
        }

        //fprintf(stderr,"Parsing: %s\n",sptr);

        switch( *sptr )
        {
            case 'c':
                // color
#ifdef PARSE_DEBUG
                qDebug("color: %s\n",sptr);
#endif
                color = sptr + 6;
                sptr = skipval(color);
                break;

            case 'd':
#ifdef PARSE_DEBUG
                qDebug("dd: %s\n",sptr);
#endif
                ddptr = sptr + 3;
                sptr = skipval(ddptr);
                break;

            case 'l':
#ifdef PARSE_DEBUG
                qDebug("label: %s\n",sptr);
#endif
                if( sptr[1] == 'a' )
                {
                    // label
                    labelptr = pastspcn(sptr+4);
                    sptr = skiplist(labelptr);

                    if( labelptr[0] == '{' )
                    {
                        labelptr++;
                        char *tptr = sptr;
                        while( *--tptr != '}' );
                        *(tptr)='\0';

                    }
                }
                else if( sptr[1] == 'b' )
                {
                    // lbbox
                    lbbptr = pastspcn(sptr+4);
                    sptr = skiplist(lbbptr);

                    if( lbbptr[0] == '{' )
                    {
                        lbbptr++;
                        char *tptr = sptr;
                        while( *--tptr != '}' );
                        *(tptr)='\0';
                    }
                }

                break;

            case 'm':
#ifdef PARSE_DEBUG
                qDebug("mark: %s\n",sptr);
#endif
                // mark
                markptr = pastspcn(sptr+4);
                sptr = skipval(markptr);
                break;

            case 't':
#ifdef PARSE_DEBUG
                qDebug("tag: %s\n",sptr);
#endif
                tagptr = pastspcn(sptr+3);
                sptr = skipval(tagptr);
                break;

            default:
               fprintf(stderr,"No match for attribute (%c) - (%d): offset %d, line %d\n",*sptr,*sptr,(int)(sptr-line),linenum);
               char *p2 = line;
               int idx=0;
               while(*p2!= '\n')
               {
                   fprintf(stderr,"%d: %c\n",idx++,*p2++);
               }
                
                sptr = skipval(sptr);
                break;
        }

        // catchup from last "skiptoken"
        if ( tagptr != NULL )
        {
            if (strlen(tags) != 0 )
            {
                strcat(tags," ");
            }
            strcat(tags,tagptr);
            tagptr=NULL;
        }
        if ( markptr != NULL )
        {
            if (strlen(marks) != 0 )
            {
                strcat(marks," ");
            }
            strcat(marks,markptr);
            markptr=NULL;
        }
        if ( labelptr != NULL )
        {
            if (strlen(label) != 0 )
            {
                strcat(label,"\n");
            }
            strcat(label,labelptr);
            labelptr=NULL;
        }
    }
#ifdef SHOW_PARSING
    //fprintf(stderr,"Total parsing: ds: %s type: %s, hierarchy: %s, layer: %d, bbox: %d,%d %d,%d tag(s): %s, marks(s): %s, color: %s label: %s\n",ds->getName(),type, hierarchy, layer, x0,y0,x0+x1,y0+y1,tags,marks, color, label);
    //fprintf(stderr,"Total: l: %d, bbox: %d,%d %d,%d (%d,%d %d,%d) (%d,%d %d,%d)\n",layer, x0,y0,x0+x1,y0+y1,oxs,oys,oxe,oye,dxs,dys,dxe,dye);
#endif
#ifdef SHOW_EXACT
    fprintf(stderr,"{id 0 type %s hierarchy %s bb {%d %d %d %d} layer %d}\n",type,hierarchy,x0,y0,x0+x1,y0+y1,layer);
#endif

    RenderBox *rb;

    rb = pool_alloc();

    ls.setMaxLayer(layer);

    rb->_layer = layer;

    if( ddptr == NULL)
    {
        rb->setW(x0,y0,x1,y1);
    }
    else if ( ddptr != NULL )
    {
        //qDebug("Poly box!\n");
        //rb = new RenderBox(0,0,0,0);
        //rb->_layer = layer;

        int pcount = 0;
        char * ptr = ddptr+1;
        long pt, pt2;
        char *eptr;
        //fprintf(stderr,"Before poly: %s\n",ddptr);
        while( *ptr != '\0' )
        {
            pt = strtol(ptr, &eptr, 10);
            pcount++;
            //fprintf(stderr,"Found poly points: %d, %d\n", pcount, pt);
            //fprintf(stderr,"poly: %p, %p, %c, %c\n", ptr, eptr, *ptr, *eptr);
            if( *eptr == '}' )
            {
                break;
            }
            ptr = eptr;

        }
        //fprintf(stderr,"%d poly points\n", pcount);
        if ( (pcount % 2) == 0 )
        {
            // found an even number of points
            rb->_pworld = new QPointArray( pcount / 2);
            uint pidx = 0;
            ptr = ddptr+1;
            while( *ptr != '\0' )
            {
                pt = strtol(ptr, &eptr, 10);
                if( *eptr == '}' )
                {
                    break;
                }
                ptr = eptr;

                pt2 = strtol(ptr, &eptr, 10);
                //qDebug("Setting points: %d, %d\n",pt,pt2);
                rb->_pworld->setPoint(pidx,pt,pt2);
                pidx ++;
                if( *eptr == '}' )
                {
                    break;
                }
                ptr = eptr;
            }
        }
    }
    else
    {
        fprintf(stderr,"Unknown data type:%s!\n",line);
        fprintf(stderr,"Coords (%s): %d,%d %d,%d, Layer: %d\n",ddptr,x0,y0,x1,y0,layer);
        fprintf(stderr,"type: %s\n",type);
        fprintf(stderr,"hierarchy: %s\n",hierarchy);
        fprintf(stderr,"tags: %s\n",tagptr);
        fprintf(stderr,"marks: %s\n",markptr);
        return;

    }
    if( strcmp(cache._hierarchy->val,hierarchy) == 0 )
    {
        rb->_hierarchy = cache._hierarchy;
    }
    else
    {
        rb->_hierarchy = tm.getTagId(hier_idx,hierarchy);
        cache._hierarchy =  rb->_hierarchy;
    }
    rb->_hierarchy->cnt++;

    if( strcmp(cache._type->val,type) == 0 )
    {
        rb->_type = cache._type;
    }
    else
    {
        rb->_type = tm.getTagId(type_idx,type);
        cache._type = rb->_type;
    }
    rb->_type->cnt++;


    // do the equivalent of: sprintf(buff,"%s/%s",hierarchy,type);
    char buff[1024];
    /*
    char *ptr = buff;
    char *eptr = hierarchy;
    */

    //*(type-1)='/';

    //qDebug("PO: ht: %s\n",buff);
    if( strcmp(cache._htype->val, ht) == 0 )
    {
        rb->_htype = cache._htype;
    }
    else
    {
        rb->_htype = tm.getTagId(htype_idx,ht);
        cache._htype = rb->_htype;
    }
    rb->_htype->cnt++;

    if( strcmp(cache._tags->val,tags) == 0 )
    {
        rb->_tags = cache._tags;
    }
    else
    {
        rb->_tags = tm.getTagId(tags_idx,tags);
        rb->_tags->status = 1;
        cache._tags = rb->_tags;
    }
    rb->_tags->cnt++;

    if( strcmp(cache._marks->val,marks) == 0 )
    {
        rb->_marks = cache._marks;
    }
    else
    {
        // sprintf is slow, but this branch isn't taken very often..
        sprintf(buff,"%s-%s","marks",ds->getName());
        rb->_marks = tm.getTagId(buff,marks);
        rb->_marks->status = 1;
        cache._marks = rb->_marks;
    }
    rb->_marks->cnt++;

    if( color != NULL )
    {
        if( strcmp(cache._color->val,color) == 0)
        {
            rb->_color = cache._color;
        }
        else
        {
            rb->_color = tm.getTagId(colors_idx,color);
            cache._color = rb->_color;
        }
        rb->_color->cnt++;
    }
    
    if( label[0] != '\0' )
    {
        rb->_label = strdup(label);

        if( lbbptr != NULL )
        {
            rb->_lbbox = strdup(lbbptr);
        }
    }

    appendRB(rb,ds);

}

void appendRB(RenderBox *rb, DataSet *ds)
{
    //qDebug("box:!"); x->show();
    //if ( strcasecmp(rb->_type->val,"via") != 0 )
    if(rb->_type == via_ptr) 
    {
        //if( strcasecmp(rb->_hierarchy->val,"power") == 0 )
        if( rb->_hierarchy == hpower_ptr)
        {
            // power
            // put vias first
            ds->prePVia(rb->_layer,rb);
            ds->incPViacnt();
        }
        //else if( strcasecmp(rb->_hierarchy->val,"signal") == 0 )
        else if( rb->_hierarchy == hsignal_ptr)
        {
            ds->preSVia(rb->_layer,rb);
            ds->incSViacnt();
        }
    }
    else if( (rb->_type == bcut_ptr) || (rb->_type == tcut_ptr))
    {
        if( rb->_hierarchy == hpower_ptr)
        {
            ds->addPVia(rb->_layer,rb);
            ds->incPViacnt();
        }
        else if( rb->_hierarchy == hsignal_ptr)
        {
            ds->addSVia(rb->_layer,rb);
            ds->incSViacnt();
        }
    }
    else
    {
        //if( strcasecmp(rb->_htype->val,"signal/wire") == 0)
        if(rb->_htype == swire_ptr)
        {
#ifdef DEBUG_APPEND
            qDebug("Signal Wire append!\n");
#endif
            ds->addSWire(rb->_layer,rb);
            ds->incSWirecnt();
        }
        else if( rb->_pworld == NULL )
        {
#ifdef DEBUG_APPEND
            qDebug("Other Object append: %s!\n",rb->_type->val);
#endif
            ds->addBox(rb->_layer,rb);
            ds->incBoxcnt();
        }
        else
        {
#ifdef DEBUG_APPEND
            qDebug("Poly append!\n");
#endif
            ds->addPoly(rb->_layer,rb);
            ds->incPolycnt();
        }
    }
}


int lower(int x0, int x1)
{
    if( x0 <= x1 )
    {
        return x0;
    }
    else
    {
        return x1;
    }
}

