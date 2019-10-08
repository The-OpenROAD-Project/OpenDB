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
#include <string>
#include "ti.h"
#include "ti_parser.hpp"

#ifdef TIDEBUG
#define TI_DEBUG(msg) do { if ( 1 ) printf(msg, yytext); } while(0)
#else
#define TI_DEBUG(msg)
#endif

extern "C" int yywrap();
int yywrap() { return 1; }
int Tilex_lineno = 1;
char Ticcode[8192];
extern const char * Tiinfile;
extern void Tierror( const char * msg );
static char * ccodeptr;
static char * ccodeend = &Ticcode[8192];
std::list<std::string> doc;
static std::string docline;

void addCCode()
{
    int l = strlen(Titext);

    if ( (ccodeptr == ccodeend) || ((ccodeptr + l + 1) >= ccodeend) )
    {
        printf("%s:%d buffer overflow on token (%s)\n", Tiinfile, Tilex_lineno, Ticcode );
        exit(1);
    }
    strcpy( ccodeptr, Titext );
    ccodeptr += l;
}

%}

newline    \n
whitespace [ \t\r\f\b]+
comment     #[^\n\r]*
qstring    \"[^\"\n]*[\"\n]
identifier [a-zA-Z_$][a-zA-Z_0-9$]*
hex        [-+]?0(x|X)([0-9]|[a-fA-F])+
oct        [-+]?00[0-7]+
integer    [-+]?[0-9]+
exponent   [eE][-+]?[0-9]+
float      [-+]?([0-9]+|([0-9]*\.[0-9]+))({exponent})?

%x CCODE
%x CCODE_RESET
%x DOC_COMMENT
%x DOC_LINE

%%

<CCODE_RESET>.           { BEGIN CCODE; ccodeptr = Ticcode; *ccodeptr = 0; unput(Titext[0]); }
<CCODE>{newline}         { ++Tilex_lineno; 
                           if ( Ticcode[0] != 0 && Ticcode[0] != '\n' ) 
                           {
                               BEGIN CCODE_RESET; 
                               return CCODE_LINE; 
                           }
                         }

<CCODE>{whitespace}      { addCCode(); }
<CCODE>"%}"              { 
                           TI_DEBUG("PUNCT (%s)\n"); 
                           if ( Ticcode[0] != 0 && Ticcode[0] != '\n' ) 
                           {
                               unput('}');
                               unput('%');
                               BEGIN CCODE_RESET; 
                               return CCODE_LINE;
                           }
                           BEGIN 0; return CCODE_END; 
                         }
<CCODE>.                 { TI_DEBUG("CCODE (%s)\n"); addCCode(); }

^{whitespace}*"/**"            { BEGIN DOC_COMMENT; doc.clear(); }

<DOC_COMMENT>"*/"                    { BEGIN 0; if ( ! doc.empty() ) return DOC_K; }
<DOC_COMMENT>{whitespace}"*"[^/]     { BEGIN DOC_LINE; docline.clear(); }
<DOC_COMMENT>{newline}               { ++Tilex_lineno; }
<DOC_COMMENT>.                       { }
<DOC_COMMENT><<EOF>>                 { Tierror("unterminated comment"); exit(1); }

<DOC_LINE>{newline}       { BEGIN DOC_COMMENT; if ( ! docline.empty() ) doc.push_back(docline); ++Tilex_lineno; }
<DOC_LINE>"*/"            { BEGIN 0; if ( ! docline.empty() ) doc.push_back(docline); if ( ! doc.empty() ) return DOC_K; }
<DOC_LINE>"\""            { docline.push_back('\\'); docline.push_back('"'); }
<DOC_LINE>.               { docline.push_back(Titext[0]); }
<DOC_LINE><<EOF>>         { Tierror("unterminated comment"); exit(1); }

{newline}     { ++Tilex_lineno; }
{whitespace}  { }
{comment}     { TI_DEBUG("COMMENT (%s)\n"); }
{qstring}     { TI_DEBUG("QSTRING (%s)\n"); return QSTRING; }
{hex}         { TI_DEBUG("HEX (%s)\n"); return NUM_INT; }
{oct}         { TI_DEBUG("OCT (%s)\n"); return NUM_INT; }
{integer}     { TI_DEBUG("INT (%s)\n"); return NUM_INT; }
{float}       { TI_DEBUG("FLT (%s)\n"); return NUM_DOUBLE; }
in            { TI_DEBUG("KEY (%s)\n"); return IN_K; }
out           { TI_DEBUG("KEY (%s)\n"); return OUT_K; }
module        { TI_DEBUG("KEY (%s)\n"); return MODULE_K; }
int           { TI_DEBUG("KEY (%s)\n"); return INT_K; }
float         { TI_DEBUG("KEY (%s)\n"); return FLOAT_K; }
string        { TI_DEBUG("KEY (%s)\n"); return STRING_K; }
bool          { TI_DEBUG("KEY (%s)\n"); return BOOL_K; }
true          { TI_DEBUG("KEY (%s)\n"); return TRUE_K; }
false         { TI_DEBUG("KEY (%s)\n"); return FALSE_K; }
usage         { TI_DEBUG("KEY (%s)\n"); return USAGE_K; }
export        { TI_DEBUG("KEY (%s)\n"); return EXPORT_K; }
constructor   { TI_DEBUG("KEY (%s)\n"); return CONSTRUCTOR_K; }
dbobject      { TI_DEBUG("KEY (%s)\n"); return DBOBJECT_K; }
zobject       { TI_DEBUG("KEY (%s)\n"); return ZOBJECT_K; }
ichannel      { TI_DEBUG("KEY (%s)\n"); return ICHANNEL_K; }
ochannel      { TI_DEBUG("KEY (%s)\n"); return OCHANNEL_K; }
list          { TI_DEBUG("KEY (%s)\n"); return LIST_K; }
notify        { TI_DEBUG("KEY (%s)\n"); return NOTIFY_K; }
rect          { TI_DEBUG("KEY (%s)\n"); return RECT_K; }
point         { TI_DEBUG("KEY (%s)\n"); return POINT_K; }
"%{"          { TI_DEBUG("PUNCT (%s)\n"); BEGIN CCODE; ccodeptr = Ticcode; *ccodeptr = 0; return CCODE_BEGIN; }
"="           { TI_DEBUG("PUNCT (%s)\n"); return '='; }
","           { TI_DEBUG("PUNCT (%s)\n"); return ','; }
";"           { TI_DEBUG("PUNCT (%s)\n"); return ';'; }
"("           { TI_DEBUG("PUNCT (%s)\n"); return '('; }
")"           { TI_DEBUG("PUNCT (%s)\n"); return ')'; }
"{"           { TI_DEBUG("PUNCT (%s)\n"); return '{'; }
"}"           { TI_DEBUG("PUNCT (%s)\n"); return '}'; }
"["           { TI_DEBUG("PUNCT (%s)\n"); return '['; }
"]"           { TI_DEBUG("PUNCT (%s)\n"); return ']'; }
{identifier}  { TI_DEBUG("ID (%s)\n"); return ID; }
.             { fprintf(stderr, "%s:%d unknown token found (%s)\n", Tiinfile, Tilex_lineno, Titext); }

%%
