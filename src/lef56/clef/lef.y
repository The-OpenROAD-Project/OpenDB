/* 
 *     This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
  *  Distribution,  Product Version 5.6, and is subject to the Cadence
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

 #include "clef/FEF_stringhash.h"
 #include "clef/lex.h"
 #include "clef/lefiDefs.h"
 #include "clef/lefiUser.h"
 #include "clef/lefiUtil.h"
 #include "clef/lefrCallBacks.h"
 #ifdef WIN32
 #include <malloc.h>
 #endif
  
 #define LYPROP_ECAP "EDGE_CAPACITANCE"
 
 #define YYINITDEPTH 5000   /* pcr 640902 - initialize the yystacksize to 300 */
 /* this may need to increase in a design gets */

 /* larger and a polygon has around 300 sizes */

 /* 11/21/2003 - incrreased to 500, design from */

 /* Artisan is greater than 300, need to find a */

 /* way to dynamically increase the size */

 /* 2/10/2004 - increased to 1000 for pcr 686073 */

 /* 3/22/2004 - increased to 2000 for pcr 695879 */

 /* 9/29/2004 - double the size for pcr 746865 */

 /* tried to overwrite the yyoverflow definition */

 /* it is impossible due to the union structure */

 
 /* 
#define YYMAXDEPTH 20000
 */

 
 int lefRetVal;
 static char lefPropDefType; /* save the current type of the property */

 
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
 
 /* *********************************************************************/
/* *********************************************************************/

 #define C_EQ 0
 #define C_NE 1
 #define C_LT 2
 #define C_LE 3
 #define C_GT 4
 #define C_GE 5
 
 extern int lefDumbMode; /* for communicating with parser
 */
extern int lefNoNum; /* likewise, says no numbers for the next N tokens
 */
extern int lefRealNum; /* Next token will be a real number
 */
extern int lefNlToken; /* likewise
  */
extern int lefDefIf; /* likewise
  */
extern int lefNdRule; /* for communicating with parser
 */

 extern int lef_errors; /* from lex.cpph
 */
extern char* Hist_text; /* for BEGINEXT - extension
 */

 extern int doneLib; /* keep track if the file is done parsing
 */
extern int ge56almostDone; /* lef file has Extension section
 */
extern int ge56done; /* done reading 5.6 and beyond library
 */

 extern char* lefrFileName;
 
 static int ignoreVersion = 0; /* ignore checking version number
 */

 int lefNamesCaseSensitive = TRUE; /* always true in 5.6
 */
int lefReaderCaseSensitive = FALSE; /* default to 0
 */

 int lefrShiftCase = 0; /* if user wants to shift to all uppercase
 */

 int lefrRelaxMode = FALSE; /* relax mode?
 */

 /* XXXXXXXXXXXXX check out these variables */

 double lef_save_x, lef_save_y; /* for interpreting (*) notation of LEF/DEF
 */

 /* #define STRING_LIST_SIZE 1024 */
 /* char string_list[STRING_LIST_SIZE]; */

 
 static char temp_name[258];
 static char* layerName;
 static char* viaName;
 static char* viaRuleName;
 static char* nonDefaultRuleName;
 static char* siteName;
 static char* arrayName;
 static char* macroName;
 static char* pinName;
 
 static int siteDef, symDef, sizeDef, pinDef, obsDef, origDef;
 static int useLenThr;
 static int layerCut, layerMastOver, layerRout, layerDir;
 static lefiAntennaEnum antennaType = lefiAntennaAR; /* 5.4 - antenna type */

 static int viaRuleLayer; /* keep track number of layer in a viarule */

 static int viaLayer; /* keep track number of layer in a via */

 static int ndLayer; /* keep track number of layer in a nondefault */

 static int numVia; /* keep track number of via */

 static int viaRuleHasDir; /* viarule layer has direction construct */

 static int viaRuleHasEnc; /* viarule layer has enclosure construct */

 static int ndRule = 0; /* keep track if inside nondefaultrule */

 static int ndLayerWidth; /* keep track if width is set at ndLayer */

 static int ndLayerSpace; /* keep track if spacing is set at ndLayer */

 static int isGenerate; /* keep track if viarule has generate */

 static int hasType; /* keep track of type in layer */

 static int hasPitch; /* keep track of pitch in layer */

 static int hasWidth; /* keep track of width in layer */

 static int hasDirection; /* keep track of direction in layer */

 static int hasParallel; /* keep track of parallelrunlength */

 static int hasInfluence; /* keep track of influence */

 static int hasLayerMincut; /* keep track of layer minimumcut */

 static int hasManufactur = 0; /* keep track of manufacture is after unit */

 static int hasPRP; /* keep track if path, rect or poly is def */

 static int needGeometry; /* keep track if path/rect/poly is defined */

 static int hasViaRule_layer = 0; /* keep track at least viarule or layer */

 static int hasSamenet; /* keep track if samenet is defined in spacing */

 static int hasSiteClass = 0; /* keep track if SITE has CLASS */

 static int hasSiteSize = 0; /* keep track if SITE has SIZE */

 
 /* the following variables to keep track the number of warnings printed.
 */
static int antennaInoutWarnings = 0;
 static int antennaInputWarnings = 0;
 static int antennaOutputWarnings = 0;
 static int arrayWarnings = 0;
 static int caseSensitiveWarnings = 0;
 static int correctionTableWarnings = 0;
 static int dielectricWarnings = 0;
 static int edgeRateThreshold1Warnings = 0;
 static int edgeRateThreshold2Warnings = 0;
 static int edgeRateScaleFactorWarnings = 0;
 static int inoutAntennaWarnings = 0;
 static int inputAntennaWarnings = 0;
 static int iRDropWarnings = 0;
 static int layerWarnings = 0;
 static int macroWarnings = 0;
 static int maxStackViaWarnings = 0;
 static int minFeatureWarnings = 0;
 static int noiseMarginWarnings = 0;
 static int noiseTableWarnings = 0;
 static int nonDefaultWarnings = 0;
 static int noWireExtensionWarnings = 0;
 static int outputAntennaWarnings = 0;
 static int pinWarnings = 0;
 static int siteWarnings = 0;
 static int spacingWarnings = 0;
 static int timingWarnings = 0;
 static int unitsWarnings = 0;
 static int useMinSpacingWarnings = 0;
 static int viaRuleWarnings = 0;
 static int viaWarnings = 0;
 
 int spParallelLength; /* the number of layer parallelrunlength */

 
 lefiNum macroNum; /* for origin & size callback */

 
 int comp_str(char *s1, int op, char *s2)
 {
 int k = strcmp(s1, s2);
 switch (op) {
 	case C_EQ: return k == 0;
 	case C_NE: return k != 0;
 	case C_GT: return k > 0;
 	case C_GE: return k >= 0;
 	case C_LT: return k < 0;
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
 	case C_GT: return k > 0;
 	case C_GE: return k >= 0;
 	case C_LT: return k < 0;
 	case C_LE: return k <= 0;
 	}
 return 0;
 }
 /* 
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
 */

 
 int validNum(int values) {
 char *outMsg;
 switch (values) {
 case 100:
 case 200:
 case 1000:
 case 2000:
 return 1;
 case 10000:
 case 20000:
 if (versionNum < 5.6) {
 if (lefrUnitsCbk) {
 if (unitsWarnings++ < lefrUnitsWarnings) {
 outMsg = (char*)lefMalloc(10000);
 sprintf (outMsg,
 "Error found when processing LEF file '%s'\nUnit %d is a 5.6 or later syntax\nYour lef file is not defined as 5.6",
 lefrFileName, values);
 yyerror(outMsg);
 lefFree(outMsg);
 }
 }
 return 0;
 } else
 return 1;
 }
 if (unitsWarnings++ < lefrUnitsWarnings)
 yyerror("Illegal value for LEF UNITS DATABASE MICRONS");
 CHKERR();
 return 0;
 }
 
 int zeroOrGt(double values) {
 if (values < 0)
 return 0;
 return 1;
 }
 
 %}
 
 %union {
 	double dval ;
 	int integer ;
 	char * string ;
 	lefPOINT pt;
 }
 
 %token <string> K_HISTORY
 %token K_ABUT K_ABUTMENT K_ACTIVE K_ANALOG K_ARRAY K_AREA
 %token K_BLOCK K_BOTTOMLEFT K_BOTTOMRIGHT
 %token K_BY K_CAPACITANCE K_CAPMULTIPLIER K_CLASS K_CLOCK K_CLOCKTYPE
 %token K_COLUMNMAJOR K_DESIGNRULEWIDTH K_INFLUENCE
 %token K_CORE K_CORNER K_COVER K_CPERSQDIST K_CURRENT
 %token K_CURRENTSOURCE K_CUT K_DEFAULT K_DATABASE K_DATA
 %token K_DIELECTRIC K_DIRECTION K_DO K_EDGECAPACITANCE
 %token K_EEQ K_END K_ENDCAP K_FALL K_FALLCS K_FALLT0 K_FALLSATT1
 %token K_FALLRS K_FALLSATCUR K_FALLTHRESH K_FEEDTHRU K_FIXED K_FOREIGN K_FROMPIN
 %token K_GENERATE K_GENERATOR K_GROUND K_HEIGHT K_HORIZONTAL K_INOUT K_INPUT
 %token K_INPUTNOISEMARGIN K_COMPONENTPIN
 %token K_INTRINSIC K_INVERT K_IRDROP K_ITERATE K_IV_TABLES K_LAYER K_LEAKAGE
 %token K_LEQ K_LIBRARY K_MACRO K_MATCH K_MAXDELAY K_MAXLOAD K_METALOVERHANG K_MILLIAMPS
 %token K_MILLIWATTS K_MINFEATURE K_MUSTJOIN K_NAMESCASESENSITIVE K_NANOSECONDS
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
 %token K_VIA K_VIARULE K_VLO K_VOLTAGE K_VOLTS K_WIDTH K_X K_Y
 %token <string> T_STRING QSTRING
 %token <dval> NUMBER
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
 %token K_NOISETABLE K_EDGERATE K_VICTIMLENGTH
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
 %token K_ABOVE K_BELOW K_CENTERTOCENTER K_CUTSIZE K_CUTSPACING K_DENSITY
 %token K_DIAG45 K_DIAG135
 %token K_DIAGMINEDGELENGTH K_DIAGSPACING K_DIAGPITCH K_DIAGWIDTH
 %token K_GENERATED K_GROUNDSENSITIVITY K_HARDSPACING K_INSIDECORNER
 %token K_LAYERS K_LENGTHSUM K_MICRONS K_MINCUTS
 %token K_MINSIZE K_NETEXPR K_OUTSIDECORNER
 %token K_PREFERENCLOSURE K_ROWCOL K_ROWPATTERN K_SOFT
 %token K_SUPPLYSENSITIVITY K_USEVIA
 %token K_USEVIARULE K_WELLTAP
 
 %type <string> start_macro end_macro
 %type <string> start_layer
 %type <string> macro_pin_use
 %type <string> macro_scan_use
 %type <string> pin_shape
 %type <string> pad_type core_type endcap_type class_type site_class
 %type <string> start_foreign spacing_type clearance_type
 %type <pt> pt
 %type <pt> macro_origin
 %type <string> layer_type layer_direction
 %type <string> electrical_direction
 %type <integer> orientation
 %type <dval> expression
 %type <integer> b_expr
 %type <string> s_expr
 %type <integer> relop spacing_value
 %type <string> opt_layer_name risefall unateness delay_or_transition
 %type <string> two_pin_trigger from_pin_trigger to_pin_trigger
 %type <string> one_pin_trigger req_layer_name
 %type <string> layer_table_type layer_enclosure_type_opt layer_minstep_type
 
 %nonassoc IF
 %left K_AND
 %left K_OR
 %left K_LE K_EQ K_LT K_NE K_GE K_GT
 %nonassoc LNOT
 %left '-' '+'
 %left '*' '/'
 %nonassoc UMINUS
 
 %%
 
 lef_file: rules extension_opt end_library
 {
 /* 11/16/2001 - Wanda da Rosa - pcr 408334
 */
/* Return 1 if there are errors
 */
if (lef_errors)
 return 1;
 if (!hasVer)
 yywarning("VERSION is a required statement.");
 /* pre 5.6, 5.6 it is obsoleted
 */
if (!hasNameCase && versionNum < 5.6)
 yywarning("NAMESCASESENSITIVE is a required statement.");
 if (!hasBusBit && versionNum < 5.6)
 yywarning("BUSBITCHARS is a required statement.");
 if (!hasDivChar && versionNum < 5.6)
 yywarning("DIVIDERCHAR is a required statement.");
 hasVer = 0;
 hasNameCase = 0;
 hasBusBit = 0;
 hasDivChar = 0;
 hasManufactur = 0;
 antennaInoutWarnings = 0;
 antennaInputWarnings = 0;
 antennaOutputWarnings = 0;
 arrayWarnings = 0;
 caseSensitiveWarnings = 0;
 correctionTableWarnings = 0;
 dielectricWarnings = 0;
 edgeRateThreshold1Warnings = 0;
 edgeRateThreshold2Warnings = 0;
 edgeRateScaleFactorWarnings = 0;
 inoutAntennaWarnings = 0;
 inputAntennaWarnings = 0;
 iRDropWarnings = 0;
 layerWarnings = 0;
 macroWarnings = 0;
 maxStackViaWarnings = 0;
 minFeatureWarnings = 0;
 noiseMarginWarnings = 0;
 noiseTableWarnings = 0;
 nonDefaultWarnings = 0;
 noWireExtensionWarnings = 0;
 outputAntennaWarnings = 0;
 siteWarnings = 0;
 spacingWarnings = 0;
 timingWarnings = 0;
 unitsWarnings = 0;
 useMinSpacingWarnings = 0;
 viaRuleWarnings = 0;
 viaWarnings = 0;
 }
 
 version: K_VERSION { lefDumbMode = 1; lefNoNum = 1;} T_STRING ';'
 {
 versionNum = convert_name2num($3);
 if (versionNum > 5.6) {
 char temp[120];
 sprintf(temp,
 "Lef parser 5.6 does not support lef file with version %s. Parser stops executions",
 $3);
 yyerror(temp);
 return 1;
 }
 /* 
versionNum = $3; Save the version number for future use */

 if (lefrVersionStrCbk) {
 CALLBACK(lefrVersionStrCbk, lefrVersionStrCbkType, $3);
 } else {
 if (lefrVersionCbk)
 CALLBACK(lefrVersionCbk, lefrVersionCbkType, versionNum);
 }
 if (versionNum > 5.3 && versionNum < 5.4) {
 ignoreVersion = 1;
 }
 use5_3 = use5_4 = 0;
 lef_errors = 0;
 hasVer = 1;
 if (versionNum < 5.6) {
 doneLib = 0;
 lefNamesCaseSensitive = lefReaderCaseSensitive;
 } else {
 doneLib = 1;
 lefNamesCaseSensitive = 1;
 }
 }
 
 
 dividerchar: K_DIVIDERCHAR QSTRING ';'
 {
 if (lefrDividerCharCbk)
 CALLBACK(lefrDividerCharCbk, lefrDividerCharCbkType, $2);
 hasDivChar = 1;
 }
 
 busbitchars: K_BUSBITCHARS QSTRING ';'
 {
 if (lefrBusBitCharsCbk)
 CALLBACK(lefrBusBitCharsCbk, lefrBusBitCharsCbkType, $2);
 hasBusBit = 1;
 }
 
 rules: /* empty */

 	| rules rule
 	| error
 	 { }
 
 end_library: /* empty */

 {
 if (versionNum >= 5.6) {
 doneLib = 1;
 ge56done = 1;
 }
 }
 | K_END K_LIBRARY
 {
 doneLib = 1;
 ge56done = 1;
 if (lefrLibraryEndCbk)
 CALLBACK(lefrLibraryEndCbk, lefrLibraryEndCbkType, 0);
 /* 11/16/2001 - Wanda da Rosa - pcr 408334
 */
/* Return 1 if there are errors
 */
/* 
if (lef_errors)
 return 1;
 if (!hasVer)
 yywarning("VERSION is a required statement.");
 if (!hasNameCase)
 yywarning("NAMESCASESENSITIVE is a required statement.");
 if (!hasBusBit && versionNum < 5.6)
 yywarning("BUSBITCHARS is a required statement.");
 if (!hasDivChar && versionNum < 5.6)
 yywarning("DIVIDERCHAR is a required statement.");
 hasVer = 0;
 hasNameCase = 0;
 hasBusBit = 0;
 hasDivChar = 0;
 hasManufactur = 0;
 */

 }
 
 rule: version | busbitchars | case_sensitivity | units_section
 | layer_rule | via | viarule | viarule_generate | dividerchar
 | wireextension | msg_statement
 | spacing_rule | dielectric | minfeature | irdrop | site | macro | array
 | def_statement | nondefault_rule | prop_def_section
 | universalnoisemargin | edgeratethreshold1
 | edgeratescalefactor | edgeratethreshold2
 | noisetable | correctiontable | input_antenna
 | output_antenna | inout_antenna
 | antenna_input | antenna_inout | antenna_output | manufacturing
 | useminspacing | clearancemeasure | maxstack_via
 | create_file_statement
 	;
 
 case_sensitivity: K_NAMESCASESENSITIVE K_ON ';'
 	 {
 if (versionNum < 5.6) {
 lefNamesCaseSensitive = TRUE;
 if (lefrCaseSensitiveCbk)
 CALLBACK(lefrCaseSensitiveCbk, lefrCaseSensitiveCbkType,
 			 lefNamesCaseSensitive);
 hasNameCase = 1;
 } else
 if (lefrCaseSensitiveCbk) /* write warning only if cbk is set */

 if (caseSensitiveWarnings++ < lefrCaseSensitiveWarnings)
 yywarning("NAMESCASESENSITIVE is obsolete in 5.6. It will be ignored.");
 	 }
 | K_NAMESCASESENSITIVE K_OFF ';'
 	 {
 if (versionNum < 5.6) {
 	 lefNamesCaseSensitive = FALSE;
 if (lefrCaseSensitiveCbk)
 	 CALLBACK(lefrCaseSensitiveCbk, lefrCaseSensitiveCbkType,
 			 lefNamesCaseSensitive);
 hasNameCase = 1;
 } else {
 if (lefrCaseSensitiveCbk) { /* write error only if cbk is set */

 if (caseSensitiveWarnings++ < lefrCaseSensitiveWarnings) {
 yyerror("NAMESCASESENSITIVE cannot be set to OFF");
 CHKERR();
 }
 }
 }
 	 }
 
 wireextension: K_NOWIREEXTENSIONATPIN K_ON ';'
 {
 if (versionNum < 5.6) {
 if (lefrNoWireExtensionCbk)
 CALLBACK(lefrNoWireExtensionCbk, lefrNoWireExtensionCbkType, "ON");
 } else
 if (lefrNoWireExtensionCbk) /* write warning only if cbk is set */

 if (noWireExtensionWarnings++ < lefrNoWireExtensionWarnings)
 yywarning("NOWIREEXTENSIONATPIN is obsolete in 5.6. It will be ignored.");
 }
 | K_NOWIREEXTENSIONATPIN K_OFF ';'
 {
 if (versionNum < 5.6) {
 if (lefrNoWireExtensionCbk)
 CALLBACK(lefrNoWireExtensionCbk, lefrNoWireExtensionCbkType, "OFF");
 } else
 if (lefrNoWireExtensionCbk) /* write warning only if cbk is set */

 if (noWireExtensionWarnings++ < lefrNoWireExtensionWarnings)
 yywarning("NOWIREEXTENSIONATPIN is obsolete in 5.6. It will be ignored.");
 }
 
 manufacturing: K_MANUFACTURINGGRID NUMBER ';'
 {
 if (lefrManufacturingCbk)
 CALLBACK(lefrManufacturingCbk, lefrManufacturingCbkType, $2);
 hasManufactur = 1;
 }
 
 useminspacing: K_USEMINSPACING spacing_type spacing_value ';'
 {
 if ((strcmp($2, "PIN") == 0) && (versionNum >= 5.6)) {
 if (lefrUseMinSpacingCbk) /* write warning only if cbk is set */

 if (useMinSpacingWarnings++ < lefrUseMinSpacingWarnings)
 yywarning("USEMINSPACING PIN is obsolete in 5.6. It will be ignored.");
 } else {
 if (lefrUseMinSpacingCbk) {
 lefiUseMinSpacing_set( &(lefrUseMinSpacing),
 $2,
$3) ;
 CALLBACK(lefrUseMinSpacingCbk, lefrUseMinSpacingCbkType,
 &lefrUseMinSpacing);
 }
 }
 }
 
 clearancemeasure: K_CLEARANCEMEASURE clearance_type ';'
 { CALLBACK(lefrClearanceMeasureCbk, lefrClearanceMeasureCbkType, $2); }
 
 clearance_type:
 K_MAXXY {$$ = (char*)"MAXXY";}
 | K_EUCLIDEAN {$$ = (char*)"EUCLIDEAN";}
 
 spacing_type:
 K_OBS {$$ = (char*)"OBS";}
 | K_PIN {$$ = (char*)"PIN";}
 
 spacing_value:
 K_ON {$$ = 1;}
 | K_OFF {$$ = 0;}
 
 units_section: start_units units_rules K_END K_UNITS
 {
 if (lefrUnitsCbk)
 CALLBACK(lefrUnitsCbk, lefrUnitsCbkType, &lefrUnits);
 }
 
 start_units: K_UNITS
 {
 lefiUnits_clear( &(lefrUnits) ) ;
 if (hasManufactur) {
 if (unitsWarnings++ < lefrUnitsWarnings) {
 yyerror("MANUFACTURINGGRID has defined before UNITS");
 CHKERR();
 }
 }
 }
 
 units_rules: /* empty */

 | units_rules units_rule
 ;
 
 units_rule: K_TIME K_NANOSECONDS NUMBER ';'
 { if (lefrUnitsCbk) lefiUnits_setTime( &(lefrUnits),
 $3) ;
 }
 | K_CAPACITANCE K_PICOFARADS NUMBER ';'
 { if (lefrUnitsCbk) lefiUnits_setCapacitance( &(lefrUnits),
 $3) ;
 }
 | K_RESISTANCE K_OHMS NUMBER ';'
 { if (lefrUnitsCbk) lefiUnits_setResistance( &(lefrUnits),
 $3) ;
 }
 | K_POWER K_MILLIWATTS NUMBER ';'
 { if (lefrUnitsCbk) lefiUnits_setPower( &(lefrUnits),
 $3) ;
 }
 | K_CURRENT K_MILLIAMPS NUMBER ';'
 { if (lefrUnitsCbk) lefiUnits_setCurrent( &(lefrUnits),
 $3) ;
 }
 | K_VOLTAGE K_VOLTS NUMBER ';'
 { if (lefrUnitsCbk) lefiUnits_setVoltage( &(lefrUnits),
 $3) ;
 }
 | K_DATABASE K_MICRONS NUMBER ';'
 {
 if(validNum((int)$3)) {
 if (lefrUnitsCbk)
 lefiUnits_setDatabase( &(lefrUnits),
 "MICRONS",
$3) ;
 }
 }
 | K_FREQUENCY K_MEGAHERTZ NUMBER ';'
 { if (lefrUnitsCbk) lefiUnits_setFrequency( &(lefrUnits),
 $3) ;
 }
 
 layer_rule: start_layer
 layer_options end_layer
 {
 if (lefrLayerCbk)
 CALLBACK(lefrLayerCbk, lefrLayerCbkType, &lefrLayer);
 }
 
 start_layer: K_LAYER {lefDumbMode = 1; lefNoNum = 1; } T_STRING
 {
 if (lefrHasMaxVS) { /* 5.5 */

 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("A MAXVIASTACK has defined before the LAYER");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk)
 lefiLayer_setName( &(lefrLayer),
 $3) ;
 useLenThr = 0;
 layerCut = 0;
 layerMastOver = 0;
 layerRout = 0;
 layerDir = 0;
 lefrHasLayer = 1;
 /* $3);
  */
layerName = strdup($3);
 hasType = 0;
 hasPitch = 0;
 hasWidth = 0;
 hasDirection = 0;
 hasParallel = 0;
 hasInfluence = 0;
 lefrHasSpacingTbl = 0;
 lefrHasSpacing = 0;
 }
 
 end_layer: K_END {lefDumbMode = 1; lefNoNum = 1; } T_STRING
 {
 if (strcmp(layerName, $3) != 0) {
 lefFree(layerName);
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("END LAYER name is different from LAYER");
 CHKERR();
 }
 }
 } else
 lefFree(layerName);
 if (!lefrRelaxMode) {
 if (hasType == 0) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("TYPE in LAYER is required");
 CHKERR();
 }
 }
 }
 if ((layerRout == 1) && (hasPitch == 0)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("PITCH in LAYER with type ROUTING is required");
 CHKERR();
 }
 }
 }
 if ((layerRout == 1) && (hasWidth == 0)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("WIDTH in LAYER with type ROUTING is required");
 CHKERR();
 }
 }
 }
 if ((layerRout == 1) && (hasDirection == 0)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("DIRECTION in LAYER with type ROUTING is required");
 CHKERR();
 }
 }
 }
 }
 }
 
 layer_options:
 /* empty */

 { }
 /* WANDA
 | layer_options layer_option
 */

 | layer_option layer_options
 { }
 
 layer_option:
 K_TYPE layer_type ';'
 {
 if (lefrLayerCbk)
 lefiLayer_setType( &(lefrLayer),
 $2) ;
 hasType = 1;
 }
 | K_PITCH NUMBER ';'
 {
 if (lefrLayerCbk) lefiLayer_setPitch( &(lefrLayer),
 $2) ;
 hasPitch = 1;
 }
 | K_PITCH NUMBER NUMBER ';'
 {
 if (lefrLayerCbk) lefiLayer_setPitchXY( &(lefrLayer),
 $2,
$3) ;
 hasPitch = 1;
 }
 | K_DIAGPITCH NUMBER ';'
 {
 if (lefrLayerCbk) lefiLayer_setDiagPitch( &(lefrLayer),
 $2) ;
 }
 | K_DIAGPITCH NUMBER NUMBER ';'
 {
 if (lefrLayerCbk) lefiLayer_setDiagPitchXY( &(lefrLayer),
 $2,
$3) ;
 }
 | K_OFFSET NUMBER ';'
 {
 if (lefrLayerCbk) lefiLayer_setOffset( &(lefrLayer),
 $2) ;
 }
 | K_OFFSET NUMBER NUMBER ';'
 {
 if (lefrLayerCbk) lefiLayer_setOffsetXY( &(lefrLayer),
 $2,
$3) ;
 }
 | K_DIAGWIDTH NUMBER ';'
 {
 if (lefrLayerCbk) lefiLayer_setDiagWidth( &(lefrLayer),
 $2) ;
 }
 | K_DIAGSPACING NUMBER ';'
 {
 if (lefrLayerCbk) lefiLayer_setDiagSpacing( &(lefrLayer),
 $2) ;
 }
 | K_WIDTH NUMBER ';' /* CUT & ROUTING
 */
{
 if (lefrLayerCbk) lefiLayer_setWidth( &(lefrLayer),
 $2) ;
 hasWidth = 1;
 }
 | K_AREA NUMBER ';'
 {
 if (lefrLayerCbk) lefiLayer_setArea( &(lefrLayer),
 $2) ;
 }
 | K_SPACING NUMBER
 {
 /* 11/22/99 - Wanda da Rosa, PCR 283762
 */
/*            Issue an error is this is defined in masterslice
 */
if (layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("SPACING can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
 CHKERR();
 }
 }
 }
 /* 5.5 either SPACING or SPACINGTABLE, not both for routing layer only
 */
if (layerRout) {
 if (lefrHasSpacingTbl) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yywarning("It is illegal to mix both SPACING rules & SPACINGTABLE rules for the same lef file in ROUTING layer");
 }
 }
 }
 if (lefrLayerCbk)
 lefiLayer_setSpacingMin( &(lefrLayer),
 $2) ;
 lefrHasSpacing = 1;
 } else {
 if (lefrLayerCbk)
 lefiLayer_setSpacingMin( &(lefrLayer),
 $2) ;
 }
 }
 opt_range_detail opt_spacing_cut ';'
 | K_DIRECTION layer_direction ';'
 {
 layerDir = 1;
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("DIRECTION can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setDirection( &(lefrLayer),
 $2) ;
 hasDirection = 1;
 }
 | K_RESISTANCE K_RPERSQ NUMBER ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("RESISTANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setResistance( &(lefrLayer),
 $3) ;
 }
 | K_RESISTANCE K_RPERSQ K_PWL '(' res_points ')' ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("RESISTANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 }
 | K_CAPACITANCE K_CPERSQDIST NUMBER ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("CAPACITANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setCapacitance( &(lefrLayer),
 $3) ;
 }
 | K_CAPACITANCE K_CPERSQDIST K_PWL '(' cap_points ')' ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("CAPACITANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 }
 | K_HEIGHT NUMBER ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("HEIGHT can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setHeight( &(lefrLayer),
 $2) ;
 }
 | K_WIREEXTENSION NUMBER ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("WIREEXTENSION can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setWireExtension( &(lefrLayer),
 $2) ;
 }
 | K_THICKNESS NUMBER ';'
 {
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("THICKNESS can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setThickness( &(lefrLayer),
 $2) ;
 }
 | K_SHRINKAGE NUMBER ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("SHRINKAGE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setShrinkage( &(lefrLayer),
 $2) ;
 }
 | K_CAPMULTIPLIER NUMBER ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("CAPMULTIPLIER can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setCapMultiplier( &(lefrLayer),
 $2) ;
 }
 | K_EDGECAPACITANCE NUMBER ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("EDGECAPACITANCE can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setEdgeCap( &(lefrLayer),
 $2) ;
 }
 /* 3/24/2000 - Wanda da Rosa. Same call for 5.4 but more options.
 Syntax is handle 1 page down.
 | K_ANTENNAAREAFACTOR NUMBER ';'
 {
 if (!layerRout && (layerCut || layerMastOver)) {
 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNAAREAFACTOR can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaArea( &(lefrLayer),
 $2) ;
 }
 */

 | K_ANTENNALENGTHFACTOR NUMBER ';'
 { /* 5.3 syntax */

 use5_3 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNALENGTHFACTOR can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 } else if (versionNum >= 5.4) {
 if (use5_4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNALENGTHFACTOR is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 }
 
 if (lefrLayerCbk) lefiLayer_setAntennaLength( &(lefrLayer),
 $2) ;
 }
 | K_CURRENTDEN NUMBER ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("CURRENTDEN can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setCurrentDensity( &(lefrLayer),
 $2) ;
 }
 | K_CURRENTDEN K_PWL '(' current_density_pwl_list ')' ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("CURRENTDEN can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 }
 | K_CURRENTDEN '(' NUMBER NUMBER ')' ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("CURRENTDEN can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setCurrentPoint( &(lefrLayer),
 $3,
$4) ;
 }
 | K_PROPERTY { lefDumbMode = 10000000; lefRealNum = 1; } layer_prop_list ';'
 {
 lefDumbMode = 0;
 lefRealNum = 0;
 }
 | K_ACCURRENTDENSITY layer_table_type
 {
 if (layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ACCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addAccurrentDensity( &(lefrLayer),
 $2) ;
 }
 layer_frequency
 | K_ACCURRENTDENSITY layer_table_type NUMBER ';'
 {
 if (layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ACCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) {
 lefiLayer_addAccurrentDensity( &(lefrLayer),
 $2) ;
 lefiLayer_setAcOneEntry( &(lefrLayer),
 $3) ;
 }
 }
 | K_DCCURRENTDENSITY K_AVERAGE NUMBER ';'
 {
 if (layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("DCCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) {
 lefiLayer_addDccurrentDensity( &(lefrLayer),
 "AVERAGE") ;
 lefiLayer_setDcOneEntry( &(lefrLayer),
 $3) ;
 }
 }
 | K_DCCURRENTDENSITY K_AVERAGE K_CUTAREA NUMBER
 {
 if (layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("DCCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (!layerCut) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("CUTAREA can only be defined in LAYER with type CUT");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) {
 lefiLayer_addDccurrentDensity( &(lefrLayer),
 "AVERAGE") ;
 lefiLayer_addNumber( &(lefrLayer),
 $4) ;
 }
 }
 number_list ';'
 { if (lefrLayerCbk) lefiLayer_addDcCutarea( &(lefrLayer) ) ;
 }
 dc_layer_table
 | K_DCCURRENTDENSITY K_AVERAGE K_WIDTH NUMBER
 {
 if (layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("DCCURRENTDENSITY can't be defined in LAYER with TYPE MASTERSLICE or OVERLAP. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("WIDTH can only be defined in LAYER with type ROUTING");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) {
 lefiLayer_addDccurrentDensity( &(lefrLayer),
 "AVERAGE") ;
 lefiLayer_addNumber( &(lefrLayer),
 $4) ;
 }
 }
 number_list ';'
 { if (lefrLayerCbk) lefiLayer_addDcWidth( &(lefrLayer) ) ;
 }
 dc_layer_table
 
 /* 3/23/2000 - 5.4 syntax. Wanda da Rosa */

 | K_ANTENNAAREARATIO NUMBER ';'
 { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNAAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNAAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (!layerRout && !layerCut && layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNAAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaAreaRatio( &(lefrLayer),
 $2) ;
 }
 | K_ANTENNADIFFAREARATIO
 { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNADIFFAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNADIFFAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (!layerRout && !layerCut && layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNADIFFAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
 CHKERR();
 }
 }
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
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNACUMAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNACUMAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (!layerRout && !layerCut && layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNACUMAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaCumAreaRatio( &(lefrLayer),
 $2) ;
 }
 | K_ANTENNACUMDIFFAREARATIO
 { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNACUMDIFFAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNACUMDIFFAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (!layerRout && !layerCut && layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNACUMDIFFAREARATIO can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
 CHKERR();
 }
 }
 }
 antennaType = lefiAntennaCDAR;
 }
 layer_antenna_pwl ';'
 | K_ANTENNAAREAFACTOR NUMBER
 { /* both 5.3 & 5.4 syntax */

 if (!layerRout && !layerCut && layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNAAREAFACTOR can only be defined in LAYER with TYPE ROUTING or CUT. Parser stops executions");
 CHKERR();
 }
 }
 }
 /* this does not need to check, since syntax is in both 5.3 & 5.4 */

 if (lefrLayerCbk) lefiLayer_setAntennaAreaFactor( &(lefrLayer),
 $2) ;
 antennaType = lefiAntennaAF;
 }
 layer_antenna_duo ';'
 | K_ANTENNASIDEAREARATIO NUMBER ';'
 { /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNASIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNASIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNASIDEAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaSideAreaRatio( &(lefrLayer),
 $2) ;
 }
 | K_ANTENNADIFFSIDEAREARATIO
 { /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNADIFFSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNADIFFSIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNADIFFSIDEAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 antennaType = lefiAntennaDSAR;
 }
 layer_antenna_pwl ';'
 | K_ANTENNACUMSIDEAREARATIO NUMBER ';'
 { /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNACUMSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNACUMSIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNACUMSIDEAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaCumSideAreaRatio( &(lefrLayer),
 $2) ;
 }
 | K_ANTENNACUMDIFFSIDEAREARATIO
 { /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNACUMDIFFSIDEAREARATIO can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNACUMDIFFSIDEAREARATIO is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNACUMDIFFSIDEAREARATIO is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 antennaType = lefiAntennaCDSAR;
 }
 layer_antenna_pwl ';'
 | K_ANTENNASIDEAREAFACTOR NUMBER
 { /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && (layerCut || layerMastOver)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNASIDEAREAFACTOR can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNASIDEAREAFACTOR is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNASIDEAREAFACTOR is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setAntennaSideAreaFactor( &(lefrLayer),
 $2) ;
 antennaType = lefiAntennaSAF;
 }
 layer_antenna_duo ';'
 | K_ANTENNAMODEL /* 5.5 */

 { /* 5.4 syntax */

 use5_4 = 1;
 if (!layerRout && !layerCut && layerMastOver) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNAMODEL can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 antennaType = lefiAntennaO;
 }
 layer_oxide ';'
 | K_SLOTWIREWIDTH NUMBER ';'
 { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("SLOTWIREWIDTH is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setSlotWireWidth( &(lefrLayer),
 $2) ;
 }
 | K_SLOTWIRELENGTH NUMBER ';'
 { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("SLOTWIRELENGTH is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setSlotWireLength( &(lefrLayer),
 $2) ;
 }
 | K_SLOTWIDTH NUMBER ';'
 { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("SLOTWIDTH is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setSlotWidth( &(lefrLayer),
 $2) ;
 }
 | K_SLOTLENGTH NUMBER ';'
 { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("SLOTLENGTH is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setSlotLength( &(lefrLayer),
 $2) ;
 }
 | K_MAXADJACENTSLOTSPACING NUMBER ';'
 { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("MAXADJACENTSLOTSPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setMaxAdjacentSlotSpacing( &(lefrLayer),
 $2) ;
 }
 | K_MAXCOAXIALSLOTSPACING NUMBER ';'
 { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("MAXCOAXIALSLOTSPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setMaxCoaxialSlotSpacing( &(lefrLayer),
 $2) ;
 }
 | K_MAXEDGESLOTSPACING NUMBER ';'
 { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("MAXEDGESLOTSPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setMaxEdgeSlotSpacing( &(lefrLayer),
 $2) ;
 }
 | K_SPLITWIREWIDTH NUMBER ';'
 { /* 5.4 syntax */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("SPLITWIREWIDTH is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setSplitWireWidth( &(lefrLayer),
 $2) ;
 }
 | K_MINIMUMDENSITY NUMBER ';'
 { /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("MINIMUMDENSITY is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setMinimumDensity( &(lefrLayer),
 $2) ;
 }
 | K_MAXIMUMDENSITY NUMBER ';'
 { /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("MAXIMUMDENSITY is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setMaximumDensity( &(lefrLayer),
 $2) ;
 }
 | K_DENSITYCHECKWINDOW NUMBER NUMBER ';'
 { /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("DENSITYCHECKWINDOW is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setDensityCheckWindow( &(lefrLayer),
 $2,
$3) ;
 }
 | K_DENSITYCHECKSTEP NUMBER ';'
 { /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("DENSITYCHECKSTEP is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setDensityCheckStep( &(lefrLayer),
 $2) ;
 }
 | K_FILLACTIVESPACING NUMBER ';'
 { /* 5.4 syntax, pcr 394389 */

 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("FILLACTIVESPACING is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setFillActiveSpacing( &(lefrLayer),
 $2) ;
 }
 | K_MAXWIDTH NUMBER ';' /* 5.5 */

 {
 /* 5.5 MAXWIDTH, is for routing layer only
 */
if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("MAXWIDTH can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("MAXWIDTH is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setMaxwidth( &(lefrLayer),
 $2) ;
 }
 | K_MINWIDTH NUMBER ';' /* 5.5 */

 {
 /* 5.5 MINWIDTH, is for routing layer only
 */
if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("MINWIDTH can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 }
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("MINWIDTH is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setMinwidth( &(lefrLayer),
 $2) ;
 }
 | K_MINENCLOSEDAREA NUMBER /* 5.5 */

 {
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("MINENCLOSEDAREA is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addMinenclosedarea( &(lefrLayer),
 $2) ;
 }
 layer_minen_width ';'
 | K_MINIMUMCUT NUMBER K_WIDTH NUMBER /* 5.5 */

 { /* pcr 409334 */

 if (lefrLayerCbk)
 lefiLayer_addMinimumcut( &(lefrLayer),
 (int)$2,$4) ;
 hasLayerMincut = 0;
 }
 layer_minimumcut_options ';'
 {
 if (!hasLayerMincut) { /* FROMABOVE nor FROMBELOW is set */

 if (lefrLayerCbk)
 lefiLayer_addMinimumcutConnect( &(lefrLayer),
 (char*)"") ;
 }
 }
 | K_MINSTEP NUMBER /* 5.5 */

 {
 if (lefrLayerCbk) lefiLayer_addMinstep( &(lefrLayer),
 $2) ;
 }
 layer_minstep_options ';' /* 5.6 */

 {
 }
 | K_PROTRUSIONWIDTH NUMBER K_LENGTH NUMBER K_WIDTH NUMBER ';' /* 5.5 */

 {
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("PROTRUSION RULE is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_setProtrusion( &(lefrLayer),
 $2,
$4,
$6) ;
 }
 | K_SPACINGTABLE /* 5.5 */

 {
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("SPACINGTABLE is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 }
 /* 5.5 either SPACING or SPACINGTABLE in a layer, not both
 */
if (lefrHasSpacing && layerRout) {
 if (lefrLayerCbk) /* write warning only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yywarning("It is illegal to mix both SPACING rules & SPACINGTABLE rules for the same lef file in ROUTING layer");
 }
 }
 if (lefrLayerCbk) lefiLayer_addSpacingTable( &(lefrLayer) ) ;
 lefrHasSpacingTbl = 1;
 }
 sp_options ';'
 /* 10/12/2003 - 5.6 syntax */

 | K_ENCLOSURE layer_enclosure_type_opt NUMBER NUMBER
 {
 if (versionNum < 5.6) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ENCLOSURE is a 5.6 syntax. Your lef file is not defined as 5.6");
 CHKERR();
 }
 }
 } else {
 if (lefrLayerCbk)
 lefiLayer_addEnclosure( &(lefrLayer),
 $2,
$3,
$4) ;
 }
 }
 layer_enclosure_width_opt ';'
 /* 12/30/2003 - 5.6 syntax */

 | K_PREFERENCLOSURE layer_enclosure_type_opt NUMBER NUMBER
 {
 if (versionNum < 5.6) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ENCLOSURE is a 5.6 syntax. Your lef file is not defined as 5.6");
 CHKERR();
 }
 }
 } else {
 if (lefrLayerCbk)
 lefiLayer_addPreferEnclosure( &(lefrLayer),
 $2,
$3,
$4) ;
 }
 }
 layer_preferenclosure_width_opt ';'
 | K_RESISTANCE NUMBER ';'
 {
 if (versionNum < 5.6) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("RESISTANCE is a 5.6 syntax. Your lef file is not defined as 5.6");
 CHKERR();
 }
 }
 } else {
 if (lefrLayerCbk)
 lefiLayer_setResPerCut( &(lefrLayer),
 $2) ;
 }
 }
 | K_DIAGMINEDGELENGTH NUMBER ';'
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("DIAGMINEDGELENGTH can only be defined in LAYER with TYPE ROUTING. Parser stops executions");
 CHKERR();
 }
 }
 } else if (versionNum < 5.6) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("DIAGMINEDGELENGTH is a 5.6 syntax. Your lef file is not defined as 5.6");
 CHKERR();
 }
 }
 } else {
 if (lefrLayerCbk)
 lefiLayer_setDiagMinEdgeLength( &(lefrLayer),
 $2) ;
 }
 }
 | K_MINSIZE
 {
 /* Use the polygon code to retrieve the points for MINSIZE
 */
lefrGeometriesPtr = (lefiGeometries*)lefMalloc(sizeof(lefiGeometries));
 lefiGeometries_Init( lefrGeometriesPtr ) ;
 lefrDoGeometries = 1;
 }
 firstPt otherPts ';'
 {
 if (lefrLayerCbk) {
 lefiGeometries_addPolygon( lefrGeometriesPtr ) ;
 lefiLayer_setMinSize( &(lefrLayer),
 lefrGeometriesPtr) ;
 }
 lefrDoGeometries = 0;
 lefiGeometries_Destroy( lefrGeometriesPtr ) ;
 /* Don't need the object anymore
 */
lefFree(lefrGeometriesPtr);
 }
 
 sp_options:
 K_PARALLELRUNLENGTH NUMBER
 {
 if (hasInfluence) { /* 5.5 - INFLUENCE table must follow a PARALLEL
 */
if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("The INFLUENCE table must follow a PARALLELRUNLENGTH table on the same layer");
 CHKERR();
 }
 }
 }
 if (hasParallel) { /* 5.5 - Only one PARALLEL table is allowed per layer
 */
if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("Only one PARALLELRUNLENGTH table is allowed per layer");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 $2) ;
 hasParallel = 1;
 }
 number_list
 {
 spParallelLength = lefiLayer_getNumber( &(lefrLayer) ) ;
 if (lefrLayerCbk) lefiLayer_addSpParallelLength( &(lefrLayer) ) ;
 }
 K_WIDTH NUMBER
 {
 if (lefrLayerCbk) {
 lefiLayer_addSpParallelWidth( &(lefrLayer),
 $7) ;
 }
 }
 number_list
 {
 if (lefiLayer_getNumber( &(lefrLayer) )!=spParallelLength) { if (lefrLayerCbk) { if (layerWarnings++ < lefrLayerWarnings) { yyerror("The number of length in PARALLELRUNLENGTH is not the same as the number of spacing in WIDTH") ;
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addSpParallelWidthSpacing( &(lefrLayer) ) ;
 }
 layer_sp_parallel_widths
 | K_INFLUENCE K_WIDTH NUMBER K_WITHIN NUMBER K_SPACING NUMBER
 {
 if (hasInfluence) { /* 5.5 - INFLUENCE table must follow a PARALLEL
 */
if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("Only one INFLUENCE table is allowed per layer");
 CHKERR();
 }
 }
 }
 if (!hasParallel) { /* 5.5 - INFLUENCE must follow a PARALLEL
 */
if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("The INFLUENCE table must follow a PARALLELRUNLENGTH table on the same layer");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) {
 lefiLayer_setInfluence( &(lefrLayer) ) ;
 lefiLayer_addSpInfluence( &(lefrLayer),
 $3,
$5,
$7) ;
 }
 }
 layer_sp_influence_widths
 
 layer_enclosure_type_opt:
 {$$ = (char*)"NULL";} /* empty */

 | K_ABOVE {$$ = (char*)"ABOVE";}
 | K_BELOW {$$ = (char*)"BELOW";}
 
 layer_enclosure_width_opt: /* empty */

 | K_WIDTH NUMBER
 {
 if (lefrLayerCbk) {
 lefiLayer_addEnclosureWidth( &(lefrLayer),
 $2) ;
 }
 }
 
 layer_preferenclosure_width_opt: /* empty */

 | K_WIDTH NUMBER
 {
 if (lefrLayerCbk) {
 lefiLayer_addPreferEnclosureWidth( &(lefrLayer),
 $2) ;
 }
 }
 
 layer_minimumcut_options: /* empty */

 /* WANDA
 | layer_minimumcut_options layer_minimumcut_option
 */

 | layer_minimumcut_option layer_minimumcut_options
 
 layer_minimumcut_option:
 K_FROMABOVE
 {
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("FROMABOVE is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 }
 hasLayerMincut = 1;
 if (lefrLayerCbk)
 lefiLayer_addMinimumcutConnect( &(lefrLayer),
 (char*)"FROMABOVE") ;
 
 }
 | K_FROMBELOW
 {
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("FROMBELOW is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 }
 hasLayerMincut = 1;
 if (lefrLayerCbk)
 lefiLayer_addMinimumcutConnect( &(lefrLayer),
 (char*)"FROMBELOW") ;
 }
 | K_LENGTH NUMBER K_WITHIN NUMBER
 {
 if (versionNum < 5.5) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("LENGTH WITHIN is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk)
 lefiLayer_addMinimumcutLengDis( &(lefrLayer),
 $2,
$4) ;
 }
 
 layer_minstep_options: /* empty */

 /* WANDA
 | layer_minstep_options layer_minstep_option
 */

 | layer_minstep_option layer_minstep_options
 
 layer_minstep_option:
 layer_minstep_type
 {
 if (lefrLayerCbk) lefiLayer_addMinstepType( &(lefrLayer),
 $1) ;
 }
 | K_LENGTHSUM NUMBER
 {
 if (lefrLayerCbk) lefiLayer_addMinstepLengthsum( &(lefrLayer),
 $2) ;
 }
 
 layer_minstep_type:
 K_INSIDECORNER {$$ = (char*)"INSIDECORNER";}
 | K_OUTSIDECORNER {$$ = (char*)"OUTSIDECORNER";}
 | K_STEP {$$ = (char*)"STEP";}
 
 layer_antenna_pwl:
 NUMBER
 { if (lefrLayerCbk)
 lefiLayer_setAntennaValue( &(lefrLayer),
 antennaType,
$1) ;
 }
 | K_PWL '(' pt pt
 { if (lefrLayerCbk) { /* require min 2 points, set the 1st 2 */

 lefrAntennaPWLPtr = (lefiAntennaPWL*)lefMalloc(sizeof(lefiAntennaPWL));
 lefiAntennaPWL_Init( lefrAntennaPWLPtr ) ;
 lefiAntennaPWL_addAntennaPWL( lefrAntennaPWLPtr,
 $3.x,
$3.y) ;
 lefiAntennaPWL_addAntennaPWL( lefrAntennaPWLPtr,
 $4.x,
$4.y) ;
 }
 }
 layer_diffusion_ratios ')'
 { if (lefrLayerCbk)
 lefiLayer_setAntennaPWL( &(lefrLayer),
 antennaType,
lefrAntennaPWLPtr) ;
 }
 
 layer_diffusion_ratios: /* empty */

 /* WANDA
 | layer_diffusion_ratios layer_diffusion_ratio
 */

 | layer_diffusion_ratio layer_diffusion_ratios
 ;
 
 layer_diffusion_ratio:
 pt
 { if (lefrLayerCbk)
 lefiAntennaPWL_addAntennaPWL( lefrAntennaPWLPtr,
 $1.x,
$1.y) ;
 }
 
 layer_antenna_duo: /* empty */

 | K_DIFFUSEONLY
 {
 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 }
 else if ((antennaType == lefiAntennaAF) && (versionNum <= 5.3)) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNAAREAFACTOR with DIFFUSEONLY is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ANTENNAAREAFACTOR with DIFFUSEONLY is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk)
 lefiLayer_setAntennaDUO( &(lefrLayer),
 antennaType) ;
 }
 
 layer_table_type:
 K_PEAK {$$ = (char*)"PEAK";}
 | K_AVERAGE {$$ = (char*)"AVERAGE";}
 | K_RMS {$$ = (char*)"RMS";}
 
 layer_frequency:
 K_FREQUENCY NUMBER
 { if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 $2) ;
 }
 number_list ';'
 { if (lefrLayerCbk) lefiLayer_addAcFrequency( &(lefrLayer) ) ;
 }
 ac_layer_table_opt
 K_TABLEENTRIES NUMBER
 { if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 $9) ;
 }
 number_list ';'
 { if (lefrLayerCbk) lefiLayer_addAcTableEntry( &(lefrLayer) ) ;
 }
 
 ac_layer_table_opt: /* empty */

 | K_CUTAREA NUMBER
 {
 if (!layerCut) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("CUTAREA can only be defined in LAYER with type CUT");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 $2) ;
 }
 number_list ';'
 { if (lefrLayerCbk) lefiLayer_addAcCutarea( &(lefrLayer) ) ;
 }
 | K_WIDTH NUMBER
 {
 if (!layerRout) {
 if (lefrLayerCbk) { /* write error only if cbk is set */

 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("WIDTH can only be defined in LAYER with type ROUTING");
 CHKERR();
 }
 }
 }
 if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 $2) ;
 }
 number_list ';'
 { if (lefrLayerCbk) lefiLayer_addAcWidth( &(lefrLayer) ) ;
 }
 
 dc_layer_table:
 K_TABLEENTRIES NUMBER
 { if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 $2) ;
 }
 number_list ';'
 { if (lefrLayerCbk) lefiLayer_addDcTableEntry( &(lefrLayer) ) ;
 }
 
 number_list:
 | number_list NUMBER
 { if (lefrLayerCbk) lefiLayer_addNumber( &(lefrLayer),
 $2) ;
 }
 
 layer_prop_list:
 layer_prop
 | layer_prop_list layer_prop
 ;
 
 layer_prop:
 T_STRING T_STRING
 {
 if (lefrLayerCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrLayerProp),
 $1) ;
 lefiLayer_addProp( &(lefrLayer),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (lefrLayerCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrLayerProp),
 $1) ;
 lefiLayer_addProp( &(lefrLayer),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING NUMBER
 {
 char temp[32];
 sprintf(temp, "%.11g", $2);
 if (lefrLayerCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrLayerProp),
 $1) ;
 lefiLayer_addNumProp( &(lefrLayer),
 $1,
$2,
temp,
propTp) ;
 }
 }
 
 current_density_pwl_list :
 current_density_pwl
 { }
 | current_density_pwl_list current_density_pwl
 { }
 
 current_density_pwl: '(' NUMBER NUMBER ')'
 { if (lefrLayerCbk) lefiLayer_setCurrentPoint( &(lefrLayer),
 $2,
$3) ;
 }
 
 cap_points :
 cap_point
 | cap_points cap_point
 ;
 
 cap_point: '(' NUMBER NUMBER ')'
 { if (lefrLayerCbk) lefiLayer_setCapacitancePoint( &(lefrLayer),
 $2,
$3) ;
 }
 
 res_points :
 res_point
 | res_points res_point
 { }
 
 res_point: '(' NUMBER NUMBER ')'
 { if (lefrLayerCbk) lefiLayer_setResistancePoint( &(lefrLayer),
 $2,
$3) ;
 }
 
 layer_type:
 K_ROUTING {$$ = (char*)"ROUTING"; layerRout = 1;}
 | K_CUT {$$ = (char*)"CUT"; layerCut = 1;}
 | K_OVERLAP {$$ = (char*)"OVERLAP"; layerMastOver = 1;}
 | K_MASTERSLICE {$$ = (char*)"MASTERSLICE"; layerMastOver = 1;}
 | K_VIRTUAL {$$ = (char*)"VIRTUAL";}
 | K_IMPLANT {$$ = (char*)"IMPLANT";}
 
 layer_direction:
 K_HORIZONTAL {$$ = (char*)"HORIZONTAL";}
 | K_VERTICAL {$$ = (char*)"VERTICAL";}
 | K_DIAG45 {$$ = (char*)"DIAG45";}
 | K_DIAG135 {$$ = (char*)"DIAG135";}
 
 layer_minen_width:
 | K_WIDTH NUMBER
 {
 if (lefrLayerCbk)
 lefiLayer_addMinenclosedareaWidth( &(lefrLayer),
 $2) ;
 }
 
 layer_oxide:
 K_OXIDE1
 {
 if (lefrLayerCbk)
 lefiLayer_addAntennaModel( &(lefrLayer),
 1) ;
 }
 | K_OXIDE2
 {
 if (lefrLayerCbk)
 lefiLayer_addAntennaModel( &(lefrLayer),
 2) ;
 }
 | K_OXIDE3
 {
 if (lefrLayerCbk)
 lefiLayer_addAntennaModel( &(lefrLayer),
 3) ;
 }
 | K_OXIDE4
 {
 if (lefrLayerCbk)
 lefiLayer_addAntennaModel( &(lefrLayer),
 4) ;
 }
 
 layer_sp_parallel_widths: /* empty */

 { }
 /* WANDA
 | layer_sp_parallel_widths layer_sp_parallel_width
 */

 | layer_sp_parallel_width layer_sp_parallel_widths
 { }
 
 layer_sp_parallel_width: K_WIDTH NUMBER
 {
 if (lefrLayerCbk) {
 lefiLayer_addSpParallelWidth( &(lefrLayer),
 $2) ;
 }
 }
 number_list
 { if (lefrLayerCbk) lefiLayer_addSpParallelWidthSpacing( &(lefrLayer) ) ;
 }
 
 layer_sp_influence_widths: /* empty */

 { }
 | layer_sp_influence_widths layer_sp_influence_width
 { }
 
 layer_sp_influence_width: K_WIDTH NUMBER K_WITHIN NUMBER K_SPACING NUMBER
 { if (lefrLayerCbk) lefiLayer_addSpInfluence( &(lefrLayer),
 $2,
$4,
$6) ;
 }
 
 maxstack_via: K_MAXVIASTACK NUMBER ';'
 {
 if (!lefrHasLayer) { /* 5.5 */

 if (lefrMaxStackViaCbk) { /* write error only if cbk is set */

 if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
 yyerror("MAXVIASTACK has to define after the LAYER");
 CHKERR();
 }
 }
 } else if (lefrHasMaxVS) {
 if (lefrMaxStackViaCbk) { /* write error only if cbk is set */

 if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
 yyerror("Only one MAXVIASTACK is allowed in a lef file");
 CHKERR();
 }
 }
 } else {
 if (lefrMaxStackViaCbk) {
 lefiMaxStackVia_setMaxStackVia( &(lefrMaxStackVia),
 (int)$2) ;
 CALLBACK(lefrMaxStackViaCbk, lefrMaxStackViaCbkType, &lefrMaxStackVia);
 }
 }
 if (versionNum < 5.5) {
 if (lefrMaxStackViaCbk) { /* write error only if cbk is set */

 if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
 yyerror("MAXVIASTACK is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 }
 lefrHasMaxVS = 1;
 }
 | K_MAXVIASTACK NUMBER K_RANGE {lefDumbMode = 2; lefNoNum= 2;}
 T_STRING T_STRING ';'
 {
 if (!lefrHasLayer) { /* 5.5 */

 if (lefrMaxStackViaCbk) { /* write error only if cbk is set */

 if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
 yyerror("MAXVIASTACK has to define after the LAYER");
 CHKERR();
 }
 }
 } else if (lefrHasMaxVS) {
 if (lefrMaxStackViaCbk) { /* write error only if cbk is set */

 if (maxStackViaWarnings++ < lefrMaxStackViaWarnings) {
 yyerror("Only one MAXVIASTACK is allowed in a lef file");
 CHKERR();
 }
 }
 } else {
 if (lefrMaxStackViaCbk) {
 lefiMaxStackVia_setMaxStackVia( &(lefrMaxStackVia),
 (int)$2) ;
 lefiMaxStackVia_setMaxStackViaRange( &(lefrMaxStackVia),
 $5,
$6) ;
 CALLBACK(lefrMaxStackViaCbk, lefrMaxStackViaCbkType, &lefrMaxStackVia);
 }
 }
 lefrHasMaxVS = 1;
 }
 
 via: start_via { hasViaRule_layer = 0; } via_option end_via
 {
 if (lefrViaCbk)
 CALLBACK(lefrViaCbk, lefrViaCbkType, &lefrVia);
 }
 
 via_keyword : K_VIA /* neededto have a VIA named via*/

 { lefDumbMode = 1; lefNoNum = 1; }
 
 start_via: via_keyword T_STRING
 {
 /* 0 is nodefault */

 if (lefrViaCbk) lefiVia_setName( &(lefrVia),
 $2,
0) ;
 viaLayer = 0;
 numVia++;
 /* $2);
  */
viaName = strdup($2);
 }
 | via_keyword T_STRING K_DEFAULT
 {
 /* 1 is default */

 if (lefrViaCbk) lefiVia_setName( &(lefrVia),
 $2,
1) ;
 viaLayer = 0;
 /* $2);
  */
viaName = strdup($2);
 }
 | via_keyword T_STRING K_GENERATED
 {
 /* 2 is generated */

 if (lefrViaCbk) lefiVia_setName( &(lefrVia),
 $2,
2) ;
 viaLayer = 0;
 /* $2);
  */
viaName = strdup($2);
 }
 
 via_viarule: K_VIARULE {lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
 K_CUTSIZE NUMBER NUMBER ';'
 K_LAYERS {lefDumbMode = 3; lefNoNum = 1; } T_STRING T_STRING T_STRING ';'
 K_CUTSPACING NUMBER NUMBER ';'
 K_ENCLOSURE NUMBER NUMBER NUMBER NUMBER ';'
 {
 if (versionNum < 5.6) {
 if (lefrViaCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("VIARULE is a 5.6 syntax. Your lef file is not defined as 5.6");
 CHKERR();
 }
 }
 } else {
 if (lefrViaCbk) lefiVia_setViaRule( &(lefrVia),
 $3,
$6,
$7,
$11,
$12,
$13,
$16,
$17,
$20,
$21,
$22,
$23) ;
 }
 viaLayer++;
 hasViaRule_layer = 1;
 }
 via_viarule_options
 ;
 
 via_viarule_options: /* empty */

 | via_viarule_options via_viarule_option
 ;
 
 via_viarule_option: K_ROWCOL NUMBER NUMBER ';'
 {
 if (lefrViaCbk) lefiVia_setRowCol( &(lefrVia),
 (int)$2,(int)$3) ;
 }
 | K_ORIGIN NUMBER NUMBER ';'
 {
 if (lefrViaCbk) lefiVia_setOrigin( &(lefrVia),
 $2,
$3) ;
 }
 | K_OFFSET NUMBER NUMBER NUMBER NUMBER ';'
 {
 if (lefrViaCbk) lefiVia_setOffset( &(lefrVia),
 $2,
$3,
$4,
$5) ;
 }
 | K_PATTERN {lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
 {
 if (lefrViaCbk) lefiVia_setPattern( &(lefrVia),
 $3) ;
 }
 ;
 
 via_option: via_viarule
 | via_other_options
 
 via_other_options: via_other_option
 via_more_options
 
 via_more_options: /* empty */

 | via_more_options via_other_option
 ;
 
 via_other_option:
 via_foreign
 { }
 | via_layer_rule
 { }
 | K_RESISTANCE NUMBER ';'
 { if (lefrViaCbk) lefiVia_setResistance( &(lefrVia),
 $2) ;
 }
 | K_PROPERTY { lefDumbMode = 1000000; lefRealNum = 1; } via_prop_list ';'
 { lefDumbMode = 0;
 lefRealNum = 0;
 }
 | K_TOPOFSTACKONLY
 {
 if (versionNum < 5.6) {
 if (lefrViaCbk) lefiVia_setTopOfStack( &(lefrVia) ) ;
 } else
 if (lefrViaCbk) /* write warning only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings)
 yywarning("TOPOFSTACKONLY is obsolete in 5.6. It will be ignored.");
 }
 
 via_prop_list:
 via_name_value_pair
 | via_prop_list via_name_value_pair
 ;
 
 via_name_value_pair:
 T_STRING NUMBER
 {
 char temp[32];
 sprintf(temp, "%.11g", $2);
 if (lefrViaCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaProp),
 $1) ;
 lefiVia_addNumProp( &(lefrVia),
 $1,
$2,
temp,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (lefrViaCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaProp),
 $1) ;
 lefiVia_addProp( &(lefrVia),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING T_STRING
 {
 if (lefrViaCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaProp),
 $1) ;
 lefiVia_addProp( &(lefrVia),
 $1,
$2,
propTp) ;
 }
 }
 
 via_foreign:
 start_foreign ';'
 {
 if (versionNum < 5.6) {
 if (lefrViaCbk) lefiVia_setForeign( &(lefrVia),
 $1,
0,
0.0,
0.0,
-1) ;
 } else
 if (lefrViaCbk) /* write warning only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings)
 yywarning("FOREIGN in VIA is obsolete. It will be ignored.");
 }
 | start_foreign pt ';'
 {
 if (versionNum < 5.6) {
 if (lefrViaCbk) lefiVia_setForeign( &(lefrVia),
 $1,
1,
$2.x,
$2.y,
-1) ;
 } else
 if (lefrViaCbk) /* write warning only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings)
 yywarning("FOREIGN in VIA is obsolete. It will be ignored.");
 }
 | start_foreign pt orientation ';'
 {
 if (versionNum < 5.6) {
 if (lefrViaCbk) lefiVia_setForeign( &(lefrVia),
 $1,
1,
$2.x,
$2.y,
$3) ;
 } else
 if (lefrViaCbk) /* write warning only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings)
 yywarning("FOREIGN in VIA is obsolete. It will be ignored.");
 }
 | start_foreign orientation ';'
 {
 if (versionNum < 5.6) {
 if (lefrViaCbk) lefiVia_setForeign( &(lefrVia),
 $1,
0,
0.0,
0.0,
$2) ;
 } else
 if (lefrViaCbk) /* write warning only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings)
 yywarning("FOREIGN in VIA is obsolete. It will be ignored.");
 }
 
 start_foreign:	K_FOREIGN {lefDumbMode = 1; lefNoNum= 1;} T_STRING
 { $$ = $3; }
 
 orientation:
 K_N {$$ = 0;}
 | K_W {$$ = 1;}
 | K_S {$$ = 2;}
 | K_E {$$ = 3;}
 | K_FN {$$ = 4;}
 | K_FW {$$ = 5;}
 | K_FS {$$ = 6;}
 | K_FE {$$ = 7;}
 | K_R0 {$$ = 0;}
 | K_R90 {$$ = 1;}
 | K_R180 {$$ = 2;}
 | K_R270 {$$ = 3;}
 | K_MY {$$ = 4;}
 | K_MYR90 {$$ = 5;}
 | K_MX {$$ = 6;}
 | K_MXR90 {$$ = 7;}
 
 via_layer_rule: via_layer via_geometries
 { }
 
 via_layer: K_LAYER {lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
 {
 if (lefrViaCbk) lefiVia_addLayer( &(lefrVia),
 $3) ;
 viaLayer++;
 hasViaRule_layer = 1;
 }
 
 via_geometries:
 /* empty */

 | via_geometries via_geometry
 ;
 
 via_geometry:
 K_RECT pt pt ';'
 { if (lefrViaCbk)
 lefiVia_addRectToLayer( &(lefrVia),
 $2.x,
$2.y,
$3.x,
$3.y) ;
 }
 | K_POLYGON /* 5.6
  */
{
 lefrGeometriesPtr = (lefiGeometries*)lefMalloc(sizeof(lefiGeometries));
 lefiGeometries_Init( lefrGeometriesPtr ) ;
 lefrDoGeometries = 1;
 }
 firstPt nextPt nextPt otherPts ';'
 {
 lefiGeometries_addPolygon( lefrGeometriesPtr ) ;
 if (lefrViaCbk)
 lefiVia_addPolyToLayer( &(lefrVia),
 lefrGeometriesPtr) ;
 /* 5.6
  */
lefiGeometries_clearPolyItems( lefrGeometriesPtr ) ;
 /* free items fields
 */
lefFree((char*)(lefrGeometriesPtr)); /* Don't need anymore, poly data has
 */
lefrDoGeometries = 0; /* copied
  */
}
 
 end_via: K_END {lefDumbMode = 1; lefNoNum = 1;} T_STRING
 {
 /* 10/17/2001 - Wanda da Rosa, PCR 404149
 */
/*              Error if no layer in via
 */
if (!viaLayer) {
 if (lefrViaCbk) { /* write error only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings) {
 yyerror("VIA requires at least one layer");
 CHKERR();
 }
 }
 }
 if (strcmp(viaName, $3) != 0) {
 lefFree(viaName);
 if (lefrViaCbk) { /* write error only if cbk is set */

 if (viaWarnings++ < lefrViaWarnings) {
 yyerror("END VIA name is different from VIA");
 CHKERR();
 }
 }
 } else
 lefFree(viaName);
 }
 
 viarule_keyword : K_VIARULE { lefDumbMode = 1; lefNoNum = 1;} T_STRING
 {
 if (lefrViaRuleCbk) lefiViaRule_setName( &(lefrViaRule),
 $3) ;
 viaRuleLayer = 0;
 /* $3);
  */
viaRuleName = strdup($3);
 isGenerate = 0;
 }
 
 viarule:
 viarule_keyword viarule_layer_list via_names opt_viarule_props end_viarule
 {
 /* 10/17/2001 - Wanda da Rosa, PCR 404163
 */
/*              Error if layer number is not equal 2.
 */
/* 11/14/2001 - Wanda da Rosa,
 */
/*              Commented out for pcr 411781
 */
/* (viaRuleLayer != 2) {
 */
/* requires two layers");
 */
if (viaRuleLayer == 0 || viaRuleLayer > 2) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("VIARULE requires two layers");
 CHKERR();
 }
 }
 }
 if (lefrViaRuleCbk)
 CALLBACK(lefrViaRuleCbk, lefrViaRuleCbkType, &lefrViaRule);
 /* 2/19/2004 - reset the ENCLOSURE overhang values which may be
 */
/* set by the old syntax OVERHANG -- Not necessary, but just incase
 */
if (lefrViaRuleCbk) lefiViaRule_clearLayerOverhang( &(lefrViaRule) ) ;
 }
 
 viarule_generate:
 viarule_keyword K_GENERATE viarule_generate_default
 {
 isGenerate = 1;
 }
 viarule_layer_list opt_viarule_props end_viarule
 {
 /* 10/17/2001 - Wanda da Rosa, PCR 404181
 */
/*              Error if layer number is not equal 3.
 */
/* 11/14/2001 - Wanda da Rosa,
 */
/*              Commented out for pcr 411781
 */
/* (viaRuleLayer != 3) {
 */
/* requires three layers");
 */
if (viaRuleLayer == 0) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("VIARULE requires three layers");
 CHKERR();
 }
 }
 } else if ((viaRuleLayer < 3) && (versionNum >= 5.6)) {
 if (lefrViaRuleCbk) /* write warning only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings)
 yywarning("turn-via is obsolete in 5.6. It will be ignored.");
 } else {
 if (lefrViaRuleCbk) {
 lefiViaRule_setGenerate( &(lefrViaRule) ) ;
 CALLBACK(lefrViaRuleCbk, lefrViaRuleCbkType, &lefrViaRule);
 }
 }
 /* 2/19/2004 - reset the ENCLOSURE overhang values which may be
 */
/* set by the old syntax OVERHANG
 */
if (lefrViaRuleCbk) lefiViaRule_clearLayerOverhang( &(lefrViaRule) ) ;
 }
 
 viarule_generate_default: /* optional */

 | K_DEFAULT /* 5.6 syntax
 */
{
 if (versionNum < 5.6) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("DEFAULT is a 5.6 syntax. Your lef file is not defined as 5.6");
 CHKERR();
 }
 }
 } else
 if (lefrViaRuleCbk) lefiViaRule_setDefault( &(lefrViaRule) ) ;
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
 
 viarule_prop: K_PROPERTY { lefDumbMode = 10000000; lefRealNum = 1; } viarule_prop_list ';'
 { lefDumbMode = 0;
 lefRealNum = 0;
 }
 
 viarule_prop_list:
 viarule_prop
 | viarule_prop_list viarule_prop
 ;
 
 viarule_prop:
 T_STRING T_STRING
 {
 if (lefrViaRuleCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaRuleProp),
 $1) ;
 lefiViaRule_addProp( &(lefrViaRule),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (lefrViaRuleCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaRuleProp),
 $1) ;
 lefiViaRule_addProp( &(lefrViaRule),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING NUMBER
 {
 char temp[32];
 sprintf(temp, "%.11g", $2);
 if (lefrViaRuleCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrViaRuleProp),
 $1) ;
 lefiViaRule_addNumProp( &(lefrViaRule),
 $1,
$2,
temp,
propTp) ;
 }
 }
 
 viarule_layer: viarule_layer_name viarule_layer_options
 {
 /* 10/18/2001 - Wanda da Rosa PCR 404181
 */
/*              Make sure the 1st 2 layers in viarule has direction
 */
/* 04/28/2004 - PCR 704072 - DIRECTION in viarule generate is
 */
/*              obsoleted in 5.6
 */
if (versionNum >= 5.6) {
 if (viaRuleLayer < 2 && !viaRuleHasDir && !viaRuleHasEnc &&
 !isGenerate) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("VIARULE requires the DIRECTION construct in LAYER");
 CHKERR();
 }
 }
 }
 } else {
 if (viaRuleLayer < 2 && !viaRuleHasDir && !viaRuleHasEnc &&
 isGenerate) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("VIARULE requires the DIRECTION construct in LAYER");
 CHKERR();
 }
 }
 }
 }
 viaRuleLayer++;
 }
 ;
 
 via_names:
 /* empty */

 | via_names via_name
 ;
 
 via_name: via_keyword T_STRING ';'
 { if (lefrViaRuleCbk) lefiViaRule_addViaName( &(lefrViaRule),
 $2) ;
 }
 
 viarule_layer_name: K_LAYER {lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
 { if (lefrViaRuleCbk) lefiViaRule_setLayer( &(lefrViaRule),
 $3) ;
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
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("Either DIRECTION or ENCLOSURE can be specified in a layer");
 CHKERR();
 }
 }
 } else {
 if ((versionNum < 5.6) || (!isGenerate)) {
 if (lefrViaRuleCbk) lefiViaRule_setHorizontal( &(lefrViaRule) ) ;
 } else
 if (lefrViaRuleCbk) /* write warning only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings)
 yywarning("DIRECTION in VIARULE is obsolete in 5.6. It will be ignored.");
 }
 viaRuleHasDir = 1;
 }
 | K_DIRECTION K_VERTICAL ';'
 {
 if (viaRuleHasEnc) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("Either DIRECTION or ENCLOSURE can be specified in a layer");
 CHKERR();
 }
 }
 } else {
 if ((versionNum < 5.6) || (!isGenerate)) {
 if (lefrViaRuleCbk) lefiViaRule_setVertical( &(lefrViaRule) ) ;
 } else
 if (lefrViaRuleCbk) /* write warning only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings)
 yywarning("DIRECTION in VIARULE is obsolete in 5.6. It will be ignored.");
 }
 viaRuleHasDir = 1;
 }
 | K_ENCLOSURE NUMBER NUMBER ';' /* 5.5 */

 {
 if (versionNum < 5.5) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("ENCLOSURE is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 }
 /* 2/19/2004 - Enforced the rule that ENCLOSURE can only be defined
 */
/* in VIARULE GENERATE
 */
if (!isGenerate) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("ENCLOSURE can only be defined in VIARULE GENERATE");
 CHKERR();
 }
 }
 }
 if (viaRuleHasDir) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("Either DIRECTION or ENCLOSURE can be specified in a layer");
 CHKERR();
 }
 }
 } else {
 if (lefrViaRuleCbk) lefiViaRule_setEnclosure( &(lefrViaRule),
 $2,
$3) ;
 }
 viaRuleHasEnc = 1;
 }
 | K_WIDTH NUMBER K_TO NUMBER ';'
 { if (lefrViaRuleCbk) lefiViaRule_setWidth( &(lefrViaRule),
 $2,
$4) ;
 }
 | K_RECT pt pt ';'
 { if (lefrViaRuleCbk)
 lefiViaRule_setRect( &(	lefrViaRule),
 $2.x,
$2.y,
$3.x,
$3.y) ;
 }
 | K_SPACING NUMBER K_BY NUMBER ';'
 { if (lefrViaRuleCbk) lefiViaRule_setSpacing( &(lefrViaRule),
 $2,
$4) ;
 }
 | K_RESISTANCE NUMBER ';'
 { if (lefrViaRuleCbk) lefiViaRule_setResistance( &(lefrViaRule),
 $2) ;
 }
 | K_OVERHANG NUMBER ';'
 {
 if (!viaRuleHasDir) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("OVERHANG can only be defined with DIRECTION");
 CHKERR();
 }
 }
 }
 /* 2/19/2004 - Enforced the rule that OVERHANG can only be defined
 */
/* in VIARULE GENERATE after 5.3
 */
if ((versionNum > 5.3) && (!isGenerate)) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("OVERHANG can only be defined in VIARULE GENERATE");
 CHKERR();
 }
 }
 }
 if (versionNum < 5.6) {
 if (lefrViaRuleCbk) lefiViaRule_setOverhang( &(lefrViaRule),
 $2) ;
 } else {
 if (lefrViaRuleCbk) /* write warning only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings)
 yywarning("OVERHANG translated into similar ENCLOSURE rule");
 /* In 5.6 & later, set it to either ENCLOSURE overhang1 or overhang2
 */
if (lefrViaRuleCbk) lefiViaRule_setOverhangToEnclosure( &(lefrViaRule),
 $2) ;
 }
 }
 | K_METALOVERHANG NUMBER ';'
 {
 /* 2/19/2004 - Enforced the rule that METALOVERHANG can only be defined
 */
/* in VIARULE GENERATE
 */
if ((versionNum > 5.3) && (!isGenerate)) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("METALOVERHANG can only be defined in VIARULE GENERATE");
 CHKERR();
 }
 }
 }
 if (versionNum < 5.6) {
 if (!viaRuleHasDir) {
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("METALOVERHANG can only be defined with DIRECTION");
 CHKERR();
 }
 }
 }
 if (lefrViaRuleCbk) lefiViaRule_setMetalOverhang( &(lefrViaRule),
 $2) ;
 } else
 if (lefrViaRuleCbk) /* write warning only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings)
 yywarning("METALOVERHANG is obsolete in 5.6. It will be ignored.");
 }
 
 end_viarule: K_END {lefDumbMode = 1; lefNoNum = 1;} T_STRING
 {
 if (strcmp(viaRuleName, $3) != 0) {
 lefFree(viaRuleName);
 if (lefrViaRuleCbk) { /* write error only if cbk is set */

 if (viaRuleWarnings++ < lefrViaRuleWarnings) {
 yyerror("END VIARULE name is different from VIARULE");
 CHKERR();
 }
 }
 } else
 lefFree(viaRuleName);
 }
 
 spacing_rule: start_spacing spacings end_spacing
 { }
 
 start_spacing: K_SPACING
 {
 hasSamenet = 0;
 if ((versionNum < 5.6) || (!ndRule)) {
 if (lefrSpacingBeginCbk)
 	 CALLBACK(lefrSpacingBeginCbk, lefrSpacingBeginCbkType, 0);
 } else
 if (lefrSpacingBeginCbk) /* write warning only if cbk is set */

 if (spacingWarnings++ < lefrSpacingWarnings)
 yywarning("SAMENET in NONDEFAULTRULE is obsolete in 5.6. It will be ignored");
 }
 
 end_spacing: K_END K_SPACING
 {
 if ((versionNum < 5.6) || (!ndRule)) {
 if ((versionNum <= 5.4) && (!hasSamenet)) {
 yyerror("SAMENET is required inside SPACING for any lef file with version 5.4 and earlier, but is not defined in the parsed lef file");
 CHKERR();
 }
 if (lefrSpacingEndCbk)
 CALLBACK(lefrSpacingEndCbk, lefrSpacingEndCbkType, 0);
 }
 }
 
 spacings:
 /* empty */

 | spacings spacing
 ;
 
 spacing: samenet_keyword T_STRING T_STRING NUMBER ';'
 {
 if ((versionNum < 5.6) || (!ndRule)) {
 if (lefrSpacingCbk) {
 lefiSpacing_set( &(lefrSpacing),
 $2,
$3,
$4,
0) ;
 CALLBACK(lefrSpacingCbk, lefrSpacingCbkType, &lefrSpacing);
 }
 }
 }
 | samenet_keyword T_STRING T_STRING NUMBER K_STACK ';'
 {
 if ((versionNum < 5.6) || (!ndRule)) {
 if (lefrSpacingCbk) {
 	 lefiSpacing_set( &(lefrSpacing),
 $2,
$3,
$4,
1) ;
 	 CALLBACK(lefrSpacingCbk, lefrSpacingCbkType, &lefrSpacing);
 }
 }
 }
 
 samenet_keyword: K_SAMENET
 /* must be followed by two names */

 { lefDumbMode = 2; lefNoNum = 2; hasSamenet = 1; }
 
 irdrop: start_irdrop ir_tables end_irdrop
 { }
 
 start_irdrop: K_IRDROP
 {
 if (versionNum < 5.4) {
 if (lefrIRDropBeginCbk)
 	 CALLBACK(lefrIRDropBeginCbk, lefrIRDropBeginCbkType, 0);
 } else
 if (lefrIRDropBeginCbk) /* write warning only if cbk is set */

 if (iRDropWarnings++ < lefrIRDropWarnings)
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
 if (lefrIRDropCbk)
 CALLBACK(lefrIRDropCbk, lefrIRDropCbkType, &lefrIRDrop);
 }
 }
 
 ir_table_values:
 /* empty */

 | ir_table_values ir_table_value
 ;
 
 ir_table_value: NUMBER NUMBER
 { if (lefrIRDropCbk) lefiIRDrop_setValues( &(lefrIRDrop),
 $1,
$2) ;
 }
 
 ir_tablename: K_TABLE T_STRING
 { if (lefrIRDropCbk) lefiIRDrop_setTableName( &(lefrIRDrop),
 $2) ;
 }
 
 minfeature: K_MINFEATURE NUMBER NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrMinFeatureCbk) {
 lefiMinFeature_set( &(lefrMinFeature),
 $2,
$3) ;
 CALLBACK(lefrMinFeatureCbk, lefrMinFeatureCbkType, &lefrMinFeature);
 }
 } else
 if (lefrMinFeatureCbk) /* write warning only if cbk is set */

 if (minFeatureWarnings++ < lefrMinFeatureWarnings)
 yywarning("MINFEATURE is obsolete in 5.4. It will be ignored.");
 }
 
 dielectric: K_DIELECTRIC NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrDielectricCbk)
 CALLBACK(lefrDielectricCbk, lefrDielectricCbkType, $2);
 } else
 if (lefrDielectricCbk) /* write warning only if cbk is set */

 if (dielectricWarnings++ < lefrDielectricWarnings)
 yywarning("DIELECTRIC is obsolete in 5.4. It will be ignored.");
 }
 
 nondefault_rule: K_NONDEFAULTRULE {lefDumbMode = 1; lefNoNum = 1;} T_STRING
 {
 (void)lefSetNonDefault($3);
 if (lefrNonDefaultCbk) lefiNonDefault_setName( &(lefrNonDefault),
 $3) ;
 ndLayer = 0;
 ndRule = 1;
 numVia = 0;
 /* $3);
  */
nonDefaultRuleName = strdup($3);
 }
 nd_hardspacing
 nd_rules {lefNdRule = 1;} end_nd_rule
 {
 /* 10/18/2001 - Wanda da Rosa, PCR 404189
 */
/*              At least 1 layer is required
 */
if ((!ndLayer) && (!lefrRelaxMode)) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("NONDEFAULTRULE requires at least one layer");
 CHKERR();
 }
 }
 }
 if ((!numVia) && (!lefrRelaxMode) && (versionNum < 5.6)) {
 /* VIA is no longer a required statement in 5.6
 */
if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("NONDEFAULTRULE requires at least one via");
 CHKERR();
 }
 }
 }
 if (lefrNonDefaultCbk) {
 lefiNonDefault_end( &(lefrNonDefault) ) ;
 CALLBACK(lefrNonDefaultCbk, lefrNonDefaultCbkType, &lefrNonDefault);
 }
 ndRule = 0;
 lefDumbMode = 0;
 (void)lefUnsetNonDefault();
 }
 
 end_nd_rule: K_END
 {
 if ((nonDefaultRuleName) && (*nonDefaultRuleName != '\0'))
 lefFree(nonDefaultRuleName);
 }
 | K_END T_STRING
 {
 if (strcmp(nonDefaultRuleName, $2) != 0) {
 lefFree(nonDefaultRuleName);
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("END NONDEFAULTRULE name is different from NONDEFAULTRULE");
 CHKERR();
 }
 }
 } else
 lefFree(nonDefaultRuleName);
 }
 ;
 
 nd_hardspacing:
 /* empty */

 | K_HARDSPACING ';' /* HARDSPACING is optional in 5.6 */

 {
 if (versionNum < 5.6) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("HARDSPACING is a 5.6 syntax, Your lef file is not defined as 5.6");
 CHKERR();
 }
 }
 } else
 if (lefrNonDefaultCbk)
 lefiNonDefault_setHardspacing( &(lefrNonDefault) ) ;
 }
 ;
 
 nd_rules: /* empty */

 | nd_rules nd_rule
 ;
 
 nd_rule:
 nd_layer
 | via
 | spacing_rule
 | nd_prop
 | usevia
 | useviarule
 | mincuts
 ;
 
 usevia: K_USEVIA T_STRING ';'
 {
 if (versionNum < 5.6) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 yyerror("USEVIA is a 5.6 syntax, Your lef file is not defined as 5.6");
 CHKERR();
 }
 } else {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addUseVia( &(lefrNonDefault),
 $2) ;
 }
 }
 
 useviarule: K_USEVIARULE T_STRING ';'
 {
 if (versionNum < 5.6) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("USEVIARULE is a 5.6 syntax, Your lef file is not defined as 5.6");
 CHKERR();
 }
 }
 } else {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addUseViaRule( &(lefrNonDefault),
 $2) ;
 }
 }
 
 mincuts: K_MINCUTS T_STRING NUMBER ';'
 {
 if (versionNum < 5.6) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("MINCUTS is a 5.6 syntax, Your lef file is not defined as 5.6");
 CHKERR();
 }
 }
 } else {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addMinCuts( &(lefrNonDefault),
 $2,
(int)$3) ;
 }
 }
 
 nd_prop: K_PROPERTY { lefDumbMode = 10000000; lefRealNum = 1; } nd_prop_list ';'
 { lefDumbMode = 0;
 lefRealNum = 0;
 }
 
 nd_prop_list:
 nd_prop
 | nd_prop_list nd_prop
 ;
 
 nd_prop:
 T_STRING T_STRING
 {
 if (lefrNonDefaultCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrNondefProp),
 $1) ;
 lefiNonDefault_addProp( &(lefrNonDefault),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (lefrNonDefaultCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrNondefProp),
 $1) ;
 lefiNonDefault_addProp( &(lefrNonDefault),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING NUMBER
 {
 if (lefrNonDefaultCbk) {
 char temp[32];
 char propTp;
 sprintf(temp, "%.11g", $2);
 propTp = lefiPropType_propType( &(lefrNondefProp),
 $1) ;
 lefiNonDefault_addNumProp( &(lefrNonDefault),
 $1,
$2,
temp,
propTp) ;
 }
 }
 
 nd_layer: K_LAYER {lefDumbMode = 1; lefNoNum = 1;} T_STRING
 {
 if (lefrNonDefaultCbk) lefiNonDefault_addLayer( &(lefrNonDefault),
 $3) ;
 ndLayer++;
 /* $3);
  */
layerName = strdup($3);
 ndLayerWidth = 0;
 ndLayerSpace = 0;
 }
 K_WIDTH NUMBER ';'
 {
 ndLayerWidth = 1;
 if (lefrNonDefaultCbk) lefiNonDefault_addWidth( &(lefrNonDefault),
 $6) ;
 }
 nd_layer_stmts K_END {lefDumbMode = 1; lefNoNum = 1;} T_STRING
 {
 if (strcmp(layerName, $12) != 0) {
 lefFree(layerName);
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("END LAYER name is different from LAYER");
 CHKERR();
 }
 }
 } else
 lefFree(layerName);
 if (!ndLayerWidth) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("WIDTH is required in LAYER inside NONDEFULTRULE");
 CHKERR();
 }
 }
 }
 if (!ndLayerSpace && versionNum < 5.6) { /* 5.6, SPACING is optional
 */
if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("SPACING is required in LAYER inside NONDEFULTRULE");
 CHKERR();
 }
 }
 }
 }
 ;
 
 nd_layer_stmts:
 /* empty */

 | nd_layer_stmts nd_layer_stmt
 ;
 
 nd_layer_stmt:
 K_SPACING NUMBER ';'
 {
 ndLayerSpace = 1;
 if (lefrNonDefaultCbk) lefiNonDefault_addSpacing( &(lefrNonDefault),
 $2) ;
 }
 | K_WIREEXTENSION NUMBER ';'
 { if (lefrNonDefaultCbk)
 lefiNonDefault_addWireExtension( &(lefrNonDefault),
 $2) ;
 }
 | K_RESISTANCE K_RPERSQ NUMBER ';'
 {
 if (ignoreVersion) {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addResistance( &(lefrNonDefault),
 $3) ;
 } else if (versionNum < 5.4) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("RESISTANCE RPERSQ is a 5.4 syntax. Your lef file is not defined as version 5.4");
 CHKERR();
 }
 }
 } else if (versionNum > 5.5) { /* obsolete in 5.6
 */
if (lefrNonDefaultCbk) /* write warning only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings)
 yywarning("RESISTANCE RPERSQ is obsolete in 5.6. It will be ignored.");
 } else if (lefrNonDefaultCbk)
 lefiNonDefault_addResistance( &(lefrNonDefault),
 $3) ;
 }
 
 | K_CAPACITANCE K_CPERSQDIST NUMBER ';'
 {
 if (ignoreVersion) {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addCapacitance( &(lefrNonDefault),
 $3) ;
 } else if (versionNum < 5.4) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("CAPACITANCE CPERSQDIST is a 5.4 syntax. Your lef file is not defined as version 5.4");
 CHKERR();
 }
 }
 } else if (versionNum > 5.5) { /* obsolete in 5.6
 */
if (lefrNonDefaultCbk) /* write warning only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings)
 yywarning("CAPACITANCE CPERSQDIST is obsolete in 5.6. It will be ignored.");
 } else if (lefrNonDefaultCbk)
 lefiNonDefault_addCapacitance( &(lefrNonDefault),
 $3) ;
 }
 | K_EDGECAPACITANCE NUMBER ';'
 {
 if (ignoreVersion) {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addEdgeCap( &(lefrNonDefault),
 $2) ;
 } else if (versionNum < 5.4) {
 if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("EDGECAPACITANCE is a 5.4 syntax. Your lef file is not defined as version 5.4");
 CHKERR();
 }
 }
 } else if (versionNum > 5.5) { /* obsolete in 5.6
 */
if (lefrNonDefaultCbk) /* write warning only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings)
 yywarning("EDGECAPACITANCE is obsolete in 5.6. It will be ignored.");
 } else if (lefrNonDefaultCbk)
 lefiNonDefault_addEdgeCap( &(lefrNonDefault),
 $2) ;
 }
 | K_DIAGWIDTH NUMBER ';'
 {
 if (versionNum < 5.6) { /* 5.6 syntax
 */
if (lefrNonDefaultCbk) { /* write error only if cbk is set */

 if (nonDefaultWarnings++ < lefrNonDefaultWarnings) {
 yyerror("DIAGWIDTH is a 5.6 syntax. Your lef file is not defined as 5.6");
 CHKERR();
 }
 }
 } else {
 if (lefrNonDefaultCbk)
 lefiNonDefault_addDiagWidth( &(lefrNonDefault),
 $2) ;
 }
 }
 
 site: start_site site_options end_site
 {
 if (lefrSiteCbk)
 CALLBACK(lefrSiteCbk, lefrSiteCbkType, &lefrSite);
 }
 
 start_site: K_SITE {lefDumbMode = 1; lefNoNum = 1;} T_STRING
 {
 if (lefrSiteCbk) lefiSite_setName( &(lefrSite),
 $3) ;
 /* $3);
  */
siteName = strdup($3);
 hasSiteClass = 0;
 hasSiteSize = 0;
 }
 
 end_site: K_END {lefDumbMode = 1; lefNoNum = 1;} T_STRING
 {
 if (strcmp(siteName, $3) != 0) {
 lefFree(siteName);
 if (lefrSiteCbk) { /* write error only if cbk is set */

 if (siteWarnings++ < lefrSiteWarnings) {
 yyerror("END SITE name is different from SITE");
 CHKERR();
 }
 }
 } else {
 lefFree(siteName);
 if (lefrSiteCbk) { /* write error only if cbk is set */

 if (hasSiteClass == 0) {
 yyerror("CLASS in SITE is required");
 CHKERR();
 }
 if (hasSiteSize == 0) {
 yyerror("SIZE in SITE is required");
 CHKERR();
 }
 }
 }
 }
 
 site_options:
 /* empty */

 | site_options site_option
 ;
 
 site_option:
 K_SIZE NUMBER K_BY NUMBER ';'
 {
 /* Workaround for pcr 640902
 */

 if (lefrSiteCbk) lefiSite_setSize( &(lefrSite),
 $2,
$4) ;
 hasSiteSize = 1;
 }
 | site_symmetry_statement
 { }
 | site_class
 {
 if (lefrSiteCbk) lefiSite_setClass( &(lefrSite),
 $1) ;
 hasSiteClass = 1;
 }
 | site_rowpattern_statement
 { }
 
 site_class:
 K_CLASS K_PAD ';' {$$ = (char*)"PAD"; }
 | K_CLASS K_CORE ';' {$$ = (char*)"CORE"; }
 | K_CLASS K_VIRTUAL ';' {$$ = (char*)"VIRTUAL"; }
 
 site_symmetry_statement: K_SYMMETRY site_symmetries ';'
 { }
 
 site_symmetries:
 /* empty */

 | site_symmetries site_symmetry
 ;
 
 site_symmetry:
 K_X
 { if (lefrSiteCbk) lefiSite_setXSymmetry( &(lefrSite) ) ;
 }
 | K_Y
 { if (lefrSiteCbk) lefiSite_setYSymmetry( &(lefrSite) ) ;
 }
 | K_R90
 { if (lefrSiteCbk) lefiSite_set90Symmetry( &(lefrSite) ) ;
 }
 
 site_rowpattern_statement: K_ROWPATTERN {lefDumbMode = 1; lefNoNum = 1;}
 site_rowpatterns ';'
 { }
 
 site_rowpatterns:
 /* empty */

 | site_rowpatterns site_rowpattern
 ;
 
 site_rowpattern: T_STRING orientation {lefDumbMode = 1; lefNoNum = 1;}
 { if (lefrSiteCbk) lefiSite_addRowPattern( &(lefrSite),
 $1,
$2) ;
 }
 
 pt:
 NUMBER NUMBER
 { $$.x = $1; $$.y = $2; }
 | '(' NUMBER NUMBER ')'
 { $$.x = $2; $$.y = $3; }
 
 macro: start_macro macro_options
 {
 if (lefrMacroCbk)
 CALLBACK(lefrMacroCbk, lefrMacroCbkType, &lefrMacro);
 lefrDoSite = 0;
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
 lefiMacro_clear( &(lefrMacro) ) ;
 if (lefrMacroBeginCbk || lefrMacroCbk) {
 /* some reader may not have MacroBeginCB, but has MacroCB set
 */
lefiMacro_setName( &(lefrMacro),
 $3) ;
 CALLBACK(lefrMacroBeginCbk, lefrMacroBeginCbkType, $3);
 }
 /* $3);
  */
macroName = strdup($3);
 }
 
 end_macro: K_END {lefDumbMode = 1; lefNoNum = 1;} T_STRING
 {
 if (strcmp(macroName, $3) != 0) {
 lefFree(macroName);
 if (lefrMacroEndCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 yyerror("END MACRO name is different from MACRO");
 CHKERR();
 }
 }
 } else
 lefFree(macroName);
 if (lefrMacroEndCbk)
 CALLBACK(lefrMacroEndCbk, lefrMacroEndCbkType, $3);
 }
 
 macro_options:
 /* empty */

 /* WANDA
 | macro_options macro_option
 */

 | macro_option macro_options
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
 { if (lefrMacroCbk) lefiMacro_setBuffer( &(lefrMacro) ) ;
 }
 | K_FUNCTION K_INVERTER ';'
 { if (lefrMacroCbk) lefiMacro_setInverter( &(lefrMacro) ) ;
 }
 | macro_obs
 { }
 | macro_density
 { }
 | macro_clocktype
 { }
 | timing
 { }
 | K_PROPERTY {lefDumbMode = 1000000; lefRealNum = 1;} macro_prop_list ';'
 { lefDumbMode = 0;
 lefRealNum = 0;
 }
 
 macro_prop_list:
 macro_name_value_pair
 | macro_prop_list macro_name_value_pair
 ;
 
 macro_symmetry_statement: K_SYMMETRY macro_symmetries ';'
 {
 if (siteDef) { /* SITE is defined before SYMMETRY */

 /* pcr 283846 suppress warning */

 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
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
 { if (lefrMacroCbk) lefiMacro_setXSymmetry( &(lefrMacro) ) ;
 }
 | K_Y
 { if (lefrMacroCbk) lefiMacro_setYSymmetry( &(lefrMacro) ) ;
 }
 | K_R90
 { if (lefrMacroCbk) lefiMacro_set90Symmetry( &(lefrMacro) ) ;
 }
 
 macro_name_value_pair:
 T_STRING NUMBER
 {
 char temp[32];
 sprintf(temp, "%.11g", $2);
 if (lefrMacroCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrMacroProp),
 $1) ;
 lefiMacro_setNumProperty( &(lefrMacro),
 $1,
$2,
temp,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (lefrMacroCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrMacroProp),
 $1) ;
 lefiMacro_setProperty( &(lefrMacro),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING T_STRING
 {
 if (lefrMacroCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrMacroProp),
 $1) ;
 lefiMacro_setProperty( &(lefrMacro),
 $1,
$2,
propTp) ;
 }
 }
 
 macro_class: K_CLASS class_type ';'
 {
 if (lefrMacroCbk) lefiMacro_setClass( &(lefrMacro),
 $2) ;
 if (lefrMacroClassTypeCbk)
 CALLBACK(lefrMacroClassTypeCbk, lefrMacroClassTypeCbkType, $2);
 }
 
 class_type:
 K_COVER {$$ = (char*)"COVER"; }
 | K_COVER K_BUMP
 { $$ = (char*)"COVER BUMP";
 if (versionNum < 5.5) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 if (lefrRelaxMode)
 yywarning("The statement COVER BUMP is a LEF verion 5.5 syntax.\nYour LEF file is version 5.4 or earlier which is illegal but will be allowed\nbecause this application does not enforce strict version checking.\nOther tools that enforce strict checking will have a syntax error when reading this file.\nYou can change the VERSION statement in this LEF file to 5.5 or higher to stop this warning.");
 else
 yyerror("COVER BUMP is a 5.5 syntax. Your lef file is not defined as 5.5 or later");
 CHKERR();
 }
 }
 }
 }
 | K_RING {$$ = (char*)"RING"; }
 | K_BLOCK {$$ = (char*)"BLOCK"; }
 | K_BLOCK K_BLACKBOX
 { $$ = (char*)"BLOCK BLACKBOX";
 if (versionNum < 5.5) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 if (lefrRelaxMode)
 yywarning("The statement BLOCK BLACKBOX is a LEF verion 5.5 syntax.\nYour LEF file is version 5.4 or earlier which is illegal but will be allowed\nbecause this application does not enforce strict version checking.\nOther tools that enforce strict checking will have a syntax error when reading this file.\nYou can change the VERSION statement in this LEF file to 5.5 or higher to stop this warning.");
 else
 yyerror("BLOCK BLACKBOX is a 5.5 syntax. Your lef file is not defined as 5.5 or later");
 CHKERR();
 }
 }
 }
 }
 | K_BLOCK K_SOFT
 {
 if (ignoreVersion) {
 $$ = (char*)"BLOCK SOFT";
 } else if (versionNum < 5.6) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 yyerror("BLOCK SOFT is a 5.6 syntax. Your lef file is not defined as version 5.6");
 CHKERR();
 }
 }
 }
 else
 $$ = (char*)"BLOCK SOFT";
 }
 | K_NONE {$$ = (char*)"NONE"; }
 | K_PAD {$$ = (char*)"PAD"; }
 | K_VIRTUAL {$$ = (char*)"VIRTUAL"; }
 | K_PAD pad_type
 { sprintf(temp_name, "PAD %s", $2);
 $$ = temp_name;
 if (versionNum < 5.5) {
 if (strcmp("AREAIO", $2) != 0) {
 sprintf(temp_name, "PAD %s", $2);
 $$ = temp_name;
 } else if (lefrMacroCbk) {
 if (macroWarnings++ < lefrMacroWarnings) {
 if (lefrRelaxMode)
 yywarning("The statement PAD AREAIO is a LEF verion 5.5 syntax.\nYour LEF file is version 5.4 or earlier which is illegal but will be allowed\nbecause this application does not enforce strict version checking.\nOther tools that enforce strict checking will have a syntax error when reading this file.\nYou can change the VERSION statement in this LEF file to 5.5 or higher to stop this warning.");
 else
 yyerror("PAD AREAIO is a 5.5 syntax. Your lef file is not defined as 5.5 or later");
 CHKERR();
 }
 }
 }
 }
 | K_CORE {$$ = (char*)"CORE"; }
 | K_CORNER
 {$$ = (char*)"CORNER";
 /* This token is NOT in the spec but has shown up in
 * some lef files. This exception came from LEFOUT
 * in 'frameworks'
 */

 }
 | K_CORE core_type
 {sprintf(temp_name, "CORE %s", $2);
 $$ = temp_name;}
 | K_ENDCAP endcap_type
 {sprintf(temp_name, "ENDCAP %s", $2);
 $$ = temp_name;}
 
 pad_type: /* {$$ = (char*)"";}
 | K_INPUT 	{$$ = (char*)"INPUT";} */

 K_INPUT 	{$$ = (char*)"INPUT";}
 | K_OUTPUT	{$$ = (char*)"OUTPUT";}
 | K_INOUT 	{$$ = (char*)"INOUT";}
 | K_POWER 	{$$ = (char*)"POWER";}
 | K_SPACER	{$$ = (char*)"SPACER";}
 | K_AREAIO {$$ = (char*)"AREAIO";}
 
 core_type:
 K_FEEDTHRU {$$ = (char*)"FEEDTHRU";}
 | K_TIEHIGH {$$ = (char*)"TIEHIGH";}
 | K_TIELOW {$$ = (char*)"TIELOW";}
 | K_SPACER
 {
 if (ignoreVersion) {
 $$ = (char*)"SPACER";
 } else if (versionNum < 5.4) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 yyerror("SPACER is a 5.4 syntax. Your lef file is not defined as version 5.4");
 CHKERR();
 }
 }
 }
 else
 $$ = (char*)"SPACER";
 }
 | K_ANTENNACELL
 {
 if (ignoreVersion) {
 $$ = (char*)"ANTENNACELL";
 } else if (versionNum < 5.4) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 yyerror("ANTENNACELL is a 5.4 syntax. Your lef file is not defined as version 5.4");
 CHKERR();
 }
 }
 }
 else
 $$ = (char*)"ANTENNACELL";
 }
 | K_WELLTAP
 {
 if (ignoreVersion) {
 $$ = (char*)"WELLTAP";
 } else if (versionNum < 5.6) {
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 yyerror("WELLTAP is a 5.6 syntax. Your lef file is not defined as version 5.6");
 CHKERR();
 }
 }
 }
 else
 $$ = (char*)"WELLTAP";
 }
 
 endcap_type:
 K_PRE 	{$$ = (char*)"PRE";}
 | K_POST 	{$$ = (char*)"POST";}
 | K_TOPLEFT 	{$$ = (char*)"TOPLEFT";}
 | K_TOPRIGHT 	{$$ = (char*)"TOPRIGHT";}
 | K_BOTTOMLEFT 	{$$ = (char*)"BOTTOMLEFT";}
 | K_BOTTOMRIGHT	{$$ = (char*)"BOTTOMRIGHT";}
 
 macro_generator: K_GENERATOR T_STRING ';'
 { if (lefrMacroCbk) lefiMacro_setGenerator( &(lefrMacro),
 $2) ;
 }
 
 macro_generate: K_GENERATE T_STRING T_STRING ';'
 { if (lefrMacroCbk) lefiMacro_setGenerate( &(lefrMacro),
 $2,
$3) ;
 }
 
 macro_source:
 K_SOURCE K_USER ';'
 {
 if (versionNum < 5.6) {
 if (lefrMacroCbk) lefiMacro_setSource( &(lefrMacro),
 "USER") ;
 } else
 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 yywarning("SOURCE is obsolete in 5.6. It will be ignored.");
 }
 | K_SOURCE K_GENERATE ';'
 {
 if (versionNum < 5.6) {
 if (lefrMacroCbk) lefiMacro_setSource( &(lefrMacro),
 "GENERATE") ;
 } else
 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 yywarning("SOURCE is obsolete in 5.6. It will be ignored.");
 }
 | K_SOURCE K_BLOCK ';'
 {
 if (versionNum < 5.6) {
 if (lefrMacroCbk) lefiMacro_setSource( &(lefrMacro),
 "BLOCK") ;
 } else
 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 yywarning("SOURCE is obsolete in 5.6. It will be ignored.");
 }
 
 macro_power: K_POWER NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrMacroCbk) lefiMacro_setPower( &(lefrMacro),
 $2) ;
 } else
 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 yywarning("MACRO POWER is obsolete in 5.4. It will be ignored.");
 }
 
 macro_origin: K_ORIGIN pt ';'
 {
 if (origDef) { /* Has multiple ORIGIN defined in a macro, stop parsing*/

 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 yyerror("ORIGIN statement is defined more than once. Parser stops executions.");
 CHKERR();
 }
 }
 }
 origDef = 1;
 if (siteDef) { /* SITE is defined before ORIGIN */

 /* pcr 283846 suppress warning */

 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 yywarning("SITE is defined before ORIGIN.");
 }
 if (pinDef) { /* PIN is defined before ORIGIN */

 /* pcr 283846 suppress warning */

 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 yywarning("PIN is defined before ORIGIN.");
 }
 if (obsDef) { /* OBS is defined before ORIGIN */

 /* pcr 283846 suppress warning */

 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 yywarning("OBS is defined before ORIGIN.");
 }
 /* 11/22/99 - Wanda da Rosa. PCR 283846
 can be defined any order.
 if (symDef) * SYMMETRY is defined before ORIGIN *
 yywarning("SYMMETRY is defined before ORIGIN.");
 */

 /* Add back it back in per Nora request on PCR 283846 */

 /* 1/14/2000 - Wanda da Rosa, PCR 288770
 if (sizeDef) * SIZE is defined before ORIGIN *
 yywarning("SIZE is defined before ORIGIN.");
 */

 
 /* Workaround for pcr 640902 */

 if (lefrMacroCbk) lefiMacro_setOrigin( &(lefrMacro),
 $2.x,
$2.y) ;
 if (lefrMacroOriginCbk) {
 macroNum.x = $2.x;
 macroNum.y = $2.y;
 CALLBACK(lefrMacroOriginCbk, lefrMacroOriginCbkType, macroNum);
 }
 }
 
 macro_foreign:
 start_foreign ';'
 { if (lefrMacroCbk)
 lefiMacro_addForeign( &(lefrMacro),
 $1,
0,
0.0,
0.0,
-1) ;
 }
 | start_foreign pt ';'
 { if (lefrMacroCbk)
 lefiMacro_addForeign( &(lefrMacro),
 $1,
1,
$2.x,
$2.y,
-1) ;
 }
 | start_foreign pt orientation ';'
 { if (lefrMacroCbk)
 lefiMacro_addForeign( &(lefrMacro),
 $1,
1,
$2.x,
$2.y,
$3) ;
 }
 | start_foreign orientation ';'
 { if (lefrMacroCbk)
 lefiMacro_addForeign( &(lefrMacro),
 $1,
0,
0.0,
0.0,
$2) ;
 }
 
 macro_eeq: K_EEQ { lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
 { if (lefrMacroCbk) lefiMacro_setEEQ( &(lefrMacro),
 $3) ;
 }
 
 macro_leq: K_LEQ { lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
 {
 if (versionNum < 5.6) {
 if (lefrMacroCbk) lefiMacro_setLEQ( &(lefrMacro),
 $3) ;
 } else
 if (lefrMacroCbk) /* write warning only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings)
 yywarning("LEQ in MACRO is obsolete in 5.6. It will be ignored.");
 }
 
 macro_site:
 macro_site_word T_STRING ';'
 {
 if (lefrMacroCbk) {
 lefiMacro_setSiteName( &(lefrMacro),
 $2) ;
 /* For later than 5.6 release
 lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
 sizeof(lefiSitePattern));
 lefiSitePattern_Init( lefrSitePatternPtr ) ;
 lefiSitePattern_setSiteName( lefrSitePatternPtr,
 $2) ;
 */

 }
 }
 | macro_site_word sitePattern ';'
 {
 if (lefrMacroCbk) {
 /* also set site name in the variable siteName_ in lefiMacro */

 /* this, if user wants to use method siteName will get the name also */

 /* Does not work, it will only set with the last sitename, if multiple
 SITEs are defined. Therefore, data will not be correct
 lefiMacro_setSitePatternName( &(lefrMacro),
 lefiSitePattern_name( lefrSitePatternPtr )) ;
 */

 lefiMacro_setSitePattern( &(	lefrMacro),
 lefrSitePatternPtr) ;
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
 if (siteWarnings++ < lefrSiteWarnings)
 yywarning("SITE is defined before SIZE.");
 return 1;
 */

 }
 sizeDef = 1;
 if (lefrMacroCbk) lefiMacro_setSize( &(lefrMacro),
 $2,
$4) ;
 if (lefrMacroSizeCbk) {
 macroNum.x = $2;
 macroNum.y = $4;
 CALLBACK(lefrMacroSizeCbk, lefrMacroSizeCbkType, macroNum);
 }
 }
 
 /* This is confusing, since FEF and LEF have opposite definitions of
 ports and pins */

 
 macro_pin: start_macro_pin macro_pin_options end_macro_pin
 {
 if (lefrPinCbk)
 CALLBACK(lefrPinCbk, lefrPinCbkType, &lefrPin);
 lefiPin_clear( &(lefrPin) ) ;
 }
 
 start_macro_pin: K_PIN {lefDumbMode = 1; lefNoNum = 1; pinDef = 1;} T_STRING
 { if (lefrPinCbk) lefiPin_setName( &(lefrPin),
 $3) ;
 /* $3);
  */
pinName = strdup($3);
 }
 
 end_macro_pin: K_END {lefDumbMode = 1; lefNoNum = 1;} T_STRING
 {
 if (strcmp(pinName, $3) != 0) {
 lefFree(pinName);
 if (lefrMacroCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 yyerror("END PIN name is different from PIN");
 CHKERR();
 }
 }
 } else
 lefFree(pinName);
 }
 
 macro_pin_options:
 /* empty */

 { }
 | macro_pin_options macro_pin_option
 { }
 
 macro_pin_option:
 start_foreign ';'
 {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_addForeign( &(lefrPin),
 $1,
0,
0.0,
0.0,
-1) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("FOREIGN in MACRO PIN is obsolete in 5.6. It will be ignored.");
 }
 | start_foreign pt ';'
 {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_addForeign( &(lefrPin),
 $1,
1,
$2.x,
$2.y,
-1) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("FOREIGN in MACRO PIN is obsolete in 5.6. It will be ignored.");
 }
 | start_foreign pt orientation ';'
 {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_addForeign( &(lefrPin),
 $1,
1,
$2.x,
$2.y,
$3) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("FOREIGN in MACRO PIN is obsolete in 5.6. It will be ignored.");
 }
 | start_foreign K_STRUCTURE ';'
 {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_addForeign( &(lefrPin),
 $1,
0,
0.0,
0.0,
-1) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("FOREIGN in MACRO PIN is obsolete in 5.6. It will be ignored.");
 }
 | start_foreign K_STRUCTURE pt ';'
 {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_addForeign( &(lefrPin),
 $1,
1,
$3.x,
$3.y,
-1) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("FOREIGN in MACRO PIN is obsolete in 5.6. It will be ignored.");
 }
 | start_foreign K_STRUCTURE pt orientation ';'
 {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_addForeign( &(lefrPin),
 $1,
1,
$3.x,
$3.y,
$4) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("FOREIGN in MACRO PIN is obsolete in 5.6. It will be ignored.");
 }
 | K_LEQ { lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
 {
 if (versionNum < 5.6) {
 if (lefrPinCbk) lefiPin_setLEQ( &(lefrPin),
 $3) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("LEQ in MACRO PIN is obsolete in 5.6. It will be ignored.");
 }
 | K_POWER NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setPower( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO POWER is obsolete in 5.4. It will be ignored.");
 }
 | electrical_direction
 { if (lefrPinCbk) lefiPin_setDirection( &(lefrPin),
 $1) ;
 }
 | K_USE macro_pin_use ';'
 { if (lefrPinCbk) lefiPin_setUse( &(lefrPin),
 $2) ;
 }
 | K_SCANUSE macro_scan_use ';'
 { }
 | K_LEAKAGE NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setLeakage( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO LEAKAGE is obsolete in 5.4. It will be ignored.");
 }
 | K_RISETHRESH NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setRiseThresh( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO RISETHRESH is obsolete in 5.4. It will be ignored.");
 }
 | K_FALLTHRESH NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setFallThresh( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO FALLTHRESH is obsolete in 5.4. It will be ignored.");
 }
 | K_RISESATCUR NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setRiseSatcur( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO RISESATCUR is obsolete in 5.4. It will be ignored.");
 }
 | K_FALLSATCUR NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setFallSatcur( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO FALLSATCUR is obsolete in 5.4. It will be ignored.");
 }
 | K_VLO NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setVLO( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO VLO is obsolete in 5.4. It will be ignored.");
 }
 | K_VHI NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setVHI( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO VHI is obsolete in 5.4. It will be ignored.");
 }
 | K_TIEOFFR NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setTieoffr( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO TIEOFFR is obsolete in 5.4. It will be ignored.");
 }
 | K_SHAPE pin_shape ';'
 { if (lefrPinCbk) lefiPin_setShape( &(lefrPin),
 $2) ;
 }
 | K_MUSTJOIN {lefDumbMode = 1; lefNoNum = 1;} T_STRING ';'
 { if (lefrPinCbk) lefiPin_setMustjoin( &(lefrPin),
 $3) ;
 }
 | K_OUTPUTNOISEMARGIN {lefDumbMode = 1;} NUMBER NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setOutMargin( &(lefrPin),
 $3,
$4) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning(
 "MACRO OUTPUTNOISEMARGIN is obsolete in 5.4. It will be ignored.");
 }
 | K_OUTPUTRESISTANCE {lefDumbMode = 1;} NUMBER NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setOutResistance( &(lefrPin),
 $3,
$4) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning(
 "MACRO OUTPUTRESISTANCE is obsolete in 5.4. It will be ignored.");
 }
 | K_INPUTNOISEMARGIN {lefDumbMode = 1;} NUMBER NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setInMargin( &(lefrPin),
 $3,
$4) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning(
 "MACRO INPUTNOISEMARGIN is obsolete in 5.4. It will be ignored.");
 }
 | K_CAPACITANCE NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setCapacitance( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO CAPACITANCE is obsolete in 5.4. It will be ignored.");
 }
 | K_MAXDELAY NUMBER ';'
 { if (lefrPinCbk) lefiPin_setMaxdelay( &(lefrPin),
 $2) ;
 }
 | K_MAXLOAD NUMBER ';'
 { if (lefrPinCbk) lefiPin_setMaxload( &(lefrPin),
 $2) ;
 }
 | K_RESISTANCE NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setResistance( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO RESISTANCE is obsolete in 5.4. It will be ignored.");
 }
 | K_PULLDOWNRES NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setPulldownres( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO PULLDOWNRES is obsolete in 5.4. It will be ignored.");
 }
 | K_CURRENTSOURCE K_ACTIVE ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setCurrentSource( &(lefrPin),
 "ACTIVE") ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning(
 "MACRO CURRENTSOURCE is obsolete in 5.4. It will be ignored.");
 }
 | K_CURRENTSOURCE K_RESISTIVE ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setCurrentSource( &(lefrPin),
 "RESISTIVE") ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning(
 "MACRO CURRENTSOURCE is obsolete in 5.4. It will be ignored.");
 }
 | K_RISEVOLTAGETHRESHOLD NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setRiseVoltage( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning(
 "MACRO RISEVOLTAGETHRESHOLD is obsolete in 5.4. It will be ignored.");
 }
 | K_FALLVOLTAGETHRESHOLD NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setFallVoltage( &(lefrPin),
 $2) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning(
 "MACRO FALLVOLTAGETHRESHOLD is obsolete in 5.4. It will be ignored.");
 }
 | K_IV_TABLES T_STRING T_STRING ';'
 {
 if (versionNum < 5.4) {
 if (lefrPinCbk) lefiPin_setTables( &(lefrPin),
 $2,
$3) ;
 } else
 if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("MACRO IV_TABLES is obsolete in 5.4. It will be ignored.");
 }
 | K_TAPERRULE T_STRING ';'
 { if (lefrPinCbk) lefiPin_setTaperRule( &(lefrPin),
 $2) ;
 }
 | K_PROPERTY {lefDumbMode = 1000000; lefRealNum = 1;} pin_prop_list ';'
 { lefDumbMode = 0;
 lefRealNum = 0;
 }
 | start_macro_port macro_port_class_option geometries K_END
 {
 lefDumbMode = 0;
 if (lefrPinCbk) {
 lefiPin_addPort( &(	lefrPin),
 lefrGeometriesPtr) ;
 	lefrGeometriesPtr = 0;
 	lefrDoGeometries = 0;
 }
 if ((needGeometry) && (needGeometry != 2)) /* if the last LAYER in PORT
 */
if (lefrPinCbk) /* write warning only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings)
 yywarning("Either PATH, RECT or POLYGON is a required in MACRO/PIN/PORT.");
 }
 | start_macro_port K_END
 /* 06/07/2001 - Wanda da Rosa. Allow empty PORT due to incorrect
 * old lef files from customers
 */

 {
 /* Since in start_macro_port it has call the Init method, here
 */
/* we need to call the Destroy method.
 */
/* Still add a null pointer to set the number of port
 */
if (lefrPinCbk) {
 lefiPin_addPort( &(lefrPin),
 lefrGeometriesPtr) ;
 lefrGeometriesPtr = 0;
 lefrDoGeometries = 0;
 }
 }
 | K_ANTENNASIZE NUMBER opt_layer_name ';'
 { /* a pre 5.4 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum >= 5.4) {
 if (use5_4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNASIZE is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaSize( &(lefrPin),
 $2,
$3) ;
 }
 | K_ANTENNAMETALAREA NUMBER opt_layer_name ';'
 { /* a pre 5.4 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum >= 5.4) {
 if (use5_4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAMETALAREA is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaMetalArea( &(lefrPin),
 $2,
$3) ;
 }
 | K_ANTENNAMETALLENGTH NUMBER opt_layer_name ';'
 { /* a pre 5.4 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum >= 5.4) {
 if (use5_4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAMETALLENGTH is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaMetalLength( &(lefrPin),
 $2,
$3) ;
 }
 | K_RISESLEWLIMIT NUMBER ';'
 { if (lefrPinCbk) lefiPin_setRiseSlewLimit( &(lefrPin),
 $2) ;
 }
 | K_FALLSLEWLIMIT NUMBER ';'
 { if (lefrPinCbk) lefiPin_setFallSlewLimit( &(lefrPin),
 $2) ;
 }
 | K_ANTENNAPARTIALMETALAREA NUMBER opt_layer_name ';'
 { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAPARTIALMETALAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAPARTIALMETALAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaPartialMetalArea( &(lefrPin),
 $2,
$3) ;
 }
 | K_ANTENNAPARTIALMETALSIDEAREA NUMBER opt_layer_name ';'
 { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAPARTIALMETALSIDEAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAPARTIALMETALSIDEAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaPartialMetalSideArea( &(lefrPin),
 $2,
$3) ;
 }
 | K_ANTENNAPARTIALCUTAREA NUMBER opt_layer_name ';'
 { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAPARTIALCUTAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAPARTIALCUTAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaPartialCutArea( &(lefrPin),
 $2,
$3) ;
 }
 | K_ANTENNADIFFAREA NUMBER opt_layer_name ';'
 { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNADIFFAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNADIFFAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaDiffArea( &(lefrPin),
 $2,
$3) ;
 }
 | K_ANTENNAGATEAREA NUMBER opt_layer_name ';'
 { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAGATEAREA is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAGATEAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaGateArea( &(lefrPin),
 $2,
$3) ;
 }
 | K_ANTENNAMAXAREACAR NUMBER req_layer_name ';'
 { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAMAXAREACAR is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAMAXAREACAR is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaMaxAreaCar( &(lefrPin),
 $2,
$3) ;
 }
 | K_ANTENNAMAXSIDEAREACAR NUMBER req_layer_name ';'
 { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAMAXSIDEAREACAR is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAMAXSIDEAREACAR is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaMaxSideAreaCar( &(lefrPin),
 $2,
$3) ;
 }
 | K_ANTENNAMAXCUTCAR NUMBER req_layer_name ';'
 { /* 5.4 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.4) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAMAXCUTCAR is a 5.4 syntax. Your lef file is not defined as 5.4");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAMAXCUTCAR is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrPinCbk) lefiPin_addAntennaMaxCutCar( &(lefrPin),
 $2,
$3) ;
 }
 | K_ANTENNAMODEL
 { /* 5.5 syntax */

 use5_4 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum < 5.5) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("ANTENNAMODEL is a 5.5 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 }
 pin_layer_oxide ';'
 | K_NETEXPR {lefDumbMode = 2; lefNoNum = 2; } QSTRING ';'
 {
 if (versionNum < 5.6) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("NETEXPR is a 5.6 syntax. Your lef file is not defined as 5.6") ;
 CHKERR();
 }
 }
 } else
 if (lefrPinCbk) lefiPin_setNetExpr( &(lefrPin),
 $3) ;
 }
 | K_SUPPLYSENSITIVITY {lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
 {
 if (versionNum < 5.6) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("SUPPLYSENSITIVITY is a 5.6 syntax. Your lef file is not defined as 5.6") ;
 CHKERR();
 }
 }
 } else
 if (lefrPinCbk) lefiPin_setSupplySensitivity( &(lefrPin),
 $3) ;
 }
 | K_GROUNDSENSITIVITY {lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
 {
 if (versionNum < 5.6) {
 if (lefrPinCbk) { /* write error only if cbk is set */

 if (pinWarnings++ < lefrPinWarnings) {
 yyerror("GROUNDSENSITIVITY is a 5.6 syntax. Your lef file is not defined as 5.6") ;
 CHKERR();
 }
 }
 } else
 if (lefrPinCbk) lefiPin_setGroundSensitivity( &(lefrPin),
 $3) ;
 }
 
 pin_layer_oxide:
 K_OXIDE1
 {
 if (lefrPinCbk)
 lefiPin_addAntennaModel( &(lefrPin),
 1) ;
 }
 | K_OXIDE2
 {
 if (lefrPinCbk)
 lefiPin_addAntennaModel( &(lefrPin),
 2) ;
 }
 | K_OXIDE3
 {
 if (lefrPinCbk)
 lefiPin_addAntennaModel( &(lefrPin),
 3) ;
 }
 | K_OXIDE4
 {
 if (lefrPinCbk)
 lefiPin_addAntennaModel( &(lefrPin),
 4) ;
 }
 
 pin_prop_list:
 pin_name_value_pair
 | pin_prop_list pin_name_value_pair
 ;
 
 pin_name_value_pair:
 T_STRING NUMBER
 {
 char temp[32];
 sprintf(temp, "%.11g", $2);
 if (lefrPinCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrPinProp),
 $1) ;
 lefiPin_setNumProperty( &(lefrPin),
 $1,
$2,
temp,
propTp) ;
 }
 }
 | T_STRING QSTRING
 {
 if (lefrPinCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrPinProp),
 $1) ;
 lefiPin_setProperty( &(lefrPin),
 $1,
$2,
propTp) ;
 }
 }
 | T_STRING T_STRING
 {
 if (lefrPinCbk) {
 char propTp;
 propTp = lefiPropType_propType( &(lefrPinProp),
 $1) ;
 lefiPin_setProperty( &(lefrPin),
 $1,
$2,
propTp) ;
 }
 }
 
 electrical_direction:
 K_DIRECTION K_INPUT ';' {$$ = (char*)"INPUT";}
 | K_DIRECTION K_OUTPUT ';' {$$ = (char*)"OUTPUT";}
 | K_DIRECTION K_OUTPUT K_TRISTATE ';' {$$ = (char*)"OUTPUT TRISTATE";}
 | K_DIRECTION K_INOUT ';' {$$ = (char*)"INOUT";}
 | K_DIRECTION K_FEEDTHRU ';' {$$ = (char*)"FEEDTHRU";}
 
 start_macro_port: K_PORT
 {
 if (lefrPinCbk) {
 	lefrDoGeometries = 1;
 hasPRP = 0;
 	lefrGeometriesPtr = (lefiGeometries*)lefMalloc( sizeof(lefiGeometries));
 lefiGeometries_Init( 	lefrGeometriesPtr ) ;
 }
 needGeometry = 0; /* don't need rect/path/poly define yet
 */
}
 
 macro_port_class_option: /* empty */

 | K_CLASS class_type ';'
 { if (lefrDoGeometries)
 lefiGeometries_addClass( lefrGeometriesPtr,
 $2) ;
 }
 
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
 /* empty */
{$$ = (char*)""; } /* non-ring shape */

 | K_ABUTMENT	{$$ = (char*)"ABUTMENT";}
 | K_RING 	{$$ = (char*)"RING";}
 | K_FEEDTHRU	{$$ = (char*)"FEEDTHRU";}
 
 geometries: geometry geometry_options
 
 geometry:
 K_LAYER {lefDumbMode = 1; lefNoNum = 1; } T_STRING
 {
 if ((needGeometry) && (needGeometry != 2)) /* 1 LAYER follow after another
 */
if (layerWarnings++ < lefrLayerWarnings)
 yywarning("Either PATH, RECT or POLYGON is a required in MACRO/PIN/PORT.");
 if (lefrDoGeometries)
 lefiGeometries_addLayer( lefrGeometriesPtr,
 $3) ;
 needGeometry = 1; /* within LAYER it requires either path, rect, poly
 */
}
 layer_spacing ';'
 | K_WIDTH NUMBER ';'
 { if (lefrDoGeometries)
 lefiGeometries_addWidth( lefrGeometriesPtr,
 $2) ;
 }
 | K_PATH firstPt otherPts ';'
 { if (lefrDoGeometries)
 lefiGeometries_addPath( lefrGeometriesPtr ) ;
 hasPRP = 1;
 needGeometry = 2;
 }
 | K_PATH K_ITERATE firstPt otherPts stepPattern ';'
 { if (lefrDoGeometries)
 lefiGeometries_addPathIter( lefrGeometriesPtr ) ;
 hasPRP = 1;
 needGeometry = 2;
 }
 | K_RECT pt pt';'
 { if (lefrDoGeometries)
 lefiGeometries_addRect( lefrGeometriesPtr,
 $2.x,
$2.y,
$3.x,
$3.y) ;
 needGeometry = 2;
 }
 | K_RECT K_ITERATE pt pt stepPattern ';'
 { if (lefrDoGeometries)
 lefiGeometries_addRectIter( lefrGeometriesPtr,
 $3.x,
$3.y,
$4.x,
$4.y) ;
 needGeometry = 2;
 }
 | K_POLYGON firstPt nextPt nextPt nextPt otherPts ';'
 {
 if (lefrDoGeometries)
 lefiGeometries_addPolygon( lefrGeometriesPtr ) ;
 hasPRP = 1;
 needGeometry = 2;
 }
 | K_POLYGON K_ITERATE firstPt nextPt nextPt nextPt otherPts stepPattern ';'
 { if (lefrDoGeometries)
 lefiGeometries_addPolygonIter( lefrGeometriesPtr ) ;
 hasPRP = 1;
 needGeometry = 2;
 }
 | via_placement
 { }
 
 geometry_options: /* empty */

 | geometry_options geometry
 
 layer_spacing: /* empty */

 | K_SPACING NUMBER
 { if (lefrDoGeometries) {
 if (zeroOrGt($2))
 lefiGeometries_addLayerMinSpacing( lefrGeometriesPtr,
 $2) ;
 else {
 yyerror("SPACING value in MACRO OBS has to be 0 or greater");
 CHKERR();
 }
 }
 }
 | K_DESIGNRULEWIDTH NUMBER
 { if (lefrDoGeometries) {
 if (zeroOrGt($2))
 lefiGeometries_addLayerRuleWidth( lefrGeometriesPtr,
 $2) ;
 else {
 yyerror("DESIGNRULEWIDTH value in MACRO OBS has to be 0 or greater");
 CHKERR();
 }
 }
 }
 
 firstPt: pt
 { if (lefrDoGeometries)
 lefiGeometries_startList( lefrGeometriesPtr,
 $1.x,
$1.y) ;
 }
 
 nextPt: pt
 { if (lefrDoGeometries)
 lefiGeometries_addToList( lefrGeometriesPtr,
 $1.x,
$1.y) ;
 }
 
 otherPts:
 /* empty */

 /* switch to avoid stack overflow with large polygon
 | nextPt otherPts
 */

 | otherPts nextPt
 ;
 
 via_placement:
 K_VIA pt {lefDumbMode = 1;} T_STRING ';'
 { if (lefrDoGeometries)
 lefiGeometries_addVia( lefrGeometriesPtr,
 $2.x,
$2.y,
$4) ;
 }
 | K_VIA K_ITERATE pt {lefDumbMode = 1; lefNoNum = 1;} T_STRING
 stepPattern ';'
 { if (lefrDoGeometries)
 lefiGeometries_addViaIter( lefrGeometriesPtr,
 $3.x,
$3.y,
$5) ;
 }
 
 
 stepPattern: K_DO NUMBER K_BY NUMBER K_STEP NUMBER NUMBER
 { if (lefrDoGeometries)
 lefiGeometries_addStepPattern( lefrGeometriesPtr,
 $2,
$4,
$6,
$7) ;
 }
 
 sitePattern: T_STRING NUMBER NUMBER orientation
 K_DO NUMBER K_BY NUMBER K_STEP NUMBER NUMBER
 {
 if (lefrDoSite) {
 	lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
 				 sizeof(lefiSitePattern));
 lefiSitePattern_Init( 	lefrSitePatternPtr ) ;
 lefiSitePattern_set( 	lefrSitePatternPtr,
 $1,
$2,
$3,
$4,
$6,
$8,
$10,
$11) ;
 	}
 }
 | T_STRING NUMBER NUMBER orientation
 {
 if (lefrDoSite) {
 	lefrSitePatternPtr = (lefiSitePattern*)lefMalloc(
 				 sizeof(lefiSitePattern));
 lefiSitePattern_Init( 	lefrSitePatternPtr ) ;
 lefiSitePattern_set( 	lefrSitePatternPtr,
 $1,
$2,
$3,
$4,
-1,
-1,
-1,
-1) ;
 	}
 }
 
 trackPattern:
 K_X NUMBER K_DO NUMBER K_STEP NUMBER
 {
 if (lefrDoTrack) {
 	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
 				sizeof(lefiTrackPattern));
 lefiTrackPattern_Init( 	lefrTrackPatternPtr ) ;
 lefiTrackPattern_set( 	lefrTrackPatternPtr,
 "X",
$2,
(int)$4,$6) ;
 }
 }
 K_LAYER {lefDumbMode = 1000000000;} trackLayers
 { lefDumbMode = 0;}
 | K_Y NUMBER K_DO NUMBER K_STEP NUMBER
 {
 if (lefrDoTrack) {
 	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
 sizeof(lefiTrackPattern));
 lefiTrackPattern_Init( 	lefrTrackPatternPtr ) ;
 lefiTrackPattern_set( 	lefrTrackPatternPtr,
 "Y",
$2,
(int)$4,$6) ;
 }
 }
 K_LAYER {lefDumbMode = 1000000000;} trackLayers
 { lefDumbMode = 0;}
 | K_X NUMBER K_DO NUMBER K_STEP NUMBER
 {
 if (lefrDoTrack) {
 	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
 sizeof(lefiTrackPattern));
 lefiTrackPattern_Init( 	lefrTrackPatternPtr ) ;
 lefiTrackPattern_set( 	lefrTrackPatternPtr,
 "X",
$2,
(int)$4,$6) ;
 }
 }
 | K_Y NUMBER K_DO NUMBER K_STEP NUMBER
 {
 if (lefrDoTrack) {
 	lefrTrackPatternPtr = (lefiTrackPattern*)lefMalloc(
 sizeof(lefiTrackPattern));
 lefiTrackPattern_Init( 	lefrTrackPatternPtr ) ;
 lefiTrackPattern_set( 	lefrTrackPatternPtr,
 "Y",
$2,
(int)$4,$6) ;
 }
 }
 
 trackLayers:
 /* empty */

 | trackLayers layer_name
 ;
 
 layer_name: T_STRING
 { if (lefrDoTrack) lefiTrackPattern_addLayer( lefrTrackPatternPtr,
 $1) ;
 }
 
 gcellPattern: K_X NUMBER K_DO NUMBER K_STEP NUMBER
 {
 if (lefrDoGcell) {
 	lefrGcellPatternPtr = (lefiGcellPattern*)lefMalloc(
 sizeof(lefiGcellPattern));
 lefiGcellPattern_Init( 	lefrGcellPatternPtr ) ;
 lefiGcellPattern_set( 	lefrGcellPatternPtr,
 "X",
$2,
(int)$4,$6) ;
 }
 }
 | K_Y NUMBER K_DO NUMBER K_STEP NUMBER
 {
 if (lefrDoGcell) {
 	lefrGcellPatternPtr = (lefiGcellPattern*)lefMalloc(
 sizeof(lefiGcellPattern));
 lefiGcellPattern_Init( 	lefrGcellPatternPtr ) ;
 lefiGcellPattern_set( 	lefrGcellPatternPtr,
 "Y",
$2,
(int)$4,$6) ;
 }
 }
 
 macro_obs: start_macro_obs geometries K_END
 {
 if (lefrObstructionCbk) {
 lefiObstruction_setGeometries( &(	lefrObstruction),
 lefrGeometriesPtr) ;
 	lefrGeometriesPtr = 0;
 	lefrDoGeometries = 0;
 CALLBACK(lefrObstructionCbk, lefrObstructionCbkType, &lefrObstruction);
 }
 lefDumbMode = 0;
 }
 | start_macro_obs K_END
 /* 08/14/00 - Wanda da Rosa. Allow empty OBS due to so many lef files
 * have empty OBS
 {
 yyerror("OBS requires either a LAYER or a VIA.");
 CHKERR();
 }
 */

 {
 /* The pointer has malloced in start, need to free manually */

 if (lefrGeometriesPtr) {
 lefiGeometries_Destroy( lefrGeometriesPtr ) ;
 lefFree(lefrGeometriesPtr);
 }
 }
 
 start_macro_obs: K_OBS
 {
 obsDef = 1;
 if (lefrObstructionCbk) {
 	lefrDoGeometries = 1;
 	lefrGeometriesPtr = (lefiGeometries*)lefMalloc(
 	 sizeof(lefiGeometries));
 lefiGeometries_Init( 	lefrGeometriesPtr ) ;
 	}
 }
 
 macro_density: K_DENSITY density_layer density_layers K_END
 {
 if (versionNum < 5.6) {
 if (lefrDensityCbk) { /* write error only if cbk is set */

 if (macroWarnings++ < lefrMacroWarnings) {
 yyerror("DENSITY is a 5.6 syntax. Your lef file is not defined as 5.6");
 CHKERR();
 }
 }
 } else
 if (lefrDensityCbk) {
 CALLBACK(lefrDensityCbk, lefrDensityCbkType, &lefrDensity);
 lefiDensity_clear( &(lefrDensity) ) ;
 }
 lefDumbMode = 0;
 }
 
 density_layers: /* empty */

 | density_layers density_layer
 ;
 
 density_layer: K_LAYER { lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
 {
 if (lefrDensityCbk)
 lefiDensity_addLayer( &(lefrDensity),
 $3) ;
 }
 density_layer_rect density_layer_rects
 
 density_layer_rects: /* empty */

 | density_layer_rects density_layer_rect
 ;
 
 density_layer_rect: K_RECT pt pt NUMBER ';'
 {
 if (lefrDensityCbk)
 lefiDensity_addRect( &(lefrDensity),
 $2.x,
$2.y,
$3.x,
$3.y,
$4) ;
 }
 
 macro_clocktype: K_CLOCKTYPE { lefDumbMode = 1; lefNoNum = 1; } T_STRING ';'
 { if (lefrMacroCbk) lefiMacro_setClockType( &(lefrMacro),
 $3) ;
 }
 
 timing: start_timing timing_options end_timing
 { }
 
 start_timing: K_TIMING
 { /* XXXXX for macros */
}
 
 end_timing: K_END K_TIMING
 {
 if (versionNum < 5.4) {
 if (lefrTimingCbk && lefiTiming_hasData( &(lefrTiming) ) )  CALLBACK(lefrTimingCbk, lefrTimingCbkType, &lefrTiming) ;
 lefiTiming_clear( &(lefrTiming) ) ;
 } else {
 if (lefrTimingCbk) /* write warning only if cbk is set */

 if (timingWarnings++ < lefrTimingWarnings)
 yywarning("MACRO TIMING is obsolete in 5.4. It will be ignored.");
 lefiTiming_clear( &(lefrTiming) ) ;
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
 if (lefrTimingCbk && lefiTiming_hasData( &(lefrTiming) ) )  CALLBACK(lefrTimingCbk, lefrTimingCbkType, &lefrTiming) ;
 }
 lefDumbMode = 1000000000;
 lefiTiming_clear( &(lefrTiming) ) ;
 }
 list_of_from_strings ';'
 { lefDumbMode = 0;}
 | K_TOPIN {lefDumbMode = 1000000000;} list_of_to_strings ';'
 { lefDumbMode = 0;}
 | risefall K_INTRINSIC NUMBER NUMBER
 { if (lefrTimingCbk) lefiTiming_addRiseFall( &(lefrTiming),
 $1,
$3,
$4) ;
 }
 slew_spec K_VARIABLE NUMBER NUMBER ';'
 { if (lefrTimingCbk) lefiTiming_addRiseFallVariable( &(lefrTiming),
 $8,
$9) ;
 }
 | risefall delay_or_transition K_UNATENESS unateness
 K_TABLEDIMENSION NUMBER NUMBER NUMBER ';'
 { if (lefrTimingCbk) {
 	if ($2[0] == 'D' || $2[0] == 'd') /* delay */

 	 lefiTiming_addDelay( &(lefrTiming),
 $1,
$4,
$6,
$7,
$8) ;
 	else
 	 lefiTiming_addTransition( &(lefrTiming),
 $1,
$4,
$6,
$7,
$8) ;
 }
 }
 | K_TABLEAXIS list_of_table_axis_numbers ';'
 { }
 | K_TABLEENTRIES list_of_table_entries ';'
 { }
 | K_RISERS NUMBER NUMBER ';'
 { if (lefrTimingCbk) lefiTiming_setRiseRS( &(lefrTiming),
 $2,
$3) ;
 }
 | K_FALLRS NUMBER NUMBER ';'
 { if (lefrTimingCbk) lefiTiming_setFallRS( &(lefrTiming),
 $2,
$3) ;
 }
 | K_RISECS NUMBER NUMBER ';'
 { if (lefrTimingCbk) lefiTiming_setRiseCS( &(lefrTiming),
 $2,
$3) ;
 }
 | K_FALLCS NUMBER NUMBER ';'
 { if (lefrTimingCbk) lefiTiming_setFallCS( &(lefrTiming),
 $2,
$3) ;
 }
 | K_RISESATT1 NUMBER NUMBER ';'
 { if (lefrTimingCbk) lefiTiming_setRiseAtt1( &(lefrTiming),
 $2,
$3) ;
 }
 | K_FALLSATT1 NUMBER NUMBER ';'
 { if (lefrTimingCbk) lefiTiming_setFallAtt1( &(lefrTiming),
 $2,
$3) ;
 }
 | K_RISET0 NUMBER NUMBER ';'
 { if (lefrTimingCbk) lefiTiming_setRiseTo( &(lefrTiming),
 $2,
$3) ;
 }
 | K_FALLT0 NUMBER NUMBER ';'
 { if (lefrTimingCbk) lefiTiming_setFallTo( &(lefrTiming),
 $2,
$3) ;
 }
 | K_UNATENESS unateness ';'
 { if (lefrTimingCbk) lefiTiming_addUnateness( &(lefrTiming),
 $2) ;
 }
 | K_STABLE K_SETUP NUMBER K_HOLD NUMBER risefall ';'
 { if (lefrTimingCbk) lefiTiming_setStable( &(lefrTiming),
 $3,
$5,
$6) ;
 }
 | two_pin_trigger from_pin_trigger to_pin_trigger K_TABLEDIMENSION NUMBER NUMBER NUMBER ';'
 { if (lefrTimingCbk) lefiTiming_addSDF2Pins( &(lefrTiming),
 $1,
$2,
$3,
$5,
$6,
$7) ;
 }
 | one_pin_trigger K_TABLEDIMENSION NUMBER NUMBER NUMBER ';'
 { if (lefrTimingCbk) lefiTiming_addSDF1Pin( &(lefrTiming),
 $1,
$3,
$4,
$4) ;
 }
 | K_SDFCONDSTART QSTRING ';'
 { if (lefrTimingCbk) lefiTiming_setSDFcondStart( &(lefrTiming),
 $2) ;
 }
 | K_SDFCONDEND QSTRING ';'
 { if (lefrTimingCbk) lefiTiming_setSDFcondEnd( &(lefrTiming),
 $2) ;
 }
 | K_SDFCOND QSTRING ';'
 { if (lefrTimingCbk) lefiTiming_setSDFcond( &(lefrTiming),
 $2) ;
 }
 | K_EXTENSION ';'
 { /* XXXXX */
}
 
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
 { if (lefrTimingCbk) lefiTiming_addTableEntry( &(lefrTiming),
 $2,
$3,
$4) ;
 }
 
 list_of_table_axis_numbers:
 NUMBER
 { if (lefrTimingCbk) lefiTiming_addTableAxisNumber( &(lefrTiming),
 $1) ;
 }
 | list_of_table_axis_numbers NUMBER
 { if (lefrTimingCbk) lefiTiming_addTableAxisNumber( &(lefrTiming),
 $2) ;
 }
 
 slew_spec:
 /* empty */

 { }
 | NUMBER NUMBER NUMBER NUMBER
 { if (lefrTimingCbk) lefiTiming_addRiseFallSlew( &(lefrTiming),
 $1,
$2,
$3,
$4) ;
 }
 | NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER
 { if (lefrTimingCbk) lefiTiming_addRiseFallSlew( &(lefrTiming),
 $1,
$2,
$3,
$4) ;
 if (lefrTimingCbk) lefiTiming_addRiseFallSlew2( &(lefrTiming),
 $5,
$6,
$7) ;
 }
 
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
 { if (lefrTimingCbk) lefiTiming_addFromPin( &(lefrTiming),
 $1) ;
 }
 | list_of_from_strings T_STRING
 { if (lefrTimingCbk) lefiTiming_addFromPin( &(lefrTiming),
 $2) ;
 }
 
 list_of_to_strings:
 T_STRING
 { if (lefrTimingCbk) lefiTiming_addToPin( &(lefrTiming),
 $1) ;
 }
 | list_of_to_strings T_STRING
 { if (lefrTimingCbk) lefiTiming_addToPin( &(lefrTiming),
 $2) ;
 }
 
 array: start_array array_rules
 {
 if (lefrArrayCbk)
 	CALLBACK(lefrArrayCbk, lefrArrayCbkType, &lefrArray);
 lefiArray_clear( &(lefrArray) ) ;
 lefrSitePatternPtr = 0;
 lefrDoSite = 0;
 }
 end_array
 
 start_array: K_ARRAY {lefDumbMode = 1; lefNoNum = 1;} T_STRING
 {
 if (lefrArrayCbk) {
 lefiArray_setName( &(	lefrArray),
 $3) ;
 	CALLBACK(lefrArrayBeginCbk, lefrArrayBeginCbkType, $3);
 }
 /* $3);
  */
arrayName = strdup($3);
 }
 
 end_array: K_END {lefDumbMode = 1; lefNoNum = 1;} T_STRING
 {
 if (lefrArrayCbk && lefrArrayEndCbk)
 	CALLBACK(lefrArrayEndCbk, lefrArrayEndCbkType, $3);
 if (strcmp(arrayName, $3) != 0) {
 lefFree(arrayName);
 if (lefrArrayCbk) { /* write error only if cbk is set */

 if (arrayWarnings++ < lefrArrayWarnings) {
 yyerror("END ARRAY name is different from ARRAY");
 CHKERR();
 }
 }
 } else
 lefFree(arrayName);
 }
 
 array_rules:
 /* empty */

 { }
 | array_rules array_rule
 { }
 
 array_rule:
 site_word { if (lefrArrayCbk) lefrDoSite = 1; lefDumbMode = 1; }
 sitePattern ';'
 {
 if (lefrArrayCbk) {
 lefiArray_addSitePattern( &(	lefrArray),
 lefrSitePatternPtr) ;
 }
 }
 | K_CANPLACE {lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; }
 sitePattern ';'
 {
 if (lefrArrayCbk) {
 lefiArray_addCanPlace( &(	lefrArray),
 lefrSitePatternPtr) ;
 }
 }
 | K_CANNOTOCCUPY {lefDumbMode = 1; if (lefrArrayCbk) lefrDoSite = 1; }
 sitePattern ';'
 {
 if (lefrArrayCbk) {
 lefiArray_addCannotOccupy( &(	lefrArray),
 lefrSitePatternPtr) ;
 }
 }
 | K_TRACKS { if (lefrArrayCbk) lefrDoTrack = 1; } trackPattern ';'
 {
 if (lefrArrayCbk) {
 lefiArray_addTrack( &(	lefrArray),
 lefrTrackPatternPtr) ;
 }
 }
 | floorplan_start floorplan_list K_END T_STRING
 {
 }
 | K_GCELLGRID { if (lefrArrayCbk) lefrDoGcell = 1; } gcellPattern ';'
 {
 if (lefrArrayCbk) {
 lefiArray_addGcell( &(	lefrArray),
 lefrGcellPatternPtr) ;
 }
 }
 | K_DEFAULTCAP NUMBER cap_list K_END K_DEFAULTCAP
 {
 if (lefrArrayCbk) {
 lefiArray_setTableSize( &(	lefrArray),
 (int)$2) ;
 }
 }
 | def_statement
 { }
 
 floorplan_start: K_FLOORPLAN T_STRING
 { if (lefrArrayCbk) lefiArray_addFloorPlan( &(lefrArray),
 $2) ;
 }
 
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
 lefiArray_addSiteToFloorPlan( &(	lefrArray),
 "CANPLACE",
	lefrSitePatternPtr) ;
 }
 | K_CANNOTOCCUPY { if (lefrArrayCbk) lefrDoSite = 1; lefDumbMode = 1; }
 sitePattern ';'
 {
 if (lefrArrayCbk)
 lefiArray_addSiteToFloorPlan( &(	lefrArray),
 "CANNOTOCCUPY",
	lefrSitePatternPtr) ;
 }
 
 cap_list:
 /* empty */

 { }
 | cap_list one_cap
 { }
 
 one_cap: K_MINPINS NUMBER K_WIRECAP NUMBER ';'
 { if (lefrArrayCbk) lefiArray_addDefaultCap( &(lefrArray),
 (int)$2,$4) ;
 }
 
 msg_statement:
 K_MESSAGE {lefDumbMode=1;lefNlToken=TRUE;} T_STRING '=' s_expr dtrm
 { lefAddStringMessage($3, $5); }
 
 create_file_statement:
 K_CREATEFILE {lefDumbMode=1;lefNlToken=TRUE;} T_STRING '=' s_expr dtrm
 { }
 
 def_statement:
 K_DEFINE {lefDumbMode=1;lefNlToken=TRUE;} T_STRING '=' expression dtrm
 {
 if (versionNum < 5.6)
 lefAddNumDefine($3, $5);
 else
 if (lefrArrayCbk) /* write warning only if cbk is set */

 if (arrayWarnings++ < lefrArrayWarnings)
 yywarning("DEFINE is obsolete in 5.6. It will be ignored.");
 }
 | K_DEFINES {lefDumbMode=1;lefNlToken=TRUE;} T_STRING '=' s_expr dtrm
 {
 if (versionNum < 5.6)
 lefAddStringDefine($3, $5);
 else
 if (lefrArrayCbk) /* write warning only if cbk is set */

 if (arrayWarnings++ < lefrArrayWarnings)
 yywarning("DEFINES is obsolete in 5.6. It will be ignored.");
 }
 | K_DEFINEB {lefDumbMode=1;lefNlToken=TRUE;} T_STRING '=' b_expr dtrm
 {
 if (versionNum < 5.6)
 lefAddBooleanDefine($3, $5);
 else
 if (lefrArrayCbk) /* write warning only if cbk is set */

 if (arrayWarnings++ < lefrArrayWarnings)
 yywarning("DEFINEB is obsolete in 5.6. It will be ignored.");
 }
 
 /* terminator for &defines. Can be semicolon or newline */

 dtrm:
 | ';' {lefNlToken = FALSE;}
 | '\n'	{lefNlToken = FALSE;}
 
 then:
 K_THEN
 | '\n' K_THEN
 ;
 
 else:
 K_ELSE
 | '\n' K_ELSE
 ;
 
 expression:
 expression '+' expression {$$ = $1 + $3; }
 | expression '-' expression {$$ = $1 - $3; }
 | expression '*' expression {$$ = $1 * $3; }
 | expression '/' expression {$$ = $1 / $3; }
 | '-' expression %prec UMINUS {$$ = -$2;}
 | '(' expression ')'		{$$ = $2;}
 | K_IF b_expr then expression else expression %prec IF
 		{$$ = ($2 != 0) ? $4 : $6;}
 | NUMBER			{$$ = $1;}
 
 b_expr:
 expression relop expression {$$ = comp_num($1,$2,$3);}
 | expression K_AND expression {$$ = $1 != 0 && $3 != 0;}
 | expression K_OR expression {$$ = $1 != 0 || $3 != 0;}
 | s_expr relop s_expr	 {$$ = comp_str($1,$2,$3);}
 | s_expr K_AND s_expr	 {$$ = $1[0] != 0 && $3[0] != 0;}
 | s_expr K_OR s_expr	 {$$ = $1[0] != 0 || $3[0] != 0;}
 | b_expr K_EQ b_expr	 {$$ = $1 == $3;}
 | b_expr K_NE b_expr	 {$$ = $1 != $3;}
 | b_expr K_AND b_expr	 {$$ = $1 && $3;}
 | b_expr K_OR b_expr	 {$$ = $1 || $3;}
 | K_NOT b_expr		 %prec LNOT {$$ = !$$;}
 | '(' b_expr ')'	 {$$ = $2;}
 | K_IF b_expr then b_expr else b_expr %prec IF
 	 {$$ = ($2 != 0) ? $4 : $6;}
 | K_TRUE		 {$$ = 1;}
 | K_FALSE		 {$$ = 0;}
 
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
 | '=' {$$ = C_EQ;}
 | '<' {$$ = C_LT;}
 | '>' {$$ = C_GT;}
 
 
 prop_def_section: K_PROPDEF
 {
 if (lefrPropBeginCbk)
 CALLBACK(lefrPropBeginCbk, lefrPropBeginCbkType, 0);
 }
 prop_stmts K_END K_PROPDEF
 {
 if (lefrPropEndCbk)
 CALLBACK(lefrPropEndCbk, lefrPropEndCbkType, 0);
 lefRealNum = 0; /* just want to make sure it is reset */

 }
 
 prop_stmts:
 /* empty */

 { }
 | prop_stmts prop_stmt
 { }
 
 prop_stmt:
 K_LIBRARY {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 }
 T_STRING prop_define ';'
 {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "library",
$3) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrLibProp),
 $3,
lefPropDefType) ;
 }
 | K_COMPONENTPIN {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 }
 T_STRING prop_define ';'
 {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "componentpin",
$3) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrCompProp),
 $3,
lefPropDefType) ;
 }
 | K_PIN {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 }
 T_STRING prop_define ';'
 {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "pin",
$3) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrPinProp),
 $3,
lefPropDefType) ;
 
 }
 | K_MACRO {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 }
 T_STRING prop_define ';'
 {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "macro",
$3) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrMacroProp),
 $3,
lefPropDefType) ;
 }
 | K_VIA {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 }
 T_STRING prop_define ';'
 {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "via",
$3) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrViaProp),
 $3,
lefPropDefType) ;
 }
 | K_VIARULE {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 }
 T_STRING prop_define ';'
 {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "viarule",
$3) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrViaRuleProp),
 $3,
lefPropDefType) ;
 }
 | K_LAYER {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 }
 T_STRING prop_define ';'
 {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "layer",
$3) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrLayerProp),
 $3,
lefPropDefType) ;
 }
 | K_NONDEFAULTRULE {lefDumbMode = 1; lefiProp_clear( &(lefrProp) ) ;
 }
 T_STRING prop_define ';'
 {
 if (lefrPropCbk) {
 lefiProp_setPropType( &(lefrProp),
 "nondefaultrule",
$3) ;
 CALLBACK(lefrPropCbk, lefrPropCbkType, &lefrProp);
 }
 lefiPropType_setPropType( &(lefrNondefProp),
 $3,
lefPropDefType) ;
 }
 
 prop_define:
 K_INTEGER { lefRealNum = 0; } opt_def_range opt_def_value
 {
 if (lefrPropCbk) lefiProp_setPropInteger( &(lefrProp) ) ;
 lefPropDefType = 'I';
 }
 | K_REAL { lefRealNum = 1; } opt_def_range opt_def_value
 {
 if (lefrPropCbk) lefiProp_setPropReal( &(lefrProp) ) ;
 lefPropDefType = 'R';
 lefRealNum = 0;
 }
 | K_STRING
 {
 if (lefrPropCbk) lefiProp_setPropString( &(lefrProp) ) ;
 lefPropDefType = 'S';
 }
 | K_STRING QSTRING
 {
 if (lefrPropCbk) lefiProp_setPropQString( &(lefrProp),
 $2) ;
 lefPropDefType = 'Q';
 }
 | K_NAMEMAPSTRING T_STRING
 {
 if (lefrPropCbk) lefiProp_setPropNameMapString( &(lefrProp),
 $2) ;
 lefPropDefType = 'S';
 }
 
 
 opt_range_detail:
 /* empty */

 {
 }
 | K_RANGE NUMBER NUMBER
 {
 if (lefrLayerCbk)
 lefiLayer_setSpacingRange( &(lefrLayer),
 $2,
$3) ;
 }
 opt_range_second
 | K_LENGTHTHRESHOLD NUMBER
 {
 if (lefrLayerCbk) {
 lefiLayer_setSpacingLength( &(lefrLayer),
 $2) ;
 }
 }
 | K_LENGTHTHRESHOLD NUMBER K_RANGE NUMBER NUMBER
 {
 if (lefrLayerCbk) {
 lefiLayer_setSpacingLength( &(lefrLayer),
 $2) ;
 lefiLayer_setSpacingLengthRange( &(lefrLayer),
 $4,
$5) ;
 }
 }
 
 opt_range_second:
 /* nothing */

 { }
 | K_USELENGTHTHRESHOLD
 {
 if (lefrLayerCbk)
 lefiLayer_setSpacingRangeUseLength( &(lefrLayer) ) ;
 }
 | K_INFLUENCE NUMBER
 {
 if (lefrLayerCbk) {
 lefiLayer_setSpacingRangeInfluence( &(lefrLayer),
 $2) ;
 lefiLayer_setSpacingRangeInfluenceRange( &(lefrLayer),
 -1,
-1) ;
 }
 }
 | K_INFLUENCE NUMBER K_RANGE NUMBER NUMBER
 {
 if (lefrLayerCbk) {
 lefiLayer_setSpacingRangeInfluence( &(lefrLayer),
 $2) ;
 lefiLayer_setSpacingRangeInfluenceRange( &(lefrLayer),
 $4,
$5) ;
 }
 }
 | K_RANGE NUMBER NUMBER
 {
 if (lefrLayerCbk)
 lefiLayer_setSpacingRangeRange( &(lefrLayer),
 $2,
$3) ;
 }
 
 opt_def_range:
 /* nothing */

 { }
 | K_RANGE NUMBER NUMBER
 { if (lefrPropCbk) lefiProp_setRange( &(lefrProp),
 $2,
$3) ;
 }
 
 opt_def_value:
 /* empty */

 { }
 | NUMBER
 { if (lefrPropCbk) lefiProp_setNumber( &(lefrProp),
 $1) ;
 }
 
 opt_spacing_cut:
 /* empty */

 | K_LAYER {lefDumbMode = 1; lefNoNum = 1; } T_STRING
 {
 if (lefrLayerCbk)
 lefiLayer_setSpacingName( &(lefrLayer),
 $3) ;
 }
 | K_ADJACENTCUTS NUMBER K_WITHIN NUMBER
 {
 if (versionNum < 5.5) {
 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("ADJACENTCUTS is a 5.5 syntax. Your lef file is not defined as 5.5");
 CHKERR();
 }
 }
 if (lefrLayerCbk)
 lefiLayer_setSpacingAdjacent( &(lefrLayer),
 (int)$2,$4) ;
 }
 | K_CENTERTOCENTER
 {
 if (versionNum < 5.6) {
 if (layerWarnings++ < lefrLayerWarnings) {
 yyerror("CENTERTOCENTER is a 5.6 syntax. Your lef file is not defined as 5.6");
 CHKERR();
 }
 }
 if (lefrLayerCbk)
 lefiLayer_setSpacingCenterToCenter( &(lefrLayer) ) ;
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
 
 /* 9/11/2001 - Wanda da Rosa. The following are obsolete in 5.4 */

 universalnoisemargin: K_UNIVERSALNOISEMARGIN NUMBER NUMBER ';'
 {
 if (versionNum < 5.4) {
 if (lefrNoiseMarginCbk) {
 lefrNoiseMargin.low = $2;
 lefrNoiseMargin.high = $3;
 CALLBACK(lefrNoiseMarginCbk, lefrNoiseMarginCbkType, &lefrNoiseMargin);
 }
 } else
 if (lefrNoiseMarginCbk) /* write warning only if cbk is set */

 if (noiseMarginWarnings++ < lefrNoiseMarginWarnings)
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
 if (lefrEdgeRateThreshold1Cbk) /* write warning only if cbk is set */

 if (edgeRateThreshold1Warnings++ < lefrEdgeRateThreshold1Warnings)
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
 if (lefrEdgeRateThreshold2Cbk) /* write warning only if cbk is set */

 if (edgeRateThreshold2Warnings++ < lefrEdgeRateThreshold2Warnings)
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
 if (lefrEdgeRateScaleFactorCbk) /* write warning only if cbk is set */

 if (edgeRateScaleFactorWarnings++ < lefrEdgeRateScaleFactorWarnings)
 yywarning(
 "EDGERATESCALEFACTOR is obsolete in 5.4. It will be ignored.");
 }
 
 noisetable: K_NOISETABLE NUMBER
 { if (lefrNoiseTableCbk) lefiNoiseTable_setup( &(lefrNoiseTable),
 (int)$2) ;
 }
 ';' noise_table_list end_noisetable dtrm
 { }
 
 end_noisetable:
 K_END K_NOISETABLE
 {
 if (versionNum < 5.4) {
 if (lefrNoiseTableCbk)
 CALLBACK(lefrNoiseTableCbk, lefrNoiseTableCbkType, &lefrNoiseTable);
 } else
 if (lefrNoiseTableCbk) /* write warning only if cbk is set */

 if (noiseTableWarnings++ < lefrNoiseTableWarnings)
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
 lefiNoiseTable_newEdge( &(lefrNoiseTable) ) ;
 lefiNoiseTable_addEdge( &(lefrNoiseTable),
 $2) ;
 }
 }
 | output_resistance_entry
 { }
 
 output_resistance_entry: K_OUTPUTRESISTANCE
 { if (lefrNoiseTableCbk) lefiNoiseTable_addResistance( &(lefrNoiseTable) ) ;
 }
 num_list ';' victim_list
 ;
 
 num_list:
 NUMBER
 { if (lefrNoiseTableCbk)
 lefiNoiseTable_addResistanceNumber( &(lefrNoiseTable),
 $1) ;
 }
 | num_list NUMBER
 { if (lefrNoiseTableCbk)
 lefiNoiseTable_addResistanceNumber( &(lefrNoiseTable),
 $2) ;
 }
 
 victim_list:
 victim
 | victim_list victim
 ;
 
 victim: K_VICTIMLENGTH NUMBER ';'
 	{ if (lefrNoiseTableCbk)
 lefiNoiseTable_addVictimLength( &(	lefrNoiseTable),
 $2) ;
 }
 K_VICTIMNOISE vnoiselist ';'
 	{ }
 
 vnoiselist:
 NUMBER
 { if (lefrNoiseTableCbk)
 lefiNoiseTable_addVictimNoise( &(lefrNoiseTable),
 $1) ;
 }
 | vnoiselist NUMBER
 { if (lefrNoiseTableCbk)
 lefiNoiseTable_addVictimNoise( &(lefrNoiseTable),
 $2) ;
 }
 
 correctiontable: K_CORRECTIONTABLE NUMBER ';'
 { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_setup( &(lefrCorrectionTable),
 (int)$2) ;
 }
 correction_table_list end_correctiontable dtrm
 { }
 
 end_correctiontable:
 K_END K_CORRECTIONTABLE
 {
 if (versionNum < 5.4) {
 if (lefrCorrectionTableCbk)
 CALLBACK(lefrCorrectionTableCbk, lefrCorrectionTableCbkType,
 &lefrCorrectionTable);
 } else
 if (lefrCorrectionTableCbk) /* write warning only if cbk is set */

 if (correctionTableWarnings++ < lefrCorrectionTableWarnings)
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
 lefiCorrectionTable_newEdge( &(lefrCorrectionTable) ) ;
 lefiCorrectionTable_addEdge( &(lefrCorrectionTable),
 $2) ;
 }
 }
 | output_list
 { }
 
 output_list: K_OUTPUTRESISTANCE
 { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addResistance( &(lefrCorrectionTable) ) ;
 }
 numo_list ';' corr_victim_list
 { }
 
 numo_list:
 NUMBER
 { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addResistanceNumber( &(lefrCorrectionTable),
 $1) ;
 }
 | numo_list NUMBER
 { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addResistanceNumber( &(lefrCorrectionTable),
 $2) ;
 }
 
 corr_victim_list:
 corr_victim
 | corr_victim_list corr_victim
 ;
 
 corr_victim:
 K_VICTIMLENGTH NUMBER ';'
 { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addVictimLength( &(lefrCorrectionTable),
 $2) ;
 }
 K_CORRECTIONFACTOR corr_list ';'
 { }
 
 corr_list:
 NUMBER
 { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addVictimCorrection( &(	lefrCorrectionTable),
 $1) ;
 }
 | corr_list NUMBER
 { if (lefrCorrectionTableCbk)
 lefiCorrectionTable_addVictimCorrection( &(	lefrCorrectionTable),
 $2) ;
 }
 
 /* end of 5.4 obsolete syntax */

 
 input_antenna: K_INPUTPINANTENNASIZE NUMBER ';'
 { /* 5.3 syntax */

 use5_3 = 1;
 if (ignoreVersion) {
 /* do nothing */

 } else if (versionNum > 5.3) {
 /* A 5.3 syntax in 5.4 */

 if (use5_4) {
 if (lefrInputAntennaCbk) { /* write warning only if cbk is set */

 if (inputAntennaWarnings++ < lefrInputAntennaWarnings) {
 yyerror("INPUTPINANTENNASIZE is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 }
 if (lefrInputAntennaCbk)
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
 if (lefrOutputAntennaCbk) { /* write warning only if cbk is set */

 if (outputAntennaWarnings++ < lefrOutputAntennaWarnings) {
 yyerror("OUTPUTPINANTENNASIZE is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 }
 if (lefrOutputAntennaCbk)
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
 if (lefrInoutAntennaCbk) { /* write warning only if cbk is set */

 if (inoutAntennaWarnings++ < lefrInoutAntennaWarnings) {
 yyerror("INOUTPINANTENNASIZE is a 5.3 or earlier syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 }
 if (lefrInoutAntennaCbk)
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
 if (lefrAntennaInputCbk) { /* write warning only if cbk is set */

 if (antennaInputWarnings++ < lefrAntennaInputWarnings) {
 yyerror("ANTENNAINPUTGATEAREA is a 5.4 syntax. Please either update your lef VERSION number or use the 5.3 syntax.");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrAntennaInputCbk) { /* write warning only if cbk is set */

 if (antennaInputWarnings++ < lefrAntennaInputWarnings) {
 yyerror("ANTENNAINPUTGATEAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrAntennaInputCbk)
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
 if (lefrAntennaInoutCbk) { /* write warning only if cbk is set */

 if (antennaInoutWarnings++ < lefrAntennaInoutWarnings) {
 yyerror("ANTENNAINOUTDIFFAREA is a 5.4 syntax. Please either update your lef VERSION number or use the 5.3 syntax.");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrAntennaInoutCbk) { /* write warning only if cbk is set */

 if (antennaInoutWarnings++ < lefrAntennaInoutWarnings) {
 yyerror("ANTENNAINOUTDIFFAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrAntennaInoutCbk)
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
 if (lefrAntennaOutputCbk) { /* write warning only if cbk is set */

 if (antennaOutputWarnings++ < lefrAntennaOutputWarnings) {
 yyerror("ANTENNAOUTPUTDIFFAREA is a 5.4 syntax. Please either update your lef VERSION number or use the 5.3 syntax.");
 CHKERR();
 }
 }
 } else if (use5_3) {
 if (lefrAntennaOutputCbk) { /* write warning only if cbk is set */

 if (antennaOutputWarnings++ < lefrAntennaOutputWarnings) {
 yyerror("ANTENNAOUTPUTDIFFAREA is a 5.4 syntax. Your lef file has both old and syntax, which is illegal.") ;
 CHKERR();
 }
 }
 }
 if (lefrAntennaOutputCbk)
 CALLBACK(lefrAntennaOutputCbk, lefrAntennaOutputCbkType, $2);
 }
 
 extension_opt: /* empty */

 | extension
 
 extension: K_BEGINEXT
 {
 if (lefrExtensionCbk)
 CALLBACK(lefrExtensionCbk, lefrExtensionCbkType, Hist_text);
 if (versionNum >= 5.6)
 ge56almostDone = 1;
 }
 
 %%
 
