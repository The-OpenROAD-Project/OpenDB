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

#ifndef WIRE_H 
#define WIRE_H 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "atypes.h"
#include "qtype.h"
#include "tech.h"
#include "box.h"
#include "zui.h"

#include "array1.h"

#include "geom.h"

#ifndef ADS_DB_H
#include "db.h"
#endif

USING_NAMESPACE_ADS;

class Ath__net;

class Ath__path
{
private:
	Ath__net *_net;

	union {		
		struct {
			uint _level:5;
			uint _trackNum:12;
			uint _qRow:5;
			uint _qCol:5;
		} _from ;

		uint _fromIterm;
	};
	union {		
		struct {
			uint _level:5;
			uint _trackNum:12;
			uint _qRow:5;
			uint _qCol:5;
		} _to ;

		uint _toIterm;
	};
	uint _num;

public:

};

class Ath__via
{
private:
	uint _trackNum:12;
	uint _xy:10;
	uint _masterNum:10;

	uint _wire1:10;
	uint _wire2:10;
	uint _node:12; // TODO

public:

};

bool Ath__intersect(int X1, int DX, int x1, int dx, int *ix1, int *ix2);

// TODO center line with width and length and direction

class Ath__box
{
private:
	uint _ownerId:23; // owner id
	uint _ownerType:4; // hier type
public:
	uint _layer:4;
	uint _valid:1;
	uint _id:29;
	uint _type:3;
public:
	int _xlo;
	int _ylo;
	int _xhi;
	int _yhi;
	Ath__box *_next;

public:
	
	Ath__box();
	uint getLen();
	uint getDir();
	int getYhi(int bound);
	int getXhi(int bound);
	int getXlo(int bound);
	int getYlo(int bound);
	int getMidY(int loBound, int hiBound);
	int getMidX(int loBound, int hiBound);
	void addExtra(uint dd);
	uint getWidth(uint *dir);
	uint getStats(uint level, uint *dir, uint *minWidth, uint *maxWidth, uint minLen);
	void writeCoordsDB(FILE *fp);
	void readCoordsDB(FILE *fp);
	uint boxCnt();
	void writeListDB(FILE *fp);
	void deallocList();
	Ath__box(int x1, int y1, int x2, int y2, uint units=1);
	void set(int x1, int y1, int x2, int y2, uint units=1);
	uint getDX();
	uint getDY();
	int getMidX();
	int getMidY();
	void printDefXYlo(FILE *fp);
	uint getLength();
	void setIdentity(Ath__box *bb);
	void setIdentity(uint type, uint ownId, uint ownType);
	void invalidateBox();
	void resetMaxBox();
	void pinBoxDef(FILE *fp, char *layerName, int x, int y, char *orient, int defUnits);
	void printPoints(FILE *fp, uint defUnits);
	void setMaxBox(int x1, int y1, int x2, int y2);
	void set(Ath__box *bb);
	void getTransformed(Ath__box *bb, int origX, int origY, char *orient=NULL);
	void setMaxBox(Ath__box *bb);
	bool outside(int x1, int y1, int x2, int y2);
	bool clip(int *x1, int *y1, int *x2, int *y2);
	bool inside(int x1, int y1, int x2, int y2);
	uint getOwner();
	uint getOwnerType();
	uint getBoxType();
	void setOwner(uint id);

	friend class Ath__bterm;
};

class Ath__trans
{
public:
	int _origX;
	int _origY;
	char _orient[3];

	void set(int x, int y, char *orient);
	void transform(Ath__box *bb);
	int getX(int x);
	int getY(int y);
};

#endif


class Ath__track;

class Ath__searchBox
{
private:
	int _ll[2];
	int _ur[2];
	uint _level;
	uint _dir;
	uint _ownId;
	uint _otherId;
	uint _type;

public:

	Ath__searchBox(int x1, int y1, int x2, int y2, uint l, int dir=-1);
	Ath__searchBox(Ath__box *bb, uint l, int dir=-1);
	Ath__searchBox(Ath__searchBox *bb, uint l, int dir=-1);
	Ath__searchBox();
	void set(int x1, int y1, int x2, int y2, uint l, int dir);
	void setMidPointSearch();
	int loXY(uint d);
	int loXY(uint d, int loBound);
	int hiXY(uint d);
	int hiXY(uint d, int hiBound);
	void setLo(uint d, int xy);
	void setHi(uint d, int xy);
	void setType(uint v);
	uint getType();

	uint getDir();
	uint getLevel();
	void setDir(int v=-1);
	void setLevel(uint v);
	void setOwnerId(uint v, uint other=0);
	uint getOwnerId();
	uint getOtherId();
	uint getLength();
	void printCoords(FILE *fp, char *msg);
	void setMaxBox(Ath__searchBox *bb);
};

class Ath__wire
{
private:
	uint _id;
	uint _srcId;
	uint _boxId;
	uint _otherId;

	Ath__track *_track;
	Ath__wire *_next;

	int _xy;  // offset from track start
	int _len;
	
	uint _up:15;
	uint _down:15;
	uint _flags:2; // ownership
	// 0=wire, 2=obs, 1=pin, 3=power

	uint _via1:15;
	uint _via2:15;
	uint _horiz:1;
	uint _ext:1;

public:
	void reset();
	void set(int xy1, int xy2);
	void setNext(Ath__wire *w) { _next= w; };
	Ath__wire* getNext() { return _next; };
	uint getBoxId();
	void setOtherId(uint id);
	uint getOtherId();
	bool isTilePin();
	bool isTileBus();
	uint getOwnerId();
	uint getSrcId();
	//void makeZuiObject(Ath__zui *zui, Ath__boxType type, int layer);

	friend class Ath__track;
	friend class Ath__grid;
	friend class dbBlockHandle;

};

class Ath__grid;

class Ath__track
{
private:
	int _x; // you need only one
	int _y;

	Ath__wire **_marker;
	uint _markerCnt;

	Ath__grid *_grid;

	uint _num:20;
	uint _markerModulo:12;

	int _width:20; // to be removed
	uint _widthIndex:4; // 0 is std width
	uint _shift:4;
	uint _centered:1;
	uint _blocked:1;
	uint _full:1;
	
public:
	void set(Ath__grid *g, int x, int y, uint n, uint width, uint markerLen, uint markerCnt);
	void freeWires(AthPool<Ath__wire> *pool);
	bool place(Ath__wire *w, int markIndex1, int markIndex2);
	bool place(Ath__wire *w, int markIndex1);
	uint adjustMarkers(int markerCnt, int start, uint markerLen);
	bool placeTrail(Ath__wire *w, uint m1, uint m2);

	bool overlapCheck(Ath__wire *w, int markIndex1, int markIndex2);
	bool isAscendingOrdered(uint markerCnt, uint *wCnt);
	Ath__grid *getGrid();
	Ath__wire *getWire_Linear(uint markerCnt, uint id);
	uint search(int xy1, int xy2, uint markIndex1, uint markIndex2, Ath__array1D<uint> *idtable);
	uint search1(int xy1, int xy2, uint markIndex1, uint markIndex2, Ath__array1D<uint> *idTable);

	bool checkAndplace(Ath__wire *w, int markIndex1);
    bool checkMarker(int markIndex);
	bool checkAndplacerOnMarker(Ath__wire *w, int markIndex);
	uint getAllWires(Ath__array1D<Ath__wire*> *boxTable, uint markerCnt);
	void resetExtFlag(uint markerCnt);

	friend class Ath__grid;
	friend class Ath__wire;
};
class Ath__quadTable;
class Ath__gridTable;

class Ath__grid
{
private:

	Ath__track **_trackTable;
	uint *_blockedTrackTable;
	uint _trackFilledCnt;
	float _emptyTrackCnt;
	uint _freeTrackCnt;
	uint _trackCnt;
	int _base;
	int _max;

	int _start; // laterraly
	int _end;

	uint _width;
	uint _pitch;
	uint _level;
	uint _layer;
	uint _dir;
	int _markerLen;
	uint _markerCnt;
	uint _placed:1;

	AthArray<Ath__wire*> *_wireTable;
	AthArray<Ath__via*> *_viaTable;

	uint _widthTable[8];
	uint _shiftTable[8];
	AthPool<Ath__track> *_trackPoolPtr;
	AthPool<Ath__wire> *_wirePoolPtr;

	uint _ignoreFlag;
	uint _schema;

public:
	Ath__grid(AthPool<Ath__track> *trackPool, AthPool<Ath__wire> *_wirePool, 
		Ath__box *bb, Ath__layer *layer, uint markerCnt=4);
	Ath__grid(AthPool<Ath__track> *trackPool, AthPool<Ath__wire> *wirePool, Ath__box *bb, 
					 uint level, uint dir, uint num, uint width, uint pitch, uint markerCnt=4);
	Ath__grid(AthPool<Ath__track> *trackPool, AthPool<Ath__wire> *wirePool,  
					 uint level, uint num, uint markerCnt);
	~Ath__grid();

	void setBoundaries(uint dir, int xlo, int ylo, int xhi, int yhi);
	void setTracks(uint dir, uint width, uint pitch, int xlo, int ylo, int xhi, int yhi, uint markerLen=0);
	void setPlaced();
	void setSchema(uint v);
	bool isPlaced();

	uint setFreeTracks();
	bool anyTrackAvailable();

	uint addWireList(Ath__box *box);
	uint getTrackCnt() { return _trackCnt; };
	uint getTrackNum1(int xy);
	uint getWidth();
	int getXYbyWidth(int xy, uint *mark);
	Ath__track *addTrack(uint ii, uint markerCnt);
	void makeTracks(uint space, uint width);
	void getBbox(Ath__box *bb);
	void getBbox(Ath__searchBox *bb);
	uint adjustMarkers();
	uint getBoundaries(Ath__zui *zui, uint dd, uint layer);
	uint addWire(Ath__box *box, int check);
	uint addWire(Ath__box *box);

	uint placeWire(Ath__searchBox *bb);
	uint placeBox(uint id, int x1, int y1, int x2, int y2);
	uint placeBox(dbBox *box);
	uint placeBox(Ath__box *box);
	uint placeBox(Ath__searchBox *bb);
	uint getBucketNum(int xy);
	uint getTrackNum(int *ll, uint d, uint *marker);
	Ath__wire *getWirePtr(uint wireId);
	void getBoxIds(Ath__array1D<uint> *wireIdTable, Ath__array1D<uint> *idtable);
	void getWireIds(Ath__array1D<uint> *wireIdTable, Ath__array1D<uint> *idtable);

	int findEmptyTrack(int ll[2], int ur[2]);
	uint getFirstTrack(uint divider);
	int getClosestTrackCoord(int xy);
	void makeZuiObjects(Ath__zui *zui);
	uint addWire(uint initTrack, Ath__box *box, int sortedOrder, int *height);
	Ath__wire *makeWire(Ath__box *box, uint *id, uint *m1, uint *m2, uint fullTrack);
	Ath__wire *makeWire(Ath__box *box, uint id, uint *m1);
	Ath__wire *makeWire(int *ll, int *ur, uint id, uint *m1);
	Ath__wire *makeWire(int xy1, int xy2, uint id1, uint id2, uint type=0);
	Ath__wire* makeWire(Ath__wire *w, uint type=0);

	void makeTrackTable(uint width, uint pitch, uint space=0);
	uint blockTracks(Ath__searchBox *box, bool ignoreLevel=true);
	uint blockTracks(dbBlock *block, Ath__array1D<uint> *idTable);
	uint blockTracks(Ath__box *box, uint ignoreLevel);
	uint blockTracks(dbBlock *block, Ath__gridTable *wireSearch);
	float updateFreeTracks(float v);
	void resetFreeTracks();

	void freeTracksAndTables();
	uint getAbsTrackNum(int xy);
	uint getMinMaxTrackNum(int xy);
	bool addOnTrack(uint track, Ath__wire *w, uint mark1, uint mark2);
	int getTrackHeight(uint track);
	uint getTrackNum(Ath__box *box);
	Ath__track* getTrack(int *ll);
	Ath__track *getTrack(int xy);
	Ath__track *getTrack(uint ii, uint markerCnt);
	bool isOrdered(bool ascending, uint *cnt);
	uint search(Ath__searchBox *bb, Ath__array1D<uint> *idtable, bool wireIdFlag=false);
	uint searchAllMarkers(Ath__searchBox *bb, Ath__array1D<uint> *idtable, bool wireIdFlag=false);

	uint addWireNext(uint initTrack, Ath__box *box, int sortedOrder, int *height);
	Ath__wire *getNewWire(Ath__box *box, int xy1, int xy2, uint *id);
	Ath__wire *makeWireCut(Ath__box *box, uint *id, uint *m1, uint *m2, uint cutFlag);
	Ath__wire *makeWireExt(Ath__box *box, uint *id, uint *m1, uint *m2, uint extFlag, int height);
	uint placeWire(uint initTrack, Ath__wire *w, uint mark1, uint mark2, int sortedOrder, int *height);
	uint addWireCut(uint cutFlag, uint initTrack, Ath__box *box, int sortedOrder, int *height);
	uint addWireExt(uint cutFlag, uint initTrack, Ath__box *box, int sortedOrder, int *height);

	void getBusObs(Ath__zui *zui);
	void getBoxes(Ath__array1D<uint> *table);
	uint getBoxes(uint ii, Ath__array1D<uint> *table);

	void placeAgainSortedThru(uint space, uint width, AthArray<Ath__box*> *obsTable);
	uint getDir();
	Ath__wire* getWire_Linear(uint id);
	void setIgnoreFlag(uint v);

	int placeTileWire(uint wireType, uint id, Ath__searchBox *bb, int loMarker, int initTrack, bool skipResetSize);
	void getBuses(Ath__array1D<Ath__box*> *boxtable, uint width);
	friend class Ath__quadTable;

};
class Ath__gridStack // DELETE after porting on new DB
{
private:
	Ath__grid *_nextGridTable[2];
	Ath__grid *_thruGridTable[2];
	Ath__grid *_cornerGridTable[2][2];
	int _loDivide[2];
	int _hiDivide[2];
	uint _level[2];
	uint _nextLevel[2];
	adsRect _bb;


public:
	
	Ath__gridStack(Ath__layer **met, Ath__layer **nextMet, uint *loFlag, uint *hiFlag, int *loLine, int *hiLine, int *ll, int *ur, AthPool<Ath__track> *trackPool, 
							   AthPool<Ath__wire> *wirePool);
	~Ath__gridStack();
	bool isThruPlaced(uint dir);
	uint getGridOutlines(Ath__zui *zui);
	uint blockTracks(AthArray<Ath__box*> *obsTable);
	uint addWire(Ath__box *box, uint trackRange);

	uint placeSortedThru(uint dir, Ath__box ** boxArray, uint cnt, uint width, uint space);
	uint blockTracks(Ath__box* boxList, uint dir);
	uint blockTracks(Ath__box* box);
	Ath__grid **getCornerGrid(uint type, uint space, uint width, uint layer0, uint layer2);
	uint placeCornerSorted(uint type, Ath__box ** boxArray, Ath__box ** boxArrayNext, 
									   uint cnt, uint space, uint width);
	Ath__grid *getThruGrid(uint dir, uint layer, uint space, uint width);
	Ath__grid *getThruGrid(uint dir);
	Ath__grid *getNextGrid(uint dir);
	Ath__grid *getNextGrid(uint dir, uint layer, uint space, uint width);
	uint placeSortedNext(uint dir, Ath__box ** boxArray, int *heightArray, uint cnt, uint space, uint width);
	void getBusObs(Ath__zui *zui);
	bool isPlacedCorner(uint type);


};
class Ath__gridTile
{
private:
	Ath__grid **_gTable;
	uint _levelCnt;
	adsRect _bb;

	bool _poolFlag;
	AthPool<Ath__track> *_trackPool;
	AthPool<Ath__wire> *_wirePool;

public:
	Ath__gridTile(uint levelCnt, int x1, int y1, int x2, int y2, 
		AthPool<Ath__track> *trackPoolPtr=NULL, AthPool<Ath__wire> *wirePoolPtr=NULL);
	~Ath__gridTile();
	Ath__grid* addGrid(Ath__box *bb, uint level, uint dir, 
							 uint layerNum, uint width, uint pitch);
	Ath__grid* addGrid(int x1, int y1, int x2, int y2, uint level, uint dir, 
							 uint layerNum, uint width, uint pitch);
	void searchWires(Ath__searchBox *bb, Ath__array1D<uint> *idtable);

	void getBounds(int *x1, int *y1, int *x2, int *y2);
	Ath__grid* getGrid(uint level);
	void addGrid(Ath__grid *g);

	void getBuses(Ath__array1D<Ath__box*> *boxTable, dbTech *tech);

};

class Ath__gridTable
{
private:
	Ath__grid ***_gridTable;
	Ath__box _bbox;
	adsRect _rectBB;
	uint _rowCnt;
	uint _colCnt;
	uint _rowSize;
	uint _colSize;
	AthPool<Ath__track> *_trackPool;
	AthPool<Ath__wire> *_wirePool;
	uint _schema;

public:
	Ath__gridTable(Ath__box *bb, uint rowSize, uint colSize, uint layer, uint dir, uint width, uint pitch);
	Ath__gridTable(dbBox *bb, uint rowSize, uint colSize, 
		uint layer, uint dir, uint width, uint pitch, uint minWidth);
	Ath__gridTable(adsRect *bb, uint layer, uint dir, uint width, uint pitch, uint minWidth);
	Ath__gridTable(adsRect *bb, uint rowCnt, uint colCnt, uint *width, uint *pitch, uint *spacing);
	~Ath__gridTable();
	Ath__grid *getGrid(uint row, uint col);
	void init1(uint memChunk, uint rowSize, uint colSize, uint dx, uint dy);
	uint getColCnt();
	Ath__wire* getWirePtr(uint id);
	int xMin();
	int xMax();
	int yMin();
	int yMax();
	uint getRowNum(int x);
	uint getColNum(int y);
	bool getRowCol(int x1, int y1, uint *row, uint *col);
	Ath__wire *addBox(Ath__box *bb);
	Ath__wire *addBox(dbBox *bb);
	bool addBox(dbBox *bb, uint row, uint col);
	uint adjustMarkers(uint startRow, uint startCol);

	uint getBoxes(Ath__box *bb, Ath__array1D<Ath__box*> *table);
	bool isOrdered(bool ascending);
	uint search(Ath__zui *zui, uint row, uint col);
	uint search(Ath__searchBox *bb, uint row, uint col, Ath__array1D<uint> *idTable, bool wireIdFlag);
	uint search(Ath__searchBox *bb, Ath__array1D<uint> *idTable);
	uint search(Ath__box *bb);
	Ath__wire * getWire_Linear(uint instId);
};


#endif
