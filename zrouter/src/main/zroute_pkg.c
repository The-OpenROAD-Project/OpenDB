/*
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
*/

#include <tcl.h>

void ZrouterInit_db(Tcl_Interp *interp);
void ZrouterInit_zui(Tcl_Interp *interp);
void ZrouterInit_tiles(Tcl_Interp *interp);
// void ZrouterInit_groute(Tcl_Interp *interp);
void ZrouterInit_ext(Tcl_Interp *interp);
void ZTCL_InitModules(Tcl_Interp *interp);
void ZRouter_InitInterfaces();
void ZRouter_InitSta(Tcl_Interp *interp);

//static char* version=ZROUTER_VERSION;
#define ZROUTER_VERSION "1.0"

#ifndef _WIN32
#define _declspec(dllexport)
#endif


_declspec(dllexport)
int
Zrouter_Init(Tcl_Interp *interp)
{

	// the technology modules provide the zroute_api
	// so that zroute is used as the namespace to provide the value
	// added tcl code which uses zroute_tm (tm=technology module)
	if (Tcl_PkgProvideEx(interp, "zroute::tm", ZROUTER_VERSION,
		(ClientData) NULL) != TCL_OK) {
		return TCL_ERROR;
	}
	 ZrouterInit_db(interp);
     Zrouter_InitLogger(interp);
	//ZrouterInit_tiles(interp);
	// ZrouterInit_zui(interp);
	// ZrouterInit_groute(interp);
	//ZrouterInit_ext(interp);
	ZTCL_InitModules(interp);


    // no factories.cpp ZRouter_InitFactories();
    // ZRouter_InitSta(interp);

	return TCL_OK;
}

