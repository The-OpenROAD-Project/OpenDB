///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2020, OpenROAD
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

#include <string>

#include "spdlog/spdlog.h"

namespace ord {

enum MessageStatus
{
  INFO  = spdlog::level::info,
  WARN  = spdlog::level::warn,
  ERROR = spdlog::level::err,
  CRIT  = spdlog::level::critical
};

enum ModuleType
{
  OPENDB,
  OPENROAD,
  OPENSTA,
  OPENRCX,
  IOPLACER,
  TRITONROUTE,
  REPLACE,
  RESIZER,
  GUI,
  FASTROUTE
};

static const char* modules_name_tbl[] = {
    "OPDB",
    "OPRD",
    "OSTA",
    "ORCX",
    "IOPL",
    "TRRT",
    "RPLC",
    "RSZR",
    "GGUI",
    "FTRT",
};

int addSinkStdout();
int removeSinkStdout();

int addSinkFile(const char* file_name);
int removeSinkFile(const char* file_name);

void init();
void init(const char* file_name);

template <typename... Args>
inline int info(ModuleType         type,
                int                id,
                const std::string& message,
                const Args&... args)
{
  return Log(type, INFO, id, message, args...);
}

template <typename... Args>
inline int warn(ModuleType         type,
                int                id,
                const std::string& message,
                const Args&... args)
{
  return Log(type, WARN, id, message, args...);
}

template <typename... Args>
inline int error(ModuleType         type,
                 int                id,
                 const std::string& message,
                 const Args&... args)
{
  return Log(type, ERROR, id, message, args...);
}

template <typename... Args>
inline int crit(ModuleType         type,
                int                id,
                const std::string& message,
                const Args&... args)
{
  return Log(type, CRIT, id, message, args...);
}

template <typename... Args>
inline int Log(ModuleType         type,
               MessageStatus      status,
               int                id,
               const std::string& message,
               const Args&... args)
{
  if (id < 0 || id > 9999)
    return -1;  // invalid id
  spdlog::log((spdlog::level::level_enum) status,
              "[{}-{:04d}] " + message,
              modules_name_tbl[type],
              id,
              args...);
  return 0;
}

}  // namespace ordlog
