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

static void tcl_append(Tcl_Interp *interp, Ath__zui *zui)
{
	if (zui==NULL)
		return;

	if (zui->getTclBufFlag()) {
		Tcl_AppendResult(interp, zui->getTclBuf(), (char*)NULL);
		return;
	}
	Tcl_AppendResult(interp, "", (char*)NULL);

	uint boxCnt= zui->getZboxes(); // iterator

	uint lboxCnt = boxCnt > 50000 ? 50000 : boxCnt;

	if (getenv("ZROUTE_ZUI_LIMIT"))
		lboxCnt= atoi(getenv("ZROUTE_ZUI_LIMIT"));

	for (uint ii= 0; ii<lboxCnt; ii++) {
		char *buf= zui->getTclBbox(ii, false);;
		Tcl_AppendResult(interp, "{", buf, "}\n", (char*)NULL);
	}
}
extern "C" int AthGetChip_zui(ClientData cdata, Tcl_Interp *interp, int objc,
							  Tcl_Obj *CONST objv[])
{
    int len;
    char *options= Tcl_GetStringFromObj(objv[1], &len);

	//const char *block=	Tcl_GetVar2(interp, options, "chip", 0);
	const char *bb=	Tcl_GetVar2(interp, options, "bbox", 0);
	const char *layer=	Tcl_GetVar2(interp, options, "layer", 0);
	const char *type=	Tcl_GetVar2(interp, options, "type", 0);
	const char *hierarchy=	Tcl_GetVar2(interp, options, "hierarchy", 0);
	const char *noclip=	Tcl_GetVar2(interp, options, "noclip", 0);
	int x=	atoi(Tcl_GetVar2(interp, options, "gx", 0));
	int y=	atoi(Tcl_GetVar2(interp, options, "gy", 0));
	int maxobjects=	atoi(Tcl_GetVar2(interp, options, "maxobjects", 0));

	Ath__zui *zui= Ath__getZtop()->chip_get((char *) bb, layer, type, hierarchy, x, y, maxobjects, noclip);
	
	tcl_append(interp, zui);

	return TCL_OK;
}
/*
static void tcl_append_self(Tcl_Interp *interp, char *blk, char *name, char *type, 
					   int x1, int y1, int x2, int y2, 
					   uint layer)
{
	char buff[1024];
	
	sprintf(buff, "{ bb { %d %d %d %d } layer { %d } Block %s type %s name %s } \n", 
		x1, y1, x2, y2, layer, blk, type, name);
	
	Tcl_AppendResult(interp, buff, (char*)NULL);
}
static void tcl_append_self(Tcl_Interp *interp, char *blk, char *name, char *type, 
					   Ath__box *bb)
{
	char buff[1024];
	
	sprintf(buff, "{ bb { %d %d %d %d } layer { %d } Block %s type %s name %s } \n", 
		bb->_xlo, bb->_ylo, bb->_xhi, bb->_yhi, bb->_layer, blk, type, name);
	
	Tcl_AppendResult(interp, buff, (char*)NULL);
}
*/

extern "C" int AthNavigate_zui(ClientData cdata, Tcl_Interp *interp, int objc,
							  Tcl_Obj *CONST objv[])
{
	int len;
    char *options= Tcl_GetStringFromObj(objv[1], &len);

	//const char *chip=		Tcl_GetVar2(interp, options, "chip", 0);
	const char *objectName=	Tcl_GetVar2(interp, options, "obj", 0);
	const char *action=		Tcl_GetVar2(interp, options, "info", 0);
	const char *selectType=	Tcl_GetVar2(interp, options, "s", 0);
	const char *bb=			Tcl_GetVar2(interp, options, "bbox", 0);
	const char *layer=		Tcl_GetVar2(interp, options, "layer", 0);
	const char *type=		Tcl_GetVar2(interp, options, "type", 0);
	const char *hierarchy=	Tcl_GetVar2(interp, options, "hierarchy", 0);

	
	Ath__zui *zui= Ath__getZtop()->_vui;
	int tcl_status= TCL_ERROR;

	if ((strcmp(selectType, "")==0) && (strcmp(action, "")==0)) // ask for menu
	{
		if (zui->getPullDownMenu((char *)objectName))
			tcl_status= TCL_OK;
	}
	else if ((strcmp(action, "")!=0) || (strcmp(selectType, "")!=0)) // select or inspect
	{ 
		uint blockId= zui->setInspectParams(
			(char *)action, (char *)objectName, (char *)bb, (char *)selectType, false);

		dbBlockHandle *blkHandle= Ath__getZtop()->getBlockHandle(blockId);

		tcl_status= TCL_ERROR;
		if (blkHandle==NULL) 
			sprintf(zui->getTclBuf(), "Invalid Block Handle Id=%d", blockId);
		else 
		{
			zui->setZuiFlags(layer, type, hierarchy);
			tcl_status= blkHandle->inspect_1(zui);
		}
	}
	tcl_append(interp, zui);

	return tcl_status;
}
extern "C" int AthGetTiles(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	// ignore all for now : TODO

	if (!(Ath__check_args(1, " name bb filter ", interp, objc, objv)))
		return TCL_ERROR;

	char *bb= Ath__getOption_string(2);
	//char *filter= Ath__getOption_string(3);
	
	Ath__zui* zui= Ath__getZtop()->tiles_get(bb);
	
	tcl_append(interp, zui);

	return TCL_OK;
}
static void tcl_append_menus(Tcl_Interp *interp, 
							 char *layers, char *types, char *bbox)
{
	char *tab= "  ";
	char buf[256];
	//uint on= 1;

	sprintf(buf, "bb      { %s }", bbox);
	Tcl_AppendResult(interp, buf, "\n\n", (char*)NULL);

	sprintf(buf, "filters {");
	Tcl_AppendResult(interp, buf, "\n", (char*)NULL);

	sprintf(buf, "%s layer \"Layers\" {", tab);
	Tcl_AppendResult(interp, buf, "\n", (char*)NULL);

	Tcl_AppendResult(interp, layers, "\n", (char*)NULL);
	sprintf(buf, "%s }", tab);
	Tcl_AppendResult(interp, buf, "\n", (char*)NULL);

	sprintf(buf, "%s type \"Hierarchy\" { ", tab);
	Tcl_AppendResult(interp, buf, "\n", (char*)NULL);

	Tcl_AppendResult(interp, types, "\n", (char*)NULL);


	sprintf(buf, "%s }", tab);
	Tcl_AppendResult(interp, buf, "\n", (char*)NULL);
	sprintf(buf, "}" );
	Tcl_AppendResult(interp, buf, "\n", (char*)NULL);
}
extern "C" int zui_chipAttr_get(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	/* TODO
		int len;
    char *info= Tcl_GetStringFromObj(objv[1], &len);

	const char *chip=	
		Tcl_GetVar2(interp, info, "chip", 0);
*/
	if (!(Ath__check_args(2, " name ", interp, objc, objv)))
		return TCL_ERROR;

	char layers[1024]; strcpy(layers, "");
	char hierarchy[1024]; strcpy(hierarchy, "");
	char object_types[1024]; strcpy(object_types, "");
	char bbox[1024]; strcpy(bbox, "");

	Ath__getZtop()->getChipAttributes(
		bbox, layers, hierarchy, object_types);
	

	char *path_menus= Ath__getZtop()->_zui->getMenuPathBuff();
	tcl_append_menus(interp, layers, path_menus, bbox);

		
	//Tcl_AppendResult(interp, "bb { 0 0 0 0 } \n", (char*)NULL);

	return TCL_OK;
}

extern "C" int pull_down_menus_get(ClientData cdata, Tcl_Interp *interp, int objc,
								   Tcl_Obj *CONST objv[])
{
	//TODO

	return TCL_OK;
}

