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

//Data Set - holds everything related to a set of objects

#include "dataset.h"

void DataSet::init_lists()
{
    _boxv = (QPtrList<RenderBox>**)malloc(sizeof(QPtrList<RenderBox>*)*MAX_LAYER);
    _swirev = (QPtrList<RenderBox>**)malloc(sizeof(QPtrList<RenderBox>*)*MAX_LAYER);
    _pviav = (QPtrList<RenderBox>**)malloc(sizeof(QPtrList<RenderBox>*)*MAX_LAYER);
    _sviav = (QPtrList<RenderBox>**)malloc(sizeof(QPtrList<RenderBox>*)*MAX_LAYER);
    _polyv = (QPtrList<RenderBox>**)malloc(sizeof(QPtrList<RenderBox>*)*MAX_LAYER);
    //fprintf(stderr,"New box/via/poly parents: %p, %p, %p\n", _boxv, _viav, _polyv);
    for(int i=0; i<MAX_LAYER; i++)
    {
        _boxv[i] = new QPtrList<RenderBox>;
        _boxv[i]->setAutoDelete(AUTO_DELETE);
        _swirev[i] = new QPtrList<RenderBox>;
        _swirev[i]->setAutoDelete(AUTO_DELETE);
        _pviav[i] = new QPtrList<RenderBox>;
        _pviav[i]->setAutoDelete(AUTO_DELETE);
        _sviav[i] = new QPtrList<RenderBox>;
        _sviav[i]->setAutoDelete(AUTO_DELETE);
        _polyv[i] = new QPtrList<RenderBox>;
        _polyv[i]->setAutoDelete(AUTO_DELETE);
    }

    _boxcnt = 0;
    _swirecnt = 0;
    _pviacnt = 0;
    _sviacnt = 0;
    _polycnt = 0;

    _x0 = LONG_MAX;
    _y0 = LONG_MAX;
    _x1 = LONG_MIN;
    _y1 = LONG_MIN;
    
}

void DataSet::free_layer(QPtrListIterator<RenderBox> *it)
{
    RenderBox *rb;
    while( (rb = it->current()) != 0)
    {
        pool_free(rb);
        ++(*it);
    }
}
#if 0
#endif
void DataSet::free_lists()
{
    QPtrListIterator<RenderBox> *it;
    for(int i=0; i<MAX_LAYER; i++)
    {
        it = getBoxIt(i);
        free_layer(it);
        delete it;

        it = getSWireIt(i);
        free_layer(it);
        delete it;

        it = getPViaIt(i);
        free_layer(it);
        delete it;

        it = getSViaIt(i);
        free_layer(it);
        delete it;

        it = getPolyIt(i);
        free_layer(it);
        delete it;

#if 0
#endif

        delete _boxv[i];
        delete _swirev[i];
        delete _pviav[i];
        delete _sviav[i];
        delete _polyv[i];
    }
    free(_boxv);
    free(_swirev);
    free(_pviav);
    free(_sviav);
    free(_polyv);

}

void DataSet::init(char *name)
{
    _name = strdup(name);
    //fprintf(stderr,"New DataSet: %s - %x!\n",name,this);

    init_lists();

}

void DataSet::checkBBox(RenderBox *rb)
{
    if( rb->_wx < _x0 )
        _x0 = rb->_wx;
    if( rb->_wy < _y0 )
        _y0 = rb->_wy;

    if( (rb->_wx + rb->_wwidth) > _x1 )
        _x1 = (rb->_wx + rb->_wwidth);
    if( (rb->_wy + rb->_wheight) > _y1 )
        _y1 = (rb->_wy + rb->_wheight);

}
void DataSet::checkPolyBBox(RenderBox *rb)
{
    unsigned int len = rb->_pworld->count();
    for(unsigned int i=0; i<len; i++)
    {
        rb->_pworld->point(i,&rb->_wx,&rb->_wy);
        checkBBox(rb);
    }

}


DataSet::DataSet(char *name) 
{
    if( name != NULL )
    {
        init(name);
    }
    else
    {
        init("none");
    }
    objectCount=0;
    startIdx=-1;
    endIdx=-1;
}
DataSet::DataSet()
{
    init("none");
}


DataSet::~DataSet()
{
#ifdef DEBUG_MEM
    fprintf(stderr,"DataSet Destructor!\n");
#endif
    free(_name);
#ifdef DEBUG_MEM
    fprintf(stderr,"DSD: free_lists();\n");
#endif
    free_lists();
#ifdef DEBUG_MEM
    fprintf(stderr,"DSD: done!\n");
#endif
};

void DataSet::addBox(int layer, RenderBox *rb) 
{ 
    _boxv[layer]->append(rb); 
    checkBBox(rb); 
}

void DataSet::addSWire(int layer, RenderBox *rb) 
{
    _swirev[layer]->append(rb);
    checkBBox(rb);  
}

void DataSet::addPVia(int layer, RenderBox *rb) 
{ 
    _pviav[layer]->append(rb);
    checkBBox(rb);  
}

void DataSet::addSVia(int layer, RenderBox *rb) 
{ 
    _sviav[layer]->append(rb);
    checkBBox(rb);  
}
void DataSet::addPoly(int layer, RenderBox *rb) 
{ 
    _polyv[layer]->append(rb);
    checkPolyBBox(rb);  
}

void DataSet::prePVia(int layer, RenderBox *rb) 
{ 
    _pviav[layer]->prepend(rb);
    checkBBox(rb);  
}

void DataSet::preSVia(int layer, RenderBox *rb) 
{ 
    _sviav[layer]->prepend(rb);
    checkBBox(rb);  
}

QPtrListIterator<RenderBox> *DataSet::getBoxIt(int layer) 
{ 
    QPtrListIterator<RenderBox> *box_it = new QPtrListIterator<RenderBox>(*_boxv[layer]); 
    return box_it;
};

QPtrListIterator<RenderBox> *DataSet::getSWireIt(int layer) 
{ 
    QPtrListIterator<RenderBox> *swire_it = new QPtrListIterator<RenderBox>(*_swirev[layer]); 
    return swire_it;
};

QPtrListIterator<RenderBox> *DataSet::getPViaIt(int layer) 
{ 
    QPtrListIterator<RenderBox> *via_it = new QPtrListIterator<RenderBox>(*_pviav[layer]); 
    return via_it;
};

QPtrListIterator<RenderBox> *DataSet::getSViaIt(int layer) 
{ 
    QPtrListIterator<RenderBox> *via_it = new QPtrListIterator<RenderBox>(*_sviav[layer]); 
    return via_it;
};

QPtrListIterator<RenderBox>* DataSet::getPolyIt(int layer) 
{ 
    QPtrListIterator<RenderBox> *poly_it = new QPtrListIterator<RenderBox>(*_polyv[layer]); 
    return poly_it;
};

RenderBox *DataSet::getBBox()
{
    _bbox._wx = _x0;
    _bbox._wy = _y0;
    _bbox._wwidth = (_x1 - _x0);
    _bbox._wheight = (_y1 - _y0);

    return &_bbox;
}

void DataSet::getBBox(char *buff)
{
    getBBox();

    if( (_boxcnt + _swirecnt + _pviacnt + _sviacnt  + _polycnt) == 0)
    {
        //fprintf(stderr,"getBBox for %s - empty\n",_name);
        sprintf(buff,"{}");
    } else {
        _bbox.getBBox(buff);
        //fprintf(stderr,"getBBox for %s - %s!\n",_name,buff);
    }
}

int DataSet::getObjectCount()
{
    return (int)(_boxcnt + _swirecnt + _pviacnt + _sviacnt + _polycnt);
}

char itembuff[4096];
void DataSet::appendItems(Tcl_Interp *interp, Tcl_Obj *list, 
        QPtrListIterator<RenderBox> *it, int poly)
{
    // no need to keep going here..
    if ( endIdx >= 0 && endIdx < objectCount )
    {
        return;
    }

    RenderBox *rb;
    while( (rb = it->current()) != 0)
    {
        if( (startIdx == -1) || 
                ( (startIdx <= objectCount) && (endIdx >= objectCount ) ) )
        {
            if( poly == 0)
            {
                sprintf(itembuff,"{id 0 type %s hierarchy %s bb {%d %d %d %d} layer %d",
                rb->_type->val, rb->_hierarchy->val, rb->_wx, rb->_wy, 
                (rb->_wx + rb->_wwidth), (rb->_wy+rb->_wheight), rb->_layer );
            }
            else
            {
                sprintf(itembuff,"{id 0 type %s hierarchy %s dd {",
                rb->_type->val, rb->_hierarchy->val);

                unsigned int len = rb->_pworld->count();
                int x,y;
                char numbuff[1024];
                for(unsigned int i=0; i<len; i++)
                {
                    rb->_pworld->point(i,&x,&y);
                    sprintf(numbuff,"%d %d",x,y);
                    strcat(itembuff,numbuff);

                    // for no space at end of }
                    if( i != (len-1) )
                        strcat(itembuff," ");
                }
                sprintf(numbuff,"} layer %d",rb->_layer);
                strcat(itembuff,numbuff);
            }

            if( rb->_color != NULL && strlen(rb->_color->val) >0 )
            {
                strcat(itembuff," color ");
                strcat(itembuff,rb->_color->val);
            }
            if( rb->_tags != NULL && strlen(rb->_tags->val)>0 )
            {
                strcat(itembuff," tag {");
                strcat(itembuff,rb->_tags->val);
                strcat(itembuff,"}");
            }
            if( rb->_marks != NULL  && strlen(rb->_marks->val)>0)
            {
                strcat(itembuff," mark {");
                strcat(itembuff,rb->_marks->val);
                strcat(itembuff,"}");
            }

            if( rb->_label != NULL )
            {
                strcat(itembuff," label {");
                strcat(itembuff,rb->_label);
                strcat(itembuff,"}");
            }
            if( rb->_lbbox != NULL )
            {
                strcat(itembuff," lbbox {");
                strcat(itembuff,rb->_lbbox);
                strcat(itembuff,"}");
            }
            strcat(itembuff," }");

            Tcl_Obj *obj = Tcl_NewStringObj(itembuff,strlen(itembuff));
            Tcl_ListObjAppendElement(interp,list,obj);
        }
        ++(*it);
        objectCount++;
        if ( objectCount > endIdx )
            break;
    }
}

Tcl_Obj *DataSet::getDataSet(Tcl_Interp *interp)
{
    Tcl_Obj *list = Tcl_NewListObj(0,NULL);

    QPtrListIterator<RenderBox> *it;
    for(int i=0; i<MAX_LAYER; i++)
    {
        it = getBoxIt(i);
        appendItems(interp,list,it,0);
        delete it;

        it = getSWireIt(i);
        appendItems(interp,list,it,0);
        delete it;

        it = getPViaIt(i);
        appendItems(interp,list,it,0);
        delete it;

        it = getSViaIt(i);
        appendItems(interp,list,it,0);
        delete it;

        it = getPolyIt(i);
        appendItems(interp,list,it,1);
        delete it;
    }

    return list;

}
Tcl_Obj *DataSet::getDataRange(Tcl_Interp *interp, int start, int end)
{
    objectCount=0;
    startIdx=start;
    endIdx=end;

    if ( endIdx > getObjectCount() )
        endIdx = getObjectCount()-1;

    return getDataSet(interp);
}

