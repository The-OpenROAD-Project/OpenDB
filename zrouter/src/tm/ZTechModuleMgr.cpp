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

#include <stdarg.h>
#include <map>

#ifdef WIN32
#include <stdlib.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#define ADS_TM_BIND_HPP

#include <string>
#include <map>
#include <errno.h>
#include "tm.h"
#include "tm_bind.h"

BEGIN_NAMESPACE_ADS

class ZTechMethodDoc
{
public:
    std::string    _name;
    std::string    _doc;

    ZTechMethodDoc( const char * name, const char * doc )
            :_name(name),
             _doc(doc)
    {
    }
    
};

class ZTechModuleDoc
{
public:
    std::string                   _name;
    std::string                   _doc;
    std::vector<ZTechMethodDoc *> _methods;

    ZTechModuleDoc( const char * name, const char * doc )
            :_name(name),
             _doc(doc)
    {
    }
};

static std::map<std::string, ZTechModuleDoc *> documentation;
    
void ZTechModuleMgr::initialize( Tcl_Interp * interp )
{
    if ( Tcl_CreateCommand( interp, "help", (Tcl_CmdProc *) &ZTechModuleMgr::man, (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL ) == NULL )
    {
    }
}

void ZTechModuleMgr::module_doc( const char * module, const char * doc )
{
    ZTechModuleDoc * m = new ZTechModuleDoc(module, doc);
    documentation[std::string(module)] = m;
}

void ZTechModuleMgr::method_doc( const char * module, const char * method, const char * doc )
{
    ZTechModuleDoc * m = documentation[std::string(module)];
    ZTechMethodDoc * d = new ZTechMethodDoc(method, doc);
    m->_methods.push_back(d);
}

void ZTechModuleMgr::variable_doc( const char * module, const char * method, const char * doc )
{
    ZTechModuleDoc * m = documentation[std::string(module)];
    ZTechMethodDoc * d = new ZTechMethodDoc(method, doc);
    m->_methods.push_back(d);
}

int ZTechModuleMgr::man( ClientData clientData, Tcl_Interp * interp, int argc, const char * argv[] )
{
    if ( (argc != 2) && (argc != 3) )
    {
        char buf[8192];
        snprintf( buf, 8191, "usage: %s class <method>\n", argv[0]);
        Tcl_AppendResult( interp, buf, NULL );
        return TCL_ERROR;
    }
    
    std::map<std::string, ZTechModuleDoc *>::iterator itr = documentation.find(std::string(argv[1]));

    if ( itr == documentation.end() )
    {
        char buf[8192];
        snprintf( buf, 8191, "Unknown class: %s\n", argv[1]);
        Tcl_AppendResult( interp, buf, NULL );
        return TCL_ERROR;
    }

    ZTechModuleDoc * m = (*itr).second;

    if ( argc == 2 )
    {
        Tcl_AppendResult( interp, m->_doc.c_str(), NULL );
        return TCL_OK;
    }
    
    std::vector<ZTechMethodDoc *>::iterator mitr;

    for( mitr = m->_methods.begin(); mitr != m->_methods.end(); ++mitr )
    {
        ZTechMethodDoc * d = *mitr;

        if ( strcmp(d->_name.c_str(), argv[2]) == 0 )
        {
            Tcl_AppendResult( interp, d->_doc.c_str(), NULL );
            return TCL_OK;
        }
             
    }
    
    char buf[8192];
    snprintf( buf, 8191, "Unknown class method: %s::%s\n", argv[1], argv[2]);
    Tcl_AppendResult( interp, buf, NULL );
    return TCL_ERROR;
}

END_NAMESPACE_ADS
