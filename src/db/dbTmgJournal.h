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

#ifndef ADS_DB_TMG_JNL_JOURNAL_H
#define ADS_DB_TMG_JNL_JOURNAL_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_JOURNAL_LOG_H
#include "dbJournalLog.h"
#endif

#include <map>
#include <list>

BEGIN_NAMESPACE_ADS

class dbIStream;
class dbOStream;
class dbBlock;
class dbMaster;
class dbNet;
class dbInst;
class dbITerm;

class dbTmgJournal
{
  public:
    enum Action
    {
        UPDATE_ITERM_TMG,
        UPDATE_BTERM_TMG,
        UPDATE_METRICS,
        END_ACTION
    };

    enum MinMax
    {
        TMG_JNL_MIN=0,
        TMG_JNL_MAX=1
    };

    class MissingScenarios
    {
        uint   _term_id;
        uint64 _scenarios;

      public:

        MissingScenarios()
            : _term_id(0U),
              _scenarios(0U)
        {
        }

        MissingScenarios( uint term_id, uint64 scenarios )
            : _term_id(term_id),
              _scenarios(scenarios)
        {
        }

        uint getTermId() const { return _term_id; }

        void getScenarios( std::list<int> & scenarios ) const
        {
            int cnt;
            uint64 mask;

            for( cnt = 0, mask = 1; cnt < 64; mask <<= 1, ++cnt )
                if ( _scenarios & mask )
                    scenarios.push_back(cnt+1);
        }
    };

  private:
    dbJournalLog  _log;
    dbBlock *     _block;
    bool          _start_action;
    uint          _action_idx;
    unsigned char _cur_action;
    int           _number_of_scenarios;
    int           _scenario;
    std::vector<MissingScenarios> _missing_scenarios;
    std::vector<MissingScenarios>::iterator _missing_scenarios_itr;

    std::list<int> ** _scen_miterms;

    // payam: changed the timing int to float
    struct Tmg
    {
        uint64        _scenarios;
        float         _slew_rise[2];
        float         _slew_fall[2];
        float         _slack_rise[2];
        float         _slack_fall[2];

        Tmg()
        {
            _scenarios = 0U;
            // payam: INT_MIN/MAX to FLT_MIN/MAX
            _slew_rise[TMG_JNL_MIN] = FLT_MAX;
            _slew_fall[TMG_JNL_MIN] = FLT_MAX;
            _slack_rise[TMG_JNL_MIN] = FLT_MAX;
            _slack_fall[TMG_JNL_MIN] = FLT_MAX;
            _slew_rise[TMG_JNL_MAX] = FLT_MIN;
            _slew_fall[TMG_JNL_MAX] = FLT_MIN;
            _slack_rise[TMG_JNL_MAX] = FLT_MAX;
            _slack_fall[TMG_JNL_MAX] = FLT_MAX;
        }
    };

    std::map<uint, Tmg> _iterm_tmg_map; // map from iterm-id to tmp-values
    std::map<uint, Tmg> _bterm_tmg_map; // map from bterm-id to tmp-values

    void getITermMissingScenarios( std::vector<MissingScenarios> & missing_scenarios );
    void getBTermMissingScenarios( std::vector<MissingScenarios> & missing_scenarios );
    void updateITermTmg(uint64 scenario);
    void updateBTermTmg(uint64 scenario);

  public:
    dbTmgJournal( dbBlock * block );
    ~dbTmgJournal();
    void clear();

    // begin eco recording
    void beginEco(int scenario);

    // end eco recording
    void endEco();

    int size()
    {
        return _log.size();
    }

    //
    // Methods to push entries into the transaction log.
    //
    // The entries in the log take the form:
    //
    //    <ACTION>
    //    <PARAM-1>
    //     ...
    //    <PARAM-N>
    //    <ACTION-OFFSET>
    //
    void beginAction( Action type );
    void pushParam( bool value );
    void pushParam( char value );
    void pushParam( unsigned char value );
    void pushParam( int value );
    void pushParam( unsigned int value );
    void pushParam( float value );
    void pushParam( double value );
    void pushParam( const char * value );
    void endAction();

    // Merge the current scenario stored in the log to the journal
    void mergeScenario( int scenario );

    // Merge the scenario into the log 
    // payam: changed vector<int> to vector<float>
    void mergeITermScenario( int scenario, uint iterm, std::vector<float> & values );
    void mergeBTermScenario( int scenario, uint bterm, std::vector<float> & values );

    // Commit the changes 
    void commit();

    int getScenITermList(int scenario, std::list<int>** ilist);
    // payam: changed list<int> to list<double>
    void getITermData(std::list<int>& itl, std::list<double>& tlist);
    // payam: changed list<int> to list<double>
    int setITermData(int scenario, std::list<double>& tlist);

    void beginITermMissingScenarios();
    void beginScenarioMissingITerms();
    MissingScenarios * nextITermMissingScenarios();

    void beginBTermMissingScenarios();
    MissingScenarios * nextBTermMissingScenarios();

    bool empty() { return _log.empty(); }

    friend dbIStream & operator>>( dbIStream & stream, dbTmgJournal & jrnl );
    friend dbOStream & operator<<( dbOStream & stream, const dbTmgJournal & jrnl );
    friend class dbDatabase;
};

dbIStream & operator>>( dbIStream & stream, dbTmgJournal & jrnl );
dbOStream & operator<<( dbOStream & stream, const dbTmgJournal & jrnl );

END_NAMESPACE_ADS    


#endif
