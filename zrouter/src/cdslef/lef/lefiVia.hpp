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

#ifndef lefiVia_h
#define lefiVia_h

#include <stdio.h>
#include "lefiKRDefs.hpp"

class lefiViaLayer {
public:
  lefiViaLayer();
  void Init();

  void Destroy();
  ~lefiViaLayer();

  void setName(const char* name);
  void addRect(double xl, double yl, double xh, double yh);

  lefiViaLayer* clone();

  int numRects();
  char* name();
  double xl(int index);
  double yl(int index);
  double xh(int index);
  double yh(int index);

protected:
  char* name_;
  int numRects_;
  int rectsAllocated_;
  double* xl_;
  double* yl_;
  double* xh_;
  double* yh_;
};



class lefiVia {
public:
  lefiVia();
  void Init();

  void Destroy();
  ~lefiVia();

  void clear();

  // setName calls clear to init
  // deflt=0 no default specified
  // deflt=1 default specified in lef file
  void setName(const char* name, int deflt);

  void setResistance(double num);
  void addProp(const char* name, const char* value, const char type);
  void addNumProp(const char* name, double d, const char* value,
                  const char type);

  // orient=-1 means no orient was specified.
  void setForeign(const char* name, int hasPnt,
	 double x, double y, int orient);
  void setTopOfStack();

  void addLayer(const char* name);
  void addRectToLayer(double xl, double yl, double xh, double yh);
  void bumpProps();

  /* make a new one */
  lefiVia* clone();

  int hasDefault() const ;
  int hasForeign() const ;
  int hasForeignPnt() const ;
  int hasForeignOrient() const ;
  int hasProperties() const ;
  int hasResistance() const ;
  int hasTopOfStack() const ;

  int numLayers() const;
  char* layerName(int layerNum) const;
  int numRects(int layerNum) const;
  double xl(int layerNum, int rectNum) const;
  double yl(int layerNum, int rectNum) const;
  double xh(int layerNum, int rectNum) const;
  double yh(int layerNum, int rectNum) const;

  char* name() const ;
  double resistance() const ;

  // Given an index from 0 to numProperties()-1 return
  // information about that property.
  int numProperties() const ;
  char* propName(int index) const;
  char* propValue(int index) const;
  double propNumber(int index) const;
  char  propType(int index) const;
  int propIsNumber(int index) const;
  int propIsString(int index) const;
  char* foreign() const;
  double foreignX() const;
  double foreignY() const;
  int foreignOrient() const;
  char* foreignOrientStr() const;

  // Debug print
  void print(FILE* f) const;

protected:
  char* name_;
  int nameSize_;

  int hasDefault_;
  int hasResistance_;
  int hasForeignPnt_;
  int hasTopOfStack_;

  int numProps_;
  int propsAllocated_;
  char** propName_;
  // The prop value is stored in the propValue_ or the propDValue_.
  // If it is a string it is in propValue_.  If it is a number,
  // then propValue_ is NULL and it is stored in propDValue_;
  char** propValue_;
  double* propDValue_;
  char*   propType_;

  int numLayers_;
  int layersAllocated_;
  lefiViaLayer** layers_;

  double resistance_;

  char* foreign_;
  double foreignX_;
  double foreignY_;
  int foreignOrient_;
};

#endif
