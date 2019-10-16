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

#include "dbTmgJournal.h"
#include "dbArrayTable.h"
#include "dbBlock.h"
#include "dbTmg.h"
#include "dbITerm.h"
#include "dbBTerm.h"
#include "dbMetrics.h"
#include "db.h"

namespace odb {

dbTmgJournal::dbTmgJournal( dbBlock * block )
        : _block(block), _start_action(false), _number_of_scenarios(0), _scenario(0)
{
    _number_of_scenarios = ((_dbBlock *) block)->_number_of_scenarios;

    _scen_miterms = (std::list<int>**)malloc(sizeof(std::list<int>*) * _number_of_scenarios);
    for(int i=0; i<_number_of_scenarios; i++)
    {
        _scen_miterms[i] = NULL;
    }
}
    
dbTmgJournal::~dbTmgJournal()
{
    clear();
    for(int i=0; i<_number_of_scenarios; i++)
    {
        if ( _scen_miterms[i] != NULL )
            _scen_miterms[i]->clear();
        delete _scen_miterms[i];
    }
    free(_scen_miterms);
}

void dbTmgJournal::clear()
{
    _log.clear();
    _start_action = false;
}

void dbTmgJournal::pushParam( bool value )
{
    _log.push( value );
}

void dbTmgJournal::pushParam( char value )
{
    _log.push( value );
}

void dbTmgJournal::pushParam( unsigned char value )
{
    _log.push( value );
}

void dbTmgJournal::pushParam( int value )
{
    _log.push( value );
}

void dbTmgJournal::pushParam( unsigned int value )
{
    _log.push( value );
}

void dbTmgJournal::pushParam( double value )
{
    _log.push( value );
}

void dbTmgJournal::pushParam( float value )
{
    _log.push( value );
}

void dbTmgJournal::pushParam( const char * value )
{
    _log.push( value );
}

void dbTmgJournal::beginAction( Action action )
{
    assert(_start_action == false);
    _start_action = true;
    _action_idx = _log.size();
    _log.push((unsigned char) action);
}

void dbTmgJournal::endAction()
{
    _start_action = false;
    _log.push((unsigned char) END_ACTION);
    _log.push( _action_idx ); // This value allows log to be scanned backwards.
}

void dbTmgJournal::mergeScenario( int scenario )
{
    assert( scenario >= 1 && scenario <= 64 );
    uint64 current_scenario = 1U << (scenario-1);
    debug("TMG_ECO","A","Reading in data for Scenario %d\n",scenario);
    
    _log.begin();

    while( ! _log.end() )
    {
#ifndef NDEBUG
        uint s = _log.idx();
#endif
        _log.pop(_cur_action);

        switch( _cur_action )
        {
            case UPDATE_ITERM_TMG:
                updateITermTmg(current_scenario);
                break;

            case UPDATE_BTERM_TMG:
                updateBTermTmg(current_scenario);
                break;

            case UPDATE_METRICS:
            {
                _dbMetrics * m = (_dbMetrics *) _block->getScenario( scenario );
                _log.pop(m->_worst_slack);
                _log.pop(m->_total_slack);
                break;
            }

            default:
                assert(0);
                break;
        }

        unsigned char end_action;
        unsigned int action_idx;
        _log.pop(end_action);
        _log.pop(action_idx);
        assert( end_action == END_ACTION );
        assert( action_idx == s );
    }
}

void dbTmgJournal::updateITermTmg(uint64 scenario)
{
    uint iterm_id;
    _log.pop(iterm_id);
    debug("TMG_ECO","B","MTE: read data for iterm %d\n",iterm_id);

    Tmg & tmg = _iterm_tmg_map[iterm_id];
    // payam: changed int to float
    float value;
    _log.pop(value);
    tmg._slew_rise[TMG_JNL_MIN] = std::min( tmg._slew_rise[TMG_JNL_MIN], value);

    _log.pop(value);
    tmg._slew_rise[TMG_JNL_MAX] = std::max( tmg._slew_rise[TMG_JNL_MAX], value);

    _log.pop(value);
    tmg._slew_fall[TMG_JNL_MIN] = std::min( tmg._slew_fall[TMG_JNL_MIN], value);

    _log.pop(value);
    tmg._slew_fall[TMG_JNL_MAX] = std::max( tmg._slew_fall[TMG_JNL_MAX], value);

    _log.pop(value);
    tmg._slack_rise[TMG_JNL_MIN] = std::min( tmg._slack_rise[TMG_JNL_MIN], value);

    _log.pop(value);
    tmg._slack_rise[TMG_JNL_MAX] = std::min( tmg._slack_rise[TMG_JNL_MAX], value);

    _log.pop(value);
    tmg._slack_fall[TMG_JNL_MIN] = std::min( tmg._slack_fall[TMG_JNL_MIN], value);

    _log.pop(value);
    tmg._slack_fall[TMG_JNL_MAX] = std::min( tmg._slack_fall[TMG_JNL_MAX], value);

    tmg._scenarios  |= scenario;
}

void dbTmgJournal::updateBTermTmg(uint64 scenario)
{
    uint bterm_id;
    _log.pop(bterm_id);
    debug("TMG_ECO","B","MTE: read data for bterm %d\n",bterm_id);

    Tmg & tmg = _bterm_tmg_map[bterm_id];
    // payam: changed from int to float
    float value;
    _log.pop(value);
    tmg._slew_rise[TMG_JNL_MIN] = std::min( tmg._slew_rise[TMG_JNL_MIN], value);

    _log.pop(value);
    tmg._slew_rise[TMG_JNL_MAX] = std::max( tmg._slew_rise[TMG_JNL_MAX], value);

    _log.pop(value);
    tmg._slew_fall[TMG_JNL_MIN] = std::min( tmg._slew_fall[TMG_JNL_MIN], value);

    _log.pop(value);
    tmg._slew_fall[TMG_JNL_MAX] = std::max( tmg._slew_fall[TMG_JNL_MAX], value);

    _log.pop(value);
    tmg._slack_rise[TMG_JNL_MIN] = std::min( tmg._slack_rise[TMG_JNL_MIN], value);

    _log.pop(value);
    tmg._slack_rise[TMG_JNL_MAX] = std::min( tmg._slack_rise[TMG_JNL_MAX], value);

    _log.pop(value);
    tmg._slack_fall[TMG_JNL_MIN] = std::min( tmg._slack_fall[TMG_JNL_MIN], value);

    _log.pop(value);
    tmg._slack_fall[TMG_JNL_MAX] = std::min( tmg._slack_fall[TMG_JNL_MAX], value);
    tmg._scenarios |= scenario;
}

// payam: changed vector<int> to vector<float>
void dbTmgJournal::mergeITermScenario( int scenario, uint iterm_id, std::vector<float> & values )
{
    assert( values.size() == 8 );
    uint64 current_scenario = 1U << (scenario-1);
    Tmg & tmg = _iterm_tmg_map[iterm_id];

    tmg._slew_rise[TMG_JNL_MIN] = values[0];
    tmg._slew_rise[TMG_JNL_MAX] = values[1];
    tmg._slew_fall[TMG_JNL_MIN] = values[2];
    tmg._slew_fall[TMG_JNL_MAX] = values[3];
    tmg._slack_rise[TMG_JNL_MIN] = values[4];
    tmg._slack_rise[TMG_JNL_MAX] = values[5];
    tmg._slack_fall[TMG_JNL_MIN] = values[6];
    tmg._slack_fall[TMG_JNL_MAX] = values[7];
    tmg._scenarios |= current_scenario;
}

// payam: changed vector<int> to vector<float>
void dbTmgJournal::mergeBTermScenario( int scenario, uint bterm_id, std::vector<float> & values )
{
    assert( values.size() == 8 );
    uint64 current_scenario = 1U << (scenario-1);
    Tmg & tmg = _bterm_tmg_map[bterm_id];

    tmg._slew_rise[TMG_JNL_MIN] = values[0];
    tmg._slew_rise[TMG_JNL_MAX] = values[1];
    tmg._slew_fall[TMG_JNL_MIN] = values[2];
    tmg._slew_fall[TMG_JNL_MAX] = values[3];
    tmg._slack_rise[TMG_JNL_MIN] = values[4];
    tmg._slack_rise[TMG_JNL_MAX] = values[5];
    tmg._slack_fall[TMG_JNL_MIN] = values[6];
    tmg._slack_fall[TMG_JNL_MAX] = values[7];
    tmg._scenarios |= current_scenario;
}

void dbTmgJournal::getITermMissingScenarios( std::vector<MissingScenarios> & missing_scenarios )
{
    std::map<uint, Tmg>::iterator itr;
    missing_scenarios.clear();

    uint64 scenario_mask;

    if ( _number_of_scenarios == 64 )
        scenario_mask = ~0U;
    else
        scenario_mask = (1U << _number_of_scenarios) - 1U;

    for( itr = _iterm_tmg_map.begin(); itr != _iterm_tmg_map.end(); ++itr )
    {
        Tmg & tmg = (*itr).second;

        uint64 scenarios = (tmg._scenarios ^ scenario_mask);

        if ( scenarios != 0U )
            missing_scenarios.push_back( MissingScenarios( (*itr).first, scenarios ) );
    }
}

int dbTmgJournal::getScenITermList(int scen, std::list<int>** list)
{
    if( (scen <= 0) || (scen > _number_of_scenarios) )
    {
        notice(0,"Invalid Scenario Number %d\n",scen);
        *list = NULL;
        return -1;
    }

    *list =(_scen_miterms[scen-1]);
    return 0;
    
}

// payam: changed list<int> to list<double>
int dbTmgJournal::setITermData(int scen, std::list<double>& tlist)
{

    if( (scen <= 0) || (scen > _number_of_scenarios) )
    {
        notice(0,"Invalid Scenario Number %d\n",scen);
        return -1;
    }

    uint64 current_scenario = 1U << (scen-1);

    std::list<int>::iterator itr;
    std::list<int> & itl = *_scen_miterms[scen-1];

    if ( (itl.size()*8) != tlist.size() )
    {
        warning(0,"SetITermData: List sizes are incompatible (%d vs %d)!\n",itl.size(),tlist.size());
        return -1;
    }
    // payam: replaced int by double
    std::list<double>::iterator ttr = tlist.begin();

    float value; // payam: changed int to float
    for( itr = itl.begin(); itr != itl.end(); ++itr)
    {

        Tmg & tmg = _iterm_tmg_map[*itr];
        debug("TMG_ECO","M","MTE: setting data for iterm %d\n",(*itr));

        value = *ttr;
        tmg._slew_rise[TMG_JNL_MIN] = 
            std::min( tmg._slew_rise[TMG_JNL_MIN], value);
        ttr++;

        value = *ttr;
        tmg._slew_rise[TMG_JNL_MAX] = 
            std::max( tmg._slew_rise[TMG_JNL_MAX], value);
        ttr++;

        value = *ttr;
        tmg._slew_fall[TMG_JNL_MIN] = 
            std::min( tmg._slew_fall[TMG_JNL_MIN], value);
        ttr++;

        value = *ttr;
        tmg._slew_fall[TMG_JNL_MAX] = 
            std::max( tmg._slew_fall[TMG_JNL_MAX], value);
        ttr++;

        value = *ttr;
        tmg._slack_rise[TMG_JNL_MIN] = 
            std::min( tmg._slack_rise[TMG_JNL_MIN], value);
        ttr++;

        value = *ttr;
        tmg._slack_rise[TMG_JNL_MAX] = 
            std::min( tmg._slack_rise[TMG_JNL_MAX], value);
        ttr++;

        value = *ttr;
        tmg._slack_fall[TMG_JNL_MIN] = 
            std::min( tmg._slack_fall[TMG_JNL_MIN], value);
        ttr++;

        value = *ttr;
        tmg._slack_fall[TMG_JNL_MAX] = 
            std::min( tmg._slack_fall[TMG_JNL_MAX], value);
        ttr++;

        tmg._scenarios  |= current_scenario;
    }

    return 0;
}

// payam: changed list<int> to list<double>
void dbTmgJournal::getITermData(std::list<int>& itl, std::list<double>& tlist)
{
    std::list<int>::iterator itr;

    for( itr = itl.begin(); itr != itl.end(); ++itr)
    {

        debug("TMG_ECO","M","MTE: getting data for iterm %d\n",(*itr));
        Tmg & tmg = _iterm_tmg_map[*itr];

        tlist.push_back(tmg._slew_rise[TMG_JNL_MIN]);
        tlist.push_back(tmg._slew_rise[TMG_JNL_MAX]);
        tlist.push_back(tmg._slew_fall[TMG_JNL_MIN]);
        tlist.push_back(tmg._slew_fall[TMG_JNL_MAX]);
        tlist.push_back(tmg._slack_rise[TMG_JNL_MIN]);
        tlist.push_back(tmg._slack_rise[TMG_JNL_MAX]);
        tlist.push_back(tmg._slack_fall[TMG_JNL_MIN]);
        tlist.push_back(tmg._slack_fall[TMG_JNL_MAX]);
    }
}


void dbTmgJournal::getBTermMissingScenarios( std::vector<MissingScenarios> & missing_scenarios )
{
    std::map<uint, Tmg>::iterator itr;
    missing_scenarios.clear();

    uint64 scenario_mask;

    if ( _number_of_scenarios == 64 )
        scenario_mask = ~0;
    else
        scenario_mask = (1U << _number_of_scenarios) - 1U;

    for( itr = _bterm_tmg_map.begin(); itr != _bterm_tmg_map.end(); ++itr )
    {
        Tmg & tmg = (*itr).second;

        uint64 scenarios = (tmg._scenarios ^ scenario_mask);

        if ( scenarios != 0 )
            missing_scenarios.push_back( MissingScenarios( (*itr).first, scenarios ) );
    }
}

void dbTmgJournal::commit()
{
    uint64 scenario_mask;

    if ( _number_of_scenarios == 64 )
        scenario_mask = ~0;
    else
        scenario_mask = (1U << _number_of_scenarios) - 1U;

    std::map<uint, Tmg>::iterator itr;

    //int cnt=0;
    //notice(0,"CTE commit: %d\n",_iterm_tmg_map.size());
    for( itr = _iterm_tmg_map.begin(); itr != _iterm_tmg_map.end(); ++itr )
    {
        uint iterm_id = (*itr).first;
        //notice(0,"Iteration %d for iterm %d\n",++cnt, iterm_id);
        Tmg & tmg = (*itr).second;
        _dbITerm * iterm = (_dbITerm *) dbITerm::getITerm( _block, iterm_id );

        if ( iterm->_tmg == 0 )
           ((dbITerm *)iterm)->initTiming();

       _dbTmg * min_tmg = ((_dbBlock *) _block)->_tmg_tbl->getPtr(iterm->_tmg + (uint) TMG_MIN);

       _dbTmg * max_tmg = ((_dbBlock *) _block)->_tmg_tbl->getPtr(iterm->_tmg + (uint) TMG_MAX);

       //notice(0,"CM: it: %d, old: %d, %d, %d, %d   %d, %d, %d, %d\n",min_tmg->_slew_rise, min_tmg->_slew_fall, min_tmg->_slack_rise, min_tmg->_slack_fall, max_tmg->_slew_rise, max_tmg->_slew_fall, max_tmg->_slack_rise, max_tmg->_slack_fall);
        min_tmg->_slew_rise = tmg._slew_rise[TMG_JNL_MIN];
        min_tmg->_slew_fall = tmg._slew_fall[TMG_JNL_MIN];
        min_tmg->_slack_rise = tmg._slack_rise[TMG_JNL_MIN];
        min_tmg->_slack_fall = tmg._slack_fall[TMG_JNL_MIN];
        max_tmg->_slew_rise = tmg._slew_rise[TMG_JNL_MAX];
        max_tmg->_slew_fall = tmg._slew_fall[TMG_JNL_MAX];
        max_tmg->_slack_rise = tmg._slack_rise[TMG_JNL_MAX];
        max_tmg->_slack_fall = tmg._slack_fall[TMG_JNL_MAX];

       //notice(0,"CM: it: %d, new: %d, %d, %d, %d   %d, %d, %d, %d\n",min_tmg->_slew_rise, min_tmg->_slew_fall, min_tmg->_slack_rise, min_tmg->_slack_fall, max_tmg->_slew_rise, max_tmg->_slew_fall, max_tmg->_slack_rise, max_tmg->_slack_fall);
    }

    for( itr = _bterm_tmg_map.begin(); itr != _bterm_tmg_map.end(); ++itr )
    {
        uint bterm_id = (*itr).first;
        Tmg & tmg = (*itr).second;
        _dbBTerm * bterm = (_dbBTerm *) dbBTerm::getBTerm( _block, bterm_id );
       _dbTmg * min_tmg = ((_dbBlock *) _block)->_tmg_tbl->getPtr(bterm->_tmg + (uint) TMG_MIN);
       _dbTmg * max_tmg = ((_dbBlock *) _block)->_tmg_tbl->getPtr(bterm->_tmg + (uint) TMG_MAX);

        min_tmg->_slew_rise = tmg._slew_rise[TMG_JNL_MIN];
        min_tmg->_slew_fall = tmg._slew_fall[TMG_JNL_MIN];
        min_tmg->_slack_rise = tmg._slack_rise[TMG_JNL_MIN];
        min_tmg->_slack_fall = tmg._slack_fall[TMG_JNL_MIN];
        max_tmg->_slew_rise = tmg._slew_rise[TMG_JNL_MAX];
        max_tmg->_slew_fall = tmg._slew_fall[TMG_JNL_MAX];
        max_tmg->_slack_rise = tmg._slack_rise[TMG_JNL_MAX];
        max_tmg->_slack_fall = tmg._slack_fall[TMG_JNL_MAX];
    }
}

void dbTmgJournal::beginScenarioMissingITerms()
{
    for(int i=0; i<_number_of_scenarios; i++)
    {
        if ( _scen_miterms[i] == NULL )
        {
            _scen_miterms[i] = new std::list<int>;
        }
        else
        {
            _scen_miterms[i]->clear();
        }
    }

    // now, loop through each iterm
    //      put each iterm into its scenario list

    std::map<uint, Tmg>::iterator itr;

    uint64 scenario_mask;

    if ( _number_of_scenarios == 64 )
        scenario_mask = ~0U;
    else
        scenario_mask = (1U << _number_of_scenarios) - 1U;

    for( itr = _iterm_tmg_map.begin(); itr != _iterm_tmg_map.end(); ++itr )
    {
        Tmg & tmg = (*itr).second;

        uint64 scenarios = (tmg._scenarios ^ scenario_mask);

        // check for no missing scenarios
        if ( scenarios == 0U) continue;

        uint64 mask = 1U;
        for(int i=0; i<_number_of_scenarios; i++)
        {
            if ( (mask & scenarios) != 0U ) 
            {
                //notice(0,"Adding iterm %d to scenario %d\n",(*itr).first,i);
                _scen_miterms[i]->push_back( (*itr).first );
            }
            mask *= 2;
        }
    }
}

void dbTmgJournal::beginITermMissingScenarios()
{
    getITermMissingScenarios( _missing_scenarios );
    _missing_scenarios_itr = _missing_scenarios.begin();
}

dbTmgJournal::MissingScenarios * dbTmgJournal::nextITermMissingScenarios()
{
    if( _missing_scenarios_itr == _missing_scenarios.end() )
    {
        _missing_scenarios.clear();
        return NULL;
    }
    
    MissingScenarios * s = &(*_missing_scenarios_itr);
    ++_missing_scenarios_itr;
    return s;
}

void dbTmgJournal::beginBTermMissingScenarios()
{
    getBTermMissingScenarios( _missing_scenarios );
    _missing_scenarios_itr = _missing_scenarios.begin();
}

dbTmgJournal::MissingScenarios * dbTmgJournal::nextBTermMissingScenarios()
{
    if( _missing_scenarios_itr == _missing_scenarios.end() )
    {
        _missing_scenarios.clear();
        return NULL;
    }
    
    MissingScenarios * s = &(*_missing_scenarios_itr);
    ++_missing_scenarios_itr;
    return s;
}

void dbTmgJournal::beginEco(int scenario)
{
    _scenario = scenario;
    _log.clear();
}

void dbTmgJournal::endEco()
{
    _dbMetrics * m = (_dbMetrics *) _block->getScenario( _scenario );
    beginAction( dbTmgJournal::UPDATE_METRICS );
    pushParam( m->_worst_slack );
    pushParam( m->_total_slack );
    endAction();
}

dbOStream & operator<<( dbOStream & stream, const dbTmgJournal & journal )
{
    stream << journal._log;
    return stream;
}

dbIStream & operator>>( dbIStream & stream, dbTmgJournal & journal )
{
    stream >> journal._log;
    return stream;
}

} // namespace
