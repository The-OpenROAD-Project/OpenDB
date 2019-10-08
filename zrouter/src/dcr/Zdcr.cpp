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

#include "IZdcr.h" 
#include "IZgui.h" 
#include "Zdcr.h"
#include "ZImplements.hpp"
#include "ZFactory.h"
#include "zui.h"
#include "dpt.h"

#ifndef _WIN32
#define SHORT_CHIP_GET_FORMAT
#endif

DECLARE_ZFACTORY(Zdcr, IZdcr)

BEGIN_NAMESPACE_ADS

DECLARE_IMPLEMENTATION(Zdcr, IZdcr)

Zdcr::Zdcr()
{
	_zui= NULL;
	_menuId= 0;
	_dpt= NULL;
	_guiCnt=1;
}
Zdcr::~Zdcr()
{
	if (_zui!=NULL)
		delete _zui;
}
uint Zdcr::getAppGuiCnt()
{
	return _guiCnt;
}
int Zdcr::inspect(const char *objectName, const char *selectType, const char *action, const char *chip, 
				  const char *bb, const char *layer, const char *hier, const char *type, FILE *outFP, bool noclip, bool write2list)
{
	bool inspect_flag= false;
	if ((selectType==NULL) && (action==NULL)) // ask for menu
	{
		if (! getPullDownMenu(objectName))
			return TCL_ERROR;
	}
	else if ((action!=NULL) || (selectType!=NULL)) // select or inspect
	{ 
		setFilters(chip, layer, type, hier);
		//uint blockId= setInspectEnv(action, objectName, bb, selectType, noclip);
		setInspectEnv(action, objectName, bb, selectType, noclip);
			
		_zui->setAsaParams(outFP, true);

#ifdef SHORT_CHIP_GET_FORMAT
	_zui->set_chip_get(true);
#endif

		inspect();
		inspect_flag= true;
	}
	uint lineLimit= 50000;
	writeToTcl(lineLimit, outFP, write2list);

	_zui->asa_output();

#ifdef SHORT_CHIP_GET_FORMAT
	if (inspect_flag)
		_zui->set_chip_get(false);
#endif

	return TCL_OK;
}
void Zdcr::inspect()
{
	for (uint ii= 1; ii<_guiCnt; ii++)
		_gui[ii]->inspect(this);
}
void Zdcr::chipAttr(char *layers, uint n, bool defTypeFlag)
{
	if (_guiCnt<2) {
		fprintf(stdout, "guiCnt= %d\n", _guiCnt);
		return;
	}
	char blocks[1024]; strcpy(blocks, "");
	uint blockIndex= 1;
	uint blockIdTable[1024];

	int X1,Y1,X2,Y2;
	uint blockId= _gui[1]->getBbox(&X1, &Y1, &X2, &Y2);

	blockIdTable[blockIndex]= blockId;
	sprintf(blocks, "%s %s %d B%d 1\n", blocks, "      ", blockIndex++, blockId);
		
	for (uint ii= 2; ii<_guiCnt; ii++) {
		int x1, y1, x2, y2;
		uint bid= _gui[ii]->getBbox(&x1, &y1, &x2, &y2);
		if (bid==0)
			continue;
		
		blockIdTable[blockIndex]= bid;
		sprintf(blocks, "%s %s %d B%d 0\n", blocks, "      ", blockIndex++, bid);
				
		X1= MIN(X1, x1);
		Y1= MIN(Y1, y1);
		X2= MAX(X2, x2);
		Y2= MAX(Y2, y2);
	}
	_zui->createBlockMapping(blockIdTable, blockIndex);

	char bbox[1024]; strcpy(bbox, "");
	sprintf(bbox, "%d %d %d %d", X1,Y1,X2,Y2);

	char menus[1024]; strcpy(menus, "");
	getMenuNames(menus);

	_zui->setMaxLayer(n+1);

	if (defTypeFlag) {
		char defTypes[1024]; strcpy(defTypes, "");
		sprintf(defTypes, "%s %s %s %s 0\n", defTypes, "      ", "clk", "Clock");
		sprintf(defTypes, "%s %s %s %s 0\n", defTypes, "      ", "macro", "Macro");
		sprintf(defTypes, "%s %s %s %s 0\n", defTypes, "      ", "mem", "Memory");
		sprintf(defTypes, "%s %s %s %s 0\n", defTypes, "      ", "buff", "Buffer");
		sprintf(defTypes, "%s %s %s %s 0\n", defTypes, "      ", "inv", "Inv");
		sprintf(defTypes, "%s %s %s %s 0\n", defTypes, "      ", "volt", "Voltage");
	
		getChipAttr(bbox, layers, menus, blocks, defTypes);
	}
	else
		getChipAttr(bbox, layers, menus, blocks, NULL);
}
void Zdcr::chip_get(const char *chip, bool gridFlag, int maxobjects, FILE *outFP)
{
	_zui->setAsaParams(outFP, true);

#ifdef SHORT_CHIP_GET_FORMAT
	_zui->set_chip_get(true);
#endif

	for (uint ii= 1; ii<_guiCnt; ii++)
		_gui[ii]->chip_get(this);
	
	if (gridFlag)
		writeToDpt();
	else
		writeToTcl(maxobjects, outFP, true);

	_zui->asa_output();

#ifdef SHORT_CHIP_GET_FORMAT
	_zui->set_chip_get(false);
#endif
}
void Zdcr::addGui(ZPtr<IZgui> z)
{
	_gui[_guiCnt++]= z;
}
char* Zdcr::makeName(const char *name)
{	
	uint len= strlen(name);
	char *a= new char[len+1];
	strcpy(a, name);
	return a;
}
char *Zdcr::setName(const char *name)
{	
	_name= makeName(name);
	return _name;
}
char *Zdcr::getName()
{	
	return _name;
}
void Zdcr::init(ZContext & context, bool dbEnv, const char *name, const char *module, bool noStore)
{
	bool skipMenuInit= true;

	_zui= new Ath__zui(skipMenuInit);
	ZALLOCATED(_zui);

	if (!noStore)
		_zui->setNoStoreBoxes(false);

	_dpt= new dpt();
	ZALLOCATED(_dpt);

	_zui->setTclInterp(context._interp);

	_name= makeName(name);
	// setupModuleMenu(module, "z");

	_memuMult= 20;

	_subMenuCnt= 1;
	_nameTable= new char*[1000];
	_subMenuIdTable= new uint[1000];
	_subMenuTypeTable= new uint[1000];
	_mapIndex= new uint[1000];
	for (uint ii= 0; ii<1000; ii++)
	{
		_subMenuIdTable[ii]= 0;
		_subMenuTypeTable[ii]= 0;
		_nameTable[ii]= NULL;
		_mapIndex[ii]= 0;
	}
	_zui->addModuleNameToMenu("block", "b", 1);
	_zui->addModuleNameToMenu("inst", "i", 2);
	_zui->addModuleNameToMenu("signal", "s", 3);
	_zui->addModuleNameToMenu("power", "p", 4);
	_zui->addModuleNameToMenu("ext", "x", 5);
	_zui->addModuleNameToMenu("tmg", "t", 6);
}
void *Zdcr::getDpt()
{
	return _dpt;
}
uint Zdcr::initDbMenus()
{
	return _zui->setDbMenus(false);
}
uint Zdcr::getMenuId(char *name)
{
	uint menuId= _zui->getHierId(name); 
	if (menuId==0)
		return 0; //TODO error report

	uint index= getIndex(menuId, 0);

	if (_nameTable[index]!=NULL)
		return menuId;

	_nameTable[index]= makeName(name);
	_mapIndex[_subMenuCnt++]= index;

	return menuId;
}
uint Zdcr::setupModuleMenu(const char *moduleName, const char *onechar, uint id)
{
//	if (_menuId>0)
//		return _menuId;

	_moduleName= makeName(moduleName);

	_menuId= _zui->addModuleNameToMenu(_moduleName, onechar, id);

	if (id>0)
		return id;

	return _menuId;
}/*
uint Zdcr::addSubMenu(uint menuId, char *name, uint boxId)
{
	ZALLOCATED(_zui);

	uint boxType= boxId;
	_subMenuIdTable[_subMenuCnt]= _zui->addBoxNameToMenu(menuId, name, &boxType);
	_subMenuTypeTable[_subMenuCnt]= boxType;

	return _subMenuCnt++;
}
*/
uint Zdcr::getIndex(uint menuId, uint boxId)
{ 
	return menuId*_memuMult + boxId;
}
uint Zdcr::addSubMenu(uint menuId, char *name, uint boxId)
{
	ZALLOCATED(_zui);

	uint index= getIndex(menuId, boxId);

	uint boxType= boxId;

	if (_subMenuIdTable[index]>0) // same tech module has already been added
		return index;

	_subMenuIdTable[index]= _zui->addBoxNameToMenu(menuId, name, &index);
	_subMenuTypeTable[index]= boxType;

	char buf[1000];
	sprintf(buf, "%s/%s", _nameTable[getIndex(menuId,0)], name);

	_nameTable[index]= makeName(buf);
	_mapIndex[_subMenuCnt++]= index;

	return index;
}
uint Zdcr::getMenuNames(char *buff)
{
	for (uint ii= 1; ii<_subMenuCnt; ii++) {
		uint index= _mapIndex[ii];
		uint submenuId= _mapIndex[ii] % 20;
		if (submenuId>0) {
			char *word= strdup(_nameTable[index]);
			char *word1= strtok(word, "/");
			char *word2= strtok(NULL, "/");
			sprintf(buff, "%s        %s \"%s\" %d\n", buff, _nameTable[index], word2, 0);
			// sprintf(buff, "%s        %s/%s/%s \"%s\" %d\n", buff, word1, "NEW", word2, word2, 0);
		}
		else
			sprintf(buff, "%s        %s \"%s\" %d\n", buff, _nameTable[index], _nameTable[index], 0);
	}

	return _subMenuCnt;
}
bool Zdcr::isBlockSelected(uint bid)
{
	return _zui->isBlockSelected(bid);
}
void Zdcr::setBlockId(uint v)
{
	_zui->setBlockId(v);
}
uint Zdcr::getBlockId()
{
	return _zui->getBlockId();
}
bool Zdcr::isInspectSubMenu(uint menuId, uint subMenuId)
{
	uint index= getIndex(menuId, subMenuId);
//	uint subMenuFilter= _subMenuIdTable[index];
	uint subMenuType= _subMenuTypeTable[index];

	return _zui->isInspectBoxType(subMenuType);
}
bool Zdcr::isSubmenuType(uint id)
{
	return _zui->isInspectBoxType(_subMenuTypeTable[id]);
}
uint Zdcr::setInspectEnv(const char *action, const char *objectName, const char *bb, const char *selectType, bool noclip)
{
	return _zui->setInspectParams((char *)action, (char *)objectName, (char *)bb, (char *)selectType, noclip);
}
char* Zdcr::getInspectName()
{
	return _zui->getInspectName();
}
uint Zdcr::getSubmenuObjId(uint *id2)
{
	return _zui->getInspNameId(id2);
}
uint Zdcr::getPullDownActionId()
{
	return _zui->getPullDownActionId();
}
bool Zdcr::msgAction()
{
	return _zui->msgAction();
}
bool Zdcr::selectAction()
{
	return _zui->selectAction();
}
bool Zdcr::isSelectedMenu(char *name)
{
	return _zui->isSelectType(name);
}
uint Zdcr::getSelectIds(uint *id1, uint *id2, uint *id3)
{
	return _zui->getSelectIds(id1, id2, id3);
}

void Zdcr::getChipAttr(char *bbox, char *layers, char *menus, char *blocks, char *defTypes)
{
	char hierarchy[1024]; strcpy(hierarchy, "");
	char objects[1024]; strcpy(objects, "");

	_zui->getHierBoxTypes(hierarchy, objects);

	char *path_menus= _zui->getMenuPathBuff();
	if (menus!=NULL)
		path_menus= menus;
	_zui->tcl_append_menus(layers, path_menus, bbox, blocks, defTypes);

	//fprintf(stdout, "%s\n%s\n%s\n", bbox, layers, path_menus);
}
int Zdcr::getCoords(const char *bb, int *x1, int *y1, int *x2, int *y2)
{
	return _zui->getCoords(bb, x1, y1, x2, y2);
}
void Zdcr::resetSearchParams(char *chip, const char *bb, const char *layer, 
						const char *hier, const char *type, bool noclip, bool q)
{
	_zui->resetZui(chip, bb, layer, hier, type, noclip, q);
}
void Zdcr::setFilters(const char *chip, const char *layer, const char *type, const char *hier)
{
	_zui->resetBlocks(chip);
	_zui->setZuiFlags(layer, type, hier);
}
bool Zdcr::getPullDownMenu(const char *objectName)
{
	return _zui->getPullDownMenu((char *)objectName);
}
uint Zdcr::addPullDownMenu(uint menuId, uint subMenuId, char *db_name, char *zui_action)
{
	return _zui->addPullDownMenu(menuId, subMenuId, db_name, zui_action);
}
uint Zdcr::addPullDownMenu(char* menu, char* subMenu, char *db_name, char *zui_action)
{
	return _zui->addPullDownMenu(menu, subMenu, _memuMult, db_name, zui_action);
}
void Zdcr::wireMsg(int x1, int y1, int x2, int y2, uint level, char *msg_buf)
{
	_zui->print_wire_msg(x1, y1, x2, y2, level, msg_buf);
}
void Zdcr::print_self(char *blk, char *name, char *extra)
{
	_zui->print_self(blk, name, extra);
}
void Zdcr::writeToTcl(uint lineLimit, FILE *fp, bool write2list)
{
	if (!_zui->isNoStoreBoxes()) {
		_zui->setOutFP(fp);
		_zui->setWrite2AsaList(write2list);
		
		// uint boxCnt= _zui->getZboxes(); // iterator
		_zui->getZboxes(); // iterator
		
		_zui->tcl_append(lineLimit);
	}
}
uint Zdcr::writeToDpt()
{
	uint boxCnt= _zui->getZboxes(); // iterator
    //fprintf(stderr,"writeToDpt: %d\n",boxCnt);
	for (uint ii= 0; ii<boxCnt; ii++) {
		Ath__zbox *b= _zui->getBox(ii);
		_dpt->box(b->_xlo, b->_ylo, b->_xhi, b->_yhi, b->_layer);
	}
	return boxCnt;
}
char *Zdcr::getFirstTclBox(int *x1, int *y1, int *x2, int *y2, uint *layer)
{
	return _zui->getFirstBox(x1, y1, x2, y2, layer);
}
void Zdcr::getBbox(int *x1, int *y1, int *x2, int *y2)
{
	_zui->getBbox(x1, y1, x2, y2);
}
void Zdcr::setSearchBox(int x1, int y1, int x2, int y2)
{
	_zui->setSearchBox(x1, y1, x2, y2, 0);
}
bool Zdcr::clipBox(int &x1, int &y1, int &x2, int &y2)
{
	return _zui->clipBox(x1, y1, x2, y2);
}
bool Zdcr::invalidateSearchBox()
{
	_zui->invalidateSearchBox();
	return true;
}
uint Zdcr::addBoxAndMsg(uint id, uint subMenuId, uint menuId, int layer, 
					  int x1, int y1,int x2, int y2, uint ownId, char *special)
{
	uint index= getIndex(menuId, subMenuId);
	uint subMenuFilter= _subMenuIdTable[index];
	uint subMenuType= _subMenuTypeTable[index];
//	uint subMenuFilter= _subMenuIdTable[subMenuId];
//	uint subMenuType= _subMenuTypeTable[subMenuId];
	return _zui->addBoxAndMsg(id, subMenuType, menuId, layer, x1, y1, x2, y2, ownId, subMenuFilter, special);
}
uint Zdcr::addBox(uint id, uint subMenuId, uint menuId, int layer, 
					  int x1, int y1,int x2, int y2, uint ownId, char *boxType)
{
	uint index= getIndex(menuId, subMenuId);
	uint subMenuFilter= _subMenuIdTable[index];
	uint subMenuType= _subMenuTypeTable[index];
//	uint subMenuFilter= _subMenuIdTable[subMenuId];
//	uint subMenuType= _subMenuTypeTable[subMenuId];
	return _zui->addBox(id, subMenuType, menuId, layer, x1, y1, x2, y2, ownId, subMenuFilter, boxType);
}
uint Zdcr::addArrow(bool right, uint subMenuId, uint menuId, int layer, 
						int labelCnt, char **label, double *val,
						int x1, int y1,int x2, int y2, uint boxFilter)
{
	uint index= getIndex(menuId, subMenuId);
	uint subMenuFilter= _subMenuIdTable[index];
	uint subMenuType= _subMenuTypeTable[index];
	
	return _zui->addArrow(right, subMenuType, menuId, layer, 
						labelCnt, label, val, x1, y1, x2, y2, subMenuFilter);
}


char *Zdcr::getMsgBuffer(bool setBuffFlag)
{
	return _zui->getTclBuf(setBuffFlag);
}
bool* Zdcr::getExcludeLayerTable()
{
	return _zui->getExcludeLayerTable();
}
bool Zdcr::getSubMenuFlag(uint menuId, uint subMenuId)
{
	//2005/05/08 return _zui->getSubMenuFlag(_subMenuTypeTable[subMenuId]);
	uint index= getIndex(menuId, subMenuId);

	return _zui->getSubMenuFlag(_subMenuIdTable[index]);
	//return _zui->getSubMenuFlag(_subMenuIdTable[subMenuId]);
}
bool Zdcr::isInspectMenu(uint menuId)
{
	// return _zui->getHierId()==_subMenuIdTable[subMenuId] ? true : false;
	return _zui->getHierId()==menuId ? true : false;
}
bool Zdcr::isInspectSubMenu(uint subMenuId)
{
	// return _zui->getHierId()==_subMenuIdTable[subMenuId] ? true : false;
	return _zui->getBoxId()==subMenuId ? true : false;
}
bool Zdcr::validSearchBbox()
{
	return _zui->isSearchBoxValid();
}
void Zdcr::setContextMarker()
{
	_zui->setCurrentMark(Ath_mark__context);
}
void Zdcr::setSignalMarker()
{
	_zui->setCurrentMark(Ath_mark__signalWire);
}
void Zdcr::setInstMarker()
{
	_zui->setCurrentMark(Ath_mark__instConn);
}
void Zdcr::resetMarker()
{
	_zui->setCurrentMark(Ath_mark__none);
}

END_NAMESPACE_ADS

