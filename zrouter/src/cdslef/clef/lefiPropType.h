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

#ifndef lefiPropType_h
#define lefiPropType_h
#include "lefiKRDefs.h"
#include <stdio.h>

/*
 * Struct holds the data type for one property, if the property is
 */

/*
 * either REAL or INTEGER.
 */

/*
 * name.
 */

/*
 * 'R' == "REAL", 'I' == "INTEGER"
 */

typedef struct lefiPropType_s {
  int             numProperties_;
  int             propertiesAllocated_;
  char          **propNames_;
  char           *propTypes_;
} lefiPropType;

EXTERN lefiPropType *
lefiPropType_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiPropType_Init
  PROTO_PARAMS(( lefiPropType * this ));

EXTERN void
lefiPropType_Destroy
  PROTO_PARAMS(( lefiPropType * this ));

EXTERN void
lefiPropType_Delete
  PROTO_PARAMS(( lefiPropType * this ));

EXTERN void
lefiPropType_setPropType
  PROTO_PARAMS(( lefiPropType * this,
                 const char *name,
                 const char type ));

EXTERN void
lefiPropType_Clear
  PROTO_PARAMS(( lefiPropType * this ));

EXTERN const char
lefiPropType_propType
  PROTO_PARAMS(( const lefiPropType * this,
                 char *name ));

EXTERN void
lefiPropType_bumpProps
  PROTO_PARAMS(( lefiPropType * this ));

#endif
