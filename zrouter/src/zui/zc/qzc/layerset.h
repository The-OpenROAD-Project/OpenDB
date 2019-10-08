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

//Layer Set 
#ifndef _LAYERSET_H
#define _LAYERSET_H

#include "debug.h"
#include "globals.h"

#include "renderbox.h"


class LayerSet
{
    private:
        char *_name;

        void init_layers()
        {
            for(int i=0; i<MAX_LAYER; i++)
            {
                _layerstatus[i] = 1;
            }
        }

        void init(char *name)
        {
            _name = strdup(name);
            //fprintf(stderr,"New LayerSet: %s!\n",name);
            _maxlayer = 0;

            init_layers();
        }

        int _maxlayer;

        int _layerstatus[MAX_LAYER];

    public:

        LayerSet(char *name) 
        {
            if( name != NULL )
            {
                init(name);
            }
            else
            {
                init("none");
            }
        }
        LayerSet()
        {
            init("none");
        }


        ~LayerSet()
        {
#ifdef DEBUG_MEM
            fprintf(stderr,"LayerSet Destructor!\n");
#endif
            free(_name);
        };

        char *getName() { return _name; }

        void setLayerStatus(int layer, int status)
        {
            //qDebug("Status for Layer Before %d = %d\n",layer, _layerstatus[layer]);
            if( layer < MAX_LAYER )
            {
                 _layerstatus[layer] = status; 
                 setMaxLayer(layer);
            }

            //qDebug("Status for Layer After %d = %d\n",layer, _layerstatus[layer]);
        }

        int getLayerStatus(int layer)
        {
            if( layer < MAX_LAYER)
            {
                return _layerstatus[layer];
            }
            return 0;
        }

        int getMaxLayer()
        {
            return _maxlayer;
        }

        void setMaxLayer(int layer)
        {
            if( layer > _maxlayer )
            {
                _maxlayer = layer;
            }
        }

        void showLayerStatus()
        {
            for(int i=0; i<MAX_LAYER; i++)
            {
                qDebug("Layer %d: %d\n",i,getLayerStatus(i));
            }
        }
};

#endif
