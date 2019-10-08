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
#include <stdlib.h>
#include <assert.h>
#include "db.h"
#include "sver_names.h"

BEGIN_NAMESPACE_ADS

sver_names::sver_names()
{
  _blkNmax = 1;
  _blkN = 0;
  _blkV[0] = _p = (char*)malloc(32768);
  _pend = _p + 32768;
}

sver_names::~sver_names()
{
  int j;
  for (j=_blkNmax-1;j>=0;--j)
    free(_blkV[j]);
}

void sver_names::clear()
{
  _blkN = 0;
  _p = _blkV[0];
  _pend = _p + 32768;
}

char *sver_names::alloc(int len)
{
  char *ret = _p;
  _p += len;
  if (_p > _pend) {
    ++_blkN;
    if (_blkN == _blkNmax) {
      _blkV[_blkNmax++] = (char*)malloc(32768);
    }
    _p = _blkV[_blkN];
    _pend = _p + 32768;
    ret = _p;
    _p += len;
    if (_p > _pend) 
      return NULL;
  }
  return ret;
}

char *sver_names::store(const char *str)
{
  int len = 1+strlen(str);
  char *x = alloc(len);
  strcpy(x,str);
  return x;
}
  
int sver_names::get_used_kb()
{
  // kilobytes of used string space
  int kb = (_p - _blkV[_blkN] + 1023)/1024;
  if (_blkN) kb += 32*(_blkN-1);
notice(0,"_blkN %d\n",_blkN);
notice(0,"_p %d\n",_p - _blkV[_blkN]);
  return kb;
}

END_NAMESPACE_ADS
