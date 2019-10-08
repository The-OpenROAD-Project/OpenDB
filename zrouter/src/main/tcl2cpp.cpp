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

#include "tcl2cpp.h"

char **Ath__tcl2Cpp::allocArray(uint m, uint n)
{
	char **A= new char*[m];
	for (uint ii= 0; ii<m; ii++)
		A[ii]= new char[n];

	return A;
}
Ath__tcl2Cpp::Ath__tcl2Cpp(uint m, uint n, uint k)
{
	_maxCmdCnt= 64;
	_cmdTable= allocArray(_maxCmdCnt, 128);;
	_useTable= allocArray(_maxCmdCnt, 128);;
	_cmdCnt= 0;;


	_args= allocArray(m, n);

	_argc= 0;

	_cmd= new char[k];

	_parse= new Ath__parser();
}
Ath__tcl2Cpp::~Ath__tcl2Cpp()
{
	for (uint k= 0; k<_cmdCnt; k++)
		delete _cmdTable[k];
	delete [] _cmdTable;

	for (uint ii= 0; ii<_maxWordCnt; ii++)
		delete [] _args[ii];
	delete [] _args;
	delete [] _cmd;

	delete _parse;
}

uint Ath__tcl2Cpp::defineCmd(char *name, char *use)
{
	strcpy(_cmdTable[_cmdCnt], use);
	strcpy(_useTable[_cmdCnt], use);

	_cmdCnt++;

	return _cmdCnt;
}
char **Ath__tcl2Cpp::getCmdTable()
{
	return _cmdTable;
}
	

char *Ath__tcl2Cpp::getTcl_string(uint ii)
{
	if (ii<_argc)
		return _args[ii];
	else
		return NULL;
}
int Ath__tcl2Cpp::getTcl_int(uint ii)
{
	if (ii<_argc)
		return atoi(_args[ii]);
	else
		return -2100000000; // TODO: what if this is user #
}
bool Ath__tcl2Cpp::getTcl_bool(uint ii)
{
	int n= atoi(_args[ii]);
	if (n>0)
		return true;
	else
		return false;
}
char** Ath__tcl2Cpp::checkArgs(int n, char *usage, Tcl_Interp *interp, 
			   int objc, Tcl_Obj *CONST objv[])
{
	int len;
	char *cmd = Tcl_GetStringFromObj(objv[0], &len);

	if (objc < n) {
		Tcl_AppendResult(interp,"usage:",cmd, usage,(char *) NULL);
		return NULL;
	}
	int ii;
	for (ii= 0; ii<objc; ii++) 
		strcpy(_args[ii], Tcl_GetStringFromObj(objv[ii], &len));
	for (ii= objc; ii<10; ii++)
		strcpy(_args[ii], "");
	
	_argc= objc;

	return _args;
}
uint Ath__tcl2Cpp::getOptionNum(char *option, char *val0, char *val1, char *val2, char *val3, char *val4)
{
	for (uint ii= 1; ii<_argc; ii++) {

		if (strcmp(_args[ii], option)!=0)
			continue;

		if ((val1!=NULL) && (strcmp(_args[ii+1], val1)==0))
			return 1;

		if ((val2!=NULL) && (strcmp(_args[ii+1], val2)==0))
			return 2;

		if ((val3!=NULL) && (strcmp(_args[ii+1], val3)==0))
			return 3;

		if ((val4!=NULL) && (strcmp(_args[ii+1], val4)==0))
			return 4;
	}
	return 0;
}
uint Ath__tcl2Cpp::getArgCnt()
{
	return _argc;
}
char* Ath__tcl2Cpp::getRequiredName(char *option)
{
	for (uint ii= 1; ii<_argc; ii++) {

		if (strcmp(_args[ii], option)!=0)
			continue;

		return _args[ii+1];
	}
	return NULL;
}
int Ath__tcl2Cpp::getInt(char *word, uint start)
{
	char buff[128];
	uint k= 0;
	for (uint ii= start; word[ii]!='\0'; ii++)
		buff[k++]= word[ii];
	buff[k++]= '\0';

	return atoi(buff);
}
int Ath__tcl2Cpp::getInt(char *option, int val, char *units1, char *units2, int *units)
{
	// TODO
	for (uint ii= 1; ii<_argc; ii++) {

		if (strcmp(_args[ii], option)!=0)
			continue;

		return atoi(_args[ii+1]);
	}

	return val;
}


/*
int tclReturn2(Tcl_Interp *interp, char*result, char *msg, char *err, char *success)
{
	if (interp==NULL) {
		return 1;
	}
	Tcl_SetVar2(interp,result,"msg",msg,0);
	Tcl_SetVar2(interp,result,"code",err,0);
	Tcl_AppendResult(interp, success,0); 
	
	return TCL_OK;
}
*/
/*
	if (!layoutTable.readLef(file))
		return tclReturn2(interp, result, "ERROR", "10", "0");
	else
		return tclReturn2(interp, result, "so many lines", "0", "1");
*/
