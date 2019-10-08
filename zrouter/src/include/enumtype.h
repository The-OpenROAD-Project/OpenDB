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

#ifndef ENUMTYPE_H 
#define ENUMTYPE_H 


#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#include "atypes.h"


class Ath__associateArray
{
private:
	uint _maxWordCnt;
	char **_nameTable;
	char **_wordTable;
	uint _cnt;
	bool *_flags;
	uint *_map;

	
public:
	Ath__associateArray(uint n, uint minWordSize, bool map=true);
	uint getCnt();
	~Ath__associateArray() ;
	uint add(uint n, char *name, char *name_prefix);
	uint add(uint n, char *name, uint k);
	char* getName(uint n);
	char* getWord(uint n);
	uint getNameIndex(char *name, int start=-1);
	uint getTableIndex(char *word);

	uint getWordMapIndex(char *name_prefix);
	void setFlagByName(char *name);
	void resetFlagByName(char *name);
	void setFlag(uint n);
	void setAllFlag();
	void resetAllFlag();
	bool getFlag(uint n);
	void printAll(char *buff);
	void getNameList(char *buff);
};

#endif


