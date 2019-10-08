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

#ifndef lefiLayer_h
#define lefiLayer_h
#include <stdio.h>
#include "lefiKRDefs.h"
typedef enum lefiAntennaEnum {
  lefiAntennaAR,
  lefiAntennaDAR,
  lefiAntennaCAR,
  lefiAntennaCDAR,
  lefiAntennaAF,
  lefiAntennaSAR,
  lefiAntennaDSAR,
  lefiAntennaCSAR,
  lefiAntennaCDSAR,
  lefiAntennaSAF,
  lefiAntennaO

} lefiAntennaEnum;

typedef struct lefiAntennaPWL_s {
  int             numAlloc_;
  int             numPWL_;
  double         *d_;
  double         *r_;
} lefiAntennaPWL;

EXTERN lefiAntennaPWL *
lefiAntennaPWL_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiAntennaPWL_Delete
  PROTO_PARAMS(( lefiAntennaPWL * this ));

EXTERN void
lefiAntennaPWL_Init
  PROTO_PARAMS(( lefiAntennaPWL * this ));

EXTERN void
lefiAntennaPWL_clear
  PROTO_PARAMS(( lefiAntennaPWL * this ));

EXTERN void
lefiAntennaPWL_Destroy
  PROTO_PARAMS(( lefiAntennaPWL * this ));

EXTERN void
lefiAntennaPWL_addAntennaPWL
  PROTO_PARAMS(( lefiAntennaPWL * this,
                 double d,
                 double r ));

EXTERN int
lefiAntennaPWL_numPWL
  PROTO_PARAMS(( const lefiAntennaPWL * this ));

EXTERN double
lefiAntennaPWL_PWLdiffusion
  PROTO_PARAMS(( lefiAntennaPWL * this,
                 int index ));

EXTERN double
lefiAntennaPWL_PWLratio
  PROTO_PARAMS(( lefiAntennaPWL * this,
                 int index ));


typedef struct lefiLayerDensity_s {
  char           *type_;
  double          oneEntry_;
  int             numFrequency_;
  double         *frequency_;
  int             numWidths_;
  double         *widths_;
  int             numTableEntries_;
  double         *tableEntries_;
  int             numCutareas_;
  double         *cutareas_;
} lefiLayerDensity;

EXTERN lefiLayerDensity *
lefiLayerDensity_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiLayerDensity_Delete
  PROTO_PARAMS(( lefiLayerDensity * this ));

EXTERN void
lefiLayerDensity_Init
  PROTO_PARAMS(( lefiLayerDensity * this,
                 const char *type ));

EXTERN void
lefiLayerDensity_Destroy
  PROTO_PARAMS(( lefiLayerDensity * this ));

EXTERN void
lefiLayerDensity_clear
  PROTO_PARAMS(( lefiLayerDensity * this ));

EXTERN void
lefiLayerDensity_setOneEntry
  PROTO_PARAMS(( lefiLayerDensity * this,
                 double entry ));

EXTERN void
lefiLayerDensity_addFrequency
  PROTO_PARAMS(( lefiLayerDensity * this,
                 int num,
                 double *frequency ));

EXTERN void
lefiLayerDensity_addWidth
  PROTO_PARAMS(( lefiLayerDensity * this,
                 int num,
                 double *width ));

EXTERN void
lefiLayerDensity_addTableEntry
  PROTO_PARAMS(( lefiLayerDensity * this,
                 int num,
                 double *entry ));

EXTERN void
lefiLayerDensity_addCutarea
  PROTO_PARAMS(( lefiLayerDensity * this,
                 int num,
                 double *cutarea ));

EXTERN char *
lefiLayerDensity_type
  PROTO_PARAMS(( lefiLayerDensity * this ));

EXTERN int
lefiLayerDensity_hasOneEntry
  PROTO_PARAMS(( lefiLayerDensity * this ));

EXTERN double
lefiLayerDensity_oneEntry
  PROTO_PARAMS(( lefiLayerDensity * this ));

EXTERN int
lefiLayerDensity_numFrequency
  PROTO_PARAMS(( lefiLayerDensity * this ));

EXTERN double
lefiLayerDensity_frequency
  PROTO_PARAMS(( lefiLayerDensity * this,
                 int index ));

EXTERN int
lefiLayerDensity_numWidths
  PROTO_PARAMS(( lefiLayerDensity * this ));

EXTERN double
lefiLayerDensity_width
  PROTO_PARAMS(( lefiLayerDensity * this,
                 int index ));

EXTERN int
lefiLayerDensity_numTableEntries
  PROTO_PARAMS(( lefiLayerDensity * this ));

EXTERN double
lefiLayerDensity_tableEntry
  PROTO_PARAMS(( lefiLayerDensity * this,
                 int index ));

EXTERN int
lefiLayerDensity_numCutareas
  PROTO_PARAMS(( lefiLayerDensity * this ));

EXTERN double
lefiLayerDensity_cutArea
  PROTO_PARAMS(( lefiLayerDensity * this,
                 int index ));

/*
 * 5.5
 */

typedef struct lefiParallel_s {
  int             numLength_;
  int             numWidth_;
  int             numWidthAllocated_;
  double         *length_;
  double         *width_;
  double         *widthSpacing_;
} lefiParallel;

EXTERN lefiParallel *
lefiParallel_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiParallel_Delete
  PROTO_PARAMS(( lefiParallel * this ));

EXTERN void
lefiParallel_Init
  PROTO_PARAMS(( lefiParallel * this ));

EXTERN void
lefiParallel_clear
  PROTO_PARAMS(( lefiParallel * this ));

EXTERN void
lefiParallel_Destroy
  PROTO_PARAMS(( lefiParallel * this ));

EXTERN void
lefiParallel_addParallelLength
  PROTO_PARAMS(( lefiParallel * this,
                 int numLength,
                 double *lengths ));

EXTERN void
lefiParallel_addParallelWidth
  PROTO_PARAMS(( lefiParallel * this,
                 double width ));

EXTERN void
lefiParallel_addParallelWidthSpacing
  PROTO_PARAMS(( lefiParallel * this,
                 int numSpacing,
                 double *spacings ));

EXTERN int
lefiParallel_numLength
  PROTO_PARAMS(( const lefiParallel * this ));

EXTERN int
lefiParallel_numWidth
  PROTO_PARAMS(( const lefiParallel * this ));

EXTERN double
lefiParallel_length
  PROTO_PARAMS(( const lefiParallel * this,
                 int iLength ));

EXTERN double
lefiParallel_width
  PROTO_PARAMS(( const lefiParallel * this,
                 int iWidth ));

EXTERN double
lefiParallel_widthSpacing
  PROTO_PARAMS(( const lefiParallel * this,
                 int iWidth,
                 int iWidthSpacing ));

/*
 * 5.5
 */

typedef struct lefiInfluence_s {
  int             numAllocated_;
  int             numWidth_;
  int             numDistance_;
  int             numSpacing_;
  double         *width_;
  double         *distance_;
  double         *spacing_;
} lefiInfluence;

EXTERN lefiInfluence *
lefiInfluence_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiInfluence_Delete
  PROTO_PARAMS(( lefiInfluence * this ));

EXTERN void
lefiInfluence_Init
  PROTO_PARAMS(( lefiInfluence * this ));

EXTERN void
lefiInfluence_clear
  PROTO_PARAMS(( lefiInfluence * this ));

EXTERN void
lefiInfluence_Destroy
  PROTO_PARAMS(( lefiInfluence * this ));

EXTERN void
lefiInfluence_addInfluence
  PROTO_PARAMS(( lefiInfluence * this,
                 double width,
                 double distance,
                 double spacing ));

EXTERN int
lefiInfluence_numInfluenceEntry
  PROTO_PARAMS(( const lefiInfluence * this ));

EXTERN double
lefiInfluence_width
  PROTO_PARAMS(( const lefiInfluence * this,
                 int index ));

EXTERN double
lefiInfluence_distance
  PROTO_PARAMS(( const lefiInfluence * this,
                 int index ));

EXTERN double
lefiInfluence_spacing
  PROTO_PARAMS(( const lefiInfluence * this,
                 int index ));

/*
 * 5.5
 */

typedef struct lefiSpacingTable_s {
  int             hasInfluence_;
  lefiInfluence  *influence_;
  lefiParallel   *parallel_;
} lefiSpacingTable;

EXTERN lefiSpacingTable *
lefiSpacingTable_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiSpacingTable_Delete
  PROTO_PARAMS(( lefiSpacingTable * this ));

EXTERN void
lefiSpacingTable_Init
  PROTO_PARAMS(( lefiSpacingTable * this ));

EXTERN void
lefiSpacingTable_clear
  PROTO_PARAMS(( lefiSpacingTable * this ));

EXTERN void
lefiSpacingTable_Destroy
  PROTO_PARAMS(( lefiSpacingTable * this ));

EXTERN void
lefiSpacingTable_addParallelLength
  PROTO_PARAMS(( lefiSpacingTable * this,
                 int numLength,
                 double *lengths ));

EXTERN void
lefiSpacingTable_addParallelWidth
  PROTO_PARAMS(( lefiSpacingTable * this,
                 double width ));

EXTERN void
lefiSpacingTable_addParallelWidthSpacing
  PROTO_PARAMS(( lefiSpacingTable * this,
                 int numSpacing,
                 double *spacing ));

EXTERN void
lefiSpacingTable_setInfluence
  PROTO_PARAMS(( lefiSpacingTable * this ));

EXTERN void
lefiSpacingTable_addInfluence
  PROTO_PARAMS(( lefiSpacingTable * this,
                 double width,
                 double distance,
                 double spacing ));

EXTERN int
lefiSpacingTable_isInfluence
  PROTO_PARAMS(( const lefiSpacingTable * this ));

EXTERN lefiInfluence *
lefiSpacingTable_influence
  PROTO_PARAMS(( const lefiSpacingTable * this ));

EXTERN lefiParallel *
lefiSpacingTable_parallel
  PROTO_PARAMS(( const lefiSpacingTable * this ));

/*
 * 5.5
 */

typedef struct lefiAntennaModel_s {
  int             hasAntennaAreaRatio_;
  int             hasAntennaDiffAreaRatio_;
  int             hasAntennaDiffAreaRatioPWL_;
  int             hasAntennaCumAreaRatio_;
  int             hasAntennaCumDiffAreaRatio_;
  int             hasAntennaCumDiffAreaRatioPWL_;
  int             hasAntennaAreaFactor_;
  int             hasAntennaAreaFactorDUO_;
  int             hasAntennaSideAreaRatio_;
  int             hasAntennaDiffSideAreaRatio_;
  int             hasAntennaDiffSideAreaRatioPWL_;
  int             hasAntennaCumSideAreaRatio_;
  int             hasAntennaCumDiffSideAreaRatio_;
  int             hasAntennaCumDiffSideAreaRatioPWL_;
  int             hasAntennaSideAreaFactor_;
  int             hasAntennaSideAreaFactorDUO_;
  char           *oxide_;
  double          antennaAreaRatio_;
  double          antennaDiffAreaRatio_;
  lefiAntennaPWL *antennaDiffAreaRatioPWL_;
  double          antennaCumAreaRatio_;
  double          antennaCumDiffAreaRatio_;
  lefiAntennaPWL *antennaCumDiffAreaRatioPWL_;
  double          antennaAreaFactor_;
  double          antennaSideAreaRatio_;
  double          antennaDiffSideAreaRatio_;
  lefiAntennaPWL *antennaDiffSideAreaRatioPWL_;
  double          antennaCumSideAreaRatio_;
  double          antennaCumDiffSideAreaRatio_;
  lefiAntennaPWL *antennaCumDiffSideAreaRatioPWL_;
  double          antennaSideAreaFactor_;
} lefiAntennaModel;

EXTERN lefiAntennaModel *
lefiAntennaModel_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiAntennaModel_Delete
  PROTO_PARAMS(( lefiAntennaModel * this ));

EXTERN void
lefiAntennaModel_Init
  PROTO_PARAMS(( lefiAntennaModel * this ));

EXTERN void
lefiAntennaModel_clear
  PROTO_PARAMS(( lefiAntennaModel * this ));

EXTERN void
lefiAntennaModel_Destroy
  PROTO_PARAMS(( lefiAntennaModel * this ));

EXTERN void
lefiAntennaModel_setAntennaModel
  PROTO_PARAMS(( lefiAntennaModel * this,
                 int oxide ));

EXTERN void
lefiAntennaModel_setAntennaAreaRatio
  PROTO_PARAMS(( lefiAntennaModel * this,
                 double value ));

EXTERN void
lefiAntennaModel_setAntennaCumAreaRatio
  PROTO_PARAMS(( lefiAntennaModel * this,
                 double value ));

EXTERN void
lefiAntennaModel_setAntennaAreaFactor
  PROTO_PARAMS(( lefiAntennaModel * this,
                 double value ));

EXTERN void
lefiAntennaModel_setAntennaSideAreaRatio
  PROTO_PARAMS(( lefiAntennaModel * this,
                 double value ));

EXTERN void
lefiAntennaModel_setAntennaCumSideAreaRatio
  PROTO_PARAMS(( lefiAntennaModel * this,
                 double value ));

EXTERN void
lefiAntennaModel_setAntennaSideAreaFactor
  PROTO_PARAMS(( lefiAntennaModel * this,
                 double value ));

EXTERN void
lefiAntennaModel_setAntennaValue
  PROTO_PARAMS(( lefiAntennaModel * this,
                 lefiAntennaEnum antennaType,
                 double value ));

EXTERN void
lefiAntennaModel_setAntennaDUO
  PROTO_PARAMS(( lefiAntennaModel * this,
                 lefiAntennaEnum antennaType ));

EXTERN void
lefiAntennaModel_setAntennaPWL
  PROTO_PARAMS(( lefiAntennaModel * this,
                 lefiAntennaEnum antennaType,
                 lefiAntennaPWL * pwl ));

EXTERN int
lefiAntennaModel_hasAntennaAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaDiffAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaDiffAreaRatioPWL
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaCumAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaCumDiffAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaCumDiffAreaRatioPWL
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaAreaFactor
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaAreaFactorDUO
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaSideAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaDiffSideAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaDiffSideAreaRatioPWL
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaCumSideAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaCumDiffSideAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaCumDiffSideAreaRatioPWL
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaSideAreaFactor
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN int
lefiAntennaModel_hasAntennaSideAreaFactorDUO
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN char *
lefiAntennaModel_antennaOxide
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN double
lefiAntennaModel_antennaAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN double
lefiAntennaModel_antennaDiffAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN lefiAntennaPWL *
lefiAntennaModel_antennaDiffAreaRatioPWL
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN double
lefiAntennaModel_antennaCumAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN double
lefiAntennaModel_antennaCumDiffAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN lefiAntennaPWL *
lefiAntennaModel_antennaCumDiffAreaRatioPWL
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN double
lefiAntennaModel_antennaAreaFactor
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN double
lefiAntennaModel_antennaSideAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN double
lefiAntennaModel_antennaDiffSideAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN lefiAntennaPWL *
lefiAntennaModel_antennaDiffSideAreaRatioPWL
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN double
lefiAntennaModel_antennaCumSideAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN double
lefiAntennaModel_antennaCumDiffSideAreaRatio
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN lefiAntennaPWL *
lefiAntennaModel_antennaCumDiffSideAreaRatioPWL
  PROTO_PARAMS(( const lefiAntennaModel * this ));

EXTERN double
lefiAntennaModel_antennaSideAreaFactor
  PROTO_PARAMS(( const lefiAntennaModel * this ));

/*
 * calls clear to init
 */

/*
 * for CUT layer
 */

/*
 * 5.5for CUT layer
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
 * 5.5
 */

/*
 * this is for the parser internal use only
 */

/*
 * 5.5 SPACINGTABLE
 */

/*
 * for CUT layer
 */

/*
 * 5.5 - for CUT layer
 */

/*
 * 5.5 - for CUT layer
 */

/*
 * 5.5 Minimum cut rules
 */

/*
 * FROMABOVE|FROMBELOW
 */

/*
 * FROMABOVE|FROMBELOW
 */

/*
 * 3/23/2000 - Wanda da Rosa.  The following are for 5.4 Antenna.
 */

/*
 * Only 5.4 or 5.3 are allowed in a lef file, but not both
 */

/*
 * 5.5
 */

/*
 * 5.5
 */

/*
 * The following is 8/21/01 5.4 enhancements
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
 * 5.5
 */

/*
 * 5.5
 */

/*
 * 5.5
 */

/*
 * 5.5 MINENCLOSEDAREA
 */

/*
 * 5.5 SPACINGTABLE
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

/*
 * pcr 409334
 */

/*
 * pcr 282799, due to mult. spacing allow
 */

/*
 * pcr 409334
 */

/*
 * for Cut & routing Layer, spacing is multiple
 */

/*
 * 5.5
 */

/*
 * 5.5
 */

/*
 * pcr 282799 & 408930, due to mult spacing allow
 */

/*
 * pcr 282799 & 408930, due to mult spacing allow
 */

/*
 * pcr 282799 & 408930, due to mult spacing allow
 */

/*
 * pcr 388183 & 408930
 */

/*
 * pcr 388183 & 408930
 */

/*
 * pcr 408930
 */

/*
 * pcr 408930
 */

/*
 * pcr 408930
 */

/*
 * pcr 408930
 */

/*
 * pcr 408930
 */

/*
 * 5.5
 */

/*
 * pcr 409334
 */

/*
 * pcr 409334
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
 * 5.5
 */

/*
 * 5.5
 */

/*
 * 5.5
 */

/*
 * I: integer, R: real, S:string
 */

/*
 * Q: quotedstring
 */

/*
 * number of ACCURRENTDENSITY
 */

/*
 * number of DCCURRENTDENSITY
 */

/*
 * 3/23/2000 - Wanda da Rosa.  The following is for 5.4 ANTENNA.
 */

/*
 * Either 5.4 or 5.3 are allowed, not both
 */

/*
 * 5.5 AntennaModel
 */

/*
 * 8/29/2001 - Wanda da Rosa.  The following is for 5.4 enhancements.
 */

/*
 * 5.5 SPACINGTABLE
 */

typedef struct lefiLayer_s {
  char           *name_;
  int             nameSize_;
  char           *type_;
  int             typeSize_;
  int             hasPitch_;
  int             hasOffset_;
  int             hasWidth_;
  int             hasArea_;
  int             hasSpacing_;
  int            *hasSpacingName_;
  int            *hasSpacingAdjacent_;
  int            *hasSpacingRange_;
  int            *hasSpacingUseLengthThreshold_;
  int            *hasSpacingLengthThreshold_;
  int             hasDirection_;
  int             hasResistance_;
  int             hasCapacitance_;
  int             hasHeight_;
  int             hasWireExtension_;
  int             hasThickness_;
  int             hasShrinkage_;
  int             hasCapMultiplier_;
  int             hasEdgeCap_;
  int             hasAntennaArea_;
  int             hasAntennaLength_;
  int             hasCurrentDensityPoint_;
  double          currentDensity_;
  double          pitch_;
  double          offset_;
  double          width_;
  double          area_;
  double          wireExtension_;
  int             numSpacings_;
  int             spacingsAllocated_;
  double         *spacing_;
  char          **spacingName_;
  int            *spacingAdjacentCuts_;
  double         *spacingAdjacentWithin_;
  double         *rangeMin_;
  double         *rangeMax_;
  double         *rangeInfluence_;
  double         *rangeInfluenceRangeMin_;
  double         *rangeInfluenceRangeMax_;
  double         *rangeRangeMin_;
  double         *rangeRangeMax_;
  double         *lengthThreshold_;
  double         *lengthThresholdRangeMin_;
  double         *lengthThresholdRangeMax_;
  int             numMinimumcut_;
  int             minimumcutAllocated_;
  int            *minimumcut_;
  double         *minimumcutWidth_;
  int            *hasMinimumcutConnection_;
  char          **minimumcutConnection_;
  int            *hasMinimumcutNumCuts_;
  double         *minimumcutLength_;
  double         *minimumcutDistance_;
  double          maxwidth_;
  double          minwidth_;
  int             numMinenclosedarea_;
  int             minenclosedareaAllocated_;
  double         *minenclosedarea_;
  double         *minenclosedareaWidth_;
  double          minstep_;
  double          protrusionWidth1_;
  double          protrusionLength_;
  double          protrusionWidth2_;
  char           *direction_;
  double          resistance_;
  double          capacitance_;
  double          height_;
  double          thickness_;
  double          shrinkage_;
  double          capMultiplier_;
  double          edgeCap_;
  double          antennaArea_;
  double          antennaLength_;
  int             numCurrentPoints_;
  int             currentPointsAllocated_;
  double         *currentWidths_;
  double         *current_;
  int             numCapacitancePoints_;
  int             capacitancePointsAllocated_;
  double         *capacitanceWidths_;
  double         *capacitances_;
  int             numResistancePoints_;
  int             resistancePointsAllocated_;
  double         *resistanceWidths_;
  double         *resistances_;
  int             numProps_;
  int             propsAllocated_;
  char          **names_;
  char          **values_;
  double         *dvalues_;
  char           *types_;
  int             numAccurrents_;
  int             accurrentAllocated_;
  lefiLayerDensity **accurrents_;
  int             numDccurrents_;
  int             dccurrentAllocated_;
  lefiLayerDensity **dccurrents_;
  int             numNums_;
  int             numAllocated_;
  double         *nums_;
  int             hasAntennaAreaRatio_;
  int             hasAntennaDiffAreaRatio_;
  int             hasAntennaDiffAreaRatioPWL_;
  int             hasAntennaCumAreaRatio_;
  int             hasAntennaCumDiffAreaRatio_;
  int             hasAntennaCumDiffAreaRatioPWL_;
  int             hasAntennaAreaFactor_;
  int             hasAntennaAreaFactorDUO_;
  int             hasAntennaSideAreaRatio_;
  int             hasAntennaDiffSideAreaRatio_;
  int             hasAntennaDiffSideAreaRatioPWL_;
  int             hasAntennaCumSideAreaRatio_;
  int             hasAntennaCumDiffSideAreaRatio_;
  int             hasAntennaCumDiffSideAreaRatioPWL_;
  int             hasAntennaSideAreaFactor_;
  int             hasAntennaSideAreaFactorDUO_;
  int             numAntennaModel_;
  int             antennaModelAllocated_;
  lefiAntennaModel **antennaModel_;
  int             hasSlotWireWidth_;
  int             hasSlotWireLength_;
  int             hasSlotWidth_;
  int             hasSlotLength_;
  int             hasMaxAdjacentSlotSpacing_;
  int             hasMaxCoaxialSlotSpacing_;
  int             hasMaxEdgeSlotSpacing_;
  int             hasSplitWireWidth_;
  int             hasMinimumDensity_;
  int             hasMaximumDensity_;
  int             hasDensityCheckWindow_;
  int             hasDensityCheckStep_;
  int             hasFillActiveSpacing_;
  double          slotWireWidth_;
  double          slotWireLength_;
  double          slotWidth_;
  double          slotLength_;
  double          maxAdjacentSlotSpacing_;
  double          maxCoaxialSlotSpacing_;
  double          maxEdgeSlotSpacing_;
  double          splitWireWidth_;
  double          minimumDensity_;
  double          maximumDensity_;
  double          densityCheckWindowLength_;
  double          densityCheckWindowWidth_;
  double          densityCheckStep_;
  double          fillActiveSpacing_;
  int             numSpacingTable_;
  int             spacingTableAllocated_;
  lefiSpacingTable **spacingTable_;
} lefiLayer;

EXTERN lefiLayer *
lefiLayer_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiLayer_Init
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_Destroy
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_Delete
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_clear
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_setName
  PROTO_PARAMS(( lefiLayer * this,
                 const char *name ));

EXTERN void
lefiLayer_setType
  PROTO_PARAMS(( lefiLayer * this,
                 const char *typ ));

EXTERN void
lefiLayer_setPitch
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setOffset
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setWidth
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setArea
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setSpacingMin
  PROTO_PARAMS(( lefiLayer * this,
                 double dist ));

EXTERN void
lefiLayer_setSpacingName
  PROTO_PARAMS(( lefiLayer * this,
                 const char *spacingName ));

EXTERN void
lefiLayer_setSpacingAdjacent
  PROTO_PARAMS(( lefiLayer * this,
                 int numCuts,
                 double distance ));

EXTERN void
lefiLayer_setSpacingRange
  PROTO_PARAMS(( lefiLayer * this,
                 double left,
                 double right ));

EXTERN void
lefiLayer_setSpacingRangeUseLength
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_setSpacingRangeInfluence
  PROTO_PARAMS(( lefiLayer * this,
                 double infLength ));

EXTERN void
lefiLayer_setSpacingRangeInfluenceRange
  PROTO_PARAMS(( lefiLayer * this,
                 double min,
                 double max ));

EXTERN void
lefiLayer_setSpacingRangeRange
  PROTO_PARAMS(( lefiLayer * this,
                 double min,
                 double max ));

EXTERN void
lefiLayer_setSpacingLength
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setSpacingLengthRange
  PROTO_PARAMS(( lefiLayer * this,
                 double min,
                 double max ));

EXTERN void
lefiLayer_setDirection
  PROTO_PARAMS(( lefiLayer * this,
                 const char *dir ));

EXTERN void
lefiLayer_setResistance
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setCapacitance
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setHeight
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setWireExtension
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setThickness
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setShrinkage
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setCapMultiplier
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setEdgeCap
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setAntennaArea
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setAntennaLength
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setCurrentDensity
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setCurrentPoint
  PROTO_PARAMS(( lefiLayer * this,
                 double width,
                 double current ));

EXTERN void
lefiLayer_setResistancePoint
  PROTO_PARAMS(( lefiLayer * this,
                 double width,
                 double res ));

EXTERN void
lefiLayer_setCapacitancePoint
  PROTO_PARAMS(( lefiLayer * this,
                 double width,
                 double cap ));

EXTERN void
lefiLayer_addProp
  PROTO_PARAMS(( lefiLayer * this,
                 const char *name,
                 const char *value,
                 const char type ));

EXTERN void
lefiLayer_addNumProp
  PROTO_PARAMS(( lefiLayer * this,
                 const char *name,
                 const double d,
                 const char *value,
                 const char type ));

EXTERN void
lefiLayer_addAccurrentDensity
  PROTO_PARAMS(( lefiLayer * this,
                 const char *type ));

EXTERN void
lefiLayer_setAcOneEntry
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_addAcFrequency
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_addAcCutarea
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_addAcTableEntry
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_addAcWidth
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_addDccurrentDensity
  PROTO_PARAMS(( lefiLayer * this,
                 const char *type ));

EXTERN void
lefiLayer_setDcOneEntry
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_addDcTableEntry
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_addDcWidth
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_addDcCutarea
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_addNumber
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setMaxwidth
  PROTO_PARAMS(( lefiLayer * this,
                 double width ));

EXTERN void
lefiLayer_setMinwidth
  PROTO_PARAMS(( lefiLayer * this,
                 double width ));

EXTERN void
lefiLayer_addMinenclosedarea
  PROTO_PARAMS(( lefiLayer * this,
                 double area ));

EXTERN void
lefiLayer_addMinenclosedareaWidth
  PROTO_PARAMS(( lefiLayer * this,
                 double width ));

EXTERN void
lefiLayer_addMinimumcut
  PROTO_PARAMS(( lefiLayer * this,
                 int cuts,
                 double width ));

EXTERN void
lefiLayer_addMinimumcutConnect
  PROTO_PARAMS(( lefiLayer * this,
                 const char *direction ));

EXTERN void
lefiLayer_addMinimumcutLengDis
  PROTO_PARAMS(( lefiLayer * this,
                 double length,
                 double distance ));

EXTERN void
lefiLayer_setMinstep
  PROTO_PARAMS(( lefiLayer * this,
                 double distance ));

EXTERN void
lefiLayer_addParellelLength
  PROTO_PARAMS(( lefiLayer * this,
                 double length ));

EXTERN void
lefiLayer_addParellelSpacing
  PROTO_PARAMS(( lefiLayer * this,
                 double width,
                 double spacing ));

EXTERN void
lefiLayer_addParellelWidth
  PROTO_PARAMS(( lefiLayer * this,
                 double width ));

EXTERN void
lefiLayer_setProtrusion
  PROTO_PARAMS(( lefiLayer * this,
                 double width,
                 double length,
                 double width2 ));

EXTERN int
lefiLayer_getNumber
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_addSpacingTable
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_addSpParallelLength
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_addSpParallelWidth
  PROTO_PARAMS(( lefiLayer * this,
                 double width ));

EXTERN void
lefiLayer_addSpParallelWidthSpacing
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_setInfluence
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN void
lefiLayer_addSpInfluence
  PROTO_PARAMS(( lefiLayer * this,
                 double width,
                 double distance,
                 double spacing ));

EXTERN int
lefiLayer_hasType
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasPitch
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasOffset
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasWidth
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasArea
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasSpacingNumber
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasSpacingName
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_hasSpacingAdjacent
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_hasSpacingRange
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_hasSpacingRangeUseLengthThreshold
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_hasSpacingRangeInfluence
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_hasSpacingRangeInfluenceRange
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_hasSpacingRangeRange
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_hasSpacingLengthThreshold
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_hasSpacingLengthThresholdRange
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_hasDirection
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasResistance
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasResistanceArray
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasCapacitance
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasCapacitanceArray
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasHeight
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasThickness
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasWireExtension
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasShrinkage
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasCapMultiplier
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasEdgeCap
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasAntennaLength
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasAntennaArea
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasCurrentDensityPoint
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasCurrentDensityArray
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasAccurrentDensity
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasDccurrentDensity
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_numProps
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN const char *
lefiLayer_propName
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN const char *
lefiLayer_propValue
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_propNumber
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN const char
lefiLayer_propType
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_propIsNumber
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_propIsString
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_numSpacing
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN char *
lefiLayer_name
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN const char *
lefiLayer_type
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_pitch
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_offset
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_width
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_area
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_spacing
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN char *
lefiLayer_spacingName
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_spacingAdjacentCuts
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_spacingAdjacentWithin
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_spacingRangeMin
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_spacingRangeMax
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_spacingRangeInfluence
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_spacingRangeInfluenceMin
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_spacingRangeInfluenceMax
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_spacingRangeRangeMin
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_spacingRangeRangeMax
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_spacingLengthThreshold
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_spacingLengthThresholdRangeMin
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_spacingLengthThresholdRangeMax
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_numMinimumcut
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_minimumcut
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_minimumcutWidth
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_hasMinimumcutConnection
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN const char *
lefiLayer_minimumcutConnection
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_hasMinimumcutNumCuts
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_minimumcutLength
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_minimumcutDistance
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN const char *
lefiLayer_direction
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_resistance
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_capacitance
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_height
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_wireExtension
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_thickness
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_shrinkage
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_capMultiplier
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_edgeCap
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_antennaLength
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_antennaArea
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_currentDensityPoint
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN void
lefiLayer_currentDensityArray
  PROTO_PARAMS(( const lefiLayer * this,
                 int *numPoints,
                 double **widths,
                 double **current ));

EXTERN void
lefiLayer_capacitanceArray
  PROTO_PARAMS(( const lefiLayer * this,
                 int *numPoints,
                 double **widths,
                 double **resValues ));

EXTERN void
lefiLayer_resistanceArray
  PROTO_PARAMS(( const lefiLayer * this,
                 int *numPoints,
                 double **widths,
                 double **capValues ));

EXTERN int
lefiLayer_numAccurrentDensity
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN lefiLayerDensity *
lefiLayer_accurrent
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_numDccurrentDensity
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN lefiLayerDensity *
lefiLayer_dccurrent
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN void
lefiLayer_setAntennaAreaRatio
  PROTO_PARAMS(( lefiLayer * this,
                 double value ));

EXTERN void
lefiLayer_setAntennaCumAreaRatio
  PROTO_PARAMS(( lefiLayer * this,
                 double value ));

EXTERN void
lefiLayer_setAntennaAreaFactor
  PROTO_PARAMS(( lefiLayer * this,
                 double value ));

EXTERN void
lefiLayer_setAntennaSideAreaRatio
  PROTO_PARAMS(( lefiLayer * this,
                 double value ));

EXTERN void
lefiLayer_setAntennaCumSideAreaRatio
  PROTO_PARAMS(( lefiLayer * this,
                 double value ));

EXTERN void
lefiLayer_setAntennaSideAreaFactor
  PROTO_PARAMS(( lefiLayer * this,
                 double value ));

EXTERN void
lefiLayer_setAntennaValue
  PROTO_PARAMS(( lefiLayer * this,
                 lefiAntennaEnum antennaType,
                 double value ));

EXTERN void
lefiLayer_setAntennaDUO
  PROTO_PARAMS(( lefiLayer * this,
                 lefiAntennaEnum antennaType ));

EXTERN void
lefiLayer_setAntennaPWL
  PROTO_PARAMS(( lefiLayer * this,
                 lefiAntennaEnum antennaType,
                 lefiAntennaPWL * pwl ));

EXTERN void
lefiLayer_addAntennaModel
  PROTO_PARAMS(( lefiLayer * this,
                 int aOxide ));

EXTERN int
lefiLayer_numAntennaModel
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN lefiAntennaModel *
lefiLayer_antennaModel
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN void
lefiLayer_setSlotWireWidth
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setSlotWireLength
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setSlotWidth
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setSlotLength
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setMaxAdjacentSlotSpacing
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setMaxCoaxialSlotSpacing
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setMaxEdgeSlotSpacing
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setSplitWireWidth
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setMinimumDensity
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setMaximumDensity
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setDensityCheckWindow
  PROTO_PARAMS(( lefiLayer * this,
                 double length,
                 double width ));

EXTERN void
lefiLayer_setDensityCheckStep
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN void
lefiLayer_setFillActiveSpacing
  PROTO_PARAMS(( lefiLayer * this,
                 double num ));

EXTERN int
lefiLayer_hasSlotWireWidth
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasSlotWireLength
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasSlotWidth
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasSlotLength
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasMaxAdjacentSlotSpacing
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasMaxCoaxialSlotSpacing
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasMaxEdgeSlotSpacing
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasSplitWireWidth
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasMinimumDensity
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasMaximumDensity
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasDensityCheckWindow
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasDensityCheckStep
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasFillActiveSpacing
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasMaxwidth
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasMinwidth
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasMinstep
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_hasProtrusion
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_slotWireWidth
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_slotWireLength
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_slotWidth
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_slotLength
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_maxAdjacentSlotSpacing
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_maxCoaxialSlotSpacing
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_maxEdgeSlotSpacing
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_splitWireWidth
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_minimumDensity
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_maximumDensity
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_densityCheckWindowLength
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_densityCheckWindowWidth
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_densityCheckStep
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_fillActiveSpacing
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_maxwidth
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_minwidth
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_minstep
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_protrusionWidth1
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_protrusionLength
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_protrusionWidth2
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN int
lefiLayer_numMinenclosedarea
  PROTO_PARAMS(( const lefiLayer * this ));

EXTERN double
lefiLayer_minenclosedarea
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_hasMinenclosedareaWidth
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN double
lefiLayer_minenclosedareaWidth
  PROTO_PARAMS(( const lefiLayer * this,
                 int index ));

EXTERN int
lefiLayer_numSpacingTable
  PROTO_PARAMS(( lefiLayer * this ));

EXTERN lefiSpacingTable *
lefiLayer_spacingTable
  PROTO_PARAMS(( lefiLayer * this,
                 int index ));

EXTERN void
lefiLayer_print
  PROTO_PARAMS(( const lefiLayer * this,
                 FILE * f ));

#endif
