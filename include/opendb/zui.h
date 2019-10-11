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

#ifndef ZUI_H
#define ZUI_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "array1.h"
#include "array2d.h"
#include "atypes.h"
#include "misc_global.h"
#include "box.h"
#include "enumtype.h"
#include "util.h"
#include "tcl.h"


#ifndef ADS_DB_H
#include "db.h"
#endif

USING_NAMESPACE_ADS;
class dcrASA;

#define ZUI_MAP_SIZE 1000
class Ath__zui;

class Ath__hierarchy
{
protected:	
	
	union {		
		struct {
			uint _box:10;
			uint _h0:4;
			uint _h1:4;
			uint _h2:4;
			uint _h3:10;
		} _type ;

		uint _all;
	};
	
public:
	Ath__hierarchy(uint b, uint h1, uint h2, uint h3);
	Ath__hierarchy(uint v);
	uint getBox();
	uint getAll();
	uint getHier(uint n);
	void setAll(uint n);
};

class Ath__zbox
{
public:
	uint _nameId; 
	uint _ownerNameId;
	int _xlo;
	int _ylo;
	int _xhi;
	int _yhi;

	uint _type:6;
	uint _hier:6;
	int _layer:5;

	uint _blockId:8;
	uint _zuiMark:7;
	char *_ownerName;

public:
	Ath__zbox();
	Ath__zbox(int x1, int y1, int x2, int y2);
	void setCoords(int x1, int y1, int x2, int y2);
	void setZuiIdentity(uint blockId, uint id, uint type, uint hier, uint ownId, uint layer);
	void clearFlags();
	void setZuiMark(uint mark);

	friend class Ath__zui;
	friend class Ath__2dCounter;
};
class Ath__2dCounter
{
private:

	uint **_array;
	uint _allocRowCnt;
	uint _allocColCnt;
	uint _rowCnt;
	uint _colCnt;
	int _loX;
	int _loY;
	int _hiX;
	int _hiY;
	uint _modX;
	uint _modY;

public:

	friend class Ath__zui;

	Ath__2dCounter(uint m, uint n);
	~Ath__2dCounter();
	void setup(Ath__box *bb, uint modX, uint modY);
	void update(Ath__zbox *bb);
	void update(int xlo, int ylo, int xhi, int yhi);
	void cntUp(int x1,	int y1,	int x2,	int y2);

	void mkPixelRects(uint layer, Ath__zui *zui);
};
class Ath__searchBox;

class Ath__zui
{
private:

	Ath__parser *_parser;

	char _filename[1024];
	FILE *_fp;
	uint _width;
	uint _pitch;
	uint _dir;
	uint _base;
	char _layerNodeString[100];

	bool _tclBufFlag;
	char* _tclBuff;
	char* _tclIdBuff;
	char* _tclLayer;
	char* _tclOwner;
	Ath__box _searchBB;

	Ath__array1D<Ath__zbox*> *_tmpZboxTable;
	Ath__array1D<uint> *_idtable;
	Ath__array1D<Ath__zbox*> **_ztable;
	AthPool<Ath__zbox> *_pool;

	uint _maxHierCnt;
	uint _maxTypeCnt;
	uint _db2hierTypeMap[32];
	uint _currentMark;
	Ath__associateArray *_markTable;
	Ath__associateArray *_boxTypeTable;
	Ath__associateArray *_hierTypeTable;
	Ath__associateArray ***_menu;
	uint _menuPathCnt;
	bool *_dbFilterTable;
	Ath__nameTable *_pathMenuTable;
	char *_menuPathBuff;

	bool _excludeLayerTable[20];

	uint _detail;
	Ath__2dCounter **_metPixelTable;
	uint _layerCnt;

	uint _currentBlockId;

	uint _inspBlockId;
	uint _inspNameId;
	uint _inspNameId_2;
	uint _inspBoxId;
	uint _inspHierId;
	char *_inspName;
	char *_inspAction;
	bool _selectFlag;

	uint _gxy[2];
	uint _maxObjects;

	uint _pullDownMenuId;

	Tcl_Interp *_tclInterp;
	FILE *_outFP;
	dcrASA *_asa;
	bool _noStoreBoxes;
	Ath__zbox _tmpZbox;
	bool _chip_get_flag;
	bool _cursor;
	uint _maxLayer;

	bool _blockIdTable[1024];
	Ath__array1D<uint> _blockTable;
	uint _blockIdMap[1024];

	uint _tcutId;
	uint _bcutId;

    /* special vars for caching!! */
    uint _prevhier;
    int _prevlayer;
    uint _prevtype;

private:
    void cacheLayer(int layer);
    void cacheHier(uint hier);
    void cacheType(uint type);
    void resetCache();
    /* end special caching vars */

public:
	Ath__zui(bool skipMenuInit=false, char *name=NULL, int m=-1, int n=-1);
	~Ath__zui();
	void setBlockId(uint n) { _currentBlockId= n; };
	uint getBlockId() { return _currentBlockId; };
	void setMaxLayer(uint n) { _maxLayer= n; };
	
	void initPixelArrays(uint layerCnt);
	void setSearchBox(int x1, int y1, int x2, int y2, int layer);
	bool clipBox(int &x1, int &y1, int &x2, int &y2);
	void invalidateSearchBox();
	void setSearchClipFlag(bool flag);
	
	uint makeZuiObjects(Ath__box *head, Ath__hierType hier, Ath__boxType box);
	Ath__array1D<uint> *getIdTable();
	void addId(uint id);
	uint getNextId();
	void resetIdTableIterator();

	char *getFirstBox();
	Ath__box* getBbox();
	void getBbox(Ath__box *bb);
	Ath__zbox* getBox(uint ii);
	char* getTclBuf(bool flag=false);
	uint getZboxes();

	bool isBlockSelected(uint n);
	void resetBlocks(uint n);
	uint resetBlocks(const char *blocks);
	void createBlockMapping(uint *blockIdTable, uint blockCnt);

	uint initBoxTypes();
	uint initHierTypes();
	void getHierBoxTypes(char *hierBuff, char *boxBuff);
	uint getBoxTypes(char *layerBuff);
	char *getMenuPathBuff();
	uint initGuiMenus(bool skipMenuInit);
	void deallocGuiMenus();
	void initPullDownMenus(bool skipMenuInit);
	uint initMarks();
	uint addPullDownMenu(char *menu, char *subMenu, uint mult, char *db_name, char *zui_action_name);

	void addPullDownMenu(uint hier, uint btype, Ath__zuiAction action, 
							   char *db_name, char *zui_action_name);
	char *getPullDownMenu(char *objHierWord, uint objType);

	uint makeMenuPath(char *menuTabName, int v, uint b, uint h1, uint h2, uint h3);
	uint defineMenuPaths(bool skipMenuInit);
	void resetMenuPathFlags();
	bool getDbFlag(char *name);
	Ath__hierarchy getHierarchyWord(char *name);


	bool getTclBufFlag();
	uint getZtableCnt();
	char* getTclBbox(int ii, bool vFlag);
	char* getTclBbox_light(Ath__zbox *b, int index);
	char* getTclBbox(Ath__zbox *b, int index, char *boxWord=NULL);
	char* getTclLayer(Ath__zbox *b);
	char* getTclOwner(Ath__zbox *b);

	uint mkPixelRects();

	void resetZtable();
	void resetTable(int detail, uint modX=0, uint modY=0, Ath__box *bb=NULL);
	void resetPixelTables(uint modX, uint modY, Ath__box *bb);
	uint addBox(uint id, uint boxType, uint hier, 
		int layer, int x1, int y1,int x2, int y2, uint ownId, uint boxFilter, char *boxWord=NULL);

	uint addBox(Ath__box *bb, uint type, uint id=0);
	uint addBox(dbBox *bb, uint boxType, uint id, int hierType= -1);
	uint addGrouteBox(uint boxType, int x1, int y1,int x2, int y2, int level, uint id);

	uint addBox(Ath__searchBox *bb, uint hierType, uint boxType, uint id=0);
	uint addWireShape(int x1, int y1, int x2, int y2, uint level, Ath__hierType hierType, Ath__boxType boxType, uint id, uint ownId);

	void makeZuiObject(Ath__box *e);
	uint makeZuiObjects_1(Ath__box *head, Ath__hierType hier, Ath__boxType box);

	void setTrackPitch(char *layerName, uint dir, int cnt, int base, int max);
	void printQuadBox(uint row, uint col, int x1, int y1, int x2, int y2);
	void printSeg(char *layer, uint cnt, int x1, int y1, int x2, int y2);
	void printSegOnTrack(int xy1, int xy2, int index);
	char getNameType(uint n);
	void getBox(uint ii, Ath__box *bb);
	bool intersect(int x1, int y1, int x2, int y2);
	void setTypeFlag(uint hierFlag, char *name_prefix);
	void resetAllTypeFlags(uint hierFlag);
	void setAllTypeFlags(uint hierFlag);
	void resetTypeFlag(uint hierFlag, char *name_prefix);
	bool getTypeFlag(uint flagType, uint n);
	void setTypeHierFlag(char *name);

	bool isLayerExluded(int n);

	void setCurrentMark(Ath__zuiMark mark);
	void computeMaxBB(uint visualGap=0);
	void bufferExistingZboxes();
	void addPreviousZboxes();

	void setAllExcludeLayers();
	void resetAllExcludeLayers();
	void setExcludeLayer(int n);
	void resetExcludeLayer(int n);
	char *getPullDownMenu(uint hierType, uint objType);
	bool getPullDownMenu(char *objectName);
	void print_self(char *blk, char *name, char *extra=NULL);
	void print_msg(char *blk, char *name, char *extra=NULL);
	void print_msg(int x1, int y1, int x2, int y2, uint layer, char *msg);

	void print_wire_msg(int x1, int y1, int x2, int y2, uint layer, char *msg);

	void initDb2HierTypeMapping();
	uint mapDb2HierType(dbBox *bb);
	void print_self(const char *blk, const char *name, char *type, int x1, int y1, int x2, int y2, uint layer);

	uint setInspectParams(char *action, char *objectName, char *bb, char *selectType, bool noclip);
	uint getHierId();
	uint getHierId(char *name);
	uint getInspNameId(uint *inspId2=NULL);
	//uint getInspNameId();
	uint getPullDownActionId();
	char *getInspectName();

	bool selectAction();
	bool selfAction();
	bool groupAction1();
	bool groupAction2();
	bool msgAction();
	bool linkAction();

	bool hierType_signal();
	bool hierType_inst();
	bool hierType_power(uint hier);
	bool hierType_power();
	bool hierType_block();
	bool hierType_groute();
	bool hierType_tile();

	bool boxType_bbox();
	bool boxType_pin();
	bool boxType_wire();
	bool boxType_via();
	bool boxType_via(uint via);
	bool boxType_obs();
	bool boxType_gpath();

	bool getHierBoxFlag(uint hier, uint box, bool ignoreFlags=false);
	bool getNetWireViasFlag(bool ignoreFlag);
	
	bool isSearchBoxValid();

	void setZuiFlags(uint hierFlag, const char *type);
	void resetZuiFlags(uint hierFlag, const char *type);
	void setZuiFlags(const char *layer, const char *hier, const char *type);
	void resetZuiFlags(const char *layer, const char *hier, const char *type);
	uint setZuiLayers(const char *layers);
	uint resetZuiLayers(const char *layers);

	char *getFirstBox(int *x1, int *y1, int *x2, int *y2, uint *layer);

	//IZdcr
	uint addModuleNameToMenu(char *name, const char *onechar, uint id);
	uint addBoxNameToMenu(uint moduleId, char *name, uint *boxType);

	void setCursorFlag(bool flag);
	char* createZuiId(uint hier, uint index, uint ownId, uint type);
	bool isNoStoreBoxes();
	uint asa_append(uint lineLimit);
	void set_chip_get(bool v);
	void asa_output();
	char* send2asa(Ath__zbox *b, char *boxWord=NULL);
	void setWrite2AsaList(bool f);
	void setNoStoreBoxes(bool f);
	void setAsaParams(FILE *fp, bool write2List);
	uint addArrow(bool right, uint boxType, uint hier, int layer, 
						int labelCnt, char **label, double *val,
						int x1, int y1,int x2, int y2, uint boxFilter);
	
	uint addBoxAndMsg(uint id, uint boxType, uint hier, int layer, 
		int x1, int y1,int x2, int y2, uint ownId, uint boxFilter, char *special);

	void setASA(dcrASA *s);
	void setTclInterp(Tcl_Interp *interp);
	void tcl_append_menus(char *layers, char *types, char *bbox, char *blocks, char *defTypes);
	uint tcl_append(uint lineLimit);
	void getBbox(int *x1, int *y1, int *x2, int *y2);
	void resetZui(char *chip, const char *bb, const char *layer, const char *hier, const char *type, bool noclip, bool q);
	int getCoords(const char *bb, int *x1, int *y1, int *x2, int *y2);
	bool getSubMenuFlag(uint n);
	bool *getExcludeLayerTable();
	bool isInspectBoxType(uint wtype);
	void setAggrParams(int x, int y, int maxobjects);
	uint* getGxy(uint *n);
	uint setDbMenus(bool skipMenuInit);
	uint addPullDownMenu(uint hier, uint btype, char *db_name, char *zui_action_name);
	bool isSelectType(char *name);
	uint getSelectIds(uint *id1, uint *id2, uint *id3);
	uint getBoxId();

	void setOutFP(FILE *fp);
	uint tcl_file_append(uint lineLimit);

};


#endif

	
