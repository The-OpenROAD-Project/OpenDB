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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "dbMetrics.h"

#include "db.h"
#include "dbBlock.h"
#include "dbDiff.h"
#include "dbDiff.hpp"
#include "dbSet.h"
#include "dbStream.h"
#include "dbTable.h"
#include "dbTable.hpp"

namespace odb {

template class dbTable<_dbMetrics>;

void _dbMetrics::differences(dbDiff&           diff,
                             const char*       field,
                             const _dbMetrics& rhs) const
{
  DIFF_BEGIN
  DIFF_FIELD(_worst_slack);
  DIFF_FIELD(_total_slack);
  DIFF_END
}

void _dbMetrics::out(dbDiff& diff, char side, const char* field) const
{
  DIFF_OUT_BEGIN
  DIFF_OUT_FIELD(_worst_slack);
  DIFF_OUT_FIELD(_total_slack);
  DIFF_END
}

int dbMetrics::getWorstSlack()
{
  _dbMetrics* m = (_dbMetrics*) this;
  return m->_worst_slack;
}

void dbMetrics::setWorstSlack(int slack)
{
  _dbMetrics* m   = (_dbMetrics*) this;
  m->_worst_slack = slack;
}

int dbMetrics::getTotalSlack()
{
  _dbMetrics* m = (_dbMetrics*) this;
  return m->_total_slack;
}

void dbMetrics::setTotalSlack(int slack)
{
  _dbMetrics* m   = (_dbMetrics*) this;
  m->_total_slack = slack;
}

dbMetrics* dbMetrics::getMetrics(dbBlock* block_, uint dbid_)
{
  _dbBlock* block = (_dbBlock*) block_;
  return (dbMetrics*) block->_metrics_tbl->getPtr(dbid_);
}

}  // namespace odb
