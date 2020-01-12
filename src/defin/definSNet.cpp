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

#include "definSNet.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "create_box.h"
#include "db.h"
#include "dbShape.h"
#include "definPolygon.h"

namespace odb {

inline uint get_net_dbid(const char* name)
{
  if (*name != 'N')
    return 0;

  ++name;

  if (*name == '\0')
    return 0;

  char* end;
  uint  dbid = strtoul(name, &end, 10);

  if (*end != '\0')
    return 0;

  return dbid;
}

definSNet::definSNet()
{
  init();
  _skip_special_wires = false;
  _skip_shields       = false;
  _skip_block_wires   = false;
  _skip_fill_wires    = false;
  _replace_wires      = false;
  _names_are_ids      = false;
}

definSNet::~definSNet()
{
}

void definSNet::init()
{
  definBase::init();
  _snet_cnt        = 0;
  _snet_iterm_cnt  = 0;
  _cur_net         = NULL;
  _cur_layer       = NULL;
  _swire           = NULL;
  _wire_type       = dbWireType::NONE;
  _wire_shape_type = dbWireShapeType::NONE;
  _shield_net      = NULL;
  _prev_x          = 0;
  _prev_y          = 0;
  _prev_ext        = 0;
  _has_prev_ext    = false;
  _width           = 0;
  _point_cnt       = 0;
}

void definSNet::begin(const char* name)
{
  //	notice(0,"Reading special net %s ...\n", name );
  assert(_cur_net == NULL);

  if (!_replace_wires) {
    _cur_net = _block->findNet(name);

    if (_cur_net == NULL)
      _cur_net = dbNet::create(_block, name);

    _cur_net->setSpecial();
  } else {
    if (_names_are_ids == false)
      _cur_net = _block->findNet(name);
    else {
      uint netid = get_net_dbid(name);

      if (netid)
        _cur_net = dbNet::getNet(_block, netid);
      else
        _cur_net = NULL;
    }

    if (_cur_net == NULL) {
      notice(0, "special net %s does not exist\n", name);
      ++_errors;
    } else {
      dbWire* wire = _cur_net->getWire();

      if (wire)
        dbWire::destroy(wire);

      dbSet<dbSWire>           swires = _cur_net->getSWires();
      dbSet<dbSWire>::iterator itr;

      for (itr = swires.begin(); itr != swires.end(); itr = swires.begin()) {
        dbSWire* swire = *itr;
        dbSWire::destroy(swire);
      }
    }

    _cur_net->setSpecial();
  }

  _snet_cnt++;
  _swire = NULL;
}

void definSNet::connection(const char* iname, const char* tname, bool synth)
{
  if ((_cur_net == NULL) || (_replace_wires == true))
    return;

  if (*iname == '*') {
    connect_all(_cur_net, tname);
    return;
  }

  if (iname[0] == 'P' || iname[0] == 'p') {
    if (iname[1] == 'I' || iname[1] == 'i') {
      if (iname[2] == 'N' || iname[2] == 'n') {
        if (iname[3] == 0) {
          if (_block->findBTerm(tname) == NULL) {
            dbBTerm::create(_cur_net, tname);
          }

          return;
        }
      }
    }
  }

  dbInst* inst = _block->findInst(iname);

  if (inst == NULL) {
    notice(0, "error: netlist component (%s) is not defined\n", iname);
    ++_errors;
    return;
  }

  dbMaster* master = inst->getMaster();
  dbMTerm*  mterm  = master->findMTerm(_block, tname);

  if (mterm == NULL) {
    notice(0,
           "error: netlist component-pin (%s, %s) is not defined\n",
           iname,
           tname);
    ++_errors;
    return;
  }

  dbITerm* iterm = dbITerm::connect(inst, _cur_net, mterm);
  _snet_iterm_cnt++;
  iterm->setSpecial();
}

void definSNet::use(defSigType type)
{
  if ((_cur_net == NULL) || (_replace_wires == true))
    return;

  switch (type) {
    case DEF_SIG_ANALOG:
      _cur_net->setSigType(dbSigType::ANALOG);
      break;
    case DEF_SIG_CLOCK:
      _cur_net->setSigType(dbSigType::CLOCK);
      break;
    case DEF_SIG_GROUND:
      _cur_net->setSigType(dbSigType::GROUND);
      break;
    case DEF_SIG_POWER:
      _cur_net->setSigType(dbSigType::POWER);
      break;
    case DEF_SIG_RESET:
      _cur_net->setSigType(dbSigType::RESET);
      break;
    case DEF_SIG_SCAN:
      _cur_net->setSigType(dbSigType::SCAN);
      break;
    case DEF_SIG_SIGNAL:
      _cur_net->setSigType(dbSigType::SIGNAL);
      break;
    case DEF_SIG_TIEOFF:
      _cur_net->setSigType(dbSigType::TIEOFF);
      break;
  }
}

void definSNet::source(defSource source)
{
  if ((_cur_net == NULL) || (_replace_wires == true))
    return;

  switch (source) {
    case DEF_DIST:
      _cur_net->setSourceType(dbSourceType::DIST);
      break;

    case DEF_NETLIST:
      _cur_net->setSourceType(dbSourceType::NETLIST);
      break;

    case DEF_TEST:
      _cur_net->setSourceType(dbSourceType::TEST);
      break;

    case DEF_TIMING:
      _cur_net->setSourceType(dbSourceType::TIMING);
      break;

    case DEF_USER:
      _cur_net->setSourceType(dbSourceType::USER);
      break;
  }
}

void definSNet::weight(int weight)
{
  if ((_cur_net == NULL) || (_replace_wires == true))
    return;

  _cur_net->setWeight(weight);
}

void definSNet::fixedbump()
{
  if ((_cur_net == NULL) || (_replace_wires == true))
    return;

  _cur_net->setFixedBump(true);
}

void definSNet::rect(const char* layer_name, int x1, int y1, int x2, int y2)
{
  if (_swire == NULL)
    return;

  dbTechLayer* layer = _tech->findLayer(layer_name);

  if (layer == NULL) {
    notice(0, "error: undefined layer (%s) referenced\n", layer_name);
    return;
  }

  dbSBox::create(_swire,
                 layer,
                 dbdist(x1),
                 dbdist(y1),
                 dbdist(x2),
                 dbdist(y2),
                 dbWireShapeType::NONE);
}

void definSNet::polygon(const char* layer_name, std::vector<defPoint>& points)
{
  dbTechLayer* layer = _tech->findLayer(layer_name);

  if (layer == NULL) {
    notice(0, "error: undefined layer (%s) referenced\n", layer_name);
    return;
  }

  std::vector<adsPoint> P;
  translate(points, P);
  definPolygon         polygon(P);
  std::vector<adsRect> R;
  polygon.decompose(R);

  std::vector<adsRect>::iterator itr;

  for (itr = R.begin(); itr != R.end(); ++itr) {
    adsRect& r = *itr;
    dbSBox::create(_swire,
                   layer,
                   r.xMin(),
                   r.yMin(),
                   r.xMax(),
                   r.yMax(),
                   dbWireShapeType::NONE);
  }
}

void definSNet::wire(defWireType type, const char* shield)
{
  if (_skip_special_wires)
    return;

  switch (type) {
    case DEF_WIRE_COVER:
      _wire_type = dbWireType::COVER;
      break;
    case DEF_WIRE_FIXED:
      _wire_type = dbWireType::FIXED;
      break;
    case DEF_WIRE_ROUTED:
      _wire_type = dbWireType::ROUTED;
      break;
    case DEF_WIRE_NOSHIELD:
      _wire_type = dbWireType::NOSHIELD;
      break;
    case DEF_WIRE_SHIELD: {
      _wire_type  = dbWireType::SHIELD;
      _shield_net = _block->findNet(shield);

      if (_shield_net == NULL) {
        notice(0, "error: SHIELD net (%s) does not exists.\n", shield);
        _wire_type = dbWireType::NONE;
        // UNCOMMENT Dimitri 10192012   ++_errors;
        ++_errors;
      }
      break;
    }
  }

  _swire = dbSWire::create(_cur_net, _wire_type, _shield_net);
}

void definSNet::path(const char* layer_name, int width)
{
  if (_skip_shields && (_wire_type == dbWireType::SHIELD))
    return;

  _cur_layer = _tech->findLayer(layer_name);

  if (_cur_layer == NULL) {
    notice(0, "error: undefined layer (%s) referenced\n", layer_name);
    ++_errors;
    dbSWire::destroy(_swire);
    _swire = NULL;
    return;
  }

  if (_swire) {
    _prev_x          = 0;
    _prev_y          = 0;
    _prev_ext        = 0;
    _has_prev_ext    = false;
    _point_cnt       = 0;
    _width           = dbdist(width);
    _wire_shape_type = dbWireShapeType::NONE;
  }
}

void definSNet::pathStyle(int style)
{
  notice(0, "error: styles are not supported on wires\n");
  ++_errors;
}

void definSNet::pathShape(const char* shape)
{
  if (_skip_shields && (_wire_type == dbWireType::SHIELD))
    return;

  _wire_shape_type = dbWireShapeType(shape);
}

void definSNet::pathPoint(int x, int y, int ext)
{
  if ((_skip_shields && (_wire_type == dbWireType::SHIELD))
      || (_skip_block_wires && (_wire_shape_type == dbWireShapeType::BLOCKWIRE))
      || (_skip_fill_wires && (_wire_shape_type == dbWireShapeType::FILLWIRE)))
    return;

  int cur_x   = dbdist(x);
  int cur_y   = dbdist(y);
  int cur_ext = dbdist(ext);

  if (_swire) {
    _point_cnt++;

    if (_point_cnt > 1)
      create_box(_swire,
                 _wire_shape_type,
                 _cur_layer,
                 _prev_x,
                 _prev_y,
                 _prev_ext,
                 _has_prev_ext,
                 cur_x,
                 cur_y,
                 ext,
                 true,
                 _width);

    _prev_x       = cur_x;
    _prev_y       = cur_y;
    _prev_ext     = cur_ext;
    _has_prev_ext = true;
  }
}

void definSNet::pathPoint(int x, int y)
{
  if ((_skip_shields && (_wire_type == dbWireType::SHIELD))
      || (_skip_block_wires && (_wire_shape_type == dbWireShapeType::BLOCKWIRE))
      || (_skip_fill_wires && (_wire_shape_type == dbWireShapeType::FILLWIRE)))
    return;

  int cur_x = dbdist(x);
  int cur_y = dbdist(y);

  if (_swire) {
    _point_cnt++;

    if (_point_cnt > 1)
      create_box(_swire,
                 _wire_shape_type,
                 _cur_layer,
                 _prev_x,
                 _prev_y,
                 _prev_ext,
                 _has_prev_ext,
                 cur_x,
                 cur_y,
                 0,
                 false,
                 _width);

    _prev_x       = cur_x;
    _prev_y       = cur_y;
    _prev_ext     = 0;
    _has_prev_ext = false;
  }
}

void definSNet::pathVia(const char* via_name)
{
  if ((_skip_shields && (_wire_type == dbWireType::SHIELD))
      || (_skip_block_wires && (_wire_shape_type == dbWireShapeType::BLOCKWIRE))
      || (_skip_fill_wires && (_wire_shape_type == dbWireShapeType::FILLWIRE)))
    return;

  dbTechVia* tech_via = _tech->findVia(via_name);

  if (tech_via != NULL) {
    if (_swire) {
      _cur_layer = create_via_array(_swire,
                                    _wire_shape_type,
                                    _cur_layer,
                                    tech_via,
                                    _prev_x,
                                    _prev_y,
                                    1,
                                    1,
                                    0,
                                    0);
      if (_cur_layer == NULL)
        _errors++;
    }
  } else {
    dbVia* via = _block->findVia(via_name);

    if (via == NULL) {
      notice(0, "error: undefined ia (%s) referenced\n", via_name);
      ++_errors;
      return;
    }

    if (_swire) {
      _cur_layer = create_via_array(_swire,
                                    _wire_shape_type,
                                    _cur_layer,
                                    via,
                                    _prev_x,
                                    _prev_y,
                                    1,
                                    1,
                                    0,
                                    0);
      if (_cur_layer == NULL)
        _errors++;
    }
  }
}

void definSNet::pathViaArray(const char* via_name,
                             int         numX,
                             int         numY,
                             int         stepX,
                             int         stepY)
{
  if ((_skip_shields && (_wire_type == dbWireType::SHIELD))
      || (_skip_block_wires && (_wire_shape_type == dbWireShapeType::BLOCKWIRE))
      || (_skip_fill_wires && (_wire_shape_type == dbWireShapeType::FILLWIRE)))
    return;

  dbTechVia* tech_via = _tech->findVia(via_name);

  if (tech_via != NULL) {
    stepX = dbdist(stepX);
    stepY = dbdist(stepY);

    if (_swire)
      _cur_layer = create_via_array(_swire,
                                    _wire_shape_type,
                                    _cur_layer,
                                    tech_via,
                                    _prev_x,
                                    _prev_y,
                                    numX,
                                    numY,
                                    stepX,
                                    stepY);
  } else {
    dbVia* via = _block->findVia(via_name);

    if (via == NULL) {
      notice(0, "error: undefined ia (%s) referenced\n", via_name);
      ++_errors;
      return;
    }

    stepX = dbdist(stepX);
    stepY = dbdist(stepY);

    if (_swire)
      _cur_layer = create_via_array(_swire,
                                    _wire_shape_type,
                                    _cur_layer,
                                    via,
                                    _prev_x,
                                    _prev_y,
                                    numX,
                                    numY,
                                    stepX,
                                    stepY);
  }
}

void definSNet::pathEnd()
{
  _cur_layer = NULL;
}

void definSNet::wireEnd()
{
  if (_swire) {
    dbSet<dbSBox> wires = _swire->getWires();

    if (wires.reversible() && wires.orderReversed())
      wires.reverse();
  }

  _swire = NULL;
}

void definSNet::property(const char* name, const char* value)
{
  if ((_cur_net == NULL) || _replace_wires)
    return;

  dbProperty* p = dbProperty::find(_cur_net, name);
  if (p)
    dbProperty::destroy(p);

  dbStringProperty::create(_cur_net, name, value);
}

void definSNet::property(const char* name, int value)
{
  if ((_cur_net == NULL) || _replace_wires)
    return;

  dbProperty* p = dbProperty::find(_cur_net, name);
  if (p)
    dbProperty::destroy(p);

  dbIntProperty::create(_cur_net, name, value);
}

void definSNet::property(const char* name, double value)
{
  if ((_cur_net == NULL) || _replace_wires)
    return;

  dbProperty* p = dbProperty::find(_cur_net, name);

  if (p)
    dbProperty::destroy(p);

  dbDoubleProperty::create(_cur_net, name, value);
}

void definSNet::end()
{
  if (_cur_net == NULL)
    return;

  if (_replace_wires == false) {
    dbSet<dbITerm> iterms = _cur_net->getITerms();

    if (iterms.reversible() && iterms.orderReversed())
      iterms.reverse();

    dbSet<dbProperty> props = dbProperty::getProperties(_cur_net);

    if (!props.empty() && props.orderReversed())
      props.reverse();
  }

  dbSet<dbSWire> swires = _cur_net->getSWires();

  if (swires.reversible() && swires.orderReversed())
    swires.reverse();

  //	notice(0,"End\n");
  _cur_net = NULL;
}

void definSNet::connect_all(dbNet* net, const char* term)
{
  dbSet<dbITerm> iterms = _block->getITerms();

  net->setWildConnected();
  std::map<dbMTerm*, int> matched_mterms;
  std::vector<dbMaster*>  masters;
  _block->getMasters(masters);
  std::vector<dbMaster*>::iterator mitr;

  for (mitr = masters.begin(); mitr != masters.end(); ++mitr) {
    dbMaster* master = *mitr;
    dbMTerm*  mterm  = master->findMTerm(_block, term);

    if (mterm != NULL)
      matched_mterms[mterm] = 1;
  }

  dbSet<dbITerm>::iterator itr;

  for (itr = iterms.begin(); itr != iterms.end(); ++itr) {
    dbITerm* iterm = *itr;

    // notice(0, "iterm %d - I%d %s M %s\n", iterm->getId(),
    // iterm->getInst()->getId(), iterm->getInst()->getConstName(),
    // iterm->getInst()->getMaster()->getConstName());

    dbMTerm* mterm = iterm->getMTerm();

    if (matched_mterms.find(mterm) != matched_mterms.end()) {
      dbITerm::connect(iterm, net);
      iterm->setSpecial();
      _snet_iterm_cnt++;
    }
  }
}

}  // namespace odb
