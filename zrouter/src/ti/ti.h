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

#include <list>
#include <string>

struct Module;
struct Method;
struct Arg;
struct Var;

struct Group;

enum ArgType
{
    INT_ARG,
    FLOAT_ARG,
    STRING_ARG,
    BOOL_ARG,
    DBOBJECT_ARG,
    ZOBJECT_ARG,
    ICHANNEL_ARG,
    OCHANNEL_ARG,
    RECT_ARG,
    POINT_ARG
};

struct Module
{
    const char *        name;
    const char *        zname;
    std::list<Method *> methods;
    std::list<Var *>    vars;
    Method *            constructor;
    std::list<std::string> doc;

    Module( const char * n, const char * zn )
    {
        name = n;
        zname = zn;
        constructor = NULL;
    }
};

struct Member
{
    std::list<std::string> doc;
};

struct Method : public Member
{
    const char *       name;
    const char *       zcmd;
    std::list<Arg *>   args;
    Method( const char * n, const char * c ) { name = n; zcmd = c; }
};

struct Arg
{
    bool               list;
    bool               input;
    ArgType            type;
    const char *       name;
    const char *       value;
    const char *       usage;

    Arg( bool i, bool l, ArgType t, const char * n )
    {
        list = l;
        input = i;
        type = t;
        name = n;
        value = NULL;
        usage = NULL;
    }
};

struct Var : public Member
{
    bool               list;
    ArgType            type;
    const char *       name;
    const char *       usage;
    const char *       notify;

    Var( bool l, ArgType t, const char * n )
    {
        list = l;
        type = t;
        name = n;
        usage = NULL;
        notify = NULL;
    }
};

struct ArgValue
{
    int          type;
    const char * value;
};

extern std::list<Module *> modules;
extern std::list<const char *> ccode;
extern std::list<std::string> doc;

