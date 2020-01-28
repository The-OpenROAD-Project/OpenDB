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

namespace odb {
template <class M>
ZConfig<M>::ZConfig()
{
  _name  = NULL;
  _usage = NULL;
}

template <class M>
ZConfig<M>::ZConfig(const char* name, const char* usage)
{
  _name  = strdup(name);
  _usage = strdup(usage);
}

template <class M>
ZConfig<M>::~ZConfig()
{
  if (_name)
    free((void*) _name);

  if (_usage)
    free((void*) _usage);
}

template <class M>
void ZConfig<M>::setMember(bool M::*mbr_bool, bool (M::*notify)(bool))
{
  _type          = ZARG_BOOL;
  _is_list       = false;
  _vbind._bool   = mbr_bool;
  _vnotify._bool = notify;
}

template <class M>
void ZConfig<M>::setMember(int M::*mbr_int, bool (M::*notify)(int))
{
  _type         = ZARG_INT;
  _is_list      = false;
  _vbind._int   = mbr_int;
  _vnotify._int = notify;
}

template <class M>
void ZConfig<M>::setMember(double M::*mbr_double, bool (M::*notify)(double))
{
  _type           = ZARG_FLOAT;
  _is_list        = false;
  _vbind._float   = mbr_double;
  _vnotify._float = notify;
}

template <class M>
void ZConfig<M>::setMember(std::string M::*mbr_string,
                           bool (M::*notify)(const std::string&))
{
  _type            = ZARG_STRING;
  _is_list         = false;
  _vbind._string   = mbr_string;
  _vnotify._string = notify;
}

template <class M>
void ZConfig<M>::setMember(dbObject* M::*mbr_dbobj,
                           bool (M::*notify)(dbObject*))
{
  _type           = ZARG_DBOBJECT;
  _is_list        = false;
  _vbind._dbobj   = mbr_dbobj;
  _vnotify._dbobj = notify;
}

template <class M>
void ZConfig<M>::setMember(ZObject* M::*mbr_zobj, bool (M::*notify)(ZObject*))
{
  _type          = ZARG_ZOBJECT;
  _is_list       = false;
  _vbind._zobj   = mbr_zobj;
  _vnotify._zobj = notify;
}

template <class M>
void ZConfig<M>::setMember(adsRect M::*mbr_rect,
                           bool (M::*notify)(const adsRect&))
{
  _type          = ZARG_RECT;
  _is_list       = false;
  _vbind._rect   = mbr_rect;
  _vnotify._rect = notify;
}

template <class M>
void ZConfig<M>::setMember(adsPoint M::*mbr_point, bool (M::*notify)(adsPoint))
{
  _type           = ZARG_POINT;
  _is_list        = false;
  _vbind._point   = mbr_point;
  _vnotify._point = notify;
}

template <class M>
void ZConfig<M>::setMember(std::list<bool> M::*mbr_bool_list,
                           bool (M::*notify)(const std::list<bool>&))
{
  _type               = ZARG_BOOL;
  _is_list            = true;
  _vbind._bool_list   = mbr_bool_list;
  _vnotify._bool_list = notify;
}

template <class M>
void ZConfig<M>::setMember(std::list<int> M::*mbr_int_list,
                           bool (M::*notify)(const std::list<int>&))
{
  _type              = ZARG_INT;
  _is_list           = true;
  _vbind._int_list   = mbr_int_list;
  _vnotify._int_list = notify;
}

template <class M>
void ZConfig<M>::setMember(std::list<double> M::*mbr_double_list,
                           bool (M::*notify)(const std::list<double>&))
{
  _type                = ZARG_FLOAT;
  _is_list             = true;
  _vbind._float_list   = mbr_double_list;
  _vnotify._float_list = notify;
}

template <class M>
void ZConfig<M>::setMember(std::list<std::string> M::*mbr_string_list,
                           bool (M::*notify)(const std::list<std::string>&))
{
  _type                 = ZARG_STRING;
  _is_list              = true;
  _vbind._string_list   = mbr_string_list;
  _vnotify._string_list = notify;
}

template <class M>
void ZConfig<M>::setMember(std::list<dbObject*> M::*mbr_dbobj_list,
                           bool (M::*notify)(const std::list<dbObject*>&))
{
  _type                = ZARG_DBOBJECT;
  _is_list             = true;
  _vbind._dbobj_list   = mbr_dbobj_list;
  _vnotify._dbobj_list = notify;
}

template <class M>
void ZConfig<M>::setMember(std::list<ZObject*> M::*mbr_zobj_list,
                           bool (M::*notify)(const std::list<ZObject*>&))
{
  _type               = ZARG_ZOBJECT;
  _is_list            = true;
  _vbind._zobj_list   = mbr_zobj_list;
  _vnotify._zobj_list = notify;
}

template <class M>
void ZConfig<M>::setMember(std::list<adsRect> M::*mbr_rect_list,
                           bool (M::*notify)(const std::list<adsRect>&))
{
  _type               = ZARG_RECT;
  _is_list            = true;
  _vbind._rect_list   = mbr_rect_list;
  _vnotify._rect_list = notify;
}

template <class M>
void ZConfig<M>::setMember(std::list<adsPoint> M::*mbr_point_list,
                           bool (M::*notify)(const std::list<adsPoint>&))
{
  _type                = ZARG_POINT;
  _is_list             = true;
  _vbind._point_list   = mbr_point_list;
  _vnotify._point_list = notify;
}

template <class M>
ZMethod<M>::~ZMethod()
{
  if (_name)
    free((void*) _name);

  typename std::vector<ZArg>::iterator itr;

  for (itr = _in_args.begin(); itr != _in_args.end(); ++itr) {
    ZArg& a = *itr;
    free((void*) a._name);
    free((void*) a._default);
    free((void*) a._usage);
  }

  for (itr = _out_args.begin(); itr != _out_args.end(); ++itr) {
    ZArg& a = *itr;
    free((void*) a._name);
    free((void*) a._default);
    free((void*) a._usage);
  }
}

template <class M>
void ZMethod<M>::addInArg(ZArgType    type,
                          bool        is_list,
                          const char* name,
                          const char* default_value,
                          const char* usage)
{
  ZArg a;
  a._type    = type;
  a._list    = is_list;
  a._name    = strdup(name);
  a._default = strdup(default_value);
  a._usage   = strdup(usage);
  _in_args.push_back(a);
}

template <class M>
void ZMethod<M>::addOutArg(ZArgType    type,
                           bool        is_list,
                           const char* name,
                           const char* default_value,
                           const char* usage)
{
  ZArg a;
  a._type    = type;
  a._list    = is_list;
  a._name    = strdup(name);
  a._default = strdup(default_value);
  a._usage   = strdup(usage);
  _out_args.push_back(a);
}

template <class M>
ZBindings<M>::ZBindings()
{
}

template <class M>
ZBindings<M>::~ZBindings()
{
  typename std::vector<ZMethod<M>*>::iterator mitr;

  for (mitr = _methods.begin(); mitr != _methods.end(); ++mitr)
    delete *mitr;

  typename std::vector<ZConfig<M>*>::iterator citr;

  for (citr = _vars.begin(); citr != _vars.end(); ++citr)
    delete *citr;
}

template <class M>
ZMethod<M>* ZBindings<M>::bindMethod(const char* zcmd, Method method)
{
  if (findMethod(zcmd) != NULL)
    return NULL;

  ZMethod<M>* b = new ZMethod<M>(zcmd, method);
  ZALLOCATED(b);
  _methods.push_back(b);
  return b;
}

template <class M>
ZMethod<M>* ZBindings<M>::findMethod(const char* name)
{
  typename std::vector<ZMethod<M>*>::iterator itr;

  for (itr = _methods.begin(); itr != _methods.end(); ++itr) {
    ZMethod<M>* method = *itr;

    if (strcmp(method->_name, name) == 0)
      return method;
  }

  return NULL;
}

template <class M>
ZConfig<M>* ZBindings<M>::bindConfig(const char* zvar, const char* usage)
{
  if (findConfig(zvar) != NULL)
    return NULL;

  ZConfig<M>* c = new ZConfig<M>(zvar, usage);
  ZALLOCATED(c);
  _vars.push_back(c);
  return c;
}

template <class M>
ZConfig<M>* ZBindings<M>::findConfig(const char* name)
{
  typename std::vector<ZConfig<M>*>::iterator itr;

  for (itr = _vars.begin(); itr != _vars.end(); ++itr) {
    ZConfig<M>* c = *itr;

    if (strcmp(c->_name, name) == 0)
      return c;
  }

  return NULL;
}

}  // namespace odb


