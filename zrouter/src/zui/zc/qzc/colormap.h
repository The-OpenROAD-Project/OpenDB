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

//Render Context - holds everything needed for a renderarea to display..
#ifndef _COLORMAP_H
#define _COLORMAP_H

#include "globals.h"
#include "debug.h"
#include <qpen.h>
#include <qbrush.h>
#include <stdlib.h>

class ColorMap
{
    private:
        char *_name;
        void init_brushpens()
        {
            for(int i=0; i<MAX_LAYER; i++)
            {
                _brushlist[i] = NULL;
                setBrush(i,Qt::red);
                _penlist[i] = NULL;
                setPen(i,Qt::green);
            }
        }

        void free_brushpens()
        {
            for(int i=0; i<MAX_LAYER; i++)
            {
                if ( _brushlist[i] != NULL )
                {
                    delete _brushlist[i];
                }
                if ( _penlist[i] != NULL )
                {
                    delete _penlist[i];
                }
            }

            if( _selPen != NULL )
            {
                delete _selPen;
            }
            if( _selBrush != NULL )
            {
                delete _selBrush;
            }
        }

        QBrush *_brushlist[MAX_LAYER];
        QPen *_penlist[MAX_LAYER];
        QBrush *_selBrush;
        QPen   *_selPen;

    public:


        ColorMap(char *name) 
        {
            if( name != NULL )
            {
                _name = strdup(name);
            }
            else
            {
                _name = strdup("none");
            }

            //fprintf(stderr,"New Colormap!\n");
            _selPen = NULL;
            _selBrush = NULL;
            init_brushpens();
        }

        ~ColorMap()
        {
#ifdef DEBUG_MEM
            fprintf(stderr,"Colormap Destructor!\n");
#endif
            free(_name);
#ifdef DEBUG_MEM
            fprintf(stderr,"CMD: free_brushpens();\n");
#endif
            free_brushpens();
#ifdef DEBUG_MEM
            fprintf(stderr,"CMD: done!\n");
#endif
        };

        char *getName() { return _name; }

        void setBrush(int layer, const char * color)
        {
            //printf("Setting brush for layer %d, window %d to color %s\n",layer, ranum, color);
            QColor c(color);
            setBrush(layer, c);
        }

        QBrush *getBrush(int layer)
        {
            if( layer < MAX_LAYER )
            {
                return _brushlist[layer];
            }
            return NULL;
        }


        void setBrush(int layer, QColor color)
        {
            if( layer < MAX_LAYER )
            {
                if( _brushlist[layer] != (QBrush*)NULL )
                    delete _brushlist[layer];

                _brushlist[layer] = new QBrush(color);
            }
        }

        void setPen(int layer, const char * color)
        {
            //printf("Setting pen for layer %d, window %d to color %s\n",layer, ranum, color);
            QColor c(color);
            setPen(layer, c);
        }

        QPen *getPen(int layer)
        {
            if( layer < MAX_LAYER )
            {
                return _penlist[layer];
            }
            return NULL;
        }

        void setPen(int layer, QColor color)
        {
            if( layer < MAX_LAYER )
            {
                if( _penlist[layer] != NULL )
                    delete _penlist[layer];

                _penlist[layer] = new QPen(color);
            }
        }

        QBrush *getSelBrush()
        {
            return _selBrush;
        }

        void setSelBrush(const char * color)
        {
            //printf("Setting Selection brush to color %s\n", color);
            QColor c(color);
            _selBrush = new QBrush(c);
        }

        QPen *getSelPen()
        {
            return _selPen;
        }

        void setSelPen(int width, const char * color)
        {
            //printf("Setting Selection pen to color %s, width %d\n", color, width);
            QColor c(color);
            _selPen = new QPen(c);
            _selPen->setWidth(width);
        }

};

#endif
