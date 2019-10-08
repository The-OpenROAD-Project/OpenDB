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

#ifndef lefiProp_h
#define lefiProp_h
#include "lefiKRDefs.h"
#include <stdio.h>

/*
 * Struct holds the data for one property.
 */

/*
 * either I:integer R:real S:string Q:quotedstring
 */

/*
 * N:property name is not defined in the property definition section
 */

/*
 * "design" "net" "macro" ...
 */

/*
 * name.
 */

/*
 * allocated size of name.
 */

/*
 * either 0:NO or 1:YES.
 */

/*
 * either 0:NO or 1:YES.
 */

/*
 * either I:integer R:real S:string Q:quotedstring.
 */

/*
 * N:property name is not defined.
 */

/*
 * if it is a string the data is here.
 */

/*
 * allocated size of stringData.
 */

/*
 * if it has a range the numbers are here.
 */

/*
 * if it is a real or int the number is here.
 */

typedef struct lefiProp_s {
  char           *propType_;
  char           *propName_;
  int             nameSize_;
  char            hasRange_;
  char            hasNumber_;
  char            hasNameMapString_;
  char            dataType_;
  char           *stringData_;
  int             stringLength_;
  double          left_, right_;
  double          d_;
} lefiProp;

EXTERN lefiProp *
lefiProp_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiProp_Init
  PROTO_PARAMS(( lefiProp * this ));

EXTERN void
lefiProp_Destroy
  PROTO_PARAMS(( lefiProp * this ));

EXTERN void
lefiProp_Delete
  PROTO_PARAMS(( lefiProp * this ));

EXTERN void
lefiProp_setPropType
  PROTO_PARAMS(( lefiProp * this,
                 const char *typ,
                 const char *string ));

EXTERN void
lefiProp_setRange
  PROTO_PARAMS(( lefiProp * this,
                 double left,
                 double right ));

EXTERN void
lefiProp_setNumber
  PROTO_PARAMS(( lefiProp * this,
                 double num ));

EXTERN void
lefiProp_setPropInteger
  PROTO_PARAMS(( lefiProp * this ));

EXTERN void
lefiProp_setPropReal
  PROTO_PARAMS(( lefiProp * this ));

EXTERN void
lefiProp_setPropString
  PROTO_PARAMS(( lefiProp * this ));

EXTERN void
lefiProp_setPropQString
  PROTO_PARAMS(( lefiProp * this,
                 const char *string ));

EXTERN void
lefiProp_setPropNameMapString
  PROTO_PARAMS(( lefiProp * this,
                 const char *string ));

EXTERN void
lefiProp_clear
  PROTO_PARAMS(( lefiProp * this ));

EXTERN const char *
lefiProp_string
  PROTO_PARAMS(( const lefiProp * this ));

EXTERN const char *
lefiProp_propType
  PROTO_PARAMS(( const lefiProp * this ));

EXTERN const char *
lefiProp_propName
  PROTO_PARAMS(( const lefiProp * this ));

EXTERN char
lefiProp_dataType
  PROTO_PARAMS(( const lefiProp * this ));

EXTERN int
lefiProp_hasNumber
  PROTO_PARAMS(( const lefiProp * this ));

EXTERN int
lefiProp_hasRange
  PROTO_PARAMS(( const lefiProp * this ));

EXTERN int
lefiProp_hasString
  PROTO_PARAMS(( const lefiProp * this ));

EXTERN int
lefiProp_hasNameMapString
  PROTO_PARAMS(( const lefiProp * this ));

EXTERN double
lefiProp_number
  PROTO_PARAMS(( const lefiProp * this ));

EXTERN double
lefiProp_left
  PROTO_PARAMS(( const lefiProp * this ));

EXTERN double
lefiProp_right
  PROTO_PARAMS(( const lefiProp * this ));

EXTERN void
lefiProp_bumpSize
  PROTO_PARAMS(( lefiProp * this,
                 int size ));

EXTERN void
lefiProp_bumpName
  PROTO_PARAMS(( lefiProp * this,
                 int size ));

EXTERN void
lefiProp_print
  PROTO_PARAMS(( const lefiProp * this,
                 FILE * f ));

#endif
