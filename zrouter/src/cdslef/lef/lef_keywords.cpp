/*
 *     This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 *  Distribution,  Product Version 5.5, and is subject to the Cadence
 *  LEF/DEF Open Source License Agreement.   Your  continued  use  of
 *  this file constitutes your acceptance of the terms of the LEF/DEF
 *  Open Source License and an agreement to abide by its  terms.   If
 *  you  don't  agree  with  this, you must remove this and any other
 *  files which are part of the distribution and  destroy any  copies
 *  made.
 *
 *     For updates, support, or to become part of the LEF/DEF Community,
 *  check www.openeda.org for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include "lefiDefs.hpp"
#include "lef_parser.h"
#include "lefiDebug.hpp"
#include "FEF_stringhash.h"
#include "crypt.hpp"
#include "lex.h"

void lef_add_to_stringtab_Int(void *hdl, const char *str, int inValue) {
    int *value;
 
    // Modified 9/2/99 -- Wanda da Rosa
    // need to free this in FEF_stringhash.cpp
    value = (int*)lefMalloc(sizeof(int));
    if (value != NULL)  {
        *value = inValue;
        lef_add_to_stringtab (hdl, str, value);
    } else {
        // Ran out of memory 
        lefiError("Not enough memory");
    }
}

void lef_init_symbol_table(void *Keyword_set)
{

    lef_add_to_stringtab_Int(Keyword_set, "&DEFINE",	K_DEFINE) ;
    lef_add_to_stringtab_Int(Keyword_set, "&DEFINEB",	K_DEFINEB) ;
    lef_add_to_stringtab_Int(Keyword_set, "&DEFINES",	K_DEFINES) ;
    lef_add_to_stringtab_Int(Keyword_set, "&MESSAGE",	K_MESSAGE) ;
    lef_add_to_stringtab_Int(Keyword_set, "&CREATEFILE",	K_CREATEFILE) ;
    lef_add_to_stringtab_Int(Keyword_set, "&OPENFILE",	K_OPENFILE) ;
    lef_add_to_stringtab_Int(Keyword_set, "&CLOSEFILE",	K_CLOSEFILE) ;
    lef_add_to_stringtab_Int(Keyword_set, "&WARNING",	K_WARNING) ;
    lef_add_to_stringtab_Int(Keyword_set, "&ERROR",	K_ERROR) ;
    lef_add_to_stringtab_Int(Keyword_set, "&FATALERROR",	K_FATALERROR) ;
    lef_add_to_stringtab_Int(Keyword_set, "ABUT",	K_ABUT) ;
    lef_add_to_stringtab_Int(Keyword_set, "ABUTMENT",	K_ABUTMENT) ;
    lef_add_to_stringtab_Int(Keyword_set, "ACTIVE",	K_ACTIVE) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANALOG",	K_ANALOG) ;
    lef_add_to_stringtab_Int(Keyword_set, "AND",	K_AND) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAAREAFACTOR", 
      K_ANTENNAAREAFACTOR) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNALENGTHFACTOR", 
      K_ANTENNALENGTHFACTOR) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNASIZE", K_ANTENNASIZE) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAMETALLENGTH",
      K_ANTENNAMETALLENGTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAMETALAREA",
      K_ANTENNAMETALAREA) ;
    lef_add_to_stringtab_Int(Keyword_set, "AREA",	K_AREA) ;
    lef_add_to_stringtab_Int(Keyword_set, "ARRAY",	K_ARRAY) ;
    lef_add_to_stringtab_Int(Keyword_set, "BEGINEXT",	K_BEGINEXT) ;
    lef_add_to_stringtab_Int(Keyword_set, "BLOCK",	K_BLOCK) ;
    lef_add_to_stringtab_Int(Keyword_set, "BOTTOMLEFT",	K_BOTTOMLEFT) ;
    lef_add_to_stringtab_Int(Keyword_set, "BOTTOMRIGHT",	K_BOTTOMRIGHT) ;
    lef_add_to_stringtab_Int(Keyword_set, "BUSBITCHARS",	K_BUSBITCHARS) ;
    lef_add_to_stringtab_Int(Keyword_set, "BUFFER",	K_BUFFER) ;
    lef_add_to_stringtab_Int(Keyword_set, "BY",		K_BY) ;
    lef_add_to_stringtab_Int(Keyword_set, "CANNOTOCCUPY",	K_CANNOTOCCUPY) ;
    lef_add_to_stringtab_Int(Keyword_set, "CANPLACE",	K_CANPLACE) ;
    lef_add_to_stringtab_Int(Keyword_set, "CAPACITANCE",	K_CAPACITANCE) ;
    lef_add_to_stringtab_Int(Keyword_set, "CAPMULTIPLIER",	K_CAPMULTIPLIER) ;
    lef_add_to_stringtab_Int(Keyword_set, "CLASS",	K_CLASS) ;
    lef_add_to_stringtab_Int(Keyword_set, "CLOCK",	K_CLOCK) ;
    lef_add_to_stringtab_Int(Keyword_set, "CLOCKTYPE",	K_CLOCKTYPE) ;
    lef_add_to_stringtab_Int(Keyword_set, "COLUMNMAJOR",	K_COLUMNMAJOR) ;
    lef_add_to_stringtab_Int(Keyword_set, "CURRENTDEN",	K_CURRENTDEN) ;
    lef_add_to_stringtab_Int(Keyword_set, "COMPONENTPIN",	K_COMPONENTPIN) ;
    lef_add_to_stringtab_Int(Keyword_set, "COMPONENTS",	K_COMPONENTS) ;
    lef_add_to_stringtab_Int(Keyword_set, "CORE",	K_CORE) ;
    lef_add_to_stringtab_Int(Keyword_set, "CORNER",	K_CORNER) ;
    lef_add_to_stringtab_Int(Keyword_set, "COVER",	K_COVER) ;
    lef_add_to_stringtab_Int(Keyword_set, "CPERSQDIST",	K_CPERSQDIST) ;
    lef_add_to_stringtab_Int(Keyword_set, "CURRENT",	K_CURRENT) ;
    lef_add_to_stringtab_Int(Keyword_set, "CURRENTSOURCE",	K_CURRENTSOURCE) ;
    lef_add_to_stringtab_Int(Keyword_set, "CUT",	K_CUT) ;
    lef_add_to_stringtab_Int(Keyword_set, "DATA",    	K_DATA) ;
    lef_add_to_stringtab_Int(Keyword_set, "DATABASE",	K_DATABASE) ;
    lef_add_to_stringtab_Int(Keyword_set, "DEFAULT",	K_DEFAULT) ;
    lef_add_to_stringtab_Int(Keyword_set, "DEFAULTCAP",	K_DEFAULTCAP) ;
    lef_add_to_stringtab_Int(Keyword_set, "DELAY",	K_DELAY) ;
    lef_add_to_stringtab_Int(Keyword_set, "DIELECTRIC",	K_DIELECTRIC) ;
    lef_add_to_stringtab_Int(Keyword_set, "DIRECTION",	K_DIRECTION) ;
    lef_add_to_stringtab_Int(Keyword_set, "DIVIDERCHAR",	K_DIVIDERCHAR) ;
    lef_add_to_stringtab_Int(Keyword_set, "DO",		K_DO) ;
    lef_add_to_stringtab_Int(Keyword_set, "E",	K_E) ;
    lef_add_to_stringtab_Int(Keyword_set, "EDGECAPACITANCE",K_EDGECAPACITANCE) ;
    lef_add_to_stringtab_Int(Keyword_set, "EEQ",	K_EEQ) ;
    lef_add_to_stringtab_Int(Keyword_set, "ELSE",	K_ELSE) ;
    lef_add_to_stringtab_Int(Keyword_set, "END",	K_END) ;
    lef_add_to_stringtab_Int(Keyword_set, "ENDEXT",	K_ENDEXT) ;
    lef_add_to_stringtab_Int(Keyword_set, "ENDCAP",	K_ENDCAP) ;
    lef_add_to_stringtab_Int(Keyword_set, "EXTENSION",	K_EXTENSION) ;
    lef_add_to_stringtab_Int(Keyword_set, "FALL",	K_FALL) ;
    lef_add_to_stringtab_Int(Keyword_set, "FALLCS",	K_FALLCS) ;
    lef_add_to_stringtab_Int(Keyword_set, "FALLRS",	K_FALLRS) ;
    lef_add_to_stringtab_Int(Keyword_set, "FALLSATCUR",	K_FALLSATCUR) ;
    lef_add_to_stringtab_Int(Keyword_set, "FALLSATT1",	K_FALLSATT1) ;
    lef_add_to_stringtab_Int(Keyword_set, "FALLSLEWLIMIT",	K_FALLSLEWLIMIT) ;
    lef_add_to_stringtab_Int(Keyword_set, "FALLT0", 	K_FALLT0) ;
    lef_add_to_stringtab_Int(Keyword_set, "FALLTHRESH",	K_FALLTHRESH) ;
    lef_add_to_stringtab_Int(Keyword_set, "FALLVOLTAGETHRESHOLD",	
      K_FALLVOLTAGETHRESHOLD) ;
    lef_add_to_stringtab_Int(Keyword_set, "FALSE",	K_FALSE) ;
    lef_add_to_stringtab_Int(Keyword_set, "FE",	K_FE) ;
    lef_add_to_stringtab_Int(Keyword_set, "FEEDTHRU",	K_FEEDTHRU) ;
    lef_add_to_stringtab_Int(Keyword_set, "FIXED",	K_FIXED) ;
    lef_add_to_stringtab_Int(Keyword_set, "FLOORPLAN",	K_FLOORPLAN) ;
    lef_add_to_stringtab_Int(Keyword_set, "FN",	K_FN) ;
    lef_add_to_stringtab_Int(Keyword_set, "FOREIGN",	K_FOREIGN) ;
    lef_add_to_stringtab_Int(Keyword_set, "FROMPIN",	K_FROMPIN) ;
    lef_add_to_stringtab_Int(Keyword_set, "FUNCTION",	K_FUNCTION) ;
    lef_add_to_stringtab_Int(Keyword_set, "FS",	K_FS) ;
    lef_add_to_stringtab_Int(Keyword_set, "FW",	K_FW) ;
    lef_add_to_stringtab_Int(Keyword_set, "GCELLGRID",	K_GCELLGRID) ;
    lef_add_to_stringtab_Int(Keyword_set, "GENERATE",	K_GENERATE) ;
    lef_add_to_stringtab_Int(Keyword_set, "GENERATOR",	K_GENERATOR) ;
    lef_add_to_stringtab_Int(Keyword_set, "GROUND",	K_GROUND) ;
    lef_add_to_stringtab_Int(Keyword_set, "HEIGHT",	K_HEIGHT) ;
    lef_add_to_stringtab_Int(Keyword_set, "HISTORY", 	K_HISTORY) ;
    lef_add_to_stringtab_Int(Keyword_set, "HOLD",	K_HOLD) ;
    lef_add_to_stringtab_Int(Keyword_set, "HORIZONTAL",	K_HORIZONTAL) ;
    lef_add_to_stringtab_Int(Keyword_set, "IF",		K_IF) ;
    lef_add_to_stringtab_Int(Keyword_set, "INOUT",	K_INOUT) ;
    lef_add_to_stringtab_Int(Keyword_set, "INOUTPINANTENNASIZE",
      K_INOUTPINANTENNASIZE) ;
    lef_add_to_stringtab_Int(Keyword_set, "INPUT",	K_INPUT) ;
    lef_add_to_stringtab_Int(Keyword_set, "INPUTPINANTENNASIZE",
      K_INPUTPINANTENNASIZE) ;
    lef_add_to_stringtab_Int(Keyword_set, "INPUTNOISEMARGIN",
      K_INPUTNOISEMARGIN) ;
    lef_add_to_stringtab_Int(Keyword_set, "INTEGER",	K_INTEGER) ;
    lef_add_to_stringtab_Int(Keyword_set, "INTRINSIC",	K_INTRINSIC) ;
    lef_add_to_stringtab_Int(Keyword_set, "INVERT",	K_INVERT) ;
    lef_add_to_stringtab_Int(Keyword_set, "INVERTER",	K_INVERTER) ;
    lef_add_to_stringtab_Int(Keyword_set, "IRDROP",	K_IRDROP) ;
    lef_add_to_stringtab_Int(Keyword_set, "ITERATE",	K_ITERATE) ;
    lef_add_to_stringtab_Int(Keyword_set, "IV_TABLES",	K_IV_TABLES) ;
    lef_add_to_stringtab_Int(Keyword_set, "LAYER",	K_LAYER) ;
    lef_add_to_stringtab_Int(Keyword_set, "LEAKAGE",	K_LEAKAGE) ;
    lef_add_to_stringtab_Int(Keyword_set, "LEQ",	K_LEQ) ;
    lef_add_to_stringtab_Int(Keyword_set, "LIBRARY",	K_LIBRARY) ;
    lef_add_to_stringtab_Int(Keyword_set, "MACRO",	K_MACRO) ;
    lef_add_to_stringtab_Int(Keyword_set, "MASTERSLICE",	K_MASTERSLICE) ;
    lef_add_to_stringtab_Int(Keyword_set, "MATCH",	K_MATCH) ;
    lef_add_to_stringtab_Int(Keyword_set, "MAXDELAY",	K_MAXDELAY) ;
    lef_add_to_stringtab_Int(Keyword_set, "MAXLOAD",	K_MAXLOAD) ;
    lef_add_to_stringtab_Int(Keyword_set, "METALOVERHANG",	K_METALOVERHANG) ;
    lef_add_to_stringtab_Int(Keyword_set, "MILLIAMPS",	K_MILLIAMPS) ;
    lef_add_to_stringtab_Int(Keyword_set, "MILLIWATTS",	K_MILLIWATTS) ;
    lef_add_to_stringtab_Int(Keyword_set, "MINFEATURE",	K_MINFEATURE) ;
    lef_add_to_stringtab_Int(Keyword_set, "MINPINS",	K_MINPINS) ;
    lef_add_to_stringtab_Int(Keyword_set, "MUSTJOIN",	K_MUSTJOIN) ;
    lef_add_to_stringtab_Int(Keyword_set, "MX",     	K_MX) ;
    lef_add_to_stringtab_Int(Keyword_set, "MY",	        K_MY) ;
    lef_add_to_stringtab_Int(Keyword_set, "MXR90",	K_MXR90) ;
    lef_add_to_stringtab_Int(Keyword_set, "MYR90",	K_MYR90) ;
    lef_add_to_stringtab_Int(Keyword_set, "N",	K_N) ;
    lef_add_to_stringtab_Int(Keyword_set, "NAMEMAPSTRING", K_NAMEMAPSTRING) ;
    lef_add_to_stringtab_Int(Keyword_set, "NAMESCASESENSITIVE",
      K_NAMESCASESENSITIVE) ;
    lef_add_to_stringtab_Int(Keyword_set, "NANOSECONDS",	K_NANOSECONDS) ;
    lef_add_to_stringtab_Int(Keyword_set, "NETS",	K_NETS) ;
    lef_add_to_stringtab_Int(Keyword_set, "NEW",	K_NEW) ;
    lef_add_to_stringtab_Int(Keyword_set, "NONDEFAULTRULE",	K_NONDEFAULTRULE) ;
    lef_add_to_stringtab_Int(Keyword_set, "NONINVERT",	K_NONINVERT) ;
    lef_add_to_stringtab_Int(Keyword_set, "NONUNATE",	K_NONUNATE) ;
    lef_add_to_stringtab_Int(Keyword_set, "NOWIREEXTENSIONATPIN",
      K_NOWIREEXTENSIONATPIN) ;
    lef_add_to_stringtab_Int(Keyword_set, "OBS",	K_OBS) ;
    lef_add_to_stringtab_Int(Keyword_set, "OFF",	K_OFF) ;
    lef_add_to_stringtab_Int(Keyword_set, "OFFSET", K_OFFSET) ;
    lef_add_to_stringtab_Int(Keyword_set, "OHMS",	K_OHMS) ;
    lef_add_to_stringtab_Int(Keyword_set, "ON",	K_ON) ;
    lef_add_to_stringtab_Int(Keyword_set, "OR",	K_OR) ;
    lef_add_to_stringtab_Int(Keyword_set, "ORIENTATION",	K_ORIENTATION) ;
    lef_add_to_stringtab_Int(Keyword_set, "ORIGIN",	K_ORIGIN) ;
    lef_add_to_stringtab_Int(Keyword_set, "OUTPUT",	K_OUTPUT) ;
    lef_add_to_stringtab_Int(Keyword_set, "OUTPUTPINANTENNASIZE",
      K_OUTPUTPINANTENNASIZE) ;
    lef_add_to_stringtab_Int(Keyword_set, "OUTPUTNOISEMARGIN",
      K_OUTPUTNOISEMARGIN) ;
    lef_add_to_stringtab_Int(Keyword_set, "OUTPUTRESISTANCE",
      K_OUTPUTRESISTANCE) ;
    lef_add_to_stringtab_Int(Keyword_set, "OVERHANG",	K_OVERHANG) ;
    lef_add_to_stringtab_Int(Keyword_set, "OVERLAP",	K_OVERLAP) ;
    lef_add_to_stringtab_Int(Keyword_set, "OVERLAPS",	K_OVERLAPS) ;
    lef_add_to_stringtab_Int(Keyword_set, "PAD",	K_PAD) ;
    lef_add_to_stringtab_Int(Keyword_set, "PATH",	K_PATH) ;
    lef_add_to_stringtab_Int(Keyword_set, "PATTERN",	K_PATTERN) ;
    lef_add_to_stringtab_Int(Keyword_set, "PICOFARADS",	K_PICOFARADS) ;
    lef_add_to_stringtab_Int(Keyword_set, "PIN",	K_PIN) ;
    lef_add_to_stringtab_Int(Keyword_set, "PITCH",	K_PITCH) ;
    lef_add_to_stringtab_Int(Keyword_set, "PLACED",	K_PLACED) ;
    lef_add_to_stringtab_Int(Keyword_set, "POLYGON",	K_POLYGON) ;
    lef_add_to_stringtab_Int(Keyword_set, "PORT",	K_PORT) ;
    lef_add_to_stringtab_Int(Keyword_set, "POST",	K_POST) ;
    lef_add_to_stringtab_Int(Keyword_set, "POWER",	K_POWER) ;
    lef_add_to_stringtab_Int(Keyword_set, "PRE",	K_PRE) ;
    lef_add_to_stringtab_Int(Keyword_set, "PROPERTY",	K_PROPERTY) ;
    lef_add_to_stringtab_Int(Keyword_set, "PROPERTYDEFINITIONS",	K_PROPDEF) ;
    lef_add_to_stringtab_Int(Keyword_set, "PULLDOWNRES",	K_PULLDOWNRES) ;
    lef_add_to_stringtab_Int(Keyword_set, "PWL",	K_PWL) ;
    lef_add_to_stringtab_Int(Keyword_set, "R0",	K_R0) ;
    lef_add_to_stringtab_Int(Keyword_set, "R90",	K_R90) ;
    lef_add_to_stringtab_Int(Keyword_set, "R180",	K_R180) ;
    lef_add_to_stringtab_Int(Keyword_set, "R270",	K_R270) ;
    lef_add_to_stringtab_Int(Keyword_set, "RANGE",	K_RANGE) ;
    lef_add_to_stringtab_Int(Keyword_set, "REAL",	K_REAL) ;
    lef_add_to_stringtab_Int(Keyword_set, "RECT",	K_RECT) ;
    lef_add_to_stringtab_Int(Keyword_set, "RESISTANCE",	K_RESISTANCE) ;
    lef_add_to_stringtab_Int(Keyword_set, "RESISTIVE",	K_RESISTIVE) ;
    lef_add_to_stringtab_Int(Keyword_set, "RING",	K_RING) ;
    lef_add_to_stringtab_Int(Keyword_set, "RISE",	K_RISE) ;
    lef_add_to_stringtab_Int(Keyword_set, "RISECS",	K_RISECS) ;
    lef_add_to_stringtab_Int(Keyword_set, "RISERS",	K_RISERS) ;
    lef_add_to_stringtab_Int(Keyword_set, "RISESATCUR",	K_RISESATCUR) ;
    lef_add_to_stringtab_Int(Keyword_set, "RISESATT1",	K_RISESATT1) ;
    lef_add_to_stringtab_Int(Keyword_set, "RISESLEWLIMIT",	K_RISESLEWLIMIT) ;
    lef_add_to_stringtab_Int(Keyword_set, "RISET0",    	K_RISET0) ;
    lef_add_to_stringtab_Int(Keyword_set, "RISETHRESH",	K_RISETHRESH) ;
    lef_add_to_stringtab_Int(Keyword_set, "RISEVOLTAGETHRESHOLD",	
      K_RISEVOLTAGETHRESHOLD) ;
    lef_add_to_stringtab_Int(Keyword_set, "ROUTING",	K_ROUTING) ;
    lef_add_to_stringtab_Int(Keyword_set, "ROWMAJOR",	K_ROWMAJOR) ;
    lef_add_to_stringtab_Int(Keyword_set, "RPERSQ",	K_RPERSQ) ;
    lef_add_to_stringtab_Int(Keyword_set, "S",	K_S) ;
    lef_add_to_stringtab_Int(Keyword_set, "SAMENET",	K_SAMENET) ;
    lef_add_to_stringtab_Int(Keyword_set, "SCANUSE",	K_SCANUSE) ;
    lef_add_to_stringtab_Int(Keyword_set, "SETUP",	K_SETUP) ;
    lef_add_to_stringtab_Int(Keyword_set, "SHAPE",	K_SHAPE) ;
    lef_add_to_stringtab_Int(Keyword_set, "SHRINKAGE",	K_SHRINKAGE) ;
    lef_add_to_stringtab_Int(Keyword_set, "SIGNAL",	K_SIGNAL) ;
    lef_add_to_stringtab_Int(Keyword_set, "SITE",	K_SITE) ;
    lef_add_to_stringtab_Int(Keyword_set, "SIZE",	K_SIZE) ;
    lef_add_to_stringtab_Int(Keyword_set, "SOURCE",	K_SOURCE) ;
    lef_add_to_stringtab_Int(Keyword_set, "SPACER",	K_SPACER) ;
    lef_add_to_stringtab_Int(Keyword_set, "SPACING",	K_SPACING) ;
    lef_add_to_stringtab_Int(Keyword_set, "SPECIALNETS",	K_SPECIALNETS) ;
    lef_add_to_stringtab_Int(Keyword_set, "STABLE",	K_STABLE) ;
    lef_add_to_stringtab_Int(Keyword_set, "STACK",	K_STACK) ;
    lef_add_to_stringtab_Int(Keyword_set, "START",	K_START) ;
    lef_add_to_stringtab_Int(Keyword_set, "STEP",	K_STEP) ;
    lef_add_to_stringtab_Int(Keyword_set, "STOP",	K_STOP) ;
    lef_add_to_stringtab_Int(Keyword_set, "STRING",	K_STRING) ;
    lef_add_to_stringtab_Int(Keyword_set, "STRUCTURE",	K_STRUCTURE) ;
    lef_add_to_stringtab_Int(Keyword_set, "SYMMETRY",	K_SYMMETRY) ;
    lef_add_to_stringtab_Int(Keyword_set, "TABLE",	K_TABLE) ;
    lef_add_to_stringtab_Int(Keyword_set, "TABLEAXIS",	K_TABLEAXIS) ;
    lef_add_to_stringtab_Int(Keyword_set, "TABLEDIMENSION",	K_TABLEDIMENSION) ;
    lef_add_to_stringtab_Int(Keyword_set, "TABLEENTRIES",	K_TABLEENTRIES) ;
    lef_add_to_stringtab_Int(Keyword_set, "TAPERRULE",	K_TAPERRULE) ;
    lef_add_to_stringtab_Int(Keyword_set, "THEN",	K_THEN) ;
    lef_add_to_stringtab_Int(Keyword_set, "THICKNESS",	K_THICKNESS) ;
    lef_add_to_stringtab_Int(Keyword_set, "TIEHIGH",	K_TIEHIGH) ;
    lef_add_to_stringtab_Int(Keyword_set, "TIELOW",	K_TIELOW) ;
    lef_add_to_stringtab_Int(Keyword_set, "TIEOFFR",	K_TIEOFFR) ;
    lef_add_to_stringtab_Int(Keyword_set, "TIME",	K_TIME) ;
    lef_add_to_stringtab_Int(Keyword_set, "TIMING",	K_TIMING) ;
    lef_add_to_stringtab_Int(Keyword_set, "TO",		K_TO) ;
    lef_add_to_stringtab_Int(Keyword_set, "TOPIN",	K_TOPIN) ;
    lef_add_to_stringtab_Int(Keyword_set, "TOPLEFT",	K_TOPLEFT) ;
    lef_add_to_stringtab_Int(Keyword_set, "TOPOFSTACKONLY", K_TOPOFSTACKONLY) ;
    lef_add_to_stringtab_Int(Keyword_set, "TOPRIGHT",	K_TOPRIGHT) ;
    lef_add_to_stringtab_Int(Keyword_set, "TRACKS",	K_TRACKS) ;
    lef_add_to_stringtab_Int(Keyword_set, "TRANSITIONTIME",	K_TRANSITIONTIME) ;
    lef_add_to_stringtab_Int(Keyword_set, "TRISTATE",	K_TRISTATE) ;
    lef_add_to_stringtab_Int(Keyword_set, "TRUE",	K_TRUE) ;
    lef_add_to_stringtab_Int(Keyword_set, "TYPE",	K_TYPE) ;
    lef_add_to_stringtab_Int(Keyword_set, "UNATENESS",	K_UNATENESS) ;
    lef_add_to_stringtab_Int(Keyword_set, "UNITS",	K_UNITS) ;
    lef_add_to_stringtab_Int(Keyword_set, "USE",	K_USE) ;
    lef_add_to_stringtab_Int(Keyword_set, "USER",	K_USER) ;
    lef_add_to_stringtab_Int(Keyword_set, "VARIABLE",	K_VARIABLE) ;
    lef_add_to_stringtab_Int(Keyword_set, "VERSION",	K_VERSION) ;
    lef_add_to_stringtab_Int(Keyword_set, "VERTICAL",	K_VERTICAL) ;
    lef_add_to_stringtab_Int(Keyword_set, "VHI",	K_VHI) ;
    lef_add_to_stringtab_Int(Keyword_set, "VIA",	K_VIA) ;
    lef_add_to_stringtab_Int(Keyword_set, "VIARULE",	K_VIARULE) ;
    lef_add_to_stringtab_Int(Keyword_set, "VIRTUAL",	K_VIRTUAL) ;
    lef_add_to_stringtab_Int(Keyword_set, "VLO",	K_VLO) ;
    lef_add_to_stringtab_Int(Keyword_set, "VOLTAGE",	K_VOLTAGE) ;
    lef_add_to_stringtab_Int(Keyword_set, "VOLTS",	K_VOLTS) ;
    lef_add_to_stringtab_Int(Keyword_set, "W",	K_W) ;
    lef_add_to_stringtab_Int(Keyword_set, "WIDTH",	K_WIDTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "WIRECAP",	K_WIRECAP) ;
    lef_add_to_stringtab_Int(Keyword_set, "WIREEXTENSION",	K_WIREEXTENSION) ;
    lef_add_to_stringtab_Int(Keyword_set, "X",	K_X) ;
    lef_add_to_stringtab_Int(Keyword_set, "Y",	K_Y) ;
    lef_add_to_stringtab_Int(Keyword_set, "namescasesensitive",
      K_NAMESCASESENSITIVE) ;
    lef_add_to_stringtab_Int(Keyword_set, "off",	K_OFF) ;
    lef_add_to_stringtab_Int(Keyword_set, "on",	K_ON) ;
    lef_add_to_stringtab_Int(Keyword_set, "UNIVERSALNOISEMARGIN",
      K_UNIVERSALNOISEMARGIN) ;
    lef_add_to_stringtab_Int(Keyword_set, "EDGERATETHRESHOLD1",
      K_EDGERATETHRESHOLD1) ;
    lef_add_to_stringtab_Int(Keyword_set, "CORRECTIONTABLE", K_CORRECTIONTABLE) ;
    lef_add_to_stringtab_Int(Keyword_set, "EDGERATESCALEFACTOR",
      K_EDGERATESCALEFACTOR) ;
    lef_add_to_stringtab_Int(Keyword_set, "EDGERATETHRESHOLD2",
      K_EDGERATETHRESHOLD2) ;
    lef_add_to_stringtab_Int(Keyword_set, "VICTIMNOISE",      K_VICTIMNOISE) ;
    lef_add_to_stringtab_Int(Keyword_set, "NOISETABLE",       K_NOISETABLE) ;
    lef_add_to_stringtab_Int(Keyword_set, "EDGERATE",         K_EDGERATE) ;
    lef_add_to_stringtab_Int(Keyword_set, "OUTPUTRESISTANCE", K_OUTPUTRESISTANCE) ;
    lef_add_to_stringtab_Int(Keyword_set, "VICTIMLENGTH",     K_VICTIMLENGTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "CORRECTIONFACTOR", K_CORRECTIONFACTOR) ;
    lef_add_to_stringtab_Int(Keyword_set, "RECOVERY",         K_RECOVERY) ;
    lef_add_to_stringtab_Int(Keyword_set, "SKEW",             K_SKEW) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANYEDGE",          K_ANYEDGE) ;
    lef_add_to_stringtab_Int(Keyword_set, "POSEDGE",          K_POSEDGE) ;
    lef_add_to_stringtab_Int(Keyword_set, "NEGEDGE",          K_NEGEDGE) ;
    lef_add_to_stringtab_Int(Keyword_set, "SDFCONDSTART",     K_SDFCONDSTART) ;
    lef_add_to_stringtab_Int(Keyword_set, "SDFCONDEND",       K_SDFCONDEND) ;
    lef_add_to_stringtab_Int(Keyword_set, "SDFCOND",          K_SDFCOND) ;
    lef_add_to_stringtab_Int(Keyword_set, "MPWH",             K_MPWH) ;
    lef_add_to_stringtab_Int(Keyword_set, "MPWL",             K_MPWL) ;
    lef_add_to_stringtab_Int(Keyword_set, "PERIOD",           K_PERIOD) ;
    lef_add_to_stringtab_Int(Keyword_set, "ACCURRENTDENSITY", K_ACCURRENTDENSITY) ;
    lef_add_to_stringtab_Int(Keyword_set, "DCCURRENTDENSITY", K_DCCURRENTDENSITY) ;
    lef_add_to_stringtab_Int(Keyword_set, "TABLEDIMENSION",   K_TABLEDIMENSION) ;
    lef_add_to_stringtab_Int(Keyword_set, "PEAK",             K_PEAK) ;
    lef_add_to_stringtab_Int(Keyword_set, "AVERAGE",          K_AVERAGE) ;
    lef_add_to_stringtab_Int(Keyword_set, "RMS",              K_RMS) ;
    lef_add_to_stringtab_Int(Keyword_set, "FREQUENCY",        K_FREQUENCY) ;
    lef_add_to_stringtab_Int(Keyword_set, "CUTAREA",          K_CUTAREA) ;
    lef_add_to_stringtab_Int(Keyword_set, "MEGAHERTZ",        K_MEGAHERTZ) ;
    lef_add_to_stringtab_Int(Keyword_set, "USELENGTHTHRESHOLD",
      K_USELENGTHTHRESHOLD) ;
    lef_add_to_stringtab_Int(Keyword_set, "LENGTHTHRESHOLD",
      K_LENGTHTHRESHOLD) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAINPUTGATEAREA",
      K_ANTENNAINPUTGATEAREA) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAINOUTDIFFAREA",
      K_ANTENNAINOUTDIFFAREA) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAOUTPUTDIFFAREA",
      K_ANTENNAOUTPUTDIFFAREA) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAGATEAREA",
      K_ANTENNAGATEAREA) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNADIFFAREA",
      K_ANTENNADIFFAREA) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAPARTIALMETALAREA",
      K_ANTENNAPARTIALMETALAREA) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAPARTIALMETALSIDEAREA",
      K_ANTENNAPARTIALMETALSIDEAREA) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAMAXAREACAR",
      K_ANTENNAMAXAREACAR) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAMAXSIDEAREACAR",
      K_ANTENNAMAXSIDEAREACAR) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAPARTIALCUTAREA",
      K_ANTENNAPARTIALCUTAREA) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAMAXCUTCAR",
      K_ANTENNAMAXCUTCAR) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAAREARATIO",
      K_ANTENNAAREARATIO) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNADIFFAREARATIO",
      K_ANTENNADIFFAREARATIO) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNACUMAREARATIO",
      K_ANTENNACUMAREARATIO) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNACUMDIFFAREARATIO",
      K_ANTENNACUMDIFFAREARATIO) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAAREAFACTOR",
      K_ANTENNAAREAFACTOR) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNASIDEAREARATIO",
      K_ANTENNASIDEAREARATIO) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNADIFFSIDEAREARATIO",
      K_ANTENNADIFFSIDEAREARATIO) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNACUMSIDEAREARATIO",
      K_ANTENNACUMSIDEAREARATIO) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNACUMDIFFSIDEAREARATIO",
      K_ANTENNACUMDIFFSIDEAREARATIO) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNASIDEAREAFACTOR",
      K_ANTENNASIDEAREAFACTOR) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNACELL", K_ANTENNACELL) ;
    lef_add_to_stringtab_Int(Keyword_set, "CLEARANCEMEASURE",
      K_CLEARANCEMEASURE) ;
    lef_add_to_stringtab_Int(Keyword_set, "DIFFUSEONLY", K_DIFFUSEONLY) ;
    lef_add_to_stringtab_Int(Keyword_set, "EUCLIDEAN", K_EUCLIDEAN) ;
    lef_add_to_stringtab_Int(Keyword_set, "FLIP", K_FLIP) ;
    lef_add_to_stringtab_Int(Keyword_set, "MANUFACTURINGGRID",
      K_MANUFACTURINGGRID) ;
    lef_add_to_stringtab_Int(Keyword_set, "MAXXY", K_MAXXY) ;
    lef_add_to_stringtab_Int(Keyword_set, "NONE", K_NONE) ;
    lef_add_to_stringtab_Int(Keyword_set, "ROWMINSPACING", K_ROWMINSPACING) ;
    lef_add_to_stringtab_Int(Keyword_set, "ROWABUTSPACING", K_ROWABUTSPACING) ;
    lef_add_to_stringtab_Int(Keyword_set, "USEMINSPACING", K_USEMINSPACING) ;
    lef_add_to_stringtab_Int(Keyword_set, "DESIGNRULEWIDTH", K_DESIGNRULEWIDTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "INFLUENCE", K_INFLUENCE) ;
    lef_add_to_stringtab_Int(Keyword_set, "MAXADJACENTSLOTSPACING", K_MAXADJACENTSLOTSPACING) ;
    lef_add_to_stringtab_Int(Keyword_set, "MAXCOAXIALSLOTSPACING", K_MAXCOAXIALSLOTSPACING) ;
    lef_add_to_stringtab_Int(Keyword_set, "MAXEDGESLOTSPACING", K_MAXEDGESLOTSPACING) ;
    lef_add_to_stringtab_Int(Keyword_set, "SLOTLENGTH", K_SLOTLENGTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "SLOTWIDTH", K_SLOTWIDTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "SLOTWIRELENGTH", K_SLOTWIRELENGTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "SLOTWIREWIDTH", K_SLOTWIREWIDTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "SPLITWIREWIDTH", K_SPLITWIREWIDTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "MAXIMUMDENSITY", K_MAXIMUMDENSITY) ;
    lef_add_to_stringtab_Int(Keyword_set, "MINIMUMCUT", K_MINIMUMCUT) ;
    lef_add_to_stringtab_Int(Keyword_set, "MINIMUMDENSITY", K_MINIMUMDENSITY) ;
    lef_add_to_stringtab_Int(Keyword_set, "DENSITYCHECKWINDOW", K_DENSITYCHECKWINDOW) ;
    lef_add_to_stringtab_Int(Keyword_set, "DENSITYCHECKSTEP", K_DENSITYCHECKSTEP) ;
    lef_add_to_stringtab_Int(Keyword_set, "FILLACTIVESPACING", K_FILLACTIVESPACING) ;
    lef_add_to_stringtab_Int(Keyword_set, "ADJACENTCUTS", K_ADJACENTCUTS) ;
    lef_add_to_stringtab_Int(Keyword_set, "ANTENNAMODEL", K_ANTENNAMODEL) ;
    lef_add_to_stringtab_Int(Keyword_set, "AREAIO", K_AREAIO) ;
    lef_add_to_stringtab_Int(Keyword_set, "BLACKBOX", K_BLACKBOX) ;
    lef_add_to_stringtab_Int(Keyword_set, "BUMP", K_BUMP) ;
    lef_add_to_stringtab_Int(Keyword_set, "ENCLOSURE", K_ENCLOSURE) ;
    lef_add_to_stringtab_Int(Keyword_set, "FROMABOVE", K_FROMABOVE) ;
    lef_add_to_stringtab_Int(Keyword_set, "FROMBELOW", K_FROMBELOW) ;
    lef_add_to_stringtab_Int(Keyword_set, "IMPLANT", K_IMPLANT) ;
    lef_add_to_stringtab_Int(Keyword_set, "LENGTH", K_LENGTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "MAXVIASTACK", K_MAXVIASTACK) ;
    lef_add_to_stringtab_Int(Keyword_set, "MAXWIDTH", K_MAXWIDTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "MINWIDTH", K_MINWIDTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "MINENCLOSEDAREA", K_MINENCLOSEDAREA) ;
    lef_add_to_stringtab_Int(Keyword_set, "MINSTEP", K_MINSTEP) ;
    lef_add_to_stringtab_Int(Keyword_set, "ORIENT", K_ORIENT) ;
    lef_add_to_stringtab_Int(Keyword_set, "OXIDE1", K_OXIDE1) ;
    lef_add_to_stringtab_Int(Keyword_set, "OXIDE2", K_OXIDE2) ;
    lef_add_to_stringtab_Int(Keyword_set, "OXIDE3", K_OXIDE3) ;
    lef_add_to_stringtab_Int(Keyword_set, "OXIDE4", K_OXIDE4) ;
    lef_add_to_stringtab_Int(Keyword_set, "PARALLELRUNLENGTH", K_PARALLELRUNLENGTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "PROTRUSIONWIDTH", K_PROTRUSIONWIDTH) ;
    lef_add_to_stringtab_Int(Keyword_set, "SPACINGTABLE", K_SPACINGTABLE) ;
    lef_add_to_stringtab_Int(Keyword_set, "WITHIN", K_WITHIN) ;
}

#define yyparse	lefyyparse
#define yylex	lefyylex
#define yyerror	lefyyerror
#define yylval	lefyylval
#define yychar	lefyychar
#define yydebug	lefyydebug
#define yynerrs	lefyynerrs

#include "lex.cpph"

