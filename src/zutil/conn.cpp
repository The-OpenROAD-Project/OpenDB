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

#include "conn.h"
#include "wire.h"

Ath__connWord::Ath__connWord(uint r1, uint c1)
{
  _conn._fromRow  = r1;
  _conn._fromCol  = c1;
  _conn._toRow    = 0;
  _conn._toCol    = 0;
  _conn._ordered  = 0;
  _conn._placed   = 0;
  _conn._posFlow  = 0;
  _conn._corner   = 0;
  _conn._straight = 0;
  _conn._dir      = 0;
}
Ath__connWord::Ath__connWord(uint r1, uint c1, uint type)
{
  _conn._fromRow  = r1;
  _conn._fromCol  = c1;
  _conn._toRow    = type;
  _conn._toCol    = type;
  _conn._ordered  = 0;
  _conn._placed   = 0;
  _conn._posFlow  = 0;
  _conn._corner   = 0;
  _conn._straight = 0;
  _conn._dir      = 0;
}
bool Ath__connWord::ordered()
{
  return (_conn._ordered > 0) ? true : false;
}

void Ath__connWord::orderByRow(uint r1, uint c1, uint r2, uint c2, uint colCnt)
{
  if (r1 > r2) {
    set(r2, c2, r1, c1, colCnt);
    _conn._ordered = 1;
  } else {
    set(r1, c1, r2, c2, colCnt);
  }
}
Ath__connWord::Ath__connWord(uint r1, uint c1, uint r2, uint c2, uint colCnt)
{
  set(r1, c1, r2, c2, colCnt);

  _conn._ordered = 0;
  _conn._placed  = 0;
  _conn._posFlow = 0;
  _conn._corner  = 0;

  _conn._straight = 0;
  _conn._dir      = 0;
  if (getRowDist() == 0) {
    _conn._straight = 1;
    if (c1 > c2) {
      set(r1, c1, r2, c2, colCnt);
      _conn._fromCol = c2;
      _conn._toCol   = c1;
      _conn._ordered = 1;
    }
  } else if (getColDist() == 0) {
    _conn._straight = 1;
    _conn._dir      = 1;
    if (r1 > r2) {
      _conn._fromRow = r2;
      _conn._toRow   = r1;
      _conn._ordered = 1;
    }
  } else {
    orderByRow(r1, c1, r2, c2, colCnt);
    _conn._corner = 1;
  }
}
int Ath__connWord::getStraight()
{
  if (_conn._straight > 0)
    return _conn._dir;
  else
    return -1;
}
int Ath__connWord::getRowDist()
{
  return _conn._toRow - _conn._fromRow;
}
int Ath__connWord::getColDist()
{
  return _conn._toCol - _conn._fromCol;
}
uint Ath__connWord::getDist()
{
  return ath__max(getRowDistAbs(), getColDistAbs());
}
uint Ath__connWord::getMinDist()
{
  return ath__min(getRowDistAbs(), getColDistAbs());
}
uint Ath__connWord::getMaxDist()
{
  return ath__max(getRowDistAbs(), getColDistAbs());
}
uint Ath__connWord::getRowDistAbs()
{
  int dist = getRowDist();
  if (dist < 0)
    return -dist;
  else
    return dist;
}
uint Ath__connWord::getSeg(uint* c1, uint* c2)
{
  if (_conn._straight == 0)
    return 0;

  if (_conn._dir == 0) {
    *c2 = _conn._toCol;
    *c1 = _conn._fromCol;

    return _conn._fromRow;
  } else {
    *c2 = _conn._toRow;
    *c1 = _conn._fromRow;

    return _conn._fromCol;
  }
}
uint Ath__connWord::getColDistAbs()
{
  int dist = getColDist();
  if (dist < 0)
    return -dist;
  else
    return dist;
}
Ath__connWord::Ath__connWord(uint v)
{
  setAll(v);
}
uint Ath__connWord::getAll()
{
  return _all;
}
void Ath__connWord::setAll(uint v)
{
  _all = v;
}
uint Ath__connWord::getFromRowCol(uint* col)
{
  *col = _conn._fromCol;
  return _conn._fromRow;
}
uint Ath__connWord::getToRowCol(uint* col)
{
  *col = _conn._toCol;
  return _conn._toRow;
}
uint Ath__connWord::getFrom()
{
  return _conn._colCnt * _conn._fromRow + _conn._fromCol;
}

uint Ath__connWord::getTo()
{
  return _conn._colCnt * _conn._toRow + _conn._toCol;
}
uint Ath__connWord::set(uint x1, uint y1, uint x2, uint y2, uint colCnt)
{
  _conn._fromRow = x1;
  _conn._fromCol = y1;
  _conn._toRow   = x2;
  _conn._toCol   = y2;
  _conn._colCnt  = colCnt;

  return _all;
}
void Ath__p2pConn::setNext(Ath__p2pConn* v)
{
  _next = v;
}
void Ath__p2pConn::setPin(Ath__qPin* q)
{
  _srcPin = q;
}
Ath__qPin* Ath__p2pConn::getSrcPin()
{
  return _srcPin;
}
Ath__connTable::Ath__connTable(AthPool<Ath__p2pConn>* pool,
                               uint                   n,
                               uint                   pinLength,
                               uint                   tileSize)
{
  if (n == 0)
    n = 32;

  _nextSegCnt = 2;
  _tileSize   = 0;
  _pinLength  = 0;
  if (tileSize > 0) {
    _nextSegCnt = tileSize / pinLength + 1;
    _tileSize   = tileSize;
    _pinLength  = pinLength;
  }

  _maxBankCnt = n;

  _straightTable[0] = new Ath__array2d<Ath__p2pConn*>(n, true);
  _straightTable[1] = new Ath__array2d<Ath__p2pConn*>(n, true);

  _nextTable[0] = new Ath__array2d<Ath__p2pConn*>(_nextSegCnt);
  _nextTable[1] = new Ath__array2d<Ath__p2pConn*>(_nextSegCnt);

  _cornerTable = new Ath__array2d<Ath__p2pConn*>(2 * n, true);
  _tmpTablePtr = NULL;
  _tmpArrayPtr = NULL;

  _poolPtr = pool;
  //_poolPtr= new AthPool<Ath__p2pConn>(1024);
}
uint Ath__connTable::getSegmentIndex(uint dir, uint dx, uint dy)
{
  if (_pinLength == 0)
    return 0;

  if (dir > 0) {  // vertical
    return dy / _pinLength;
  } else {
    return dx / _pinLength;
  }
}

Ath__p2pConn* Ath__connTable::addConn(uint          netId,
                                      Ath__connWord w,
                                      uint          dx,
                                      uint          dy)
{
  Ath__p2pConn* conn = _poolPtr->alloc();

  conn->_netId = netId;
  conn->_conn  = w;

  uint dist = w.getDist();

  int dir = w.getStraight();
  if (dir < 0) {
    _cornerTable->add(dist, conn);
  } else {
    if (dist > 1) {
      _straightTable[dir]->add(dist, conn);
    } else {
      if (dy == 0) {  // default
        _nextTable[dir]->add(0, conn);
      } else {
        uint kk = getSegmentIndex(dir, dx, dy);
        _nextTable[dir]->add(kk, conn);
      }
    }
  }
  return conn;
}
uint Ath__connTable::addStraightConn(uint dir, uint dist, Ath__p2pConn* p2p)
{
  return _straightTable[dir]->add(dist, p2p);
}
uint Ath__connTable::addCornerConn2(uint          type,
                                    Ath__p2pConn* p2p,
                                    uint          ii,
                                    uint          jj)
{
  Ath__connWord w(ii, jj, type);
  p2p->_conn = w;

  return _cornerTable->add(type, p2p);
}
void Ath__connTable::printConnStats(FILE* fp)
{
  fprintf(fp, "\n\nNet length distribution per tile unit length\n");
  fprintf(fp, "--------------------------------------------\n");

  char buff[128];
  for (uint ii = 0; ii < _nextSegCnt; ii++) {
    sprintf(buff,
            " next tile HORIZONTAL conns - Length= %7d",
            (ii + 1) * _pinLength);
    _nextTable[0]->printCnt(fp, ii, buff);
  }
  fprintf(fp, "\n");
  for (uint jj = 0; jj < _nextSegCnt; jj++) {
    sprintf(
        buff, " next tile VERTICAL conns - Length= %7d", (jj + 1) * _pinLength);
    _nextTable[1]->printCnt(fp, jj, buff);
  }

  fprintf(fp, "\n\n--------- straight tile-feedthru's\n");

  _straightTable[0]->printAllCnts(fp, "tile-dist", "  HORIZONTAL");
  _straightTable[1]->printAllCnts(fp, "tile-dist", "  VERTICAL");

  fprintf(fp, "\n\n--------- corner tile-feedthru's\n");
  _cornerTable->printAllCnts(fp, "tile-dist", "CORNER");
}
uint Ath__connTable::startNextIterator(uint dir, uint seg)
{
  _tmpTablePtr = _nextTable[dir];
  return _tmpTablePtr->startIterator(seg);
}
uint Ath__connTable::startCornerIterator(uint dist)
{
  _tmpTablePtr = _cornerTable;
  return _tmpTablePtr->startIterator(dist);
}
uint Ath__connTable::startStraightIterator(uint dir, uint dist)
{
  _tmpTablePtr = _straightTable[dir];
  return _tmpTablePtr->startIterator(dist);
}
int Ath__connTable::getNextConn(Ath__connWord* conn, uint* netId)
{
  Ath__p2pConn* p2p  = NULL;
  uint          next = _tmpTablePtr->getNext(&p2p);
  if (next == 0)
    return 0;

  *netId = p2p->_netId;
  *conn  = p2p->_conn;
  return next;
}
Ath__p2pConn* Ath__connTable::getNextConn()
{
  Ath__p2pConn* p2p = NULL;

  if (_tmpTablePtr->getNext(&p2p) == 0)
    return NULL;

  return p2p;
}
Ath__array2d<Ath__p2pConn*>* Ath__connTable::startStraightArrayIterator(
    uint dir)
{
  _tmpArrayPtr = _straightTable[dir];
  for (uint ii = 0; ii < _maxBankCnt; ii++) {
    _tmpCurrentIndex[ii] = 0;
    _tmpCnt[ii]          = _tmpArrayPtr->getCnt(ii);
  }
  return _tmpArrayPtr;
}
bool Ath__connTable::getNextArrayConn(uint ii, Ath__connWord* conn, uint* netId)
{
  if (_tmpCurrentIndex[ii] >= _tmpCnt[ii])
    return false;

  Ath__p2pConn* p2p = _tmpArrayPtr->get(ii, _tmpCurrentIndex[ii]);

  _tmpCurrentIndex[ii]++;

  *netId = p2p->_netId;
  *conn  = p2p->_conn;

  return true;
}

uint Ath__qPin::getToRowCol(uint* col)
{
  return _conn.getToRowCol(col);
}
uint Ath__qPin::getTurnRowCol(uint* row, uint* col)
{
  Ath__qPin* src = getSrcPin();

  *row = src->_head->_conn.getFromRowCol(col);

  uint tmp1;  // TODO
  return src->_head->_conn.getToRowCol(&tmp1);
}
int Ath__qPin::getTurnDist(uint* row, uint* col, int* dir)
{
  uint turnRow, turnCol;
  getTurnRowCol(&turnRow, &turnCol);

  int rowDist = turnRow - *row;
  int colDist = turnCol - *col;
  if (rowDist == 0)  // horizontal
  {
    *dir = 1;
    if (colDist < 0) {
      *col = turnCol;
      return -colDist;
    }
    return colDist;
  }
  if (colDist == 0)  // vertical
  {
    *dir = 0;
    if (rowDist < 0) {
      *row = turnRow;
      return -rowDist;
    }
    return rowDist;
  }
  *dir = -1;
  return -1;
}
bool Ath__qPin::isTargeted()
{
  return (_targeted > 0) ? true : false;
}
bool Ath__qPin::isAssigned()
{
  return (_assigned > 0) ? true : false;
}
void Ath__qPin::setTargeted()
{
  _targeted = 1;
}
void Ath__qPin::setPlaced()
{
  _placed = 1;
}
bool Ath__qPin::isPlaced()
{
  return (_placed > 0) ? true : false;
}
bool Ath__qPin::isSrc()
{
  return (_src > 0) ? true : false;
}
Ath__qPin* Ath__qPin::getSrcPin()
{
  if (_src > 0)
    return this;
  else
    return _head->getSrcPin();
}
Ath__qPin* Ath__qPin::getDstPin()
{
  if (_src > 0)
    return _next;
  else
    return this;
}
Ath__box* Ath__qPin::getInstBox()
{
  return _instBB;
}
uint Ath__qPin::getLayer()
{
  if (_portBB != NULL)
    return _portBB->_layer;
  if (_targetBB != NULL)
    return _targetBB->_layer;
  if (_instBB != NULL)
    return _instBB->_layer;
  return 0;
}
void Ath__qPin::addBusBox(Ath__box* bb)
{
  bb->_next = _busList;
  _busList  = bb;
}
void Ath__qPin::setInstBox(Ath__box* bb)
{
  _instBB = bb;
}
void Ath__qPin::getNextObsBox(Ath__searchBox* sbb)
{
  Ath__box* bb = _obsList;
  if (_obsList->_next != NULL)
    bb = _obsList->_next;

  sbb->set(bb->_xlo, bb->_ylo, bb->_xhi, bb->_yhi, bb->_layer, 0);
}
void Ath__qPin::getObsBox(Ath__searchBox* sbb)
{
  Ath__box* bb = _obsList;
  sbb->set(bb->_xlo, bb->_ylo, bb->_xhi, bb->_yhi, bb->_layer, 0);
}
void Ath__qPin::getTargetBox(Ath__searchBox* bb)
{
  bb->set(_targetBB->_xlo,
          _targetBB->_ylo,
          _targetBB->_xhi,
          _targetBB->_yhi,
          _targetBB->_layer,
          0);
}
uint Ath__qPin::makeZuiObject(Ath__zui* zui,
                              uint      width,
                              bool      actual,
                              bool      instFlag)
{
  if (width <= 0)
    width = 1000;

  if (instFlag) {
    uint w2 = width / 2;
    zui->addBox(_nameId,
                Ath_box__pin,
                Ath_hier__tile,
                _instBB->_layer,
                _instBB->getMidX() - w2,
                _instBB->getMidY() - w2,
                _instBB->getMidX() + w2,
                _instBB->getMidY() + w2);

    return _instBB->_layer;
  }
  if (actual && (_portBB != NULL)) {
    int x1 = _portBB->_xlo;
    int y1 = _portBB->_ylo;
    int x2 = _portBB->_xhi;
    int y2 = _portBB->_yhi;

    zui->addBox(
        _nameId, Ath_box__pin, Ath_hier__tile, _portBB->_layer, x1, y1, x2, y2);

    return _portBB->_layer;
  }
  if (_targetBB != NULL) {
    uint w2 = width / 2;
    int  x1 = _targetBB->_xlo;
    int  y1 = _targetBB->_ylo;
    int  x2 = _targetBB->_xhi;
    int  y2 = _targetBB->_yhi;
    if (x1 == x2) {
      x1 -= w2;
      x2 += w2;
    } else if (y1 == y2) {
      y1 -= w2;
      y2 += w2;
    }
    zui->addBox(_nameId,
                Ath_box__pin,
                Ath_hier__tile,
                _targetBB->_layer,
                x1,
                y1,
                x2,
                y2);

    return _targetBB->_layer;
  }
}
Ath__box* Ath__qPin::getPortCoords(int*  x1,
                                   int*  y1,
                                   int*  x2,
                                   int*  y2,
                                   uint* layer)
{
  if (_portBB == NULL)
    return NULL;

  *x1    = _portBB->_xlo;
  *y1    = _portBB->_ylo;
  *x2    = _portBB->_xhi;
  *y2    = _portBB->_yhi;
  *layer = _portBB->_layer;

  return _portBB;
}
Ath__box* Ath__qPin::getBusList()
{
  return _busList;
}

uint Ath__qPin::makePinObsZui(Ath__zui* zui,
                              int       width,
                              int       x1,
                              int       y1,
                              int       x2,
                              int       y2,
                              int       layer)
{
  uint w2 = width / 2;

  if (x1 == x2) {
    x1 -= w2;
    x2 += w2;
  } else if (y1 == y2) {
    y1 -= w2;
    y2 += w2;
  }
  return zui->addBox(
      _nameId, Ath_box__bus, Ath_hier__tile, layer, x1, y1, x2, y2);
}
uint Ath__qPin::makeBusZuiObject(Ath__zui* zui, uint width)
{
  uint cnt = 0;
  // if (obs==NULL) // nextTile shapes don't have shapes
  for (Ath__box* obs = _obsList; obs != NULL; obs = obs->_next) {
    cnt += makePinObsZui(
        zui, width, obs->_xlo, obs->_ylo, obs->_xhi, obs->_yhi, obs->_layer);
  }
  return cnt;
}

void Ath__qPin::reset()
{
  _src        = 0;
  _tjunction  = 0;
  _placed     = 0;
  _fixed      = 0;
  _assigned   = 0;
  _targeted   = 0;
  _nameId     = 0;
  _instBB     = NULL;
  _targetBB   = NULL;
  _portBB     = NULL;
  _portWireId = 0;
  _obsList    = NULL;
  _busList    = NULL;
}
void Ath__qPin::setPortWireId(uint id)
{
  _portWireId = id;
}
void Ath__qPin::pinBoxDef(FILE* fp, char* layerName, char* orient, int defUnits)
{
  fprintf(stdout, "TODO: pinBoxDef\n");
  return;
  /*
          fprintf(fp, " ( %d %d ) %s ", _bb->_xlo/defUnits, _bb->_ylo/defUnits,
     orient);
          
          fprintf(fp, " + LAYER %s ( %d %d ) ( %d %d ) ",  layerName,
                  0, 0, _bb->getDX()/defUnits, _bb->getDY()/defUnits);
  */
}

Ath__qPinTable::Ath__qPinTable(AthPool<Ath__qPin>* qPinPool,
                               AthPool<Ath__box>*  boxPool,
                               uint                n)
{
  _table = new Ath__array1D<Ath__qPin*>(n);
  for (uint jj = 0; jj < 2; jj++) {
    _nextPinShape[jj]     = NULL;
    _straightPinShape[jj] = NULL;
    _thruObsShape[jj]     = NULL;
  }
  for (uint ii = 0; ii < 4; ii++) {
    _cornerPinShape[ii] = NULL;
    _cornerObsShape[ii] = NULL;
  }
  _pool = qPinPool;
  //_pool->setDbg(1);
  _pinBoxPool = boxPool;
}
Ath__box* Ath__qPinTable::getHeadStraightPinShape(uint dir)
{
  return _straightPinShape[dir];
}
Ath__box* Ath__qPinTable::getHeadNextPinShape(uint dir)
{
  return _nextPinShape[dir];
}
Ath__box* Ath__qPinTable::getHeadCornerPinShape(uint type)
{
  return _cornerPinShape[type];
}
Ath__box* Ath__qPinTable::getHeadCornerObsShape(uint type)
{
  return _cornerObsShape[type];
}
Ath__box* Ath__qPinTable::getHeadObsShape(uint dir)
{
  return _thruObsShape[dir];
}
void Ath__qPinTable::freeBoxes(Ath__box* head)
{
  Ath__box* e = head;

  while (e != NULL) {
    Ath__box* f = e->_next;
    _pinBoxPool->free(e);
    e = f;
  }
}
void Ath__qPinTable::freeNextPinShapes()
{
  freeBoxes(_nextPinShape[0]);
  freeBoxes(_nextPinShape[1]);
  _nextPinShape[0] = NULL;
  _nextPinShape[1] = NULL;
}
Ath__box* Ath__qPinTable::newPinBox(uint layer, int x1, int y1, int x2, int y2)
{
  uint      n;
  Ath__box* a = _pinBoxPool->alloc(NULL, &n);
  a->set(x1, y1, x2, y2);
  a->_id    = n;
  a->_layer = layer;

  return a;
}
Ath__box* Ath__qPinTable::addPinBox(Ath__box* e, Ath__box* head)
{
  e->_next = head;
  return e;
}
Ath__box* Ath__qPinTable::addNextPinBox(uint pinId,
                                        uint dir,
                                        uint layer,
                                        int  x1,
                                        int  y1,
                                        int  x2,
                                        int  y2)
{
  Ath__box* bbPin = newPinBox(layer, x1, y1, x2, y2);
  bbPin->setIdentity(Ath_box__pin, pinId, Ath_hier__tile);
  _nextPinShape[dir] = addPinBox(bbPin, _nextPinShape[dir]);
  return bbPin;
}
Ath__box* Ath__qPinTable::addThruObsBox(uint netId,
                                        uint dir,
                                        uint layer,
                                        int  x1,
                                        int  y1,
                                        int  x2,
                                        int  y2)
{
  Ath__box* bbPin = newPinBox(layer, x1, y1, x2, y2);
  bbPin->setIdentity(Ath_box__bus, netId, Ath_hier__tile);
  _thruObsShape[dir] = addPinBox(bbPin, _thruObsShape[dir]);
  return _thruObsShape[dir];
}
Ath__box* Ath__qPinTable::addCornerPinBox(uint pinId,
                                          uint type,
                                          uint layer,
                                          int  x1,
                                          int  y1,
                                          int  x2,
                                          int  y2)
{
  Ath__box* bbPin = newPinBox(layer, x1, y1, x2, y2);
  bbPin->setIdentity(Ath_box__pin, pinId, Ath_hier__tile);
  _cornerPinShape[type] = addPinBox(bbPin, _cornerPinShape[type]);
  return _cornerPinShape[type];
}
Ath__box* Ath__qPinTable::addCornerObsBox(uint netId,
                                          uint type,
                                          uint layer,
                                          int  x1,
                                          int  y1,
                                          int  x2,
                                          int  y2)
{
  Ath__box* bbPin = newPinBox(layer, x1, y1, x2, y2);
  bbPin->setIdentity(Ath_box__bus, netId, Ath_hier__tile);
  _cornerObsShape[type] = addPinBox(bbPin, _cornerObsShape[type]);
  return bbPin;
}
Ath__box* Ath__qPinTable::addStraightPinBox(uint pinId,
                                            uint dir,
                                            uint layer,
                                            int  x1,
                                            int  y1,
                                            int  x2,
                                            int  y2)
{
  Ath__box* bbPin = newPinBox(layer, x1, y1, x2, y2);
  bbPin->setIdentity(Ath_box__pin, pinId, Ath_hier__tile);
  _straightPinShape[dir] = addPinBox(bbPin, _straightPinShape[dir]);

  return bbPin;
}

Ath__qPinTable::~Ath__qPinTable()
{
  for (uint jj = 0; jj < 2; jj++) {
    freeBoxes(_nextPinShape[jj]);
    freeBoxes(_straightPinShape[jj]);
    freeBoxes(_thruObsShape[jj]);
    freeBoxes(_cornerPinShape[jj]);
  }
  freeBoxes(_cornerPinShape[2]);
  freeBoxes(_cornerPinShape[3]);
}
Ath__qPin* Ath__qPinTable::addPin(Ath__qPin*          next,
                                  uint                netId,
                                  uint                ioNetId,
                                  Ath__connWord       w,
                                  AthPool<Ath__qPin>* pool)
{
  uint       id;
  Ath__qPin* pin = _pool->alloc(NULL, &id);

  pin->_conn = w;
  pin->reset();
  pin->_netId   = netId;
  pin->_ioNetId = ioNetId;
  pin->_nameId  = id;

  pin->_next = NULL;
  if (next != NULL) {
    next->_next = pin;
    pin->_src   = 0;
  } else {
    _table->add(pin);
    pin->_src = 1;
  }
  return pin;
}
Ath__qPin* Ath__qPinTable::getNextSrcPin_next()
{
  Ath__qPin* srcPin;

  while (_table->getNext(srcPin)) {
    if (srcPin->_src == 0)
      continue;
    int cornerFlag = srcPin->_conn.getStraight();
    if (cornerFlag < 0)
      continue;

    if (srcPin->_conn.getDist() > 1)
      continue;

    return srcPin;
  }
  return NULL;
}
Ath__qPin* Ath__qPinTable::getNextSrcPin_thru()
{
  Ath__qPin* srcPin;

  while (_table->getNext(srcPin)) {
    if (srcPin->_src == 0)
      continue;

    if (srcPin->_conn.getStraight() < 0)
      continue;

    if (srcPin->_conn.getDist() < 2)
      continue;

    return srcPin;
  }
  return NULL;
}
Ath__qPin* Ath__qPinTable::getNextSrcPin_corner()
{
  Ath__qPin* srcPin;

  while (_table->getNext(srcPin)) {
    if (srcPin->_src == 0)
      continue;

    int cornerFlag = srcPin->_conn.getStraight();
    if (cornerFlag >= 0)
      continue;

    //		if (srcPin->_conn.getDist()>1)
    //			continue;

    if (srcPin->_assigned == 0)
      continue;

    return srcPin;
  }
  return NULL;
}
Ath__qPin* Ath__qPinTable::getNextSrcPin_all()
{
  Ath__qPin* srcPin;

  while (_table->getNext(srcPin)) {
    if (srcPin->_src == 0)
      continue;

    return srcPin;
  }
  return NULL;
}
bool Ath__qPinTable::startIterator()
{
  if (_table->getCnt() <= 0)
    return false;

  _table->resetIterator();
  return true;
}
uint Ath__qPinTable::getCnt()
{
  return _table->getCnt();
}
Ath__qPin* Ath__qPinTable::get(uint ii)
{
  return _table->get(ii);
}

Ath__qBus::Ath__qBus(uint n)
{
  _table = new Ath__array1D<Ath__p2pConn*>(n);
}
void Ath__qBus::addConn(Ath__p2pConn* conn)
{
  _table->add(conn);
}
uint Ath__qBus::getCnt()
{
  return _table->getCnt();
}
Ath__p2pConn* Ath__qBus::get(uint ii)
{
  return _table->get(ii);
}
