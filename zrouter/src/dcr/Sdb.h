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

#ifndef ADS_DCR_SDB_H
#define ADS_DCR_SDB_H

#include "ISdb.h"  // Why do you need here
#include "IZdcr.h"

#ifndef ADS_ZIMPLEMENTS_H
#include "ZImplements.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#include "db.h"
#include "dbShape.h"

#include "array1.h"

class Ath__gridTable;

BEGIN_NAMESPACE_ADS

class dbBlock;
class dbBox;
class dbTech;
class dbNet;
class dbSBox;
//class dbShape;
//class dbWireShapeItr;

///
/// Sdb - Implementation for ZRoute Area Search Infrastructure
///
/// 

class Sdb : public ZImplements<Sdb, ISdb>
{
	Ath__gridTable *_search;

	Ath__array1D<uint> *_idTable;
	//Ath__searchBox areaBb;
	
private:
	uint getViaLevel(dbSBox *s);
	uint getViaLevel(dbShape *s);
	uint addNetSBoxes(dbNet *net, uint wtype, bool wireVia);
	uint addNetShapesOnSearch(dbNet * net, uint wtype, bool wireVia);
	uint addNetShapesOnSearch(dbNet * net, uint wtype, uint vtype);


public:
	Sdb();
	~Sdb();
	
	// ISdb Interface
	
	void removeMarkedNetWires();
	void reMakeSdb(dbTech *tech, dbBlock *block);
	void cleanSdb();
	void dumpTrackCounts(FILE *fp);
	void setExtControl(dbBlock *block, bool useDbSdb, uint adj, uint npsrc, uint nptgt, uint ccUp, bool allNet, uint contextDepth, Ath__array1D<int> **contextArray, uint *contextLength, Ath__array1D<SEQ*> ***dgContextArray, uint *dgContextDepth, uint *dgContextPlanes, uint *dgContextTracks, uint *dgContextBaseLvl, int *dgContextLowLvl, int *dgContextHiLvl, uint *dgContextBaseTrack, int *dgContextLowTrack, int *dgContextHiTrack, int **dgContextTrackBase, AthPool<SEQ> *seqPool);
	void adjustOverlapMakerEnd();
	void adjustMetalFill();

	void setupForBoxes(adsRect maxRect, uint minHeight, uint minWidth);

	void setMaxArea(int x1, int y1, int x2, int y2);
	void resetMaxArea();

	void addBox(dbBox * bb, uint wtype, uint id);
	uint setExtrusionMarker(uint startRow, uint startCol);
	Ath__gridTable* getSearchPtr(); //EXT
	void setDefaultWireType(uint v); //EXT

	void searchBoxIds(int x1, int y1, int x2, int y2);


	uint addBox(int x1, int y1, int x2, int y2, uint level, uint id1, uint id2, uint wtype);
	void getBox(uint wid, int *x1, int *y1, int *x2, int *y2, uint *level, uint *id1, uint *id2, uint *wtype);
	uint addBlockages(dbBlock * block);
	void initSearchForNets(dbTech *tech, dbBlock *block);
	void addPowerNets(dbBlock *block, uint wtype, bool wireVia);
	void addSignalNets(dbBlock *block, uint wtype, bool wireVia);
	void addSignalNets(dbBlock *block, uint wtype, uint vtype);

	void searchWireIds(int x1, int y1, int x2, int y2, bool ignoreLayers, bool *exludeTable);
	uint makeGuiBoxes(ZPtr<IZdcr> dcr, uint hierType, uint boxType, bool useSdbId, uint excludeId1=0);
	uint searchSpread(void *ip, uint spreadTrackDist, std::vector<dbNet *> & inets, char *bbox, void (*doSpread)(void *ip, uint, void *, void *, void *));
	uint couplingCaps(ZPtr<ISdb> ccSdb, uint couplingDist, ZInterface *Interface, void (*extCompute)(int *, void *), void *);
	uint startIterator();
	uint getNextWireId();
	bool getNextBox(int &x1, int &y1, int &x2, int &y2, uint &level, uint &netId, uint &shapeId);
	void getCCdist(uint wid, uint *width, uint *level, uint *id1, uint *id2);
	void getIds(uint wid, uint *id1, uint *id2, uint *wtype);


/*
	ZPtr<Sdb_r> getReadInterface()
	{
		ZPtr<Sdb_r> p;
		adsNewComponent( _context, p );
		Sdb_r * s = (Sdb_r *) p.getPtr();
		s->_sdb = ZPtr<ISdb>(this);
		return p;
	}
	
	ZPtr<Sdb_w> getWriteInterface()
	{
		ZPtr<Sdb_w> p;
		adsNewComponent( _context, p );
		Sdb_w * s = (Sdb_w *) p.getPtr();
		s->_sdb = ZPtr<ISdb>(this);
		return p;
	}
*/
};

END_NAMESPACE_ADS

#endif
