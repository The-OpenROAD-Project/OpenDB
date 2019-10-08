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

#ifndef LEFRREADER_H
#define LEFRREADER_H

#include <stdarg.h>
#include <stdio.h>

#include "lefiKRDefs.hpp"
#include "lefiDefs.hpp"
#include "lefiUser.hpp"
#include "lefiUtil.hpp"

/* The reader initialization.  Must be called before lefrRead().  */
EXTERN int lefrInit PROTO_PARAMS(( void ));
/* The reader initialization to start reading another LEF file.
 * It must be called before the 2nd, 3rd, etc call to lefrRead(). */
EXTERN int lefrReset PROTO_PARAMS(( void ));

/* Change the comment character in LEF.  The normal character is
 * '#'.   You can change it to anything you want, but be careful.
 */
EXTERN void lefrSetCommentChar PROTO_PARAMS((char c));

/* Allow the parser to upshift all names if the LEF
 * file is case insensitive.  The default is no shift, so the user
 * must do case insensitive matching.
 */
EXTERN void lefrSetShiftCase PROTO_PARAMS(( void ));

/* Allow the user to change the casesensitivity anytime during
 * parsing.
 * caseSen = 0, will turn off the casesensitivity
 * caseSen != 0, will turn on the casesensitivity
 */
EXTERN void lefrSetCaseSensitivity PROTO_PARAMS((int caseSense));

/* The reader request the file name they are parsing
 */
EXTERN const char * lefrFName PROTO_PARAMS(( void ));

/*
 * The main reader function.
 * The file should already be opened.  This requirement allows
 * the reader to be used with stdin or a pipe.  The file name
 * is only used for error messages.  The includeSearchPath is
 * a colon-delimited list of directories in which to find
 * include files.
 */
EXTERN int lefrRead
  PROTO_PARAMS(( FILE *file,
                 const char *fileName,
                 lefiUserData userData ));

/*
 * Set all of the callbacks that have not yet been set to a function
 * that will add up how many times a given lef data type was ignored
 * (ie no callback was done).  The statistics can later be printed out.
 */
EXTERN void lefrSetRegisterUnusedCallbacks PROTO_PARAMS(( void ));
EXTERN void lefrPrintUnusedCallbacks PROTO_PARAMS(( FILE* f ));

/*
 * Set/get the client-provided user data.  lefi doesn't look at
 * this data at all, it simply passes the opaque lefiUserData pointer
 * back to the application with each callback.  The client can
 * change the data at any time, and it will take effect on the
 * next callback.  The lefi reader and writer maintain separate
 * user data pointers.
 */
EXTERN void lefrSetUserData PROTO_PARAMS(( lefiUserData ));
EXTERN lefiUserData lefrGetUserData PROTO_PARAMS(( void ));
 
/*
 * The error and warning functions.  These take a printf style
 * argument list, and will call the corresponding callbacks
 * (lefrErrorCbkFnType and lefrWarningCbkFnType) if they are supplied.
 * The messages are formatted before calling the callbacks.
 */
/*
 * EXTERN void lefrError PROTO_PARAMS(( const char *, ... ));
 * EXTERN void lefrWarning PROTO_PARAMS(( const char *, ... ));
 * EXTERN void lefrVError PROTO_PARAMS(( const char *, va_list ));
 * EXTERN void lefrVWarning PROTO_PARAMS(( const char *, va_list ));
 * EXTERN int  lefrGetCurrentLineNumber PROTO_PARAMS((void));
 * EXTERN const char *lefrGetCurrentFileName PROTO_PARAMS((void));
 */
 
/*
 * An enum describing all of the types of reader callbacks.
 */
typedef enum {
  lefrUnspecifiedCbkType = 0,
  lefrVersionCbkType,
  lefrVersionStrCbkType,
  lefrDividerCharCbkType,
  lefrBusBitCharsCbkType,
  lefrUnitsCbkType,
  lefrCaseSensitiveCbkType,
  lefrNoWireExtensionCbkType,
  lefrPropBeginCbkType,
  lefrPropCbkType,
  lefrPropEndCbkType,
  lefrLayerCbkType,
  lefrViaCbkType,
  lefrViaRuleCbkType,
  lefrSpacingCbkType,
  lefrIRDropCbkType,
  lefrDielectricCbkType,
  lefrMinFeatureCbkType,
  lefrNonDefaultCbkType,
  lefrSiteCbkType,
  lefrMacroBeginCbkType,
  lefrPinCbkType,
  lefrMacroCbkType,
  lefrObstructionCbkType,
  lefrArrayCbkType,

  /* NEW CALLBACKS - each callback has its own type.  For each callback
   * that you add, you must add an item to this enum. */

  lefrSpacingBeginCbkType,
  lefrSpacingEndCbkType,
  lefrArrayBeginCbkType,
  lefrArrayEndCbkType,
  lefrIRDropBeginCbkType,
  lefrIRDropEndCbkType,
  lefrNoiseMarginCbkType,
  lefrEdgeRateThreshold1CbkType,
  lefrEdgeRateThreshold2CbkType,
  lefrEdgeRateScaleFactorCbkType,
  lefrNoiseTableCbkType,
  lefrCorrectionTableCbkType,
  lefrInputAntennaCbkType,
  lefrOutputAntennaCbkType,
  lefrInoutAntennaCbkType,
  lefrAntennaInputCbkType,
  lefrAntennaInoutCbkType,
  lefrAntennaOutputCbkType,
  lefrManufacturingCbkType,
  lefrUseMinSpacingCbkType,
  lefrClearanceMeasureCbkType,
  lefrTimingCbkType,
  lefrMacroClassTypeCbkType,
  lefrMacroOriginCbkType,
  lefrMacroSizeCbkType,
  lefrMacroEndCbkType,
  lefrMaxStackViaCbkType,

  lefrLibraryEndCbkType
} lefrCallbackType_e;
 
 
/* Declarations of function signatures for each type of callback.
 * These declarations are type-safe when compiling with ANSI C
 * or C++; you will only be able to register a function pointer
 * with the correct signature for a given type of callback.
 *
 * Each callback function is expected to return 0 if successful.
 * A non-zero return code will cause the reader to abort.
 *
 * The lefrDesignStart and lefrDesignEnd callback is only called once.
 * Other callbacks may be called multiple times, each time with a different
 * set of data.
 *
 * For each callback, the Def API will make the callback to the
 * function supplied by the client, which should either make a copy
 * of the Def object, or store the data in the client's own data structures.
 * The Def API will delete or reuse each object after making the callback,
 * so the client should not keep a pointer to it.
 *
 * All callbacks pass the user data pointer provided in lefrRead()
 * or lefrSetUserData() back to the client; this can be used by the
 * client to obtain access to the rest of the client's data structures.
 *
 * The user data pointer is obtained using lefrGetUserData() immediately
 * prior to making each callback, so the client is free to change the
 * user data on the fly if necessary.
 *
 * Callbacks with the same signature are passed a callback type
 * parameter, which allows an application to write a single callback
 * function, register that function for multiple callbacks, then
 * switch based on the callback type to handle the appropriate type of
 * data.
 */
 

/* A declaration of the signature of all callbacks that return nothing. */
typedef int (*lefrVoidCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
		   void* num,
                   lefiUserData ));

/* A declaration of the signature of all callbacks that return a string. */
typedef int (*lefrStringCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   const char *string,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a integer. */
typedef int (*lefrIntegerCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   int number,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a double. */
typedef int (*lefrDoubleCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   double number,
                   lefiUserData ));

/* A declaration of the signature of all callbacks that return a lefiUnits. */
typedef int (*lefrUnitsCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiUnits* units,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiLayer. */
typedef int (*lefrLayerCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiLayer* l,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiVia. */
typedef int (*lefrViaCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiVia* l,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiViaRule. */
typedef int (*lefrViaRuleCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiViaRule* l,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiSpacing. */
typedef int (*lefrSpacingCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiSpacing* l,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiIRDrop. */
typedef int (*lefrIRDropCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiIRDrop* l,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiMinFeature. */
typedef int (*lefrMinFeatureCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiMinFeature* l,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiNonDefault. */
typedef int (*lefrNonDefaultCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiNonDefault* l,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiSite. */
typedef int (*lefrSiteCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiSite* l,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiMacro. */
typedef int (*lefrMacroCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiMacro* l,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiPin. */
typedef int (*lefrPinCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiPin* l,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiObstruction. */
typedef int (*lefrObstructionCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiObstruction* l,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiArray. */
typedef int (*lefrArrayCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiArray* l,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiProp. */
typedef int (*lefrPropCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiProp* p,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiNoiseMargin. */
typedef int (*lefrNoiseMarginCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   struct lefiNoiseMargin* p,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiNoiseTable. */
typedef int (*lefrNoiseTableCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiNoiseTable* p,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiCorrectionTable. */
typedef int (*lefrCorrectionTableCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiCorrectionTable* p,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiTiming. */
typedef int (*lefrTimingCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiTiming* p,
                   lefiUserData ));
 
/* A declaration of the signature of all callbacks that return a lefiUseMinSpacing. */
typedef int (*lefrUseMinSpacingCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiUseMinSpacing* l,
                   lefiUserData ));
 
 /* NEW CALLBACK - If your callback returns a pointer to a new class then
  * you must add a type function here. */


/* A declaration of the signature of all callbacks that return a lefiMaxStackVia. */
typedef int (*lefrMaxStackViaCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiMaxStackVia* l,
                   lefiUserData ));

typedef int (*lefrMacroNumCbkFnType)
    PROTO_PARAMS(( lefrCallbackType_e,
                   lefiNum l,
                   lefiUserData ));

/* Functions to call to register a callback function.
 */
EXTERN void lefrSetUnitsCbk(lefrUnitsCbkFnType);
EXTERN void lefrSetVersionCbk(lefrDoubleCbkFnType);
EXTERN void lefrSetVersionStrCbk(lefrStringCbkFnType);
EXTERN void lefrSetDividerCharCbk(lefrStringCbkFnType);
EXTERN void lefrSetBusBitCharsCbk(lefrStringCbkFnType);
EXTERN void lefrSetNoWireExtensionCbk(lefrStringCbkFnType);
EXTERN void lefrSetCaseSensitiveCbk(lefrIntegerCbkFnType);
EXTERN void lefrSetNoWireExtensionCbk(lefrStringCbkFnType);
EXTERN void lefrSetPropBeginCbk(lefrVoidCbkFnType);
EXTERN void lefrSetPropCbk(lefrPropCbkFnType);
EXTERN void lefrSetPropEndCbk(lefrVoidCbkFnType);
EXTERN void lefrSetLayerCbk(lefrLayerCbkFnType);
EXTERN void lefrSetViaCbk(lefrViaCbkFnType);
EXTERN void lefrSetViaRuleCbk(lefrViaRuleCbkFnType);
EXTERN void lefrSetSpacingCbk(lefrSpacingCbkFnType);
EXTERN void lefrSetIRDropCbk(lefrIRDropCbkFnType);
EXTERN void lefrSetDielectricCbk(lefrDoubleCbkFnType);
EXTERN void lefrSetMinFeatureCbk(lefrMinFeatureCbkFnType);
EXTERN void lefrSetNonDefaultCbk(lefrNonDefaultCbkFnType);
EXTERN void lefrSetSiteCbk(lefrSiteCbkFnType);
EXTERN void lefrSetMacroBeginCbk(lefrStringCbkFnType);
EXTERN void lefrSetPinCbk(lefrPinCbkFnType);
EXTERN void lefrSetObstructionCbk(lefrObstructionCbkFnType);
EXTERN void lefrSetArrayCbk(lefrArrayCbkFnType);
EXTERN void lefrSetMacroCbk(lefrMacroCbkFnType);
EXTERN void lefrSetLibraryEndCbk(lefrVoidCbkFnType);

/* NEW CALLBACK - each callback must have a function to allow the user
 * to set it.  Add the function here. */

EXTERN void lefrSetTimingCbk(lefrTimingCbkFnType);
EXTERN void lefrSetSpacingBeginCbk(lefrVoidCbkFnType);
EXTERN void lefrSetSpacingEndCbk(lefrVoidCbkFnType);
EXTERN void lefrSetArrayBeginCbk(lefrStringCbkFnType);
EXTERN void lefrSetArrayEndCbk(lefrStringCbkFnType);
EXTERN void lefrSetIRDropBeginCbk(lefrVoidCbkFnType);
EXTERN void lefrSetIRDropEndCbk(lefrVoidCbkFnType);
EXTERN void lefrSetNoiseMarginCbk(lefrNoiseMarginCbkFnType);
EXTERN void lefrSetEdgeRateThreshold1Cbk(lefrDoubleCbkFnType);
EXTERN void lefrSetEdgeRateThreshold2Cbk(lefrDoubleCbkFnType);
EXTERN void lefrSetEdgeRateScaleFactorCbk(lefrDoubleCbkFnType);
EXTERN void lefrSetNoiseTableCbk(lefrNoiseTableCbkFnType);
EXTERN void lefrSetCorrectionTableCbk(lefrCorrectionTableCbkFnType);
EXTERN void lefrSetInputAntennaCbk(lefrDoubleCbkFnType);
EXTERN void lefrSetOutputAntennaCbk(lefrDoubleCbkFnType);
EXTERN void lefrSetInoutAntennaCbk(lefrDoubleCbkFnType);
EXTERN void lefrSetAntennaInputCbk(lefrDoubleCbkFnType);
EXTERN void lefrSetAntennaInoutCbk(lefrDoubleCbkFnType);
EXTERN void lefrSetAntennaOutputCbk(lefrDoubleCbkFnType);
EXTERN void lefrSetClearanceMeasureCbk(lefrStringCbkFnType);
EXTERN void lefrSetManufacturingCbk(lefrDoubleCbkFnType);
EXTERN void lefrSetUseMinSpacingCbk(lefrUseMinSpacingCbkFnType);
EXTERN void lefrSetMacroClassTypeCbk(lefrStringCbkFnType);
EXTERN void lefrSetMacroOriginCbk(lefrMacroNumCbkFnType);
EXTERN void lefrSetMacroSizeCbk(lefrMacroNumCbkFnType);
EXTERN void lefrSetMacroEndCbk(lefrStringCbkFnType);
EXTERN void lefrSetMaxStackViaCbk(lefrMaxStackViaCbkFnType);


/*
 * Set all of the callbacks that have not yet been set to the following
 * function.  This is especially useful if you want to check to see
 * if you forgot anything.
 */
EXTERN void lefrSetUnusedCallbacks PROTO_PARAMS((lefrVoidCbkFnType func));

/*
 * Return the current line number in the parser.
 */
EXTERN int lefrLineNumber PROTO_PARAMS(( void ));

/* Routine to set the message logging routine for errors */
typedef void (*LEFI_LOG_FUNCTION)(const char*);
void lefrSetLogFunction( LEFI_LOG_FUNCTION );

/* Routine to set the message logging routine for warnings */
typedef void (*LEFI_WARNING_LOG_FUNCTION)(const char*);
void lefrSetWarningLogFunction( LEFI_WARNING_LOG_FUNCTION );

/* Routine to set the user defined malloc routine */
typedef void* (*LEFI_MALLOC_FUNCTION)(int);
void lefrSetMallocFunction( LEFI_MALLOC_FUNCTION );

/* Routine to set the user defined realloc routine */
typedef void* (*LEFI_REALLOC_FUNCTION)(void *, int);
void lefrSetReallocFunction( LEFI_REALLOC_FUNCTION );

/* Routine to set the user defined free routine */
typedef void (*LEFI_FREE_FUNCTION)(void *);
void lefrSetFreeFunction( LEFI_FREE_FUNCTION );

/* Routine to set the line number callback routine */
typedef void (*LEFI_LINE_NUMBER_FUNCTION)(int);
void lefrSetLineNumberFunction( LEFI_LINE_NUMBER_FUNCTION);

/* Set the number of lines before calling the line function callback routine */
/* Default is 10000 */
void lefrSetDeltaNumberLines ( int );

extern void lefrNerr( int num );
extern void lefrNwarn( int num );

/* Return codes for the user callbacks.
 * The user should return one of these values. */
#define PARSE_OK 0      /* continue parsing */
#define STOP_PARSE 1    /* stop parsing with no error message */
#define PARSE_ERROR 2   /* stop parsing, print an error message */

#endif
