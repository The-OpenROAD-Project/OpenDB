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

#ifndef ADS_TM_HPP
#define ADS_TM_HPP

#include <string.h>

#include "tm_bind.h"
#include "ZSession.h"
#include "ZNamespace.h"
#include "ZException.h"

namespace odb {

template <class M>
ZTechModule<M>::ZTechModule(ZArgs* in, ZArgs* out)
{
}

template <class M>
ZTechModule<M>::~ZTechModule()
{
}

template <class M>
const char* ZTechModule<M>::getModuleName()
{
  return _module;
}

inline const char* argtype(ZArgType type, bool is_list)
{
  const char* t;

  if (is_list)
    return "list";

  switch (type) {
    case ZARG_UNKNOWN:
      t = "unknown";
      break;

    case ZARG_INT:
      t = "int";
      break;

    case ZARG_FLOAT:
      t = "float";
      break;

    case ZARG_STRING:
      t = "string";
      break;

    case ZARG_BOOL:
      t = "bool";
      break;

    case ZARG_ZOBJECT:
      t = "zobject";
      break;

    case ZARG_DBOBJECT:
      t = "dbobject";
      break;

    case ZARG_ICHANNEL:
      t = "ichannel";
      break;

    case ZARG_OCHANNEL:
      t = "ochannel";
      break;

    case ZARG_RECT:
      t = "box";
      break;

    case ZARG_POINT:
      t = "point";
      break;

    default:
      ZASSERT(0);
  }

  return t;
}

template <class M>
int ZTechModule<M>::usage(ClientData  clientData,
                          Tcl_Interp* interp,
                          int         argc,
                          const char* argv[])
{
  if (_bindings == NULL) {
    Tcl_AppendResult(interp, "module has not been bound to Tcl", NULL);
    return TCL_ERROR;
  }

  std::string us;

  us.append("{\n");

  typename std::vector<ZMethod<M>*>::iterator mitr;

  for (mitr = _bindings->_methods.begin(); mitr != _bindings->_methods.end();
       ++mitr) {
    ZMethod<M>* binding = *mitr;
    char        buf[BUFSIZ];

    if (strcmp(binding->_name, _module) == 0)
      snprintf(buf, BUFSIZ, "  constructor {\n");
    else
      snprintf(buf, BUFSIZ, "  %s {\n", binding->_name);
    us.append(buf);
    us.append("    inputs {\n");

    typename std::vector<ZArg>::iterator aitr;
    for (aitr = binding->_in_args.begin(); aitr != binding->_in_args.end();
         ++aitr) {
      ZArg& arg = *aitr;
      if (arg._list)
        snprintf(buf,
                 BUFSIZ,
                 "      { default \"%s\" type %s var \"%s\" usage \"%s\" }\n",
                 arg._default,
                 argtype(arg._type, true),
                 arg._name,
                 arg._usage);
      else
        snprintf(buf,
                 BUFSIZ,
                 "      { default \"%s\" type %s var \"%s\" usage \"%s\" }\n",
                 arg._default,
                 argtype(arg._type, false),
                 arg._name,
                 arg._usage);
      us.append(buf);
    }

    us.append("    }\n");
    us.append("    outputs {\n");

    for (aitr = binding->_out_args.begin(); aitr != binding->_out_args.end();
         ++aitr) {
      ZArg& arg = *aitr;
      if (arg._list)
        snprintf(buf,
                 BUFSIZ,
                 "      { default \"%s\" type %s var \"%s\" usage \"%s\" }\n",
                 arg._default,
                 argtype(arg._type, true),
                 arg._name,
                 arg._usage);
      else
        snprintf(buf,
                 BUFSIZ,
                 "      { default \"%s\" type %s var \"%s\" usage \"%s\" }\n",
                 arg._default,
                 argtype(arg._type, false),
                 arg._name,
                 arg._usage);
      us.append(buf);
    }

    us.append("    }\n");
    us.append("  }\n");
  }

  us.append("  configure {\n");
  us.append("    inputs {\n");

  typename std::vector<ZConfig<M>*>::iterator citr;

  for (citr = _bindings->_vars.begin(); citr != _bindings->_vars.end();
       ++citr) {
    ZConfig<M>* var = *citr;
    char        buf[BUFSIZ];
    if (var->_is_list)
      snprintf(buf,
               BUFSIZ,
               "      { default \"\" type %s var \"%s\" usage \"%s\" }\n",
               argtype(var->_type, true),
               var->_name,
               var->_usage);
    else
      snprintf(buf,
               BUFSIZ,
               "      { default \"\" type %s var \"%s\" usage \"%s\" }\n",
               argtype(var->_type, false),
               var->_name,
               var->_usage);
    us.append(buf);
  }

  us.append("    }\n");
  us.append("    outputs {\n");
  us.append("    }\n");
  us.append("  }\n");

  us.append("  cget {\n");
  us.append("    inputs {\n");

  for (citr = _bindings->_vars.begin(); citr != _bindings->_vars.end();
       ++citr) {
    ZConfig<M>* var = *citr;
    char        buf[BUFSIZ];
    if (var->_is_list)
      snprintf(buf,
               BUFSIZ,
               "      { default \"\" type %s var \"%s\" usage \"%s\" }\n",
               argtype(var->_type, true),
               var->_name,
               var->_usage);
    else
      snprintf(buf,
               BUFSIZ,
               "      { default \"\" type %s var \"%s\" usage \"%s\" }\n",
               argtype(var->_type, false),
               var->_name,
               var->_usage);
    us.append(buf);
  }

  us.append("    }\n");
  us.append("    outputs {\n");
  for (citr = _bindings->_vars.begin(); citr != _bindings->_vars.end();
       ++citr) {
    ZConfig<M>* var = *citr;
    char        buf[BUFSIZ];
    if (var->_is_list)
      snprintf(buf,
               BUFSIZ,
               "      { default \"\" type %s var \"%s\" usage \"%s\" }\n",
               argtype(var->_type, true),
               var->_name,
               var->_usage);
    else
      snprintf(buf,
               BUFSIZ,
               "      { default \"\" type %s var \"%s\" usage \"%s\" }\n",
               argtype(var->_type, false),
               var->_name,
               var->_usage);
    us.append(buf);
  }
  us.append("    }\n");
  us.append("  }\n");

  us.append("}");
  Tcl_AppendResult(interp, us.c_str(), NULL);
  return TCL_OK;
}

inline void* str2addr(const char* str)
{
#ifdef WIN32
  union
  {
    uint  i;
    void* p;
  } addr;
  addr.i = strtoul(str, NULL, 0);
#else
  union
  {
    uint   i;
    uint64 l;
    void*  p;
  } addr;

  if (sizeof(void*) == 4)
    addr.i = strtoul(str, NULL, 0);
  else
    addr.l = strtoull(str, NULL, 0);
#endif

  return addr.p;
}

//
// This method constructs an instance of a module in Tcl
//
template <class M>
int ZTechModule<M>::construct(ClientData  clientData,
                              Tcl_Interp* interp,
                              int         argc,
                              const char* argv[])
{
  if (!((argc == 4) || (argc == 5))) {
    Tcl_AppendResult(interp,
                     "invalid arg count, method takes two or three arguments, "
                     "usage: MODULE_tm var CIN COUT CSET?",
                     NULL);
    return TCL_ERROR;
  }

  ZMethod<M>* b = _bindings->findMethod(_module);

  if (b == NULL) {
    Tcl_AppendResult(interp, "cannot find constructor method", NULL);
    return TCL_ERROR;
  }

  dbDatabase* db       = NULL;
  ZSession*   zsession = NULL;

  const char* session = Tcl_GetVar2(interp, (char*) argv[2], "session", 0);

  if (session == NULL || session[0] == '\0') {
    if (strcmp(_module, "ZSession") != 0) {
      Tcl_AppendResult(interp, "session argument missing", NULL);
      return TCL_ERROR;
    }
  } else {
    zsession = (ZSession*) str2addr(session);

    if (strcmp(_module, "ZDB") != 0) {
      const char* dbname = Tcl_GetVar2(interp, (char*) argv[2], "db", 0);

      if (dbname && (dbname[0] != '\0')) {
        db = (dbDatabase*) zsession->_ns->resolveDB(dbname);

        if (db == NULL) {
          Tcl_AppendResult(interp, "unknown database name", NULL);
          return TCL_ERROR;
        }
      } else {
        Tcl_AppendResult(interp, "db argument missing", NULL);
        return TCL_ERROR;
      }
    }
  }

  ZArgs*   in_args;
  ZArgs*   out_args;
  M*       m = NULL;
  ZContext context;
  context._session = zsession;
  context._interp  = interp;

  try {
    in_args = (b->_get_in_args)(
        b->_in_args, context, argv[2], argc == 5 ? argv[4] : NULL);
    out_args = (b->_get_out_args)(b->_out_args, context);
    m        = (M*) malloc(sizeof(M));

    if (m == NULL) {
      delete in_args;
      delete out_args;
      Tcl_AppendResult(interp, "out of memory", NULL);
      return TCL_ERROR;
    }

    m->ZInterface::_context = context;
    m->ZTechModule<M>::_db  = db;
    new (m) M(in_args, out_args);
  } catch (ZException& ex) {
    delete m;
    delete in_args;
    delete out_args;
    Tcl_AppendResult(interp, ex._msg, NULL);
    return TCL_ERROR;
  }

  if (Tcl_CreateCommand(interp,
                        argv[1],
                        (Tcl_CmdProc*) dispatch,
                        (ClientData) m,
                        (Tcl_CmdDeleteProc*) NULL)
      == NULL) {
    delete m;
    delete in_args;
    delete out_args;
    return TCL_ERROR;
  }

  int i;
  for (i = 0; i < out_args->getCount(); ++i)
    Tcl_SetVar2(interp,
                (char*) argv[3],
                (char*) out_args->getName(i),
                out_args->getValue(i),
                0);

  delete in_args;
  delete out_args;
  return TCL_OK;
}

//
// This method executes a method on a module-instance from Tcl
//
template <class M>
int ZTechModule<M>::dispatch(ClientData  clientData,
                             Tcl_Interp* interp,
                             int         argc,
                             const char* argv[])
{
  M* ztm = (M*) clientData;

  if (argc == 1) {
    Tcl_AppendResult(interp, "method not specified", NULL);
    return TCL_ERROR;
  }

  if ((argv[1][0] == 'd') && (argv[1][1] == 'e')
      && (strcmp(argv[1] + 2, "lete") == 0)) {
    int r = Tcl_DeleteCommand(interp, argv[0]);
    ztm->~M();
    free((void*) ztm);
    return r;
  }

  if (!((argc == 4) || (argc == 5))) {
    Tcl_AppendResult(interp,
                     "invalid arg count, method takes two or three arguments, "
                     "usage: MODULE_tm var CIN COUT CSET?",
                     NULL);
    return TCL_ERROR;
  }

  if ((argv[1][0] == 'c') && (argv[1][1] == 'o')
      && (strcmp(argv[1] + 2, "nfigure") == 0)) {
    ZArgs                                       in(ztm->_context);
    ZArgs                                       out(ztm->_context);
    typename std::vector<ZConfig<M>*>::iterator itr;

    for (itr = _bindings->_vars.begin(); itr != _bindings->_vars.end(); ++itr) {
      ZConfig<M>* config = *itr;
      const char* value
          = Tcl_GetVar2(interp, (char*) argv[2], (char*) config->_name, 0);
      if (value)
        in.addArg(config->_name, value, config->_type, false);
    }

    try {
      ztm->configure(&in, &out);
    } catch (ZException& e) {
      Tcl_AppendResult(interp, " ", e._msg, NULL);
      return TCL_ERROR;
    }

    return TCL_OK;
  }

  if ((argv[1][0] == 'c') && (argv[1][1] == 'g')
      && (strcmp(argv[1] + 2, "et") == 0)) {
    ZArgs                                       in(ztm->_context);
    ZArgs                                       out(ztm->_context);
    typename std::vector<ZConfig<M>*>::iterator itr;

    for (itr = _bindings->_vars.begin(); itr != _bindings->_vars.end(); ++itr) {
      ZConfig<M>* config = *itr;
      const char* value
          = Tcl_GetVar2(interp, (char*) argv[2], (char*) config->_name, 0);
      if (value)
        in.addArg(config->_name, value, config->_type, false);
    }

    try {
      ztm->cget(&in, &out);
    } catch (ZException& e) {
      Tcl_AppendResult(interp, " ", e._msg, NULL);
      return TCL_ERROR;
    }

    int i;
    for (i = 0; i < out.getCount(); ++i)
      Tcl_SetVar2(
          interp, (char*) argv[3], (char*) out.getName(i), out.getValue(i), 0);
    return TCL_OK;
  }

  ZMethod<M>* b = _bindings->findMethod(argv[1]);

  if (b == NULL) {
    char buffer[256];
    snprintf(buffer,
             256,
             "Attempt to execute unknown method \"%s\" on module \"%s\".",
             argv[1],
             _module);
    Tcl_AppendResult(interp, buffer, NULL);
    return TCL_ERROR;
  }

  ZArgs* in_args;
  ZArgs* out_args;

  int r;

  try {
    // fprintf(stderr,"DISPATCH TEST\n");
    in_args = (b->_get_in_args)(
        b->_in_args, ztm->_context, argv[2], argc == 5 ? argv[4] : NULL);
    out_args = (b->_get_out_args)(b->_out_args, ztm->_context);
    r        = (ztm->*(b->_method))(in_args, out_args);
  } catch (ZException& e) {
    // fprintf(stderr,"DISPATCH CATCH: %s\n",e._msg);
    delete in_args;
    delete out_args;
    Tcl_AppendResult(interp, " ", e._msg, NULL);
    return TCL_ERROR;
  }

  int i;
  for (i = 0; i < out_args->getCount(); ++i)
    Tcl_SetVar2(interp,
                (char*) argv[3],
                (char*) out_args->getName(i),
                out_args->getValue(i),
                0);

  char buf[16];
  snprintf(buf, 16, "%d", r);
  if (strcmp(_module, "Dcr")) {
    Tcl_AppendResult(interp, buf, NULL);
  }
  delete in_args;
  delete out_args;
  return TCL_OK;
}

template <class M>
int ZTechModule<M>::configure(ZArgs* in, ZArgs* out)
{
  int i;

  M* m = (M*) this;

  for (i = 0; i < in->getCount(); ++i) {
    const char* name    = in->getName(i);
    ZConfig<M>* zconfig = _bindings->findConfig(name);

    if (zconfig == NULL)
      throw ZException("unknown configuration variable %s", name);

    switch (zconfig->_type) {
      case ZARG_INT: {
        if (zconfig->_is_list == false) {
          int v = in->getInt(i);
          if (zconfig->_vnotify._int == NULL)
            m->*(zconfig->_vbind._int) = v;
          else if ((m->*zconfig->_vnotify._int)(v) == true)
            m->*(zconfig->_vbind._int) = v;
          else
            throw ZException("change not accepted for %s", name);
        } else {
          std::list<int> v;
          in->getInt(i, v);
          if (zconfig->_vnotify._int_list == NULL)
            m->*(zconfig->_vbind._int_list) = v;
          else if ((m->*zconfig->_vnotify._int_list)(v) == true)
            m->*(zconfig->_vbind._int_list) = v;
          else
            throw ZException("change not accepted for %s", name);
        }
        break;
      }

      case ZARG_FLOAT: {
        if (zconfig->_is_list == false) {
          double v = in->getFloat(i);
          if (zconfig->_vnotify._float == NULL)
            m->*(zconfig->_vbind._float) = v;
          else if ((m->*zconfig->_vnotify._float)(v) == true)
            m->*(zconfig->_vbind._float) = v;
          else
            throw ZException("change not accepted for %s", name);
        } else {
          std::list<double> v;
          in->getFloat(i, v);
          if (zconfig->_vnotify._float_list == NULL)
            m->*(zconfig->_vbind._float_list) = v;
          else if ((m->*zconfig->_vnotify._float_list)(v) == true)
            m->*(zconfig->_vbind._float_list) = v;
          else
            throw ZException("change not accepted for %s", name);
        }
        break;
      }

      case ZARG_STRING: {
        if (zconfig->_is_list == false) {
          std::string v = in->getString(i);
          if (zconfig->_vnotify._string == NULL)
            m->*(zconfig->_vbind._string) = v;
          else if ((m->*zconfig->_vnotify._string)(v) == true)
            m->*(zconfig->_vbind._string) = v;

          else
            throw ZException("change not accepted for %s", name);
        } else {
          std::list<std::string> v;
          in->getString(i, v);
          if (zconfig->_vnotify._string_list == NULL)
            m->*(zconfig->_vbind._string_list) = v;
          else if ((m->*zconfig->_vnotify._string_list)(v) == true)
            m->*(zconfig->_vbind._string_list) = v;
          else
            throw ZException("change not accepted for %s", name);
        }
        break;
      }

      case ZARG_BOOL: {
        if (zconfig->_is_list == false) {
          bool v = in->getBool(i);
          if (zconfig->_vnotify._bool == NULL)
            m->*(zconfig->_vbind._bool) = v;
          else if ((m->*zconfig->_vnotify._bool)(v) == true)
            m->*(zconfig->_vbind._bool) = v;
          else
            throw ZException("change not accepted for %s", name);
        } else {
          std::list<bool> v;
          in->getBool(i, v);
          if (zconfig->_vnotify._bool_list == NULL)
            m->*(zconfig->_vbind._bool_list) = v;
          else if ((m->*zconfig->_vnotify._bool_list)(v) == true)
            m->*(zconfig->_vbind._bool_list) = v;
          else
            throw ZException("change not accepted for %s", name);
        }
        break;
      }

      case ZARG_ZOBJECT: {
        if (zconfig->_is_list == false) {
          ZObject* v = in->getZObject(i);
          if (zconfig->_vnotify._zobj == NULL)
            m->*(zconfig->_vbind._zobj) = v;
          else if ((m->*zconfig->_vnotify._zobj)(v) == true)
            m->*(zconfig->_vbind._zobj) = v;
          else
            throw ZException("change not accepted for %s", name);
        } else {
          std::list<ZObject*> v;
          in->getZObject(i, v);
          if (zconfig->_vnotify._zobj_list == NULL)
            m->*(zconfig->_vbind._zobj_list) = v;
          else if ((m->*zconfig->_vnotify._zobj_list)(v) == true)
            m->*(zconfig->_vbind._zobj_list) = v;
          else
            throw ZException("change not accepted for %s", name);
        }
        break;
      }

      case ZARG_DBOBJECT: {
        if (zconfig->_is_list == false) {
          dbObject* v = in->getDbObject(i);
          if (zconfig->_vnotify._dbobj == NULL)
            m->*(zconfig->_vbind._dbobj) = v;
          else if ((m->*zconfig->_vnotify._dbobj)(v) == true)
            m->*(zconfig->_vbind._dbobj) = v;
          else
            throw ZException("change not accepted for %s", name);
        } else {
          std::list<dbObject*> v;
          in->getDbObject(i, v);
          if (zconfig->_vnotify._dbobj_list == NULL)
            m->*(zconfig->_vbind._dbobj_list) = v;
          else if ((m->*zconfig->_vnotify._dbobj_list)(v) == true)
            m->*(zconfig->_vbind._dbobj_list) = v;
          else
            throw ZException("change not accepted for %s", name);
        }
        break;
      }

      default:
        throw ZException("invalid argument type");
    }
  }

  return TCL_OK;
}

template <class M>
int ZTechModule<M>::cget(ZArgs* in, ZArgs* out)
{
  int i;

  M* m = (M*) this;

  for (i = 0; i < in->getCount(); ++i) {
    const char* name    = in->getName(i);
    ZConfig<M>* zconfig = _bindings->findConfig(name);
    out->addArg(name, "", zconfig->_type, false);

    if (zconfig == NULL)
      throw ZException("unknown configuration variable %s", name);

    switch (zconfig->_type) {
      case ZARG_INT: {
        if (zconfig->_is_list == false)
          out->setInt(i, m->*(zconfig->_vbind._int));
        else
          out->setInt(i, m->*(zconfig->_vbind._int_list));
        break;
      }

      case ZARG_FLOAT: {
        if (zconfig->_is_list == false)
          out->setFloat(i, m->*(zconfig->_vbind._float));
        else
          out->setFloat(i, m->*(zconfig->_vbind._float_list));
        break;
      }

      case ZARG_STRING: {
        if (zconfig->_is_list == false)
          out->setString(i, m->*(zconfig->_vbind._string));
        else
          out->setString(i, m->*(zconfig->_vbind._string_list));
        break;
      }

      case ZARG_BOOL: {
        if (zconfig->_is_list == false)
          out->setBool(i, m->*(zconfig->_vbind._bool));
        else
          out->setBool(i, m->*(zconfig->_vbind._bool_list));
        break;
      }

      case ZARG_ZOBJECT: {
        if (zconfig->_is_list == false)
          out->setZObject(i, m->*(zconfig->_vbind._zobj));
        else
          out->setZObject(i, m->*(zconfig->_vbind._zobj_list));
        break;
      }

      case ZARG_DBOBJECT: {
        if (zconfig->_is_list == false)
          out->setDbObject(i, m->*(zconfig->_vbind._dbobj));
        else
          out->setDbObject(i, m->*(zconfig->_vbind._dbobj_list));
        break;
      }

      default:
        throw ZException("invalid argument type");
    }
  }
  return TCL_OK;
}

}  // namespace odb

#endif
