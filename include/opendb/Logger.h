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
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "string"
namespace ordlog {
using std::string;
enum MessageStatus
{
  TRACE,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  CRIT
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

///
/// initializes {logger_name} in filename target location
///
int init(const char* file_name, const char* logger_name = "ord_logger", bool make_default = true)
{
  if (spdlog::get(logger_name).get() != nullptr)
    return -1;  // already initialized
  auto logger = spdlog::basic_logger_mt(logger_name, file_name);
  if(make_default)
  {
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%^%l%$] %v");
  }else
    logger.get()->set_pattern("[%^%l%$] %v");
  return 0;
}

///
/// initializes {logger_name} in stdout
///
int initDefault(const char* logger_name = "ord_logger", bool make_default = true)
{
  if (spdlog::get(logger_name).get() != nullptr)
    return -1;  // already initialized
  auto logger = spdlog::stdout_color_mt(logger_name);
  if(make_default)
  {
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%^%l%$] %v");
  }else
    logger.get()->set_pattern("[%^%l%$] %v");
  return 0;
}

///
/// drop {logger_name}
///
int drop(const char* logger_name = "ord_logger")
{
  spdlog::logger* logger = spdlog::get(logger_name).get();
  if (logger == nullptr)
    return -1;  // uninitialized
  logger->flush();
  spdlog::drop(logger_name);
  return 0;
}

///
/// logging to default ord_logger
///
template <typename... Args>
int info(ModuleType _type, int id, string message, const Args&... args)
{
  return Log(_type, INFO, id, message, args...);
}

template <typename... Args>
int warn(ModuleType _type, int id, string message, const Args&... args)
{
  return Log(_type, WARN, id, message, args...);
}

template <typename... Args>
int error(ModuleType _type, int id, string message, const Args&... args)
{
  return Log(_type, ERROR, id, message, args...);
}

template <typename... Args>
int crit(ModuleType _type, int id, string message, const Args&... args)
{
  return Log(_type, CRIT, id, message, args...);
}

///
/// logging to special logger {logger_name}
///
template <typename... Args>
int info(const char* logger_name,
         ModuleType  _type,
         int         id,
         string      message,
         const Args&... args)
{
  return Log(logger_name, _type, INFO, id, message, args...);
}

template <typename... Args>
int warn(const char* logger_name,
         ModuleType  _type,
         int         id,
         string      message,
         const Args&... args)
{
  return Log(logger_name, _type, WARN, id, message, args...);
}

template <typename... Args>
int error(const char* logger_name,
          ModuleType  _type,
          int         id,
          string      message,
          const Args&... args)
{
  return Log(logger_name, _type, ERROR, id, message, args...);
}

template <typename... Args>
int crit(const char* logger_name,
         ModuleType  _type,
         int         id,
         string      message,
         const Args&... args)
{
  return Log(logger_name, _type, CRIT, id, message, args...);
}

template <typename... Args>
int Log(ModuleType    _type,
        MessageStatus _status,
        int           id,
        string        message,
        const Args&... args)
{
  if (id < 0 || id > 9999)
    return -1;  // invalid id
  initDefault(); 
  message                = "[{}-{:04d}] " + message;
  const char* type       = modules_name_tbl[_type];
  spdlog::log((spdlog::level::level_enum) _status, message, type, id, args...);
  return 0;
}

template <typename... Args>
int Log(const char*   logger_name,
        ModuleType    _type,
        MessageStatus _status,
        int           id,
        string        message,
        const Args&... args)
{
  if (id < 0 || id > 9999)
    return -1;  // invalid id
  initDefault(logger_name,false);
  auto logger = spdlog::get(logger_name).get();
  message                = "[{}-{:04d}] " + message;
  const char* type       = modules_name_tbl[_type];
  logger->log((spdlog::level::level_enum) _status, message, type, id, args...);
  return 0;
}
}  // namespace ordlog