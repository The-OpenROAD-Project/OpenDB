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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <stdio.h>
#include "atypes.h"
#include "db1.h"
#include "layout.h"
#include "tcl.h"
#include "tech.h"

#include "ads.h"
#include "db.h"

#include "ISdb.h"
#include "IZdcr.h"
#include "dbShape.h"
#include "geom.h"

using namespace odb;

class Groute;

class Ath__db;
class Ath__tech;

class dbBlockHandle
{
 public:
  dbBlock*    _block;
  dbDatabase* _db;
  dbTech*     _tech;
  uint        _schema;
  uint        _id;

  // Ath__layoutTable *_layoutTable;

  ZPtr<ISdb>  _netSdb;
  ZPtr<ISdb>  _instSdb;
  ZPtr<IZdcr> _extGuiDcr;

  Ath__gridTable* _dbInstSearch;
  Ath__gridTable* _dbNetWireSearch;
  Ath__gridTable* _dbTrackSearch;
  Ath__quadTable* _quad;

  Ath__gridTable* _grGcellSearch;
  Ath__gridTable* _grGpathSearch;
  Groute*         _grObject;

 public:
  dbBlockHandle(dbBlock* blk, dbTech* tech, uint id, uint schema);
  uint makeInstSearchDb();
  void makeNetWireSearchDb();
  void getInstBoxes(Ath__zui* zui, Ath__hierType hier, bool ignoreFlag = false);
  void addBoxList(Ath__zui* zui, dbInst* inst, bool ignoreFlag = false);

  dbNet* getDbNet(Ath__zui* zui);

  void inspectInst(Ath__zui* zui);
  void inspectInstPin(Ath__zui* zui);
  void inspectInstObs(Ath__zui* zui);
  void inspectSignal(Ath__zui* zui);
  void inspectPower(Ath__zui* zui);
  void inspectBterm(Ath__zui* zui);
  void inspectBlock(Ath__zui* zui);
  void inspectBlockObs(Ath__zui* zui);

  void inspectTilePin(Ath__zui* zui, uint boxId, const char* action);
  int  inspect_1(Ath__zui*   vzui,
                 char        objType,
                 int         boxType,
                 uint        nameId,
                 const char* action,
                 const char* objName);
  void getSignalWires(Ath__zui*     zui,
                      Ath__hierType hier,
                      bool          ignoreFlag = false);
  void addBoxList(Ath__zui* zui, dbNet* net, Ath__boxType boxType);
  uint addInstBoxes(Ath__zui* zui, dbNet* net, bool ignoreZuiFlags);
  void getPowerWires(Ath__zui*     zui,
                     Ath__hierType hier,
                     bool          ignoreFlag = false);
  void getPowerWires(Ath__zui* zui, Ath__hierType hier, uint layer);
  uint getWireVias(Ath__zui*     zui,
                   bool          ignoreFlag,
                   Ath__hierType hier,
                   Ath__boxType  boxType,
                   dbNet*        targetNet   = NULL,
                   bool          excludeFlag = false);
  uint getWiresAndVias_all(Ath__zui* zui, dbNet* targetNet, bool ignoreFlag);

  uint getTilePins(Ath__zui* zui);
  uint getTileBuses(Ath__zui* zui);

  uint getQuadCnt(int modSize, int xy1, int xy2);
  uint makeQuadTable(uint rowCnt, uint colCnt, uint metH, uint metV);
  uint assignTracks(uint metH, uint metV);

  uint   getShapeCoords(dbShape*        s,
                        Ath__searchBox* bb,
                        uint            id1,
                        uint            id2,
                        uint            level);
  dbNet* getShapeCoords(bool            wireVia,
                        Ath__searchBox* bb,
                        uint            wireId,
                        uint            shapeId);

  uint   createTblocks();
  dbNet* getNetAndCoords(Ath__searchBox* bb,
                         bool            skipVia,
                         bool            signal,
                         uint            wid);
  void   getSignalWireIds(Ath__zui* zui, bool ignoreLayers = false);
  void   getSearchWireIds(Ath__zui* zui, bool ignoreLayers = false);
  void   getSignalWires(Ath__zui* zui, Ath__hierType hier, dbNet* net);
  void   getSignalWiresMinusNet(Ath__zui*     zui,
                                Ath__hierType hier,
                                dbNet*        targetNet);
  uint   getNetMaxBox(dbNet* net, Ath__searchBox* maxBB);
  void   getWires(int x1, int y1, int x2, int y2);
  void   getInstShapes(Ath__zui*     zui,
                       Ath__hierType hier,
                       bool          vias,
                       bool          pins,
                       bool          obs);

  void addNetSBoxesOnSearch(dbNet* net, bool skipVias);
  void addNetShapesOnSearch(dbNet* net, bool skipVias);
  void makeGrouteSearchDb();

  uint getItermShapes(Ath__zui*     zui,
                      dbInst*       inst,
                      Ath__hierType hier,
                      bool          viaFlag);
  uint getInstObs(Ath__zui*     zui,
                  dbInst*       inst,
                  Ath__hierType hier,
                  bool          viaFlag);
  uint addInstShapes(Ath__zui*     zui,
                     dbInst*       inst,
                     Ath__hierType hier,
                     bool          vias,
                     bool          pinFlag,
                     bool          obsFlag);
  uint getBtermBoxes(Ath__zui* zui, Ath__hierType hier, bool ignoreFlag);
  uint getBlockBox(Ath__zui*     zui,
                   Ath__hierType hier,
                   uint          level,
                   uint          dd,
                   bool          ignoreFlag);

  int inspect_1(Ath__zui* zui);

  uint getViaLevel(dbShape* s);
  uint getViaLevel(dbSBox* s);
  int  getShapeLevel(dbShape* s, bool wireVia);
  int  getShapeLevel(dbSBox* s, bool wireVia);
  void getSignalWiresAndVias(Ath__zui*     zui,
                             Ath__hierType hier,
                             bool          ignoreFlag);
  uint getNetConnectivity(Ath__zui* zui,
                          dbNet*    net,
                          bool      contextFlag,
                          bool      ignoreLayerFlags,
                          bool      ignoreZuiFlags,
                          bool      ignoreBB = false);
  uint getItermShapes(Ath__zui*     zui,
                      dbITerm*      iterm,
                      Ath__hierType hier,
                      bool          viaFlag);

  void getNetSBoxesFromSearch(dbNet*              net,
                              Ath__array1D<uint>* idTable,
                              Ath__searchBox*     maxBox);
  void getNetShapesFromSearch(dbNet*              net,
                              Ath__array1D<uint>* idTable,
                              Ath__searchBox*     maxBox);
  void getWireIds(Ath__array1D<uint>* wireIdTable, Ath__array1D<uint>* idtable);

  uint getNetContext(Ath__zui* zui, dbNet* net, bool ignoreLayerFlags);
  uint addInstBoxes(Ath__zui* zui,
                    dbInst*   inst,
                    bool      instBoxes,
                    bool      termShapes,
                    bool      instObs);
  uint getNetContext(Ath__zui* zui,
                     dbNet*    net,
                     bool      ignoreZuiFlags,
                     bool      ignoreLayerFlags);
  uint getNetFromSearch(Ath__zui* zui,
                        dbNet*    net,
                        bool      ignoreZuiFlags,
                        bool      ignoreBB);
  uint getConnectivityWires(Ath__zui* zui, dbInst* inst, bool ignoreZuiFlags);
  void addInstConnList(Ath__zui* zui, dbInst* inst, bool ignoreZuiFlags);
  dbInst*  getDbInst(Ath__zui* zui);
  dbBTerm* getDbBTerm(Ath__zui* zui);
  void     white(Ath__zui* zui, Ath__hierType hier, bool ignoreFlag);

  uint addBtermBoxes(Ath__zui* zui, dbBTerm* bterm, Ath__hierType hier);
  uint addBtermBoxes(Ath__zui*       zui,
                     dbSet<dbBTerm>& bterms,
                     Ath__hierType   hier,
                     bool            ignoreFlag);
  uint addBlockagesOnSearch(dbBlock* block);
  uint getBlockObs(Ath__zui* zui, bool ignoreFlag);
  uint getDbBoxId(uint wid, uint wireType);

  bool setInterface2SdbAtDB();  // ISDB
  bool connect2BlockSdb();      // ISDB

  uint makeTrackSearchDb();
  uint getTracks(Ath__zui* zui, bool ignoreLayers);
  uint addTracks(dbTrackGrid* g, uint dir, uint level, int* lo, int* hi);
};

class Ath__zroute
{
 public:
  dbDatabase* _new_db;
  dbTech*     _new_tech;
  dbLib*      _new_lib;
  dbChip*     _new_chip;

  uint _dbSchema;

  Ath__zui*    _zui;
  Ath__zui*    _vui;
  Ath__parser* _parser;

  uint  _currentBlockIndex;
  uint  _currentWordCnt;
  uint* _tmpTable_uint;
  uint  _allFlag;

  Ath__db* _rdb;
  Ath__db* _wdb;

  Groute* grouteHandle;

  dbBlockHandle* _handleTable[128];
  uint           _handleCnt;

  bool _skipTopBlock;
  bool _topBlockOnly;
  bool _isdbFlag;

 public:
  Ath__zroute(uint lcnt, uint mcnt, uint bcnt);
  Ath__layoutTable* createLayoutTable(char* design);
  ~Ath__zroute();
  Ath__layoutTable* getLayoutTable(char* design);
  int               getCoords(char* bb, int* x1, int* y1, int* x2, int* y2);
  int               get2ints(char* bb, char* sep, int* x1, int* y1);
  char getNameIds(const char* word, char* sep, int* nameId, int* blockId);
  char getNameIds3(const char* word,
                   char*       sep,
                   int*        nameId,
                   int*        blockId,
                   int*        typeId);
  int  getInt(char* word, int start);
  uint zui_get(char* filter);
  Ath__layoutTable* getBlock(uint ii);
  uint makeTiles(char* list, int rows, int cols, char* sepH, char* sepV);
  uint makeTiles_1(char* list, int rows, int cols, char* sepH, char* sepV);
  Ath__layoutTable* getFirstBlock(char* list);
  Ath__layoutTable* getNextBlock();
  int               assign_tracks(char* list, char* topH, char* topV);
  int               assign_tracks_1(char* list, char* topH, char* topV);

  Ath__zui* chip_get(const char* bb,
                     const char* layer,
                     const char* type,
                     const char* hierarchy);
  Ath__zui* chip_get(const char* bb,
                     const char* layer,
                     const char* type,
                     const char* hierarchy,
                     uint        x,
                     uint        y,
                     int         maxobjects,
                     const char* noclip);
  Ath__zui* tiles_get(char* bb);

  void     getChipAttributes(char* bbox,
                             char* layers,
                             char* hierarchy,
                             char* objects);
  int      assign_pins(char* list,
                       char* tile,
                       int   connType,
                       int   priority,
                       int   style,
                       int   space,
                       int   space_units,
                       int   width,
                       int   width_units,
                       int   length,
                       int   length_units,
                       char* top_h_layer,
                       char* top_v_layer);
  int      assign_pins_1(char* list,
                         char* tile,
                         int   connType,
                         int   priority,
                         int   style,
                         int   space,
                         int   space_units,
                         int   width,
                         int   width_units,
                         int   length,
                         int   length_units,
                         char* top_h_layer,
                         char* top_v_layer);
  int      make_buses(char* list, char* tile, int opt, int width, int space);
  int      make_buses_1(char* list, char* tile, int opt, int width, int space);
  Ath__db* destroy_db(Ath__db* db);
  Ath__db* create_db(Ath__db* db,
                     char*    rw,
                     char*    dir,
                     int      format,
                     FILE*    tclFP,
                     bool     singleFile);

  void resetZuiFlags(const char* layer, const char* hier, const char* type);
  void setZuiFlags(const char* layer, const char* hier, const char* type);
  void resetZuiFlags(uint hierFlag, const char* type);
  void setZuiFlags(uint hierFlag, const char* type);

  uint setZuiLayers(const char* layers);
  uint resetZuiLayers(const char* layers);

  char*             getBlockName(uint id);
  Ath__layoutTable* getBlock(char* objectName,
                             char* objectType,
                             int*  nameId,
                             int*  blockId,
                             int*  boxType);

  Ath__zui* getPullDownMenu(char* objectName);
  Ath__zui* resetZui(uint blockId, char* bb, bool vFlag, bool noclip);

  void           chip_get_1();
  uint           getLayers_1(char* layerBuff);
  void           initHandleTable();
  void           destroyHandleTable();
  uint           mkSearchDb(dbChip* chip, bool isdb);
  dbBlock*       getDbBlock(uint ii);
  dbBlockHandle* getBlockHandle(dbBlock* blk);
  dbBlockHandle* getBlockHandle(uint ii);
  void           getMaxGuiBox_0(char* buffer);
  void           getMaxGuiBox_1(char* buffer);
  uint           tiles_get_1(char* bb);

  dbBlockHandle* getFirstBlockHandle(char* list);
  dbBlockHandle* getNextBlockHandle();
  uint           makeTblocks(char* list);
  void mkSearchDb(uint force, bool topBlockOnly, bool skipTopBlock, bool isdb);
};
