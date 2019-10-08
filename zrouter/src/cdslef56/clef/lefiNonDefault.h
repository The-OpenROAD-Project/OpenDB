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

#ifndef lefiNonDefault_h
#define lefiNonDefault_h
#include <stdio.h>
#include "lefiKRDefs.h"
#include "lefiVia.h"
#include "lefiMisc.h"

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * A non default rule can have one or more layers.
 */

/*
 * The layer information is kept in an array.
 */

/*
 * obsolete in 5.6
 */

/*
 * obsolete in 5.6
 */

/*
 * obsolete in 5.6
 */

/*
 * obsolete in 5.6
 */

/*
 * obsolete in 5.6
 */

/*
 * obsolete in 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * A non default rule can have one or more vias.
 */

/*
 * These routines return the via info.
 */

/*
 * A non default rule can have one or more spacing rules.
 */

/*
 * These routines return the that info.
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * Debug print
 */

/*
 * save the users' functions.
 */

/*
 * save the users' functions.
 */

/*
 * Layer information
 */

/*
 * 5.4
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

typedef struct lefiNonDefault_s {
  void           *oldViaCbk_;
  void           *oldSpacingCbk_;
  void           *oldSpacingBeginCbk_;
  void           *oldSpacingEndCbk_;
  void           *oldUserData_;
  int             nameSize_;
  char           *name_;
  int             numLayers_;
  int             layersAllocated_;
  char          **layerName_;
  double         *width_;
  double         *spacing_;
  double         *wireExtension_;
  char           *hasWidth_;
  char           *hasSpacing_;
  char           *hasWireExtension_;
  double         *resistance_;
  double         *capacitance_;
  double         *edgeCap_;
  char           *hasResistance_;
  char           *hasCapacitance_;
  char           *hasEdgeCap_;
  double         *diagWidth_;
  char           *hasDiagWidth_;
  int             numVias_;
  int             allocatedVias_;
  lefiVia       **viaRules_;
  int             numSpacing_;
  int             allocatedSpacing_;
  lefiSpacing   **spacingRules_;
  int             hardSpacing_;
  int             numUseVias_;
  int             allocatedUseVias_;
  char          **useViaName_;
  int             numUseViaRules_;
  int             allocatedUseViaRules_;
  char          **useViaRuleName_;
  int             numMinCuts_;
  int             allocatedMinCuts_;
  char          **cutLayerName_;
  int            *numCuts_;
  int             numProps_;
  int             propsAllocated_;
  char          **names_;
  char          **values_;
  double         *dvalues_;
  char           *types_;
} lefiNonDefault;

EXTERN lefiNonDefault *
lefiNonDefault_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiNonDefault_Init
  PROTO_PARAMS(( lefiNonDefault * this ));

EXTERN void
lefiNonDefault_Destroy
  PROTO_PARAMS(( lefiNonDefault * this ));

EXTERN void
lefiNonDefault_Delete
  PROTO_PARAMS(( lefiNonDefault * this ));

EXTERN void
lefiNonDefault_setName
  PROTO_PARAMS(( lefiNonDefault * this,
                 const char *name ));

EXTERN void
lefiNonDefault_addLayer
  PROTO_PARAMS(( lefiNonDefault * this,
                 const char *name ));

EXTERN void
lefiNonDefault_addWidth
  PROTO_PARAMS(( lefiNonDefault * this,
                 double num ));

EXTERN void
lefiNonDefault_addWireExtension
  PROTO_PARAMS(( lefiNonDefault * this,
                 double num ));

EXTERN void
lefiNonDefault_addSpacing
  PROTO_PARAMS(( lefiNonDefault * this,
                 double num ));

EXTERN void
lefiNonDefault_addSpacingRule
  PROTO_PARAMS(( lefiNonDefault * this,
                 lefiSpacing * s ));

EXTERN void
lefiNonDefault_addResistance
  PROTO_PARAMS(( lefiNonDefault * this,
                 double num ));

EXTERN void
lefiNonDefault_addCapacitance
  PROTO_PARAMS(( lefiNonDefault * this,
                 double num ));

EXTERN void
lefiNonDefault_addEdgeCap
  PROTO_PARAMS(( lefiNonDefault * this,
                 double num ));

EXTERN void
lefiNonDefault_addViaRule
  PROTO_PARAMS(( lefiNonDefault * this,
                 lefiVia * v ));

EXTERN void
lefiNonDefault_addDiagWidth
  PROTO_PARAMS(( lefiNonDefault * this,
                 double num ));

EXTERN void
lefiNonDefault_end
  PROTO_PARAMS(( lefiNonDefault * this ));

EXTERN void
lefiNonDefault_clear
  PROTO_PARAMS(( lefiNonDefault * this ));

EXTERN void
lefiNonDefault_addProp
  PROTO_PARAMS(( lefiNonDefault * this,
                 const char *name,
                 const char *value,
                 const char type ));

EXTERN void
lefiNonDefault_addNumProp
  PROTO_PARAMS(( lefiNonDefault * this,
                 const char *name,
                 const double d,
                 const char *value,
                 const char type ));

EXTERN void
lefiNonDefault_setHardspacing
  PROTO_PARAMS(( lefiNonDefault * this ));

EXTERN void
lefiNonDefault_addUseVia
  PROTO_PARAMS(( lefiNonDefault * this,
                 const char *name ));

EXTERN void
lefiNonDefault_addUseViaRule
  PROTO_PARAMS(( lefiNonDefault * this,
                 const char *name ));

EXTERN void
lefiNonDefault_addMinCuts
  PROTO_PARAMS(( lefiNonDefault * this,
                 const char *name,
                 int numCuts ));

EXTERN const char *
lefiNonDefault_name
  PROTO_PARAMS(( const lefiNonDefault * this ));

EXTERN int
lefiNonDefault_hasHardspacing
  PROTO_PARAMS(( const lefiNonDefault * this ));

EXTERN int
lefiNonDefault_numProps
  PROTO_PARAMS(( const lefiNonDefault * this ));

EXTERN const char *
lefiNonDefault_propName
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN const char *
lefiNonDefault_propValue
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN double
lefiNonDefault_propNumber
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN const char
lefiNonDefault_propType
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_propIsNumber
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_propIsString
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_numLayers
  PROTO_PARAMS(( const lefiNonDefault * this ));

EXTERN const char *
lefiNonDefault_layerName
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_hasLayerWidth
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN double
lefiNonDefault_layerWidth
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_hasLayerSpacing
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN double
lefiNonDefault_layerSpacing
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_hasLayerWireExtension
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN double
lefiNonDefault_layerWireExtension
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_hasLayerResistance
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN double
lefiNonDefault_layerResistance
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_hasLayerCapacitance
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN double
lefiNonDefault_layerCapacitance
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_hasLayerEdgeCap
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN double
lefiNonDefault_layerEdgeCap
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_hasLayerDiagWidth
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN double
lefiNonDefault_layerDiagWidth
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_numVias
  PROTO_PARAMS(( const lefiNonDefault * this ));

EXTERN lefiVia *
lefiNonDefault_viaRule
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_numSpacingRules
  PROTO_PARAMS(( const lefiNonDefault * this ));

EXTERN lefiSpacing *
lefiNonDefault_spacingRule
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_numUseVia
  PROTO_PARAMS(( const lefiNonDefault * this ));

EXTERN const char *
lefiNonDefault_viaName
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_numUseViaRule
  PROTO_PARAMS(( const lefiNonDefault * this ));

EXTERN const char *
lefiNonDefault_viaRuleName
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_numMinCuts
  PROTO_PARAMS(( const lefiNonDefault * this ));

EXTERN const char *
lefiNonDefault_cutLayerName
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN int
lefiNonDefault_numCuts
  PROTO_PARAMS(( const lefiNonDefault * this,
                 int index ));

EXTERN void
lefiNonDefault_print
  PROTO_PARAMS(( lefiNonDefault * this,
                 FILE * f ));

#endif
