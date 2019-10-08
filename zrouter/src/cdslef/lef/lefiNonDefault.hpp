/*
 *     This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 *  Distribution,  Product Version 5.5, and is subject to the Cadence
 *  LEF/DEF Open Source License Agreement.   Your  continued  use  of
 *  this file constitutes your acceptance of the terms of the LEF/DEF
 *  Open Source License and an agreement to abide by its  terms.   If
 *  you  don't  agree  with  this, you must remove this and any other
 *  files which are part of the distribution and  destroy any  copies
 *  made.
 *
 *     For updates, support, or to become part of the LEF/DEF Community,
 *  check www.openeda.org for details.
 */

#ifndef lefiNonDefault_h
#define lefiNonDefault_h

#include <stdio.h>
#include "lefiKRDefs.hpp"
#include "lefiVia.hpp"
#include "lefiMisc.hpp"

class lefiNonDefault {
public:
  lefiNonDefault();
  void Init();

  void Destroy();
  ~lefiNonDefault();

  void setName(const char* name);
  void addLayer(const char* name);
  void addWidth(double num);
  void addWireExtension(double num);
  void addSpacing(double num);
  void addSpacingRule(lefiSpacing* s);
  void addResistance(double num);
  void addCapacitance(double num);
  void addEdgeCap(double num);
  void addViaRule(lefiVia* v);
  void end();
  void clear();
  void addProp(const char* name, const char* value, const char type);
  void addNumProp(const char* name, const double d,
                  const char* value, const char type);

  const char* name() const;

  int numProps() const;
  const char*  propName(int index) const;
  const char*  propValue(int index) const;
  double propNumber(int index) const;
  const char   propType(int index) const;
  int    propIsNumber(int index) const;
  int    propIsString(int index) const;
  
  // A non default rule can have one or more layers.
  // The layer information is kept in an array.
  int numLayers() const ;
  const char* layerName(int index) const ;
  int hasLayerWidth(int index) const; double layerWidth(int index) const ;
  int hasLayerSpacing(int index) const ;
  double layerSpacing(int index) const ;
  int hasLayerWireExtension(int index) const ;
  double layerWireExtension(int index) const ;
  int hasLayerResistance(int index) const;
  double layerResistance(int index) const;
  int hasLayerCapacitance(int index) const;
  double layerCapacitance(int index) const;
  int hasLayerEdgeCap(int index) const;
  double layerEdgeCap(int index) const;

  // A non default rule can have one or more vias.
  // These routines return the via info.
  int numVias() const ;
  lefiVia* viaRule(int index) const ;

  // A non default rule can have one or more spacing rules.
  // These routines return the that info.
  int numSpacingRules() const ;
  lefiSpacing* spacingRule(int index) const ;

  // Debug print
  void print(FILE* f);

protected:
  void* oldViaCbk_;     // save the users' functions.
  void* oldSpacingCbk_;     // save the users' functions.
  void* oldSpacingBeginCbk_;
  void* oldSpacingEndCbk_;
  void* oldUserData_;

  int nameSize_;
  char* name_;

  // Layer information
  int numLayers_;
  int layersAllocated_;
  char** layerName_;
  double* width_;
  double* spacing_;
  double* wireExtension_;
  char* hasWidth_;
  char* hasSpacing_;
  char* hasWireExtension_;

  // 5.4
  double* resistance_;
  double* capacitance_;
  double* edgeCap_;
  char*   hasResistance_;
  char*   hasCapacitance_;
  char*   hasEdgeCap_;

  int numVias_;
  int allocatedVias_;
  lefiVia** viaRules_;

  int numSpacing_;
  int allocatedSpacing_;
  lefiSpacing** spacingRules_;

  int numProps_;
  int propsAllocated_;
  char**  names_;
  char**  values_;
  double* dvalues_;
  char*   types_;
};

#endif
