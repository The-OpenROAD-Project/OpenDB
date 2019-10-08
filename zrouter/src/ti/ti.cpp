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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include "ti.h"

extern int Tierrors;
extern FILE * Tiin;
extern const char * Tiinfile;
extern int Tiparse();
std::list<Module *> modules;
std::list<const char *> ccode;
const char * arg0;
const char * znamespace = "zroute";
const char * zpostfix = "_tm";

void check_modules();
void write_header( const char * out_prefix );
void write_cpp( const char * out_prefix );
void write_ZInOutArgs_classes( FILE * out );
void write_ZIn_class( FILE * out, Module * module, Method * method );
void write_in_args( FILE * out, std::list<Arg *> & args, const char * indent );
void write_ZOut_class( FILE * out, Module * module, Method * method );
void write_ZInOutArgs_cpp( FILE * out );
void write_out_args( FILE * out, std::list<Arg *> & args, const char * indent );
void write_ZIn_cpp( FILE * out, Module * module, Method * method );
void write_ZOut_cpp( FILE * out, Module * module, Method * method );
void write_ccode( FILE * out );
void write_template_instantations( FILE * out );
void write_static_initializations( FILE * out );
void write_static_declarations( FILE * out );
void write_ztcl_register_interfaces( FILE * out );
void write_ztcl_register_interface( FILE * out, Module * module );
void write_method_bindings_cpp( FILE * out, Module * module, Method * method );
void write_var_config_cpp( FILE * out, Module * module, Var * var );

static void usage()
{
    fprintf(stderr, "usage: %s -p out_prefix [-ns <namespace>] <ti_file>\n", arg0 );
    exit(1);
}

static char * next_arg( int & argc, char ** & argv )
{
    if ( argc == 0 )
        usage();

    --argc;
    return *++argv;
}

int main( int argc, char ** argv )
{
    const char * ti_file = NULL;
    const char * out_prefix = NULL;

    arg0 = argv[0];

    for( ++argv, --argc; argc > 0; ++argv, --argc )
    {
        char * argp = *argv;
        
        if ( strcmp( argp, "-p" ) == 0 )
        {
            if ( out_prefix )
                usage();
            
            out_prefix = next_arg(argc, argv);
        }

        else if ( strcmp( argp, "-ns" ) == 0 )
        {
            znamespace = next_arg(argc, argv);
        }
        
        else if ( argp[0] == '-' )
        {
            usage();
        }
        else
        {
            if ( ti_file )
                usage();

            ti_file = argp;
        }
    }

    if ( ti_file == NULL || out_prefix == NULL )
        usage();

    Tiinfile = ti_file;
    Tiin = fopen(ti_file, "r");

    if ( Tiin == NULL )
    {
        printf("Cannot open file (%s)\n", ti_file );
        exit(1);
    }
    
    Tiparse();

    if ( Tierrors > 0 )
        exit(1);
    
    fclose(Tiin);
    check_modules();
    write_header( out_prefix );
    write_cpp( out_prefix );
    return 0;
}

void check_modules()
{
    std::list<Module *>::iterator module_itr;
    for( module_itr = modules.begin(); module_itr != modules.end(); ++module_itr )
    {
        Module * module = * module_itr;
        // add constructors if they were not defined.

        if ( module->constructor == NULL )
        {
            Method * m = new Method( module->name, module->name );
            module->constructor = m;
            module->methods.push_back(m);
        }
    }
}

void write_header( const char * out_prefix )
{
    char header_file[256];
    sprintf(header_file, "%s.h", out_prefix );
    FILE * out = fopen( header_file, "w");

    if ( out == NULL )
    {
        printf("Cannot open file (%s)\n", header_file );
        exit(1);
    }

    fprintf( out, "#ifndef ADS_%s_H\n", out_prefix);
    fprintf( out, "#define ADS_%s_H\n", out_prefix);
    fprintf( out, "\n");
    fprintf( out, "#ifndef ADS_TM_H\n");
    fprintf( out, "#include \"tm.h\"\n");
    fprintf( out, "#endif\n");
    fprintf( out, "\n");
    fprintf( out, "#ifndef ADS_DB_H\n");
    fprintf( out, "#include \"db.h\"\n");
    fprintf( out, "#endif\n");
    fprintf( out, "\n");
    fprintf( out, "BEGIN_NAMESPACE_ADS\n");
    fprintf( out, "\n");
    fprintf( out, "class ZObject;\n");
    fprintf( out, "\n");

    std::list<Module *>::iterator module_itr;
    for( module_itr = modules.begin(); module_itr != modules.end(); ++module_itr )
    {
        Module * module = * module_itr;
        fprintf( out, "class %s;\n", module->name );
    }

    write_ZInOutArgs_classes( out );
    fprintf( out, "\n");
    fprintf( out, "END_NAMESPACE_ADS\n");
    fprintf( out, "\n#endif\n");
    fclose(out);
}

void write_cpp( const char * out_prefix )
{
    char cpp_file[256];
    sprintf(cpp_file, "%s.cpp", out_prefix );

    FILE * out = fopen( cpp_file, "w");

    if ( out == NULL )
    {
        printf("Cannot open file (%s)\n", cpp_file );
        exit(1);
    }

    fprintf( out, "#include <tcl.h>\n");
    fprintf( out, "#include \"tm.h\"\n");
    fprintf( out, "#include \"tm_bind.h\"\n");
    fprintf( out, "#include \"tm.hpp\"\n");
    fprintf( out, "#include \"tm_bind.hpp\"\n");
    fprintf( out, "#include \"%s.h\"\n", out_prefix);
    fprintf( out, "#include <list>\n");
    fprintf( out, "#include <string>\n");

    write_ccode( out );
    fprintf( out, "\nBEGIN_NAMESPACE_ADS\n");
    write_template_instantations( out );
    write_static_initializations( out );
    write_static_declarations( out );
    write_ZInOutArgs_cpp( out );
    fprintf( out, "\nEND_NAMESPACE_ADS\n");
    fprintf( out, "\nUSING_NAMESPACE_ADS;\n");
    write_ztcl_register_interfaces( out );
    fclose(out);
}

void write_ZInOutArgs_classes( FILE * out )
{
    std::list<Module *>::iterator module_itr;
    for( module_itr = modules.begin(); module_itr != modules.end(); ++module_itr )
    {
        Module * module = * module_itr;
        std::list<Method *>::iterator method_itr;

        for( method_itr = module->methods.begin(); method_itr != module->methods.end(); ++method_itr )
        {
            Method * method = * method_itr;
            write_ZIn_class( out, module, method );
            write_ZOut_class( out, module, method );
        }
    }
}

void write_ZIn_class( FILE * out, Module * module, Method * method )
{
    fprintf( out, "\nclass ZIn_%s_%s : public ZArgs\n", module->name, method->name );
    fprintf( out, "{\n");
    fprintf( out, "public:\n");
    fprintf( out, "    ZIn_%s_%s( const std::vector<ZArg> & args, ZContext & context, const char * vname, const char * sname )\n",
             module->name, method->name );
    fprintf( out, "        : ZArgs( args, context, vname, sname )\n");
    fprintf( out, "    {\n");
    fprintf( out, "    }\n");
    write_in_args( out, method->args, "    ");
    fprintf( out, "};\n");
}

void write_in_args( FILE * out, std::list<Arg *> & args, const char * indent )
{
    std::list<Arg *>::iterator itr;

    for( itr = args.begin(); itr != args.end(); ++itr )
    {
        Arg * a = * itr;

        if ( a->input == false )
            continue;

        fprintf( out, "%s", indent );

        fprintf( out, "bool %sSet() const;\n", a->name );

        switch( a->type )
        {
            case INT_ARG:
                if ( a->list )
                    fprintf( out, "void %s( std::list<int> & l ) const;\n", a->name );
                else
                    fprintf( out, "int %s() const;\n", a->name );
                break;
                
            case FLOAT_ARG:
                if ( a->list )
                    fprintf( out, "void %s( std::list<double> & l ) const;\n", a->name );
                else
                    fprintf( out, "double %s() const;\n", a->name );
                break;
                
            case STRING_ARG:
                if ( a->list )
                    fprintf( out, "void %s( std::list<std::string> & l ) const;\n", a->name );
                else
                    fprintf( out, "const char * %s() const;\n", a->name );
                break;
                
            case BOOL_ARG:
                fprintf( out, "bool %s() const;\n", a->name );
                break;

            case ZOBJECT_ARG:
                if ( a->list )
                    fprintf( out, "void %s( std::list<ZObject*> & l ) const;\n", a->name );
                else
                    fprintf( out, "ZObject * %s() const;\n", a->name );
                break;

            case DBOBJECT_ARG:
                if ( a->list )
                    fprintf( out, "void %s( std::list<dbObject*> & l ) const;\n", a->name );
                else
                    fprintf( out, "dbObject * %s() const;\n", a->name );
                break;

            case ICHANNEL_ARG:
                if ( a->list )
                    fprintf( out, "void %s( std::list<FILE*> & l ) const;\n", a->name );
                else
                    fprintf( out, "FILE * %s() const;\n", a->name );
                break;

            case OCHANNEL_ARG:
                if ( a->list )
                    fprintf( out, "void %s( std::list<FILE*> & l ) const;\n", a->name );
                else
                    fprintf( out, "FILE * %s() const;\n", a->name );
                break;

            case RECT_ARG:
                if ( a->list )
                    fprintf( out, "void %s( std::list<adsRect> & l ) const;\n", a->name );
                else
                    fprintf( out, "const adsRect * %s() const;\n", a->name );
                break;

            case POINT_ARG:
                if ( a->list )
                    fprintf( out, "void %s( std::list<adsPoint> & l ) const;\n", a->name );
                else
                    fprintf( out, "const adsPoint * %s() const;\n", a->name );
                break;
        }
    }
}

void write_ZOut_class( FILE * out, Module * module, Method * method )
{
    fprintf( out, "\nclass ZOut_%s_%s : public ZArgs\n", module->name, method->name );
    fprintf( out, "{\n");
    fprintf( out, "public:\n");
    fprintf( out, "    ZOut_%s_%s( const std::vector<ZArg> & args, ZContext & context )\n",
             module->name, method->name );
    fprintf( out, "        : ZArgs( args, context )\n");
    fprintf( out, "    {\n");
    fprintf( out, "    }\n");
    write_out_args( out, method->args, "    ");
    fprintf( out, "};\n");
}

void write_out_args( FILE * out, std::list<Arg *> & args, const char * indent )
{
    std::list<Arg *>::iterator itr;

    for( itr = args.begin(); itr != args.end(); ++itr )
    {
        Arg * a = * itr;

        if ( a->input == true )
            continue;

        fprintf( out, "%s", indent );

        switch( a->type )
        {
            case INT_ARG:
                if ( a->list )
                    fprintf( out, "void %s( const std::list<int> & l );\n", a->name );
                else
                    fprintf( out, "void %s( int n );\n", a->name );
                break;
                
            case FLOAT_ARG:
                if ( a->list )
                    fprintf( out, "void %s( const std::list<double> & l );\n", a->name );
                else
                    fprintf( out, "void %s( double d );\n", a->name );
                break;
                
            case STRING_ARG:
                if ( a->list )
                    fprintf( out, "void %s( const std::list<std::string> & l );\n", a->name );
                else
                    fprintf( out, "void %s( const char * s );\n", a->name );
                break;
                
            case BOOL_ARG:
                fprintf( out, "void %s( bool b );\n", a->name );
                break;

            case ZOBJECT_ARG:
                if ( a->list )
                    fprintf( out, "void %s( const std::list<ZObject *> & l );\n", a->name );
                else
                    fprintf( out, "void %s( ZObject * );\n", a->name );
                break;

            case DBOBJECT_ARG:
                if ( a->list )
                    fprintf( out, "void %s( const std::list<dbObject *> & l );\n", a->name );
                else
                    fprintf( out, "void %s( dbObject * );\n", a->name );
                break;

            case ICHANNEL_ARG:
            case OCHANNEL_ARG:
                break;

            case RECT_ARG:
                if ( a->list )
                    fprintf( out, "void %s( const std::list<adsRect> & l );\n", a->name );
                else
                    fprintf( out, "void %s( const adsRect & r );\n", a->name );
                break;

            case POINT_ARG:
                if ( a->list )
                    fprintf( out, "void %s( const std::list<adsPoint> & l );\n", a->name );
                else
                    fprintf( out, "void %s( adsPoint p );\n", a->name );
                break;
        }
    }
}

void write_ZInOutArgs_cpp( FILE * out )
{
    std::list<Module *>::iterator module_itr;
    for( module_itr = modules.begin(); module_itr != modules.end(); ++module_itr )
    {
        Module * module = * module_itr;
        std::list<Method *>::iterator method_itr;

        for( method_itr = module->methods.begin(); method_itr != module->methods.end(); ++method_itr )
        {
            Method * method = * method_itr;
            write_ZIn_cpp( out, module, method );
            write_ZOut_cpp( out, module, method );
        }
    }
}

void write_ZIn_cpp( FILE * out, Module * module, Method * method )
{
    int i = 0;
    std::list<Arg *>::iterator itr;

    for( itr = method->args.begin(); itr != method->args.end(); ++itr )
    {
        Arg * a = * itr;

        if ( a->input == false )
            continue;

        fprintf( out, "\nbool ZIn_%s_%s::%sSet() const\n", module->name, method->name, a->name );
        fprintf( out, "{\n");
        fprintf( out, "    return isSet( %d );\n", i);
        fprintf( out, "}\n");

        switch( a->type )
        {
            case INT_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZIn_%s_%s::%s( std::list<int> & l ) const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    getInt( %d, l );\n", i);
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nint ZIn_%s_%s::%s() const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    return getInt(%d);\n", i );
                    fprintf( out, "}\n");
                }
                break;
                
            case FLOAT_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZIn_%s_%s::%s( std::list<double> & l ) const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    getFloat( %d, l );\n", i);
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\ndouble ZIn_%s_%s::%s() const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    return getFloat(%d);\n", i );
                    fprintf( out, "}\n");
                }
                break;
                
            case STRING_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZIn_%s_%s::%s( std::list<std::string> & l ) const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    getString( %d, l );\n", i);
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nconst char * ZIn_%s_%s::%s() const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    const char * s = getValue(%d);\n", i );
                    fprintf( out, "    if ( *s == \'\\0\' ) return NULL;\n" );
                    fprintf( out, "    return s;\n" );
                    fprintf( out, "}\n");
                }
                break;
                
            case BOOL_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZIn_%s_%s::%s( std::list<std::bool> & l ) const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    getBool( %d, l );\n", i);
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nbool ZIn_%s_%s::%s() const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    return getBool(%d);\n", i );
                    fprintf( out, "}\n");
                }
                break;

            case ZOBJECT_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZIn_%s_%s::%s( std::list<ZObject *> & l ) const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    getZObject( %d, l );\n", i);
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nZObject * ZIn_%s_%s::%s() const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    return getZObject(%d);\n", i );
                    fprintf( out, "}\n");
                }
                break;

            case DBOBJECT_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZIn_%s_%s::%s( std::list<dbObject *> & l ) const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    getDbObject( %d, l );\n", i);
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\ndbObject * ZIn_%s_%s::%s() const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    return getDbObject( %d );\n", i);
                    fprintf( out, "}\n");
                }
                break;

            case ICHANNEL_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZIn_%s_%s::%s( std::list<FILE *> & l ) const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    getIChannel( %d, l );\n", i);
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nFILE * ZIn_%s_%s::%s() const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    return getIChannel( %d );\n", i);
                    fprintf( out, "}\n");
                }
                break;

            case OCHANNEL_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZIn_%s_%s::%s( std::list<FILE *> & l ) const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    getOChannel( %d, l );\n", i);
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nFILE * ZIn_%s_%s::%s() const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    return getOChannel( %d );\n", i);
                    fprintf( out, "}\n");
                }
                break;

            case RECT_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZIn_%s_%s::%s( std::list<adsRect> & l ) const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    getRect( %d, l );\n", i);
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nconst adsRect *ZIn_%s_%s::%s() const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    static adsRect rtrct;\n");
		    fprintf( out, "    if (getRect(%d))\n", i );
		    fprintf( out, "    {\n");
                    fprintf( out, "        rtrct = *(getRect(%d));\n", i );
                    fprintf( out, "        return &rtrct;\n");
		    fprintf( out, "    }\n");
                    fprintf( out, "    return NULL;\n");
                    fprintf( out, "}\n");
                }
                break;

            case POINT_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZIn_%s_%s::%s( std::list<adsRect> & l ) const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    getPoint( %d, l );\n", i);
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nconst adsPoint *ZIn_%s_%s::%s() const\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    static adsPoint rtpt;\n");
		    fprintf( out, "    if (getPoint(%d))\n", i );
		    fprintf( out, "    {\n");
                    fprintf( out, "        rtpt = *(getPoint(%d));\n", i );
                    fprintf( out, "        return &rtpt;\n");
		    fprintf( out, "    }\n");
                    fprintf( out, "    return NULL;\n");
                    fprintf( out, "}\n");
                }
                break;
        }

        ++i;
    }

    fprintf( out, "\nZArgs * ZIn_%s_%s_getTclArgs( const std::vector<ZArg> & args, ZContext & context, const char * vname, const char * sname )\n",
             module->name, method->name );
    fprintf( out, "{\n");
    fprintf( out, "    ZIn_%s_%s * zargs = new ZIn_%s_%s(args, context, vname, sname);\n",
             module->name, method->name, module->name, method->name );
    fprintf( out, "    if ( zargs == NULL ) throw ZOutOfMemory();\n");
    fprintf( out, "    return zargs;\n");
    fprintf( out, "}\n");
}

void write_ZOut_cpp( FILE * out, Module * module, Method * method )
{
    int i = 0;
    std::list<Arg *>::iterator itr;

    for( itr = method->args.begin(); itr != method->args.end(); ++itr )
    {
        Arg * a = * itr;

        if ( a->input == true )
            continue;

        switch( a->type )
        {
            case INT_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( const std::list<int> & l )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setInt(%d, l);\n", i );
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( int n )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setInt(%d, n);\n", i );
                    fprintf( out, "}\n");
                }
                break;
                
            case FLOAT_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( const std::list<double> & l )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setFloat(%d, l);\n", i );
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( double n )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setFloat(%d, n);\n", i );
                    fprintf( out, "}\n");
                }
                break;
                
            case STRING_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( const std::list<std::string> & l )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setString(%d, l);\n", i );
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( const char * s )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setString(%d, s);\n", i );
                    fprintf( out, "}\n");
                }
                break;
                
            case BOOL_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( const std::list<bool> & l )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setBool(%d, l);\n", i );
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( bool b )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setBool(%d, b);\n", i );
                    fprintf( out, "}\n");
                }
                
                break;

            case ZOBJECT_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( const std::list<ZObject *> & l )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setZObject( %d, l );\n", i );
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( ZObject * o )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setZObject( %d, o );\n", i );
                    fprintf( out, "}\n");
                }
                break;

            case DBOBJECT_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( const std::list<dbObject *> & l )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setDbObject( %d, l );\n", i );
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( dbObject * o )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setDbObject( %d, o );\n", i );
                    fprintf( out, "}\n");
                }
                break;

            case RECT_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( const std::list<adsRect> & l )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setRect(%d, l);\n", i );
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( const adsRect & r )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setRect(%d, r);\n", i );
                    fprintf( out, "}\n");
                }
                break;

            case POINT_ARG:
                if ( a->list )
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( const std::list<adsPoint> & l )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setPoint(%d, l);\n", i );
                    fprintf( out, "}\n");
                }
                else
                {
                    fprintf( out, "\nvoid ZOut_%s_%s::%s( adsPoint p )\n", module->name, method->name, a->name );
                    fprintf( out, "{\n");
                    fprintf( out, "    setPoint(%d, p);\n", i );
                    fprintf( out, "}\n");
                }
                break;

            default:
                break;
        }
        ++i;
    }

    fprintf( out, "\nZArgs * ZOut_%s_%s_getTclArgs( const std::vector<ZArg> & args, ZContext & context )\n",
             module->name, method->name );
    fprintf( out, "{\n");
    fprintf( out, "    ZOut_%s_%s * zargs = new ZOut_%s_%s(args, context);\n",
             module->name, method->name, module->name, method->name );
    fprintf( out, "    if ( zargs == NULL ) throw ZOutOfMemory();\n");
    fprintf( out, "    return zargs;\n");
    fprintf( out, "}\n");
}

void write_ccode( FILE * out )
{
    fprintf( out, "\n"); 
    std::list<const char *>::iterator citr;
    for( citr = ccode.begin(); citr != ccode.end(); ++citr )
    {
        const char * line = * citr;
        fprintf( out, "%s\n", line);
    }
}
    
void write_template_instantations( FILE * out )
{
    fprintf( out, "\n");

    std::list<Module *>::iterator itr;
    for( itr = modules.begin(); itr != modules.end(); ++itr )
    {
        Module * m = * itr;
        fprintf( out, "template class ZTechModule<%s>;\n", m->name );
        fprintf( out, "template class ZMethod<%s>;\n", m->name );
        fprintf( out, "template class ZConfig<%s>;\n", m->name );
        fprintf( out, "template class ZBindings<%s>;\n", m->name );
    }
}

void write_static_initializations( FILE * out )
{
    fprintf( out, "\n");

    std::list<Module *>::iterator itr;
    for( itr = modules.begin(); itr != modules.end(); ++itr )
    {
        Module * m = * itr;
        fprintf( out, "template<> ZBindings<%s> * ZTechModule<%s>::_bindings = NULL;\n", m->name, m->name );
        fprintf( out, "template<> const char * ZTechModule<%s>::_module = \"%s\";\n", m->name, m->name );
    }
}

void write_static_declarations( FILE * out )
{
    fprintf( out, "\n");

    std::list<Module *>::iterator module_itr;
    for( module_itr = modules.begin(); module_itr != modules.end(); ++module_itr )
    {
        Module * module = * module_itr;

        std::list<Method *>::iterator method_itr;
        for( method_itr = module->methods.begin(); method_itr != module->methods.end(); ++method_itr )
        {
            Method * method = * method_itr;
            fprintf( out, "static ZArgs * ZOut_%s_%s_getTclArgs( const std::vector<ZArg> & args, ZContext & context );\n",
                     module->name, method->name );
            fprintf( out, "static ZArgs * ZIn_%s_%s_getTclArgs( const std::vector<ZArg> & , ZContext & , const char * , const char * );\n",
                     module->name, method->name );
        }
    }
}

void write_ztcl_register_interfaces( FILE * out )
{
    std::list<Module *>::iterator itr;
    for( itr = modules.begin(); itr != modules.end(); ++itr )
    {
        Module * m = * itr;
        write_ztcl_register_interface( out, m );
    }
}

void write_ztcl_register_interface( FILE * out, Module * module )
{
    fprintf( out, "\n");
    fprintf( out, "void ZTCL_RegisterInterface_%s( Tcl_Interp * interp )\n", module->name );
    fprintf( out, "{\n");
    fprintf( out, "    if ( Tcl_CreateCommand( interp, \"%s::%s%s\", &%s::construct, NULL, NULL ) == NULL )\n",
             znamespace, module->zname, zpostfix, module->name );
    fprintf( out, "    {\n");
    fprintf( out, "        // TODO issue and error or something\n");
    fprintf( out, "    }\n");
    fprintf( out, "\n");
    fprintf( out, "    if ( Tcl_CreateCommand( interp, \"%s::%s%s_usage\", &%s::usage, NULL, NULL ) == NULL )\n",
             znamespace, module->zname, zpostfix, module->name );
    fprintf( out, "    {\n");
    fprintf( out, "        // TODO issue and error or something\n");
    fprintf( out, "    }\n");
    fprintf( out, "\n");
    fprintf( out, "    if ( ZTechModule<%s>::_bindings != NULL ) return;\n", module->name );
    fprintf( out, "\n");
    fprintf( out, "    ZBindings<%s> * bindings = new ZBindings<%s>;\n", module->name, module->name );
    fprintf( out, "    if ( bindings == NULL )\n" );
    fprintf( out, "        throw ZOutOfMemory();\n");
    

    std::list<Method *>::iterator itr;
    for( itr = module->methods.begin(); itr != module->methods.end(); ++itr )
    {
        Method * m = * itr;
        write_method_bindings_cpp(out, module, m);
    }

    std::list<Var *>::iterator vitr;
    for( vitr = module->vars.begin(); vitr != module->vars.end(); ++vitr )
    {
        Var * v = * vitr;
        write_var_config_cpp(out, module, v);
    }

    fprintf( out, "\n");
    fprintf( out, "    ZTechModule<%s>::_bindings = bindings;\n", module->name );
    fprintf( out, "    std::string doc(\"\");\n");

    std::list<std::string>::iterator ditr;
    for( ditr = module->doc.begin(); ditr != module->doc.end(); ++ditr )
    {
        std::string & l = *ditr;
        fprintf( out, "    doc += \"%s\\n\";\n", l.c_str() );
    }

    fprintf( out, "    ZTechModuleMgr::module_doc( \"%s\", doc.c_str());\n", module->zname );

    for( itr = module->methods.begin(); itr != module->methods.end(); ++itr )
    {
        Method * m = * itr;

        fprintf( out, "    doc = \"\";\n");

        std::list<std::string>::iterator ditr;
        for( ditr = m->doc.begin(); ditr != m->doc.end(); ++ditr )
        {
            std::string & l = *ditr;
            fprintf( out, "    doc += \"%s\\n\";\n", l.c_str() );
        }

        fprintf( out, "    ZTechModuleMgr::method_doc( \"%s\", \"%s\", doc.c_str());\n", module->zname, m->zcmd );
    }

    for( vitr = module->vars.begin(); vitr != module->vars.end(); ++vitr )
    {
        Var * v = * vitr;

        fprintf( out, "    doc = \"\";\n");

        std::list<std::string>::iterator ditr;
        for( ditr = v->doc.begin(); ditr != v->doc.end(); ++ditr )
        {
            std::string & l = *ditr;
            fprintf( out, "    doc += \"%s\\n\";\n", l.c_str() );
        }

        fprintf( out, "    ZTechModuleMgr::variable_doc( \"%s\", \"%s\", doc.c_str());\n", module->zname, v->name );
    }
    
    fprintf( out, "}\n");
}

void write_method_bindings_cpp( FILE * out, Module * module, Method * method )
{
    if ( method == module->constructor )
        fprintf( out, "    ZMethod<%s> * method_%s = bindings->bindMethod( \"%s\", NULL );\n",
                 module->name, module->name, module->name);
    else
        fprintf( out, "    ZMethod<%s> * method_%s = bindings->bindMethod( \"%s\", &%s::%s );\n",
                 module->name, method->name, method->zcmd, module->name, method->name );

    fprintf( out, "    method_%s->_get_in_args = &ZIn_%s_%s_getTclArgs;\n", method->name, module->name, method->name );
    fprintf( out, "    method_%s->_get_out_args = &ZOut_%s_%s_getTclArgs;\n", method->name, module->name, method->name );

    std::list<Arg *>::iterator itr;

    for( itr = method->args.begin(); itr != method->args.end(); ++itr )
    {
        Arg * a = * itr;

        if ( a->input )
            fprintf( out, "    method_%s->addInArg", method->name );
        else
            fprintf( out, "    method_%s->addOutArg", method->name );

        const char * usage = "";
        
        if ( a->usage )
            usage = a->usage;

        switch( a->type )
        {
            case INT_ARG:
            {
                const char * value = "0";
                if ( a->value )
                    value = a->value;
                if ( a->list )
                    fprintf( out, "( ZARG_INT, true, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                else
                    fprintf( out, "( ZARG_INT, false, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                break;
            }
                
            case FLOAT_ARG:
            {
                const char * value = "0.0";
                if ( a->value )
                    value = a->value;
                if ( a->list )
                    fprintf( out, "( ZARG_FLOAT, true, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                else
                    fprintf( out, "( ZARG_FLOAT, false, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                break;
            }
                
            case STRING_ARG:
            {
                const char * value = "";
                if ( a->value )
                    value = a->value;
                if ( a->list )
                    fprintf( out, "( ZARG_STRING, true, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                else
                    fprintf( out, "( ZARG_STRING, false, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                break;
            }
                
            case BOOL_ARG:
            {
                const char * value = "false";
                if ( a->value )
                    value = a->value;
                fprintf( out, "( ZARG_BOOL, false, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                break;
            }

            case ZOBJECT_ARG:
            {
                const char * value = "";
                if ( a->value )
                    value = a->value;
                if ( a->list )
                    fprintf( out, "( ZARG_ZOBJECT, true, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                else
                    fprintf( out, "( ZARG_ZOBJECT, false, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                break;
            }

            case DBOBJECT_ARG:
            {
                const char * value = "";
                if ( a->value )
                    value = a->value;
                if ( a->list )
                    fprintf( out, "( ZARG_DBOBJECT, true, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                else
                    fprintf( out, "( ZARG_DBOBJECT, false, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                break;
            }
            case ICHANNEL_ARG:
            {
                const char * value = "";
                if ( a->value )
                    value = a->value;
                if ( a->list )
                    fprintf( out, "( ZARG_ICHANNEL, true, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                else
                    fprintf( out, "( ZARG_ICHANNEL, false, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                break;
            }
            case OCHANNEL_ARG:
            {
                const char * value = "";
                if ( a->value )
                    value = a->value;
                if ( a->list )
                    fprintf( out, "( ZARG_OCHANNEL, true, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                else
                    fprintf( out, "( ZARG_OCHANNEL, false, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                break;
            }

            case RECT_ARG:
            {
                const char * value = "";
                if ( a->value )
                    value = a->value;
                if ( a->list )
                    fprintf( out, "( ZARG_RECT, true, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                else
                    fprintf( out, "( ZARG_RECT, false, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                break;
            }

            case POINT_ARG:
            {
                const char * value = "";
                if ( a->value )
                    value = a->value;
                if ( a->list )
                    fprintf( out, "( ZARG_POINT, true, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                else
                    fprintf( out, "( ZARG_POINT, false, \"%s\", \"%s\", \"%s\" );\n", a->name, value, usage );
                break;
            }
        }
    }
}

void write_var_config_cpp( FILE * out, Module * module, Var * var )
{
    const char * usage = "";
    
    if ( var->usage )
        usage = var->usage;

    fprintf( out, "    ZConfig<%s> * var_%s = bindings->bindConfig( \"%s\", \"%s\" );\n",
             module->name, var->name, var->name, usage );

    if ( var->notify == NULL )
        fprintf( out, "    var_%s->setMember( &%s::%s );\n",
                 var->name, module->name, var->name );
    else
        fprintf( out, "    var_%s->setMember( &%s::%s, &%s::%s );\n",
                 var->name, module->name, var->name, module->name, var->notify );
}

