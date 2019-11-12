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

#include "enumtype.h"
#include "itype.h"


Ath__itype::Ath__itype(unsigned int ii, unsigned int n)
{
	if (ii==0)
		setAll(0);
	else
		set(ii, n);
}
Ath__itype::Ath__itype(unsigned int type, unsigned int index, unsigned int num)
{
	setType(type);
	set(index, num);
}

unsigned int Ath__itype::getAll()
{
	return _all;
}	
unsigned int Ath__itype::getType()
{
	return _type;
}	
void Ath__itype::setAll(unsigned int n)
{
	_all= n;
}
void Ath__itype::setType(unsigned int type)
{
	_type= type;
}
void Ath__itype::setIndex(unsigned int ii)
{
	if (_type<24) {
		if (_type<16) {
			_t16._index= ii;
		}
		else {
			_t32._index= ii;
		}
	}
	else {
		if (_type==25) {
			_t128._index= ii;
		}
		else if (_type==26) {
			_t256._index= ii;
		}
		else if (_type==27) {
			_t512._index= ii;
		}
		else if (_type==28) {
			_t1k._index= ii;
		}
		else if (_type==29) {
			_t2k._index= ii;
		}
		else if (_type==30) {
			_t4k._index= ii;
		}
		else {
			_big._index= ii;
		}
	}
}
void Ath__itype::set(unsigned int ii, unsigned int n)
{
	if (_type<24) {
		if (_type<16) {
			_t16._index= ii;
			_t16._num= n;
		}
		else {
			_t32._index= ii;
			_t32._num= n;
		}
	}
	else {
		if (_type==25) {
			_t128._index= ii;
			_t128._num= n;
		}
		else if (_type==26) {
			_t256._index= ii;
			_t256._num= n;
		}
		else if (_type==27) {
			_t512._index= ii;
			_t512._num= n;
		}
		else if (_type==28) {
			_t1k._index= ii;
			_t1k._num= n;
		}
		else if (_type==29) {
			_t2k._index= ii;
			_t2k._num= n;
		}
		else if (_type==30) {
			_t4k._index= ii;
			_t4k._num= n;
		}
		else {
			_big._index= ii;
			_big._num= n;
		}
	}
}
unsigned int Ath__itype::get(unsigned int *index, unsigned int *num, unsigned int type)
{
	unsigned int ii;
	unsigned int n;
	
	if (type>31)
		type= _type;
	
	if (type<24) {
		if (type<16) {
			ii= _t16._index;
			n= _t16._num;
		}
		else {
			ii= _t32._index;
			n= _t32._num;
		}
	}
	else {
		if (type==25) {
			ii= _t128._index;
			n= _t128._num;
		}
		else if (type==26) {
			ii= _t256._index;
			n= _t256._num;
		}
		else if (type==27) {
			ii= _t512._index;
			n= _t512._num;
		}
		else if (type==28) {
			ii= _t1k._index;
			n= _t1k._num;
		}
		else if (type==29) {
			ii= _t2k._index;
			n= _t2k._num;
		}
		else if (type==30) {
			ii= _t4k._index;
			n= _t4k._num;
		}
		else {
			ii= _big._index;
			n= _big._num;
		}
	}
	*index= ii;
	*num= n;
	return _type;
}
unsigned int Ath__itype::cnt2type(unsigned int cnt)
{
	if (cnt<24) {
		return cnt;
	}
	else {
		if (cnt<=128) {
			return 25;
		}
		else if (cnt<=256) {
			return 26;
		}
		else if (cnt<=512) {
			return 27;
		}
		else if (cnt<=1024) {
			return 28;
		}
		else if (cnt<=2048) {
			return 29;
		}
		else if (cnt<=4096) {
			return 30;
		}
		else {
			return 31;
		}
	}
}
Ath__associateArray::Ath__associateArray(uint n, uint minWordSize, bool map)
{
	if (n<=0)
		n= 100;
	_maxWordCnt= n;
	_nameTable= new char*[_maxWordCnt];
	_wordTable= new  char*[_maxWordCnt];
	_flags= new bool[_maxWordCnt];

	if (_nameTable==NULL)
		ATH__failMessage("Cannot allocate array of char*");
	
	if (minWordSize<=0)
		minWordSize= 4;

	for (uint ii= 0; ii<_maxWordCnt; ii++) {
		_nameTable[ii]= ATH__allocCharWord(minWordSize);
		_wordTable[ii]= ATH__allocCharWord(minWordSize);
		_flags[ii]= false;
	}
	
	_map= NULL;
	if (map) 
	{
		_map= new uint[256];
		
		for (uint jj= 0; jj<256; jj++)
			_map[jj]= 0;
		
		_cnt= 0;
	}
	else {
		
		_cnt= 1;
	}
}
uint Ath__associateArray::getCnt() 
{ 
	return _cnt; 
}
Ath__associateArray::~Ath__associateArray() 
{
	for (uint ii= 0; ii<_maxWordCnt; ii++) {
		ATH__deallocCharWord(_nameTable[ii]);
		ATH__deallocCharWord(_wordTable[ii]);
	}
	delete [] _nameTable;
	delete [] _wordTable;
	delete [] _flags;
	if (_map!=NULL)
		delete [] _map;
}
uint Ath__associateArray::getWordMapIndex(char *word)
{
	uint mapIndex= (uint) word[0];
	return mapIndex;
}
void Ath__associateArray::setFlag(uint n)
{
	_flags[n]= true;
}
void Ath__associateArray::setFlagByName(char *name)
{
//	uint n= _map[getMapIndex(name_prefix)];
	uint n= getNameIndex(name);
	_flags[n]= true;
}
void Ath__associateArray::resetFlagByName(char *name)
{
//  uint n= _map[getMapIndex(name_prefix)];
	uint n= getNameIndex(name);

	_flags[n]= false;
}
void Ath__associateArray::setAllFlag()
{
	for (uint ii= 0; ii<_cnt; ii++)
		_flags[ii]= true;
}
void Ath__associateArray::resetAllFlag()
{
	for (uint ii= 0; ii<_cnt; ii++)
		_flags[ii]= false;
}
bool Ath__associateArray::getFlag(uint n)
{
	return _flags[n];
}
void Ath__associateArray::printAll(char *buff)
{
    buff[0] = 0;
	char *buff_alias = buff;

	for (uint ii=1; ii<_cnt; ii++) {
		sprintf(buff, "%s %s \"%s\" ",
			buff_alias, _nameTable[ii], _wordTable[ii]);
	}
}
void Ath__associateArray::getNameList(char *buff)
{
    buff[0] = 0;
	char *buff_alias = buff;

	for (uint ii= 0; ii<_cnt; ii++) {		
		sprintf(buff, "%s %s %s ", buff_alias, _nameTable[ii], _nameTable[ii]);
	}
	//sprintf(buff, "%s %s", buff, "\0");
}
uint Ath__associateArray::getTableIndex(char *word)
{
	uint mapIndex= getWordMapIndex(word);

	return _map[mapIndex];
}
uint Ath__associateArray::add(uint n, char *name, uint k)
{
	if (_map!=NULL)
	{
		_map[k]= n;
	}
	else
	{
		n= _cnt;
	}
	strcpy(_nameTable[n], name);
	_cnt++;

	return n;
}
uint Ath__associateArray::add(uint n, char *name, char *word)
{
	uint mapIndex= n;
	
	if (_map!=NULL)
	{
		mapIndex= getWordMapIndex(word);
		_map[mapIndex]= n;

		strcpy(_nameTable[n], name);
		strcpy(_wordTable[n], word);
		_cnt++;
		
		return n;
	}
	else {
		n= getNameIndex(name);
		if (n>0)
			return n;
		
		n= _cnt;
		strcpy(_nameTable[n], name);
		strcpy(_wordTable[n], word);
		_cnt++;
		
		return n;
	}
}
char* Ath__associateArray::getName(uint n) 
{ 
	return _nameTable[n]; 
}
char* Ath__associateArray::getWord(uint n) 
{ 
	return _wordTable[n]; 
}
uint Ath__associateArray::getNameIndex(char *name, int start) 
{ 
	if (start<0)
		start= 0;

	for (uint ii= start; ii<_cnt; ii++) {
		if (strcmp(_nameTable[ii], name)==0) 
			return ii;
	}
	return 0;
}

