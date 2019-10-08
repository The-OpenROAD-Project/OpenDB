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

#ifdef _WIN32
#include "direct.h"
#endif

#include "layout.h"
#include "z2tcl.h"
#include "tcl2cpp.h"


#include "tech.h"

uint newSchemaFlag= 0;

static Ath__zroute *Z= NULL;

static Ath__tcl2Cpp *tcl2Cpp= NULL;

//-------------------------------------------------------------
Ath__zroute *Ath__getZtop()
//-------------------------------------------------------------
{
	assert(Z);
	return Z;
}
//-------------------------------------------------------------

void Ath__zroute_init(uint layerCnt, uint masterCnt)
{
#ifdef ZUI
	Z= new Ath__zroute(layerCnt, masterCnt, (uint) 200);
	assert(Z);
#endif
	
	tcl2Cpp= new Ath__tcl2Cpp(50, 512, 10000);
}
void Ath__init_new_schema()
{
	newSchemaFlag= 1;
	fprintf(stdout, "initializing new schema\n\n");
	Z->_dbSchema = 1;
}
char** Ath__check_args(int n, char *usage, Tcl_Interp *interp, 
			   int objc, Tcl_Obj *CONST objv[])
{
	return tcl2Cpp->checkArgs(2, usage, interp, objc, objv);
}

char *Ath__getOption_string(uint ii)
{
	return tcl2Cpp->getTcl_string(ii);
}

char* Ath__getOption_string_required(char *option)
{
	return tcl2Cpp->getRequiredName(option);
}
uint Ath__getOption_index(char *option, char *val0, char *val1, char *val2, char *val3, char *val4)
{
	return tcl2Cpp->getOptionNum(option, val0, val1, val2, val3, val4);
}

int Ath__getOption_int(uint ii)
{
	return tcl2Cpp->getTcl_int(ii);
}
int Ath__getOption_int(char *option, int val, char *units1, char *units2, int *units)
{
	return tcl2Cpp->getInt(option, val, units1, units2, units);
}

