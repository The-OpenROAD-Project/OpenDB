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
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "lefiVia.h"
#include "lefiDebug.h"
#include "lefiUtil.h"

/*
 * lefiViaLayer
 */ lefiViaLayer *
lefiViaLayer_CreateUninitialized()
{
  lefiViaLayer *this = (lefiViaLayer *) malloc(sizeof(lefiViaLayer));

  lefiViaLayer_Init(this);
  return (this);
}

void 
lefiViaLayer_Init(lefiViaLayer * this)
{
  this->name_ = 0;
  this->rectsAllocated_ = 2;
  this->numRects_ = 0;
  this->xl_ = (double *) lefMalloc(sizeof(double) * 2);
  this->yl_ = (double *) lefMalloc(sizeof(double) * 2);
  this->xh_ = (double *) lefMalloc(sizeof(double) * 2);
  this->yh_ = (double *) lefMalloc(sizeof(double) * 2);
}

void 
lefiViaLayer_Destroy(lefiViaLayer * this)
{
  if (this->xl_) {
    lefFree((char *) (this->xl_));
    lefFree((char *) (this->yl_));
    lefFree((char *) (this->xh_));
    lefFree((char *) (this->yh_));
  }
  lefFree(this->name_);
}

void
lefiViaLayer_Delete(lefiViaLayer * this)
{
  if (this == NULL)
    return;
  lefiViaLayer_Destroy(this);
  free(this);
}

void 
lefiViaLayer_setName(lefiViaLayer * this,
		     const char *name)
{
  int     len = strlen(name) + 1;

  this->name_ = (char *) lefMalloc(len);
  strcpy(this->name_, CASE(name));
}

void 
lefiViaLayer_addRect(lefiViaLayer * this,
		     double xl,
		     double yl,
		     double xh,
		     double yh)
{
  if (this->numRects_ == this->rectsAllocated_) {
    int     i;

    double *newxl;

    double *newyl;

    double *newxh;

    double *newyh;

    this->rectsAllocated_ = (this->rectsAllocated_ == 0) ? 2 : this->rectsAllocated_ * 2;
    newxl = (double *) lefMalloc(sizeof(double) * this->rectsAllocated_);
    newyl = (double *) lefMalloc(sizeof(double) * this->rectsAllocated_);
    newxh = (double *) lefMalloc(sizeof(double) * this->rectsAllocated_);
    newyh = (double *) lefMalloc(sizeof(double) * this->rectsAllocated_);
    for (i = 0;
	 i < this->numRects_;
	 i++) {
      newxl[i] = this->xl_[i];
      newyl[i] = this->yl_[i];
      newxh[i] = this->xh_[i];
      newyh[i] = this->yh_[i];
    }
    if (this->xl_) {
      lefFree((char *) (this->xl_));
      lefFree((char *) (this->yl_));
      lefFree((char *) (this->xh_));
      lefFree((char *) (this->yh_));
    }
    this->xl_ = newxl;
    this->yl_ = newyl;
    this->xh_ = newxh;
    this->yh_ = newyh;
  }
  this->xl_[this->numRects_] = xl;
  this->yl_[this->numRects_] = yl;
  this->xh_[this->numRects_] = xh;
  this->yh_[this->numRects_] = yh;
  this->numRects_ += 1;
}

int 
lefiViaLayer_numRects(lefiViaLayer * this)
{
  return this->numRects_;
}

lefiViaLayer *
lefiViaLayer_clone(lefiViaLayer * this)
{
  lefiViaLayer *vl = (lefiViaLayer *) lefMalloc(sizeof(lefiViaLayer));

  int     i;

  vl->xl_ = (double *) lefMalloc(sizeof(double) * this->numRects_);
  vl->yl_ = (double *) lefMalloc(sizeof(double) * this->numRects_);
  vl->xh_ = (double *) lefMalloc(sizeof(double) * this->numRects_);
  vl->yh_ = (double *) lefMalloc(sizeof(double) * this->numRects_);
  vl->numRects_ = this->numRects_;
  vl->rectsAllocated_ = this->numRects_;
  for (i = 0;
       i < this->numRects_;
       i++) {
    vl->xl_[i] = this->xl_[i];
    vl->yl_[i] = this->yl_[i];
    vl->xh_[i] = this->xh_[i];
    vl->yh_[i] = this->yh_[i];
  }
  vl->name_ = (char *) lefMalloc(strlen(this->name_) + 1);
  strcpy(vl->name_, this->name_);
  return vl;
}

char   *
lefiViaLayer_name(lefiViaLayer * this)
{
  return this->name_;
}

double 
lefiViaLayer_xl(lefiViaLayer * this,
		int index)
{
  if (index < 0 || index >= this->numRects_) {
    lefiError("index out of bounds");
    return 0;
  }
  return this->xl_[index];
}

double 
lefiViaLayer_yl(lefiViaLayer * this,
		int index)
{
  if (index < 0 || index >= this->numRects_) {
    lefiError("index out of bounds");
    return 0;
  }
  return this->yl_[index];
}

double 
lefiViaLayer_xh(lefiViaLayer * this,
		int index)
{
  if (index < 0 || index >= this->numRects_) {
    lefiError("index out of bounds");
    return 0;
  }
  return this->xh_[index];
}

double 
lefiViaLayer_yh(lefiViaLayer * this,
		int index)
{
  if (index < 0 || index >= this->numRects_) {
    lefiError("index out of bounds");
    return 0;
  }
  return this->yh_[index];
}

/*
 * lefiVia
 */ lefiVia *
lefiVia_CreateUninitialized()
{
  lefiVia *this = (lefiVia *) malloc(sizeof(lefiVia));

  lefiVia_Init(this);
  return (this);
}

void 
lefiVia_Init(lefiVia * this)
{
  this->nameSize_ = 16;
  this->name_ = (char *) lefMalloc(16);
  this->foreign_ = 0;
  this->numProps_ = 0;
  this->propsAllocated_ = 0;
  this->layersAllocated_ = 3;
  this->layers_ = (lefiViaLayer **) lefMalloc(sizeof(lefiViaLayer *) * 3);
  this->numLayers_ = 0;
  lefiVia_clear(this);
}

void 
lefiVia_Destroy(lefiVia * this)
{
  lefiVia_clear(this);
  lefFree(this->name_);
  lefFree((char *) (this->layers_));
  if (this->propName_)
    lefFree((char *) (this->propName_));
  if (this->propValue_)
    lefFree((char *) (this->propValue_));
  if (this->propDValue_)
    lefFree((char *) (this->propDValue_));
  if (this->propType_)
    lefFree((char *) (this->propType_));
}

void
lefiVia_Delete(lefiVia * this)
{
  if (this == NULL)
    return;
  lefiVia_Destroy(this);
  free(this);
}

lefiVia *
lefiVia_clone(lefiVia * this)
{
  int     i;

  lefiViaLayer *l;

  lefiVia *v = (lefiVia *) lefMalloc(sizeof(lefiVia));

  v->nameSize_ = strlen(this->name_) + 1;
  v->name_ = (char *) lefMalloc(v->nameSize_);
  strcpy(v->name_, this->name_);
  v->foreign_ = 0;
  if (lefiVia_hasForeign(this)) {
    lefiVia_setForeign(v,
		       this->foreign_,
		       lefiVia_hasForeignPnt(this), this->foreignX_, this->foreignY_, this->foreignOrient_);
  }
  v->hasDefault_ = this->hasDefault_;
  v->hasResistance_ = this->hasResistance_;
  v->hasForeignPnt_ = this->hasForeignPnt_;
  v->hasTopOfStack_ = this->hasTopOfStack_;
  v->numProps_ = this->numProps_;
  v->propsAllocated_ = this->numProps_;
  if (this->numProps_ > 0) {
    v->propName_ = (char **) lefMalloc(sizeof(char *) * this->numProps_);
    v->propValue_ = (char **) lefMalloc(sizeof(char *) * this->numProps_);
    v->propDValue_ = (double *) lefMalloc(sizeof(double) * this->numProps_);
    v->propType_ = (char *) lefMalloc(sizeof(char) * this->numProps_);
    for (i = 0;
	 i < this->numProps_;
	 i++) {
      v->propName_[i] = (char *) lefMalloc(strlen(this->propName_[i]) + 1);
      strcpy(v->propName_[i], this->propName_[i]);
      if (this->propValue_[i]) {
	v->propValue_[i] = (char *) lefMalloc(strlen(this->propValue_[i]) + 1);
	strcpy(v->propValue_[i], this->propValue_[i]);
      }
      else
	v->propValue_[i] = 0;
      v->propDValue_[i] = this->propDValue_[i];
      v->propType_[i] = this->propType_[i];
    }
  }
  else {
    v->propName_ = 0;
    v->propValue_ = 0;
    v->propDValue_ = 0;
    v->propType_ = 0;
  }
  v->layersAllocated_ = this->numLayers_;
  v->numLayers_ = this->numLayers_;
  v->layers_ = (lefiViaLayer **) lefMalloc(sizeof(lefiViaLayer *) * this->numLayers_);
  for (i = 0;
       i < this->numLayers_;
       i++) {
    l = this->layers_[i];
    v->layers_[i] = lefiViaLayer_clone(l);
  }
  v->resistance_ = this->resistance_;
  if (this->foreignOrient_ == 0)
    v->foreignOrient_ = -1;
  else
    v->foreignOrient_ = this->foreignOrient_;
  return v;
}

void 
lefiVia_clear(lefiVia * this)
{
  int     i;

  if (this->name_)
    *(this->name_) = '\0';
  if (this->foreign_)
    lefFree(this->foreign_);
  this->foreign_ = 0;
  this->hasDefault_ = 0;
  this->hasResistance_ = 0;
  this->hasForeignPnt_ = 0;
  this->hasTopOfStack_ = 0;
  this->foreignOrient_ = -1;
  for (i = 0;
       i < this->numProps_;
       i++) {
    lefFree(this->propName_[i]);
    this->propName_[i] = 0;
    if (this->propValue_[i])
      lefFree(this->propValue_[i]);
    this->propValue_[i] = 0;
    this->propType_[i] = ' ';
  }
  this->numProps_ = 0;
  for (i = 0;
       i < this->numLayers_;
       i++) {
    lefiViaLayer_Destroy(this->layers_[i]);
    lefFree((char *) (this->layers_[i]));
    this->layers_[i] = 0;
  }
  this->numLayers_ = 0;
}

void 
lefiVia_setName(lefiVia * this,
		const char *name,
		int deflt)
{
  int     len;

  lefiVia_clear(this);
  this->hasDefault_ = deflt ? 1 : 0;
  len = strlen(name) + 1;
  if (len > this->nameSize_) {
    lefFree(this->name_);
    this->name_ = (char *) lefMalloc(len);
    this->nameSize_ = len;
  }
  strcpy(this->name_, CASE(name));
}

void 
lefiVia_setResistance(lefiVia * this,
		      double num)
{
  this->hasResistance_ = 1;
  this->resistance_ = num;
}

void 
lefiVia_bumpProps(lefiVia * this)
{
  int     i;

  double *d;

  char  **n;

  char  **v;

  char   *t;

  if (this->propsAllocated_ == 0)
    this->propsAllocated_ = 2;
  else
    this->propsAllocated_ *= 2;
  d = (double *) lefMalloc(sizeof(double) * this->propsAllocated_);
  n = (char **) lefMalloc(sizeof(char *) * this->propsAllocated_);
  v = (char **) lefMalloc(sizeof(char *) * this->propsAllocated_);
  t = (char *) lefMalloc(sizeof(char) * this->propsAllocated_);
  for (i = 0;
       i < this->numProps_;
       i++) {
    d[i] = this->propDValue_[i];
    n[i] = this->propName_[i];
    v[i] = this->propValue_[i];
    t[i] = this->propType_[i];
  }
  if (this->numProps_ > 0) {
    lefFree((char *) (this->propDValue_));
    lefFree((char *) (this->propName_));
    lefFree((char *) (this->propValue_));
    lefFree((char *) (this->propType_));
  }
  this->propDValue_ = d;
  this->propName_ = n;
  this->propValue_ = v;
  this->propType_ = t;
}

void 
lefiVia_addProp(lefiVia * this,
		const char *name,
		const char *value,
		const char type)
{
  int     len = strlen(name) + 1;

  if (this->numProps_ == this->propsAllocated_)
    lefiVia_bumpProps(this);
  this->propName_[this->numProps_] = (char *) lefMalloc(len);
  strcpy(this->propName_[this->numProps_], CASE(name));
  len = strlen(value) + 1;
  this->propValue_[this->numProps_] = (char *) lefMalloc(len);
  strcpy(this->propValue_[this->numProps_], CASE(value));
  this->propDValue_[this->numProps_] = 0.0;
  this->propType_[this->numProps_] = type;
  this->numProps_ += 1;
}

void 
lefiVia_addNumProp(lefiVia * this,
		   const char *name,
		   double d,
		   const char *value,
		   const char type)
{
  int     len = strlen(name) + 1;

  if (this->numProps_ == this->propsAllocated_)
    lefiVia_bumpProps(this);
  this->propName_[this->numProps_] = (char *) lefMalloc(len);
  strcpy(this->propName_[this->numProps_], CASE(name));
  len = strlen(value) + 1;
  this->propValue_[this->numProps_] = (char *) lefMalloc(len);
  strcpy(this->propValue_[this->numProps_], CASE(value));
  this->propDValue_[this->numProps_] = d;
  this->propType_[this->numProps_] = type;
  this->numProps_ += 1;
}

void 
lefiVia_setForeign(lefiVia * this,
		   const char *name,
		   int hasPnt,
		   double x,
		   double y,
		   int orient)
{
  int     len = strlen(name) + 1;

  this->hasForeignPnt_ = hasPnt;
  this->foreignOrient_ = orient;
  this->foreignX_ = x;
  this->foreignY_ = y;
  this->foreign_ = (char *) lefMalloc(len);
  strcpy(this->foreign_, CASE(name));
}

void 
lefiVia_setTopOfStack(lefiVia * this)
{
  this->hasTopOfStack_ = 1;
}

void 
lefiVia_addLayer(lefiVia * this,
		 const char *name)
{
  lefiViaLayer *newl;

  if (this->numLayers_ == this->layersAllocated_) {
    int     i;

    lefiViaLayer **l;

    if (this->layersAllocated_ == 0)
      this->layersAllocated_ = 2;
    else
      this->layersAllocated_ *= 2;
    l = (lefiViaLayer **) lefMalloc(sizeof(lefiViaLayer *) * this->layersAllocated_);
    for (i = 0;
	 i < this->numLayers_;
	 i++) {
      l[i] = this->layers_[i];
    }
    lefFree((char *) (this->layers_));
    this->layers_ = l;
  }
  newl = (lefiViaLayer *) lefMalloc(sizeof(lefiViaLayer));
  lefiViaLayer_Init(newl);
  this->layers_[this->numLayers_] = newl;
  lefiViaLayer_setName(newl,
		       name);
  this->numLayers_ += 1;
}

void 
lefiVia_addRectToLayer(lefiVia * this,
		       double xl,
		       double yl,
		       double xh,
		       double yh)
{
  lefiViaLayer_addRect(this->layers_[this->numLayers_ - 1],
		       xl,
		       yl,
		       xh,
		       yh);
}

int 
lefiVia_hasDefault(const lefiVia * this)
{
  return this->hasDefault_;
}

int 
lefiVia_hasForeign(const lefiVia * this)
{
  return this->foreign_ ? 1 : 0;
}

int 
lefiVia_hasForeignPnt(const lefiVia * this)
{
  return this->hasForeignPnt_;
}

int 
lefiVia_hasForeignOrient(const lefiVia * this)
{
  return this->foreignOrient_ == -1 ? 0 : 1;
}

int 
lefiVia_hasProperties(const lefiVia * this)
{
  return this->numProps_ ? 1 : 0;
}

int 
lefiVia_hasResistance(const lefiVia * this)
{
  return this->hasResistance_;
}

int 
lefiVia_hasTopOfStack(const lefiVia * this)
{
  return this->hasTopOfStack_;
}

int 
lefiVia_numLayers(const lefiVia * this)
{
  return this->numLayers_;
}

char   *
lefiVia_layerName(const lefiVia * this,
		  int layerNum)
{
  lefiViaLayer *vl;

  if (layerNum < 0 || layerNum >= this->numLayers_) {
    lefiError("ERROR: bad layer number index");
    return 0;
  }
  vl = this->layers_[layerNum];
  return lefiViaLayer_name(vl);
}

int 
lefiVia_numRects(const lefiVia * this,
		 int layerNum)
{
  lefiViaLayer *vl;

  if (layerNum < 0 || layerNum >= this->numLayers_) {
    lefiError("ERROR: bad layer number index");
    return 0;
  }
  vl = this->layers_[layerNum];
  return lefiViaLayer_numRects(vl);
}

double 
lefiVia_xl(const lefiVia * this,
	   int layerNum,
	   int rectNum)
{
  lefiViaLayer *vl;

  if (layerNum < 0 || layerNum >= this->numLayers_) {
    lefiError("ERROR: bad layer number index");
    return 0;
  }
  vl = this->layers_[layerNum];
  return lefiViaLayer_xl(vl,
			 rectNum);
}

double 
lefiVia_yl(const lefiVia * this,
	   int layerNum,
	   int rectNum)
{
  lefiViaLayer *vl;

  if (layerNum < 0 || layerNum >= this->numLayers_) {
    lefiError("ERROR: bad layer number index");
    return 0;
  }
  vl = this->layers_[layerNum];
  return lefiViaLayer_yl(vl,
			 rectNum);
}

double 
lefiVia_xh(const lefiVia * this,
	   int layerNum,
	   int rectNum)
{
  lefiViaLayer *vl;

  if (layerNum < 0 || layerNum >= this->numLayers_) {
    lefiError("ERROR: bad layer number index");
    return 0;
  }
  vl = this->layers_[layerNum];
  return lefiViaLayer_xh(vl,
			 rectNum);
}

double 
lefiVia_yh(const lefiVia * this,
	   int layerNum,
	   int rectNum)
{
  lefiViaLayer *vl;

  if (layerNum < 0 || layerNum >= this->numLayers_) {
    lefiError("ERROR: bad layer number index");
    return 0;
  }
  vl = this->layers_[layerNum];
  return lefiViaLayer_yh(vl,
			 rectNum);
}

char   *
lefiVia_name(const lefiVia * this)
{
  return this->name_;
}

double 
lefiVia_resistance(const lefiVia * this)
{
  return this->resistance_;
}

/*
 * Given an index from 0 to numProperties()-1 return
 *//* information about that property. */ int 
lefiVia_numProperties(const lefiVia * this)
{
  return this->numProps_;
}

char   *
lefiVia_propName(const lefiVia * this,
		 int index)
{
  if (index < 0 || index >= this->numProps_) {
    lefiError("bad index for via property");
    return 0;
  }
  return this->propName_[index];
}

char   *
lefiVia_propValue(const lefiVia * this,
		  int index)
{
  if (index < 0 || index >= this->numProps_) {
    lefiError("bad index for via property");
    return 0;
  }
  return this->propValue_[index];
}

double 
lefiVia_propNumber(const lefiVia * this,
		   int index)
{
  if (index < 0 || index >= this->numProps_) {
    lefiError("bad index for via property");
    return 0;
  }
  return this->propDValue_[index];
}

char 
lefiVia_propType(const lefiVia * this,
		 int index)
{
  if (index < 0 || index >= this->numProps_) {
    lefiError("bad index for via property");
    return 0;
  }
  return this->propType_[index];
}

int 
lefiVia_propIsNumber(const lefiVia * this,
		     int index)
{
  if (index < 0 || index >= this->numProps_) {
    lefiError("bad index for via property");
    return 0;
  }
  return this->propDValue_[index] ? 1 : 0;
}

int 
lefiVia_propIsString(const lefiVia * this,
		     int index)
{
  if (index < 0 || index >= this->numProps_) {
    lefiError("bad index for via property");
    return 0;
  }
  return this->propDValue_[index] ? 0 : 1;
}

char   *
lefiVia_foreign(const lefiVia * this)
{
  return this->foreign_;
}

double 
lefiVia_foreignX(const lefiVia * this)
{
  return this->foreignX_;
}

double 
lefiVia_foreignY(const lefiVia * this)
{
  return this->foreignY_;
}

int 
lefiVia_foreignOrient(const lefiVia * this)
{
  return this->foreignOrient_;
}

char   *
lefiVia_foreignOrientStr(const lefiVia * this)
{
  return (lefiOrientStr(this->foreignOrient_));
}

/*
 * Debug print
 */ void 
lefiVia_print(const lefiVia * this,
	      FILE * f)
{
  int     i;

  int     h;

  fprintf(f, "Via %s:\n", lefiVia_name(this));
  if (lefiVia_hasDefault(this))
    fprintf(f, " DEFAULT\n");
  if (lefiVia_hasForeign(this)) {
    fprintf(f, " foreign %s", lefiVia_foreign(this));
    if (lefiVia_hasForeignPnt(this)) {
      fprintf(f, " %g,%g", lefiVia_foreignX(this), lefiVia_foreignY(this));
    }
    if (lefiVia_hasForeignOrient(this)) {
      fprintf(f, " orient %s", lefiVia_foreignOrientStr(this));
    }
    fprintf(f, "\n");
  }
  if (lefiVia_hasResistance(this))
    fprintf(f, " RESISTANCE %g\n", lefiVia_resistance(this));
  if (lefiVia_hasProperties(this)) {
    for (i = 0;
	 i < lefiVia_numProperties(this);
	 i++)
      if (lefiVia_propIsString(this,
			       i))
	fprintf(f, " PROP %s %s\n", lefiVia_propName(this,
						 i), lefiVia_propValue(this,
								       i));
      else
	fprintf(f, " PROP %s %g\n", lefiVia_propName(this,
						i), lefiVia_propNumber(this,
								       i));
  }
  for (i = 0;
       i < lefiVia_numLayers(this);
       i++) {
    fprintf(f, " LAYER %s\n", lefiVia_layerName(this,
						i));
    for (h = 0;
	 h < lefiVia_numRects(this,
			      i);
	 h++) {
      fprintf(f, " RECT %g,%g %g,%g\n", lefiVia_xl(this,
						   i,
						   h), lefiVia_yl(this,
								  i,
							h), lefiVia_xh(this,
								       i,
							h), lefiVia_yh(this,
								       i,
								       h));
    }
  }
}
