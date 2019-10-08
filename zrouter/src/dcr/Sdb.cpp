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

#include "ISdb.h" 
#include "Sdb.h"
#include "ZImplements.hpp"
#include "ZFactory.h"
#include <wire.h>
#include "logger.h"

DECLARE_ZFACTORY(Sdb, ISdb)

BEGIN_NAMESPACE_ADS

DECLARE_IMPLEMENTATION(Sdb, ISdb)

Sdb::Sdb()
{
	_search= NULL;
	_idTable= new Ath__array1D<uint>(64000);
}
Sdb::~Sdb()
{
	if (_search!=NULL)
		delete _search;
	_search = NULL;
}
void Sdb::cleanSdb()
{
	if (_search!=NULL)
		delete _search;
	_search = NULL;
}
void Sdb::reMakeSdb(dbTech *tech, dbBlock *block)
{
	cleanSdb();
	initSearchForNets (tech, block);
	adsRect r;
	block->getDieArea(r);
	setMaxArea(r.xMin(), r.yMin(), r.xMax(), r.yMax());
	addPowerNets(block, 11 /*_power_wire_id*/, true);
	addSignalNets(block, 9 /*_signal_wire_id*/, true);
}
void Sdb::setupForBoxes(adsRect maxRect, uint minHeight, uint minWidth)
{
	_search= new Ath__gridTable(&maxRect, 0, 1, 1, minHeight, minWidth);
	ZALLOCATED(_search);
}
void Sdb::addBox(dbBox * bb, uint wtype, uint id)
{
	_search->addBox(bb, wtype, id);
}
uint Sdb::setExtrusionMarker(uint startRow, uint startCol)
{
	return _search->setExtrusionMarker(startRow, startCol);
}
Ath__gridTable* Sdb::getSearchPtr()
{
	return _search;
}
void Sdb::setDefaultWireType(uint v)
{
	_search->setDefaultWireType(v);
}

void Sdb::resetMaxArea()
{
	_search->resetMaxArea();
}
void Sdb::setMaxArea(int x1, int y1, int x2, int y2)
{
	_search->setMaxArea(x1, y1, x2, y2);
}
void Sdb::setExtControl (dbBlock *block, bool useDbSdb, uint adj, uint npsrc, uint nptgt, uint ccUp, bool allNet, uint contextDepth, Ath__array1D<int> **contextArray, uint *contextLength, Ath__array1D<SEQ*> ***dgContextArray, uint *dgContextDepth, uint *dgContextPlanes, uint *dgContextTracks, uint *dgContextBaseLvl, int *dgContextLowLvl, int *dgContextHiLvl, uint *dgContextBaseTrack, int *dgContextLowTrack, int *dgContextHiTrack, int **dgContextTrackBase, AthPool<SEQ> *seqPool)
{
	_search->setExtControl (block, useDbSdb, adj, npsrc, nptgt, ccUp, allNet, contextDepth, contextArray, contextLength, dgContextArray, dgContextDepth, dgContextPlanes, dgContextTracks, dgContextBaseLvl, dgContextLowLvl, dgContextHiLvl, dgContextBaseTrack, dgContextLowTrack, dgContextHiTrack, dgContextTrackBase, seqPool);
}
void Sdb::removeMarkedNetWires()
{
	_search->removeMarkedNetWires();
}
void Sdb::adjustOverlapMakerEnd()
{
	_search->adjustOverlapMakerEnd();
}
void Sdb::adjustMetalFill()
{
	_search->adjustMetalFill();
}
void Sdb::getBox(uint wid, int *x1, int *y1, int *x2, int *y2, uint *level, uint *id1, uint *id2, uint *wtype)
{
	_search->getBox(wid, x1, y1, x2, y2, level, id1, id2, wtype);
}
void Sdb::getCCdist(uint wid, uint *width, uint *level, uint *id1, uint *id2)
{
	_search->getCCdist(wid, width, level, id1, id2);
}
void Sdb::getIds(uint wid, uint *id1, uint *id2, uint *wtype)
{
	_search->getIds(wid, id1, id2, wtype);
}
uint Sdb::addBox(int x1, int y1, int x2, int y2, uint level, uint id1, uint id2, uint wtype)
{
	return _search->addBox(x1, y1, x2, y2, level, id1, id2, wtype);
}
uint Sdb::addBlockages(dbBlock * block)
{
    dbSet<dbObstruction> obstructions = block->getObstructions();

    int bcnt= obstructions.size();

    if ( bcnt == 0 )
        return 0;
    
	uint cnt= 0;
    dbSet<dbObstruction>::iterator obs_itr;

    for( obs_itr = obstructions.begin(); obs_itr != obstructions.end(); ++obs_itr )
    {
        dbObstruction * obs = * obs_itr;

        dbBox * bbox = obs->getBBox();
        uint level= bbox->getTechLayer()->getRoutingLevel();

		_search->addBox(bbox->xMin(), bbox->yMin(), bbox->xMax(), bbox->yMax(),
			level, bbox->getId(), 0, 2);
		cnt ++;
    }
	return cnt;
}
void Sdb::initSearchForNets(dbTech *tech, dbBlock *block)
{
	uint W[16];
	uint S[16];
	uint P[16];
	int X1[16];
	int Y1[16];

	dbSet<dbTechLayer> layers = tech->getLayers();
	dbSet<dbTechLayer>::iterator itr;
	dbTrackGrid *tg = NULL;
  
	adsRect maxRect;
	block->getDieArea( maxRect );

	std::vector<int> trackXY(32000);
	uint n= 0;  
	for ( itr = layers.begin(); itr != layers.end(); ++itr )
	{
		dbTechLayer * layer = *itr;
		dbTechLayerType type = layer->getType();
        
		if ( type.getValue() != dbTechLayerType::ROUTING )
			continue;

		n= layer->getRoutingLevel();
		//W[n]= layer->getWidth();
		W[n]= 1;
		S[n]= layer->getSpacing();
		P[n]= layer->getPitch();
		if (P[n] <= 0) 
			error(0, "Layer %s, routing level %d, has pitch %d !!\n", layer->getConstName(), n, P[n]);
		tg = block->findTrackGrid(layer);
		if (tg)
		{
			tg->getGridX(trackXY);
			X1[n] = trackXY[0] - layer->getWidth()/2;
			tg->getGridY(trackXY);
			Y1[n] = trackXY[0] - layer->getWidth()/2;
		}
		else
		{
			X1[n] = maxRect.xMin();
			Y1[n] = maxRect.yMin();
		}
	}
	uint layerCnt= n+1;

	_search= new Ath__gridTable(&maxRect, 2, layerCnt, W, P, S, X1, Y1);
	_search->setBlock(block);
}
uint Sdb::getViaLevel(dbSBox *s)
{
	dbTechVia *via= s->getTechVia();
	if (via!=NULL) 
	{
		return via->getTopLayer()->getRoutingLevel();
	}
	else 
	{
		dbVia *via= s->getBlockVia();
		return via->getTopLayer()->getRoutingLevel();
	}
}
uint Sdb::getViaLevel(dbShape *s)
{
	dbTechVia *via= s->getTechVia();
	if (via!=NULL) 
	{
		return via->getTopLayer()->getRoutingLevel();
		/*
		dbTechLayer * getTopLayer();
		dbTechLayer * getBottomLayer();
		*/
	}
	else 
	{
		dbVia *via= s->getVia();
		return via->getTopLayer()->getRoutingLevel();
	}
}
uint Sdb::addNetSBoxes(dbNet *net, uint wtype, bool wireVia)
{	
	uint cnt= 0;
	uint sboxCnt= 0;
	dbSet<dbSWire> swires= net->getSWires();
	dbSet<dbSWire>::iterator itr;

	for( itr = swires.begin(); itr != swires.end(); ++itr ) {
        dbSWire * swire = *itr;
	    dbSet<dbSBox> wires= swire->getWires();
        dbSet<dbSBox>::iterator box_itr;
	
		for( box_itr = wires.begin(); box_itr != wires.end(); ++box_itr ) {
			dbSBox * s = *box_itr;
			
			uint level= 0;

			if (s->isVia()) 
			{
				if (wireVia)
					continue;

				level= getViaLevel(s);
			}
			else
			{
				if (!wireVia)
					continue;
				level= s->getTechLayer()->getRoutingLevel();
			}
			sboxCnt ++;
			cnt += _search->addBox(s->xMin(), s->yMin(), s->xMax(), s->yMax(), 
				level, s->getId(), 0, wtype);
        }
	}
	return sboxCnt;
}
void Sdb::addPowerNets(dbBlock *block, uint wtype, bool wireVia)
{
	//uint initCnt= _search->getWireCnt();
	uint cnt= 0;

	dbSet<dbNet> nets = block->getNets();   
    dbSet<dbNet>::iterator net_itr;
	
    for( net_itr = nets.begin(); net_itr != nets.end(); ++net_itr ) {
        dbNet * net = *net_itr;

		if (!((net->getSigType() == dbSigType::POWER) || (net->getSigType() == dbSigType::GROUND)))
			continue;

		cnt += addNetSBoxes(net, wtype, wireVia);
	}
//	uint adjustedMarkerCnt;
	// do marker adjustment by adjustOverlapMakerEnd in netRC.cpp
	// adjustedMarkerCnt= _search->adjustMarkers(0, 1);

	//notice(0, "%d power Sboxes created %d Ath__wires\n", cnt, _search->getWireCnt()-initCnt);
 
}
uint Sdb::addNetShapesOnSearch(dbNet * net, uint wtype, uint vtype)
{	
	dbWire * wire = net->getWire();
	
	if ( wire ==NULL )
		return 0;

	uint cnt= 0;

	//uint wireId= wire->getId();
	
	dbWireShapeItr shapes;
	ads::dbShape s;
	uint ttype;

	for( shapes.begin(wire); shapes.next(s); )
	{				
		uint level= 0;
		
		int shapeId= shapes.getShapeId();			
		
		if (s.isVia()) 
		{
			ttype = vtype;
			level= getViaLevel(&s);
		}
		else
		{
			ttype = wtype;
			level= s.getTechLayer()->getRoutingLevel();
		}
		cnt ++;
		_search->addBox(s.xMin(), s.yMin(), s.xMax(), s.yMax(), 
//DKF 063005				level, wireId, shapeId, ttype);
				level, net->getId(), shapeId, ttype);
	}
	return cnt;
}
void Sdb::addSignalNets(dbBlock *block, uint wtype, uint vtype)
{
	//uint initCnt= _search->getWireCnt();
	uint cnt= 0;
	dbSet<dbNet> nets = block->getNets();   
    dbSet<dbNet>::iterator net_itr;
	
    for( net_itr = nets.begin(); net_itr != nets.end(); ++net_itr ) {
        dbNet * net = *net_itr;

		if ((net->getSigType() == dbSigType::POWER) || (net->getSigType() == dbSigType::GROUND))
			continue;

		cnt += addNetShapesOnSearch(net, wtype, vtype);
	}
	_search->adjustOverlapMakerEnd();
	//notice(0, "%d signal shapes created %d Ath__wires\n", cnt, _search->getWireCnt()-initCnt);
}

uint Sdb::addNetShapesOnSearch(dbNet * net, uint wtype, bool wireVia)
{	
	dbWire * wire = net->getWire();
	
	if ( wire ==NULL )
		return 0;

	uint cnt= 0;

	//uint wireId= wire->getId();
	
	dbWireShapeItr shapes;
	ads::dbShape s;
	
	for( shapes.begin(wire); shapes.next(s); )
	{				
		uint level= 0;
		
		int shapeId= shapes.getShapeId();			
		
		if (s.isVia()) 
		{
			if (wireVia)
				continue;
			
			level= getViaLevel(&s);
		}
		else
		{
			if (!wireVia)
				continue;

			level= s.getTechLayer()->getRoutingLevel();
		}
		cnt ++;
		_search->addBox(s.xMin(), s.yMin(), s.xMax(), s.yMax(), 
//DKF 063005				level, wireId, shapeId, wtype);
				level, net->getId(), shapeId, wtype);
	}
	return cnt;
}
void Sdb::addSignalNets(dbBlock *block, uint wtype, bool wireVia)
{
	//uint initCnt= _search->getWireCnt();
	uint cnt= 0;
	dbSet<dbNet> nets = block->getNets();   
    dbSet<dbNet>::iterator net_itr;
	
    for( net_itr = nets.begin(); net_itr != nets.end(); ++net_itr ) {
        dbNet * net = *net_itr;

		if ((net->getSigType() == dbSigType::POWER) || (net->getSigType() == dbSigType::GROUND))
			continue;

		cnt += addNetShapesOnSearch(net, wtype, wireVia);
	}
	_search->adjustOverlapMakerEnd();
	//notice(0, "%d signal shapes created %d Ath__wires\n", cnt, _search->getWireCnt()-initCnt);
}

void Sdb::searchBoxIds(int x1, int y1, int x2, int y2)
{
	_idTable->resetCnt(0);
//	_areaBb.set(x1, y1, x2, y2);

	if (_search==NULL)
		return;
	
	_search->search(x1, y1, x2, y2, 0, 0, _idTable, true);
}
void Sdb::dumpTrackCounts(FILE *fp)
{
	if (_search==NULL)
		return;
	_search->dumpTrackCounts(fp);
}

void Sdb::searchWireIds(int x1, int y1, int x2, int y2, bool ignoreLayers, bool *exludeTable)
{
	_idTable->resetCnt(0);
//	_areaBb.set(x1, y1, x2, y2);

	if (_search==NULL)
		return;

	for (uint dir= 0; dir<_search->getRowCnt(); dir++) 
	{
		for (uint layer= 1; layer<_search->getColCnt(); layer++) 
		{
			if (! ignoreLayers && ((exludeTable!=NULL) && exludeTable[layer]) )
				continue;
			
		//	_areaBb.set(layer,dir);
			_search->search(x1, y1, x2, y2, dir, layer, _idTable, true);
		}
	}
}
uint Sdb::makeGuiBoxes(ZPtr<IZdcr> dcr, uint menuId, uint subMenuId, bool useSdbId, uint excludeId1)
{
/*
	if (! ignoreZuiFlags && ! zui->getHierBoxFlag(hier, box))
		return 0;

	bool wireVia= ! zui->boxType_via(box);
	bool signal= ! zui->hierType_power(hier);
*/
	uint cnt= 0;
	
	_idTable->resetIterator();
	uint id= 0;
	while (_idTable->getNext(id))
	{
		int x1, y1, x2, y2;
		uint level, id1, id2, wtype;
		
	_search->getBox(id, &x1, &y1, &x2, &y2, &level, &id1, &id2, &wtype);
		
		if (subMenuId != wtype)
			continue;

		if (excludeId1 == id1)
			continue;
		
		if (useSdbId)
			cnt += dcr->addBox(id, subMenuId, menuId, level, x1, y1, x2, y2, 0);
		else
			cnt += dcr->addBox(id1, subMenuId, menuId, level, x1, y1, x2, y2, id2);
	}
	return cnt;
}
bool Sdb::getNextBox(int &x1, int &y1, int &x2, int &y2, uint &level, uint &netId, uint &shapeId)
{
	uint id;
	if (!_idTable->getNext(id))
		return false;

	uint wtype;
	_search->getBox(id, &x1, &y1, &x2, &y2, &level, &netId, &shapeId, &wtype);

	return true;
}

uint Sdb::getNextWireId()
{
	uint id;
	if (!_idTable->getNext(id))
		return 0;
	return id;
}
uint Sdb::startIterator()
{
	_idTable->resetIterator();
	return 0;
}
	
uint Sdb::searchSpread(void *ip, uint spreadTrackDist, std::vector<dbNet *> & inets, char *bbox, void (*doSpread)(void *, uint, void *, void *, void *))
{
	return _search->searchSpread(ip, spreadTrackDist, inets, bbox, doSpread);
}


uint Sdb::couplingCaps(ZPtr<ISdb> ccSdb, uint couplingDist, ZInterface *Interface, void (*coupleAndCompute)(int *, void *), void *computePtr)
{
	if (ccSdb==NULL)
		return _search->couplingCaps(NULL, couplingDist, Interface, _idTable, coupleAndCompute, computePtr);
	else 
		return _search->couplingCaps(ccSdb->getSearchPtr(), couplingDist, Interface, _idTable, coupleAndCompute, computePtr);
}
END_NAMESPACE_ADS

