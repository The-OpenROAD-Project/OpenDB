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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef DB1_H
#define DB1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atypes.h"
#include "parse.h"
#include "util.h"

class Ath__db
{
 private:
  uint _format;
  uint _charCnt;
  uint _blkCnt;

  char  _mode[3];
  char* _userDBdir;
  char* _tmpCurrentDir;

  char* _dbInfo;

  char*       _topDir;
  char*       _libDir;
  char**      _blockDirTable;
  Ath__parser _parse;

  FILE* _dbfp;
  FILE* _dbInfoFP;

  bool _singleDBfile;
  bool _closeFileFlag;

 public:
  Ath__db(char* dir, uint format, char* mode, FILE* fp, bool singleFile = NULL);
  ~Ath__db();
  void setStreamMode(char* mode);
  bool isMultipleFiles(uint blockCnt);

  FILE* openFile(char* name, char* mode);
  void  writeDesignName(char* filename);
  void  writeBlockCnt(uint cnt);
  uint  readBlockCnt();
  void  readDesignName(char* filename);
  FILE* openNextFP();
  FILE* openDesignFP(char* name);
  void  closeDesignFP();
  void  closeInfoFP();
  void  setLibDir();
  void  setTopDir(char* defBlockName = NULL);
  char* initFileName(char* name, uint n);
  char* initFileName2(char* name1, char* name2, uint n);
  void  initBlockNames(char* name, uint m, uint n);
  void  deallocBlockNames(uint m);
};

#endif
