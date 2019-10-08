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

#ifndef lefiViaRule_h
#define lefiViaRule_h
#include <stdio.h>
#include "lefiKRDefs.h"

/*
 * 5.5
 */

/*
 * 5.5
 */

/*
 * 5.5
 */

/*
 * 5.5
 */

/*
 * Debug print
 */

/*
 * 5.5
 */

/*
 * 5.5
 */

typedef struct lefiViaRuleLayer_s {
  char           *name_;
  char            direction_;
  double          overhang1_;
  double          overhang2_;
  int             hasWidth_;
  int             hasResistance_;
  int             hasOverhang_;
  int             hasMetalOverhang_;
  int             hasSpacing_;
  int             hasRect_;
  double          widthMin_;
  double          widthMax_;
  double          overhang_;
  double          metalOverhang_;
  double          resistance_;
  double          spacingStepX_;
  double          spacingStepY_;
  double          xl_, yl_, xh_, yh_;
} lefiViaRuleLayer;

EXTERN lefiViaRuleLayer *
lefiViaRuleLayer_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiViaRuleLayer_Init
  PROTO_PARAMS(( lefiViaRuleLayer * this ));

EXTERN void
lefiViaRuleLayer_Destroy
  PROTO_PARAMS(( lefiViaRuleLayer * this ));

EXTERN void
lefiViaRuleLayer_Delete
  PROTO_PARAMS(( lefiViaRuleLayer * this ));

EXTERN void
lefiViaRuleLayer_setName
  PROTO_PARAMS(( lefiViaRuleLayer * this,
                 const char *name ));

EXTERN void
lefiViaRuleLayer_setHorizontal
  PROTO_PARAMS(( lefiViaRuleLayer * this ));

EXTERN void
lefiViaRuleLayer_setVertical
  PROTO_PARAMS(( lefiViaRuleLayer * this ));

EXTERN void
lefiViaRuleLayer_setEnclosure
  PROTO_PARAMS(( lefiViaRuleLayer * this,
                 double overhang1,
                 double overhang2 ));

EXTERN void
lefiViaRuleLayer_setWidth
  PROTO_PARAMS(( lefiViaRuleLayer * this,
                 double minW,
                 double maxW ));

EXTERN void
lefiViaRuleLayer_setOverhang
  PROTO_PARAMS(( lefiViaRuleLayer * this,
                 double d ));

EXTERN void
lefiViaRuleLayer_setMetalOverhang
  PROTO_PARAMS(( lefiViaRuleLayer * this,
                 double d ));

EXTERN void
lefiViaRuleLayer_setResistance
  PROTO_PARAMS(( lefiViaRuleLayer * this,
                 double d ));

EXTERN void
lefiViaRuleLayer_setSpacing
  PROTO_PARAMS(( lefiViaRuleLayer * this,
                 double x,
                 double y ));

EXTERN void
lefiViaRuleLayer_setRect
  PROTO_PARAMS(( lefiViaRuleLayer * this,
                 double xl,
                 double yl,
                 double xh,
                 double yh ));

EXTERN int
lefiViaRuleLayer_hasDirection
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN int
lefiViaRuleLayer_hasEnclosure
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN int
lefiViaRuleLayer_hasWidth
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN int
lefiViaRuleLayer_hasResistance
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN int
lefiViaRuleLayer_hasOverhang
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN int
lefiViaRuleLayer_hasMetalOverhang
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN int
lefiViaRuleLayer_hasSpacing
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN int
lefiViaRuleLayer_hasRect
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN char *
lefiViaRuleLayer_name
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN int
lefiViaRuleLayer_isHorizontal
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN int
lefiViaRuleLayer_isVertical
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_enclosureOverhang1
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_enclosureOverhang2
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_widthMin
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_widthMax
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_overhang
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_metalOverhang
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_resistance
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_spacingStepX
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_spacingStepY
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_xl
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_yl
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_xh
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN double
lefiViaRuleLayer_yh
  PROTO_PARAMS(( const lefiViaRuleLayer * this ));

EXTERN void
lefiViaRuleLayer_print
  PROTO_PARAMS(( const lefiViaRuleLayer * this,
                 FILE * f ));

/*
 * This should clear out all the old stuff.
 */

/*
 * Add one of possibly many via names
 */

/*
 * These routines set a part of the active layer.
 */

/*
 * This routine sets and creates the active layer.
 */

/*
 * There are 2 or 3 layers in a rule.
 */

/*
 * numLayers() tells how many.
 */

/*
 * If a third layer exists then it is the cut layer.
 */

/*
 * Debug print
 */

typedef struct lefiViaRule_s {
  char           *name_;
  int             nameSize_;
  int             hasGenerate_;
  int             numLayers_;
  lefiViaRuleLayer layers_[3];
  int             numVias_;
  int             viasAllocated_;
  char          **vias_;
  int             numProps_;
  int             propsAllocated_;
  char          **names_;
  char          **values_;
  double         *dvalues_;
  char           *types_;
} lefiViaRule;

EXTERN lefiViaRule *
lefiViaRule_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiViaRule_Init
  PROTO_PARAMS(( lefiViaRule * this ));

EXTERN void
lefiViaRule_clear
  PROTO_PARAMS(( lefiViaRule * this ));

EXTERN void
lefiViaRule_Destroy
  PROTO_PARAMS(( lefiViaRule * this ));

EXTERN void
lefiViaRule_Delete
  PROTO_PARAMS(( lefiViaRule * this ));

EXTERN void
lefiViaRule_setGenerate
  PROTO_PARAMS(( lefiViaRule * this ));

EXTERN void
lefiViaRule_setName
  PROTO_PARAMS(( lefiViaRule * this,
                 const char *name ));

EXTERN void
lefiViaRule_addViaName
  PROTO_PARAMS(( lefiViaRule * this,
                 const char *name ));

EXTERN void
lefiViaRule_setRect
  PROTO_PARAMS(( lefiViaRule * this,
                 double xl,
                 double yl,
                 double xh,
                 double yh ));

EXTERN void
lefiViaRule_setSpacing
  PROTO_PARAMS(( lefiViaRule * this,
                 double x,
                 double y ));

EXTERN void
lefiViaRule_setWidth
  PROTO_PARAMS(( lefiViaRule * this,
                 double x,
                 double y ));

EXTERN void
lefiViaRule_setResistance
  PROTO_PARAMS(( lefiViaRule * this,
                 double d ));

EXTERN void
lefiViaRule_setOverhang
  PROTO_PARAMS(( lefiViaRule * this,
                 double d ));

EXTERN void
lefiViaRule_setMetalOverhang
  PROTO_PARAMS(( lefiViaRule * this,
                 double d ));

EXTERN void
lefiViaRule_setVertical
  PROTO_PARAMS(( lefiViaRule * this ));

EXTERN void
lefiViaRule_setHorizontal
  PROTO_PARAMS(( lefiViaRule * this ));

EXTERN void
lefiViaRule_setEnclosure
  PROTO_PARAMS(( lefiViaRule * this,
                 double overhang1,
                 double overhang2 ));

EXTERN void
lefiViaRule_addProp
  PROTO_PARAMS(( lefiViaRule * this,
                 const char *name,
                 const char *value,
                 const char type ));

EXTERN void
lefiViaRule_addNumProp
  PROTO_PARAMS(( lefiViaRule * this,
                 const char *name,
                 const double d,
                 const char *value,
                 const char type ));

EXTERN void
lefiViaRule_setLayer
  PROTO_PARAMS(( lefiViaRule * this,
                 const char *name ));

EXTERN int
lefiViaRule_hasGenerate
  PROTO_PARAMS(( const lefiViaRule * this ));

EXTERN char *
lefiViaRule_name
  PROTO_PARAMS(( const lefiViaRule * this ));

EXTERN int
lefiViaRule_numLayers
  PROTO_PARAMS(( const lefiViaRule * this ));

EXTERN lefiViaRuleLayer *
lefiViaRule_layer
  PROTO_PARAMS(( lefiViaRule * this,
                 int index ));

EXTERN int
lefiViaRule_numVias
  PROTO_PARAMS(( const lefiViaRule * this ));

EXTERN char *
lefiViaRule_viaName
  PROTO_PARAMS(( const lefiViaRule * this,
                 int index ));

EXTERN int
lefiViaRule_numProps
  PROTO_PARAMS(( const lefiViaRule * this ));

EXTERN const char *
lefiViaRule_propName
  PROTO_PARAMS(( const lefiViaRule * this,
                 int index ));

EXTERN const char *
lefiViaRule_propValue
  PROTO_PARAMS(( const lefiViaRule * this,
                 int index ));

EXTERN double
lefiViaRule_propNumber
  PROTO_PARAMS(( const lefiViaRule * this,
                 int index ));

EXTERN const char
lefiViaRule_propType
  PROTO_PARAMS(( const lefiViaRule * this,
                 int index ));

EXTERN int
lefiViaRule_propIsNumber
  PROTO_PARAMS(( const lefiViaRule * this,
                 int index ));

EXTERN int
lefiViaRule_propIsString
  PROTO_PARAMS(( const lefiViaRule * this,
                 int index ));

EXTERN void
lefiViaRule_print
  PROTO_PARAMS(( const lefiViaRule * this,
                 FILE * f ));

#endif
