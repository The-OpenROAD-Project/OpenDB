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

#include "z2tcl.h"
#include "tcl2cpp.h"

Ath__zroute *Ath__getZtop();

static Ath__tcl2Cpp *tcl2Cpp= NULL;

extern "C" int AthInitZroute(ClientData cdata, Tcl_Interp *interp, 
						   int objc, Tcl_Obj *CONST objv[])
{
	tcl2Cpp= new Ath__tcl2Cpp(50, 512, 10000);

	Ath__zroute_init(1000, 512);

	return TCL_OK;
}

static void tcl_append_v(Tcl_Interp *interp)
{
	uint boxCnt= Ath__getTclBoxCntV();
	if (boxCnt<=0) {
		Tcl_AppendResult(interp, "{}", (char*)NULL);
		return;
	}
	for (uint ii= 0; ii<boxCnt; ii++) {
		char *buf= Ath__getTclBoxV(ii);
		Tcl_AppendResult(interp, buf, (char*)NULL);
	}
}
static void tcl_append(Tcl_Interp *interp)
{
	uint boxCnt= Ath__getTclBoxCnt();
	if (boxCnt<=0) {
		Tcl_AppendResult(interp, "{}", (char*)NULL);
		return;
	}
	for (uint ii= 0; ii<boxCnt; ii++) {
		char *buf= Ath__getTclBox(ii);
		Tcl_AppendResult(interp, buf, (char*)NULL);
	}
}
extern "C" int zui_get(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	char *use= " type";

	if (!(tcl2Cpp->checkArgs(2, use, interp, objc, objv)))
		return TCL_ERROR;

	char *filter= tcl2Cpp->getTcl_string(1);
	char *bb= tcl2Cpp->getTcl_string(2);
	
	uint boxCnt= Ath__zui_get(filter);

	tcl_append(interp);

	return TCL_OK;
}
extern "C" int AthMakeTiles(ClientData cdata, Tcl_Interp *interp, int objc,
						   Tcl_Obj *CONST objv[])
{
	char *use= " rows cols ";

	if (!(tcl2Cpp->checkArgs(3, use, interp, objc, objv)))
		return TCL_ERROR;

	char *list= tcl2Cpp->getTcl_string(1);

	char *sepH= tcl2Cpp->getTcl_string(4);
	char *sepV= tcl2Cpp->getTcl_string(5);
	int rows= tcl2Cpp->getTcl_int(2);
	int cols= tcl2Cpp->getTcl_int(3);
	if (cols<0)
		cols= 0;

	return getTop()->makeTiles(list, rows, cols, sepH, sepV);
	/*
	if (!Ath__makeTiles(list, rows, cols, sepH, sepV))
		return TCL_ERROR;
	else
		return TCL_OK;
		*/
}
extern "C" int AthAssignTracks(ClientData cdata, Tcl_Interp *interp, int objc,
						   Tcl_Obj *CONST objv[])
{
	if (!(tcl2Cpp->checkArgs(1, " metH metV ", interp, objc, objv))) {
		return TCL_ERROR;
	}
	char *block= tcl2Cpp->getRequiredName("block");

	char *metH= tcl2Cpp->getRequiredName("metH");
	char *metV= tcl2Cpp->getRequiredName("metV");

	if (!Ath__assign_tracks(block, metH, metV))
		return TCL_ERROR;
	else
		return TCL_OK;
}
extern "C" int AthGetChip_zui(ClientData cdata, Tcl_Interp *interp, int objc,
							  Tcl_Obj *CONST objv[])
{
	char *use= " name bb filter ";

	if (!(tcl2Cpp->checkArgs(4, use, interp, objc, objv)))
		return TCL_ERROR;

	char *bb= tcl2Cpp->getTcl_string(2);
	char *filter= tcl2Cpp->getTcl_string(3);

	uint boxCnt= Ath__chip_get(bb, filter);
	
	tcl_append(interp);

	return TCL_OK;
}
extern "C" int AthNavigate_zui(ClientData cdata, Tcl_Interp *interp, int objc,
							  Tcl_Obj *CONST objv[])
{
	char *use= " name block ";

	if (!(tcl2Cpp->checkArgs(2, use, interp, objc, objv)))
		return TCL_ERROR;

	char *chip= tcl2Cpp->getTcl_string(1);
	char *objectName= tcl2Cpp->getTcl_string(2);
	char *bb= tcl2Cpp->getTcl_string(3);
	char *filter= tcl2Cpp->getTcl_string(4);

	Ath__navigate(objectName, bb);

	tcl_append_v(interp);

	return TCL_OK;
}
extern "C" int AthGetTiles(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	// ignore all for now : TODO

	// TODO *
	if (!(tcl2Cpp->checkArgs(1, " name bb filter ", interp, objc, objv)))
		return TCL_ERROR;

	char *bb= tcl2Cpp->getTcl_string(2);
	char *filter= tcl2Cpp->getTcl_string(3);
	
	uint boxCnt= Ath__tiles_get(bb);
	
	tcl_append(interp);

	return TCL_OK;
}


extern "C" int zui_chipAttr_get(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(tcl2Cpp->checkArgs(2, " name ", interp, objc, objv)))
		return TCL_ERROR;

	char layers[1024];
	char hierarchy[1024];
	char object_types[1024];

//3/15/04	char *hierarchy= "top block macro tile cell"; 
//3/15/04	char *object_types= "IOpin BLKpin Ipin line track via obs wire";

	int x1=0 , y1=0, x2=0, y2=0;
	if (Ath__chipAttr_get(&x1, &y1, &x2, &y2, layers,
		hierarchy, object_types)>0){


		char buf[1000];
		sprintf(buf, "bb { %d %d %d %d } layers { %s } objects { %s } hierarchy { %s }", 
			x1, y1, x2, y2, layers, object_types, hierarchy);
		Tcl_AppendResult(interp, buf, "\n", (char*)NULL);
	}
	else {
		Tcl_AppendResult(interp, "bb { 0 0 0 0 } \n", (char*)NULL);
	}

	return TCL_OK;
}
extern "C" int AthLayersPrint(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(tcl2Cpp->checkArgs(1, " name", interp, objc, objv))) {
		return TCL_ERROR;
	}
	uint layerCnt= Ath__print_routing_layers();

	return TCL_OK;
}	
extern "C" int AthPinsAssign(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(tcl2Cpp->checkArgs(1, " name", interp, objc, objv))) {
		return TCL_ERROR;
	}
	char *block= tcl2Cpp->getRequiredName("block");
	char *tile= tcl2Cpp->getRequiredName("tile");
	char *metH= tcl2Cpp->getRequiredName("metH");
	char *metV= tcl2Cpp->getRequiredName("metV");

	int connType=	tcl2Cpp->getOptionNum("connections", "all", "next", "jumper", "corner");

	int priority=	tcl2Cpp->getOptionNum("priority", "order", "length");

	int style=		tcl2Cpp->getOptionNum("style", "edge", "area", "stagger", "opt");

	int space_units;
	int space= tcl2Cpp->getInt("spacing", 1, "nm", "gp", &space_units);

	int width_units;
	int width= tcl2Cpp->getInt("width", 1, "nm", "gp", &width_units);

	int length_units;
	int length= tcl2Cpp->getInt("length", 5, "nm", "gp", &length_units);

	uint pinCnt= Ath__pins_assign(block, tile, connType, priority, style, 
		space, space_units, width, width_units, length, length_units,
		metH, metV);

	return TCL_OK;
}
extern "C" int AthBusMake(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(tcl2Cpp->checkArgs(1, " name", interp, objc, objv))) {
		return TCL_ERROR;
	}
	char *block= tcl2Cpp->getRequiredName("block");
	char *tile= tcl2Cpp->getRequiredName("tile");

	int opt= tcl2Cpp->getOptionNum("opt", "even", "xtalk");

	int space= 0;
	int width= 0;
	
	if (opt>0) {
		int space_units;
		space= tcl2Cpp->getInt("spacing", 0, "nm", "gp", &space_units);
		int width_units;
		width= tcl2Cpp->getInt("width", 0, "nm", "gp", &width_units);
	}
	uint pinCnt= Ath__bus_make(block, tile, opt, width, space);

	return TCL_OK;
}
extern "C" int AthTilesRoute(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(tcl2Cpp->checkArgs(1, " name", interp, objc, objv))) {
		return TCL_ERROR;
	}
	char *tile= tcl2Cpp->getRequiredName("tile");

	uint pinCnt= Ath__tiles_route(NULL, tile);

	return TCL_OK;
}

