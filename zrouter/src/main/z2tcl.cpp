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
#include "../defout/defout.h"
	
int getZuiObjects(Ath__zui *zui, bool gcells, bool gpaths, bool congestion, bool hotspots);

// USING_NAMESPACE_ADS;

Ath__zroute::Ath__zroute(uint layerCnt, uint masterCnt, uint initBlockCnt)
{
	_new_db = NULL;
	_new_tech = NULL;
	_new_lib = NULL;
	_new_chip = NULL;
	
	_dbSchema= 1;
	
	//uint units= 1000;
	_zui= new Ath__zui();
	_vui= new Ath__zui();
	_parser= new Ath__parser();
	
	_tmpTable_uint= new uint[1000000]; //TODO
	
	_rdb= NULL;
	_wdb= NULL;
	
	initHandleTable();
	_topBlockOnly= false;
	_skipTopBlock= false;
	
	_isdbFlag= false;
	
	// _extGuiDcr= NULL;

}
void Ath__zroute::initHandleTable()
{
	_handleCnt= 0;
	for (uint ii= 0; ii<128; ii++)
		_handleTable[ii]= NULL;
}
void Ath__zroute::destroyHandleTable()
{
	for (uint ii= 0; ii<128; ii++)
		if (_handleTable[ii] != NULL)
			delete _handleTable[ii];
}
Ath__zroute::~Ath__zroute()
{
	delete _zui;
	delete _vui;
	delete _parser;
		
	delete [] _tmpTable_uint;

}
dbBlockHandle *Ath__zroute::getFirstBlockHandle(char *list)
{
	//uint blkCnt= mkSearchDb(false, false);
	mkSearchDb(NULL, false);

	_currentBlockIndex= 0;
	_currentWordCnt= 0;
	_allFlag= 0;

	if (list==NULL)
		return _handleTable[1];

	if (strcmp(list, "ALL")==0) {
		_allFlag= 1;
		_currentBlockIndex= 1;
		dbBlockHandle *handle= _handleTable[1]; // skipping top level

		if (_handleCnt>2) {
			
			handle= _handleTable[2]; // skipping top level
			if (handle==NULL)
				return NULL;
			
			_currentBlockIndex= 2;
		}
		return handle;
	}
	dbBlock *topBlock= _new_chip->getBlock();
	
	_currentWordCnt= _parser->mkWords(list);
	for (uint ii= 0; ii<_currentWordCnt; ii++) {
		dbBlock *block= topBlock->findChild(_parser->get(ii));
		if (block==NULL)
			continue;
		
		_currentBlockIndex= ii;
		return _handleTable[block->getId()];
	}
	return NULL;
}
dbBlockHandle *Ath__zroute::getNextBlockHandle()
{
	if (_allFlag>0) {
		_currentBlockIndex ++;

		if (_currentBlockIndex >= _handleCnt)
			return NULL;

		return _handleTable[_currentBlockIndex];
	}
	dbBlock *topBlock= _new_chip->getBlock();

	for (uint ii= _currentBlockIndex+1; ii<_currentWordCnt; ii++) {
		dbBlock *block= topBlock->findChild(_parser->get(ii));
		if (block==NULL)
			continue;
		_currentBlockIndex= ii;
		return _handleTable[block->getId()];
	}
	return NULL;
}
uint Ath__zroute::makeTblocks(char *list)
{
	uint cnt= 0;
	dbBlockHandle *blkHandle= getFirstBlockHandle(list);
	for ( ; blkHandle!=NULL; blkHandle= getNextBlockHandle() ) {

		fprintf(stdout, "\n...creating Tblocks from tiles for block %s\n", 
			blkHandle->_block->getName().c_str());

		blkHandle->createTblocks();
		cnt ++;
	}
	return cnt;
}
uint Ath__zroute::makeTiles_1(char *list, int rows, int cols, char* sepH, char *sepV)
{
	uint levelH= 0;
	uint levelV= 0;
	if (sepH!=NULL)
		levelH= _new_tech->findLayer(sepH)->getRoutingLevel();

	if (sepV!=NULL)
		levelV= _new_tech->findLayer(sepV)->getRoutingLevel();

	uint cnt= 0;
	dbBlockHandle *blkHandle= getFirstBlockHandle(list);
	for ( ; blkHandle!=NULL; blkHandle= getNextBlockHandle() ) {

		fprintf(stdout, "\n...making tiles for block %s\n", 
			blkHandle->_block->getName().c_str());

		blkHandle->makeQuadTable(rows, cols, levelH, levelV);
		cnt ++;
	}
	return cnt;
}
uint Ath__zroute::makeTiles(char *list, int rows, int cols, char* sepH, char *sepV)
{
	mkSearchDb(NULL, false);
	return makeTiles_1(list, rows, cols, sepH, sepV);
}
int Ath__zroute::assign_tracks_1(char *list, char* topH, char *topV)
{
	uint top_h= _new_tech->findLayer(topH)->getRoutingLevel();
	uint top_v= _new_tech->findLayer(topV)->getRoutingLevel();

	uint cnt= 0;
	dbBlockHandle *blkHandle= getFirstBlockHandle(list);
	for ( ; blkHandle!=NULL; blkHandle= getNextBlockHandle() ) {

		fprintf(stdout, "\n...Assigning Tracks for block %s\n", 
			blkHandle->_block->getName().c_str());

		blkHandle->assignTracks(top_h, top_v);
		cnt ++;
	}
	return cnt;
}
int Ath__zroute::assign_tracks(char *list, char* topH, char *topV)
{
	return assign_tracks_1(list, topH, topV);
}
int Ath__zroute::assign_pins_1(char *list, char *tile, int connType, int priority, int style, 
		int space, int space_units, int width, int width_units, int length, int length_units,
		char *top_h_layer, char *top_v_layer)
{
	uint top_h= _new_tech->findLayer(top_h_layer)->getRoutingLevel();
	uint top_v= _new_tech->findLayer(top_v_layer)->getRoutingLevel();

	width *= 200;
	length *= 200;

	uint cnt= 0;
	dbBlockHandle *blkHandle= getFirstBlockHandle(list);
	for ( ; blkHandle!=NULL; blkHandle= getNextBlockHandle() ) 
	{
		fprintf(stdout, "\n...Assigning Pins for block %s\n", 
			blkHandle->_block->getName().c_str());

		cnt += blkHandle->_quad->makeTopPins(
			connType, priority, style, space, width, length, top_h, top_v);
	}
	return cnt;
}
int Ath__zroute::assign_pins(char *list, char *tile, int connType, int priority, int style, 
		int space, int space_units, int width, int width_units, int length, int length_units,
		char *top_h_layer, char *top_v_layer)
{
	return assign_pins_1(list, tile, connType, priority, style, space, space_units, 
			width, width_units, length, length_units, top_h_layer, top_v_layer);
}
int Ath__zroute::make_buses_1(char *list, char *tile, int opt, int width, int space)
{
	int row, col;
	int n= get2ints(tile, "_", &row, &col);

	uint cnt= 0;
	dbBlockHandle *blkHandle= getFirstBlockHandle(list);
	for ( ; blkHandle!=NULL; blkHandle= getNextBlockHandle() ) 
	{
		fprintf(stdout, "\n...Placing Bus lines for block %s\n", 
			blkHandle->_block->getName().c_str());

		if (n==0)
			cnt += blkHandle->_quad->makeTopBuses_1(opt, width, space);
		else
			cnt += blkHandle->_quad->makeTopBuses_1(row, col, opt, width, space);
	}
	return cnt;
}
int Ath__zroute::make_buses(char *list, char *tile, int opt, int width, int space)
{
	return make_buses_1(list, tile, opt, width, space);
}

uint Ath__zroute::getLayers_1(char *buff) // TODO turn char* to dbString
{
	if (!(_dbSchema>0))
		return 0;
	
	dbTech * tech= _new_tech;

    dbString layerBuff = "       0 SUB 1\n";

    uint cnt = 0;
    dbSet<dbTechLayer> layers = tech->getLayers();
    dbSet<dbTechLayer>::iterator itr;
    
    for ( itr = layers.begin(); itr != layers.end(); ++itr )
    {
        dbTechLayer * layer = *itr;
        dbTechLayerType type = layer->getType();
        
        if ( type.getValue() == dbTechLayerType::ROUTING )
        {
            dbString lname = layer->getName();
            char buffer[2048];
            
            sprintf(buffer, "       %d %s 1\n",
                    	layer->getRoutingLevel(), lname.c_str() );

            layerBuff += buffer;
        }
    }

    strcpy(buff, layerBuff.c_str());

    //sprintf(layerBuff, "%s %d %s ", layerBuff, ++cnt, "block");
    return cnt;
}
void Ath__zroute::getMaxGuiBox_1(char *buffer)
{
	int maxInt= 2100000000;
	adsRect rect;
	rect.reset(maxInt, maxInt, -maxInt, -maxInt);
	uint cnt= 0;

	uint n= mkSearchDb(NULL, false);
	for (uint ii= 1; ii<=n; ii++) {

		dbBox *box= _handleTable[ii]->_block->getBBox();
		if (box->xMin()==box->xMax())
			continue;

		adsRect b;
		box->getBox(b);

		rect.merge(b);
		cnt ++;
	}
	if (cnt>0) {
		sprintf(buffer, "%d %d %d %d", 
			rect.xMin(), rect.yMin(), rect.xMax(), rect.yMax());
	}
}
void Ath__zroute::getChipAttributes(char *bbox, char *layers, 
									char *hierarchy, char *objects)
{
	if (_new_chip==NULL)
	{
		fprintf(stdout, "DB not connected to Zui or there is no chip loaded\n");
		return;
	}
	_zui->getHierBoxTypes(hierarchy, objects);
	
	Ath__box bb;
	bb.resetMaxBox();
	
	getLayers_1(layers);
	getMaxGuiBox_1(bbox);
}
uint Ath__zroute::tiles_get_1(char *bb)
{	
	uint visualWidth= 5000;
	uint layer= 15;

	uint boxCnt= 0;
	uint n= mkSearchDb(NULL, false);
	for (uint ii= 1; ii<=n; ii++) {
		
		dbBlockHandle *blockHandle= _handleTable[ii];
		if (blockHandle->_quad!=NULL) {
			boxCnt= blockHandle->_quad->getAllTileBoxes(_zui, layer, visualWidth);
		}
		else {
			dbBox *box= blockHandle->_block->getBBox();
			_zui->addBox(box, 0, 0);
		}
	}
	return boxCnt;
}
void Ath__zroute::mkSearchDb(uint force, bool topBlockOnly, bool skipTopBlock, bool isdb)
{
	_isdbFlag= isdb;

	if (force>0) {
		_handleCnt = 0;
	}
	else if (_handleCnt>0) {
		return;
	}
	_topBlockOnly= topBlockOnly;
	_skipTopBlock= skipTopBlock;
	//uint n= mkSearchDb(NULL, isdb);
	mkSearchDb(NULL, isdb);
}

Ath__zui* Ath__zroute::tiles_get(char *bb)
{
	_zui->resetTable(1);
	_zui->invalidateSearchBox();
	_zui->resetExcludeLayer(14); // for pins
	_zui->resetExcludeLayer(15); // for tiles

	uint boxCnt= tiles_get_1(bb);

	if (boxCnt<=0) {
		sprintf(_zui->getTclBuf(), "No data available; maybe no def data loaded yet!\n");
		return 0;
	}
	return _zui;
}
Ath__zui *Ath__zroute::resetZui(uint blockId, char *bb, bool vFlag, bool noclip)
{
	Ath__zui *zui= _zui;
	if (vFlag)
		zui= _vui;

	zui->resetTable(1);

	int x1, x2, y1, y2;
	int n= getCoords(bb, &x1, &y1, &x2, &y2);
	if (n>0) {		
		zui->setSearchBox(x1, y1, x2, y2, 0);
	}
	else {
		zui->invalidateSearchBox();
	}
	zui->setSearchClipFlag(!noclip);

	zui->setBlockId(blockId);

	return zui;
}
void Ath__zroute::resetZuiFlags(uint hierFlag, const char *type)
{
	if ((type==NULL) || (strcmp(type, "")==0)) {
		_zui->resetAllTypeFlags(hierFlag);
		return;
	}
	_parser->mkWords((char *) type, " ");
	for (int ii= 0; ii<_parser->getWordCnt(); ii++) {
		char* word= _parser->get(ii);
		_zui->resetTypeFlag(hierFlag, word);
	}
}
void Ath__zroute::resetZuiFlags(const char *layer, const char *hier, const char *type)
{
	resetZuiFlags(0, type);
	resetZuiFlags(1, hier);
}
void Ath__zroute::setZuiFlags(uint hierFlag, const char *type)
{	
	_zui->resetAllTypeFlags(0);
	_zui->resetAllTypeFlags(1);

_zui->resetMenuPathFlags();

	if ((type==NULL) || (strcmp(type, "")==0))
		return;
	
	_parser->mkWords((char *) type, " ");
	for (int ii= 0; ii<_parser->getWordCnt(); ii++) {
		char* word= _parser->get(ii);
		// _zui->setTypeFlag(hierFlag, word);
		_zui->setTypeHierFlag(word);
	}
}
void Ath__zroute::setZuiFlags(const char *layer, const char *hier, const char *type)
{
	setZuiLayers(layer);
//	setZuiFlags(0, type);
	setZuiFlags(1, hier);
}
uint Ath__zroute::resetZuiLayers(const char *layers)
{
	if ((layers==NULL) || (strcmp(layers, "")==0)) {
		_zui->resetAllExcludeLayers();
		return 1;
	}
	_parser->mkWords((char *) layers, " ");
	for (int ii= 0; ii<_parser->getWordCnt(); ii++) {
		int n= _parser->getInt(ii);
		_zui->resetExcludeLayer(n);
	}
	return _parser->getWordCnt();
}
uint Ath__zroute::setZuiLayers(const char *layers)
{
	if ((layers==NULL) || (strcmp(layers, "")==0)) { // get all
		_zui->resetAllExcludeLayers();
		return 1;
	}
	_zui->setAllExcludeLayers();
	_zui->resetExcludeLayer(15);
	_parser->mkWords((char *) layers, " ");
	for (int ii= 0; ii<_parser->getWordCnt(); ii++) {
		int n= _parser->getInt(ii);
		_zui->resetExcludeLayer(n);
	}
	return _parser->getWordCnt();
}

Ath__zui* Ath__zroute::chip_get(const char *bb, const char *layer, const char *type, const char *hierarchy, uint x, uint y, int maxobjects, const char *noclip)
{
	bool noClipFlag= false;
	if ((noclip==NULL) || (strcmp(noclip, "")==0))
		noClipFlag= false;
	else if (strcmp(noclip, "")!=0)
		noClipFlag= true;
	resetZui(0, (char *)bb, false, noClipFlag);	
	setZuiFlags(layer, type, hierarchy);

	_zui->setAggrParams(0,0,0);
#ifndef _WIN32
	if (getenv("TEST_AGGREGATION"))
#endif
		_zui->setAggrParams(x, y, maxobjects);
	
	chip_get_1();

	return _zui;
}
int Ath__zroute::getCoords(char *bb, int *x1, int *y1, int *x2, int *y2)
{
	if (bb==NULL)
		return 0;

	int n= _parser->mkWords(bb);
	if (n>0) {
		*x1= _parser->getInt(0);
		*y1= _parser->getInt(1);
		*x2= _parser->getInt(2);
		*y2= _parser->getInt(3);
	}
	return n;
}
int Ath__zroute::getInt(char *word, int start)
{
	char buff[128];
	uint k= 0;
	for (uint ii= start; word[ii]!='\0'; ii++)
		buff[k++]= word[ii];
	buff[k++]= '\0';

	return atoi(buff);
}
char Ath__zroute::getNameIds3(const char *word, char *sep, int *nameId, int *blockId, int *typeId)
{
	if (word==NULL)
		return '\0';

	int n= _parser->mkWords(word, sep);
	if (n<=1)
		return '\0';

	*blockId= getInt(_parser->get(0), 1);
	*nameId= getInt(_parser->get(1), 1);
	*typeId= _parser->getInt(2);

	return _parser->get(1)[0];
}
char Ath__zroute::getNameIds(const char *word, char *sep, int *nameId, int *blockId)
{
	if (word==NULL)
		return '\0';

	int n= _parser->mkWords(word, sep);
	if (n<=1)
		return '\0';

	*blockId= getInt(_parser->get(0), 1);
	*nameId= getInt(_parser->get(1), 1);

	return _parser->get(1)[0];
}
int Ath__zroute::get2ints(char *bb, char *sep, int *x1, int *y1)
{
	if (bb==NULL)
		return 0;

	int n= _parser->mkWords(bb, sep);
	if (n>0) 
	{
		*x1= _parser->getInt(0);
		*y1= _parser->getInt(1);
	}
	return n;
}
Ath__zui* Ath__zroute::getPullDownMenu(char *objectName)
{
	int nameId;
	int blockId;
	int boxType;
	char objType= getNameIds3(objectName, ".", &nameId, &blockId, &boxType);
	if (nameId==0) {
		sprintf(_vui->getTclBuf(), "Invalid Object"); 
		return _vui; 
	}
	
	Ath__hierType hierType= Ath_hier__chip;

	switch (objType) {
		
	case 't':  // tile
		hierType= Ath_hier__tile;
		break;
		
	case 'i':  // bus
		hierType= Ath_hier__inst;
		break;

	case 'n':  
		hierType= Ath_hier__signal;
		break;

	case 'g':  
		hierType= Ath_hier__groute;
		break;
	}

	_vui->getPullDownMenu(hierType, boxType);
	return _vui;
}
void Ath__zroute::chip_get_1()
{
	uint visualGap= 1000;
	uint tileLayer= 15;

	uint n= mkSearchDb(NULL, false);
	for (uint ii= 1; ii<=n; ii++) {
		_zui->setBlockId(ii);
		
		dbBlockHandle *handle= getBlockHandle(ii);

		if (_isdbFlag) // ISDB
			handle->setInterface2SdbAtDB();
		
		//dbBox *bb= handle->_block->getBBox();

		handle->getBlockBox(_zui, Ath_hier__block, tileLayer, visualGap,false);
	
		if (handle->_quad!=NULL)
			handle->_quad->getTileBoxes(_zui, tileLayer, visualGap);

		handle->getBlockObs(_zui, false);

		handle->getInstBoxes(_zui, Ath_hier__inst);
		handle->white(_zui, Ath_hier__inst, false);

		handle->getInstShapes(_zui, Ath_hier__inst, false, false, false);

		handle->getWiresAndVias_all(_zui, NULL, false);

		handle->getTilePins(_zui);
		handle->getTileBuses(_zui);
/*
		if (_zui->getTypeFlag(hier, Ath_hier__block))
			boxCnt += block->_instTable->getInstBlocks(_zui, Ath_hier__block);

		boxCnt += block->getBlockages(_zui);

		boxCnt += block->getTileBuses(_zui);
*/

		handle->getBtermBoxes(_zui, Ath_hier__block, false);
		handle->getTracks(_zui, false);
	}

	if (n>0)
	{
/*
		getZuiObjects(_zui, 
			_zui->getDbFlag("groute/gcell"),
			_zui->getDbFlag("groute/gpath"), 
			_zui->getDbFlag("groute/spots"), false);
*/
	}
}
