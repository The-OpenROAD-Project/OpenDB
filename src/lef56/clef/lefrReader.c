/*
 * This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 * Distribution,  Product Version 5.6, and is subject to the Cadence LEF/DEF
 * Open Source License Agreement.   Your  continued  use  of this file
 * constitutes your acceptance of the terms of the LEF/DEF Open Source
 * License and an agreement to abide by its  terms.   If you  don't  agree
 * with  this, you must remove this and any other files which are part of the
 * distribution and  destroy any  copies made.
 * 
 * For updates, support, or to become part of the LEF/DEF Community, check
 * www.openeda.org for details.
 */
#include "lefrReader.h"
#include "lex.h"
#include <stdlib.h>
#include <string.h>
#include "lefiDebug.h"
#define CURRENT_VERSION 5.6
extern int lefyyparse(void);	/* do the parsing */

extern void lef_lex_init(void);	/* start the lexer */

extern void lef_lex_un_init(void);	/* free up lexer space */

extern int lef_nlines;		/* lexer line number */

static char lefrErrMsg[1024];

extern void lefrNerr(int num);

extern void lefrNwarn(int num);

extern int lefrRelaxMode;

extern double convert_name2num(char *versionName);

int     lefiDeltaNumberLines = 10000;

void 
lefiNerr(int i)
{
  sprintf(lefrErrMsg, "ERROR number %d\n", i);
  lefiError(lefrErrMsg);
  exit(2);
}

void 
lefiNwarn(int i)
{
  sprintf(lefrErrMsg, "WARNING number %d\n", i);
  lefiError(lefrErrMsg);
  exit(2);
}

double 
convert_name2num(char *versionName)
{
  char    majorNm[80];

  char    minorNm[80];

  char   *subMinorNm = NULL;

  char   *p1;

  char   *versionNm = strdup(versionName);

  double  major = 0, minor = 0, subMinor = 0;

  double  version;

  sscanf(versionNm, "%[^.].%s", majorNm, minorNm);
  if ((p1 = strchr(minorNm, '.'))) {
    subMinorNm = p1 + 1;
    *p1 = '\0';
  }
  major = atof(majorNm);
  minor = atof(minorNm);
  if (subMinorNm)
    subMinor = atof(subMinorNm);
  version = major;
  if (minor > 0)
    version = major + minor / 10;
  if (subMinor > 0)
    version = version + subMinor / 1000;
  lefFree(versionNm);
  return version;
}

/*
 * Global variables
 *//* 5.5 *//* 5.5 this & lefrHasMaxVS is to keep track that *//* MAXVIASTACK
 * has to be after all layers *//* keep track of spacing table in a layer *//* k
 * eep track of spacing in a layer *//* if user has set default version value *//* k
 * eep track if the library is done parsing *//* 5.6 END LIBRARY is optional. *//* h
 * ave reached the EXTENSION SECTION *//* a 5.6 and it has END LIBRARY
 * statement *//* The following global variables are for storing the
 * propertydefination *//* types.  Only real & integer need to store since
 * the parser can *//* distinguish string and quote string *//* The following
 * global variables are used in lef.y *//* keep track if there is any 5.3 on
 * 5.4 file  */
/* keep track if only 5.4 is allowed  */
/* keep track if version is in file  */
/* keep track if namescasesensitive is in file  */
/* keep track if bustbitchars is in file  */
/* keep track if dividerchar is in file  */
/* keep track for oxide  */

/*
 * NEW CALLBACK - If you create a new global instance of a class, then place
 * it here.
 */

/*
 * List of call back routines
 *//* These are filled in by the user.  See the *//* "set" routines at the end
 * of the file *//* NEW CALLBACK - Add a line here for each new callback
 * routine.  */

/*
 * Flags to control number of warnings to print out, max will be 999
 *//* Function to initialize global variables. *//* This make sure the global
    variables are initialized */ lefiUserData lefrUserData = 0;

char   *lefrFileName = 0;

static int lefrRegisterUnused = 0;

FILE   *lefrFile = 0;

lefiAntennaPWL *lefrAntennaPWLPtr = 0;

lefiArray lefrArray;

lefiCorrectionTable lefrCorrectionTable;

lefiDensity lefrDensity;

lefiGcellPattern *lefrGcellPatternPtr = 0;

lefiGeometries *lefrGeometriesPtr = 0;

lefiIRDrop lefrIRDrop;

lefiLayer lefrLayer;

lefiMacro lefrMacro;

lefiMaxStackVia lefrMaxStackVia;

lefiMinFeature lefrMinFeature;

struct lefiNoiseMargin lefrNoiseMargin;

lefiNoiseTable lefrNoiseTable;

lefiNonDefault lefrNonDefault;

lefiObstruction lefrObstruction;

lefiPin lefrPin;

lefiProp lefrProp;

lefiSite lefrSite;

lefiSitePattern *lefrSitePatternPtr = 0;

lefiSpacing lefrSpacing;

lefiTiming lefrTiming;

lefiTrackPattern *lefrTrackPatternPtr = 0;

lefiUnits lefrUnits;

lefiUseMinSpacing lefrUseMinSpacing;

lefiVia lefrVia;

lefiViaRule lefrViaRule;

int     lefrIsReset = 0;

int     lefrDoGcell = 0;

int     lefrDoGeometries = 0;

int     lefrDoTrack = 0;

int     lefrDoSite = 0;

int     lefrHasLayer = 0;

int     lefrHasMaxVS = 0;

int     lefrHasSpacingTbl;

int     lefrHasSpacing;

int     userHasSetVer = 0;

int     doneLib = 1;

int     ge56almostDone = 0;

int     ge56done = 0;

lefiPropType lefrLibProp;

lefiPropType lefrCompProp;

lefiPropType lefrPinProp;

lefiPropType lefrMacroProp;

lefiPropType lefrViaProp;

lefiPropType lefrViaRuleProp;

lefiPropType lefrLayerProp;

lefiPropType lefrNondefProp;

int     use5_3 = 0;

int     use5_4 = 0;

int     hasVer = 0;

int     hasNameCase = 0;

int     hasBusBit = 0;

int     hasDivChar = 0;

int     aOxide = 0;

double  versionNum = CURRENT_VERSION;

lefrUnitsCbkFnType lefrUnitsCbk = 0;

lefrIntegerCbkFnType lefrCaseSensitiveCbk = 0;

lefrStringCbkFnType lefrNoWireExtensionCbk = 0;

lefrStringCbkFnType lefrDividerCharCbk = 0;

lefrStringCbkFnType lefrBusBitCharsCbk = 0;

lefrDoubleCbkFnType lefrVersionCbk = 0;

lefrStringCbkFnType lefrVersionStrCbk = 0;

lefrLayerCbkFnType lefrLayerCbk = 0;

lefrViaCbkFnType lefrViaCbk = 0;

lefrViaRuleCbkFnType lefrViaRuleCbk = 0;

lefrSpacingCbkFnType lefrSpacingCbk = 0;

lefrIRDropCbkFnType lefrIRDropCbk = 0;

lefrDoubleCbkFnType lefrDielectricCbk = 0;

lefrMinFeatureCbkFnType lefrMinFeatureCbk = 0;

lefrNonDefaultCbkFnType lefrNonDefaultCbk = 0;

lefrSiteCbkFnType lefrSiteCbk = 0;

lefrStringCbkFnType lefrMacroBeginCbk = 0;

lefrPinCbkFnType lefrPinCbk = 0;

lefrObstructionCbkFnType lefrObstructionCbk = 0;

lefrArrayCbkFnType lefrArrayCbk = 0;

lefrMacroCbkFnType lefrMacroCbk = 0;

lefrVoidCbkFnType lefrLibraryEndCbk = 0;

lefrVoidCbkFnType lefrPropBeginCbk = 0;

lefrVoidCbkFnType lefrPropEndCbk = 0;

lefrPropCbkFnType lefrPropCbk = 0;

lefrVoidCbkFnType lefrSpacingBeginCbk = 0;

lefrVoidCbkFnType lefrSpacingEndCbk = 0;

lefrStringCbkFnType lefrArrayBeginCbk = 0;

lefrStringCbkFnType lefrArrayEndCbk = 0;

lefrTimingCbkFnType lefrTimingCbk = 0;

lefrVoidCbkFnType lefrIRDropBeginCbk = 0;

lefrVoidCbkFnType lefrIRDropEndCbk = 0;

lefrNoiseMarginCbkFnType lefrNoiseMarginCbk = 0;

lefrDoubleCbkFnType lefrEdgeRateThreshold1Cbk = 0;

lefrDoubleCbkFnType lefrEdgeRateThreshold2Cbk = 0;

lefrDoubleCbkFnType lefrEdgeRateScaleFactorCbk = 0;

lefrNoiseTableCbkFnType lefrNoiseTableCbk = 0;

lefrCorrectionTableCbkFnType lefrCorrectionTableCbk = 0;

lefrDoubleCbkFnType lefrInputAntennaCbk = 0;

lefrDoubleCbkFnType lefrOutputAntennaCbk = 0;

lefrDoubleCbkFnType lefrInoutAntennaCbk = 0;

lefrDoubleCbkFnType lefrAntennaInputCbk = 0;

lefrDoubleCbkFnType lefrAntennaInoutCbk = 0;

lefrDoubleCbkFnType lefrAntennaOutputCbk = 0;

lefrDoubleCbkFnType lefrManufacturingCbk = 0;

lefrUseMinSpacingCbkFnType lefrUseMinSpacingCbk = 0;

lefrStringCbkFnType lefrClearanceMeasureCbk = 0;

lefrStringCbkFnType lefrMacroClassTypeCbk = 0;

lefrMacroNumCbkFnType lefrMacroOriginCbk = 0;

lefrMacroNumCbkFnType lefrMacroSizeCbk = 0;

lefrStringCbkFnType lefrMacroEndCbk = 0;

lefrMaxStackViaCbkFnType lefrMaxStackViaCbk = 0;

lefrStringCbkFnType lefrExtensionCbk = 0;

lefrDensityCbkFnType lefrDensityCbk = 0;

int     lefrAntennaInoutWarnings = 999;

int     lefrAntennaInputWarnings = 999;

int     lefrAntennaOutputWarnings = 999;

int     lefrArrayWarnings = 999;

int     lefrCaseSensitiveWarnings = 999;

int     lefrCorrectionTableWarnings = 999;

int     lefrDielectricWarnings = 999;

int     lefrEdgeRateThreshold1Warnings = 999;

int     lefrEdgeRateThreshold2Warnings = 999;

int     lefrEdgeRateScaleFactorWarnings = 999;

int     lefrInoutAntennaWarnings = 999;

int     lefrInputAntennaWarnings = 999;

int     lefrIRDropWarnings = 999;

int     lefrLayerWarnings = 999;

int     lefrMacroWarnings = 999;

int     lefrMaxStackViaWarnings = 999;

int     lefrMinFeatureWarnings = 999;

int     lefrNoiseMarginWarnings = 999;

int     lefrNoiseTableWarnings = 999;

int     lefrNonDefaultWarnings = 999;

int     lefrNoWireExtensionWarnings = 999;

int     lefrOutputAntennaWarnings = 999;

int     lefrPinWarnings = 999;

int     lefrSiteWarnings = 999;

int     lefrSpacingWarnings = 999;

int     lefrTimingWarnings = 999;

int     lefrUnitsWarnings = 999;

int     lefrUseMinSpacingWarnings = 999;

int     lefrViaRuleWarnings = 999;

int     lefrViaWarnings = 999;

void 
lefrInitGlobals()
{
  lefrUserData = 0;
  lefrIsReset = 0;
  return;
}

/*
 * Parser control by the user.
 *//* Reader initialization */ int 
lefrInit()
{
  lefrInitGlobals();
  lefrReset();

#ifndef __cplusplus
  lefiUnits_Init(&(lefrUnits));
  lefiLayer_Init(&(lefrLayer));
  lefiVia_Init(&(lefrVia));
  lefiViaRule_Init(&(lefrViaRule));
  lefiSpacing_Init(&(lefrSpacing));
  lefiIRDrop_Init(&(lefrIRDrop));
  lefiProp_Init(&(lefrProp));
  lefiTiming_Init(&(lefrTiming));
  lefiNoiseTable_Init(&(lefrNoiseTable));
  lefiCorrectionTable_Init(&(lefrCorrectionTable));
  lefiUseMinSpacing_Init(&(lefrUseMinSpacing));
  lefiMaxStackVia_Init(&(lefrMaxStackVia));
  lefiNonDefault_Init(&(lefrNonDefault));
  lefiMacro_Init(&(lefrMacro));
  lefiPin_Init(&(lefrPin));
  lefiDensity_Init(&(lefrDensity));
  lefiObstruction_Init(&(lefrObstruction));
  lefiSite_Init(&(lefrSite));
#endif

  return 0;
}

int 
lefrReset()
{
  lefrIsReset = 1;
  use5_3 = 0;
  use5_4 = 0;
  hasVer = 0;
  hasNameCase = 0;
  hasBusBit = 0;
  hasDivChar = 0;
  aOxide = 0;
  doneLib = 1;
  ge56almostDone = 0;
  ge56done = 0;
  if (!userHasSetVer)
    versionNum = CURRENT_VERSION;
  lefrFileName = 0;
  lefrFile = 0;
  lefrSitePatternPtr = 0;
  lefrDoSite = 0;
  lefrGeometriesPtr = 0;
  lefrDoGeometries = 0;
  lefrTrackPatternPtr = 0;
  lefrDoTrack = 0;
  lefrGcellPatternPtr = 0;
  lefrDoGcell = 0;
  lefrAntennaPWLPtr = 0;
  lefrHasLayer = 0;
  lefrHasMaxVS = 0;
  lefrHasSpacingTbl = 0;
  lefrHasSpacing = 0;
  return 0;
}

const char *
lefrFName()
{
  return lefrFileName;
}

int 
lefrReleaseNResetMemory()
{
  lefiCorrectionTable_Destroy(&(lefrCorrectionTable));
  lefiDensity_Destroy(&(lefrDensity));
  lefiIRDrop_Destroy(&(lefrIRDrop));
  lefiLayer_Destroy(&(lefrLayer));
  lefiMacro_Destroy(&(lefrMacro));
  lefiMaxStackVia_Destroy(&(lefrMaxStackVia));
  lefiNoiseTable_Destroy(&(lefrNoiseTable));
  lefiNonDefault_Destroy(&(lefrNonDefault));
  lefiPin_Destroy(&(lefrPin));
  lefiProp_Destroy(&(lefrProp));
  lefiSpacing_Destroy(&(lefrSpacing));
  lefiTiming_Destroy(&(lefrTiming));
  lefiUnits_Destroy(&(lefrUnits));
  lefiObstruction_Destroy(&(lefrObstruction));
  lefiSite_Destroy(&(lefrSite));
  lefiUseMinSpacing_Destroy(&(lefrUseMinSpacing));
  lefiVia_Destroy(&(lefrVia));
  lefiViaRule_Destroy(&(lefrViaRule));
  lefiCorrectionTable_Init(&(lefrCorrectionTable));
  lefiDensity_Init(&(lefrDensity));
  lefiIRDrop_Init(&(lefrIRDrop));
  lefiLayer_Init(&(lefrLayer));
  lefiMacro_Init(&(lefrMacro));
  lefiNoiseTable_Init(&(lefrNoiseTable));
  lefiNonDefault_Init(&(lefrNonDefault));
  lefiPin_Init(&(lefrPin));
  lefiProp_Init(&(lefrProp));
  lefiSpacing_Init(&(lefrSpacing));
  lefiTiming_Init(&(lefrTiming));
  lefiUnits_Init(&(lefrUnits));
  lefiObstruction_Init(&(lefrObstruction));
  lefiSite_Init(&(lefrSite));
  lefiUseMinSpacing_Init(&(lefrUseMinSpacing));
  lefiVia_Init(&(lefrVia));
  lefiViaRule_Init(&(lefrViaRule));
  return 0;
}

int 
lefrRead(FILE * f, const char *fName, lefiUserData uData)
{
  int     status;

  if (lefrIsReset == 0) {
    fprintf(stderr, "ERROR: lefrRead called before lefrInit\n");
    return -1;
  }
  lefrFileName = (char *) fName;
  lefrFile = f;
  lefrUserData = uData;
  lef_lex_init();
  status = lefyyparse();
  lef_lex_un_init();
  lefrIsReset = 0;
  return status;
}

void 
lefrSetUnusedCallbacks(lefrVoidCbkFnType func)
{
  if (lefrArrayBeginCbk == 0)
    lefrArrayBeginCbk = (lefrStringCbkFnType) func;
  if (lefrArrayCbk == 0)
    lefrArrayCbk = (lefrArrayCbkFnType) func;
  if (lefrArrayEndCbk == 0)
    lefrArrayEndCbk = (lefrStringCbkFnType) func;
  if (lefrDividerCharCbk == 0)
    lefrDividerCharCbk = (lefrStringCbkFnType) func;
  if (lefrBusBitCharsCbk == 0)
    lefrBusBitCharsCbk = (lefrStringCbkFnType) func;
  if (lefrCaseSensitiveCbk == 0)
    lefrCaseSensitiveCbk = (lefrIntegerCbkFnType) func;
  if (lefrNoWireExtensionCbk == 0)
    lefrNoWireExtensionCbk = (lefrStringCbkFnType) func;
  if (lefrCorrectionTableCbk == 0)
    lefrCorrectionTableCbk = (lefrCorrectionTableCbkFnType) func;
  if (lefrDielectricCbk == 0)
    lefrDielectricCbk = (lefrDoubleCbkFnType) func;
  if (lefrEdgeRateScaleFactorCbk == 0)
    lefrEdgeRateScaleFactorCbk = (lefrDoubleCbkFnType) func;
  if (lefrEdgeRateThreshold1Cbk == 0)
    lefrEdgeRateThreshold1Cbk = (lefrDoubleCbkFnType) func;
  if (lefrEdgeRateThreshold2Cbk == 0)
    lefrEdgeRateThreshold2Cbk = (lefrDoubleCbkFnType) func;
  if (lefrIRDropBeginCbk == 0)
    lefrIRDropBeginCbk = (lefrVoidCbkFnType) func;
  if (lefrIRDropCbk == 0)
    lefrIRDropCbk = (lefrIRDropCbkFnType) func;
  if (lefrIRDropEndCbk == 0)
    lefrIRDropEndCbk = (lefrVoidCbkFnType) func;
  if (lefrLayerCbk == 0)
    lefrLayerCbk = (lefrLayerCbkFnType) func;
  if (lefrLibraryEndCbk == 0)
    lefrLibraryEndCbk = (lefrVoidCbkFnType) func;
  if (lefrMacroBeginCbk == 0)
    lefrMacroBeginCbk = (lefrStringCbkFnType) func;
  if (lefrMacroCbk == 0)
    lefrMacroCbk = (lefrMacroCbkFnType) func;
  if (lefrMacroClassTypeCbk == 0)
    lefrMacroClassTypeCbk = (lefrStringCbkFnType) func;
  if (lefrMacroOriginCbk == 0)
    lefrMacroOriginCbk = (lefrMacroNumCbkFnType) func;
  if (lefrMacroSizeCbk == 0)
    lefrMacroSizeCbk = (lefrMacroNumCbkFnType) func;
  if (lefrTimingCbk == 0)
    lefrTimingCbk = (lefrTimingCbkFnType) func;
  if (lefrMinFeatureCbk == 0)
    lefrMinFeatureCbk = (lefrMinFeatureCbkFnType) func;
  if (lefrNoiseMarginCbk == 0)
    lefrNoiseMarginCbk = (lefrNoiseMarginCbkFnType) func;
  if (lefrNoiseTableCbk == 0)
    lefrNoiseTableCbk = (lefrNoiseTableCbkFnType) func;
  if (lefrNonDefaultCbk == 0)
    lefrNonDefaultCbk = (lefrNonDefaultCbkFnType) func;
  if (lefrObstructionCbk == 0)
    lefrObstructionCbk = (lefrObstructionCbkFnType) func;
  if (lefrPinCbk == 0)
    lefrPinCbk = (lefrPinCbkFnType) func;
  if (lefrPropBeginCbk == 0)
    lefrPropBeginCbk = (lefrVoidCbkFnType) func;
  if (lefrPropCbk == 0)
    lefrPropCbk = (lefrPropCbkFnType) func;
  if (lefrPropEndCbk == 0)
    lefrPropEndCbk = (lefrVoidCbkFnType) func;
  if (lefrSiteCbk == 0)
    lefrSiteCbk = (lefrSiteCbkFnType) func;
  if (lefrSpacingBeginCbk == 0)
    lefrSpacingBeginCbk = (lefrVoidCbkFnType) func;
  if (lefrSpacingCbk == 0)
    lefrSpacingCbk = (lefrSpacingCbkFnType) func;
  if (lefrSpacingEndCbk == 0)
    lefrSpacingEndCbk = (lefrVoidCbkFnType) func;
  if (lefrUnitsCbk == 0)
    lefrUnitsCbk = (lefrUnitsCbkFnType) func;
  if ((lefrVersionCbk == 0) && (lefrVersionStrCbk == 0)) {
    lefrVersionCbk = (lefrDoubleCbkFnType) func;
    lefrVersionStrCbk = (lefrStringCbkFnType) func;
  }
  if (lefrViaCbk == 0)
    lefrViaCbk = (lefrViaCbkFnType) func;
  if (lefrViaRuleCbk == 0)
    lefrViaRuleCbk = (lefrViaRuleCbkFnType) func;
  if (lefrInputAntennaCbk == 0)
    lefrInputAntennaCbk = (lefrDoubleCbkFnType) func;
  if (lefrOutputAntennaCbk == 0)
    lefrOutputAntennaCbk = (lefrDoubleCbkFnType) func;
  if (lefrInoutAntennaCbk == 0)
    lefrInoutAntennaCbk = (lefrDoubleCbkFnType) func;
  if (lefrAntennaInputCbk == 0)
    lefrAntennaInputCbk = (lefrDoubleCbkFnType) func;
  if (lefrAntennaInoutCbk == 0)
    lefrAntennaInoutCbk = (lefrDoubleCbkFnType) func;
  if (lefrAntennaOutputCbk == 0)
    lefrAntennaOutputCbk = (lefrDoubleCbkFnType) func;
  if (lefrManufacturingCbk == 0)
    lefrManufacturingCbk = (lefrDoubleCbkFnType) func;
  if (lefrUseMinSpacingCbk == 0)
    lefrUseMinSpacingCbk = (lefrUseMinSpacingCbkFnType) func;
  if (lefrClearanceMeasureCbk == 0)
    lefrClearanceMeasureCbk = (lefrStringCbkFnType) func;
  if (lefrMacroClassTypeCbk == 0)
    lefrMacroClassTypeCbk = (lefrStringCbkFnType) func;
  if (lefrMacroOriginCbk == 0)
    lefrMacroOriginCbk = (lefrMacroNumCbkFnType) func;
  if (lefrMacroSizeCbk == 0)
    lefrMacroSizeCbk = (lefrMacroNumCbkFnType) func;
  if (lefrMacroEndCbk == 0)
    lefrMacroEndCbk = (lefrStringCbkFnType) func;
  if (lefrMaxStackViaCbk == 0)
    lefrMaxStackViaCbk = (lefrMaxStackViaCbkFnType) func;
  if (lefrExtensionCbk == 0)
    lefrExtensionCbk = (lefrStringCbkFnType) func;
  if (lefrDensityCbk == 0)
    lefrDensityCbk = (lefrDensityCbkFnType) func;
}

/* These count up the number of times an unset callback is called...  */
static int lefrUnusedCount[100];

int 
lefrCountFunc(lefrCallbackType_e e, void *v, lefiUserData d)
{
  int     i = (int) e;

  if (lefiDebug(23))
    printf("count %d 0x%p 0x%p\n", (int) e, v, d);
  if (i >= 0 && i < 100) {
    lefrUnusedCount[i] += 1;
    return 0;
  }
  return 1;
}

void 
lefrSetRegisterUnusedCallbacks()
{
  int     i;

  lefrRegisterUnused = 1;
  lefrSetUnusedCallbacks(lefrCountFunc);
  for (i = 0;
       i < 100;
       i++)
    lefrUnusedCount[i] = 0;
}

void 
lefrPrintUnusedCallbacks(FILE * f)
{
  int     i;

  int     first = 1;

  int     trueCB = 1;

  if (lefrRegisterUnused == 0) {
    fprintf(f, "ERROR: lefrSetRegisterUnusedCallbacks was not called to setup this data.\n");
    return;
  }
  for (i = 0;
       i < 100;
       i++) {
    if (lefrUnusedCount[i]) {
      if (first && (lefrCallbackType_e) i != lefrMacroClassTypeCbkType && (lefrCallbackType_e) i != lefrMacroOriginCbkType && (lefrCallbackType_e) i != lefrMacroSizeCbkType && (lefrCallbackType_e) i != lefrMacroEndCbkType) {
	fprintf(f, "LEF items that were present but ignored because of no callback:\n");
	first = 0;
      }
      switch ((lefrCallbackType_e) i) {
      case lefrArrayBeginCbkType:
	fprintf(f, "ArrayBegin");
	break;
      case lefrArrayCbkType:
	fprintf(f, "Array");
	break;
      case lefrArrayEndCbkType:
	fprintf(f, "ArrayEnd");
	break;
      case lefrDividerCharCbkType:
	fprintf(f, "DividerChar");
	break;
      case lefrBusBitCharsCbkType:
	fprintf(f, "BusBitChars");
	break;
      case lefrNoWireExtensionCbkType:
	fprintf(f, "NoWireExtensionAtPins");
	break;
      case lefrCaseSensitiveCbkType:
	fprintf(f, "CaseSensitive");
	break;
      case lefrCorrectionTableCbkType:
	fprintf(f, "CorrectionTable");
	break;
      case lefrDielectricCbkType:
	fprintf(f, "Dielectric");
	break;
      case lefrEdgeRateScaleFactorCbkType:
	fprintf(f, "EdgeRateScaleFactor");
	break;
      case lefrEdgeRateThreshold1CbkType:
	fprintf(f, "EdgeRateThreshold1");
	break;
      case lefrEdgeRateThreshold2CbkType:
	fprintf(f, "EdgeRateThreshold2");
	break;
      case lefrIRDropBeginCbkType:
	fprintf(f, "IRDropBegin");
	break;
      case lefrIRDropCbkType:
	fprintf(f, "IRDrop");
	break;
      case lefrIRDropEndCbkType:
	fprintf(f, "IRDropEnd");
	break;
      case lefrLayerCbkType:
	fprintf(f, "Layer");
	break;
      case lefrLibraryEndCbkType:
	fprintf(f, "LibraryEnd");
	break;
      case lefrMacroBeginCbkType:
	fprintf(f, "MacroBegin");
	break;
      case lefrMacroCbkType:
	fprintf(f, "Macro");
	break;
      case lefrMinFeatureCbkType:
	fprintf(f, "MinFeature");
	break;
      case lefrNoiseMarginCbkType:
	fprintf(f, "NoiseMargin");
	break;
      case lefrNoiseTableCbkType:
	fprintf(f, "NoiseTable");
	break;
      case lefrNonDefaultCbkType:
	fprintf(f, "NonDefault");
	break;
      case lefrObstructionCbkType:
	fprintf(f, "Obstruction");
	break;
      case lefrPinCbkType:
	fprintf(f, "Pin");
	break;
      case lefrPropBeginCbkType:
	fprintf(f, "PropBegin");
	break;
      case lefrPropCbkType:
	fprintf(f, "Prop");
	break;
      case lefrPropEndCbkType:
	fprintf(f, "PropEnd");
	break;
      case lefrSiteCbkType:
	fprintf(f, "Site");
	break;
      case lefrSpacingBeginCbkType:
	fprintf(f, "SpacingBegin");
	break;
      case lefrSpacingCbkType:
	fprintf(f, "Spacing");
	break;
      case lefrSpacingEndCbkType:
	fprintf(f, "SpacingEnd");
	break;
      case lefrUnitsCbkType:
	fprintf(f, "Units");
	break;
      case lefrVersionCbkType:
	fprintf(f, "Version");
	break;
      case lefrVersionStrCbkType:
	fprintf(f, "Version");
	break;
      case lefrViaCbkType:
	fprintf(f, "Via");
	break;
      case lefrViaRuleCbkType:
	fprintf(f, "ViaRule");
	break;
      case lefrInputAntennaCbkType:
	fprintf(f, "InputAntenna");
	break;
      case lefrOutputAntennaCbkType:
	fprintf(f, "OutputAntenna");
	break;
      case lefrInoutAntennaCbkType:
	fprintf(f, "InoutAntenna");
	break;
      case lefrAntennaInputCbkType:
	fprintf(f, "AntennaInput");
	break;
      case lefrAntennaInoutCbkType:
	fprintf(f, "AntennaInout");
	break;
      case lefrAntennaOutputCbkType:
	fprintf(f, "AntennaOutput");
	break;
      case lefrManufacturingCbkType:
	fprintf(f, "Manufacturing");
	break;
      case lefrUseMinSpacingCbkType:
	fprintf(f, "UseMinSpacing");
	break;
      case lefrClearanceMeasureCbkType:
	fprintf(f, "ClearanceMeasure");
	break;
      case lefrTimingCbkType:
	fprintf(f, "Timing");
	break;
      case lefrMaxStackViaCbkType:
	fprintf(f, "MaxStackVia");
	break;
      case lefrExtensionCbkType:
	fprintf(f, "Extension");
	break;
      case lefrMacroClassTypeCbkType:
      case lefrMacroOriginCbkType:
      case lefrMacroSizeCbkType:
      case lefrMacroEndCbkType:
	trueCB = 0;
	break;
      case lefrDensityCbkType:
	fprintf(f, "Density");
	break;
      default:
	fprintf(f, "BOGUS ENTRY");
	break;
      }
      if (trueCB)
	fprintf(f, " %d\n", lefrUnusedCount[i]);
      else
	trueCB = 1;
    }
  }
}

void 
lefrUnsetCallbacks()
{
  lefrAntennaInputCbk = 0;
  lefrAntennaInoutCbk = 0;
  lefrAntennaOutputCbk = 0;
  lefrArrayBeginCbk = 0;
  lefrArrayCbk = 0;
  lefrArrayEndCbk = 0;
  lefrDividerCharCbk = 0;
  lefrBusBitCharsCbk = 0;
  lefrCaseSensitiveCbk = 0;
  lefrClearanceMeasureCbk = 0;
  lefrCorrectionTableCbk = 0;
  lefrDensityCbk = 0;
  lefrDielectricCbk = 0;
  lefrEdgeRateScaleFactorCbk = 0;
  lefrEdgeRateThreshold1Cbk = 0;
  lefrEdgeRateThreshold2Cbk = 0;
  lefrExtensionCbk = 0;
  lefrInoutAntennaCbk = 0;
  lefrInputAntennaCbk = 0;
  lefrIRDropBeginCbk = 0;
  lefrIRDropCbk = 0;
  lefrIRDropEndCbk = 0;
  lefrLayerCbk = 0;
  lefrLibraryEndCbk = 0;
  lefrMacroEndCbk = 0;
  lefrMacroBeginCbk = 0;
  lefrMacroCbk = 0;
  lefrMacroClassTypeCbk = 0;
  lefrMacroOriginCbk = 0;
  lefrMacroSizeCbk = 0;
  lefrManufacturingCbk = 0;
  lefrMaxStackViaCbk = 0;
  lefrTimingCbk = 0;
  lefrMinFeatureCbk = 0;
  lefrNoiseMarginCbk = 0;
  lefrNoiseTableCbk = 0;
  lefrNonDefaultCbk = 0;
  lefrNoWireExtensionCbk = 0;
  lefrObstructionCbk = 0;
  lefrOutputAntennaCbk = 0;
  lefrPinCbk = 0;
  lefrPropBeginCbk = 0;
  lefrPropCbk = 0;
  lefrPropEndCbk = 0;
  lefrSiteCbk = 0;
  lefrSpacingBeginCbk = 0;
  lefrSpacingCbk = 0;
  lefrSpacingEndCbk = 0;
  lefrUnitsCbk = 0;
  lefrUseMinSpacingCbk = 0;
  lefrVersionCbk = 0;
  lefrVersionStrCbk = 0;
  lefrViaCbk = 0;
  lefrViaRuleCbk = 0;
}

void 
lefrSetUserData(lefiUserData d)
{
  lefrUserData = d;
}

lefiUserData 
lefrGetUserData()
{
  return lefrUserData;
}

void 
lefrSetUnitsCbk(lefrUnitsCbkFnType f)
{
  lefrUnitsCbk = f;
}

void 
lefrSetDividerCharCbk(lefrStringCbkFnType f)
{
  lefrDividerCharCbk = f;
}

void 
lefrSetNoWireExtensionCbk(lefrStringCbkFnType f)
{
  lefrNoWireExtensionCbk = f;
}

void 
lefrSetBusBitCharsCbk(lefrStringCbkFnType f)
{
  lefrBusBitCharsCbk = f;
}

void 
lefrSetCaseSensitiveCbk(lefrIntegerCbkFnType f)
{
  lefrCaseSensitiveCbk = f;
}

void 
lefrSetVersionCbk(lefrDoubleCbkFnType f)
{
  lefrVersionCbk = f;
}

void 
lefrSetVersionStrCbk(lefrStringCbkFnType f)
{
  lefrVersionStrCbk = f;
}

void 
lefrSetLayerCbk(lefrLayerCbkFnType f)
{
  lefrLayerCbk = f;
}

void 
lefrSetViaCbk(lefrViaCbkFnType f)
{
  lefrViaCbk = f;
}

void 
lefrSetViaRuleCbk(lefrViaRuleCbkFnType f)
{
  lefrViaRuleCbk = f;
}

void 
lefrSetSpacingCbk(lefrSpacingCbkFnType f)
{
  lefrSpacingCbk = f;
}

void 
lefrSetIRDropCbk(lefrIRDropCbkFnType f)
{
  lefrIRDropCbk = f;
}

void 
lefrSetDielectricCbk(lefrDoubleCbkFnType f)
{
  lefrDielectricCbk = f;
}

void 
lefrSetMinFeatureCbk(lefrMinFeatureCbkFnType f)
{
  lefrMinFeatureCbk = f;
}

void 
lefrSetNonDefaultCbk(lefrNonDefaultCbkFnType f)
{
  lefrNonDefaultCbk = f;
}

void 
lefrSetSiteCbk(lefrSiteCbkFnType f)
{
  lefrSiteCbk = f;
}

void 
lefrSetMacroCbk(lefrMacroCbkFnType f)
{
  lefrMacroCbk = f;
}

void 
lefrSetMacroBeginCbk(lefrStringCbkFnType f)
{
  lefrMacroBeginCbk = f;
}

void 
lefrSetPinCbk(lefrPinCbkFnType f)
{
  lefrPinCbk = f;
}

void 
lefrSetObstructionCbk(lefrObstructionCbkFnType f)
{
  lefrObstructionCbk = f;
}

void 
lefrSetArrayCbk(lefrArrayCbkFnType f)
{
  lefrArrayCbk = f;
}

void 
lefrSetLibraryEndCbk(lefrVoidCbkFnType f)
{
  lefrLibraryEndCbk = f;
}

/*
 * NEW CALLBACK - Each callback routine must have a routine that allows the
 * user to set it.  The set routines go here.
 */
void 
lefrSetPropBeginCbk(lefrVoidCbkFnType f)
{
  lefrPropBeginCbk = f;
}

void 
lefrSetPropEndCbk(lefrVoidCbkFnType f)
{
  lefrPropEndCbk = f;
}

void 
lefrSetPropCbk(lefrPropCbkFnType f)
{
  lefrPropCbk = f;
}

void 
lefrSetSpacingBeginCbk(lefrVoidCbkFnType f)
{
  lefrSpacingBeginCbk = f;
}

void 
lefrSetSpacingEndCbk(lefrVoidCbkFnType f)
{
  lefrSpacingEndCbk = f;
}

void 
lefrSetArrayBeginCbk(lefrStringCbkFnType f)
{
  lefrArrayBeginCbk = f;
}

void 
lefrSetArrayEndCbk(lefrStringCbkFnType f)
{
  lefrArrayEndCbk = f;
}

void 
lefrSetIRDropBeginCbk(lefrVoidCbkFnType f)
{
  lefrIRDropBeginCbk = f;
}

void 
lefrSetIRDropEndCbk(lefrVoidCbkFnType f)
{
  lefrIRDropEndCbk = f;
}

void 
lefrSetNoiseMarginCbk(lefrNoiseMarginCbkFnType f)
{
  lefrNoiseMarginCbk = f;
}

void 
lefrSetEdgeRateThreshold1Cbk(lefrDoubleCbkFnType f)
{
  lefrEdgeRateThreshold1Cbk = f;
}

void 
lefrSetEdgeRateThreshold2Cbk(lefrDoubleCbkFnType f)
{
  lefrEdgeRateThreshold2Cbk = f;
}

void 
lefrSetEdgeRateScaleFactorCbk(lefrDoubleCbkFnType f)
{
  lefrEdgeRateScaleFactorCbk = f;
}

void 
lefrSetTimingCbk(lefrTimingCbkFnType f)
{
  lefrTimingCbk = f;
}

void 
lefrSetNoiseTableCbk(lefrNoiseTableCbkFnType f)
{
  lefrNoiseTableCbk = f;
}

void 
lefrSetCorrectionTableCbk(lefrCorrectionTableCbkFnType f)
{
  lefrCorrectionTableCbk = f;
}

void 
lefrSetInputAntennaCbk(lefrDoubleCbkFnType f)
{
  lefrInputAntennaCbk = f;
}

void 
lefrSetOutputAntennaCbk(lefrDoubleCbkFnType f)
{
  lefrOutputAntennaCbk = f;
}

void 
lefrSetInoutAntennaCbk(lefrDoubleCbkFnType f)
{
  lefrInoutAntennaCbk = f;
}

void 
lefrSetAntennaInputCbk(lefrDoubleCbkFnType f)
{
  lefrAntennaInputCbk = f;
}

void 
lefrSetAntennaInoutCbk(lefrDoubleCbkFnType f)
{
  lefrAntennaInoutCbk = f;
}

void 
lefrSetAntennaOutputCbk(lefrDoubleCbkFnType f)
{
  lefrAntennaOutputCbk = f;
}

void 
lefrSetManufacturingCbk(lefrDoubleCbkFnType f)
{
  lefrManufacturingCbk = f;
}

void 
lefrSetUseMinSpacingCbk(lefrUseMinSpacingCbkFnType f)
{
  lefrUseMinSpacingCbk = f;
}

void 
lefrSetClearanceMeasureCbk(lefrStringCbkFnType f)
{
  lefrClearanceMeasureCbk = f;
}

/*
 * 5.5
 */ void 
lefrSetMaxStackViaCbk(lefrMaxStackViaCbkFnType f)
{
  lefrMaxStackViaCbk = f;
}

/*
 * 4/4/2003
 */ void 
lefrSetExtensionCbk(lefrStringCbkFnType f)
{
  lefrExtensionCbk = f;
}

/*
 * 06-20-2001, Wanda da Rosa, PCR 377398
 *//* This callback is added only for PKS, they need a extra call *//* back of
 * the Macro type to handle big number of obstructions. *//* Nothing change
    on the existing code for the lefiMacro class. */ void 
lefrSetMacroClassTypeCbk(lefrStringCbkFnType f)
{
  lefrMacroClassTypeCbk = f;
}

/*
 * 09-10-2002, Wanda da Rosa
 *//* The callbacks MacroOrigin & MacroSize is for FE only */ void 
lefrSetMacroOriginCbk(lefrMacroNumCbkFnType f)
{
  lefrMacroOriginCbk = f;
}

void 
lefrSetMacroSizeCbk(lefrMacroNumCbkFnType f)
{
  lefrMacroSizeCbk = f;
}

void 
lefrSetMacroEndCbk(lefrStringCbkFnType f)
{
  lefrMacroEndCbk = f;
}

/*
 * 5.6
 */ void 
lefrSetDensityCbk(lefrDensityCbkFnType f)
{
  lefrDensityCbk = f;
}

int 
lefrLineNumber()
{
  return lef_nlines;
}

/* User defined error message function  */
LEFI_LOG_FUNCTION lefiErrorLogFunction = 0;

void 
lefrSetLogFunction(LEFI_LOG_FUNCTION f)
{
  lefiErrorLogFunction = f;
}

/* User defined warning message function  */
LEFI_WARNING_LOG_FUNCTION lefiWarningLogFunction = 0;

void 
lefrSetWarningLogFunction(LEFI_WARNING_LOG_FUNCTION f)
{
  lefiWarningLogFunction = f;
}

/* User defined malloc function  */
LEFI_MALLOC_FUNCTION lefiMallocFunction = 0;

void 
lefrSetMallocFunction(LEFI_MALLOC_FUNCTION f)
{
  lefiMallocFunction = f;
}

/* User defined realloc function  */
LEFI_REALLOC_FUNCTION lefiReallocFunction = 0;

void 
lefrSetReallocFunction(LEFI_REALLOC_FUNCTION f)
{
  lefiReallocFunction = f;
}

/* User defined free function  */
LEFI_FREE_FUNCTION lefiFreeFunction = 0;

void 
lefrSetFreeFunction(LEFI_FREE_FUNCTION f)
{
  lefiFreeFunction = f;
}

/* Callback routine with the parsed line number  */
LEFI_LINE_NUMBER_FUNCTION lefiLineNumberFunction = 0;

void 
lefrSetLineNumberFunction(LEFI_LINE_NUMBER_FUNCTION f)
{
  lefiLineNumberFunction = f;
}

void 
lefrSetDeltaNumberLines(int numLines)
{
  lefiDeltaNumberLines = numLines;
}

/* from the lexer  */
extern char lefcommentChar;

void 
lefrSetShiftCase()
{
  lefrShiftCase = 1;
}

void 
lefrSetCommentChar(char c)
{
  lefcommentChar = c;
}

void 
lefrSetCaseSensitivity(int caseSense)
{
  lefNamesCaseSensitive = caseSense;
  lefReaderCaseSensitive = caseSense;
}

void 
lefrSetRelaxMode()
{
  lefrRelaxMode = TRUE;
}

void 
lefrUnsetRelaxMode()
{
  lefrRelaxMode = FALSE;
}

void 
lefrSetVersionValue(char *version)
{
  userHasSetVer = 1;
  versionNum = convert_name2num(version);
}

/* gzip read function set by lefzlib for gzip only  */
LEFI_READ_FUNCTION lefiReadFunction = 0;

void 
lefrSetReadFunction(LEFI_READ_FUNCTION f)
{
  lefiReadFunction = f;
}

void 
lefrUnsetReadFunction()
{
  lefiReadFunction = 0;
}

/*
 * Set the maximum number of warnings
 */ void 
lefrSetAntennaInoutWarnings(int warn)
{
  lefrAntennaInoutWarnings = warn;
}

void 
lefrSetAntennaInputWarnings(int warn)
{
  lefrAntennaInputWarnings = warn;
}

void 
lefrSetAntennaOutputWarnings(int warn)
{
  lefrAntennaOutputWarnings = warn;
}

void 
lefrSetArrayWarnings(int warn)
{
  lefrArrayWarnings = warn;
}

void 
lefrSetCaseSensitiveWarnings(int warn)
{
  lefrCaseSensitiveWarnings = warn;
}

void 
lefrSetCorrectionTableWarnings(int warn)
{
  lefrCorrectionTableWarnings = warn;
}

void 
lefrSetDielectricWarnings(int warn)
{
  lefrDielectricWarnings = warn;
}

void 
lefrSetEdgeRateThreshold1Warnings(int warn)
{
  lefrEdgeRateThreshold1Warnings = warn;
}

void 
lefrSetEdgeRateThreshold2Warnings(int warn)
{
  lefrEdgeRateThreshold2Warnings = warn;
}

void 
lefrSetEdgeRateScaleFactorWarnings(int warn)
{
  lefrEdgeRateScaleFactorWarnings = warn;
}

void 
lefrSetInoutAntennaWarnings(int warn)
{
  lefrInoutAntennaWarnings = warn;
}

void 
lefrSetInputAntennaWarnings(int warn)
{
  lefrInputAntennaWarnings = warn;
}

void 
lefrSetIRDropWarnings(int warn)
{
  lefrIRDropWarnings = warn;
}

void 
lefrSetLayerWarnings(int warn)
{
  lefrLayerWarnings = warn;
}

void 
lefrSetMacroWarnings(int warn)
{
  lefrMacroWarnings = warn;
}

void 
lefrSetMaxStackViaWarnings(int warn)
{
  lefrMaxStackViaWarnings = warn;
}

void 
lefrSetMinFeatureWarnings(int warn)
{
  lefrMinFeatureWarnings = warn;
}

void 
lefrSetNoiseMarginWarnings(int warn)
{
  lefrNoiseMarginWarnings = warn;
}

void 
lefrSetNoiseTableWarnings(int warn)
{
  lefrNoiseTableWarnings = warn;
}

void 
lefrSetNonDefaultWarnings(int warn)
{
  lefrNonDefaultWarnings = warn;
}

void 
lefrSetNoWireExtensionWarnings(int warn)
{
  lefrNoWireExtensionWarnings = warn;
}

void 
lefrSetOutputAntennaWarnings(int warn)
{
  lefrOutputAntennaWarnings = warn;
}

void 
lefrSetPinWarnings(int warn)
{
  lefrPinWarnings = warn;
}

void 
lefrSetSiteWarnings(int warn)
{
  lefrSiteWarnings = warn;
}

void 
lefrSetSpacingWarnings(int warn)
{
  lefrSpacingWarnings = warn;
}

void 
lefrSetTimingWarnings(int warn)
{
  lefrTimingWarnings = warn;
}

void 
lefrSetUnitsWarnings(int warn)
{
  lefrUnitsWarnings = warn;
}

void 
lefrSetUseMinSpacingWarnings(int warn)
{
  lefrUseMinSpacingWarnings = warn;
}

void 
lefrSetViaRuleWarnings(int warn)
{
  lefrViaRuleWarnings = warn;
}

void 
lefrSetViaWarnings(int warn)
{
  lefrViaWarnings = warn;
}
