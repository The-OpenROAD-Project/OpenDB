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

#include "asa.h"

using namespace ads;

BEGIN_NAMESPACE_ADS


static char _ath_logbuffer[1024];

void error(int code, const char *msg, ...)
{
    va_list args;
    va_start(args,msg);

    vsnprintf(_ath_logbuffer,sizeof(_ath_logbuffer),msg,args);
    va_end(args);

    fprintf(stderr,"Error: error %d: %s\n",code,_ath_logbuffer);
}


END_NAMESPACE_ADS

int main(int argc, char *argv[])
{

    dcrASA asa;

    FILE *f = fopen("asa.out","w");
    asa.setOutput(f);

    char buff[128];
    for(int i=0;i <100; i++)
    {
        sprintf(buff,"%d",i);
        asa.append(buff);
    }


    asa.setList(true);
    for(int i=100;i <200; i++)
    {
        sprintf(buff,"{%d} {%d} {%d %d}\n",i,i,i,i);
        asa.append(buff);
    }

    fclose(f);

    return 0;

}
