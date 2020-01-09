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

#include "zui.h"
#include <stdio.h>
#include <stdlib.h>
#include "dbTypes.h"
#include "wire.h"

#define ZUI_GRID_SIZE 1024

Ath__zbox::Ath__zbox()
{
  setCoords(0, 0, 0, 0);
  clearFlags();
}
Ath__zbox::Ath__zbox(int x1, int y1, int x2, int y2)
{
  setCoords(x1, y1, x2, y2);
  clearFlags();
}
void Ath__zbox::setCoords(int x1, int y1, int x2, int y2)
{
  _xlo = x1;
  _ylo = y1;
  _xhi = x2;
  _yhi = y2;
}
void Ath__zbox::clearFlags()
{
  setZuiIdentity(0, 0, 0, 0, 0, 0);
}
void Ath__zbox::setZuiIdentity(uint blockId,
                               uint id,
                               uint type,
                               uint ownId,
                               uint hier,
                               uint layer)
{
  _blockId     = blockId;
  _nameId      = id;
  _type        = type;
  _hier        = hier;
  _ownerNameId = ownId;
  _layer       = layer;
}
void Ath__zbox::setZuiMark(uint mark)
{
  _zuiMark = mark;
}

Ath__2dCounter::Ath__2dCounter(uint m, uint n)
{
  _allocRowCnt = m;
  _allocColCnt = n;

  _array = new uint*[m];
  for (uint ii = 0; ii < m; ii++) {
    _array[ii] = new uint[n];
    for (uint jj = 0; jj < n; jj++)
      _array[ii][jj] = 0;
  }
}
Ath__2dCounter::~Ath__2dCounter()
{
  for (uint ii = 0; ii < _allocRowCnt; ii++)
    delete[] _array[ii];
  delete _array;
}
void Ath__2dCounter::setup(Ath__box* bb, uint modX, uint modY)
{
  _loX = bb->_xlo;
  _loY = bb->_ylo;
  _hiX = bb->_xhi;
  _hiY = bb->_yhi;

  if (modX <= 0) {
    ATH__failMessage("Ath__2dCounter:invalid modulo num!");
  }
  if (modY <= 0) {
    ATH__failMessage("Ath__2dCounter:invalid modulo num!");
  }
  _modX   = modX;
  _modY   = modY;
  _rowCnt = (_hiY - _loY) / modY;
  if (_rowCnt >= _allocRowCnt) {
    ATH__failMessage("Ath__2dCounter:not enough rows allocated");
  }

  _colCnt = (_hiX - _loX) / modX;
  if (_colCnt >= _allocColCnt) {
    ATH__failMessage("Ath__2dCounter:not enough cols allocated");
  }
  for (uint ii = 0; ii < _rowCnt; ii++)
    for (uint jj = 0; jj < _colCnt; jj++)
      _array[ii][jj] = 0;
}
void Ath__2dCounter::mkPixelRects(uint layer, Ath__zui* zui)
{ /* TODO
         for (uint ii= 0; ii<_rowCnt; ii++) {
                 uint y1= _loY + ii*_modY;
                 uint y2= y1+_modY;

                 uint jj= 0;
                 while (jj<_colCnt) {
                         uint kk= jj;
                         for ( ; (kk<_colCnt)&&(_array[ii][kk]==0);kk++)
                                 ;
                         if (kk==_colCnt)
                                 break;

                         uint start= kk;
                         for ( ; (kk<_colCnt)&&(_array[ii][kk]>0);kk++)
                                 ;
                         uint end= kk;

                         uint x1= _loX + start*_modX;
                         uint x2= _loX + (end+1)*_modX;

                         zui->addBox(0, Ath_hier__tile, 0,0, layer,
     x1,y1,x2,y2);

                         jj= kk;
                 }
         }
         */
}

uint Ath__zui::mkPixelRects()
{
  resetZtable();
  _detail = 1;
  for (uint ii = 0; ii < _layerCnt; ii++)
    _metPixelTable[ii]->mkPixelRects(ii, this);

  return 0;
}
void Ath__2dCounter::cntUp(int x1, int y1, int x2, int y2)
{
  if (y2 == y1) {  // horizontal
    for (int jj = x1; jj <= x2; jj++)
      _array[y1][jj]++;
  } else if (x2 == x1) {  // vertical
    for (int jj = y1; jj <= y2; jj++)
      _array[jj][x1]++;
  } else {
    for (int ii = y1; ii <= y2; ii++)
      for (int jj = x1; jj <= x2; jj++)
        _array[ii][jj]++;
  }
}
void Ath__2dCounter::update(Ath__zbox* bb)
{
  int x1 = (bb->_xlo - _loX) / _modX;
  int y1 = (bb->_ylo - _loY) / _modY;
  int x2 = (bb->_xhi - _loX) / _modX;
  int y2 = (bb->_yhi - _loY) / _modY;

  cntUp(x1, y1, x2, y2);
}
void Ath__2dCounter::update(int xlo, int ylo, int xhi, int yhi)
{
  int x1 = (xlo - _loX) / _modX;
  int y1 = (ylo - _loY) / _modY;
  int x2 = (xhi - _loX) / _modX;
  int y2 = (yhi - _loY) / _modY;

  cntUp(x1, y1, x2, y2);
}
void Ath__zui::initPixelArrays(uint layerCnt)
{
  _metPixelTable = new Ath__2dCounter*[_layerCnt];
  for (uint ii = 0; ii < _layerCnt; ii++)
    _metPixelTable[ii] = new Ath__2dCounter(ZUI_MAP_SIZE, ZUI_MAP_SIZE);
}
void Ath__zui::getHierBoxTypes(char* hierBuff, char* boxBuff)
{
  _hierTypeTable->getNameList(hierBuff);
  _boxTypeTable->getNameList(boxBuff);
}
uint Ath__zui::initGuiMenus()
{
  initDb2HierTypeMapping();
  _boxTypeTable  = new Ath__associateArray(16, 64);
  _hierTypeTable = new Ath__associateArray(16, 64);
  uint typeCnt   = initBoxTypes();
  uint hierCnt   = initHierTypes();
  resetAllExcludeLayers();

  _menu = new Ath__associateArray**[hierCnt];
  for (uint ii = 0; ii < hierCnt; ii++) {
    _menu[ii] = new Ath__associateArray*[typeCnt];
    for (uint jj = 0; jj < typeCnt; jj++)
      _menu[ii][jj] = new Ath__associateArray(32, 64, false);
  }
  defineMenuPaths();

  initPullDownMenus();

  _markTable = new Ath__associateArray(16, 32);
  initMarks();

  return typeCnt;
}
void Ath__zui::deallocGuiMenus()
{
  for (uint ii = 0; ii < _hierTypeTable->getCnt(); ii++) {
    for (uint jj = 0; jj < _boxTypeTable->getCnt(); jj++)
      delete _menu[ii][jj];

    delete[] _menu[ii];
  }
  delete[] _menu;

  delete _boxTypeTable;
  delete _hierTypeTable;
}
void Ath__zui::addPullDownMenu(uint           hier,
                               uint           btype,
                               Ath__zuiAction action,
                               char*          db_name,
                               char*          zui_action_name)
{
  _menu[hier][btype]->add(action, db_name, zui_action_name);
}
char* Ath__zui::getPullDownMenu(char* objHierWord, uint objType)
{
  _tclBufFlag = true;
  uint n      = _hierTypeTable->getTableIndex(objHierWord);
  _menu[n][objType]->printAll(_tclBuff);

  return _tclBuff;
}
char* Ath__zui::getPullDownMenu(uint hierType, uint boxType)
{
  _tclBufFlag = true;
  _menu[hierType][boxType]->printAll(_tclBuff);

  return _tclBuff;
}
bool Ath__zui::getPullDownMenu(char* objectName)
{
  if (objectName == NULL) {
    sprintf(_tclBuff, "%s", "NULL Object Name");
    return false;
  }

  int n = _parser->mkWords(objectName, ".");
  if (n < 3) {
    sprintf(_tclBuff, "%s", "Invalid Object Name");
    return false;
  }
  int   boxType     = _parser->getInt(2);
  char* objHierWord = _parser->get(1);

  char* buff = getPullDownMenu(objHierWord, boxType);

  return true;
}
bool Ath__zui::hierType_signal()
{
  return (_inspHierId == Ath_hier__signal) ? true : false;
}
bool Ath__zui::hierType_inst()
{
  return (_inspHierId == Ath_hier__inst) ? true : false;
}
bool Ath__zui::hierType_block()
{
  return (_inspHierId == Ath_hier__block) ? true : false;
}
bool Ath__zui::hierType_groute()
{
  return (_inspHierId == Ath_hier__groute) ? true : false;
}
bool Ath__zui::hierType_power()
{
  return (_inspHierId == Ath_hier__power) ? true : false;
}
bool Ath__zui::hierType_tile()
{
  return (_inspHierId == Ath_hier__tile) ? true : false;
}
bool Ath__zui::boxType_bbox()
{
  return (_inspBoxId == Ath_box__bbox) ? true : false;
}
bool Ath__zui::boxType_wire()
{
  return (_inspBoxId == Ath_box__wire) ? true : false;
}
bool Ath__zui::boxType_via()
{
  return (_inspBoxId == Ath_box__via) ? true : false;
}
bool Ath__zui::boxType_obs()
{
  return (_inspBoxId == Ath_box__obs) ? true : false;
}
bool Ath__zui::boxType_gpath()
{
  return (_inspBoxId == Ath_box__gpath) ? true : false;
}
bool Ath__zui::boxType_pin()
{
  return (_inspBoxId == Ath_box__pin) ? true : false;
}
bool Ath__zui::boxType_via(uint box)
{
  return (box == Ath_box__via) ? true : false;
}
bool Ath__zui::hierType_power(uint hier)
{
  return (hier == Ath_hier__power) ? true : false;
}
bool Ath__zui::getHierBoxFlag(uint hier, uint box, bool ignoreFlags)
{
  if (ignoreFlags)
    return true;

  char buf[128];
  sprintf(
      buf, "%s/%s", _hierTypeTable->getName(hier), _boxTypeTable->getName(box));

  return getDbFlag(buf);
}
bool Ath__zui::getNetWireViasFlag(bool ignoreFlag)
{
  return !ignoreFlag
         && (getHierBoxFlag(Ath_hier__signal, Ath_box__wire)
             || getHierBoxFlag(Ath_hier__signal, Ath_box__via)
             || getHierBoxFlag(Ath_hier__power, Ath_box__wire)
             || getHierBoxFlag(Ath_hier__power, Ath_box__via));
}
uint Ath__zui::getHierId()
{
  return _inspHierId;
}
uint Ath__zui::getInspNameId()
{
  return _inspNameId;
}
char* Ath__zui::getInspectName()
{
  return _inspName;
}
bool Ath__zui::msgAction()
{
  if (strcmp(_inspAction, "msg/msg") == 0)
    return true;
  else
    return false;
}
bool Ath__zui::selfAction()
{
  if (strstr(_inspAction, "self") != NULL)
    return true;
  else
    return false;
}
bool Ath__zui::groupAction1()
{
  // TODO - have to
  return strstr(_inspAction, "1/group") ? true : false;
}
bool Ath__zui::groupAction2()
{
  return strstr(_inspAction, "2/group") ? true : false;
}
bool Ath__zui::linkAction()
{
  if (strstr(_inspAction, "links") != NULL)
    return true;
  else
    return false;
}
bool Ath__zui::selectAction()
{
  return _selectFlag;
}
uint Ath__zui::setInspectParams(char* action,
                                char* objectName,
                                char* bb,
                                char* selectType)
{
  resetTable(1);

  if (_parser->mkWords(bb, " ") > 0) {
    setSearchBox(_parser->getInt(0),
                 _parser->getInt(1),
                 _parser->getInt(2),
                 _parser->getInt(3),
                 0);
  } else {
    invalidateSearchBox();
  }

  _selectFlag = false;

  if (strcmp(selectType, "") != 0) {
    _selectFlag = true;
    int n       = _parser->mkWords(selectType, "/");
    if (n <= 0) {
      _inspBlockId = 0;
      return 0;
    }
    _inspBlockId = 1;
    setBlockId(_inspBlockId);

    _inspHierId = _hierTypeTable->getTableIndex(_parser->get(0));
    _inspBoxId  = 7;

    strcpy(_inspName, objectName);
    if (strcmp(_parser->get(1), "id") == 0)
      _inspNameId = atoi(objectName);
    else
      _inspNameId = 0;

    strcpy(_inspAction, "group");
    return _inspBlockId;
  }

  strcpy(_inspName, objectName);

  int n = _parser->mkWords(objectName, ".");
  if (n < 3) {
    sprintf(_tclBuff, "%s", "Invalid Object Name");
    return 0;
  }
  _inspNameId = _parser->getInt(1, 1);
  if (_inspNameId == 0) {
    sprintf(_tclBuff, "%s", "Invalid NameId");
    return 0;
  }
  _inspHierId = _hierTypeTable->getTableIndex(_parser->get(1));

  _inspBlockId = _parser->getInt(0, 1);
  ;
  if (_inspBlockId == 0) {
    sprintf(_tclBuff, "%s", "Invalid Block Id");
    return 0;
  }
  setBlockId(_inspBlockId);

  _inspBoxId = _parser->getInt(2);

  strcpy(_inspAction, action);

  return _inspBlockId;
}

void Ath__zui::setCurrentMark(Ath__zuiMark mark)
{
  _currentMark = mark;
}
uint Ath__zui::initMarks()
{
  _currentMark = 0;
  uint n       = 0;
  n            = _markTable->add(Ath_mark__none, "", "");
  n            = _markTable->add(Ath_mark__main, "main", "m");
  n            = _markTable->add(Ath_mark__signalWire, "signalWires", "s");
  n            = _markTable->add(Ath_mark__powerWire, "power", "p");
  n            = _markTable->add(Ath_mark__instConn, "instConn", "i");
  n            = _markTable->add(Ath_mark__netConn, "netConn", "n");
  n            = _markTable->add(Ath_mark__btermConn, "btermConn", "m");
  n            = _markTable->add(Ath_mark__pinTarget, "pinTarget", "g");
  n            = _markTable->add(Ath_mark__pinGravity, "pinGravity", "v");
  n            = _markTable->add(Ath_mark__pinPlaced, "pinPlaced", "d");
  n            = _markTable->add(Ath_mark__busPlaced, "bus", "b");
  n            = _markTable->add(Ath_mark__tile, "tile", "t");
  n            = _markTable->add(Ath_mark__context, "context", "c");

  return _markTable->getCnt();
}
void Ath__zui::initPullDownMenus()
{
  //	"self {Node Attributes}  nets  {Connectivity}"

  addPullDownMenu(
      Ath_hier__inst, Ath_box__bbox, Ath_gui__self, "self", "Inst Attributes");
  addPullDownMenu(Ath_hier__inst,
                  Ath_box__bbox,
                  Ath_gui__flightLines,
                  "inst/links",
                  "Inst Flight Lines");
  addPullDownMenu(Ath_hier__inst,
                  Ath_box__bbox,
                  Ath_gui__connectivity,
                  "inst1/group",
                  "Inst Connectivity");
  addPullDownMenu(Ath_hier__inst,
                  Ath_box__bbox,
                  Ath_gui__connectPlus,
                  "inst2/group",
                  "Inst Connectivity+Wires");

  addPullDownMenu(Ath_hier__inst,
                  Ath_box__pin,
                  Ath_gui__self,
                  "self",
                  "Inst Pin Attributes");
  addPullDownMenu(Ath_hier__inst,
                  Ath_box__pin,
                  Ath_gui__connectivity,
                  "pin1/group",
                  "Net Connectivity");
  addPullDownMenu(Ath_hier__inst,
                  Ath_box__pin,
                  Ath_gui__connectPlus,
                  "pin2/group",
                  "Net Connectivity+Context");

  addPullDownMenu(
      Ath_hier__inst, Ath_box__obs, Ath_gui__self, "self", "Inst Attributes");
  addPullDownMenu(Ath_hier__inst,
                  Ath_box__obs,
                  Ath_gui__flightLines,
                  "obs/links",
                  "Inst Flight Lines");
  addPullDownMenu(Ath_hier__inst,
                  Ath_box__obs,
                  Ath_gui__connectivity,
                  "obs1/group",
                  "Inst Connectivity");

  addPullDownMenu(
      Ath_hier__signal, Ath_box__wire, Ath_gui__self, "self", "Net Attributes");
  addPullDownMenu(Ath_hier__signal,
                  Ath_box__wire,
                  Ath_gui__connectivity,
                  "net1/group",
                  "Net Connectivity");
  addPullDownMenu(Ath_hier__signal,
                  Ath_box__wire,
                  Ath_gui__connectPlus,
                  "net2/group",
                  "Net Connectivity+Context");

  addPullDownMenu(
      Ath_hier__block, Ath_box__pin, Ath_gui__self, "self", "Bterm Attributes");
  addPullDownMenu(Ath_hier__block,
                  Ath_box__pin,
                  Ath_gui__connectivity,
                  "bterm1/group",
                  "Bterm To Net Connectivity");

  /*
          addPullDownMenu(Ath_hier__tnet, Ath_box__pin, Ath_gui__self,
     "self", "Attributes"); addPullDownMenu(Ath_hier__tnet, Ath_box__pin,
     Ath_gui__connectivity,	"conn", "Connectivity");
          addPullDownMenu(Ath_hier__tnet, Ath_box__pin, Ath_gui__flightLines,
     "iconn", "Inst Conns");
          
          addPullDownMenu(Ath_hier__tile, Ath_box__pin,
                  Ath_gui__self,	"self", "Tile Pin Attributes");
          addPullDownMenu(Ath_hier__tile, Ath_box__pin,
                  Ath_gui__connectivity,	"tile/links", "Tile Pin
     Connectivity"); addPullDownMenu(Ath_hier__tile, Ath_box__pin,
                  Ath_gui__flightLines,	"tile/group", "Tile Pin Group");
          
          addPullDownMenu(Ath_hier__tile, Ath_box__bus, Ath_gui__self, "self",
     "Bus Attributes"); addPullDownMenu(Ath_hier__tile, Ath_box__bus,
     Ath_gui__connectivity,	"tile/links", "Bus Connectivity");
          addPullDownMenu(Ath_hier__tile, Ath_box__bus, Ath_gui__flightLines,
     "tile/group", "Bus Group");
  */
  addPullDownMenu(Ath_hier__groute,
                  Ath_box__gcell,
                  Ath_gui__self,
                  "self",
                  "Gcell Attributes");
  //	addPullDownMenu(Ath_hier__groute, Ath_box__wire, Ath_gui__connectivity,
  //"net/links", "Net Connectivity"); 	addPullDownMenu(Ath_hier__groute,
  // Ath_box__wire, Ath_gui__flightLines,	"net/group", "Net Group");
}
uint Ath__zui::mapDb2HierType(dbBox* bb)
{
  dbBoxOwner ownerType = bb->getOwnerType();

  if (ownerType == dbBoxOwner::NET) {
    dbNet* net = (dbNet*) bb->getBoxOwner();
    if (net->getSigType() == dbSigType::SIGNAL)
      return Ath_hier__signal;
    else if (net->getSigType() == dbSigType::CLOCK)
      return Ath_hier__signal;
    else
      return Ath_hier__power;
  }
  return _db2hierTypeMap[ownerType];
  /*
          _db2hierTypeMap[0]= 0;
          _db2hierTypeMap[dbBoxOwner::BLOCK]= Ath_hier__block;
          _db2hierTypeMap[dbBoxOwner::INST]= Ath_hier__inst;
          _db2hierTypeMap[dbBoxOwner::NET]= Ath_hier__signal;
          _db2hierTypeMap[dbBoxOwner::BTERM]= 0;
          _db2hierTypeMap[dbBoxOwner::VIA]= 0;
          _db2hierTypeMap[dbBoxOwner::OBSTRUCTION]= 0;
          _db2hierTypeMap[dbBoxOwner::BLOCKAGE]= 0;
          _db2hierTypeMap[dbBoxOwner::TBLOCK]= Ath_hier__tile;
          _db2hierTypeMap[dbBoxOwner::TNET]= Ath_hier__tnet;
          _db2hierTypeMap[dbBoxOwner::TTERM]= Ath_hier__tile;
          _db2hierTypeMap[dbBoxOwner::MASTER]= 0;
          _db2hierTypeMap[dbBoxOwner::MPIN]= 0;
          _db2hierTypeMap[dbBoxOwner::TECH_VIA]= 0;
  */
}
void Ath__zui::initDb2HierTypeMapping()
{
  _db2hierTypeMap[0]                       = 0;
  _db2hierTypeMap[dbBoxOwner::BLOCK]       = Ath_hier__block;
  _db2hierTypeMap[dbBoxOwner::INST]        = Ath_hier__inst;
  _db2hierTypeMap[dbBoxOwner::NET]         = Ath_hier__signal;
  _db2hierTypeMap[dbBoxOwner::BTERM]       = Ath_hier__block;
  _db2hierTypeMap[dbBoxOwner::VIA]         = 0;
  _db2hierTypeMap[dbBoxOwner::OBSTRUCTION] = 0;
  _db2hierTypeMap[dbBoxOwner::BLOCKAGE]    = 0;
  _db2hierTypeMap[dbBoxOwner::TBLOCK]      = Ath_hier__tile;
  _db2hierTypeMap[dbBoxOwner::TNET]        = Ath_hier__tnet;
  _db2hierTypeMap[dbBoxOwner::TTERM]       = Ath_hier__tile;
  _db2hierTypeMap[dbBoxOwner::MASTER]      = 0;
  _db2hierTypeMap[dbBoxOwner::MPIN]        = 0;
  _db2hierTypeMap[dbBoxOwner::TECH_VIA]    = 0;
}
uint Ath__zui::initHierTypes()
{
  uint n = 0;

  n = _hierTypeTable->add(Ath_hier__chip, "chip", "c");
  n = _hierTypeTable->add(Ath_hier__inst, "inst", "i");
  n = _hierTypeTable->add(Ath_hier__block, "block", "b");
  n = _hierTypeTable->add(Ath_hier__macro, "macro", "m");
  n = _hierTypeTable->add(Ath_hier__cell, "cell", "e");
  n = _hierTypeTable->add(Ath_hier__power, "power", "p");
  n = _hierTypeTable->add(Ath_hier__signal, "signal", "n");
  n = _hierTypeTable->add(Ath_hier__tnet, "tnet", "z");
  n = _hierTypeTable->add(Ath_hier__tile, "tile", "t");
  n = _hierTypeTable->add(Ath_hier__groute, "groute", "g");

  return _hierTypeTable->getCnt();
}
char* Ath__zui::getMenuPathBuff()
{
  return _menuPathBuff;
}
void Ath__zui::resetMenuPathFlags()
{
  for (uint ii = 0; ii < _menuPathCnt; ii++)
    _dbFilterTable[ii + 1] = false;
}

uint Ath__zui::defineMenuPaths()
{
  _menuPathCnt   = 0;
  _pathMenuTable = new Ath__nameTable(256);
  _menuPathBuff  = new char[4096];

  sprintf(_menuPathBuff, "");

  makeMenuPath("Block", 0, Ath_box__zbox, Ath_hier__block, 0, 0);
  makeMenuPath("Bbox", 0, Ath_box__bbox, Ath_hier__block, 0, 0);
  makeMenuPath("Pin", 0, Ath_box__pin, Ath_hier__block, 0, 0);
  makeMenuPath("Obs", 0, Ath_box__obs, Ath_hier__block, 0, 0);

  //	makeMenuPath("Macro", 0, Ath_box__zbox, Ath_hier__macro, 0, 0);
  //	makeMenuPath("Bbox", 0, Ath_box__bbox, Ath_hier__macro, 0, 0);

  makeMenuPath("Inst", 0, Ath_box__zbox, Ath_hier__inst, 0, 0);
  makeMenuPath("Bbox", 0, Ath_box__bbox, Ath_hier__inst, 0, 0);
  makeMenuPath("Obs", 0, Ath_box__obs, Ath_hier__inst, 0, 0);
  makeMenuPath("Pin", 0, Ath_box__pin, Ath_hier__inst, 0, 0);

  makeMenuPath("Signal", 0, Ath_box__zbox, Ath_hier__signal, 0, 0);
  makeMenuPath("Wire", 0, Ath_box__wire, Ath_hier__signal, 0, 0);
  makeMenuPath("Via", 0, Ath_box__via, Ath_hier__signal, 0, 0);

  makeMenuPath("Power", 0, Ath_box__zbox, Ath_hier__power, 0, 0);
  makeMenuPath("Wire", 0, Ath_box__wire, Ath_hier__power, 0, 0);
  makeMenuPath("Via", 0, Ath_box__via, Ath_hier__power, 0, 0);

  makeMenuPath("Tile", 0, Ath_box__zbox, Ath_hier__tile, 0, 0);
  makeMenuPath("Bbox", 0, Ath_box__bbox, Ath_hier__tile, 0, 0);
  makeMenuPath("Tnet", 0, Ath_box__zbox, Ath_hier__tile, Ath_hier__tnet, 0);
  makeMenuPath("Pin", 0, Ath_box__pin, Ath_hier__tile, Ath_hier__tnet, 0);
  makeMenuPath("Bus", 0, Ath_box__bus, Ath_hier__tile, Ath_hier__tnet, 0);

  makeMenuPath("Groute", 0, Ath_box__zbox, Ath_hier__groute, 0, 0);
  makeMenuPath("gcell", 0, Ath_box__gcell, Ath_hier__groute, 0, 0);
  makeMenuPath("gpath", 0, Ath_box__gpath, Ath_hier__groute, 0, 0);
  makeMenuPath("spots", 0, Ath_box__spots, Ath_hier__groute, 0, 0);

  _dbFilterTable = new bool[_menuPathCnt + 1];
  resetMenuPathFlags();

  return 0;
}
uint Ath__zui::initBoxTypes()
{
  uint n = 0;

  n = _boxTypeTable->add(Ath_box__zbox, "zbox", "z");
  n = _boxTypeTable->add(Ath_box__bbox, "bbox", "b");
  n = _boxTypeTable->add(Ath_box__wire, "wire", "w");
  n = _boxTypeTable->add(Ath_box__via, "via", "v");
  n = _boxTypeTable->add(Ath_box__obs, "obs", "o");
  n = _boxTypeTable->add(Ath_box__track, "track", "t");
  n = _boxTypeTable->add(Ath_box__bus, "bus", "u");
  n = _boxTypeTable->add(Ath_box__pin, "pin", "p");
  n = _boxTypeTable->add(Ath_box__gcell, "gcell", "c");
  n = _boxTypeTable->add(Ath_box__gpath, "gpath", "g");
  n = _boxTypeTable->add(Ath_box__spots, "spots", "s");

  return _boxTypeTable->getCnt();
}
uint Ath__zui::makeMenuPath(char* menuTabName,
                            int   v,
                            uint  b,
                            uint  h1,
                            uint  h2,
                            uint  h3)
{
  Ath__hierarchy h(b, h1, h2, h3);
  uint           n = h.getAll();

  char bufName[1000];
  sprintf(bufName, "%s", _hierTypeTable->getName(h1));
  if (h2 > 0) {
    sprintf(bufName, "%s/%s", bufName, _hierTypeTable->getName(h2));
  }
  if (h3 > 0) {
    sprintf(bufName, "%s/%s", bufName, _hierTypeTable->getName(h3));
  }
  if (b > 0) {
    sprintf(bufName, "%s/%s", bufName, _boxTypeTable->getName(b));
  }
  uint num = _pathMenuTable->addNewName(bufName, n);

  sprintf(_menuPathBuff,
          "%s        %s \"%s\" %d\n",
          _menuPathBuff,
          bufName,
          menuTabName,
          v);

  _menuPathCnt++;
  return num;
}

// Ath__hierarchy::Ath__hierarchy(Ath__boxType b, Ath__hierType h1,
// Ath__hierType h2, Ath__hierType h3)
Ath__hierarchy::Ath__hierarchy(uint b, uint h1, uint h2, uint h3)
{
  _type._box = b;
  _type._h0  = h1;
  _type._h1  = h2;
  _type._h2  = h3;
  _type._h3  = 0;
}
Ath__hierarchy::Ath__hierarchy(uint v)
{
  setAll(v);
}
uint Ath__hierarchy::getBox()
{
  return _type._box;
}
uint Ath__hierarchy::getHier(uint n)
{
  if (n == 0) {
    return _type._h0;
  }
  if (n % 2 == 0) {
    return _type._h2;
  }
  if (n > 1) {
    return _type._h3;
  }
  return _type._h1;
}
uint Ath__hierarchy::getAll()
{
  return _all;
}
void Ath__hierarchy::setAll(uint v)
{
  _all = v;
}
Ath__hierarchy Ath__zui::getHierarchyWord(char* name)
{
  uint           tagId   = _pathMenuTable->getTagId(name);
  uint           pathNum = _pathMenuTable->getDataId(tagId);
  Ath__hierarchy h(pathNum);
  return h;
}
bool Ath__zui::getDbFlag(char* name)
{
  return _dbFilterTable[_pathMenuTable->getTagId(name)];
}

void Ath__zui::setTypeHierFlag(char* name)
{
  _dbFilterTable[_pathMenuTable->getTagId(name)] = true;
  return;
  // TO DELETE
  Ath__hierarchy h = getHierarchyWord(name);

  uint bb = h.getBox();
  _boxTypeTable->setFlag(bb);

  uint hier0 = h.getHier(0);
  _hierTypeTable->setFlag(hier0);

  uint hier1 = h.getHier(1);
  _hierTypeTable->setFlag(hier1);
}

void Ath__zui::setTypeFlag(uint flagType, char* name)
{
  if (flagType > 0)
    _boxTypeTable->setFlagByName(name);
  else
    _hierTypeTable->setFlagByName(name);
}
void Ath__zui::setAllTypeFlags(uint flagType)
{
  if (flagType > 0)
    _boxTypeTable->setAllFlag();
  else
    _hierTypeTable->setAllFlag();
}
void Ath__zui::resetAllTypeFlags(uint flagType)
{
  if (flagType > 0)
    _boxTypeTable->resetAllFlag();
  else
    _hierTypeTable->resetAllFlag();
}
void Ath__zui::resetTypeFlag(uint flagType, char* name)
{
  if (flagType > 0)
    _boxTypeTable->resetFlagByName(name);
  else
    _hierTypeTable->resetFlagByName(name);
}

void Ath__zui::resetZuiFlags(uint hierFlag, const char* type)
{
  if ((type == NULL) || (strcmp(type, "") == 0)) {
    resetAllTypeFlags(hierFlag);
    return;
  }
  _parser->mkWords((char*) type, " ");
  for (int ii = 0; ii < _parser->getWordCnt(); ii++) {
    char* word = _parser->get(ii);
    resetTypeFlag(hierFlag, word);
  }
}
void Ath__zui::resetZuiFlags(const char* layer,
                             const char* hier,
                             const char* type)
{
  resetZuiFlags(0, type);
  resetZuiFlags(1, hier);
}
void Ath__zui::setZuiFlags(uint hierFlag, const char* type)
{
  resetAllTypeFlags(0);
  resetAllTypeFlags(1);

  resetMenuPathFlags();

  if ((type == NULL) || (strcmp(type, "") == 0))
    return;

  _parser->mkWords((char*) type, " ");
  for (int ii = 0; ii < _parser->getWordCnt(); ii++) {
    char* word = _parser->get(ii);
    // _zui->setTypeFlag(hierFlag, word);
    setTypeHierFlag(word);
  }
}
uint Ath__zui::resetZuiLayers(const char* layers)
{
  if ((layers == NULL) || (strcmp(layers, "") == 0)) {
    resetAllExcludeLayers();
    return 1;
  }
  _parser->mkWords((char*) layers, " ");
  for (int ii = 0; ii < _parser->getWordCnt(); ii++) {
    int n = _parser->getInt(ii);
    resetExcludeLayer(n);
  }
  return _parser->getWordCnt();
}

uint Ath__zui::setZuiLayers(const char* layers)
{
  if ((layers == NULL) || (strcmp(layers, "") == 0)) {  // get all
    resetAllExcludeLayers();
    return 1;
  }
  setAllExcludeLayers();
  resetExcludeLayer(15);

  _parser->mkWords((char*) layers, " ");
  for (int ii = 0; ii < _parser->getWordCnt(); ii++) {
    int n = _parser->getInt(ii);
    resetExcludeLayer(n);
  }
  return _parser->getWordCnt();
}
void Ath__zui::setZuiFlags(const char* layer,
                           const char* hier,
                           const char* type)
{
  setZuiLayers(layer);
  //	setZuiFlags(0, type);
  setZuiFlags(1, hier);
}

bool Ath__zui::getTypeFlag(uint flagType, uint n)
{
  if (flagType > 0)
    return _boxTypeTable->getFlag(n);
  else
    return _hierTypeTable->getFlag(n);
}
void Ath__zui::setAllExcludeLayers()
{
  for (uint ii = 0; ii < 20; ii++)
    _excludeLayerTable[ii] = true;
}
void Ath__zui::resetAllExcludeLayers()
{
  for (uint ii = 0; ii < 20; ii++)
    _excludeLayerTable[ii] = false;
}
bool Ath__zui::isLayerExluded(int n)
{
  return _excludeLayerTable[n];
}
void Ath__zui::setExcludeLayer(int n)
{
  _excludeLayerTable[n] = true;
}
void Ath__zui::resetExcludeLayer(int n)
{
  _excludeLayerTable[n] = false;
}
Ath__zbox* Ath__zui::getBox(uint ii)
{
  return _tmpZboxTable->get(ii);
}
void Ath__zui::getBox(uint ii, Ath__box* bb)
{
  Ath__zbox* zbb = _tmpZboxTable->get(ii);
  bb->_xlo       = zbb->_xlo;
  bb->_ylo       = zbb->_ylo;
  bb->_xhi       = zbb->_xhi;
  bb->_yhi       = zbb->_yhi;
  bb->_layer     = zbb->_layer;
}
void Ath__zui::setSearchBox(int x1, int y1, int x2, int y2, int layer)
{
  _searchBB.set(x1, y1, x2, y2);
  _searchBB._layer = layer;
}
void Ath__zui::invalidateSearchBox()
{
  _searchBB._valid = 0;
}
bool Ath__zui::getTclBufFlag()
{
  return _tclBufFlag;
}
char* Ath__zui::getTclBuf()
{
  return _tclBuff;
}
char* Ath__zui::getFirstBox(int* x1, int* y1, int* x2, int* y2, uint* layer)
{
  uint       boxCnt = getZboxes();  // iterator
  Ath__zbox* b      = getBox(0);
  *x1               = b->_xlo;
  *y1               = b->_ylo;
  *x2               = b->_xhi;
  *y2               = b->_yhi;
  *layer            = b->_layer;

  return getTclBbox(0, false);
}
char* Ath__zui::getFirstBox()
{
  uint boxCnt = getZboxes();  // iterator
  return getTclBbox(0, false);
}
void Ath__zui::print_msg(char* blk, char* name, char* extra)
{
  _tclBufFlag = true;
  char buff[10000];
  sprintf(buff, "{ msg {%s Block {%s} name {%s}", getFirstBox(), blk, name);
  if (extra != NULL)
    sprintf(_tclBuff, "%s %s}\n", buff, extra);
  else
    sprintf(_tclBuff, "%s}}\n", buff);
}
void Ath__zui::print_self(char* blk, char* name, char* extra)
{
  _tclBufFlag = true;
  char buff[10000];
  sprintf(buff, "{%s Block {%s} name {%s}", getFirstBox(), blk, name);
  if (extra != NULL)
    sprintf(_tclBuff, "%s %s}\n", buff, extra);
  else
    sprintf(_tclBuff, "%s}\n", buff);
}
void Ath__zui::print_wire_msg(int   x1,
                              int   y1,
                              int   x2,
                              int   y2,
                              uint  layer,
                              char* msg)
{
  _tclBufFlag = true;
  /*
  sprintf(_tclBuff, "{ id %s bb { %d %d %d %d } layer { %d } msg {%d %d  %d %d
  %s} }\n", objName, x1, y1, x2, y2, layer, x1, y1, x2, y2, msg);
  */
  sprintf(_tclBuff,
          "{msg {%s wire {%d %d  %d %d} layer %d}}\n",
          msg,
          x1,
          y1,
          x2,
          y2,
          layer);
}
void Ath__zui::print_msg(int x1, int y1, int x2, int y2, uint layer, char* msg)
{
  _tclBufFlag = true;
  sprintf(_tclBuff, "{msg {%s bbox {%d %d  %d %d}}}\n", msg, x1, y1, x2, y2);
}
void Ath__zui::print_self(const char* blk,
                          const char* name,
                          char*       type,
                          int         x1,
                          int         y1,
                          int         x2,
                          int         y2,
                          uint        layer)
{
  _tclBufFlag = true;
  sprintf(_tclBuff,
          "{ bb { %d %d %d %d } layer { %d } Block %s type %s name %s } \n",
          x1,
          y1,
          x2,
          y2,
          layer,
          blk,
          type,
          name);
}
char* Ath__zui::getTclBbox_light(Ath__zbox* b, int index)
{
  char buff[128];

  sprintf(buff,
          "id b%d.%s%d.%d",
          b->_blockId,
          _hierTypeTable->getWord(b->_hier),
          index,
          b->_type);

  sprintf(_tclBuff,
          "%s bb {%d %d %d %d} layer %d hierarchy chip type bbox",
          buff,
          b->_xlo,
          b->_ylo,
          b->_xhi,
          b->_yhi,
          b->_layer);

  return _tclBuff;
}
char* Ath__zui::getTclBbox(Ath__zbox* b, int index)
{
  // return getTclBbox_light(b, index);

  bool diagFlag = false;

  char buff[128];
  if ((index < 0) && (b->_ownerName != NULL)) {
    sprintf(buff, "{%s}", b->_ownerName);
  } else {
    sprintf(buff,
            "id b%d.%s%d.%d",
            b->_blockId,
            _hierTypeTable->getWord(b->_hier),
            index,
            b->_type);

    if (b->_zuiMark > 0) {
      sprintf(buff, "%s mark %s", buff, _markTable->getName(b->_zuiMark));
    }
  }
  // if ((!diagFlag) || (b->_layer!=5)) {
  if (!diagFlag) {
    sprintf(_tclBuff,
            "%s type %s hierarchy %s bb {%d %d %d %d} layer %d",
            buff,
            _boxTypeTable->getName(b->_type),
            _hierTypeTable->getName(b->_hier),
            b->_xlo,
            b->_ylo,
            b->_xhi,
            b->_yhi,
            b->_layer);
  } else if (diagFlag) {
    int w   = b->_yhi - b->_ylo;
    int len = b->_xhi - b->_xlo;
    int x2, y2;
    if (w < len) {
      x2 = b->_xhi;
      y2 = b->_ylo;
    } else {
      x2 = b->_xlo;
      y2 = b->_yhi;
    }
    sprintf(_tclBuff,
            "%s type %s hierarchy %s dd {%d %d %d %d %d %d} layer %d",
            buff,
            _boxTypeTable->getName(b->_type),
            _hierTypeTable->getName(b->_hier),
            b->_xlo,
            b->_ylo,
            x2,
            y2,
            b->_xlo,
            b->_ylo,
            b->_layer);
  }
  /*
  else if ((diagFlag) && (b->_layer==5)) {
          int w= (b->_yhi-b->_ylo);
          int len= b->_xhi-b->_xlo;
          sprintf(_tclBuff, "{ type %s id %s dd { %d %d %d %d %d %d %d %d %d %d}
  layer %d }\n", _boxTypeTable->getName(b->_type), buff, b->_xlo, b->_ylo,
                  b->_xlo+w, b->_ylo,
                  b->_xhi, b->_ylo+len,
                  b->_xhi-w, b->_ylo+len,
                  b->_xlo, b->_ylo,
                  b->_layer);
  }
  */
  return _tclBuff;
}
void Ath__zui::computeMaxBB(uint visualGap)
{
  _searchBB.resetMaxBox();
  uint cnt = 0;
  for (uint ii = 0; ii < 15; ii++) {
    for (uint jj = 0; jj < _ztable[ii]->getCnt(); jj++) {
      Ath__zbox* z = _ztable[ii]->get(jj);
      _searchBB.setMaxBox(z->_xlo, z->_ylo, z->_xhi, z->_yhi);
    }
  }
  _searchBB._valid = 1;
  _searchBB.addExtra(visualGap);
}
void Ath__zui::bufferExistingZboxes()
{
  getZboxes();
  resetZtable();
}
void Ath__zui::addPreviousZboxes()
{
  for (uint jj = 0; jj < _tmpZboxTable->getCnt(); jj++) {
    Ath__zbox* b = _tmpZboxTable->get(jj);
    _ztable[b->_layer]->add(b);
  }
}

uint Ath__zui::getZboxes()
{
  _tmpZboxTable->resetCnt(0);
  for (uint jj = 0; jj < _ztable[15]->getCnt(); jj++)  // tiles
    _tmpZboxTable->add(_ztable[15]->get(jj));

  uint cnt = 0;
  for (uint ii = 0; ii < 15; ii++)
    for (uint jj = 0; jj < _ztable[ii]->getCnt(); jj++)
      _tmpZboxTable->add(_ztable[ii]->get(jj));

  return _tmpZboxTable->getCnt();
}
uint Ath__zui::getZtableCnt()
{
  uint cnt = 0;
  for (uint ii = 0; ii < _layerCnt; ii++)
    cnt += _ztable[ii]->getCnt();
  return cnt;
}
char* Ath__zui::getTclBbox(int ii, bool vFlag)
{
  Ath__zbox* b = getBox(ii);

  // return (vFlag) ? getTclBbox(b, -ii) : getTclBbox(b, ii) ;
  return (vFlag) ? getTclBbox(b, -ii) : getTclBbox(b, b->_nameId);
}
char* Ath__zui::getTclLayer(Ath__zbox* b)
{
  sprintf(_tclLayer, "");

  if (b->_layer >= 0)
    sprintf(_tclLayer, "layer %d", b->_layer);

  return _tclLayer;
}
char* Ath__zui::getTclOwner(Ath__zbox* b)
{
  sprintf(_tclOwner, "");

  if (b->_ownerName != NULL) {
    sprintf(_tclOwner, "");
    // sprintf(_tclOwner, "%s %s", _otype[b->_owner], b->_ownerName);
  }
  return _tclOwner;
}

Ath__zui::Ath__zui(char* name, int m, int n)
{
  _tmpZboxTable = new Ath__array1D<Ath__zbox*>(64000);

  _idtable = new Ath__array1D<uint>(64000);

  _parser = new Ath__parser();

  _layerCnt  = 20;
  _ztable    = new Ath__array1D<Ath__zbox*>*[_layerCnt];
  _ztable[0] = new Ath__array1D<Ath__zbox*>(64000);
  for (uint ii = 1; ii < _layerCnt; ii++)
    _ztable[ii] = new Ath__array1D<Ath__zbox*>(16000);

  //_ZUI_GRID_SIZE
  // for (uint ii= 0; ii<12800

  _pool = new AthPool<Ath__zbox>(false, 1000000, 1024);

  _tclBufFlag = false;
  _tclBuff    = new char[10024];
  _tclLayer   = new char[100];
  _tclOwner   = new char[100];

  initGuiMenus();

  if (name != NULL) {
    if ((m >= 0) && (n >= 0))
      sprintf(_filename, "%s_%d_%d_.zui", name, m, n);
    else if (m >= 0)
      sprintf(_filename, "%s_%d_.zui", name, m);
    else
      sprintf(_filename, "%s.zui", name);

    _fp = ATH__openFile(_filename, "w");
  } else {
    _fp = NULL;
  }
  _blockId = 0;
  _detail  = 1;

  _inspName   = new char[1024];
  _inspAction = new char[1024];
  ;

  //	initPixelArrays(8);
}
Ath__zui::~Ath__zui()
{
  delete _tmpZboxTable;

  delete _parser;

  for (uint ii = 0; ii < _layerCnt; ii++)
    delete _ztable[ii];

  delete[] _ztable;

  delete _idtable;
  delete _tclBuff;
  delete _tclLayer;
  delete _tclOwner;

  delete _pathMenuTable;
  delete[] _inspName;
  delete[] _inspAction;

  fflush(_fp);
  ATH__closeFile(_fp);
}
void Ath__zui::resetPixelTables(uint modX, uint modY, Ath__box* bb)
{
  for (uint ii = 0; ii < _layerCnt; ii++) {
    _metPixelTable[ii]->setup(bb, modX, modY);
  }
}

void Ath__zui::resetZtable()
{
  for (uint ii = 0; ii < _layerCnt; ii++) {
    _ztable[ii]->resetCnt(0);
  }
  _idtable->resetCnt(0);
}
void Ath__zui::resetTable(int detail, uint modX, uint modY, Ath__box* bb)
{
  setBlockId(0);
  _tclBufFlag  = false;
  _currentMark = 0;
  resetZtable();

  _detail = detail;

  if (detail == 0) {
    if (bb == NULL)
      bb = &_searchBB;

    resetPixelTables(modX, modY, bb);
  }
}
bool Ath__zui::isSearchBoxValid()
{
  if (_searchBB._valid > 0)
    return true;
  else
    return false;
}
void Ath__zui::getBbox(Ath__box* bb)
{
  bb->set(&_searchBB);
}
Ath__box* Ath__zui::getBbox()
{
  return &_searchBB;
}
void Ath__zui::makeZuiObject(Ath__box* e)
{
  addBox(e, e->_type);
}
uint Ath__zui::makeZuiObjects_1(Ath__box*     head,
                                Ath__hierType hier,
                                Ath__boxType  box)
{
  uint cnt = 0;
  for (Ath__box* e = head; e != NULL; e = e->_next) {
    cnt++;
    uint id = e->getOwner();

    cnt += addBox(id, box, hier, e->_layer, e->_xlo, e->_ylo, e->_xhi, e->_yhi);
  }
  return cnt;
}
uint Ath__zui::makeZuiObjects(Ath__box*     head,
                              Ath__hierType hier,
                              Ath__boxType  box)
{
  uint cnt = 0;
  for (Ath__box* e = head; e != NULL; e = e->_next) {
    cnt++;
    uint id = e->getOwner();
    if (id <= 0)
      id = cnt;
    addBox(e, box);
  }
  return cnt;
}
bool Ath__zui::intersect(int x1, int y1, int x2, int y2)
{
  return !_searchBB.outside(x1, y1, x2, y2);
}
Ath__array1D<uint>* Ath__zui::getIdTable()
{
  return _idtable;
}
void Ath__zui::addId(uint id)
{
  _idtable->add(id);
}
uint Ath__zui::getNextId()
{
  uint n;
  if (_idtable->getNext(n))
    return n;
  else
    return 0;
}
void Ath__zui::resetIdTableIterator()
{
  _idtable->resetIterator();
}

uint Ath__zui::addBox(uint          id,
                      uint          boxType,
                      Ath__hierType hier,
                      int           layer,
                      int           x1,
                      int           y1,
                      int           x2,
                      int           y2,
                      uint          ownId)
{
  if (!_searchBB.clip(&x1, &y1, &x2, &y2))
    return 0;

  if (_excludeLayerTable[layer])
    return 0;

  if (_detail > 0) {
    Ath__zbox* b = _pool->alloc();

    b->setCoords(x1, y1, x2, y2);

    b->setZuiIdentity(_blockId, id, boxType, ownId, hier, layer);
    b->setZuiMark(_currentMark);

    _ztable[layer]->add(b);
  } else {
    _metPixelTable[layer]->update(x1, y1, x2, y2);
  }
  return 1;
}
uint Ath__zui::addGrouteBox(uint boxType,
                            int  x1,
                            int  y1,
                            int  x2,
                            int  y2,
                            int  level,
                            uint id)
// boxType can be only one of: Ath_box__gcell, Ath_box__gpath, Ath_box__spots
{
  if (_excludeLayerTable[level])
    return 0;

  if (!_searchBB.clip(&x1, &y1, &x2, &y2))
    return 0;

  if (_detail > 0) {
    Ath__zbox* b = _pool->alloc();
    b->setCoords(x1, y1, x2, y2);

    b->setZuiIdentity(_blockId, id, boxType, 0, Ath_hier__groute, level);

    b->setZuiMark(_currentMark);

    _ztable[level]->add(b);
  } else {
    _metPixelTable[level]->update(x1, y1, x2, y2);
  }
  return 1;
}
uint Ath__zui::addBox(Ath__searchBox* bb, uint hierType, uint boxType, uint id)
{
  uint level = bb->getLevel();
  if (_excludeLayerTable[level])
    return 0;

  int x1 = bb->loXY(0);
  int y1 = bb->loXY(1);
  int x2 = bb->hiXY(0);
  int y2 = bb->hiXY(1);

  if (!_searchBB.clip(&x1, &y1, &x2, &y2))
    return 0;

  if (_detail > 0) {
    Ath__zbox* b = _pool->alloc();
    b->setCoords(x1, y1, x2, y2);

    if (id == 0)
      id = bb->getOtherId();

    b->setZuiIdentity(_blockId, id, boxType, bb->getOwnerId(), hierType, level);

    b->setZuiMark(_currentMark);

    _ztable[level]->add(b);
  } else {
    _metPixelTable[level]->update(x1, y1, x2, y2);
  }
  return 1;
}
uint Ath__zui::addBox(dbBox* bb, uint boxType, uint id, int ownerType)
{
  uint layer = 0;
  if (bb->getTechLayer() != NULL)
    layer = bb->getTechLayer()->getRoutingLevel();

  if (_excludeLayerTable[layer])
    return 0;

  int x1 = bb->xMin();
  int y1 = bb->yMin();
  int x2 = bb->xMax();
  int y2 = bb->yMax();

  if (!_searchBB.clip(&x1, &y1, &x2, &y2))
    return 0;

  if (_detail > 0) {
    Ath__zbox* b = _pool->alloc();
    b->setCoords(x1, y1, x2, y2);

    if (id == 0)
      id = bb->getId();

    if (ownerType < 0)
      ownerType = mapDb2HierType(bb);

    b->setZuiIdentity(
        _blockId, id, boxType, bb->getBoxOwner()->getId(), ownerType, layer);

    b->setZuiMark(_currentMark);

    _ztable[layer]->add(b);
  } else {
    _metPixelTable[layer]->update(x1, y1, x2, y2);
  }
  return 1;
}
uint Ath__zui::addWireShape(int           x1,
                            int           y1,
                            int           x2,
                            int           y2,
                            uint          level,
                            Ath__hierType hierType,
                            Ath__boxType  boxType,
                            uint          id,
                            uint          ownId)
{
  if (_excludeLayerTable[level])
    return 0;

  if (!_searchBB.clip(&x1, &y1, &x2, &y2))
    return 0;

  if (_detail > 0) {
    Ath__zbox* b = _pool->alloc();
    b->setCoords(x1, y1, x2, y2);

    b->setZuiIdentity(_blockId, id, boxType, ownId, hierType, level);

    b->setZuiMark(_currentMark);

    _ztable[level]->add(b);

    return 1;
  }
  return 0;
}
uint Ath__zui::addBox(Ath__box* bb, uint boxType, uint id)
{
  int x1 = bb->_xlo;
  int y1 = bb->_ylo;
  int x2 = bb->_xhi;
  int y2 = bb->_yhi;

  if (!_searchBB.clip(&x1, &y1, &x2, &y2))
    return 0;

  if (_excludeLayerTable[bb->_layer])
    return 0;

  if (_detail > 0) {
    Ath__zbox* b = _pool->alloc();
    b->setCoords(x1, y1, x2, y2);

    if (id == 0)
      id = bb->_id;

    b->setZuiIdentity(
        _blockId, id, boxType, bb->getOwner(), bb->getOwnerType(), bb->_layer);

    b->setZuiMark(_currentMark);

    _ztable[bb->_layer]->add(b);
  } else {
    _metPixelTable[bb->_layer]->update(x1, y1, x2, y2);
  }
  return 1;
}
void Ath__zui::setTrackPitch(char* layerName,
                             uint  dir,
                             int   cnt,
                             int   base,
                             int   max)
{
  uint tot = (max - base) / 2;
  _pitch   = tot / cnt;
  _width   = _pitch / 3;
  _dir     = dir;
  _base    = base;
  sprintf(_layerNodeString, "node %s L", layerName);
}

void Ath__zui::printQuadBox(uint row, uint col, int x1, int y1, int x2, int y2)
{
  fprintf(_fp, "node quad q_%d_%d %d %d %d %d\n", row, col, x1, y1, x2, y2);
}

void Ath__zui::printSeg(char* layer, uint cnt, int x1, int y1, int x2, int y2)
{
  fprintf(_fp, "node %s L%d %d %d %d %d\n", layer, cnt, x1, y1, x2, y2);
}
void Ath__zui::printSegOnTrack(int xy1, int xy2, int index)
{
  uint bot = _base + index * _pitch;
  uint top = bot + _width;
  if (_dir > 0) {
    fprintf(
        _fp, "%s%d %d %d %d %d\n", _layerNodeString, index, bot, xy1, top, xy2);
  } else {
    fprintf(
        _fp, "%s%d %d %d %d %d\n", _layerNodeString, index, xy1, bot, xy2, top);
  }
}
