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

void Ath__quadTable::createEdgePins(Ath__qPin* src,
                                    Ath__qPin* dst,
                                    uint       row,
                                    uint       col,
                                    uint       dir,
                                    int        layer,
                                    Ath__box*  bb,
                                    int        length)
{
  int x1 = bb->_xlo;
  int y1 = bb->_ylo;
  int x2 = bb->_xhi;
  int y2 = bb->_yhi;

  uint netId = src->_netId;

  int visualGap = 50;
  if (dir > 0) {
    int yBound = _rowDivider->getNext(row);

    src->_targetBB = _qPinTable[row][col]->addNextPinBox(netId,
                                                         dir,
                                                         layer,
                                                         x1,
                                                         yBound - length,
                                                         x2 + length / 2,
                                                         yBound - visualGap);

    dst->_targetBB = _qPinTable[row + 1][col]->addNextPinBox(netId,
                                                             dir,
                                                             layer,
                                                             x1,
                                                             yBound + visualGap,
                                                             x2 + length / 2,
                                                             yBound + length);

    //		_qShapeTable[row][col]->addWire(pinLow, 10);
  } else {
    int xBound = _colDivider->getNext(col);

    src->_targetBB = _qPinTable[row][col]->addNextPinBox(netId,
                                                         dir,
                                                         layer,
                                                         xBound - length,
                                                         y1,
                                                         xBound - visualGap,
                                                         y2 + length / 2);

    dst->_targetBB = _qPinTable[row][col + 1]->addNextPinBox(netId,
                                                             dir,
                                                             layer,
                                                             xBound + visualGap,
                                                             y1,
                                                             xBound + length,
                                                             y2 + length / 2);
  }
}
void Ath__quadTable::cornerPinPlus(uint netId,
                                   uint row,
                                   uint col,
                                   uint type,
                                   uint dir,
                                   int  x1,
                                   int  y1,
                                   int  x2,
                                   int  y2)
{
  Ath__qPinTable* pins  = _qPinTable[row][col];
  Ath__qPinTable* pins2 = _qPinTable[row + 1][col + 1];

  int yBound    = _rowDivider->getNext(row);
  int YplusGap  = yBound + _pinVisualGap;
  int YminusGap = yBound - _pinVisualGap;
  int YplusLen  = yBound + _pinLength;
  int YminusLen = yBound - _pinLength;

  int xBound    = _colDivider->getNext(col);
  int XplusGap  = xBound + _pinVisualGap;
  int XminusGap = xBound - _pinVisualGap;
  int XplusLen  = xBound + _pinLength;
  int XminusLen = xBound - _pinLength;

  if (dir > 0) {
    Ath__qPinTable* thru = _qPinTable[row + 1][col];

    thru->addCornerObsBox(
        netId, type, _topMetV, x1, YplusGap, x1 + _pinWidth, y2);
    thru->addCornerObsBox(
        netId, type, _topMetH, x1, y2, XplusGap, y2 + _pinWidth);

    if (_pinStyle == 0) {  // edge
      pins->addCornerPinBox(
          netId, type, _topMetV, x1, YminusLen, x1 + _pinWidth, YminusGap);
      pins2->addCornerPinBox(
          netId, type, _topMetH, XplusGap, y2, XplusLen, y2 + _pinWidth);
    }
    if (_pinStyle == 1) {  // area
      pins->addCornerPinBox(
          netId, type, _topMetV, x1, y1, x1 + _pinWidth, YminusGap);
      pins2->addCornerPinBox(
          netId, type, _topMetH, XplusGap, y2, x2, y2 + _pinWidth);
    }
  } else {
    Ath__qPinTable* thru = _qPinTable[row][col + 1];

    thru->addCornerObsBox(
        netId, type, _topMetV, x2, y1, x2 + _pinWidth, YminusGap);
    thru->addCornerObsBox(
        netId, type, _topMetH, XplusGap, y1, x2, y1 + _pinWidth);

    if (_pinStyle == 0) {  // edge
      pins->addCornerPinBox(
          netId, type, _topMetV, x1, y1, XminusGap, y1 + _pinWidth);
      pins2->addCornerPinBox(
          netId, type, _topMetH, x2, YplusGap, x2 + _pinWidth, y2);
    }
    if (_pinStyle == 1) {  // area
      pins->addCornerPinBox(
          netId, type, _topMetV, x1, y1, XminusGap, y1 + _pinWidth);
      pins2->addCornerPinBox(
          netId, type, _topMetH, x2, YplusGap, x2 + _pinWidth, y2);
    }
  }
}

void Ath__quadTable::cornerPinMinus(uint netId,
                                    uint row,
                                    uint col,
                                    uint type,
                                    uint dir,
                                    int  x1,
                                    int  y1,
                                    int  x2,
                                    int  y2)
{
  Ath__qPinTable* pins1 = _qPinTable[row][col];
  Ath__qPinTable* pins2 = _qPinTable[row + 1][col - 1];

  int yBound    = _rowDivider->getNext(row);
  int YplusGap  = yBound + _pinVisualGap;
  int YminusGap = yBound - _pinVisualGap;
  int YplusLen  = yBound + _pinLength;
  int YminusLen = yBound - _pinLength;

  int xBound    = _colDivider->getNext(col - 1);
  int XplusGap  = xBound + _pinVisualGap;
  int XminusGap = xBound - _pinVisualGap;
  int XplusLen  = xBound + _pinLength;
  int XminusLen = xBound - _pinLength;

  // (x1,y1) down to (x2,y2)
  if (dir == 0) {
    Ath__qPinTable* thru = _qPinTable[row][col - 1];

    thru->addCornerObsBox(
        netId, type, _topMetV, x1, y2, x1 + _pinWidth, YminusGap);
    thru->addCornerObsBox(
        netId, type, _topMetH, x1, y2, XminusGap, y2 + _pinWidth);

    if (_pinStyle == 0) {  // edge
      pins1->addCornerPinBox(
          netId, type, _topMetV, x1, YplusGap, x1 + _pinWidth, YplusLen);
      pins2->addCornerPinBox(
          netId, type, _topMetH, XplusGap, y2, XplusLen, y2 + _pinWidth);
    }
    if (_pinStyle == 1) {  // area
      pins1->addCornerPinBox(
          netId, type, _topMetV, x1, YplusGap, x1 + _pinWidth, y1);
      pins2->addCornerPinBox(
          netId, type, _topMetH, XplusGap, y2, x2, y2 + _pinWidth);
    }
  } else {  // up, left or right,down
    Ath__qPinTable* thru = _qPinTable[row + 1][col];

    thru->addCornerObsBox(
        netId, type, _topMetV, x2, YplusGap, x2 + _pinWidth, y1);
    thru->addCornerObsBox(
        netId, type, _topMetH, XplusGap, y1, x2, y1 + _pinWidth);

    if (_pinStyle == 0) {  // edge
      pins2->addCornerPinBox(
          netId, type, _topMetH, XminusLen, y1, XminusGap, y1 + _pinWidth);
      pins1->addCornerPinBox(
          netId, type, _topMetV, x2, YminusLen, x2 + _pinWidth, YminusGap);
    }
    if (_pinStyle == 1) {  // area
      pins2->addCornerPinBox(
          netId, type, _topMetH, x1, y1, XminusGap, y1 + _pinWidth);
      pins1->addCornerPinBox(
          netId, type, _topMetV, x2, y2, x2 + _pinWidth, YplusGap);
    }
  }
}
uint Ath__quadTable::makeCornerPinShapes(uint ii, uint jj)
{
  uint cnt = 0;

  startQpinIterator(ii, jj);
  while (Ath__qPin* srcPin = getNextSrcPin_corner()) {
    Ath__qPin* dstPin = srcPin->_next;

    cnt++;

    uint col = jj;
    uint row = ii;

    /*
    int x1= srcPin->_x;
    int x2= dstPin->_x;
    int y1= srcPin->_y;
    int y2= dstPin->_y;
    */

    int x1 = srcPin->getInstBox()->getMidX();
    int x2 = dstPin->getInstBox()->getMidX();
    int y1 = srcPin->getInstBox()->getMidY();
    int y2 = dstPin->getInstBox()->getMidY();

    Ath__p2pConn* p2p = srcPin->_head;
    uint          tmp1;  // TODO
    uint          cornerType = p2p->_conn.getToRowCol(&tmp1);

    uint midCol;
    uint midRow = p2p->_conn.getFromRowCol(&midCol);
    uint dir    = 0;  // horizontal
    if (midRow - row > 0) {
      dir = 1;
    }

    if (srcPin->_conn.getColDist() > 0) {  // types
      cornerPinPlus(srcPin->_nameId, row, col, cornerType, dir, x1, y1, x2, y2);
    } else {
      cornerPinMinus(
          srcPin->_nameId, row, col, cornerType, dir, x2, y2, x1, y1);
    }
  }
  return cnt;
}
void Ath__quadTable::jumperPins(uint netId,
                                uint row1,
                                uint col1,
                                uint row2,
                                uint col2,
                                uint dir,
                                int  x1,
                                int  y1,
                                int  x2,
                                int  y2)
{
  Ath__qPinTable* pins1 = _qPinTable[row1][col1];
  Ath__qPinTable* pins2 = _qPinTable[row2][col2];

  if (dir > 0) {
    int y1Bound     = _rowDivider->getNext(row1);
    int Y1_minusGap = y1Bound - _pinVisualGap;
    int Y1_minusLen = y1Bound - _pinLength;

    int y2Bound    = _rowDivider->get(row2);
    int Y2_plusGap = y2Bound + _pinVisualGap;
    int Y2_plusLen = y2Bound + _pinLength;

    x1 = (x1 + x2) / 2;
    x2 = x1 + _pinWidth;

    for (uint row = row1 + 1; row < row2; row++) {
      _qPinTable[row][col1]->addThruObsBox(
          netId,
          0,
          _topMetV,
          x1,
          _rowDivider->get(row) + 5 * _pinVisualGap,
          x2,
          _rowDivider->getNext(row) - 5 * _pinVisualGap);
    }

    if (_pinStyle == 1) {  // edge
      pins1->addStraightPinBox(
          netId, 1, _topMetV, x1, Y1_minusLen, x2, Y1_minusGap);
      pins2->addStraightPinBox(
          netId, 1, _topMetV, x1, Y2_plusGap, x2, Y2_plusLen);
    }
    if (_pinStyle == 0) {  // area
      pins1->addStraightPinBox(netId, 1, _topMetV, x1, y1, x2, Y1_minusGap);
      pins2->addStraightPinBox(netId, 1, _topMetV, x1, Y2_plusGap, x2, y2);
    }
  } else {
    int x1Bound     = _colDivider->getNext(col1);
    int X1_minusGap = x1Bound - _pinVisualGap;
    int X1_minusLen = x1Bound - _pinLength;

    int x2Bound    = _colDivider->get(col2);
    int X2_plusGap = x2Bound + _pinVisualGap;
    int X2_plusLen = x2Bound + _pinLength;

    y1 = (y1 + y2) / 2;
    y2 = y1 + _pinWidth;

    for (uint col = col1 + 1; col < col2; col++) {
      _qPinTable[row1][col]->addThruObsBox(
          netId,
          1,
          _topMetH,
          _colDivider->get(col) + 5 * _pinVisualGap,
          y1,
          _colDivider->getNext(col) - 5 * _pinVisualGap,
          y2);
    }
    if (_pinStyle == 1) {  // edge
      pins1->addStraightPinBox(
          netId, 0, _topMetH, X1_minusLen, y1, X1_minusGap, y2);
      pins2->addStraightPinBox(
          netId, 0, _topMetH, X2_plusGap, y1, X2_plusLen, y2);
    }
    if (_pinStyle == 0) {  // area
      pins1->addStraightPinBox(netId, 0, _topMetH, x1, y1, X1_minusGap, y2);
      pins2->addStraightPinBox(netId, 0, _topMetH, X2_plusGap, y1, x2, y2);
    }
  }
}
uint Ath__quadTable::getTilePins(Ath__zui* zui)
{
  uint cnt = 0;
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      startQpinIterator(ii, jj);
      while (Ath__qPin* pin = getNextSrcPin_all()) {
        for (Ath__qPin* q = pin; q != NULL; q = q->_next) {
          q->makeZuiObject(zui, 0);
          cnt++;
        }
      }
    }
  }
  return cnt;
}

/* TODO the placeShapes, sorted
uint Ath__quadTable::makeTopBuses_1(uint ii, uint jj, uint opt, uint width, uint
space)
{
        assert((ii>=0)&&(ii<_rowCnt)&&(jj>=0)&&(jj<_colCnt));
        placePins_next(ii, jj);
        placePins_thru(ii, jj);

        placePins_corner(ii, jj, 1);

        uint dist= ath__max(_rowCnt, _colCnt);

        for (; dist>1; dist++)
                placePins_corner(ii, jj, dist);

        return 1;
        
        Ath__qCounter *q= _quad[ii][jj];

        placeShapes(ii, jj, 0, _qPinTable[ii][jj]->getHeadObsShape(0), width,
space); placeShapes(ii, jj, 1, _qPinTable[ii][jj]->getHeadObsShape(1), width,
space);

        for (uint type= 0; type<4; type++) {
                if ( !_qShapeTable[ii][jj]->isPlacedCorner(type)) //TODO
                        placeCornerObsShapes(ii, jj, type, width, space);
        }
        Ath__gridTile ***_busTable;
        for (uint dir= 0; dir<2; dir++) {
                uint d= (dir>0) ? 0 : 1; // TODO
                if (_qShapeTable[ii][jj]->getNextGrid(d)->isPlaced()) //TODO
                        continue;
                uint cnt= getShapesSorted(dir,
_qPinTable[ii][jj]->getHeadNextPinShape(d)); if (cnt<=0) continue;
                
                uint trackWidth= _qShapeTable[ii][jj]->placeSortedNext(
                        dir, _tmpBoxArray, _tmpIntArray, cnt, width, space);
                
//TODO DELETE		createPlacedPins(dir, cnt, _tmpBoxArray, _tmpIntArray,
trackWidth);
        }
        return 1;
}
*/
uint Ath__quadTable::makeTopBuses(uint ii,
                                  uint jj,
                                  uint opt,
                                  uint width,
                                  uint space)
{
  assert((ii >= 0) && (ii < _rowCnt) && (jj >= 0) && (jj < _colCnt));

  Ath__qCounter* q = _quad[ii][jj];

  placeShapes(ii, jj, 0, _qPinTable[ii][jj]->getHeadObsShape(0), width, space);
  placeShapes(ii, jj, 1, _qPinTable[ii][jj]->getHeadObsShape(1), width, space);

  for (uint type = 0; type < 4; type++) {
    if (!_qShapeTable[ii][jj]->isPlacedCorner(type))  // TODO
      placeCornerObsShapes(ii, jj, type, width, space);
  }
  for (uint dir = 0; dir < 2; dir++) {
    uint d = (dir > 0) ? 0 : 1;                            // TODO
    if (_qShapeTable[ii][jj]->getNextGrid(d)->isPlaced())  // TODO
      continue;
    uint cnt = getShapesSorted(dir, _qPinTable[ii][jj]->getHeadNextPinShape(d));
    if (cnt <= 0)
      continue;

    uint trackWidth = _qShapeTable[ii][jj]->placeSortedNext(
        dir, _tmpBoxArray, _tmpIntArray, cnt, width, space);

    // TODO createPlacedPins(dir, cnt, _tmpBoxArray, _tmpIntArray, trackWidth);
  }
  return 1;
}
uint Ath__quadTable::getShapesSorted(uint dir, Ath__box* head)
{
  uint cnt = 0;
  for (Ath__box* e = head; e != NULL; e = e->_next) {
    _tmpBoxArray[cnt++] = e;
  }
  if (cnt <= 0)
    return 0;

  if (dir > 0)
    sortY(_tmpBoxArray, cnt);
  else
    sortX(_tmpBoxArray, cnt);

  return cnt;
}
uint Ath__quadTable::placeShapes(uint      row,
                                 uint      col,
                                 uint      dir,
                                 Ath__box* head,
                                 uint      width,
                                 uint      space)
{
  uint cnt = 0;
  if (!_qShapeTable[row][col]->isThruPlaced(dir)) {
    cnt = getShapesSorted(dir, head);
    _qShapeTable[row][col]->placeSortedThru(
        dir, _tmpBoxArray, cnt, width, space);
  } else {
    Ath__grid*     g = _qShapeTable[row][col]->getThruGrid(dir);
    Ath__qCounter* q = _quad[row][col];
    if (q->_thru[dir] > 0) {
      // space= g->_pitch-g->_width;
      uint slack = q->_tracks[dir] / 2 - q->_thru[dir];
      if (slack < 20)
        return 0;
      /*
                              else if (slack<50)
                                      space= space + space/4;
                              else if (slack<100)
                                      space= space + space/2;

                              if (width+space==0) {
                                      space= 2*space -
         (q->_thru[dir]*space/(q->_tracks[dir]/2));
                              }
      */
      g->placeAgainSortedThru(width, space, _obsTable[row][col]);
    }
  }
  return cnt;
}
uint Ath__quadTable::placeCornerObsShapes(uint row,
                                          uint col,
                                          uint type,
                                          uint width,
                                          uint space)
{
  uint      cnt = 0;
  Ath__box* e   = _qPinTable[row][col]->getHeadCornerObsShape(type);
  for (; e != NULL; e = e->_next) {
    _tmpBoxArray[cnt] = e;

    if (type == 0) {
      ;
    }
    e                     = e->_next;
    _tmpBoxArrayNext[cnt] = e;

    cnt++;
  }
  if (cnt <= 0)
    return 0;

  sortByLength(_tmpBoxArray, _tmpBoxArrayNext, cnt);

  _qShapeTable[row][col]->placeCornerSorted(
      type, _tmpBoxArray, _tmpBoxArrayNext, cnt, width, space);

  return cnt;
}
void Ath__quadTable::sortX(Ath__box** A, uint cnt, uint dbg)
{
  if (cnt < 2)
    return;

  if (dbg > 0) {
    fprintf(stdout, "UNSORTED -----------------\n");
    for (uint i = 0; i < cnt; i++)
      fprintf(stdout, "%9d\n", A[i]->_xlo);
  }
  for (uint ii = 0; ii < cnt - 1; ++ii) {
    for (uint jj = cnt - 1; ii < jj; --jj) {
      if (A[jj - 1]->_xlo > A[jj]->_xlo) {
        Ath__box* tmp = A[jj - 1];
        A[jj - 1]     = A[jj];
        A[jj]         = tmp;
      }
    }
  }
  if (dbg > 0) {
    fprintf(stdout, "SORTED -----------------\n");
    for (uint jj = 0; jj < cnt; ++jj)
      fprintf(stdout, "%9d\n", A[jj]->_xlo);
  }
}
void Ath__quadTable::sortByLength(Ath__box** A,
                                  Ath__box** B,
                                  uint       cnt,
                                  uint       dbg)
{
  if (cnt < 2)
    return;

  if (dbg > 0) {
    fprintf(stdout, "UNSORTED -----------------\n");
    for (uint i = 0; i < cnt; i++) {
      fprintf(stdout,
              "%9d  %d %d\n",
              A[i]->getLength() + B[i]->getLength(),
              A[i]->getLength(),
              B[i]->getLength());
    }
  }
  for (uint ii = 0; ii < cnt - 1; ++ii) {
    for (uint jj = cnt - 1; ii < jj; --jj) {
      uint len1 = A[jj - 1]->getLength() + B[jj - 1]->getLength();
      uint len2 = A[jj]->getLength() + B[jj]->getLength();

      // if (A[jj-1]->_yhi -A[jj-1]->_ylo > A[jj]->_yhi-A[jj]->_ylo) {
      if (len1 > len2) {
        Ath__box* tmp = A[jj - 1];
        A[jj - 1]     = A[jj];
        A[jj]         = tmp;

        Ath__box* tmpB = B[jj - 1];
        B[jj - 1]      = B[jj];
        B[jj]          = tmpB;
      }
    }
  }
  if (dbg > 0) {
    fprintf(stdout, "SORTED -----------------\n");
    for (uint i = 0; i < cnt; i++) {
      fprintf(stdout,
              "%9d  %d %d\n",
              A[i]->getLength() + B[i]->getLength(),
              A[i]->getLength(),
              B[i]->getLength());
    }
  }
}
void Ath__quadTable::sortY(Ath__box** A, uint cnt, uint dbg)
{
  if (cnt < 2)
    return;

  if (dbg > 0) {
    fprintf(stdout, "UNSORTED -----------------\n");
    for (uint i = 0; i < cnt; i++)
      fprintf(stdout, "%9d\n", A[i]->_ylo);
  }
  for (uint ii = 0; ii < cnt - 1; ++ii) {
    for (uint jj = cnt - 1; ii < jj; --jj) {
      if (A[jj - 1]->_ylo > A[jj]->_ylo) {
        Ath__box* tmp = A[jj - 1];
        A[jj - 1]     = A[jj];
        A[jj]         = tmp;
      }
    }
  }
  if (dbg > 0) {
    fprintf(stdout, "SORTED -----------------\n");
    for (uint jj = 0; jj < cnt; ++jj)
      fprintf(stdout, "%9d\n", A[jj]->_ylo);
  }
}
uint Ath__quadTable::makeZuiObjects(Ath__box* head, Ath__zui* zui)
{
  uint cnt = 0;
  for (Ath__box* e = head; e != NULL; e = e->_next) {
    zui->addBox(e->getOwner(),
                e->getBoxType(),
                Ath_hier__tile,
                e->_layer,
                e->_xlo,
                e->_ylo,
                e->_xhi,
                e->_yhi);
    cnt++;
  }
  return cnt;
}
uint Ath__quadTable::getQuadBoundaries(Ath__zui* zui,
                                       uint      busFlag,
                                       uint      w,
                                       uint      layer)
{
  uint cnt = 0;
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      cnt += getBoundaries(ii, jj, zui, w, layer);

      if ((busFlag > 0) && (_qShapeTable != NULL))
        cnt += _qShapeTable[ii][jj]->getGridOutlines(zui);
    }
  }
  return cnt;
}
uint Ath__quadTable::getTopPins(Ath__zui* zui)
{
  getPinObsShapes(zui);
  return 1;

  uint cnt = 0;

  return cnt;
}
uint Ath__quadTable::getBusObs(uint ii, uint jj, Ath__zui* zui)
{
  _qShapeTable[ii][jj]->getBusObs(zui);
  return 1;
}
uint Ath__quadTable::getBusShapes(Ath__zui* zui)
{
  uint cnt = 0;
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      _qShapeTable[ii][jj]->getBusObs(zui);
    }
  }
  return cnt;
}
uint Ath__quadTable::getPinObsShapes(Ath__zui* zui)
{
  uint cnt = 0;
  for (uint ii = 0; ii < _rowCnt; ii++) {
    for (uint jj = 0; jj < _colCnt; jj++) {
      ;  // _qShapeTable[ii][jj]->makeZuiObjects(zui);
    }
  }
  return cnt;
}
