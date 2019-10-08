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

#ifndef lefiVia_h
#define lefiVia_h
#include <stdio.h>
#include "lefiKRDefs.h"

typedef struct lefiViaLayer_s {
  char           *name_;
  int             numRects_;
  int             rectsAllocated_;
  double         *xl_;
  double         *yl_;
  double         *xh_;
  double         *yh_;
} lefiViaLayer;

EXTERN lefiViaLayer *
lefiViaLayer_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiViaLayer_Init
  PROTO_PARAMS(( lefiViaLayer * this ));

EXTERN void
lefiViaLayer_Destroy
  PROTO_PARAMS(( lefiViaLayer * this ));

EXTERN void
lefiViaLayer_Delete
  PROTO_PARAMS(( lefiViaLayer * this ));

EXTERN void
lefiViaLayer_setName
  PROTO_PARAMS(( lefiViaLayer * this,
                 const char *name ));

EXTERN void
lefiViaLayer_addRect
  PROTO_PARAMS(( lefiViaLayer * this,
                 double xl,
                 double yl,
                 double xh,
                 double yh ));

EXTERN lefiViaLayer *
lefiViaLayer_clone
  PROTO_PARAMS(( lefiViaLayer * this ));

EXTERN int
lefiViaLayer_numRects
  PROTO_PARAMS(( lefiViaLayer * this ));

EXTERN char *
lefiViaLayer_name
  PROTO_PARAMS(( lefiViaLayer * this ));

EXTERN double
lefiViaLayer_xl
  PROTO_PARAMS(( lefiViaLayer * this,
                 int index ));

EXTERN double
lefiViaLayer_yl
  PROTO_PARAMS(( lefiViaLayer * this,
                 int index ));

EXTERN double
lefiViaLayer_xh
  PROTO_PARAMS(( lefiViaLayer * this,
                 int index ));

EXTERN double
lefiViaLayer_yh
  PROTO_PARAMS(( lefiViaLayer * this,
                 int index ));

/*
 * setName calls clear to init
 */

/*
 * deflt=0 no default specified
 */

/*
 * deflt=1 default specified in lef file
 */

/*
 * orient=-1 means no orient was specified.
 */

/*
 * Given an index from 0 to numProperties()-1 return
 */

/*
 * information about that property.
 */

/*
 * Debug print
 */

/*
 * The prop value is stored in the propValue_ or the propDValue_.
 */

/*
 * If it is a string it is in propValue_.  If it is a number,
 */

/*
 * then propValue_ is NULL and it is stored in propDValue_;
 */

typedef struct lefiVia_s {
  char           *name_;
  int             nameSize_;
  int             hasDefault_;
  int             hasResistance_;
  int             hasForeignPnt_;
  int             hasTopOfStack_;
  int             numProps_;
  int             propsAllocated_;
  char          **propName_;
  char          **propValue_;
  double         *propDValue_;
  char           *propType_;
  int             numLayers_;
  int             layersAllocated_;
  lefiViaLayer  **layers_;
  double          resistance_;
  char           *foreign_;
  double          foreignX_;
  double          foreignY_;
  int             foreignOrient_;
} lefiVia;

EXTERN lefiVia *
lefiVia_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiVia_Init
  PROTO_PARAMS(( lefiVia * this ));

EXTERN void
lefiVia_Destroy
  PROTO_PARAMS(( lefiVia * this ));

EXTERN void
lefiVia_Delete
  PROTO_PARAMS(( lefiVia * this ));

EXTERN void
lefiVia_clear
  PROTO_PARAMS(( lefiVia * this ));

EXTERN void
lefiVia_setName
  PROTO_PARAMS(( lefiVia * this,
                 const char *name,
                 int deflt ));

EXTERN void
lefiVia_setResistance
  PROTO_PARAMS(( lefiVia * this,
                 double num ));

EXTERN void
lefiVia_addProp
  PROTO_PARAMS(( lefiVia * this,
                 const char *name,
                 const char *value,
                 const char type ));

EXTERN void
lefiVia_addNumProp
  PROTO_PARAMS(( lefiVia * this,
                 const char *name,
                 double d,
                 const char *value,
                 const char type ));

EXTERN void
lefiVia_setForeign
  PROTO_PARAMS(( lefiVia * this,
                 const char *name,
                 int hasPnt,
                 double x,
                 double y,
                 int orient ));

EXTERN void
lefiVia_setTopOfStack
  PROTO_PARAMS(( lefiVia * this ));

EXTERN void
lefiVia_addLayer
  PROTO_PARAMS(( lefiVia * this,
                 const char *name ));

EXTERN void
lefiVia_addRectToLayer
  PROTO_PARAMS(( lefiVia * this,
                 double xl,
                 double yl,
                 double xh,
                 double yh ));

EXTERN void
lefiVia_bumpProps
  PROTO_PARAMS(( lefiVia * this ));


/* make a new one   */
EXTERN lefiVia *
lefiVia_clone
  PROTO_PARAMS(( lefiVia * this ));

EXTERN int
lefiVia_hasDefault
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_hasForeign
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_hasForeignPnt
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_hasForeignOrient
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_hasProperties
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_hasResistance
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_hasTopOfStack
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_numLayers
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN char *
lefiVia_layerName
  PROTO_PARAMS(( const lefiVia * this,
                 int layerNum ));

EXTERN int
lefiVia_numRects
  PROTO_PARAMS(( const lefiVia * this,
                 int layerNum ));

EXTERN double
lefiVia_xl
  PROTO_PARAMS(( const lefiVia * this,
                 int layerNum,
                 int rectNum ));

EXTERN double
lefiVia_yl
  PROTO_PARAMS(( const lefiVia * this,
                 int layerNum,
                 int rectNum ));

EXTERN double
lefiVia_xh
  PROTO_PARAMS(( const lefiVia * this,
                 int layerNum,
                 int rectNum ));

EXTERN double
lefiVia_yh
  PROTO_PARAMS(( const lefiVia * this,
                 int layerNum,
                 int rectNum ));

EXTERN char *
lefiVia_name
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_resistance
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_numProperties
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN char *
lefiVia_propName
  PROTO_PARAMS(( const lefiVia * this,
                 int index ));

EXTERN char *
lefiVia_propValue
  PROTO_PARAMS(( const lefiVia * this,
                 int index ));

EXTERN double
lefiVia_propNumber
  PROTO_PARAMS(( const lefiVia * this,
                 int index ));

EXTERN char
lefiVia_propType
  PROTO_PARAMS(( const lefiVia * this,
                 int index ));

EXTERN int
lefiVia_propIsNumber
  PROTO_PARAMS(( const lefiVia * this,
                 int index ));

EXTERN int
lefiVia_propIsString
  PROTO_PARAMS(( const lefiVia * this,
                 int index ));

EXTERN char *
lefiVia_foreign
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_foreignX
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_foreignY
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_foreignOrient
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN char *
lefiVia_foreignOrientStr
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN void
lefiVia_print
  PROTO_PARAMS(( const lefiVia * this,
                 FILE * f ));

#endif
