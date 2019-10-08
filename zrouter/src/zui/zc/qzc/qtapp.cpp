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

#include "qtapp.h"


QtApp::QtApp() 
{ 

    counter *c;

    //for Debugging only..
    //int x=1; char *y="-sync";

    int x=0; char *y=NULL;
    app = new QApplication(x,&y);

    _racnt = -1;

    //dlist.setAuto(0);

    images.setAuto(0);
    //ilist.setAuto(0);

    datasets.setAuto(0);
    curDS = new DataSet("default");
    c = datasets.getTagId("default");
    c->data = (void*)curDS;

    cmaps.setAuto(0);
    curCM = new ColorMap("default");
    c = cmaps.getTagId("default");
    c->data = (void*)curCM;

    // initialize default tags for uncommon values
    tags.getTagId("tags","")->status = 1;
    //tags.getTagId("marks","")->status = 1;

    for(int i=0; i<MAX_RA; i++)
    {
        ra[i] = NULL;
    }

};

QtApp::~QtApp() 
{ 
#ifdef DEBUG_MEM
    fprintf(stderr,"QtApp destructor!\n");
#endif
    // get rid of tag entries in datasets so destructor works

    tmEraser<DataSet> ter;
    ter.eraseTM(datasets);
    //ter.eraseTM(dlist);

    tmEraser<ColorMap> tec;
    tec.eraseTM(cmaps);

    tmEraser<Image> tei;
    tei.eraseTM(images);
    //tei.eraseTM(ilist);

    for(int i=0; i<MAX_RA; i++)
    {
        if( ra[i] != NULL )
        {
            delete ra[i];
        }
    }
};

