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

%{
#include <string.h>
#include <stdlib.h>
#include <math.h>
/* # include <sys/types.h> */
/* # include <sys/stat.h> */
/* # include <fcntl.h> */
/* # include <unistd.h> */
/* # ifndef	WIN32 */
/* # include <sys/param.h> */
/* # endif */
#include "FEF_stringhash.h"
#include "lex.h"
#include "lefiDefs.hpp"
#include "lefiUser.hpp"
#include "lefiUtil.hpp"
#include "lefrCallBacks.hpp"
#ifdef WIN32
#include <malloc.h>
#endif

#define LYPROP_ECAP "EDGE_CAPACITANCE"

int lefRetVal;
static char lefPropDefType;    /* save the current type of the property */

/* Macro to describe how we handle a callback.
 * If the function was set then call it.
 * If the function returns non zero then there was an error
 * so call the error routine and exit.
 */
#define CALLBACK(func, typ, data) \
    if (!lef_errors) { \
      if (func) { \
        if ((lefRetVal = (*func)(typ, data, lefrUserData)) == 0) { \
        } else if (lefRetVal == STOP_PARSE) { \
	  return lefRetVal; \
        } else { \
	  yyerror("Error in callback"); \
	  return lefRetVal; \
        } \
      } \
    }

#define CHKERR() \
    if (lef_errors > 20) { \
      yyerror("Too many syntax errors"); \
      lef_errors = 0; \
      return 1; \
    }

extern int lefNlines, lefNtokens;
extern FILE* lefFile;

/**********************************************************************/
/**********************************************************************/

#define C_EQ 0
#define C_NE 1
#define C_LT 2
#define C_LE 3
#define C_GT 4
#define C_GE 5

extern int lefDumbMode;   // for communicating with parser
extern int lefNoNum;      // likewise, says no numbers for the next N tokens
extern int lefRealNum;    // Next token will be a real number
extern int lefNlToken;    // likewise
extern int lefDefIf;      // likewise

extern int lef_errors;    // from lex.cpph

static int ignoreVersion = 0; // ignore checking version number

int lefNamesCaseSensitive = FALSE;  // are names case sensitive?

/* XXXXXXXXXXXXX check out these variables */
double lef_save_x, lef_save_y;  // for interpreting (*) notation of LEF/DEF

/* #define STRING_LIST_SIZE 1024 */
/* char string_list[STRING_LIST_SIZE]; */

static char temp_name[256];
static char layerName[128];
static char viaName[128];
static char viaRuleName[128];
static char nonDefaultRuleName[128];
static char siteName[128];
static char arrayName[128];
static char macroName[128];
static char pinName[128];

static int siteDef, symDef, sizeDef, pinDef, obsDef, origDef;
static int useLenThr;
static int layerCut, layerMastOver, layerRout, layerDir;
static lefiAntennaEnum antennaType = lefiAntennaAR;  /* 5.4 - antenna type */
static int viaRuleLayer;       /* keep track number of layer in a viarule */
static int viaLayer;           /* keep track number of layer in a via */
static int ndLayer;            /* keep track number of layer in a nondefault */
static int numVia;             /* keep track number of via */
static int viaRuleHasDir;      /* viarule layer has direction construct */
static int viaRuleHasEnc;      /* viarule layer has enclosure construct */
static int ndLayerWidth;       /* keep track if width is set at ndLayer */
static int ndLayerSpace;       /* keep track if spacing is set at ndLayer */
static int isGenerate;         /* keep track if viarule has generate */
static int hasPitch;           /* keep track of pitch in layer */
static int hasWidth;           /* keep track of width in layer */
static int hasDirection;       /* keep track of direction in layer */
static int hasParallel;        /* keep track of parallelrunlength */
static int hasInfluence;       /* keep track of influence */

int spParallelLength;          /* the number of layer parallelrunlength */

lefiNum macroNum;              /* for origin & size callback */

int comp_str(char *s1, int op, char *s2)
{
    int k = strcmp(s1, s2);
    switch (op) {
	case C_EQ: return k == 0;
	case C_NE: return k != 0;
	case C_GT: return k >  0;
	case C_GE: return k >= 0;
	case C_LT: return k <  0;
	case C_LE: return k <= 0;
	}
    return 0;
}
int comp_num(double s1, int op, double s2)
{
    double k = s1 - s2;
    switch (op) {
	case C_EQ: return k == 0;
	case C_NE: return k != 0;
	case C_GT: return k >  0;
	case C_GE: return k >= 0;
	case C_LT: return k <  0;
	case C_LE: return k <= 0;
	}
    return 0;
}
double convert_name2num(char *versionName)
{
    char majorNm[80];
    char minorNm[80];
    char *subMinorNm = NULL;
    char *p1;
    char *versionNm = strdup(versionName);
  
    double major = 0, minor = 0, subMinor = 0;
    double version;

    sscanf(versionNm, "%[^.].%s", majorNm, minorNm);
    if (p1 = strchr(minorNm, '.')) {
       subMinorNm = p1+1; 
       *p1 = '\0';
    }
    major = atof(majorNm);
    minor = atof(minorNm);
    if (subMinorNm)
       subMinor = atof(subMinorNm);

    version = major;
    
    if (minor > 0)
       version = major + minor/10;

    if (subMinor > 0)
       version = version + subMinor/1000;

    lefFree(versionNm);
    return version;
}
%}

%union {
	double dval ;
	int       integer ;
	char *    string ;
	lefPOINT  pt;
}

%token <string> K_HISTORY
%token K_ABUT K_ABUTMENT K_ACTIVE K_ANALOG K_ARRAY K_AREA
%token K_BLOCK K_BOTTOMLEFT K_BOTTOMRIGHT
%token K_BY K_CAPACITANCE K_CAPMULTIPLIER K_CLASS K_CLOCK K_CLOCKTYPE
%token K_COLUMNMAJOR K_DESIGNRULEWIDTH K_INFLUENCE
%token K_COMPONENTS K_CORE K_CORNER K_COVER K_CPERSQDIST K_CURRENT 
%token K_CURRENTSOURCE K_CUT K_DEFAULT K_DATABASE K_DATA
%token K_DIELECTRIC K_DIRECTION K_DO K_EDGECAPACITANCE
%token K_EEQ K_END K_ENDCAP K_FALL K_FALLCS K_FALLT0 K_FALLSATT1
%token K_FALLRS K_FALLSATCUR K_FALLTHRESH K_FEEDTHRU K_FIXED K_FOREIGN K_FROMPIN
%token K_GENERATE K_GENERATOR K_GROUND K_HEIGHT K_HORIZONTAL K_INOUT K_INPUT
%token K_INPUTNOISEMARGIN K_COMPONENTPIN
%token K_INTRINSIC K_INVERT K_IRDROP K_ITERATE K_IV_TABLES K_LAYER K_LEAKAGE
%token K_LEQ K_LIBRARY K_MACRO K_MATCH K_MAXDELAY K_MAXLOAD K_METALOVERHANG K_MILLIAMPS
%token K_MILLIWATTS K_MINFEATURE K_MAXLOAD K_MUSTJOIN K_NAMESCASESENSITIVE K_NANOSECONDS
%token K_NETS K_NEW K_NONDEFAULTRULE
%token K_NONINVERT K_NONUNATE K_OBS K_OHMS K_OFFSET K_ORIENTATION K_ORIGIN K_OUTPUT
%token K_OUTPUTNOISEMARGIN K_OUTPUTRESISTANCE
%token K_OVERHANG K_OVERLAP K_OFF K_ON K_OVERLAPS K_PAD K_PATH K_PATTERN K_PICOFARADS
%token K_PIN K_PITCH
%token K_PLACED K_POLYGON K_PORT K_POST K_POWER K_PRE K_PULLDOWNRES K_RECT
%token K_RESISTANCE K_RESISTIVE K_RING K_RISE K_RISECS K_RISERS K_RISESATCUR K_RISETHRESH
%token K_RISESATT1 K_RISET0 K_RISEVOLTAGETHRESHOLD K_FALLVOLTAGETHRESHOLD
%token K_ROUTING K_ROWMAJOR K_RPERSQ K_SAMENET K_SCANUSE K_SHAPE K_SHRINKAGE
%token K_SIGNAL K_SITE K_SIZE K_SOURCE K_SPACER K_SPACING K_SPECIALNETS K_STACK
%token K_START K_STEP K_STOP K_STRUCTURE K_SYMMETRY K_TABLE K_THICKNESS K_TIEHIGH
%token K_TIELOW K_TIEOFFR K_TIME K_TIMING K_TO K_TOPIN K_TOPLEFT K_TOPRIGHT
%token K_TOPOFSTACKONLY
%token K_TRISTATE K_TYPE K_UNATENESS K_UNITS K_USE K_VARIABLE K_VERTICAL K_VHI
%token K_VIA K_VIARULE K_VLO K_VOLTAGE K_VOLTS K_WIDTH K_X K_Y K_R90
%token <string> T_STRING QSTRING
%token <dval>   NUMBER
%token K_N K_S K_E K_W K_FN K_FS K_FE K_FW
%token K_R0 K_R90 K_R180 K_R270 K_MX K_MY K_MXR90 K_MYR90
%token K_USER K_MASTERSLICE
%token K_ENDMACRO K_ENDMACROPIN K_ENDVIARULE K_ENDVIA K_ENDLAYER K_ENDSITE
%token K_CANPLACE K_CANNOTOCCUPY K_TRACKS K_FLOORPLAN K_GCELLGRID K_DEFAULTCAP
%token K_MINPINS K_WIRECAP
%token K_STABLE K_SETUP K_HOLD
%token K_DEFINE K_DEFINES K_DEFINEB K_IF K_THEN K_ELSE K_FALSE K_TRUE 
%token K_EQ K_NE K_LE K_LT K_GE K_GT K_OR K_AND K_NOT
%token K_DELAY K_TABLEDIMENSION K_TABLEAXIS K_TABLEENTRIES K_TRANSITIONTIME
%token K_EXTENSION
%token K_PROPDEF K_STRING K_INTEGER K_REAL K_RANGE K_PROPERTY
%token K_VIRTUAL K_BUSBITCHARS K_VERSION
%token K_BEGINEXT K_ENDEXT
%token K_UNIVERSALNOISEMARGIN K_EDGERATETHRESHOLD1 K_CORRECTIONTABLE
%token K_EDGERATESCALEFACTOR K_EDGERATETHRESHOLD2 K_VICTIMNOISE
%token K_NOISETABLE K_EDGERATE K_OUTPUTRESISTANCE K_VICTIMLENGTH
%token K_CORRECTIONFACTOR K_OUTPUTPINANTENNASIZE
%token K_INPUTPINANTENNASIZE K_INOUTPINANTENNASIZE
%token K_CURRENTDEN K_PWL K_ANTENNALENGTHFACTOR K_TAPERRULE
%token K_DIVIDERCHAR K_ANTENNASIZE K_ANTENNAMETALLENGTH K_ANTENNAMETALAREA
%token K_RISESLEWLIMIT K_FALLSLEWLIMIT K_FUNCTION K_BUFFER K_INVERTER
%token K_NAMEMAPSTRING K_NOWIREEXTENSIONATPIN K_WIREEXTENSION
%token K_MESSAGE K_CREATEFILE K_OPENFILE K_CLOSEFILE K_WARNING
%token K_ERROR K_FATALERROR
%token K_RECOVERY K_SKEW K_ANYEDGE K_POSEDGE K_NEGEDGE
%token K_SDFCONDSTART K_SDFCONDEND K_SDFCOND
%token K_MPWH K_MPWL K_PERIOD
%token K_ACCURRENTDENSITY K_DCCURRENTDENSITY K_AVERAGE K_PEAK K_RMS K_FREQUENCY
%token K_CUTAREA K_MEGAHERTZ K_USELENGTHTHRESHOLD K_LENGTHTHRESHOLD
%token K_ANTENNAINPUTGATEAREA K_ANTENNAINOUTDIFFAREA K_ANTENNAOUTPUTDIFFAREA
%token K_ANTENNAAREARATIO K_ANTENNADIFFAREARATIO K_ANTENNACUMAREARATIO
%token K_ANTENNACUMDIFFAREARATIO K_ANTENNAAREAFACTOR K_ANTENNASIDEAREARATIO
%token K_ANTENNADIFFSIDEAREARATIO K_ANTENNACUMSIDEAREARATIO
%token K_ANTENNACUMDIFFSIDEAREARATIO K_ANTENNASIDEAREAFACTOR
%token K_ANTENNAOUTPUTDIFFAREA K_DIFFUSEONLY K_MANUFACTURINGGRID
%token K_ANTENNACELL K_CLEARANCEMEASURE K_EUCLIDEAN K_MAXXY
%token K_USEMINSPACING K_ROWMINSPACING K_ROWABUTSPACING K_FLIP K_NONE
%token K_ANTENNAPARTIALMETALAREA K_ANTENNAPARTIALMETALSIDEAREA
%token K_ANTENNAGATEAREA K_ANTENNADIFFAREA K_ANTENNAMAXAREACAR
%token K_ANTENNAMAXSIDEAREACAR K_ANTENNAPARTIALCUTAREA K_ANTENNAMAXCUTCAR
%token K_SLOTWIREWIDTH K_SLOTWIRELENGTH K_SLOTWIDTH K_SLOTLENGTH
%token K_MAXADJACENTSLOTSPACING K_MAXCOAXIALSLOTSPACING K_MAXEDGESLOTSPACING
%token K_SPLITWIREWIDTH K_MINIMUMDENSITY K_MAXIMUMDENSITY K_DENSITYCHECKWINDOW
%token K_DENSITYCHECKSTEP K_FILLACTIVESPACING K_MINIMUMCUT K_ADJACENTCUTS
%token K_ANTENNAMODEL K_BUMP K_ENCLOSURE K_FROMABOVE K_FROMBELOW
%token K_IMPLANT K_LENGTH K_MAXVIASTACK K_AREAIO K_BLACKBOX
%token K_MAXWIDTH K_MINENCLOSEDAREA K_MINSTEP K_ORIENT K_OXIDE1 K_OXIDE2
%token K_OXIDE3 K_OXIDE4 K_PARALLELRUNLENGTH K_MINWIDTH
%token K_PROTRUSIONWIDTH K_SPACINGTABLE K_WITHIN


%type <string> start_macro end_macro
%type <string> start_layer
%type <string> macro_pin_use
%type <string> macro_scan_use
%type <string> pin_shape
%type <string> pad_type core_type endcap_type class_type site_class
%type <string> start_foreign spacing_type clearance_type
%type <pt> pt 
%type <pt> macro_origin
%type <string> layer_option layer_options layer_type layer_direction
%type <string> electrical_direction
%type <integer> orientation
%type <dval> expression
%type <integer> b_expr
%type <string>  s_expr
%type <integer> relop spacing_value
%type <string> opt_layer_name risefall unateness delay_or_transition
%type <string> two_pin_trigger from_pin_trigger to_pin_trigger
%type <string> one_pin_trigger req_layer_name
%type <string> layer_table_type layer_connect_type

%nonassoc IF
%left K_AND
%left K_OR
%left K_LE K_EQ K_LT K_NE K_GE K_GT
%nonassoc LNOT
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

%%

lef_file: rules end_library
	;

version: K_VERSION { lefDumbMode = 1; lefNoNum = 1;} T_STRING ';'
      { 
         versionNum = convert_name2num($3);
/*
         versionNum = $3;         Save the version number for future use */
         if (lefrVersionStrCbk) {
            CALLBACK(lefrVersionStrCbk, lefrVersionStrCbkType, $3);
         } else {
            CALLBACK(lefrVersionCbk, lefrVersionCbkType, versionNum);
         }
         if (versionNum > 5.3 && versionNum < 5.4) {
            ignoreVersion = 1;
         }
         use5_3 = use5_4 = 0;
         lef_errors = 0;
         hasVer = 1;
      }


dividerchar: K_DIVIDERCHAR QSTRING ';'
      {
        CALLBACK(lefrDividerCharCbk, lefrDividerCharCbkType, $2);
        hasDivChar = 1;
      }

busbitchars: K_BUSBITCHARS QSTRING ';'
      {
        CALLBACK(lefrBusBitCharsCbk, lefrBusBitCharsCbkType, $2); 
        hasBusBit = 1;
      }

rules: /* empty */
	| rules rule
	| error 
	    { }

end_library: K_END K_LIBRARY
      {
        CALLBACK(lefrLibraryEndCbk, lefrLibraryEndCbkType, 0);
        // 11/16/2001 - Wanda da Rosa - pcr 408334
        // Return 1 if there are errors
        if (lef_errors)
           return 1;
        if (!hasVer)
           yywarning("VERSION is a required statement.");
        if (!hasNameCase)
           yywarning("NAMESCASESENSITIVE is a required statement.");
        if (!hasBusBit)
           yywarning("BUSBITCHARS is a required statement.");
        if (!hasDivChar)
           yywarning("DIVIDERCHAR is a required statement.");
        hasVer = 0;
        hasNameCase = 0;
        hasBusBit = 0;
        hasDivChar = 0;
      }

rule:  version | busbitchars | case_sensitivity | units_section
    | layer_rule | via | viarule | viarule_generate | dividerchar
    | wireextension | msg_statement
    | spacing_rule | dielectric | minfeature | irdrop | site | macro | array
    | def_statement | nondefault_rule | prop_def_section
    | universalnoisemargin | edgeratethreshold1
    | edgeratescalefactor | edgeratethreshold2
    | noisetable | correctiontable | input_antenna
    | output_antenna | inout_antenna | extension
    | antenna_input | antenna_inout | antenna_output | manufacturing
    | useminspacing | clearancemeasure | maxstack_via
    | create_file_statement
    	;

case_sensitivity: K_NAMESCASESENSITIVE K_ON ';'
	  {
	    lefNamesCaseSensitive = TRUE;
	    CALLBACK(lefrCaseSensitiveCbk, lefrCaseSensitiveCbkType,
			     lefNamesCaseSensitive);
            hasNameCase = 1;
	  }
      | K_NAMESCASESENSITIVE K_OFF ';'
	  {
	    lefNamesCaseSensitive = FALSE;
	    CALLBACK(lefrCaseSensitiveCbk, lefrCaseSensitiveCbkType,
			     lefNamesCaseSensitive);
            hasNameCase = 1;
	  }

wireextension: K_NOWIREEXTENSIONATPIN K_ON ';'
    { CALLBACK(lefrNoWireExtensionCbk, lefrNoWireExtensionCbkType, "ON"); }
  | K_NOWIREEXTENSIONATPIN K_OFF ';'
    { CALLBACK(lefrNoWireExtensionCbk, lefrNoWireExtensionCbkType, "OFF"); }

manufacturing: K_MANUFACTURINGGRID NUMBER ';'
    { CALLBACK(lefrManufacturingCbk, lefrManufacturingCbkType, $2); }

useminspacing: K_USEMINSPACING spacing_type spacing_value ';'
  {
    if (lefrUseMinSpacingCbk)
      lefrUseMinSpacing.lefiUseMinSpacing::set($2, $3);
    CALLBACK(lefrUseMinSpacingCbk, lefrUseMinSpacingCbkType,
             &lefrUseMinSpacing);
  }

clearancemeasure: K_CLEARANCEMEASURE clearance_type ';'
    { CALLBACK(lefrClearanceMeasureCbk, lefrClearanceMeasureCbkType, $2); }

clearance_type:
  K_MAXXY   {$$ = (char*)"MAXXY";}
  | K_EUCLIDEAN   {$$ = (char*)"EUCLIDEAN";}

spacing_type:
  K_OBS     {$$ = (char*)"OBS";}
  | K_PIN   {$$ = (char*)"PIN";}

spacing_value:
  K_ON      {$$ = 1;}
  | K_OFF   {$$ = 0;}

units_section: start_units units_rules K_END K_UNITS
    { CALLBACK(lefrUnitsCbk, lefrUnitsCbkType, &lefrUnits); }

start_units: K_UNITS
    { lefrUnits.lefiUnits::clear(); }

units_rules: /* empty */
  | units_rules units_rule
  ;

units_rule: K_TIME K_NANOSECONDS NUMBER ';'
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setTime($3); }
  | K_CAPACITANCE K_PICOFARADS NUMBER ';'
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setCapacitance($3); }
  | K_RESISTANCE K_OHMS NUMBER ';'
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setResistance($3); }
  | K_POWER K_MILLIWATTS NUMBER ';'
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setPower($3); }
  | K_CURRENT K_MILLIAMPS NUMBER ';'
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setCurrent($3); }
  | K_VOLTAGE K_VOLTS NUMBER ';'
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setVoltage($3); }
  | K_DATABASE { lefDumbMode = 1; } T_STRING NUMBER ';'
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setDatabase($3, $4); }
  | K_FREQUENCY K_MEGAHERTZ NUMBER ';'
    { if (lefrUnitsCbk) lefrUnits.lefiUnits::setFrequency($3); }

layer_rule: start_layer layer_options end_layer
    { CALLBACK(lefrLayerCbk, lefrLayerCbkType, &lefrLayer); }

start_layer: K_LAYER {lefDumbMode = 1; lefNoNum = 1; } T_STRING
    { 
      if (lefrHasMaxVS) {   /* 5.5 */
        yyerror("A MAXVIASTACK has defined before the LAYER");
        CHKERR();
      }
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setName($3);
      useLenThr = 0;
      layerCut = 0;
      layerMastOver = 0;
      layerRout = 0;
      layerDir = 0;
      lefrHasLayer = 1;
      strcpy(layerName, $3);
      hasPitch = 0;
      hasWidth = 0;
      hasDirection = 0;
      hasParallel = 0;
      hasInfluence = 0;
    }

end_layer: K_END {lefDumbMode = 1; lefNoNum = 1; } T_STRING
    { 
      if (strcmp(layerName, $3) != 0) {
        yyerror("END LAYER name is different from LAYER");
        CHKERR(); 
      }
      if ((layerRout == 1) && (hasPitch == 0)) {
        yyerror("PITCH in LAYER with type ROUTING is required");
        CHKERR(); 
      }
      if ((layerRout == 1) && (hasWidth == 0)) {
        yyerror("WIDTH in LAYER with type ROUTING is required");
        CHKERR(); 
      }
      if ((layerRout == 1) && (hasDirection == 0)) {
        yyerror("DIRECTION in LAYER with type ROUTING is required");
        CHKERR(); 
      }
    }

layer_options:
  /* empty */
    { }
  | layer_options layer_option
    { }

layer_option:
  K_TYPE layer_type ';'
    { if (lefrLayerCbk) lefrLayer.lefiLayer::setType($2); }
  | K_PITCH NUMBER ';'
    { 
      if (lefrLayerCbk) lefrLayer.lefiLayer::setPitch($2);
      hasPitch = 1;  
    }
  | K_OFFSET NUMBER ';'
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::setOffset($2);
    }
  | K_WIDTH NUMBER ';'
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::setWidth($2);
      hasWidth = 1;  
    }
  | K_AREA NUMBER ';'
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::setArea($2);
    }
  | K_SPACING NUMBER
    {
      // 11/22/99 - Wanda da Rosa, PCR 283762
      //            Issue an error is this is defined in masterslice
      if (layerMastOver) {
         yyerror("SPACING can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
        CHKERR();
      }
      // 5.5 either SPACING or SPACINGTABLE, not both for routing layer only
      if (layerRout) {
        if (lefrHasSpacingTbl) {
           yywarning("It is illegal to mix both SPACING rules & SPACINGTABLE rules for the same lef file in ROUTING layer");
        }
        if (lefrLayerCbk)
           lefrLayer.lefiLayer::setSpacingMin($2);
        lefrHasSpacing = 1;
      } else { 
        if (lefrLayerCbk)
           lefrLayer.lefiLayer::setSpacingMin($2);
      }
    }
    opt_range_detail opt_spacing_cut ';'
  | K_DIRECTION layer_direction ';'
    {
      layerDir = 1;
      if (!layerRout) {
        yyerror("DIRECTION can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDirection($2);
      hasDirection = 1;  
    }
  | K_RESISTANCE K_RPERSQ NUMBER ';'
    {
      if (!layerRout) {
        yyerror("RESISTANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setResistance($3);
    }
  | K_RESISTANCE K_RPERSQ K_PWL '(' res_points ')' ';'
    {
      if (!layerRout) {
        yyerror("RESISTANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
    }
  | K_CAPACITANCE K_CPERSQDIST NUMBER ';'
    {
      if (!layerRout) {
        yyerror("CAPACITANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setCapacitance($3);
    }
  | K_CAPACITANCE K_CPERSQDIST K_PWL '(' cap_points ')' ';'
    {
      if (!layerRout) {
        yyerror("CAPACITANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
    }
  | K_HEIGHT NUMBER ';'
    {
      if (!layerRout) {
        yyerror("HEIGHT can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setHeight($2);
    }
  | K_WIREEXTENSION NUMBER ';'
    {
      if (!layerRout) {
        yyerror("WIREEXTENSION can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setWireExtension($2);
    }
  | K_THICKNESS NUMBER ';'
    {
      if (!layerRout && (layerCut || layerMastOver)) {
        yyerror("THICKNESS can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setThickness($2);
    }
  | K_SHRINKAGE NUMBER ';'
    {
      if (!layerRout) {
        yyerror("SHRINKAGE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setShrinkage($2);
    }
  | K_CAPMULTIPLIER NUMBER ';'
    {
      if (!layerRout) {
        yyerror("CAPMULTIPLIER can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setCapMultiplier($2);
    }
  | K_EDGECAPACITANCE NUMBER ';'
    {
      if (!layerRout) {
        yyerror("EDGECAPACITANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setEdgeCap($2);
    }
/* 3/24/2000 - Wanda da Rosa.  Same call for 5.4 but more options.
              Syntax is handle 1 page down.
  | K_ANTENNAAREAFACTOR NUMBER ';'
    {
      if (!layerRout && (layerCut || layerMastOver)) {
        yyerror("ANTENNAAREAFACTOR can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaArea($2);
    }
*/
  | K_ANTENNALENGTHFACTOR NUMBER ';'
    { /* 5.3 syntax */
      use5_3 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
        yyerror("ANTENNALENGTHFACTOR can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      } else if (versionNum >= 5.4) {
        if (use5_4) {
           yyerror("ANTENNALENGTHFACTOR is a 5.3 or earlier syntax. Your lef file has both old and new syntax, which is illegal.");
           CHKERR();
        }
      }

      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaLength($2);
    }
  | K_CURRENTDEN NUMBER ';'
    {
      if (!layerRout) {
        yyerror("CURRENTDEN can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setCurrentDensity($2);
    }
  | K_CURRENTDEN K_PWL '(' current_density_pwl_list ')' ';'
    { 
      if (!layerRout) {
        yyerror("CURRENTDEN can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
    }
  | K_CURRENTDEN '(' NUMBER NUMBER ')' ';'
    {
      if (!layerRout) {
        yyerror("CURRENTDEN can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setCurrentPoint($3, $4);
    }
  | K_PROPERTY { lefDumbMode = 10000000; } layer_prop_list ';'
    {
      lefDumbMode = 0;
    }
  | K_ACCURRENTDENSITY layer_table_type
    {
      if (layerMastOver) {
        yyerror("ACCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addAccurrentDensity($2);
    }
    ac_layer_table_opt_list ';'
  | K_ACCURRENTDENSITY layer_table_type NUMBER ';'
    {
      if (layerMastOver) {
        yyerror("ACCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) {
           lefrLayer.lefiLayer::addAccurrentDensity($2);
           lefrLayer.lefiLayer::setAcOneEntry($3);
      }
    }
  | K_DCCURRENTDENSITY K_AVERAGE
    {
      if (layerMastOver) {
        yyerror("DCCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addDccurrentDensity("AVERAGE");
    }
    dc_layer_table_opt_list ';'
  | K_DCCURRENTDENSITY K_AVERAGE NUMBER ';'
    {
      if (layerMastOver) {
        yyerror("DCCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addDccurrentDensity("AVERAGE");
         lefrLayer.lefiLayer::setDcOneEntry($3);
      }
    }
/* 3/23/2000 - 5.4 syntax.  Wanda da Rosa */
  | K_ANTENNAAREARATIO NUMBER ';'
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("ANTENNAAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      } else if (use5_3) {
         yyerror("ANTENNAAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
         CHKERR();
      }
      if (!layerRout && !layerCut && layerMastOver) {
         yyerror("ANTENNAAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaAreaRatio($2);
    }
  | K_ANTENNADIFFAREARATIO
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("ANTENNADIFFAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      } else if (use5_3) {
         yyerror("ANTENNADIFFAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
         CHKERR();
      }
      if (!layerRout && !layerCut && layerMastOver) {
        yyerror("ANTENNADIFFAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
        CHKERR();
      }
      antennaType = lefiAntennaDAR; 
    }
    layer_antenna_pwl ';'
  | K_ANTENNACUMAREARATIO NUMBER ';'
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("ANTENNACUMAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      } else if (use5_3) {
         yyerror("ANTENNACUMAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
         CHKERR();
      }
      if (!layerRout && !layerCut && layerMastOver) {
        yyerror("ANTENNACUMAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
        CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaCumAreaRatio($2);
    }
  | K_ANTENNACUMDIFFAREARATIO
    {  /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("ANTENNACUMDIFFAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      } else if (use5_3) {
         yyerror("ANTENNACUMDIFFAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
         CHKERR();
      }
      if (!layerRout && !layerCut && layerMastOver) {
         yyerror("ANTENNACUMDIFFAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
         CHKERR();
      }
      antennaType = lefiAntennaCDAR;
    }
    layer_antenna_pwl ';'
  | K_ANTENNAAREAFACTOR NUMBER
    { /* both 5.3  & 5.4 syntax */
      if (!layerRout && !layerCut && layerMastOver) {
         yyerror("ANTENNAAREAFACTOR can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
         CHKERR();
      }
      /* this does not need to check, since syntax is in both 5.3 & 5.4 */
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaAreaFactor($2);
      antennaType = lefiAntennaAF;
    }
    layer_antenna_duo ';'
  | K_ANTENNASIDEAREARATIO NUMBER ';'
    { /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         yyerror("ANTENNASIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
         CHKERR();
      }
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("ANTENNASIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      } else if (use5_3) {
         yyerror("ANTENNASIDEAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaSideAreaRatio($2);
    }
  | K_ANTENNADIFFSIDEAREARATIO
    {  /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         yyerror("ANTENNADIFFSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
         CHKERR();
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("ANTENNADIFFSIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      } else if (use5_3) {
         yyerror("ANTENNADIFFSIDEAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
         CHKERR();
      }
      antennaType = lefiAntennaDSAR;
    }
    layer_antenna_pwl ';'
  | K_ANTENNACUMSIDEAREARATIO NUMBER ';'
    { /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         yyerror("ANTENNACUMSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
         CHKERR();
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("ANTENNACUMSIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      } else if (use5_3) {
         yyerror("ANTENNACUMSIDEAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaCumSideAreaRatio($2);
    }
  | K_ANTENNACUMDIFFSIDEAREARATIO
    {  /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         yyerror("ANTENNACUMDIFFSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
         CHKERR();
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("ANTENNACUMDIFFSIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      } else if (use5_3) {
         yyerror("ANTENNACUMDIFFSIDEAREARATIO is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
         CHKERR();
      }
      antennaType = lefiAntennaCDSAR;
    }
    layer_antenna_pwl ';'
  | K_ANTENNASIDEAREAFACTOR NUMBER
    { /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && (layerCut || layerMastOver)) {
         yyerror("ANTENNASIDEAREAFACTOR can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
         CHKERR();
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("ANTENNASIDEAREAFACTOR is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      } else if (use5_3) {
         yyerror("ANTENNASIDEAREAFACTOR is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal."); 
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setAntennaSideAreaFactor($2);
      antennaType = lefiAntennaSAF;
    }
    layer_antenna_duo ';'
  | K_ANTENNAMODEL /* 5.5 */
    { /* 5.4 syntax */
      use5_4 = 1;
      if (!layerRout && !layerCut && layerMastOver) {
         yyerror("ANTENNAMODEL can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
         CHKERR();
      }
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.5) {
         yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file is not defined as 5.5");
         CHKERR();
      } else if (use5_3) {
         yyerror("ANTENNAMODEL is a 5.5 syntax.  Your lef file has both old and new syntax, which is illegal."); 
         CHKERR();
      }
      antennaType = lefiAntennaO;
    }
    layer_oxide ';'
  | K_SLOTWIREWIDTH NUMBER ';'
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("SLOTWIREWIDTH is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotWireWidth($2);
    }
  | K_SLOTWIRELENGTH NUMBER ';'
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("SLOTWIRELENGTH is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotWireLength($2);
    }
  | K_SLOTWIDTH NUMBER ';'
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("SLOTWIDTH is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotWidth($2);
    }
  | K_SLOTLENGTH NUMBER ';'
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("SLOTLENGTH is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setSlotLength($2);
    }
  | K_MAXADJACENTSLOTSPACING NUMBER ';'
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("MAXADJACENTSLOTSPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxAdjacentSlotSpacing($2);
    }
  | K_MAXCOAXIALSLOTSPACING NUMBER ';'
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("MAXCOAXIALSLOTSPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxCoaxialSlotSpacing($2);
    }
  | K_MAXEDGESLOTSPACING NUMBER ';'
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("MAXEDGESLOTSPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxEdgeSlotSpacing($2);
    }
  | K_SPLITWIREWIDTH NUMBER ';'
    { /* 5.4 syntax */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("SPLITWIREWIDTH is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setSplitWireWidth($2);
    }
  | K_MINIMUMDENSITY NUMBER ';'
    { /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("MINIMUMDENSITY is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMinimumDensity($2);
    }
  | K_MAXIMUMDENSITY NUMBER ';'
    { /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("MAXIMUMDENSITY is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMaximumDensity($2);
    }
  | K_DENSITYCHECKWINDOW NUMBER NUMBER ';'
    { /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("DENSITYCHECKWINDOW is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDensityCheckWindow($2, $3);
    }
  | K_DENSITYCHECKSTEP NUMBER ';'
    { /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("DENSITYCHECKSTEP is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setDensityCheckStep($2);
    }
  | K_FILLACTIVESPACING NUMBER ';'
    { /* 5.4 syntax, pcr 394389 */
      if (ignoreVersion) {
         /* do nothing */
      } else if (versionNum < 5.4) {
         yyerror("FILLACTIVESPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setFillActiveSpacing($2);
    }
  | K_MAXWIDTH NUMBER ';'              /* 5.5 */
    {
      // 5.5 MAXWIDTH, is for routing layer only
      if (!layerRout) {
        yyerror("MAXWIDTH can only be defined in LAYER with TYPE ROUTING.  Parser stops executions");
        CHKERR();
      }
      if (versionNum < 5.5) {
         yyerror("MAXWIDTH is a 5.5 syntax. Your lef file is not defined as 5.5");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMaxwidth($2);
    }
  | K_MINWIDTH NUMBER ';'              /* 5.5 */
    {
      // 5.5 MINWIDTH, is for routing layer only
      if (!layerRout) {
        yyerror("MINWIDTH can only be defined in LAYER with TYPE ROUTING.  Parser stops executions");
        CHKERR();
      }
      if (versionNum < 5.5) {
         yyerror("MINWIDTH is a 5.5 syntax. Your lef file is not defined as 5.5");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMinwidth($2);
    }
  | K_MINENCLOSEDAREA NUMBER           /* 5.5 */
    {
      if (versionNum < 5.5) {
         yyerror("MINENCLOSEDAREA is a 5.5 syntax. Your lef file is not defined as 5.5");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addMinenclosedarea($2);
    }
    layer_minen_width ';'
  | K_MINIMUMCUT NUMBER K_WIDTH NUMBER /* 5.5 */
    { /* pcr 409334 */
      if (lefrLayerCbk) lefrLayer.lefiLayer::addMinimumcut($2, $4); 
    }
    layer_connect_type
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::addMinimumcutConnect($6);
    }
    layer_cuts_required ';'
  | K_MINSTEP NUMBER ';'               /* 5.5 */
    {
      if (lefrLayerCbk) lefrLayer.lefiLayer::setMinstep($2);
    }
  | K_PROTRUSIONWIDTH NUMBER K_LENGTH NUMBER K_WIDTH NUMBER ';'  /* 5.5 */
    {
      if (versionNum < 5.5) {
         yyerror("PROTRUSION RULE is a 5.5 syntax. Your lef file is not defined as 5.5");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::setProtrusion($2, $4, $6);
    }
  | K_SPACINGTABLE                    /* 5.5 */
    {
      if (versionNum < 5.5) {
         yyerror("SPACINGTABLE is a 5.5 syntax. Your lef file is not defined as 5.5");
         CHKERR();
      }
      // 5.5 either SPACING or SPACINGTABLE in a layer, not both
      if (lefrHasSpacing && layerRout) {
         yywarning("It is illegal to mix both SPACING rules & SPACINGTABLE rules for the same lef file in ROUTING layer");
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addSpacingTable();
      lefrHasSpacingTbl = 1;
    }
    sp_options ';'

sp_options:
  K_PARALLELRUNLENGTH NUMBER
    { 
      if (hasInfluence) {  // 5.5 - INFLUENCE table must follow a PARALLEL
          yyerror("The INFLUENCE table must follow a PARALLELRUNLENGTH table on the same layer");
          CHKERR();
      }
      if (hasParallel) { // 5.5 - Only one PARALLEL table is allowed per layer
          yyerror("Only one PARALLELRUNLENGTH table is allowed per layer");
          CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber($2);
      hasParallel = 1;
    }
    number_list
    {
      spParallelLength = lefrLayer.lefiLayer::getNumber();
      if (lefrLayerCbk) lefrLayer.lefiLayer::addSpParallelLength();
    }
    K_WIDTH NUMBER
    { 
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addSpParallelWidth($7);
      }
    }
    number_list
    { 
      if (lefrLayer.lefiLayer::getNumber() != spParallelLength) {
         yyerror("The number of length in PARALLELRUNLENGTH is not the same as the number of spacing in WIDTH");
         CHKERR();
      }
      if (lefrLayerCbk) lefrLayer.lefiLayer::addSpParallelWidthSpacing();
    }
    layer_sp_parallel_widths
  | K_INFLUENCE K_WIDTH NUMBER K_WITHIN NUMBER K_SPACING NUMBER
    {
      if (hasInfluence) {  // 5.5 - INFLUENCE table must follow a PARALLEL
          yyerror("Only one INFLUENCE table is allowed per layer");
          CHKERR();
      }
      if (!hasParallel) {  // 5.5 - INFLUENCE must follow a PARALLEL
          yyerror("The INFLUENCE table must follow a PARALLELRUNLENGTH table on the same layer");
          CHKERR();
      }
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::setInfluence();
         lefrLayer.lefiLayer::addSpInfluence($3, $5, $7);
      }
    }
    layer_sp_influence_widths

layer_antenna_pwl:
  NUMBER
      { if (lefrLayerCbk)
          lefrLayer.lefiLayer::setAntennaValue(antennaType, $1); }
  | K_PWL '(' pt pt
      { if (lefrLayerCbk) { /* require min 2 points, set the 1st 2 */
          lefrAntennaPWLPtr = (lefiAntennaPWL*)lefMalloc(sizeof(lefiAntennaPWL));
          lefrAntennaPWLPtr->lefiAntennaPWL::Init();
          lefrAntennaPWLPtr->lefiAntennaPWL::addAntennaPWL($3.x, $3.y);
          lefrAntennaPWLPtr->lefiAntennaPWL::addAntennaPWL($4.x, $4.y);
        }
      }
    layer_diffusion_ratios ')'
      { if (lefrLayerCbk)
          lefrLayer.lefiLayer::setAntennaPWL(antennaType, lefrAntennaPWLPtr);
      }

layer_diffusion_ratios: /* empty */
  | layer_diffusion_ratios layer_diffusion_ratio
  ;

layer_diffusion_ratio:
  pt
  { if (lefrLayerCbk)
      lefrAntennaPWLPtr->lefiAntennaPWL::addAntennaPWL($1.x, $1.y);
  }

layer_antenna_duo: /* empty */
  | K_DIFFUSEONLY
      { 
        use5_4 = 1;
        if (ignoreVersion) {
           /* do nothing */
        }
        else if ((antennaType == lefiAntennaAF) && (versionNum <= 5.3)) {
           yyerror("ANTENNAAREAFACTOR with DIFFUSEONLY is a 5.4 syntax. Your lef file is not defined as 5.4");
           CHKERR();
        } else if (use5_3) {
           yyerror("ANTENNAAREAFACTOR with DIFFUSEONLY is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
           CHKERR();
        }
        if (lefrLayerCbk)
          lefrLayer.lefiLayer::setAntennaDUO(antennaType);
      }

layer_table_type:
  K_PEAK       {$$ = (char*)"PEAK";}
  | K_AVERAGE  {$$ = (char*)"AVERAGE";}
  | K_RMS      {$$ = (char*)"RMS";}

ac_layer_table_opt_list:
  | ac_layer_table_opt_list ac_layer_table_opt

ac_layer_table_opt:
  K_FREQUENCY NUMBER
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber($2); }
  number_list ';'
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addAcFrequency(); }
  | K_CUTAREA NUMBER
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber($2); }
  number_list ';'
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addAcCutarea(); }
  | K_TABLEENTRIES NUMBER
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber($2); }
  number_list
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addAcTableEntry(); }
  | K_WIDTH NUMBER
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber($2); }
  number_list ';'
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addAcWidth(); }

dc_layer_table_opt_list:
  | dc_layer_table_opt_list dc_layer_table_opt

dc_layer_table_opt:
  K_CUTAREA NUMBER
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber($2); }
  number_list ';'
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addDcCutarea(); }
  | K_TABLEENTRIES NUMBER
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber($2); }
  number_list
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addDcTableEntry(); }
  | K_WIDTH NUMBER
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber($2); }
  number_list ';'
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addDcWidth(); }

number_list:
  | number_list NUMBER
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addNumber($2); }

layer_prop_list:
  layer_prop
  | layer_prop_list layer_prop
  ;

layer_prop:
  T_STRING T_STRING
    {
      if (lefrLayerCbk) {
        char propTp;
        propTp = lefrLayerProp.lefiPropType::propType($1);
        lefrLayer.lefiLayer::addProp($1, $2, propTp);
      }
    }
  | T_STRING QSTRING
    {
      if (lefrLayerCbk) {
        char propTp;
        propTp = lefrLayerProp.lefiPropType::propType($1);
        lefrLayer.lefiLayer::addProp($1, $2, propTp);
      }
    }
  | T_STRING NUMBER
    {
      char temp[32];
      sprintf(temp, "%g", $2);
      if (lefrLayerCbk) {
        char propTp;
        propTp = lefrLayerProp.lefiPropType::propType($1);
        lefrLayer.lefiLayer::addNumProp($1, $2, temp, propTp);
      }
    }

current_density_pwl_list :
  current_density_pwl
    { }
  | current_density_pwl_list current_density_pwl
    { }

current_density_pwl: '(' NUMBER NUMBER ')'
    { if (lefrLayerCbk) lefrLayer.lefiLayer::setCurrentPoint($2, $3); }

cap_points :
  cap_point
  | cap_points cap_point
  ;

cap_point: '(' NUMBER NUMBER ')'
    { if (lefrLayerCbk) lefrLayer.lefiLayer::setCapacitancePoint($2, $3); }

res_points :
  res_point
  | res_points res_point
    { }

res_point: '(' NUMBER NUMBER ')'
    { if (lefrLayerCbk) lefrLayer.lefiLayer::setResistancePoint($2, $3); }

layer_type:
  K_ROUTING       {$$ = (char*)"ROUTING"; layerRout = 1;}
  | K_CUT         {$$ = (char*)"CUT"; layerCut = 1;}
  | K_OVERLAP     {$$ = (char*)"OVERLAP"; layerMastOver = 1;}
  | K_MASTERSLICE {$$ = (char*)"MASTERSLICE"; layerMastOver = 1;}
  | K_VIRTUAL     {$$ = (char*)"VIRTUAL";}
  | K_IMPLANT     {$$ = (char*)"IMPLANT";}

layer_direction:
  K_HORIZONTAL      {$$ = (char*)"HORIZONTAL";}
  |  K_VERTICAL     {$$ = (char*)"VERTICAL";}

layer_connect_type: {$$ = (char*)"";}
  | K_FROMABOVE
    {
      if (versionNum < 5.5) {
         yyerror("FROMABOVE is a 5.5 syntax. Your lef file is not defined as 5.5");
         CHKERR();
      }
      $$ = (char*)"FROMABOVE";

    }
  | K_FROMBELOW
    {
      if (versionNum < 5.5) {
         yyerror("FROMBELOW is a 5.5 syntax. Your lef file is not defined as 5.5");
         CHKERR();
      }
      $$ = (char*)"FROMBELOW";
    }

layer_cuts_required:
  | K_LENGTH NUMBER K_WITHIN NUMBER
    {   
      if (versionNum < 5.5) {
         yyerror("LENGTH WITHIN is a 5.5 syntax. Your lef file is not defined as 5.5");
         CHKERR();
      }
      if (lefrLayerCbk)
         lefrLayer.lefiLayer::addMinimumcutLengDis($2, $4);
    }

layer_minen_width:
  | K_WIDTH NUMBER
    {
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addMinenclosedareaWidth($2);
    }

layer_oxide:
  K_OXIDE1
    {
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addAntennaModel(1);
    }
  | K_OXIDE2
    {
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addAntennaModel(2);
    }
  | K_OXIDE3
    {
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addAntennaModel(3);
    }
  | K_OXIDE4
    {
    if (lefrLayerCbk)
       lefrLayer.lefiLayer::addAntennaModel(4);
    }

layer_sp_parallel_widths: /* empty */
    { }
  | layer_sp_parallel_widths layer_sp_parallel_width
    { }

layer_sp_parallel_width: K_WIDTH NUMBER
    { 
      if (lefrLayerCbk) {
         lefrLayer.lefiLayer::addSpParallelWidth($2);
      }
    }
    number_list
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addSpParallelWidthSpacing(); }
 
layer_sp_influence_widths: /* empty */
    { }
  | layer_sp_influence_widths layer_sp_influence_width
    { }

layer_sp_influence_width: K_WIDTH NUMBER K_WITHIN NUMBER K_SPACING NUMBER
    { if (lefrLayerCbk) lefrLayer.lefiLayer::addSpInfluence($2, $4, $6); }

maxstack_via: K_MAXVIASTACK NUMBER ';'
    {
      if (!lefrHasLayer) {  /* 5.5 */
        yyerror("MAXVIASTACK has to define after the LAYER");
        CHKERR();
      } else if (lefrHasMaxVS) {
        yyerror("Only one MAXVIASTACK is allowed in a lef file");
        CHKERR();
      } else {
        if (lefrMaxStackViaCbk)
           lefrMaxStackVia.lefiMaxStackVia::setMaxStackVia($2);
        CALLBACK(lefrMaxStackViaCbk, lefrMaxStackViaCbkType, &lefrMaxStackVia);
      }
      if (versionNum < 5.5) {
         yyerror("MAXVIASTACK is a 5.5 syntax. Your lef file is not defined as 5.5");
         CHKERR();
      }
      lefrHasMaxVS = 1;
    }
  | K_MAXVIASTACK NUMBER K_RANGE {lefDumbMode = 2; lefNoNum= 2;}
    T_STRING T_STRING ';'
    {
      if (!lefrHasLayer) {  /* 5.5 */
        yyerror("MAXVIASTACK has to define after the LAYER");
        CHKERR();
      } else if (lefrHasMaxVS) {
        yyerror("Only one MAXVIASTACK is allowed in a lef file");
        CHKERR();
      } else {
        if (lefrMaxStackViaCbk) {
           lefrMaxStackVia.lefiMaxStackVia::setMaxStackVia($2);
           lefrMaxStackVia.lefiMaxStackVia::setMaxStackViaRange($5, $6);
        }
        CALLBACK(lefrMaxStackViaCbk, lefrMaxStackViaCbkType, &lefrMaxStackVia);
      }
      lefrHasMaxVS = 1;
    }

via: start_via via_options end_via
    { CALLBACK(lefrViaCbk, lefrViaCbkType, &lefrVia); }


via_keyword : K_VIA                 /*needed to have a VIA named via*/
     { lefDumbMode = 1; lefNoNum = 1;}

start_via: via_keyword T_STRING 
    {
      /* 0 is nodefault */
      if (lefrViaCbk) lefrVia.lefiVia::setName($2, 0);
      viaLayer = 0;
      numVia++;
      strcpy(viaName, $2);
    }
  | via_keyword T_STRING K_DEFAULT
    {
      /* 1 is default */
      if (lefrViaCbk) lefrVia.lefiVia::setName($2, 1);
      viaLayer = 0;
      strcpy(viaName, $2);
    }

via_options:
  /* empty */
  | via_options via_option
  ;

via_option:
  via_foreign 
    { }
  | via_layer_rule 
    { }
  | K_RESISTANCE NUMBER ';'
    { if (lefrViaCbk) lefrVia.lefiVia::setResistance($2); }
  | K_PROPERTY { lefDumbMode = 1000000; } via_prop_list ';'
    { lefDumbMode = 0; }
  | K_TOPOFSTACKONLY
    { if (lefrViaCbk) lefrVia.lefiVia::setTopOfStack(); }

via_prop_list:
  via_name_value_pair
  | via_prop_list via_name_value_pair
  ;

via_name_value_pair:
  T_STRING NUMBER
    { 
      char temp[32];
      sprintf(temp, "%g", $2);
      if (lefrViaCbk) {
         char propTp;
         propTp = lefrViaProp.lefiPropType::propType($1);
         lefrVia.lefiVia::addNumProp($1, $2, temp, propTp);
      }
    }
  | T_STRING QSTRING
    {
      if (lefrViaCbk) {
         char propTp;
         propTp = lefrViaProp.lefiPropType::propType($1);
         lefrVia.lefiVia::addProp($1, $2, propTp);
      }
    }
  | T_STRING T_STRING
    {
      if (lefrViaCbk) {
         char propTp;
         propTp = lefrViaProp.lefiPropType::propType($1);
         lefrVia.lefiVia::addProp($1, $2, propTp);
      }
    }

via_foreign:
  start_foreign ';'
    { if (lefrViaCbk) lefrVia.lefiVia::setForeign($1, 0, 0.0, 0.0, -1); }
  | start_foreign pt ';'
    { if (lefrViaCbk) lefrVia.lefiVia::setForeign($1, 1, $2.x, $2.y, -1); }
  | start_foreign pt orientation ';'
    { if (lefrViaCbk) lefrVia.lefiVia::setForeign($1, 1, $2.x, $2.y, $3); }
  | start_foreign orientation ';'
    { if (lefrViaCbk) lefrVia.lefiVia::setForeign($1, 0, 0.0, 0.0, $2); }

start_foreign:	K_FOREIGN {lefDumbMode = 1; lefNoNum= 1;} T_STRING
    { $$ = $3; }

orientation:
  K_N         {$$ = 0;}
  | K_W       {$$ = 1;}
  | K_S       {$$ = 2;}
  | K_E       {$$ = 3;}
  | K_FN      {$$ = 4;}
  | K_FW      {$$ = 5;}
  | K_FS      {$$ = 6;}
  | K_FE      {$$ = 7;}
  | K_R0      {$$ = 0;}
  | K_R90     {$$ = 1;}
  | K_R180    {$$ = 2;}
  | K_R270    {$$ = 3;}
  | K_MY      {$$ = 4;}
  | K_MYR90   {$$ = 5;}
  | K_MX      {$$ = 6;}
  | K_MXR90   {$$ = 7;}

via_layer_rule: via_layer via_rects
    { }

via_layer: K_LAYER {lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
    {
      if (lefrViaCbk) lefrVia.lefiVia::addLayer($3);
      viaLayer++;
    }

via_rects:
  /* empty */
  | via_rects via_rect 
  ;

via_rect: K_RECT pt pt ';'
    { if (lefrViaCbk)
      lefrVia.lefiVia::addRectToLayer($2.x, $2.y, $3.x, $3.y); }

end_via: K_END {lefDumbMode = 1; lefNoNum = 1;} T_STRING 
    { 
      // 10/17/2001 - Wanda da Rosa, PCR 404149
      //              Error if no layer in via
      if (!viaLayer) {
         yyerror("VIA requires at least one layer");
         CHKERR();
      }
      if (strcmp(viaName, $3) != 0) {
        yyerror("END VIA name is different from VIA");
        CHKERR();
      }
    }

viarule_keyword : K_VIARULE { lefDumbMode = 1; lefNoNum = 1;} T_STRING
    { 
      if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setName($3);
      viaRuleLayer = 0;
      strcpy(viaRuleName, $3);
      isGenerate = 0;
    }

viarule:
  viarule_keyword viarule_layer_list via_names opt_viarule_props end_viarule
    {
      // 10/17/2001 - Wanda da Rosa, PCR 404163
      //              Error if layer number is not equal 2.
      // 11/14/2001 - Wanda da Rosa,
      //              Commented out for pcr 411781
      //if (viaRuleLayer != 2) {
         //yyerror("VIARULE requires two layers");
         //CHKERR();
      //}
      if (viaRuleLayer = 0 || viaRuleLayer > 2) {
         yyerror("VIARULE requires two layers");
         CHKERR();
      }
      CALLBACK(lefrViaRuleCbk, lefrViaRuleCbkType, &lefrViaRule);
    }

viarule_generate:
  viarule_keyword K_GENERATE
    {
      isGenerate = 1;
    }
  viarule_layer_list opt_viarule_props end_viarule
    {
      // 10/17/2001 - Wanda da Rosa, PCR 404181
      //              Error if layer number is not equal 3.
      // 11/14/2001 - Wanda da Rosa,
      //              Commented out for pcr 411781
      //if (viaRuleLayer != 3) {
         //yyerror("VIARULE requires three layers");
         //CHKERR();
      //}
      if (viaRuleLayer = 0) {
         yyerror("VIARULE requires three layers");
         CHKERR();
      }
      if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setGenerate();
      CALLBACK(lefrViaRuleCbk, lefrViaRuleCbkType, &lefrViaRule);
    }

viarule_layer_list :
  viarule_layer
  | viarule_layer_list viarule_layer
  ;

opt_viarule_props:
  /* empty */
  | viarule_props
  ;

viarule_props:
  viarule_prop
  | viarule_props viarule_prop
  ;

viarule_prop: K_PROPERTY { lefDumbMode = 10000000; } viarule_prop_list ';'
    { lefDumbMode = 0; }

viarule_prop_list:
  viarule_prop
  | viarule_prop_list viarule_prop
  ;

viarule_prop:
  T_STRING T_STRING
    {
      if (lefrViaRuleCbk) {
         char propTp;
         propTp = lefrViaRuleProp.lefiPropType::propType($1);
         lefrViaRule.lefiViaRule::addProp($1, $2, propTp);
      }
    }
  | T_STRING QSTRING
    {
      if (lefrViaRuleCbk) {
         char propTp;
         propTp = lefrViaRuleProp.lefiPropType::propType($1);
         lefrViaRule.lefiViaRule::addProp($1, $2, propTp);
      }
    }
  | T_STRING NUMBER
    {
      char temp[32];
      sprintf(temp, "%g", $2);
      if (lefrViaRuleCbk) {
         char propTp;
         propTp = lefrViaRuleProp.lefiPropType::propType($1);
         lefrViaRule.lefiViaRule::addNumProp($1, $2, temp, propTp);
      }
    }

viarule_layer: viarule_layer_name viarule_layer_options
    {
      // 10/18/2001 - Wanda da Rosa PCR 404181
      //              Make sure the 1st 2 layers in viarule has direction
      if (viaRuleLayer < 2 && !viaRuleHasDir && !viaRuleHasEnc && isGenerate) {
         yyerror("VIARULE requires the DIRECTION construct in LAYER");
         CHKERR(); 
      }
      viaRuleLayer++;
    }
  ;

via_names:
  /* empty */
  | via_names via_name
  ;

via_name: via_keyword T_STRING ';'
    { if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::addViaName($2); }

viarule_layer_name: K_LAYER {lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
    { if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setLayer($3);
      viaRuleHasDir = 0;
      viaRuleHasEnc = 0;
    }

viarule_layer_options:
  /* empty */
  | viarule_layer_options viarule_layer_option
  ;

viarule_layer_option:
  K_DIRECTION K_HORIZONTAL ';'
    {
      if (viaRuleHasEnc) {
        yyerror("Either DIRECTION or ENCLOSURE can be specified in a layer");
        CHKERR();
      } else {
        if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setHorizontal();
      }
      viaRuleHasDir = 1;
    }
  | K_DIRECTION K_VERTICAL ';'
    { 
      if (viaRuleHasEnc) {
        yyerror("Either DIRECTION or ENCLOSURE can be specified in a layer");
        CHKERR();
      } else {
        if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setVertical();
      }
      viaRuleHasDir = 1;
    }
  | K_ENCLOSURE NUMBER NUMBER ';'    /* 5.5 */
    {
      if (versionNum < 5.5) {
         yyerror("ENCLOSURE is a 5.5 syntax. Your lef file is not defined as 5.5");
         CHKERR();
      }
      if (viaRuleHasDir) {
         yyerror("Either DIRECTION or ENCLOSURE can be specified in a layer");
         CHKERR();
      } else {
         if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setEnclosure($2, $3);
      }
      viaRuleHasEnc = 1;
    }
  | K_WIDTH NUMBER K_TO NUMBER ';'
    { if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setWidth($2,$4); }
  | K_RECT pt pt ';'
    { if (lefrViaRuleCbk)
	lefrViaRule.lefiViaRule::setRect($2.x, $2.y, $3.x, $3.y); }
  | K_SPACING NUMBER K_BY NUMBER ';'
    { if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setSpacing($2,$4); }
  | K_RESISTANCE NUMBER ';'
    { if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setResistance($2); }
  | K_OVERHANG NUMBER ';'
    {
      if (!viaRuleHasDir) {
         yyerror("Either OVERHANG can only be defined with DIRECTION");
         CHKERR();
      }
      if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setOverhang($2);
    }
  | K_METALOVERHANG NUMBER ';'
    {
      if (!viaRuleHasDir) {
         yyerror("Either METALOVERHANG can only be defined with DIRECTION");
         CHKERR();
      }
      if (lefrViaRuleCbk) lefrViaRule.lefiViaRule::setMetalOverhang($2);
    }

end_viarule: K_END {lefDumbMode = 1; lefNoNum = 1;}  T_STRING 
    {
      if (strcmp(viaRuleName, $3) != 0) {
        yyerror("END VIARULE name is different from VIARULE");
        CHKERR();
      }
    }

spacing_rule: start_spacing spacings end_spacing 
    { }

start_spacing: K_SPACING
    { if (lefrSpacingBeginCbk)
	CALLBACK(lefrSpacingBeginCbk, lefrSpacingBeginCbkType, 0); }

end_spacing: K_END K_SPACING
    { if (lefrSpacingEndCbk)
            CALLBACK(lefrSpacingEndCbk, lefrSpacingEndCbkType, 0); }

spacings:
  spacing
  | spacings spacing
  ;

spacing:  samenet_keyword T_STRING T_STRING NUMBER ';'
    {
      if (lefrSpacingCbk) {
	lefrSpacing.lefiSpacing::set($2, $3, $4, 0);
	CALLBACK(lefrSpacingCbk, lefrSpacingCbkType, &lefrSpacing);
      }
    }
  | samenet_keyword T_STRING T_STRING NUMBER K_STACK ';'
    {
      if (lefrSpacingCbk) {
	lefrSpacing.lefiSpacing::set($2, $3, $4, 1);
	CALLBACK(lefrSpacingCbk, lefrSpacingCbkType, &lefrSpacing);
      }
    }

samenet_keyword: K_SAMENET
    { lefDumbMode = 2; lefNoNum = 2; } /* must be followed by two names */

irdrop: start_irdrop ir_tables end_irdrop
    { }

start_irdrop: K_IRDROP
    {
      if (versionNum < 5.4) {
        if (lefrIRDropBeginCbk) 
	  CALLBACK(lefrIRDropBeginCbk, lefrIRDropBeginCbkType, 0);
      } else
        yywarning("IRDROP is obsolete in 5.4. It will be ignored.");
    }

end_irdrop: K_END K_IRDROP
    {
      if (versionNum < 5.4) {
        if (lefrIRDropEndCbk)
	  CALLBACK(lefrIRDropEndCbk, lefrIRDropEndCbkType, 0);
      }
    }
      

ir_tables:
  /* empty */
  | ir_tables ir_table
  ;

ir_table: ir_tablename ir_table_values ';'
    { 
      if (versionNum < 5.4) {
        CALLBACK(lefrIRDropCbk, lefrIRDropCbkType, &lefrIRDrop);
      }
    }

ir_table_values:
  /* empty */
  | ir_table_values ir_table_value 
  ;

ir_table_value: NUMBER NUMBER 
  { if (lefrIRDropCbk) lefrIRDrop.lefiIRDrop::setValues($1, $2); }

ir_tablename: K_TABLE T_STRING
  { if (lefrIRDropCbk) lefrIRDrop.lefiIRDrop::setTableName($2); }

minfeature: K_MINFEATURE NUMBER NUMBER ';'
  {
    if (versionNum < 5.4) {
       if (lefrMinFeatureCbk)
         lefrMinFeature.lefiMinFeature::set($2, $3);
       CALLBACK(lefrMinFeatureCbk, lefrMinFeatureCbkType, &lefrMinFeature);
    } else
       yywarning("MINFEATURE is obsolete in 5.4. It will be ignored.");
  }

dielectric: K_DIELECTRIC NUMBER ';'
  {
    if (versionNum < 5.4) {
       CALLBACK(lefrDielectricCbk, lefrDielectricCbkType, $2);
    } else
       yywarning("DIELECTRIC is obsolete in 5.4. It will be ignored.");
  }

nondefault_rule: K_NONDEFAULTRULE T_STRING
  {
    if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::setName($2);
    ndLayer = 0;
    numVia = 0;
    strcpy(nonDefaultRuleName, $2);
  }
  nd_rules end_nd_rule
  {
    // 10/18/2001 - Wanda da Rosa, PCR 404189
    //              At least 1 layer is required
    if (!ndLayer) {
       yyerror("NONDEFAULTRULE requires at least one layer");
       CHKERR();
    }
    if (!numVia) {
       yyerror("NONDEFAULTRULE requires at least one via");
       CHKERR();
    }
    if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::end();
    CALLBACK(lefrNonDefaultCbk, lefrNonDefaultCbkType, &lefrNonDefault);
  }

end_nd_rule: K_END
  | K_END T_STRING
    {
      if (strcmp(nonDefaultRuleName, $2) != 0) {
        yyerror("END NONDEFAULTRULE name is different from NONDEFAULTRULE");
        CHKERR();
      }
    }
  ;

nd_rules:
  /* empty */
  | nd_rules nd_rule
  ;

nd_rule:
  nd_layer
  | via
  | spacing_rule
  | nd_prop
  ;

nd_prop: K_PROPERTY { lefDumbMode = 10000000; } nd_prop_list ';'
    { lefDumbMode = 0; }

nd_prop_list:
  nd_prop
  | nd_prop_list nd_prop
  ;

nd_prop:
  T_STRING T_STRING
    {
      if (lefrNonDefaultCbk) {
         char propTp;
         propTp = lefrNondefProp.lefiPropType::propType($1);
         lefrNonDefault.lefiNonDefault::addProp($1, $2, propTp);
      }
    }
  | T_STRING QSTRING
    {
      if (lefrNonDefaultCbk) {
         char propTp;
         propTp = lefrNondefProp.lefiPropType::propType($1);
         lefrNonDefault.lefiNonDefault::addProp($1, $2, propTp);
      }
    }
  | T_STRING NUMBER
    {
      char temp[32];
      sprintf(temp, "%g", $2);
      if (lefrNonDefaultCbk) {
         char propTp;
         propTp = lefrNondefProp.lefiPropType::propType($1);
         lefrNonDefault.lefiNonDefault::addNumProp($1, $2, temp, propTp);
      }
    }

nd_layer: K_LAYER T_STRING
  {
    if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::addLayer($2);
    ndLayer++;
    strcpy(layerName, $2);
    ndLayerWidth = 0;
    ndLayerSpace = 0;
  }
  nd_layer_stmts K_END T_STRING
  {
    if (strcmp(layerName, $6) != 0) {
      yyerror("END LAYER name is different from LAYER");
      CHKERR();
    }
    if (!ndLayerWidth) {
      yyerror("WIDTH is required in LAYER inside NONDEFULTRULE");
      CHKERR();
    }
    if (!ndLayerSpace) {
      yyerror("SPACING is required in LAYER inside NONDEFULTRULE");
      CHKERR();
    }
  }
  ;

nd_layer_stmts:
  /* empty */
  | nd_layer_stmts nd_layer_stmt
  ;

nd_layer_stmt:
  K_WIDTH NUMBER ';'
    { 
      ndLayerWidth = 1;
      if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::addWidth($2);
    }
  | K_SPACING NUMBER ';'
    {
      ndLayerSpace = 1;
      if (lefrNonDefaultCbk) lefrNonDefault.lefiNonDefault::addSpacing($2);
    }
  | K_WIREEXTENSION NUMBER ';'
    { if (lefrNonDefaultCbk)
         lefrNonDefault.lefiNonDefault::addWireExtension($2); }
  | K_RESISTANCE K_RPERSQ NUMBER ';'
    {
      if (ignoreVersion) {
         if (lefrNonDefaultCbk)
            lefrNonDefault.lefiNonDefault::addResistance($3);
      } else if (versionNum < 5.4) {
         yyerror("RESISTANCE RPERSQ is a 5.4 syntax.  Your lef file is not defined as version 5.4");
         CHKERR();
      }
      else if (lefrNonDefaultCbk)
         lefrNonDefault.lefiNonDefault::addResistance($3);
    } 
 
  | K_CAPACITANCE K_CPERSQDIST NUMBER ';'
    {
      if (ignoreVersion) {
         if (lefrNonDefaultCbk)
            lefrNonDefault.lefiNonDefault::addCapacitance($3);
      } else if (versionNum < 5.4) {
         yyerror("CAPACITANCE CPERSQDIST is a 5.4 syntax.  Your lef file is not defined as version 5.4");
         CHKERR();
      }
      else if (lefrNonDefaultCbk)
         lefrNonDefault.lefiNonDefault::addCapacitance($3);
    }
  | K_EDGECAPACITANCE NUMBER ';'
    {
      if (ignoreVersion) {
         if (lefrNonDefaultCbk)
            lefrNonDefault.lefiNonDefault::addEdgeCap($2);
      } else if (versionNum < 5.4) {
         yyerror("EDGECAPACITANCE is a 5.4 syntax.  Your lef file is not defined as version 5.4");
         CHKERR();
      }
      else if (lefrNonDefaultCbk)
         lefrNonDefault.lefiNonDefault::addEdgeCap($2);
    }

site: start_site site_options end_site
    { CALLBACK(lefrSiteCbk, lefrSiteCbkType, &lefrSite); }

start_site: K_SITE {lefDumbMode = 1; lefNoNum = 1;} T_STRING 
    { 
      if (lefrSiteCbk) lefrSite.lefiSite::setName($3);
      strcpy(siteName, $3);
    }

end_site: K_END {lefDumbMode = 1; lefNoNum = 1;} T_STRING
    {
      if (strcmp(siteName, $3) != 0) {
        yyerror("END SITE name is different from SITE");
        CHKERR();
      }
    }

site_options:
  /* empty */
  | site_options site_option
  ;

site_option:
  K_SIZE NUMBER K_BY NUMBER ';' 
    { if (lefrSiteCbk) lefrSite.lefiSite::setSize($2,$4); }
  | site_symmetry_statement
    { }
  | site_class 
    { if (lefrSiteCbk) lefrSite.lefiSite::setClass($1); }

site_class:
  K_CLASS K_PAD ';' {$$ = (char*)"PAD"; }
  | K_CLASS K_CORE ';'  {$$ = (char*)"CORE"; }
  | K_CLASS K_VIRTUAL ';'  {$$ = (char*)"VIRTUAL"; }

site_symmetry_statement: K_SYMMETRY site_symmetries ';'
    { }

site_symmetries:
  /* empty */
  | site_symmetries site_symmetry
  ;

site_symmetry:
  K_X 
    { if (lefrSiteCbk) lefrSite.lefiSite::setXSymmetry(); }
  | K_Y 
    { if (lefrSiteCbk) lefrSite.lefiSite::setYSymmetry(); }
  | K_R90
    { if (lefrSiteCbk) lefrSite.lefiSite::set90Symmetry(); }


pt:
  NUMBER NUMBER
    { $$.x = $1; $$.y = $2; }
  | '(' NUMBER NUMBER ')'
    { $$.x = $2; $$.y = $3; }

macro: start_macro macro_options
    { CALLBACK(lefrMacroCbk, lefrMacroCbkType, &lefrMacro); lefrDoSite = 0;
    }
    end_macro

start_macro: K_MACRO {lefDumbMode = 1; lefNoNum = 1;} T_STRING 
    {
      siteDef = 0;
      symDef = 0;
      sizeDef = 0; 
      pinDef = 0; 
      obsDef = 0; 
      origDef = 0;
      lefrMacro.lefiMacro::clear();      
      if (lefrMacroCbk) lefrMacro.lefiMacro::setName($3);
      CALLBACK(lefrMacroBeginCbk, lefrMacroBeginCbkType, $3);
      strcpy(macroName, $3);
    }

end_macro: K_END {lefDumbMode = 1; lefNoNum = 1;} T_STRING
    {
      if (strcmp(macroName, $3) != 0) {
        yyerror("END MACRO name is different from MACRO");
        CHKERR();
      }
      CALLBACK(lefrMacroEndCbk, lefrMacroEndCbkType, $3);
    }

macro_options:
  /* empty */
  | macro_options macro_option 
  ;

macro_option:
  macro_class 
  | macro_generator 
  | macro_generate 
  | macro_source
  | macro_symmetry_statement 
  | macro_origin 
      { }
  | macro_power 
      { }
  | macro_foreign
      { }
  | macro_eeq 
  | macro_leq 
  | macro_size 
      { }
  | macro_site 
      { }
  | macro_pin 
      { }
  | K_FUNCTION K_BUFFER ';'
      { if (lefrMacroCbk) lefrMacro.lefiMacro::setBuffer(); }
  | K_FUNCTION K_INVERTER ';'
      { if (lefrMacroCbk) lefrMacro.lefiMacro::setInverter(); }
  | macro_obs 
      { }
  | macro_clocktype 
      { }
  | timing
      { }
  | K_PROPERTY {lefDumbMode = 1000000;} macro_prop_list  ';'
      { lefDumbMode = 0; }

macro_prop_list:
  macro_name_value_pair
  | macro_prop_list macro_name_value_pair
  ;

macro_symmetry_statement: K_SYMMETRY macro_symmetries ';'
    {
      if (siteDef) { /* SITE is defined before SYMMETRY */
          /* pcr 283846 suppress warning */
          yywarning("SITE is defined before SYMMETRY.");
      }
      symDef = 1;
    }

macro_symmetries:
  /* empty */
  | macro_symmetries macro_symmetry
  ;

macro_symmetry:
  K_X 
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setXSymmetry(); }
  | K_Y 
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setYSymmetry(); }
  | K_R90
    { if (lefrMacroCbk) lefrMacro.lefiMacro::set90Symmetry(); }

macro_name_value_pair:
  T_STRING NUMBER
    {
      char temp[32];
      sprintf(temp, "%g", $2);
      if (lefrMacroCbk) {
         char propTp;
         propTp = lefrMacroProp.lefiPropType::propType($1);
         lefrMacro.lefiMacro::setNumProperty($1, $2, temp,  propTp);
      }
    }
  | T_STRING QSTRING
    {
      if (lefrMacroCbk) {
         char propTp;
         propTp = lefrMacroProp.lefiPropType::propType($1);
         lefrMacro.lefiMacro::setProperty($1, $2, propTp);
      }
    }
  | T_STRING T_STRING
    {
      if (lefrMacroCbk) {
         char propTp;
         propTp = lefrMacroProp.lefiPropType::propType($1);
         lefrMacro.lefiMacro::setProperty($1, $2, propTp);
      }
    }

macro_class: K_CLASS class_type ';'
    {
       if (lefrMacroCbk) lefrMacro.lefiMacro::setClass($2);
       if (lefrMacroClassTypeCbk)
          CALLBACK(lefrMacroClassTypeCbk, lefrMacroClassTypeCbkType, $2);
    }

class_type:
  K_COVER {$$ = (char*)"COVER"; }
  | K_COVER K_BUMP {$$ = (char*)"COVER BUMP"; }
  | K_RING    {$$ = (char*)"RING"; }
  | K_BLOCK   {$$ = (char*)"BLOCK"; }
  | K_BLOCK K_BLACKBOX  {$$ = (char*)"BLOCK BLACKBOX"; }
  | K_NONE    {$$ = (char*)"NONE"; }
  | K_PAD     {$$ = (char*)"PAD"; }
  | K_VIRTUAL {$$ = (char*)"VIRTUAL"; }
  | K_PAD  pad_type 
      {sprintf(temp_name, "PAD %s", $2);
      $$ = temp_name;} 
  | K_CORE    {$$ = (char*)"CORE"; }
  | K_CORNER 
      {$$ = (char*)"CORNER";
      /* This token is NOT in the spec but has shown up in 
       * some lef files.  This exception came from LEFOUT
       * in 'frameworks'
       */
      }
  | K_CORE core_type
      {sprintf(temp_name, "CORE %s", $2);
      $$ = temp_name;} 
  | K_ENDCAP endcap_type
      {sprintf(temp_name, "ENDCAP %s", $2);
      $$ = temp_name;} 

pad_type:
  K_INPUT 	{$$ = (char*)"INPUT";}
  | K_OUTPUT	{$$ = (char*)"OUTPUT";}
  | K_INOUT 	{$$ = (char*)"INOUT";}
  | K_POWER 	{$$ = (char*)"POWER";}
  | K_SPACER	{$$ = (char*)"SPACER";}
  | K_AREAIO	{$$ = (char*)"AREAIO";}

core_type:
  K_FEEDTHRU        {$$ = (char*)"FEEDTHRU";}
  | K_TIEHIGH       {$$ = (char*)"TIEHIGH";}
  | K_TIELOW        {$$ = (char*)"TIELOW";}
  | K_SPACER
    { 
      if (ignoreVersion)
        $$ = (char*)"SPACER";
      else if (versionNum < 5.4) {
        yyerror("SPACER is a 5.4 syntax.  Your lef file is not defined as version 5.4"); 
        CHKERR();
      }
      else
        $$ = (char*)"SPACER";
    }
  | K_ANTENNACELL
    { 
      if (ignoreVersion)
        $$ = (char*)"ANTENNACELL";
      else if (versionNum < 5.4) {
        yyerror("ANTENNACELL is a 5.4 syntax.  Your lef file is not defined as version 5.4"); 
        CHKERR();
      }
      else
        $$ = (char*)"ANTENNACELL";
    }

endcap_type:
  K_PRE 	{$$ = (char*)"PRE";}
  | K_POST 	{$$ = (char*)"POST";}
  | K_TOPLEFT 	{$$ = (char*)"TOPLEFT";}
  | K_TOPRIGHT 	{$$ = (char*)"TOPRIGHT";}
  | K_BOTTOMLEFT 	{$$ = (char*)"BOTTOMLEFT";}
  | K_BOTTOMRIGHT	{$$ = (char*)"BOTTOMRIGHT";}

macro_generator: K_GENERATOR T_STRING ';'
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setGenerator($2); }

macro_generate: K_GENERATE T_STRING T_STRING ';'
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setGenerate($2, $3); }

macro_source:
  K_SOURCE K_USER ';'
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setSource("USER"); }
  | K_SOURCE K_GENERATE ';'
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setSource("GENERATE"); }
  | K_SOURCE K_BLOCK ';'
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setSource("BLOCK"); }

macro_power: K_POWER NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrMacroCbk) lefrMacro.lefiMacro::setPower($2);
      } else
        yywarning("MACRO POWER is obsolete in 5.4. It will be ignored.");
    }

macro_origin: K_ORIGIN pt ';'
    { 
       if (origDef) { /* Has multiple ORIGIN defined in a macro, stop parsing*/
          yyerror("ORIGIN statement is defined more than once.  Parser stops executions.");
          CHKERR();
       }
       origDef = 1;
       if (siteDef) { /* SITE is defined before ORIGIN */
          /* pcr 283846 suppress warning */
          yywarning("SITE is defined before ORIGIN.");
       }
       if (pinDef) { /* PIN is defined before ORIGIN */
          /* pcr 283846 suppress warning */
          yywarning("PIN is defined before ORIGIN.");
       }
       if (obsDef) { /* OBS is defined before ORIGIN */
          /* pcr 283846 suppress warning */
          yywarning("OBS is defined before ORIGIN.");
       }
       /* 11/22/99 - Wanda da Rosa. PCR 283846 
                     can be defined any order.
       if (symDef)  * SYMMETRY is defined before ORIGIN *
          yywarning("SYMMETRY is defined before ORIGIN.");
       */
       /* Add back it back in per Nora request on PCR 283846 */
       /* 1/14/2000 - Wanda da Rosa, PCR 288770
       if (sizeDef)  * SIZE is defined before ORIGIN *
          yywarning("SIZE is defined before ORIGIN.");
       */
      
       if (lefrMacroCbk) lefrMacro.lefiMacro::setOrigin($2.x, $2.y);
       if (lefrMacroOriginCbk) {
          macroNum.x = $2.x; 
          macroNum.y = $2.y; 
          CALLBACK(lefrMacroOriginCbk, lefrMacroOriginCbkType, macroNum);
       }
    }

macro_foreign:
  start_foreign ';'
    { if (lefrMacroCbk)
      lefrMacro.lefiMacro::setForeign($1, 0, 0.0, 0.0, -1); }
  | start_foreign pt ';'
    { if (lefrMacroCbk)
      lefrMacro.lefiMacro::setForeign($1, 1, $2.x, $2.y, -1); }
  | start_foreign pt orientation ';'
    { if (lefrMacroCbk)
      lefrMacro.lefiMacro::setForeign($1, 1, $2.x, $2.y, $3); }
  | start_foreign orientation ';'
    { if (lefrMacroCbk)
      lefrMacro.lefiMacro::setForeign($1, 0, 0.0, 0.0, $2); }

macro_eeq: K_EEQ { lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setEEQ($3); }

macro_leq: K_LEQ { lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setLEQ($3); }

macro_site:
  macro_site_word  T_STRING ';'
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setSiteName($2); }
  | macro_site_word sitePattern ';'
    {
      if (lefrMacroCbk) {
	lefrMacro.lefiMacro::setSitePattern(lefrSitePatternPtr);
	lefrSitePatternPtr = 0;
      }
    }

macro_site_word: K_SITE
    { lefDumbMode = 1; lefNoNum = 1; siteDef = 1;
        if (lefrMacroCbk) lefrDoSite = 1; }

site_word: K_SITE
    { lefDumbMode = 1; lefNoNum = 1; }

macro_size: K_SIZE NUMBER K_BY NUMBER ';'
    { 
      if (siteDef) { /* SITE is defined before SIZE */
         /* pcr 283846 suppress warning
         yywarning("SITE is defined before SIZE.");
         return 1; 
         */
      }
      sizeDef = 1;
      if (lefrMacroCbk) lefrMacro.lefiMacro::setSize($2, $4);
       if (lefrMacroSizeCbk) {
          macroNum.x = $2; 
          macroNum.y = $4; 
          CALLBACK(lefrMacroSizeCbk, lefrMacroSizeCbkType, macroNum);
       }
    }

/* This is confusing, since FEF and LEF have opposite definitions of
   ports and pins */

macro_pin: start_macro_pin macro_pin_options end_macro_pin
    { CALLBACK(lefrPinCbk, lefrPinCbkType, &lefrPin);
      lefrPin.lefiPin::clear();
    }

start_macro_pin: K_PIN {lefDumbMode = 1; lefNoNum = 1; pinDef = 1;} T_STRING 
    { if (lefrPinCbk) lefrPin.lefiPin::setName($3);
      strcpy(pinName, $3);
    }

end_macro_pin: K_END {lefDumbMode = 1; lefNoNum = 1;} T_STRING
    {
      if (strcmp(pinName, $3) != 0) {
        yyerror("END PIN name is different from PIN");
        CHKERR();
      }
    }

macro_pin_options:
  /* empty */
    { }
  | macro_pin_options macro_pin_option 
    { }

macro_pin_option:
  start_foreign ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setForeign($1, 0, 0.0, 0.0, -1); }
  | start_foreign pt ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setForeign($1, 1, $2.x, $2.y, -1); }
  | start_foreign pt orientation ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setForeign($1, 1, $2.x, $2.y, $3); }
  | start_foreign K_STRUCTURE ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setForeign($1, 0, 0.0, 0.0, -1); }
  | start_foreign K_STRUCTURE pt ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setForeign($1, 1, $3.x, $3.y, -1); }
  | start_foreign K_STRUCTURE pt orientation ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setForeign($1, 1, $3.x, $3.y, $4); }
  | K_LEQ { lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setLEQ($3); }
  | K_POWER NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setPower($2);
      } else
        yywarning("MACRO POWER is obsolete in 5.4. It will be ignored.");
    }
  | electrical_direction
    { if (lefrPinCbk) lefrPin.lefiPin::setDirection($1); }
  | K_USE macro_pin_use ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setUse($2); }
  | K_SCANUSE macro_scan_use ';'
    { }
  | K_LEAKAGE NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setLeakage($2);
      } else
        yywarning("MACRO LEAKAGE is obsolete in 5.4. It will be ignored.");
    }
  | K_RISETHRESH NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setRiseThresh($2);
      } else
        yywarning("MACRO RISETHRESH is obsolete in 5.4. It will be ignored.");
    }
  | K_FALLTHRESH NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setFallThresh($2);
      } else
        yywarning("MACRO FALLTHRESH is obsolete in 5.4. It will be ignored.");
    }
  | K_RISESATCUR NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setRiseSatcur($2);
      } else
        yywarning("MACRO RISESATCUR is obsolete in 5.4. It will be ignored.");
    }
  | K_FALLSATCUR NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setFallSatcur($2);
      } else
        yywarning("MACRO FALLSATCUR is obsolete in 5.4. It will be ignored.");
    }
  | K_VLO NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setVLO($2);
      } else
        yywarning("MACRO VLO is obsolete in 5.4. It will be ignored.");
    }
  | K_VHI NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setVHI($2);
      } else
        yywarning("MACRO VHI is obsolete in 5.4. It will be ignored.");
    }
  | K_TIEOFFR NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setTieoffr($2);
      } else
        yywarning("MACRO TIEOFFR is obsolete in 5.4. It will be ignored.");
    }
  | K_SHAPE pin_shape ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setShape($2); }
  | K_MUSTJOIN {lefDumbMode = 1; lefNoNum = 1;} T_STRING ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setMustjoin($3); }
  | K_OUTPUTNOISEMARGIN {lefDumbMode = 1;} NUMBER NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setOutMargin($3, $4);
      } else
        yywarning(
          "MACRO OUTPUTNOISEMARGIN is obsolete in 5.4. It will be ignored.");
    }
  | K_OUTPUTRESISTANCE {lefDumbMode = 1;} NUMBER NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setOutResistance($3, $4);
      } else
        yywarning(
          "MACRO OUTPUTRESISTANCE is obsolete in 5.4. It will be ignored.");
    }
  | K_INPUTNOISEMARGIN {lefDumbMode = 1;} NUMBER NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setInMargin($3, $4);
      } else
        yywarning(
          "MACRO INPUTNOISEMARGIN is obsolete in 5.4. It will be ignored.");
    }
  | K_CAPACITANCE NUMBER ';' 
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setCapacitance($2);
      } else
        yywarning("MACRO CAPACITANCE is obsolete in 5.4. It will be ignored.");
    }
  | K_MAXDELAY NUMBER ';' 
    { if (lefrPinCbk) lefrPin.lefiPin::setMaxdelay($2); }
  | K_MAXLOAD NUMBER ';' 
    { if (lefrPinCbk) lefrPin.lefiPin::setMaxload($2); }
  | K_RESISTANCE NUMBER ';' 
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setResistance($2);
      } else
        yywarning("MACRO RESISTANCE is obsolete in 5.4. It will be ignored.");
    }
  | K_PULLDOWNRES NUMBER ';' 
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setPulldownres($2);
      } else
        yywarning("MACRO PULLDOWNRES is obsolete in 5.4. It will be ignored.");
    }
  | K_CURRENTSOURCE K_ACTIVE ';' 
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setCurrentSource("ACTIVE");
      } else
        yywarning(
          "MACRO CURRENTSOURCE is obsolete in 5.4. It will be ignored.");
    }
  | K_CURRENTSOURCE K_RESISTIVE ';' 
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setCurrentSource("RESISTIVE");
      } else
        yywarning(
          "MACRO CURRENTSOURCE is obsolete in 5.4. It will be ignored.");
    }
  | K_RISEVOLTAGETHRESHOLD NUMBER ';' 
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setRiseVoltage($2);
      } else
        yywarning(
          "MACRO RISEVOLTAGETHRESHOLD is obsolete in 5.4. It will be ignored.");
    }
  | K_FALLVOLTAGETHRESHOLD NUMBER ';' 
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setFallVoltage($2);
      } else
        yywarning(
          "MACRO FALLVOLTAGETHRESHOLD is obsolete in 5.4. It will be ignored.");
    }
  | K_IV_TABLES T_STRING T_STRING ';'
    {
      if (versionNum < 5.4) {
        if (lefrPinCbk) lefrPin.lefiPin::setTables($2, $3);
      } else
        yywarning("MACRO IV_TABLES is obsolete in 5.4. It will be ignored.");
    }
  | K_TAPERRULE T_STRING ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setTaperRule($2); }
  | K_PROPERTY {lefDumbMode = 1000000;} pin_prop_list ';'
    { lefDumbMode = 0; }
  | start_macro_port macro_port_class_option geometries K_END
    {
      lefDumbMode = 0;
      if (lefrPinCbk) {
	lefrPin.lefiPin::addPort(lefrGeometriesPtr);
	lefrGeometriesPtr = 0;
	lefrDoGeometries = 0;
      }
    }
  | start_macro_port K_END
    /* 06/07/2001 - Wanda da Rosa.  Allow empty PORT due to incorrect
     *              old lef files from customers
     */
    {
      // Since in start_macro_port it has call the Init method, here
      // we need to call the Destroy method.
      // Still add a null pointer to set the number of port
      lefrPin.lefiPin::addPort(lefrGeometriesPtr);
      lefrGeometriesPtr = 0;
      lefrDoGeometries = 0;
    }
  | K_ANTENNASIZE NUMBER opt_layer_name ';'
    {  /* a pre 5.4 syntax */
      use5_3 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum >= 5.4) {
        if (use5_4) {
           yyerror("ANTENNASIZE is a 5.3 or earlier syntax.  Your lef file has both old and new syntax, which is illegal.");
           CHKERR();
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaSize($2, $3);
    }
  | K_ANTENNAMETALAREA NUMBER opt_layer_name ';'
    {  /* a pre 5.4 syntax */
      use5_3 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum >= 5.4) {
        if (use5_4) {
           yyerror("ANTENNAMETALAREA is a 5.3 or earlier syntax.  Your lef file has both old and new syntax, which is illegal.");
           CHKERR();
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMetalArea($2, $3);
    }
  | K_ANTENNAMETALLENGTH NUMBER opt_layer_name ';'
    { /* a pre 5.4 syntax */ 
      use5_3 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum >= 5.4) {
        if (use5_4) {
           yyerror("ANTENNAMETALLENGTH is a 5.3 or earlier syntax.  Your lef file has both old and new syntax, which is illegal.");
           CHKERR();
        }
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMetalLength($2, $3);
    }
  | K_RISESLEWLIMIT NUMBER ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setRiseSlewLimit($2); }
  | K_FALLSLEWLIMIT NUMBER ';'
    { if (lefrPinCbk) lefrPin.lefiPin::setFallSlewLimit($2); }
  | K_ANTENNAPARTIALMETALAREA NUMBER opt_layer_name ';'
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        yyerror("ANTENNAPARTIALMETALAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
        CHKERR();
      } else if (use5_3) {
           yyerror("ANTENNAPARTIALMETALAREA is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
           CHKERR();
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaPartialMetalArea($2, $3);
    }
  | K_ANTENNAPARTIALMETALSIDEAREA NUMBER opt_layer_name ';'
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        yyerror("ANTENNAPARTIALMETALSIDEAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
        CHKERR();
      } else if (use5_3) {
         yyerror("ANTENNAPARTIALMETALSIDEAREA is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
         CHKERR();
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaPartialMetalSideArea($2, $3);
    }
  | K_ANTENNAPARTIALCUTAREA NUMBER opt_layer_name ';'
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        yyerror("ANTENNAPARTIALCUTAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
        CHKERR();
      } else if (use5_3) {
        yyerror("ANTENNAPARTIALCUTAREA is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
        CHKERR();
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaPartialCutArea($2, $3);
    }
  | K_ANTENNADIFFAREA NUMBER opt_layer_name ';'
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        yyerror("ANTENNADIFFAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
        CHKERR();
      } else if (use5_3) {
        yyerror("ANTENNADIFFAREA is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
        CHKERR();
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaDiffArea($2, $3);
    }
  | K_ANTENNAGATEAREA NUMBER opt_layer_name ';'
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        yyerror("ANTENNAGATEAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
        CHKERR();
      } else if (use5_3) {
        yyerror("ANTENNAGATEAREA is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
        CHKERR();
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaGateArea($2, $3);
    }
  | K_ANTENNAMAXAREACAR NUMBER req_layer_name ';'
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        yyerror("ANTENNAMAXAREACAR is a 5.4 syntax. Your lef file is not defined as 5.4");
        CHKERR();
      } else if (use5_3) {
         yyerror("ANTENNAMAXAREACAR is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
        CHKERR();
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMaxAreaCar($2, $3);
    }
  | K_ANTENNAMAXSIDEAREACAR NUMBER req_layer_name ';'
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        yyerror("ANTENNAMAXSIDEAREACAR is a 5.4 syntax. Your lef file is not defined as 5.4");
        CHKERR();
      } else if (use5_3) {
        yyerror("ANTENNAMAXSIDEAREACAR is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
        CHKERR();
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMaxSideAreaCar($2, $3);
    }
  | K_ANTENNAMAXCUTCAR NUMBER req_layer_name ';'
    { /* 5.4 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.4) {
        yyerror("ANTENNAMAXCUTCAR is a 5.4 syntax. Your lef file is not defined as 5.4");
        CHKERR();
      } else if (use5_3) {
        yyerror("ANTENNAMAXCUTCAR is a 5.4 syntax. Your lef file has both old and new syntax, which is illegal.");
        CHKERR();
      }
      if (lefrPinCbk) lefrPin.lefiPin::addAntennaMaxCutCar($2, $3);
    }
  | K_ANTENNAMODEL
    { /* 5.5 syntax */
      use5_4 = 1;
      if (ignoreVersion) {
        /* do nothing */
      } else if (versionNum < 5.5) {
        yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file is not defined as 5.5");
        CHKERR();
      } else if (use5_3) {
        yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file has both old and new syntax, which is illegal.");
        CHKERR();
      }
    }
    pin_layer_oxide ';'

pin_layer_oxide:
  K_OXIDE1
    {
    if (lefrPinCbk)
       lefrPin.lefiPin::addAntennaModel(1);
    }
  | K_OXIDE2
    {
    if (lefrPinCbk)
       lefrPin.lefiPin::addAntennaModel(2);
    }
  | K_OXIDE3
    {
    if (lefrPinCbk)
       lefrPin.lefiPin::addAntennaModel(3);
    }
  | K_OXIDE4
    {
    if (lefrPinCbk)
       lefrPin.lefiPin::addAntennaModel(4);
    }

pin_prop_list:
  pin_name_value_pair
  | pin_prop_list pin_name_value_pair
  ;

pin_name_value_pair:
  T_STRING NUMBER
    { 
      char temp[32];
      sprintf(temp, "%g", $2);
      if (lefrPinCbk) {
         char propTp;
         propTp = lefrPinProp.lefiPropType::propType($1);
         lefrPin.lefiPin::setNumProperty($1, $2, temp, propTp);
      }
    }
  | T_STRING QSTRING
    {
      if (lefrPinCbk) {
         char propTp;
         propTp = lefrPinProp.lefiPropType::propType($1);
         lefrPin.lefiPin::setProperty($1, $2, propTp);
      }
    }
  | T_STRING T_STRING
    {
      if (lefrPinCbk) {
         char propTp;
         propTp = lefrPinProp.lefiPropType::propType($1);
         lefrPin.lefiPin::setProperty($1, $2, propTp);
      }
    }

electrical_direction:
  K_DIRECTION K_INPUT ';'         {$$ = (char*)"INPUT";}
  | K_DIRECTION K_OUTPUT ';'            {$$ = (char*)"OUTPUT";}
  | K_DIRECTION K_OUTPUT K_TRISTATE ';' {$$ = (char*)"OUTPUT TRISTATE";}
  | K_DIRECTION K_INOUT  ';'            {$$ = (char*)"INOUT";}
  | K_DIRECTION K_FEEDTHRU ';'          {$$ = (char*)"FEEDTHRU";}

start_macro_port: K_PORT
    {
      if (lefrPinCbk) {
	lefrDoGeometries = 1;
	lefrGeometriesPtr = (lefiGeometries*)lefMalloc( sizeof(lefiGeometries));
	lefrGeometriesPtr->lefiGeometries::Init();
      }
    }

macro_port_class_option: /* empty */
  | K_CLASS class_type ';'
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addClass($2); }

macro_pin_use:
  K_SIGNAL	{$$ = (char*)"SIGNAL";}
  | K_ANALOG 	{$$ = (char*)"ANALOG";}
  | K_POWER 	{$$ = (char*)"POWER";}
  | K_GROUND	{$$ = (char*)"GROUND";}
  | K_CLOCK	{$$ = (char*)"CLOCK";}
  | K_DATA	{$$ = (char*)"DATA";}

macro_scan_use:
  K_INPUT {$$ = (char*)"INPUT";}
  | K_OUTPUT	{$$ = (char*)"OUTPUT";}
  | K_START	{$$ = (char*)"START";}
  | K_STOP	{$$ = (char*)"STOP";}

pin_shape:
  /* empty */   {$$ = (char*)""; }      /* non-ring shape */
  | K_ABUTMENT	{$$ = (char*)"ABUTMENT";}
  | K_RING 	{$$ = (char*)"RING";}
  | K_FEEDTHRU	{$$ = (char*)"FEEDTHRU";}

geometries: geometry geometry_options

geometry:
  K_LAYER {lefDumbMode = 1; lefNoNum = 1; } T_STRING
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addLayer($3);
    }
  layer_spacing ';'
  | K_WIDTH NUMBER ';'
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addWidth($2); }
  | K_PATH firstPt otherPts ';'
    { if (lefrDoGeometries) lefrGeometriesPtr->lefiGeometries::addPath(); }
  | K_PATH K_ITERATE firstPt otherPts stepPattern ';'
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addPathIter(); }
  | K_RECT pt pt ';'
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addRect($2.x, $2.y, $3.x, $3.y); }
  | K_RECT K_ITERATE pt pt stepPattern ';'
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addRectIter($3.x, $3.y, $4.x, $4.y);
    }
  | K_POLYGON firstPt nextPt nextPt nextPt otherPts ';'
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addPolygon(); }
  | K_POLYGON K_ITERATE firstPt nextPt nextPt nextPt otherPts stepPattern ';'
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addPolygonIter(); }
  | via_placement
    { }

geometry_options: /* empty */
  | geometry_options geometry

layer_spacing: /* empty */
  | K_SPACING NUMBER
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addLayerMinSpacing($2); }
  | K_DESIGNRULEWIDTH NUMBER
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addLayerRuleWidth($2); }

firstPt: pt  
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::startList($1.x, $1.y); }

nextPt:  pt
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addToList($1.x, $1.y); }

otherPts:
  /* empty */
  | nextPt otherPts
  ;

via_placement:
  K_VIA pt {lefDumbMode = 1;} T_STRING ';'
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addVia($2.x, $2.y, $4); }
  | K_VIA K_ITERATE pt {lefDumbMode = 1; lefNoNum = 1;} T_STRING
    stepPattern ';'
    { if (lefrDoGeometries)
        lefrGeometriesPtr->lefiGeometries::addViaIter($3.x, $3.y, $5); }
        

stepPattern: K_DO NUMBER K_BY NUMBER K_STEP NUMBER NUMBER
     { if (lefrDoGeometries)
         lefrGeometriesPtr->lefiGeometries::addStepPattern($2, $4, $6, $7); }

sitePattern: T_STRING NUMBER NUMBER orientation
  K_DO NUMBER K_BY NUMBER K_STEP NUMBER NUMBER
    {
      if (lefrDoSite) {
	lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
				   sizeof(lefiSitePattern));
	lefrSitePatternPtr->lefiSitePattern::Init();
	lefrSitePatternPtr->lefiSitePattern::set($1, $2, $3, $4, $6, $8,
	  $10, $11);
	}
    }

trackPattern:
  K_X NUMBER K_DO NUMBER K_STEP NUMBER 
    { 
      if (lefrDoTrack) {
	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
				sizeof(lefiTrackPattern));
	lefrTrackPatternPtr->lefiTrackPattern::Init();
	lefrTrackPatternPtr->lefiTrackPattern::set("X", $2, (int)$4, $6);
      }    
    }
    K_LAYER {lefDumbMode = 1000000000;} trackLayers
    { lefDumbMode = 0;}
  | K_Y NUMBER K_DO NUMBER K_STEP NUMBER
    { 
      if (lefrDoTrack) {
	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
                                    sizeof(lefiTrackPattern));
	lefrTrackPatternPtr->lefiTrackPattern::Init();
	lefrTrackPatternPtr->lefiTrackPattern::set("Y", $2, (int)$4, $6);
      }    
    }
    K_LAYER {lefDumbMode = 1000000000;} trackLayers
    { lefDumbMode = 0;}
  | K_X NUMBER K_DO NUMBER K_STEP NUMBER 
    { 
      if (lefrDoTrack) {
	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
                                    sizeof(lefiTrackPattern));
	lefrTrackPatternPtr->lefiTrackPattern::Init();
	lefrTrackPatternPtr->lefiTrackPattern::set("X", $2, (int)$4, $6);
      }    
    }
  | K_Y NUMBER K_DO NUMBER K_STEP NUMBER
    { 
      if (lefrDoTrack) {
	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
                                    sizeof(lefiTrackPattern));
	lefrTrackPatternPtr->lefiTrackPattern::Init();
	lefrTrackPatternPtr->lefiTrackPattern::set("Y", $2, (int)$4, $6);
      }    
    }

trackLayers:
  /* empty */
  | trackLayers layer_name
  ;

layer_name: T_STRING
    { if (lefrDoTrack) lefrTrackPatternPtr->lefiTrackPattern::addLayer($1); }

gcellPattern: K_X NUMBER K_DO NUMBER K_STEP NUMBER
    {
      if (lefrDoGcell) {
	lefrGcellPatternPtr = (lefiGcellPattern*)lefMalloc(
                                    sizeof(lefiGcellPattern));
	lefrGcellPatternPtr->lefiGcellPattern::Init();
	lefrGcellPatternPtr->lefiGcellPattern::set("X", $2, (int)$4, $6);
      }    
    }
  | K_Y NUMBER K_DO NUMBER K_STEP NUMBER
    {
      if (lefrDoGcell) {
	lefrGcellPatternPtr = (lefiGcellPattern*)lefMalloc(
                                    sizeof(lefiGcellPattern));
	lefrGcellPatternPtr->lefiGcellPattern::Init();
	lefrGcellPatternPtr->lefiGcellPattern::set("Y", $2, (int)$4, $6);
      }    
    }

macro_obs: start_macro_obs geometries K_END
    { 
      if (lefrObstructionCbk) {
	lefrObstruction.lefiObstruction::setGeometries(lefrGeometriesPtr);
	lefrGeometriesPtr = 0;
	lefrDoGeometries = 0;
      }
      CALLBACK(lefrObstructionCbk, lefrObstructionCbkType, &lefrObstruction);
      lefDumbMode = 0;
    }
  | start_macro_obs K_END
    /* 08/14/00 - Wanda da Rosa.  Allow empty OBS due to so many lef files
     * have empty OBS
    {
       yyerror("OBS requires either a LAYER or a VIA.");
       CHKERR();
    }
    */

start_macro_obs: K_OBS
    {
      obsDef = 1;
      if (lefrObstructionCbk) {
	lefrDoGeometries = 1;
	lefrGeometriesPtr = (lefiGeometries*)lefMalloc(
	    sizeof(lefiGeometries));
	lefrGeometriesPtr->lefiGeometries::Init();
	}
    }

macro_clocktype: K_CLOCKTYPE { lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
    { if (lefrMacroCbk) lefrMacro.lefiMacro::setClockType($3); }

timing: start_timing timing_options end_timing
    { }

start_timing: K_TIMING 
    { /* XXXXX for macros */ }

end_timing: K_END K_TIMING 
  {
    if (versionNum < 5.4) {
      if (lefrTimingCbk && lefrTiming.lefiTiming::hasData())
        CALLBACK(lefrTimingCbk, lefrTimingCbkType, &lefrTiming);
      lefrTiming.lefiTiming::clear();
    } else {
      yywarning("MACRO TIMING is obsolete in 5.4. It will be ignored.");
      lefrTiming.lefiTiming::clear();
    }
  }

timing_options:
  /* empty */
  | timing_options timing_option
  ;

timing_option:
  K_FROMPIN 
    {
    if (versionNum < 5.4) {
      if (lefrTimingCbk && lefrTiming.lefiTiming::hasData())
        CALLBACK(lefrTimingCbk, lefrTimingCbkType, &lefrTiming);
    }
    lefDumbMode = 1000000000;
    lefrTiming.lefiTiming::clear();
    }
    list_of_from_strings ';'
    { lefDumbMode = 0;}
  | K_TOPIN {lefDumbMode = 1000000000;} list_of_to_strings ';'
    { lefDumbMode = 0;}
  | risefall K_INTRINSIC NUMBER NUMBER
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFall($1,$3,$4); }
    slew_spec K_VARIABLE NUMBER NUMBER ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFallVariable($8,$9); }
  | risefall delay_or_transition K_UNATENESS unateness
    K_TABLEDIMENSION NUMBER NUMBER NUMBER ';' 
    { if (lefrTimingCbk) {
	if ($2[0] == 'D' || $2[0] == 'd') /* delay */
	  lefrTiming.lefiTiming::addDelay($1, $4, $6, $7, $8);
	else
	  lefrTiming.lefiTiming::addTransition($1, $4, $6, $7, $8);
	}
    }
  | K_TABLEAXIS list_of_table_axis_numbers ';'
    { }
  | K_TABLEENTRIES list_of_table_entries ';'
    { }
  | K_RISERS NUMBER NUMBER ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setRiseRS($2,$3); }
  | K_FALLRS NUMBER NUMBER ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setFallRS($2,$3); }
  | K_RISECS NUMBER NUMBER ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setRiseCS($2,$3); }
  | K_FALLCS NUMBER NUMBER ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setFallCS($2,$3); }
  | K_RISESATT1 NUMBER NUMBER ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setRiseAtt1($2,$3); }
  | K_FALLSATT1 NUMBER NUMBER ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setFallAtt1($2,$3); }
  | K_RISET0 NUMBER NUMBER ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setRiseTo($2,$3); }
  | K_FALLT0 NUMBER NUMBER ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setFallTo($2,$3); }
  | K_UNATENESS unateness ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addUnateness($2); }
  | K_STABLE K_SETUP NUMBER K_HOLD NUMBER risefall ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setStable($3,$5,$6); }
  | two_pin_trigger from_pin_trigger to_pin_trigger K_TABLEDIMENSION NUMBER NUMBER NUMBER ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addSDF2Pins($1,$2,$3,$5,$6,$7); }
  | one_pin_trigger K_TABLEDIMENSION NUMBER NUMBER NUMBER ';' 
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addSDF1Pin($1,$3,$4,$4); }
  | K_SDFCONDSTART QSTRING ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setSDFcondStart($2); }
  | K_SDFCONDEND QSTRING ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setSDFcondEnd($2); }
  | K_SDFCOND QSTRING ';'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::setSDFcond($2); }
  | K_EXTENSION ';'
    { /* XXXXX */ }

one_pin_trigger:
  K_MPWH
    { $$ = (char*)"MPWH";}
  | K_MPWL
    { $$ = (char*)"MPWL";}
  | K_PERIOD
    { $$ = (char*)"PERIOD";}

two_pin_trigger :
  K_SETUP
    { $$ = (char*)"SETUP";}
  | K_HOLD
    { $$ = (char*)"HOLD";}
  | K_RECOVERY
    { $$ = (char*)"RECOVERY";}
  | K_SKEW
    { $$ = (char*)"SKEW";}

from_pin_trigger:
  K_ANYEDGE
    { $$ = (char*)"ANYEDGE";}
  | K_POSEDGE
    { $$ = (char*)"POSEDGE";}
  | K_NEGEDGE 
    { $$ = (char*)"NEGEDGE";}

to_pin_trigger:
  K_ANYEDGE
    { $$ = (char*)"ANYEDGE";}
  | K_POSEDGE
    { $$ = (char*)"POSEDGE";}
  | K_NEGEDGE 
    { $$ = (char*)"NEGEDGE";}

delay_or_transition :
  K_DELAY
    { $$ = (char*)"DELAY"; }
  | K_TRANSITIONTIME
    { $$ = (char*)"TRANSITION"; }

list_of_table_entries:
  table_entry
    { }
  | list_of_table_entries table_entry
    { }

table_entry: '(' NUMBER NUMBER NUMBER ')'
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addTableEntry($2,$3,$4); }

list_of_table_axis_numbers:
  NUMBER
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addTableAxisNumber($1); }
  | list_of_table_axis_numbers NUMBER
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addTableAxisNumber($2); }

slew_spec:
  /* empty */
    { }
  | NUMBER NUMBER NUMBER NUMBER 
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFallSlew($1,$2,$3,$4); }
  |  NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER 
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFallSlew($1,$2,$3,$4);
      if (lefrTimingCbk) lefrTiming.lefiTiming::addRiseFallSlew2($5,$6,$7); }

risefall:
  K_RISE
    { $$ = (char*)"RISE"; }
  | K_FALL 
    { $$ = (char*)"FALL"; }

unateness:
  K_INVERT
    { $$ = (char*)"INVERT"; }
  | K_NONINVERT
    { $$ = (char*)"NONINVERT"; }
  | K_NONUNATE 
    { $$ = (char*)"NONUNATE"; }

list_of_from_strings:
  T_STRING
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addFromPin($1); }
  | list_of_from_strings T_STRING 
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addFromPin($2); }

list_of_to_strings:
  T_STRING
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addToPin($1); }
  | list_of_to_strings T_STRING 
    { if (lefrTimingCbk) lefrTiming.lefiTiming::addToPin($2); }

array: start_array array_rules
    {
	CALLBACK(lefrArrayCbk, lefrArrayCbkType, &lefrArray);
	lefrArray.lefiArray::clear();
	lefrSitePatternPtr = 0;
	lefrDoSite = 0;
    }
    end_array

start_array: K_ARRAY {lefDumbMode = 1; lefNoNum = 1;} T_STRING
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::setName($3);
	CALLBACK(lefrArrayBeginCbk, lefrArrayBeginCbkType, $3);
      }
      strcpy(arrayName, $3);
    }

end_array: K_END {lefDumbMode = 1; lefNoNum = 1;} T_STRING
    {
      if (lefrArrayCbk)
	CALLBACK(lefrArrayEndCbk, lefrArrayEndCbkType, $3);
      if (strcmp(arrayName, $3) != 0) {
        yyerror("END ARRAY name is different from ARRAY");
        CHKERR();
      }
    }

array_rules:
  /* empty */
    { }
  | array_rules array_rule
    { }

array_rule:
  site_word { if (lefrArrayCbk) lefrDoSite = 1; lefDumbMode = 1; }
    sitePattern  ';'
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addSitePattern(lefrSitePatternPtr);
      }
    }
  | K_CANPLACE {lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; }
    sitePattern ';'
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addCanPlace(lefrSitePatternPtr);
      }
    }
  | K_CANNOTOCCUPY {lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; }
    sitePattern ';'
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addCannotOccupy(lefrSitePatternPtr);
      }
    }
  | K_TRACKS { if (lefrArrayCbk) lefrDoTrack = 1; } trackPattern ';'
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addTrack(lefrTrackPatternPtr);
      }
    }
  | floorplan_start floorplan_list K_END T_STRING 
    {
    }
  | K_GCELLGRID { if (lefrArrayCbk) lefrDoGcell = 1; } gcellPattern ';'
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::addGcell(lefrGcellPatternPtr);
      }
    }
  | K_DEFAULTCAP NUMBER cap_list K_END K_DEFAULTCAP
    {
      if (lefrArrayCbk) {
	lefrArray.lefiArray::setTableSize((int)$2);
      }
    }
  | def_statement
    { }

floorplan_start: K_FLOORPLAN T_STRING
    { if (lefrArrayCbk) lefrArray.lefiArray::addFloorPlan($2); }
	
floorplan_list:
  /* empty */
    { }
  | floorplan_list floorplan_element
    { }

floorplan_element:
  K_CANPLACE { lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; }
    sitePattern ';'
    {
      if (lefrArrayCbk)
	lefrArray.lefiArray::addSiteToFloorPlan("CANPLACE",
	lefrSitePatternPtr);
    }
  | K_CANNOTOCCUPY { if (lefrArrayCbk) lefrDoSite = 1; lefDumbMode = 1; }
    sitePattern ';'
    {
      if (lefrArrayCbk)
	lefrArray.lefiArray::addSiteToFloorPlan("CANNOTOCCUPY",
	lefrSitePatternPtr);
     }

cap_list:
  /* empty */
    { }
  | cap_list one_cap
    { }

one_cap: K_MINPINS NUMBER K_WIRECAP NUMBER ';'
    { if (lefrArrayCbk) lefrArray.lefiArray::addDefaultCap((int)$2, $4); }

msg_statement:
  K_MESSAGE {lefDumbMode=1;lefNlToken=TRUE;} T_STRING '=' s_expr dtrm
    { lefAddStringMessage($3, $5); }

create_file_statement:
  K_CREATEFILE {lefDumbMode=1;lefNlToken=TRUE;} T_STRING '=' s_expr dtrm
    { }

def_statement:
  K_DEFINE {lefDumbMode=1;lefNlToken=TRUE;} T_STRING '=' expression dtrm
    { lefAddNumDefine($3, $5); }
  |  K_DEFINES {lefDumbMode=1;lefNlToken=TRUE;} T_STRING '=' s_expr dtrm
    { lefAddStringDefine($3, $5); }
  |  K_DEFINEB {lefDumbMode=1;lefNlToken=TRUE;} T_STRING '=' b_expr dtrm
    { lefAddBooleanDefine($3, $5); }

/* terminator for &defines.  Can be semicolon or newline */
dtrm:
  |  ';' {lefNlToken = FALSE;}
  |  '\n'	{lefNlToken = FALSE;}

then:
  K_THEN
  | '\n' K_THEN
  ;

else:
  K_ELSE
  | '\n' K_ELSE
  ;

expression:
  expression '+' expression   {$$ = $1 + $3; }
  | expression '-' expression   {$$ = $1 - $3; }
  | expression '*' expression   {$$ = $1 * $3; }
  | expression '/' expression   {$$ = $1 / $3; }
  | '-' expression %prec UMINUS {$$ = -$2;}
  | '(' expression ')'		{$$ = $2;}
  | K_IF b_expr then expression else expression %prec IF
		{$$ = ($2 != 0) ? $4 : $6;}
  | NUMBER			{$$ = $1;}

b_expr:
  expression relop expression {$$ = comp_num($1,$2,$3);}
  | expression K_AND expression {$$ = $1 != 0 && $3 != 0;}
  | expression K_OR  expression {$$ = $1 != 0 || $3 != 0;}
  | s_expr relop s_expr	      {$$ = comp_str($1,$2,$3);}
  | s_expr K_AND s_expr	      {$$ = $1[0] != 0 && $3[0] != 0;}
  | s_expr K_OR  s_expr	      {$$ = $1[0] != 0 || $3[0] != 0;}
  | b_expr K_EQ b_expr	      {$$ = $1 == $3;}
  | b_expr K_NE b_expr	      {$$ = $1 != $3;}
  | b_expr K_AND b_expr	      {$$ = $1 && $3;}
  | b_expr K_OR  b_expr	      {$$ = $1 || $3;}
  | K_NOT b_expr		      %prec LNOT {$$ = !$$;}
  | '(' b_expr ')'	      {$$ = $2;}
  | K_IF b_expr then b_expr else b_expr %prec IF
	  {$$ = ($2 != 0) ? $4 : $6;}
  | K_TRUE		      {$$ = 1;}
  | K_FALSE		      {$$ = 0;}

s_expr:
  s_expr '+' s_expr
    {
      $$ = (char*)lefMalloc(strlen($1)+strlen($3)+1);
      strcpy($$,$1);
      strcat($$,$3);
    }
  | '(' s_expr ')'
    { $$ = $2; }
  | K_IF b_expr then s_expr else s_expr %prec IF
    {
      lefDefIf = TRUE;
      if ($2 != 0) {
	$$ = $4;	
      } else {
	$$ = $6;
      }
    }
  | QSTRING
    { $$ = $1; }

relop:
  K_LE {$$ = C_LE;}
  | K_LT {$$ = C_LT;}
  | K_GE {$$ = C_GE;}
  | K_GT {$$ = C_GT;}
  | K_EQ {$$ = C_EQ;}
  | K_NE {$$ = C_NE;}
  | '='  {$$ = C_EQ;}
  | '<'  {$$ = C_LT;}
  | '>'  {$$ = C_GT;}


prop_def_section: K_PROPDEF
    { CALLBACK(lefrPropBeginCbk, lefrPropBeginCbkType, 0); }
    prop_stmts K_END K_PROPDEF
    { 
      CALLBACK(lefrPropEndCbk, lefrPropEndCbkType, 0);
      lefRealNum = 0;     /* just want to make sure it is reset */
    }

prop_stmts:
  /* empty */
    { }
  | prop_stmts prop_stmt
    { }

prop_stmt:
  K_LIBRARY {lefDumbMode = 1; lefrProp.lefiProp::clear(); }
    T_STRING prop_define ';'
    { 
      if (lefrPropCbk) lefrProp.lefiProp::setPropType("library", $3);
      CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      lefrLibProp.lefiPropType::setPropType($3, lefPropDefType);
    }
  | K_COMPONENTPIN {lefDumbMode = 1; lefrProp.lefiProp::clear(); }
    T_STRING prop_define ';'
    { 
      if (lefrPropCbk) lefrProp.lefiProp::setPropType("componentpin", $3);
      CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      lefrCompProp.lefiPropType::setPropType($3, lefPropDefType);
    }
  | K_PIN {lefDumbMode = 1; lefrProp.lefiProp::clear(); }
    T_STRING prop_define ';'
    { 
      if (lefrPropCbk) lefrProp.lefiProp::setPropType("pin", $3);
      CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      lefrPinProp.lefiPropType::setPropType($3, lefPropDefType);
      
    }
  | K_MACRO {lefDumbMode = 1; lefrProp.lefiProp::clear(); }
    T_STRING prop_define ';'
    { 
      if (lefrPropCbk) lefrProp.lefiProp::setPropType("macro", $3);
      CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      lefrMacroProp.lefiPropType::setPropType($3, lefPropDefType);
    }
  | K_VIA {lefDumbMode = 1; lefrProp.lefiProp::clear(); }
    T_STRING prop_define ';'
    { 
      if (lefrPropCbk) lefrProp.lefiProp::setPropType("via", $3);
      CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      lefrViaProp.lefiPropType::setPropType($3, lefPropDefType);
    }
  | K_VIARULE {lefDumbMode = 1; lefrProp.lefiProp::clear(); }
    T_STRING prop_define ';'
    { 
      if (lefrPropCbk) lefrProp.lefiProp::setPropType("viarule", $3);
      CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      lefrViaRuleProp.lefiPropType::setPropType($3, lefPropDefType);
    }
  | K_LAYER {lefDumbMode = 1; lefrProp.lefiProp::clear(); }
    T_STRING prop_define ';'
    { 
      if (lefrPropCbk) lefrProp.lefiProp::setPropType("layer", $3);
      CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      lefrLayerProp.lefiPropType::setPropType($3, lefPropDefType);
    }
  | K_NONDEFAULTRULE {lefDumbMode = 1; lefrProp.lefiProp::clear(); }
    T_STRING prop_define ';'
    { 
      if (lefrPropCbk) lefrProp.lefiProp::setPropType("nondefaultrule", $3);
      CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
      lefrNondefProp.lefiPropType::setPropType($3, lefPropDefType);
    }
    
prop_define:
  K_INTEGER { lefRealNum = 0 } opt_def_range opt_def_value 
    { 
      if (lefrPropCbk) lefrProp.lefiProp::setPropInteger();
      lefPropDefType = 'I'
    }
  | K_REAL { lefRealNum = 1 } opt_def_range opt_def_value
    { 
      if (lefrPropCbk) lefrProp.lefiProp::setPropReal();
      lefPropDefType = 'R';
      lefRealNum = 0;
    }
  | K_STRING
    {
      if (lefrPropCbk) lefrProp.lefiProp::setPropString();
      lefPropDefType = 'S'
    }
  | K_STRING QSTRING
    {
      if (lefrPropCbk) lefrProp.lefiProp::setPropQString($2);
      lefPropDefType = 'Q'
    }
  | K_NAMEMAPSTRING T_STRING
    {
      if (lefrPropCbk) lefrProp.lefiProp::setPropNameMapString($2);
      lefPropDefType = 'S'
    }


opt_range_detail:
  /* empty */
    { 
    }
  | K_RANGE NUMBER NUMBER
    { 
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingRange($2, $3);
    }
    opt_range_second
  | K_LENGTHTHRESHOLD NUMBER
    {
      if (lefrLayerCbk) {
        lefrLayer.lefiLayer::setSpacingLength($2);
      }
    }
  | K_LENGTHTHRESHOLD NUMBER K_RANGE NUMBER NUMBER
    {
      if (lefrLayerCbk) {
        lefrLayer.lefiLayer::setSpacingLength($2);
        lefrLayer.lefiLayer::setSpacingLengthRange($4, $5);
      }
    }

opt_range_second:
  /* nothing */
    { }
  | K_USELENGTHTHRESHOLD
    {
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingRangeUseLength();
    }
  | K_INFLUENCE NUMBER
    {
      if (lefrLayerCbk) {
        lefrLayer.lefiLayer::setSpacingRangeInfluence($2);
        lefrLayer.lefiLayer::setSpacingRangeInfluenceRange(-1, -1);
      }
    }
  | K_INFLUENCE NUMBER K_RANGE NUMBER NUMBER
    {
      if (lefrLayerCbk) {
        lefrLayer.lefiLayer::setSpacingRangeInfluence($2);
        lefrLayer.lefiLayer::setSpacingRangeInfluenceRange($4, $5);
      }
    }
  | K_RANGE NUMBER NUMBER
    {
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingRangeRange($2, $3);
    }

opt_def_range:
  /* nothing */
    { }
  | K_RANGE NUMBER NUMBER
    { lefrProp.lefiProp::setRange($2, $3); }

opt_def_value:
  /* empty */
    { }
  | NUMBER
    { if (lefrPropCbk) lefrProp.lefiProp::setNumber($1); }

opt_spacing_cut:
  /* empty */
  | K_LAYER {lefDumbMode = 1; lefNoNum = 1; } T_STRING
    {
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingName($3);
    }
  | K_ADJACENTCUTS NUMBER K_WITHIN NUMBER
    {
      if (versionNum < 5.5) {
         yyerror("ADJACENTCUTS is a 5.5 syntax. Your lef file is not defined as 5.5");
         CHKERR();
      }
      if (lefrLayerCbk)
        lefrLayer.lefiLayer::setSpacingAdjacent($2, $4);
    }

opt_layer_name:
  /* empty */
    { $$ = 0; }
  | K_LAYER {lefDumbMode = 1; lefNoNum = 1; } T_STRING
    { $$ = $3; }

req_layer_name:
  /* pcr 355313 */
   K_LAYER {lefDumbMode = 1; lefNoNum = 1; } T_STRING
    { $$ = $3; }

/* 9/11/2001 - Wanda da Rosa.  The following are obsolete in 5.4 */
universalnoisemargin: K_UNIVERSALNOISEMARGIN NUMBER NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrNoiseMarginCbk) {
          lefrNoiseMargin.low = $2;
          lefrNoiseMargin.high = $3;
        }
        CALLBACK(lefrNoiseMarginCbk, lefrNoiseMarginCbkType, &lefrNoiseMargin);
      } else
        yywarning(
          "UNIVERSALNOISEMARGIN is obsolete in 5.4. It will be ignored.");
    }

edgeratethreshold1: K_EDGERATETHRESHOLD1 NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrEdgeRateThreshold1Cbk) {
          CALLBACK(lefrEdgeRateThreshold1Cbk,
          lefrEdgeRateThreshold1CbkType, $2);
        }
      } else
        yywarning("EDGERATETHRESHOLD1 is obsolete in 5.4. It will be ignored.");
    }

edgeratethreshold2: K_EDGERATETHRESHOLD2 NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrEdgeRateThreshold2Cbk) {
	  CALLBACK(lefrEdgeRateThreshold2Cbk,
	  lefrEdgeRateThreshold2CbkType, $2);
        }
      } else
        yywarning("EDGERATETHRESHOLD2 is obsolete in 5.4. It will be ignored.");
    }

edgeratescalefactor: K_EDGERATESCALEFACTOR NUMBER ';'
    {
      if (versionNum < 5.4) {
        if (lefrEdgeRateScaleFactorCbk) {
	  CALLBACK(lefrEdgeRateScaleFactorCbk,
	  lefrEdgeRateScaleFactorCbkType, $2);
        }
      } else
        yywarning(
          "EDGERATESCALEFACTOR is obsolete in 5.4. It will be ignored.");
    }

noisetable: K_NOISETABLE NUMBER
    { if (lefrNoiseTableCbk) lefrNoiseTable.lefiNoiseTable::setup((int)$2); }
    ';' noise_table_list end_noisetable dtrm
    { }

end_noisetable:
  K_END K_NOISETABLE
  {
    if (versionNum < 5.4) {
      CALLBACK(lefrNoiseTableCbk, lefrNoiseTableCbkType, &lefrNoiseTable);
    } else
      yywarning("NOISETABLE is obsolete in 5.4. It will be ignored.");
  }


noise_table_list :
  noise_table_entry
  | noise_table_list noise_table_entry
  ;

noise_table_entry:
  K_EDGERATE NUMBER ';'
    { if (lefrNoiseTableCbk)
         {
            lefrNoiseTable.lefiNoiseTable::newEdge();
            lefrNoiseTable.lefiNoiseTable::addEdge($2);
         }
    }
  | output_resistance_entry
    { }

output_resistance_entry: K_OUTPUTRESISTANCE
    { if (lefrNoiseTableCbk) lefrNoiseTable.lefiNoiseTable::addResistance(); }
    num_list ';' victim_list
    ;

num_list:
  NUMBER
    { if (lefrNoiseTableCbk)
    lefrNoiseTable.lefiNoiseTable::addResistanceNumber($1); }
   | num_list NUMBER
    { if (lefrNoiseTableCbk)
    lefrNoiseTable.lefiNoiseTable::addResistanceNumber($2); }

victim_list:
  victim
  | victim_list victim
  ;

victim: K_VICTIMLENGTH NUMBER ';'
	{ if (lefrNoiseTableCbk)
	lefrNoiseTable.lefiNoiseTable::addVictimLength($2); }
      K_VICTIMNOISE vnoiselist ';'
	{ }

vnoiselist:
  NUMBER
    { if (lefrNoiseTableCbk)
    lefrNoiseTable.lefiNoiseTable::addVictimNoise($1); }
  | vnoiselist NUMBER
    { if (lefrNoiseTableCbk)
    lefrNoiseTable.lefiNoiseTable::addVictimNoise($2); }

correctiontable: K_CORRECTIONTABLE NUMBER ';'
    { if (lefrCorrectionTableCbk)
    lefrCorrectionTable.lefiCorrectionTable::setup((int)$2); }
    correction_table_list end_correctiontable dtrm
    { }

end_correctiontable:
  K_END K_CORRECTIONTABLE
  {
    if (versionNum < 5.4) {
      CALLBACK(lefrCorrectionTableCbk, lefrCorrectionTableCbkType,
               &lefrCorrectionTable);
    } else
      yywarning("CORRECTIONTABLE is obsolete in 5.4. It will be ignored.");
  }

correction_table_list:
  correction_table_item
  | correction_table_list correction_table_item
  ;

correction_table_item:
  K_EDGERATE NUMBER ';'
    { if (lefrCorrectionTableCbk)
         {
            lefrCorrectionTable.lefiCorrectionTable::newEdge();
            lefrCorrectionTable.lefiCorrectionTable::addEdge($2);
         }
    }
  | output_list
    { }

output_list: K_OUTPUTRESISTANCE
  { if (lefrCorrectionTableCbk)
  lefrCorrectionTable.lefiCorrectionTable::addResistance(); }
  numo_list ';' corr_victim_list
  { }

numo_list:
  NUMBER
    { if (lefrCorrectionTableCbk)
    lefrCorrectionTable.lefiCorrectionTable::addResistanceNumber($1); }
  | numo_list NUMBER
    { if (lefrCorrectionTableCbk)
    lefrCorrectionTable.lefiCorrectionTable::addResistanceNumber($2); }

corr_victim_list:
   corr_victim
   | corr_victim_list corr_victim
   ;

corr_victim:
  K_VICTIMLENGTH NUMBER ';'
     { if (lefrCorrectionTableCbk)
     lefrCorrectionTable.lefiCorrectionTable::addVictimLength($2); }
  K_CORRECTIONFACTOR corr_list ';'
     { }

corr_list:
  NUMBER
    { if (lefrCorrectionTableCbk)
	lefrCorrectionTable.lefiCorrectionTable::addVictimCorrection($1); }
  | corr_list NUMBER
    { if (lefrCorrectionTableCbk)
	lefrCorrectionTable.lefiCorrectionTable::addVictimCorrection($2); }

/* end of 5.4 obsolete syntax */

input_antenna: K_INPUTPINANTENNASIZE NUMBER ';'
    { /* 5.3 syntax */
        use5_3 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum > 5.3) {
           /* A 5.3 syntax in 5.4 */
           if (use5_4) {
              yyerror("INPUTPINANTENNASIZE is a 5.3 or earlier syntax.  Your lef file has both old and new syntax, which is illegal.");
              CHKERR();
           }
        }
        CALLBACK(lefrInputAntennaCbk, lefrInputAntennaCbkType, $2);
    }

output_antenna: K_OUTPUTPINANTENNASIZE NUMBER ';'
    { /* 5.3 syntax */
        use5_3 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum > 5.3) {
           /* A 5.3 syntax in 5.4 */
           if (use5_4) {
              yyerror("OUTPUTPINANTENNASIZE is a 5.3 or earlier syntax.  Your lef file has both old and new syntax, which is illegal.");
              CHKERR();
           }
        }
        CALLBACK(lefrOutputAntennaCbk, lefrOutputAntennaCbkType, $2);
    }

inout_antenna: K_INOUTPINANTENNASIZE NUMBER ';'
    { /* 5.3 syntax */
        use5_3 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum > 5.3) {
           /* A 5.3 syntax in 5.4 */
           if (use5_4) {
              yyerror("INOUTPINANTENNASIZE is a 5.3 or earlier syntax.  Your lef file has both old and new syntax, which is illegal.");
              CHKERR();
           }
        }
        CALLBACK(lefrInoutAntennaCbk, lefrInoutAntennaCbkType, $2);
    }

antenna_input: K_ANTENNAINPUTGATEAREA NUMBER ';'
    { /* 5.4 syntax */
        /* 11/12/2002 - this is obsoleted in 5.5, suppose should be ingored */
        /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/
        use5_4 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum < 5.4) {
           yyerror("ANTENNAINPUTGATEAREA is a 5.4 syntax. Please either update your lef VERSION number or use the 5.3 syntax.");
           CHKERR();
        } else if (use5_3) {
           yyerror("ANTENNAINPUTGATEAREA is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal.");
           CHKERR();
        }
        CALLBACK(lefrAntennaInputCbk, lefrAntennaInputCbkType, $2);
    }

antenna_inout: K_ANTENNAINOUTDIFFAREA NUMBER ';'
    { /* 5.4 syntax */
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/
        use5_4 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum < 5.4) {
              yyerror("ANTENNAINOUTDIFFAREA is a 5.4 syntax.  Please either update your lef VERSION number or use the 5.3 syntax.");
              CHKERR();
        } else if (use5_3) {
           yyerror("ANTENNAINOUTDIFFAREA is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal.");
           CHKERR();
        }
        CALLBACK(lefrAntennaInoutCbk, lefrAntennaInoutCbkType, $2);
    }

antenna_output: K_ANTENNAOUTPUTDIFFAREA NUMBER ';'
    { /* 5.4 syntax */
        /* 11/12/2002 - this is obsoleted in 5.5, & will be ignored */
        /* 12/16/2002 - talked to Dave Noice, leave them in here for debugging*/
        use5_4 = 1;
        if (ignoreVersion) {
           /* do nothing */
        } else if (versionNum < 5.4) {
              yyerror("ANTENNAOUTPUTDIFFAREA is a 5.4 syntax.  Please either update your lef VERSION number or use the 5.3 syntax.");
              CHKERR();
        } else if (use5_3) {
           yyerror("ANTENNAOUTPUTDIFFAREA is a 5.4 syntax.  Your lef file has both old and new syntax, which is illegal.");
           CHKERR();
        }
        CALLBACK(lefrAntennaOutputCbk, lefrAntennaOutputCbkType, $2);
    }

/*
extension: K_BEGINEXT { lefDumbMode = 1000000; } T_STRING
           extension_stmts K_ENDEXT
    { }

extension_stmts: * empty *
    | extension_stmts extension_stmt
    ;

extension_stmt: T_STRING
    { }
*/

extension: K_BEGINEXT
    { }

%%
