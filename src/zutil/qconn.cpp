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

bool Ath__quadTable::checkMidRes(uint  r1,
                                 uint  c1,
                                 uint  r2,
                                 uint  c2,
                                 uint* midRow,
                                 uint* midCol,
                                 uint* turn)
{
  // assume dist=2, rdist=1, cdist=1

  if (c2 > c1) {
    if (_quad[r2][c1]->checkTurn(0)) {
      *midRow = r2;
      *midCol = c1;
      *turn   = 0;
      return true;
    }
    if (_quad[r1][c2]->checkTurn(1)) {
      *midRow = r1;
      *midCol = c2;
      *turn   = 1;
      return true;
    }
  } else {
    if (_quad[r2][c1]->checkTurn(2)) {
      *midRow = r2;
      *midCol = c1;
      *turn   = 2;
      return true;
    }
    if (_quad[r1][c2]->checkTurn(3)) {
      *midRow = r1;
      *midCol = c2;
      *turn   = 3;
      return true;
    }
  }
  fprintf(stdout, "Cannot assign conn %d %d  %d %d\n", r1, c1, r2, c2);
  return false;
}
void Ath__quadTable::calcNextRes(Ath__connWord w)
{
  uint c1;
  uint r1  = w.getFromRowCol(&c1);
  uint dir = w.getStraight();

  _quad[r1][c1]->updateNext(dir);
}
uint Ath__quadTable::checkThruRes(Ath__connWord w)
{
  uint c1;
  uint r1 = w.getFromRowCol(&c1);
  uint c2;
  uint r2 = w.getToRowCol(&c2);

  uint cnt = 0;
  uint dir = w.getStraight();
  if (dir == 0) {
    for (uint col = c1 + 1; col < c2; col++)
      cnt += _quad[r1][col]->checkThru(dir);
  } else {
    for (uint row = r1 + 1; row < r2; row++)
      cnt += _quad[row][c1]->checkThru(dir);
  }
  if (cnt > 0) {
    fprintf(stdout, "Cannot do THRU %d %d  %d %d\n", r1, c1, r2, c2);
  }
  return cnt;
}

void Ath__quadTable::calcThruResources(Ath__p2pConn* p2p)
{
  Ath__connWord w = p2p->_conn;
  uint          c1;
  uint          r1 = w.getFromRowCol(&c1);
  uint          c2;
  uint          r2 = w.getToRowCol(&c2);

  uint dir = w.getStraight();
  if (dir == 0) {
    for (uint col = c1 + 1; col < c2; col++) {
      _quad[r1][col]->updateThru(dir);
      _thruPinTable[r1][col]->addStraightConn(0, c2 - c1, p2p);
    }
  } else {
    for (uint row = r1 + 1; row < r2; row++) {
      _quad[row][c1]->updateThru(dir);
      _thruPinTable[row][c1]->addStraightConn(1, r2 - r1, p2p);
    }
  }
  //_2dQuadTable[w.getFrom()][w.getTo()]++;
}
Ath__box* Ath__quadTable::allocPinBox()
{
  uint      n;
  Ath__box* bb = _pinBoxPool->alloc(NULL, &n);
  bb->_id      = n;
  bb->_layer   = 0;
  return bb;
}

Ath__qPin* Ath__quadTable::mkQpin(uint                r1,
                                  uint                c1,
                                  uint                netId,
                                  uint                ioNetId,
                                  Ath__connWord       w,
                                  AthPool<Ath__qPin>* pool)
{
  Ath__qPin* pin = _qPinTable[r1][c1]->addPin(NULL, netId, ioNetId, w, NULL);

  setPinNetConn(pin);

  Ath__box* bb = allocPinBox();

  uint layer = w.getStraight();
  if (layer == 0)
    bb->_layer = 1;
  else if (layer > 0)
    bb->_layer = 2;

  bb->set(_tmpBox[r1][c1]);
  bb->setIdentity(Ath_box__pin, pin->_nameId, Ath_hier__tile);

  pin->_instBB = bb;

  return pin;
}
void Ath__quadTable::addConn2Pins(Ath__connWord w,
                                  Ath__qPin*    srcPin,
                                  Ath__qPin*    dstPin)
{
  dstPin->_src  = 0;
  srcPin->_src  = 1;
  srcPin->_next = dstPin;

  Ath__box* bb = srcPin->getInstBox();

  Ath__p2pConn* conn
      = _connTable->addConn(srcPin->_netId, w, bb->getDX(), bb->getDY());

  conn->setPin(srcPin);
  srcPin->_head = conn;
  dstPin->_head = conn;
}
void Ath__quadTable::defineConn(uint netId, uint src, uint dst)
{
  // ASSUMPTION 1-to-1 correspondence between tile pin and ioNet
  // can go form pin to ionet ONLY

  uint c1;
  uint r1 = _targetArray[src].getFromRowCol(&c1);
  uint c2;
  uint r2 = _targetArray[dst].getFromRowCol(&c2);

  Ath__connWord w(r1, c1, r2, c2, _colCnt);

  Ath__qPin* pin1 = mkQpin(r1, c1, netId, _tmpNetArray[src], w);
  Ath__qPin* pin2 = mkQpin(r2, c2, netId, _tmpNetArray[dst], w);

  if (w.ordered())
    addConn2Pins(w, pin2, pin1);
  else
    addConn2Pins(w, pin1, pin2);
}
void Ath__quadTable::defineConn3row(uint          netId,
                                    Ath__connWord w01,
                                    Ath__connWord w02,
                                    Ath__connWord w23,
                                    uint          r0,
                                    uint          c0,
                                    uint          r1,
                                    uint          c1,
                                    uint          r2,
                                    uint          c2)
{
  uint tjunction = 0;

  uint midCol;
  if (c0 < c1) {
    if (c2 <= c1) {  // T junction
      tjunction = 1;

      midCol = c0;
      if (c2 > c0)
        midCol = c2;
    }
  } else {
    if (c2 >= c1) {  // T junction
      tjunction = 1;

      midCol = c2;
      if (c2 > c0)
        midCol = c0;
    }
  }
  if (tjunction > 0) {
    Ath__connWord w1(r1, midCol, r0, c0, _colCnt);
    Ath__connWord w2(r1, midCol, r1, c1, _colCnt);
    Ath__connWord w3(r1, midCol, r2, c2, _colCnt);
    _connTable->addConn(netId, w1);
    _connTable->addConn(netId, w2);
    _connTable->addConn(netId, w3);

    Ath__qPin* src  = mkQpin(r1, midCol, netId, 0, w1);
    src->_tjunction = 1;
    Ath__qPin* pin0 = mkQpin(r0, c0, netId, 0, w1);
    Ath__qPin* pin1 = mkQpin(r1, c1, netId, 0, w2);
    Ath__qPin* pin2 = mkQpin(r2, c2, netId, 0, w3);

    addConn4Pins(src, w1, pin0, w2, pin1, w3, pin2);
  } else {
    Ath__qPin* src  = mkQpin(r0, c0, netId, 0, w01);
    Ath__qPin* pin1 = mkQpin(r1, c1, netId, 0, w01);
    Ath__qPin* pin2 = mkQpin(r2, c2, netId, 0, w23);

    addConn3Pins(src, w01, pin1, w23, pin2);
  }
}
void Ath__quadTable::defineConn3col(uint          netId,
                                    Ath__connWord w01,
                                    Ath__connWord w02,
                                    Ath__connWord w23,
                                    uint          r0,
                                    uint          c0,
                                    uint          r1,
                                    uint          c1,
                                    uint          r2,
                                    uint          c2)
{
  // TODO mkQpin globally
  uint midRow;
  uint tjunction = 0;

  if (r0 < r1) {
    if (r2 <= r1) {  // T junction
      midRow = r0;
      if (r2 > r0)
        midRow = r2;

      tjunction = 1;
    }
  } else {
    if (r2 >= r1) {  // T junction
      midRow = r2;
      if (r2 > r0)
        midRow = r0;

      tjunction = 1;
    }
  }
  if (tjunction > 0) {
    Ath__connWord w1(midRow, c1, r0, c0, _colCnt);
    Ath__connWord w2(midRow, c1, r1, c1, _colCnt);
    Ath__connWord w3(midRow, c1, r2, c2, _colCnt);

    Ath__qPin* src  = mkQpin(midRow, c1, netId, 0, w1);
    src->_tjunction = 1;
    Ath__qPin* pin0 = mkQpin(r0, c0, netId, 0, w1);
    Ath__qPin* pin1 = mkQpin(r1, c1, netId, 0, w2);
    Ath__qPin* pin2 = mkQpin(r2, c2, netId, 0, w3);

    addConn4Pins(src, w1, pin0, w2, pin1, w3, pin2);
  } else {
    Ath__qPin* src  = mkQpin(r0, c0, netId, 0, w01);
    Ath__qPin* pin1 = mkQpin(r1, c1, netId, 0, w01);
    Ath__qPin* pin2 = mkQpin(r2, c2, netId, 0, w23);

    addConn3Pins(src, w01, pin1, w23, pin2);
  }
}
void Ath__quadTable::addConn3Pins(Ath__qPin*    srcPin,
                                  Ath__connWord w,
                                  Ath__qPin*    pin1,
                                  Ath__connWord w23,
                                  Ath__qPin*    pin2)
{
  srcPin->_src = 1;
  pin1->_src   = 0;
  pin2->_src   = 0;

  srcPin->_next = pin1;
  pin1->_next   = pin2;

  Ath__p2pConn* conn1 = _connTable->addConn(srcPin->_netId, w);
  Ath__p2pConn* conn2 = _connTable->addConn(srcPin->_netId, w23);

  conn1->setPin(srcPin);
  conn2->setPin(pin2);
  srcPin->_head = conn1;
  pin1->_head   = conn1;
  pin2->_head   = conn2;
}
void Ath__quadTable::addConn4Pins(Ath__qPin*    srcPin,
                                  Ath__connWord w1,
                                  Ath__qPin*    pin1,
                                  Ath__connWord w2,
                                  Ath__qPin*    pin2,
                                  Ath__connWord w3,
                                  Ath__qPin*    pin3)
{
  srcPin->_src = 1;
  pin1->_src   = 0;
  pin2->_src   = 0;
  pin3->_src   = 0;

  srcPin->_next = pin1;
  pin1->_next   = pin2;
  pin2->_next   = pin3;

  Ath__p2pConn* conn1 = _connTable->addConn(srcPin->_netId, w1);
  Ath__p2pConn* conn2 = _connTable->addConn(srcPin->_netId, w2);
  Ath__p2pConn* conn3 = _connTable->addConn(srcPin->_netId, w3);

  conn1->setPin(pin1);
  conn2->setPin(pin2);
  conn2->setPin(pin3);

  pin1->_head = conn1;
  pin2->_head = conn2;
  pin3->_head = conn3;

  srcPin->_head = NULL;  // virtual pin
}
void Ath__quadTable::defineConn3strH(uint          netId,
                                     Ath__connWord w,
                                     uint          r2,
                                     uint          c2)
{
  uint c0;
  uint c1;
  uint r0 = w.getSeg(&c0, &c1);

  Ath__qPin* src = mkQpin(r0, c1, netId, 0, w);

  uint col;
  if (c2 <= c0) {
    col = c0;
  } else if (c2 >= c1) {
    col = c1;
  } else {
    col             = c2;  // T junction
    src->_tjunction = 1;   // TODO
  }
  Ath__connWord w23(r0, col, r2, c2, _colCnt);

  Ath__qPin* pin1 = mkQpin(r0, c1, netId, 0, w);
  Ath__qPin* pin2 = mkQpin(r2, c2, netId, 0, w23);

  addConn3Pins(src, w, pin1, w23, pin2);
}
void Ath__quadTable::defineConn3strV(uint          netId,
                                     Ath__connWord w,
                                     uint          r2,
                                     uint          c2)
{
  _connTable->addConn(netId, w);

  uint r0;
  uint r1;
  uint c0 = w.getSeg(&r0, &r1);

  Ath__qPin* src = mkQpin(r1, c0, netId, 0, w);

  uint row;
  if (r2 <= r0) {
    row = r0;
  } else if (r2 >= r1) {
    row = r1;
  } else {
    row             = r2;
    src->_tjunction = 1;  // TODO
  }
  Ath__connWord w23(row, c0, r2, c2, _colCnt);

  Ath__qPin* pin1 = mkQpin(r1, c0, netId, 0, w);
  Ath__qPin* pin2 = mkQpin(r2, c2, netId, 0, w23);

  addConn3Pins(src, w, pin1, w23, pin2);
}

void Ath__quadTable::defineConn(uint netId, uint src, uint dst1, uint dst2)
{
  // TODO, connect IO net

  uint c0;
  uint r0 = _targetArray[src].getFromRowCol(&c0);
  uint c1;
  uint r1 = _targetArray[dst1].getFromRowCol(&c1);
  uint c2;
  uint r2 = _targetArray[dst2].getFromRowCol(&c2);

  Ath__connWord w01(r0, c0, r1, c1, _colCnt);
  Ath__connWord w02(r0, c0, r2, c2, _colCnt);
  Ath__connWord w23(r1, c1, r2, c2, _colCnt);

  if ((w01.getStraight() >= 0) || (w02.getStraight() >= 0)
      || (w23.getStraight() >= 0)) {
    int dir = w01.getStraight();
    if (dir == 0) {
      defineConn3strH(netId, w01, r2, c2);
      return;
    } else if (dir == 1) {
      defineConn3strV(netId, w01, r2, c2);
      return;
    }

    dir = w02.getStraight();
    if (dir == 0) {
      defineConn3strH(netId, w02, r1, c1);
      return;
    } else if (dir == 1) {
      defineConn3strV(netId, w02, r1, c1);
      return;
    }

    dir = w23.getStraight();
    if (dir == 0) {
      defineConn3strH(netId, w23, r0, c0);
      return;
    } else if (dir == 1) {
      defineConn3strV(netId, w23, r0, c0);
      return;
    }
  } else if (((w01.getRowDist() > 0) && (w02.getRowDist() > 0))
             || ((w01.getRowDist() < 0) && (w02.getRowDist() < 0))) {
    if (w01.getRowDist() > 0) {
      if (r1 < r2) {
        defineConn3row(netId, w01, w02, w23, r0, c0, r1, c1, r2, c2);
      } else {
        defineConn3row(netId, w02, w01, w23, r0, c0, r2, c2, r1, c1);
      }
    } else {
      if (r1 > r2) {
        defineConn3row(netId, w01, w02, w23, r0, c0, r1, c1, r2, c2);
      } else {
        defineConn3row(netId, w02, w01, w23, r0, c0, r2, c2, r1, c1);
      }
    }
  } else if (((w01.getColDist() > 0) && (w02.getColDist() > 0))
             || ((w01.getColDist() < 0) && (w02.getColDist() < 0))) {
    if (w01.getColDist() > 0) {
      if (c1 < c2) {
        defineConn3col(netId, w01, w02, w23, r0, c0, r1, c1, r2, c2);
      } else {
        defineConn3col(netId, w02, w01, w23, r0, c0, r2, c2, r1, c1);
      }
    } else {
      if (c1 > c2) {
        defineConn3col(netId, w01, w02, w23, r0, c0, r1, c1, r2, c2);
      } else {
        defineConn3col(netId, w02, w01, w23, r0, c0, r2, c2, r1, c1);
      }
    }
  } else {
    Ath__qPin* src  = mkQpin(r0, c0, netId, 0, w01);
    Ath__qPin* pin1 = mkQpin(r1, c1, netId, 0, w01);
    Ath__qPin* pin2 = mkQpin(r2, c2, netId, 0, w23);

    addConn3Pins(src, w01, pin1, w23, pin2);
  }
}

uint Ath__quadTable::processConnections(uint netId, uint termCnt)
{
  if (_tmpCnt == 0)
    return 0;

  _targetCnt = 0;

  for (uint ii = 0; ii < _tmpCnt; ii++) {
    if (addTarget(_tmpArray[ii], netId, termCnt)) {  // local net
      _fanoutArray[1]++;
      return 1;
    }
  }
  _fanoutArray[_targetCnt]++;

  if (_targetCnt == 2) {
    defineConn(netId, 0, 1);
  } else if (_targetCnt == 3) {
    // defineConn(netId, 0, 1, 2);
  }
  return _targetCnt;
}
