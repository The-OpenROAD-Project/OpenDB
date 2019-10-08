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

#include <vector>
#include <map>

#ifdef _WIN32
#include "direct.h"
#endif

#include "layout.h"
#include "tcl.h"
#include <z2tcl.h>
#include "tech.h"
#include "../lefin/lefin.h"
#include "../defin/defin.h"
#include "lefout.h"
#include "dbShape.h"

USING_NAMESPACE_ADS;

dbBlockHandle::dbBlockHandle(dbBlock *blk, dbTech *tech, uint id, uint schema)
{
	_block= blk;
	_tech= tech;
	_schema= schema;
	_id= id;

	_quad= NULL;
	_dbInstSearch= NULL;
	_dbNetWireSearch= NULL;
	_dbTrackSearch= NULL;

	_netSdb= NULL;
	_instSdb= NULL;

	_grGcellSearch= NULL;
	_grGpathSearch= NULL;
}
bool dbBlockHandle::connect2BlockSdb()
{
	// _instSdb= _block->getSearchDbInsts(); // ISDB
	// _netSdb= _block->getSearchDbNets(); // ISDB

	return false;
}
bool dbBlockHandle::setInterface2SdbAtDB()
{
	_dbInstSearch= _instSdb->getSearchPtr(); // ISDB
//	_dbNetWireSearch= _netSdb->getSearchPtr(); // ISDB

	return false;
}

uint dbBlockHandle::makeInstSearchDb()
{
	uint maxInt= 2100000000;
	uint minWidth= maxInt;
	uint minHeight= maxInt;

	dbSet<dbInst> insts = _block->getInsts();
        
    dbSet<dbInst>::iterator inst_itr;
	
	//dbBox *maxBox= _block->getBBox();
	adsRect maxRect;
	_block->getDieArea( maxRect );

	maxRect.reset(maxInt, maxInt, -maxInt, -maxInt) ;
	//maxBox->getBox(maxRect);

	uint instCnt= 0;
    for( inst_itr = insts.begin(); inst_itr != insts.end(); ++inst_itr )
    {
        dbInst * inst = *inst_itr;
        
		if (inst->getMaster()->getMTermCount()<=0)
			continue;


        dbBox * bb = inst->getBBox();

		minWidth=  ath__min(minWidth,  bb->getDX());
		minHeight= ath__min(minHeight, bb->getDY());

		adsRect r;
		bb->getBox(r);
		maxRect.merge(r);
		instCnt ++;
	}
	_dbInstSearch= NULL;
	if (instCnt==0)
		return instCnt;

//TODO	uint rowSize= 100 * minHeight;
//TODO	uint colSize= 100 * minWidth;
	
	_dbInstSearch= new Ath__gridTable(&maxRect, 0, 1, 1, minHeight, minWidth);
	
    for( inst_itr = insts.begin(); inst_itr != insts.end(); ++inst_itr )
    {
        dbInst * inst = *inst_itr;
		if (inst->getMaster()->getType()==dbMasterType::CORE_FEEDTHRU)
			continue;
		if (inst->getMaster()->getMTermCount()<=0)
			continue;

        dbBox * ibox = inst->getBBox();
        _dbInstSearch->addBox(ibox, 0, 0);
	}
	uint adjustedMarkerCnt= _dbInstSearch->setExtrusionMarker(0, 0);
	fprintf(stdout, "Added %d instance boxes on Search DB\n", 
		adjustedMarkerCnt);

	return instCnt;
}
uint dbBlockHandle::makeTrackSearchDb()
{
	adsRect maxRect;
	_block->getDieArea( maxRect );

	uint W[16];
	uint S[16];
	uint P[16];

    dbSet<dbTechLayer> layers = _tech->getLayers();
    dbSet<dbTechLayer>::iterator itr;
  
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
	}
	uint layerCnt= n+1;

	_dbTrackSearch= new Ath__gridTable(&maxRect, 2, layerCnt, W, P, S);
	uint cnt= 0;

	dbSet<dbTechLayer>::iterator litr;
	dbTechLayer *layer;
	for (litr = layers.begin(); litr != layers.end(); ++litr) 
	{
		layer = *litr;
        if ( layer->getType() != dbTechLayerType::ROUTING )
			continue;
		
		dbTrackGrid *g= _block->findTrackGrid(layer);
		if (g==NULL)
			continue;

		uint level= layer->getRoutingLevel();

		int lo[2]= { maxRect.xMin(), maxRect.yMin() };
		int hi[2]= { maxRect.xMax(), maxRect.yMax() };

		cnt += addTracks(g, 1, level, lo, hi);

		lo[0]= maxRect.xMin();
		lo[1]= maxRect.yMin();
		hi[0]= maxRect.xMax();
		hi[1]= maxRect.yMax();

		cnt += addTracks(g, 0, level, lo, hi);
	}
	return cnt;
}
uint dbBlockHandle::addTracks(dbTrackGrid *g, uint dir, uint level, int *lo, int *hi)
{
	std::vector<int> trackXY(32000);

	if (dir>0) // horizontla
		g->getGridY(trackXY);
	else
		g->getGridX(trackXY);
	
	uint cnt= 0;
	for( uint ii= 0; ii<trackXY.size(); ii++)
	{
		int xy= trackXY[ii];
		lo[dir]= xy;
		hi[dir]= xy+1;
		cnt ++;
		
		Ath__searchBox bb;
		bb.set(lo[0], lo[1], hi[0], hi[1], level, -1);
		bb.setOwnerId(xy, 0);
		
		_dbTrackSearch->getGrid(dir, level)->placeWire(&bb);
		cnt ++;
	}
	return cnt;
}

uint dbBlockHandle::getShapeCoords(dbShape *s, Ath__searchBox *bb, uint id1, uint id2, uint level)
{
	bb->set(s->xMin(), s->yMin(), s->xMax(), s->yMax(), level, -1);
	bb->setOwnerId(id1, id2);
	return bb->getDir();
}
dbNet* dbBlockHandle::getShapeCoords(bool wireVia, Ath__searchBox *bb, uint wireId, uint shapeId)
{
	//wireVia=true, wire
	if (shapeId>0) 
	{
		dbShape s;
		dbWire * w = dbWire::getWire(_block, wireId);
		w->getShape( shapeId, s );

		int level= getShapeLevel(&s, wireVia);
		if (level<0)
			return NULL;
		
		bb->set(s.xMin(), s.yMin(), s.xMax(), s.yMax(), level, -1);
		bb->setOwnerId(wireId, shapeId);

		dbNet *net= w->getNet();
		return net;
	}
	else 
	{
		dbSBox * s= dbSBox::getSBox(_block, wireId);

		int level= getShapeLevel(s, wireVia);
		if (level<0)
			return NULL;

		bb->set(s->xMin(), s->yMin(), s->xMax(), s->yMax(), level, -1);
		bb->setOwnerId(wireId, shapeId);

		dbSWire *sw= (dbSWire *) s->getBoxOwner();
		dbNet *net= sw->getNet();
		return net;
	}
}
int dbBlockHandle::getShapeLevel(ads::dbSBox *s, bool wireVia)
{	
	if (s->isVia())
	{
		if (wireVia) // request for wire
			return -1;
		
		return getViaLevel(s);
	}
	else 
	{
		if (!wireVia) // request for via
			return -1;

		return s->getTechLayer()->getRoutingLevel();
	}
}
int dbBlockHandle::getShapeLevel(ads::dbShape *s, bool wireVia)
{	
	if (s->isVia())
	{
		if (wireVia) // request for wire
			return -1;
		
		return getViaLevel(s);
	}
	else 
	{
		if (!wireVia) // request for via
			return -1;

		return s->getTechLayer()->getRoutingLevel();
	}
}
uint dbBlockHandle::getViaLevel(ads::dbSBox *s)
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
		dbVia *via= s->getBlockVia();
		return via->getTopLayer()->getRoutingLevel();
	}
}
uint dbBlockHandle::getViaLevel(ads::dbShape *s)
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
void dbBlockHandle::getNetSBoxesFromSearch(dbNet *net, Ath__array1D<uint> *idTable, Ath__searchBox *maxBox)
{	
	dbSet<dbSWire> swires= net->getSWires();
	dbSet<dbSWire>::iterator itr;
	for( itr = swires.begin(); itr != swires.end(); ++itr ) {
        dbSWire * swire = *itr;
	    dbSet<dbSBox> wires= swire->getWires();
        dbSet<dbSBox>::iterator box_itr;
	
		for( box_itr = wires.begin(); box_itr != wires.end(); ++box_itr ) {
			dbSBox * s = *box_itr;
			
			uint boxId= s->getId();
			
			uint level= 0;

			if (s->isVia()) 
				level= getViaLevel(s);
			else
				level= s->getTechLayer()->getRoutingLevel();
			
			Ath__searchBox sbb;
			sbb.set(s->xMin(), s->yMin(), s->xMax(), s->yMax(), level, -1);
			sbb.setOwnerId(boxId, 0);
			
			if (maxBox!=NULL)
				maxBox->setMaxBox(&sbb);

			uint dir= sbb.getDir();
						
			_dbNetWireSearch->getGrid(dir, level)->search(&sbb, idTable, true);
        }
	}
}
void dbBlockHandle::getNetShapesFromSearch(dbNet * net, Ath__array1D<uint> *idTable, Ath__searchBox *maxBox)
{	
        dbWire * wire = net->getWire();

        if ( wire != NULL )
        {
		uint wireId= wire->getId();

		dbWireShapeItr shapes;
		ads::dbShape s;
		
		for( shapes.begin(wire); shapes.next(s); )
		{				
			uint level= 0;

			int shapeId= shapes.getShapeId();			
			Ath__searchBox bb;
			uint dir= getShapeCoords(&s, &bb, wireId, shapeId, level);

			if (s.isVia()) 
				level= getViaLevel(&s);
			else
				level= s.getTechLayer()->getRoutingLevel();
			
			if (maxBox!=NULL)
				maxBox->setMaxBox(&bb);
			
			_dbNetWireSearch->getGrid(dir, level)->search(&bb, idTable, true);
		}
	}
}
void dbBlockHandle::addNetShapesOnSearch(dbNet * net, bool skipVias)
{	
        dbWire * wire = net->getWire();

        if ( wire != NULL )
        {
		uint wireId= wire->getId();

		dbWireShapeItr shapes;
		ads::dbShape s;
		
		for( shapes.begin(wire); shapes.next(s); )
		{				
			uint level= 0;

			int shapeId= shapes.getShapeId();			
			Ath__searchBox bb;
			uint dir= getShapeCoords(&s, &bb, wireId, shapeId, level);

			if (s.isVia()) 
			{
				if (skipVias)
					continue;

				level= getViaLevel(&s);
			}
			else
				level= s.getTechLayer()->getRoutingLevel();

			_dbNetWireSearch->getGrid(dir, level)->placeWire(&bb);
		}
	}
}
uint dbBlockHandle::addBlockagesOnSearch( dbBlock * block )
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

        //dbInst * inst = obs->getInstance();
        //if ( obs->isSlotObstruction() )
        //if ( obs->isFillObstruction() )            
        //if ( obs->isPushedDown() )

        dbBox * bbox = obs->getBBox();
        uint level= bbox->getTechLayer()->getRoutingLevel();
        Ath__searchBox bb(bbox->xMin(), bbox->yMin(), bbox->xMax(), bbox->yMax(), level);
		bb.setOwnerId(bbox->getId(), 0);
		bb.setType(2); // 2 for obstructions
		uint dir= bb.getDir();
		
		_dbNetWireSearch->getGrid(dir, level)->placeWire(&bb);
		cnt ++;
    }
	return cnt;
}
void dbBlockHandle::makeNetWireSearchDb()
{
	uint W[16];
	uint S[16];
	uint P[16];

    dbSet<dbTechLayer> layers = _tech->getLayers();
    dbSet<dbTechLayer>::iterator itr;
  
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
		//P[n]= layer->getPitch();
		P[n]= layer->getWidth() + layer->getSpacing();
	}
	uint layerCnt= n+1;

	adsRect maxRect;
	_block->getDieArea( maxRect );

	_dbNetWireSearch= new Ath__gridTable(&maxRect, 2, layerCnt, W, P, S);

	//addBlockagesOnSearch( _block );

	dbSet<dbNet> nets = _block->getNets();   
    dbSet<dbNet>::iterator net_itr;
	
	bool skipVias= false;
    for( net_itr = nets.begin(); net_itr != nets.end(); ++net_itr ) {
        dbNet * net = *net_itr;

		addNetShapesOnSearch(net, skipVias);
		addNetSBoxesOnSearch(net, skipVias);
	}
	_dbNetWireSearch->setExtrusionMarker(0, 1);

//	fprintf(stdout, "Added %d net shapes and vias on Search DB\n", 
//		adjustedMarkerCnt);
}
uint dbBlockHandle::getBlockBox(Ath__zui *zui, Ath__hierType hier, uint level, uint dd, bool ignoreFlag)
{
	if (!ignoreFlag && !zui->getDbFlag("block/bbox"))
		return 0;

	// dbBox *bb= handle->_block->getBBox();
	adsRect s;
	_block->getDieArea( s );

	return zui->addWireShape(s.xMin()+dd, s.yMin()+dd, s.xMax()-dd, s.yMax()-dd, 
					level, hier, Ath_box__bbox, _id, _id);
}
uint dbBlockHandle::addBtermBoxes(Ath__zui *zui, dbBTerm *bterm, Ath__hierType hier)
{
	ads::dbShape s;
	if (! bterm->getFirstPin(s)) // TWG: added bpins
		return 0;
	
	return zui->addWireShape(s.xMin(), s.yMin(), s.xMax(), s.yMax(), 
		s.getTechLayer()->getRoutingLevel(),
		hier, Ath_box__pin, bterm->getId(), _id);
}
uint dbBlockHandle::addBtermBoxes(Ath__zui *zui, dbSet<dbBTerm> &bterms, Ath__hierType hier, bool ignoreFlag)
{
    if ( bterms.size() == 0 )
        return 0;

	if (!ignoreFlag && !zui->getDbFlag("block/pin"))
		return 0;
      
	uint cnt= 0;
    dbSet<dbBTerm>::iterator itr;
    for( itr = bterms.begin(); itr != bterms.end(); ++itr )
    {
        dbBTerm * bterm = *itr;
		cnt += addBtermBoxes(zui, bterm, hier);
    }
	return cnt;
}
uint dbBlockHandle::getBtermBoxes(Ath__zui *zui, Ath__hierType hier, bool ignoreFlag)
{
    dbSet<dbBTerm> bterms = _block->getBTerms();	
	return addBtermBoxes(zui, bterms, hier, ignoreFlag);
}
uint dbBlockHandle::getItermShapes(Ath__zui *zui, dbITerm * iterm, Ath__hierType hier, bool viaFlag)
{	
	dbInst *inst= iterm->getInst();
	uint cnt= 0;
	
	dbITermShapeItr term_shapes;
	
	ads::dbShape s;
	for( term_shapes.begin(iterm); term_shapes.next(s); )
	{
		if (s.isVia()) {
			continue;
		}
		else {
			
			if (viaFlag)
				continue;
			
			cnt += zui->addWireShape(s.xMin(), s.yMin(), s.xMax(), s.yMax(), 
				s.getTechLayer()->getRoutingLevel(),
				hier, Ath_box__pin, iterm->getId(), inst->getId());
		}
	}
	return cnt;
}
uint dbBlockHandle::getItermShapes(Ath__zui *zui, dbInst *inst, Ath__hierType hier, bool viaFlag)
{	
	uint cnt= 0;
	dbSet<dbITerm> iterms = inst->getITerms();
	
	dbSet<dbITerm>::iterator iterm_itr;
	
	for( iterm_itr = iterms.begin(); iterm_itr != iterms.end(); ++iterm_itr )
	{
		dbITerm * iterm = *iterm_itr;
		
		cnt += getItermShapes(zui, iterm, hier, viaFlag);
	}
	return cnt;
}
uint dbBlockHandle::getInstObs(Ath__zui *zui, dbInst *inst, Ath__hierType hier, bool viaFlag)
{	
	uint cnt= 0;
	
	dbInstShapeItr obs_shapes;
	
	ads::dbShape s;
	for( obs_shapes.begin(inst, dbInstShapeItr::OBSTRUCTIONS); obs_shapes.next(s); )
	{
		if (s.isVia()) 
		{
			continue;
		}
		else {
			
			if (viaFlag)
				continue;
			
			cnt += zui->addWireShape(s.xMin(), s.yMin(), s.xMax(), s.yMax(), 
				s.getTechLayer()->getRoutingLevel(),
				hier, Ath_box__obs, inst->getId(), inst->getId());
		}
	}
	return cnt;
}
uint dbBlockHandle::addInstShapes(Ath__zui *zui, dbInst *inst, Ath__hierType hier, bool vias, bool pinFlag, bool obsFlag)
{
	uint cnt= 0;
	if (pinFlag) 
	{
		cnt += getItermShapes(zui, inst, hier, false);
		if (vias)
			cnt += getItermShapes(zui, inst, hier, true);
	}
	if (obsFlag)
	{
		cnt += getInstObs(zui, inst, hier, false);
		if (vias)
			cnt += getInstObs(zui, inst, hier, true);
	}
	return cnt;
}
void dbBlockHandle::getInstShapes(Ath__zui *zui, Ath__hierType hier, bool vias, bool pins, bool obs)
{	
	if (_dbInstSearch==NULL)
		return;

	bool obsFlag= obs  || zui->getDbFlag("inst/obs");
	bool pinFlag= pins || zui->getDbFlag("inst/pin");

	if (!(obsFlag || pinFlag))
		return;

	uint cnt= 0;

	Ath__searchBox bb(zui->getBbox(), 0, 0);
	_dbInstSearch->search(&bb, 0, 0, zui->getIdTable(), false); //single grid
	
	while (uint boxId= zui->getNextId()) {
		dbBox *bb= dbBox::getBox(_block, boxId);
		
		dbInst *inst= (dbInst *) bb->getBoxOwner();

		cnt += addInstShapes(zui, inst, hier, vias, pinFlag, obsFlag);
	}
}
uint dbBlockHandle::getBlockObs(Ath__zui *zui, bool ignoreFlag)
{
	if (!ignoreFlag && !zui->getDbFlag("block/obs"))
		return 0;

	getSearchWireIds(zui);

	uint cnt= 0;
	while (uint id= zui->getNextId()) {

		uint dbId= getDbBoxId(id, 2);
		if (dbId==0)
			continue;

		dbBox *bb= dbBox::getBox(_block, dbId);

		cnt += zui->addBox(bb, Ath_box__obs, 0, Ath_hier__block);
	}
	return cnt;
}
void dbBlockHandle::getInstBoxes(Ath__zui *zui, Ath__hierType hier, bool ignoreFlag)
{
	if (!ignoreFlag && !zui->getDbFlag("inst/bbox"))
		return;
	
	if (_dbInstSearch==NULL)
		return;
	
	Ath__searchBox bb(zui->getBbox(), 0, 0);
	zui->getIdTable()->resetCnt();
	_dbInstSearch->search(&bb, 0, 0, zui->getIdTable(), false); //single grid
	
	while (uint boxId= zui->getNextId()) {
		dbBox *bb= dbBox::getBox(_block, boxId);
		zui->addBox(bb, Ath_box__bbox, 0);
	}	
}
void dbBlockHandle::white(Ath__zui *zui, Ath__hierType hier, bool ignoreFlag)
{
	if (!ignoreFlag && !zui->getDbFlag("inst/white"))
		return;
	
	if (_dbInstSearch==NULL)
		return;
	
	Ath__searchBox bb(zui->getBbox(), 0, 0);
	zui->getIdTable()->resetCnt();
	_dbInstSearch->white(&bb, 0, 0, zui->getIdTable(), NULL); //single grid
	
	while (uint id= zui->getNextId()) 
	{
		Ath__wire *w= _dbInstSearch->getWirePtr(id);

		Ath__searchBox bb;
		w->getCoords(&bb);
		bb.setLevel(5);
		
		zui->addBox(&bb, Ath_hier__block, Ath_box__white, id);

		_dbInstSearch->releaseWire(id);
	}	
}
void dbBlockHandle::getWireIds(Ath__array1D<uint> *wireIdTable, Ath__array1D<uint> *idtable)
{	
	// remove duplicate entries
	
	for (uint ii= 0; ii<wireIdTable->getCnt(); ii++) 
	{
		uint wid= wireIdTable->get(ii);
		Ath__wire *w= _dbNetWireSearch->getWirePtr(wid);
		
		uint srcId= w->getSrcId();
		if (srcId>0) {
			w= _dbNetWireSearch->getWirePtr(srcId);
		}
		if (w->_ext>0)
			continue;
		
		w->_ext= 1;
		idtable->add(w->_id);
	}
	
	for (uint jj= 0; jj<wireIdTable->getCnt(); jj++) 
	{
		Ath__wire *w= _dbNetWireSearch->getWirePtr( wireIdTable->get(jj) );
		
		w->_ext= 0;
	}
}
uint dbBlockHandle::getDbBoxId(uint wid, uint wireType)
{
	Ath__wire *w= _dbNetWireSearch->getWirePtr(wid);
	if (w->_flags!=wireType)
		return 0;

	return w->getBoxId();
}

dbNet* dbBlockHandle::getNetAndCoords(Ath__searchBox *bb, bool wireVia, bool signal, uint wid)
{
	Ath__wire *w= _dbNetWireSearch->getWirePtr(wid);
	if (w->_flags!=0)
		return NULL;

	dbNet *net= getShapeCoords(wireVia, bb, w->getBoxId(), w->getOtherId());

	if (net==NULL)
		return NULL;

	dbSigType type= net->getSigType();
	bool powerFlag= (type==dbSigType::POWER)||(type==dbSigType::GROUND);

				
	if (signal) {
		if (powerFlag)
			return NULL;
	}
	else if (! powerFlag) {
		return NULL;
	}

	bb->setOwnerId(net->getId());
	
	return net;
}
uint dbBlockHandle::getWireVias(Ath__zui *zui, bool ignoreZuiFlags,
								Ath__hierType hier, Ath__boxType box, dbNet* targetNet, bool excludeFlag)
{
	if (! ignoreZuiFlags && ! zui->getHierBoxFlag(hier, box))
		return 0;

	bool wireVia= ! zui->boxType_via(box);
	bool signal= ! zui->hierType_power(hier);

	zui->resetIdTableIterator();

	uint cnt= 0;
	while (uint id= zui->getNextId()) {

		Ath__searchBox bb;
		dbNet* net= getNetAndCoords(&bb, wireVia, signal, id);

		if (net==NULL)
			continue;

		if (targetNet!=NULL) 
		{
			if ( excludeFlag && (net==targetNet))
				continue;
			if ( !excludeFlag && (net!=targetNet))
				continue;
		}
		cnt += zui->addBox(&bb, hier, box, id);
	}
	return cnt;
}

void dbBlockHandle::getPowerWires(Ath__zui *zui, Ath__hierType hier, uint layer)
{
	if (_dbNetWireSearch==NULL)
		return;
	
	if (zui->isLayerExluded(layer))
		return;

	//bool skipVias= ! zui->getDbFlag("power/via");

	zui->getIdTable()->resetCnt();

	for (uint dir= 0; dir<2; dir++) 
	{
		Ath__searchBox bb(zui->getBbox(), layer, dir);
		_dbNetWireSearch->search(&bb, dir, layer, zui->getIdTable(), true); //single grid			
	}
	getWireVias(zui, false, hier, Ath_box__wire);
/*
	while (uint id= zui->getNextId()) {

		Ath__searchBox bb;
		dbNet* net= getNetAndCoords(&bb, skipVias, id);
		if (net==NULL)
			continue;

		dbSigType type= net->getSigType();
				
		if (!((type==dbSigType::POWER)||(type==dbSigType::GROUND)))
			continue;
		
		zui->addBox(&bb, Ath_hier__power, Ath_box__wire, id);
	}
*/
}
void dbBlockHandle::getPowerWires(Ath__zui *zui, Ath__hierType hier, bool ignoreFlag)
{
	if (_dbNetWireSearch==NULL)
		return;
	
	if (!ignoreFlag && !zui->getDbFlag("power/wire"))
		return;
	
	for (uint layer= 1; layer<_dbNetWireSearch->getColCnt(); layer++) 
	{
		getPowerWires(zui, hier, layer);
	}	
}
void dbBlockHandle::getSignalWireIds(Ath__zui *zui, bool ignoreLayers)
{		
	for (uint dir= 0; dir<2; dir++) 
	{
		for (uint layer= 1; layer<_dbNetWireSearch->getColCnt(); layer++) 
		{
			if (! ignoreLayers && zui->isLayerExluded(layer))
				continue;

			Ath__searchBox bb(zui->getBbox(), layer, dir);
			_dbNetWireSearch->search(&bb, dir, layer, zui->getIdTable(), true); //single grid
		}
	}
}

uint dbBlockHandle::getTracks(Ath__zui *zui, bool ignoreLayers)
{
	if (_dbTrackSearch==NULL)
		return 0;
	
	if (! zui->getDbFlag("block/track") )
		return 0;

	zui->getIdTable()->resetCnt();
	for (uint dir= 0; dir<2; dir++) 
	{
		for (uint layer= 1; layer<_dbNetWireSearch->getColCnt(); layer++) 
		{
			if (! ignoreLayers && zui->isLayerExluded(layer))
				continue;

			Ath__searchBox bb(zui->getBbox(), layer, dir);
			_dbTrackSearch->search(&bb, dir, layer, zui->getIdTable(), true); //single grid
		}
	}
	while (uint id= zui->getNextId()) 
	{
		Ath__wire *w= _dbTrackSearch->getWirePtr(id);


		Ath__searchBox bb;
		w->getCoords(&bb);
		bb.setLevel(15);
		
		zui->addBox(&bb, Ath_hier__block, Ath_box__track, id);
	}
	return zui->getIdTable()->getCnt();
}
void dbBlockHandle::getSearchWireIds(Ath__zui *zui, bool ignoreLayers)
{		
	if (_dbNetWireSearch==NULL)
		return;
	
	uint maxObjects= 0;
	uint *gxy= zui->getGxy(&maxObjects);
	
	for (uint dir= 0; dir<2; dir++) 
	{
		for (uint layer= 1; layer<_dbNetWireSearch->getColCnt(); layer++) 
		{
			if (! ignoreLayers && zui->isLayerExluded(layer))
				continue;

			Ath__searchBox bb(zui->getBbox(), layer, dir);

			if (gxy[0]==0)
				_dbNetWireSearch->search(&bb, dir, layer, zui->getIdTable(), true); //single grid
			else
				_dbNetWireSearch->getGrid(dir, layer)->search(&bb, gxy, zui->getIdTable(), NULL);
		}
	}
}
/*
void dbBlockHandle::getSignalWiresMinusNet(Ath__zui *zui, Ath__hierType hier, dbNet* targetNet)
{	
	while (uint id= zui->getNextId()) {

		Ath__searchBox bb;
		dbNet *net= getNetAndCoords(&bb, false, id);

		if (net==targetNet)
			continue;
		
		zui->addBox(&bb, Ath_hier__signal, Ath_box__wire, id);
	}	
}
*/
/*
void dbBlockHandle::getSignalWires(Ath__zui *zui, Ath__hierType hier, dbNet* targetNet)
{	
	while (uint id= zui->getNextId()) {

		Ath__searchBox bb;
		dbNet *net= getNetAndCoords(&bb, false, id);

		if (net!=targetNet)
			continue;
		
		zui->addBox(&bb, Ath_hier__signal, Ath_box__wire, id);
	}	
}
*/
uint dbBlockHandle::getWiresAndVias_all(Ath__zui *zui, dbNet *targetNet, bool ignoreFlag)
{
	if (_dbNetWireSearch==NULL)
		return 0;

	if (! zui->getNetWireViasFlag(ignoreFlag))
		return 0;

	zui->getIdTable()->resetCnt();
	getSearchWireIds(zui);
	
	uint cnt= 0;
	
	cnt += getWireVias(zui, ignoreFlag, Ath_hier__power, Ath_box__wire, targetNet);
	cnt += getWireVias(zui, ignoreFlag, Ath_hier__power, Ath_box__via, targetNet);
	cnt += getWireVias(zui, ignoreFlag, Ath_hier__signal, Ath_box__wire, targetNet);
	cnt += getWireVias(zui, ignoreFlag, Ath_hier__signal, Ath_box__via, targetNet);
	
	return cnt;
}
uint dbBlockHandle::getTileBuses(Ath__zui *zui)
{
	if (_quad==NULL)
		return 0;

	uint cnt= 0;
	if (zui->getDbFlag("tile/tnet/bus"))
		cnt += _quad->getTileBuses_1(zui);

	return cnt;
}
uint dbBlockHandle::getTilePins(Ath__zui *zui)
{
	if (_quad==NULL)
		return 0;

	uint cnt= 0;
	if (zui->getDbFlag("tile/tnet/pin"))
		cnt += _quad->getTilePins_1(zui);

	return cnt;
}
uint Ath__zroute::mkSearchDb(dbChip *chip, bool isdb)
{
	// assume all defs have been read in.
	if (_handleCnt>0)
		return _handleCnt;

	if (chip==NULL)
		chip= _new_chip;

	dbBlock *topBlock= chip->getBlock();
	if (topBlock==NULL)
		return 0;

	_handleCnt= 0;
	if ((! _skipTopBlock) || (_topBlockOnly) ) {
		_handleTable[1]= new dbBlockHandle(topBlock, _new_tech, 1, _dbSchema);
		_handleCnt= 1;
		
	}
	if (! _topBlockOnly ) {
		
		dbSet<dbBlock> blocks= topBlock->getChildren();
		dbSet<dbBlock>::iterator itr;
		
		for (itr= blocks.begin(); itr!=blocks.end(); ++itr) {
			dbBlock *blk= *itr;
			_handleTable[++_handleCnt]= 
				new dbBlockHandle(blk, _new_tech, _handleCnt, _dbSchema);
		}
	}
	for (uint ii= 1; ii<=_handleCnt; ii++) {
		if (!isdb)
		{
			_handleTable[ii]->makeTrackSearchDb();
			_handleTable[ii]->makeInstSearchDb();
			_handleTable[ii]->makeNetWireSearchDb();
		}
		else 
		{
			_handleTable[ii]->connect2BlockSdb();
		}
		
		if (ii>1)
			continue;
		
		_handleTable[ii]->makeGrouteSearchDb(); // only for top
	}	
	return _handleCnt;
}

dbBlock *Ath__zroute::getDbBlock(uint ii)
{
	return _handleTable[ii]->_block;
}

dbBlockHandle *Ath__zroute::getBlockHandle(dbBlock *blk)
{
	return _handleTable[blk->getId()];
}
dbBlockHandle *Ath__zroute::getBlockHandle(uint ii)
{
	return _handleTable[ii];
}
int dbBlockHandle::inspect_1(Ath__zui *zui)
{
	if (zui->hierType_signal())
		inspectSignal(zui);

	else if (zui->hierType_inst())
		inspectInst(zui);

	else if (zui->hierType_power())
		inspectPower(zui);

	else if (zui->hierType_block())
		inspectBlock(zui);

	else if (zui->hierType_groute())
	{
/*
		if (boxType==Ath_box__gcell)
			inspectGcell(vzui, nameId, action);
		else if (boxType==Ath_box__gpath)
			inspectGpath(vzui, nameId, action);
*/
	}

	else if (zui->hierType_tile())
	{
/*
		if (boxType==Ath_box__pin)
			inspectTilePin(vzui, nameId, action);
		else if (boxType==Ath_box__bus)
			inspectTilePin(vzui, nameId, action);
		//	inspectBus(vzui, nameId, action);
*/
	}

	return TCL_OK;
}
void dbBlockHandle::addInstConnList(Ath__zui *zui, dbInst *inst, bool ignoreZuiFlags)
{
	bool instBoxes= zui->getHierBoxFlag(Ath_hier__inst, Ath_box__bbox, ignoreZuiFlags);
	bool termShapes= zui->getHierBoxFlag(Ath_hier__inst, Ath_box__pin, ignoreZuiFlags);
	bool instObs= zui->getHierBoxFlag(Ath_hier__inst, Ath_box__obs, ignoreZuiFlags);

	std::vector<dbInst *> connectivity;
	inst->getConnectivity(connectivity);
	
	std::vector<dbInst *>::iterator inst_itr;

	zui->setCurrentMark(Ath_mark__main);
	uint cnt= addInstBoxes(zui, inst, instBoxes, termShapes, instObs);
		
	zui->setCurrentMark(Ath_mark__instConn);
	for( inst_itr = connectivity.begin(); inst_itr != connectivity.end(); ++inst_itr )
	{
		dbInst * inst1 = *inst_itr;
		cnt += addInstBoxes(zui, inst1, instBoxes, termShapes, instObs);
	}
}
void dbBlockHandle::inspectInstObs(Ath__zui *zui)
{
	//uint id= zui->getInspNameId(&id2);
	uint id= zui->getInspNameId();
	dbInst * inst= NULL;
	if (id>0) 
	{
        // wfs 020305 changed from getITerm to getInst
		inst= dbInst::getInst(_block, id);
	}
	else 
	{
		return; // by name
	}

	zui->setCurrentMark(Ath_mark__main);
	
	if (zui->msgAction()) 
	{
		char msg_buf[1024];
		sprintf(msg_buf, "Obstruction Inst %d %s Block %s",
			inst->getId(), inst->getName().c_str(),
			_block->getName().c_str());

		if (getInstObs(zui, inst, Ath_hier__inst, false)>0)
		{
			int x1,y1,x2,y2;
			uint layer;
			zui->getFirstBox(&x1, &y1, &x2, &y2, &layer);
			
			zui->print_msg(x1, y1, x2, y2, layer, msg_buf);
		}
		else 
		{
			dbBox *bb= inst->getBBox();
			zui->print_msg(bb->xMin(), bb->yMin(), bb->xMax(), bb->yMax(), 0, msg_buf);
		}
	}
	else if (zui->linkAction())
	{
		addInstConnList(zui, inst, true);
 	}	
	else if (zui->groupAction1()) 
	{
		addInstBoxes(zui, inst, true, true, true);
	}	
	else if (zui->groupAction2()) 
	{
		addInstConnList(zui, inst, false);
	}
}
void dbBlockHandle::inspectInstPin(Ath__zui *zui)
{
	uint id= zui->getInspNameId();
	dbITerm * iterm= NULL;
	if (id>0) 
	{
		iterm= dbITerm::getITerm(_block, id);
	}
	else 
	{
		return; // by name
	}
	dbNet *net= iterm->getNet();
	dbInst *inst= iterm->getInst();

	zui->setCurrentMark(Ath_mark__main);
	
	if (zui->msgAction()) 
	{
		char msg_buf[1024];
		sprintf(msg_buf, "Iterm %d %s  Inst %d %s Block %s",
			id, iterm->getMTerm()->getName().c_str(), 
			inst->getId(), inst->getName().c_str(),
			_block->getName().c_str());

		if (getItermShapes(zui, iterm, Ath_hier__inst, false)>0)
		{
			int x1,y1,x2,y2;
			uint layer;
			zui->getFirstBox(&x1, &y1, &x2, &y2, &layer);
			
			zui->print_msg(x1, y1, x2, y2, layer, msg_buf);
		}
		else 
		{
			dbBox *bb= inst->getBBox();
			zui->print_msg(bb->xMin(), bb->yMin(), bb->xMax(), bb->yMax(), 0, msg_buf);
		}
	}
	else if (zui->groupAction1()) 
	{
		getNetConnectivity(zui, net, false, true, true, true);
	}
	else if (zui->groupAction2()) 
	{
		getNetConnectivity(zui, net, true, true, true, true);
	}
}
void dbBlockHandle::inspectInst(Ath__zui *zui)
{
	if (zui->selectAction())
	{
		addInstBoxes(zui, getDbInst(zui), true, true, true);
		return;
	}
	if (!(zui->boxType_bbox()))
	{
		if (zui->boxType_pin())
			inspectInstPin(zui);
		else if (zui->boxType_obs())
			inspectInstObs(zui);

		return;
	}
	bool ignoreZuiFlags= false;

	uint id= zui->getInspNameId();
	dbBox *bb= dbBox::getBox(_block, id);

	dbInst *inst= (dbInst *) bb->getBoxOwner();

	zui->setCurrentMark(Ath_mark__main);

	if (zui->msgAction()) 
	{	
		addInstBoxes(zui, inst, true, false, false);

		char msg_buf[1024];
		sprintf(msg_buf, "Inst %d %s Block %s",
			inst->getId(), inst->getName().c_str(),
			_block->getName().c_str());
		
		zui->print_msg(bb->xMin(), bb->yMin(), bb->xMax(), bb->yMax(), 0, msg_buf);
	}
	else if (zui->selfAction()) 
	{
		addInstBoxes(zui, inst, true, false, false);

        dbString name = inst->getName();
		
        dbString designName = _block->getName();
		zui->print_self((char *) designName.c_str(), (char *) name.c_str());
	}
	else if (zui->linkAction())
	{
		addInstConnList(zui, inst, Ath_box__bbox);
 	}	
	else if (zui->groupAction1()) 
	{
		addInstConnList(zui, inst, ignoreZuiFlags);
	}
	else if (zui->groupAction2()) 
	{
		addInstConnList(zui, inst, ignoreZuiFlags);
		getConnectivityWires(zui, inst, false);
	}
}
void dbBlockHandle::addNetSBoxesOnSearch(dbNet *net, bool skipVias)
{	
	dbSet<dbSWire> swires= net->getSWires();
	dbSet<dbSWire>::iterator itr;
	for( itr = swires.begin(); itr != swires.end(); ++itr ) {
        dbSWire * swire = *itr;
	    dbSet<dbSBox> wires= swire->getWires();
        dbSet<dbSBox>::iterator box_itr;
	
		for( box_itr = wires.begin(); box_itr != wires.end(); ++box_itr ) {
			dbSBox * s = *box_itr;
			
			uint boxId= s->getId();
			
			uint level= 0;

			if (s->isVia()) 
			{
				if (skipVias)
					continue;

				level= getViaLevel(s);
			}
			else
				level= s->getTechLayer()->getRoutingLevel();
			
			Ath__searchBox sbb;
			sbb.set(s->xMin(), s->yMin(), s->xMax(), s->yMax(), level, -1);
			sbb.setOwnerId(boxId, 0);

			uint dir= sbb.getDir();
			
			_dbNetWireSearch->getGrid(dir, level)->placeWire(&sbb);
        }
	}
}
/*
void dbBlockHandle::getSboxIds(Ath__zui *zui, dbNet *net)
{	
	dbSet<dbSWire> swires= net->getSWires();
	dbSet<dbSWire>::iterator itr;
	for( itr = swires.begin(); itr != swires.end(); ++itr ) {
        dbSWire * swire = *itr;
	    dbSet<dbSBox> wires= swire->getWires();
        dbSet<dbSBox>::iterator box_itr;
	
	    for( box_itr = wires.begin(); box_itr != wires.end(); ++box_itr ) {
	    	dbSBox * bb = *box_itr;
		    if (bb->isVia())
		    	continue;

			uint id= dbSBox::bb->getId();

			dbSBox * bb= dbSBox::getBox(_block, id);

		    zui->addBox(bb, boxType);
        }
	}
}
*/
/*
void dbBlockHandle::addBoxList(Ath__zui *zui, dbNet *net, Ath__boxType boxType)
{	
	dbSet<dbSWire> swires= net->getSWires();
	dbSet<dbSWire>::iterator itr;
	for( itr = swires.begin(); itr != swires.end(); ++itr ) {
        dbSWire * swire = *itr;
	    dbSet<dbSBox> wires= swire->getWires();
        dbSet<dbSBox>::iterator box_itr;
	
	    for( box_itr = wires.begin(); box_itr != wires.end(); ++box_itr ) {
	    	dbSBox * bb = *box_itr;
		    if (bb->isVia())
		    	continue;

			uint id= bb->getId();

			//dbSBox * bb= dbSBox::getBox(_block, id);

		    zui->addBox(bb, boxType, 0);
        }
	}
}
*/
uint dbBlockHandle::addInstBoxes(Ath__zui *zui, dbInst *inst, bool instBoxes, bool termShapes, bool instObs)
{
	uint cnt= 0;
	
	if (instBoxes)
		cnt += zui->addBox(inst->getBBox(), Ath_box__bbox, 0);
	
	if (termShapes)
		cnt += getItermShapes(zui, inst, Ath_hier__inst, false);
	
	if (instObs)
		cnt += getInstObs(zui, inst, Ath_hier__inst, false);

	return cnt;
}
/*
uint dbBlockHandle::addBtermBoxes(Ath__zui *zui, dbNet *net, bool ignoreZuiFlags)
{
	bool instBoxes= zui->getHierBoxFlag(Ath_hier__inst, Ath_box__bbox, ignoreZuiFlags);
	bool termShapes= zui->getHierBoxFlag(Ath_hier__inst, Ath_box__pin, ignoreZuiFlags);
	bool instObs= zui->getHierBoxFlag(Ath_hier__inst, Ath_box__obs, ignoreZuiFlags);

	dbSet<dbBTerm> bterms = net->getBTerms();
	dbSet<dbBTerm>::iterator bterm_itr;

	for( bterm_itr = bterms.begin(); bterm_itr != bterms.end(); ++bterm_itr) {
		dbBTerm * bterm = *bterm_itr;
*/
uint dbBlockHandle::addInstBoxes(Ath__zui *zui, dbNet *net, bool ignoreZuiFlags)
{
	bool instBoxes= zui->getHierBoxFlag(Ath_hier__inst, Ath_box__bbox, ignoreZuiFlags);
	bool termShapes= zui->getHierBoxFlag(Ath_hier__inst, Ath_box__pin, ignoreZuiFlags);
	bool instObs= zui->getHierBoxFlag(Ath_hier__inst, Ath_box__obs, ignoreZuiFlags);

	dbSet<dbITerm> iterms = net->getITerms();
	dbSet<dbITerm>::iterator iterm_itr;
	
	uint cnt= 0;
	for( iterm_itr = iterms.begin(); iterm_itr != iterms.end(); ++iterm_itr )
	{
		dbITerm * iterm = *iterm_itr;

		if (termShapes)
			cnt += getItermShapes(zui, iterm, Ath_hier__inst, false);

		dbInst * inst = iterm->getInst();
		dbBox * bb = inst->getBBox();

		if (instBoxes)
			cnt += zui->addBox(bb, Ath_box__bbox, 0);

		if (instObs)
			cnt += getInstObs(zui, inst, Ath_hier__inst, false);
	}
	if (termShapes)
	{
		dbSet<dbBTerm> bterms = _block->getBTerms();
		addBtermBoxes(zui, bterms, Ath_hier__block, true);
	}

	return cnt;
}
uint dbBlockHandle::getNetMaxBox(dbNet* net, Ath__searchBox *maxBB)
{
	uint len= 0;
	
        dbWire * wire = net->getWire();
        
        if ( wire != NULL )
	{
		uint wireId= wire->getId();
		
		dbWireShapeItr shapes;
		ads::dbShape s;
		
		for( shapes.begin(wire); shapes.next(s); )
		{
			if (s.isVia())
				continue;
			
			int shapeId= shapes.getShapeId();
			
			//uint level= s.getTechLayer()->getRoutingLevel();
			
			Ath__searchBox bb;
			
			if (getShapeCoords(true, &bb, wireId, shapeId)==NULL)
				continue;
			
			maxBB->setMaxBox(&bb);
			
			len += bb.getLength();
		}
	}
	
	return len;
}
void dbBlockHandle::getWires(int x1, int y1, int x2, int y2)
{
	Ath__zui zui;
	zui.setSearchBox(x1, y1, x2, y2, 0);
	
	getSignalWireIds(&zui);
	
	while (uint wid= zui.getNextId()) {
		
		Ath__wire *sw= _dbNetWireSearch->getWirePtr(wid);
		uint wireId= sw->getBoxId();
		uint shapeId= sw->getOtherId();

		dbShape s;
		dbWire * w = dbWire::getWire(_block, wireId);
		w->getShape( shapeId, s );
		//dbNet *net= w->getNet();
	}
}
uint dbBlockHandle::getNetFromSearch(Ath__zui *zui, dbNet *net, bool ignoreZuiFlags, bool ignoreBB)
{
	Ath__array1D<uint> wireIdTable(16000);

	if (!ignoreBB && !zui->isSearchBoxValid())
	{
		Ath__searchBox maxBB(ath__maxInt, ath__maxInt, -ath__maxInt, -ath__maxInt, 0);
		getNetSBoxesFromSearch(net, &wireIdTable, &maxBB);
		getNetShapesFromSearch(net, &wireIdTable, &maxBB);

		uint dd= 1000;
		zui->getBbox()->set(maxBB.loXY(0)-dd, maxBB.loXY(1)-dd, maxBB.hiXY(0)+dd, maxBB.hiXY(1)+dd);
	}
	else {
		getNetSBoxesFromSearch(net, &wireIdTable, NULL);
		getNetShapesFromSearch(net, &wireIdTable, NULL);
	}
	getWireIds(&wireIdTable, zui->getIdTable());
	
	uint cnt= getWireVias(zui, ignoreZuiFlags, Ath_hier__signal, Ath_box__wire, net);
	cnt    += getWireVias(zui, ignoreZuiFlags, Ath_hier__signal, Ath_box__via, net);

	return cnt;
}

uint dbBlockHandle::getNetConnectivity(Ath__zui *zui, dbNet *net, bool contextFlag, 
									   bool ignoreLayerFlags, bool ignoreZuiFlags, bool ignoreBB)
{	
	if (net==NULL)
		return 0;

	zui->setCurrentMark(Ath_mark__signalWire);	
	uint cnt= getNetFromSearch(zui, net, ignoreZuiFlags, ignoreBB);

	zui->setCurrentMark(Ath_mark__instConn);
	cnt += addInstBoxes(zui, net, ignoreZuiFlags);

	zui->setCurrentMark(Ath_mark__btermConn);
    dbSet<dbBTerm> bterms = _block->getBTerms();
	cnt += addBtermBoxes(zui, bterms, Ath_hier__block, ignoreZuiFlags);

	if (contextFlag) 
		cnt += getNetContext(zui, net, ignoreZuiFlags, ignoreLayerFlags);

	return cnt;
}
uint dbBlockHandle::getNetContext(Ath__zui *zui, dbNet *net, bool ignoreZuiFlags, bool ignoreLayerFlags)
{
	getSignalWireIds(zui, ignoreLayerFlags);
	
	zui->setCurrentMark(Ath_mark__context);	
	uint cnt= getWireVias(zui, ignoreZuiFlags, Ath_hier__signal, Ath_box__wire, net, true);
	cnt    += getWireVias(zui, ignoreZuiFlags, Ath_hier__signal, Ath_box__via, net, true);
	
	return cnt;
}
uint dbBlockHandle::getConnectivityWires(Ath__zui *zui, dbInst *inst, bool ignoreZuiFlags)
{
	zui->setCurrentMark(Ath_mark__signalWire);	

    dbSet<dbITerm> iterms = inst->getITerms();
    dbSet<dbITerm>::iterator iterm_itr;

	uint cnt= 0;
    for( iterm_itr = iterms.begin(); iterm_itr != iterms.end(); ++iterm_itr )
    {
        dbITerm * iterm = *iterm_itr;
        dbNet * net = iterm->getNet();
		if (net==NULL)
			continue;

		dbSigType type= net->getSigType();
		if ((type==POWER)||(type==GROUND))
			continue;

		cnt += getNetFromSearch(zui, net, ignoreZuiFlags, true);
	}
	return cnt;
}
void dbBlockHandle::inspectPower(Ath__zui *zui)
{
	/*
	if (zui->selectAction())
	{
		selectNet(zui);
		return;
	}
*/
	uint wid= zui->getInspNameId();
	Ath__searchBox bb;
	char *viaWire= "Wire";
	dbNet *net= getNetAndCoords(&bb, true, false, wid);
	if (net==NULL) // look for via
	{
		net= getNetAndCoords(&bb, false, false, wid);
		viaWire= "Via";
	}

	zui->setCurrentMark(Ath_mark__main);
	
	if (zui->msgAction())
	{
		char msg_buf[1024];
		sprintf(msg_buf, "Power %s Net %d %s Block %s",
			viaWire, net->getId(), net->getName().c_str(),
			_block->getName().c_str());
		
		zui->print_wire_msg(bb.loXY(0), bb.loXY(1), bb.hiXY(0), bb.hiXY(1), 
			bb.getLevel(), msg_buf);
	}
}
dbInst *dbBlockHandle::getDbInst(Ath__zui *zui)
{
	uint id= zui->getInspNameId();
	dbInst* inst= NULL;
	if (id==0)
	{
		inst= _block->findInst(zui->getInspectName());
		if (inst==NULL) 
		{
			fprintf(stdout, "Cannot find instance %s\n", 
				zui->getInspectName());
			return NULL;
		}
		id= inst->getId();
	}
	else {
		inst= dbInst::getInst(_block, id);
	}
	return inst;
}
dbNet *dbBlockHandle::getDbNet(Ath__zui *zui)
{
	uint id= zui->getInspNameId();
	dbNet *net= NULL;
	if (id==0)
	{
		net= _block->findNet(zui->getInspectName());
		if (net==NULL) 
		{
			fprintf(stdout, "Cannot find net %s\n", 
				zui->getInspectName());
			return NULL;
		}
		id= net->getId();
	}
	else {
		net= dbNet::getNet(_block, id);
	}
	return net;
}
dbBTerm *dbBlockHandle::getDbBTerm(Ath__zui *zui)
{
	uint id= zui->getInspNameId();
	dbBTerm *bterm= NULL;
	if (id==0)
	{
		bterm= _block->findBTerm(zui->getInspectName());
		if (bterm==NULL) 
		{
			fprintf(stdout, "Cannot find bterm %s\n", zui->getInspectName());
			return NULL;
		}
		id= bterm->getId();
	}
	else {
		bterm= dbBTerm::getBTerm(_block, id);
	}
	return bterm;
}
void dbBlockHandle::inspectSignal(Ath__zui *zui)
{
	if (zui->selectAction())
	{
		getNetConnectivity(zui, getDbNet(zui), false, true, true);
		return;
	}

	uint wid= zui->getInspNameId();

	Ath__searchBox bb;
	char* wireVia= "Wire";
	dbNet *net= getNetAndCoords(&bb, true, true, wid);
	if (net==NULL) 
	{
		net= getNetAndCoords(&bb, false, true, wid);
		wireVia= "Via";
	}

	zui->setCurrentMark(Ath_mark__main);
	
	if (zui->msgAction())
	{
		char msg_buf[1024];
		sprintf(msg_buf, "Signal %s %d %s Block %s",
			wireVia, net->getId(), net->getName().c_str(),
			_block->getName().c_str());
		
		zui->print_wire_msg(bb.loXY(0), bb.loXY(1), bb.hiXY(0), bb.hiXY(1), 
			bb.getLevel(), msg_buf);
	}
	else if (zui->selfAction()) {
		
		zui->addBox(&bb, Ath_hier__signal, Ath_box__wire, wid);

		Ath__searchBox maxBB(&bb,0);
		uint len= getNetMaxBox(net, &maxBB);
		char buf[100];
		sprintf(buf, "wireLength %d", len);

		dbString name = net->getName();
		
        dbString designName = _block->getName();
		zui->print_self((char *) designName.c_str(), (char *) name.c_str(), buf);
	}
	else if (zui->groupAction1()) 
	{
		getNetConnectivity(zui, net, false, false, false);
	}
	else if (zui->groupAction2()) 
	{
		getNetConnectivity(zui, net, true, false, false);
	}
	else if (zui->linkAction()) {
//		getNetConnectivity(zui, net);
	}
}
void dbBlockHandle::inspectBterm(Ath__zui *zui)
{
	if (zui->selectAction())
	{
		//dbBTerm * bterm= getDbBTerm(zui);
		getNetConnectivity(zui, getDbNet(zui), false, true, true, true);

		return;
	}

	uint id= zui->getInspNameId();
	dbBTerm * bterm= dbBTerm::getBTerm(_block, id);

	zui->setCurrentMark(Ath_mark__main);
	
	if (zui->msgAction())
	{
		ads::dbShape s;
		if (! bterm->getFirstPin(s))
			return;
		
		char msg_buf[1024];
		sprintf(msg_buf, "Bterm %d %s Block %s",
			id, bterm->getName().c_str(),
			_block->getName().c_str());
		
		zui->print_wire_msg(s.xMin(), s.yMin(), s.xMax(), s.yMax(),
			s.getTechLayer()->getRoutingLevel(), msg_buf);
	}
	else if (zui->groupAction1()) 
	{
		dbNet *net= bterm->getNet();
		getNetConnectivity(zui, net, false, true, true, true);
	}
}
void dbBlockHandle::inspectBlockObs(Ath__zui *zui)
{
	uint id= zui->getInspNameId();
	dbBox *bb= dbBox::getBox(_block, id);
	dbObstruction *obs= (dbObstruction *) bb->getBoxOwner();
	dbInst *inst = obs->getInstance();

	zui->setCurrentMark(Ath_mark__main);
	
	if (zui->msgAction())
	{
		char msg_buf[1024];
		if (inst!=NULL) 
		{
			sprintf(msg_buf, "Blockage Inst %d %s Block %s",
				inst->getId(), inst->getName().c_str(),
				_block->getName().c_str());
		}
		else 
		{
			sprintf(msg_buf, "Blockage %d Block %s",
				id, _block->getName().c_str());
		}
		
		zui->print_wire_msg(bb->xMin(), bb->yMin(), bb->xMax(), bb->yMax(),
			bb->getTechLayer()->getRoutingLevel(), msg_buf);
	}
}
void dbBlockHandle::inspectBlock(Ath__zui *zui)
{
	if (zui->boxType_pin())
	{
		inspectBterm(zui);
		return;
	}
	if (zui->boxType_obs())
	{
		inspectBlockObs(zui);
		return;
	}

	zui->setCurrentMark(Ath_mark__main);
	
	if (zui->msgAction())
	{		
		char msg_buf[1024];
		sprintf(msg_buf, "Block %d %s",	_id, _block->getName().c_str());
		
		// dbBox *bb= handle->_block->getBBox();

		adsRect s;
		_block->getDieArea( s );
		
		zui->print_msg(s.xMin(), s.yMin(), s.xMax(), s.yMax(),0, msg_buf);
	}
}

void dbBlockHandle::inspectTilePin(Ath__zui *zui, uint boxId, const char *action)
{
	//Ath__qPin *qPin= _quad->getPinFromBox(boxId);
	Ath__qPin *qPin= _quad->getPin(boxId);
	if (strcmp(action, "self")==0) 
	{
		//TODO
		// char *name= _netTable->getName(nameId);
		//TODO_NOW zui->print_self(_designName, "Tile Pin", "TilePin", NULL);
	}
	else if (strstr(action, "group")!=NULL) 
	{
		Ath__qPin *q1= qPin->getSrcPin();
		Ath__qPin *q2= qPin->getDstPin();

		uint layer1= q1->getLayer();
		uint layer2= q2->getLayer();
		uint width1= _tech->findRoutingLayer(layer1)->getWidth();
		uint width2= _tech->findRoutingLayer(layer2)->getWidth();
		
		zui->setCurrentMark(Ath_mark__main);

		zui->setCurrentMark(Ath_mark__pinTarget);
		q1->makeZuiObject(zui, width1); // target
		q2->makeZuiObject(zui, width2);

		zui->setCurrentMark(Ath_mark__pinGravity);
		q1->makeZuiObject(zui, width1, false, true); // inst gravity center
		q2->makeZuiObject(zui, width2, false, true);

		zui->setCurrentMark(Ath_mark__pinPlaced);
		q1->makeZuiObject(zui, width1, true); // placed
		q2->makeZuiObject(zui, width2, true);

		zui->setCurrentMark(Ath_mark__busPlaced);
		q1->makeBusZuiObject(zui, width1);
		q2->makeBusZuiObject(zui, width2);


		zui->setCurrentMark(Ath_mark__netConn);
		dbNet *net= dbNet::getNet(_block, q1->_netId);
		addInstBoxes(zui, net, false);

		zui->computeMaxBB(10000);

		zui->bufferExistingZboxes(); // add them later 

		zui->setCurrentMark(Ath_mark__context);
		getInstBoxes(zui, Ath_hier__inst, true);
		_quad->getTilePins_1(zui);
		_quad->getTileBuses_1(zui);
		// getSignalWires(zui, Ath_hier__signal, true);

		zui->setCurrentMark(Ath_mark__powerWire);
		getPowerWires(zui, Ath_hier__power, layer1);
		if (layer1!=layer2)
			getPowerWires(zui, Ath_hier__power, layer2);

		zui->addPreviousZboxes(); // want to appear last on GUI

		zui->setCurrentMark(Ath_mark__tile);
		_quad->getTileBoundLines(zui, 15, 50, true);
	}	
	else if (strstr(action, "link")!=NULL) 
	{
		//zui->setCurrentMark(Ath_mark__main);
		//qPin->makeZuiObject(zui, width);

		// zui->setCurrentMark(Ath_mark__related);
		//_instTable->getInstConnections(zui, inst);
	}
}
uint dbBlockHandle::getQuadCnt(int modSize, int xy1, int xy2)
{
	int dxy= xy2-xy1;
	int mod= dxy/modSize + 1;
	
	if (mod>32)
		mod= 32;
	
	return mod;
}
uint dbBlockHandle::makeQuadTable(uint rowCnt, uint colCnt, uint metH, uint metV)
{
	dbBox *bb= _block->getBBox();
	
	if (colCnt<=0) {
		rowCnt= getQuadCnt(rowCnt*1000, bb->yMin(), bb->yMax());
		colCnt= getQuadCnt(rowCnt*1000, bb->xMin(), bb->xMax());
	}	
	if (_quad!=NULL)
		delete _quad;
		
	Ath__grid *gH= NULL;
	Ath__grid *gV= NULL;
	if ((_dbNetWireSearch!=NULL)&&(metH>0)&&(metV>0)) {
		gH= _dbNetWireSearch->getGrid(1, metH);
		gV= _dbNetWireSearch->getGrid(0, metV);
	}
	
	_quad= new Ath__quadTable(rowCnt, bb->yMin(), bb->yMax(), 
		colCnt, bb->xMin(), bb->xMax(), gH, gV);
	
	_quad->initBusTable(bb, _tech, 64, 128);

	_quad->addTileBlockages(_block, _tech);

	_quad->groupConnections(_block);
	
	return 1;
}

uint Ath__quadTable::groupConnections(dbBlock * block) 
{
    uint netCnt= 0;
    uint itermCnt= 0;

    dbSet<dbNet> nets = block->getNets();
    dbSet<dbNet>::iterator net_itr;
    
    for( net_itr = nets.begin(); net_itr != nets.end(); ++net_itr )
    {
        dbNet * net = *net_itr;
		dbSigType type= net->getSigType();
		if ((type==POWER)||(type==GROUND))
			continue;

        ++netCnt;

        dbSet<dbITerm> iterms = net->getITerms();
        dbSet<dbITerm>::iterator iterm_itr;
        
        uint termCnt = 0;

        for( iterm_itr = iterms.begin(); iterm_itr != iterms.end(); ++iterm_itr )
        {
            dbITerm * iterm = *iterm_itr;
            dbInst * inst = iterm->getInst();
            dbBox * bb = inst->getBBox();
            
			int ix, iy;
			if (iterm->getAvgXY(&ix, &iy))
				addPoint(bb->xMin(), bb->yMin(), bb->xMax(), bb->yMax(), iterm->getId());
			else
				addPoint(bb->xMin(), bb->yMin(), bb->xMax(), bb->yMax(), iterm->getId());

            ++termCnt;
        }

        itermCnt += termCnt;
		processConnections(net->getId(), termCnt);
        reset();
    }
	printConnectivityStats(stdout, netCnt, itermCnt);

    return itermCnt;
}
uint Ath__quadTable::assignTracks(uint metH, uint metV)
{
	setFreeTracks(metH);
	setFreeTracks(metV);

	assignTracksForThru(_colCnt, 1, 0, metH);
	assignTracksForThru(_rowCnt, 1, 1, metV);


	uint minDist= 0;
	assignTracksDist2(1, 1, metH, metV);

	minDist= 1;
	uint dist= ath__max(_colCnt, _rowCnt);
	for (; dist>1; dist--) // all thin turns
		assignTracksDist2(dist, minDist, metH, metV);

	minDist= 2;
	dist= ath__max(_colCnt, _rowCnt);
	for (; dist>1; dist--) // all turns
		assignTracksDist2(dist, minDist, metH, metV);

	return 1;
}
uint dbBlockHandle::assignTracks(uint metH, uint metV)
{
	return _quad->assignTracks(metH, metV);
}
void Ath__quadTable::setFreeTracks(uint level)
{
	for (uint ii= 0; ii<_rowCnt; ii++) 
	{
		for (uint jj= 0; jj<_colCnt; jj++) 
		{
			_busTable[ii][jj]->getGrid(level)->resetFreeTracks();
		}
	}
}

uint Ath__grid::blockTracks(dbBlock *block, Ath__gridTable *wireSearch)
{
	Ath__array1D<uint> idtable(64000);
	Ath__searchBox bb; getBbox(&bb);
	
	wireSearch->search(&bb, bb.getDir(), bb.getLevel(), &idtable, true); //single grid
	
	blockTracks(block, &idtable);

	return idtable.getCnt();
}
bool Ath__grid::anyTrackAvailable()
{
	return (_emptyTrackCnt>0) ? true : false;
}
float Ath__grid::updateFreeTracks(float v)
{
	_emptyTrackCnt += v;
	return _emptyTrackCnt;
}
void Ath__grid::resetFreeTracks()
{
	_emptyTrackCnt= 1.0 * _freeTrackCnt;
}
void Ath__quadTable::assignResourcesThru(Ath__p2pConn* p2p, uint level)
{
	Ath__connWord w= p2p->_conn;
	uint c1; uint r1= w.getFromRowCol(&c1);
	uint c2; uint r2= w.getToRowCol(&c2);
	
	uint dir= w.getStraight();
	if (dir==0) 
	{
		for (uint col= c1+1; col<c2; col++) {
			_busTable[r1][col]->getGrid(level)->updateFreeTracks(-1.0);
		}
	}
	else 
	{
		for (uint row= r1+1; row<r2; row++) {
			_busTable[row][c1]->getGrid(level)->updateFreeTracks(-1.0);
		}
	}
	_thruPinTable[r1][c1]->addStraightConn(0, dir, p2p);
	_thruPinTable[r2][c2]->addStraightConn(0, dir, p2p);
}
void Ath__quadTable::assignTracksForThru(uint dist2, uint dist1, uint dir, uint level)
{
	uint cnt= 0;
	for (uint dist= dist2; dist>dist1; dist--) {
		if (_connTable->startStraightIterator(dir, dist)==0)
			continue;

		while (Ath__p2pConn* p2p= _connTable->getNextConn()) {
			assignResourcesThru(p2p, level);
			cnt++;
		}
	}
	fprintf(stdout, "distance: %d to %d = %d for assignStraightTracks[layer=%d], direction=%d\n", 
		dist1, dist2, cnt, level, dir);
}
bool Ath__quadTable::updateCornerResources(uint row, uint col)
{
	Ath__grid *vGrid= _busTable[row][col]->getGrid(_topMetV);
	Ath__grid *hGrid= _busTable[row][col]->getGrid(_topMetH);

	if (!vGrid->anyTrackAvailable()) {
		fprintf(stdout, "No routing Level %d (vertical) Resources available for tile %d %d\n",
			_topMetV, row, col);
		return false;
	}
	if (!hGrid->anyTrackAvailable()) {
		fprintf(stdout, "No routing Level %d (horizontal) Resources available for tile %d %d\n",
			_topMetH, row, col);
		return false;
	}
	vGrid->updateFreeTracks(-0.5);
	hGrid->updateFreeTracks(-0.5);

	return true;
}
bool Ath__quadTable::getCornerResources(uint r1, uint c1, uint r2, uint c2, uint *midRow, uint *midCol, uint *turn)
{
	// assume dist=2, rdist=1, cdist=1
	// r2>r1 default
	if (c2>c1) {  //
		if (updateCornerResources(r2, c1))
		{

			*midRow= r2;
			*midCol= c1;
			*turn= 0;    // Go Up, then Right
			return true;
		}
		if (updateCornerResources(r1, c2)) 
		{
			*midRow= r1;
			*midCol= c2;
			*turn= 1;	// Go Right, Then Up 

			fprintf(stdout, "Alternative connection %d %d  %d %d OK!\n",
				r1, c1, r2, c2);
			return true;
		}
	}
	else {
		if (updateCornerResources(r2, c1)) 
		{
			*midRow= r2;
			*midCol= c1;
			*turn= 2;   // Go Up, then Left
			return true;
		}
		if (updateCornerResources(r1, c2)) 
		{
			*midRow= r1;
			*midCol= c2;
			*turn= 3;   // Go Left, Then Up

			fprintf(stdout, "Alternative connection %d %d  %d %d OK!\n",
				r1, c1, r2, c2);
			return true;
		}
	}
	return false;
}
uint Ath__quadTable::assignTracksDist2(uint dist, uint minDist, uint metH, uint metV)
{
	_topMetH= metH;
	_topMetV= metV;

	uint cnt= 0;
	if (_connTable->startCornerIterator(dist)==0) // have stored by max distance in one dir
		return 0;
	
	while (Ath__p2pConn* p2p= _connTable->getNextConn()) {

		if (p2p->_srcPin->_assigned>0)
			continue;

		Ath__connWord w= p2p->_conn;
		uint c1; uint r1= w.getFromRowCol(&c1);
		uint c2; uint r2= w.getToRowCol(&c2);
		uint midRow; 
		uint midCol;
		uint type;
		
		uint rowDist= r2-r1;
		uint colDist= c2-c1;
		if (colDist<0)
			colDist = -colDist;
		
		if ( !((rowDist==minDist) || (colDist==minDist)) )
			continue;

		if (getCornerResources(r1, c1, r2, c2, &midRow, &midCol, &type)) 
		{
				Ath__connWord v(midRow, midCol, type);
				p2p->_conn= v;

			// _thruPinTable[midRow][midCol]->addCornerConn2(type, p2p, midRow, midCol);
			p2p->_srcPin->_assigned= 1;
		}
		else 
		{
			p2p->_srcPin->_assigned= 0;
			cnt ++; // TODO connection ...
		}
	}
	return cnt;
}

uint Ath__quadTable::addQnet(uint netId, Ath__connWord w, uint n1, uint cnt, uint row, uint col)
{
	uint n;
	Ath__qNet *net= _qNetPool->alloc(NULL, &n);
	
	net->_id= n;
	net->_origNetId= netId;
	net->_conn= w;
	net->_qPin= NULL;

	net->_startTermIndex= n1;
	net->_itermCnt= cnt;

	net->_startBTermIndex= 0;
	net->_btermCnt= 0;

	net->_row= row;
	net->_col= col;

	return n;
}
void Ath__qNet::updateBterms(uint n1, uint cnt)
{
	_startBTermIndex= n1;
	_btermCnt= cnt;
}
void Ath__quadTable::setPinNetConn(Ath__qPin *pin)
{
	Ath__qNet *net= _qNetPool->get(pin->_ioNetId);
	net->_qPin= pin;
}

uint dbBlockHandle::createTblocks()
{
	if (_quad==NULL)
	{
		fprintf(stdout, "ERROR: No tiles exist\n");
		return 0;
	}
	if ( ! _quad->createTblocks(_block, _tech, _dbNetWireSearch) )
	{
		fprintf(stdout, "ERROR: Cannot create some Tblocks\n");
		return 0;
	}
	return 1;
}
void Ath__quadTable::getPowerWireIds(int x1, int y1, int x2, int y2, Ath__gridTable *dbNetWireSearch, Ath__array1D<uint> *wireDbIdTable)
{
	Ath__array1D<uint> wireIdTable(1000);

	for (uint layer= 1; layer<dbNetWireSearch->getColCnt(); layer++) 
	{
		for (uint dir= 0; dir<2; dir++) 
		{
			Ath__searchBox bb(x1, y1, x2, y2, layer, dir);
			dbNetWireSearch->search(&bb, dir, layer, &wireIdTable, true); //single grid			
		}	
	}
	for (uint ii= 0; ii<wireIdTable.getCnt(); ii++)
	{
		Ath__wire *w= dbNetWireSearch->getWirePtr(wireIdTable.get(ii));
		if (w->getOtherId()>0)
			continue; // signal

		wireDbIdTable->add(w->getBoxId());
	}
}
bool Ath__quadTable::addPowerWires(uint row, uint col, dbBlock* topBlock, dbBlock* tblock, dbTech *tech, 
								   Ath__gridTable *dbNetWireSearch, Ath__array1D<uint> *wireDbIdTable)
{
	int x1, y1, x2, y2;
	_busTable[row][col]->getBounds(&x1, &y1, &x2, &y2);
	Ath__box tileBox(x1, y1, x2, y2);
	
	wireDbIdTable->resetCnt();
	getPowerWireIds(x1, y1, x2, y2, dbNetWireSearch, wireDbIdTable);
	
	dbSet<dbNet> nets = topBlock->getNets();   
	dbSet<dbNet>::iterator net_itr;
	for( net_itr = nets.begin(); net_itr != nets.end(); ++net_itr ) 
	{	
		dbNet *powerNet = *net_itr;
		dbSigType ptype= powerNet->getSigType();

		if (! ( (ptype==dbSigType::POWER)||(ptype==dbSigType::GROUND) ) )
		{
			continue;
		}
		
		dbString netName= powerNet->getName();
		
		dbNet *powerBlkNet= tblock->findNet(netName.c_str());
		if (powerBlkNet==NULL)
			continue;

		dbWireType type( dbWireType::FIXED ); 
		dbSWire * swire = dbSWire::create( powerBlkNet, type );
		dbWireShapeType stype( dbWireShapeType::RING ); //TODO
		
		for (uint ii= 0; ii<wireDbIdTable->getCnt(); ii++ )
		{
			uint wireId= wireDbIdTable->get(ii);
			
			dbSBox* s= dbSBox::getSBox(topBlock, wireId);
			
			dbSWire *sw= (dbSWire *) s->getBoxOwner();
			if (powerNet != sw->getNet())
				continue;
			
			int sx1= s->xMin();
			int sy1= s->yMin();
			int sx2= s->xMax();
			int sy2= s->yMax();
			
			tileBox.clip(&sx1, &sy1, &sx2, &sy2);
			//uint level= s->getTechLayer()->getRoutingLevel();
			dbSBox::create( swire, s->getTechLayer(), sx1, sy1, sx2, sy2, stype);
		}
	}
	return true;
}
/*
            tmpi = (_ylo +(step-1)/2 + step -orgY)/step;
            if (tmpi > 0) {
                orgY += tmpi*step;
                count -= tmpi;
            }
            tmpi = 1 + (_yhi -(step-1)/2 - orgY)/step;
            if (count > tmpi) count = tmpi;
            if (count < 2) continue;
*/
/*
bool Ath__quadTable::addTracks(Ath__box *bb, dbBlock* topBlock, dbBlock* tblock, dbTech *tech)
{
	dbSet<dbTechLayer> layers = tech->getLayers();
	dbSet<dbTechLayer>::iterator litr;
	dbTechLayer *layer;
	dbTrackGrid *tg0,*tg;
	for (litr = layers.begin(); litr != layers.end(); ++litr) {
		layer = *litr;
		tg0 = topBlock->findTrackGrid(layer);
		if (!tg0) 
			continue;

		tg = dbTrackGrid::create(tblock,layer);
		int org,cnt,step;
		if (tg0->getNumGridPatternsX()) {
			tg0->getGridPatternX(0,org,cnt,step);
			tg->addGridPatternX(org,cnt,step);
		}
		if (tg0->getNumGridPatternsY()) {
			tg0->getGridPatternY(0,org,cnt,step);
			tg->addGridPatternY(org,cnt,step);
		}
	}
	return true;
}
*/
bool Ath__quadTable::createTblocks(dbBlock* block, dbTech *tech, Ath__gridTable *dbNetWireSearch)
{
	Ath__array1D<uint> wireDbIdTable(4000);

	for (uint ii= 0; ii<_rowCnt; ii++) 
	{
		for (uint jj= 0; jj<_colCnt; jj++) 
		{
			dbBlock* tblock= createTblock(block, tech, ii, jj);
			if (tblock==NULL) 
			{
				fprintf(stdout, "ERROR: I cannot create tblock for tile %d,%d\n",
					ii,jj);
				return false;
			} else {
				int x1, y1, x2, y2;
				_busTable[ii][jj]->getBounds(&x1, &y1, &x2, &y2);
				//Ath__box tileBox(x1, y1, x2, y2);
		
				addPowerWires(ii, jj, block, tblock, 
					tech, dbNetWireSearch, &wireDbIdTable);
				
				// addTracks(&tileBox, block, tblock, tech);
            }
		}
	}
	return true;
}

dbBlock* Ath__quadTable::createTblock(dbBlock* block, dbTech *tech, uint row, uint col)
{
	//create Tblock

	char tileName[100];
	sprintf(tileName,"T%d", row*_colCnt + col + 1);
	dbBlock *tblock= dbBlock::create(block, tileName, block->getHierarchyDelimeter());
	if (tblock==NULL)
		return NULL;
	
	char leftd, rightd;
	block->getBusDelimeters(leftd, rightd);
	tblock->setBusDelimeters(leftd, rightd);
	int x1, y1, x2, y2;
	_busTable[row][col]->getBounds(&x1, &y1, &x2, &y2);

	adsRect die;
	die.init(x1, y1, x2, y2);
	tblock->setDieArea(die);

	std::map<dbInst*, bool> pushed;

	// Identify local nets
	for (uint ii= 0; ii<_localNetTable[row][col]->getCnt(); ii++)
	{
		uint netId= _localNetTable[row][col]->get(ii);

		// DB iterator for all instances of the net
		dbNet *net= dbNet::getNet(block, netId);
        dbSet<dbITerm> iterms = net->getITerms();
        dbSet<dbITerm>::iterator iterm_itr;
        
        //uint termCnt = 0;

        for( iterm_itr = iterms.begin(); iterm_itr != iterms.end(); ++iterm_itr )
        {
            dbITerm * iterm = *iterm_itr;
            dbInst * inst = iterm->getInst();

			bool & p = pushed[inst];
			if ( p == false )
			{
				// obsolete: dbInst::push( tblock, inst, false );
				p = true;
			}
		}
	}
/*
	for (uint jj= 0; jj<_ioNetTable[row][col]->getCnt(); jj++)
	{
		uint ioQnetId= _ioNetTable[row][col]->get(jj);

		Ath__qNet *qNet= _qNetPool->get(ioQnetId);
		if (qNet->_qPin==NULL)
			continue; // 2-tile nets are done only!


		uint startTermIndex= qNet->_startTermIndex;

		for (uint ii= qNet->_startTermIndex; ii<startTermIndex+qNet->_itermCnt; ii++) 
		{
			uint termId= _termTable[row][col]->get(ii);
			dbITerm *iterm= dbITerm::getITerm(block, termId);
			dbInst *inst= iterm->getInst();

			bool & p = pushed[inst];
			if ( p == false )
			{
				dbInst::push( tblock, inst, false );
				p = true;
			}
		}
		dbNet *origNet= dbNet::getNet(block, qNet->_origNetId);
        dbString n = origNet->getName();
        dbNet *tnet= tblock->findNet(n.c_str());
		
		char ttermName[100];
		sprintf(ttermName,"Term_%d_tile_%d_%d", jj, row, col);
        dbBTerm *tterm= dbBTerm::create(tnet, ttermName);

		// Add physical pin port bbox
		Ath__qPin *qPin= qNet->_qPin;
		uint level;
		int x1, y1, x2, y2;
		if (qPin->getPortCoords(&x1, &y1, &x2, &y2, &level)==NULL)
			continue;

		dbBox::create(tterm, tech->findRoutingLayer(level), x1, y1, x2, y2);

		** TODO: have to add Block terms, also in function groupConnections
		for (uint ii= qNet->_startBTermIndex; ii<startTermIndex+qNet->_itermCnt; ii++) 
		{
			uint termId= _btermTable[row][col]->get(ii);
			dbBTerm *bterm= dbBTerm::getBTerm(block, termId);

		}
		**
	}
*/
	// Add obstructions for all wires crossing the tile that belong to Znets
	Ath__array1D<Ath__box*> boxTable(1000);
	_busTable[row][col]->getBuses(&boxTable, tech);
	for (uint kk= 0; kk<boxTable.getCnt(); kk++)
	{
		Ath__box *bb= boxTable.get(kk);
        dbObstruction::create(tblock, tech->findRoutingLayer(bb->_layer),
                      bb->_xlo, bb->_ylo, bb->_xhi, bb->_yhi);

		delete bb;
	}
	return tblock;
}

int getZuiObjects(Ath__zui *zui, bool gcells, bool gpaths, bool congestion, bool hotspots);


void dbBlockHandle::makeGrouteSearchDb()
{

//_grObject->getZuiObjects(NULL, false, false, false, false);
	
	// getZuiObjects(NULL, false, false, false, false);

	/*
Groute* getGrouteObject();

	_grObject= getGrouteObject();
	*/
}
void setGrAtZui(Groute* ptr)
{
	;
}

