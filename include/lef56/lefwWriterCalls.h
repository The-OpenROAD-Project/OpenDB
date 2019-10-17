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

#ifndef LEFI_WRITER_H
#define LEFI_WRITER_H
#include <stdarg.h>
#include <stdio.h>
#include "lefiKRDefs.h"
#include "lefiDefs.h"
#include "lefiUser.h"

/*
 * The main writer function. The file should already be opened.  This
 * requirement allows the writer to be used with stdin or a pipe.  The file
 * name is only used for error messages.  The includeSearchPath is a
 * colon-delimited list of directories in which to find include files.
 */
EXTERN int lefwWrite
  PROTO_PARAMS(( FILE * file,
                 const char *fileName,
                 lefiUserData userData ));

/*
 * Set all of the callbacks that have not yet been set to a function that
 * will add up how many times a given lef data type was ignored (ie no
 * callback was done).  The statistics can later be printed out.
 */
EXTERN void lefwSetRegisterUnusedCallbacks PROTO_PARAMS(( void ));

EXTERN void lefwPrintUnusedCallbacks PROTO_PARAMS(( FILE * f ));

/*
 * Set/get the client-provided user data.  lefi doesn't look at this data at
 * all, it simply passes the opaque lefiUserData pointer back to the
 * application with each callback.  The client can change the data at any
 * time, and it will take effect on the next callback.  The lefi writer and
 * writer maintain separate user data pointers.
 */
EXTERN void lefwSetUserData PROTO_PARAMS(( lefiUserData ));

EXTERN lefiUserData lefwGetUserData PROTO_PARAMS(( void ));

/*
 * An enum describing all of the types of writer callbacks.
 */
typedef enum {
  lefwUnspecifiedCbkType = 0,
  lefwVersionCbkType,
  lefwCaseSensitiveCbkType,
  lefwNoWireExtensionCbkType,
  lefwBusBitCharsCbkType,
  lefwDividerCharCbkType,
  lefwManufacturingGridCbkType,
  lefwUseMinSpacingCbkType,
  lefwClearanceMeasureCbkType,
  lefwUnitsCbkType,
  lefwAntennaInputGateAreaCbkType,
  lefwAntennaInOutDiffAreaCbkType,
  lefwAntennaOutputDiffAreaCbkType,
  lefwPropDefCbkType,
  lefwLayerCbkType,
  lefwViaCbkType,
  lefwViaRuleCbkType,
  lefwNonDefaultCbkType,
  lefwCrossTalkCbkType,
  lefwNoiseTableCbkType,
  lefwCorrectionTableCbkType,
  lefwSpacingCbkType,
  lefwMinFeatureCbkType,
  lefwDielectricCbkType,
  lefwIRDropCbkType,
  lefwSiteCbkType,
  lefwArrayCbkType,
  lefwMacroCbkType,
  lefwAntennaCbkType,
  lefwExtCbkType,
  lefwEndLibCbkType

  /*
       * NEW CALLBACKS - each callback has its own type.  For each callback
       * that you add, you must add an item to this enum.
       */

} lefwCallbackType_e;

/*
 * Declarations of function signatures for each type of callback. These
 * declarations are type-safe when compiling with ANSI C or C++; you will
 * only be able to register a function pointer with the correct signature for
 * a given type of callback.
 * 
 * Each callback function is expected to return 0 if successful. A non-zero
 * return code will cause the writer to abort.
 * 
 * The lefwDesignStart and lefwDesignEnd callback is only called once. Other
 * callbacks may be called multiple times, each time with a different set of
 * data.
 * 
 * For each callback, the Lef API will make the callback to the function
 * supplied by the client, which should either make a copy of the Lef object,
 * or store the data in the client's own data structures. The Lef API will
 * delete or reuse each object after making the callback, so the client
 * should not keep a pointer to it.
 * 
 * All callbacks pass the user data pointer provided in lefwRead() or
 * lefwSetUserData() back to the client; this can be used by the client to
 * obtain access to the rest of the client's data structures.
 * 
 * The user data pointer is obtained using lefwGetUserData() immediately prior
 * to making each callback, so the client is free to change the user data on
 * the fly if necessary.
 * 
 * Callbacks with the same signature are passed a callback type parameter, which
 * allows an application to write a single callback function, register that
 * function for multiple callbacks, then switch based on the callback type to
 * handle the appropriate type of data.
 */
/* A declaration of the signature of all callbacks that return nothing. */
typedef int (*lefwVoidCbkFnType)
  PROTO_PARAMS(( lefwCallbackType_e,
                 lefiUserData ));

/*
 * NEW CALLBACK - If your callback returns a pointer to a new class then you
 * must add a type function here.
 */

/*
 * Functions to call to register a callback function.
 */
EXTERN void lefwSetVersionCbk(lefwVoidCbkFnType);

EXTERN void lefwSetCaseSensitiveCbk(lefwVoidCbkFnType);

EXTERN void lefwSetNoWireExtensionCbk(lefwVoidCbkFnType);

EXTERN void lefwSetBusBitCharsCbk(lefwVoidCbkFnType);

EXTERN void lefwSetDividerCharCbk(lefwVoidCbkFnType);

EXTERN void lefwSetManufacturingGridCbk(lefwVoidCbkFnType);

EXTERN void lefwSetUseMinSpacingCbk(lefwVoidCbkFnType);

EXTERN void lefwSetClearanceMeasureCbk(lefwVoidCbkFnType);

EXTERN void lefwSetUnitsCbk(lefwVoidCbkFnType);

EXTERN void lefwAntennaInputGateAreaCbk(lefwVoidCbkFnType);

EXTERN void lefwAntennaInOutDiffAreaCbk(lefwVoidCbkFnType);

EXTERN void lefwAntennaOutputDiffAreaCbk(lefwVoidCbkFnType);

EXTERN void lefwSetPropDefCbk(lefwVoidCbkFnType);

EXTERN void lefwSetLayerCbk(lefwVoidCbkFnType);

EXTERN void lefwSetViaCbk(lefwVoidCbkFnType);

EXTERN void lefwSetViaRuleCbk(lefwVoidCbkFnType);

EXTERN void lefwSetNonDefaultCbk(lefwVoidCbkFnType);

EXTERN void lefwSetCrossTalkCbk(lefwVoidCbkFnType);

EXTERN void lefwSetNoiseTableCbk(lefwVoidCbkFnType);

EXTERN void lefwSetCorrectionTableCbk(lefwVoidCbkFnType);

EXTERN void lefwSetSpacingCbk(lefwVoidCbkFnType);

EXTERN void lefwSetMinFeatureCbk(lefwVoidCbkFnType);

EXTERN void lefwSetDielectricCbk(lefwVoidCbkFnType);

EXTERN void lefwSetIRDropCbk(lefwVoidCbkFnType);

EXTERN void lefwSetSiteCbk(lefwVoidCbkFnType);

EXTERN void lefwSetArrayCbk(lefwVoidCbkFnType);

EXTERN void lefwSetMacroCbk(lefwVoidCbkFnType);

EXTERN void lefwSetAntennaCbk(lefwVoidCbkFnType);

EXTERN void lefwSetExtCbk(lefwVoidCbkFnType);

EXTERN void lefwSetEndLibCbk(lefwVoidCbkFnType);

/*
 * NEW CALLBACK - each callback must have a function to allow the user to set
 * it.  Add the function here.
 */

/*
 * Set all of the callbacks that have not yet been set to the following
 * function.  This is especially useful if you want to check to see if you
 * forgot anything.
 */
EXTERN void lefwSetUnusedCallbacks PROTO_PARAMS(( lefwVoidCbkFnType func ));

/* Routine to set the message logging routine for errors */
typedef void (*LEFI_LOG_FUNCTION) (const char *);

void            lefwSetLogFunction(LEFI_LOG_FUNCTION);

/* Routine to set the message logging routine for warnings */
typedef void (*LEFI_WARNING_LOG_FUNCTION) (const char *);

void            lefwSetWarningLogFunction(LEFI_WARNING_LOG_FUNCTION);

#endif
