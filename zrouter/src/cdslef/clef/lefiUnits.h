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

#ifndef lefiUnits_h
#define lefiUnits_h
#include <stdio.h>
#include "lefiKRDefs.h"

/*
 * Debug print
 */

typedef struct lefiUnits_s {
  int             hasDatabase_;
  int             hasCapacitance_;
  int             hasResistance_;
  int             hasTime_;
  int             hasPower_;
  int             hasCurrent_;
  int             hasVoltage_;
  int             hasFrequency_;
  char           *databaseName_;
  double          databaseNumber_;
  double          capacitance_;
  double          resistance_;
  double          power_;
  double          time_;
  double          current_;
  double          voltage_;
  double          frequency_;
} lefiUnits;

EXTERN lefiUnits *
lefiUnits_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiUnits_Init
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN void
lefiUnits_Destroy
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN void
lefiUnits_Delete
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN void
lefiUnits_setDatabase
  PROTO_PARAMS(( lefiUnits * this,
                 const char *name,
                 double num ));

EXTERN void
lefiUnits_clear
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN void
lefiUnits_setTime
  PROTO_PARAMS(( lefiUnits * this,
                 double num ));

EXTERN void
lefiUnits_setCapacitance
  PROTO_PARAMS(( lefiUnits * this,
                 double num ));

EXTERN void
lefiUnits_setResistance
  PROTO_PARAMS(( lefiUnits * this,
                 double num ));

EXTERN void
lefiUnits_setPower
  PROTO_PARAMS(( lefiUnits * this,
                 double num ));

EXTERN void
lefiUnits_setCurrent
  PROTO_PARAMS(( lefiUnits * this,
                 double num ));

EXTERN void
lefiUnits_setVoltage
  PROTO_PARAMS(( lefiUnits * this,
                 double num ));

EXTERN void
lefiUnits_setFrequency
  PROTO_PARAMS(( lefiUnits * this,
                 double num ));

EXTERN int
lefiUnits_hasDatabase
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN int
lefiUnits_hasCapacitance
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN int
lefiUnits_hasResistance
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN int
lefiUnits_hasTime
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN int
lefiUnits_hasPower
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN int
lefiUnits_hasCurrent
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN int
lefiUnits_hasVoltage
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN int
lefiUnits_hasFrequency
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN const char *
lefiUnits_databaseName
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN double
lefiUnits_databaseNumber
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN double
lefiUnits_capacitance
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN double
lefiUnits_resistance
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN double
lefiUnits_time
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN double
lefiUnits_power
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN double
lefiUnits_current
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN double
lefiUnits_voltage
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN double
lefiUnits_frequency
  PROTO_PARAMS(( lefiUnits * this ));

EXTERN void
lefiUnits_print
  PROTO_PARAMS(( lefiUnits * this,
                 FILE * f ));

#endif
