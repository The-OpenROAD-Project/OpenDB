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

#ifndef lefiVia_h
#define lefiVia_h
#include <stdio.h>
#include "lefiKRDefs.h"
#include "lefiMisc.h"

/*
 * 5.6
 */

/*
 * 5.6
 */

typedef struct lefiViaLayer_s {
  char           *name_;
  int             numRects_;
  int             rectsAllocated_;
  double         *xl_;
  double         *yl_;
  double         *xh_;
  double         *yh_;
  int             numPolys_;
  int             polysAllocated_;
  struct lefiGeomPolygon **polygons_;
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

EXTERN void
lefiViaLayer_addPoly
  PROTO_PARAMS(( lefiViaLayer * this,
                 lefiGeometries * geom ));

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

EXTERN int
lefiViaLayer_numPolygons
  PROTO_PARAMS(( lefiViaLayer * this ));

EXTERN struct lefiGeomPolygon *
lefiViaLayer_getPolygon
  PROTO_PARAMS(( const lefiViaLayer * this,
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
 * 5.6, this no longer in 5.6, should be removed
 */

/*
 * 5.6
 */

/*
 * 5.6
 */

/*
 * Given an index from 0 to numProperties()-1 return
 */

/*
 * information about that property.
 */

/*
 * 5.6 VIARULE inside a VIA
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

typedef struct lefiVia_s {
  char           *name_;
  int             nameSize_;
  int             hasDefault_;
  int             hasGenerated_;
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
  char           *viaRuleName_;
  double          xSize_;
  double          ySize_;
  char           *botLayer_;
  char           *cutLayer_;
  char           *topLayer_;
  double          xSpacing_;
  double          ySpacing_;
  double          xBotEnc_;
  double          yBotEnc_;
  double          xTopEnc_;
  double          yTopEnc_;
  int             numRows_;
  int             numCols_;
  double          xOffset_;
  double          yOffset_;
  double          xBotOs_;
  double          yBotOs_;
  double          xTopOs_;
  double          yTopOs_;
  char           *cutPattern_;
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
                 int viaType ));

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
lefiVia_addPolyToLayer
  PROTO_PARAMS(( lefiVia * this,
                 lefiGeometries * geom ));

EXTERN void
lefiVia_bumpProps
  PROTO_PARAMS(( lefiVia * this ));

EXTERN void
lefiVia_setViaRule
  PROTO_PARAMS(( lefiVia * this,
                 const char *viaRuleName,
                 double xSize,
                 double ySize,
                 const char *botLayer,
                 const char *cutLayer,
                 const char *topLayer,
                 double xCut,
                 double yCut,
                 double xBotEnc,
                 double yBotEnc,
                 double xTopEnc,
                 double yTopEnc ));

EXTERN void
lefiVia_setRowCol
  PROTO_PARAMS(( lefiVia * this,
                 int numRows,
                 int numCols ));

EXTERN void
lefiVia_setOrigin
  PROTO_PARAMS(( lefiVia * this,
                 double xOffset,
                 double yOffset ));

EXTERN void
lefiVia_setOffset
  PROTO_PARAMS(( lefiVia * this,
                 double xBot,
                 double yBot,
                 double xTop,
                 double yTop ));

EXTERN void
lefiVia_setPattern
  PROTO_PARAMS(( lefiVia * this,
                 const char *cutPattern ));


/* make a new one   */
EXTERN lefiVia *
lefiVia_clone
  PROTO_PARAMS(( lefiVia * this ));

EXTERN int
lefiVia_hasDefault
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_hasGenerated
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

EXTERN int
lefiVia_numPolygons
  PROTO_PARAMS(( const lefiVia * this,
                 int layerNum ));

EXTERN struct lefiGeomPolygon
lefiVia_getPolygon
  PROTO_PARAMS(( const lefiVia * this,
                 int layerNum,
                 int polyNum ));

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

EXTERN int
lefiVia_hasViaRule
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN const char *
lefiVia_viaRuleName
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_xCutSize
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_yCutSize
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN const char *
lefiVia_botMetalLayer
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN const char *
lefiVia_cutLayer
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN const char *
lefiVia_topMetalLayer
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_xCutSpacing
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_yCutSpacing
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_xBotEnc
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_yBotEnc
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_xTopEnc
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_yTopEnc
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_hasRowCol
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_numCutRows
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_numCutCols
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_hasOrigin
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_xOffset
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_yOffset
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_hasOffset
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_xBotOffset
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_yBotOffset
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_xTopOffset
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN double
lefiVia_yTopOffset
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN int
lefiVia_hasCutPattern
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN const char *
lefiVia_cutPattern
  PROTO_PARAMS(( const lefiVia * this ));

EXTERN void
lefiVia_print
  PROTO_PARAMS(( const lefiVia * this,
                 FILE * f ));

#endif
