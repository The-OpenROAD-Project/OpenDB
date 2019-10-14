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

#ifndef lefiMisc_h
#define lefiMisc_h
#include <stdio.h>
#include "lefiKRDefs.h"

/*
 * The different types of items in a geometry list.
 */
struct lefiGeomRect {
  double          xl;
  double          yl;
  double          xh;
  double          yh;
};

struct lefiGeomRectIter {
  double          xl;
  double          yl;
  double          xh;
  double          yh;
  double          xStart;
  double          yStart;
  double          xStep;
  double          yStep;
};

struct lefiGeomPath {
  int             numPoints;
  double         *x;
  double         *y;
};

struct lefiGeomPathIter {
  int             numPoints;
  double         *x;
  double         *y;
  double          xStart;
  double          yStart;
  double          xStep;
  double          yStep;
};

struct lefiGeomPolygon {
  int             numPoints;
  double         *x;
  double         *y;
};

struct lefiGeomPolygonIter {
  int             numPoints;
  double         *x;
  double         *y;
  double          xStart;
  double          yStart;
  double          xStep;
  double          yStep;
};

enum lefiGeomEnum {
  lefiGeomUnknown = 0,
  lefiGeomLayerE,
  lefiGeomLayerMinSpacingE,
  lefiGeomLayerRuleWidthE,
  lefiGeomWidthE,
  lefiGeomPathE,
  lefiGeomPathIterE,
  lefiGeomRectE,
  lefiGeomRectIterE,
  lefiGeomPolygonE,
  lefiGeomPolygonIterE,
  lefiGeomViaE,
  lefiGeomViaIterE,
  lefiGeomClassE,
  lefiGeomEnd

};

struct lefiGeomVia {
  char           *name;
  double          x;
  double          y;
};

struct lefiGeomViaIter {
  char           *name;
  double          x;
  double          y;
  double          xStart;
  double          yStart;
  double          xStep;
  double          yStep;
};

typedef struct lefiGeometries_s {
  int             numItems_;
  int             itemsAllocated_;
  enum lefiGeomEnum *itemType_;
  void          **items_;
  int             numPoints_;
  int             pointsAllocated_;
  double         *x_;
  double         *y_;
  double          xStart_;
  double          yStart_;
  double          xStep_;
  double          yStep_;
} lefiGeometries;

EXTERN lefiGeometries *
lefiGeometries_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiGeometries_Init
  PROTO_PARAMS(( lefiGeometries * this ));

EXTERN void
lefiGeometries_Destroy
  PROTO_PARAMS(( lefiGeometries * this ));

EXTERN void
lefiGeometries_Delete
  PROTO_PARAMS(( lefiGeometries * this ));

EXTERN void
lefiGeometries_clear
  PROTO_PARAMS(( lefiGeometries * this ));

EXTERN void
lefiGeometries_clearPolyItems
  PROTO_PARAMS(( lefiGeometries * this ));

EXTERN void
lefiGeometries_add
  PROTO_PARAMS(( lefiGeometries * this,
                 void *v,
                 enum lefiGeomEnum e ));

EXTERN void
lefiGeometries_addLayer
  PROTO_PARAMS(( lefiGeometries * this,
                 const char *name ));

EXTERN void
lefiGeometries_addLayerMinSpacing
  PROTO_PARAMS(( lefiGeometries * this,
                 double spacing ));

EXTERN void
lefiGeometries_addLayerRuleWidth
  PROTO_PARAMS(( lefiGeometries * this,
                 double width ));

EXTERN void
lefiGeometries_addClass
  PROTO_PARAMS(( lefiGeometries * this,
                 const char *name ));

EXTERN void
lefiGeometries_addWidth
  PROTO_PARAMS(( lefiGeometries * this,
                 double w ));

EXTERN void
lefiGeometries_addPath
  PROTO_PARAMS(( lefiGeometries * this ));

EXTERN void
lefiGeometries_addPathIter
  PROTO_PARAMS(( lefiGeometries * this ));


/*
 * pcr 481783 & 560504
 */
EXTERN void
lefiGeometries_addRect
  PROTO_PARAMS(( lefiGeometries * this,
                 double xl,
                 double yl,
                 double xh,
                 double yh ));

EXTERN void
lefiGeometries_addRectIter
  PROTO_PARAMS(( lefiGeometries * this,
                 double xl,
                 double yl,
                 double xh,
                 double yh ));


/*
 * void addRect(char* tmpStr1, char* tmpStr2, char* tmpStr3, char* tmpStr4);
 * void addRectIter(char* tmpStr1, char* tmpStr2, char* tmpStr3, char*
 * tmpStr4);
 */
EXTERN void
lefiGeometries_addPolygon
  PROTO_PARAMS(( lefiGeometries * this ));

EXTERN void
lefiGeometries_addPolygonIter
  PROTO_PARAMS(( lefiGeometries * this ));

EXTERN void
lefiGeometries_addVia
  PROTO_PARAMS(( lefiGeometries * this,
                 double x,
                 double y,
                 const char *name ));

EXTERN void
lefiGeometries_addViaIter
  PROTO_PARAMS(( lefiGeometries * this,
                 double x,
                 double y,
                 const char *name ));

EXTERN void
lefiGeometries_addStepPattern
  PROTO_PARAMS(( lefiGeometries * this,
                 double xStart,
                 double yStart,
                 double xStep,
                 double yStep ));

EXTERN void
lefiGeometries_startList
  PROTO_PARAMS(( lefiGeometries * this,
                 double x,
                 double y ));

EXTERN void
lefiGeometries_addToList
  PROTO_PARAMS(( lefiGeometries * this,
                 double x,
                 double y ));

EXTERN int
lefiGeometries_numItems
  PROTO_PARAMS(( const lefiGeometries * this ));

EXTERN enum lefiGeomEnum
lefiGeometries_itemType
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN struct lefiGeomRect *
lefiGeometries_getRect
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN struct lefiGeomRectIter *
lefiGeometries_getRectIter
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN struct lefiGeomPath *
lefiGeometries_getPath
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN struct lefiGeomPathIter *
lefiGeometries_getPathIter
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN char *
lefiGeometries_getLayer
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN double
lefiGeometries_getLayerMinSpacing
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN double
lefiGeometries_getLayerRuleWidth
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN double
lefiGeometries_getWidth
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN struct lefiGeomPolygon *
lefiGeometries_getPolygon
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN struct lefiGeomPolygonIter *
lefiGeometries_getPolygonIter
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN char *
lefiGeometries_getClass
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN struct lefiGeomVia *
lefiGeometries_getVia
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN struct lefiGeomViaIter *
lefiGeometries_getViaIter
  PROTO_PARAMS(( const lefiGeometries * this,
                 int index ));

EXTERN void
lefiGeometries_print
  PROTO_PARAMS(( const lefiGeometries * this,
                 FILE * f ));

/*
 * Debug print
 */

typedef struct lefiSpacing_s {
  int             name1Size_;
  int             name2Size_;
  char           *name1_;
  char           *name2_;
  double          distance_;
  int             hasStack_;
} lefiSpacing;

EXTERN lefiSpacing *
lefiSpacing_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiSpacing_Init
  PROTO_PARAMS(( lefiSpacing * this ));

EXTERN void
lefiSpacing_Destroy
  PROTO_PARAMS(( lefiSpacing * this ));

EXTERN void
lefiSpacing_Delete
  PROTO_PARAMS(( lefiSpacing * this ));

EXTERN lefiSpacing *
lefiSpacing_clone
  PROTO_PARAMS(( lefiSpacing * this ));

EXTERN void
lefiSpacing_set
  PROTO_PARAMS(( lefiSpacing * this,
                 const char *name1,
                 const char *name2,
                 double num,
                 int hasStack ));

EXTERN int
lefiSpacing_hasStack
  PROTO_PARAMS(( const lefiSpacing * this ));

EXTERN const char *
lefiSpacing_name1
  PROTO_PARAMS(( const lefiSpacing * this ));

EXTERN const char *
lefiSpacing_name2
  PROTO_PARAMS(( const lefiSpacing * this ));

EXTERN double
lefiSpacing_distance
  PROTO_PARAMS(( const lefiSpacing * this ));

EXTERN void
lefiSpacing_print
  PROTO_PARAMS(( const lefiSpacing * this,
                 FILE * f ));

/*
 * Debug print
 */

typedef struct lefiIRDrop_s {
  int             nameSize_;
  int             value1Size_;
  int             value2Size_;
  int             numValues_;
  int             valuesAllocated_;
  char           *name_;
  double         *value1_;
  double         *value2_;
} lefiIRDrop;

EXTERN lefiIRDrop *
lefiIRDrop_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiIRDrop_Init
  PROTO_PARAMS(( lefiIRDrop * this ));

EXTERN void
lefiIRDrop_Destroy
  PROTO_PARAMS(( lefiIRDrop * this ));

EXTERN void
lefiIRDrop_Delete
  PROTO_PARAMS(( lefiIRDrop * this ));

EXTERN void
lefiIRDrop_clear
  PROTO_PARAMS(( lefiIRDrop * this ));

EXTERN void
lefiIRDrop_setTableName
  PROTO_PARAMS(( lefiIRDrop * this,
                 const char *name ));

EXTERN void
lefiIRDrop_setValues
  PROTO_PARAMS(( lefiIRDrop * this,
                 double name1,
                 double name2 ));

EXTERN const char *
lefiIRDrop_name
  PROTO_PARAMS(( const lefiIRDrop * this ));

EXTERN double
lefiIRDrop_value1
  PROTO_PARAMS(( const lefiIRDrop * this,
                 int index ));

EXTERN double
lefiIRDrop_value2
  PROTO_PARAMS(( const lefiIRDrop * this,
                 int index ));

EXTERN int
lefiIRDrop_numValues
  PROTO_PARAMS(( const lefiIRDrop * this ));

EXTERN void
lefiIRDrop_print
  PROTO_PARAMS(( const lefiIRDrop * this,
                 FILE * f ));

/*
 * Debug print
 */

typedef struct lefiMinFeature_s {
  double          one_;
  double          two_;
} lefiMinFeature;

EXTERN lefiMinFeature *
lefiMinFeature_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiMinFeature_Init
  PROTO_PARAMS(( lefiMinFeature * this ));

EXTERN void
lefiMinFeature_Destroy
  PROTO_PARAMS(( lefiMinFeature * this ));

EXTERN void
lefiMinFeature_Delete
  PROTO_PARAMS(( lefiMinFeature * this ));

EXTERN void
lefiMinFeature_set
  PROTO_PARAMS(( lefiMinFeature * this,
                 double one,
                 double two ));

EXTERN double
lefiMinFeature_one
  PROTO_PARAMS(( const lefiMinFeature * this ));

EXTERN double
lefiMinFeature_two
  PROTO_PARAMS(( const lefiMinFeature * this ));

EXTERN void
lefiMinFeature_print
  PROTO_PARAMS(( const lefiMinFeature * this,
                 FILE * f ));

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
 * bit 0-x   bit 1-y   bit 2-90
 */

/*
 * 5.6 ROWPATTERN
 */

typedef struct lefiSite_s {
  int             nameSize_;
  char           *name_;
  int             hasClass_;
  char            siteClass_[8];
  double          sizeX_;
  double          sizeY_;
  int             hasSize_;
  int             symmetry_;
  int             numRowPattern_;
  int             rowPatternAllocated_;
  char          **siteNames_;
  int            *siteOrients_;
} lefiSite;

EXTERN lefiSite *
lefiSite_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiSite_Init
  PROTO_PARAMS(( lefiSite * this ));

EXTERN void
lefiSite_Destroy
  PROTO_PARAMS(( lefiSite * this ));

EXTERN void
lefiSite_Delete
  PROTO_PARAMS(( lefiSite * this ));

EXTERN void
lefiSite_setName
  PROTO_PARAMS(( lefiSite * this,
                 const char *name ));

EXTERN void
lefiSite_setClass
  PROTO_PARAMS(( lefiSite * this,
                 const char *cls ));

EXTERN void
lefiSite_setSize
  PROTO_PARAMS(( lefiSite * this,
                 double x,
                 double y ));

EXTERN void
lefiSite_setXSymmetry
  PROTO_PARAMS(( lefiSite * this ));

EXTERN void
lefiSite_setYSymmetry
  PROTO_PARAMS(( lefiSite * this ));

EXTERN void
lefiSite_set90Symmetry
  PROTO_PARAMS(( lefiSite * this ));

EXTERN void
lefiSite_addRowPattern
  PROTO_PARAMS(( lefiSite * this,
                 const char *name,
                 int orient ));

EXTERN const char *
lefiSite_name
  PROTO_PARAMS(( const lefiSite * this ));

EXTERN int
lefiSite_hasClass
  PROTO_PARAMS(( const lefiSite * this ));

EXTERN const char *
lefiSite_siteClass
  PROTO_PARAMS(( const lefiSite * this ));

EXTERN double
lefiSite_sizeX
  PROTO_PARAMS(( const lefiSite * this ));

EXTERN double
lefiSite_sizeY
  PROTO_PARAMS(( const lefiSite * this ));

EXTERN int
lefiSite_hasSize
  PROTO_PARAMS(( const lefiSite * this ));

EXTERN int
lefiSite_hasXSymmetry
  PROTO_PARAMS(( const lefiSite * this ));

EXTERN int
lefiSite_hasYSymmetry
  PROTO_PARAMS(( const lefiSite * this ));

EXTERN int
lefiSite_has90Symmetry
  PROTO_PARAMS(( const lefiSite * this ));

EXTERN int
lefiSite_hasRowPattern
  PROTO_PARAMS(( const lefiSite * this ));

EXTERN int
lefiSite_numSites
  PROTO_PARAMS(( const lefiSite * this ));

EXTERN char *
lefiSite_siteName
  PROTO_PARAMS(( const lefiSite * this,
                 int index ));

EXTERN int
lefiSite_siteOrient
  PROTO_PARAMS(( const lefiSite * this,
                 int index ));

EXTERN char *
lefiSite_siteOrientStr
  PROTO_PARAMS(( const lefiSite * this,
                 int index ));

EXTERN void
lefiSite_print
  PROTO_PARAMS(( const lefiSite * this,
                 FILE * f ));

/*
 * 5.6
 */

/*
 * Debug print
 */

typedef struct lefiSitePattern_s {
  int             nameSize_;
  char           *name_;
  int             orient_;
  double          x_;
  double          y_;
  double          xStart_;
  double          yStart_;
  double          xStep_;
  double          yStep_;
} lefiSitePattern;

EXTERN lefiSitePattern *
lefiSitePattern_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiSitePattern_Init
  PROTO_PARAMS(( lefiSitePattern * this ));

EXTERN void
lefiSitePattern_Destroy
  PROTO_PARAMS(( lefiSitePattern * this ));

EXTERN void
lefiSitePattern_Delete
  PROTO_PARAMS(( lefiSitePattern * this ));

EXTERN void
lefiSitePattern_set
  PROTO_PARAMS(( lefiSitePattern * this,
                 const char *name,
                 double x,
                 double y,
                 int orient,
                 double xStart,
                 double yStart,
                 double xStep,
                 double yStep ));

EXTERN const char *
lefiSitePattern_name
  PROTO_PARAMS(( const lefiSitePattern * this ));

EXTERN int
lefiSitePattern_orient
  PROTO_PARAMS(( const lefiSitePattern * this ));

EXTERN const char *
lefiSitePattern_orientStr
  PROTO_PARAMS(( const lefiSitePattern * this ));

EXTERN double
lefiSitePattern_x
  PROTO_PARAMS(( const lefiSitePattern * this ));

EXTERN double
lefiSitePattern_y
  PROTO_PARAMS(( const lefiSitePattern * this ));

EXTERN int
lefiSitePattern_hasStepPattern
  PROTO_PARAMS(( const lefiSitePattern * this ));

EXTERN double
lefiSitePattern_xStart
  PROTO_PARAMS(( const lefiSitePattern * this ));

EXTERN double
lefiSitePattern_yStart
  PROTO_PARAMS(( const lefiSitePattern * this ));

EXTERN double
lefiSitePattern_xStep
  PROTO_PARAMS(( const lefiSitePattern * this ));

EXTERN double
lefiSitePattern_yStep
  PROTO_PARAMS(( const lefiSitePattern * this ));

EXTERN void
lefiSitePattern_print
  PROTO_PARAMS(( const lefiSitePattern * this,
                 FILE * f ));

/*
 * Debug print
 */

typedef struct lefiTrackPattern_s {
  int             nameSize_;
  char           *name_;
  double          start_;
  int             numTracks_;
  double          space_;
  int             numLayers_;
  int             layerAllocated_;
  char          **layerNames_;
} lefiTrackPattern;

EXTERN lefiTrackPattern *
lefiTrackPattern_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiTrackPattern_Init
  PROTO_PARAMS(( lefiTrackPattern * this ));

EXTERN void
lefiTrackPattern_Destroy
  PROTO_PARAMS(( lefiTrackPattern * this ));

EXTERN void
lefiTrackPattern_Delete
  PROTO_PARAMS(( lefiTrackPattern * this ));

EXTERN void
lefiTrackPattern_clear
  PROTO_PARAMS(( lefiTrackPattern * this ));

EXTERN void
lefiTrackPattern_set
  PROTO_PARAMS(( lefiTrackPattern * this,
                 const char *name,
                 double start,
                 int numTracks,
                 double space ));

EXTERN void
lefiTrackPattern_addLayer
  PROTO_PARAMS(( lefiTrackPattern * this,
                 const char *name ));

EXTERN const char *
lefiTrackPattern_name
  PROTO_PARAMS(( const lefiTrackPattern * this ));

EXTERN double
lefiTrackPattern_start
  PROTO_PARAMS(( const lefiTrackPattern * this ));

EXTERN int
lefiTrackPattern_numTracks
  PROTO_PARAMS(( const lefiTrackPattern * this ));

EXTERN double
lefiTrackPattern_space
  PROTO_PARAMS(( const lefiTrackPattern * this ));

EXTERN int
lefiTrackPattern_numLayers
  PROTO_PARAMS(( const lefiTrackPattern * this ));

EXTERN const char *
lefiTrackPattern_layerName
  PROTO_PARAMS(( const lefiTrackPattern * this,
                 int index ));

EXTERN void
lefiTrackPattern_print
  PROTO_PARAMS(( const lefiTrackPattern * this,
                 FILE * f ));

/*
 * Debug print
 */

typedef struct lefiGcellPattern_s {
  int             nameSize_;
  char           *name_;
  double          start_;
  int             numCRs_;
  double          space_;
} lefiGcellPattern;

EXTERN lefiGcellPattern *
lefiGcellPattern_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiGcellPattern_Init
  PROTO_PARAMS(( lefiGcellPattern * this ));

EXTERN void
lefiGcellPattern_Destroy
  PROTO_PARAMS(( lefiGcellPattern * this ));

EXTERN void
lefiGcellPattern_Delete
  PROTO_PARAMS(( lefiGcellPattern * this ));

EXTERN void
lefiGcellPattern_set
  PROTO_PARAMS(( lefiGcellPattern * this,
                 const char *name,
                 double start,
                 int numCRs,
                 double space ));

EXTERN const char *
lefiGcellPattern_name
  PROTO_PARAMS(( const lefiGcellPattern * this ));

EXTERN double
lefiGcellPattern_start
  PROTO_PARAMS(( const lefiGcellPattern * this ));

EXTERN int
lefiGcellPattern_numCRs
  PROTO_PARAMS(( const lefiGcellPattern * this ));

EXTERN double
lefiGcellPattern_space
  PROTO_PARAMS(( const lefiGcellPattern * this ));

EXTERN void
lefiGcellPattern_print
  PROTO_PARAMS(( const lefiGcellPattern * this,
                 FILE * f ));


typedef struct lefiAlias_itr_s {
  int             num_;
} lefiAlias_itr;

EXTERN lefiAlias_itr *
lefiAlias_itr_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiAlias_itr_Init
  PROTO_PARAMS(( lefiAlias_itr * this ));

EXTERN void
lefiAlias_itr_Destroy
  PROTO_PARAMS(( lefiAlias_itr * this ));

EXTERN void
lefiAlias_itr_Delete
  PROTO_PARAMS(( lefiAlias_itr * this ));

EXTERN int
lefiAlias_itr_Next
  PROTO_PARAMS(( lefiAlias_itr * this ));

EXTERN const char *
lefiAlias_itr_Key
  PROTO_PARAMS(( lefiAlias_itr * this ));

EXTERN const char *
lefiAlias_itr_Data
  PROTO_PARAMS(( lefiAlias_itr * this ));

/*
 * Debug print
 */

typedef struct lefiUseMinSpacing_s {
  char           *name_;
  int             value_;
} lefiUseMinSpacing;

EXTERN lefiUseMinSpacing *
lefiUseMinSpacing_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiUseMinSpacing_Init
  PROTO_PARAMS(( lefiUseMinSpacing * this ));

EXTERN void
lefiUseMinSpacing_Destroy
  PROTO_PARAMS(( lefiUseMinSpacing * this ));

EXTERN void
lefiUseMinSpacing_Delete
  PROTO_PARAMS(( lefiUseMinSpacing * this ));

EXTERN void
lefiUseMinSpacing_set
  PROTO_PARAMS(( lefiUseMinSpacing * this,
                 const char *name,
                 int value ));

EXTERN const char *
lefiUseMinSpacing_name
  PROTO_PARAMS(( const lefiUseMinSpacing * this ));

EXTERN int
lefiUseMinSpacing_value
  PROTO_PARAMS(( const lefiUseMinSpacing * this ));

EXTERN void
lefiUseMinSpacing_print
  PROTO_PARAMS(( const lefiUseMinSpacing * this,
                 FILE * f ));

/*
 * 5.5 for Maximum Stacked-via rule
 */

/*
 * Debug print
 */

typedef struct lefiMaxStackVia_s {
  int             value_;
  int             hasRange_;
  char           *bottomLayer_;
  char           *topLayer_;
} lefiMaxStackVia;

EXTERN lefiMaxStackVia *
lefiMaxStackVia_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiMaxStackVia_Init
  PROTO_PARAMS(( lefiMaxStackVia * this ));

EXTERN void
lefiMaxStackVia_Destroy
  PROTO_PARAMS(( lefiMaxStackVia * this ));

EXTERN void
lefiMaxStackVia_Delete
  PROTO_PARAMS(( lefiMaxStackVia * this ));

EXTERN void
lefiMaxStackVia_clear
  PROTO_PARAMS(( lefiMaxStackVia * this ));

EXTERN void
lefiMaxStackVia_setMaxStackVia
  PROTO_PARAMS(( lefiMaxStackVia * this,
                 int value ));

EXTERN void
lefiMaxStackVia_setMaxStackViaRange
  PROTO_PARAMS(( lefiMaxStackVia * this,
                 const char *bottomLayer,
                 const char *topLayer ));

EXTERN int
lefiMaxStackVia_maxStackVia
  PROTO_PARAMS(( const lefiMaxStackVia * this ));

EXTERN int
lefiMaxStackVia_hasMaxStackViaRange
  PROTO_PARAMS(( const lefiMaxStackVia * this ));

EXTERN const char *
lefiMaxStackVia_maxStackViaBottomLayer
  PROTO_PARAMS(( const lefiMaxStackVia * this ));

EXTERN const char *
lefiMaxStackVia_maxStackViaTopLayer
  PROTO_PARAMS(( const lefiMaxStackVia * this ));

EXTERN void
lefiMaxStackVia_print
  PROTO_PARAMS(( const lefiMaxStackVia * this,
                 FILE * f ));

#endif
