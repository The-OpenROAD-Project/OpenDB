/*
 * This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 * Distribution,  Product Version 5.5, and is subject to the Cadence LEF/DEF
 * Open Source License Agreement.   Your  continued  use  of this file
 * constitutes your acceptance of the terms of the LEF/DEF Open Source
 * License and an agreement to abide by its  terms.   If you  don't  agree
 * with  this, you must remove this and any other files which are part of the
 * distribution and  destroy any  copies made.
 * 
 * For updates, support, or to become part of the LEF/DEF Community, check
 * www.openeda.org for details.
 */

/*
 * This file contains code for implementing the lefwriter 5.3
 *//* It has all the functions user can call in their callbacks or *//* just
 * their writer to write out the correct lef syntax. *//* Author: Wanda da
 * Rosa *//* Date:   Summer, 1998 *//* Revisions: */
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "lefwWriter.h"
#include "crypt.h"

/*
 * States of the writer.
 */
#define LEFW_UNINIT                   0
#define LEFW_INIT                     1
#define LEFW_ARRAY_START              2
#define LEFW_BEGINEXT_START           3
#define LEFW_CORRECTTABLE_START       4
#define LEFW_IRDROP_START             5
#define LEFW_LAYER_START              6
#define LEFW_LAYERROUTING_START       7
#define LEFW_LAYERROUTING_SP_START    8
#define LEFW_MACRO_START              9
#define LEFW_NOISETABLE_START        10
#define LEFW_NONDEFAULTRULE_START    11
#define LEFW_PROPERTYDEF_START       12
#define LEFW_SPACING_START           13
#define LEFW_UNITS_START             14
#define LEFW_VIA_START               15
#define LEFW_VIARULE_START           16
#define LEFW_VIARULEGEN_START        17
#define LEFW_ANTENNASIZE             18
#define LEFW_ARRAY                   19
#define LEFW_BEGINEXT                20
#define LEFW_BUSBITCHARS             21
#define LEFW_CASESENSITIVE           22
#define LEFW_CORRECTTABLE            23
#define LEFW_DIELECTRIC              24
#define LEFW_DIVIDERCHAR             25
#define LEFW_EDGERATE                26
#define LEFW_IRDROP                  27
#define LEFW_LAYER                   28
#define LEFW_LAYERACCURRENT          29
#define LEFW_LAYERDCCURRENT          30
#define LEFW_LAYERROUTING            31
#define LEFW_LAYERROUTINGWIDTH       32
#define LEFW_MACRO                   33
#define LEFW_MACRO_CAPACITANCE       34
#define LEFW_MACRO_INPUTNOISEMARGIN  35
#define LEFW_MACRO_FALLCURRENTSOURCE 36
#define LEFW_MACRO_FALLSATCUR        37
#define LEFW_MACRO_FALLTHRESH        38
#define LEFW_MACRO_FALLVOLTAGETHRESHOLD 39
#define LEFW_MACRO_IV_TABLES         40
#define LEFW_MACRO_LEAKAGE           41
#define LEFW_MACRO_OUTPUTNOISEMARGIN 42
#define LEFW_MACRO_OUTPUTRESISTANCE  43
#define LEFW_MACRO_POWER             44
#define LEFW_MACRO_PULLDOWNRES       45
#define LEFW_MACRO_RESISTANCE        46
#define LEFW_MACRO_RISESATCUR        47
#define LEFW_MACRO_RISETHRESH        48
#define LEFW_MACRO_RISEVOLTAGETHRESHOLD 49
#define LEFW_MACRO_TIEOFFR           50
#define LEFW_MACRO_TIMING            51
#define LEFW_MACRO_VHI               52
#define LEFW_MACRO_VLO               53
#define LEFW_MINFEATURE              54
#define LEFW_NONDEFAULTRULE          55
#define LEFW_NOISEMARGIN             56
#define LEFW_NOISETABLE              57
#define LEFW_NOWIREEXTATPIN          58
#define LEFW_PROPERTYDEF             59
#define LEFW_SCALEFACTOR             60
#define LEFW_SITE                    61
#define LEFW_SPACING                 62
#define LEFW_THRESHOLD1              63
#define LEFW_THRESHOLD2              64
#define LEFW_UNITS                   65
#define LEFW_VERSION                 66
#define LEFW_VIA                     67
#define LEFW_VIARULE                 68
#define LEFW_VIARULEGEN              69
#define LEFW_END                     70
#define LEFW_ARRAY_END               71
#define LEFW_BEGINEXT_END            72
#define LEFW_CORRECTTABLE_END        73
#define LEFW_IRDROP_END              74
#define LEFW_LAYER_END               75
#define LEFW_LAYERROUTING_END        76
#define LEFW_LAYERROUTING_SPACINGTABLE_END 77
#define LEFW_MACRO_END               78
#define LEFW_NOISETABLE_END          79
#define LEFW_NONDEFAULTRULE_END      80
#define LEFW_PROPERTYDEF_END         81
#define LEFW_SITE_END                82
#define LEFW_SPACING_END             83
#define LEFW_VIA_END                 84
#define LEFW_VIARULE_END             85
#define LEFW_VIARULEGEN_END          86
#define LEFW_UNITS_END               87

/*
 * 5.4
 *//* ANTENNA for LAYER */
#define LEFW_ANTENNAINPUTGATEAREA    88
#define LEFW_ANTENNAINOUTDIFFAREA    89
#define LEFW_ANTENNAOUTPUTDIFFAREA   90
#define LEFW_ANTENNAMODEL            91
#define LEFW_ANTENNAAREARATIO        92
#define LEFW_ANTENNADIFFAREARATIO    93
#define LEFW_ANTENNADIFFAREARATIOPWL 94
#define LEFW_ANTENNACUMAREARATIO     95
#define LEFW_ANTENNACUMDIFFAREARATIO 96
#define LEFW_ANTENNACUMDIFFAREARATIOPWL      97
#define LEFW_ANTENNAAREAFACTOR       98
#define LEFW_ANTENNASIDEAREARATIO    99
#define LEFW_ANTENNADIFFSIDEAREARATIO        100
#define LEFW_ANTENNADIFFSIDEAREARATIOPWL     101
#define LEFW_ANTENNACUMSIDEAREARATIO 102
#define LEFW_ANTENNACUMDIFFSIDEAREARATIO     103
#define LEFW_ANTENNACUMDIFFSIDEAREARATIOPWL  104
#define LEFW_ANTENNASIDEAREAFACTOR   105

/*
 * ANTENNA for MACRO PIN
 */
#define LEFW_ANTENNAPARTIALMETALAREA 106
#define LEFW_ANTENNAPARTIALMETALSIDEAREA     107
#define LEFW_ANTENNAGATEAREA         108
#define LEFW_ANTENNADIFFAREA         109
#define LEFW_ANTENNAMAXAREACAR       110
#define LEFW_ANTENNAMAXSIDEAREACAR   111
#define LEFW_ANTENNAPARTIALCUTAREA   112
#define LEFW_ANTENNAMAXCUTCAR        113
#define LEFW_CLEARANCEMEASURE        114
#define LEFW_DENSITYCHECKWINDOW      115
#define LEFW_DENSITYCHECKSTEP        116
#define LEFW_DESIGNRULEWIDTH         117
#define LEFW_FILLACTIVESPACING       118
#define LEFW_MANUFACTURINGGRID       119
#define LEFW_MAXADJACENTSLOTSPACING  120
#define LEFW_MAXCOAXIALSLOTSPACING   121
#define LEFW_MAXEDGESLOTSPACING      122
#define LEFW_MAXIMUMDENSITY          123
#define LEFW_MINIMUMDENSITY          124
#define LEFW_ROWABUTSPACING          125
#define LEFW_ROWMINSPACING           126
#define LEFW_SLOTWIREWIDTH           127
#define LEFW_SLOTWIRELENGTH          128
#define LEFW_SLOTWIDTH               129
#define LEFW_SLOTLENGTH              130
#define LEFW_SPLITWIREWIDTH          131
#define LEFW_USEMINSPACING           132
#define LEFW_DONE                    999
#define MAXSYN                       133

/*
 * Global Variables
 */ FILE *lefwFile = 0;		/* File to write to. */

int     lefwSynArray[MAXSYN];	/* array of syntax */

int     lefwLines = 0;		/* number of lines written */

int     lefwState = LEFW_UNINIT;/* Current state of writer */

int     lefwDidInit = 0;	/* required section */

int     lefwDidLayer = 0;	/* required section */

int     lefwDidVia = 0;		/* required section */

int     lefwDidViaRule = 0;	/* required section */

int     lefwDidViaSite = 0;	/* required section */

int     lefwDidViaMacro = 0;	/* required section */

int     lefwCounter = 0;	/* number of nets, components in section */

int     lefwLineItemCounter = 0;/* number of items on current line */

int     lefwHasArrayReq = 0;	/* array required data */

int     lefwIsArrayFloorp = 0;	/* array floorplan flag */

int     lefwIsArrayDef = 0;	/* array default cap flag */

int     lefwIsCorrectTable = 0;	/* correctiontable flag */

int     lefwIsCut = 0;		/* cut layer */

int     lefwIsEdgerate = 0;	/* edgerate within noisetable */

int     lefwIsFloorp = 0;	/* floorplan within array */

int     lefwIsImplant = 0;	/* implant layer */

int     lefwIsMacroObs = 0;	/* macro obs flag */

int     lefwIsMacroObsLayer = 0;/* macro obs layer within macro obs */

int     lefwIsMacroPin = 0;	/* macro pin flag */

int     lefwIsMacroPinPortLayer = 0;	/* macro pin port layer within macro
					 * pin port */

int     lefwIsMacroPinPort = 0;	/* macro pin port within macro pin */

int     lefwIsMacroTiming = 0;	/* macro timing flag */

int     lefwIsMacroTimingModel = 0;	/* macro timing model flag */

int     lefwIsMaxviastack = 0;	/* maximum stacked-via */

int     lefwIsNoiseTable = 0;	/* noisetable flag */

int     lefwIsNonDefaultRule = 0;	/* nondefaultrule flag */

int     lefwIsOutResist = 0;	/* outputResistance within edgerate */

int     lefwIsRouting = 0;	/* routing */

int     lefwIsRoutingMinCut = 0;/* routing Minimumcut */

int     lefwIsRoutingMinCutDist = 0;	/* routing Minimumcut distance */

int     lefwIsRoutingMinCutLen = 0;	/* routing Minimumcut length within */

int     lefwIsRoutingReqData = 0;	/* layer routing required data are
					 * provided */

int     lefwNumViaRuleLayers = 0;	/* number of via rule in a via rule */

int     lefwOldState = 0;	/* the previous state */

int     lefwTableLen = 0;	/* width or cutarea for the tableEntries */

int     lefwHasInit = 0;	/* for lefwInit has called */

int     lefwHasInitCbk = 0;	/* for lefwInitCbk has called */

static int lefwWriteEncrypt = 0;/* for writing out encrypted file, default is
				 * 0 */

static int prtSemiColon = 0;	/* sometimes ; is not printed yet */

static int prtNewLine = 0;	/* sometimes need to print a new line */

static int lefwHasLayerRoutingRange = 0;	/* mark if layer routing has
						 * range */

static int lefwHasLayerRoutingSpacing = 0;	/* layer routing is
						 * lengththreshold */

static int lefwAMaxAreaCar = 0;	/* only allow to define once per pin */

static int lefwAMaxSideAreaCar = 0;	/* only allow to define once per pin */

static int lefwAMaxCutCar = 0;	/* only allow to define once per pin */

static int lefwSpacingVal = 0;	/* either this or lefwWidthVal */

static int lefwWidthVal = 0;	/* is allowed */

static double versionNum = 5.5;	/* default to 5.5 */

static int lefwAntenna54 = 0;	/* to keep track 5.4 antenna is used */

static int lefwAntenna53 = 0;	/* to keep track 5.3 antenna is used */

static int lefwObsoleteNum = -1;/* keep track the obsolote syntax for error */

static int lefw54Num = -1;	/* keep track the 5.4 syntax for error */
char    lefwStateStr[MAXSYN][80] = {"UNITS", /* 0 */ "INIT", /* 1 */ "ARRAY", /* 2 */ "BEGINEXT", /* 3 */ "CORRECTTABLE", /* 4 */ "IRDROP", /* 5 */ "LAYER", /* 6 */ "LAYERROUTING", /* 7 */ "LAYERROUTINGSPACINGTABLE", /* 8 */ "MACRO", /* 9 */ "NOISETABLE", /* 10 */ "NONDEFAULTRULE", /* 11 */ "PROPERTYDEFINITION", /* 12 */ "SPACING", /* 13 */ "UNITS", /* 14 */ "VIA", /* 15 */ "VIARULE", /* 16 */ "VIARULEGEN", /* 17 */ "ANTENNASIZE", /* 18 */ "ARRAY", /* 19 */ "BEGINEXT", /* 20 */ "BUSBITCHARS", /* 21 */ "CASESENSITIVE", /* 22 */ "CORRECTABLE", /* 23 */ "DIELECTRIC", /* 24 */ "DIVIDERCHAR", /* 25 */ "EDGERATE", /* 26 */ "IRDROP", /* 27 */ "LAYER", /* 28 */ "LAYERACCURRENT", /* 29 */ "LAYERDCCURRENT", /* 30 */ "LAYERROUTING", /* 31 */ "LAYERROUTINGWIDTH", /* 32 */ "MACRO", /* 33 */ "MACRO_CAPACITANCE", /* 34 */ "MACRO_INPUTNOISEMARGIN", /* 35 */ "MACRO_FALLCURRENTSOURCE", /* 36 */ "MACRO_FALLSATCUR", /* 37 */ "MACRO_FALLTHRESH", /* 38 */ "MACRO_FALLVOLTAGETHRESHOLD", /* 39 */ "MACRO_IV_TABLES", /* 40 */ "MACRO_LEAKAGE", /* 41 */ "MACRO_OUTPUTNOISEMARGIN", /* 42 */ "MACRO_OUTPUTRESISTANCE", /* 43 */ "MACRO_POWER", /* 44 */ "MACRO_PULLDOWNRES", /* 45 */ "MACRO_RESISTANCE", /* 46 */ "MACRO_RISESATCUR", /* 47 */ "MACRO_RISETHRESH", /* 48 */ "MACRO_RISEVOLTAGETHRESHOLD", /* 49 */ "MACRO_TIEOFFR", /* 50 */ "MACRO_TIMING", /* 51 */ "MACRO_VHI", /* 52 */ "MACRO_VLO", /* 53 */ "MINFEATURE", /* 54 */ "NONDEFAULTRULE", /* 55 */ "NOISEMARGIN", /* 56 */ "NOISETABLE", /* 57 */ "NOWIREEXTSIONATPIN", /* 58 */ "PROPERTYDEFINITION", /* 59 */ "EDGERATESCALEFACTOR", /* 60 */ "SITE", /* 61 */ "SPACING", /* 62 */ "EDGERATETHRESHOLD1", /* 63 */ "EDGERATETHRESHOLD2", /* 64 */ "UNTIS", /* 65 */ "VERSION", /* 66 */ "VIA", /* 67 */ "VIARULE", /* 68 */ "VIARULEGEN", /* 69 */ "END", /* 70 */ "ARRAY", /* 71 */ "BEGINEXT", /* 72 */ "CORRECTABLE", /* 73 */ "IRDROP", /* 74 */ "LAYER", /* 75 */ "LAYERROUTING", /* 76 */ "LAYERROUTINGSPACINGTABLE", /* 77 */ "MACRO", /* 78 */ "NOISETABLE", /* 79 */ "NONDEFAULTRULE", /* 80 */ "PROERTYDEFINITION", /* 81 */ "SITE", /* 82 */ "SPACING", /* 83 */ "VIA", /* 84 */ "VIARULE", /* 85 */ "VIARULEGEN", /* 86 */ "UNITS", /* 87 */ "ANTENNAINPUTGATEAREA", /* 88 */ "ANTENNAINOUTDIFFAREA", /* 89 */ "ANTENNAOUTPUTDIFFAREA", /* 90 */ "ANTENNAMODEL", /* 91 */ "ANTENNAAREARATIO", /* 92 */ "ANTENNADIFFAREARATIO", /* 93 */ "ANTENNADIFFAREARATIO PWL", /* 94 */ "ANTENNACUMAREARATIO", /* 95 */ "ANTENNACUMDIFFAREARATIO", /* 96 */ "ANTENNACUMDIFFAREARATIO PWL", /* 97 */ "ANTENNAAREAFACTOR", /* 98 */ "ANTENNASIDEAREARATIO", /* 99 */ "ANTENNADIFFSIDEAREARATIO", /* 100 */ "ANTENNADIFFSIDEAREARATIO PWL", /* 101 */ "ANTENNACUMSIDEAREARATIO", /* 102 */ "ANTENNACUMDIFFSIDEAREARATIO", /* 103 */ "ANTENNACUMDIFFSIDEAREARATIO PWL", /* 104 */ "ANTENNASIDEAREAFACTOR", /* 105 */ "ANTENNAPARTIALMETALAREA", /* 106 */ "ANTENNAPARTIALMETALSIDEAREA", /* 107 */ "ANTENNAGATEAREA", /* 108 */ "ANTENNADIFFAREA", /* 109 */ "ANTENNAMAXAREACAR", /* 110 */ "ANTENNAMAXSIDEAREACAR", /* 111 */ "ANTENNAPARTIALCUTAREA", /* 112 */ "ANTENNAMAXCUTCAR", /* 113 */ "CLEARANCEMEASURE", /* 114 */ "DENSITYCHECKWINDOW", /* 115 */ "DENSITYCHECKSTEP", /* 116 */ "DESIGNRULEWIDTH", /* 117 */ "FILLACTIVESPACING", /* 118 */ "MANUFACTURINGGRID", /* 119 */ "MAXADJACENTSLOTSPACING", /* 120 */ "MAXCOAXIALSLOTSPACING", /* 121 */ "MAXEDGESLOTSPACING", /* 122 */ "MAXIMUMDENSITY", /* 123 */ "MINIMUMDENSITY", /* 124 */ "ROWABUTSPACING", /* 125 */ "ROWMINSPACING", /* 126 */ "SLOTWIREWIDTH", /* 127 */ "SLOTWIRELENGTH", /* 128 */ "SLOTWIDTH", /* 129 */ "SLOTLENGTH", /* 130 */ "SPLITWIREWIDTH", /* 131 */ "USEMINSPACING" /* 132 */ };

/*
 * internal function
 */ char *
lefwAddr(const char *x)
{
  return (char *) x;
}

/*
 * internal function
 */ char *
lefwOrient(int num)
{
  switch (num) {
    case 0:return lefwAddr("N");
  case 1:
    return lefwAddr("W");
  case 2:
    return lefwAddr("S");
  case 3:
    return lefwAddr("E");
  case 4:
    return lefwAddr("FN");
  case 5:
    return lefwAddr("FW");
  case 6:
    return lefwAddr("FS");
  case 7:
    return lefwAddr("FE");
  };
  return lefwAddr("BOGUS");
}

/*
 * this function is required to be called first to initialize the array
 *//* Either this function or lefwInitCbk can be called, cannot be both */ int 
lefwInit(FILE * f)
{
  int     i;

  /* Check if the user is valid  */

  /*
   * don't need anymore if (!lefiValidUser()) exit(77);
   */

  /*
   * if (lefwFile) return LEFW_BAD_ORDER;
   */ lefwFile = f;
  if (lefwHasInitCbk == 1) {	/* lefwInitCbk has already called, issue an
				 * error */
    fprintf(lefwFile,
    "ERROR: lefwInitCbk has already called, cannot call lefwInit again.\n");
    fprintf(lefwFile, "Writer Exit.\n");
    exit(LEFW_BAD_ORDER);
  }
  lefwState = LEFW_UNINIT;	/* Current state of writer */
  lefwDidInit = 0;		/* required section */
  for (i = 0; i < MAXSYN; i++)
    lefwSynArray[i] = 0;
  lefwDidInit = 1;
  lefwState = LEFW_INIT;
  lefwHasInit = 1;
  return LEFW_OK;
}

/*
 * this function is required to be called first to initialize the array
 *//* Either this function or lefwInit can be called, cannot be both */ int 
lefwInitCbk(FILE * f)
{
  int     i;

  /*
   * if (lefwFile) return LEFW_BAD_ORDER;
   */ lefwFile = f;
  if (lefwHasInit == 1) {	/* lefwInit has already called, issue an
				 * error */
    if (lefwWriteEncrypt) {
      encPrint(lefwFile, (char *)
	       "ERROR: lefwInit has already called, cannot call lefwInitCbk again.\n");
      encPrint(lefwFile, (char *) "Writer Exit.\n");
    }
    else {
      fprintf(lefwFile,
	      "ERROR: lefwInit has already called, cannot call lefwInitCbk again.\n");
      fprintf(lefwFile, "Writer Exit.\n");
    }
    exit(LEFW_BAD_ORDER);
  }
  lefwState = LEFW_UNINIT;	/* Current state of writer */
  lefwDidInit = 0;		/* required section */
  for (i = 0; i < MAXSYN; i++)
    lefwSynArray[i] = 0;
  lefwDidInit = 1;
  lefwState = LEFW_INIT;
  lefwHasInitCbk = 1;
  return LEFW_OK;
}

/*
 * This function sets the global variable lefwEncrypt to 1, which will
 *//* write out an output encrypted file, make sure to call lefwCloseEncrypt *//* b
    efore closing the output file */ int 
lefwEncrypt()
{
  if (!lefwFile || (lefwFile == stdout)) {

    /*
     * No output filename is given
     */ fprintf(stderr, "Need an output file if writing in encrypted format.\n");
    return LEFW_BAD_ORDER;
  }

  /*
   * Set the flag in crypt.cpp
   */ (void) encWritingEncrypted();
  lefwWriteEncrypt = 1;
  return LEFW_OK;
}

/*
 * This function needs to be called if lefwEncrypt is called
 *//* It will clear the encrypted buffer */ int 
lefwCloseEncrypt()
{
  if (lefwWriteEncrypt)
    encClearBuf(lefwFile);
  return LEFW_OK;
}

int 
lefwNewLine()
{
  if (!lefwFile)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "\n");
  else
    fprintf(lefwFile, "\n");
  return LEFW_OK;
}

int 
lefwVersion(int vers1, int vers2)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_VERSION])
    return LEFW_ALREADY_DEFINED;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "VERSION %d.%d ;\n", vers1, vers2);
  else
    fprintf(lefwFile, "VERSION %d.%d ;\n", vers1, vers2);
  if (vers2 >= 10)
    versionNum = vers1 + (vers2 / 100.0);
  else
    versionNum = vers1 + (vers2 / 10.0);
  lefwSynArray[LEFW_VERSION] = 1;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwCaseSensitive(const char *caseSensitive)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_CASESENSITIVE])
    return LEFW_ALREADY_DEFINED;
  if (strcmp(caseSensitive, "ON") && strcmp(caseSensitive, "OFF"))
    return LEFW_BAD_DATA;	/* has to be either ON or OFF */
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "NAMESCASESENSITIVE %s ;\n", caseSensitive);
  else
    fprintf(lefwFile, "NAMESCASESENSITIVE %s ;\n", caseSensitive);
  lefwSynArray[LEFW_CASESENSITIVE] = 1;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwNoWireExtensionAtPin(const char *noWireExt)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_NOWIREEXTATPIN])
    return LEFW_ALREADY_DEFINED;
  if (strcmp(noWireExt, "ON") && strcmp(noWireExt, "OFF"))
    return LEFW_BAD_DATA;	/* has to be either ON or OFF */
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "NOWIREEXTENSIONATPIN %s ;\n", noWireExt);
  else
    fprintf(lefwFile, "NOWIREEXTENSIONATPIN %s ;\n", noWireExt);
  lefwSynArray[LEFW_NOWIREEXTATPIN] = 1;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMinfeature(double minFeatureX, double minFeatureY)
{
  lefwObsoleteNum = LEFW_DIELECTRIC;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwSynArray[LEFW_MINFEATURE])
    return LEFW_ALREADY_DEFINED;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "MINFEATURE %g %g ;\n", minFeatureX, minFeatureY);
  else
    fprintf(lefwFile, "MINFEATURE %g %g ;\n", minFeatureX, minFeatureY);
  lefwSynArray[LEFW_MINFEATURE] = 1;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwDielectric(double dielectric)
{
  lefwObsoleteNum = LEFW_DIELECTRIC;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwSynArray[LEFW_DIELECTRIC])
    return LEFW_ALREADY_DEFINED;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "DIELECTRIC %g ;\n", dielectric);
  else
    fprintf(lefwFile, "DIELECTRIC %g ;\n", dielectric);
  lefwSynArray[LEFW_DIELECTRIC] = 1;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwBusBitChars(const char *busBitChars)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_BUSBITCHARS])
    return LEFW_ALREADY_DEFINED;
  if (busBitChars && busBitChars != 0 && *busBitChars != 0) {
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) "BUSBITCHARS \"%s\" ;\n", busBitChars);
    else
      fprintf(lefwFile, "BUSBITCHARS \"%s\" ;\n", busBitChars);
  }
  lefwLines++;
  lefwSynArray[LEFW_BUSBITCHARS] = 1;
  return LEFW_OK;
}

int 
lefwDividerChar(const char *dividerChar)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_DIVIDERCHAR])
    return LEFW_ALREADY_DEFINED;
  if (dividerChar && dividerChar != 0 && *dividerChar != 0) {
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) "DIVIDERCHAR \"%s\" ;\n", dividerChar);
    else
      fprintf(lefwFile, "DIVIDERCHAR \"%s\" ;\n", dividerChar);
  }
  lefwLines++;
  lefwSynArray[LEFW_DIVIDERCHAR] = 1;
  return LEFW_OK;
}

int 
lefwManufacturingGrid(double grid)
{
  lefw54Num = LEFW_MANUFACTURINGGRID;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_MANUFACTURINGGRID])
    return LEFW_ALREADY_DEFINED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "MANUFACTURINGGRID %g ;\n", grid);
  else
    fprintf(lefwFile, "MANUFACTURINGGRID %g ;\n", grid);
  lefwLines++;
  lefwSynArray[LEFW_MANUFACTURINGGRID] = 1;
  return LEFW_OK;
}

int 
lefwUseMinSpacing(const char *type, const char *onOff)
{
  lefw54Num = LEFW_USEMINSPACING;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs *//* Can define more than
   * once *//* if (lefwSynArray[LEFW_USEMINSPACING]) return
   * LEFW_ALREADY_DEFINED; */
  if (!type || (strcmp(type, "OBS") && strcmp(type, "PIN")))
    return LEFW_BAD_DATA;	/* has to be OBS or PIN */
  if (!onOff || (strcmp(onOff, "ON") && strcmp(onOff, "OFF")))
    return LEFW_BAD_DATA;	/* has to be ON or OFF */
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "USEMINSPACING %s %s ;\n", type, onOff);
  else
    fprintf(lefwFile, "USEMINSPACING %s %s ;\n", type, onOff);
  lefwLines++;

  /*
   * lefwSynArray[LEFW_USEMINSPACING] = 1;
   */ return LEFW_OK;
}

int 
lefwClearanceMeasure(const char *type)
{
  lefw54Num = LEFW_CLEARANCEMEASURE;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_CLEARANCEMEASURE])
    return LEFW_ALREADY_DEFINED;
  if (!type || (strcmp(type, "MAXXY") && strcmp(type, "EUCLIDEAN")))
    return LEFW_BAD_DATA;	/* has to be MAXXY or EUCLIDEAN */
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "CLEARANCEMEASURE %s ;\n", type);
  else
    fprintf(lefwFile, "CLEARANCEMEASURE %s ;\n", type);
  lefwLines++;
  lefwSynArray[LEFW_CLEARANCEMEASURE] = 1;
  return LEFW_OK;
}

int 
lefwStartUnits()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwSynArray[LEFW_UNITS_START])
    return LEFW_ALREADY_DEFINED;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "UNITS\n");
  else
    fprintf(lefwFile, "UNITS\n");
  lefwLines++;
  lefwSynArray[LEFW_UNITS_START] = 1;
  lefwState = LEFW_UNITS_START;
  return LEFW_OK;
}

int 
lefwUnits(double time,		/* optional  */
	  double capacitance,	/* optional  */
	  double resistance,	/* optional  */
	  double power,		/* optional  */
	  double current,	/* optional  */
	  double voltage,	/* optional  */
	  double database	/* optional  */
)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_UNITS_START &&
      lefwState != LEFW_UNITS)
    return LEFW_BAD_ORDER;
  if (time) {
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) " TIME NANOSECONDS %g ;\n", time);
    else
      fprintf(lefwFile, " TIME NANOSECONDS %g ;\n", time);
  }
  if (capacitance) {
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) " CAPACITANCE PICOFARADS %g ;\n", capacitance);
    else
      fprintf(lefwFile, " CAPACITANCE PICOFARADS %g ;\n", capacitance);
  }
  if (resistance) {
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) " RESISTANCE OHMS %g ;\n", resistance);
    else
      fprintf(lefwFile, " RESISTANCE OHMS %g ;\n", resistance);
  }
  if (power) {
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) " POWER MILLIWATTS %g ;\n", power);
    else
      fprintf(lefwFile, " POWER MILLIWATTS %g ;\n", power);
  }
  if (current) {
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) " CURRENT MILLIAMPS %g ;\n", current);
    else
      fprintf(lefwFile, " CURRENT MILLIAMPS %g ;\n", current);
  }
  if (voltage) {
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) " VOLTAGE VOLTS %g ;\n", voltage);
    else
      fprintf(lefwFile, " VOLTAGE VOLTS %g ;\n", voltage);
  }
  if (database) {
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) " DATABASE MICRONS %g ;\n", database);
    else
      fprintf(lefwFile, " DATABASE MICRONS %g ;\n", database);
  }
  lefwState = LEFW_UNITS;
  return LEFW_OK;
}

int 
lefwUnitsFrequency(double frequency)
{				/* required since only by itself  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_UNITS_START &&
      lefwState != LEFW_UNITS)
    return LEFW_BAD_ORDER;
  if (!frequency)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " FREQUENCY MEGAHERTZ %g ;\n", frequency);
  else
    fprintf(lefwFile, " FREQUENCY MEGAHERTZ %g ;\n", frequency);
  lefwState = LEFW_UNITS;
  return LEFW_OK;
}

int 
lefwEndUnits()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_UNITS_START &&
      lefwState != LEFW_UNITS)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END UNITS\n\n");
  else
    fprintf(lefwFile, "END UNITS\n\n");
  lefwLines++;
  lefwState = LEFW_UNITS_END;
  return LEFW_OK;
}

int 
lefwStartLayer(const char *layerName, const char *type)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_LAYER_START ||
      lefwState == LEFW_LAYER)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwIsMaxviastack)
    return LEFW_BAD_ORDER;	/* Layer after maximum stacked-via */
  if (strcmp(type, "CUT") == 0)
    lefwIsCut = 1;
  else if (strcmp(type, "IMPLANT") == 0)	/* IMPLANT is 5.5 syntax */
    lefwIsImplant = 1;
  else if (strcmp(type, "MASTERSLICE") && strcmp(type, "OVERLAP") &&
	   strcmp(type, "IMPLANT"))	/* IMPLANT is 5.5 syntax */
    return LEFW_BAD_DATA;	/* type is CUT, MASTERSLICE or OVERLAP only */
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) "LAYER %s\n", layerName);
    encPrint(lefwFile, (char *) " TYPE %s ;\n", type);
  }
  else {
    fprintf(lefwFile, "LAYER %s\n", layerName);
    fprintf(lefwFile, " TYPE %s ;\n", type);
  }
  lefwIsRouting = 0;
  lefwLines = lefwLines + 2;
  lefwState = LEFW_LAYER_START;
  return LEFW_OK;
}

int 
lefwLayer(double spacing,	/* optional  */
	  const char *name2)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYER_START &&
      lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;
  if ((!lefwIsCut) && (!lefwIsImplant))
    return LEFW_BAD_DATA;

  /*
   * only cut & implant call this func
   */ if (spacing) {
    if (lefwWriteEncrypt) {
      encPrint(lefwFile, (char *) " SPACING %g ", spacing);
      if (name2)
	encPrint(lefwFile, (char *) "LAYER %s ", name2);
      encPrint(lefwFile, (char *) ";\n");
    }
    else {
      fprintf(lefwFile, " SPACING %g ", spacing);
      if (name2)
	fprintf(lefwFile, "LAYER %s ", name2);
      fprintf(lefwFile, ";\n");
    }
  }
  else if (name2)
    return LEFW_BAD_ORDER;
  lefwLines++;
  lefwState = LEFW_LAYER;
  return LEFW_OK;
}

int 
lefwLayerWidth(double minWidth)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYER_START &&
      lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;
  if (!lefwIsImplant)
    return LEFW_BAD_DATA;	/* use cut for implant for now */
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " WIDTH %g ;\n", minWidth);
  else
    fprintf(lefwFile, " WIDTH %g ;\n", minWidth);
  lefwLines++;
  lefwState = LEFW_LAYER;
  return LEFW_OK;
}

int 
lefwLayerSpacingAdjacent(double spacing,	/* optional  */
			 double distance)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYER_START &&
      lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;
  if (!lefwIsCut)
    return LEFW_BAD_DATA;	/* only cut calls this func */
  if (lefwIsMaxviastack)
    return LEFW_BAD_ORDER;	/* Layer after maximum stacked-via */
  if (spacing) {
    if (lefwWriteEncrypt) {
      encPrint(lefwFile, (char *) " SPACING %g ", spacing);
      if (distance)
	encPrint(lefwFile, (char *) "ADJACENTWITHIN %g ", distance);
      encPrint(lefwFile, (char *) ";\n");
    }
    else {
      fprintf(lefwFile, " SPACING %g ", spacing);
      if (distance)
	fprintf(lefwFile, "ADJACENTWITHIN %g ", distance);
      fprintf(lefwFile, ";\n");
    }
  }
  else if (distance)
    return LEFW_BAD_ORDER;
  lefwLines++;
  lefwState = LEFW_LAYER;
  return LEFW_OK;
}

int 
lefwEndLayer(const char *layerName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYER_START &&
      lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END %s\n\n", layerName);
  else
    fprintf(lefwFile, "END %s\n\n", layerName);
  lefwLines++;
  lefwState = LEFW_LAYER_END;
  lefwIsCut = 0;
  lefwIsImplant = 0;
  return LEFW_OK;
}

int 
lefwStartLayerRouting(const char *layerName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_LAYERROUTING_START ||
      lefwState == LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) "LAYER %s\n", layerName);
    encPrint(lefwFile, (char *) " TYPE ROUTING ;\n");
  }
  else {
    fprintf(lefwFile, "LAYER %s\n", layerName);
    fprintf(lefwFile, " TYPE ROUTING ;\n");
  }
  lefwIsRouting = 1;
  lefwIsRoutingReqData = 0;
  lefwLines = lefwLines + 2;
  lefwState = LEFW_LAYERROUTING_START;
  lefwSynArray[LEFW_LAYERROUTING_START] = 1;
  return LEFW_OK;
}

int 
lefwLayerRouting(const char *direction,
		 double width)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func *//* this
				 * routine should call only once per layer
				 * routing */
  if (lefwSynArray[LEFW_LAYERROUTING])
    return LEFW_ALREADY_DEFINED;
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt) {
    if (!direction || direction == 0 || *direction == 0) {
      encPrint(lefwFile, (char *) "DIRECTION is required in Layer(Routing).\n");
      return LEFW_BAD_DATA;
    }
    if (strcmp(direction, "HORIZONTAL") && strcmp(direction, "VERTICAL")) {
      encPrint(lefwFile, (char *)
       "DIRECTION in Layer(Routing) can only be HORIZONTAL or VERTICAL.\n");
      return LEFW_BAD_DATA;	/* direction is either HORIZONTAL or VERTICAL
				 * only */
    }
    encPrint(lefwFile, (char *) " DIRECTION %s ;\n", direction);
    encPrint(lefwFile, (char *) " WIDTH %g ;\n", width);
  }
  else {
    if (!direction || direction == 0 || *direction == 0) {
      fprintf(lefwFile, "DIRECTION is required in Layer(Routing).\n");
      return LEFW_BAD_DATA;
    }
    if (strcmp(direction, "HORIZONTAL") && strcmp(direction, "VERTICAL")) {
      fprintf(lefwFile,
       "DIRECTION in Layer(Routing) can only be HORIZONTAL or VERTICAL.\n");
      return LEFW_BAD_DATA;	/* direction is either HORIZONTAL or VERTICAL
				 * only */
    }
    fprintf(lefwFile, " DIRECTION %s ;\n", direction);
    fprintf(lefwFile, " WIDTH %g ;\n", width);
  }
  lefwLines += 2;
  lefwIsRoutingReqData = 1;
  lefwSynArray[LEFW_LAYERROUTING] = 1;
  lefwIsRoutingMinCut = 0;
  return LEFW_OK;
}

int 
lefwLayerRoutingPitch(double pitch)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }

  /*
   * this routine should call only once per layer routing
   *//* it is required for SE only, other is optional */ if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " PITCH %g ;\n", pitch);
  else
    fprintf(lefwFile, " PITCH %g ;\n", pitch);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingOffset(double offset)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " OFFSET %g ;\n", offset);
  else
    fprintf(lefwFile, " OFFSET %g ;\n", offset);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingArea(double area)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " AREA %g ;\n", area);
  else
    fprintf(lefwFile, " AREA %g ;\n", area);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingMinimumcut(double numCuts, double minWidth)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " MINIMUMCUT %g WIDTH %g ", numCuts,
	     minWidth);
  else
    fprintf(lefwFile, " MINIMUMCUT %g WIDTH %g ", numCuts, minWidth);
  lefwLines++;
  prtSemiColon = 1;
  lefwIsRoutingMinCut = 1;
  lefwIsRoutingMinCutDist = 0;
  lefwIsRoutingMinCutLen = 0;
  return LEFW_OK;
}

int 
lefwLayerRoutingMinimumcutConnections(const char *direction)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwIsRoutingMinCut || lefwIsRoutingMinCutDist)
    return LEFW_BAD_ORDER;

  /*
   * lefwLayerRoutingMinimumcut has not being called or
   *//* lefwLayerRoutingMinimumcutConnections has already called */ if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "\n %s ", direction);
  else
    fprintf(lefwFile, "\n %s ", direction);
  lefwLines++;
  prtSemiColon = 1;
  lefwIsRoutingMinCutDist = 1;
  if (lefwIsRoutingMinCutLen)	/* both distance & length within has defined */
    lefwIsRoutingMinCut = 0;
  return LEFW_OK;
}

int 
lefwLayerRoutingMinimumcutLengthWithin(double length, double distance)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwIsRoutingMinCut || lefwIsRoutingMinCutLen)
    return LEFW_BAD_ORDER;

  /*
   * lefwLayerRoutingMinimumcut has not being called or
   *//* lefwLayerRoutingMinimumcutLengthWithin has already called */ if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "\n LENGTH %g WITHIN %g ", length, distance);
  else
    fprintf(lefwFile, "\n LEGNTH %g WITHIN %g ", length, distance);
  lefwLines++;
  prtSemiColon = 1;
  lefwIsRoutingMinCutLen = 1;
  if (lefwIsRoutingMinCutDist)	/* both distance & length within has defined */
    lefwIsRoutingMinCut = 0;
  return LEFW_OK;
}

int 
lefwLayerRoutingSpacing(double spacing)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SPACING %g ", spacing);
  else
    fprintf(lefwFile, " SPACING %g ", spacing);
  lefwLines++;
  lefwHasLayerRoutingRange = 0;	/* initialization */
  lefwHasLayerRoutingSpacing = 1;	/* initialization */
  prtNewLine = 1;
  prtSemiColon = 1;
  return LEFW_OK;
}

int 
lefwLayerRoutingSpacingRange(double leftRange, double rightRange)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (!lefwHasLayerRoutingSpacing)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "RANGE %g %g ", leftRange, rightRange);
  else
    fprintf(lefwFile, "RANGE %g %g ", leftRange, rightRange);
  lefwHasLayerRoutingRange = 1;	/* initialization */
  lefwLines++;
  prtNewLine = 1;
  prtSemiColon = 1;
  return LEFW_OK;
}

int 
lefwLayerRoutingSpacingRangeUseLengthThreshold()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (!lefwHasLayerRoutingRange) {

    /*
     * This routine can be called only if range are non zero
     */ fprintf(lefwFile,
		"ERROR: lefwLayerRoutingSpacingUseLengthThreshold cannot be called if\n");
    fprintf(lefwFile,
	    " lefwLayerRoutingSpacingRange has not been called.\n");
    return LEFW_BAD_DATA;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "\n USELENGTHTHRESHOLD ");
  else
    fprintf(lefwFile, "\n USELENGTHTHRESHOLD ");
  prtSemiColon = 1;
  prtNewLine = 1;
  lefwHasLayerRoutingRange = 0;	/* reset to 0 */
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingSpacingRangeInfluence(double infValue, double subMinWidth,
				      double subMaxWidth)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (!lefwHasLayerRoutingRange) {

    /*
     * This routine can be called only if range are non zero
     */ fprintf(lefwFile,
	   "ERROR: lefwLayerRoutingSpacingInfluence cannot be called if\n");
    fprintf(lefwFile,
    " lefRange and rightRange in lefwLayerRoutingSpacing are both zero.\n");
    return LEFW_BAD_DATA;
  }
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) "\n INFLUENCE %g ", infValue);
    if (subMinWidth || subMaxWidth)
      encPrint(lefwFile, (char *) "RANGE %g %g ", subMinWidth, subMaxWidth);
  }
  else {
    fprintf(lefwFile, "\n INFLUENCE %g ", infValue);
    if (subMinWidth || subMaxWidth)
      fprintf(lefwFile, (char *) "RANGE %g %g ", subMinWidth, subMaxWidth);
  }
  prtNewLine = 1;
  prtSemiColon = 1;
  lefwHasLayerRoutingRange = 0;	/* reset to 0 */
  return LEFW_OK;
}

int 
lefwLayerRoutingSpacingRangeRange(double minWidth, double maxWidth)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (!lefwHasLayerRoutingRange) {

    /*
     * This routine can be called only if range are non zero
     */ fprintf(lefwFile,
	   "ERROR: lefwLayerRoutingSpacingInfluence cannot be called if\n");
    fprintf(lefwFile,
    " lefRange and rightRange in lefwLayerRoutingSpacing are both zero.\n");
    return LEFW_BAD_DATA;
  }
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) "RANGE %g %g ", minWidth, maxWidth);
  }
  else {
    fprintf(lefwFile, (char *) "RANGE %g %g ", minWidth, maxWidth);
  }
  prtNewLine = 1;
  prtSemiColon = 1;
  lefwHasLayerRoutingRange = 0;	/* reset to 0 */
  return LEFW_OK;
}

int 
lefwLayerRoutingSpacingLengthThreshold(double lengthValue,
				       double minWidth, double maxWidth)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (!lefwHasLayerRoutingSpacing)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " LENGTHTHRESHOLD %g ", lengthValue);
    if (minWidth || maxWidth) {
      encPrint(lefwFile, (char *) "RANGE %g %g ", minWidth, maxWidth);
    }
  }
  else {
    fprintf(lefwFile, " LENGTHTHRESHOLD %g ", lengthValue);
    if (minWidth || maxWidth) {
      fprintf(lefwFile, "RANGE %g %g ", minWidth, maxWidth);
    }
  }
  prtSemiColon = 1;
  return LEFW_OK;
}

int 
lefwLayerRoutingWireExtension(double wireExtension)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " WIREEXTENSION %g ;\n", wireExtension);
  else
    fprintf(lefwFile, " WIREEXTENSION %g ;\n", wireExtension);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingResistance(const char *resistance)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (!resistance || resistance == 0 || *resistance == 0)
    return LEFW_BAD_DATA;
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " RESISTANCE RPERSQ %s ;\n", resistance);
  else
    fprintf(lefwFile, " RESISTANCE RPERSQ %s ;\n", resistance);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingCapacitance(const char *capacitance)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (!capacitance || capacitance == 0 || *capacitance == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " CAPACITANCE CPERSQDIST %s ;\n", capacitance);
  else
    fprintf(lefwFile, " CAPACITANCE CPERSQDIST %s ;\n", capacitance);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingHeight(double height)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " HEIGHT %g ;\n", height);
  else
    fprintf(lefwFile, " HEIGHT %g ;\n", height);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingThickness(double thickness)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " THICKNESS %g ;\n", thickness);
  else
    fprintf(lefwFile, " THICKNESS %g ;\n", thickness);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingShrinkage(double shrinkage)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SHRINKAGE %g ;\n", shrinkage);
  else
    fprintf(lefwFile, " SHRINKAGE %g ;\n", shrinkage);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingCapMultiplier(double capMultiplier)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " CAPMULTIPLIER %g ;\n", capMultiplier);
  else
    fprintf(lefwFile, " CAPMULTIPLIER %g ;\n", capMultiplier);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingEdgeCap(double edgeCap)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " EDGECAPACITANCE %g ;\n", edgeCap);
  else
    fprintf(lefwFile, " EDGECAPACITANCE %g ;\n", edgeCap);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingAntennaArea(double antennaArea)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ANTENNAAREAFACTOR %g ;\n", antennaArea);
  else
    fprintf(lefwFile, " ANTENNAAREAFACTOR %g ;\n", antennaArea);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingAntennaLength(double antennaLength)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ANTENNALENGTHFACTOR %g ;\n", antennaLength);
  else
    fprintf(lefwFile, " ANTENNALENGTHFACTOR %g ;\n", antennaLength);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingMaxwidth(double width)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " MAXWIDTH %g ;\n", width);
  else
    fprintf(lefwFile, " MAXWIDTH %g ;\n", width);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingMinwidth(double width)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " MINWIDTH %g ;\n", width);
  else
    fprintf(lefwFile, " MINWIDTH %g ;\n", width);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingMinenclosedarea(int numMinenclosed, double *area,
				double *width)
{
  int     i;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt) {
    for (i = 0; i < numMinenclosed; i++) {
      encPrint(lefwFile, (char *) " MINENCLOSEDAREA %g ", area[i]);
      if (width[i] != 0)
	encPrint(lefwFile, (char *) "WIDTH %g ", width[i]);
      encPrint(lefwFile, (char *) ";\n");
    }
  }
  else {
    for (i = 0; i < numMinenclosed; i++) {
      fprintf(lefwFile, " MINENCLOSEDAREA %g ", area[i]);
      if (width[i] != 0)
	fprintf(lefwFile, "WIDTH %g ", width[i]);
      fprintf(lefwFile, ";\n");
    }
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingMinstep(double distance)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " MINSTEP %g ;\n", distance);
  else
    fprintf(lefwFile, " MINSTEP %g ;\n", distance);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingProtrusion(double width1, double length, double width2)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRouting)
    return LEFW_BAD_DATA;	/* only routing calls this func */
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " PROTRUSIONWIDTH %g LENGTH %g WIDTH %g ;\n",
	     width1, length, width2);
  else
    fprintf(lefwFile, " PROTRUSIONWIDTH %g LENGTH %g WIDTH %g ;\n",
	    width1, length, width2);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingStartSpacingtableParallel(int numLength, double *length)
{
  int     i;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRoutingReqData) {
    return LEFW_BAD_ORDER;
  }
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " SPACINGTABLE\n");
    encPrint(lefwFile, (char *) " PARALLELRUNLENGTH");
    for (i = 0; i < numLength; i++)
      encPrint(lefwFile, (char *) " %g", length[i]);
  }
  else {
    fprintf(lefwFile, " SPACINGTABLE\n");
    fprintf(lefwFile, " PARALLELRUNLENGTH");
    for (i = 0; i < numLength; i++)
      fprintf(lefwFile, " %g", length[i]);
  }
  lefwState = LEFW_LAYERROUTING_SP_START;
  lefwLines += 2;
  return LEFW_OK;
}

int 
lefwLayerRoutingSpacingtableParallelWidth(double width,
					  int numSpacing, double *spacing)
{
  int     i;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_SP_START &&
      lefwState != LEFW_LAYERROUTINGWIDTH)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) "\n WIDTH %g", width);
    for (i = 0; i < numSpacing; i++)
      encPrint(lefwFile, (char *) " %g", spacing[i]);
  }
  else {
    fprintf(lefwFile, "\n WIDTH %g", width);
    for (i = 0; i < numSpacing; i++)
      fprintf(lefwFile, " %g", spacing[i]);
  }
  lefwState = LEFW_LAYERROUTINGWIDTH;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingStartSpacingtableInfluence()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRoutingReqData) {
    return LEFW_BAD_ORDER;
  }
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " SPACINGTABLE\n");
    encPrint(lefwFile, (char *) " INFLUENCE");
  }
  else {
    fprintf(lefwFile, " SPACINGTABLE\n");
    fprintf(lefwFile, " INFLUENCE");
  }
  lefwState = LEFW_LAYERROUTINGWIDTH;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutingSpacingInfluenceWidth(double width, double distance,
				      double spacing)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTINGWIDTH)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "\n WIDTH %g WITHIN %g SPACING %g", width,
	     distance, spacing);
  else
    fprintf(lefwFile, "\n WIDTH %g WITHIN %g SPACING %g", width,
	    distance, spacing);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerRoutineEndSpacingtable()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTINGWIDTH)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ;\n");
  else
    fprintf(lefwFile, " ;\n");
  lefwLines++;
  lefwState = LEFW_LAYERROUTING;
  return LEFW_OK;
}

int 
lefwEndLayerRouting(const char *layerName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START &&
      lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;
  if (!lefwIsRoutingReqData) {
    return LEFW_BAD_ORDER;
  }
  if (prtSemiColon) {

    /*
     * the previous statement hasn't written the ; yet
     */ if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) ";\n");
    else
      fprintf(lefwFile, ";\n");
    prtSemiColon = 0;
  }
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END %s\n\n", layerName);
  else
    fprintf(lefwFile, "END %s\n\n", layerName);
  lefwLines++;
  lefwSynArray[LEFW_LAYERROUTING] = 0;
  lefwState = LEFW_LAYERROUTING_END;
  lefwIsRouting = 0;
  return LEFW_OK;
}

int 
lefwLayerACCurrentDensity(const char *type, double value)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING
      && lefwState != LEFW_LAYER_START && lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;
  if (strcmp(type, "PEAK") && strcmp(type, "AVERAGE") && strcmp(type, "RMS"))
    return LEFW_BAD_DATA;	/* type is PEAK, AVERAGE & RMS only */
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ACCURRENTDENSITY %s", type);
    lefwLines++;
    lefwTableLen = 5;
    if (value)
      encPrint(lefwFile, (char *) " %g\n", value);	/* that's it for
							 * accurrentdensity */
    else {
      encPrint(lefwFile, (char *) "\n");	/* expect to have frequency &
						 * tableentries */
      lefwOldState = lefwState;	/* save the previous state */
      lefwState = LEFW_LAYERACCURRENT;
    }
  }
  else {
    fprintf(lefwFile, " ACCURRENTDENSITY %s", type);
    lefwLines++;
    lefwTableLen = 5;
    if (value)
      fprintf(lefwFile, " %g\n", value);	/* that's it for
						 * accurrentdensity */
    else {
      fprintf(lefwFile, "\n");	/* expect to have frequency & tableentries */
      lefwOldState = lefwState;	/* save the previous state */
      lefwState = LEFW_LAYERACCURRENT;
    }
  }
  return LEFW_OK;
}

int 
lefwLayerACFrequency(int numFrequency, double *frequency)
{
  int     i;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERACCURRENT)
    return LEFW_BAD_ORDER;
  if (numFrequency <= 0)
    return LEFW_BAD_DATA;	/* frequency are required */
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " FREQUENCY ");
    for (i = 0; i < numFrequency; i++) {
      encPrint(lefwFile, (char *) "%g ", *frequency++);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " FREQUENCY ");
    for (i = 0; i < numFrequency; i++) {
      fprintf(lefwFile, "%g ", *frequency++);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerACWidth(int numWidths, double *widths)
{
  int     i;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERACCURRENT)
    return LEFW_BAD_ORDER;
  if (numWidths <= 0)
    return LEFW_OK;		/* width is optional */
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " WIDTH ");
    for (i = 0; i < numWidths; i++) {
      encPrint(lefwFile, (char *) "%g ", *widths++);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " WIDTH ");
    for (i = 0; i < numWidths; i++) {
      fprintf(lefwFile, "%g ", *widths++);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwTableLen = numWidths;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerACCutarea(int numCutareas, double *cutareas)
{
  int     i;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERACCURRENT)
    return LEFW_BAD_ORDER;
  if (numCutareas <= 0)
    return LEFW_OK;		/* cutarea is optional */
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " CUTAREA ");
    for (i = 0; i < numCutareas; i++) {
      encPrint(lefwFile, (char *) "%g ", *cutareas++);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " CUTAREA ");
    for (i = 0; i < numCutareas; i++) {
      fprintf(lefwFile, "%g ", *cutareas++);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwTableLen = numCutareas;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerACTableEntries(int numEntries, double *entries)
{
  int     i;

  int     j = 0;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERACCURRENT)
    return LEFW_BAD_ORDER;
  if (numEntries <= 0)
    return LEFW_BAD_DATA;	/* tableEntries are required */
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " TABLEENTRIES\n ");
    for (i = 0; i < numEntries; i++) {
      if (++j > lefwTableLen) {
	encPrint(lefwFile, (char *) "\n ");
	j = 1;
      }
      encPrint(lefwFile, (char *) "%g ", *entries++);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " TABLEENTRIES\n ");
    for (i = 0; i < numEntries; i++) {
      if (++j > lefwTableLen) {
	fprintf(lefwFile, "\n ");
	j = 1;
      }
      fprintf(lefwFile, "%g ", *entries++);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwState = lefwOldState;	/* restore the previous state */
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerDCCurrentDensity(const char *type, double value)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING
      && lefwState != LEFW_LAYER_START && lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;
  if (strcmp(type, "AVERAGE"))
    return LEFW_BAD_DATA;	/* type is AVERAGE */
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " DCCURRENTDENSITY %s", type);
    lefwLines++;
    lefwTableLen = 5;
    if (value)
      encPrint(lefwFile, (char *) " %g\n", value);	/* that's it for
							 * accurrentdensity */
    else {
      encPrint(lefwFile, (char *) "\n");	/* expect to have frequency &
						 * tableentries */
      lefwOldState = lefwState;	/* save the previous state */
      lefwState = LEFW_LAYERDCCURRENT;
    }
  }
  else {
    fprintf(lefwFile, " DCCURRENTDENSITY %s", type);
    lefwLines++;
    lefwTableLen = 5;
    if (value)
      fprintf(lefwFile, " %g\n", value);	/* that's it for
						 * accurrentdensity */
    else {
      fprintf(lefwFile, "\n");	/* expect to have frequency & tableentries */
      lefwOldState = lefwState;	/* save the previous state */
      lefwState = LEFW_LAYERDCCURRENT;
    }
  }
  return LEFW_OK;
}

int 
lefwLayerDCWidth(int numWidths, double *widths)
{
  int     i;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERDCCURRENT)
    return LEFW_BAD_ORDER;

  /*
   * width is allowed in Layer routing only
   */ if (lefwOldState != LEFW_LAYER_START && lefwOldState != LEFW_LAYER &&
	  lefwOldState != LEFW_LAYERROUTING_START &&
	  lefwOldState != LEFW_LAYERROUTING)
    return LEFW_BAD_DATA;
  if (numWidths <= 0)
    return LEFW_BAD_DATA;	/* width is required */
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " WIDTH ");
    for (i = 0; i < numWidths; i++) {
      encPrint(lefwFile, (char *) "%g ", *widths++);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " WIDTH ");
    for (i = 0; i < numWidths; i++) {
      fprintf(lefwFile, "%g ", *widths++);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwTableLen = numWidths;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerDCCutarea(int numCutareas, double *cutareas)
{
  int     i;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERDCCURRENT)
    return LEFW_BAD_ORDER;

  /*
   * cutarea is allowed in Layer non-routing only
   */ if (lefwOldState != LEFW_LAYER_START && lefwOldState != LEFW_LAYER &&
	  lefwOldState != LEFW_LAYERROUTING_START &&
	  lefwOldState != LEFW_LAYERROUTING)
    return LEFW_BAD_DATA;
  if (numCutareas <= 0)
    return LEFW_BAD_DATA;	/* cutarea is required */
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " CUTAREA ");
    for (i = 0; i < numCutareas; i++) {
      encPrint(lefwFile, (char *) "%g ", *cutareas++);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " CUTAREA ");
    for (i = 0; i < numCutareas; i++) {
      fprintf(lefwFile, "%g ", *cutareas++);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwTableLen = numCutareas;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerDCTableEntries(int numEntries, double *entries)
{
  int     i;

  int     j = 0;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERDCCURRENT)
    return LEFW_BAD_ORDER;
  if (numEntries <= 0)
    return LEFW_BAD_DATA;	/* tableEntries are required */
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " TABLEENTRIES\n ");
    for (i = 0; i < numEntries; i++) {
      if (++j > lefwTableLen) {
	encPrint(lefwFile, (char *) "\n ");
	j = 1;
      }
      encPrint(lefwFile, (char *) "%g ", *entries++);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " TABLEENTRIES\n ");
    for (i = 0; i < numEntries; i++) {
      if (++j > lefwTableLen) {
	fprintf(lefwFile, "\n ");
	j = 1;
      }
      fprintf(lefwFile, "%g ", *entries++);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwState = lefwOldState;	/* restore the previous state */
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaModel(const char *oxide)
{
  lefw54Num = LEFW_ANTENNAMODEL;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING
      && lefwState != LEFW_LAYER_START && lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;

  /*
   * only routing or cut calls this func
   */ if (!lefwIsRouting && !lefwIsCut)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ANTENNAMODEL %s ;\n", oxide);
  else
    fprintf(lefwFile, " ANTENNAMODEL %s ;\n", oxide);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaAreaRatio(double value)
{
  lefw54Num = LEFW_ANTENNAAREARATIO;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING
      && lefwState != LEFW_LAYER_START && lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;

  /*
   * only routing or cut calls this func
   */ if (!lefwIsRouting && !lefwIsCut)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ANTENNAAREARATIO %g ;\n", value);
  else
    fprintf(lefwFile, " ANTENNAAREARATIO %g ;\n", value);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaDiffAreaRatio(double value)
{
  lefw54Num = LEFW_ANTENNADIFFAREARATIO;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING
      && lefwState != LEFW_LAYER_START && lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;

  /*
   * only routing or cut calls this func
   */ if (!lefwIsRouting && !lefwIsCut)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ANTENNADIFFAREARATIO %g ;\n", value);
  else
    fprintf(lefwFile, " ANTENNADIFFAREARATIO %g ;\n", value);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaDiffAreaRatioPwl(int numPwls, double *diffusions,
				 double *ratios)
{
  int     i;

  lefw54Num = LEFW_ANTENNADIFFAREARATIOPWL;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING
      && lefwState != LEFW_LAYER_START && lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;

  /*
   * only routing or cut calls this func
   */ if (!lefwIsRouting && !lefwIsCut)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNADIFFAREARATIO PWL ( ");
    for (i = 0; i < numPwls; i++)
      encPrint(lefwFile, (char *) "( %g %g ) ", *diffusions++, *ratios++);
    encPrint(lefwFile, (char *) ") ;\n");
  }
  else {
    fprintf(lefwFile, " ANTENNADIFFAREARATIO PWL ( ");
    for (i = 0; i < numPwls; i++)
      fprintf(lefwFile, "( %g %g ) ", *diffusions++, *ratios++);
    fprintf(lefwFile, ") ;\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaCumAreaRatio(double value)
{
  lefw54Num = LEFW_ANTENNACUMAREARATIO;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING
      && lefwState != LEFW_LAYER_START && lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;

  /*
   * only routing or cut calls this func
   */ if (!lefwIsRouting && !lefwIsCut)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ANTENNACUMAREARATIO %g ;\n", value);
  else
    fprintf(lefwFile, " ANTENNACUMAREARATIO %g ;\n", value);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaCumDiffAreaRatio(double value)
{
  lefw54Num = LEFW_ANTENNACUMDIFFAREARATIO;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING
      && lefwState != LEFW_LAYER_START && lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;

  /*
   * only routing or cut calls this func
   */ if (!lefwIsRouting && !lefwIsCut)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ANTENNACUMDIFFAREARATIO %g ;\n", value);
  else
    fprintf(lefwFile, " ANTENNACUMDIFFAREARATIO %g ;\n", value);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaCumDiffAreaRatioPwl(int numPwls, double *diffusions,
				    double *ratios)
{
  int     i;

  lefw54Num = LEFW_ANTENNACUMDIFFAREARATIOPWL;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING
      && lefwState != LEFW_LAYER_START && lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;

  /*
   * only routing or cut calls this func
   */ if (!lefwIsRouting && !lefwIsCut)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNACUMDIFFAREARATIO PWL ( ");
    for (i = 0; i < numPwls; i++)
      encPrint(lefwFile, (char *) "( %g %g ) ", *diffusions++, *ratios++);
    encPrint(lefwFile, (char *) ") ;\n");
  }
  else {
    fprintf(lefwFile, " ANTENNACUMDIFFAREARATIO PWL ( ");
    for (i = 0; i < numPwls; i++)
      fprintf(lefwFile, "( %g %g ) ", *diffusions++, *ratios++);
    fprintf(lefwFile, ") ;\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaAreaFactor(double value, const char *diffUseOnly)
{
  lefw54Num = LEFW_ANTENNAAREAFACTOR;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING
      && lefwState != LEFW_LAYER_START && lefwState != LEFW_LAYER)
    return LEFW_BAD_ORDER;

  /*
   * only routing or cut calls this func
   */ if (!lefwIsRouting && !lefwIsCut)
    return LEFW_BAD_DATA;
  if ((versionNum < 5.4) &&
      (diffUseOnly && strcmp(diffUseOnly, "DIFFUSEONLY") == 0))
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNAAREAFACTOR %g ", value);
    if (diffUseOnly && strcmp(diffUseOnly, "DIFFUSEONLY") == 0)
      encPrint(lefwFile, (char *) " DIFFUSEONLY ");
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNAAREAFACTOR %g ", value);
    if (diffUseOnly && strcmp(diffUseOnly, "DIFFUSEONLY") == 0)
      fprintf(lefwFile, "DIFFUSEONLY ");
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaSideAreaRatio(double value)
{
  lefw54Num = LEFW_ANTENNASIDEAREARATIO;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;

  /*
   * only routing calls this func
   */ if (!lefwIsRouting)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ANTENNASIDEAREARATIO %g ;\n", value);
  else
    fprintf(lefwFile, " ANTENNASIDEAREARATIO %g ;\n", value);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaDiffSideAreaRatio(double value)
{
  lefw54Num = LEFW_ANTENNADIFFSIDEAREARATIO;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;

  /*
   * only routing calls this func
   */ if (!lefwIsRouting)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ANTENNADIFFSIDEAREARATIO %g ;\n", value);
  else
    fprintf(lefwFile, " ANTENNADIFFSIDEAREARATIO %g ;\n", value);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaDiffSideAreaRatioPwl(int numPwls, double *diffusions,
				     double *ratios)
{
  int     i;

  lefw54Num = LEFW_ANTENNADIFFSIDEAREARATIOPWL;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;

  /*
   * only routing or cut calls this func
   */ if (!lefwIsRouting)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNADIFFSIDEAREARATIO PWL ( ");
    for (i = 0; i < numPwls; i++)
      encPrint(lefwFile, (char *) "( %g %g ) ", *diffusions++, *ratios++);
    encPrint(lefwFile, (char *) ") ;\n");
  }
  else {
    fprintf(lefwFile, " ANTENNADIFFSIDEAREARATIO PWL ( ");
    for (i = 0; i < numPwls; i++)
      fprintf(lefwFile, "( %g %g ) ", *diffusions++, *ratios++);
    fprintf(lefwFile, ") ;\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaCumSideAreaRatio(double value)
{
  lefw54Num = LEFW_ANTENNACUMSIDEAREARATIO;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;

  /*
   * only routing calls this func
   */ if (!lefwIsRouting)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ANTENNACUMSIDEAREARATIO %g ;\n", value);
  else
    fprintf(lefwFile, " ANTENNACUMSIDEAREARATIO %g ;\n", value);
  lefwLines++;
  return LEFW_OK;
}

/*
 * 11/25/2002 - Craig Files (Agilent) Renamed from
 *//* lefwLayerAntennaCumSideDiffAreaRatio to match the header */ int 
lefwLayerAntennaCumDiffSideAreaRatio(double value)
{
  lefw54Num = LEFW_ANTENNACUMDIFFSIDEAREARATIO;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;

  /*
   * only routing calls this func
   */ if (!lefwIsRouting)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ANTENNACUMDIFFSIDEAREARATIO %g ;\n", value);
  else
    fprintf(lefwFile, " ANTENNACUMDIFFSIDEAREARATIO %g ;\n", value);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaCumDiffSideAreaRatioPwl(int numPwls, double *diffusions,
					double *ratios)
{
  int     i;

  lefw54Num = LEFW_ANTENNACUMDIFFSIDEAREARATIOPWL;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;

  /*
   * only routing or cut calls this func
   */ if (!lefwIsRouting)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNACUMDIFFSIDEAREARATIO PWL ( ");
    for (i = 0; i < numPwls; i++)
      encPrint(lefwFile, (char *) "( %g %g ) ", *diffusions++, *ratios++);
    encPrint(lefwFile, (char *) ") ;\n");
  }
  else {
    fprintf(lefwFile, " ANTENNACUMDIFFSIDEAREARATIO PWL ( ");
    for (i = 0; i < numPwls; i++)
      fprintf(lefwFile, "( %g %g ) ", *diffusions++, *ratios++);
    fprintf(lefwFile, ") ;\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwLayerAntennaSideAreaFactor(double value, const char *diffUseOnly)
{
  lefw54Num = LEFW_ANTENNASIDEAREAFACTOR;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_LAYERROUTING_START && lefwState != LEFW_LAYERROUTING)
    return LEFW_BAD_ORDER;

  /*
   * only routing calls this func
   */ if (!lefwIsRouting)
    return LEFW_BAD_DATA;
  if ((versionNum < 5.4) &&
      (diffUseOnly && strcmp(diffUseOnly, "DIFFUSEONLY") == 0))
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNASIDEAREAFACTOR %g ", value);
    if (diffUseOnly && strcmp(diffUseOnly, "DIFFUSEONLY") == 0)
      encPrint(lefwFile, (char *) " DIFFUSEONLY ");
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNASIDEAREAFACTOR %g ", value);
    if (diffUseOnly && strcmp(diffUseOnly, "DIFFUSEONLY") == 0)
      fprintf(lefwFile, "DIFFUSEONLY ");
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwSlotWireWidth(double minWireWidthForSlotting)
{
  lefw54Num = LEFW_SLOTWIREWIDTH;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SLOTWIREWIDTH %g ;\n",
	     minWireWidthForSlotting);
  else
    fprintf(lefwFile, " SLOTWIREWIDTH %g ;\n", minWireWidthForSlotting);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwSlotWireLength(double minWireLengthForSlotting)
{
  lefw54Num = LEFW_SLOTWIRELENGTH;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SLOTWIRELENGTH %g ;\n",
	     minWireLengthForSlotting);
  else
    fprintf(lefwFile, " SLOTWIRELENGTH %g ;\n", minWireLengthForSlotting);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwSlotWidth(double minSlotWidth)
{
  lefw54Num = LEFW_SLOTWIDTH;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SLOTWIDTH %g ;\n", minSlotWidth);
  else
    fprintf(lefwFile, " SLOTWIDTH %g ;\n", minSlotWidth);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwSlotLength(double minSlotLength)
{
  lefw54Num = LEFW_SLOTLENGTH;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SLOTLENGTH %g ;\n", minSlotLength);
  else
    fprintf(lefwFile, " SLOTLENGTH %g ;\n", minSlotLength);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMaxAdjacentSlotSpacing(double maxAdjSlotSpacing)
{
  lefw54Num = LEFW_MAXADJACENTSLOTSPACING;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " MAXADJACENTSLOTSPACING %g ;\n",
	     maxAdjSlotSpacing);
  else
    fprintf(lefwFile, " MAXADJACENTSLOTSPACING %g ;\n", maxAdjSlotSpacing);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMaxCoaxialSlotSpacing(double maxCoaxSlotSpacing)
{
  lefw54Num = LEFW_MAXCOAXIALSLOTSPACING;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " MAXCOAXIALSLOTSPACING %g ;\n",
	     maxCoaxSlotSpacing);
  else
    fprintf(lefwFile, " MAXCOAXIALSLOTSPACING %g ;\n", maxCoaxSlotSpacing);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMaxEdgeSlotSpacing(double maxEdgeSlotSpacing)
{
  lefw54Num = LEFW_MAXEDGESLOTSPACING;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " MAXEDGESLOTSPACING %g ;\n",
	     maxEdgeSlotSpacing);
  else
    fprintf(lefwFile, " MAXEDGESLOTSPACING %g ;\n", maxEdgeSlotSpacing);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwSplitWireWidth(double minWireWidthForSplitting)
{
  lefw54Num = LEFW_SPLITWIREWIDTH;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SPLITWIREWIDTH %g ;\n",
	     minWireWidthForSplitting);
  else
    fprintf(lefwFile, " SPLITWIREWIDTH %g ;\n", minWireWidthForSplitting);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMinimumDensity(double minDensity)
{
  lefw54Num = LEFW_MINIMUMDENSITY;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " MINIMUMDENSITY %g ;\n", minDensity);
  else
    fprintf(lefwFile, " MINIMUMDENSITY %g ;\n", minDensity);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMaximumDensity(double maxDensity)
{
  lefw54Num = LEFW_MAXIMUMDENSITY;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " MAXIMUMDENSITY %g ;\n", maxDensity);
  else
    fprintf(lefwFile, " MAXIMUMDENSITY %g ;\n", maxDensity);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwDensityCheckWindow(double checkWindowLength, double checkWindowWidth)
{
  lefw54Num = LEFW_DENSITYCHECKWINDOW;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " DENSITYCHECKWINDOW %g %g ;\n",
	     checkWindowLength, checkWindowWidth);
  else
    fprintf(lefwFile, " DENSITYCHECKWINDOW %g %g ;\n", checkWindowLength,
	    checkWindowWidth);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwDensityCheckStep(double checkStepValue)
{
  lefw54Num = LEFW_DENSITYCHECKSTEP;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " DENSITYCHECKSTEP %g ;\n",
	     checkStepValue);
  else
    fprintf(lefwFile, " DENSITYCHECKSTEP %g ;\n", checkStepValue);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwFillActiveSpacing(double fillToActiveSpacing)
{
  lefw54Num = LEFW_FILLACTIVESPACING;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " FILLACTIVESPACING %g ;\n",
	     fillToActiveSpacing);
  else
    fprintf(lefwFile, " FILLACTIVESPACING %g ;\n", fillToActiveSpacing);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMaxviastack(int value, const char *bottomLayer,
		const char *topLayer)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwIsMaxviastack)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) "MAXVIASTACK %d ", value);
    if (bottomLayer)
      encPrint(lefwFile, (char *) "RANGE %s %s ;\n", bottomLayer, topLayer);
    else
      encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, "MAXVIASTACK %d ", value);
    if (bottomLayer)
      fprintf(lefwFile, "RANGE %s %s ;\n", bottomLayer, topLayer);
    else
      fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwStartPropDef()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwSynArray[LEFW_PROPERTYDEF_START])
    return LEFW_ALREADY_DEFINED;
  lefwSynArray[LEFW_ANTENNASIZE] = 1;
  if (lefwState == LEFW_PROPERTYDEF_START ||
      lefwState == LEFW_PROPERTYDEF)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "PROPERTYDEFINITIONS\n");
  else
    fprintf(lefwFile, "PROPERTYDEFINITIONS\n");
  lefwLines++;
  lefwState = LEFW_PROPERTYDEF_START;
  lefwSynArray[LEFW_PROPERTYDEF_START] = 1;
  return LEFW_OK;
}

int 
lefwIntPropDef(const char *objType,
	       const char *propName,
	       double leftRange, double rightRange,	/* optional  */
	       int propValue	/* optional  */
)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_PROPERTYDEF_START &&
      lefwState != LEFW_PROPERTYDEF)
    return LEFW_BAD_ORDER;
  if ((!objType && !*objType) || (!propName && !*propName))	/* require */
    return LEFW_BAD_DATA;
  if (strcmp(objType, "LIBRARY") && strcmp(objType, "VIA") &&
      strcmp(objType, "MACRO") && strcmp(objType, "PIN") &&
      strcmp(objType, "LAYER") && strcmp(objType, "NONDEFAULTRULE") &&
      strcmp(objType, "VIARULE"))
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " %s %s INTEGER ", objType, propName);
    if (leftRange || rightRange)
      encPrint(lefwFile, (char *) "RANGE %g %g ", leftRange, rightRange);
    if (propValue)
      encPrint(lefwFile, (char *) "%d ", propValue);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " %s %s INTEGER ", objType, propName);
    if (leftRange || rightRange)
      fprintf(lefwFile, "RANGE %g %g ", leftRange, rightRange);
    if (propValue)
      fprintf(lefwFile, "%d ", propValue);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_PROPERTYDEF;
  return LEFW_OK;
}

int 
lefwRealPropDef(const char *objType,
		const char *propName,
		double leftRange, double rightRange,	/* optional  */
		double propValue/* optional  */
)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_PROPERTYDEF_START &&
      lefwState != LEFW_PROPERTYDEF)
    return LEFW_BAD_ORDER;
  if ((!objType && !*objType) || (!propName && !*propName))	/* require */
    return LEFW_BAD_DATA;
  if (strcmp(objType, "LIBRARY") && strcmp(objType, "VIA") &&
      strcmp(objType, "MACRO") && strcmp(objType, "PIN") &&
      strcmp(objType, "LAYER") && strcmp(objType, "NONDEFAULTRULE") &&
      strcmp(objType, "VIARULE"))
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " %s %s REAL ", objType, propName);
    if (leftRange || rightRange)
      encPrint(lefwFile, (char *) "RANGE %g %g ", leftRange, rightRange);
    if (propValue)
      encPrint(lefwFile, (char *) "%g ", propValue);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " %s %s REAL ", objType, propName);
    if (leftRange || rightRange)
      fprintf(lefwFile, "RANGE %g %g ", leftRange, rightRange);
    if (propValue)
      fprintf(lefwFile, "%g ", propValue);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_PROPERTYDEF;
  return LEFW_OK;
}

int 
lefwStringPropDef(const char *objType,
		  const char *propName,
		  double leftRange, double rightRange,	/* optional  */
		  const char *propValue	/* optional  */
)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_PROPERTYDEF_START &&
      lefwState != LEFW_PROPERTYDEF)
    return LEFW_BAD_ORDER;
  if ((!objType && !*objType) || (!propName && !*propName))
    return LEFW_BAD_DATA;
  if (strcmp(objType, "LIBRARY") && strcmp(objType, "VIA") &&
      strcmp(objType, "MACRO") && strcmp(objType, "PIN") &&
      strcmp(objType, "LAYER") && strcmp(objType, "NONDEFAULTRULE") &&
      strcmp(objType, "VIARULE"))
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " %s %s STRING ", objType, propName);
    if (leftRange || rightRange)
      encPrint(lefwFile, (char *) "RANGE %g %g ", leftRange, rightRange);
    if (propValue)
      encPrint(lefwFile, (char *) "\"%s\" ", propValue);	/* string, set quotes */
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " %s %s STRING ", objType, propName);
    if (leftRange || rightRange)
      fprintf(lefwFile, "RANGE %g %g ", leftRange, rightRange);
    if (propValue)
      fprintf(lefwFile, "\"%s\" ", propValue);	/* string, set quotes */
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_PROPERTYDEF;
  return LEFW_OK;
}

int 
lefwEndPropDef()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_PROPERTYDEF_START &&
      lefwState != LEFW_PROPERTYDEF)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END PROPERTYDEFINITIONS\n\n");
  else
    fprintf(lefwFile, "END PROPERTYDEFINITIONS\n\n");
  lefwLines++;
  lefwState = LEFW_PROPERTYDEF_END;
  return LEFW_OK;
}

int 
lefwEnd()
{
  if (!lefwFile)
    return 1;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END LIBRARY\n");
  else
    fprintf(lefwFile, "END LIBRARY\n");
  lefwLines++;

  /*
   * lefwFile = 0;
   */ lefwState = LEFW_DONE;
  return LEFW_OK;
}

int 
lefwStartVia(const char *viaName, const char *isDefault)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_VIA_START ||
      lefwState == LEFW_VIA)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END &&
      (!lefwIsNonDefaultRule))	/* via defined in nondefaultrule  */
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) "VIA %s ", viaName);
    if (isDefault && strcmp(isDefault, "DEFAULT") == 0)
      encPrint(lefwFile, (char *) "DEFAULT");
    encPrint(lefwFile, (char *) "\n");
  }
  else {
    fprintf(lefwFile, "VIA %s ", viaName);
    if (isDefault && strcmp(isDefault, "DEFAULT") == 0)
      fprintf(lefwFile, "DEFAULT");
    fprintf(lefwFile, "\n");
  }
  lefwLines++;
  lefwSynArray[LEFW_VIA_START] = 1;
  lefwState = LEFW_VIA_START;
  lefwDidLayer = 0;		/* hasn't set the layer yet */
  return LEFW_OK;
}

int 
lefwViaTopofstackonly()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_VIA_START &&
      lefwState != LEFW_VIA)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " TOPOFSTACKONLY\n");
  else
    fprintf(lefwFile, " TOPOFSTACKONLY\n");
  lefwLines++;
  return LEFW_OK;
}

int 
lefwViaForeign(const char *foreignName,
	       double xl,	/* optional  */
	       double yl,	/* optional  */
	       int orient)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_VIA_START &&
      lefwState != LEFW_VIA)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " FOREIGN %s ", foreignName);
    if (xl || yl) {
      encPrint(lefwFile, (char *) "%g %g ", xl, yl);
      if (orient >= 0 && orient <= 7)
	encPrint(lefwFile, (char *) "%s ", lefwOrient(orient));
    }
    else if (orient > 0 && orient <= 7) {	/* assume user has 0 0 for pt  */
      encPrint(lefwFile, (char *) "%g %g ", xl, yl);
      encPrint(lefwFile, (char *) "%s ", lefwOrient(orient));
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " FOREIGN %s ", foreignName);
    if (xl || yl) {
      fprintf(lefwFile, "%g %g ", xl, yl);
      if (orient >= 0 && orient <= 7)
	fprintf(lefwFile, "%s ", lefwOrient(orient));
    }
    else if (orient > 0 && orient <= 7) {	/* assume user has 0 0 for pt  */
      fprintf(lefwFile, "%g %g ", xl, yl);
      fprintf(lefwFile, "%s ", lefwOrient(orient));
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_VIA;
  return LEFW_OK;
}

int 
lefwViaForeignStr(const char *foreignName,
		  double xl,	/* optional  */
		  double yl,	/* optional  */
		  const char *orient)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_VIA_START &&
      lefwState != LEFW_VIA)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " FOREIGN %s ", foreignName);
    if (xl || yl) {
      encPrint(lefwFile, (char *) "%g %g ", xl, yl);
      if (orient && *orient != '\0')
	encPrint(lefwFile, (char *) "%s ", orient);
    }
    else if (orient && *orient != '\0') {	/* assume user has 0 0 for pt  */
      encPrint(lefwFile, (char *) "%g %g ", xl, yl);
      encPrint(lefwFile, (char *) "%s ", orient);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " FOREIGN %s ", foreignName);
    if (xl || yl) {
      fprintf(lefwFile, "%g %g ", xl, yl);
      if (orient && *orient != '\0')
	fprintf(lefwFile, "%s ", orient);
    }
    else if (orient && *orient != '\0') {	/* assume user has 0 0 for pt  */
      fprintf(lefwFile, "%g %g ", xl, yl);
      fprintf(lefwFile, "%s ", orient);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_VIA;
  return LEFW_OK;
}

int 
lefwViaResistance(double resistance)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_VIA_START &&
      lefwState != LEFW_VIA)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " RESISTANCE %g ;\n", resistance);
  else
    fprintf(lefwFile, " RESISTANCE %g ;\n", resistance);
  lefwLines++;
  lefwState = LEFW_VIA;
  return LEFW_OK;
}

int 
lefwViaLayer(const char *layerName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_VIA_START && lefwState != LEFW_VIA)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " LAYER %s ;\n", layerName);
  else
    fprintf(lefwFile, " LAYER %s ;\n", layerName);
  lefwLines++;
  lefwState = LEFW_VIA;
  lefwDidLayer = 1;
  return LEFW_OK;
}

int 
lefwViaLayerRect(double x1l, double y1l, double x2l, double y2l)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_VIA_START && lefwState != LEFW_VIA)
    return LEFW_BAD_ORDER;
  if (!lefwDidLayer)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " RECT %g %g %g %g ;\n", x1l, y1l, x2l, y2l);
  else
    fprintf(lefwFile, " RECT %g %g %g %g ;\n", x1l, y1l, x2l, y2l);
  lefwLines++;
  lefwState = LEFW_VIA;
  return LEFW_OK;
}

int 
lefwStringProperty(const char *propName, const char *propValue)
{
  if ((lefwState != LEFW_VIA) && (lefwState != LEFW_LAYER) &&
      (lefwState != LEFW_VIARULE) && (lefwState != LEFW_MACRO_START) &&
      (lefwState != LEFW_MACRO) && (lefwState != LEFW_VIA_START) &&
      (lefwState != LEFW_VIARULE_START) && (lefwState != LEFW_LAYER_START) &&
      (lefwState != LEFW_BEGINEXT))
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " PROPERTY %s \"%s\" ;\n", propName, propValue);
  else
    fprintf(lefwFile, " PROPERTY %s \"%s\" ;\n", propName, propValue);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwRealProperty(const char *propName, double propValue)
{
  if ((lefwState != LEFW_VIA) && (lefwState != LEFW_LAYER) &&
      (lefwState != LEFW_VIARULE) && (lefwState != LEFW_MACRO_START) &&
      (lefwState != LEFW_MACRO) && (lefwState != LEFW_VIA_START) &&
      (lefwState != LEFW_VIARULE_START) && (lefwState != LEFW_LAYER_START) &&
      (lefwState != LEFW_BEGINEXT))
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " PROPERTY %s %g ;\n", propName, propValue);
  else
    fprintf(lefwFile, " PROPERTY %s %g ;\n", propName, propValue);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwIntProperty(const char *propName, int propValue)
{
  if ((lefwState != LEFW_VIA) && (lefwState != LEFW_LAYER) &&
      (lefwState != LEFW_VIARULE) && (lefwState != LEFW_MACRO_START) &&
      (lefwState != LEFW_MACRO) && (lefwState != LEFW_VIA_START) &&
      (lefwState != LEFW_VIARULE_START) && (lefwState != LEFW_LAYER_START) &&
      (lefwState != LEFW_BEGINEXT))
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " PROPERTY %s %d ;\n", propName, propValue);
  else
    fprintf(lefwFile, " PROPERTY %s %d ;\n", propName, propValue);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwEndVia(const char *viaName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_VIA_START &&
      lefwState != LEFW_VIA)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END %s\n\n", viaName);
  else
    fprintf(lefwFile, "END %s\n\n", viaName);
  lefwLines++;
  lefwState = LEFW_VIA_END;
  lefwDidLayer = 0;
  return LEFW_OK;
}

/*
 * internal function
 */ int 
lefwViaRulePrtLayer(const char *layerName, const char *direction,
		    double minWidth, double maxWidth,
		    double overhang, double metalOverhang)
{
  if (!layerName || layerName == 0 || *layerName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " LAYER %s ;\n", layerName);
  else
    fprintf(lefwFile, " LAYER %s ;\n", layerName);
  if (direction && strcmp(direction, "HORIZONTAL") &&
      strcmp(direction, "VERTICAL"))
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " DIRECTION %s ;\n", direction);
    lefwLines = lefwLines + 2;
    if (minWidth || maxWidth) {
      encPrint(lefwFile, (char *) " WIDTH %g TO %g ;\n", minWidth, maxWidth);
      lefwLines++;
    }
    if (overhang) {
      encPrint(lefwFile, (char *) " OVERHANG %g ;\n", overhang);
      lefwLines++;
    }
    if (metalOverhang) {
      encPrint(lefwFile, (char *) " METALOVERHANG %g ;\n", metalOverhang);
      lefwLines++;
    }
  }
  else {
    fprintf(lefwFile, " DIRECTION %s ;\n", direction);
    lefwLines = lefwLines + 2;
    if (minWidth || maxWidth) {
      fprintf(lefwFile, " WIDTH %g TO %g ;\n", minWidth, maxWidth);
      lefwLines++;
    }
    if (overhang) {
      fprintf(lefwFile, " OVERHANG %g ;\n", overhang);
      lefwLines++;
    }
    if (metalOverhang) {
      fprintf(lefwFile, " METALOVERHANG %g ;\n", metalOverhang);
      lefwLines++;
    }
  }
  return LEFW_OK;
}

int 
lefwStartViaRule(const char *viaRuleName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_VIARULE_START ||
      lefwState == LEFW_VIARULE)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (!viaRuleName || viaRuleName == 0 || *viaRuleName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "VIARULE %s\n", viaRuleName);
  else
    fprintf(lefwFile, "VIARULE %s\n", viaRuleName);
  lefwLines++;
  lefwSynArray[LEFW_VIARULE_START] = 1;
  lefwState = LEFW_VIARULE_START;
  lefwDidLayer = 0;		/* hasn't set the layer yet */
  lefwNumViaRuleLayers = 0;
  return LEFW_OK;
}

int 
lefwViaRuleLayer(const char *layerName, const char *direction,
		 double minWidth, double maxWidth,	/* optional  */
		 double overhang, double metalOverhang)
{				/* optional  */
  int     status;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_VIARULE_START &&
      lefwState != LEFW_VIARULE)
    return LEFW_BAD_ORDER;
  if (lefwNumViaRuleLayers >= 2)
    return LEFW_BAD_ORDER;
  if ((status = lefwViaRulePrtLayer(layerName, direction, minWidth, maxWidth,
				    overhang, metalOverhang)) != LEFW_OK)
    return status;
  lefwNumViaRuleLayers++;
  lefwState = LEFW_VIARULE;
  return LEFW_OK;
}

int 
lefwViaRuleVia(const char *viaName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_VIARULE)
    return LEFW_BAD_ORDER;
  if (lefwNumViaRuleLayers != 2)/* this routine has to be after two layers */
    return LEFW_BAD_ORDER;	/* be written */
  if (!viaName || !*viaName)	/* viaName is required */
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " VIA %s ;\n", viaName);
  else
    fprintf(lefwFile, " VIA %s ;\n", viaName);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwEndViaRule(const char *viaRuleName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_VIARULE_START &&
      lefwState != LEFW_VIARULE)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END %s\n\n", viaRuleName);
  else
    fprintf(lefwFile, "END %s\n\n", viaRuleName);
  lefwLines++;
  lefwState = LEFW_VIARULE_END;
  lefwDidLayer = 0;
  lefwNumViaRuleLayers = 0;
  return LEFW_OK;
}

int 
lefwStartViaRuleGen(const char *viaRuleName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_VIARULEGEN_START ||
      lefwState == LEFW_VIARULEGEN)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (!viaRuleName || viaRuleName == 0 || *viaRuleName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "VIARULE %s GENERATE\n", viaRuleName);
  else
    fprintf(lefwFile, "VIARULE %s GENERATE\n", viaRuleName);
  lefwLines++;
  lefwSynArray[LEFW_VIARULEGEN_START] = 1;
  lefwState = LEFW_VIARULEGEN_START;
  lefwDidLayer = 0;		/* hasn't set the layer yet */
  lefwNumViaRuleLayers = 0;
  return LEFW_OK;
}

int 
lefwViaRuleGenLayer(const char *layerName, const char *direction,
		    double minWidth, double maxWidth,	/* optional  */
		    double overhang, double metalOverhang)
{				/* optional  */
  int     status;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_VIARULEGEN_START &&
      lefwState != LEFW_VIARULEGEN)
    return LEFW_BAD_ORDER;
  if (lefwNumViaRuleLayers >= 2)
    return LEFW_BAD_ORDER;
  if ((status = lefwViaRulePrtLayer(layerName, direction, minWidth, maxWidth,
				    overhang, metalOverhang)) != LEFW_OK)
    return status;
  lefwNumViaRuleLayers++;
  lefwState = LEFW_VIARULEGEN;
  return LEFW_OK;
}

int 
lefwViaRuleGenLayerEnclosure(const char *layerName,
			     double overhang1, double overhang2,
			     double minWidth, double maxWidth)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_VIARULEGEN_START &&
      lefwState != LEFW_VIARULEGEN)
    return LEFW_BAD_ORDER;
  if (lefwNumViaRuleLayers >= 2)
    return LEFW_BAD_ORDER;
  if (!layerName || layerName == 0 || *layerName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " LAYER %s ;\n", layerName);
  else
    fprintf(lefwFile, " LAYER %s ;\n", layerName);
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ENCLOSURE %g %g ;\n", overhang1,
	     overhang2);
    lefwLines = lefwLines + 2;
    if (minWidth || maxWidth) {
      encPrint(lefwFile, (char *) " WIDTH %g TO %g ;\n", minWidth,
	       maxWidth);
      lefwLines++;
    }
  }
  else {
    fprintf(lefwFile, " ENCLOSURE %g %g ;\n", overhang1, overhang2);
    lefwLines = lefwLines + 2;
    if (minWidth || maxWidth) {
      fprintf(lefwFile, " WIDTH %g TO %g ;\n", minWidth, maxWidth);
      lefwLines++;
    }
  }
  lefwNumViaRuleLayers++;
  lefwState = LEFW_VIARULEGEN;
  return LEFW_OK;
}

int 
lefwViaRuleGenLayer3(const char *layerName,
		     double xl, double yl, double xh, double yh,
		     double xSpacing, double ySpacing,
		     double resistance)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_VIARULEGEN)
    return LEFW_BAD_ORDER;
  if (lefwNumViaRuleLayers != 2)/* this routine has to be after two layers */
    return LEFW_BAD_ORDER;	/* be written */
  if (!layerName || layerName == 0 || *layerName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " LAYER %s ;\n", layerName);
    encPrint(lefwFile, (char *) " RECT %g %g %g %g ;\n", xl, yl, xh, yh);
    encPrint(lefwFile, (char *) " SPACING %g BY %g ;\n", xSpacing, ySpacing);
    if (resistance) {
      encPrint(lefwFile, (char *) " RESISTANCE %g ;\n", resistance);
      lefwLines++;
    }
  }
  else {
    fprintf(lefwFile, " LAYER %s ;\n", layerName);
    fprintf(lefwFile, " RECT %g %g %g %g ;\n", xl, yl, xh, yh);
    fprintf(lefwFile, " SPACING %g BY %g ;\n", xSpacing, ySpacing);
    if (resistance) {
      fprintf(lefwFile, " RESISTANCE %g ;\n", resistance);
      lefwLines++;
    }
  }
  lefwLines += 3;
  return LEFW_OK;
}

int 
lefwEndViaRuleGen(const char *viaRuleName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_VIARULEGEN_START &&
      lefwState != LEFW_VIARULEGEN)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END %s\n\n", viaRuleName);
  else
    fprintf(lefwFile, "END %s\n\n", viaRuleName);
  lefwLines++;
  lefwState = LEFW_VIARULEGEN_END;
  lefwDidLayer = 0;
  lefwNumViaRuleLayers = 0;
  return LEFW_OK;
}

int 
lefwStartNonDefaultRule(const char *ruleName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_NONDEFAULTRULE_START ||
      lefwState == LEFW_NONDEFAULTRULE)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (!ruleName || ruleName == 0 || *ruleName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "NONDEFAULTRULE %s\n", ruleName);
  else
    fprintf(lefwFile, "NONDEFAULTRULE %s\n", ruleName);
  lefwLines++;
  lefwSynArray[LEFW_NONDEFAULTRULE_START] = 1;
  lefwState = LEFW_NONDEFAULTRULE_START;
  lefwDidLayer = 0;		/* hasn't set the layer yet */
  lefwIsNonDefaultRule = 1;
  return LEFW_OK;
}

int 
lefwNonDefaultRuleLayer(const char *routingLayerName,
		      double width, double minSpacing, double wireExtension,
			double resist, double capacitance, double edgeCap)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_NONDEFAULTRULE_START &&
      lefwState != LEFW_NONDEFAULTRULE)
    return LEFW_BAD_ORDER;
  if (!routingLayerName || routingLayerName == 0 || *routingLayerName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " LAYER %s\n", routingLayerName);
    encPrint(lefwFile, (char *) " WIDTH %g ;\n", width);
    encPrint(lefwFile, (char *) " SPACING %g ;\n", minSpacing);
    if (wireExtension) {
      encPrint(lefwFile, (char *) " WIREEXTENSION %g ;\n", wireExtension);
      lefwLines++;
    }
    if (resist) {
      encPrint(lefwFile, (char *) " RESISTANCE RPERSQ %g ;\n", resist);
      lefwLines++;
    }
    if (capacitance) {
      encPrint(lefwFile, (char *) " CAPACITANCE CPERSQDIST %g ;\n",
	       capacitance);
      lefwLines++;
    }
    if (edgeCap) {
      encPrint(lefwFile, (char *) " EDGECAPACITANCE %g ;\n", edgeCap);
      lefwLines++;
    }
    encPrint(lefwFile, (char *) " END %s\n\n", routingLayerName);
  }
  else {
    fprintf(lefwFile, " LAYER %s\n", routingLayerName);
    fprintf(lefwFile, " WIDTH %g ;\n", width);
    fprintf(lefwFile, " SPACING %g ;\n", minSpacing);
    if (wireExtension) {
      fprintf(lefwFile, " WIREEXTENSION %g ;\n", wireExtension);
      lefwLines++;
    }
    if (resist) {
      fprintf(lefwFile, " RESISTANCE RPERSQ %g ;\n", resist);
      lefwLines++;
    }
    if (capacitance) {
      fprintf(lefwFile, " CAPACITANCE CPERSQDIST %g ;\n", capacitance);
      lefwLines++;
    }
    if (edgeCap) {
      fprintf(lefwFile, " EDGECAPACITANCE %g ;\n", edgeCap);
      lefwLines++;
    }
    fprintf(lefwFile, " END %s\n\n", routingLayerName);
  }
  lefwLines = lefwLines + 4;
  lefwSynArray[LEFW_NONDEFAULTRULE] = 1;
  lefwState = LEFW_NONDEFAULTRULE;
  return LEFW_OK;
}

int 
lefwEndNonDefaultRule(const char *ruleName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_NONDEFAULTRULE_START &&
      lefwState != LEFW_NONDEFAULTRULE &&
      lefwState != LEFW_VIA_END && lefwState != LEFW_SPACING_END)
    return LEFW_BAD_ORDER;
  /* can be right after a via or spacing defined in nondefaultrule  */
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END %s\n\n", ruleName);
  else
    fprintf(lefwFile, "END %s\n\n", ruleName);
  lefwLines++;
  lefwState = LEFW_NONDEFAULTRULE_END;
  lefwDidLayer = 0;
  lefwIsNonDefaultRule = 0;
  return LEFW_OK;
}

int 
lefwStartSpacing()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_SPACING_START ||
      lefwState == LEFW_SPACING)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END &&
      (!lefwIsNonDefaultRule))	/* spacing defined in nondefaultrule */
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "SPACING\n");
  else
    fprintf(lefwFile, "SPACING\n");
  lefwLines++;
  lefwState = LEFW_SPACING_START;
  return LEFW_OK;
}

int 
lefwSpacing(const char *layerName1, const char *layerName2,
	    double minSpace, const char *stack)
{				/* optioanl  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_SPACING_START &&
      lefwState != LEFW_SPACING)
    return LEFW_BAD_ORDER;
  if (!layerName1 || layerName1 == 0 || *layerName1 == 0)
    return LEFW_BAD_DATA;
  if (!layerName2 || layerName2 == 0 || *layerName2 == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " SAMENET %s %s %g ", layerName1, layerName2,
	     minSpace);
    if (stack && strcmp(stack, "STACK") == 0)
      encPrint(lefwFile, (char *) "STACK ");
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " SAMENET %s %s %g ", layerName1, layerName2,
	    minSpace);
    if (stack && strcmp(stack, "STACK") == 0)
      fprintf(lefwFile, "STACK ");
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_SPACING;
  return LEFW_OK;
}

int 
lefwEndSpacing()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_SPACING_START &&
      lefwState != LEFW_SPACING)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END SPACING\n\n");
  else
    fprintf(lefwFile, "END SPACING\n\n");
  lefwLines++;
  lefwState = LEFW_SPACING_END;
  return LEFW_OK;
}

int 
lefwUniversalNoiseMargin(double high, double low)
{
  lefwObsoleteNum = LEFW_NOISEMARGIN;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_NOISEMARGIN])
    return LEFW_ALREADY_DEFINED;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "UNIVERSALNOISEMARGIN %g %g ;\n", high, low);
  else
    fprintf(lefwFile, "UNIVERSALNOISEMARGIN %g %g ;\n", high, low);
  lefwSynArray[LEFW_NOISEMARGIN] = 1;
  return LEFW_OK;
}

int 
lefwEdgeRateThreshold1(double num)
{
  lefwObsoleteNum = LEFW_THRESHOLD1;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_THRESHOLD1])
    return LEFW_ALREADY_DEFINED;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "EDGERATETHRESHOLD1 %g ;\n", num);
  else
    fprintf(lefwFile, "EDGERATETHRESHOLD1 %g ;\n", num);
  lefwSynArray[LEFW_THRESHOLD1] = 1;
  return LEFW_OK;
}

int 
lefwEdgeRateThreshold2(double num)
{
  lefwObsoleteNum = LEFW_THRESHOLD2;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_THRESHOLD2])
    return LEFW_ALREADY_DEFINED;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "EDGERATETHRESHOLD2 %g ;\n", num);
  else
    fprintf(lefwFile, "EDGERATETHRESHOLD2 %g ;\n", num);
  lefwSynArray[LEFW_THRESHOLD2] = 1;
  return LEFW_OK;
}

int 
lefwEdgeRateScaleFactor(double num)
{
  lefwObsoleteNum = LEFW_SCALEFACTOR;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_SCALEFACTOR])
    return LEFW_ALREADY_DEFINED;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "EDGERATESCALEFACTOR %g ;\n", num);
  else
    fprintf(lefwFile, "EDGERATESCALEFACTOR %g ;\n", num);
  lefwSynArray[LEFW_SCALEFACTOR] = 1;
  return LEFW_OK;
}

int 
lefwStartNoiseTable(int num)
{
  lefwObsoleteNum = LEFW_NOISETABLE_START;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_NOISETABLE_START ||
      lefwState == LEFW_NOISETABLE)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_NOISETABLE_START])
    return LEFW_ALREADY_DEFINED;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "NOISETABLE %d ;\n", num);
  else
    fprintf(lefwFile, "NOISETABLE %d ;\n", num);
  lefwLines++;
  lefwState = LEFW_NOISETABLE_START;
  lefwIsNoiseTable = 1;
  lefwIsEdgerate = 0;
  lefwSynArray[LEFW_NOISETABLE_START] = 1;
  return LEFW_OK;
}

/*
 * for both noisetable and correctiontable
 *//* Obsolete in 5.4 */ int 
lefwEdgeRate(double num)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsNoiseTable && !lefwIsCorrectTable)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " EDGERATE %g ;\n", num);
  else
    fprintf(lefwFile, " EDGERATE %g ;\n", num);
  lefwLines++;
  if (lefwState == LEFW_NOISETABLE_START)
    lefwState = LEFW_NOISETABLE;
  else if (lefwState == LEFW_CORRECTTABLE_START)
    lefwState = LEFW_CORRECTTABLE;
  lefwIsEdgerate = 1;
  lefwIsOutResist = 0;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwOutputResistance(int numResists, double *resistance)
{
  int     i;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsEdgerate)
    return LEFW_BAD_ORDER;
  if (lefwIsOutResist)
    return LEFW_BAD_ORDER;
  if (numResists <= 0)
    return LEFW_OK;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " OUTPUTRESISTANCE ");
    for (i = 0; i < numResists; i++) {
      encPrint(lefwFile, (char *) "%g ", *resistance++);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " OUTPUTRESISTANCE ");
    for (i = 0; i < numResists; i++) {
      fprintf(lefwFile, "%g ", *resistance++);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwIsOutResist = 1;
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwVictims(int length, int numNoises, double *noises)
{
  int     i;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsOutResist)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " VICTIMLENGTH %d ;\n", length);
    if (numNoises <= 0)
      return LEFW_BAD_DATA;	/* noises are required */
    for (i = 0; i < numNoises; i++) {
      if (lefwIsNoiseTable)
	encPrint(lefwFile, (char *) " VICTIMNOISE ");
      else
	encPrint(lefwFile, (char *) " CORRECTIONFACTOR ");
      encPrint(lefwFile, (char *) "%g ", *noises++);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " VICTIMLENGTH %d ;\n", length);
    if (numNoises <= 0)
      return LEFW_BAD_DATA;	/* noises are required */
    for (i = 0; i < numNoises; i++) {
      if (lefwIsNoiseTable)
	fprintf(lefwFile, " VICTIMNOISE ");
      else
	fprintf(lefwFile, " CORRECTIONFACTOR ");
      fprintf(lefwFile, "%g ", *noises++);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwEndNoiseTable()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_NOISETABLE_START &&
      lefwState != LEFW_NOISETABLE)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END NOISETABLE\n\n");
  else
    fprintf(lefwFile, "END NOISETABLE\n\n");
  lefwLines++;
  lefwState = LEFW_NOISETABLE_END;
  lefwIsNoiseTable = 0;
  lefwIsOutResist = 0;
  return LEFW_OK;
}

int 
lefwStartCorrectTable(int num)
{
  lefwObsoleteNum = LEFW_CORRECTTABLE_START;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_CORRECTTABLE_START ||
      lefwState == LEFW_CORRECTTABLE)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwSynArray[LEFW_CORRECTTABLE_START])
    return LEFW_ALREADY_DEFINED;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "CORRECTIONTABLE %d ;\n", num);
  else
    fprintf(lefwFile, "CORRECTIONTABLE %d ;\n", num);
  lefwLines++;
  lefwState = LEFW_CORRECTTABLE_START;
  lefwIsCorrectTable = 1;
  lefwIsEdgerate = 0;
  lefwSynArray[LEFW_CORRECTTABLE_START] = 1;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwEndCorrectTable()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_CORRECTTABLE_START &&
      lefwState != LEFW_CORRECTTABLE)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END CORRECTIONTABLE\n\n");
  else
    fprintf(lefwFile, "END CORRECTIONTABLE\n\n");
  lefwLines++;
  lefwState = LEFW_CORRECTTABLE_END;
  lefwIsNoiseTable = 0;
  return LEFW_OK;
}

int 
lefwMinFeature(double x, double y)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_MINFEATURE])
    return LEFW_ALREADY_DEFINED;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "MINFEATURE %g %g ;\n", x, y);
  else
    fprintf(lefwFile, "MINFEATURE %g %g ;\n", x, y);
  lefwSynArray[LEFW_MINFEATURE] = 1;
  return LEFW_OK;
}

/*
 * ******************** int lefwDielectric (float dielectric) { if (!
 * lefwFile) return LEFW_UNINITIALIZED; if (! lefwDidInit) return
 * LEFW_BAD_ORDER; if (lefwState != LEFW_INIT && lefwState < LEFW_END) return
 * LEFW_BAD_ORDER; if (lefwSynArray[LEFW_DIELECTRIC]) return
 * LEFW_ALREADY_DEFINED; fprintf(lefwFile, "DIELECTRIC %g ;\n", dielectric);
 * lefwSynArray[LEFW_DIELECTRIC] = 1; return LEFW_OK; } *******************
 */
int 
lefwStartIrdrop()
{
  lefwObsoleteNum = LEFW_IRDROP_START;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_IRDROP_START ||
      lefwState == LEFW_IRDROP)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_IRDROP_START])
    return LEFW_ALREADY_DEFINED;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "IRDROP\n");
  else
    fprintf(lefwFile, "IRDROP\n");
  lefwLines++;
  lefwState = LEFW_IRDROP_START;
  lefwSynArray[LEFW_IRDROP_START] = 1;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwIrdropTable(const char *tableName, const char *currentsNvolts)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_IRDROP_START &&
      lefwState != LEFW_IRDROP)
    return LEFW_BAD_ORDER;
  if (!tableName || tableName == 0 || *tableName == 0)
    return LEFW_BAD_DATA;
  if (!currentsNvolts || currentsNvolts == 0 || *currentsNvolts == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " TABLE %s %s ;\n", tableName, currentsNvolts);
  else
    fprintf(lefwFile, " TABLE %s %s ;\n", tableName, currentsNvolts);
  lefwLines++;
  lefwState = LEFW_IRDROP;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwEndIrdrop()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_IRDROP_START &&
      lefwState != LEFW_IRDROP)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END IRDROP\n\n");
  else
    fprintf(lefwFile, "END IRDROP\n\n");
  lefwLines++;
  lefwState = LEFW_IRDROP_END;
  return LEFW_OK;
}

int 
lefwSite(const char *siteName, const char *classType, const char *symmetry,
	 double width, double height)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_SITE)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (!siteName || siteName == 0 || *siteName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) "SITE %s\n", siteName);
    if (classType && strcmp(classType, "PAD") && strcmp(classType, "CORE"))
      return LEFW_BAD_DATA;
    encPrint(lefwFile, (char *) " CLASS %s ;\n", classType);
    if (symmetry)
      encPrint(lefwFile, (char *) " SYMMETRY %s ;\n", symmetry);
    encPrint(lefwFile, (char *) " SIZE %g BY %g ;\n", width, height);

    /*
     * encPrint(lefwFile, (char*)"END %s\n\n", siteName);
     */ 
  }
  else {
    fprintf(lefwFile, "SITE %s\n", siteName);
    if (classType && strcmp(classType, "PAD") && strcmp(classType, "CORE"))
      return LEFW_BAD_DATA;
    fprintf(lefwFile, " CLASS %s ;\n", classType);
    if (symmetry)
      fprintf(lefwFile, " SYMMETRY %s ;\n", symmetry);
    fprintf(lefwFile, " SIZE %g BY %g ;\n", width, height);

    /*
     * a separate function call, lefwEndSite
     *//* fprintf(lefwFile, "END %s\n\n", siteName); */ 
  }
  lefwLines = lefwLines + 4;
  lefwSynArray[LEFW_SITE] = 1;
  lefwState = LEFW_SITE;
  return LEFW_OK;
}

int 
lefwEndSite(const char *siteName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_SITE)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END %s\n\n", siteName);
  else
    fprintf(lefwFile, "END %s\n\n", siteName);
  lefwLines++;
  lefwState = LEFW_SITE_END;
  return LEFW_OK;
}

int 
lefwStartArray(const char *arrayName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_ARRAY_START ||
      lefwState == LEFW_ARRAY)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (!arrayName || arrayName == 0 || *arrayName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "ARRAY %s\n", arrayName);
  else
    fprintf(lefwFile, "ARRAY %s\n", arrayName);
  lefwLines++;
  lefwState = LEFW_ARRAY_START;
  lefwSynArray[LEFW_ARRAY_START] = 1;
  lefwHasArrayReq = 0;
  lefwIsArrayFloorp = 0;
  return LEFW_OK;
}

int 
lefwArraySite(const char *name, double origX, double origY, int orient,
	      double numX, double numY, double spaceX, double spaceY)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_ARRAY_START &&
      lefwState != LEFW_ARRAY)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SITE %s %g %g %s DO %g BY %g STEP %g %g ;\n",
	name, origX, origY, lefwOrient(orient), numX, numY, spaceX, spaceY);
  else
    fprintf(lefwFile, " SITE %s %g %g %s DO %g BY %g STEP %g %g ;\n",
	name, origX, origY, lefwOrient(orient), numX, numY, spaceX, spaceY);
  lefwLines++;
  lefwHasArrayReq = 1;
  lefwState = LEFW_ARRAY;
  return LEFW_OK;
}

int 
lefwArraySiteStr(const char *name, double origX, double origY,
		 const char *orient, double numX, double numY,
		 double spaceX, double spaceY)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_ARRAY_START &&
      lefwState != LEFW_ARRAY)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SITE %s %g %g %s DO %g BY %g STEP %g %g ;\n",
	     name, origX, origY, orient, numX, numY, spaceX, spaceY);
  else
    fprintf(lefwFile, " SITE %s %g %g %s DO %g BY %g STEP %g %g ;\n",
	    name, origX, origY, orient, numX, numY, spaceX, spaceY);
  lefwLines++;
  lefwHasArrayReq = 1;
  lefwState = LEFW_ARRAY;
  return LEFW_OK;
}

int 
lefwArrayCanplace(const char *name, double origX, double origY, int orient,
		  double numX, double numY, double spaceX, double spaceY)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_ARRAY_START &&
      lefwState != LEFW_ARRAY)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " CANPLACE %s %g %g %s DO %g BY %g STEP %g %g ;\n",
	name, origX, origY, lefwOrient(orient), numX, numY, spaceX, spaceY);
  else
    fprintf(lefwFile, " CANPLACE %s %g %g %s DO %g BY %g STEP %g %g ;\n",
	name, origX, origY, lefwOrient(orient), numX, numY, spaceX, spaceY);
  lefwLines++;
  lefwHasArrayReq = 1;
  lefwState = LEFW_ARRAY;
  return LEFW_OK;
}

int 
lefwArrayCanplaceStr(const char *name, double origX, double origY,
		     const char *orient, double numX, double numY,
		     double spaceX, double spaceY)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_ARRAY_START &&
      lefwState != LEFW_ARRAY)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " CANPLACE %s %g %g %s DO %g BY %g STEP %g %g ;\n",
	     name, origX, origY, orient, numX, numY, spaceX, spaceY);
  else
    fprintf(lefwFile, " CANPLACE %s %g %g %s DO %g BY %g STEP %g %g ;\n",
	    name, origX, origY, orient, numX, numY, spaceX, spaceY);
  lefwLines++;
  lefwHasArrayReq = 1;
  lefwState = LEFW_ARRAY;
  return LEFW_OK;
}

int 
lefwArrayCannotoccupy(const char *name, double origX, double origY,
		      int orient, double numX, double numY, double spaceX,
		      double spaceY)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_ARRAY_START &&
      lefwState != LEFW_ARRAY)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " CANNOTOCCUPY %s %g %g %s DO %g BY %g STEP %g %g ;\n",
	name, origX, origY, lefwOrient(orient), numX, numY, spaceX, spaceY);
  else
    fprintf(lefwFile, " CANNOTOCCUPY %s %g %g %s DO %g BY %g STEP %g %g ;\n",
	name, origX, origY, lefwOrient(orient), numX, numY, spaceX, spaceY);
  lefwLines++;
  lefwHasArrayReq = 1;
  lefwState = LEFW_ARRAY;
  return LEFW_OK;
}

int 
lefwArrayCannotoccupyStr(const char *name, double origX, double origY,
			 const char *orient, double numX, double numY,
			 double spaceX, double spaceY)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_ARRAY_START &&
      lefwState != LEFW_ARRAY)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " CANNOTOCCUPY %s %g %g %s DO %g BY %g STEP %g %g ;\n",
	     name, origX, origY, orient, numX, numY, spaceX, spaceY);
  else
    fprintf(lefwFile, " CANNOTOCCUPY %s %g %g %s DO %g BY %g STEP %g %g ;\n",
	    name, origX, origY, orient, numX, numY, spaceX, spaceY);
  lefwLines++;
  lefwHasArrayReq = 1;
  lefwState = LEFW_ARRAY;
  return LEFW_OK;
}

int 
lefwArrayTracks(const char *xy, double start, int numTracks, double space,
		const char *layers)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_ARRAY_START &&
      lefwState != LEFW_ARRAY)
    return LEFW_BAD_ORDER;
  if (!xy || xy == 0 || *xy == 0)
    return LEFW_BAD_DATA;
  if (strcmp(xy, "X") && strcmp(xy, "Y"))
    return LEFW_BAD_DATA;
  if (!layers || layers == 0 || *layers == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " TRACKS %s %g DO %d STEP %g LAYER %s ;\n",
	     xy, start, numTracks, space, layers);
  else
    fprintf(lefwFile, " TRACKS %s %g DO %d STEP %g LAYER %s ;\n",
	    xy, start, numTracks, space, layers);
  lefwLines++;
  lefwHasArrayReq = 1;
  lefwState = LEFW_ARRAY;
  return LEFW_OK;
}

int 
lefwStartArrayFloorplan(const char *name)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_ARRAY_START &&
      lefwState != LEFW_ARRAY)
    return LEFW_BAD_ORDER;
  if (lefwIsArrayFloorp)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " FLOORPLAN %s\n", name);
  else
    fprintf(lefwFile, " FLOORPLAN %s\n", name);
  lefwLines++;
  lefwHasArrayReq = 1;
  lefwIsArrayFloorp = 1;
  return LEFW_OK;
}

int 
lefwArrayFloorplan(const char *site, const char *name,
		   double origX, double origY, int orient, int numX,
		   int numY, double spaceX, double spaceY)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwIsArrayFloorp)
    return LEFW_BAD_ORDER;
  if (site && strcmp(site, "CANPLACE") && strcmp(site, "CANNOTOCCUPY"))
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " %s %s %g %g %s DO %d BY %d STEP %g %g ;\n", site,
	name, origX, origY, lefwOrient(orient), numX, numY, spaceX, spaceY);
  else
    fprintf(lefwFile, " %s %s %g %g %s DO %d BY %d STEP %g %g ;\n", site,
	name, origX, origY, lefwOrient(orient), numX, numY, spaceX, spaceY);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwArrayFloorplanStr(const char *site, const char *name, double origX,
		      double origY, const char *orient, int numX,
		      int numY, double spaceX, double spaceY)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwIsArrayFloorp)
    return LEFW_BAD_ORDER;
  if (site && strcmp(site, "CANPLACE") && strcmp(site, "CANNOTOCCUPY"))
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " %s %s %g %g %s DO %d BY %d STEP %g %g ;\n", site,
	     name, origX, origY, orient, numX, numY, spaceX, spaceY);
  else
    fprintf(lefwFile, " %s %s %g %g %s DO %d BY %d STEP %g %g ;\n", site,
	    name, origX, origY, orient, numX, numY, spaceX, spaceY);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwEndArrayFloorplan(const char *name)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwIsArrayFloorp)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " END %s\n\n", name);
  else
    fprintf(lefwFile, " END %s\n\n", name);
  lefwLines++;
  lefwIsArrayFloorp = 0;
  return LEFW_OK;
}

int 
lefwArrayGcellgrid(const char *xy, double startXY, int colRows,
		   double spaceXY)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_ARRAY_START &&
      lefwState != LEFW_ARRAY)
    return LEFW_BAD_ORDER;
  if (!xy || xy == 0 || *xy == 0)
    return LEFW_BAD_DATA;
  if (strcmp(xy, "X") && strcmp(xy, "Y"))
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " GCELLGRID %s %g DO %d STEP %g ;\n", xy,
	     startXY, colRows, spaceXY);
  else
    fprintf(lefwFile, " GCELLGRID %s %g DO %d STEP %g ;\n", xy,
	    startXY, colRows, spaceXY);
  lefwLines++;
  lefwHasArrayReq = 1;
  lefwState = LEFW_ARRAY;
  return LEFW_OK;
}

int 
lefwStartArrayDefaultCap(int size)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_ARRAY_START &&
      lefwState != LEFW_ARRAY)
    return LEFW_BAD_ORDER;
  if (lefwIsArrayDef)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " DEFAULTCAP %d\n", size);
  else
    fprintf(lefwFile, " DEFAULTCAP %d\n", size);
  lefwLines++;
  lefwIsArrayDef = 1;
  return LEFW_OK;
}

int 
lefwArrayDefaultCap(double numPins, double cap)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwIsArrayDef)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " MINPINS %g WIRECAP %g ;\n", numPins, cap);
  else
    fprintf(lefwFile, " MINPINS %g WIRECAP %g ;\n", numPins, cap);
  lefwLines++;
  return LEFW_OK;
}

int 
lefwEndArrayDefaultCap()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwIsArrayDef)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " END DEFAULTCAP\n\n");
  else
    fprintf(lefwFile, " END DEFAULTCAP\n\n");
  lefwLines++;
  lefwIsArrayDef = 0;
  return LEFW_OK;
}

int 
lefwEndArray(const char *arrayName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_ARRAY_START &&
      lefwState != LEFW_ARRAY)
    return LEFW_BAD_ORDER;
  if (!arrayName || arrayName == 0 || *arrayName == 0)
    return LEFW_BAD_DATA;
  if (lefwIsFloorp || lefwIsArrayDef)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END %s\n\n", arrayName);
  else
    fprintf(lefwFile, "END %s\n\n", arrayName);
  lefwLines++;
  lefwState = LEFW_ARRAY_END;
  return LEFW_OK;
}

int 
lefwStartMacro(const char *macroName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState == LEFW_MACRO_START ||
      lefwState == LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (!macroName || macroName == 0 || *macroName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "MACRO %s\n", macroName);
  else
    fprintf(lefwFile, "MACRO %s\n", macroName);
  lefwLines++;
  lefwState = LEFW_MACRO_START;
  lefwIsMacroPin = 0;
  lefwIsMacroObs = 0;
  lefwIsMacroTiming = 0;
  return LEFW_OK;
}

int 
lefwMacroClass(const char *value1, const char *value2)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (!value1 || value1 == 0 || *value1 == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    if (strcmp(value1, "RING") == 0)
      encPrint(lefwFile, (char *) " CLASS %s ;\n", value1);
    else if (strcmp(value1, "BLOCK") == 0) {
      if (value2) {
	if (strcmp(value2, "BLACKBOX"))
	  return LEFW_BAD_DATA;
	encPrint(lefwFile, (char *) " CLASS %s %s ;\n", value1, value2);
      }
      else
	encPrint(lefwFile, (char *) " CLASS %s ;\n", value1);
    }
    else if (strcmp(value1, "COVER") == 0) {
      if (value2) {
	if (strcmp(value2, "BUMP"))
	  return LEFW_BAD_DATA;
	encPrint(lefwFile, (char *) " CLASS %s %s ;\n", value1, value2);
      }
      else
	encPrint(lefwFile, (char *) " CLASS %s ;\n", value1);
    }
    else if (strcmp(value1, "PAD") == 0) {
      if (value2) {
	if (strcmp(value2, "INPUT") && strcmp(value2, "OUTPUT") &&
	    strcmp(value2, "INOUT") && strcmp(value2, "POWER") &&
	    strcmp(value2, "SPACER") && strcmp(value2, "AREAIO"))
	  return LEFW_BAD_DATA;
	encPrint(lefwFile, (char *) " CLASS %s %s ;\n", value1, value2);
      }
      else
	encPrint(lefwFile, (char *) " CLASS %s ;\n", value1);
    }
    else if (strcmp(value1, "CORE") == 0) {
      if (value2) {
	if (strcmp(value2, "FEEDTHRU") && strcmp(value2, "TIEHIGH") &&
	    strcmp(value2, "TIELOW") && strcmp(value2, "SPACER") &&
	    strcmp(value2, "ANTENNACELL"))
	  return LEFW_BAD_DATA;
	encPrint(lefwFile, (char *) " CLASS %s %s ;\n", value1, value2);
      }
      else
	encPrint(lefwFile, (char *) " CLASS %s ;\n", value1);
    }
    else if (strcmp(value1, "ENDCAP") == 0) {
      if (value2) {
	if (strcmp(value2, "PRE") && strcmp(value2, "POST") &&
	    strcmp(value2, "TOPLEFT") && strcmp(value2, "TOPRIGHT") &&
	    strcmp(value2, "BOTTOMLEFT") && strcmp(value2, "BOTTOMRIGHT"))
	  return LEFW_BAD_DATA;
	encPrint(lefwFile, (char *) " CLASS %s %s ;\n", value1, value2);
      }
      else
	return LEFW_BAD_DATA;	/* require 2nd value */
    }
    else
      return LEFW_BAD_DATA;
  }
  else {
    if (strcmp(value1, "RING") == 0)
      fprintf(lefwFile, " CLASS %s ;\n", value1);
    else if (strcmp(value1, "BLOCK") == 0) {
      if (value2) {
	if (strcmp(value2, "BLACKBOX"))
	  return LEFW_BAD_DATA;
	fprintf(lefwFile, " CLASS %s %s ;\n", value1, value2);
      }
      else
	fprintf(lefwFile, " CLASS %s ;\n", value1);
    }
    else if (strcmp(value1, "COVER") == 0) {
      if (value2) {
	if (strcmp(value2, "BUMP"))
	  return LEFW_BAD_DATA;
	fprintf(lefwFile, " CLASS %s %s ;\n", value1, value2);
      }
      else
	fprintf(lefwFile, " CLASS %s ;\n", value1);
    }
    else if (strcmp(value1, "PAD") == 0) {
      if (value2) {
	if (strcmp(value2, "INPUT") && strcmp(value2, "OUTPUT") &&
	    strcmp(value2, "INOUT") && strcmp(value2, "POWER") &&
	    strcmp(value2, "SPACER") && strcmp(value2, "AREAIO"))
	  return LEFW_BAD_DATA;
	fprintf(lefwFile, " CLASS %s %s ;\n", value1, value2);
      }
      else
	fprintf(lefwFile, " CLASS %s ;\n", value1);
    }
    else if (strcmp(value1, "CORE") == 0) {
      if (value2) {
	if (strcmp(value2, "FEEDTHRU") && strcmp(value2, "TIEHIGH") &&
	    strcmp(value2, "TIELOW") && strcmp(value2, "SPACER") &&
	    strcmp(value2, "ANTENNACELL"))
	  return LEFW_BAD_DATA;
	fprintf(lefwFile, " CLASS %s %s ;\n", value1, value2);
      }
      else
	fprintf(lefwFile, " CLASS %s ;\n", value1);
    }
    else if (strcmp(value1, "ENDCAP") == 0) {
      if (value2) {
	if (strcmp(value2, "PRE") && strcmp(value2, "POST") &&
	    strcmp(value2, "TOPLEFT") && strcmp(value2, "TOPRIGHT") &&
	    strcmp(value2, "BOTTOMLEFT") && strcmp(value2, "BOTTOMRIGHT"))
	  return LEFW_BAD_DATA;
	fprintf(lefwFile, " CLASS %s %s ;\n", value1, value2);
      }
      else
	return LEFW_BAD_DATA;	/* require 2nd value */
    }
    else
      return LEFW_BAD_DATA;
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroSource(const char *value1)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (!value1 || value1 == 0 || *value1 == 0)
    return LEFW_BAD_DATA;
  if (strcmp(value1, "USER") == 0 || strcmp(value1, "GENERATE") == 0 ||
      strcmp(value1, "BLOCK") == 0)
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) " SOURCE %s ;\n", value1);
    else
      fprintf(lefwFile, " SOURCE %s ;\n", value1);
  else
    return LEFW_BAD_DATA;
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroForeign(const char *name,
		 double xl, double yl, int orient)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " FOREIGN %s ", name);
    if (xl || yl) {
      encPrint(lefwFile, (char *) "%g %g ", xl, yl);
      if (orient >= 0 && orient <= 7)
	encPrint(lefwFile, (char *) "%s ", lefwOrient(orient));
    }
    else if (orient > 0 && orient <= 7) {	/* assume user has 0, 0 pt  */
      encPrint(lefwFile, (char *) "%g %g ", xl, yl);
      encPrint(lefwFile, (char *) "%s ", lefwOrient(orient));
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " FOREIGN %s ", name);
    if (xl || yl) {
      fprintf(lefwFile, "%g %g ", xl, yl);
      if (orient >= 0 && orient <= 7)
	fprintf(lefwFile, "%s ", lefwOrient(orient));
    }
    else if (orient > 0 && orient <= 7) {	/* assume user has 0, 0 pt  */
      fprintf(lefwFile, "%g %g ", xl, yl);
      fprintf(lefwFile, "%s ", lefwOrient(orient));
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroForeignStr(const char *name,
		    double xl, double yl, const char *orient)
{				/* optional */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) "   FOREIGN %s ", name);
    if (xl || yl) {
      encPrint(lefwFile, (char *) "%g %g ", xl, yl);
      if (orient && *orient != '\0')
	encPrint(lefwFile, (char *) "%s ", orient);
    }
    else if (orient && *orient != '\0') {	/* assume user has 0, 0 pt  */
      encPrint(lefwFile, (char *) "%g %g ", xl, yl);
      encPrint(lefwFile, (char *) "%s ", orient);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " FOREIGN %s ", name);
    if (xl || yl) {
      fprintf(lefwFile, "%g %g ", xl, yl);
      if (orient && *orient != '\0')
	fprintf(lefwFile, "%s ", orient);
    }
    else if (orient && *orient != '\0') {	/* assume user has 0, 0 pt  */
      fprintf(lefwFile, "%g %g ", xl, yl);
      fprintf(lefwFile, "%s ", orient);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroOrigin(double xl, double yl)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ORIGIN %g %g ;\n", xl, yl);
  else
    fprintf(lefwFile, " ORIGIN %g %g ;\n", xl, yl);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroEEQ(const char *macroName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (!macroName || macroName == 0 || *macroName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " EEQ %s ;\n", macroName);
  else
    fprintf(lefwFile, " EEQ %s ;\n", macroName);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroLEQ(const char *macroName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (!macroName || macroName == 0 || *macroName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " LEQ %s ;\n", macroName);
  else
    fprintf(lefwFile, " LEQ %s ;\n", macroName);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroSize(double width, double height)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SIZE %g BY %g ;\n", width, height);
  else
    fprintf(lefwFile, " SIZE %g BY %g ;\n", width, height);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroSymmetry(const char *symmetry)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (!symmetry || symmetry == 0 || *symmetry == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SYMMETRY %s ;\n", symmetry);
  else
    fprintf(lefwFile, " SYMMETRY %s ;\n", symmetry);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroSite(const char *siteName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (!siteName || siteName == 0 || *siteName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SITE %s ;\n", siteName);
  else
    fprintf(lefwFile, " SITE %s ;\n", siteName);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroSitePattern(const char *name, double origX, double origY,
		     int orient, int numX, int numY, double spaceX,
		     double spaceY)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SITE %s %g %g %s DO %d BY %d STEP %g %g ;\n",
	name, origX, origY, lefwOrient(orient), numX, numY, spaceX, spaceY);
  else
    fprintf(lefwFile, " SITE %s %g %g %s DO %d BY %d STEP %g %g ;\n",
	name, origX, origY, lefwOrient(orient), numX, numY, spaceX, spaceY);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroSitePatternStr(const char *name, double origX, double origY,
			const char *orient, int numX, int numY,
			double spaceX, double spaceY)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " SITE %s %g %g %s DO %d BY %d STEP %g %g ;\n",
	     name, origX, origY, orient, numX, numY, spaceX, spaceY);
  else
    fprintf(lefwFile, " SITE %s %g %g %s DO %d BY %d STEP %g %g ;\n",
	    name, origX, origY, orient, numX, numY, spaceX, spaceY);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPower(double power)
{
  lefwObsoleteNum = LEFW_MACRO_POWER;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " POWER %g ;\n", power);
  else
    fprintf(lefwFile, " POWER %g ;\n", power);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwEndMacro(const char *macroName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (!macroName || macroName == 0 || *macroName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "END %s\n\n", macroName);
  else
    fprintf(lefwFile, "END %s\n\n", macroName);
  lefwLines++;
  lefwState = LEFW_MACRO_END;
  return LEFW_OK;
}

int 
lefwStartMacroPin(const char *pinName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (!pinName || pinName == 0 || *pinName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " PIN %s\n", pinName);
  else
    fprintf(lefwFile, " PIN %s\n", pinName);
  lefwIsMacroPin = 1;
  lefwIsMacroPinPort = 0;	/* port within macropin */
  lefwLines++;
  lefwAMaxAreaCar = 0;		/* reset */
  lefwAMaxSideAreaCar = 0;	/* reset */
  lefwAMaxCutCar = 0;		/* reset */
  return LEFW_OK;
}

int 
lefwMacroPinTaperRule(const char *ruleName)
{				/* opitonal  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (!ruleName || ruleName == 0 || *ruleName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " TAPERRULE %s ;\n", ruleName);
  else
    fprintf(lefwFile, " TAPERRULE %s ;\n", ruleName);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinForeign(const char *name,
		    double xl, double yl, int orient)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " FOREIGN %s ", name);
    if (xl || yl) {
      encPrint(lefwFile, (char *) "STRUCTURE %g %g ", xl, yl);
      if (orient >= 0 && orient <= 7)
	encPrint(lefwFile, (char *) "%s ", lefwOrient(orient));
    }
    else if (orient > 0 && orient <= 7) {	/* assume user has 0 0 for pt  */
      encPrint(lefwFile, (char *) "STRUCTURE %g %g ", xl, yl);
      encPrint(lefwFile, (char *) "%s ", lefwOrient(orient));
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " FOREIGN %s ", name);
    if (xl || yl) {
      fprintf(lefwFile, "STRUCTURE %g %g ", xl, yl);
      if (orient >= 0 && orient <= 7)
	fprintf(lefwFile, "%s ", lefwOrient(orient));
    }
    else if (orient > 0 && orient <= 7) {	/* assume user has 0 0 for pt  */
      fprintf(lefwFile, "STRUCTURE %g %g ", xl, yl);
      fprintf(lefwFile, "%s ", lefwOrient(orient));
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinForeignStr(const char *name,
		       double xl, double yl,	/* optional  */
		       const char *orient)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " FOREIGN %s ", name);
    if (xl || yl) {
      encPrint(lefwFile, (char *) "STRUCTURE %g %g ", xl, yl);
      if (orient && *orient != '\0')
	encPrint(lefwFile, (char *) "%s ", orient);
    }
    else if (orient && *orient != '\0') {	/* assume user has 0 0 for pt  */
      encPrint(lefwFile, (char *) "STRUCTURE %g %g ", xl, yl);
      encPrint(lefwFile, (char *) "%s ", orient);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " FOREIGN %s ", name);
    if (xl || yl) {
      fprintf(lefwFile, "STRUCTURE %g %g ", xl, yl);
      if (orient && *orient != '\0')
	fprintf(lefwFile, "%s ", orient);
    }
    else if (orient && *orient != '\0') {	/* assume user has 0 0 for pt  */
      fprintf(lefwFile, "STRUCTURE %g %g ", xl, yl);
      fprintf(lefwFile, "%s ", orient);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinLEQ(const char *pinName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (!pinName || pinName == 0 || *pinName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " LEQ %s ;\n", pinName);
  else
    fprintf(lefwFile, " LEQ %s ;\n", pinName);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinDirection(const char *direction)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (!direction || direction == 0 || *direction == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " DIRECTION %s ;\n", direction);
  else
    fprintf(lefwFile, " DIRECTION %s ;\n", direction);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinUse(const char *use)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (!use || use == 0 || *use == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " USE %s ;\n", use);
  else
    fprintf(lefwFile, " USE %s ;\n", use);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinShape(const char *name)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt) {
    if (!name || name == 0 || *name == 0)
      encPrint(lefwFile, (char *) " SHAPE ;\n");
    else
      encPrint(lefwFile, (char *) " SHAPE %s ;\n", name);
  }
  else {
    if (!name || name == 0 || *name == 0)
      fprintf(lefwFile, " SHAPE ;\n");
    else
      fprintf(lefwFile, " SHAPE %s ;\n", name);
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinMustjoin(const char *name)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " MUSTJOIN %s ;\n", name);
  else
    fprintf(lefwFile, " MUSTJOIN %s ;\n", name);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinOutputnoisemargin(int high, int low)
{
  lefwObsoleteNum = LEFW_MACRO_OUTPUTNOISEMARGIN;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " OUTPUTNOISEMARGIN %d %d ;\n", high, low);
  else
    fprintf(lefwFile, " OUTPUTNOISEMARGIN %d %d ;\n", high, low);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinOutputresistance(int high, int low)
{
  lefwObsoleteNum = LEFW_MACRO_OUTPUTRESISTANCE;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " OUTPUTRESISTANCE %d %d ;\n", high, low);
  else
    fprintf(lefwFile, " OUTPUTRESISTANCE %d %d ;\n", high, low);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinInputnoisemargin(int high, int low)
{
  lefwObsoleteNum = LEFW_MACRO_INPUTNOISEMARGIN;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " INPUTNOISEMARGIN %d %d ;\n", high, low);
  else
    fprintf(lefwFile, " INPUTNOISEMARGIN %d %d ;\n", high, low);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinPower(double power)
{
  lefwObsoleteNum = LEFW_MACRO_POWER;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " POWER %g ;\n", power);
  else
    fprintf(lefwFile, " POWER %g ;\n", power);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinLeakage(double leakage)
{
  lefwObsoleteNum = LEFW_MACRO_LEAKAGE;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " LEAKAGE %g ;\n", leakage);
  else
    fprintf(lefwFile, " LEAKAGE %g ;\n", leakage);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinCapacitance(double capacitance)
{
  lefwObsoleteNum = LEFW_MACRO_CAPACITANCE;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " CAPACITANCE %g ;\n", capacitance);
  else
    fprintf(lefwFile, " CAPACITANCE %g ;\n", capacitance);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinResistance(double resistance)
{
  lefwObsoleteNum = LEFW_MACRO_RESISTANCE;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " RESISTANCE %g ;\n", resistance);
  else
    fprintf(lefwFile, " RESISTANCE %g ;\n", resistance);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinPulldownres(double resistance)
{
  lefwObsoleteNum = LEFW_MACRO_PULLDOWNRES;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " PULLDOWNRES %g ;\n", resistance);
  else
    fprintf(lefwFile, " PULLDOWNRES %g ;\n", resistance);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinTieoffr(double resistance)
{
  lefwObsoleteNum = LEFW_MACRO_TIEOFFR;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " TIEOFFR %g ;\n", resistance);
  else
    fprintf(lefwFile, " TIEOFFR %g ;\n", resistance);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinVHI(double voltage)
{
  lefwObsoleteNum = LEFW_MACRO_VHI;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " VHI %g ;\n", voltage);
  else
    fprintf(lefwFile, " VHI %g ;\n", voltage);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinVLO(double voltage)
{
  lefwObsoleteNum = LEFW_MACRO_VLO;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " VLO %g ;\n", voltage);
  else
    fprintf(lefwFile, " VLO %g ;\n", voltage);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinRisevoltagethreshold(double voltage)
{
  lefwObsoleteNum = LEFW_MACRO_RISEVOLTAGETHRESHOLD;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " RISEVOLTAGETHRESHOLD %g ;\n", voltage);
  else
    fprintf(lefwFile, " RISEVOLTAGETHRESHOLD %g ;\n", voltage);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinFallvoltagethreshold(double voltage)
{
  lefwObsoleteNum = LEFW_MACRO_FALLVOLTAGETHRESHOLD;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " FALLVOLTAGETHRESHOLD %g ;\n", voltage);
  else
    fprintf(lefwFile, " FALLVOLTAGETHRESHOLD %g ;\n", voltage);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinRisethresh(double capacitance)
{
  lefwObsoleteNum = LEFW_MACRO_RISETHRESH;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " RISETHRESH %g ;\n", capacitance);
  else
    fprintf(lefwFile, " RISETHRESH %g ;\n", capacitance);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinFallthresh(double capacitance)
{
  lefwObsoleteNum = LEFW_MACRO_FALLTHRESH;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " FALLTHRESH %g ;\n", capacitance);
  else
    fprintf(lefwFile, " FALLTHRESH %g ;\n", capacitance);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinRisesatcur(double current)
{
  lefwObsoleteNum = LEFW_MACRO_RISESATCUR;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " RISESATCUR %g ;\n", current);
  else
    fprintf(lefwFile, " RISESATCUR %g ;\n", current);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinFallsatcur(double current)
{
  lefwObsoleteNum = LEFW_MACRO_FALLSATCUR;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " FALLSATCUR %g ;\n", current);
  else
    fprintf(lefwFile, " FALLSATCUR %g ;\n", current);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinCurrentsource(const char *name)
{
  lefwObsoleteNum = LEFW_MACRO_FALLCURRENTSOURCE;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (strcmp(name, "ACTIVE") && strcmp(name, "RESISTIVE"))
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " CURRENTSOURCE %s ;\n", name);
  else
    fprintf(lefwFile, " CURRENTSOURCE %s ;\n", name);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinIV_Tables(const char *lowName, const char *highName)
{
  lefwObsoleteNum = LEFW_MACRO_IV_TABLES;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (!lowName || lowName == 0 || *lowName == 0)
    return LEFW_BAD_DATA;
  if (!highName || highName == 0 || *highName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " IV_TABLES %s %s ;\n", lowName, highName);
  else
    fprintf(lefwFile, " IV_TABLES %s %s ;\n", lowName, highName);
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinAntennasize(double value, const char *layerName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (lefwAntenna54)
    return LEFW_MIX_VERSION_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNASIZE %g ", value);
    if (layerName)
      encPrint(lefwFile, (char *) "LAYER %s ", layerName);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNASIZE %g ", value);
    if (layerName)
      fprintf(lefwFile, "LAYER %s ", layerName);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  lefwAntenna53 = 1;
  return LEFW_OK;
}

int 
lefwMacroPinAntennaMetalArea(double value, const char *layerName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNAMETALAREA %g ", value);
    if (layerName)
      encPrint(lefwFile, (char *) "LAYER %s ", layerName);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNAMETALAREA %g ", value);
    if (layerName)
      fprintf(lefwFile, "LAYER %s ", layerName);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  return LEFW_OK;
}

int 
lefwMacroPinAntennaMetalLength(double value, const char *layerName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (lefwAntenna54)
    return LEFW_MIX_VERSION_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNAMETALLENGTH %g ", value);
    if (layerName)
      encPrint(lefwFile, (char *) "LAYER %s ", layerName);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNAMETALLENGTH %g ", value);
    if (layerName)
      fprintf(lefwFile, "LAYER %s ", layerName);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  lefwAntenna53 = 1;
  return LEFW_OK;
}

int 
lefwMacroPinAntennaPartialMetalArea(double value, const char *layerName)
{
  lefw54Num = LEFW_ANTENNAPARTIALMETALAREA;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwAntenna53)
    return LEFW_MIX_VERSION_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNAPARTIALMETALAREA %g ", value);
    if (layerName)
      encPrint(lefwFile, (char *) "LAYER %s ", layerName);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNAPARTIALMETALAREA %g ", value);
    if (layerName)
      fprintf(lefwFile, "LAYER %s ", layerName);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  lefwAntenna54 = 1;
  return LEFW_OK;
}

int 
lefwMacroPinAntennaPartialMetalSideArea(double value,
					const char *layerName)
{
  lefw54Num = LEFW_ANTENNAPARTIALMETALSIDEAREA;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwAntenna53)
    return LEFW_MIX_VERSION_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNAPARTIALMETALSIDEAREA %g ", value);
    if (layerName)
      encPrint(lefwFile, (char *) "LAYER %s ", layerName);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNAPARTIALMETALSIDEAREA %g ", value);
    if (layerName)
      fprintf(lefwFile, "LAYER %s ", layerName);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  lefwAntenna54 = 1;
  return LEFW_OK;
}

int 
lefwMacroPinAntennaPartialCutArea(double value, const char *layerName)
{
  lefw54Num = LEFW_ANTENNAPARTIALCUTAREA;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwAntenna53)
    return LEFW_MIX_VERSION_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNAPARTIALCUTAREA %g ", value);
    if (layerName)
      encPrint(lefwFile, (char *) "LAYER %s ", layerName);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNAPARTIALCUTAREA %g ", value);
    if (layerName)
      fprintf(lefwFile, "LAYER %s ", layerName);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  lefwAntenna54 = 1;
  return LEFW_OK;
}

int 
lefwMacroPinAntennaDiffArea(double value, const char *layerName)
{
  lefw54Num = LEFW_ANTENNADIFFAREA;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwAntenna53)
    return LEFW_MIX_VERSION_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNADIFFAREA %g ", value);
    if (layerName)
      encPrint(lefwFile, (char *) "LAYER %s ", layerName);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNADIFFAREA %g ", value);
    if (layerName)
      fprintf(lefwFile, "LAYER %s ", layerName);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  lefwAntenna54 = 1;
  return LEFW_OK;
}

int 
lefwMacroPinAntennaModel(const char *oxide)
{
  lefw54Num = LEFW_ANTENNAGATEAREA;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwAntenna53)
    return LEFW_MIX_VERSION_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNAMODEL %s ;\n", oxide);
  }
  else {
    fprintf(lefwFile, " ANTENNAMODEL %s ;\n", oxide);
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  lefwAntenna54 = 1;
  return LEFW_OK;
}

int 
lefwMacroPinAntennaGateArea(double value, const char *layerName)
{
  lefw54Num = LEFW_ANTENNAGATEAREA;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwAntenna53)
    return LEFW_MIX_VERSION_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNAGATEAREA %g ", value);
    if (layerName)
      encPrint(lefwFile, (char *) "LAYER %s ", layerName);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNAGATEAREA %g ", value);
    if (layerName)
      fprintf(lefwFile, "LAYER %s ", layerName);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  lefwAntenna54 = 1;
  return LEFW_OK;
}

int 
lefwMacroPinAntennaMaxAreaCar(double value, const char *layerName)
{
  lefw54Num = LEFW_ANTENNAMAXAREACAR;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (lefwAMaxAreaCar)
    return LEFW_BAD_DATA;	/* ANTENNAMAXAREACAR has defined *//* more
				 * than once */
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwAntenna53)
    return LEFW_MIX_VERSION_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNAMAXAREACAR %g ", value);
    if (layerName)
      encPrint(lefwFile, (char *) "LAYER %s ", layerName);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNAMAXAREACAR %g ", value);
    if (layerName)
      fprintf(lefwFile, "LAYER %s ", layerName);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  lefwAntenna54 = 1;
  return LEFW_OK;
}

int 
lefwMacroPinAntennaMaxSideAreaCar(double value, const char *layerName)
{
  lefw54Num = LEFW_ANTENNAMAXSIDEAREACAR;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (lefwAMaxSideAreaCar)
    return LEFW_BAD_DATA;	/* ANTENNAMAXSIDEAREACAR has *//* defined
				 * more than once */
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwAntenna53)
    return LEFW_MIX_VERSION_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNAMAXSIDEAREACAR %g ", value);
    if (layerName)
      encPrint(lefwFile, (char *) "LAYER %s ", layerName);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNAMAXSIDEAREACAR %g ", value);
    if (layerName)
      fprintf(lefwFile, "LAYER %s ", layerName);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  lefwAntenna54 = 1;
  return LEFW_OK;
}

int 
lefwMacroPinAntennaMaxCutCar(double value, const char *layerName)
{
  lefw54Num = LEFW_ANTENNAMAXCUTCAR;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (lefwAMaxCutCar)
    return LEFW_BAD_DATA;	/* ANTENNAMAXCUTCAR has *//* defined more
				 * than once */
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwAntenna53)
    return LEFW_MIX_VERSION_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " ANTENNAMAXCUTCAR %g ", value);
    if (layerName)
      encPrint(lefwFile, (char *) "LAYER %s ", layerName);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " ANTENNAMAXCUTCAR %g ", value);
    if (layerName)
      fprintf(lefwFile, "LAYER %s ", layerName);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwState = LEFW_MACRO;
  lefwAntenna54 = 1;
  return LEFW_OK;
}

int 
lefwEndMacroPin(const char *pinName)
{
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (!pinName || pinName == 0 || *pinName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " END %s\n\n", pinName);
  else
    fprintf(lefwFile, " END %s\n\n", pinName);
  lefwLines++;
  lefwIsMacroPin = 0;
  return LEFW_OK;
}

int 
lefwStartMacroPinPort(const char *classType)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPin)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt) {
    if (classType) {
      if (strcmp(classType, "NONE") && strcmp(classType, "CORE"))
	return LEFW_BAD_DATA;
      encPrint(lefwFile, (char *) " PORT CLASS %s ;\n", classType);
    }
    else
      encPrint(lefwFile, (char *) " PORT \n");
  }
  else {
    if (classType) {
      if (strcmp(classType, "NONE") && strcmp(classType, "CORE"))
	return LEFW_BAD_DATA;
      fprintf(lefwFile, " PORT CLASS %s ;\n", classType);
    }
    else
      fprintf(lefwFile, " PORT \n");
  }
  lefwLines++;
  lefwIsMacroPinPort = 1;
  lefwIsMacroPinPortLayer = 0;
  lefwSpacingVal = 0;
  lefwWidthVal = 0;
  return LEFW_OK;
}

int 
lefwMacroPinPortLayer(const char *layerName, double spacing)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPinPort)
    return LEFW_BAD_ORDER;
  if (!layerName || layerName == 0 || *layerName == 0)
    return LEFW_BAD_DATA;
  if (lefwWidthVal)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " LAYER %s ", layerName);
    if (spacing)
      encPrint(lefwFile, (char *) "SPACING %g ", spacing);
    encPrint(lefwFile, (char *) ";\n ");
  }
  else {
    fprintf(lefwFile, " LAYER %s ", layerName);
    if (spacing)
      fprintf(lefwFile, "SPACING %g ", spacing);
    fprintf(lefwFile, ";\n ");
  }
  lefwLines++;
  lefwIsMacroPinPortLayer = 1;
  lefwSpacingVal = 1;
  return LEFW_OK;
}

int 
lefwMacroPinPortDesignRuleWidth(const char *layerName, double width)
{
  lefw54Num = LEFW_DESIGNRULEWIDTH;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPinPort)
    return LEFW_BAD_ORDER;
  if (!layerName || layerName == 0 || *layerName == 0)
    return LEFW_BAD_DATA;
  if (lefwSpacingVal)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " LAYER %s ", layerName);
    if (width)
      encPrint(lefwFile, (char *) "DESIGNRULEWIDTH %g ", width);
    encPrint(lefwFile, (char *) ";\n ");
  }
  else {
    fprintf(lefwFile, " LAYER %s ", layerName);
    if (width)
      fprintf(lefwFile, "DESIGNRULEWIDTH %g ", width);
    fprintf(lefwFile, ";\n ");
  }
  lefwLines++;
  lefwIsMacroPinPortLayer = 1;
  lefwWidthVal = 1;
  return LEFW_OK;
}

int 
lefwMacroPinPortLayerWidth(double width)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPinPortLayer)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " WIDTH %g ;\n", width);
  else
    fprintf(lefwFile, " WIDTH %g ;\n", width);
  lefwLines++;
  lefwIsMacroPinPortLayer = 1;
  return LEFW_OK;
}

int 
lefwMacroPinPortLayerPath(int num_paths, double *xl, double *yl,
			  int numX, int numY,	/* optional  */
			  double spaceX, double spaceY)
{				/* optional  */
  int     i, numItem;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPinPortLayer)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt) {
    if (numX || numY || spaceX || spaceY) {
      encPrint(lefwFile, (char *) " PATH ITERATE ");
      for (i = 0; i < num_paths; i++) {
	if (i == 0)
	  encPrint(lefwFile, (char *) "%g %g\n", xl[i], yl[i]);
	else
	  encPrint(lefwFile, (char *) " %g %g\n", xl[i], yl[i]);
	lefwLines++;
      }
      encPrint(lefwFile, (char *)
	       " DO %d BY %d STEP %g %g ", numX, numY, spaceX, spaceY);
    }
    else {
      encPrint(lefwFile, (char *) " PATH ");
      numItem = 0;
      for (i = 0; i < num_paths; i++) {
	if (numItem > 5) {
	  encPrint(lefwFile, (char *) "\n");
	  encPrint(lefwFile, (char *) " %g %g ", xl[i], yl[i]);
	  numItem = 0;
	  lefwLines++;
	}
	else
	  encPrint(lefwFile, (char *) "%g %g ", xl[i], yl[i]);
	numItem++;
      }
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    if (numX || numY || spaceX || spaceY) {
      fprintf(lefwFile, " PATH ITERATE ");
      for (i = 0; i < num_paths; i++) {
	if (i == 0)
	  fprintf(lefwFile, "%g %g\n", xl[i], yl[i]);
	else
	  fprintf(lefwFile, " %g %g\n", xl[i], yl[i]);
	lefwLines++;
      }
      fprintf(lefwFile,
	      " DO %d BY %d STEP %g %g ", numX, numY, spaceX, spaceY);
    }
    else {
      fprintf(lefwFile, " PATH ");
      numItem = 0;
      for (i = 0; i < num_paths; i++) {
	if (numItem > 5) {
	  fprintf(lefwFile, "\n");
	  fprintf(lefwFile, " %g %g ", xl[i], yl[i]);
	  numItem = 0;
	  lefwLines++;
	}
	else
	  fprintf(lefwFile, "%g %g ", xl[i], yl[i]);
	numItem++;
      }
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMacroPinPortLayerRect(double xl1, double yl1,
			  double xl2, double yl2,
			  int numX, int numY,	/* optional  */
			  double spaceX, double spaceY)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPinPortLayer)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt) {
    if (numX || numY || spaceX || spaceY) {
      encPrint(lefwFile, (char *) " RECT ITERATE %g %g %g %g ",
	       xl1, yl1, xl2, yl2);
      encPrint(lefwFile, (char *) "DO %d BY %d STEP %g %g ", numX, numY, spaceX,
	       spaceY);
    }
    else {
      encPrint(lefwFile, (char *) " RECT %g %g %g %g ", xl1, yl1, xl2, yl2);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    if (numX || numY || spaceX || spaceY) {
      fprintf(lefwFile, " RECT ITERATE %g %g %g %g ",
	      xl1, yl1, xl2, yl2);
      fprintf(lefwFile, "DO %d BY %d STEP %g %g ", numX, numY, spaceX,
	      spaceY);
    }
    else {
      fprintf(lefwFile, " RECT %g %g %g %g ", xl1, yl1, xl2, yl2);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMacroPinPortLayerPolygon(int num_polys, double *xl, double *yl,
			     int numX, int numY,	/* optional  */
			     double spaceX, double spaceY)
{				/* optional  */
  int     i;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPinPortLayer)
    return LEFW_BAD_ORDER;
  if (num_polys < 4)
    return LEFW_BAD_DATA;	/* at least 4 points */
  if (lefwWriteEncrypt) {
    if (numX || numY || spaceX || spaceY) {
      encPrint(lefwFile, (char *) " POLYGON ITERATE ");
      for (i = 0; i < num_polys; i++) {
	if (i == 0)
	  encPrint(lefwFile, (char *) "%g %g\n", *xl++, *yl++);
	else
	  encPrint(lefwFile, (char *) " %g %g\n", *xl++, *yl++);
	lefwLines++;
      }
      encPrint(lefwFile, (char *)
	       " DO %d BY %d STEP %g %g ;\n", numX, numY, spaceX,
	       spaceY);
    }
    else {
      encPrint(lefwFile, (char *) " POLYGON ");
      for (i = 0; i < num_polys; i++) {
	if (i == 0)
	  encPrint(lefwFile, (char *) "%g %g", *xl++, *yl++);
	else
	  encPrint(lefwFile, (char *) "\n %g %g", *xl++, *yl++);
	lefwLines++;
      }
      encPrint(lefwFile, (char *) " ;\n");
    }
  }
  else {
    if (numX || numY || spaceX || spaceY) {
      fprintf(lefwFile, " POLYGON ITERATE ");
      for (i = 0; i < num_polys; i++) {
	if (i == 0)
	  fprintf(lefwFile, "%g %g\n", *xl++, *yl++);
	else
	  fprintf(lefwFile, " %g %g\n", *xl++, *yl++);
	lefwLines++;
      }
      fprintf(lefwFile,
	      " DO %d BY %d STEP %g %g ;\n", numX, numY, spaceX,
	      spaceY);
    }
    else {
      fprintf(lefwFile, " POLYGON ");
      for (i = 0; i < num_polys; i++) {
	if (i == 0)
	  fprintf(lefwFile, "%g %g", *xl++, *yl++);
	else
	  fprintf(lefwFile, "\n %g %g", *xl++, *yl++);
	lefwLines++;
      }
      fprintf(lefwFile, " ;\n");
    }
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMacroPinPortVia(double xl, double yl, const char *viaName,
		    int numX, int numY,	/* optional  */
		    double spaceX, double spaceY)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroPinPort)
    return LEFW_BAD_ORDER;
  if (!viaName || viaName == 0 || *viaName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    if (numX || numY || spaceX || spaceY) {
      encPrint(lefwFile, (char *) " VIA ITERATE %g %g %s ", xl, yl,
	       viaName);
      encPrint(lefwFile, (char *) "DO %d BY %d STEP %g %g ", numX, numY, spaceX,
	       spaceY);
    }
    else
      encPrint(lefwFile, (char *) " VIA %g %g %s ", xl, yl, viaName);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    if (numX || numY || spaceX || spaceY) {
      fprintf(lefwFile, " VIA ITERATE %g %g %s ", xl, yl, viaName);
      fprintf(lefwFile, "DO %d BY %d STEP %g %g ", numX, numY, spaceX,
	      spaceY);
    }
    else
      fprintf(lefwFile, " VIA %g %g %s ", xl, yl, viaName);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwEndMacroPinPort()
{
  if (!lefwIsMacroPinPort)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " END\n");
  else
    fprintf(lefwFile, " END\n");
  lefwLines++;
  lefwIsMacroPinPort = 0;
  return LEFW_OK;
}

int 
lefwStartMacroObs()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (lefwIsMacroObs)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " OBS\n");
  else
    fprintf(lefwFile, " OBS\n");
  lefwIsMacroObs = 1;
  lefwSpacingVal = 0;
  lefwWidthVal = 0;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMacroObsLayer(const char *layerName, double spacing)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroObs)
    return LEFW_BAD_ORDER;
  if (!layerName || layerName == 0 || *layerName == 0)
    return LEFW_BAD_DATA;
  if (lefwWidthVal)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " LAYER %s ", layerName);
    if (spacing)
      encPrint(lefwFile, (char *) "SPACING %g ", spacing);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " LAYER %s ", layerName);
    if (spacing)
      fprintf(lefwFile, "SPACING %g ", spacing);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwIsMacroObsLayer = 1;
  lefwSpacingVal = 1;
  return LEFW_OK;
}

int 
lefwMacroObsDesignRuleWidth(const char *layerName, double width)
{
  lefw54Num = LEFW_DESIGNRULEWIDTH;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroObs)
    return LEFW_BAD_ORDER;
  if (!layerName || layerName == 0 || *layerName == 0)
    return LEFW_BAD_DATA;
  if (lefwSpacingVal)
    return LEFW_BAD_DATA;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " LAYER %s ", layerName);
    if (width)
      encPrint(lefwFile, (char *) "DESIGNRULEWIDTH %g ", width);
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    fprintf(lefwFile, " LAYER %s ", layerName);
    if (width)
      fprintf(lefwFile, "DESIGNRULEWIDTH %g ", width);
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwIsMacroObsLayer = 1;
  lefwWidthVal = 1;
  return LEFW_OK;
}

int 
lefwMacroObsLayerWidth(double width)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroObsLayer)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " WIDTH %g ;\n", width);
  else
    fprintf(lefwFile, " WIDTH %g ;\n", width);
  lefwLines++;
  lefwIsMacroObsLayer = 1;
  return LEFW_OK;
}

int 
lefwMacroObsLayerPath(int num_paths, double *xl, double *yl,
		      int numX, int numY,	/* optional  */
		      double spaceX, double spaceY)
{				/* optional  */
  int     i, numItem;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroObsLayer)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt) {
    if (numX || numY || spaceX || spaceY) {
      encPrint(lefwFile, (char *) " PATH ITERATE ");
      for (i = 0; i < num_paths; i++) {
	if (i == 0)
	  encPrint(lefwFile, (char *) "%g %g\n", *xl++, *yl++);
	else
	  encPrint(lefwFile, (char *) " %g %g\n", *xl++, *yl++);
	lefwLines++;
      }
      encPrint(lefwFile, (char *)
	       " DO %d BY %d STEP %g %g ", numX, numY, spaceX, spaceY);
    }
    else {
      encPrint(lefwFile, (char *) " PATH ");
      numItem = 0;
      for (i = 0; i < num_paths; i++) {
	if (numItem > 5) {
	  encPrint(lefwFile, (char *) "\n");
	  encPrint(lefwFile, (char *) " %g %g ", xl[i], yl[i]);
	  numItem = 0;
	  lefwLines++;
	}
	else
	  encPrint(lefwFile, (char *) "%g %g ", xl[i], yl[i]);
	numItem++;
      }
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    if (numX || numY || spaceX || spaceY) {
      fprintf(lefwFile, " PATH ITERATE ");
      for (i = 0; i < num_paths; i++) {
	if (i == 0)
	  fprintf(lefwFile, "%g %g\n", *xl++, *yl++);
	else
	  fprintf(lefwFile, " %g %g\n", *xl++, *yl++);
	lefwLines++;
      }
      fprintf(lefwFile,
	      " DO %d BY %d STEP %g %g ", numX, numY, spaceX, spaceY);
    }
    else {
      fprintf(lefwFile, " PATH ");
      numItem = 0;
      for (i = 0; i < num_paths; i++) {
	if (numItem > 5) {
	  fprintf(lefwFile, "\n");
	  fprintf(lefwFile, " %g %g ", xl[i], yl[i]);
	  numItem = 0;
	  lefwLines++;
	}
	else
	  fprintf(lefwFile, "%g %g ", xl[i], yl[i]);
	numItem++;
      }
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMacroObsLayerRect(double xl1, double yl1, double xl2, double yl2,
		      int numX, int numY,	/* optional  */
		      double spaceX, double spaceY)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroObsLayer)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt) {
    if (numX || numY || spaceX || spaceY) {
      encPrint(lefwFile, (char *) " RECT ITERATE %g %g %g %g ",
	       xl1, yl1, xl2, yl2);
      encPrint(lefwFile, (char *) "DO %d BY %d STEP %g %g ", numX, numY, spaceX,
	       spaceY);
    }
    else {
      encPrint(lefwFile, (char *) " RECT %g %g %g %g ", xl1, yl1, xl2, yl2);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    if (numX || numY || spaceX || spaceY) {
      fprintf(lefwFile, " RECT ITERATE %g %g %g %g ",
	      xl1, yl1, xl2, yl2);
      fprintf(lefwFile, "DO %d BY %d STEP %g %g ", numX, numY, spaceX,
	      spaceY);
    }
    else {
      fprintf(lefwFile, " RECT %g %g %g %g ", xl1, yl1, xl2, yl2);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMacroObsLayerPolygon(int num_polys, double *xl, double *yl,
			 int numX, int numY,	/* optional  */
			 double spaceX, double spaceY)
{				/* optional  */
  int     i;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroObsLayer)
    return LEFW_BAD_ORDER;
  if (num_polys < 4)
    return LEFW_BAD_DATA;	/* at least 4 points */
  if (lefwWriteEncrypt) {
    if (numX || numY || spaceX || spaceY) {
      encPrint(lefwFile, (char *) " POLYGON ITERATE ");
      for (i = 0; i < num_polys; i++) {
	if (i == 0)
	  encPrint(lefwFile, (char *) "%g %g\n", *xl++, *yl++);
	else
	  encPrint(lefwFile, (char *) " %g %g\n", *xl++, *yl++);
	lefwLines++;
      }
      encPrint(lefwFile, (char *)
	       " DO %d BY %d STEP %g %g ;", numX, numY, spaceX, spaceY);
    }
    else {
      encPrint(lefwFile, (char *) " POLYGON ");
      for (i = 0; i < num_polys; i++) {
	if (i == 0)
	  encPrint(lefwFile, (char *) "%g %g", *xl++, *yl++);
	else
	  encPrint(lefwFile, (char *) "\n %g %g", *xl++, *yl++);
	lefwLines++;
      }
      encPrint(lefwFile, (char *) " ;\n");
    }
  }
  else {
    if (numX || numY || spaceX || spaceY) {
      fprintf(lefwFile, " POLYGON ITERATE ");
      for (i = 0; i < num_polys; i++) {
	if (i == 0)
	  fprintf(lefwFile, "%g %g\n", *xl++, *yl++);
	else
	  fprintf(lefwFile, " %g %g\n", *xl++, *yl++);
	lefwLines++;
      }
      fprintf(lefwFile,
	      " DO %d BY %d STEP %g %g ;", numX, numY, spaceX, spaceY);
    }
    else {
      fprintf(lefwFile, " POLYGON ");
      for (i = 0; i < num_polys; i++) {
	if (i == 0)
	  fprintf(lefwFile, "%g %g", *xl++, *yl++);
	else
	  fprintf(lefwFile, "\n %g %g", *xl++, *yl++);
	lefwLines++;
      }
      fprintf(lefwFile, " ;\n");
    }
  }
  lefwLines++;
  return LEFW_OK;
}

int 
lefwMacroObsVia(double xl, double yl, const char *viaName,
		int numX, int numY,	/* optional  */
		double spaceX, double spaceY)
{				/* optional  */
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroObs)
    return LEFW_BAD_ORDER;
  if (!viaName || viaName == 0 || *viaName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    if (numX || numY || spaceX || spaceY) {
      encPrint(lefwFile, (char *) " VIA ITERATE %g %g %s ", xl, yl, viaName);
      encPrint(lefwFile, (char *) "DO %d BY %d STEP %g %g ", numX, numY, spaceX,
	       spaceY);
    }
    else {
      encPrint(lefwFile, (char *) " VIA %g %g %s ", xl, yl, viaName);
    }
    encPrint(lefwFile, (char *) ";\n");
  }
  else {
    if (numX || numY || spaceX || spaceY) {
      fprintf(lefwFile, " VIA ITERATE %g %g %s ", xl, yl, viaName);
      fprintf(lefwFile, "DO %d BY %d STEP %g %g ", numX, numY, spaceX,
	      spaceY);
    }
    else {
      fprintf(lefwFile, " VIA %g %g %s ", xl, yl, viaName);
    }
    fprintf(lefwFile, ";\n");
  }
  lefwLines++;
  lefwIsMacroObsLayer = 0;
  return LEFW_OK;
}

int 
lefwEndMacroObs()
{
  if (!lefwIsMacroObs)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " END\n");
  else
    fprintf(lefwFile, " END\n");
  lefwLines++;
  lefwIsMacroObs = 0;
  return LEFW_OK;
}

int 
lefwStartMacroTiming()
{
  lefwObsoleteNum = LEFW_MACRO_TIMING;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_MACRO_START &&
      lefwState != LEFW_MACRO)
    return LEFW_BAD_ORDER;
  if (lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (versionNum >= 5.4)
    return LEFW_OBSOLETE;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " TIMING\n");
  else
    fprintf(lefwFile, " TIMING\n");
  lefwIsMacroTiming = 1;
  lefwIsMacroTimingModel = 0;
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwMacroTimingPin(const char *fromPin, const char *toPin)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (fromPin) {
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) " FROMPIN %s ;\n", fromPin);
    else
      fprintf(lefwFile, " FROMPIN %s ;\n", fromPin);
    lefwLines++;
  }
  if (toPin) {
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) " TOPIN %s ;\n", toPin);
    else
      fprintf(lefwFile, " TOPIN %s ;\n", toPin);
    lefwLines++;
  }
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwMacroTimingIntrinsic(const char *riseFall, double min, double max,
			 double slewT1, double slewT1Min,	/* optional  */
			 double slewT1Max, double slewT2,	/* optional  */
			 double slewT2Min, double slewT2Max,	/* optional  */
			 double slewT3,	/* optional  */
			 double varMin, double varMax)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (strcmp(riseFall, "RISE") && strcmp(riseFall, "FALL"))
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt) {
    encPrint(lefwFile, (char *) " %s INTRINSIC %g %g ", riseFall, min, max);
    if (slewT1 || slewT1Min || slewT1Max || slewT2) {
      encPrint(lefwFile, (char *) "%g %g %g %g ", slewT1, slewT1Min, slewT1Max,
	       slewT2);
      if (slewT2Min || slewT2Max || slewT3)
	encPrint(lefwFile, (char *) "%g %g %g \n", slewT2Min, slewT2Max, slewT3);
    }
    else if (slewT2Min || slewT2Max || slewT3)
      return LEFW_BAD_DATA;	/* slewT2Min... has to be inside of slewT1... */
    encPrint(lefwFile, (char *) " VARIABLE %g %g ;\n", varMin, varMax);
  }
  else {
    fprintf(lefwFile, " %s INTRINSIC %g %g ", riseFall, min, max);
    if (slewT1 || slewT1Min || slewT1Max || slewT2) {
      fprintf(lefwFile, "%g %g %g %g ", slewT1, slewT1Min, slewT1Max, slewT2);
      if (slewT2Min || slewT2Max || slewT3)
	fprintf(lefwFile, "%g %g %g \n", slewT2Min, slewT2Max, slewT3);
    }
    else if (slewT2Min || slewT2Max || slewT3)
      return LEFW_BAD_DATA;	/* slewT2Min... has to be inside of slewT1... */
    fprintf(lefwFile, " VARIABLE %g %g ;\n", varMin, varMax);
  }
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwMacroTimingRisers(double min, double max)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " RISERS %g %g ;\n", min, max);
  else
    fprintf(lefwFile, " RISERS %g %g ;\n", min, max);
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwMacroTimingFallrs(double min, double max)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " FALLRS %g %g ;\n", min, max);
  else
    fprintf(lefwFile, " FALLRS %g %g ;\n", min, max);
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwMacroTimingRisecs(double min, double max)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " RISECS %g %g ;\n", min, max);
  else
    fprintf(lefwFile, " RISECS %g %g ;\n", min, max);
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwMacroTimingFallcs(double min, double max)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " FALLCS %g %g ;\n", min, max);
  else
    fprintf(lefwFile, " FALLCS %g %g ;\n", min, max);
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwMacroTimingRisesatt1(double min, double max)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " RISESATT1 %g %g ;\n", min, max);
  else
    fprintf(lefwFile, " RISESATT1 %g %g ;\n", min, max);
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwMacroTimingFallsatt1(double min, double max)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " FALLSATT1 %g %g ;\n", min, max);
  else
    fprintf(lefwFile, " FALLSATT1 %g %g ;\n", min, max);
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwMacroTimingRiset0(double min, double max)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " RISET0 %g %g ;\n", min, max);
  else
    fprintf(lefwFile, " RISET0 %g %g ;\n", min, max);
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwMacroTimingFallt0(double min, double max)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " FALLT0 %g %g ;\n", min, max);
  else
    fprintf(lefwFile, " FALLT0 %g %g ;\n", min, max);
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwMacroTimingUnateness(const char *unateness)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (!lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (strcmp(unateness, "INVERT") && strcmp(unateness, "NONINVERT") &&
      strcmp(unateness, "NONUNATE"))
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " UNATENESS %s ; \n", unateness);
  else
    fprintf(lefwFile, " UNATENESS %s ; \n", unateness);
  lefwLines++;
  return LEFW_OK;
}

/*
 * Obsolete in 5.4
 */ int 
lefwEndMacroTiming()
{
  if (!lefwIsMacroTiming)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " END TIMING\n\n");
  else
    fprintf(lefwFile, " END TIMING\n\n");
  lefwLines++;
  lefwIsMacroTiming = 0;
  return LEFW_OK;
}

int 
lefwAntenna(const char *type, double value)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (!type || type == 0 || *type == 0)
    return LEFW_BAD_DATA;
  if (strcmp(type, "INPUTPINANTENNASIZE") &&
      strcmp(type, "OUTPUTPINANTENNASIZE") &&
      strcmp(type, "INOUTPINANTENNASIZE"))
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "%s %g ;\n", type, value);
  else
    fprintf(lefwFile, "%s %g ;\n", type, value);
  lefwLines++;
  lefwSynArray[LEFW_ANTENNASIZE] = 1;
  return LEFW_OK;
}

int 
lefwAntennaInputGateArea(double inputGateArea)
{
  lefw54Num = LEFW_ANTENNAINPUTGATEAREA;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_ANTENNAINPUTGATEAREA])
    return LEFW_ALREADY_DEFINED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "ANTENNAINPUTGATEAREA %g ;\n",
	     inputGateArea);
  else
    fprintf(lefwFile, "ANTENNAINPUTGATEAREA %g ;\n", inputGateArea);
  lefwLines++;
  lefwSynArray[LEFW_ANTENNAINPUTGATEAREA] = 1;
  return LEFW_OK;
}

int 
lefwAntennaInOutDiffArea(double inOutDiffArea)
{
  lefw54Num = LEFW_ANTENNAINOUTDIFFAREA;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_ANTENNAINOUTDIFFAREA])
    return LEFW_ALREADY_DEFINED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "ANTENNAINOUTDIFFAREA %g ;\n",
	     inOutDiffArea);
  else
    fprintf(lefwFile, "ANTENNAINOUTDIFFAREA %g ;\n", inOutDiffArea);
  lefwLines++;
  lefwSynArray[LEFW_ANTENNAINOUTDIFFAREA] = 1;
  return LEFW_OK;
}

int 
lefwAntennaOutputDiffArea(double outputDiffArea)
{
  lefw54Num = LEFW_ANTENNAOUTPUTDIFFAREA;
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_INIT && lefwState < LEFW_END)
    return LEFW_BAD_ORDER;	/* not of the ENDs */
  if (lefwSynArray[LEFW_ANTENNAOUTPUTDIFFAREA])
    return LEFW_ALREADY_DEFINED;
  if (versionNum < 5.4)
    return LEFW_WRONG_VERSION;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "ANTENNAOUTPUTDIFFAREA %g ;\n",
	     outputDiffArea);
  else
    fprintf(lefwFile, "ANTENNAOUTPUTDIFFAREA %g ;\n", outputDiffArea);
  lefwLines++;
  lefwSynArray[LEFW_ANTENNAOUTPUTDIFFAREA] = 1;
  return LEFW_OK;
}

int 
lefwStartBeginext(const char *name)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState == LEFW_BEGINEXT_START ||
      lefwState == LEFW_BEGINEXT)
    return LEFW_BAD_ORDER;
  if (!name || name == 0 || *name == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "BEGINEXT \"%s\"", name);	/* \n will be added
								 * later */
  else
    fprintf(lefwFile, "BEGINEXT \"%s\"", name);
  lefwState = LEFW_BEGINEXT_START;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwBeginextCreator(const char *creatorName)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_BEGINEXT_START &&
      lefwState != LEFW_BEGINEXT)
    return LEFW_BAD_ORDER;
  if (!creatorName || creatorName == 0 || *creatorName == 0)
    return LEFW_BAD_DATA;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "\n CREATOR \"%s\"", creatorName);
  else
    fprintf(lefwFile, "\n CREATOR \"%s\"", creatorName);
  lefwState = LEFW_BEGINEXT;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwBeginextDate()
{
  time_t  todayTime;

  char   *rettime;

  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_BEGINEXT_START &&
      lefwState != LEFW_BEGINEXT)
    return LEFW_BAD_ORDER;
  todayTime = time(NULL);	/* time in UTC */
  rettime = ctime(&todayTime);	/* convert to string */
  rettime[strlen(rettime) - 1] = '\0';	/* replace \n with \0 */
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "\n DATE \"%s\"", rettime);
  else
    fprintf(lefwFile, "\n DATE \"%s\"", rettime);
  lefwState = LEFW_BEGINEXT;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwBeginextRevision(int vers1, int vers2)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_BEGINEXT_START &&
      lefwState != LEFW_BEGINEXT)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "\n REVISION %d.%d", vers1, vers2);
  else
    fprintf(lefwFile, "\n REVISION %d.%d", vers1, vers2);
  lefwState = LEFW_BEGINEXT;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwBeginextSyntax(const char *title, const char *string)
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_BEGINEXT_START &&
      lefwState != LEFW_BEGINEXT)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) "\n - %s %s", title, string);
  else
    fprintf(lefwFile, "\n - %s %s", title, string);
  lefwState = LEFW_BEGINEXT;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwEndBeginext()
{
  if (!lefwFile)
    return LEFW_UNINITIALIZED;
  if (!lefwDidInit)
    return LEFW_BAD_ORDER;
  if (lefwState != LEFW_BEGINEXT_START &&
      lefwState != LEFW_BEGINEXT)
    return LEFW_BAD_ORDER;
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) ";\nENDEXT\n\n");
  else
    fprintf(lefwFile, ";\nENDEXT\n\n");
  lefwState = LEFW_BEGINEXT_END;
  lefwLines++;
  return LEFW_OK;
}

int 
lefwCurrentLineNumber()
{
  return lefwLines;
}

void 
lefwPrintError(int status)
{
  switch (status) {
    case LEFW_OK:
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) "No Error.\n");
    else
      fprintf(lefwFile, "No Error.\n");
    break;
  case LEFW_UNINITIALIZED:
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) "Need to call lefwInit first.\n");
    else
      fprintf(lefwFile, "Need to call lefwInit first.\n");
    break;
  case LEFW_BAD_ORDER:
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) "Incorrect order of data.\n");
    else
      fprintf(lefwFile, "Incorrect order of data.\n");
    break;
  case LEFW_BAD_DATA:
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) "Invalid data.\n");
    else
      fprintf(lefwFile, "Invalid data.\n");
    break;
  case LEFW_ALREADY_DEFINED:
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) "Section is allowed to define only once.\n");
    else
      fprintf(lefwFile, "Section is allowed to define only once.\n");
    break;
  case LEFW_WRONG_VERSION:
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) "Version number is set before 5.4, API: %s is for 5.4.\n", lefwStateStr[lefw54Num]);
    else
      fprintf(lefwFile, "Version number is set before 5.4, but 5.4, API: %s is for 5.4.\n", lefwStateStr[lefw54Num]);
    break;
  case LEFW_MIX_VERSION_DATA:
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) "You have made both 5.4 & pre 5.4 Antenna API called, which is invalid.\n");
    else
      fprintf(lefwFile, "You have made both 5.4 & pre 5.4 Antenna API called, which is invalid.\n");
    break;
  case LEFW_OBSOLETE:
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) "%s is no longer valid in 5.4.\n",
	       lefwStateStr[lefwObsoleteNum]);
    else
      fprintf(lefwFile, "%s is no longer valid in 5.4.\n",
	      lefwStateStr[lefwObsoleteNum]);
  }
  return;
}

void 
lefwAddComment(const char *comment)
{
  if (comment) {
    if (lefwWriteEncrypt)
      encPrint(lefwFile, (char *) "# %s\n", comment);
    else
      fprintf(lefwFile, "# %s\n", comment);
  }
  return;
}

void 
lefwAddIndent()
{
  if (lefwWriteEncrypt)
    encPrint(lefwFile, (char *) " ");
  else
    fprintf(lefwFile, " ");
  return;
}

/*
 * ************************** Questions: - Is only one row rule allowed - Is
 * only one tracks rule allowed - In the die area is a zero area allowed?
 * overlaps? - What type of checking is needed for the rows and tracks do
 * loop? - Can you have a default prop with a number AND a range? - What is
 * the pin properties section mentioned in the 5.1 spec? ***************************
 */
