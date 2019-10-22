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

/* Utility to convert numbers to base-100 for efficient transfer across
** the network (for zui)
** Author: Mattias Hembruch
*/

#include "b100.h"

void b100(int num, FILE *fp)
{
    int st = num;
    int d1;

    if( num == INT_MIN )
    {
        fputs("L@LK1",fp);
        return;
    }

    if ( num < 0 )
    {
        fputc(B100_MINUS,fp);
        st = -num;
    }

    while(1)
    {
        d1 =  st % 100;
        fputc(B100_START+d1,fp);
        if( (st /= 100) == 0) break;
    } 
}

char *unb100(char *enc, int *resout)
{
    char *ptr = enc;
    register int res = 0;
    int flag = 0;
    register int factor=1;
    if ( *ptr == B100_MINUS )
    {
        flag = 1;
        *ptr++;
    }
    while( *ptr >= B100_START )
    {
        res = res + (*ptr++-B100_START)*factor;
        factor *= 100;
    }
    if( flag )
    {
        res = -res;
    }
    *resout = res;

    // next position to read
    return ptr;
}

