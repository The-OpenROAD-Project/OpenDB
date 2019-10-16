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

#include "dbITerm.h"
#include "dbDatabase.h"
#include "dbHier.h"
#include "dbBTerm.h"
#include "dbChip.h"
#include "dbInst.h"
#include "dbTmg.h"
#include "dbInstHdr.h"
#include "dbNet.h"
#include "dbLib.h"
#include "dbMaster.h"
#include "dbMTerm.h"
#include "dbBlock.h"
#include "dbBlockCallBackObj.h"
#include "dbTable.h"
#include "dbArrayTable.h"
#include "dbTable.hpp"
#include "dbShape.h"
#include "dbJournal.h"
#include "dbTmgJournal.h"
#include "db.h"
#include "dbDiff.hpp"
#include "logger.h"

#ifdef _WIN32
#define MINFLOAT -1.0e+20
#define MAXFLOAT 1.0e+20
#endif

namespace odb {
#ifdef FULL_ECO
#define FLAGS(iterm) (*((uint *) &iterm->_flags))
#endif

template class dbTable<_dbITerm>;

bool _dbITerm::operator==( const _dbITerm & rhs ) const
{
    if( _flags._mterm_idx != rhs._flags._mterm_idx )
        return false;
    
    if( _flags._spef != rhs._flags._spef )
        return false;
    
    if( _flags._special != rhs._flags._special )
        return false;
    
    if( _flags._connected != rhs._flags._connected )
        return false;

    if( _ext_id != rhs._ext_id )
        return false;
    
    if( _net != rhs._net )
        return false;
    
    if( _inst != rhs._inst )
        return false;

    if( _tmg != rhs._tmg )
        return false;
    
    if( _next_net_iterm != rhs._next_net_iterm )
        return false;
    
    if( _prev_net_iterm != rhs._prev_net_iterm )
        return false;
    
    return true;
}

bool _dbITerm::operator<( const _dbITerm & rhs ) const
{
    _dbBlock * lhs_blk = (_dbBlock *) getOwner();
    _dbBlock * rhs_blk = (_dbBlock *) rhs.getOwner();

    _dbInst * lhs_inst = lhs_blk->_inst_tbl->getPtr(_inst);
    _dbInst * rhs_inst = rhs_blk->_inst_tbl->getPtr(rhs._inst);
    int r = strcmp( lhs_inst->_name, rhs_inst->_name );

    if ( r < 0 )
        return true;

    if ( r > 0 )
        return false;

    _dbMTerm * lhs_mterm = getMTerm();
    _dbMTerm * rhs_mterm = rhs.getMTerm();
    return strcmp( lhs_mterm->_name, rhs_mterm->_name ) < 0;
}

void _dbITerm::differences( dbDiff & diff, const char * field, const _dbITerm & rhs ) const
{
    if ( ! diff.deepDiff() )
    {
        DIFF_BEGIN
        DIFF_FIELD( _net);
        DIFF_FIELD( _inst);
        DIFF_FIELD( _tmg);
        DIFF_FIELD( _flags._mterm_idx);
        DIFF_FIELD( _flags._spef);
        DIFF_FIELD( _flags._special);
        DIFF_FIELD( _flags._connected);
        DIFF_FIELD( _ext_id);
        DIFF_FIELD( _next_net_iterm);
        DIFF_FIELD( _prev_net_iterm);
        DIFF_END
    }
    else
    {
        _dbBlock * lhs_blk = (_dbBlock *) getOwner();
        _dbBlock * rhs_blk = (_dbBlock *) rhs.getOwner();
        _dbInst * lhs_inst = lhs_blk->_inst_tbl->getPtr(_inst);
        _dbInst * rhs_inst = rhs_blk->_inst_tbl->getPtr(rhs._inst);
        _dbMTerm * lhs_mterm = getMTerm();
        _dbMTerm * rhs_mterm = rhs.getMTerm();
        assert( strcmp( lhs_inst->_name, rhs_inst->_name ) == 0 );
        assert( strcmp( lhs_mterm->_name, rhs_mterm->_name ) == 0 );

        diff.begin_object("<> %s (_dbITerm)\n", lhs_mterm->_name );
        
        if ( (_net != 0) && (rhs._net != 0) )
        {
            _dbNet * lhs_net = lhs_blk->_net_tbl->getPtr(_net);
            _dbNet * rhs_net = rhs_blk->_net_tbl->getPtr(rhs._net);
            diff.diff( "_net", lhs_net->_name, rhs_net->_name );
        }
        else if ( _net != 0 )
        {
            _dbNet * lhs_net = lhs_blk->_net_tbl->getPtr(_net);
            diff.out( dbDiff::LEFT, "_net", lhs_net->_name );
        }
        else if ( rhs._net != 0 )
        {
            _dbNet * rhs_net = rhs_blk->_net_tbl->getPtr(rhs._net);
            diff.out( dbDiff::RIGHT, "_net", rhs_net->_name );
        }

        DIFF_OBJECT( _tmg, lhs_blk->_tmg_tbl, rhs_blk->_tmg_tbl );
        DIFF_FIELD( _flags._spef);
        DIFF_FIELD( _flags._special);
        DIFF_FIELD( _flags._connected);
        DIFF_FIELD( _ext_id);
        diff.end_object();
    }
}

void _dbITerm::out( dbDiff & diff, char side, const char * field  ) const
{
    if ( ! diff.deepDiff() )
    {
        DIFF_OUT_BEGIN
        DIFF_OUT_FIELD( _net);
        DIFF_OUT_FIELD( _inst);
        DIFF_OUT_FIELD( _tmg);
        DIFF_OUT_FIELD( _flags._mterm_idx);
        DIFF_OUT_FIELD( _flags._spef);
        DIFF_OUT_FIELD( _flags._special);
        DIFF_OUT_FIELD( _flags._connected);
        DIFF_OUT_FIELD( _ext_id);
        DIFF_OUT_FIELD( _next_net_iterm);
        DIFF_OUT_FIELD( _prev_net_iterm);
        DIFF_END
    }
    else
    {
        _dbMTerm * mterm = getMTerm();
        diff.begin_object("%c %s (_dbITerm)\n", side, mterm->_name );
        _dbBlock * blk = (_dbBlock *) getOwner();

        if ( _net != 0 )
        {
            _dbNet * net = blk->_net_tbl->getPtr(_net);
            diff.out( side, "_net", net->_name );
        }

        DIFF_OUT_OBJECT( _tmg, blk->_tmg_tbl );
        DIFF_OUT_FIELD( _flags._spef);
        DIFF_OUT_FIELD( _flags._special);
        DIFF_OUT_FIELD( _flags._connected);
        DIFF_OUT_FIELD( _ext_id);
        diff.end_object();
    }
}

_dbMTerm *
_dbITerm::getMTerm() const
{
    _dbBlock * block = (_dbBlock *) getOwner();
    _dbInst * inst = block->_inst_tbl->getPtr(_inst);
    _dbInstHdr * inst_hdr = block->_inst_hdr_tbl->getPtr(inst->_inst_hdr);
    _dbDatabase * db = getDatabase();
    _dbLib * lib = db->_lib_tbl->getPtr(inst_hdr->_lib);
    _dbMaster * master = lib->_master_tbl->getPtr(inst_hdr->_master);
    dbId<_dbMTerm> mterm = inst_hdr->_mterms[_flags._mterm_idx];
    return master->_mterm_tbl->getPtr(mterm);
}

_dbInst *
_dbITerm::getInst() const
{
    _dbBlock * block = (_dbBlock *) getOwner();
    _dbInst * inst = block->_inst_tbl->getPtr(_inst);
    return inst;
}

////////////////////////////////////////////////////////////////////
//
// dbITerm - Methods
//
////////////////////////////////////////////////////////////////////

dbInst *
dbITerm::getInst()
{
    _dbITerm * iterm = (_dbITerm *) this;
    _dbBlock * block = (_dbBlock *) getOwner();
    _dbInst * inst = block->_inst_tbl->getPtr(iterm->_inst);
    return (dbInst *) inst;
}

dbNet *
dbITerm::getNet()
{
    _dbITerm * iterm = (_dbITerm *) this;
    _dbBlock * block = (_dbBlock *) getOwner();

    if ( iterm->_net == 0 )
        return NULL;
    
    _dbNet * net = block->_net_tbl->getPtr(iterm->_net);
    return (dbNet *) net;
}

dbMTerm *
dbITerm::getMTerm()
{
    _dbITerm * iterm = (_dbITerm *) this;
    _dbBlock * block = (_dbBlock *) getOwner();
    _dbInst * inst = block->_inst_tbl->getPtr(iterm->_inst);
    _dbInstHdr * inst_hdr = block->_inst_hdr_tbl->getPtr(inst->_inst_hdr);
    _dbDatabase * db = getDatabase();
    _dbLib * lib = db->_lib_tbl->getPtr(inst_hdr->_lib);
    _dbMaster * master = lib->_master_tbl->getPtr(inst_hdr->_master);
    dbId<_dbMTerm> mterm = inst_hdr->_mterms[iterm->_flags._mterm_idx];
    return (dbMTerm *) master->_mterm_tbl->getPtr(mterm);
}

dbBTerm *
dbITerm::getBTerm()
{
    _dbITerm * iterm = (_dbITerm *) this;
    _dbBlock * block = (_dbBlock *) getOwner();
    _dbInst * inst = block->_inst_tbl->getPtr(iterm->_inst);

    if ( inst->_hierarchy == 0 )
        return NULL;
        
    _dbHier * hier = block->_hier_tbl->getPtr(inst->_hierarchy);

    _dbChip * chip = (_dbChip *) block->getOwner();
    _dbBlock * child = chip->_block_tbl->getPtr( hier->_child_block );
    dbId<_dbBTerm> bterm = hier->_child_bterms[iterm->_flags._mterm_idx];
    return (dbBTerm *) child->_bterm_tbl->getPtr(bterm);
}

void dbITerm::initTiming()
{
    _dbITerm * iterm = (_dbITerm *) this;
    _dbBlock * block = (_dbBlock *) getOwner();
    ((dbBlock*)block)->initTmbTbl();  //  ex: "Tmg tmg" after "db read"

    if ( iterm->_tmg == 0 )
        iterm->_tmg = block->_tmg_tbl->createArray();

    uint scns = (uint)block->_number_of_scenarios;
    for (uint ks = 0; ks <= scns; ks++) {
        _dbTmg * tmg_min = block->_tmg_tbl->getPtr(iterm->_tmg + TMG_MIN + ks*2);
        _dbTmg * tmg_max = block->_tmg_tbl->getPtr(iterm->_tmg + TMG_MAX + ks*2);

        // payam: changes MAX/MIN_INT to MAX/MINFLOAT
        tmg_min->_slew_rise = FLT_MAX;
        tmg_min->_slew_fall = FLT_MAX;
        tmg_min->_slack_rise = FLT_MAX;
        tmg_min->_slack_fall = FLT_MAX;
        ///tmg_min->_scenario._slew_rise = 0;
        ///tmg_min->_scenario._slew_fall = 0;
        ///tmg_min->_scenario._slack_rise = 0;
        ///tmg_min->_scenario._slack_fall = 0;

        tmg_max->_slew_rise = FLT_MIN;
        tmg_max->_slew_fall = FLT_MIN;
        tmg_max->_slack_rise = FLT_MAX;
        tmg_max->_slack_fall = FLT_MAX;
        ///tmg_max->_scenario._slew_rise = 0;
        ///tmg_max->_scenario._slew_fall = 0;
        ///tmg_max->_scenario._slack_rise = 0;
        ///tmg_max->_scenario._slack_fall = 0;
    }
}
bool dbITerm::getWorstSlack(float wns[2], float tns[2], uint scenario[2])
{ 
    _dbITerm * iterm = (_dbITerm *) this; 
    _dbBlock * block = (_dbBlock *) getOwner(); 
   
    if ( iterm->_tmg == 0 ) 
        return false; 
    
    //_dbTmg * tmg_min = block->_tmg_tbl->getPtr(iterm->_tmg); 
    //_dbTmg * tmg_max = block->_tmg_tbl->getPtr(iterm->_tmg+1U); 
    
    //bool worstTerm= tmg_min->updateSlacks(wns, tns, scenario, TMG_MIN); 
    //worstTerm= tmg_max->updateSlacks(wns, tns, scenario, TMG_MAX) || worstTerm; 

    uint scns = (uint)block->_number_of_scenarios;
    _dbTmg * tmg_min;
    _dbTmg * tmg_max;
    bool worstTermMin = false;
    bool worstTermMax = false;
    for (uint ks = 1; ks <= scns; ks++) {
        tmg_min = block->_tmg_tbl->getPtr(iterm->_tmg + TMG_MIN + ks*2);
        tmg_max = block->_tmg_tbl->getPtr(iterm->_tmg + TMG_MAX + ks*2);
        if (tmg_min->updateSlacks(wns, tns, TMG_MIN)) {
            scenario[TMG_MIN] = ks;
            worstTermMin = true;
        }
        if (tmg_max->updateSlacks(wns, tns, TMG_MAX)) {
            scenario[TMG_MAX] = ks;
            worstTermMax = true;
        }
    }
    if (worstTermMin)
        tns[TMG_MIN] += wns[TMG_MIN];
    if (worstTermMax)
        tns[TMG_MAX] += wns[TMG_MAX];
    
    return (worstTermMin || worstTermMax);
}
// payam: return type from int to float
float dbITerm::getSlewRise( dbTimingMode mode )
{ 
    uint scenario = 0; 
    return getSlewRise( mode, scenario );
}
float dbITerm::getSlewRise( dbTimingMode mode, uint scenario )
{
    _dbITerm * iterm = (_dbITerm *) this;

    if ( iterm->_tmg == 0 )
        initTiming();
    
    _dbBlock * block = (_dbBlock *) getOwner();
    float slew = block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + scenario*2)->_slew_rise;
    if(isDebug("TMG_DB", "L")) {
	const char* iname= getInst()->getConstName();
	const char* mname= getMTerm()->getConstName();
	debug("TMG_DB","L","#%d SlewRise[%d]-[S%d]: %g %s/%s\n", 
		getId(), mode, scenario, slew, iname, mname);
    }
    return slew;
}

// payam: return type from int to float
float dbITerm::getSlewFall( dbTimingMode mode )
{
    uint scenario = 0; 
    return getSlewFall( mode, scenario );
}
float dbITerm::getSlewFall( dbTimingMode mode, uint scenario )
{
    _dbITerm * iterm = (_dbITerm *) this;

    if ( iterm->_tmg == 0 )
        initTiming();

    _dbBlock * block = (_dbBlock *) getOwner();
    float slew = block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + scenario*2)->_slew_fall;
    if(isDebug("TMG_DB", "L")) {
	const char* iname= getInst()->getConstName();
	const char* mname= getMTerm()->getConstName();
	debug("TMG_DB","L","#%d SlewFall[%d]-[S%d]: %g %s/%s\n", 
		getId(), mode, scenario, slew, iname, mname);
    }
    return slew;
}

// payam: return type from int to float
float dbITerm::getSlackRise( dbTimingMode mode )
{
    uint scenario = 0; 
    return getSlackRise( mode, scenario );
}
float dbITerm::getSlackRise( dbTimingMode mode, uint scenario )
{
    _dbITerm * iterm = (_dbITerm *) this;

    if ( iterm->_tmg == 0 )
        initTiming();

    _dbBlock * block = (_dbBlock *) getOwner();
    float slack = block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + scenario*2)->_slack_rise;

    if(isDebug("TMG_DB", "I")) {
	const char* iname= getInst()->getConstName();
	const char* mname= getMTerm()->getConstName();
	debug("TMG_DB","I","#%d SlackRise[%d]-[S%d]: %g %s/%s\n", 
		getId(), mode, scenario, slack, iname, mname);

    	_dbBlock * block = (_dbBlock *) getOwner();
    	uint scns = (uint)block->_number_of_scenarios;
	for (uint ii= 0; ii<=scns; ii++) {
    		float v = block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + ii*2)->_slack_rise;
		debug("TMG_DB","I","\t%d %g\n", ii, v);
	}

    }
    return slack;
}

// payam: return type from int to float
float dbITerm::getSlackFall( dbTimingMode mode )
{
    uint scenario = 0; 
    return getSlackFall( mode, scenario );
}
float dbITerm::getSlackFall( dbTimingMode mode, uint scenario )
{
    _dbITerm * iterm = (_dbITerm *) this;

    if ( iterm->_tmg == 0 )
        initTiming();

    _dbBlock * block = (_dbBlock *) getOwner();
    float slack = block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + scenario*2)->_slack_fall;
    if(isDebug("TMG_DB", "I")) {
	const char* iname= getInst()->getConstName();
	const char* mname= getMTerm()->getConstName();
	debug("TMG_DB","I","#%d SlackFall[%d]-[S%d]: %g %s/%s\n", 
		getId(), mode, scenario, slack, iname, mname);

    	_dbBlock * block = (_dbBlock *) getOwner();
    	uint scns = (uint)block->_number_of_scenarios;
	for (uint ii= 0; ii<=scns; ii++) {
    		float v = block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + ii*2)->_slack_fall;
		debug("TMG_DB","I","\t%d %g\n", ii, v);
	}
    }
    return slack;
}

// payam: type from int to float
void dbITerm::setSlewRise( dbTimingMode mode, float value, uint scenario )
{
    _dbITerm * iterm = (_dbITerm *) this;

    if ( iterm->_tmg == 0 )
        initTiming();

    _dbBlock * block = (_dbBlock *) getOwner();
    block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + scenario*2)->_slew_rise = value;
    if (scenario == 0)
        return;
    float wslew = value;
    float nslew;
    // dimitri_fix uint ws = scenario;
    for (uint ks = 1; ks <= (uint)block->_number_of_scenarios && ks != scenario; ks++) {
        nslew = block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + ks*2)->_slew_rise;
        if ((mode == TMG_MIN && nslew < wslew) || (mode == TMG_MAX && nslew > wslew)) {
            wslew = nslew;
            // dimitri_fix ws = ks;
        }
    }
    block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode)->_slew_rise = wslew;
}

// payam: type from int to float
void dbITerm::setSlewFall( dbTimingMode mode, float value, uint scenario )
{
    _dbITerm * iterm = (_dbITerm *) this;

    if ( iterm->_tmg == 0 )
        initTiming();

    _dbBlock * block = (_dbBlock *) getOwner();
    block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + scenario*2)->_slew_fall = value;
    if (scenario == 0)
        return;
    float wslew = value;
    float nslew;
    // dimitri_fix uint ws = scenario;
    for (uint ks = 1; ks <= (uint)block->_number_of_scenarios && ks != scenario; ks++) {
        nslew = block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + ks*2)->_slew_fall;
        if ((mode == TMG_MIN && nslew < wslew) || (mode == TMG_MAX && nslew > wslew)) {
            wslew = nslew;
            // dimitri_fix ws = ks;
        }
    }
    block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode)->_slew_fall = wslew;
}
void dbITerm::setSlackRise( dbTimingMode mode, float value, uint scenario )
{
    _dbITerm * iterm = (_dbITerm *) this;

    if ( iterm->_tmg == 0 )
        initTiming();

    _dbBlock * block = (_dbBlock *) getOwner();
    block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + scenario*2)->_slack_rise = value;
    if (scenario == 0)
        return;
    float wslack = value;
    float nslack;
    // dimitri_fix uint ws = scenario;
    for (uint ks = 1; ks <= (uint)block->_number_of_scenarios && ks != scenario; ks++) {
        nslack = block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + ks*2)->_slack_rise;
        if (nslack < wslack) {
            wslack = nslack;
            // dimitri_fix ws = ks;
        }
    }
    block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode)->_slack_rise = wslack;

    uint ii= (uint) mode;
    if ((block->_flags._active_pins>0) && (value<0.0) ) { 
        block->_WNS[ii]= MIN(block->_WNS[ii], value); 
        block->_TNS[ii] += value;
    }
}

// payam: type from int to float
void dbITerm::setSlackFall( dbTimingMode mode, float value, uint scenario )
{
    _dbITerm * iterm = (_dbITerm *) this;

    if ( iterm->_tmg == 0 )
        initTiming();

    _dbBlock * block = (_dbBlock *) getOwner();
    block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + scenario*2)->_slack_fall = value;
    if (scenario == 0)
        return;
    float wslack = value;
    float nslack;
// dimitri_fix     uint ws = scenario;
    for (uint ks = 1; ks <= (uint)block->_number_of_scenarios && ks != scenario; ks++) {
        nslack = block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode + ks*2)->_slack_fall;
        if (nslack < wslack) {
            wslack = nslack;
            // dimitri_fix ws = ks;
        }
    }
    block->_tmg_tbl->getPtr(iterm->_tmg + (uint) mode)->_slack_fall = wslack;

    uint ii= (uint) mode;
    if ((block->_flags._active_pins>0) && (value<0.0) ) { 
        block->_WNS[ii]= MIN(block->_WNS[ii], value); 
        block->_TNS[ii] += value;
    }
}

//void dbITerm::ecoTiming()
//{
//    _dbITerm * iterm = (_dbITerm *) this;
//    _dbBlock * block = (_dbBlock *) getOwner();
//
//    if ( iterm->_tmg == 0 )
//        initTiming();
//
//    if ( block->_tmg_journal )
//    {
//        _dbTmg * min_tmg = block->_tmg_tbl->getPtr(iterm->_tmg + (uint) TMG_MIN);
//        _dbTmg * max_tmg = block->_tmg_tbl->getPtr(iterm->_tmg + (uint) TMG_MAX);
//        block->_tmg_journal->beginAction( dbTmgJournal::UPDATE_ITERM_TMG );
//        block->_tmg_journal->pushParam( iterm->getId() );
//        block->_tmg_journal->pushParam( min_tmg->_slew_rise );
//        block->_tmg_journal->pushParam( max_tmg->_slew_rise );
//        block->_tmg_journal->pushParam( min_tmg->_slew_fall );
//        block->_tmg_journal->pushParam( max_tmg->_slew_fall );
//        block->_tmg_journal->pushParam( min_tmg->_slack_rise );
//        block->_tmg_journal->pushParam( max_tmg->_slack_rise );
//        block->_tmg_journal->pushParam( min_tmg->_slack_fall );
//        block->_tmg_journal->pushParam( max_tmg->_slack_fall );
//        block->_tmg_journal->endAction();
//    }
//}

dbBlock *
dbITerm::getBlock()
{
    return (dbBlock *) getOwner();
}
void dbITerm::setClocked(bool v)
{
    _dbITerm * iterm = (_dbITerm *) this;
    iterm->_flags._clocked = v;
}
bool dbITerm::isClocked()
{
	bool masterFlag= getMTerm()->getSigType()==dbSigType::CLOCK ? true : false;
    _dbITerm * iterm = (_dbITerm *) this;
    return iterm->_flags._clocked>0 || masterFlag ? true : false;
}
void dbITerm::setMark(uint v)
{
    _dbITerm * iterm = (_dbITerm *) this;
    iterm->_flags._mark = v;
}
bool dbITerm::isSetMark()
{
    _dbITerm * iterm = (_dbITerm *) this;
    return iterm->_flags._mark>0 ? true : false;
}

bool dbITerm::isSpecial()
{
    _dbITerm * iterm = (_dbITerm *) this;
    return iterm->_flags._special == 1;
}

void dbITerm::setSpecial()
{
    _dbITerm * iterm = (_dbITerm *) this;
    //_dbBlock * block = (_dbBlock *) getOwner();
    // dimitri_fix: need to FIX on FULL_ECO uint prev_flags = FLAGS(iterm);
	#ifdef FULL_ECO
		uint prev_flags = FLAGS(iterm);
	#endif

    iterm->_flags._special = 1;

#ifdef FULL_ECO
    if ( block->_journal )
    {
        debug("DB_ECO","A","ECO: Iterm %d, setSpecial\n",getId());
        block->_journal->updateField( this, _dbNet::FLAGS, prev_flags, FLAGS(iterm) );
    }
#endif
}

void dbITerm::clearSpecial()
{
    _dbITerm * iterm = (_dbITerm *) this;
    //_dbBlock * block = (_dbBlock *) getOwner();
    // dimitri_fix: need to FIX on FULL_ECO //uint prev_flags = FLAGS(iterm);
	#ifdef FULL_ECO
		uint prev_flags = FLAGS(iterm);
	#endif

    iterm->_flags._special = 0;

#ifdef FULL_ECO
    if ( block->_journal )
    {
        debug("DB_ECO","A","ECO: Iterm %d, clearSpecial\n",getId());
        block->_journal->updateField( this, _dbNet::FLAGS, prev_flags, FLAGS(iterm) );
    }
#endif
}

void dbITerm::setSpef(uint v)
{
    _dbITerm * iterm = (_dbITerm *) this;
    //_dbBlock * block = (_dbBlock *) getOwner();
    // dimitri_fix: need to FIX on FULL_ECO //uint prev_flags = FLAGS(iterm);
	#ifdef FULL_ECO
		uint prev_flags = FLAGS(iterm);
	#endif

    iterm->_flags._spef= v;

#ifdef FULL_ECO
    if ( block->_journal )
    {
        debug("DB_ECO","A","ECO: Iterm %d, setSpef\n",getId());
        block->_journal->updateField( this, _dbNet::FLAGS, prev_flags, FLAGS(iterm) );
    }
#endif
}

// added TmgTmpD, which says whether a term is an new pin or not - payam
void dbITerm::setTmgTmpD(bool v)
{
    _dbITerm * iterm = (_dbITerm *) this;
    iterm->_flags._tmgTmpD = v;
}

bool dbITerm::isSetTmgTmpD()
{
    _dbITerm * iterm = (_dbITerm *) this;
    return (iterm->_flags._tmgTmpD==1) ? true : false;
}

// added TmgTmpC, which says whether a term is an endpoint or not - payam
void dbITerm::setTmgTmpC(bool v)
{
    _dbITerm * iterm = (_dbITerm *) this;
    iterm->_flags._tmgTmpC = v;
}

bool dbITerm::isSetTmgTmpC()
{
    _dbITerm * iterm = (_dbITerm *) this;
    return (iterm->_flags._tmgTmpC==1) ? true : false;
}

void dbITerm::setTmgTmpB(bool v)
{
    _dbITerm * iterm = (_dbITerm *) this;
    iterm->_flags._tmgTmpB = v;
}

bool dbITerm::isSetTmgTmpB()
{
    _dbITerm * iterm = (_dbITerm *) this;
    return (iterm->_flags._tmgTmpB==1) ? true : false;
}

void dbITerm::setTmgTmpA(bool v)
{
    _dbITerm * iterm = (_dbITerm *) this;
    iterm->_flags._tmgTmpA = v;
}

bool dbITerm::isSetTmgTmpA()
{
    _dbITerm * iterm = (_dbITerm *) this;
    return (iterm->_flags._tmgTmpA==1) ? true : false;
}

bool dbITerm::isSpef()
{
    _dbITerm * iterm = (_dbITerm *) this;
    return (iterm->_flags._spef>0) ? true : false;
}

void dbITerm::setExtId(uint v)
{
    _dbITerm * iterm = (_dbITerm *) this;
    iterm->_ext_id= v;
}

uint dbITerm::getExtId()
{
    _dbITerm * iterm = (_dbITerm *) this;
    return iterm->_ext_id;
}

bool dbITerm::isConnected()
{
    _dbITerm * iterm = (_dbITerm *) this;
    return iterm->_flags._connected == 1;
}

void dbITerm::setConnected()
{
    _dbITerm * iterm = (_dbITerm *) this;
    // dimitri_fix: need to FIX on FULL_ECO uint prev_flags = FLAGS(iterm);
	#ifdef FULL_ECO
    _dbBlock * block = (_dbBlock *) getOwner();
		uint prev_flags = FLAGS(iterm);
	#endif

    iterm->_flags._connected = 1;

#ifdef FULL_ECO
    if ( block->_journal )
    {
        debug("DB_ECO","A","ECO: Iterm %d, setConnected\n",getId());
        block->_journal->updateField( this, _dbNet::FLAGS, prev_flags, FLAGS(iterm) );
    }
#endif
}

void dbITerm::clearConnected()
{
    _dbITerm * iterm = (_dbITerm *) this;
    // uint prev_flags = FLAGS(iterm);
	#ifdef FULL_ECO
    _dbBlock * block = (_dbBlock *) getOwner();
		uint prev_flags = FLAGS(iterm);
	#endif

    
    iterm->_flags._connected = 0;

#ifdef FULL_ECO
    if ( block->_journal )
    {
        debug("DB_ECO","A","ECO: Iterm %d, clearConnected\n",getId());
        block->_journal->updateField( this, _dbNet::FLAGS, prev_flags, FLAGS(iterm) );
    }
#endif
}
    
dbITerm *
dbITerm::connect( dbInst * inst_, dbNet * net_, dbMTerm * mterm_ )
{
    _dbInst * inst = (_dbInst *) inst_;
    //_dbNet * net = (_dbNet *) net_;
    _dbMTerm * mterm = (_dbMTerm *) mterm_;
    _dbBlock * block = (_dbBlock *) inst->getOwner();
    _dbITerm * iterm = block->_iterm_tbl->getPtr( inst->_iterms[mterm->_order_id] );
    connect( (dbITerm *) iterm, net_);
    return (dbITerm *) iterm;
}

void
dbITerm::connect( dbITerm * iterm_, dbNet * net_ )
{
    _dbITerm * iterm = (_dbITerm *) iterm_;
    _dbNet * net = (_dbNet *) net_;
    _dbBlock * block = (_dbBlock *) iterm->getOwner();

    if ( iterm->_net != 0 )
        disconnect(iterm_);
    
    if ( block->_journal )
    {
        debug("DB_ECO","A","ECO: connect Iterm %d to net %d\n",iterm_->getId(),net_->getId());
        block->_journal->beginAction( dbJournal::CONNECT_OBJECT );
        block->_journal->pushParam( dbITermObj );
        block->_journal->pushParam( iterm_->getId() );
        block->_journal->pushParam( net_->getId() );
        block->_journal->endAction();
    }

    iterm->_net = net->getOID();

    if ( net->_iterms != 0 )
    {
        _dbITerm * tail = block->_iterm_tbl->getPtr( net->_iterms );
        iterm->_next_net_iterm = net->_iterms;
        iterm->_prev_net_iterm = 0;
        tail->_prev_net_iterm = iterm->getOID();
    } else {
        iterm->_next_net_iterm = 0;
        iterm->_prev_net_iterm = 0;
    }

    net->_iterms = iterm->getOID();

   std::list<dbBlockCallBackObj *>::iterator  cbitr;
   for (cbitr = block->_callbacks.begin(); cbitr !=  block->_callbacks.end(); ++cbitr)
      (**cbitr)().inDbITermConnect(iterm_); // client ECO optimization - payam
}

void
dbITerm::disconnect( dbITerm * iterm_ )
{
    _dbITerm * iterm = (_dbITerm *) iterm_;

    if ( iterm->_net == 0 )
        return;

    _dbBlock * block = (_dbBlock *) iterm->getOwner();
    _dbNet * net = block->_net_tbl->getPtr(iterm->_net);

    if ( block->_journal )
    {
        debug("DB_ECO","A","ECO: disconnect Iterm %d\n",iterm_->getId());
        block->_journal->beginAction( dbJournal::DISCONNECT_OBJECT );
        block->_journal->pushParam( dbITermObj );
        block->_journal->pushParam( iterm_->getId() );
        block->_journal->endAction();
    }

    std::list<dbBlockCallBackObj *>::iterator  cbitr;
    for (cbitr = block->_callbacks.begin(); cbitr !=  block->_callbacks.end(); ++cbitr)
      (**cbitr)().inDbITermDisconnect(iterm_);
    
    uint id = iterm->getOID();

    if ( net->_iterms == id )
    {
        net->_iterms = iterm->_next_net_iterm;

        if ( net->_iterms != 0 )
        {
            _dbITerm * t = block->_iterm_tbl->getPtr( net->_iterms );
            t->_prev_net_iterm = 0;
        }
    }
    else
    {
        if ( iterm->_next_net_iterm != 0 )
        {
            _dbITerm * next = block->_iterm_tbl->getPtr( iterm->_next_net_iterm );
            next->_prev_net_iterm = iterm->_prev_net_iterm;
        }

        if ( iterm->_prev_net_iterm != 0 )
        {
            _dbITerm * prev = block->_iterm_tbl->getPtr( iterm->_prev_net_iterm );
            prev->_next_net_iterm = iterm->_next_net_iterm;
        }
    }

    iterm->_net = 0;

    if ( iterm->_tmg )
        iterm_->initTiming();
}

dbSet<dbITerm>::iterator dbITerm::disconnect( dbSet<dbITerm>::iterator & itr )
{
    dbITerm * it = *itr;
    dbSet<dbITerm>::iterator next = ++itr;
    disconnect(it);
    return next;
}

dbSigType dbITerm::getSigType()
{
    _dbMTerm * mterm = (_dbMTerm *) getMTerm();
    return dbSigType( mterm->_flags._sig_type );
}
dbIoType dbITerm::getIoType()
{ 
    _dbMTerm * mterm = (_dbMTerm *) getMTerm(); 
    return dbIoType( mterm->_flags._io_type );
}
bool dbITerm::isOutputSignal(bool io)
{
	_dbMTerm * mterm = (_dbMTerm *) getMTerm();
	dbSigType sType= dbSigType( mterm->_flags._sig_type );
	dbIoType ioType= dbIoType( mterm->_flags._io_type );

	if ((sType == dbSigType::GROUND)|| (sType == dbSigType::POWER))
		return false;

	if (ioType==dbIoType::OUTPUT)
		return true;

	if (io && (ioType==dbIoType::INOUT))
		return true;

	return false;
}
bool dbITerm::isInputSignal(bool io)
{
	_dbMTerm * mterm = (_dbMTerm *) getMTerm();
	dbSigType sType= dbSigType( mterm->_flags._sig_type );
	dbIoType ioType= dbIoType( mterm->_flags._io_type );

	if ((sType == dbSigType::GROUND)|| (sType == dbSigType::POWER))
		return false;

	if (ioType==dbIoType::INPUT)
		return true;

	if (io && (ioType==dbIoType::INOUT))
		return true;

	return false;
}

dbITerm *
dbITerm::getITerm( dbBlock * block_, uint dbid )
{ 
    _dbBlock * block = (_dbBlock *) block_; 
    return (dbITerm *) block->_iterm_tbl->getPtr(dbid);
}

bool
dbITerm::getAvgXY( int *x, int *y)
{
    dbMTerm *mterm = getMTerm(); 
    int nn = 0; 
    double xx=0.0, yy=0.0; 
    int px,py; 
    dbInst *inst = getInst(); 
    inst->getOrigin(px,py); 
    adsPoint origin = adsPoint(px,py); 
    dbOrientType orient = inst->getOrient(); 
    dbTransform transform( orient, origin );


    dbSet<dbMPin> mpins = mterm->getMPins(); 
    dbSet<dbMPin>::iterator mpin_itr; 
    for (mpin_itr = mpins.begin(); mpin_itr != mpins.end(); mpin_itr++) { 
        dbMPin *mpin = *mpin_itr; 
        dbSet<dbBox> boxes = mpin->getGeometry(); 
        dbSet<dbBox>::iterator box_itr; 
        for (box_itr = boxes.begin(); box_itr != boxes.end(); box_itr++) { 
            dbBox * box = *box_itr; adsRect rect; 
            box->getBox( rect ); 
            transform.apply( rect ); 
            xx += rect.xMin()+rect.xMax(); 
            yy += rect.yMin()+rect.yMax(); 
            nn += 2; 
        } 
    } 
    if (!nn) { 
        warning(0, "Can not find physical location of iterm %s/%s\n", 
                getInst()->getConstName(), getMTerm()->getConstName()); return false; 
    } 
    xx /= nn; 
    yy /= nn; 
    *x = int(xx); 
    *y = int(yy); 
    return true;
}
void dbITerm::print(FILE *fp, const char *trail)
{
	if (fp==NULL)
	{
		notice(0, "%d %s %s %s%s", getId(), getMTerm()->getConstName(),
			getMTerm()->getMaster()->getConstName(),getInst()->getConstName(), trail);
	}
	else {
		fprintf(fp, "%d %s %s %s%s", getId(), getMTerm()->getConstName(),
			getMTerm()->getMaster()->getConstName(),getInst()->getConstName(), trail);
	}
}
} // namespace
