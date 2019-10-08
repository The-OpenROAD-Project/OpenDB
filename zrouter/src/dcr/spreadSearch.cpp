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

#include <stdio.h>
#include "wire.h"

BEGIN_NAMESPACE_ADS


void v_printWireNeighbor (void *ip, uint met, void *v_srcWire, void *v_topNeighbor, void *v_botNeighbor)
{
	Ath__wire *srcWire = (Ath__wire *)v_srcWire;
	Ath__array1D<Ath__wire*> *topNeighbor = (Ath__array1D<Ath__wire*> *)v_topNeighbor;
	Ath__array1D<Ath__wire*> *botNeighbor = (Ath__array1D<Ath__wire*> *)v_botNeighbor;
	srcWire->printWireNeighbor (met, topNeighbor, botNeighbor);
}
END_NAMESPACE_ADS

void Ath__wire::printOneWire (FILE *ptfile)
{
	int x1, y1, x2, y2;
	uint dir;
	getCoords (&x1, &y1, &x2, &y2, &dir);
	fprintf (ptfile, "    net=%d shape=%d dir=%d x1=%d y1=%d x2=%d y2=%d\n", _boxId, _otherId, dir, x1, y1, x2, y2);
}
void Ath__wire::printWireNeighbor (uint met, Ath__array1D<Ath__wire*> *topNeighbor, Ath__array1D<Ath__wire*> *botNeighbor)
{
	dbBlock *block = _track->getGrid()->getGridTable()->getBlock();
	FILE *ptfile = block->getPtFile();
	fprintf (ptfile, "\n\nLevel %d\n", met);
	fprintf (ptfile, "  Spreading Wire:\n");
	printOneWire(ptfile);
	uint jj;
	fprintf (ptfile, "\n  Top Wires:\n");
	for (jj = 0; jj < topNeighbor->getCnt(); jj++)
		topNeighbor->get(jj)->printOneWire(ptfile);
	fprintf (ptfile, "\n  Bottom Wires:\n");
	for (jj = 0; jj < botNeighbor->getCnt(); jj++)
		botNeighbor->get(jj)->printOneWire(ptfile);
}

void Ath__track::findNeighborWire(Ath__wire *w1, Ath__array1D<Ath__wire*> *nb, bool tohi)
{

	
	uint NoPowerTarget = _grid->getGridTable()->noPowerTarget();
	//bool allNet = _grid->getGridTable()->allNet();
	_grid->getGridTable()->allNet();
	//int w1start = w1->_xy;
	int w1end = w1->_xy + w1->_len;
	//int w1top = w1->_base + w1->_width;
	int w2end;
	Ath__wire *w2= getTargetWire();
	while (w2)
	{
		if ( w2->_xy >= w1end)
			break;
		Ath__wire *nebw = w2;
		if (w2->_srcId>0)
			nebw = _grid->getWirePtr(w2->_srcId);
		w2end = w2->_xy + w2->_len;
		if (w1->_xy >= w2end || nebw == w1)
		{
			w2= nextTargetWire(NoPowerTarget);
			continue;
		}
//		if ((tohi && w2->base >= w1top) || (!tohi && w1->_base >= w2->_base+w2->_width))
//		{
//			w2= nextTargetWire(NoPowerTarget);
//			continue;
//		}
		if (nebw->_ext==0)
			nb->add(nebw);
		nebw->_ext= 1;
		if (w2end > w1end)
			break;
		w2= nextTargetWire(NoPowerTarget);
	}
}

uint Ath__track::searchSpread(uint srcTrack, void *ip, uint trackDist, uint met, void (*doSpread)(void *, uint, void *, void *, void *))
{
	bool tohi = true;
	uint trackFoundH = initTargetTracks (srcTrack, trackDist, tohi);
	tohi = false;
	uint trackFoundL = initTargetTracks (srcTrack, trackDist, tohi);
	if (!trackFoundH && !trackFoundL)
		return 0;

	//uint dir= _grid->getDir();

	Ath__array1D<Ath__wire*> topNeighbor;
	Ath__array1D<Ath__wire*> botNeighbor;

	int noPowerSource = _grid->getGridTable()->noPowerSource();
	bool allNet = _grid->getGridTable()->allNet();
	uint wireCnt = 0;
	Ath__wire* wire= NULL;
	Ath__track *tstrack;
	uint delt;
	for (uint jj= _grid->searchLowMarker(); jj<=_grid->searchHiMarker(); jj++)
	{
		for (wire= _marker[jj]; wire!=NULL; wire= wire->_next)
		{
			if (jj<_markerCnt-1 && wire==_marker[jj+1])
				break;
			if (noPowerSource && wire->isPower())
				continue;
			if (!allNet && !wire->getNet()->isMarked())
				continue;
			if (wire->_srcId > 0)
				continue;

//			if (_grid->getMinMaxTrackNum(wire->_base+wire->_width) != srcTrack)
//			{
//				if (!upRangeIncreased)
//					increaseUpRange();
//				upRangeIncreased = 1;
//				delt = 1;
//			}

			wireCnt++;
			topNeighbor.resetCnt();
			botNeighbor.resetCnt();
			
			delt = 0;
			tohi = true;
			tstrack = this;
			while (nextSubTrackInRange(tstrack, delt, trackDist, srcTrack, tohi))
			{
				tstrack->findNeighborWire(wire, &topNeighbor, tohi);

			}
			delt = 0;
			tohi = false;
			tstrack = this;
			while (nextSubTrackInRange(tstrack, delt, trackDist, srcTrack, tohi))
			{
				tstrack->findNeighborWire(wire, &botNeighbor, tohi);

			}
			doSpread (ip, met, (void *)wire, (void *)(&topNeighbor), (void *)(&botNeighbor));
			uint j1;
			for (j1 = 0; j1 < topNeighbor.getCnt(); j1++)
				topNeighbor.get(j1)->setExt(0);
			for (j1 = 0; j1 < botNeighbor.getCnt(); j1++)
				botNeighbor.get(j1)->setExt(0);
		}
	}
	return wireCnt;
}

uint Ath__grid::searchSpread(void *ip, uint spreadTrackDist, void (*doSpread)(void *, uint, void *, void *, void *))
{

	setSearchDomain(0);
	uint cnt= 0;
	for (uint ii= _searchLowTrack; ii<=_searchHiTrack; ii++)
	{
		Ath__track *btrack= _trackTable[ii];
		if (btrack==NULL)
			continue;

		Ath__track *track = NULL;
		bool tohi = true;
		while ((track = btrack->getNextSubTrack(track, tohi))>0)
		{
			uint cnt1= track->searchSpread(ii, ip, spreadTrackDist, _level, doSpread);
			cnt += cnt1;
		}
	}

	return cnt;
}
uint Ath__gridTable::searchSpread(void *ip, uint spreadTrackDist, std::vector<dbNet *> & inets, char *bbox, void (*doSpread)(void *, uint, void *, void *, void *))
{
	uint nn = inets.size();
	nn ? setAllNet(false) : setAllNet(true);
	uint ni;
	for (ni = 0; ni < nn; ni++)
		inets[ni]->setMark(true);
	setNoPowerSource (1);
	setNoPowerTarget (0);
	uint cnt= 0;
	for (uint jj= 0; jj<_colCnt; jj++) {
		// for (uint ii= 0; ii<_rowCnt; ii++) {
		for (int ii= _rowCnt-1; ii>=0; ii--) {

			Ath__grid *netGrid= _gridTable[ii][jj];
			if (netGrid==NULL)
				continue;

			netGrid->searchSpread(ip, spreadTrackDist, doSpread);
		}
	}
	for (ni = 0; ni < nn; ni++)
		inets[ni]->setMark(false);
	return cnt;
}

