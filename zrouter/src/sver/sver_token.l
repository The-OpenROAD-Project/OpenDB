/*
 BSD 3-Clause License

 Copyright (c) 2019, The Regents of the University of California
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

%{
#include <stdlib.h>
#include "sver.h"
#include "sver_parser.h"
#ifdef WIN32
#include <io.h>
#endif

/*int sver_parser_lineno; */

%}

%option noyywrap

newline    \n
whitespace [ \t\r\f\b]+
identifier [\\a-zA-Z_$][\/\.a-zA-Z_0-9$\[\]]*
dotid      \.[\\a-zA-Z_$][\/\.a-zA-Z_0-9$\[\]]*
trail_comment    \/\/[^\n\r]*
start_com   \/\*
end_com     \*\/
busrange   \[[0-9]+:[0-9]+\]
conid      1\'b[0-9]
other      [^ \t\r\f\b\n]+

%s COMMENT

%%

{newline}     { ++sver_parser_lineno; }
{whitespace}  { }
<INITIAL>{start_com}        { BEGIN COMMENT; }
<INITIAL>{trail_comment}    { }
<INITIAL>"("           { return OPEN; }
<INITIAL>")"           { return CLOSE; }
<INITIAL>"{"           { return CURLY; }
<INITIAL>"}"           { return CLOSE_CURLY; }
<INITIAL>","           { return COMMA; }
<INITIAL>";"           { return SEMI; }
<INITIAL>"="           { return EQ; }
<INITIAL>assign        { return ASSIGN; }
<INITIAL>module        { return MODULE; }
<INITIAL>endmodule     { return ENDMODULE; }
<INITIAL>input         { return INPUT; }
<INITIAL>inout         { return INOUT; }
<INITIAL>output        { return OUTPUT; }
<INITIAL>wire          { return WIRE; }
<INITIAL>supply1       { return WIRE; }
<INITIAL>{busrange}    { return BUSRANGE; }
<INITIAL>{identifier}  { return ID; }
<INITIAL>{dotid}       { return DOTID; }
<INITIAL>{conid}       { return CONID; }

<COMMENT>{end_com}     { BEGIN INITIAL; }
<COMMENT>{other}       { }

%%
