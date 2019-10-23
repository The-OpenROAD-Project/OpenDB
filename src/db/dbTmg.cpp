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

#include "dbTmg.h"
#include "dbITerm.h"
#include "dbBTerm.h"
#include "dbBlock.h"
#include "dbSet.h"
#include "dbDiff.h"
#include "dbStream.h"
#include "dbArrayTable.h"
#include "dbArrayTable.hpp"
#include "dbDiff.hpp"
#include "db.h"

namespace odb {

template class dbArrayTable<_dbTmg>;

float _dbTmg::min( float a,  float b, bool &secondNumber)
{
    float m= a;
    secondNumber= false;

    if (a>b) {
        m= b;
        secondNumber= true;
    }
    return m;
}
float _dbTmg::max( float a,  float b, bool &secondNumber)
{
    float m= a;
    secondNumber= false;

    if (a<b) {
        m= b;
        secondNumber= true;
    }
    return m;
}
void _dbTmg::merge( _dbTmg & dst, _dbTmg & src, int slew_op, int slack_op)
{
//    bool srcMinMax;

    switch( slew_op )
    {
        case dbDatabase::TMG_OVERWRITE:
            dst._slew_rise = src._slew_rise;
            dst._slew_fall = src._slew_fall;
//#ifdef ADS_DB_TMG_SCENARIO
//            dst._scenario._slew_rise = src._scenario._slew_rise;
//            dst._scenario._slew_fall = src._scenario._slew_fall;
//#endif
            break;
            
        case dbDatabase::TMG_MERGE_MIN:
//#ifdef ADS_DB_TMG_SCENARIO
//            dst._slew_rise = _dbTmg::min(dst._slew_rise, src._slew_rise, srcMinMax);
//            if (srcMinMax)
//                dst._scenario._slew_rise = src._scenario._slew_rise;
//
//            dst._slew_fall = _dbTmg::min(dst._slew_fall, src._slew_fall, srcMinMax);
//            if (srcMinMax)
//                dst._scenario._slew_fall = src._scenario._slew_fall;
//#else
            dst._slew_rise = std::min(dst._slew_rise, src._slew_rise);
            dst._slew_fall = std::min(dst._slew_fall, src._slew_fall);
//#endif
            break;
            
        case dbDatabase::TMG_MERGE_MAX:
//#ifdef ADS_DB_TMG_SCENARIO
//            dst._slew_rise = _dbTmg::max(dst._slew_rise, src._slew_rise, srcMinMax);
//            if (srcMinMax)
//                dst._scenario._slew_rise = src._scenario._slew_rise;
//
//            dst._slew_fall = _dbTmg::max(dst._slew_fall, src._slew_fall, srcMinMax);
//            if (srcMinMax)
//                dst._scenario._slew_fall = src._scenario._slew_fall;
//#else
            dst._slew_rise = std::max(dst._slew_rise, src._slew_rise);
            dst._slew_fall = std::max(dst._slew_fall, src._slew_fall);
//#endif
            break;

        default:
            assert(0);
    }

    debug("DB_ECO","M","slack merge rise: %g, remote: %g, fall: %g, remote: %g, op: ",dst._slack_rise, src._slack_rise, dst._slack_fall, src._slack_fall);
    switch( slack_op )
    {

        case dbDatabase::TMG_OVERWRITE:
            debug("DB_ECO","N"," overwrite ");
            dst._slack_rise = src._slack_rise;
            dst._slack_fall = src._slack_fall;
//#ifdef ADS_DB_TMG_SCENARIO
//            dst._scenario._slack_rise = src._scenario._slack_rise;
//            dst._scenario._slack_fall = src._scenario._slack_fall;
//#endif
            break;
            
        case dbDatabase::TMG_MERGE_MIN:
            debug("DB_ECO","N"," min ");
//#ifdef ADS_DB_TMG_SCENARIO
//            dst._slack_rise = _dbTmg::min(dst._slack_rise, src._slack_rise, srcMinMax);
//            if (srcMinMax)
//                dst._scenario._slack_rise = src._scenario._slack_rise;
//
//            dst._slack_fall = _dbTmg::min(dst._slack_fall, src._slack_fall, srcMinMax);
//            if (srcMinMax)
//                dst._scenario._slack_fall = src._scenario._slack_fall;
//#else
            dst._slack_rise = std::min(dst._slack_rise, src._slack_rise);
            dst._slack_fall = std::min(dst._slack_fall, src._slack_fall);
//#endif
            break;
            
        case dbDatabase::TMG_MERGE_MAX:
            debug("DB_ECO","N"," max ");
//#ifdef ADS_DB_TMG_SCENARIO
//            dst._slack_rise = _dbTmg::max(dst._slack_rise, src._slack_rise, srcMinMax);
//            if (srcMinMax)
//                dst._scenario._slack_rise = src._scenario._slack_rise;
//
//            dst._slack_fall = _dbTmg::max(dst._slack_fall, src._slack_fall, srcMinMax);
//            if (srcMinMax)
//                dst._scenario._slack_fall = src._scenario._slack_fall;
//#else
            dst._slack_rise = std::max(dst._slack_rise, src._slack_rise);
            dst._slack_fall = std::max(dst._slack_fall, src._slack_fall);
//#endif
            break;

        default:
            assert(0);
    }
    debug("DB_ECO","M","final slack rise: %g, final fall: %g\n",dst._slack_rise, dst._slack_fall);
}
//bool _dbTmg::updateSlacks(float wns[2], float tns[2], uint scenario[2], uint mode)
bool _dbTmg::updateSlacks(float wns[2], float tns[2], uint mode)
{
    bool fallMinMax;
    float slack= _dbTmg::min(_slack_rise, _slack_fall, fallMinMax);
    
    if (slack>=0.0)
        return false;

    uint ii= mode; 

    //scenario[ii]= _scenario._slack_rise; 
    //if (fallMinMax) 
    //    scenario[ii]= _scenario._slack_fall;

    
    if (wns[ii]>slack) { 
        wns[ii]= slack; 
    //    tns[ii] += slack; 
        return true;
    }
    return false;
}

void _dbTmg::differences( dbDiff & diff, const char * field, const _dbTmg & rhs ) const
{
    DIFF_FIELD(_slew_rise);
    DIFF_FIELD(_slew_fall);
    DIFF_FIELD(_slack_rise);
    DIFF_FIELD(_slack_fall);
//    DIFF_FIELD(_scenario._slew_rise);
//    DIFF_FIELD(_scenario._slew_fall);
//    DIFF_FIELD(_scenario._slack_rise);
//    DIFF_FIELD(_scenario._slack_fall);
}

void _dbTmg::out( dbDiff & diff, char side, const char * field ) const
{
    DIFF_OUT_FIELD(_slew_rise);
    DIFF_OUT_FIELD(_slew_fall);
    DIFF_OUT_FIELD(_slack_rise);
    DIFF_OUT_FIELD(_slack_fall);
//    DIFF_OUT_FIELD(_scenario._slew_rise);
//    DIFF_OUT_FIELD(_scenario._slew_fall);
//    DIFF_OUT_FIELD(_scenario._slack_rise);
//    DIFF_OUT_FIELD(_scenario._slack_fall);
}

} // namespace
