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
#include "spdlog/details/console_globals.h"
#include "spdlog/spdlog.h"
#include "string"
#include <mutex>
namespace ordlog {
using std::string;
enum MessageStatus
{
  INFO = 2,
  WARN = 3,
  ERROR = 4,
  CRIT = 5
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

int removeSinkStdout()
{
  if(spdlog::default_logger().get()==nullptr)
    return -1;
  auto default_sinks = &spdlog::default_logger().get()->sinks();
  for(auto sink = default_sinks->begin();sink!=default_sinks->end();++sink)
  {
    auto stdout_sink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(sink->get());
    if(stdout_sink==nullptr)
      continue;
    else
    {
      default_sinks->erase(sink);
      return 0;
    }
  }
  return -2;
}

int addSinkStdout()
{
  if(spdlog::default_logger().get()==nullptr)
    return -1;
  auto default_sinks = &spdlog::default_logger().get()->sinks();
  for(auto sink = default_sinks->begin();sink!=default_sinks->end();++sink)
  {
    auto stdout_sink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(sink->get());
    if(stdout_sink==nullptr)
      continue;
    else
      return -2;
  }
  auto new_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  new_sink.get()->set_pattern("[%^%l%$] %v");
  default_sinks->push_back(new_sink);
  return 0;
}

int addSinkFile(const char* file_name)
{
  if(spdlog::default_logger().get()==nullptr)
    return -1;
  auto default_sinks = &spdlog::default_logger().get()->sinks();
  for(auto sink = default_sinks->begin();sink!=default_sinks->end();++sink)
  {
    auto file_sink = dynamic_cast<spdlog::sinks::basic_file_sink<std::mutex>*>(sink->get());
    if(file_sink==nullptr)
      continue;
    if(strcmp(file_sink->filename().c_str(),file_name)==0)
      return -2;//sink already exists
  }
  auto new_sink = std::make_shared<spdlog::sinks::basic_file_sink<std::mutex>>(file_name);
  new_sink.get()->set_pattern("[%^%l%$] %v");
  default_sinks->push_back(new_sink);
  return 0;
}

int removeSinkFile(const char* file_name)
{
  if(spdlog::default_logger().get()==nullptr)
    return -1;
  auto default_sinks = &spdlog::default_logger().get()->sinks();
  for(auto sink = default_sinks->begin();sink!=default_sinks->end();++sink)
  {
    auto file_sink = dynamic_cast<spdlog::sinks::basic_file_sink<std::mutex>*>(sink->get());
    if(file_sink==nullptr)
      continue;
    if(strcmp(file_sink->filename().c_str(),file_name)==0)
    {
      default_sinks->erase(sink);
      return 0;
    }
  }
  return -2;
}

void init()
{
  spdlog::set_pattern("[%^%l%$] %v");
  addSinkStdout();
}

void init(const char* file_name)
{
  spdlog::set_pattern("[%^%l%$] %v");
  removeSinkStdout();
  addSinkFile(file_name);
}

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

template <typename... Args>
int Log(ModuleType    _type,
        MessageStatus _status,
        int           id,
        string        message,
        const Args&... args)
{
  if (id < 0 || id > 9999)
    return -1;  // invalid id
  message                = "[{}-{:04d}] " + message;
  const char* type       = modules_name_tbl[_type];
  spdlog::log((spdlog::level::level_enum) _status, message, type, id, args...);
  return 0;
}

}  // namespace ordlog
