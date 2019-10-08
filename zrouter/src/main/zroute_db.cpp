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

#include "ztop.h"

extern "C" int AthInitZroute(ClientData cdata, Tcl_Interp *interp, 
						   int objc, Tcl_Obj *CONST objv[])
{
	Ath__zroute_init(1000, 512);

	return TCL_OK;
}
extern "C" int AthLayersPrint(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(Ath__check_args(1, " name", interp, objc, objv))) {
		return TCL_ERROR;
	}

	return TCL_OK;
}
extern "C" int AthDbSchema(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	int len;
    char *options= Tcl_GetStringFromObj(objv[1], &len);
	int version=	atoi(Tcl_GetVar2(interp, options, "v", 0));

	if (version>0)
		Ath__init_new_schema();

	return TCL_OK;
}

extern "C" int AthMakeSearch(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{	
	int len;
    char *options= Tcl_GetStringFromObj(objv[1], &len);
	int force=	atoi(Tcl_GetVar2(interp, options, "force", 0));

	bool skipTopBlock= true;
	bool topOnly= false;
	if (atoi(Tcl_GetVar2(interp, options, "top", 0))>0) {
		topOnly= true;
		skipTopBlock= false;
	}
	bool isdb= false;
	if (atoi(Tcl_GetVar2(interp, options, "isdb", 0))>0) {
		isdb= true;
	}
	// TODO Block
#ifdef ZUI
	Ath__getZtop()->mkSearchDb(force, topOnly, skipTopBlock, isdb);
#endif
	return TCL_OK;
}
