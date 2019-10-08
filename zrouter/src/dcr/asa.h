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

#ifndef ADS_dcrASA_H
#define ADS_dcrASA_H

#include "tm.h"

#ifndef ADS_H
#include "ads.h"
#endif

#include "tcl.h"
#include "logger.h"
#include "b100.h"

//BEGIN_NAMESPACE_ADS

#define ASA_LAYER 17
#define ASA_HIER 18
#define ASA_TYPE 19
#define ASA_EQ 20

class dcrASA
{
    public:
        dcrASA();
        ~dcrASA();

        void setOutput(FILE *fp);
        void setOutput(Tcl_Interp *in);

        void append(char *x);
        void appendRecord(char *id, int layer, char *hierarchy, char *type, 
            int x0, int y0, int x1, int y1, char *special);
        void appendRecord(char *id, int x0, int y0, int x1, int y1, char *special);
        void output();

        void setList(bool type);

        void clear();
        void reset();

        void setLayer(int layer);
        void setHier(char *hier);
        void setType(char *type);

    private:

        typedef enum { UNKNOWN, INTERP, CHANNEL } DEST_TYPE;
        char fbuff[65536];
        char pbuff[65536];

        DEST_TYPE _dest;

        Tcl_Interp *_interp;
        FILE *_fp;
        bool _ls; // list style

        char *toList(char *x);
        void appendChar(char *x);
};

//END_NAMESPACE_ADS

#endif
