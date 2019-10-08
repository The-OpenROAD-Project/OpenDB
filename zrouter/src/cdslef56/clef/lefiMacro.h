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

#ifndef lefiMacro_h
#define lefiMacro_h
#include <stdio.h>
#include "lefiKRDefs.h"
#include "lefiMisc.h"

typedef struct lefiObstruction_s {
  lefiGeometries *geometries_;
} lefiObstruction;

EXTERN lefiObstruction *
lefiObstruction_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiObstruction_Init
  PROTO_PARAMS(( lefiObstruction * this ));

EXTERN void
lefiObstruction_Destroy
  PROTO_PARAMS(( lefiObstruction * this ));

EXTERN void
lefiObstruction_Delete
  PROTO_PARAMS(( lefiObstruction * this ));

EXTERN void
lefiObstruction_clear
  PROTO_PARAMS(( lefiObstruction * this ));

EXTERN void
lefiObstruction_setGeometries
  PROTO_PARAMS(( lefiObstruction * this,
                 lefiGeometries * g ));

EXTERN lefiGeometries *
lefiObstruction_geometries
  PROTO_PARAMS(( const lefiObstruction * this ));

EXTERN void
lefiObstruction_print
  PROTO_PARAMS(( const lefiObstruction * this,
                 FILE * f ));

/*
 * 5.5
 */

typedef struct lefiPinAntennaModel_s {
  char           *oxide_;
  int             hasReturn_;
  int             numAntennaGateArea_;
  int             antennaGateAreaAllocated_;
  double         *antennaGateArea_;
  char          **antennaGateAreaLayer_;
  int             numAntennaMaxAreaCar_;
  int             antennaMaxAreaCarAllocated_;
  double         *antennaMaxAreaCar_;
  char          **antennaMaxAreaCarLayer_;
  int             numAntennaMaxSideAreaCar_;
  int             antennaMaxSideAreaCarAllocated_;
  double         *antennaMaxSideAreaCar_;
  char          **antennaMaxSideAreaCarLayer_;
  int             numAntennaMaxCutCar_;
  int             antennaMaxCutCarAllocated_;
  double         *antennaMaxCutCar_;
  char          **antennaMaxCutCarLayer_;
} lefiPinAntennaModel;

EXTERN lefiPinAntennaModel *
lefiPinAntennaModel_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiPinAntennaModel_Delete
  PROTO_PARAMS(( lefiPinAntennaModel * this ));

EXTERN void
lefiPinAntennaModel_Init
  PROTO_PARAMS(( lefiPinAntennaModel * this ));

EXTERN void
lefiPinAntennaModel_clear
  PROTO_PARAMS(( lefiPinAntennaModel * this ));

EXTERN void
lefiPinAntennaModel_Destroy
  PROTO_PARAMS(( lefiPinAntennaModel * this ));

EXTERN void
lefiPinAntennaModel_setAntennaModel
  PROTO_PARAMS(( lefiPinAntennaModel * this,
                 int oxide ));

EXTERN void
lefiPinAntennaModel_addAntennaGateArea
  PROTO_PARAMS(( lefiPinAntennaModel * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPinAntennaModel_addAntennaMaxAreaCar
  PROTO_PARAMS(( lefiPinAntennaModel * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPinAntennaModel_addAntennaMaxSideAreaCar
  PROTO_PARAMS(( lefiPinAntennaModel * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPinAntennaModel_addAntennaMaxCutCar
  PROTO_PARAMS(( lefiPinAntennaModel * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPinAntennaModel_setAntennaReturnFlag
  PROTO_PARAMS(( lefiPinAntennaModel * this,
                 int flag ));

EXTERN int
lefiPinAntennaModel_hasAntennaGateArea
  PROTO_PARAMS(( const lefiPinAntennaModel * this ));

EXTERN int
lefiPinAntennaModel_hasAntennaMaxAreaCar
  PROTO_PARAMS(( const lefiPinAntennaModel * this ));

EXTERN int
lefiPinAntennaModel_hasAntennaMaxSideAreaCar
  PROTO_PARAMS(( const lefiPinAntennaModel * this ));

EXTERN int
lefiPinAntennaModel_hasAntennaMaxCutCar
  PROTO_PARAMS(( const lefiPinAntennaModel * this ));

EXTERN char *
lefiPinAntennaModel_antennaOxide
  PROTO_PARAMS(( const lefiPinAntennaModel * this ));

EXTERN int
lefiPinAntennaModel_numAntennaGateArea
  PROTO_PARAMS(( const lefiPinAntennaModel * this ));

EXTERN double
lefiPinAntennaModel_antennaGateArea
  PROTO_PARAMS(( const lefiPinAntennaModel * this,
                 int index ));

EXTERN const char *
lefiPinAntennaModel_antennaGateAreaLayer
  PROTO_PARAMS(( const lefiPinAntennaModel * this,
                 int index ));

EXTERN int
lefiPinAntennaModel_numAntennaMaxAreaCar
  PROTO_PARAMS(( const lefiPinAntennaModel * this ));

EXTERN double
lefiPinAntennaModel_antennaMaxAreaCar
  PROTO_PARAMS(( const lefiPinAntennaModel * this,
                 int index ));

EXTERN const char *
lefiPinAntennaModel_antennaMaxAreaCarLayer
  PROTO_PARAMS(( const lefiPinAntennaModel * this,
                 int index ));

EXTERN int
lefiPinAntennaModel_numAntennaMaxSideAreaCar
  PROTO_PARAMS(( const lefiPinAntennaModel * this ));

EXTERN double
lefiPinAntennaModel_antennaMaxSideAreaCar
  PROTO_PARAMS(( const lefiPinAntennaModel * this,
                 int index ));

EXTERN const char *
lefiPinAntennaModel_antennaMaxSideAreaCarLayer
  PROTO_PARAMS(( const lefiPinAntennaModel * this,
                 int index ));

EXTERN int
lefiPinAntennaModel_numAntennaMaxCutCar
  PROTO_PARAMS(( const lefiPinAntennaModel * this ));

EXTERN double
lefiPinAntennaModel_antennaMaxCutCar
  PROTO_PARAMS(( const lefiPinAntennaModel * this,
                 int index ));

EXTERN const char *
lefiPinAntennaModel_antennaMaxCutCarLayer
  PROTO_PARAMS(( const lefiPinAntennaModel * this,
                 int index ));

EXTERN int
lefiPinAntennaModel_hasReturn
  PROTO_PARAMS(( const lefiPinAntennaModel * this ));

/*
 * 5.5
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
 * 5.5
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
 * 5.5
 */

/*
 * 5.5 AntennaModel
 */

/*
 * save the current index of the antenna
 */

typedef struct lefiPin_s {
  int             nameSize_;
  char           *name_;
  char            hasLEQ_;
  char            hasDirection_;
  char            hasUse_;
  char            hasShape_;
  char            hasMustjoin_;
  char            hasOutMargin_;
  char            hasOutResistance_;
  char            hasInMargin_;
  char            hasPower_;
  char            hasLeakage_;
  char            hasMaxload_;
  char            hasMaxdelay_;
  char            hasCapacitance_;
  char            hasResistance_;
  char            hasPulldownres_;
  char            hasTieoffr_;
  char            hasVHI_;
  char            hasVLO_;
  char            hasRiseVoltage_;
  char            hasFallVoltage_;
  char            hasRiseThresh_;
  char            hasFallThresh_;
  char            hasRiseSatcur_;
  char            hasFallSatcur_;
  char            hasCurrentSource_;
  char            hasTables_;
  char            hasAntennasize_;
  char            hasRiseSlewLimit_;
  char            hasFallSlewLimit_;
  int             numForeigns_;
  int             foreignAllocated_;
  int            *hasForeignOrient_;
  int            *hasForeignPoint_;
  int            *foreignOrient_;
  double         *foreignX_;
  double         *foreignY_;
  char          **foreign_;
  int             LEQSize_;
  char           *LEQ_;
  int             mustjoinSize_;
  char           *mustjoin_;
  double          outMarginH_;
  double          outMarginL_;
  double          outResistanceH_;
  double          outResistanceL_;
  double          inMarginH_;
  double          inMarginL_;
  double          power_;
  double          leakage_;
  double          maxload_;
  double          maxdelay_;
  double          capacitance_;
  double          resistance_;
  double          pulldownres_;
  double          tieoffr_;
  double          VHI_;
  double          VLO_;
  double          riseVoltage_;
  double          fallVoltage_;
  double          riseThresh_;
  double          fallThresh_;
  double          riseSatcur_;
  double          fallSatcur_;
  int             lowTableSize_;
  char           *lowTable_;
  int             highTableSize_;
  char           *highTable_;
  double          riseSlewLimit_;
  double          fallSlewLimit_;
  int             numAntennaModel_;
  int             antennaModelAllocated_;
  int             curAntennaModelIndex_;
  lefiPinAntennaModel **antennaModel_;
  int             numAntennaSize_;
  int             antennaSizeAllocated_;
  double         *antennaSize_;
  char          **antennaSizeLayer_;
  int             numAntennaMetalArea_;
  int             antennaMetalAreaAllocated_;
  double         *antennaMetalArea_;
  char          **antennaMetalAreaLayer_;
  int             numAntennaMetalLength_;
  int             antennaMetalLengthAllocated_;
  double         *antennaMetalLength_;
  char          **antennaMetalLengthLayer_;
  int             numAntennaPartialMetalArea_;
  int             antennaPartialMetalAreaAllocated_;
  double         *antennaPartialMetalArea_;
  char          **antennaPartialMetalAreaLayer_;
  int             numAntennaPartialMetalSideArea_;
  int             antennaPartialMetalSideAreaAllocated_;
  double         *antennaPartialMetalSideArea_;
  char          **antennaPartialMetalSideAreaLayer_;
  int             numAntennaPartialCutArea_;
  int             antennaPartialCutAreaAllocated_;
  double         *antennaPartialCutArea_;
  char          **antennaPartialCutAreaLayer_;
  int             numAntennaDiffArea_;
  int             antennaDiffAreaAllocated_;
  double         *antennaDiffArea_;
  char          **antennaDiffAreaLayer_;
  char           *taperRule_;
  char           *netEpxr_;
  char           *ssPinName_;
  char           *gsPinName_;
  char            direction_[32];
  char            use_[12];
  char            shape_[12];
  char            currentSource_[12];
  int             numProperties_;
  int             propertiesAllocated_;
  char          **propNames_;
  char          **propValues_;
  double         *propNums_;
  char           *propTypes_;
  int             numPorts_;
  int             portsAllocated_;
  lefiGeometries **ports_;
} lefiPin;

EXTERN lefiPin *
lefiPin_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiPin_Init
  PROTO_PARAMS(( lefiPin * this ));

EXTERN void
lefiPin_Destroy
  PROTO_PARAMS(( lefiPin * this ));

EXTERN void
lefiPin_Delete
  PROTO_PARAMS(( lefiPin * this ));

EXTERN void
lefiPin_clear
  PROTO_PARAMS(( lefiPin * this ));

EXTERN void
lefiPin_bump
  PROTO_PARAMS(( lefiPin * this,
                 char **array,
                 int len,
                 int *size ));

EXTERN void
lefiPin_setName
  PROTO_PARAMS(( lefiPin * this,
                 const char *name ));

EXTERN void
lefiPin_addPort
  PROTO_PARAMS(( lefiPin * this,
                 lefiGeometries * g ));

EXTERN void
lefiPin_addForeign
  PROTO_PARAMS(( lefiPin * this,
                 const char *name,
                 int hasPnt,
                 double x,
                 double y,
                 int orient ));

EXTERN void
lefiPin_setLEQ
  PROTO_PARAMS(( lefiPin * this,
                 const char *name ));

EXTERN void
lefiPin_setDirection
  PROTO_PARAMS(( lefiPin * this,
                 const char *name ));

EXTERN void
lefiPin_setUse
  PROTO_PARAMS(( lefiPin * this,
                 const char *name ));

EXTERN void
lefiPin_setShape
  PROTO_PARAMS(( lefiPin * this,
                 const char *name ));

EXTERN void
lefiPin_setMustjoin
  PROTO_PARAMS(( lefiPin * this,
                 const char *name ));

EXTERN void
lefiPin_setOutMargin
  PROTO_PARAMS(( lefiPin * this,
                 double high,
                 double low ));

EXTERN void
lefiPin_setOutResistance
  PROTO_PARAMS(( lefiPin * this,
                 double high,
                 double low ));

EXTERN void
lefiPin_setInMargin
  PROTO_PARAMS(( lefiPin * this,
                 double high,
                 double low ));

EXTERN void
lefiPin_setPower
  PROTO_PARAMS(( lefiPin * this,
                 double power ));

EXTERN void
lefiPin_setLeakage
  PROTO_PARAMS(( lefiPin * this,
                 double current ));

EXTERN void
lefiPin_setMaxload
  PROTO_PARAMS(( lefiPin * this,
                 double capacitance ));

EXTERN void
lefiPin_setMaxdelay
  PROTO_PARAMS(( lefiPin * this,
                 double delayTime ));

EXTERN void
lefiPin_setCapacitance
  PROTO_PARAMS(( lefiPin * this,
                 double capacitance ));

EXTERN void
lefiPin_setResistance
  PROTO_PARAMS(( lefiPin * this,
                 double resistance ));

EXTERN void
lefiPin_setPulldownres
  PROTO_PARAMS(( lefiPin * this,
                 double resistance ));

EXTERN void
lefiPin_setTieoffr
  PROTO_PARAMS(( lefiPin * this,
                 double resistance ));

EXTERN void
lefiPin_setVHI
  PROTO_PARAMS(( lefiPin * this,
                 double voltage ));

EXTERN void
lefiPin_setVLO
  PROTO_PARAMS(( lefiPin * this,
                 double voltage ));

EXTERN void
lefiPin_setRiseVoltage
  PROTO_PARAMS(( lefiPin * this,
                 double voltage ));

EXTERN void
lefiPin_setFallVoltage
  PROTO_PARAMS(( lefiPin * this,
                 double voltage ));

EXTERN void
lefiPin_setRiseThresh
  PROTO_PARAMS(( lefiPin * this,
                 double capacitance ));

EXTERN void
lefiPin_setFallThresh
  PROTO_PARAMS(( lefiPin * this,
                 double capacitance ));

EXTERN void
lefiPin_setRiseSatcur
  PROTO_PARAMS(( lefiPin * this,
                 double current ));

EXTERN void
lefiPin_setFallSatcur
  PROTO_PARAMS(( lefiPin * this,
                 double current ));

EXTERN void
lefiPin_setCurrentSource
  PROTO_PARAMS(( lefiPin * this,
                 const char *name ));

EXTERN void
lefiPin_setTables
  PROTO_PARAMS(( lefiPin * this,
                 const char *highName,
                 const char *lowName ));

EXTERN void
lefiPin_setProperty
  PROTO_PARAMS(( lefiPin * this,
                 const char *name,
                 const char *value,
                 const char type ));

EXTERN void
lefiPin_setNumProperty
  PROTO_PARAMS(( lefiPin * this,
                 const char *name,
                 double d,
                 const char *value,
                 const char type ));

EXTERN void
lefiPin_addAntennaModel
  PROTO_PARAMS(( lefiPin * this,
                 int oxide ));

EXTERN void
lefiPin_addAntennaSize
  PROTO_PARAMS(( lefiPin * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPin_addAntennaMetalArea
  PROTO_PARAMS(( lefiPin * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPin_addAntennaMetalLength
  PROTO_PARAMS(( lefiPin * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPin_addAntennaPartialMetalArea
  PROTO_PARAMS(( lefiPin * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPin_addAntennaPartialMetalSideArea
  PROTO_PARAMS(( lefiPin * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPin_addAntennaGateArea
  PROTO_PARAMS(( lefiPin * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPin_addAntennaDiffArea
  PROTO_PARAMS(( lefiPin * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPin_addAntennaMaxAreaCar
  PROTO_PARAMS(( lefiPin * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPin_addAntennaMaxSideAreaCar
  PROTO_PARAMS(( lefiPin * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPin_addAntennaPartialCutArea
  PROTO_PARAMS(( lefiPin * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPin_addAntennaMaxCutCar
  PROTO_PARAMS(( lefiPin * this,
                 double value,
                 const char *layer ));

EXTERN void
lefiPin_setRiseSlewLimit
  PROTO_PARAMS(( lefiPin * this,
                 double value ));

EXTERN void
lefiPin_setFallSlewLimit
  PROTO_PARAMS(( lefiPin * this,
                 double value ));

EXTERN void
lefiPin_setTaperRule
  PROTO_PARAMS(( lefiPin * this,
                 const char *name ));

EXTERN void
lefiPin_setNetExpr
  PROTO_PARAMS(( lefiPin * this,
                 const char *name ));

EXTERN void
lefiPin_setSupplySensitivity
  PROTO_PARAMS(( lefiPin * this,
                 const char *pinName ));

EXTERN void
lefiPin_setGroundSensitivity
  PROTO_PARAMS(( lefiPin * this,
                 const char *pinName ));

EXTERN void
lefiPin_bumpProps
  PROTO_PARAMS(( lefiPin * this ));

EXTERN int
lefiPin_hasForeign
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasForeignOrient
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN int
lefiPin_hasForeignPoint
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN int
lefiPin_hasLEQ
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasDirection
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasUse
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasShape
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasMustjoin
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasOutMargin
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasOutResistance
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasInMargin
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasPower
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasLeakage
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasMaxload
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasMaxdelay
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasCapacitance
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasResistance
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasPulldownres
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasTieoffr
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasVHI
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasVLO
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasRiseVoltage
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasFallVoltage
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasRiseThresh
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasFallThresh
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasRiseSatcur
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasFallSatcur
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasCurrentSource
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasTables
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasAntennaSize
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasAntennaMetalArea
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasAntennaMetalLength
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasAntennaPartialMetalArea
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasAntennaPartialMetalSideArea
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasAntennaPartialCutArea
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasAntennaDiffArea
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasAntennaModel
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasTaperRule
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasRiseSlewLimit
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasFallSlewLimit
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasNetExpr
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasSupplySensitivity
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_hasGroundSensitivity
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN const char *
lefiPin_name
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_numPorts
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN lefiGeometries *
lefiPin_port
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN int
lefiPin_numForeigns
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN const char *
lefiPin_foreignName
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char *
lefiPin_taperRule
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_foreignOrient
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char *
lefiPin_foreignOrientStr
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN double
lefiPin_foreignX
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN double
lefiPin_foreignY
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char *
lefiPin_LEQ
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN const char *
lefiPin_direction
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN const char *
lefiPin_use
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN const char *
lefiPin_shape
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN const char *
lefiPin_mustjoin
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_outMarginHigh
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_outMarginLow
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_outResistanceHigh
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_outResistanceLow
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_inMarginHigh
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_inMarginLow
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_power
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_leakage
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_maxload
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_maxdelay
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_capacitance
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_resistance
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_pulldownres
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_tieoffr
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_VHI
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_VLO
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_riseVoltage
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_fallVoltage
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_riseThresh
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_fallThresh
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_riseSatcur
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_fallSatcur
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_riseSlewLimit
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_fallSlewLimit
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN const char *
lefiPin_currentSource
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN const char *
lefiPin_tableHighName
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN const char *
lefiPin_tableLowName
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_numAntennaSize
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_antennaSize
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char *
lefiPin_antennaSizeLayer
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN int
lefiPin_numAntennaMetalArea
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_antennaMetalArea
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char *
lefiPin_antennaMetalAreaLayer
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN int
lefiPin_numAntennaMetalLength
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_antennaMetalLength
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char *
lefiPin_antennaMetalLengthLayer
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN int
lefiPin_numAntennaPartialMetalArea
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_antennaPartialMetalArea
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char *
lefiPin_antennaPartialMetalAreaLayer
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN int
lefiPin_numAntennaPartialMetalSideArea
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_antennaPartialMetalSideArea
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char *
lefiPin_antennaPartialMetalSideAreaLayer
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN int
lefiPin_numAntennaPartialCutArea
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_antennaPartialCutArea
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char *
lefiPin_antennaPartialCutAreaLayer
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN int
lefiPin_numAntennaDiffArea
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN double
lefiPin_antennaDiffArea
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char *
lefiPin_antennaDiffAreaLayer
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char *
lefiPin_netExpr
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN const char *
lefiPin_supplySensitivity
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN const char *
lefiPin_groundSensitivity
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN int
lefiPin_numAntennaModel
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN lefiPinAntennaModel *
lefiPin_antennaModel
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN int
lefiPin_numProperties
  PROTO_PARAMS(( const lefiPin * this ));

EXTERN const char *
lefiPin_propName
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char *
lefiPin_propValue
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN double
lefiPin_propNum
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN const char
lefiPin_propType
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN int
lefiPin_propIsNumber
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN int
lefiPin_propIsString
  PROTO_PARAMS(( const lefiPin * this,
                 int index ));

EXTERN void
lefiPin_print
  PROTO_PARAMS(( const lefiPin * this,
                 FILE * f ));

/*
 * 5.6
 */

typedef struct lefiDensity_s {
  int             numLayers_;
  int             layersAllocated_;
  char          **layerName_;
  int            *numRects_;
  int            *rectsAllocated_;
  struct lefiGeomRect **rects_;
  double        **densityValue_;
} lefiDensity;

EXTERN lefiDensity *
lefiDensity_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiDensity_Init
  PROTO_PARAMS(( lefiDensity * this ));

EXTERN void
lefiDensity_Destroy
  PROTO_PARAMS(( lefiDensity * this ));

EXTERN void
lefiDensity_Delete
  PROTO_PARAMS(( lefiDensity * this ));

EXTERN void
lefiDensity_clear
  PROTO_PARAMS(( lefiDensity * this ));

EXTERN void
lefiDensity_addLayer
  PROTO_PARAMS(( lefiDensity * this,
                 const char *name ));

EXTERN void
lefiDensity_addRect
  PROTO_PARAMS(( lefiDensity * this,
                 double x1,
                 double y1,
                 double x2,
                 double y2,
                 double value ));

EXTERN int
lefiDensity_numLayer
  PROTO_PARAMS(( const lefiDensity * this ));

EXTERN char *
lefiDensity_layerName
  PROTO_PARAMS(( const lefiDensity * this,
                 int index ));

EXTERN int
lefiDensity_numRects
  PROTO_PARAMS(( const lefiDensity * this,
                 int index ));

EXTERN struct lefiGeomRect
lefiDensity_getRect
  PROTO_PARAMS(( const lefiDensity * this,
                 int index,
                 int rectIndex ));

EXTERN double
lefiDensity_densityValue
  PROTO_PARAMS(( const lefiDensity * this,
                 int index,
                 int rectIndex ));

EXTERN void
lefiDensity_print
  PROTO_PARAMS(( const lefiDensity * this,
                 FILE * f ));

/*
 * orient=-1 means no orient was specified.
 */

/*
 * Debug print
 */

/*
 * X=1  Y=2  R90=4  (can be combined)
 */

typedef struct lefiMacro_s {
  int             nameSize_;
  char           *name_;
  char            macroClass_[32];
  char            source_[12];
  int             generatorSize_;
  char           *generator_;
  char            hasClass_;
  char            hasGenerator_;
  char            hasGenerate_;
  char            hasPower_;
  char            hasOrigin_;
  char            hasSource_;
  char            hasEEQ_;
  char            hasLEQ_;
  char            hasSymmetry_;
  char            hasSiteName_;
  char            hasSize_;
  char            hasClockType_;
  char            isBuffer_;
  char            isInverter_;
  char           *EEQ_;
  int             EEQSize_;
  char           *LEQ_;
  int             LEQSize_;
  char           *gen1_;
  int             gen1Size_;
  char           *gen2_;
  int             gen2Size_;
  double          power_;
  double          originX_;
  double          originY_;
  double          sizeX_;
  double          sizeY_;
  int             numSites_;
  int             sitesAllocated_;
  lefiSitePattern **pattern_;
  int             numForeigns_;
  int             foreignAllocated_;
  int            *hasForeignOrigin_;
  int            *hasForeignPoint_;
  int            *foreignOrient_;
  double         *foreignX_;
  double         *foreignY_;
  char          **foreign_;
  int             siteNameSize_;
  char           *siteName_;
  char           *clockType_;
  int             clockTypeSize_;
  int             numProperties_;
  int             propertiesAllocated_;
  char          **propNames_;
  char          **propValues_;
  double         *propNums_;
  char           *propTypes_;
} lefiMacro;

EXTERN lefiMacro *
lefiMacro_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiMacro_Init
  PROTO_PARAMS(( lefiMacro * this ));

EXTERN void
lefiMacro_Destroy
  PROTO_PARAMS(( lefiMacro * this ));

EXTERN void
lefiMacro_Delete
  PROTO_PARAMS(( lefiMacro * this ));

EXTERN void
lefiMacro_clear
  PROTO_PARAMS(( lefiMacro * this ));

EXTERN void
lefiMacro_bump
  PROTO_PARAMS(( lefiMacro * this,
                 char **array,
                 int len,
                 int *size ));

EXTERN void
lefiMacro_setName
  PROTO_PARAMS(( lefiMacro * this,
                 const char *name ));

EXTERN void
lefiMacro_setGenerator
  PROTO_PARAMS(( lefiMacro * this,
                 const char *name ));

EXTERN void
lefiMacro_setGenerate
  PROTO_PARAMS(( lefiMacro * this,
                 const char *name1,
                 const char *name2 ));

EXTERN void
lefiMacro_setPower
  PROTO_PARAMS(( lefiMacro * this,
                 double d ));

EXTERN void
lefiMacro_setOrigin
  PROTO_PARAMS(( lefiMacro * this,
                 double x,
                 double y ));

EXTERN void
lefiMacro_setClass
  PROTO_PARAMS(( lefiMacro * this,
                 const char *name ));

EXTERN void
lefiMacro_setSource
  PROTO_PARAMS(( lefiMacro * this,
                 const char *name ));

EXTERN void
lefiMacro_setEEQ
  PROTO_PARAMS(( lefiMacro * this,
                 const char *name ));

EXTERN void
lefiMacro_setLEQ
  PROTO_PARAMS(( lefiMacro * this,
                 const char *name ));

EXTERN void
lefiMacro_setClockType
  PROTO_PARAMS(( lefiMacro * this,
                 const char *name ));

EXTERN void
lefiMacro_setProperty
  PROTO_PARAMS(( lefiMacro * this,
                 const char *name,
                 const char *value,
                 const char type ));

EXTERN void
lefiMacro_setNumProperty
  PROTO_PARAMS(( lefiMacro * this,
                 const char *name,
                 double d,
                 const char *value,
                 const char type ));

EXTERN void
lefiMacro_bumpProps
  PROTO_PARAMS(( lefiMacro * this ));

EXTERN void
lefiMacro_addForeign
  PROTO_PARAMS(( lefiMacro * this,
                 const char *name,
                 int hasPnt,
                 double x,
                 double y,
                 int orient ));

EXTERN void
lefiMacro_setXSymmetry
  PROTO_PARAMS(( lefiMacro * this ));

EXTERN void
lefiMacro_setYSymmetry
  PROTO_PARAMS(( lefiMacro * this ));

EXTERN void
lefiMacro_set90Symmetry
  PROTO_PARAMS(( lefiMacro * this ));

EXTERN void
lefiMacro_setSiteName
  PROTO_PARAMS(( lefiMacro * this,
                 const char *name ));

EXTERN void
lefiMacro_setSitePattern
  PROTO_PARAMS(( lefiMacro * this,
                 lefiSitePattern * p ));

EXTERN void
lefiMacro_setSize
  PROTO_PARAMS(( lefiMacro * this,
                 double x,
                 double y ));

EXTERN void
lefiMacro_setBuffer
  PROTO_PARAMS(( lefiMacro * this ));

EXTERN void
lefiMacro_setInverter
  PROTO_PARAMS(( lefiMacro * this ));

EXTERN int
lefiMacro_hasClass
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasGenerator
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasGenerate
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasPower
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasOrigin
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasEEQ
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasLEQ
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasSource
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasXSymmetry
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasYSymmetry
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_has90Symmetry
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasSiteName
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasSitePattern
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasSize
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasForeign
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_hasForeignOrigin
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN int
lefiMacro_hasForeignOrient
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN int
lefiMacro_hasForeignPoint
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN int
lefiMacro_hasClockType
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_isBuffer
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_isInverter
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_numSitePattern
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_numProperties
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN const char *
lefiMacro_propName
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN const char *
lefiMacro_propValue
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN double
lefiMacro_propNum
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN const char
lefiMacro_propType
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN int
lefiMacro_propIsNumber
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN int
lefiMacro_propIsString
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN const char *
lefiMacro_name
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN const char *
lefiMacro_macroClass
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN const char *
lefiMacro_generator
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN const char *
lefiMacro_EEQ
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN const char *
lefiMacro_LEQ
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN const char *
lefiMacro_source
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN const char *
lefiMacro_clockType
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN double
lefiMacro_originX
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN double
lefiMacro_originY
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN double
lefiMacro_power
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN void
lefiMacro_generate
  PROTO_PARAMS(( const lefiMacro * this,
                 char **name1,
                 char **name2 ));

EXTERN lefiSitePattern *
lefiMacro_sitePattern
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN const char *
lefiMacro_siteName
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN double
lefiMacro_sizeX
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN double
lefiMacro_sizeY
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_numForeigns
  PROTO_PARAMS(( const lefiMacro * this ));

EXTERN int
lefiMacro_foreignOrient
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN const char *
lefiMacro_foreignOrientStr
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN double
lefiMacro_foreignX
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN double
lefiMacro_foreignY
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN const char *
lefiMacro_foreignName
  PROTO_PARAMS(( const lefiMacro * this,
                 int index ));

EXTERN void
lefiMacro_print
  PROTO_PARAMS(( const lefiMacro * this,
                 FILE * f ));

/*
 * addSDF2Pins & addSDF1Pin are for 5.1
 */

/*
 * The following are for 5.1
 */

typedef struct lefiTiming_s {
  int             numFrom_;
  char          **from_;
  int             fromAllocated_;
  int             numTo_;
  char          **to_;
  int             toAllocated_;
  int             hasTransition_;
  int             hasDelay_;
  int             hasRiseSlew_;
  int             hasRiseSlew2_;
  int             hasFallSlew_;
  int             hasFallSlew2_;
  int             hasRiseIntrinsic_;
  int             hasFallIntrinsic_;
  int             hasRiseRS_;
  int             hasRiseCS_;
  int             hasFallRS_;
  int             hasFallCS_;
  int             hasUnateness_;
  int             hasFallAtt1_;
  int             hasRiseAtt1_;
  int             hasFallTo_;
  int             hasRiseTo_;
  int             hasStableTiming_;
  int             hasSDFonePinTrigger_;
  int             hasSDFtwoPinTrigger_;
  int             hasSDFcondStart_;
  int             hasSDFcondEnd_;
  int             hasSDFcond_;
  int             nowRise_;
  int             numOfAxisNumbers_;
  double         *axisNumbers_;
  int             axisNumbersAllocated_;
  int             numOfTableEntries_;
  int             tableEntriesAllocated_;
  double         *table_;

  /* three numbers per entry   */
  char           *delayRiseOrFall_;
  char           *delayUnateness_;
  double          delayTableOne_;
  double          delayTableTwo_;
  double          delayTableThree_;
  char           *transitionRiseOrFall_;
  char           *transitionUnateness_;
  double          transitionTableOne_;
  double          transitionTableTwo_;
  double          transitionTableThree_;
  double          riseIntrinsicOne_;
  double          riseIntrinsicTwo_;
  double          riseIntrinsicThree_;
  double          riseIntrinsicFour_;
  double          fallIntrinsicOne_;
  double          fallIntrinsicTwo_;
  double          fallIntrinsicThree_;
  double          fallIntrinsicFour_;
  double          riseSlewOne_;
  double          riseSlewTwo_;
  double          riseSlewThree_;
  double          riseSlewFour_;
  double          riseSlewFive_;
  double          riseSlewSix_;
  double          riseSlewSeven_;
  double          fallSlewOne_;
  double          fallSlewTwo_;
  double          fallSlewThree_;
  double          fallSlewFour_;
  double          fallSlewFive_;
  double          fallSlewSix_;
  double          fallSlewSeven_;
  double          riseRSOne_;
  double          riseRSTwo_;
  double          riseCSOne_;
  double          riseCSTwo_;
  double          fallRSOne_;
  double          fallRSTwo_;
  double          fallCSOne_;
  double          fallCSTwo_;
  char           *unateness_;
  double          riseAtt1One_;
  double          riseAtt1Two_;
  double          fallAtt1One_;
  double          fallAtt1Two_;
  double          fallToOne_;
  double          fallToTwo_;
  double          riseToOne_;
  double          riseToTwo_;
  double          stableSetup_;
  double          stableHold_;
  char           *stableRiseFall_;
  char           *SDFtriggerType_;
  char           *SDFfromTrigger_;
  char           *SDFtoTrigger_;
  double          SDFtriggerTableOne_;
  double          SDFtriggerTableTwo_;
  double          SDFtriggerTableThree_;
  char           *SDFcondStart_;
  char           *SDFcondEnd_;
  char           *SDFcond_;
} lefiTiming;

EXTERN lefiTiming *
lefiTiming_Create
  PROTO_PARAMS((  ));

EXTERN void
lefiTiming_Init
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN void
lefiTiming_Destroy
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN void
lefiTiming_Delete
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN void
lefiTiming_addRiseFall
  PROTO_PARAMS(( lefiTiming * this,
                 const char *risefall,
                 double one,
                 double two ));

EXTERN void
lefiTiming_addRiseFallVariable
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two ));

EXTERN void
lefiTiming_addRiseFallSlew
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two,
                 double three,
                 double four ));

EXTERN void
lefiTiming_addRiseFallSlew2
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two,
                 double three ));

EXTERN void
lefiTiming_setRiseRS
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two ));

EXTERN void
lefiTiming_setFallRS
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two ));

EXTERN void
lefiTiming_setRiseCS
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two ));

EXTERN void
lefiTiming_setFallCS
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two ));

EXTERN void
lefiTiming_setRiseAtt1
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two ));

EXTERN void
lefiTiming_setFallAtt1
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two ));

EXTERN void
lefiTiming_setRiseTo
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two ));

EXTERN void
lefiTiming_setFallTo
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two ));

EXTERN void
lefiTiming_addUnateness
  PROTO_PARAMS(( lefiTiming * this,
                 const char *typ ));

EXTERN void
lefiTiming_setStable
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two,
                 const char *typ ));

EXTERN void
lefiTiming_addTableEntry
  PROTO_PARAMS(( lefiTiming * this,
                 double one,
                 double two,
                 double three ));

EXTERN void
lefiTiming_addTableAxisNumber
  PROTO_PARAMS(( lefiTiming * this,
                 double one ));

EXTERN void
lefiTiming_addFromPin
  PROTO_PARAMS(( lefiTiming * this,
                 const char *name ));

EXTERN void
lefiTiming_addToPin
  PROTO_PARAMS(( lefiTiming * this,
                 const char *name ));

EXTERN void
lefiTiming_addDelay
  PROTO_PARAMS(( lefiTiming * this,
                 const char *risefall,
                 const char *unateness,
                 double one,
                 double two,
                 double three ));

EXTERN void
lefiTiming_addTransition
  PROTO_PARAMS(( lefiTiming * this,
                 const char *risefall,
                 const char *unateness,
                 double one,
                 double two,
                 double three ));

EXTERN void
lefiTiming_addSDF2Pins
  PROTO_PARAMS(( lefiTiming * this,
                 const char *trigType,
                 const char *fromTrig,
                 const char *toTrig,
                 double one,
                 double two,
                 double three ));

EXTERN void
lefiTiming_addSDF1Pin
  PROTO_PARAMS(( lefiTiming * this,
                 const char *trigType,
                 double one,
                 double two,
                 double three ));

EXTERN void
lefiTiming_setSDFcondStart
  PROTO_PARAMS(( lefiTiming * this,
                 const char *condStart ));

EXTERN void
lefiTiming_setSDFcondEnd
  PROTO_PARAMS(( lefiTiming * this,
                 const char *condEnd ));

EXTERN void
lefiTiming_setSDFcond
  PROTO_PARAMS(( lefiTiming * this,
                 const char *cond ));

EXTERN int
lefiTiming_hasData
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN void
lefiTiming_clear
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_numFromPins
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_fromPin
  PROTO_PARAMS(( lefiTiming * this,
                 int index ));

EXTERN int
lefiTiming_numToPins
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_toPin
  PROTO_PARAMS(( lefiTiming * this,
                 int index ));

EXTERN int
lefiTiming_hasTransition
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasDelay
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasRiseSlew
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasRiseSlew2
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasFallSlew
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasFallSlew2
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasRiseIntrinsic
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasFallIntrinsic
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_numOfAxisNumbers
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double *
lefiTiming_axisNumbers
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_numOfTableEntries
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN void
lefiTiming_tableEntry
  PROTO_PARAMS(( lefiTiming * this,
                 int num,
                 double *one,
                 double *two,
                 double *three ));

EXTERN const char *
lefiTiming_delayRiseOrFall
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_delayUnateness
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_delayTableOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_delayTableTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_delayTableThree
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_transitionRiseOrFall
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_transitionUnateness
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_transitionTableOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_transitionTableTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_transitionTableThree
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallIntrinsicOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallIntrinsicTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallIntrinsicThree
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallIntrinsicFour
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseIntrinsicOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseIntrinsicTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseIntrinsicThree
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseIntrinsicFour
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallSlewOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallSlewTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallSlewThree
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallSlewFour
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallSlewFive
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallSlewSix
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallSlewSeven
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseSlewOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseSlewTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseSlewThree
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseSlewFour
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseSlewFive
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseSlewSix
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseSlewSeven
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasRiseRS
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseRSOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseRSTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasRiseCS
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseCSOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseCSTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasFallRS
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallRSOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallRSTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasFallCS
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallCSOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallCSTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasUnateness
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_unateness
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasRiseAtt1
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseAtt1One
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseAtt1Two
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasFallAtt1
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallAtt1One
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallAtt1Two
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasFallTo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallToOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_fallToTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasRiseTo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseToOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_riseToTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasStableTiming
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_stableSetup
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_stableHold
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_stableRiseFall
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasSDFonePinTrigger
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasSDFtwoPinTrigger
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasSDFcondStart
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasSDFcondEnd
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN int
lefiTiming_hasSDFcond
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_SDFonePinTriggerType
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_SDFtwoPinTriggerType
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_SDFfromTrigger
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_SDFtoTrigger
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_SDFtriggerOne
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_SDFtriggerTwo
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN double
lefiTiming_SDFtriggerThree
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_SDFcondStart
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_SDFcondEnd
  PROTO_PARAMS(( lefiTiming * this ));

EXTERN const char *
lefiTiming_SDFcond
  PROTO_PARAMS(( lefiTiming * this ));

#endif
