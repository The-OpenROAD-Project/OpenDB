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

#ifndef lefiMisc_h
#define lefiMisc_h

#include <stdio.h>
#include "lefiKRDefs.hpp"

class lefiSpacing {
public:
  lefiSpacing();
  void Init();

  void Destroy();
  ~lefiSpacing();

  lefiSpacing* clone();

  void set(const char* name1, const char* name2, double num, int hasStack);

  int hasStack() const;

  const char* name1() const;
  const char* name2() const;
  double distance() const;

  // Debug print
  void print(FILE* f) const;

protected:
  int name1Size_;
  int name2Size_;
  char* name1_;
  char* name2_;
  double distance_;
  int hasStack_;
};



class lefiIRDrop {
public:
  lefiIRDrop();
  void Init();

  void Destroy();
  ~lefiIRDrop();

  void clear();
  void setTableName(const char* name);
  void setValues(double name1, double name2);

  const char* name() const;
  double value1(int index) const;
  double value2(int index) const;

  int numValues() const;

  // Debug print
  void print(FILE* f) const;

protected:
  int nameSize_;
  int value1Size_;
  int value2Size_;
  int numValues_;
  int valuesAllocated_;
  char* name_;
  double* value1_;
  double* value2_;
};


class lefiMinFeature {
public:
  lefiMinFeature();
  void Init();

  void Destroy();
  ~lefiMinFeature();

  void set(double one, double two);

  double one() const;
  double two() const;

  // Debug print
  void print(FILE* f) const;

protected:
  double one_;
  double two_;
};


class lefiSite {
public:
  lefiSite();
  void Init();

  void Destroy();
  ~lefiSite();

  void setName(const char* name);
  void setClass(const char* cls);
  void setSize(double x, double y);
  void setXSymmetry();
  void setYSymmetry();
  void set90Symmetry();

  const char* name() const;
  int hasClass() const;
  const char* siteClass() const;
  double sizeX() const;
  double sizeY() const;
  int hasSize() const;
  int hasXSymmetry() const;
  int hasYSymmetry() const;
  int has90Symmetry() const;

  // Debug print
  void print(FILE* f) const;

protected:
  int nameSize_;
  char* name_;
  int hasClass_;
  char siteClass_[8];
  double sizeX_;
  double sizeY_;
  int hasSize_;
  int symmetry_;   // bit 0-x   bit 1-y   bit 2-90
};


class lefiSitePattern {
public:
  lefiSitePattern();
  void Init();

  void Destroy();
  ~lefiSitePattern();

  void set(const char* name, double x, double y, int orient,
	   double xStart, double yStart, double xStep, double yStep);

  const char* name() const;
  int orient() const;
  const char* orientStr() const;
  double x() const;
  double y() const;
  double xStart() const;
  double yStart() const;
  double xStep() const;
  double yStep() const;

  // Debug print
  void print(FILE* f) const;

protected:
  int nameSize_;
  char* name_;
  int orient_;
  double x_;
  double y_;
  double xStart_;
  double yStart_;
  double xStep_;
  double yStep_;
};


class lefiTrackPattern {
public:
  lefiTrackPattern();
  void Init();

  void Destroy();
  ~lefiTrackPattern();

  void clear();
  void set(const char* name, double start, int numTracks, double space);
  void addLayer(const char* name);

  const char* name() const;
  double start() const;
  int numTracks() const;
  double space() const;

  int numLayers() const;
  const char* layerName(int index) const;

  // Debug print
  void print(FILE* f) const;

protected:
  int nameSize_;
  char* name_;
  double start_;
  int numTracks_;
  double space_;

  int numLayers_;
  int layerAllocated_;
  char** layerNames_;
};


class lefiGcellPattern {
public:
  lefiGcellPattern();
  void Init();

  void Destroy();
  ~lefiGcellPattern();

  void set(const char* name, double start, int numCRs, double space);

  const char* name() const;
  double start() const;
  int numCRs() const;
  double space() const;

  // Debug print
  void print(FILE* f) const;

protected:
  int nameSize_;
  char* name_;
  double start_;
  int numCRs_;
  double space_;
};



class lefiAlias_itr {
public:
  lefiAlias_itr();
  void Init();

  void Destroy();
  ~lefiAlias_itr();

  int Next();
  const char* Key();
  const char* Data();

protected:
  int num_;
};


class lefiUseMinSpacing {
public:
  lefiUseMinSpacing();
  void Init();

  void Destroy();
  ~lefiUseMinSpacing();

  void set(const char* name, int value);

  const char* name() const;
  int   value() const;

  // Debug print
  void print(FILE* f) const;

protected:
  char* name_;
  int   value_;
};


// 5.5 for Maximum Stacked-via rule
class lefiMaxStackVia {
public:
  lefiMaxStackVia();
  void Init();

  void Destroy();
  ~lefiMaxStackVia();

  void clear();
  void setMaxStackVia(int value);
  void setMaxStackViaRange(const char* bottomLayer, const char* topLayer);

  int maxStackVia() const;
  int hasMaxStackViaRange() const;
  const char* maxStackViaBottomLayer() const;
  const char* maxStackViaTopLayer() const;

  // Debug print
  void print(FILE* f) const;

protected:
  int   value_;
  int   hasRange_;
  char* bottomLayer_;
  char* topLayer_;
};

#endif

