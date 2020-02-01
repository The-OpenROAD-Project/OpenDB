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

#include "misc_global.h"
#include "qtype.h"

int Ath__quadTable::getTileXlo(uint row, uint col)
{
  int x1, y1, x2, y2;
  _busTable[row][col]->getBounds(&x1, &y1, &x2, &y2);
  return x1;
}
int Ath__quadTable::getTileYlo(uint row, uint col)
{
  int x1, y1, x2, y2;
  _busTable[row][col]->getBounds(&x1, &y1, &x2, &y2);
  return y1;
}
int Ath__quadTable::getTileXhi(uint row, uint col)
{
  int x1, y1, x2, y2;
  _busTable[row][col]->getBounds(&x1, &y1, &x2, &y2);
  return x2;
}
int Ath__quadTable::getTileXlo(uint col)
{
  return getTileXlo(0, col);
}
int Ath__quadTable::getTileYlo(uint row)
{
  return getTileYlo(row, 0);
}
int Ath__quadTable::getTileXhi(uint col)
{
  return getTileXhi(0, col);
}
int Ath__quadTable::getTileYhi(uint row, uint col)
{
  int x1, y1, x2, y2;
  _busTable[row][col]->getBounds(&x1, &y1, &x2, &y2);
  return y2;
}
int Ath__quadTable::getTileYhi(uint row)
{
  return getTileYhi(row, 0);
}

void Ath__quadTable::startQpinIterator(uint ii, uint jj)
{
  _qPinTmpTable = _qPinTable[ii][jj];
  _qPinTmpTable->startIterator();
}

Ath__qPin* Ath__quadTable::getNextSrcPin_next()
{
  return _qPinTmpTable->getNextSrcPin_next();
}
Ath__qPin* Ath__quadTable::getNextSrcPin_thru()
{
  return _qPinTmpTable->getNextSrcPin_thru();
}
Ath__qPin* Ath__quadTable::getNextSrcPin_corner()
{
  return _qPinTmpTable->getNextSrcPin_corner();
}
Ath__qPin* Ath__quadTable::getNextSrcPin_all()
{
  return _qPinTmpTable->getNextSrcPin_all();
}
uint Ath__quadTable::makePinTargetsStraight(uint       row,
                                            uint       col,
                                            uint*      met,
                                            Ath__qPin* srcPin,
                                            int*       xlo,
                                            int*       ylo,
                                            int*       xhi,
                                            int*       yhi)
{
  uint dist = srcPin->_conn.getDist();

  Ath__qPinTable* srcPinTable = _qPinTable[row][col];
  Ath__qPinTable* dstPinTable;

  uint dir = 0;
  if (srcPin->_conn.getRowDist() > 0)  // vertical
  {
    dstPinTable = _qPinTable[row + dist][col];
    dir         = 1;
  } else {
    dstPinTable = _qPinTable[row][col + dist];
  }
  uint      netId = srcPin->_netId;
  Ath__box* srcBB
      = srcPinTable->addNextPinBox(netId, dir, met[dir], 0, 0, 0, 0);
  Ath__box* dstBB
      = dstPinTable->addNextPinBox(netId, dir, met[dir], 0, 0, 0, 0);

  Ath__qPin* dstPin = srcPin->_next;
  Ath__box*  srcBox = srcPin->getInstBox();
  Ath__box*  dstBox = dstPin->getInstBox();

  uint n = srcBox->_id;
  // Need to guard for instances being cut by the divide line

  int srcMidX = srcBox->getMidX(getTileXlo(col), getTileXhi(col));

  int srcMidY = srcBox->getMidY(getTileYlo(row), _rowDivider->getNext(row));

  if (dir > 0)  // vertical
  {
    int dstMidX = dstBox->getMidX(getTileXlo(col), ath__maxInt);
    int dstMidY = dstBox->getMidY(getTileYlo(row + dist), ath__maxInt);

    int x1 = (srcMidX + dstMidX) / 2;

    int yloBound = _rowDivider->getNext(row);
    int yhiBound = getTileYlo(row + dist);

    srcBB->set(x1, srcMidY, x1, yloBound);
    dstBB->set(x1, yhiBound, x1, dstMidY);

    if (xlo != NULL) {
      *xlo = x1;
      *xhi = x1;
      *ylo = yloBound;
      *yhi = yhiBound;
    }
  } else {
    int dstMidX = dstBox->getMidX(getTileXlo(col + dist), ath__maxInt);
    int dstMidY = dstBox->getMidY(getTileYlo(row), ath__maxInt);

    int y1 = (srcMidY + dstMidY) / 2;

    int xloBound = getTileXhi(col);
    int xhiBound = getTileXlo(col + dist);

    srcBB->set(srcMidX, y1, xloBound, y1);
    dstBB->set(xhiBound, y1, dstMidX, y1);

    if (xlo != NULL) {
      *xlo = xloBound;
      *xhi = xhiBound;
      *ylo = y1;
      *yhi = y1;
    }
  }
  srcPin->_targetBB = srcBB;
  dstPin->_targetBB = dstBB;

  return dir;
}
uint Ath__quadTable::makeNextPinShapes(uint row, uint col, int priority)
{
  uint met[2] = {_topMetH - 2, _topMetV - 2};

  uint cnt = 0;
  startQpinIterator(row, col);
  while (Ath__qPin* srcPin = getNextSrcPin_next()) {
    if (_pinStyle == 0) {  // area pins
      makePinTargetsStraight(row, col, met, srcPin);
    } else if (_pinStyle == 1) {
      // TODO createEdgePins(srcPin, dstPin, row, col, dir, layer, &pinBB,
      // _pinLength);
    }
    cnt += 2;
  }
  return cnt;
}
uint Ath__quadTable::makeJumperPinShapes(uint row, uint col)
{
  uint met[2] = {_topMetH, _topMetV};

  uint cnt = 0;

  startQpinIterator(row, col);
  while (Ath__qPin* srcPin = getNextSrcPin_thru()) {
    if (_pinStyle == 0) {  // area pins
      int  x1, y1, x2, y2;
      uint dir
          = makePinTargetsStraight(row, col, met, srcPin, &x1, &y1, &x2, &y2);

      srcPin->_obsList = addBusBox(
          x1, y1, x2, y2, met[dir], srcPin->_netId, srcPin->_obsList);
    } else if (_pinStyle == 1) {
      // TODO createEdgePins(srcPin, dstPin, row, col, dir, layer, &pinBB,
      // _pinLength);
    }
    cnt += 2;
  }
  return cnt;
}
void Ath__quadTable::pinToTurnPathH(Ath__qPin*      pin,
                                    Ath__qPinTable* pinTable,
                                    uint            dist,
                                    uint            type,
                                    int             y,
                                    int             x1,
                                    int             x2,
                                    int             x3,
                                    int             x4)
{
  uint ownerId = pin->_nameId;

  // Area Pin from Src point to end of tile, horizontally
  pin->_targetBB = pinTable->addCornerPinBox(
      ownerId, type, _topMetH, x1, y, x2, y);  // go right

  // segment from edge of turning tile to the turing point
  pin->_obsList = addBusBox(x3, y, x4, y, _topMetH, ownerId, pin->_obsList);

  // straight segment from src tile to turning tile, horizontally
  if (dist > 1) {
    pin->_obsList = addBusBox(x2, y, x3, y, _topMetH, ownerId, pin->_obsList);
  }
}
void Ath__quadTable::pinToTurnPathV(Ath__qPin*      pin,
                                    Ath__qPinTable* pinTable,
                                    uint            dist,
                                    uint            type,
                                    int             x,
                                    int             y1,
                                    int             y2,
                                    int             y3,
                                    int             y4)
{
  uint ownerId = pin->_nameId;
  // Area Pin from Src point to end of tile, vertically
  pin->_targetBB = pinTable->addCornerPinBox(
      ownerId, type, _topMetV, x, y1, x, y2);  // go up

  // TODO_NOW pin->_targetBB= addBusBox( x, y1, x, y2, _topMetV, ownerId,
  // pin->_targetBB);

  // segment from edge of turning tile to the turing point
  pin->_obsList = addBusBox(x, y3, x, y4, _topMetV, ownerId, pin->_obsList);

  // straight segment from src tile to turning tile, vertically
  if (dist > 1) {
    pin->_obsList = addBusBox(x, y2, x, y3, _topMetV, ownerId, pin->_obsList);
  }
}
void Ath__quadTable::turnToPinPathV(Ath__qPin*      pin,
                                    Ath__qPinTable* pinTable,
                                    uint            dist,
                                    uint            type,
                                    int             x,
                                    int             y1,
                                    int             y2,
                                    int             y3,
                                    int             y4)
{
  // tile(row,col) is dst
  uint ownerId = pin->_nameId;

  // Area Pin from Src point to end of tile, horizontally

  pin->_targetBB = pinTable->addCornerPinBox(
      ownerId, type, _topMetV, x, y3, x, y4);  // go right

  // segment from edge turing point to dst point
  pin->_obsList = addBusBox(x, y1, x, y2, _topMetV, ownerId, pin->_obsList);

  // straight segment from src tile to turning tile, horizontally

  if (dist > 1) {
    pin->_obsList = addBusBox(x, y2, x, y3, _topMetV, ownerId, pin->_obsList);
  }
}
void Ath__quadTable::turnToPinPathH(Ath__qPin*      pin,
                                    Ath__qPinTable* pinTable,
                                    uint            dist,
                                    uint            type,
                                    int             y,
                                    int             x1,
                                    int             x2,
                                    int             x3,
                                    int             x4)
{
  // tile(row,col) is dst
  uint ownerId = pin->_nameId;

  // Area Pin from Src point to end of tile, horizontally

  pin->_targetBB = pinTable->addCornerPinBox(
      ownerId, type, _topMetH, x3, y, x4, y);  // go right

  // segment from edge turing point to dst point

  pin->_obsList = addBusBox(x1, y, x2, y, _topMetH, ownerId, pin->_obsList);

  // straight segment from src tile to turning tile, horizontally

  if (dist > 1) {
    pin->_obsList = addBusBox(x2, y, x3, y, _topMetH, ownerId, pin->_obsList);
  }
}
void Ath__quadTable::getConnPoints(Ath__box* srcBox,
                                   Ath__box* dstBox,
                                   uint      fromRow,
                                   uint      fromCol,
                                   uint      toRow,
                                   uint      toCol,
                                   int*      x1,
                                   int*      y1,
                                   int*      x2,
                                   int*      y2)
{
  // all getTile* calls compensate for cutting instances in the middle during
  // tiling

  *x1 = srcBox->getMidX(getTileXlo(fromRow, fromCol),
                        getTileXhi(fromRow, fromCol));

  *y1 = srcBox->getMidY(getTileYlo(fromRow, fromCol),
                        getTileYhi(fromRow, fromCol));

  *x2 = dstBox->getMidX(getTileXlo(toRow, toCol), getTileXhi(toRow, toCol));
  *y2 = dstBox->getMidY(getTileYlo(toRow, toCol), getTileYhi(toRow, toCol));
}

uint Ath__quadTable::makeCornerPinShapes(uint fromRow, uint fromCol, uint dist)
{
  uint cnt = 0;

  startQpinIterator(fromRow, fromCol);
  while (Ath__qPin* srcPin = getNextSrcPin_corner()) {
    if (srcPin->isTargeted())
      continue;

    if (srcPin->_conn.getMinDist() != dist)
      continue;

    Ath__qPinTable* srcPinTable = _qPinTable[fromRow][fromCol];

    Ath__qPin* dstPin = srcPin->_next;
    uint       toCol;
    uint       toRow = dstPin->getToRowCol(&toCol);

    Ath__qPinTable* dstPinTable = _qPinTable[toRow][toCol];

    int x1, y1, x2, y2;

    getConnPoints(srcPin->getInstBox(),
                  dstPin->getInstBox(),
                  fromRow,
                  fromCol,
                  toRow,
                  toCol,
                  &x1,
                  &y1,
                  &x2,
                  &y2);

    uint turnRow, turnCol;
    uint cornerType = srcPin->getTurnRowCol(&turnRow, &turnCol);

    if (cornerType % 2 == 0)  // Go Up, then Right or Left
    {
      int yBound1 = getTileYhi(fromRow, fromCol);
      int yBound2 = getTileYlo(turnRow, turnCol);

      pinToTurnPathV(srcPin,
                     srcPinTable,
                     turnRow - fromRow,
                     cornerType,
                     x1,
                     y1,
                     yBound1,
                     yBound2,
                     y2);

      if (cornerType == 0)  // Go Right
      {
        int xBound1 = getTileXhi(turnRow, turnCol);
        int xBound2 = getTileXlo(toRow, toCol);

        turnToPinPathH(dstPin,
                       dstPinTable,
                       toCol - turnCol,
                       cornerType,
                       y2,
                       x1,
                       xBound1,
                       xBound2,
                       x2);
      } else if (cornerType == 2)  // Go Left
      {
        int xBound1 = getTileXhi(toRow, toCol);
        int xBound2 = getTileXlo(turnRow, turnCol);

        pinToTurnPathH(dstPin,
                       dstPinTable,
                       turnCol - toCol,
                       cornerType,
                       y2,
                       x2,
                       xBound1,
                       xBound2,
                       x1);
      }
    } else  // Go Right or Left, Then Up
    {
      if (cornerType == 1)  // Go Right
      {
        int xBound1 = getTileXhi(fromRow, fromCol);
        int xBound2 = getTileXlo(turnRow, turnCol);

        pinToTurnPathH(srcPin,
                       srcPinTable,
                       turnCol - fromCol,
                       cornerType,
                       y1,
                       x1,
                       xBound1,
                       xBound2,
                       x2);
      } else if (cornerType == 3)  // Go Left
      {
        int xBound1 = getTileXhi(turnRow, turnCol);
        int xBound2 = getTileXlo(fromRow, fromCol);

        turnToPinPathH(srcPin,
                       srcPinTable,
                       fromCol - turnCol,
                       cornerType,
                       y1,
                       x2,
                       xBound1,
                       xBound2,
                       x1);
      }
      // Go Up

      int yBound1 = getTileYhi(turnRow, turnCol);
      int yBound2 = getTileYlo(toRow, toCol);

      turnToPinPathV(dstPin,
                     dstPinTable,
                     toRow - turnRow,
                     cornerType,
                     x2,
                     y1,
                     yBound1,
                     yBound2,
                     y2);
    }
    srcPin->setTargeted();
    dstPin->setTargeted();
    cnt++;
  }
  return cnt;
}

void Ath__quadTable::placePin(Ath__qPin* pin, uint width)
{
  uint w2        = width / 2;
  pin->_targetBB = _pinBoxPool->alloc();
  pin->_targetBB->set(pin->_targetBB->getMidX() - w2,
                      pin->_targetBB->getMidY() - w2,
                      pin->_targetBB->getMidX() + w2,
                      pin->_targetBB->getMidY() + w2);
}
uint Ath__quadTable::makeTopPins(int connType,
                                 int priority,
                                 int style,
                                 int space,
                                 int width,
                                 int length,
                                 int top_h_layer,
                                 int top_v_layer)
{
  _pinVisualGap = 50;
  _pinLength    = length;
  _pinWidth     = width;
  _pinStyle     = style;
  _topMetH      = top_h_layer;
  _topMetV      = top_v_layer;
  _pinSpace     = space;

  uint cnt = 0;

  uint ii;
  for (ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      if ((connType == 0) || (connType == 1))
        cnt += makeNextPinShapes(ii, jj, priority);
    }
  }
  for (ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      if ((connType == 0) || (connType == 2))
        makeJumperPinShapes(ii, jj);
    }
  }
  uint maxDist = ath__max(_rowCnt, _colCnt);

  for (uint dist = 1; dist <= maxDist; dist++) {
    for (uint ii = 0; ii < _rowCnt; ii++) {
      for (uint jj = 0; jj < _colCnt; jj++) {
        if ((connType == 0) || (connType == 3))
          makeCornerPinShapes(ii, jj, dist);
      }
    }
  }
  return cnt;
}

uint Ath__quadTable::getAllTileBoxes(Ath__zui* zui, uint layer, uint w)
{
  for (uint ii = 0; ii < _tileCnt; ii++) {
    uint gridId = ii;
    uint row    = gridId / _colCnt;
    uint col    = gridId % _colCnt;

    Ath__gridTile* gridTile = _tileTable[gridId];

    int x1, y1, x2, y2;
    gridTile->getBounds(&x1, &y1, &x2, &y2);

    zui->addBox(gridId,
                Ath_box__bbox,
                Ath_hier__tile,
                layer,
                x1 + w,
                y1 + w,
                x2 - w,
                y2 - w);
  }
  return _tileCnt;
}
uint Ath__quadTable::getTileBoundLines(Ath__zui* zui,
                                       uint      layer,
                                       uint      w,
                                       bool      ignore)
{
  if (!ignore && (!zui->getDbFlag("tile/bbox")))
    return 0;

  Ath__searchBox bb(zui->getBbox(), 0, 0);

  Ath__array1D<uint>* idtable = zui->getIdTable();

  idtable->resetCnt();
  _tileSearchGrid->search(&bb, idtable);  // single grid

  for (uint ii = 0; ii < idtable->getCnt(); ii++) {
    uint gridId = idtable->get(ii);
    uint row    = gridId / _colCnt;
    uint col    = gridId % _colCnt;

    Ath__gridTile* gridTile = _tileTable[gridId];

    int x1, y1, x2, y2;
    gridTile->getBounds(&x1, &y1, &x2, &y2);

    zui->addBox(
        gridId, Ath_box__bbox, Ath_hier__tile, layer, x1, y1, x2, y1 + w);
    zui->addBox(
        gridId, Ath_box__bbox, Ath_hier__tile, layer, x1, y1, x1 + w, y2);
    zui->addBox(
        gridId, Ath_box__bbox, Ath_hier__tile, layer, x1, y2, x2, y2 + w);
    zui->addBox(
        gridId, Ath_box__bbox, Ath_hier__tile, layer, x2, y1, x2 + w, y2);
  }
  return idtable->getCnt();
}
uint Ath__quadTable::getTileBoxes(Ath__zui* zui,
                                  uint      layer,
                                  uint      w,
                                  bool      ignore)
{
  if (!ignore && (!zui->getDbFlag("tile/bbox")))
    return 0;

  Ath__searchBox bb(zui->getBbox(), 0, 0);

  Ath__array1D<uint>* idtable = zui->getIdTable();

  idtable->resetCnt();
  _tileSearchGrid->search(&bb, idtable);  // single grid

  for (uint ii = 0; ii < idtable->getCnt(); ii++) {
    uint gridId = idtable->get(ii);
    uint row    = gridId / _colCnt;
    uint col    = gridId % _colCnt;

    Ath__gridTile* gridTile = _tileTable[gridId];

    int x1, y1, x2, y2;
    gridTile->getBounds(&x1, &y1, &x2, &y2);

    zui->addBox(gridId,
                Ath_box__bbox,
                Ath_hier__tile,
                layer,
                x1 + w,
                y1 + w,
                x2 - w,
                y2 - w);
  }
  return idtable->getCnt();
}
uint Ath__quadTable::getTileBuses_1(Ath__zui* zui)
{
  if (!((_placePins_next > 0) && (_placePins_thru > 0))) {
    for (uint ii = 0; ii < _tileCnt; ii++) {
      // uint gridId= idtable->get(ii);
      uint gridId = ii;
      uint row    = gridId / _colCnt;
      uint col    = gridId % _colCnt;

      startQpinIterator(row, col);
      while (Ath__qPin* pin = getNextSrcPin_thru()) {
        pin->getSrcPin()->makeBusZuiObject(zui, 1000);
      }
    }
    return 1;
  }
  Ath__array1D<uint> wireIdTable(16000);

  Ath__array1D<uint> tileIdTable(32);
  Ath__searchBox     bb(zui->getBbox(), 0, 0);

  _tileSearchGrid->search(&bb, &tileIdTable);  // single grid

  for (uint ii = 0; ii < tileIdTable.getCnt(); ii++) {
    uint           gridId = tileIdTable.get(ii);
    Ath__gridTile* tile   = _tileTable[gridId];

    wireIdTable.resetCnt();
    tile->searchWires(&bb, &wireIdTable);

    for (uint jj = 0; jj < wireIdTable.getCnt(); jj++) {
      uint       wireId = wireIdTable.get(jj);
      Ath__wire* wire   = getWire(wireId);
      if (!wire->isTileBus())
        continue;

      Ath__qPin* pin     = _qBoxPool->get(wire->getBoxId());
      Ath__box*  busList = pin->getBusList();
      zui->makeZuiObjects_1(busList, Ath_hier__tile, Ath_box__bus);
    }
  }
  return 1;
}
uint Ath__quadTable::getTilePins_unplaced(Ath__zui*           zui,
                                          Ath__array1D<uint>* idtable,
                                          Ath__qPin* (*func)())
{
  uint cnt = 0;
  for (uint ii = 0; ii < idtable->getCnt(); ii++) {
    uint gridId = idtable->get(ii);
    uint row    = gridId / _colCnt;
    uint col    = gridId % _colCnt;

    startQpinIterator(row, col);
    while (Ath__qPin* pin = (*func)()) {
      for (Ath__qPin* q = pin; q != NULL; q = q->_next) {
        q->makeZuiObject(zui, 0);
        cnt++;
      }
    }
  }
  return cnt;
}
uint Ath__quadTable::getTilePins_unplaced_corner(Ath__zui*           zui,
                                                 Ath__array1D<uint>* idtable)
{
  uint cnt = 0;
  for (uint ii = 0; ii < idtable->getCnt(); ii++) {
    uint gridId = idtable->get(ii);
    uint row    = gridId / _colCnt;
    uint col    = gridId % _colCnt;

    startQpinIterator(row, col);
    while (Ath__qPin* pin = getNextSrcPin_corner()) {
      for (Ath__qPin* q = pin; q != NULL; q = q->_next) {
        q->makeZuiObject(zui, 0);
        cnt++;
      }
    }
  }
  return cnt;
}
uint Ath__quadTable::getTilePins_unplaced(Ath__zui*           zui,
                                          Ath__array1D<uint>* idtable)
{
  uint cnt = 0;
  for (uint ii = 0; ii < idtable->getCnt(); ii++) {
    uint gridId = idtable->get(ii);
    uint row    = gridId / _colCnt;
    uint col    = gridId % _colCnt;

    startQpinIterator(row, col);
    while (Ath__qPin* pin = getNextSrcPin_all()) {
      for (Ath__qPin* q = pin; q != NULL; q = q->_next) {
        q->makeZuiObject(zui, 0);
        cnt++;
      }
    }
  }
  return cnt;
}
uint Ath__quadTable::getTilePins_placed(Ath__searchBox*     bb,
                                        Ath__zui*           zui,
                                        Ath__array1D<uint>* tileIdTable)
{
  Ath__array1D<uint> wireIdTable(64000);
  uint               cnt = 0;
  for (uint ii = 0; ii < tileIdTable->getCnt(); ii++) {
    uint           gridId = tileIdTable->get(ii);
    Ath__gridTile* tile   = _tileTable[gridId];

    wireIdTable.resetCnt();
    tile->searchWires(bb, &wireIdTable);

    for (uint jj = 0; jj < wireIdTable.getCnt(); jj++) {
      uint       wireId = wireIdTable.get(jj);
      Ath__wire* wire   = getWire(wireId);
      if (!wire->isTilePin())
        continue;

      Ath__qPin* pin = _qBoxPool->get(wire->getBoxId());
      pin->makeZuiObject(zui, 0, true);
    }
  }
  return cnt;
}
uint Ath__quadTable::getTilePins_1(Ath__zui* zui)
{
  Ath__searchBox bb(zui->getBbox(), 0, 0);

  Ath__array1D<uint>* idtable = zui->getIdTable();

  idtable->resetCnt();
  _tileSearchGrid->search(&bb, idtable);  // single grid

  if (!(_placePins_corner > 0))
    getTilePins_unplaced_corner(zui, idtable);

  if (!((_placePins_next > 0) && (_placePins_thru > 0)))
    return getTilePins_unplaced(zui, idtable);

  return getTilePins_placed(&bb, zui, idtable);
}
Ath__box* Ath__quadTable::newPinBox(Ath__searchBox* bb, uint ownId)
{
  uint      n;
  Ath__box* a = _pinBoxPool->alloc(NULL, &n);
  a->set(bb->loXY(0), bb->loXY(1), bb->hiXY(0), bb->hiXY(1));
  a->_id    = n;
  a->_layer = bb->getLevel();
  if (ownId > 0)
    a->setOwner(ownId);

  return a;
}
Ath__box* Ath__quadTable::addBusBox(int       x1,
                                    int       y1,
                                    int       x2,
                                    int       y2,
                                    uint      layer,
                                    uint      netId,
                                    Ath__box* next)
{
  uint      n;
  Ath__box* obs = _pinBoxPool->alloc(NULL, &n);
  obs->set(x1, y1, x2, y2);
  obs->_id    = n;
  obs->_layer = layer;
  obs->setIdentity(Ath_box__bus, netId, Ath_hier__tile);

  obs->_next = next;

  return obs;
}

uint Ath__quadTable::placePins_next(uint row, uint col)
{
  uint cnt = 0;
  startQpinIterator(row, col);
  while (Ath__qPin* srcPin = getNextSrcPin_next()) {
    Ath__searchBox srcBB;
    srcPin->getTargetBox(&srcBB);

    Ath__qPin*     dstPin = srcPin->_next;
    Ath__searchBox dstBB;
    dstPin->getTargetBox(&dstBB);

    Ath__grid* srcGrid = _busTable[row][col]->getGrid(srcBB.getLevel());

    uint       dir     = srcGrid->getDir();
    Ath__grid* dstGrid = NULL;
    if (dir > 0)  // horizontal
      dstGrid = _busTable[row][col + 1]->getGrid(dstBB.getLevel());
    else
      dstGrid = _busTable[row + 1][col]->getGrid(dstBB.getLevel());

    uint srcTrackNum
        = srcGrid->placeTileWire(1, srcPin->_nameId, &srcBB, 1, -1, false);
    uint dstTrackNum = dstGrid->placeTileWire(
        1, dstPin->_nameId, &dstBB, 0, srcTrackNum, false);

    if (srcTrackNum != dstTrackNum) {
      srcBB.printCoords(stdout, "Cannot place src pin ");
      dstBB.printCoords(stdout, "Cannot place dst pin ");
      printf(" netId %d\n", srcPin->_netId);
      continue;
      // TODO: have to remove ant placed pins!
    }
    srcPin->_portBB = newPinBox(&srcBB, srcPin->_nameId);
    dstPin->_portBB = newPinBox(&dstBB, dstPin->_nameId);

    cnt++;
  }

  return cnt;
}
int Ath__quadTable::placePinOnGrid(Ath__qPin*      pin,
                                   Ath__searchBox* pinBB,
                                   uint            row,
                                   uint            col,
                                   int             initTrack)
{
  pin->getTargetBox(pinBB);

  Ath__grid* srcGrid = _busTable[row][col]->getGrid(pinBB->getLevel());

  int trackNum
      = srcGrid->placeTileWire(1, pin->_nameId, pinBB, -1, initTrack, false);
  if (trackNum < 0) {
    pinBB->printCoords(stdout, "Cannot place src pin ");
    printf(" netId %d\n", pin->_netId);
    return -1;
  }
  return trackNum;
}
int Ath__quadTable::placeObsOnGrid(Ath__qPin*      pin,
                                   Ath__searchBox* obsBB,
                                   uint            row,
                                   uint            col,
                                   int             initTrack)
{
  Ath__grid* obsGrid = _busTable[row][col]->getGrid(obsBB->getLevel());

  int trackNum
      = obsGrid->placeTileWire(2, pin->_nameId, obsBB, -1, initTrack, false);

  if ((initTrack >= 0) && (initTrack != trackNum)) {
    obsBB->printCoords(stdout, "Cannot place bus ");
    printf(" netId %d\n", pin->_netId);
    return -1;  // TODO: have to remove any placed pins!
  }
  pin->addBusBox(newPinBox(obsBB, pin->_nameId));

  return trackNum;
}
int Ath__quadTable::placeObsOnGrids(Ath__qPin* pin,
                                    uint       row,
                                    uint       col,
                                    int        initTrack)
{
  // consecutive grids for straight feed thrus

  uint initTrackNum = initTrack;

  int  pinHeight;
  uint srcRow = row;
  uint srcCol = col;
  int  dir;
  int  turnDist = pin->getTurnDist(&srcRow, &srcCol, &dir);

  for (uint d = 1; d < turnDist; d++) {
    if (dir > 0)  // horizontal
      srcCol++;
    else
      srcRow++;

    Ath__searchBox connBB;
    pin->getObsBox(&connBB);  // it is adjusted every loop

    int trackNum = placeObsOnGrid(pin, &connBB, srcRow, srcCol, initTrackNum);

    if (trackNum < 0)
      return -1;

    initTrackNum = trackNum;
    pinHeight    = connBB.loXY(dir);
  }
  // Place the turning point

  Ath__searchBox turnBB;
  pin->getNextObsBox(&turnBB);  // it is adjusted every loop

  if (turnDist > 1)
    turnBB.setLo(dir, pinHeight);

  uint turnRow, turnCol;
  pin->getTurnRowCol(&turnRow, &turnCol);

  int n = placeObsOnGrid(pin, &turnBB, turnRow, turnCol, initTrackNum);

  if (n < 0) {
    turnBB.printCoords(stdout, "Cannot place src pin ");
    printf(" ObsOnGrids netId %d\n", pin->_netId);
    return -1;
    // TODO: have to remove any placed pins!
  }
  return n;
}
int Ath__quadTable::preplacePin_corner(Ath__qPin* pin, uint row, uint col)
{
  uint maxRange = 30;

  Ath__searchBox pinBB;
  pin->getTargetBox(&pinBB);
  uint       level   = pinBB.getLevel();
  Ath__grid* srcGrid = _busTable[row][col]->getGrid(level);
  Ath__grid* obsGrid;
  int        dir;
  uint       srcRow   = row;
  uint       srcCol   = col;
  int        turnDist = pin->getTurnDist(&srcRow, &srcCol, &dir);
  // dir == srcGrid->_dir

  Ath__searchBox turnBB;
  pin->getNextObsBox(&turnBB);
  uint turnRow, turnCol;
  pin->getTurnRowCol(&turnRow, &turnCol);

  int         d, ii, max_ii = 2 * maxRange;
  int         initTrackNum = srcGrid->getMinMaxTrackNum(pinBB.loXY(dir));
  int         trackNum;
  Ath__track* track;
  int         loMarker;
  for (ii = 1; ii < max_ii; ii++) {
    trackNum = initTrackNum + ((ii % 2) ? ii / 2 : -ii / 2);
    if (trackNum < 0)
      continue;

    track = srcGrid->getTrack(trackNum, srcGrid->_markerCnt);
    if (!track)
      continue;
    loMarker = srcGrid->getBucketNum(pinBB.loXY(!dir, srcGrid->_start));
    if (!track->checkMarker(loMarker))
      continue;

    for (d = 1; d < turnDist; d++) {
      if (dir > 0)
        obsGrid = _busTable[row][srcCol + d]->getGrid(level);
      else
        obsGrid = _busTable[srcRow + d][col]->getGrid(level);
      track = obsGrid->getTrack(trackNum, obsGrid->_markerCnt);
      if (!track)
        break;
      if (!track->checkMarker(0))
        break;
    }
    if (d < turnDist)
      continue;

    obsGrid = _busTable[turnRow][turnCol]->getGrid(level);
    track   = obsGrid->getTrack(trackNum, obsGrid->_markerCnt);
    if (!track)
      continue;
    loMarker = obsGrid->getBucketNum(turnBB.loXY(!dir, obsGrid->_start));
    if (track->checkMarker(loMarker))
      return trackNum;
  }
  return -1;
}
uint Ath__quadTable::placePins_corner(uint row, uint col, uint dist)
{
  uint cnt = 0;
  startQpinIterator(row, col);
  while (Ath__qPin* srcPin = getNextSrcPin_corner()) {
    if (!srcPin->isTargeted())
      continue;
    if (srcPin->isPlaced())
      continue;

    if (srcPin->_conn.getMaxDist() != dist)
      continue;

#if 1
    int initTrackNum = preplacePin_corner(srcPin, row, col);
#else
    int initTrackNum = -1;
#endif

    int trackNum = placeObsOnGrids(srcPin, row, col, initTrackNum);
    if (trackNum < 0)
      continue;

    Ath__searchBox srcBB;
    if (placePinOnGrid(srcPin, &srcBB, row, col, trackNum) < 0)
      continue;

    ////////////////////////////////////////////////////////////

    Ath__qPin* dstPin = srcPin->_next;
    uint       toCol;
    uint       toRow = dstPin->getToRowCol(&toCol);

#if 1
    initTrackNum = preplacePin_corner(dstPin, toRow, toCol);
#else
    initTrackNum     = -1;
#endif

    trackNum = placeObsOnGrids(dstPin, toRow, toCol, initTrackNum);
    if (trackNum < 0)
      continue;

    Ath__searchBox dstBB;
    if (placePinOnGrid(dstPin, &dstBB, toRow, toCol, trackNum) < 0)
      continue;

    srcPin->setPlaced();
    dstPin->setPlaced();

    srcPin->_portBB = newPinBox(&srcBB, srcPin->_nameId);
    dstPin->_portBB = newPinBox(&dstBB, dstPin->_nameId);

    cnt++;
  }
  return cnt;
}
int Ath__quadTable::preplacePin_thru(Ath__qPin* srcPin,
                                     uint       row,
                                     uint       col,
                                     uint       level)
{
  uint maxRange = 30;

  Ath__grid* srcGrid = _busTable[row][col]->getGrid(level);
  Ath__grid *dstGrid, *obsGrid;
  uint       dir  = srcGrid->getDir();
  uint       dist = srcPin->_conn.getDist();
  if (dir > 0)
    dstGrid = _busTable[row][col + dist]->getGrid(level);
  else
    dstGrid = _busTable[row + dist][col]->getGrid(level);
  Ath__searchBox srcBB, dstBB, connBB;
  srcPin->getTargetBox(&srcBB);
  Ath__qPin* dstPin = srcPin->_next;
  dstPin->getTargetBox(&dstBB);
  int d, ii, max_ii = 2 * maxRange;
  int initTrackNum = srcGrid->getMinMaxTrackNum(srcBB.loXY(dir));
  int trackNum;
  for (ii = 1; ii < max_ii; ii++) {
    trackNum = initTrackNum + ((ii % 2) ? ii / 2 : -ii / 2);
    if (trackNum < 0)
      continue;
    if (!srcGrid->getTrack(trackNum, srcGrid->_markerCnt))
      continue;
    if (!dstGrid->getTrack(trackNum, dstGrid->_markerCnt))
      continue;
    for (d = 1; d < dist; d++) {
      if (dir > 0)
        obsGrid = _busTable[row][col + d]->getGrid(level);
      else
        obsGrid = _busTable[row + d][col]->getGrid(level);
      Ath__track* track = obsGrid->getTrack(trackNum, obsGrid->_markerCnt);
      if (!track)
        break;
      if (!track->checkMarker(0))
        break;
    }
    if (d == dist)
      break;
  }
  if (ii == max_ii)
    return -1;
  return trackNum;
}
uint Ath__quadTable::placePins_thru(uint row, uint col)
{
  uint cnt = 0;
  startQpinIterator(row, col);
  while (Ath__qPin* srcPin = getNextSrcPin_thru()) {
    Ath__searchBox srcBB;
    srcPin->getTargetBox(&srcBB);

    Ath__searchBox connBB;

    Ath__qPin*     dstPin = srcPin->_next;
    Ath__searchBox dstBB;
    dstPin->getTargetBox(&dstBB);

    Ath__grid* srcGrid = _busTable[row][col]->getGrid(srcBB.getLevel());

#if 1
    int initTrackNum = preplacePin_thru(srcPin, row, col, srcBB.getLevel());
#else
    int initTrackNum = -1;
#endif
    int actualTrackNum = -1;
    for (uint d = 1; d < srcPin->_conn.getDist(); d++) {
      Ath__grid* obsGrid = NULL;
      if (srcGrid->getDir() > 0)  // horizontal
        obsGrid = _busTable[row][col + d]->getGrid(dstBB.getLevel());
      else
        obsGrid = _busTable[row + d][col]->getGrid(dstBB.getLevel());

      srcPin->getObsBox(&connBB);  // it is adjusted every loop

      actualTrackNum = obsGrid->placeTileWire(
          2, srcPin->_nameId, &connBB, 0, initTrackNum, false);

      if (actualTrackNum < 0
          || (initTrackNum >= 0) && (initTrackNum != actualTrackNum)) {
        connBB.printCoords(stdout, "Cannot place bus ");
        printf(" netId %d\n", srcPin->_netId);
        continue;
        // TODO: have to remove ant placed pins!
      }

      srcPin->addBusBox(newPinBox(&connBB, srcPin->_nameId));

      initTrackNum = actualTrackNum;
    }
    int srcTrackNum = srcGrid->placeTileWire(
        1, srcPin->_nameId, &srcBB, 1, initTrackNum, false);

    uint dist = srcPin->_conn.getDist();

    Ath__grid* dstGrid = NULL;
    if (srcGrid->getDir() > 0)  // horizontal
      dstGrid = _busTable[row][col + dist]->getGrid(dstBB.getLevel());
    else
      dstGrid = _busTable[row + dist][col]->getGrid(dstBB.getLevel());

    int dstTrackNum = dstGrid->placeTileWire(
        1, dstPin->_nameId, &dstBB, 0, initTrackNum, false);

    if (srcTrackNum != dstTrackNum) {
      srcBB.printCoords(stdout, "Cannot place src pin ");
      dstBB.printCoords(stdout, "Cannot place dst pin ");
      printf(" netId %d\n", srcPin->_netId);
      continue;
      // TODO: have to remove any placed pins!
    }
    cnt++;

    srcPin->_portBB = newPinBox(&srcBB, srcPin->_nameId);
    dstPin->_portBB = newPinBox(&dstBB, dstPin->_nameId);
  }
  return cnt;
}
uint Ath__quadTable::makeTopBuses_1(uint ii,
                                    uint jj,
                                    uint opt,
                                    uint width,
                                    uint space)
{
  assert((ii >= 0) && (ii < _rowCnt) && (jj >= 0) && (jj < _colCnt));
  placePins_next(ii, jj);
  placePins_thru(ii, jj);

  placePins_corner(ii, jj, 1);

  uint dist = ath__max(_rowCnt, _colCnt);

  for (; dist > 1; dist++)
    placePins_corner(ii, jj, dist);

  return 1;

  Ath__qCounter* q = _quad[ii][jj];

  placeShapes(ii, jj, 0, _qPinTable[ii][jj]->getHeadObsShape(0), width, space);
  placeShapes(ii, jj, 1, _qPinTable[ii][jj]->getHeadObsShape(1), width, space);

  for (uint type = 0; type < 4; type++) {
    if (!_qShapeTable[ii][jj]->isPlacedCorner(type))  // TODO
      placeCornerObsShapes(ii, jj, type, width, space);
  }
  Ath__gridTile*** _busTable;
  for (uint dir = 0; dir < 2; dir++) {
    uint d = (dir > 0) ? 0 : 1;                            // TODO
    if (_qShapeTable[ii][jj]->getNextGrid(d)->isPlaced())  // TODO
      continue;
    uint cnt = getShapesSorted(dir, _qPinTable[ii][jj]->getHeadNextPinShape(d));
    if (cnt <= 0)
      continue;

    uint trackWidth = _qShapeTable[ii][jj]->placeSortedNext(
        dir, _tmpBoxArray, _tmpIntArray, cnt, width, space);

    // TODO DELETE		createPlacedPins(dir, cnt, _tmpBoxArray,
    // _tmpIntArray, trackWidth);
  }
  return 1;
}
uint Ath__quadTable::makeTopBuses_corner(uint opt, uint space, uint width)
{
  _placePins_corner = 1;

  uint cnt = 0;
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      placePins_corner(ii, jj, 1);
    }
  }
  uint dist = ath__max(_rowCnt, _colCnt);
  for (; dist > 1; dist--) {
    for (uint ii = 0; ii < _rowCnt; ii++)
      for (uint jj = 0; jj < _colCnt; jj++)

        placePins_corner(ii, jj, dist);
  }
  return cnt;
}
uint Ath__quadTable::makeTopBuses_thru(uint opt, uint space, uint width)
{
  _placePins_thru = 1;

  uint cnt = 0;
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      placePins_thru(ii, jj);
    }
  }
  return cnt;
}
uint Ath__quadTable::makeTopBuses_next(uint opt, uint space, uint width)
{
  _placePins_next = 1;

  uint cnt = 0;
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      placePins_next(ii, jj);
    }
  }
  return cnt;
}
uint Ath__quadTable::makeTopBuses_1(uint opt, uint space, uint width)
{
  uint cnt = 0;
  cnt += makeTopBuses_next(opt, space, width);
  cnt += makeTopBuses_thru(opt, space, width);
  cnt += makeTopBuses_corner(opt, space, width);

  return cnt;
}
