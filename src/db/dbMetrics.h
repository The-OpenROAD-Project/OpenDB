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

#pragma once

#include "dbCore.h"
#include "dbId.h"
#include "odb.h"

namespace odb {

class dbIStream;
class dbOStream;
class dbDiff;

class _dbMetrics : public _dbObject
{
 public:
  int _worst_slack;
  int _total_slack;

  _dbMetrics(_dbDatabase*);
  _dbMetrics(_dbDatabase*, const _dbMetrics& i);
  ~_dbMetrics();
  bool operator==(const _dbMetrics& rhs) const;
  bool operator!=(const _dbMetrics& rhs) const { return !operator==(rhs); }
  bool operator<(const _dbMetrics& rhs) const;
  void differences(dbDiff&           diff,
                   const char*       field,
                   const _dbMetrics& rhs) const;
  void out(dbDiff& diff, char side, const char* field) const;
  static void merge(_dbMetrics& dst,
                    _dbMetrics& src,
                    int         slew_op,
                    int         slack_op);
};

inline _dbMetrics::_dbMetrics(_dbDatabase*) : _worst_slack(2), _total_slack(2)
{
}

inline _dbMetrics::_dbMetrics(_dbDatabase*, const _dbMetrics& i)
    : _worst_slack(i._worst_slack), _total_slack(i._total_slack)
{
}

inline _dbMetrics::~_dbMetrics()
{
}

inline bool _dbMetrics::operator==(const _dbMetrics& rhs) const
{
  if (_worst_slack != rhs._worst_slack)
    return false;

  if (_total_slack != rhs._total_slack)
    return false;

  return true;
}

inline bool _dbMetrics::operator<(const _dbMetrics& rhs) const
{
  return getOID() < rhs.getOID();
}

inline dbOStream& operator<<(dbOStream& stream, const _dbMetrics& tmg)
{
  stream << tmg._worst_slack;
  stream << tmg._total_slack;
  return stream;
}

inline dbIStream& operator>>(dbIStream& stream, _dbMetrics& tmg)
{
  stream >> tmg._worst_slack;
  stream >> tmg._total_slack;
  return stream;
}

}  // namespace odb
