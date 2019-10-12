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
#include "definTypes.h"
#include <vector>
#include "def_parser.hpp"
#include "hash.h"
#include "def.h"
#ifdef WIN32
#include <io.h>
#endif

extern YYSTYPE deflval;
int deflex_kid = 0;
bool deflex_casesens = true;
int deflex_linecnt = 1000000;

static hashTable keywords;
int defdebug;
#ifdef YYDEBUG
#define DEF_DEBUG(msg) do { if ( defdebug > 1 ) printf(msg, yytext); } while(0)
#else
#define DEF_DEBUG(msg)
#endif

static bool isNetPathKey( int type )
{
    switch(type)
    {
        case NEW_K:
        case N_K:
        case S_K:
        case E_K:
        case W_K:
        case FN_K:
        case FS_K:
        case FE_K:
        case FW_K:
            return true;
    }

    return false;
}

inline int id()
{
    ///
    /// A DEF an identifier is anything delimeted by white-space, with the exception of comments (#...) and quoted-strings.
    /// To make matters worse, a number can also be an identifier. For the most part, the DEF grammer allows the parser to 
    /// disambiguate numbers and identifies, in as such, the grammer does not reduce to a identifier and a number.
    /// Thus a DEF identifier is defined as (see gramer above):
    ///
    ///     ident : IDENT | NUM_INT | NUM_DOUBLE ;
    ///
    /// Because an identfier is anything, a keyword can also be an identifier and the parser must disambiuate between a keyword
    /// and an identifier. Antlr provides an exception mechanism that is employed to disambiguate between identifiers and
    /// kewords (literals). Ninty-nine percent of the grammer does not reduce co-currenently to a keyword and an identifier.
    /// When a keyword is encountered which should be an identifier, the parser generates an "error". The ident is then
    /// declared as:
    /// 
    ///     ident : IDENT | NUM_INT | NUM_DOUBLE | error { check-for-a-keyword } ;
    /// 
    /// If a parse error occurs then the token is checked for a keyword, if a keyword is found then parsing confinues as normal.
    /// 
    if ( yytext[1] == 0 )
    {
        if ( yytext[0] == ';' )
        {
            DEF_DEBUG("PUNCT (%s)\n");
            return( ';' );
        }
        else if ( yytext[0] == '(' )
        {
            DEF_DEBUG("PUNCT (%s)\n");
            return( '(' );
        }
        else if ( yytext[0] == ')' )
        {
            DEF_DEBUG("PUNCT (%s)\n");
            return( ')' );
        }
        else if ( yytext[0] == '+' )
        {
            DEF_DEBUG("PUNCT (%s)\n");
            return( '+' );
        }
        else if ( yytext[0] == '-' )
        {
            DEF_DEBUG("PUNCT (%s)\n");
            return( '-' );
        }
        else if ( yytext[0] == '*' )
        {
            DEF_DEBUG("PUNCT (%s)\n");
            return( '*' );
        }
    }

    if ( deflex_casesens == false )
    {
        register char * p;
        register int c;
        for( p = yytext; (c = *p) != '\0'; ++p )
        {
            if ( (c >= 'a') && (c <= 'z') )
                *p = c - 'a' + 'A';
        }
    }

    switch( deflex_kid )
    {
        case 0:
        {
            int type;
            if ( keywords.find( yytext, type ) )
            {
                DEF_DEBUG("KEYWORD (%s)\n");
                return type;
            }

            break;
        }
        case NET_PATH_ID:
        {
            int type;
            if ( keywords.find( yytext, type ) && isNetPathKey(type) )
            {
                DEF_DEBUG("KEYWORD (%s)\n");
                return type;
            }
            break;
        }
        case SNET_PATH_ID:
        {
            if ( strcmp( yytext, "NEW" ) == 0 )
            {
                DEF_DEBUG("KEYWORD (%s)\n");
                return NEW_K;
            }
            else if ( strcmp( yytext, "DO" ) == 0 )
            {
                DEF_DEBUG("KEYWORD (%s)\n");
                return DO_K;
            }
            break;
        }
        case NET_ID:
        {
            if ( strcmp( yytext, "MUSTJOIN" ) == 0 )
            {
                DEF_DEBUG("KEYWORD (%s)\n");
                return MUSTJOIN_K;
            }
            break;
        }
    }
    
    DEF_DEBUG("IDENT (%s)\n");
    return IDENT;
}

extern "C" int yywrap();
int yywrap() { return 1; }
int deflex_lineno = 1;
%}

newline    \n
whitespace [ \t\r\f\b]+
comment    #[^\n\r]*
qstring    \"[^\"\n]*[\"\n]
identifier [^\"# \n\r\t\f\b][^ \n\r\t\f\b]*
hex        [-+]?0(x|X)([0-9]|[a-fA-F])+
oct        [-+]?00[0-7]+
integer    [-+]?[0-9]+
exponent   [eE][-+]?[0-9]+
float      [-+]?([0-9]+|([0-9]*\.[0-9]+))({exponent})?

%x HISTORY
%x EXTENSION
%%

<HISTORY>{newline}     { ++deflex_lineno; if ( (deflex_lineno % deflex_linecnt) == 0 ) defparse_linecnt(); }
<HISTORY>{whitespace}  { }
<HISTORY>[^ \t\n\r\f\b]* { 
                           DEF_DEBUG("HISTORY TEXT (%s)\n");
                           if ( yytext[0] == ';' && yytext[1] == 0 ) 
                           {
                               unput(';');
                               BEGIN 0;
                               return HISTORY_TEXT; 
                           }
                         }

<EXTENSION>{newline}     { ++deflex_lineno; if ( (deflex_lineno % deflex_linecnt) == 0 ) defparse_linecnt(); }
<EXTENSION>{whitespace}  { }
<EXTENSION>[^ \t\n\r\f\b]* { if ( strcmp(yytext, "ENDEXT") == 0 ) 
                             {
                                 unput('T'); unput('X'); unput('E'); unput('D'); unput('N'); unput('E');
                                 BEGIN 0;
                             }
                             else 
                             {
                                 DEF_DEBUG("EXT-TOKEN (%s)\n");
                             }
                           }
{newline}     { ++deflex_lineno; if ( (deflex_lineno % deflex_linecnt) == 0 ) defparse_linecnt(); }
{whitespace}  { }
{comment}     { DEF_DEBUG("COMMENT (%s)\n"); }
{qstring}     { DEF_DEBUG("QSTRING (%s)\n"); return QSTRING; }
{hex}         { DEF_DEBUG("HEX (%s)\n"); deflval._int = strtol( yytext, NULL, 16 ); return NUM_INT; }
{oct}         { DEF_DEBUG("OCT (%s)\n"); deflval._int = strtol( yytext, NULL, 8  ); return NUM_INT; }
{integer}     { DEF_DEBUG("INT (%s)\n"); deflval._int = strtol( yytext, NULL, 10 ); return NUM_INT; }
{float}       { DEF_DEBUG("FLT (%s)\n"); deflval._double = strtod( yytext, NULL ); return NUM_DOUBLE; }
{identifier}  { return id(); }

%%

void deflex_init()
{
    static bool init_keywords = false;

    if ( init_keywords == false )
    {
        keywords.setSize(512);
        keywords.insert( "ANALOG", ANALOG_K );
        keywords.insert( "ANTENNAMODEL", ANTENNAMODEL_K );
        keywords.insert( "ANTENNAPINDIFFAREA", ANTENNAPINDIFFAREA_K );
        keywords.insert( "ANTENNAPINGATEAREA", ANTENNAPINGATEAREA_K );
        keywords.insert( "ANTENNAPINMAXAREACAR", ANTENNAPINMAXAREACAR_K );
        keywords.insert( "ANTENNAPINMAXCUTCAR", ANTENNAPINMAXCUTCAR_K );
        keywords.insert( "ANTENNAPINMAXSIDEAREACAR", ANTENNAPINMAXSIDEAREACAR_K );
        keywords.insert( "ANTENNAPINPARTIALCUTAREA", ANTENNAPINPARTIALCUTAREA_K );
        keywords.insert( "ANTENNAPINPARTIALMETALAREA", ANTENNAPINPARTIALMETALAREA_K );
        keywords.insert( "ANTENNAPINPARTIALMETALSIDEAREA", ANTENNAPINPARTIALMETALSIDEAREA_K );
        keywords.insert( "BALANCED", BALANCED_K );
        keywords.insert( "BEGINEXT", BEGINEXT_K );
        keywords.insert( "BITS", BITS_K );
        keywords.insert( "BLOCKAGES", BLOCKAGES_K );
        keywords.insert( "BUSBITCHARS", BUSBITCHARS_K );
        keywords.insert( "BY", BY_K );
        keywords.insert( "CLOCK", CLOCK_K );
        keywords.insert( "COMMONSCANPINS", COMMONSCANPINS_K );
        keywords.insert( "COMPONENTPIN", COMPONENTPIN_K );
        keywords.insert( "COMPONENTS", COMPONENTS_K );
        keywords.insert( "COMPONENT", COMPONENT_K );
        keywords.insert( "COVER", COVER_K );
        keywords.insert( "DESIGN", DESIGN_K );
        keywords.insert( "DIEAREA", DIEAREA_K );
        keywords.insert( "DIRECTION", DIRECTION_K );
        keywords.insert( "DISTANCE", DISTANCE_K );
        keywords.insert( "DIST", DIST_K );
        keywords.insert( "DIVIDERCHAR", DIVIDERCHAR_K );
        keywords.insert( "DO", DO_K );
        keywords.insert( "E", E_K );
        keywords.insert( "EEQMASTER", EEQMASTER_K );
        keywords.insert( "ENDEXT", ENDEXT_K );
        keywords.insert( "END", END_K );
        keywords.insert( "ESTCAP", ESTCAP_K );
        keywords.insert( "FE", FE_K );
        keywords.insert( "FEEDTHRU", FEEDTHRU_K );
        keywords.insert( "FENCE", FENCE_K );
        keywords.insert( "FILLS", FILLS_K );
        keywords.insert( "FIXEDBUMP", FIXEDBUMP_K );
        keywords.insert( "FIXED", FIXED_K );
        keywords.insert( "FLOATING", FLOATING_K );
        keywords.insert( "FN", FN_K );
        keywords.insert( "FOREIGN", FOREIGN_K );
        keywords.insert( "FREQUENCY", FREQUENCY_K );
        keywords.insert( "FS", FS_K );
        keywords.insert( "FW", FW_K );
        keywords.insert( "GCELLGRID", GCELLGRID_K );
        keywords.insert( "GROUND", GROUND_K );
        keywords.insert( "GROUPS", GROUPS_K );
        keywords.insert( "GROUP", GROUP_K );
        keywords.insert( "GUIDE", GUIDE_K );
        keywords.insert( "HISTORY", HISTORY_K );
        keywords.insert( "INOUT", INOUT_K );
        keywords.insert( "INPUT", INPUT_K );
        keywords.insert( "INTEGER", INTEGER_K );
        keywords.insert( "IN", IN_K );
        keywords.insert( "LAYER", LAYER_K );
        keywords.insert( "MAXBITS", MAXBITS_K );
        keywords.insert( "MICRONS", MICRONS_K );
        keywords.insert( "MUSTJOIN", MUSTJOIN_K );
        keywords.insert( "N", N_K );
        keywords.insert( "NAMESCASESENSITIVE", NAMESCASESENSITIVE_K );
        keywords.insert( "NETLIST", NETLIST_K );
        keywords.insert( "NETS", NETS_K );
        keywords.insert( "NET", NET_K );
        keywords.insert( "NEW", NEW_K );
        keywords.insert( "NONDEFAULTRULE", NONDEFAULTRULE_K );
        keywords.insert( "NOSHIELD", NOSHIELD_K );
        keywords.insert( "OFF", OFF_K );
        keywords.insert( "ON", ON_K );
        keywords.insert( "ORDERED", ORDERED_K );
        keywords.insert( "ORIGINAL", ORIGINAL_K );
        keywords.insert( "OUTPUT", OUTPUT_K );
        keywords.insert( "OUT", OUT_K );
        keywords.insert( "OXIDE1", OXIDE1_K );
        keywords.insert( "OXIDE2", OXIDE2_K );
        keywords.insert( "OXIDE3", OXIDE3_K );
        keywords.insert( "OXIDE4", OXIDE4_K );
        keywords.insert( "PARTITION", PARTITION_K );
        keywords.insert( "PATTERNNAME", PATTERNNAME_K );
        keywords.insert( "PATTERN", PATTERN_K );
        keywords.insert( "PINPROPERTIES", PINPROPERTIES_K );
        keywords.insert( "PINS", PINS_K );
        keywords.insert( "PLACED", PLACED_K );
        keywords.insert( "PLACEMENT", PLACEMENT_K );
        keywords.insert( "POWER", POWER_K );
        keywords.insert( "PROPERTYDEFINITIONS", PROPERTYDEFINITIONS_K );
        keywords.insert( "PROPERTY", PROPERTY_K );
        keywords.insert( "PUSHDOWN", PUSHDOWN_K );
        keywords.insert( "RANGE", RANGE_K );
        keywords.insert( "REAL", REAL_K );
        keywords.insert( "RECT", RECT_K );
        keywords.insert( "REGIONS", REGIONS_K );
        keywords.insert( "REGION", REGION_K );
        keywords.insert( "RESET", RESET_K );
        keywords.insert( "ROUTED", ROUTED_K );
        keywords.insert( "ROW", ROW_K );
        keywords.insert( "S", S_K );
        keywords.insert( "SCANCHAINS", SCANCHAINS_K );
        keywords.insert( "SCAN", SCAN_K );
        keywords.insert( "SHAPE", SHAPE_K );
        keywords.insert( "SHIELDNET", SHIELDNET_K );
        keywords.insert( "SHIELD", SHIELD_K );
        keywords.insert( "SIGNAL", SIGNAL_K );
        keywords.insert( "SLOTS", SLOTS_K );
        keywords.insert( "SOURCE", SOURCE_K );
        keywords.insert( "SPECIALNETS", SPECIALNETS_K );
        keywords.insert( "SPECIALNET", SPECIALNET_K );
        keywords.insert( "SPECIAL", SPECIAL_K );
        keywords.insert( "START", START_K );
        keywords.insert( "STEINER", STEINER_K );
        keywords.insert( "STEP", STEP_K );
        keywords.insert( "STOP", STOP_K );
        keywords.insert( "STRING", STRING_K );
        keywords.insert( "SUBNET", SUBNET_K );
        keywords.insert( "SYNTHESIZED", SYNTHESIZED_K );
        keywords.insert( "TAPERRULE", TAPERRULE_K );
        keywords.insert( "TAPER", TAPER_K );
        keywords.insert( "TECHNOLOGY", TECHNOLOGY_K );
        keywords.insert( "TEST", TEST_K );
        keywords.insert( "TIEOFF", TIEOFF_K );
        keywords.insert( "TIMING", TIMING_K );
        keywords.insert( "TRACKS", TRACKS_K );
        keywords.insert( "TRUNK", TRUNK_K );
        keywords.insert( "TYPE", TYPE_K );
        keywords.insert( "UNITS", UNITS_K );
        keywords.insert( "UNPLACED", UNPLACED_K );
        keywords.insert( "USER", USER_K );
        keywords.insert( "USE", USE_K );
        keywords.insert( "VERSION", VERSION_K );
        keywords.insert( "VIAS", VIAS_K );
        keywords.insert( "VOLTAGE", VOLTAGE_K );
        keywords.insert( "VPIN", VPIN_K );
        keywords.insert( "WEIGHT", WEIGHT_K );
        keywords.insert( "WIREDLOGIC", WIREDLOGIC_K );
        keywords.insert( "XTALK", XTALK_K );
        keywords.insert( "W", W_K );
        keywords.insert( "X", X_K );
        keywords.insert( "Y", Y_K );
        keywords.insert( "POLYGON", POLYGON_K );
        keywords.insert( "SPACING", SPACING_K );
        keywords.insert( "DESIGNRULEWIDTH", DESIGNRULEWIDTH_K );
        keywords.insert( "NETEXPR", NETEXPR_K );
        keywords.insert( "SUPPLYSENSITIVITY", SUPPLYSENSITIVITY_K );
        keywords.insert( "GROUNDSENSITIVITY", GROUNDSENSITIVITY_K );
        keywords.insert( "VIARULE", VIARULE_K );
        keywords.insert( "CUTSIZE", CUTSIZE_K );
        keywords.insert( "LAYERS", LAYERS_K );
        keywords.insert( "CUTSPACING", CUTSPACING_K );
        keywords.insert( "ENCLOSURE", ENCLOSURE_K );
        keywords.insert( "ROWCOL", ROWCOL_K );
        keywords.insert( "ORIGIN", ORIGIN_K );
        keywords.insert( "OFFSET", OFFSET_K );
        keywords.insert( "HARDSPACING", HARDSPACING_K );
        keywords.insert( "WIDTH", WIDTH_K );
        keywords.insert( "DIAGWIDTH", DIAGWIDTH_K );
        keywords.insert( "WIREEXT", WIREEXT_K );
        keywords.insert( "MINCUTS", MINCUTS_K );
        keywords.insert( "NONDEFAULTRULES", NONDEFAULTRULES_K );
        keywords.insert( "VIA", VIA_K );
        keywords.insert( "HALO", HALO_K );
        keywords.insert( "STYLES", STYLES_K );
        keywords.insert( "STYLE", STYLE_K );
        keywords.insert( "PATTERN", PATTERN_K );
        keywords.insert( "PORT", PORT_K );
        init_keywords = false;
    }

    deflex_lineno = 1;
    deflex_casesens = true;
    BEGIN 0;
}

void deflex_done()
{
}

void deflex_history()
{
    BEGIN HISTORY;
}

void deflex_extension()
{
    BEGIN EXTENSION;
}
