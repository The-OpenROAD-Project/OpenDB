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

#ifndef ADS_TM_BIND_H
#define ADS_TM_BIND_H

#include <string.h>

#ifndef _TCL
#include <tcl.h>
#endif

#include "ads.h"
#include "geom.h"

namespace odb {

class ZArgs;
template <class M>
class ZBindings;

/////////////////////////////////
/// Binding definition
/////////////////////////////////

struct ZArg
{
  ZArgType    _type;
  bool        _list;
  char*       _name;
  const char* _default;
  const char* _usage;
};

struct ZVar
{
  ZArgType    _type;
  bool        _list;
  char*       _name;
  const char* _usage;
};

template <class M>
class ZMethod
{
 public:
  const char*        _name;
  typename M::Method _method;
  std::vector<ZArg>  _in_args;
  std::vector<ZArg>  _out_args;
  ZArgs* (*_get_in_args)(const std::vector<ZArg>& in_args,
                         ZContext&                context,
                         const char*              vname,
                         const char*              sname);
  ZArgs* (*_get_out_args)(const std::vector<ZArg>& out_args, ZContext& context);

  ZMethod()
  {
    _name   = NULL;
    _method = NULL;
  }

  ZMethod(const char* name, typename M::Method method)
  {
    _name   = strdup(name);
    _method = method;
  }

  ~ZMethod();

  void addInArg(ZArgType    type,
                bool        is_list,
                const char* name,
                const char* default_value,
                const char* usage);
  void addOutArg(ZArgType    type,
                 bool        is_list,
                 const char* name,
                 const char* default_value,
                 const char* usage);
};

template <class M>
class ZConfig
{
 public:
  const char* _name;
  const char* _usage;
  ZArgType    _type;
  bool        _is_list;

  union
  {
    bool M::*_bool;
    int M::*_int;
    double M::* _float;
    std::string M::*_string;
    dbObject* M::*_dbobj;
    ZObject* M::*_zobj;
    adsRect M::*_rect;
    adsPoint M::*_point;

    std::list<bool> M::*_bool_list;
    std::list<int> M::*_int_list;
    std::list<double> M::* _float_list;
    std::list<std::string> M::*_string_list;
    std::list<dbObject*> M::*_dbobj_list;
    std::list<ZObject*> M::*_zobj_list;
    std::list<adsRect> M::*_rect_list;
    std::list<adsPoint> M::*_point_list;
  } _vbind;

  union
  {
    bool (M::*_bool)(bool);
    bool (M::*_int)(int);
    bool (M::*_float)(double);
    bool (M::*_string)(const std::string&);
    bool (M::*_dbobj)(dbObject*);
    bool (M::*_zobj)(ZObject*);
    bool (M::*_rect)(const adsRect&);
    bool (M::*_point)(adsPoint);
    bool (M::*_bool_list)(const std::list<bool>&);
    bool (M::*_int_list)(const std::list<int>&);
    bool (M::*_float_list)(const std::list<double>&);
    bool (M::*_string_list)(const std::list<std::string>&);
    bool (M::*_dbobj_list)(const std::list<dbObject*>&);
    bool (M::*_zobj_list)(const std::list<ZObject*>&);
    bool (M::*_rect_list)(const std::list<adsRect>&);
    bool (M::*_point_list)(const std::list<adsPoint>&);
  } _vnotify;

  ZConfig();
  ZConfig(const char* name, const char* usage);
  ~ZConfig();

  void setMember(bool M::*mbr_bool, bool (M::*notify)(bool) = NULL);
  void setMember(int M::*mbr_int, bool (M::*notify)(int) = NULL);
  void setMember(double M::*mbr_double, bool (M::*notify)(double) = NULL);
  void setMember(std::string M::*mbr_string,
                 bool (M::*notify)(const std::string&) = NULL);
  void setMember(dbObject* M::*mbr_dbobj, bool (M::*notify)(dbObject*) = NULL);
  void setMember(ZObject* M::*mbr_zobj, bool (M::*notify)(ZObject*) = NULL);
  void setMember(adsRect M::*mbr_rect,
                 bool (M::*notify)(const adsRect&) = NULL);
  void setMember(adsPoint M::*mbr_rect, bool (M::*notify)(adsPoint) = NULL);

  void setMember(std::list<bool> M::*mbr_int_list,
                 bool (M::*notify)(const std::list<bool>&) = NULL);
  void setMember(std::list<int> M::*mbr_int_list,
                 bool (M::*notify)(const std::list<int>&) = NULL);
  void setMember(std::list<double> M::*mbr_double_list,
                 bool (M::*notify)(const std::list<double>&) = NULL);
  void setMember(std::list<std::string> M::*mbr_string_list,
                 bool (M::*notify)(const std::list<std::string>&) = NULL);
  void setMember(std::list<dbObject*> M::*mbr_dbobj_list,
                 bool (M::*notify)(const std::list<dbObject*>&) = NULL);
  void setMember(std::list<ZObject*> M::*mbr_zobj_list,
                 bool (M::*notify)(const std::list<ZObject*>&) = NULL);
  void setMember(std::list<adsRect> M::*mbr_rect_list,
                 bool (M::*notify)(const std::list<adsRect>&) = NULL);
  void setMember(std::list<adsPoint> M::*mbr_point_list,
                 bool (M::*notify)(const std::list<adsPoint>&) = NULL);
};

template <class M>
class ZBindings
{
 public:
  typedef typename M::Method Method;
  std::vector<ZMethod<M>*>   _methods;
  std::vector<ZConfig<M>*>   _vars;

  ZBindings();
  ~ZBindings();

  ZMethod<M>* bindMethod(const char* zcmd, Method method);
  ZMethod<M>* findMethod(const char* name);
  ZConfig<M>* bindConfig(const char* zvar, const char* usage);
  ZConfig<M>* findConfig(const char* name);
};

}  // namespace odb

#endif
