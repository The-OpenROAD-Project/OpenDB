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
#include "lefrReader.h"
#include "lex.h"
#include <stdlib.h>
#include <string.h>
#include "lefiDebug.h"
extern int lefyyparse(void);	/* do the parsing */

extern void lef_lex_init(void);	/* start the lexer */

extern void lef_lex_un_init(void);	/* free up lexer space */

extern int lef_nlines;		/* lexer line number */

static char lefrErrMsg[1024];

extern void lefrNerr(int num);

extern void lefrNwarn(int num);

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

/*
 * Global variables
 *//* 5.5 *//* 5.5 this & lefrHasMaxVS is to keep track that *//* MAXVIASTACK
 * has to be after all layers *//* keep track of spacing table in a layer *//* k
 * eep track of spacing in a layer *//* The following global variables are
 * for storing the propertydefination *//* types.  Only real & integer need
 * to store since the parser can *//* distinguish string and quote string *//* T
 * he following global variables are used in lef.y *//* keep track if there
 * is any 5.3 on 5.4 file  */
/* keep track if only 5.4 is allowed  */
/* keep track if version is in file  */
/* keep track if namescasesensitive is in file  */
/* keep track if bustbitchars is in file  */
/* keep track if dividerchar is in file  */
/* keep track for oxide  */
/* default is 5.4  */

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
 * Function to initialize global variables.
 *//* This make sure the global variables are initialized */ lefiUserData lefrUserData = 0;

char   *lefrFileName = 0;

static int lefrRegisterUnused = 0;

FILE   *lefrFile = 0;

lefiUnits lefrUnits;

lefiLayer lefrLayer;

lefiVia lefrVia;

lefiViaRule lefrViaRule;

lefiSpacing lefrSpacing;

lefiIRDrop lefrIRDrop;

lefiMinFeature lefrMinFeature;

lefiNonDefault lefrNonDefault;

lefiSite lefrSite;

lefiMacro lefrMacro;

lefiPin lefrPin;

lefiObstruction lefrObstruction;

lefiArray lefrArray;

lefiSitePattern *lefrSitePatternPtr = 0;

int     lefrDoSite = 0;

lefiGeometries *lefrGeometriesPtr = 0;

int     lefrDoGeometries = 0;

lefiTrackPattern *lefrTrackPatternPtr = 0;

int     lefrDoTrack = 0;

lefiGcellPattern *lefrGcellPatternPtr = 0;

int     lefrDoGcell = 0;

int     lefrIsReset = 0;

int     lefrShiftCase = 0;

lefiProp lefrProp;

lefiTiming lefrTiming;

struct lefiNoiseMargin lefrNoiseMargin;

lefiNoiseTable lefrNoiseTable;

lefiCorrectionTable lefrCorrectionTable;

lefiUseMinSpacing lefrUseMinSpacing;

lefiAntennaPWL *lefrAntennaPWLPtr = 0;

lefiMaxStackVia lefrMaxStackVia;

int     lefrHasLayer = 0;

int     lefrHasMaxVS = 0;

int     lefrHasSpacingTbl;

int     lefrHasSpacing;

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

double  versionNum = 5.5;

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

void 
lefrInitGlobals()
{
  lefrUserData = 0;
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
  lefrIsReset = 0;
  lefrShiftCase = 0;
  lefrAntennaPWLPtr = 0;
  lefrHasLayer = 0;
  lefrHasMaxVS = 0;
  lefrHasSpacingTbl = 0;
  lefrHasSpacing = 0;
  return;
}

/*
 * Parser control by the user.
 *//* Reader initialization */ int 
lefrInit()
{
  lefrInitGlobals();
  lefrReset();
  return 0;
}

int 
lefrReset()
{
  lefrIsReset = 1;

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
#endif

  use5_3 = 0;
  use5_4 = 0;
  hasVer = 0;
  hasNameCase = 0;
  hasBusBit = 0;
  hasDivChar = 0;
  aOxide = 0;
  versionNum = 5.5;
  return 0;
}

const char *
lefrFName()
{
  return lefrFileName;
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
      if (first && i != 47 && i != 48 && i != 49) {
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
      case lefrMacroClassTypeCbkType:
      case lefrMacroOriginCbkType:
      case lefrMacroSizeCbkType:
      case lefrMacroEndCbkType:
	trueCB = 0;
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
}
