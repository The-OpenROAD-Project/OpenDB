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

#ifndef lefiCrossTalk_h
#define lefiCrossTalk_h
#include <stdio.h>
#include "lefiKRDefs.h"

/*
 * Structure returned for the noise margin callback. This lef construct has
 * two floating point numbers.
 */
struct lefiNoiseMargin {
  double          high;
  double          low;
};

typedef struct lefiNoiseVictim_s {
  double          length_;
  int             numNoises_;
  int             noisesAllocated_;
  double         *noises_;
} lefiNoiseVictim;

EXTERN lefiNoiseVictim *
lefiNoiseVictim_Create
  PROTO_PARAMS(( double d ));

EXTERN void
lefiNoiseVictim_Init
  PROTO_PARAMS(( lefiNoiseVictim * this,
                 double d ));

EXTERN void
lefiNoiseVictim_Destroy
  PROTO_PARAMS(( lefiNoiseVictim * this ));

EXTERN void
lefiNoiseVictim_Delete
  PROTO_PARAMS(( lefiNoiseVictim * this ));

EXTERN void
lefiNoiseVictim_clear
  PROTO_PARAMS(( lefiNoiseVictim * this ));

EXTERN void
lefiNoiseVictim_addVictimNoise
  PROTO_PARAMS(( lefiNoiseVictim * this,
                 double d ));

EXTERN double
lefiNoiseVictim_length
  PROTO_PARAMS(( lefiNoiseVictim * this ));

EXTERN int
lefiNoiseVictim_numNoises
  PROTO_PARAMS(( lefiNoiseVictim * this ));

EXTERN double
lefiNoiseVictim_noise
  PROTO_PARAMS(( lefiNoiseVictim * this,
                 int index ));


typedef struct lefiNoiseResistance_s {
  int             numNums_;
  int             numsAllocated_;
  double         *nums_;
  int             numVictims_;
  int             victimsAllocated_;
  lefiNoiseVictim **victims_;
} lefiNoiseResistance;

EXTERN lefiNoiseResistance *
lefiNoiseResistance_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiNoiseResistance_Init
  PROTO_PARAMS(( lefiNoiseResistance * this ));

EXTERN void
lefiNoiseResistance_Destroy
  PROTO_PARAMS(( lefiNoiseResistance * this ));

EXTERN void
lefiNoiseResistance_Delete
  PROTO_PARAMS(( lefiNoiseResistance * this ));

EXTERN void
lefiNoiseResistance_clear
  PROTO_PARAMS(( lefiNoiseResistance * this ));

EXTERN void
lefiNoiseResistance_addResistanceNumber
  PROTO_PARAMS(( lefiNoiseResistance * this,
                 double d ));

EXTERN void
lefiNoiseResistance_addVictimLength
  PROTO_PARAMS(( lefiNoiseResistance * this,
                 double d ));

EXTERN void
lefiNoiseResistance_addVictimNoise
  PROTO_PARAMS(( lefiNoiseResistance * this,
                 double d ));

EXTERN int
lefiNoiseResistance_numNums
  PROTO_PARAMS(( lefiNoiseResistance * this ));

EXTERN double
lefiNoiseResistance_num
  PROTO_PARAMS(( lefiNoiseResistance * this,
                 int index ));

EXTERN int
lefiNoiseResistance_numVictims
  PROTO_PARAMS(( lefiNoiseResistance * this ));

EXTERN lefiNoiseVictim *
lefiNoiseResistance_victim
  PROTO_PARAMS(( lefiNoiseResistance * this,
                 int index ));


typedef struct lefiNoiseEdge_s {
  double          edge_;
  int             numResistances_;
  int             resistancesAllocated_;
  lefiNoiseResistance **resistances_;
} lefiNoiseEdge;

EXTERN lefiNoiseEdge *
lefiNoiseEdge_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiNoiseEdge_Init
  PROTO_PARAMS(( lefiNoiseEdge * this ));

EXTERN void
lefiNoiseEdge_Destroy
  PROTO_PARAMS(( lefiNoiseEdge * this ));

EXTERN void
lefiNoiseEdge_Delete
  PROTO_PARAMS(( lefiNoiseEdge * this ));

EXTERN void
lefiNoiseEdge_clear
  PROTO_PARAMS(( lefiNoiseEdge * this ));

EXTERN void
lefiNoiseEdge_addEdge
  PROTO_PARAMS(( lefiNoiseEdge * this,
                 double d ));

EXTERN void
lefiNoiseEdge_addResistance
  PROTO_PARAMS(( lefiNoiseEdge * this ));

EXTERN void
lefiNoiseEdge_addResistanceNumber
  PROTO_PARAMS(( lefiNoiseEdge * this,
                 double d ));

EXTERN void
lefiNoiseEdge_addVictimLength
  PROTO_PARAMS(( lefiNoiseEdge * this,
                 double d ));

EXTERN void
lefiNoiseEdge_addVictimNoise
  PROTO_PARAMS(( lefiNoiseEdge * this,
                 double d ));

EXTERN double
lefiNoiseEdge_edge
  PROTO_PARAMS(( lefiNoiseEdge * this ));

EXTERN int
lefiNoiseEdge_numResistances
  PROTO_PARAMS(( lefiNoiseEdge * this ));

EXTERN lefiNoiseResistance *
lefiNoiseEdge_resistance
  PROTO_PARAMS(( lefiNoiseEdge * this,
                 int index ));


typedef struct lefiNoiseTable_s {
  int             num_;
  int             numEdges_;
  int             edgesAllocated_;
  lefiNoiseEdge **edges_;
} lefiNoiseTable;

EXTERN lefiNoiseTable *
lefiNoiseTable_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiNoiseTable_Init
  PROTO_PARAMS(( lefiNoiseTable * this ));

EXTERN void
lefiNoiseTable_Destroy
  PROTO_PARAMS(( lefiNoiseTable * this ));

EXTERN void
lefiNoiseTable_Delete
  PROTO_PARAMS(( lefiNoiseTable * this ));

EXTERN void
lefiNoiseTable_setup
  PROTO_PARAMS(( lefiNoiseTable * this,
                 int i ));

EXTERN void
lefiNoiseTable_newEdge
  PROTO_PARAMS(( lefiNoiseTable * this ));

EXTERN void
lefiNoiseTable_addEdge
  PROTO_PARAMS(( lefiNoiseTable * this,
                 double d ));

EXTERN void
lefiNoiseTable_addResistance
  PROTO_PARAMS(( lefiNoiseTable * this ));

EXTERN void
lefiNoiseTable_addResistanceNumber
  PROTO_PARAMS(( lefiNoiseTable * this,
                 double d ));

EXTERN void
lefiNoiseTable_addVictimLength
  PROTO_PARAMS(( lefiNoiseTable * this,
                 double d ));

EXTERN void
lefiNoiseTable_addVictimNoise
  PROTO_PARAMS(( lefiNoiseTable * this,
                 double d ));

EXTERN void
lefiNoiseTable_clear
  PROTO_PARAMS(( lefiNoiseTable * this ));

EXTERN int
lefiNoiseTable_num
  PROTO_PARAMS(( lefiNoiseTable * this ));

EXTERN int
lefiNoiseTable_numEdges
  PROTO_PARAMS(( lefiNoiseTable * this ));

EXTERN lefiNoiseEdge *
lefiNoiseTable_edge
  PROTO_PARAMS(( lefiNoiseTable * this,
                 int index ));


typedef struct lefiCorrectionVictim_s {
  double          length_;
  int             numCorrections_;
  int             correctionsAllocated_;
  double         *corrections_;
} lefiCorrectionVictim;

EXTERN lefiCorrectionVictim *
lefiCorrectionVictim_Create
  PROTO_PARAMS(( double d ));

EXTERN void
lefiCorrectionVictim_Init
  PROTO_PARAMS(( lefiCorrectionVictim * this,
                 double d ));

EXTERN void
lefiCorrectionVictim_Destroy
  PROTO_PARAMS(( lefiCorrectionVictim * this ));

EXTERN void
lefiCorrectionVictim_Delete
  PROTO_PARAMS(( lefiCorrectionVictim * this ));

EXTERN void
lefiCorrectionVictim_clear
  PROTO_PARAMS(( lefiCorrectionVictim * this ));

EXTERN void
lefiCorrectionVictim_addVictimCorrection
  PROTO_PARAMS(( lefiCorrectionVictim * this,
                 double d ));

EXTERN double
lefiCorrectionVictim_length
  PROTO_PARAMS(( lefiCorrectionVictim * this ));

EXTERN int
lefiCorrectionVictim_numCorrections
  PROTO_PARAMS(( lefiCorrectionVictim * this ));

EXTERN double
lefiCorrectionVictim_correction
  PROTO_PARAMS(( lefiCorrectionVictim * this,
                 int index ));


typedef struct lefiCorrectionResistance_s {
  int             numNums_;
  int             numsAllocated_;
  double         *nums_;
  int             numVictims_;
  int             victimsAllocated_;
  lefiCorrectionVictim **victims_;
} lefiCorrectionResistance;

EXTERN lefiCorrectionResistance *
lefiCorrectionResistance_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiCorrectionResistance_Init
  PROTO_PARAMS(( lefiCorrectionResistance * this ));

EXTERN void
lefiCorrectionResistance_Destroy
  PROTO_PARAMS(( lefiCorrectionResistance * this ));

EXTERN void
lefiCorrectionResistance_Delete
  PROTO_PARAMS(( lefiCorrectionResistance * this ));

EXTERN void
lefiCorrectionResistance_clear
  PROTO_PARAMS(( lefiCorrectionResistance * this ));

EXTERN void
lefiCorrectionResistance_addResistanceNumber
  PROTO_PARAMS(( lefiCorrectionResistance * this,
                 double d ));

EXTERN void
lefiCorrectionResistance_addVictimLength
  PROTO_PARAMS(( lefiCorrectionResistance * this,
                 double d ));

EXTERN void
lefiCorrectionResistance_addVictimCorrection
  PROTO_PARAMS(( lefiCorrectionResistance * this,
                 double d ));

EXTERN int
lefiCorrectionResistance_numNums
  PROTO_PARAMS(( lefiCorrectionResistance * this ));

EXTERN double
lefiCorrectionResistance_num
  PROTO_PARAMS(( lefiCorrectionResistance * this,
                 int index ));

EXTERN int
lefiCorrectionResistance_numVictims
  PROTO_PARAMS(( lefiCorrectionResistance * this ));

EXTERN lefiCorrectionVictim *
lefiCorrectionResistance_victim
  PROTO_PARAMS(( lefiCorrectionResistance * this,
                 int index ));


typedef struct lefiCorrectionEdge_s {
  double          edge_;
  int             numResistances_;
  int             resistancesAllocated_;
  lefiCorrectionResistance **resistances_;
} lefiCorrectionEdge;

EXTERN lefiCorrectionEdge *
lefiCorrectionEdge_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiCorrectionEdge_Init
  PROTO_PARAMS(( lefiCorrectionEdge * this ));

EXTERN void
lefiCorrectionEdge_Destroy
  PROTO_PARAMS(( lefiCorrectionEdge * this ));

EXTERN void
lefiCorrectionEdge_Delete
  PROTO_PARAMS(( lefiCorrectionEdge * this ));

EXTERN void
lefiCorrectionEdge_clear
  PROTO_PARAMS(( lefiCorrectionEdge * this ));

EXTERN void
lefiCorrectionEdge_addEdge
  PROTO_PARAMS(( lefiCorrectionEdge * this,
                 double d ));

EXTERN void
lefiCorrectionEdge_addResistance
  PROTO_PARAMS(( lefiCorrectionEdge * this ));

EXTERN void
lefiCorrectionEdge_addResistanceNumber
  PROTO_PARAMS(( lefiCorrectionEdge * this,
                 double d ));

EXTERN void
lefiCorrectionEdge_addVictimLength
  PROTO_PARAMS(( lefiCorrectionEdge * this,
                 double d ));

EXTERN void
lefiCorrectionEdge_addVictimCorrection
  PROTO_PARAMS(( lefiCorrectionEdge * this,
                 double d ));

EXTERN double
lefiCorrectionEdge_edge
  PROTO_PARAMS(( lefiCorrectionEdge * this ));

EXTERN int
lefiCorrectionEdge_numResistances
  PROTO_PARAMS(( lefiCorrectionEdge * this ));

EXTERN lefiCorrectionResistance *
lefiCorrectionEdge_resistance
  PROTO_PARAMS(( lefiCorrectionEdge * this,
                 int index ));


typedef struct lefiCorrectionTable_s {
  int             num_;
  int             numEdges_;
  int             edgesAllocated_;
  lefiCorrectionEdge **edges_;
} lefiCorrectionTable;

EXTERN lefiCorrectionTable *
lefiCorrectionTable_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiCorrectionTable_Init
  PROTO_PARAMS(( lefiCorrectionTable * this ));

EXTERN void
lefiCorrectionTable_Destroy
  PROTO_PARAMS(( lefiCorrectionTable * this ));

EXTERN void
lefiCorrectionTable_Delete
  PROTO_PARAMS(( lefiCorrectionTable * this ));

EXTERN void
lefiCorrectionTable_setup
  PROTO_PARAMS(( lefiCorrectionTable * this,
                 int i ));

EXTERN void
lefiCorrectionTable_newEdge
  PROTO_PARAMS(( lefiCorrectionTable * this ));

EXTERN void
lefiCorrectionTable_addEdge
  PROTO_PARAMS(( lefiCorrectionTable * this,
                 double d ));

EXTERN void
lefiCorrectionTable_addResistance
  PROTO_PARAMS(( lefiCorrectionTable * this ));

EXTERN void
lefiCorrectionTable_addResistanceNumber
  PROTO_PARAMS(( lefiCorrectionTable * this,
                 double d ));

EXTERN void
lefiCorrectionTable_addVictimLength
  PROTO_PARAMS(( lefiCorrectionTable * this,
                 double d ));

EXTERN void
lefiCorrectionTable_addVictimCorrection
  PROTO_PARAMS(( lefiCorrectionTable * this,
                 double d ));

EXTERN void
lefiCorrectionTable_clear
  PROTO_PARAMS(( lefiCorrectionTable * this ));

EXTERN int
lefiCorrectionTable_num
  PROTO_PARAMS(( lefiCorrectionTable * this ));

EXTERN int
lefiCorrectionTable_numEdges
  PROTO_PARAMS(( lefiCorrectionTable * this ));

EXTERN lefiCorrectionEdge *
lefiCorrectionTable_edge
  PROTO_PARAMS(( lefiCorrectionTable * this,
                 int index ));

#endif
