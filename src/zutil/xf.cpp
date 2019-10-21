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

#include <stdio.h>
#include "xf.h"

void x(unsigned char *ptr)
{
    if ( (*ptr != '\n') && (*ptr != 'u') && (*ptr != 245)  && (*ptr != 138))
    {
        *ptr = (*ptr ^ 255);
        //*ptr = (*ptr ^ 42);
    }
}

void swapBits(unsigned char *ptr)
{
    unsigned char bottom = *ptr & 0x0F;
    unsigned char top = *ptr & 0xF0;
    *ptr = (bottom << 4) | (top >> 4);
}
void ec(unsigned char *ptr)
{
    swapBits(ptr);
    x(ptr);
}
void dc(unsigned char *ptr)
{
    x(ptr);
    swapBits(ptr);
}
void hide(char *buff, int len)
{
    //printf("Hide: %s,%d\n",buff,len);
    //char *sptr = buff;;
    char *eptr = (buff+len-1);
    char *ptr = buff;

    // preserve newlines
    while( ptr <= eptr )
    {
        ec((unsigned char *)ptr);
        ptr++;
    }
}
void unhide(char *buff, int len)
{
    //printf("UnHide: %d, %s\n",len,buff);
    //char *sptr = buff;;
    char *eptr = (buff+len-1);
    char *ptr = buff;

    // preserve newlines
    while( ptr <= eptr )
    {
        dc((unsigned char *)ptr);
        ptr++;
    }
}
