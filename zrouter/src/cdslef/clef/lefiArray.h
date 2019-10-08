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

#ifndef lefiArray_h
#define lefiArray_h
#include <stdio.h>
#include "lefiKRDefs.h"
#include "lefiMisc.h"

typedef struct lefiArrayFloorPlan_s {
  int             numPatterns_;
  int             patternsAllocated_;
  lefiSitePattern **patterns_;
  char          **types_;
  char           *name_;
} lefiArrayFloorPlan;

EXTERN void
lefiArrayFloorPlan_Init
  PROTO_PARAMS(( lefiArrayFloorPlan * this,
                 const char *name ));

EXTERN void
lefiArrayFloorPlan_Destroy
  PROTO_PARAMS(( lefiArrayFloorPlan * this ));

EXTERN void
lefiArrayFloorPlan_addSitePattern
  PROTO_PARAMS(( lefiArrayFloorPlan * this,
                 const char *typ,
                 lefiSitePattern * s ));

EXTERN int
lefiArrayFloorPlan_numPatterns
  PROTO_PARAMS(( const lefiArrayFloorPlan * this ));

EXTERN lefiSitePattern *
lefiArrayFloorPlan_pattern
  PROTO_PARAMS(( lefiArrayFloorPlan * this,
                 int index ));

EXTERN char *
lefiArrayFloorPlan_typ
  PROTO_PARAMS(( lefiArrayFloorPlan * this,
                 int index ));

EXTERN const char *
lefiArrayFloorPlan_name
  PROTO_PARAMS(( lefiArrayFloorPlan * this ));

/*
 * Debug print
 */

typedef struct lefiArray_s {
  int             nameSize_;
  char           *name_;
  int             patternsAllocated_;
  int             numPatterns_;
  lefiSitePattern **pattern_;
  int             canAllocated_;
  int             numCan_;
  lefiSitePattern **canPlace_;
  int             cannotAllocated_;
  int             numCannot_;
  lefiSitePattern **cannotOccupy_;
  int             tracksAllocated_;
  int             numTracks_;
  lefiTrackPattern **track_;
  int             gAllocated_;
  int             numG_;
  lefiGcellPattern **gcell_;
  int             hasDefault_;
  int             tableSize_;
  int             numDefault_;
  int             defaultAllocated_;
  int            *minPins_;
  double         *caps_;
  int             numFloorPlans_;
  int             floorPlansAllocated_;
  lefiArrayFloorPlan **floors_;
} lefiArray;

EXTERN lefiArray *
lefiArray_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiArray_Init
  PROTO_PARAMS(( lefiArray * this ));

EXTERN void
lefiArray_Destroy
  PROTO_PARAMS(( lefiArray * this ));

EXTERN void
lefiArray_Delete
  PROTO_PARAMS(( lefiArray * this ));

EXTERN void
lefiArray_setName
  PROTO_PARAMS(( lefiArray * this,
                 const char *name ));

EXTERN void
lefiArray_addSitePattern
  PROTO_PARAMS(( lefiArray * this,
                 lefiSitePattern * s ));

EXTERN void
lefiArray_setTableSize
  PROTO_PARAMS(( lefiArray * this,
                 int tsize ));

EXTERN void
lefiArray_addDefaultCap
  PROTO_PARAMS(( lefiArray * this,
                 int minPins,
                 double cap ));

EXTERN void
lefiArray_addCanPlace
  PROTO_PARAMS(( lefiArray * this,
                 lefiSitePattern * s ));

EXTERN void
lefiArray_addCannotOccupy
  PROTO_PARAMS(( lefiArray * this,
                 lefiSitePattern * s ));

EXTERN void
lefiArray_addTrack
  PROTO_PARAMS(( lefiArray * this,
                 lefiTrackPattern * t ));

EXTERN void
lefiArray_addGcell
  PROTO_PARAMS(( lefiArray * this,
                 lefiGcellPattern * g ));

EXTERN void
lefiArray_addFloorPlan
  PROTO_PARAMS(( lefiArray * this,
                 const char *name ));

EXTERN void
lefiArray_addSiteToFloorPlan
  PROTO_PARAMS(( lefiArray * this,
                 const char *typ,
                 lefiSitePattern * p ));

EXTERN void
lefiArray_clear
  PROTO_PARAMS(( lefiArray * this ));

EXTERN void
lefiArray_bump
  PROTO_PARAMS(( lefiArray * this,
                 void ***arr,
                 int used,
                 int *allocated ));

EXTERN int
lefiArray_numSitePattern
  PROTO_PARAMS(( lefiArray * this ));

EXTERN int
lefiArray_numCanPlace
  PROTO_PARAMS(( lefiArray * this ));

EXTERN int
lefiArray_numCannotOccupy
  PROTO_PARAMS(( lefiArray * this ));

EXTERN int
lefiArray_numTrack
  PROTO_PARAMS(( lefiArray * this ));

EXTERN int
lefiArray_numGcell
  PROTO_PARAMS(( lefiArray * this ));

EXTERN int
lefiArray_hasDefaultCap
  PROTO_PARAMS(( lefiArray * this ));

EXTERN const char *
lefiArray_name
  PROTO_PARAMS(( lefiArray * this ));

EXTERN lefiSitePattern *
lefiArray_sitePattern
  PROTO_PARAMS(( lefiArray * this,
                 int index ));

EXTERN lefiSitePattern *
lefiArray_canPlace
  PROTO_PARAMS(( lefiArray * this,
                 int index ));

EXTERN lefiSitePattern *
lefiArray_cannotOccupy
  PROTO_PARAMS(( lefiArray * this,
                 int index ));

EXTERN lefiTrackPattern *
lefiArray_track
  PROTO_PARAMS(( lefiArray * this,
                 int index ));

EXTERN lefiGcellPattern *
lefiArray_gcell
  PROTO_PARAMS(( lefiArray * this,
                 int index ));

EXTERN int
lefiArray_tableSize
  PROTO_PARAMS(( lefiArray * this ));

EXTERN int
lefiArray_numDefaultCaps
  PROTO_PARAMS(( lefiArray * this ));

EXTERN int
lefiArray_defaultCapMinPins
  PROTO_PARAMS(( lefiArray * this,
                 int index ));

EXTERN double
lefiArray_defaultCap
  PROTO_PARAMS(( lefiArray * this,
                 int index ));

EXTERN int
lefiArray_numFloorPlans
  PROTO_PARAMS(( lefiArray * this ));

EXTERN const char *
lefiArray_floorPlanName
  PROTO_PARAMS(( lefiArray * this,
                 int index ));

EXTERN int
lefiArray_numSites
  PROTO_PARAMS(( lefiArray * this,
                 int index ));

EXTERN const char *
lefiArray_siteType
  PROTO_PARAMS(( lefiArray * this,
                 int floorIndex,
                 int siteIndex ));

EXTERN lefiSitePattern *
lefiArray_site
  PROTO_PARAMS(( lefiArray * this,
                 int floorIndex,
                 int siteIndex ));

EXTERN void
lefiArray_print
  PROTO_PARAMS(( lefiArray * this,
                 FILE * f ));

#endif
