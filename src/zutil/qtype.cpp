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
#include <stdlib.h>

#ifdef _WIN32
#include <direct.h>
#endif

#include "dbShape.h"
#include "misc_global.h"
#include "net.h"
#include "qtype.h"
#include "wire.h"

Ath__qCoord::Ath__qCoord(int n, int min, int max)
{
  _num = n;
  _mod = (max - min) / n;
  _min = min;
  _max = max;

  for (uint ii = 0; ii < _num; ii++)
    set(ii, _min + ii * _mod);

  set(_num, _max);
}
int Ath__qCoord::get(uint index)
{
  assert(index <= _num);
  return _x[index];
}
int Ath__qCoord::getCenter(uint index, int offset)
{
  return get(index) + _mod / 2 + offset;
}
int Ath__qCoord::getNext(uint index)
{
  if (index < _num)
    return _x[index + 1];
  else
    return get(index) + _mod;
}
void Ath__qCoord::set(uint index, int x0)
{
  assert(index <= _num);

  _x[index] = x0;
}
int Ath__qCoord::getIndex(int x)
{
  int dist = (x - _min);
  if (dist < 0)
    return 0;

  uint nn = dist / _mod;
  if (x >= _x[nn]) {
    for (uint ii = nn + 1; ii < _num + 1; ii++) {
      if (x < _x[ii])
        return ii - 1;
    }
  } else {
    for (uint ii = nn - 1; ii >= 0; ii--) {
      if (x >= _x[ii])
        return ii;
    }
  }
  return -1;
}
int Ath__qCoord::getMinMaxIndex(int x)
{
  int dist = (x - _min);
  if (dist < 0)
    return 0;

  uint nn = dist / _mod;
  if (nn >= _num)
    return _num - 1;

  return getIndex(x);
}
uint Ath__qCoord::getIndex4(int x0)
{
  if (x0 < _x[2]) {  // 0,1

    if (x0 >= _x[1]) {  // 0,1
      return 1;
    } else if (x0 >= _x[0]) {
      return 0;
    } else {
      fprintf(stderr, "ERROR, %d smaller than lower bound %d\n", x0, _x[0]);
      return 0;
    }
  } else {  // 2, 3

    if (x0 < _x[3]) {  // 0,1
      return 2;
    } else if (x0 <= _x[4]) {
      return 3;
    } else {
      fprintf(stderr, "ERROR, %d larger than upper bound %d\n", x0, _x[4]);
      return 3;
    }
  }
}

Ath__qCounter::Ath__qCounter(uint ii, uint jj)
{
  _localPin = 0;
  _topPin   = 0;
  _cross    = 0;
  _hot      = 0;

  init(_next, 2, 0);
  init(_thru, 2, 0);
  init(_turn, 4, 0);
  init(_tjog, 4, 0);

  _tracks[0] = 0;
  _tracks[1] = 0;

  setName(ii, jj);

  HV[0] = "HORIZONTAL";
  HV[1] = "VERTICAL";
}
Ath__qCounter::~Ath__qCounter()
{
  if (_name != NULL)
    delete _name;
}
void Ath__qCounter::setName(uint hCnt, uint vCnt)
{
  _name = new char[10];
  sprintf(_name, "%d_%d", hCnt, vCnt);
}
char* Ath__qCounter::getName()
{
  return _name;
};

void Ath__qCounter::updateNext(uint n)
{
  _next[n]--;
}
void Ath__qCounter::updateTurn(uint n)
{
  _turn[n]--;
}
void Ath__qCounter::updateThru(uint n)
{
  _thru[n]--;
}
bool Ath__qCounter::checkTurn(uint n)
{
  if (_turn[n] > 0)
    return true;
  else {
    printSummary(_row, _col);
    return false;
  }
}
uint Ath__qCounter::checkThru(uint n)
{
  if (_thru[n] > 0)
    return 0;
  else {
    return printSummary(_row, _col);
  }
}
void Ath__qCounter::resetTracks(uint dir, uint hCnt, uint turn)
{
  if (_thru[dir] == 0) {
    if (_turn[dir] + _turn[dir + 2] > 0) {
      _thru[dir]     = hCnt / 2 + turn / 2;
      _turn[dir]     = turn - turn / 2;
      _turn[dir + 2] = turn - turn / 2;
    } else {
      _thru[dir]     = hCnt / 2;
      _turn[dir]     = turn;
      _turn[dir + 2] = turn;
    }
  } else if ((_turn[dir] == 0) || (_turn[dir + 2] == 0)) {
    if (_thru[dir] > 0) {
      _thru[dir]     = hCnt / 2 - turn / 2;
      _turn[dir]     = turn + turn / 2;
      _turn[dir + 2] = turn + turn / 2;
    } else {
      _thru[dir]     = hCnt / 2;
      _turn[dir]     = turn;
      _turn[dir + 2] = turn;
    }
  } else {
    _thru[dir]     = hCnt / 2;
    _turn[dir]     = turn;
    _turn[dir + 2] = turn;
  }
}

void Ath__qCounter::resetTracks(uint r, uint c, uint hCnt, uint vCnt)
{
  uint turn = (vCnt + hCnt) / 8;
  _row      = r;
  _col      = c;
  if (_hot > 0) {
    resetTracks(0, hCnt, turn);
    resetTracks(1, vCnt, turn);
    _tracks[0] = hCnt;
    _tracks[1] = vCnt;
  } else {
    _thru[0] = hCnt / 2;
    _thru[1] = vCnt / 2;
    init(_turn, 4, turn);
  }
  _tracks[0] = hCnt;
  _tracks[1] = vCnt;
  _hot       = 0;
}
void Ath__qCounter::setMaxTracks(uint dir, uint trackCnt)
{
  uint turn = trackCnt / 2;

  _tracks[dir]   = trackCnt;
  _thru[dir]     = trackCnt / 2;
  _turn[dir]     = turn / 2;
  _turn[dir + 2] = turn / 2;
}
void Ath__qCounter::setTracks(uint r, uint c, uint hCnt, uint vCnt, int dir)
{
  _row = r;
  _col = c;
  if (dir < 0) {
    _tracks[0] = hCnt;
    _tracks[1] = vCnt;
    _thru[0]   = hCnt / 2;
    _thru[1]   = vCnt / 2;
    uint turn  = (vCnt + hCnt) / 8;
    init(_turn, 4, turn);
  } else {
    if (dir > 0)
      setMaxTracks(dir, vCnt);
    else
      setMaxTracks(dir, hCnt);
  }
}
void Ath__qCounter::init(int* A, uint n, int v)
{
  for (uint ii = 0; ii < n; ii++)
    A[ii] = v;
}
int Ath__qCounter::sumTurns()
{
  return _turn[0] + _turn[1] + _turn[2] + _turn[3];
}
bool Ath__qCounter::checkAllRes(uint dir, int threshold)
{
  if ((_thru[dir] - threshold < 0) || (sumTurns() / 2 - threshold < 0))
    return false;

  return true;
}
uint Ath__qCounter::thruSummary(uint ii, uint jj, uint dir, uint threshold)
{
  if (checkAllRes(dir, threshold))
    return 0;

  fprintf(
      stdout, "quad[%d][%d]=%d  %s tracks\n", ii, jj, _tracks[dir], HV[dir]);
  fprintf(stdout, "\tThru= %4d\n", _thru[dir]);
  fprintf(stdout, "\tTurn[0]= %4d Turn[2]= %4d\n", _turn[0], _turn[2]);
  fprintf(stdout, "\tTurn[1]= %4d Turn[3]= %4d\n", _turn[1], _turn[3]);

  return 1;
}
uint Ath__qCounter::printSummary(uint ii, uint jj)
{
  if (_hot > 0)
    return 1;

  fprintf(stdout,
          "quad[%d][%d]=%d  %s tracks, %d thru\n",
          ii,
          jj,
          _tracks[0],
          HV[0],
          _thru[0]);
  fprintf(stdout,
          "quad[%d][%d]=%d  %s tracks, %d thru\n",
          ii,
          jj,
          _tracks[1],
          HV[1],
          _thru[1]);

  fprintf(stdout, "\tTurn[0]= %4d Turn[2]= %4d\n", _turn[0], _turn[2]);
  fprintf(stdout, "\tTurn[1]= %4d Turn[3]= %4d\n", _turn[1], _turn[3]);

  _hot = 1;
  return 1;
}
uint Ath__qCounter::printHotSummary()
{
  if (_hot == 0)
    return 0;

  uint ii = _row;
  uint jj = _col;

  fprintf(stdout,
          "quad[%d][%d]=%d  %s tracks, %d thru\n",
          ii,
          jj,
          _tracks[0],
          HV[0],
          _thru[0]);
  fprintf(stdout,
          "quad[%d][%d]=%d  %s tracks, %d thru\n",
          ii,
          jj,
          _tracks[1],
          HV[1],
          _thru[1]);

  fprintf(stdout, "\tTurn[0]= %4d Turn[2]= %4d\n", _turn[0], _turn[2]);
  fprintf(stdout, "\tTurn[1]= %4d Turn[3]= %4d\n", _turn[1], _turn[3]);

  return 1;
}

Ath__array1D<uint>*** Ath__quadTable::alloc2Darray1d(uint m,
                                                     uint n,
                                                     uint allocChunk)
{
  Ath__array1D<uint>*** A = new Ath__array1D<uint>**[m];
  for (uint ii = 0; ii < m; ii++) {
    A[ii] = new Ath__array1D<uint>*[n];
    for (uint jj = 0; jj < n; jj++)
      A[ii][jj] = new Ath__array1D<uint>(allocChunk);
  }
  return A;
}
uint** Ath__quadTable::alloc2Darray(uint m, uint n)
{
  uint** A = new uint*[m];
  for (uint ii = 0; ii < m; ii++) {
    A[ii] = new uint[n];
    for (uint jj = 0; jj < n; jj++)
      A[ii][jj] = 0;
  }
  return A;
}
void Ath__quadTable::de_alloc2Darray(uint** A, uint m)
{
  for (uint ii = 0; ii < m; ii++) {
    delete[] A[ii];
  }
  delete[] A;
}
void Ath__quadTable::de_alloc2Darray1d(Ath__array1D<uint>*** A, uint m, uint n)
{
  for (uint ii = 0; ii < m; ii++) {
    for (uint jj = 0; jj < n; jj++)
      delete A[ii][jj];
    delete[] A[ii];
  }
  delete[] A;
}
void Ath__quadTable::initPinTable(uint n)
{
  _thruPinPool = new AthPool<Ath__p2pConn>(false, 128);

  _thruPinTable = new Ath__connTable**[_rowCnt];
  for (uint ii = 0; ii < _rowCnt; ii++) {
    _thruPinTable[ii] = new Ath__connTable*[_colCnt];
    for (uint jj = 0; jj < _colCnt; jj++)
      _thruPinTable[ii][jj]
          = new Ath__connTable(_thruPinPool, n, _minTileSize / 4, _minTileSize);
  }
}
void Ath__quadTable::freePinTable(bool skipPool)
{
  if (!skipPool && (_thruPinPool != NULL))
    delete _thruPinPool;

  if (_thruPinTable == NULL)
    return;

  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++)
      delete _thruPinTable[ii][jj];
    delete[] _thruPinTable[ii];
  }
  delete[] _thruPinTable;
  _thruPinTable = NULL;
}
void Ath__quadTable::qInit(AthPool<Ath__box>*  pinBoxPool,
                           AthPool<Ath__qPin>* qPinPool)
{
  _quad      = new Ath__qCounter**[_rowCnt];
  _tmpBox    = new Ath__box**[_rowCnt];
  _qPinTable = new Ath__qPinTable**[_rowCnt];
  _obsTable  = new AthArray<Ath__box*>**[_rowCnt];

  uint ii;
  for (ii = 0; ii < _rowCnt; ii++) {
    _quad[ii]      = new Ath__qCounter*[_colCnt];
    _tmpBox[ii]    = new Ath__box*[_colCnt];
    _qPinTable[ii] = new Ath__qPinTable*[_colCnt];
    _obsTable[ii]  = new AthArray<Ath__box*>*[_colCnt];

    for (uint k = 0; k < _colCnt; k++) {
      _quad[ii][k]   = new Ath__qCounter(ii, k);
      _tmpBox[ii][k] = new Ath__box();
      _tmpBox[ii][k]->resetMaxBox();

      _qPinTable[ii][k] = new Ath__qPinTable(qPinPool, pinBoxPool, 512);
      _obsTable[ii][k]  = new AthArray<Ath__box*>(64);
    }
  }
}
void Ath__quadTable::qFree()
{
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint k = 0; k < _colCnt; k++) {
      delete _quad[ii][k];
      delete _tmpBox[ii][k];
      delete _qPinTable[ii][k];
      delete _obsTable[ii][k];
    }
    delete[] _quad[ii];
    delete[] _qPinTable[ii];
    delete[] _tmpBox[ii];
    delete[] _obsTable[ii];
  }
  delete[] _quad;
  delete[] _qPinTable;
  delete[] _tmpBox;
  delete[] _obsTable;
}

void Ath__quadTable::getDivideLines(int*  ll,
                                    int*  ur,
                                    uint* loDivide,
                                    uint* hiDivide,
                                    int*  xy1,
                                    int*  xy2)
{
  uint size[2];

  for (uint ii = 0; ii < 2; ii++) {
    size[ii] = ur[ii] - ll[ii];

    xy1[ii] = ll[ii];
    if (loDivide[ii] > 0)
      xy1[ii] += size[ii] / loDivide[ii];

    xy2[ii] = ur[ii];
    if (hiDivide[ii] > 0)
      xy2[ii] -= size[ii] / hiDivide[ii];
  }
}
Ath__qPin* Ath__quadTable::getPin(uint id)
{
  return _qBoxPool->get(id);
}
Ath__qPin* Ath__quadTable::getPinFromBox(uint id)
{
  Ath__box* pinBox = _pinBoxPool->get(id);

  return _qBoxPool->get(pinBox->getOwner());
}
Ath__wire* Ath__quadTable::getWire(uint id)
{
  return _wirePool->get(id);
}
uint Ath__quadTable::getDir(dbTechLayer* layer)
{
  return layer->getDirection() > 1 ? 0 : 1;  // 0=vertical 1=horizontal
}
void Ath__quadTable::initBusTable(dbBox*  blkBB,
                                  dbTech* tech,
                                  uint    trackInit,
                                  uint    wireInit)
{
  initWireGridPools(trackInit, wireInit);

  _tileCnt           = _rowCnt * _colCnt;
  _tileTable         = new Ath__gridTile*[_tileCnt];
  uint kk            = 0;
  uint minTileHeight = ath__maxInt;

  _busTable = new Ath__gridTile**[_rowCnt];
  for (uint ii = 0; ii < _rowCnt; ii++) {
    _busTable[ii] = new Ath__gridTile*[_colCnt];

    for (uint jj = 0; jj < _colCnt; jj++) {
      int y1 = _rowDivider->get(ii);
      int y2 = _rowDivider->getNext(ii);

      uint dy = y2 - y1;
      if (minTileHeight > dy)
        minTileHeight = dy;

      int x1 = _colDivider->get(jj);
      int x2 = _colDivider->getNext(jj);

      uint levelCnt = tech->getRoutingLayerCount() + 1;
      _busTable[ii][jj]
          = new Ath__gridTile(levelCnt, x1, y1, x2, y2, _trackPool, _wirePool);

      _tileTable[kk++] = _busTable[ii][jj];

      dbSet<dbTechLayer>           layers = tech->getLayers();
      dbSet<dbTechLayer>::iterator itr;

      for (itr = layers.begin(); itr != layers.end(); ++itr) {
        dbTechLayer*    layer = *itr;
        dbTechLayerType type  = layer->getType();

        if (type.getValue() != dbTechLayerType::ROUTING)
          continue;

        uint markerCnt = 2;
        uint width     = 1;
        uint level     = layer->getRoutingLevel();
        uint dir       = getDir(layer);

        Ath__grid* g = new Ath__grid(
            _trackPool, _wirePool, level, layer->getNumber(), markerCnt);

        g->setTracks(dir, width, layer->getPitch(), x1, y1, x2, y2);
        g->setSchema(1);

        _busTable[ii][jj]->addGrid(g);
      }
    }
  }
  _tileSearchGrid = new Ath__grid(_trackPool, _wirePool, 0, 1, 2 * _colCnt);

  _tileSearchGrid->setTracks(1,
                             1,
                             minTileHeight,
                             blkBB->xMin(),
                             blkBB->yMin(),
                             blkBB->xMax(),
                             blkBB->yMax());
  _tileSearchGrid->setSchema(1);

  for (uint tt = 0; tt < _tileCnt; tt++) {
    Ath__gridTile* g = _tileTable[tt];
    int            x1, y1, x2, y2;
    g->getBounds(&x1, &y1, &x2, &y2);
    _tileSearchGrid->placeBox(tt, x1, y1, x2 - 1, y2 - 1);
  }
  _tileSearchGrid->setExtrusionMarker();
}
void Ath__quadTable::addTileBlockages(dbBlock* block, dbTech* tech)
{
  Ath__array1D<uint> idtable(16);

  dbSet<dbNet>           nets = block->getNets();
  dbSet<dbNet>::iterator net_itr;
  for (net_itr = nets.begin(); net_itr != nets.end(); ++net_itr) {
    dbNet* net = *net_itr;

    dbSigType type = net->getSigType();
    if (!((type == POWER) || (type == GROUND)))
      continue;

    dbWire* wire = net->getWire();

    if (wire != NULL) {
      uint wireId = wire->getId();

      dbWireShapeItr shapes;
      odb::dbShape   s;
      for (shapes.begin(wire); shapes.next(s);) {
        if (s.isVia())
          continue;

        int shapeId = shapes.getShapeId();

        uint level = s.getTechLayer()->getRoutingLevel();

        Ath__searchBox bb;
        bb.set(s.xMin(), s.yMin(), s.xMax(), s.yMax(), level, -1);

        if (bb.getLength() < 400000)  // TODO_NOW
          continue;

        if (bb.getDir() + 1 == s.getTechLayer()->getDirection())
          continue;

        idtable.resetCnt();
        _tileSearchGrid->search(&bb, &idtable);  // single grid

        for (uint ii = 0; ii < idtable.getCnt(); ii++) {
          uint gridId = idtable.get(ii);

          Ath__grid* g = _tileTable[gridId]->getGrid(level);
          g->blockTracks(&bb);
        }
      }
    }
  }
  for (uint ii = 0; ii < _tileCnt; ii++) {
    for (uint level = 1; level < tech->getRoutingLayerCount() + 1; level++) {
      _tileTable[ii]->getGrid(level)->setFreeTracks();
    }
  }
}
Ath__grid**** Ath__quadTable::initGridTable(dbTechLayer** layerTable,
                                            uint          dirCnt,
                                            uint          schema)
{
  Ath__grid**** gTable = new Ath__grid***[dirCnt];

  for (uint dir = 0; dir < dirCnt; dir++) {
    gTable[dir] = new Ath__grid**[_rowCnt];

    dbTechLayer* layer   = layerTable[dir];
    uint         width   = layer->getWidth();
    uint         spacing = layer->getSpacing();
    uint         pitch   = layer->getPitch();
    uint         level   = layer->getRoutingLevel();

    for (uint ii = 0; ii < _rowCnt; ii++) {
      gTable[dir][ii] = new Ath__grid*[_colCnt];
      int y1          = _rowDivider->get(ii);
      int y2          = _rowDivider->getNext(ii);

      for (uint jj = 0; jj < _colCnt; jj++) {
        int x1 = _colDivider->get(jj);
        int x2 = _colDivider->getNext(jj);

        Ath__grid* g = new Ath__grid(_trackPool, _wirePool, level, 1, 8);

        g->setTracks(dir, 1, pitch, x1, y1, x2, y2);
        g->setSchema(schema);

        gTable[dir][ii][jj] = g;
      }
    }
  }
  return gTable;
}
void Ath__quadTable::destroyGridTable(Ath__grid**** gTable, uint dirCnt)
{
  for (uint dir = 0; dir < dirCnt; dir++) {
    for (uint ii = 0; ii < _rowCnt; ii++) {
      for (uint jj = 0; jj < _colCnt; jj++) {
        delete gTable[dir][ii][jj];
      }
      delete[] gTable[dir][ii];
    }
    delete[] gTable[dir];
  }
  delete[] gTable;
}

void Ath__quadTable::initWireGridPools(uint trackInit, uint wireInit)
{
  _trackPool = new AthPool<Ath__track>(false, trackInit);
  _trackPool->alloc();
  _wirePool = new AthPool<Ath__wire>(false, wireInit);
  _wirePool->alloc();

  _tmpIntArray     = new int[1000000];
  _tmpBoxArray     = new Ath__box*[1000000];
  _tmpBoxArrayNext = new Ath__box*[1000000];
}
void Ath__quadTable::destroyWireGridPools()
{
  delete _trackPool;
  delete _wirePool;

  delete[] _tmpIntArray;
  delete[] _tmpBoxArray;
  delete[] _tmpBoxArrayNext;
}
void Ath__quadTable::free_shapeTable()
{
  if (_qShapeTable == NULL)
    return;

  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint k = 0; k < _colCnt; k++) {
      if (_qShapeTable[ii][k] != NULL)
        delete _qShapeTable[ii][k];
    }
    delete[] _qShapeTable[ii];
  }
  delete[] _qShapeTable;

  delete _trackPool;
  delete _wirePool;

  delete[] _tmpIntArray;
  delete[] _tmpBoxArray;
  delete[] _tmpBoxArrayNext;

  delete[] _topDir;
  delete[] _tmpFullDir;
}
void Ath__qCoord::snapOnPower(Ath__grid* g)
{
  if (g == NULL)
    return;

  for (uint ii = 1; ii < _num; ii++) {
    int xy  = get(ii);
    int pxy = g->getClosestTrackCoord(xy);
    int dd  = pxy - xy;
    dd      = abs(dd);
    if (dd < _mod * 0.1)  // to guard against nonexisten power grid lines
      set(ii, pxy);
  }
}
Ath__quadTable::Ath__quadTable(uint       m,
                               int        minY,
                               int        maxY,
                               uint       n,
                               int        minX,
                               int        maxX,
                               Ath__grid* gH,
                               Ath__grid* gV)
{
  assert((m < MAX32 + 1) && (n < MAX32 + 1));

  _topDir     = new char[10000];
  _tmpFullDir = new char[10000];
  strcpy(_topDir, "DESIGN/TOP");

  _rowCnt = m;
  _colCnt = n;

  _minTileSize = ath__min((maxY - minY) / _rowCnt, (maxX - minX) / _colCnt);

  initPinTable(0);

  _tmpItermTable = alloc2Darray1d(_rowCnt, _colCnt, 32);
  _localNetTable = alloc2Darray1d(_rowCnt, _colCnt, 32);
  _termTable     = alloc2Darray1d(_rowCnt, _colCnt, 32);
  _ioNetTable    = alloc2Darray1d(_rowCnt, _colCnt, 16);

  _targetArray = new Ath__connWord[10000];
  _tmpArray    = new Ath__connWord[100000];
  _fanoutArray = new uint[10000];
  for (uint ii = 0; ii < 10000; ii++)
    _fanoutArray[ii] = 0;

  _rowDivider = new Ath__qCoord(m, minY, maxY);  // y coords
  _rowDivider->snapOnPower(gH);
  _colDivider = new Ath__qCoord(n, minX, maxX);  // x coords
  _colDivider->snapOnPower(gV);

  _tmpCnt = 0;

  _connTable
      = new Ath__connTable(_thruPinPool, 0, _minTileSize / 8, _minTileSize);

  _gFP = NULL;
  _qFP = NULL;

  _pinBoxPool = new AthPool<Ath__box>(false, 10000);
  _pinBoxPool->alloc();
  _qBoxPool = new AthPool<Ath__qPin>(false, 10000);
  _qBoxPool->alloc();
  _qNetPool = new AthPool<Ath__qNet>(false, 10000);
  _qNetPool->alloc();

  qInit(_pinBoxPool, _qBoxPool);

  _pinVisualGap = 0;
  _pinLength    = 0;
  _pinWidth     = 0;
  _pinStyle     = 0;
  _pinSpace     = 0;
  _topMetH      = 0;
  _topMetV      = 0;
  _qShapeTable  = NULL;

  _placePins_thru   = 0;
  _placePins_next   = 0;
  _placePins_corner = 0;
}
Ath__quadTable::~Ath__quadTable()
{
  freePinTable();

  qFree();

  de_alloc2Darray1d(_tmpItermTable, _rowCnt, _colCnt);
  de_alloc2Darray1d(_localNetTable, _rowCnt, _colCnt);
  de_alloc2Darray1d(_ioNetTable, _rowCnt, _colCnt);
  de_alloc2Darray1d(_termTable, _rowCnt, _colCnt);

  delete[] _tmpArray;
  delete[] _targetArray;
  delete[] _fanoutArray;
  delete _rowDivider;
  delete _colDivider;

  delete _connTable;

  ATH__closeFile(_gFP);
  ATH__closeFile(_qFP);

  delete _qBoxPool;
  delete _pinBoxPool;
  delete _qNetPool;

  free_shapeTable();
}
Ath__connWord Ath__quadTable::getRowCol(int  x1,
                                        int  y1,
                                        int  x2,
                                        int  y2,
                                        uint termtag)
{
  int row = _rowDivider->getIndex(y1);
  assert(row >= 0);

  int col = _colDivider->getIndex(x1);
  assert(col >= 0);

  _tmpItermTable[row][col]->add(termtag);
  _tmpBox[row][col]->setMaxBox(x1, y1, x2, y2);

  Ath__connWord w(row, col);
  return w;
}

void Ath__quadTable::addPoint(int x1, int y1, int x2, int y2, uint termtag)
{
  _tmpArray[_tmpCnt++] = getRowCol(x1, y1, x2, y2, termtag);
}
bool Ath__quadTable::addTarget(Ath__connWord w, uint netId, uint netTermCnt)
{
  uint col;
  uint row = w.getFromRowCol(&col);

  uint tmpItermCnt = _tmpItermTable[row][col]->getCnt();

  if (tmpItermCnt <= 0)  // TODO: Must check for block terms
    return false;

  if (tmpItermCnt < netTermCnt) {  // not local

    uint startTermIndex = _termTable[row][col]->getCnt();
    for (uint ii = 0; ii < tmpItermCnt; ii++) {
      uint termId = _tmpItermTable[row][col]->get(ii);
      _termTable[row][col]->add(termId);
    }
    _tmpItermTable[row][col]->resetCnt(0);

    uint ioQnetId = addQnet(netId, w, startTermIndex, tmpItermCnt, row, col);
    _ioNetTable[row][col]->add(ioQnetId);

    _tmpNetArray[_targetCnt]
        = ioQnetId;  // TO be used later during qPin creation
    _targetArray[_targetCnt++] = w;

    return false;
  } else {  // LOCAL NET
    _localNetTable[row][col]->add(netId);
    _tmpItermTable[row][col]->resetCnt(0);

    return true;
  }
}
void Ath__quadTable::printTargets(int type, int netIndex, FILE* fp)
{
  fprintf(fp, "net net_%d_%d  ", type, netIndex);
  for (uint ii = 0; ii < _targetCnt; ii++) {
    Ath__connWord v = _targetArray[ii];
    fprintf(fp, "q_%d_%d ", v.getFrom(), v.getTo());
  }
  fprintf(fp, "\n");
}
void Ath__quadTable::printSquareNode(FILE*       fp,
                                     uint        cnt,
                                     uint        ii,
                                     uint        jj,
                                     char*       type,
                                     const char* preName,
                                     int         x1,
                                     int         y1,
                                     int         xy2,
                                     uint        width,
                                     bool        horizFlag)
{
  if (cnt == 0)
    return;

  fprintf(fp,
          "%s %s q_%d_%d_%s%d  %d %d   ",
          "node",
          type,
          ii,
          jj,
          preName,
          cnt,
          x1,
          y1);

  if (horizFlag)
    fprintf(fp, "%d %d\n", xy2, y1 + cnt * width);
  else
    fprintf(fp, "%d %d\n", x1 + cnt * width, xy2);
}
FILE* Ath__quadTable::openQuadFile(const char* dir, int row, int col)
{
  char buff[256];

  if ((row < 0) && (col < 0))
    sprintf(buff, "%s/%s", dir, "quad");
  else if ((row >= 0) && (col >= 0))
    sprintf(buff, "%s/%s_%d_%d", dir, "q", row, col);
  else if (row >= 0)
    sprintf(buff, "%s/%s%d", dir, "row", row);
  else if (col >= 0)
    sprintf(buff, "%s/%s%d", dir, "col", col);

  return ATH__openFile(buff, "w");
}
void Ath__quadTable::print1q(uint  row,
                             uint  col,
                             int   offsetX,
                             int   offsetY,
                             uint  width,
                             char* dir)
{
  FILE* fp = openQuadFile(dir, row, col);
  printThruCnts(row, col, offsetX, offsetY, width, fp);
  printNextCnts(row, col, offsetX, offsetY, width, fp);
  printHVCnts(row, col, offsetX, offsetY, width, fp);
  printVHCnts(row, col, offsetX, offsetY, width, fp);
  ATH__closeFile(fp);
}
void Ath__quadTable::printAllQuads(int   offsetX,
                                   int   offsetY,
                                   uint  width,
                                   char* dir)
{
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      print1q(ii, jj, offsetX, offsetY, width, dir);
    }
  }
}
uint Ath__quadTable::getQuadBoxes(Ath__zui* zui)
{
  uint iCnt = 0;
  if (_quad == NULL)
    return 0;

  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      Ath__qCounter* q = _quad[ii][jj];

      int x1 = _colDivider->get(jj);
      int x2 = _colDivider->getNext(jj);
      int y1 = _rowDivider->get(ii);
      int y2 = _rowDivider->getNext(ii);

      if (q->_name == NULL)
        q->setName(ii, jj);

      zui->addBox(
          10000 * ii + jj, Ath_box__bbox, Ath_hier__tile, 0, x1, y1, x2, y2);

      iCnt++;
    }
  }
  return iCnt;
}
void Ath__quadTable::printQuad(int offsetX, int offsetY, uint width, char* dir)
{
  FILE* fp = openQuadFile(dir, -1, -1);

  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      printThruCnts(ii, jj, offsetX, offsetY, width, fp);
      printNextCnts(ii, jj, offsetX, offsetY, width, fp);
      printHVCnts(ii, jj, offsetX, offsetY, width, fp);
      printVHCnts(ii, jj, offsetX, offsetY, width, fp);
    }
  }
  ATH__closeFile(fp);
}
void Ath__quadTable::printRows(int offsetX, int offsetY, uint width, char* dir)
{
  for (uint ii = 0; ii < _rowCnt; ii++) {
    printQuadRow(ii, offsetX, offsetY, width, dir);
  }
}
void Ath__quadTable::printCols(int offsetX, int offsetY, uint width, char* dir)
{
  for (uint ii = 0; ii < _colCnt; ii++) {
    printQuadCol(ii, offsetX, offsetY, width, dir);
  }
}
void Ath__quadTable::printQuadRow(uint  row,
                                  int   offsetX,
                                  int   offsetY,
                                  uint  width,
                                  char* dir)
{
  FILE* fp = openQuadFile(dir, row, -1);
  uint  jj;
  for (jj = 0; jj < _colCnt; jj++) {
    printThruCnts(row, jj, offsetX, offsetY, width, fp);
    printNextCnts(row, jj, offsetX, offsetY, width, fp);
    printHVCnts(row, jj, offsetX, offsetY, width, fp);
    printVHCnts(row, jj, offsetX, offsetY, width, fp);
  }
  ATH__closeFile(fp);
}
void Ath__quadTable::printQuadCol(uint  col,
                                  int   offsetX,
                                  int   offsetY,
                                  uint  width,
                                  char* dir)
{
  FILE* fp = openQuadFile(dir, -1, col);
  for (uint ii = 0; ii < _rowCnt; ii++) {
    printThruCnts(ii, col, offsetX, offsetY, width, fp);
    printNextCnts(ii, col, offsetX, offsetY, width, fp);
    printHVCnts(ii, col, offsetX, offsetY, width, fp);
    printVHCnts(ii, col, offsetX, offsetY, width, fp);
  }
  ATH__closeFile(fp);
}

void Ath__quadTable::printThruCnts(uint  ii,
                                   uint  jj,
                                   int   offsetX,
                                   int   offsetY,
                                   uint  width,
                                   FILE* fp)
{
  Ath__qCounter* q = _quad[ii][jj];

  int minX = _colDivider->get(jj);
  int maxX = _colDivider->getNext(jj);
  int minY = _rowDivider->get(ii);
  int maxY = _rowDivider->getNext(ii);
  int x1   = minX + offsetX;
  int y1   = minY + offsetY;

  printSquareNode(
      fp, q->_thru[0], ii, jj, "metH", "H", minX, y1, maxX, width, true);

  printSquareNode(
      fp, q->_thru[1], ii, jj, "metV", "V", x1, minY, maxY, width, false);
}
void Ath__quadTable::printNextCnts(uint  ii,
                                   uint  jj,
                                   int   offsetX,
                                   int   offsetY,
                                   uint  width,
                                   FILE* fp)
{
  Ath__qCounter* q = _quad[ii][jj];

  int x1 = _colDivider->getCenter(jj, 0);
  int x2 = _colDivider->getCenter(jj + 1, 0);
  int y1 = _rowDivider->getCenter(ii, 0);
  int y2 = _rowDivider->getCenter(ii + 1, 0);

  printSquareNode(
      fp, q->_next[0], ii, jj, "metHH", "HH", x1, y1, x2, width, true);

  printSquareNode(
      fp, q->_next[1], ii, jj, "metVV", "VV", x1, y1, y2, width, false);
}
void Ath__quadTable::printVHCnts(uint  ii,
                                 uint  jj,
                                 int   offsetX,
                                 int   offsetY,
                                 uint  width,
                                 FILE* fp)
{
  Ath__qCounter* q = _quad[ii][jj];

  int centX = _colDivider->getCenter(jj, offsetX);
  int centY = _rowDivider->getCenter(ii, -offsetY);
  int maxX  = _colDivider->getNext(jj);
  int minY  = _rowDivider->get(ii);
  /*
          printSquareNode(fp, q->_turnVH, ii, jj,"turnVH","VHV",
                  centX, minY, centY, width, 0);
          printSquareNode(fp, q->_turnVH, ii, jj,"turnVH","VHH",
                  centX, centY, maxX, width, 1);
                  */
}
void Ath__quadTable::printHVCnts(uint  ii,
                                 uint  jj,
                                 int   offsetX,
                                 int   offsetY,
                                 uint  width,
                                 FILE* fp)
{
  Ath__qCounter* q = _quad[ii][jj];

  int centX = _colDivider->getCenter(jj, -offsetX);
  int centY = _rowDivider->getCenter(ii, -offsetY);

  int minX = _colDivider->get(jj);
  int maxX = _colDivider->getNext(jj);
  int minY = _rowDivider->get(ii);
  int maxY = _rowDivider->getNext(ii);
  /*
          printSquareNode(fp, q->_turnHV, ii, jj,"turnHV","HVV",
                  centX, minY, centY, width, 0);
          printSquareNode(fp, q->_turnHV, ii, jj,"turnHV","HVH",
                  minX, centY, centX, width, 1);
                  */
}
uint Ath__quadTable::getNextCnt()
{
  uint cnt = 0;
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      cnt += _quad[ii][jj]->_next[0];
      cnt += _quad[ii][jj]->_next[1];
    }
  }
  return cnt;
}

void Ath__quadTable::reset(Ath__connWord w)
{
  uint col;
  uint row = w.getFromRowCol(&col);

  _tmpItermTable[row][col]->resetCnt(0);
  _tmpBox[row][col]->resetMaxBox();
}
void Ath__quadTable::reset()
{
  reset(_tmpSrc);
  for (uint ii = 0; ii < _tmpCnt; ii++) {
    reset(_tmpArray[ii]);
  }
  _tmpCnt = 0;
}
void Ath__quadTable::printFanoutTable(FILE* fp)
{
  fprintf(fp, "\n\ntile-pins per net distribution\n");
  fprintf(fp, "------------------------------------------\n");

  for (uint ii = 0; ii < 10000; ii++) {
    if (_fanoutArray[ii] > 0)
      fprintf(stdout, "%2d %6d\n", ii, _fanoutArray[ii]);
  }
}
void Ath__quadTable::printConnectivityStats(FILE* fp, uint netCnt, uint termCnt)
{
  printFanoutTable(fp);

  _connTable->printConnStats(fp);
}
void Ath__quadTable::assignStraightTracks(uint dir, bool makePin)
{
  uint cnt = 0;
  for (uint dist = 31; dist > 1; dist--) {
    if (_connTable->startStraightIterator(dir, dist) == 0)
      continue;

    while (Ath__p2pConn* p2p = _connTable->getNextConn()) {
      calcThruResources(p2p);
      cnt++;
    }
  }
  fprintf(stdout, "%d for assignStraightTracks[%d]\n", cnt, dir);
}
void Ath__quadTable::printQuadRow(uint row, Ath__zui* zui)
{
  for (uint jj = 0; jj < _colCnt; jj++) {
    zui->printQuadBox(row,
                      jj,
                      _colDivider->get(jj),
                      _rowDivider->get(row),
                      _colDivider->getNext(jj),
                      _rowDivider->getNext(row));
  }
}
void Ath__quadTable::printQuads()
{
  Ath__zui zui("QUADS");

  for (uint ii = 0; ii < _rowCnt; ii++) {
    printQuadRow(ii, &zui);
  }
}
void Ath__quadTable::printRow(uint row, Ath__qPinTable* pinTable, Ath__zui* zui)
{
  for (uint l = 0; l < pinTable->getCnt(); l++) {
    Ath__qPin*    p = pinTable->get(l);
    Ath__connWord v = p->_conn;
    uint          c1;
    uint          r1 = v.getFromRowCol(&c1);
    uint          c2;
    uint          r2 = v.getToRowCol(&c2);
    zui->printSegOnTrack(_colDivider->get(c1), _colDivider->get(c2), l);
  }
}
void Ath__quadTable::printPinTable(char*           qName,
                                   uint            row,
                                   Ath__qPinTable* pinTable)
{
  if (pinTable->getCnt() <= 0)
    return;

  Ath__zui zui(qName, row);
  printQuadRow(row, &zui);
  zui.setTrackPitch("METAL5",
                    0,
                    pinTable->getCnt(),
                    _rowDivider->get(row),
                    _rowDivider->getNext(row));

  for (uint l = 0; l < pinTable->getCnt(); l++) {
    for (Ath__qPin* p = pinTable->get(l); p != NULL; p = p->_next) {
      Ath__connWord v = p->_conn;
      uint          c1;
      uint          r1 = v.getFromRowCol(&c1);
      uint          c2;
      uint          r2 = v.getToRowCol(&c2);

      zui.printSegOnTrack(_colDivider->get(c1 + 1), _colDivider->get(c2), l);
    }
  }
}
uint Ath__quadTable::addObs(Ath__box* bb)
{
  int row1 = _rowDivider->getMinMaxIndex(bb->_ylo);
  assert(row1 >= 0);
  int row2 = _rowDivider->getMinMaxIndex(bb->_yhi);
  assert(row2 >= 0);

  int col1 = _colDivider->getMinMaxIndex(bb->_xlo);
  assert(col1 >= 0);

  int col2 = _colDivider->getMinMaxIndex(bb->_xhi);
  assert(col2 >= 0);

  int cnt = 0;
  for (int ii = row1; ii < row2 + 1; ii++) {
    for (int jj = col1; jj < col2 + 1; jj++) {
      cnt += _obsTable[ii][jj]->add(bb);
    }
  }
  return cnt;
}
void Ath__quadTable::blockTracks()
{
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      _qShapeTable[ii][jj]->blockTracks(_obsTable[ii][jj]);
    }
  }
}
void Ath__quadTable::setTracks(uint hPitch, uint vPitch)
{
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      int  x1   = _colDivider->get(jj);
      int  x2   = _colDivider->get(jj + 1);
      int  y1   = _rowDivider->get(ii);
      int  y2   = _rowDivider->get(ii + 1);
      uint hCnt = (y2 - y1) / hPitch;
      uint vCnt = (x2 - x1) / vPitch;

      Ath__qCounter* q = _quad[ii][jj];

      if ((ii == 0) || (ii == _rowCnt - 1))
        q->setTracks(ii, jj, hCnt, vCnt, 0);
      else if ((jj == 0) || (jj == _colCnt - 1))
        q->setTracks(ii, jj, hCnt, vCnt, 1);
      else
        q->setTracks(ii, jj, hCnt, vCnt);
    }
  }
}
uint Ath__quadTable::assignTracksDist2()
{
  uint cnt  = 0;
  uint dist = 1;
  if (_connTable->startCornerIterator(dist) == 0)
    return 0;

  while (Ath__p2pConn* p2p = _connTable->getNextConn()) {
    Ath__connWord w = p2p->_conn;
    uint          c1;
    uint          r1 = w.getFromRowCol(&c1);
    uint          c2;
    uint          r2 = w.getToRowCol(&c2);
    uint          midRow;
    uint          midCol;
    uint          type;

    if (checkMidRes(r1, c1, r2, c2, &midRow, &midCol, &type)) {
      _quad[midRow][midCol]->updateTurn(type);
      _thruPinTable[midRow][midCol]->addCornerConn2(type, p2p, midRow, midCol);
      p2p->_srcPin->_assigned = 1;
    } else {
      p2p->_srcPin->_assigned = 0;
      cnt++;  // TODO connection....
    }
  }
  return cnt;
}
uint Ath__quadTable::assignThinTurns(uint dir)
{
  uint cnt     = 0;
  uint connCnt = 0;

  for (uint dist = 31; dist > 2; dist--) {
    if (_connTable->startCornerIterator(dist) == 0)
      continue;

    while (Ath__p2pConn* p2p = _connTable->getNextConn()) {
      Ath__connWord w     = p2p->_conn;
      uint          netId = p2p->_netId;

      uint rowDist = w.getRowDistAbs();
      uint colDist = w.getColDistAbs();
      if (!((rowDist == 1) || (colDist == 1)))
        continue;

      uint c1;
      uint r1 = w.getFromRowCol(&c1);
      uint c2;
      uint r2 = w.getToRowCol(&c2);
      uint midRow;
      uint midCol;
      uint type;

      if (!(checkMidRes(r1, c1, r2, c2, &midRow, &midCol, &type))) {
        cnt++;
        continue;  // TODO connection
      }
      connCnt++;

      Ath__p2pConn* midConn = NULL;
      Ath__p2pConn* conn2   = NULL;
      if (rowDist == 1) {
        int cdist = 1;
        if (w.getColDist() < 0)
          cdist = -1;
        Ath__connWord w0(r1, c1, midRow, midCol + cdist, _colCnt);
        midConn = _connTable->addConn(netId, w0);

        Ath__connWord w2(r2, c2, midRow, midCol, _colCnt);
        conn2 = _connTable->addConn(netId, w2);
      } else {
        Ath__connWord w2(r2, c2, midRow - 1, midCol, _colCnt);
        midConn = _connTable->addConn(netId, w2);

        Ath__connWord w0(r1, c1, midRow, midCol, _colCnt);
        conn2 = _connTable->addConn(netId, w0);
      }
      midConn->setPin(p2p->_srcPin);
      _thruPinTable[midRow][midCol]->addCornerConn2(
          type, midConn, midRow, midCol);
      _quad[midRow][midCol]->updateTurn(type);

      conn2->setPin(p2p->_srcPin);
      calcThruResources(conn2);

      p2p->setNext(midConn);
      midConn->setNext(conn2);
    }
  }
  fprintf(stdout, "%d for assignThinTurns\n", connCnt);

  return cnt;
}
uint Ath__quadTable::assignNotThinTurns(uint dir)
{
  uint cnt     = 0;
  uint connCnt = 0;

  for (uint dist = 31; dist > 2; dist--) {
    if (_connTable->startCornerIterator(dist) == 0)
      continue;

    while (Ath__p2pConn* p2p = _connTable->getNextConn()) {
      Ath__connWord w     = p2p->_conn;
      uint          netId = p2p->_netId;

      uint rowDist = w.getRowDistAbs();
      uint colDist = w.getColDistAbs();

      if ((rowDist == 1) || (colDist == 1))
        continue;

      uint c1;
      uint r1 = w.getFromRowCol(&c1);
      uint c2;
      uint r2 = w.getToRowCol(&c2);
      uint midRow;
      uint midCol;
      uint type;

      if (!(checkMidRes(r1, c1, r2, c2, &midRow, &midCol, &type))) {
        cnt++;
        continue;  // TODO connection
      }
      connCnt++;

      Ath__p2pConn* midConn = NULL;
      Ath__p2pConn* conn1   = NULL;
      Ath__p2pConn* conn2   = NULL;

      if (w.getColDist() > 0) {
        Ath__connWord w0(midRow - 1, midCol, midRow, midCol + 1, _colCnt);
        midConn = _connTable->addConn(netId, w0);
      } else {
        Ath__connWord w0(midRow - 1, midCol, midRow, midCol - 1, _colCnt);
        midConn = _connTable->addConn(netId, w0);
      }

      Ath__connWord w1(r1, c1, midRow, midCol, _colCnt);
      conn1 = _connTable->addConn(netId, w1);

      Ath__connWord w2(r2, c2, midRow, midCol, _colCnt);
      conn2 = _connTable->addConn(netId, w2);

      midConn->setPin(p2p->_srcPin);
      _thruPinTable[midRow][midCol]->addCornerConn2(
          type, midConn, midRow, midCol);
      _quad[midRow][midCol]->updateTurn(type);

      conn1->setPin(p2p->_srcPin);
      calcThruResources(conn1);

      conn2->setPin(p2p->_srcPin);
      calcThruResources(conn2);

      p2p->setNext(midConn);
      midConn->setNext(conn1);
      conn1->setNext(conn2);
    }
  }
  fprintf(stdout, "%d for assign_Not_ThinTurns\n", connCnt);

  return cnt;
}
void Ath__quadTable::checkResources(int threshold)
{
  uint hotCnt = 0;
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      hotCnt += _quad[ii][jj]->thruSummary(ii, jj, 0, threshold);
      hotCnt += _quad[ii][jj]->thruSummary(ii, jj, 1, threshold);
    }
  }
}
uint Ath__quadTable::printHotSpots()
{
  uint hotCnt = 0;
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      hotCnt += _quad[ii][jj]->printHotSummary();
    }
  }
  return hotCnt;
}
void Ath__quadTable::resetTracks(uint hPitch, uint vPitch)
{
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      int x1 = _colDivider->get(jj);
      int x2 = _colDivider->get(jj + 1);
      int y1 = _rowDivider->get(ii);
      int y2 = _rowDivider->get(ii + 1);

      Ath__qCounter* q = _quad[ii][jj];
      q->resetTracks(ii, jj, (y2 - y1) / hPitch, (x2 - x1) / vPitch);
    }
  }
}
uint Ath__quadTable::getBoundaries(uint      ii,
                                   uint      jj,
                                   Ath__zui* zui,
                                   uint      w,
                                   uint      layer)
{
  int minX = _colDivider->get(jj);
  int maxX = _colDivider->getNext(jj);
  int minY = _rowDivider->get(ii);
  int maxY = _rowDivider->getNext(ii);

  zui->addBox(ii * 10000 + jj,
              Ath_box__bbox,
              Ath_hier__tile,
              layer,
              minX + w,
              minY + w,
              maxX - w,
              maxY - w);
  /*
          zui->addBox(QUAD_TYPE, CHIP, 0, minX,minY,minX+w, maxY, NULL);
          zui->addBox(QUAD_TYPE, CHIP, 0, maxX,minY,maxX+w, maxY, NULL);
          zui->addBox(QUAD_TYPE, CHIP, 0, minX,minY,maxX, minY+w, NULL);
          zui->addBox(QUAD_TYPE, CHIP, 0, minX,maxY,maxX, maxY+w, NULL);
  */
  return 1;
}
void Ath__quadTable::getTileBounds(uint ii,
                                   uint jj,
                                   int* minX,
                                   int* maxX,
                                   int* minY,
                                   int* maxY,
                                   int* centerX,
                                   int* centerY)
{
  uint w = 4000;
  *minX  = _colDivider->get(jj) + w;
  *maxX  = _colDivider->getNext(jj) - w;
  *minY  = _rowDivider->get(ii) + w;
  *maxY  = _rowDivider->getNext(ii) - w;

  *centerX = _colDivider->getCenter(jj, 0);
  *centerY = _rowDivider->getCenter(ii, 0);
}
uint Ath__quadTable::getSpacing(uint cnt,
                                int  minX,
                                int  maxX,
                                uint width,
                                int* centerX)
{
  uint n   = (maxX - minX) / 3;
  *centerX = minX + n;

  int spacing = (n - cnt * width) / n;
  if (spacing <= 0)
    spacing = width / 4;

  return spacing;
}
uint Ath__quadTable::assignTracks(uint hPitch, uint vPitch, bool resetRes)
{
  _hPitch = hPitch;
  _vPitch = vPitch;

  uint unConnCnt = 0;

  if (resetRes) {
    resetTracks(hPitch, vPitch);
  } else {
    setTracks(hPitch, vPitch);
  }

  assignStraightTracks(0, resetRes);
  assignStraightTracks(1, resetRes);
  unConnCnt += assignTracksDist2();
  unConnCnt += assignThinTurns(0);
  unConnCnt += assignNotThinTurns(0);
  /*
          checkResources(100);
  */

  uint hotCnt = printHotSpots();
  fprintf(stdout, "%d hot spots\n", hotCnt);

  freePinTable(true);

  return hotCnt;
}
