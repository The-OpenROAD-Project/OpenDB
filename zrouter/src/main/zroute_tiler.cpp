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

#include "ztop.h"

extern "C" int AthMakeTBlocks(ClientData cdata, Tcl_Interp *interp, int objc,
						   Tcl_Obj *CONST objv[])
{
	int len;
    char *options= Tcl_GetStringFromObj(objv[1], &len);
	const char *list=	Tcl_GetVar2(interp, options, "block", 0);

	if (!Ath__getZtop()->makeTblocks((char *) list))
		return TCL_ERROR;
	else
		return TCL_OK;
}
extern "C" int AthMakeTiles(ClientData cdata, Tcl_Interp *interp, int objc,
						   Tcl_Obj *CONST objv[])
{
	char *use= " rows cols ";

	if (!(Ath__check_args(3, use, interp, objc, objv)))
		return TCL_ERROR;

	char *list= Ath__getOption_string(1);

	char *sepH= Ath__getOption_string(4);
	char *sepV= Ath__getOption_string(5);
	int rows= Ath__getOption_int(2);
	int cols= Ath__getOption_int(3);
	if (cols<0)
		cols= 0;

	if (!Ath__getZtop()->makeTiles(list, rows, cols, sepH, sepV))
		return TCL_ERROR;
	else
		return TCL_OK;
}
extern "C" int AthAssignTracks(ClientData cdata, Tcl_Interp *interp, int objc,
						   Tcl_Obj *CONST objv[])
{
	if (!(Ath__check_args(1, " metH metV ", interp, objc, objv))) {
		return TCL_ERROR;
	}
	char *block= Ath__getOption_string_required("block");

	char *metH= Ath__getOption_string_required("metH");
	char *metV= Ath__getOption_string_required("metV");

	if (!Ath__getZtop()->assign_tracks(block, metH, metV))
		return TCL_ERROR;
	else
		return TCL_OK;
}
extern "C" int AthPinsAssign(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(Ath__check_args(1, " name", interp, objc, objv))) {
		return TCL_ERROR;
	}
	char *block= Ath__getOption_string_required("block");
	char *tile= Ath__getOption_string_required("tile");
	char *metH= Ath__getOption_string_required("metH");
	char *metV= Ath__getOption_string_required("metV");

	int connType=	Ath__getOption_index("connections", "all", "next", "jumper", "corner");

	int priority=	Ath__getOption_index("priority", "order", "length");

	int style=		Ath__getOption_index("style", "edge", "area", "stagger", "opt");

	int space_units;
	int space= Ath__getOption_int("spacing", 1, "nm", "gp", &space_units);

	int width_units;
	int width= Ath__getOption_int("width", 1, "nm", "gp", &width_units);

	int length_units;
	int length= Ath__getOption_int("length", 5, "nm", "gp", &length_units);

	Ath__getZtop()->assign_pins(block, tile, connType, priority, style, 
		space, space_units, width, width_units, length, length_units,
		metH, metV);

	return TCL_OK;
}
extern "C" int AthBusMake(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(Ath__check_args(1, " name", interp, objc, objv))) {
		return TCL_ERROR;
	}
	char *block= Ath__getOption_string_required("block");
	char *tile= Ath__getOption_string_required("tile");

	int opt= Ath__getOption_index("opt", "even", "xtalk");

	int space= 0;
	int width= 0;
	
	if (opt>0) {
		int space_units;
		space= Ath__getOption_int("spacing", 0, "nm", "gp", &space_units);
		int width_units;
		width= Ath__getOption_int("width", 0, "nm", "gp", &width_units);
	}
	Ath__getZtop()->make_buses(block, tile, opt, width, space);

	return TCL_OK;
}
extern "C" int AthTilesRoute(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(Ath__check_args(1, " name", interp, objc, objv))) {
		return TCL_ERROR;
	}
	char *tile= Ath__getOption_string_required("tile");

	int row, col;
	int n= Ath__getZtop()->get2ints(tile, "_", &row, &col);
	if (n<=0) {
		row= -1;
		col= -1;
	}
//	uint pinCnt= Ath__getZtop()->getBlockById(1)->writeDefQuads("tile.def", row, col);

	return TCL_OK;
}

