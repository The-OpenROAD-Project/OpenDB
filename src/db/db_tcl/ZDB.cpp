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

#include <errno.h>
#include <ctype.h>
#include "ZDB.h"
#include "ZSession.h"
#include "ZNamespace.h"
#include "ZDB_Tcl.h"
#include "db.h"
#include "logger.h"
#include "dbExtControl.h"
#include "dbWireGraph.h"
#include "../../lefin/lefin.h"
#include "../../defin/defin.h"
#include "lefout.h"
#include "../../defout/defout.h"
// #include "../../ui/IUi.h"
#include "../dbDiff.h"
#include "ztop.h"
#include "wOrder.h"
#include "IZdcr.h"
#include "IZgui.h"
#include "../dbSearch.h"
#include "../dbBlock.h"
#include "../dbTmgJournal.h"
#include "../dbFlatten.h"
#include "sdc_db.h"
#include "gz.h"
//#include "../../tmg/tmg_db.h" - breaking the link to TMG - payam 01/09/2006

#ifdef _WIN32
#define random rand
#define strncasecmp _strnicoll
#endif

extern int db_test_wires( dbDatabase * db );
extern void db_test_wires2( dbBlock * block );

namespace odb {
extern void db_report_regions( dbBlock *blk );
int AthResourceLog(char *title, int smallScale=0);
// breaking the link to TMG - payam 01/09/2006
#ifdef FULL_ECO
void tmgDbdestroyRcmodels(std::vector<dbNet *> &nets);
#endif

static uint parseDbUnits( const char *fileName)
{
	Ath__parser parser;
	parser.openFile((char *) fileName);

	uint units= 0;

	while (parser.parseNextLine()>0) {
		//parser.printWords(stdout);

		if (parser.isKeyword(0, "MACRO")) {
			units= 0;
			break;
		}

		if (parser.isKeyword(0, "DATABASE")) {
			units=	parser.getInt(2);
			break;
		}

		if (parser.isKeyword(0, "UNITS") && 
			parser.isKeyword(1, "DISTANCE") &&
			parser.isKeyword(2, "MICRONS")) {

			units=	parser.getInt(3);
			break;
		}
	}
//	notice(0, "Parsing file %s for DATABASE MICRONS ...\n", fileName);

	return units;
}
ZDB::ZDB( ZArgs * in, ZArgs * out ) : ZTechModule<ZDB>( in, out )
{
    ZIn_ZDB_ZDB * in_args = (ZIn_ZDB_ZDB *) in;
    ZOut_ZDB_ZDB * out_args = (ZOut_ZDB_ZDB *) out;

    dbObject * o = in_args->duplicate();

    if ( o )
    {
        if ( o->getObjectType() != dbDatabaseObj )
        {
            error(0, "-duplicate argument is not of type dbDatabase");
        }
        
        _db = dbDatabase::duplicate( (dbDatabase *) o );
    }
    else
        _db = dbDatabase::create();

    _context._session->_ns->registerDb(_db);
    out_args->db(_db);
    _io_bufsize = 65536;
    bufsize = "64K";
}

ZDB::~ZDB()
{
#ifdef CONNECT_ZTOP
    // BEGIN temporary kludge to connect to ztop
    Ath__zroute * Z = Ath__getZtop();
    if ( Z->_new_db == _db )
    {
        Z->_new_db = NULL;
        Z->_new_lib = NULL;
        Z->_new_tech = NULL;
        Z->_new_chip = NULL;
    }
    // END temporary kludge to connect to ztop
#endif
    _context._session->_ns->unregisterDb(_db);
    dbDatabase::destroy(_db);
}

bool ZDB::bufsizeChange( const std::string & s )
{
    const char * b = s.c_str();
    char buffer[128];
    int i;
   
    for(  i = 0; (i < 127) && isdigit(*b); ++i, ++b )
        buffer[i] = *b;

    buffer[i] = '\0';
        
    if ( *b == '\0' )
    {
        _io_bufsize = atoi( buffer );
        return true;
    }
    else if ( (*b == 'K') ||(*b == 'k') )
    {
        _io_bufsize = atoi( buffer ) * 1024;
        return true;
    }
    else if ( (*b == 'M') ||(*b == 'm') )
    {
        _io_bufsize = atoi( buffer ) * 1024 * 1024;
        return true;
    }

    return false;
}

int ZDB::clear( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_clear * in_args = (ZIn_ZDB_clear *) in;
	if ( in_args->block() )
    {
        dbBlock* blk= (dbBlock *) in_args->block();
        blk->clear();
		return TCL_OK;
    }
	
    _db->clear();
    return TCL_OK;
}
int ZDB::find( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_find * in_args = (ZIn_ZDB_find *) in;
    ZOut_ZDB_find * out_args = (ZOut_ZDB_find *) out;

    dbChip * chip = _db->getChip();

    if ( chip == NULL )
    {
        warning(0, "database has no chip");
        return TCL_ERROR;
    }

    dbBlock * block = chip->getBlock();
    
    if ( block == NULL )
    {
        warning(0, "chip has no block");
        return TCL_ERROR;
    }
        
    int argc = 0;

    if ( in_args->lib() )
        argc++;

    if ( in_args->bterm() )
        argc++;
    
    if ( in_args->net() )
        argc++;

    if ( in_args->inst() )
        argc++;

    if ( in_args->block() )
        argc++;

    if ( argc == 0 )
        return TCL_OK;

    if ( argc != 1 )
    {
        warning(0, "multiple object types specified");
        return TCL_ERROR;
    }
    
    if ( in_args->lib() )
    {
        dbLib * lib;
        lib = _db->findLib( in_args->lib() );

        if ( lib == NULL )
        {
            warning(0, "library not found");
            return TCL_ERROR;
        }
    
        out_args->object(lib);
    }
    else if ( in_args->bterm() )
    {
        dbBTerm * bterm;
        bterm = block->findBTerm( in_args->bterm() );

        if ( bterm == NULL )
        {
            warning(0, "bterm not found");
            return TCL_ERROR;
        }
    
        out_args->object(bterm);
    }

    else if ( in_args->net() )
    {
        dbNet * net;
        net = block->findNet( in_args->net() );

        if ( net == NULL )
        {
            warning(0, "net not found");
            return TCL_ERROR;
        }
    
        out_args->object(net);
    }
    
    else if ( in_args->inst() )
    {
        dbInst * inst;
        inst = block->findInst( in_args->inst() );

        if ( inst == NULL )
        {
            warning(0, "inst not found");
            return TCL_ERROR;
        }
    
        out_args->object(inst);
    }
    else if ( in_args->block() )
    {
        dbBlock * child;
        child = block->findChild( in_args->block() );

        if ( child == NULL )
        {
            warning(0, "block %s not found",in_args->block());
            return TCL_ERROR;
        }
        out_args->object(child);
    }

    return TCL_OK;
}
int ZDB::get_children( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_get_children * in_args = (ZIn_ZDB_get_children *) in;
    ZOut_ZDB_get_children * out_args = (ZOut_ZDB_get_children *) out;
	
	dbBlock* block= NULL;
	
	if ( in_args->chip() || in_args->top()) {
		dbChip * chip = _db->getChip();
		block= chip->getBlock();
	}
	else if ( in_args->block() ) {
		block= (dbBlock*) in_args->block();
	}
	
	std::string name_list;
	std::list<dbBlock*> blk_list;

	dbSet<dbBlock> children = block->getChildren();
	dbSet<dbBlock>::iterator itr;
	
    uint cnt= 0;
	for( itr = children.begin(); itr != children.end(); ++itr )
	{
		dbBlock *blk = *itr;

		dbString name= blk->getName();
		name_list.append(" ");
		name_list.append(name.c_str());
		//notice(0, "block %s\n", name.c_str());

		blk_list.push_back(blk);
        cnt++;
	}
//	out_args->block_list(blk_list);
    notice(0, "Total %d blocks \n", cnt);
	out_args->block_name_list(name_list.c_str());
	return TCL_OK;
}
int ZDB::get( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_get * in_args = (ZIn_ZDB_get *) in;
    ZOut_ZDB_get * out_args = (ZOut_ZDB_get *) out;
    
    int argc = 0;

    if ( in_args->db() )
        argc++;

    if ( in_args->chip() )
        argc++;

    if ( in_args->tech() )
        argc++;
    
    if ( in_args->top() )
        argc++;

    if ( argc == 0 )
    {
        warning(0, "object type not specified");
        return TCL_ERROR;
    }

    if ( argc != 1 )
    {
        warning(0, "multiple object types specified");
        return TCL_ERROR;
    }

    if ( in_args->db() )
    {
        out_args->object(_db);
    }

    else if ( in_args->chip() )
    {
        dbChip * chip = _db->getChip();

        if ( chip == NULL )
        {
            warning(0, "chip does not exist");
            return TCL_ERROR;
        }

        out_args->object(chip);
    }

    else if ( in_args->tech() )
    {
        dbTech * tech = _db->getTech();

        if ( tech == NULL )
        {
            warning(0, "technology does not exist");
            return TCL_ERROR;
        }

        out_args->object(tech);
    }
    
    else if ( in_args->top() )
    {
        dbChip * chip = _db->getChip();

        if ( chip == NULL )
        {
            warning(0, "chip does not exist");
            return TCL_ERROR;
        }

        dbBlock * block = chip->getBlock();

        if ( block == NULL )
        {
            warning(0, "top-block does not exist");
            return TCL_ERROR;
        }

        out_args->object(block);
    }

    return TCL_OK;
}
int ZDB::load_lef( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_load_lef * in_args = (ZIn_ZDB_load_lef *) in;


	bool ignoreNonRoutingLayers= in_args->ignore_non_routing_layers();
    lefin reader(_db, ignoreNonRoutingLayers);

    if ( in_args->noobs() )
        reader.skipObstructions();

    int user_dbu_per_micron= in_args->dbu_per_micron();
	if (user_dbu_per_micron > 0 )
        	reader.dbu_per_micron( user_dbu_per_micron );
	
	uint def_dbu_per_micron= 0;
	if (user_dbu_per_micron <= 0) {
		std::list<std::string> def_file_list;
		in_args->target_def_list_file(def_file_list);
		if (! def_file_list.empty() && (user_dbu_per_micron <= 0)) {
			bool different_dbu_per_def= false;
			
			std::list<std::string>::iterator it;
			for (it = def_file_list.begin(); it != def_file_list.end(); ++it ) {
				std::string str = *it;
				
				uint n= parseDbUnits(str.c_str()); 
                
                if (n==0) {
			        warning(0, "There is no <DISTANCE UNITS> statements in DEF file %s\n", 
                            str.c_str());
                    continue;
                }
				
				if ((def_dbu_per_micron>0) && (n!=def_dbu_per_micron))
					different_dbu_per_def= true;
				
				if (n>def_dbu_per_micron)
					def_dbu_per_micron= n;
			}
			if (different_dbu_per_def) {
				warning(0, "There are multiple UNIT DISTANCE MICRONS values defined in DEF files");
				warning(0, "Maximum value defined in DEF files is set to %d\n", def_dbu_per_micron);
			}
		}
	}
    
    dbLib * lib;
	
    std::list<std::string> file_list;
    in_args->list_file(file_list);
	
    if (! file_list.empty()) {
		if (user_dbu_per_micron <= 0) {
			bool different_dbu_per_lef= false;
			uint lef_dbu_per_micron= 0;

			std::list<std::string>::iterator it;
			for (it = file_list.begin(); it != file_list.end(); ++it ) {
				std::string str = *it;

				uint n= parseDbUnits(str.c_str());
                if (n==0)
                    continue;

				if ((lef_dbu_per_micron>0) && (n!=lef_dbu_per_micron))
					different_dbu_per_lef= true;

				if (n>lef_dbu_per_micron)
					lef_dbu_per_micron= n;
			}
			if (lef_dbu_per_micron==0) { 
                lef_dbu_per_micron = 1000;
                warning(0, "There is no <DATABASE MICRONS> statements in LEF(s) files");
                warning(0, "Use default value %d\n", lef_dbu_per_micron);
            }
            else if (different_dbu_per_lef) {
				warning(0, "There are multiple DATABASE MICRONS values defined in LEF files\n");
				warning(0, "Use the maximum value %d\n", lef_dbu_per_micron);
			}
			if ((def_dbu_per_micron>0) && (lef_dbu_per_micron>0) && (def_dbu_per_micron!=lef_dbu_per_micron))
			    
				warning(0, "Incompatibility between LEF db units %d and DEF db units %d\n", 
					lef_dbu_per_micron, def_dbu_per_micron);
			if (def_dbu_per_micron || lef_dbu_per_micron) {
				uint final_dbu_per_micron = def_dbu_per_micron>lef_dbu_per_micron ? def_dbu_per_micron : lef_dbu_per_micron;

				reader.dbu_per_micron(final_dbu_per_micron);
				warning(0, "Setting db units per micron to %d\n", final_dbu_per_micron);
			}
		}
        lib = reader.createTechAndLib( in_args->lib(), file_list );
		
        if ( lib == NULL )
        {
            warning(0, "Failed to read lef files");
            return TCL_ERROR;
        }
        return TCL_OK;
    }
	if (user_dbu_per_micron<=0) {
		uint lef_dbu_per_micron= parseDbUnits(in_args->file());

		if (lef_dbu_per_micron==0) {
			warning(0, "There is no <DATABASE MICRONS> statements in lef file %s\n", in_args->file());
        }
        else if ((def_dbu_per_micron>0) && (def_dbu_per_micron>lef_dbu_per_micron)) {
			warning(0, "Incompatibility between LEF db units %d and DEF db units %d\n", 
				lef_dbu_per_micron, def_dbu_per_micron);
			
			lef_dbu_per_micron= def_dbu_per_micron;
			
			warning(0, "Setting db units per micron to %d\n", lef_dbu_per_micron);
			reader.dbu_per_micron(lef_dbu_per_micron);
		}
	}
    if ( in_args->techonly() )
    {
        if ( reader.createTech( in_args->file() ) == NULL )
        {
            warning(0, "Failed to read lef file (%s)", in_args->file() );
            return TCL_ERROR;
        }
    }
    
    else if ( in_args->notech() || in_args->update() )
    {
        if ( in_args->update() )
        {
            lib = _db->findLib( in_args->lib() );

            if ( lib == NULL )
            {
                warning(0, "cannot update library, because it does not exist");
                return TCL_ERROR;
            }
            
            if ( ! reader.updateLib( lib, in_args->file() ) )
            {
                warning(0, "failed to read lef file");
                return TCL_ERROR;
            }
        }
        else
            lib = reader.createLib( in_args->lib(), in_args->file() );

        if ( lib == NULL )
        {
            warning(0, "Failed to read lef file (%s)", in_args->file() );
            return TCL_ERROR;
        }
    }
    else
    {
        lib = reader.createTechAndLib( in_args->lib(), in_args->file() );

        if ( lib == NULL )
        {
            warning(0, "Failed to read lef file (%s)", in_args->file() );
            return TCL_ERROR;
        }
    }

    return TCL_OK;
}

int ZDB::load_def( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_load_def * in_args = (ZIn_ZDB_load_def *) in;

    defin reader(_db);

    if ( in_args->noconns() )
        reader.skipConnections();

    if ( in_args->nowires() )
        reader.skipWires();

    if ( in_args->nospcwires() )
        reader.skipSpecialWires();

    if ( in_args->noshields() )
        reader.skipShields();

    if ( in_args->noblockwires() )
        reader.skipBlockWires();

    if ( in_args->nofillwires() )
        reader.skipFillWires();

    if ( in_args->name() )
        reader.useBlockName( in_args->name() );
    
    dbChip * chip;

    std::vector<dbLib *> search_libs;
    
    dbSet<dbLib> libs = _db->getLibs();

//    if ( libs.size() == 0 )
//    {
//        warning(0, "Database contains no libraries");
//        return TCL_ERROR;
//    }
    
    dbSet<dbLib>::iterator itr;

    for( itr = libs.begin(); itr != libs.end(); ++itr )
        search_libs.push_back( *itr );

    dbBlock * parent = (dbBlock *) in_args->block();

    if ( parent )
    {
        const char * inst_name = in_args->instance();
        dbInst * parent_inst = NULL;

        if ( inst_name )
        {
            parent_inst = parent->findInst( inst_name );

            if ( parent_inst == NULL )
            {
                warning(0, "Instance (%s) does not exist\n", inst_name );
                return TCL_ERROR;
            }

            if ( parent_inst->getParent() )
            {
                warning(0, "Instance (%s) already bound to a block.\n", inst_name );
                return TCL_ERROR;
            }
            
                notice(0, "Before DEF Read: Instance (%d) (%s) bound to block (%d) (%s).\n", 
			parent_inst->getId(),inst_name, parent->getId(), parent->getConstName() );
        }

        dbBlock * block = reader.createBlock( parent, search_libs, in_args->file() );

        if ( block == NULL )
        {
            warning(0, "Failed to read def file (%s)", in_args->file() );
            return TCL_ERROR;
        }

        if ( parent_inst )
        {
            if ( ! parent_inst->bindBlock(block) )
            {
                warning(0, "Cannot bind block to instance (%s):\n", inst_name );
                warning(0, "    Block-terminals do not have a 1-to-1 corespondence to the master terminals\n");
                dbBlock::destroy(block);
                return TCL_ERROR;
            }

            //void test_bind( dbInst *, dbBlock * );
            //test_bind( parent_inst, block );
                //notice(0, "AFTER DEF read: Instance (%d) (%s) of MasterBlock (%d) (%s) bound to block (%d) (%s).\n", parent_inst->getId(), inst_name, block->getId(), block->getConstName(), parent->getId(), parent->getConstName() );
		dbInst *ii= block->getParentInst();
                notice(0, "AFTER DEF read: Instance (%d) (%s) -- ParentInst (%d) (%s) of MasterBlock (%d) (%s) bound to block (%d) (%s).\n", parent_inst->getId(), inst_name, ii->getId(), ii->getConstName(), block->getId(), block->getConstName(), parent->getId(), parent->getConstName() );
        }
    }
    else
    {
        chip = reader.createChip( search_libs, in_args->file() );

        if ( chip == NULL )
        {
            warning(0, "Failed to read def file (%s)", in_args->file() );
            return TCL_ERROR;
        }
    }
    char msg1[1024];
    sprintf(msg1, "End DEF:  %s. ", in_args->file());
    AthResourceLog (msg1, 0); 

    return TCL_OK;
}
int ZDB::reset_inst_hier( ZArgs * in, ZArgs * out )
{
    	ZIn_ZDB_reset_inst_hier * in_args = (ZIn_ZDB_reset_inst_hier *) in;

    	dbBlock * block = (dbBlock *) in_args->block();
	if (block==NULL) {
		const char *block_name= in_args->block_name();
		if (block_name==NULL) {
			if (!in_args->top()) {
            			warning(0, "command either one option\n");
            			return TCL_ERROR;
			}
			dbChip *chip= _db->getChip();
        		block= chip->getBlock();
		}
	}
	dbSet<dbInst> insts= block->getInsts();
    	dbSet<dbInst>::iterator itr;

	uint cnt= 0;
    	for( itr = insts.begin(); itr != insts.end(); ++itr ) {
		dbInst *inst= *itr;
		cnt += inst->resetHierarchy(true);
	}
	notice(0, "reset hierarchy references %d instances\n", cnt) ;
	return TCL_OK;
}
int ZDB::bind_block( ZArgs * in, ZArgs * out )
{
    	ZIn_ZDB_bind_block * in_args = (ZIn_ZDB_bind_block *) in;

        const char * inst_name = in_args->instance();
    	if ( inst_name==NULL ) {
            	warning(0, "commnad requires instance name\n");
            	return TCL_ERROR;
    	}

    	dbBlock * block = (dbBlock *) in_args->block();
	if (block==NULL) {
		const char *block_name= in_args->block_name();
		if (block_name==NULL) {
            		warning(0, "commnad requires either dbblock or block name\n");
            		return TCL_ERROR;
		}
		dbChip *chip= _db->getChip();
        	dbBlock * child = chip->getBlock()->findChild(block_name);
		if (child==NULL) {
            		warning(0, "Cannot find block with master name %s\n",	
				block_name);
            		return TCL_ERROR;
		}
		block= child;
    	}
    	dbBlock * parent = block->getParent();

    	if ( parent==NULL ) {
            	warning(0, "Block does not have any parent block\n");
            	return TCL_ERROR;
    	}
        dbInst * parent_inst = parent->findInst( inst_name );

	if ( parent_inst == NULL )
       	{
               	warning(0, "Instance (%s) does not exist\n", inst_name);
               	return TCL_ERROR;
       	}

       	if ( parent_inst->getParent() )
       	{
              	warning(0, "Instance (%s) already bound to block (TODO)\n", 
			inst_name);
			//inst_name, parent_inst->getParent()->getName());
               	return TCL_ERROR;
       	}
        if ( ! parent_inst->bindBlock(block, in_args->force()) )
        {
                warning(0, "Cannot bind block to instance (%s):\n", inst_name );
                //dbBlock::destroy(block);
                return TCL_ERROR;
         }
	notice(0, "Binding successful\n");

            //void test_bind( dbInst *, dbBlock * );
            //test_bind( parent_inst, block );
    	return TCL_OK;
}

int ZDB::load_def_eco( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_load_def_eco * in_args = (ZIn_ZDB_load_def_eco *) in;

    defin reader(_db);

    if ( in_args->nowires() )
        reader.skipWires();

    if ( in_args->nospcwires() )
        reader.skipSpecialWires();

    if ( in_args->noshields() )
        reader.skipShields();
    
    if ( in_args->noblockwires() )
        reader.skipBlockWires();

    if ( in_args->nofillwires() )
        reader.skipFillWires();

    if ( in_args->id2names() )
        reader.namesAreDBIDs();

    if (in_args->assemblewires())
        reader.setAssemblyMode();

    dbBlock * block = (dbBlock *) in_args->block();

    if ( block == NULL )
    {
        warning(0, "please select a block to apply ECO");
        return TCL_ERROR;
    }

    if (! (in_args->replacewires() || in_args->assemblewires()) )
    {
        warning(0, "please select an ECO action: [-replacewires || -assemblewires]");
        return TCL_ERROR;
    }

    if ( ! reader.replaceWires( block, in_args->file() ) )
    {
        warning(0, "Failed to read def file (%s)", in_args->file() );
        return TCL_ERROR;
    }
    else
    {
        std::vector<dbNet *> replacedNets;
        block->updateNetFlags(replacedNets);
        //block->destroyParasitics(replacedNets);
        // need to remove nets from extNetSdb first
        // tmg_db::destroyRcmodels(replacedNets); payam 01/09/2006

#ifdef FULL_ECO
#ifndef _WIN32
        tmgDbdestroyRcmodels(replacedNets);
#endif
#endif
    }
    
    return TCL_OK;
}

bool ZDB::read_layer_aliases( const char * file )
{
    dbTech * tech = _db->getTech();

    if ( tech == NULL )
    {
        warning(0, "Database has no technology, cannot map layer names");
        return false;
    }
    
    FILE * f = fopen( file, "r" );

    if ( f == NULL )
    {
        warning(0, "Cannot open layer mapping file (%s)", file );
        return false;
    }

    char buffer[1024];

    while( fgets( buffer, 1024, f ) )
    {
        char layer[1024];
        char alias[1024];
        sscanf(buffer, "%s %s", layer, alias );

        dbTechLayer * l = tech->findLayer( layer );

        if ( l == NULL )
        {
            warning(0, "Cannot find technology layer (%s).", layer );
            fclose(f);
            return false;
        }

        l->setAlias( alias );
    }

    fclose(f);
    return true;
}
int ZDB::default_class_site( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_default_class_site * in_args = (ZIn_ZDB_default_class_site *) in;

    dbSet<dbLib> libs = _db->getLibs(); 
    if ( libs.size() == 0 ) { 
        warning(0, "Database contains no libraries"); 
        return TCL_ERROR; 
    }
    const char *class_name= in_args->class_name();
    const char *site_name= in_args->site_name();
    
    dbSet<dbLib>::iterator itr; 
    for( itr = libs.begin(); itr != libs.end(); ++itr ) { 
        dbLib *lib= *itr; 
        
        notice(0, "\nLooking at Library %s for cells with empty sites\n",
                    lib->getConstName()); 
        dbSite *target_site= lib->findSite( site_name ); 
        if (target_site==NULL) { 
            warning(0, "Cannot find site name %s in Library %s\n",
                    site_name, lib->getConstName()); 
            warning(0, "Will not set site names of the masters for this libaray!\n"); 
            continue; 
        }
        
        uint cCnt= 0;
        uint cnt= 0;
        uint aCnt= 0;

        dbSet<dbMaster> masters= lib->getMasters(); 
        dbSet<dbMaster>::iterator mitr; 
        for( mitr = masters.begin(); mitr != masters.end(); ++mitr ) { 
            dbMaster *m= *mitr;

            const char *type= m->getType().getString();
            if (strcmp(type, class_name)!=0)
                continue;

            cCnt++; // same class

            dbSite *existingSite= m->getSite();
            if (existingSite!=NULL) {
                aCnt ++;
                continue;
            }

            m->setSite(target_site);
            verbose(0, "Master cell %s was assigned site %s\n",
                    m->getConstName(), site_name);
            cnt++;
        }
        notice(0, "Library %s has %d cells of class <%s>\n",
                lib->getConstName(), cCnt, class_name);
        notice(0, "Library %s has %d cells of class <%s> with assigned site\n",
                lib->getConstName(), aCnt, class_name);
        notice(0, "\t%d cells were assigned site %s\n",
                cnt, site_name);
    }
    return TCL_OK;
}
int ZDB::check_lef_sites( ZArgs * in, ZArgs * out )
{
    notice(0, "\nChecking for cells with empty sites\n");

    dbSet<dbLib> libs = _db->getLibs(); 
    if ( libs.size() == 0 ) { 
        warning(0, "Database contains no libraries"); 
        return TCL_ERROR; 
    }
    dbSet<dbLib>::iterator itr; 
    for( itr = libs.begin(); itr != libs.end(); ++itr ) { 
        dbLib *lib= *itr; 
        
        notice(0, "\nChecking Library %s for cells with empty sites\n",
                    lib->getConstName()); 
        
        uint cCnt= 0;
        uint cnt= 0;
        uint aCnt= 0;

        dbSet<dbMaster> masters= lib->getMasters(); 
        dbSet<dbMaster>::iterator mitr; 
        for( mitr = masters.begin(); mitr != masters.end(); ++mitr ) { 
            dbMaster *m= *mitr;

            cnt++;

            const char *type= m->getType().getString();
            if (type==NULL) { 
                cCnt++; // same class
                warning(0, "Master cell %s is not assigned a <CLASS> in LEF\n", 
                        m->getConstName());
            }
            dbSite *existingSite= m->getSite();
            if (existingSite==NULL) {
                if (type!=NULL) { 
                    warning(0, "Master cell %s of CLASS %s has no SITE definition \n", 
                        m->getConstName(), type);
                }
                else { 
                    warning(0, "Master cell %s has no SITE definition in LEF\n", 
                        m->getConstName());
                }
                aCnt ++;
            }
        }
        notice(0, "Library %s has %d total cells\n", lib->getConstName(), cnt);

        notice(0, "Library %s has %d cells without class\n",
                lib->getConstName(), cCnt);
        notice(0, "Library %s has %d cells without site\n",
                lib->getConstName(), aCnt);
    }
    return TCL_OK;
}
int ZDB::save_lef( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_save_lef * in_args = (ZIn_ZDB_save_lef *) in;

    lefout writer;

    dbLib * lib = _db->findLib( in_args->lib() );

    if ( in_args->usemasterids() )
        writer.setUseMasterIds(true);

    if ( in_args->layermap() )
    {
        if ( read_layer_aliases( in_args->layermap() ) )
            writer.setUseLayerAlias(true);
        else
            return TCL_ERROR;
    }

    if ( lib == NULL )
    {
        warning(0, "Library (%s) does not exist", in_args->lib() );
        return TCL_ERROR;
    }

    if ( in_args->notech() )
    {
        if ( ! writer.writeLib( lib, in_args->file() ) )
        {
            warning(0, "Failed to write lef file");
            return TCL_ERROR;
        }
    }
    else
    {
        if ( ! writer.writeTechAndLib( lib, in_args->file() ) )
        {
            warning(0, "Failed to write lef file");
            return TCL_ERROR;
        }
    }

    
    return TCL_OK;
}

int ZDB::save_def( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_save_def * in_args = (ZIn_ZDB_save_def *) in;

    defout writer;

    dbChip * chip = _db->getChip();
    
    if ( chip == NULL )
    {
        warning(0, "No chip to defout exist" );
        return TCL_ERROR;
    }

    dbBlock * block = (dbBlock *) in_args->block();

    if ( block == NULL )
    {
        block = chip->getBlock();
	if (in_args->block_inst()!=NULL) {
        	block = block->findChild( in_args->block_inst() );

        	if ( block == NULL )
        	{
            		warning(0, "block %s not found",in_args->block_inst());
            		return TCL_ERROR;
        	}
	}
    }
    if ( in_args->use_ids() )
        writer.setUseNetInstIds(true);

    if ( in_args->usemasterids() )
        writer.setUseMasterIds(true);

    if ( in_args->layermap() )
    {
        if ( read_layer_aliases( in_args->layermap() ) )
            writer.setUseLayerAlias(true);
        else
            return TCL_ERROR;
    }

    if ( strcmp(in_args->version(), "5.3" ) == 0 )
        writer.setVersion( defout::DEF_5_3);
    else if ( strcmp(in_args->version(), "5.4" ) == 0 )
        writer.setVersion( defout::DEF_5_4);
    else if ( strcmp(in_args->version(), "5.6" ) == 0 )
        writer.setVersion( defout::DEF_5_6);
    else
    {
        warning(0, "Invalid version %s\n", in_args->version() );
        return TCL_ERROR;
    }

    std::vector<dbNet *>snets;
    dbNet *select_net = NULL;
    std::list<int> net_id_list;
    in_args->list_net_id(net_id_list);
    if (! net_id_list.empty()) {
        std::list<int>::iterator it;
        for (it = net_id_list.begin(); it != net_id_list.end(); ++it ) {
            int netid = *it;
            if (netid==0) continue;
            select_net = dbNet::getNet(block, netid);
            if (select_net->isMarked())
                continue;
            select_net->setMark(true);
            snets.push_back(select_net);
            writer.selectNet(select_net);
        }
    }

    if (in_args->net()) {
        std::vector<dbNet *> inets;
        const char *netNames = in_args->net();
        block->findSomeNet(netNames, inets);
        uint j;
        for (j=0;j<inets.size();j++)
        {
            select_net = inets[j];
            snets.push_back(select_net);
            select_net->setMark(true);
            writer.selectNet(select_net);
        }
    }

    std::vector<dbNet *>pnets;
    std::vector<dbInst *>pinsts;
    dbNet *ccNet, *inet;
    uint jj;
    if (in_args->net_of_inst())
    {
        for( jj= 0; jj < snets.size(); jj++)
        {
            dbSet<dbITerm> niterms = snets[jj]->getITerms();
            dbSet<dbITerm>::iterator ntitr;
            for ( ntitr = niterms.begin(); ntitr != niterms.end(); ++ntitr ) {
                dbInst *inst = (*ntitr)->getInst();
                if (inst->getUserFlag1())
                    continue;
                pinsts.push_back(inst);
                inst->setUserFlag1();
                dbSet<dbITerm> iiterms = inst->getITerms();
                dbSet<dbITerm>::iterator ititr;
                dbSigType stype;
                for ( ititr = iiterms.begin(); ititr != iiterms.end(); ++ititr ) {
                    inet = (*ititr)->getNet();
                    if (inet)
                        stype = inet->getSigType();
                    if (!inet || inet->isMarked() || stype == dbSigType::POWER || stype == dbSigType::GROUND)
                        continue;
                    writer.selectNet(inet);
                    inet->setMark(true);
                    inet->setMark_1(true);
                    pnets.push_back(inet);
                }
            }
        }
        for( jj= 0; jj < pinsts.size(); jj++)
            pinsts[jj]->clearUserFlag1();
    }

    if (in_args->cc_partner())
    {
        for( jj= 0; jj < snets.size(); jj++)
        {
            dbSet<dbCapNode> capNodes = snets[jj]->getCapNodes();
            dbSet<dbCapNode>::iterator citr;
            for( citr = capNodes.begin(); citr != capNodes.end(); ++citr )
            {
                dbCapNode * capn = *citr;
                dbSet<dbCCSeg> ccSegs = capn->getCCSegs();
                dbSet<dbCCSeg>::iterator ccitr;
                for( ccitr = ccSegs.begin(); ccitr != ccSegs.end(); )
                {
                    dbCCSeg * cc = *ccitr;
                    ++ccitr;
                    dbCapNode *tcap = cc->getSourceCapNode();
                    if (tcap != capn)
                        ccNet = tcap->getNet();
                    else
                        ccNet = cc->getTargetCapNode()->getNet();
                    if (ccNet->isMarked())
                        continue;
                    writer.selectNet(ccNet);
                    ccNet->setMark(true);
                    ccNet->setMark_1(true);
                    pnets.push_back(ccNet);
                }
            }
        }
    }

    bool write_block_ok = writer.writeBlock( block, in_args->file() );

    for( jj= 0; jj < snets.size(); jj++)
        snets[jj]->setMark(false);
    for( jj= 0; jj < pnets.size(); jj++) {
        pnets[jj]->setMark(false);
        pnets[jj]->setMark_1(false);
    }
    if (!write_block_ok)
    {
        warning(0, "Failed to write def file");
        return TCL_ERROR;
    }

    if (!in_args->mini_lef() || snets.size() == 0)
        return TCL_OK;

    dbLib * lib = _db->findLib( "lib" );
    if (!lib)
    {
        warning(0, "Library lib does not exist");
        return TCL_ERROR;
    }
    std::vector<dbMaster *>smasters;
    dbMaster *master;
    for( jj= 0; jj < snets.size(); jj++)
    {
        dbSet<dbITerm> iterms = snets[jj]->getITerms();
        dbSet<dbITerm>::iterator titr;
        for ( titr = iterms.begin(); titr != iterms.end(); ++titr ) {
            master = (*titr)->getInst()->getMaster();
            if (master->isMarked())
                continue;
            smasters.push_back(master);
            master->setMark(1);
        }
    }
    lefout lwriter;
    lwriter.setWriteMarkedMasters(true);
    char lefname[256];
    sprintf(lefname, "%s.lef", (char*)in_args->file());
    bool lefWriteOk = lwriter.writeTechAndLib( lib, &lefname[0] );
    for( jj= 0; jj < smasters.size(); jj++)
        smasters[jj]->setMark(0);
    if (!lefWriteOk)
    {
        warning(0, "Failed to write lef file");
        return TCL_ERROR;
    }
    return TCL_OK;
}

int ZDB::read( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_read * in_args = (ZIn_ZDB_read *) in;

    int argc = 0;

    if ( in_args->tech() )
        argc++;

    if ( in_args->lib() )
        argc++;

    if ( in_args->block() )
        argc++;

    if ( in_args->libs() )
        argc++;

    if ( argc > 1 )
    {
        warning(0, "only one object can be read at a time");
        return TCL_ERROR;
    }

    if ( in_args->nets() || in_args->parasitics() || in_args->nets() )
    {
        if ( in_args->block() == NULL )
        {
            warning(0, "No block specified");
            return TCL_ERROR;
        }
    }

    FILE * f = in_args->channel();
    
    if ( f == NULL )
    {
        f = fopen( in_args->file(), "rb" );

        if ( f == NULL )
        {
            warning( 0, "Failed to open file \"%s\", because %s.", in_args->file(), strerror(errno) );
            return TCL_ERROR;
        }
    }

    char * buffer = (char *) malloc( _io_bufsize );

    if ( buffer == NULL )
    {
        warning( 0, "Memory allocation failed for io buffer");
        fclose(f);
        return TCL_ERROR;
    }
    
    setvbuf( f, buffer, _IOFBF, _io_bufsize );
    
    if ( in_args->tech() )
    {
        try {
            _db->readTech( f );
        }
        catch( ZException & e )
        {
            warning( 0, "Failed to read tech, because %s.", e._msg );
            dbDatabase::destroy(_db);
            _db = dbDatabase::create();
            fclose(f);
            free( (void *) buffer );
            return TCL_ERROR;
        }
    }

    else if ( in_args->lib() )
    {
        try {
            _db->readLib( f, (dbLib *) in_args->lib() );
        }
        catch( ZException & e )
        {
            warning( 0, "Failed to read lib, because %s.", e._msg );
            dbDatabase::destroy(_db);
            _db = dbDatabase::create();
            fclose(f);
            free( (void *) buffer );
            return TCL_ERROR;
        }
    }

    else if ( in_args->libs() )
    {
        try {
            _db->readLibs( f );
        }
        catch( ZException & e )
        {
            warning( 0, "Failed to read libs, because %s.", e._msg );
            dbDatabase::destroy(_db);
            _db = dbDatabase::create();
            fclose(f);
            free( (void *) buffer );
            return TCL_ERROR;
        }
    }

    else if ( in_args->block() )
    {
        if ( in_args->wires() )
        {
            try {
                _db->readNets( f, (dbBlock *) in_args->block() );
                _db->readWires( f, (dbBlock *) in_args->block() );
            }
            catch( ZException & e )
            {
                warning( 0, "Failed to read wires, because %s.", e._msg );
                dbDatabase::destroy(_db);
                _db = dbDatabase::create();
                fclose(f);
                free( (void *) buffer );
                return TCL_ERROR;
            }
        }

        else if ( in_args->nets() )
        {
            try {
                _db->readNets( f, (dbBlock *) in_args->block() );
            }
            catch( ZException & e )
            {
                warning( 0, "Failed to read wires, because %s.", e._msg );
                dbDatabase::destroy(_db);
                _db = dbDatabase::create();
                fclose(f);
                free( (void *) buffer );
                return TCL_ERROR;
            }
        }
    
        else if ( in_args->parasitics() )
        {
            try {
                if ( !in_args->no_nets() )
                {
                    _db->readNets( f, (dbBlock *) in_args->block() );
                }
                _db->readParasitics( f, (dbBlock *) in_args->block() );
            }
            catch( ZException & e )
            {
                warning( 0, "Failed to read parasitics, because %s.", e._msg );
                dbDatabase::destroy(_db);
                _db = dbDatabase::create();
                fclose(f);
                free( (void *) buffer );
                return TCL_ERROR;
            }
        }
        else
        {
            try {
                _db->readBlock( f, (dbBlock *) in_args->block() );
            }
            catch( ZException & e )
            {
                warning( 0, "Failed to read block, because %s.", e._msg );
                dbDatabase::destroy(_db);
                _db = dbDatabase::create();
                fclose(f);
                free( (void *) buffer );
                return TCL_ERROR;
            }
        }
    }

    else
    {
        try {
            //int timer = _new_htimer(); 
            _db->read( f );
            //int sdiff,ndiff;
            //_get_htimer(timer,sdiff,ndiff);
            //notice(0,"SDIFF: %d\n",sdiff);
        }
        catch( ZException & e )
        {
            warning( 0, "Failed to read database, because %s.", e._msg );
            dbDatabase::destroy(_db);
            _db = dbDatabase::create();
            fclose(f);
            free( (void *) buffer );
            return TCL_ERROR;
        }
    }
    
    fclose(f);
    free( (void *) buffer );
    return TCL_OK;
}

int ZDB::write( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_write * in_args = (ZIn_ZDB_write *) in;

    int argc = 0;

    if ( in_args->tech() )
        argc++;

    if ( in_args->lib() )
        argc++;

    if ( in_args->block() )
        argc++;

    if ( in_args->libs() )
        argc++;

    if ( argc > 1 )
    {
        warning(0, "only one object can be written at a time");
        return TCL_ERROR;
    }

    if ( in_args->nets() || in_args->parasitics() || in_args->nets() )
    {
        if ( in_args->block() == NULL )
        {
            warning(0, "No block specified");
            return TCL_ERROR;
        }
    }

    FILE * f = in_args->channel();
    
    if ( f == NULL )
    {
        f = fopen( in_args->file(), "wb" );

        if ( f == NULL )
        {
            warning( 0, "Failed to open file \"%s\", because %s.", in_args->file(), strerror(errno) );
            return TCL_ERROR;
        }
    }

    char * buffer = (char *) malloc( _io_bufsize );

    if ( buffer == NULL )
    {
        warning( 0, "Memory allocation failed for io buffer");
        fclose(f);
        return TCL_ERROR;
    }
    
    setvbuf( f, buffer, _IOFBF, _io_bufsize );

    if ( in_args->tech() )
    {
        try {
            _db->writeTech( f );
        }
        catch( ZException & e )
        {
            warning( 0, "Failed to write tech, because %s.", e._msg );
            fclose(f);
            free( (void *) buffer );
            return TCL_ERROR;
        }
    }
    
    else if ( in_args->lib() )
    {
        try {
            _db->writeLib( f, (dbLib *) in_args->lib() );
        }
        
        catch( ZException & e )
        {
            warning( 0, "Failed to write lib, because %s.", e._msg );
            fclose(f);
            free( (void *) buffer );
            return TCL_ERROR;
        }
    }

    else if ( in_args->libs() )
    {
        try {
            _db->writeLibs( f );
        }
        catch( ZException & e )
        {
            warning( 0, "Failed to write lib, because %s.", e._msg );
            fclose(f);
            free( (void *) buffer );
            return TCL_ERROR;
        }
    }

    else if ( in_args->block() )
    {
        if ( in_args->wires() )
        {
            try {
                _db->writeNets( f, (dbBlock *) in_args->block() );
                _db->writeWires( f, (dbBlock *) in_args->block() );
            }
            catch( ZException & e )
            {
                warning( 0, "Failed to write wires, because %s.", e._msg );
                fclose(f);
                free( (void *) buffer );
                return TCL_ERROR;
            }
        }

        else if ( in_args->nets() )
        {
            try {
                _db->writeNets( f, (dbBlock *) in_args->block() );
            }
            catch( ZException & e )
            {
                    warning( 0, "Failed to write wires, because %s.", e._msg );
                fclose(f);
                free( (void *) buffer );
                return TCL_ERROR;
            }
        }

        else if ( in_args->parasitics() )
        {
            try {
                if ( !in_args->no_nets() )
                {
                    _db->writeNets( f, (dbBlock *) in_args->block() );
                }
                _db->writeParasitics( f, (dbBlock *) in_args->block() );
            }
            catch( ZException & e )
            {
                warning( 0, "Failed to write parasitics, because %s.", e._msg );
                dbDatabase::destroy(_db);
                _db = dbDatabase::create();
                fclose(f);
                free( (void *) buffer );
                return TCL_ERROR;
            }
        }

        else
        {
		dbBlock *b= (dbBlock *) in_args->block();
		if (in_args->skip_hier_refs())
			b->set_skip_hier_stream(true);
            try {
                _db->writeBlock( f, (dbBlock *) in_args->block() );
            }
            catch( ZException & e )
            {
                warning( 0, "Failed to write block, because %s.", e._msg );
                fclose(f);
                free( (void *) buffer );
                return TCL_ERROR;
            }
		if (in_args->skip_hier_refs())
			b->set_skip_hier_stream(false);
        }
    }
    else
    {
        try {
            _db->write( f );
        }
        catch( ZException & e )
        {
            warning( 0, "Failed to write database, because %s.", e._msg );
            fclose(f);
            free( (void *) buffer );
            return TCL_ERROR;
        }
    }

    fclose(f);
    free( (void *) buffer );
    return TCL_OK;
}

inline bool set_timing_read_op( dbDatabase::TimingReadOp & op, const char * val )
{
    if( strcmp( val, "min" ) == 0 )
        op = dbDatabase::TMG_MERGE_MIN;
    else if( strcmp( val, "max" ) == 0 )
        op = dbDatabase::TMG_MERGE_MAX;
    else if( strcmp( val, "overwrite" ) == 0 )
        op = dbDatabase::TMG_OVERWRITE;
    else
        return false;

    return true;
}
    
int ZDB::read_timing( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_read_timing * in_args = (ZIn_ZDB_read_timing *) in;

    dbBlock * block = (dbBlock *) in_args->block();
    assert( block->getObjectType() == dbBlockObj );
    
    if ( block == NULL )
    {
        warning(0, "No block specified");
        return TCL_ERROR;
    }

    dbDatabase::TimingReadOp slew_op[2];
    dbDatabase::TimingReadOp slack_op[2];

    if ( ! in_args->merge() )
    {
        slew_op[0] = dbDatabase::TMG_OVERWRITE;
        slew_op[1] = dbDatabase::TMG_OVERWRITE;
        slack_op[0] = dbDatabase::TMG_OVERWRITE;
        slack_op[1] = dbDatabase::TMG_OVERWRITE;
    }
    else
    {
        if ( ! set_timing_read_op( slew_op[TMG_MIN], in_args->min_slew_merge() ) )
        {
            warning(0, "Invalid argument, -min_slew_merge %s.",
                    in_args->min_slew_merge());
            return TCL_ERROR;
        }

        if ( ! set_timing_read_op( slack_op[TMG_MIN], in_args->min_slack_merge() ) )
        {
            warning(0, "Invalid argument, -min_slack_merge %s.",
                    in_args->min_slack_merge());
            return TCL_ERROR;
        }

        if ( ! set_timing_read_op( slew_op[TMG_MAX], in_args->max_slew_merge() ) )
        {
            warning(0, "Invalid argument, -max_slew_merge %s.",
                    in_args->max_slew_merge());
            return TCL_ERROR;
        }

        if ( ! set_timing_read_op( slack_op[TMG_MAX], in_args->max_slack_merge() ) )
        {
            warning(0, "Invalid argument, -max_slack_merge %s.",
                    in_args->max_slack_merge());
            return TCL_ERROR;
        }
    }

    FILE * f = in_args->channel();
    
    if ( f == NULL )
    {
        f = fopen( in_args->file(), "rb" );

        if ( f == NULL )
        {
            warning( 0, "Failed to open file \"%s\", because %s.", in_args->file(), strerror(errno) );
            return TCL_ERROR;
        }
    }

    char * buffer = (char *) malloc( _io_bufsize );

    if ( buffer == NULL )
    {
        warning( 0, "Memory allocation failed for io buffer");
        fclose(f);
        return TCL_ERROR;
    }
    
    setvbuf( f, buffer, _IOFBF, _io_bufsize );

    try
    {
        if ( strcmp( in_args->mode(), "min" ) == 0 )
            _db->readTiming( f, block, TMG_MIN, slew_op[TMG_MIN], slack_op[TMG_MIN] );
        if ( strcmp( in_args->mode(), "max" ) == 0 )
            _db->readTiming( f, block, TMG_MAX, slew_op[TMG_MAX], slack_op[TMG_MAX] );
        else
            _db->readTiming( f, block, slew_op, slack_op );
    }
    catch( ZException & e )
    {
        warning( 0, "Failed to read database, because %s.", e._msg );
        dbDatabase::destroy(_db);
        _db = dbDatabase::create();
        fclose(f);
        free( (void *) buffer );
        return TCL_ERROR;
    }
    
    fclose(f);
    free( (void *) buffer );
    return TCL_OK;
}

int ZDB::write_timing( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_write_timing * in_args = (ZIn_ZDB_write_timing *) in;

    dbBlock * block = (dbBlock *) in_args->block();
    assert( block->getObjectType() == dbBlockObj );

    if ( block == NULL )
    {
        warning(0, "No block specified");
        return TCL_ERROR;
    }

    FILE * f = in_args->channel();
    
    if ( f == NULL )
    {
        f = fopen( in_args->file(), "wb" );

        if ( f == NULL )
        {
            warning( 0, "Failed to open file \"%s\", because %s.", in_args->file(), strerror(errno) );
            return TCL_ERROR;
        }
    }

    char * buffer = (char *) malloc( _io_bufsize );

    if ( buffer == NULL )
    {
        warning( 0, "Memory allocation failed for io buffer");
        fclose(f);
        return TCL_ERROR;
    }
    
    setvbuf( f, buffer, _IOFBF, _io_bufsize );

    try
    {
        if ( strcmp( in_args->mode(), "min" ) == 0 )
            _db->writeTiming( f, block, TMG_MIN );
        if ( strcmp( in_args->mode(), "max" ) == 0 )
            _db->writeTiming( f, block, TMG_MAX );
        else
            _db->writeTiming( f, block );
    }
    catch( ZException & e )
    {
        warning( 0, "Failed to write timing, because %s.", e._msg );
        fclose(f);
        free( (void *) buffer );
        return TCL_ERROR;
    }

    fclose(f);
    free( (void *) buffer );
    return TCL_OK;
}

int ZDB::create( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_create * in_args = (ZIn_ZDB_create *) in;
    ZOut_ZDB_create * out_args = (ZOut_ZDB_create *) out;

    int argc = 0;
 
    if ( in_args->tech() )
        argc++;
    
    if ( in_args->lib() )
        argc++;

    if ( in_args->chip() )
        argc++;

    if ( in_args->block() )
        argc++;

    if ( argc > 1 )
    {
        warning(0, "only one object can be created at a time");
        return TCL_ERROR;
    }

    if ( in_args->tech() )
    {
        dbTech * tech = dbTech::create(_db);

        if ( tech == NULL )
        {
            warning(0, "a technology already exists");
            return TCL_ERROR;
        }
        out_args->object( tech );
    }
    
    if ( in_args->lib() )
    {
        if ( _db->getTech() == NULL )
        {
            warning(0, "technology required before a lib can be created.");
            return TCL_ERROR;
        }

        dbLib * lib = dbLib::create(_db, in_args->lib() );

        if ( lib == NULL )
        {
            warning(0, "this library already exists");
            return TCL_ERROR;
        }

        out_args->object( lib );
    }

    if ( in_args->chip() )
    {
        if ( _db->getTech() == NULL )
        {
            warning(0, "technology required before a chip can be created.");
            return TCL_ERROR;
        }

        if ( _db->getChip() )
        {
            warning(0, "a chip already exists");
            return TCL_ERROR;
        }
        
        dbChip * chip = dbChip::create(_db);

        if ( chip == NULL )
        {
            warning(0, "failed to create chip");
            return TCL_ERROR;
        }

        out_args->object( chip );
    }

    if ( in_args->block() )
    {
        dbBlock * parent = (dbBlock *) in_args->parent();

        if ( parent == NULL )
        {
            dbChip * chip = _db->getChip();

            if ( chip == NULL )
            {
                warning(0, "a chip does not exist");
                return TCL_ERROR;
            }

            dbBlock * block = dbBlock::create( chip, in_args->block() );

            if ( block == NULL )
            {
                warning(0, "top block already exists, please specify a parent block");
                return TCL_ERROR;
            }

            out_args->object( block );
        }
        else
        {
            dbBlock * block = dbBlock::create( parent, in_args->block() );

            if ( block == NULL )
            {
                warning(0, "this block already exists");
                return TCL_ERROR;
            }

            out_args->object( block );
        }
    }

    return TCL_OK;
}

int ZDB::remove_wires( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_remove_wires * in_args = (ZIn_ZDB_remove_wires *) in;

    if ( in_args->net() )
    {
    dbNet *tpnet = (dbNet *) in_args->net();
    while (tpnet->getWire())
        dbWire::destroy(tpnet->getWire());
    }

    return TCL_OK;
}
int ZDB::destroy( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_destroy * in_args = (ZIn_ZDB_destroy *) in;

    if ( in_args->lib() )
    {
        dbLib::destroy( (dbLib *) in_args->lib() );
    }

    if ( in_args->chip() )
    {
        dbChip::destroy( (dbChip *) in_args->chip() );
    }

    if ( in_args->block() )
    {
        dbBlock::destroy( (dbBlock *) in_args->block() );
    }

    return TCL_OK;
}

int ZDB::duplicate( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_duplicate * in_args = (ZIn_ZDB_duplicate *) in;
    ZOut_ZDB_duplicate * out_args = (ZOut_ZDB_duplicate *) out;

    dbBlock * block = (dbBlock *) in_args->block();
    const char * s = in_args->name();

    if ( block->getObjectType() != dbBlockObj )
    {
        warning(0, "-block argument is not of type dbBlock");
        return TCL_ERROR;
    }
    
    dbBlock * dup = dbBlock::duplicate( block, s );

    if ( dup == NULL )
    {
        warning(0, "Cannot duplicate a top-block");
        return TCL_ERROR;
    }
    
    out_args->object(dup);
    return TCL_OK;
}

int ZDB::diff( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_diff * in_args = (ZIn_ZDB_diff *) in;
    ZOut_ZDB_diff * out_args = (ZOut_ZDB_diff *) out;

    FILE * out_file = in_args->channel();

    if ( out_file == NULL )
    {
        const char * n = in_args->outfile();

        if ( n && strcmp(n,"") != 0 )
        {
            out_file = fopen( in_args->outfile(), "wb" );

            if ( out_file == NULL )
            {
                warning(0, "Cannot open file %s\n", n);
                return TCL_ERROR;
            }
        }
        else 
            out_file = stdout;
    }
    
    dbDatabase * other = (dbDatabase *) in_args->other();

    if ( other == NULL )
    {
        warning(0, "usage: diff -other <db>\n");
        return TCL_ERROR;
    }

    if ( other->getObjectType() != dbDatabaseObj )
    {
        warning(0, "Argument <other> must be a database object.\n");
        return TCL_ERROR;
    }
    
    if ( dbDatabase::diff(_db, other, out_file, in_args->indent()) )
        out_args->result(true);
    else
        out_args->result(false);

    return TCL_OK;
}

int ZDB::differences( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_differences * in_args = (ZIn_ZDB_differences *) in;
    ZOut_ZDB_differences * out_args = (ZOut_ZDB_differences *) out;

    FILE * out_file = in_args->channel();

    if ( out_file == NULL )
    {
        const char * n = in_args->outfile();

        if ( n && strcmp(n,"") != 0 )
        {
            out_file = fopen( in_args->outfile(), "wb" );

            if ( out_file == NULL )
            {
                warning(0, "Cannot open file %s\n", n);
                return TCL_ERROR;
            }
        }
        else 
            out_file = stdout;
    }
    
    dbBlock * block1 = (dbBlock *) in_args->block1();

    if ( block1 == NULL )
    {
        warning(0, "usage: diff -block1 <block> -block2 <block>\n");
        return TCL_ERROR;
    }

    dbBlock * block2 = (dbBlock *) in_args->block2();

    if ( block1 == NULL )
    {
        warning(0, "usage: diff -block1 <block> -block2 <block>\n");
        return TCL_ERROR;
    }

    if ( block1->getObjectType() != dbBlockObj )
    {
        warning(0, "Argument <block1> must be a block object.\n");
        return TCL_ERROR;
    }

    if ( block2->getObjectType() != dbBlockObj )
    {
        warning(0, "Argument <block2> must be a block object.\n");
        return TCL_ERROR;
    }
    
    if ( dbBlock::differences(block1, block2, out_file, in_args->indent()) )
        out_args->result(true);
    else
        out_args->result(false);
    return TCL_OK;
}

int ZDB::names( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_names * in_args = (ZIn_ZDB_names *) in;
    char name[256];

    dbChip * chip = _db->getChip();

    if ( chip == NULL )
        return TCL_OK;

    dbBlock * block = chip->getBlock();
    
    if ( in_args->net() )
    {
        dbSet<dbNet> nets = block->getNets();
        dbSet<dbNet>::iterator itr;
        for( itr = nets.begin(); itr != nets.end(); ++itr )
        {
            dbNet * o = *itr;
            o->getDbName(name);
            printf("%s\n", name);
            ZASSERT( o == dbObject::resolveDbName( _db, name ) );
        }
    }
    
    if ( in_args->inst() )
    {
        dbSet<dbInst> insts = block->getInsts();
        dbSet<dbInst>::iterator itr;
        for( itr = insts.begin(); itr != insts.end(); ++itr )
        {
            dbInst * o = *itr;
            o->getDbName(name);
            printf("%s\n", name);
            ZASSERT( o == dbObject::resolveDbName( _db, name ) );
        }
    }

    if ( in_args->bterm() )
    {
        dbSet<dbBTerm> bterms = block->getBTerms();
        dbSet<dbBTerm>::iterator itr;
        for( itr = bterms.begin(); itr != bterms.end(); ++itr )
        {
            dbBTerm * o = *itr;
            o->getDbName(name);
            printf("%s\n", name);
            ZASSERT( o == dbObject::resolveDbName( _db, name ) );
        }
    }

    if ( in_args->iterm() )
    {
        dbSet<dbITerm> iterms = block->getITerms();
        dbSet<dbITerm>::iterator itr;
        for( itr = iterms.begin(); itr != iterms.end(); ++itr )
        {
            dbITerm * o = *itr;
            o->getDbName(name);
            printf("%s\n", name);
            ZASSERT( o == dbObject::resolveDbName( _db, name ) );
        }
    }

    if ( in_args->gcell() )
    {
        dbGCellGrid * o =  block->getGCellGrid();
        o->getDbName(name);
        printf("%s\n", name);
        ZASSERT( o == dbObject::resolveDbName( _db, name ) );
    }

    if ( in_args->track() )
    {
        dbSet<dbTrackGrid> trackgrids = block->getTrackGrids();
        dbSet<dbTrackGrid>::iterator itr;
        for( itr = trackgrids.begin(); itr != trackgrids.end(); ++itr )
        {
            dbTrackGrid * o = *itr;
            o->getDbName(name);
            printf("%s\n", name);
            ZASSERT( o == dbObject::resolveDbName( _db, name ) );
        }
    }

    if ( in_args->wire() )
    {
    }

    return TCL_OK;
}

int ZDB::test_wires( ZArgs * in, ZArgs * out )
{
    db_test_wires( _db );
    return TCL_OK;
}

int ZDB::test_wires2( ZArgs * in, ZArgs * out )
{
    dbChip * chip = _db->getChip();

    if ( chip )
    {
        dbBlock * block = chip->getBlock();

        if ( block )
            db_test_wires2( block );
    }
    
    return TCL_OK;
}

int ZDB::test_error( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_test_error * in_args = (ZIn_ZDB_test_error *) in;
    const char *msg = in_args->message();
    notice(0,"ERROR message: %s\n",msg);
    error(0,msg);
    return TCL_OK;
}

int ZDB::test_wire_graph( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_test_wire_graph * in_args = (ZIn_ZDB_test_wire_graph *) in;
    dbBlock * block = (dbBlock *) in_args->block();

    if ( block == NULL )
    {
        printf("missing block argument\n");
        return TCL_ERROR;
    }

    const char * net_name = in_args->net();

    if ( net_name != NULL )
    {
        dbNet * net = block->findNet(net_name);

        if ( net == NULL )
        {
            printf("cannot find net %s\n", net_name );
            return TCL_ERROR;
        }

        dbWireGraph G;
        if ( net->getWire() )
        {
           G.decode( net->getWire() );
           G.encode( net->getWire() );
        }

        return TCL_OK;
    }

    dbSet<dbNet> nets = block->getNets();
    dbSet<dbNet>::iterator itr;

    for( itr = nets.begin(); itr != nets.end(); ++itr )
    {
        dbNet * net = *itr;
        dbWireGraph G;
        if ( net->getWire() )
        {
           G.decode( net->getWire() );
           G.encode( net->getWire() );
        }
    }
    
    return TCL_OK;
}

int ZDB::order_wires( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_order_wires * in_args = (ZIn_ZDB_order_wires *) in;
    dbBlock * block = (dbBlock *) in_args->block();

    if ( block == NULL )
    {
        dbChip * chip = _db->getChip();
        if ( chip == NULL )
        {
            warning(0, "No chip" );
            return TCL_ERROR;
        }
        block = chip->getBlock();
    }

    const char *net_name_or_id = in_args->net();
    bool force = in_args->force();
    bool verbose = in_args->verbose();
    bool quiet = in_args->quiet();
    int maxLength = in_args->max_length();
    int cutLength = in_args->cut_length();

    orderWires( block, net_name_or_id, force, verbose, quiet, cutLength, maxLength);
    return TCL_OK;
}

int ZDB::ext_info( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_ext_info * in_args = (ZIn_ZDB_ext_info *) in;
    ZOut_ZDB_ext_info * out_args = (ZOut_ZDB_ext_info *) out;
    dbBlock * block = (dbBlock *) in_args->block();

    if ( block == NULL )
    {
        dbChip * chip = _db->getChip();
        if ( chip == NULL )
        {
            warning(0, "No chip" );
            return TCL_ERROR;
        }
        block = chip->getBlock();
    }
    dbExtControl *extControl = block->getExtControl();
    if (extControl->_extracted)
        out_args->extracted(true);
    else
        out_args->extracted(false);

    // MH - Jan 2, 2007
    if (extControl->_foreign)
        out_args->foreign(true);
    else
        out_args->foreign(false);

    return TCL_OK;

}
int ZDB::translate_sdc( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_translate_sdc * in_args = (ZIn_ZDB_translate_sdc *) in;
    //ZOut_ZDB_translate_sdc * out_args = (ZOut_ZDB_translate_sdc *) out;

    dbBlock * block = (dbBlock *) in_args->block();

    if ( block == NULL )
    {
        dbChip * chip = _db->getChip();
        if ( chip == NULL )
        {
            warning(0, "No chip" );
            return TCL_ERROR;
        }
        block = chip->getBlock();
		if (block==NULL) {
			warning(0, "cannot find dbBlock\n");
			return TCL_ERROR;
		}
    }
	Sdc_db *sdc= new Sdc_db();

	sdc->translate(block, in_args->infile(), in_args->outfile());

	delete sdc;

	return TCL_OK;
}

int ZDB::get_extraction_corners( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_get_extraction_corners * in_args = (ZIn_ZDB_get_extraction_corners *) in;
    ZOut_ZDB_get_extraction_corners * out_args = (ZOut_ZDB_get_extraction_corners *) out;
    dbBlock * block = (dbBlock *) in_args->block();

    if ( block == NULL )
    {
        dbChip * chip = _db->getChip();
        if ( chip == NULL )
        {
            warning(0, "No chip" );
            return TCL_ERROR;
        }
        block = chip->getBlock();
    }
    std::list<std::string> ecl;
    block->getExtCornerNames(ecl);
    out_args->names(ecl);

    return TCL_OK;

}
int ZDB::get_block_stats( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_get_block_stats * in_args = (ZIn_ZDB_get_block_stats *) in;
    ZOut_ZDB_get_block_stats * out_args = (ZOut_ZDB_get_block_stats *) out;

    dbBlock * block = (dbBlock *) in_args->block();

    if ( block == NULL )
    {
        dbChip * chip = _db->getChip();
        if ( chip == NULL )
        {
            warning(0, "No chip" );
            return TCL_ERROR;
        }
        block = chip->getBlock();
    }
	dbSet<dbInst> insts= block->getInsts();
	uint instCnt= insts.size();
	out_args->inst_count(instCnt);

	dbSet<dbNet> nets= block->getNets();
	uint netCnt= nets.size();
	out_args->net_count(netCnt);

	dbNet *net;
	uint wireCnt = 0;
	dbSet<dbNet>::iterator net_itr;
	for( net_itr = nets.begin(); net_itr != nets.end(); ++net_itr ) {
		net = *net_itr;
		dbSigType type= net->getSigType();
		if ((type==dbSigType::POWER)||(type==dbSigType::GROUND))
			continue;
		if (net->getWire())
			wireCnt += net->getWire()->count();
	}
	out_args->wire_count(wireCnt);

	dbSet<dbITerm> iterms= block->getITerms();
	uint itermCnt= iterms.size();
	out_args->iterm_count(itermCnt);

	dbSet<dbBTerm> bterms= block->getBTerms();
	uint btermCnt= bterms.size();
	out_args->bterm_count(btermCnt);

	if (in_args->print()) {
		notice(0, "%d insts\n", instCnt);
		notice(0, "%d nets\n", netCnt);
		notice(0, "%d wires\n", wireCnt);
		notice(0, "%d iterms\n", itermCnt);
		notice(0, "%d bterms\n", btermCnt);
	}

    return TCL_OK;

}
int ZDB::find_disconnects( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_find_disconnects * in_args = (ZIn_ZDB_find_disconnects *) in;
    dbBlock * block = (dbBlock *) in_args->block();
    if ( block == NULL )
    {
        dbChip * chip = _db->getChip();
        if ( chip == NULL )
        {
            warning(0, "No chip" );
            return TCL_ERROR;
        }
        block = chip->getBlock();
    }
    findDisconnects( block, in_args->verbose() );
    return TCL_OK;
}



int ZDB::connect_ztop( ZArgs * in, ZArgs * out )
{
#ifdef CONNECT_ZTOP
    Ath__zroute * Z = Ath__getZtop();

    if ( Z == NULL )
    {
        warning(0, "getZTop() returned NULL");
        return TCL_ERROR;
    }
    
    Z->_new_db = _db;
    Z->_new_chip = _db->getChip();
    Z->_new_tech = _db->getTech();
    dbSet<dbLib> libs = _db->getLibs();

    if ( libs.size() == 0 )
        Z->_new_lib = NULL;
    else
        Z->_new_lib = *libs.begin();
#endif
    return TCL_OK;
}

int ZDB::make_area_search( ZArgs * in, ZArgs * out )
{
#ifdef ZUI
    ZIn_ZDB_make_area_search * in_args = (ZIn_ZDB_make_area_search *) in;
    
    dbBlock *top= _db->getChip()->getBlock();

    dbBlock * block = (dbBlock *) in_args->block();
    if (block!=NULL) {
        block->initSearchBlock(_db->getTech(), in_args->nets(), in_args->insts(), _context);
        return TCL_OK;
    }
    if (in_args->all_blocks()) {        
        dbSet<dbBlock> children = top->getChildren();
        dbSet<dbBlock>::iterator itr;
        
        for( itr = children.begin(); itr != children.end(); ++itr )
        {
            dbBlock *blk = *itr;
            blk->initSearchBlock(_db->getTech(), in_args->nets(), in_args->insts(), _context);
        }
    }
    if (in_args->top()) {
        top->initSearchBlock(_db->getTech(), in_args->nets(), in_args->insts(), _context);
    }
#endif
    return TCL_OK;
}

int ZDB::clear_timing( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_clear_timing * in_args = (ZIn_ZDB_clear_timing *) in;

    dbBlock * block = (dbBlock *) in_args->block();
    
    if ( block == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    assert( block->getObjectType() == dbBlockObj );
    block->clearTiming();
    return TCL_OK;
}

int ZDB::init_timing( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_init_timing * in_args = (ZIn_ZDB_init_timing *) in;

    dbBlock * block = (dbBlock *) in_args->block();
    
    if ( block == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    assert( block->getObjectType() == dbBlockObj );
    block->initTiming();
    return TCL_OK;
}

int ZDB::test_timing_modes( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_test_timing_modes * in_args = (ZIn_ZDB_test_timing_modes *) in;

    dbBlock * block = (dbBlock *) in_args->block();
    
    if ( block == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    assert( block->getObjectType() == dbBlockObj );
    block->clearTiming();

    dbSet<dbITerm> iterms = block->getITerms();
    dbSet<dbITerm>::iterator iitr;
    for( iitr = iterms.begin(); iitr != iterms.end(); ++iitr )
    {
        dbITerm * it = *iitr;

        if ( it->isSpecial() )
            continue;

        it->initTiming();
        it->setSlewRise( TMG_MIN, random() );
        it->setSlewFall( TMG_MIN, random() );
        it->setSlackRise( TMG_MIN, random() );
        it->setSlackFall( TMG_MIN, random() );
        it->setSlewRise( TMG_MAX, random() );
        it->setSlewFall( TMG_MAX, random() );
        it->setSlackRise( TMG_MAX, random() );
        it->setSlackFall( TMG_MAX, random() );
    }
    
    dbSet<dbBTerm> bterms = block->getBTerms();
    dbSet<dbBTerm>::iterator bitr;
    for( bitr = bterms.begin(); bitr != bterms.end(); ++bitr )
    {
        dbBTerm * bt = *bitr;
        bt->initTiming();
        bt->setSlewRise( TMG_MIN, random() );
        bt->setSlewFall( TMG_MIN, random() );
        bt->setSlackRise( TMG_MIN, random() );
        bt->setSlackFall( TMG_MIN, random() );
        bt->setSlewRise( TMG_MAX, random() );
        bt->setSlewFall( TMG_MAX, random() );
        bt->setSlackRise( TMG_MAX, random() );
        bt->setSlackFall( TMG_MAX, random() );
    }
    
    return TCL_OK;
}
    
int ZDB::attach_gui( ZArgs * in_args, ZArgs * out_args )
{
#ifdef ZUI
    ZIn_ZDB_attach_gui * in = (ZIn_ZDB_attach_gui *) in_args;
    //ZOut_ZDB_attach_gui * out = (ZOut_ZDB_attach_gui *) out_args;

    ZPtr<IZdcr> dcr= in->dcr();

    dbBlock *top= _db->getChip()->getBlock();

    dbBlock * block = (dbBlock *) in->block();
    if (block!=NULL) {
        dbBlockSearch *blkSearch= block->getSearchDb();
        if (blkSearch==NULL) {
            fprintf(stdout, "Have to call <db make_area_search -block>\n");
            return TCL_OK;
        }
        blkSearch->createGuiPlug(dcr, _context);
        return TCL_OK;
    }

    if (in->all_blocks()) {        
        dbSet<dbBlock> children = top->getChildren();
        dbSet<dbBlock>::iterator itr;
        
        for( itr = children.begin(); itr != children.end(); ++itr )
        {
            dbBlock *blk = *itr;
            dbBlockSearch *blkSearch= blk->getSearchDb();
            if (blkSearch==NULL) {
                fprintf(stdout, "Have to call <db make_area_search -all_blocks>\n");
                return TCL_OK;
            }
            blkSearch->createGuiPlug(dcr, _context);        }
    }
    if (in->top()) {
        dbBlockSearch *blkSearch= top->getSearchDb();
        if (blkSearch==NULL) {
            fprintf(stdout, "Have to call <db make_area_search -top>\n");
            return TCL_OK;
        }
        blkSearch->createGuiPlug(dcr, _context);
    }

    fprintf(stdout, "Attached Gui to DB block ...\n");
#endif
	warning(0, "ifdef zui");

    return TCL_OK;
}
int ZDB::test_list( ZArgs * in_args, ZArgs * out_args )
{
    ZIn_ZDB_test_list * in = (ZIn_ZDB_test_list *) in_args;
    ZOut_ZDB_test_list * out = (ZOut_ZDB_test_list *) out_args;

    std::list<adsRect> rect_list;
    std::list<int> int_list;

    in->rects_in(rect_list);
    out->rects_out(rect_list);
    in->ints_in(int_list);
    out->ints_out(int_list);
    
    return TCL_OK;
}

int ZDB::test_swap_master( ZArgs * in_args, ZArgs * out_args )
{
    dbDatabase * db = dbDatabase::create();
    //dbTech * tech = dbTech::create(db);
    dbLib * lib = dbLib::create(db, "lib");
    dbChip * chip = dbChip::create(db);
    dbBlock * block = dbBlock::create(chip, "block");

    dbMaster * m1 = dbMaster::create(lib, "M1");
    dbMTerm * a = dbMTerm::create(m1, "A" );
    dbMTerm * b = dbMTerm::create(m1, "B" );
    dbMTerm * c = dbMTerm::create(m1, "C" );
    m1->setFrozen();

    dbMaster * m2 = dbMaster::create(lib, "M2");
    dbMTerm::create(m2, "B" );
    dbMTerm::create(m2, "A" );
    dbMTerm::create(m2, "C" );
    m2->setFrozen();

    dbInst * inst = dbInst::create( block, m1, "I1" );

    dbNet * x = dbNet::create( block, "X" );
    dbITerm::connect( inst, x, a );
    
    dbNet * y = dbNet::create( block, "Y" );
    dbITerm::connect( inst, y, b );

    dbNet * z = dbNet::create( block, "Z" );
    dbITerm::connect( inst, z, c );

    dbSet<dbITerm> iterms = inst->getITerms();
    dbSet<dbITerm>::iterator itr;

    for( itr = iterms.begin(); itr != iterms.end(); ++itr )
    {
        dbITerm * iterm = *itr;
        dbString in = iterm->getInst()->getName();
        dbString tn = iterm->getMTerm()->getName();
        printf("INST: %s/%s\n", in.c_str(), tn.c_str() );
    }

    iterms = x->getITerms();

    for( itr = iterms.begin(); itr != iterms.end(); ++itr )
    {
        dbITerm * iterm = *itr;
        dbString nn = iterm->getNet()->getName();
        dbString tn = iterm->getMTerm()->getName();
        printf("NET: %s/%s\n", nn.c_str(), tn.c_str() );
    }

    iterms = y->getITerms();

    for( itr = iterms.begin(); itr != iterms.end(); ++itr )
    {
        dbITerm * iterm = *itr;
        dbString nn = iterm->getNet()->getName();
        dbString tn = iterm->getMTerm()->getName();
        printf("NET: %s/%s\n", nn.c_str(), tn.c_str() );
    }

    iterms = z->getITerms();

    for( itr = iterms.begin(); itr != iterms.end(); ++itr )
    {
        dbITerm * iterm = *itr;
        dbString nn = iterm->getNet()->getName();
        dbString tn = iterm->getMTerm()->getName();
        printf("NET: %s/%s\n", nn.c_str(), tn.c_str() );
    }
    
    inst->swapMaster(m2);
    iterms = inst->getITerms();
    
    for( itr = iterms.begin(); itr != iterms.end(); ++itr )
    {
        dbITerm * iterm = *itr;
        dbString in = iterm->getInst()->getName();
        dbString tn = iterm->getMTerm()->getName();
        printf("INST: %s/%s\n", in.c_str(), tn.c_str() );
    }

    iterms = x->getITerms();

    for( itr = iterms.begin(); itr != iterms.end(); ++itr )
    {
        dbITerm * iterm = *itr;
        dbString nn = iterm->getNet()->getName();
        dbString tn = iterm->getMTerm()->getName();
        printf("NET: %s/%s\n", nn.c_str(), tn.c_str() );
    }

    iterms = y->getITerms();

    for( itr = iterms.begin(); itr != iterms.end(); ++itr )
    {
        dbITerm * iterm = *itr;
        dbString nn = iterm->getNet()->getName();
        dbString tn = iterm->getMTerm()->getName();
        printf("NET: %s/%s\n", nn.c_str(), tn.c_str() );
    }

    iterms = z->getITerms();

    for( itr = iterms.begin(); itr != iterms.end(); ++itr )
    {
        dbITerm * iterm = *itr;
        dbString nn = iterm->getNet()->getName();
        dbString tn = iterm->getMTerm()->getName();
        printf("NET: %s/%s\n", nn.c_str(), tn.c_str() );
    }
    return TCL_OK;
}

static void region_callback( void * data, adsRect r )
{
    char * str = (char *) data;
    printf("%s (%d %d) (%d %d)\n", str, r.xMin(), r.yMin(), r.xMax(), r.yMax());
    fflush(stdout);
}

int ZDB::test_ui_region_callback( ZArgs * in_args, ZArgs * out_args )
{
	/* for testing
    ZIn_ZDB_test_ui_region_callback * in = (ZIn_ZDB_test_ui_region_callback *) in_args;

    if ( in->ui() != NULL )
   {
        char * str = strdup("ui region callback test function");
        ZPtr<IUi> ui = in->ui();
        ui->setRegionCallback( region_callback, str );
    }
*/
    return TCL_OK;    
}

bool
ZDB::_parseNonDefaultRule(const char *inrule, dbTechNonDefaultRule *& out_ndrule)
{
  bool have_ndrule = false;
  out_ndrule = NULL;
  if (inrule)
    {
      if (strncasecmp(inrule,"default",7) == 0)
	{
	  have_ndrule = true;
	}
      else
	{
	  out_ndrule = _db->getTech()->findNonDefaultRule(inrule);
	  if (out_ndrule)
	    have_ndrule = true;
	}
    }

  return have_ndrule;
}

int ZDB::set_attribute_for_signal_nets( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_set_attribute_for_signal_nets * in_args = (ZIn_ZDB_set_attribute_for_signal_nets *) in;

    if (! (in_args->new_rule() || in_args->new_wiretype()) )
      {
	warning(0, "Since neither -new_rule nor -new_wiretype were set, command will do nothing\n");
	return TCL_ERROR;
      }

    dbTech * tech = _db->getTech();
    if ((!tech) && (in_args->rule_only() || in_args->new_rule()))
      {
	warning(0, "No technology data found -- cannot set non-default rule\n");
	return TCL_ERROR;
      }

    bool have_inrule, have_setrule;
    dbTechNonDefaultRule *in_ndrule, *new_rule;
    have_inrule = _parseNonDefaultRule(in_args->rule_only(), in_ndrule);
    have_setrule = _parseNonDefaultRule(in_args->new_rule(), new_rule);

    if (in_args->new_rule() && !have_setrule)
      {
	warning(0,"Value %s of -new_rule not valid.\n", in_args->new_rule());
	return TCL_ERROR;
      }

    dbWireType  wrtype(in_args->new_wiretype());
    bool have_wiretype = (wrtype != dbWireType::NONE) || (in_args->new_wiretype() && (strncasecmp(in_args->new_wiretype(),"NONE",4) == 0));
    if (in_args->new_wiretype() && !have_wiretype)
      {
	warning(0,"Value %s of -new_wiretype not valid.\n", in_args->new_wiretype());
	return TCL_ERROR;
      }

    dbChip * chip = _db->getChip();
    if ( chip == NULL )
      {
	warning(0, "chip does not exist");
	return TCL_ERROR;
      }
    dbBlock * topblock = chip->getBlock();

    int rulecnt = 0, wrcnt = 0;
    dbNet  *curdbnet= NULL;
    if (in_args->net() && (strlen(in_args->net()) > 0))
      {
	if (dbIdValidation::isId(in_args->net()))
	  curdbnet = dbNet::getNet(topblock, atoi(in_args->net()));
	else
	  curdbnet = topblock->findNet(in_args->net());
	if (!curdbnet)
	  {
	    warning(0,"\nCould not find net %s\n", in_args->net());
	    return TCL_ERROR;
	  }
	
	if (have_setrule)
	  {
	    verbose(0, "Setting non-default rule %s on net %s\n", new_rule->getName().c_str(), curdbnet->getConstName());
	    curdbnet->setNonDefaultRule(new_rule);
	    ++rulecnt;
	  }

	if (have_wiretype)
	  {
	    verbose(0, "Setting wire type %s on net %s\n", wrtype.getString(), curdbnet->getConstName());
	    curdbnet->setWireType(wrtype);
	    ++wrcnt;
	  }
      }
    else
      {
	dbSet<dbNet>  dbnets = topblock->getNets();
	dbSet<dbNet>::iterator  nitr;
	for (nitr = dbnets.begin(); nitr != dbnets.end(); ++nitr)
	  {
	    dbNet *curdbnet1 = (*nitr);
	    if((curdbnet1->getSigType() == dbSigType::POWER) || (curdbnet1->getSigType() == dbSigType::GROUND))
	      continue;

	    if (have_inrule && (curdbnet1->getNonDefaultRule() != in_ndrule))
	      continue;
	  
	    if (have_setrule)
	      {
		verbose(0, "Setting non-default rule %s on net %s\n", new_rule->getName().c_str(), curdbnet1->getConstName());
		curdbnet1->setNonDefaultRule(new_rule);
		++rulecnt;
	      }

	    if (have_wiretype)
	      {
		verbose(0, "Setting wire type %s on net %s\n", wrtype.getString(), curdbnet1->getConstName());
		curdbnet1->setWireType(wrtype);
		++wrcnt;
	      }
	  }
      }

    if (rulecnt > 0)
      {
	if (new_rule)
	  {
	    if (in_args->net())
	      notice(0,"Setting rule %s for net %s\n", new_rule->getName().c_str(), curdbnet->getName().c_str());
	    else
	      notice(0, "Setting rule %s for %d nets\n", new_rule->getName().c_str(), rulecnt);
	  }
	else
	  {
	    if (in_args->net())
	      notice(0, "Setting net %s to default design rule\n", curdbnet->getName().c_str());
	    else
	      notice(0, "Setting %d nets to default design rule\n", rulecnt);
	  }
      }

    if (wrcnt > 0)
      {
	if (in_args->net())
	  notice(0, "Setting wire type %s for net %s\n", wrtype.getString(), curdbnet->getName().c_str());
	else
	  notice(0, "Setting wire type %s for %d nets\n", wrtype.getString(), wrcnt);
      }

    if ((rulecnt == 0) && (wrcnt == 0))
      notice(0,"No action taken on nets!\n");

    return TCL_OK;
}

int ZDB::count_attribute_for_signal_nets( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_count_attribute_for_signal_nets * in_args = (ZIn_ZDB_count_attribute_for_signal_nets *) in;
    ZOut_ZDB_count_attribute_for_signal_nets * out_args = (ZOut_ZDB_count_attribute_for_signal_nets *) out;

    dbChip   *chip;
    dbBlock  *inblk = (dbBlock *) in_args->block();
    if (!inblk)
      {
	chip = _db->getChip();
	if (!chip)
	  {
	    warning(0, "chip does not exist");
	    return TCL_ERROR;
	  }

	inblk = chip->getBlock();
	if (!inblk)
	  {
	    warning(0, "Top block does not exist");
	    return TCL_ERROR;
	  }
      }

    dbSet<dbNet>  all_nets = inblk->getNets();
    dbSet<dbNet>::iterator nitr;
    int alt = 0, disc = 0;
    for (nitr = all_nets.begin(); nitr != all_nets.end(); ++nitr)
      {
	if(((*nitr)->getSigType() == dbSigType::POWER) || ((*nitr)->getSigType() == dbSigType::GROUND))
	  continue;

	if ((*nitr)->isWireAltered())
	  ++alt;
	if ((*nitr)->isDisconnected())
	  ++disc;
      }

    out_args->altered(alt);
    out_args->disconnected(disc);

    return TCL_OK;
}

int ZDB::rlog( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_rlog * in_args = (ZIn_ZDB_rlog *) in;
    int ssc = in_args->ss() ? 1 : 0;
    AthResourceLog ((char *) in_args->tag(), ssc);
    return TCL_OK;    
}

int ZDB::check_layer( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_check_layer * in_args = (ZIn_ZDB_check_layer *) in;
    dbTech *tech = _db->getTech();
    if (!tech)
    {
        warning(0, "dbTech does not exist. Please do load_lef.\n");
        return TCL_ERROR;
    }
    tech->checkLayer(in_args->type(), in_args->width(), in_args->pitch(), in_args->spacing());
    return TCL_OK;
}

int ZDB::check_backslash( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_check_backslash * in_args = (ZIn_ZDB_check_backslash *) in;
    ZOut_ZDB_check_backslash * out_args = (ZOut_ZDB_check_backslash *) out;
    bool chknet = in_args->net();
    bool chkinst = in_args->inst();
    bool chkmterm = in_args->mterm();
    bool chkbterm = in_args->bterm();

    dbChip   *chip = _db->getChip();
    if (!chip)
    {
        warning(0, "chip does not exist");
        return TCL_ERROR;
    }
    dbBlock  *block = chip->getBlock();

    dbNet *net;
    dbInst *inst;
    dbMTerm * mterm;
    dbBTerm * bterm;
    char *name;
    int netbsh, instbsh, mtermbsh, btermbsh;
    netbsh = instbsh = mtermbsh = btermbsh = 0;
    int net1stbsh, inst1stbsh, mterm1stbsh, bterm1stbsh;
    net1stbsh = inst1stbsh = mterm1stbsh = bterm1stbsh = 0;
    if (!chknet && !chkinst && !chkmterm && !chkbterm)
        chknet = chkinst = chkmterm = chkbterm = true;
    dbSet<dbNet> bnet = block->getNets();
    dbSet<dbNet>::iterator nitr;
    for( nitr = bnet.begin(); nitr != bnet.end(); ++nitr)
    {
        net = (dbNet *) *nitr;
        dbSigType type= net->getSigType();
        if ((type==dbSigType::POWER)||(type==dbSigType::GROUND))
            continue;
        if (chknet)
        {
            name = (char *)net->getConstName();
            if (strchr(name, '\\'))
            {
                netbsh++;
                if (name[0] == '\\')
                {
                   warning (0, "Net %d %s has leading backslash in the name\n", net->getId(), name);
                   net1stbsh++;
                }
                else
                   warning (0, "Net %d %s has backslash in the name\n", net->getId(), name);
            }
        }
        if (chkmterm)
        {
            dbSet<dbITerm> iterms = net->getITerms();
            dbSet<dbITerm>::iterator iterm_itr;
            for( iterm_itr = iterms.begin(); iterm_itr != iterms.end(); ++iterm_itr )
            {
                mterm = (*iterm_itr)->getMTerm();
                name = (char *)mterm->getConstName();
                if (!mterm->isSetMark() && strchr(name, '\\'))
                {
                    mterm->setMark(1);
                    mtermbsh++;
                    if (name[0] == '\\')
                    {
                       warning (0, "MTerm %d %s of master %s has leading backslash in the name\n", mterm->getId(), name, (char *)mterm->getMaster()->getConstName());
                       mterm1stbsh++;
                    }
                    else
                       warning (0, "MTerm %d %s of master %s has backslash in the name\n", mterm->getId(), name, (char *)mterm->getMaster()->getConstName());
                }
            }
        }
        if (chkbterm)
        {
            dbSet<dbBTerm> bterms = net->getBTerms();
            dbSet<dbBTerm>::iterator bterm_itr;
            for( bterm_itr = bterms.begin(); bterm_itr != bterms.end(); ++bterm_itr )
            {
                bterm = *bterm_itr;
                name = (char *)bterm->getConstName();
                if (!bterm->isSetMark() && strchr(name, '\\'))
                {
                    bterm->setMark(1);
                    btermbsh++;
                    if (name[0] == '\\')
                    {
                       warning (0, "BTerm %d %s has leading backslash in the name\n", bterm->getId(), name);
                       bterm1stbsh++;
                    }
                    else
                       warning (0, "BTerm %d %s has backslash in the name\n", bterm->getId(), name);
                }
            }
        }
    }
    if (chkinst)
    {
        dbSet<dbInst> insts = block->getInsts();
        dbSet<dbInst>::iterator inst_itr;
        for( inst_itr = insts.begin(); inst_itr != insts.end(); ++inst_itr )
        {
            inst = *inst_itr;
            name = (char *)inst->getConstName();
            if (!strchr(name, '\\'))
                continue;
            instbsh++;
            if (name[0] == '\\')
            {
               warning (0, "Inst %d %s has leading backslash in the name\n", inst->getId(), name);
               inst1stbsh++;
            }
            else
               warning (0, "Inst %d %s has backslash in the name\n", inst->getId(), name);
        }
    }
    if (netbsh)
        warning (0, "%d nets have backslash in the name\n", netbsh);
    if (net1stbsh)
        warning (0, "%d nets have leading backslash in the name\n", net1stbsh);
    if (instbsh)
        warning (0, "%d insts have backslash in the name\n", instbsh);
    if (inst1stbsh)
        warning (0, "%d insts have leading backslash in the name\n", inst1stbsh);
    if (mtermbsh)
        warning (0, "%d mterms have backslash in the name\n", mtermbsh);
    if (mterm1stbsh)
        warning (0, "%d mterms have leading backslash in the name\n", mterm1stbsh);
    if (btermbsh)
        warning (0, "%d bterms have backslash in the name\n", btermbsh);
    if (bterm1stbsh)
        warning (0, "%d bterms have leading backslash in the name\n", bterm1stbsh);
    if (netbsh || instbsh || mtermbsh || btermbsh)
        out_args->has_backslash(true);
    else
        out_args->has_backslash(false);
    if (!chkmterm && !chkbterm)
        return TCL_OK;
    for( nitr = bnet.begin(); nitr != bnet.end(); ++nitr)
    {
        net = (dbNet *) *nitr;
        dbSigType type= net->getSigType();
        if ((type==dbSigType::POWER)||(type==dbSigType::GROUND))
            continue;
        if (chkmterm)
        {
            dbSet<dbITerm> iterms = net->getITerms();
            dbSet<dbITerm>::iterator iterm_itr;
            for( iterm_itr = iterms.begin(); iterm_itr != iterms.end(); ++iterm_itr )
                (*iterm_itr)->getMTerm()->setMark(0);
        }
        if (chkbterm)
        {
            dbSet<dbBTerm> bterms = net->getBTerms();
            dbSet<dbBTerm>::iterator bterm_itr;
            for( bterm_itr = bterms.begin(); bterm_itr != bterms.end(); ++bterm_itr )
                (*bterm_itr)->setMark(0);
        }
    }
    return TCL_OK;
}


int ZDB::print_wire( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_print_wire * in_args = (ZIn_ZDB_print_wire *) in;
    dbChip   *chip = _db->getChip();
    if (!chip)
    {
        warning(0, "chip does not exist");
        return TCL_ERROR;
    }
    
    dbBlock  *block = chip->getBlock();
    if (!block)
    {
        warning(0, "Top block does not exist");
        return TCL_ERROR;
    }
    std::vector<dbNet *> inets;
    const char *netNames = in_args->net();
    const char *tag = in_args->tag();
    block->findSomeNet(netNames, inets);
    dbNet *net;
    uint j;
    for (j=0;j<inets.size();j++)
    {
        net = inets[j];
        net->printWire((char*)tag);
    }
    return TCL_OK;
}

int ZDB::print_rseg( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_print_rseg * in_args = (ZIn_ZDB_print_rseg *) in;
    dbChip   *chip = _db->getChip();
    if (!chip)
    {
        warning(0, "chip does not exist");
        return TCL_ERROR;
    }
    
    dbBlock  *block = chip->getBlock();
    if (!block)
    {
        warning(0, "Top block does not exist");
        return TCL_ERROR;
    }
    std::vector<dbNet *> inets;
    const char *netNames = in_args->net();
    const char *tag = in_args->tag();
    block->findSomeNet(netNames, inets);
    dbNet *net;
    uint j;
    for (j=0;j<inets.size();j++)
    {
        net = inets[j];
        net->printRSeg((char*)tag);
    }
    return TCL_OK;
}

int ZDB::print_capn( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_print_capn * in_args = (ZIn_ZDB_print_capn *) in;
    dbChip   *chip = _db->getChip();
    if (!chip)
    {
        warning(0, "chip does not exist");
        return TCL_ERROR;
    }
    
    dbBlock  *block = chip->getBlock();
    if (!block)
    {
        warning(0, "Top block does not exist");
        return TCL_ERROR;
    }
    std::vector<dbNet *> inets;
    const char *netNames = in_args->net();
    const char *tag = in_args->tag();
    block->findSomeNet(netNames, inets);
    dbNet *net;
    uint j;
    for (j=0;j<inets.size();j++)
    {
        net = inets[j];
        net->printCapN((char*)tag);
    }
    return TCL_OK;
}

int ZDB::print_wnp( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_print_wnp * in_args = (ZIn_ZDB_print_wnp *) in;
    dbChip   *chip = _db->getChip();
    if (!chip)
    {
        warning(0, "chip does not exist");
        return TCL_ERROR;
    }
    
    dbBlock  *block = chip->getBlock();
    if (!block)
    {
        warning(0, "Top block does not exist");
        return TCL_ERROR;
    }
    std::vector<dbNet *> inets;
    const char *netNames = in_args->net();
    const char *tag = in_args->tag();
    block->findSomeNet(netNames, inets);
    dbNet *net;
    uint j;
    for (j=0;j<inets.size();j++)
    {
        net = inets[j];
        net->printWnP((char*)tag);
    }
    return TCL_OK;
}
int ZDB::diff_net_cap( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_diff_net_cap * in_args = (ZIn_ZDB_diff_net_cap *) in;
    dbChip   *chip = _db->getChip();
    if (!chip)
    {
        warning(0, "chip does not exist");
        return TCL_ERROR;
    }
    
    dbBlock  *block = chip->getBlock();
    if (!block)
    {
        warning(0, "Top block does not exist");
        return TCL_ERROR;
    }

	const char *ref_file= in_args->ref_file();
	if (ref_file==NULL) {
        warning(0, "Have to specify the option ref_file");
        return TCL_ERROR;
    }
	FILE *outFP= NULL;
	const char *out_file= in_args->out_file();
	if (out_file==NULL)
		out_file= "diff_cap.out";
	outFP= fopen(out_file,"w");

	FILE *logFP= NULL;
	const char *log_file= in_args->log_file();
	if (log_file==NULL)
		log_file= "diff_cap.log";
	logFP= fopen(log_file,"w");

	Ath__parser parser;
	parser.openFile((char*) ref_file);

	notice(0, "Reading ref_file %s ...\n", ref_file);

	while (parser.parseNextLine()>0) {
		//parser.printWords(stdout);
		
		char *netName= parser.get(0);
		double refCap= parser.getDouble(1);
		
		if (refCap<=0.0) {
			if (logFP!=NULL) {
				fprintf(logFP, "Non positive cap %g for net %s at line %d\n",
					refCap, netName, parser.getLineNum());
			}
			else {
				warning(0, "Non positive cap %g for net %s at line %d\n",
					refCap, netName, parser.getLineNum());
			}
			continue;
		}

		dbNet *net= block->findNet(netName);
		if (net==NULL) {
			if (logFP!=NULL) {
				fprintf(logFP, "Cannot find in DB net %s at line %d\n",
					netName, parser.getLineNum());
			}
			else {
				warning(0, "Cannot find in DB net %s at line %d\n",
					netName, parser.getLineNum());
			}
			continue;
		}
		
		double dbCap= net->getTotalCapacitance(0,true);
				
		double diffCap = 100.0*((dbCap-refCap)/refCap);
		
		if (outFP!=NULL) {
			fprintf(outFP, "%4.1f - %s %13.6f ref %13.6f corner %d %d %s\n", 
				diffCap, "netCap", dbCap, refCap, 0, net->getId(), netName);
		}
		else {
			notice(0, "%4.1f - %s %13.6f ref %13.6f corner %d %d %s\n", 
				diffCap, "netCap", dbCap, refCap, 0, net->getId(), netName);
		}		
	}
	if (logFP!=NULL)
		fclose(logFP);
	if (outFP!=NULL)
		fclose(outFP);

    return TCL_OK;
}
int ZDB::begin_eco( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_begin_eco * in_args = (ZIn_ZDB_begin_eco *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    dbDatabase::beginEco( (dbBlock *) in_args->block() );
    return TCL_OK;    
}

int ZDB::end_eco( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_end_eco * in_args = (ZIn_ZDB_end_eco *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    dbDatabase::endEco( (dbBlock *) in_args->block() );
    return TCL_OK;    
}

int ZDB::eco_empty( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_eco_empty * in_args = (ZIn_ZDB_eco_empty *) in;
    ZOut_ZDB_eco_empty * out_args = (ZOut_ZDB_eco_empty *) out;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    if ( dbDatabase::ecoEmpty( (dbBlock *) in_args->block() ) )
        out_args->result(true);
    else
        out_args->result(false);

    return TCL_OK;    
}

int ZDB::read_eco( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_read_eco * in_args = (ZIn_ZDB_read_eco *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    FILE * f = in_args->channel();
    
    if ( f == NULL )
    {
        f = fopen( in_args->file(), "rb" );

        if ( f == NULL )
        {
            warning( 0, "Failed to open file \"%s\", because %s.", in_args->file(), strerror(errno) );
            return TCL_ERROR;
        }
    }

    char * buffer = (char *) malloc( _io_bufsize );

    if ( buffer == NULL )
    {
        warning( 0, "Memory allocation failed for io buffer");
        fclose(f);
        return TCL_ERROR;
    }
    
    setvbuf( f, buffer, _IOFBF, _io_bufsize );   

    try {
        dbDatabase::readEco( (dbBlock *) in_args->block(), f );
    }
    catch( ZException & e )
    {
        warning( 0, "Failed to read database ECO, because %s.", e._msg );
        fclose(f);
        free( (void *) buffer );
        return TCL_ERROR;
    }

    free((void *) buffer);
    fclose(f);
    return TCL_OK;    
}

int ZDB::write_eco( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_write_eco * in_args = (ZIn_ZDB_write_eco *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    FILE * f = in_args->channel();
    
    if ( f == NULL )
    {
        f = fopen( in_args->file(), "wb" );

        if ( f == NULL )
        {
            warning( 0, "Failed to open file \"%s\", because %s.", in_args->file(), strerror(errno) );
            return TCL_ERROR;
        }
    }

    char * buffer = (char *) malloc( _io_bufsize );

    if ( buffer == NULL )
    {
        warning( 0, "Memory allocation failed for io buffer");
        fclose(f);
        return TCL_ERROR;
    }
    
    setvbuf( f, buffer, _IOFBF, _io_bufsize );

    try {
        dbDatabase::writeEco( (dbBlock *) in_args->block(), f );
    }
    catch( ZException & e )
    {
        warning( 0, "Failed to write database ECO, because %s.", e._msg );
        fclose(f);
        free( (void *) buffer );
        return TCL_ERROR;
    }

    fclose(f);
    free((void *) buffer);
    return TCL_OK;    
}

int ZDB::check_eco( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_check_eco * in_args = (ZIn_ZDB_check_eco *) in;
    ZOut_ZDB_check_eco * out_args = (ZOut_ZDB_check_eco *) out;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    int rc=0;
    try {
        rc = dbDatabase::checkEco( (dbBlock *) in_args->block());
    }
    catch( ZException & e )
    {
        warning( 0, "Failed to check database, because %s.", e._msg );
        out_args->result(0);
        return TCL_ERROR;
    }

    out_args->result(rc);

    return TCL_OK;    
}

int ZDB::commit_eco( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_commit_eco * in_args = (ZIn_ZDB_commit_eco *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    dbDatabase::commitEco( (dbBlock *) in_args->block() );
    return TCL_OK;    
}

int ZDB::begin_tmg_eco( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_begin_tmg_eco * in_args = (ZIn_ZDB_begin_tmg_eco *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->scenario() <= 0 )
    {
        warning(0,"Please specify a valid scenario number 1..N\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    dbDatabase::beginTmgEco( (dbBlock *) in_args->block(), in_args->scenario() );
    return TCL_OK;    
}

int ZDB::end_tmg_eco( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_end_tmg_eco * in_args = (ZIn_ZDB_end_tmg_eco *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    dbDatabase::endTmgEco( (dbBlock *) in_args->block() );
    return TCL_OK;    
}

int ZDB::eco_tmg_empty( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_eco_tmg_empty * in_args = (ZIn_ZDB_eco_tmg_empty *) in;
    ZOut_ZDB_eco_tmg_empty * out_args = (ZOut_ZDB_eco_tmg_empty *) out;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    if ( dbDatabase::ecoTmgEmpty( (dbBlock *) in_args->block() ) )
        out_args->result(true);
    else
        out_args->result(false);

    return TCL_OK;    
}
void start_timer();
void end_timer(char *msg);

int ZDB::read_tmg_eco( ZArgs * in, ZArgs * out )
{
    start_timer();
    ZIn_ZDB_read_tmg_eco * in_args = (ZIn_ZDB_read_tmg_eco *) in;

    debug("TMG_ECO","A","RTE: %d\n",in_args->scenario());
    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->scenario() <= 0 )
    {
        warning(0,"Please specify a valid scenarion 1..N\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    FILE * f = in_args->channel();
    
    if ( f == NULL )
    {
        f = fopen( in_args->file(), "rb" );

        if ( f == NULL )
        {
            warning( 0, "Failed to open file \"%s\", because %s.", in_args->file(), strerror(errno) );
            return TCL_ERROR;
        }
    }

    char * buffer = (char *) malloc( _io_bufsize );

    if ( buffer == NULL )
    {
        warning( 0, "Memory allocation failed for io buffer");
        fclose(f);
        return TCL_ERROR;
    }
    
    setvbuf( f, buffer, _IOFBF, _io_bufsize );   

    try {
        debug("TMG_ECO","A","RTE2: %d\n",in_args->scenario());
        dbDatabase::mergeTmgEco( (dbBlock *) in_args->block(), f, in_args->scenario() );
    }
    catch( ZException & e )
    {
        warning( 0, "Failed to read database ECO, because %s.", e._msg );
        fclose(f);
        free( (void *) buffer );
        return TCL_ERROR;
    }

    free((void *) buffer);
    fclose(f);
    end_timer("Read TMG ECO");
    return TCL_OK;    
}

int ZDB::write_tmg_eco( ZArgs * in, ZArgs * out )
{
    start_timer();
    ZIn_ZDB_write_tmg_eco * in_args = (ZIn_ZDB_write_tmg_eco *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    FILE * f = in_args->channel();
    
    if ( f == NULL )
    {
        f = fopen( in_args->file(), "wb" );

        if ( f == NULL )
        {
            warning( 0, "Failed to open file \"%s\", because %s.", in_args->file(), strerror(errno) );
            return TCL_ERROR;
        }
    }

    char * buffer = (char *) malloc( _io_bufsize );

    if ( buffer == NULL )
    {
        warning( 0, "Memory allocation failed for io buffer");
        fclose(f);
        return TCL_ERROR;
    }
    
    setvbuf( f, buffer, _IOFBF, _io_bufsize );

    try {
        dbDatabase::writeTmgEco( (dbBlock *) in_args->block(), f );
    }
    catch( ZException & e )
    {
        warning( 0, "Failed to write database ECO, because %s.", e._msg );
        fclose(f);
        free( (void *) buffer );
        return TCL_ERROR;
    }

    fclose(f);
    free((void *) buffer);
    end_timer("Write TMG ECO");
    return TCL_OK;    
}

int ZDB::check_tmg_eco( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_check_tmg_eco * in_args = (ZIn_ZDB_check_tmg_eco *) in;
    ZOut_ZDB_check_tmg_eco * out_args = (ZOut_ZDB_check_tmg_eco *) out;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    int rc=0;
    try {
        rc = dbDatabase::checkTmgEco( (dbBlock *) in_args->block());
    }
    catch( ZException & e )
    {
        warning( 0, "Failed to check database, because %s.", e._msg );
        out_args->result(0);
        return TCL_ERROR;
    }

    out_args->result(rc);

    return TCL_OK;    
}

int ZDB::commit_tmg_eco( ZArgs * in, ZArgs * out )
{
    start_timer();
    ZIn_ZDB_commit_tmg_eco * in_args = (ZIn_ZDB_commit_tmg_eco *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block");
        return TCL_ERROR;
    }

    dbDatabase::commitTmgEco( (dbBlock *) in_args->block() );
    end_timer("TMG ECO commit");
    return TCL_OK;    
}

int ZDB::create_net( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_create_net * in_args = (ZIn_ZDB_create_net *) in;
    ZOut_ZDB_create_net * out_args = (ZOut_ZDB_create_net *) out;

    if ( in_args->name() == NULL )
    {
        warning(0,"Please specify a name\n");
        return TCL_ERROR;
    }
    
    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block\n");
        return TCL_ERROR;
    }

    dbBlock * block = (dbBlock *) in_args->block();

    std::string type(in_args->type());
    dbSigType sig_type;

    if (  type == "signal" )
    {
        sig_type = dbSigType::SIGNAL;
    }
    else if (  type == "power" )
    {
        sig_type = dbSigType::POWER;
    }
    else if (  type == "ground" )
    {
        sig_type = dbSigType::GROUND;
    }
    else if (  type == "analog" )
    {
        sig_type = dbSigType::ANALOG;
    }
    else if (  type == "reset" )
    {
        sig_type = dbSigType::RESET;
    }
    else if (  type == "scan" )
    {
        sig_type = dbSigType::SCAN;
    }
    else if (  type == "tieoff" )
    {
        sig_type = dbSigType::TIEOFF;
    }
    else
    {
        warning(0,"invalid type argument (%s)\n", type.c_str());
        return TCL_ERROR;
    }

    dbNet * net = dbNet::create( block, in_args->name() );
    net->setSigType(sig_type);

    out_args->net(net);
    return TCL_OK;
}

int ZDB::delete_net( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_delete_net * in_args = (ZIn_ZDB_delete_net *) in;

   if ( in_args->name() == NULL )
    {
        warning(0,"Please specify a name\n");
        return TCL_ERROR;
    }
    
    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block\n");
        return TCL_ERROR;
    }

    dbBlock * block = (dbBlock *) in_args->block();
    dbNet * net = block->findNet( in_args->name() );

    if ( net == NULL )
    {
        warning(0,"net %s does not exist\n", in_args->name() );
        return TCL_ERROR;
    }

    dbNet::destroy(net);
    return TCL_OK;
}

int ZDB::create_inst( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_create_inst * in_args = (ZIn_ZDB_create_inst *) in;
    ZOut_ZDB_create_inst * out_args = (ZOut_ZDB_create_inst *) out;

    if ( in_args->name() == NULL )
    {
        warning(0,"Please specify a name\n");
        return TCL_ERROR;
    }
    
    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block\n");
        return TCL_ERROR;
    }

    dbBlock * block = (dbBlock *) in_args->block();
    dbLib * lib = _db->findLib( in_args->lib() );

    if ( lib == NULL )
    {
        warning(0, "library (%s) not found\n", in_args->lib());
        return TCL_ERROR;
    }

    if ( in_args->master() == NULL )
    {
        warning(0, "please specify a master, -master <name>\n");
        return TCL_ERROR;
    }

    dbMaster * master = lib->findMaster( in_args->master() );

    if ( master == NULL )
    {
        warning(0, "master (%s) not found\n", in_args->master());
        return TCL_ERROR;
    }

    std::string status( in_args->status() );
    dbPlacementStatus pstat;

    if ( status == "none" )
    {
        pstat = dbPlacementStatus::NONE;
    }
    else if ( status == "unplaced" )
    {
        pstat = dbPlacementStatus::UNPLACED;
    }
    else if ( status == "suggested" )
    {
        pstat = dbPlacementStatus::SUGGESTED;
    }
    else if ( status == "placed" )
    {
        pstat = dbPlacementStatus::PLACED;
    }
    else if ( status == "locked" )
    {
        pstat = dbPlacementStatus::LOCKED;
    }
    else if ( status == "firm" )
    {
        pstat = dbPlacementStatus::FIRM;
    }
    else if ( status == "cover" )
    {
        pstat = dbPlacementStatus::COVER;
    }
    else
    {
        warning(0, "invalid placement status (%s)\n", status.c_str() );
        return TCL_ERROR;
    }

    std::string orient( in_args->orient() );
    dbOrientType otype;

    if ( orient == "R0" )
    {
        otype = dbOrientType::R0;
    }
    else if ( orient == "R90" )
    {
        otype = dbOrientType::R90;
    }
    else if ( orient == "R180" )
    {
        otype = dbOrientType::R180;
    }
    else if ( orient == "R270" )
    {
        otype = dbOrientType::R270;
    }
    else if ( orient == "MY" )
    {
        otype = dbOrientType::MY;
    }
    else if ( orient == "MYR90" )
    {
        otype = dbOrientType::MYR90;
    }
    else if ( orient == "MX" )
    {
        otype = dbOrientType::MX;
    }
    else if ( orient == "MXR90" )
    {
        otype = dbOrientType::MXR90;
    }
    else
    {
        warning(0, "invalid orientation (%s)\n", orient.c_str() );
        return TCL_ERROR;
    }
    
    dbInst * inst = dbInst::create( block, master, in_args->name() );
    inst->setOrigin( in_args->x(), in_args->y() );
    inst->setOrient( otype );
    inst->setPlacementStatus(pstat);
    out_args->inst(inst);
    return TCL_OK;
}

int ZDB::delete_inst( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_delete_inst * in_args = (ZIn_ZDB_delete_inst *) in;
    if ( in_args->name() == NULL )
    {
        warning(0,"Please specify a name\n");
        return TCL_ERROR;
    }
    
    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block\n");
        return TCL_ERROR;
    }

    dbBlock * block = (dbBlock *) in_args->block();
    dbInst * inst = block->findInst( in_args->name() );

    if ( inst == NULL )
    {
        warning(0,"instance %s does not exist\n", in_args->name() );
        return TCL_ERROR;
    }

    dbInst::destroy(inst);
    return TCL_OK;
}

int ZDB::connect_iterm( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_connect_iterm * in_args = (ZIn_ZDB_connect_iterm *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block\n");
        return TCL_ERROR;
    }

    dbBlock * block = (dbBlock *) in_args->block();

    dbNet * net = block->findNet( in_args->net() );

    if ( net == NULL )
    {
        warning(0,"net %s does not exist\n", in_args->net() );
        return TCL_ERROR;
    }

    dbInst * inst = block->findInst( in_args->inst() );

    if ( inst == NULL )
    {
        warning(0,"instance %s does not exist\n", in_args->inst() );
        return TCL_ERROR;
    }

    dbITerm * iterm = inst->findITerm( in_args->term() );

    if ( iterm == NULL )
    {
        warning(0,"terminal %s does not exist\n", in_args->term() );
        return TCL_ERROR;
    }

    dbITerm::connect(iterm, net);
    return TCL_OK;
}

int ZDB::disconnect_iterm( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_disconnect_iterm * in_args = (ZIn_ZDB_disconnect_iterm *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block\n");
        return TCL_ERROR;
    }

    if ( in_args->inst() == NULL )
    {
        warning(0,"Please specify a instance\n");
        return TCL_ERROR;
    }

    if ( in_args->term() == NULL )
    {
        warning(0,"Please specify a terminal\n");
        return TCL_ERROR;
    }

    dbBlock * block = (dbBlock *) in_args->block();
    dbInst * inst = block->findInst( in_args->inst() );

    if ( inst == NULL )
    {
        warning(0,"instance %s does not exist\n", in_args->inst() );
        return TCL_ERROR;
    }

    dbITerm * iterm = inst->findITerm( in_args->term() );

    if ( iterm == NULL )
    {
        warning(0,"terminal %s does not exist\n", in_args->term() );
        return TCL_ERROR;
    }

    dbITerm::disconnect(iterm);
    return TCL_OK;
}

int ZDB::swap_master( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_swap_master * in_args = (ZIn_ZDB_swap_master *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->block()->getObjectType() != dbBlockObj )
    {
        warning(0,"-block argument is not a block\n");
        return TCL_ERROR;
    }

    if ( in_args->inst() == NULL )
    {
        warning(0,"Please specify a instance\n");
        return TCL_ERROR;
    }

    dbLib * lib = _db->findLib( in_args->lib() );

    if ( lib == NULL )
    {
        warning(0, "library (%s) not found\n", in_args->lib());
        return TCL_ERROR;
    }

    if ( in_args->master() == NULL )
    {
        warning(0, "please specify a master\n");
        return TCL_ERROR;
    }

    dbMaster * master = lib->findMaster( in_args->master() );

    if ( master == NULL )
    {
        warning(0, "master (%s) not found\n", in_args->master());
        return TCL_ERROR;
    }

    dbBlock * block = (dbBlock *) in_args->block();

    dbInst * inst = block->findInst( in_args->inst() );

    if ( inst == NULL )
    {
        warning(0,"instance %s does not exist\n", in_args->inst() );
        return TCL_ERROR;
    }
    
    if ( ! inst->swapMaster(master) )
    {
        warning(0, "swap master failed\n");
        return TCL_ERROR;
    }
    
    return TCL_OK;
}

int ZDB::test_eco( ZArgs * in, ZArgs * out )
{
    extern void test_eco();
    test_eco();
    return TCL_OK;
}

int ZDB::begin_iterm_missing_scenarios( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_begin_iterm_missing_scenarios * in_args = (ZIn_ZDB_begin_iterm_missing_scenarios *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    _dbBlock * block = (_dbBlock *) in_args->block();

    if ( block->_tmg_journal_pending == NULL )
    {
        warning(0,"Specified block has no timing journal\n");
        return TCL_ERROR;
    }
    
    block->_tmg_journal_pending->beginITermMissingScenarios();
    return TCL_OK;
}

int ZDB::begin_scenario_missing_iterms( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_begin_scenario_missing_iterms * in_args = (ZIn_ZDB_begin_scenario_missing_iterms *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    _dbBlock * block = (_dbBlock *) in_args->block();

    if ( block->_tmg_journal_pending == NULL )
    {
        warning(0,"Specified block has no timing journal\n");
        return TCL_ERROR;
    }
    
    block->_tmg_journal_pending->beginScenarioMissingITerms();
    return TCL_OK;
}

int ZDB::get_scenario_missing_iterms( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_get_scenario_missing_iterms * in_args = (ZIn_ZDB_get_scenario_missing_iterms *) in;
    ZOut_ZDB_get_scenario_missing_iterms * out_args = (ZOut_ZDB_get_scenario_missing_iterms *) out;
    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    _dbBlock * block = (_dbBlock *) in_args->block();

    if ( block->_tmg_journal_pending == NULL )
    {
        warning(0,"Specified block has no timing journal\n");
        return TCL_ERROR;
    }

    int scen = in_args->scenario();
    std::list<int> *lp;
    if ( block->_tmg_journal_pending->getScenITermList(scen,&lp) == 0 )
    {
        out_args->itermlist(*lp);
        return TCL_OK;
    } else {
        return TCL_ERROR;
    }

}

int ZDB::get_iterm_timing_data( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_get_iterm_timing_data * in_args = (ZIn_ZDB_get_iterm_timing_data *) in;
    ZOut_ZDB_get_iterm_timing_data * out_args = (ZOut_ZDB_get_iterm_timing_data *) out;
    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    _dbBlock * block = (_dbBlock *) in_args->block();

    if ( block->_tmg_journal_pending == NULL )
    {
        warning(0,"Specified block has no timing journal\n");
        return TCL_ERROR;
    }

    std::list<int> itl;
    in_args->itermlist(itl);

    // payam: changed int to double
    std::list<double> lp;

    block->_tmg_journal_pending->getITermData(itl,lp);

    out_args->tlist(lp);
    return TCL_OK;
}

int ZDB::set_scenario_missing_iterms( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_set_scenario_missing_iterms * in_args = (ZIn_ZDB_set_scenario_missing_iterms *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    _dbBlock * block = (_dbBlock *) in_args->block();

    if ( block->_tmg_journal_pending == NULL )
    {
        warning(0,"Specified block has no timing journal\n");
        return TCL_ERROR;
    }

    int scen = in_args->scenario();

    // payam: changed int to double
    std::list<double> tl;
    in_args->tlist(tl);

    if ( block->_tmg_journal_pending->setITermData(scen, tl) == 0 )
    {
        return TCL_OK;
    } else {
        return TCL_ERROR;
    }

}

int ZDB::next_iterm_missing_scenarios( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_next_iterm_missing_scenarios * in_args = (ZIn_ZDB_next_iterm_missing_scenarios *) in;
    ZOut_ZDB_next_iterm_missing_scenarios * out_args = (ZOut_ZDB_next_iterm_missing_scenarios *) out;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    _dbBlock * block = (_dbBlock *) in_args->block();

    if ( block->_tmg_journal_pending == NULL )
    {
        warning(0,"Specified block has no timing journal\n");
        return TCL_ERROR;
    }

    dbTmgJournal::MissingScenarios * s = block->_tmg_journal_pending->nextITermMissingScenarios();

    if ( s == NULL )
        return TCL_ERROR;

    out_args->iterm((int) s->getTermId());

    std::list<int> scenarios;
    s->getScenarios(scenarios);
    out_args->scenarios(scenarios);
    return TCL_OK;
}

int ZDB::begin_bterm_missing_scenarios( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_begin_bterm_missing_scenarios * in_args = (ZIn_ZDB_begin_bterm_missing_scenarios *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    _dbBlock * block = (_dbBlock *) in_args->block();

    if ( block->_tmg_journal_pending == NULL )
    {
        warning(0,"Specified block has no timing journal\n");
        return TCL_ERROR;
    }
    
    block->_tmg_journal_pending->beginBTermMissingScenarios();
    return TCL_OK;
}

int ZDB::next_bterm_missing_scenarios( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_next_bterm_missing_scenarios * in_args = (ZIn_ZDB_next_bterm_missing_scenarios *) in;
    ZOut_ZDB_next_bterm_missing_scenarios * out_args = (ZOut_ZDB_next_bterm_missing_scenarios *) out;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    _dbBlock * block = (_dbBlock *) in_args->block();

    if ( block->_tmg_journal_pending == NULL )
    {
        warning(0,"Specified block has no timing journal\n");
        return TCL_ERROR;
    }

    dbTmgJournal::MissingScenarios * s = block->_tmg_journal_pending->nextBTermMissingScenarios();

    if ( s == NULL )
        return TCL_ERROR;

    out_args->bterm((int) s->getTermId());
    std::list<int> scenarios;
    s->getScenarios(scenarios);
    out_args->scenarios(scenarios);
    return TCL_OK;
}

int ZDB::update_iterm_missing_scenario( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_update_iterm_missing_scenario * in_args = (ZIn_ZDB_update_iterm_missing_scenario *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->scenario() == -1 )
    {
        warning(0,"Please specify a valid scenario number\n");
        return TCL_ERROR;
    }

    if ( in_args->iterm() == 0 )
    {
        warning(0,"Please specify a valid iterm id\n");
        return TCL_ERROR;
    }

    _dbBlock * block = (_dbBlock *) in_args->block();

    if ( block->_tmg_journal_pending == NULL )
    {
        warning(0,"Specified block has no timing journal\n");
        return TCL_ERROR;
    }

    std::list<int> V;
    in_args->values(V);

    if ( V.size() != 8 )
    {
        warning(0,"values argument must contain 8 integers");
        return TCL_ERROR;
    }
    
    std::list<int>::iterator itr;
    // payam: changed int to float
    std::vector<float> values;
    values.reserve(8);

    for( itr = V.begin(); itr != V.end(); ++itr )
        values.push_back(*itr);
    
    block->_tmg_journal_pending->mergeITermScenario( in_args->scenario(), (uint) in_args->iterm(), values );
    return TCL_OK;
}

int ZDB::update_bterm_missing_scenario( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_update_bterm_missing_scenario * in_args = (ZIn_ZDB_update_bterm_missing_scenario *) in;

    if ( in_args->block() == NULL )
    {
        warning(0,"Please specify a block\n");
        return TCL_ERROR;
    }

    if ( in_args->scenario() == -1 )
    {
        warning(0,"Please specify a valid scenario number\n");
        return TCL_ERROR;
    }

    if ( in_args->bterm() == 0 )
    {
        warning(0,"Please specify a valid bterm id\n");
        return TCL_ERROR;
    }

    _dbBlock * block = (_dbBlock *) in_args->block();

    if ( block->_tmg_journal_pending == NULL )
    {
        warning(0,"Specified block has no timing journal\n");
        return TCL_ERROR;
    }

    std::list<int> V;
    in_args->values(V);

    if ( V.size() != 8 )
    {
        warning(0,"values argument must contain 8 integers");
        return TCL_ERROR;
    }
    
    std::list<int>::iterator itr;
    // payam: changed int to float
    std::vector<float> values;
    values.reserve(8);

    for( itr = V.begin(); itr != V.end(); ++itr )
        values.push_back(*itr);
    
    block->_tmg_journal_pending->mergeBTermScenario( in_args->scenario(), (uint) in_args->bterm(), values );
    return TCL_OK;
}

int ZDB::set_number_of_scenarios( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_set_number_of_scenarios * in_args = (ZIn_ZDB_set_number_of_scenarios *) in;

    dbBlock * block = (dbBlock *) in_args->block();
    if ( block == NULL )
    {
        warning(0,"Please specify a block, default block is top\n");
        dbChip * chip = _db->getChip();

        if ( chip == NULL )
        {
            warning(0, "chip does not exist");
            return TCL_ERROR;
        }

        block = chip->getBlock();

        if ( block == NULL )
        {
            warning(0, "top-block does not exist");
            return TCL_ERROR;
        }
    }

    if ( in_args->count() <= 0 )
    {
        warning(0,"Please specify a valid number of scenarios\n");
        return TCL_ERROR;
    }

    //dbBlock * block = (dbBlock *) in_args->block();
    block->setNumberOfScenarios( in_args->count() );
    return TCL_OK;
}

int ZDB::testlist( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_testlist * in_args = (ZIn_ZDB_testlist *) in;
    ZOut_ZDB_testlist * out_args = (ZOut_ZDB_testlist *) out;

    if ( in_args->scenario() == -1 )
    {
        warning(0,"Please specify a valid scenario number\n");
        return TCL_ERROR;
    }

    printf("-scenario %d\n", in_args->scenario() );
    

    if ( in_args->bterm() == 0 )
    {
        warning(0,"Please specify a valid bterm id\n");
        return TCL_ERROR;
    }

    printf("-bterm %d\n", in_args->bterm() );

    std::list<int> V;
    in_args->values(V);
    std::list<int>::iterator itr;

    printf("values = {");
    
    for( itr = V.begin(); itr != V.end(); ++itr )
    {
        printf(" %d", *itr);
    }

    printf(" }\n");

    out_args->results(V);
    return TCL_OK;
}

struct CMP
{
    bool operator()( dbBTerm * a, dbBTerm * b) const
    {
        dbString s1 = a->getNet()->getName();
        dbString s2 = b->getNet()->getName();

        return strcmp( (const char *) s1, (const char *) s2 ) < 0;
    }
};

void test_bind( dbInst * inst, dbBlock * block )
{
    dbSet<dbITerm> iterms = inst->getITerms();
    dbSet<dbITerm>::iterator iitr;

    for( iitr = iterms.begin(); iitr != iterms.end(); ++iitr )
    {
        dbITerm * iterm = *iitr;
        dbMTerm * mterm = iterm->getMTerm();
        dbNet * net = iterm->getNet();
        dbBTerm * bterm = iterm->getBTerm();

        printf("DOWN %s %s\n", (const char *) net->getName(), (const char *) mterm->getName() );
        assert( mterm->getName() == bterm->getName() );
    }

    
    dbSet<dbBTerm> bterms = block->getBTerms();
    dbSet<dbBTerm>::iterator bitr;  


    for( bitr = bterms.begin(); bitr != bterms.end(); ++bitr )
    {
        dbBTerm * bterm = *bitr;
        dbITerm * iterm = bterm->getITerm();
        dbMTerm * mterm = iterm->getMTerm();
        dbNet * net = iterm->getNet();
        
        printf("UP %s %s\n", (const char *) net->getName(), (const char *) mterm->getName() );
        assert( mterm->getName() == bterm->getName() );
    }
}

int ZDB::report_regions( ZArgs * in, ZArgs * out )
{
    dbBlock *blk = NULL;
    dbChip *chip = _db->getChip();
    if (chip) blk = chip->getBlock();
    if (blk) 
      db_report_regions(blk);
    return TCL_OK;
}

int ZDB::set_rebuffer_altered_nets( ZArgs * in, ZArgs * out )
{
    dbBlock *blk = NULL;
    dbChip *chip = _db->getChip();
    if (chip) blk = chip->getBlock();
    if (blk) 
      blk->setBufferAltered(true);
    return TCL_OK;
}

int ZDB::no_rebuffer_altered_nets( ZArgs * in, ZArgs * out )
{
    dbBlock *blk = NULL;
    dbChip *chip = _db->getChip();
    if (chip) blk = chip->getBlock();
    if (blk) 
      blk->setBufferAltered(false);
    return TCL_OK;
}

int ZDB::print_control( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_print_control * in_args = (ZIn_ZDB_print_control *) in;
    const char *name = in_args->name();
    const char *nets = in_args->nets();
    const char *insts = in_args->insts();
    dbBlock *blk = NULL;
    dbChip *chip = _db->getChip();
    if (chip) blk = chip->getBlock();
    if (blk) 
      blk->setPrintControl(name, nets, insts);
    return TCL_OK;
}

int ZDB::flatten( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_flatten * in_args = (ZIn_ZDB_flatten *) in;

    dbBlock * block = (dbBlock *) in_args->block();

    if ( block == NULL )
    {
        dbChip * chip = _db->getChip();

        if ( chip == NULL )
            return TCL_OK;
        
        block = chip->getBlock();

        if ( block == NULL )
            return TCL_OK;
    }
    
    dbFlatten F;

    if ( in_args->create_boundary_regions() )
        F.setCreateBoundaryRegions(true);
    
    if ( in_args->create_bterm_map() )
        F.setCreateBTermMap(true);

    if ( in_args->copy_parasitics() )
        F.setCopyParasitics(true);

    if ( F.flatten( block, in_args->levels() ) )
        return TCL_OK;
         
    return TCL_ERROR;
}
int ZDB::timing_info( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_timing_info * in_args = (ZIn_ZDB_timing_info *) in;

    dbChip * chip = _db->getChip(); 
    dbBlock *block= chip->getBlock();
    if (block==NULL) {
        error(0, "There is no dbBlock in the design\n"); 
        return TCL_ERROR;
    }

    if (in_args->init()) {
        block->setTmgUpdateIndex(0);
        return TCL_OK;
    }
    if (in_args->reset()) {
        block->setTmgUpdateIndex(0);
        return TCL_OK;
    }
    bool wns= ! in_args->tns();
    bool max= ! in_args->min();
    const char *file= in_args->file();
    if (file!=NULL) {
        FILE *fp= fopen(file, "w");
        if (fp==NULL) {
            warning(0, "Cannot open file %s\nWill skill command\n", file);
            return TCL_OK;
        }
        uint cnt= block->printSlackTrace(max, wns, fp);
        notice(0, "Have generated %d timing slacks\n", cnt);
    }
    return TCL_OK;
}

int ZDB::create_property( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_create_property * in_args = (ZIn_ZDB_create_property *) in;
    //ZOut_ZDB_create_property * out_args = (ZOut_ZDB_create_property *) out;

	dbObject *obj= in_args->object();

	if (obj==NULL)
		obj= _db;

    if ( in_args->string_value() ) {
		dbStringProperty::create( obj, in_args->name(), in_args->string_value() );
	}
	else if ( in_args->float_value() ) {
		dbDoubleProperty::create( obj, in_args->name(), in_args->float_value() );
	}
    else if ( in_args->int_value() ) {
		dbIntProperty::create( obj, in_args->name(), in_args->int_value() );
	}
    else if ( in_args->bool_value() ) {
		dbBoolProperty::create( obj, in_args->name(), in_args->bool_value() );
	}
	return TCL_OK;
}
int ZDB::find_property( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_find_property * in_args = (ZIn_ZDB_find_property *) in;
    ZOut_ZDB_find_property * out_args = (ZOut_ZDB_find_property *) out;

	dbObject *obj= in_args->object();

	if (obj==NULL)
		obj= _db;
	
	dbProperty *p= dbProperty::find( obj, in_args->name());

	if (p==NULL) {
		warning(0, "property not found");
		return TCL_ERROR;
	}
    
    switch( p->getType() )
    {
	    case dbProperty::STRING_PROP:
        {
            dbStringProperty * sp = (dbStringProperty *) p;
		    out_args->value(sp->getValue().c_str());
            break;
        }
        
	    case dbProperty::DOUBLE_PROP:
        {
            dbDoubleProperty * dp = (dbDoubleProperty *) p;
		    char buf[128];
		    sprintf(buf, "%g", dp->getValue());
		    out_args->value(buf);	
            break;
        }
        
	    case dbProperty::INT_PROP:
        {
            dbIntProperty * ip = (dbIntProperty *) p;
		    char buf[64];
		    sprintf(buf, "%d", ip->getValue());
		    out_args->value(buf);	
            break;
        }
        
	    case dbProperty::BOOL_PROP:
        {
            dbBoolProperty * bp = (dbBoolProperty *) p;
		    char buf[64];
		    sprintf(buf, "%d", bp->getValue());
		    out_args->value(buf);	
            break;
        }
    }
    
	return TCL_OK;
}
void convert_file(char *inf, long in_mode, char *outf, long out_mode)
{
#ifndef _WIN32
    char buff[1024];
    
    AFILE *fin = ATH__fopen((char*)inf,"r",in_mode);
    AFILE *fout = ATH__fopen((char*)outf,"w",out_mode);
    int len;
    while( (len = ATH__fread(buff,1,1023,fin)) > 0 )
    {
        ATH__fwrite(buff,len,1,fout);
    }
    ATH__fclose(fin);
    ATH__fflush(fout);
    ATH__fclose(fout);
#endif

}
int ZDB::encrypt_file( ZArgs * in, ZArgs * out )
{
 #ifndef _WIN32
   ZIn_ZDB_encrypt_file * in_args = (ZIn_ZDB_encrypt_file *) in;
    if ( (in_args->filename() == NULL) || (in_args->outfile() == NULL) )
    {
        warning(0,"Must specify both input and output filename for file encryption!\n");
        return TCL_ERROR;
    }
    char *inf = (char *)in_args->filename();
    char *outf = (char *)in_args->outfile();
    convert_file(inf,AF_UNCOMP,outf,AF_ENCRYPT);
#endif
    return TCL_OK;
}

int ZDB::decrypt_file( ZArgs * in, ZArgs * out )
{
 #ifndef _WIN32
    ZIn_ZDB_decrypt_file * in_args = (ZIn_ZDB_decrypt_file *) in;
    if ( (in_args->filename() == NULL) || (in_args->outfile() == NULL) )
    {
        warning(0,"Must specify both input and output filename for file decryption!\n");
        return TCL_ERROR;
    }
    char *inf = (char *)in_args->filename();
    char *outf = (char *)in_args->outfile();
    convert_file(inf,AF_ENCRYPT,outf,AF_UNCOMP);
#endif
    return TCL_OK;
}
    
int ZDB::eco_update_wires( ZArgs * in, ZArgs * out )
{
    ZIn_ZDB_eco_update_wires * in_args = (ZIn_ZDB_eco_update_wires *) in;
    dbBlock * block = (dbBlock *) in_args->block();

    if ( block == NULL )
    {
        dbChip * chip = _db->getChip();

        if ( chip == NULL )
            return TCL_OK;
        
        block = chip->getBlock();

        if ( block == NULL )
            return TCL_OK;
    }

    dbSet<dbNet> nets = block->getNets();
    dbSet<dbNet>::iterator itr;

    for( itr = nets.begin(); itr != nets.end(); ++itr )
    {
        dbNet * net = *itr;

        if ( net->getWire() )
            net->getWire()->ecoUpdate();
    }

    return TCL_OK;
}
} // namespace

