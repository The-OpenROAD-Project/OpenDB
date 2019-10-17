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
  this->polysAllocated_ = 2;
  this->numPolys_ = 0;
  this->polygons_ = ((struct lefiGeomPolygon **) lefMalloc(sizeof(struct lefiGeomPolygon *) * 2));
}

void 
lefiViaLayer_Destroy(lefiViaLayer * this)
{
  int     i;

  if (this->xl_) {
    lefFree((char *) (this->xl_));
    lefFree((char *) (this->yl_));
    lefFree((char *) (this->xh_));
    lefFree((char *) (this->yh_));
  }
  if (this->polygons_) {
    struct lefiGeomPolygon *geom;

    for (i = 0;
	 i < this->numPolys_;
	 i++) {
      geom = this->polygons_[i];
      lefFree((char *) (geom->x));
      lefFree((char *) (geom->y));
      lefFree((char *) (this->polygons_[i]));
    }
    lefFree((char *) (this->polygons_));
    this->polygons_ = 0;
    this->numPolys_ = 0;
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

void 
lefiViaLayer_addPoly(lefiViaLayer * this,
		     lefiGeometries * geom)
{
  if (this->numPolys_ == this->polysAllocated_) {
    int     i;

    struct lefiGeomPolygon **poly;

    this->polysAllocated_ = (this->polysAllocated_ == 0) ? 2 : this->polysAllocated_ * 2;
    poly = (struct lefiGeomPolygon **) lefMalloc(sizeof(struct lefiGeomPolygon *) * this->polysAllocated_);
    for (i = 0;
	 i < this->numPolys_;
	 i++) {
      poly[i] = this->polygons_[i];
    }
    if (this->polygons_)
      lefFree((char *) (this->polygons_));
    this->polygons_ = poly;
  }
  this->polygons_[this->numPolys_] = lefiGeometries_getPolygon(geom,
							       0);
  this->numPolys_ += 1;
}

int 
lefiViaLayer_numRects(lefiViaLayer * this)
{
  return this->numRects_;
}

int 
lefiViaLayer_numPolygons(lefiViaLayer * this)
{
  return this->numPolys_;
}

lefiViaLayer *
lefiViaLayer_clone(lefiViaLayer * this)
{
  lefiViaLayer *vl = (lefiViaLayer *) lefMalloc(sizeof(lefiViaLayer));

  int     i, j;

  vl->xl_ = (double *) lefMalloc(sizeof(double) * this->numRects_);
  vl->yl_ = (double *) lefMalloc(sizeof(double) * this->numRects_);
  vl->xh_ = (double *) lefMalloc(sizeof(double) * this->numRects_);
  vl->yh_ = (double *) lefMalloc(sizeof(double) * this->numRects_);
  vl->numRects_ = this->numRects_;
  vl->rectsAllocated_ = this->rectsAllocated_;
  for (i = 0;
       i < this->numRects_;
       i++) {
    vl->xl_[i] = this->xl_[i];
    vl->yl_[i] = this->yl_[i];
    vl->xh_[i] = this->xh_[i];
    vl->yh_[i] = this->yh_[i];
  }
  vl->numPolys_ = this->numPolys_;
  vl->polysAllocated_ = this->polysAllocated_;
  vl->polygons_ = (struct lefiGeomPolygon **) lefMalloc(sizeof(struct lefiGeomPolygon *) * this->polysAllocated_);
  for (i = 0;
       i < this->numPolys_;
       i++) {
    vl->polygons_[i] = (struct lefiGeomPolygon *) lefMalloc(sizeof(struct lefiGeomPolygon));
    vl->polygons_[i]->numPoints = this->polygons_[i]->numPoints;
    vl->polygons_[i]->x = (double *) lefMalloc(sizeof(double) * this->polygons_[i]->numPoints);
    vl->polygons_[i]->y = (double *) lefMalloc(sizeof(double) * this->polygons_[i]->numPoints);
    for (j = 0;
	 j < this->polygons_[i]->numPoints;
	 j++) {
      vl->polygons_[i]->x[j] = this->polygons_[i]->x[j];
      vl->polygons_[i]->y[j] = this->polygons_[i]->y[j];
    }
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

struct lefiGeomPolygon *
lefiViaLayer_getPolygon(const lefiViaLayer * this,
			int index)
{
  return this->polygons_[index];
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
  this->viaRuleName_ = 0;
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
  if (this->viaRuleName_)
    lefFree((char *) (this->viaRuleName_));
  if (this->botLayer_)
    lefFree((char *) (this->botLayer_));
  if (this->cutLayer_)
    lefFree((char *) (this->cutLayer_));
  if (this->topLayer_)
    lefFree((char *) (this->topLayer_));
  if (this->cutPattern_)
    lefFree((char *) (this->cutPattern_));
  this->propName_ = 0;
  this->propValue_ = 0;
  this->propDValue_ = 0;
  this->propType_ = 0;
  this->viaRuleName_ = 0;
  this->botLayer_ = 0;
  this->cutLayer_ = 0;
  this->topLayer_ = 0;
  this->cutPattern_ = 0;
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
  v->hasGenerated_ = this->hasGenerated_;
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
  v->layersAllocated_ = this->layersAllocated_;
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
  v->viaRuleName_ = 0;
  v->botLayer_ = 0;
  v->cutLayer_ = 0;
  v->topLayer_ = 0;
  v->cutPattern_ = 0;
  if (this->viaRuleName_)
    v->viaRuleName_ = strdup(this->viaRuleName_);
  v->xSize_ = this->xSize_;
  v->ySize_ = this->ySize_;
  if (this->botLayer_)
    v->botLayer_ = strdup(this->botLayer_);
  if (this->cutLayer_)
    v->cutLayer_ = strdup(this->cutLayer_);
  if (this->topLayer_)
    v->topLayer_ = strdup(this->topLayer_);
  v->xSpacing_ = this->xSpacing_;
  v->ySpacing_ = this->ySpacing_;
  v->xBotEnc_ = this->xBotEnc_;
  v->yBotEnc_ = this->yBotEnc_;
  v->xTopEnc_ = this->xTopEnc_;
  v->yTopEnc_ = this->yTopEnc_;
  v->numRows_ = this->numRows_;
  v->numCols_ = this->numCols_;
  v->xOffset_ = this->xOffset_;
  v->yOffset_ = this->yOffset_;
  v->xBotOs_ = this->xBotOs_;
  v->yBotOs_ = this->yBotOs_;
  v->xTopOs_ = this->xTopOs_;
  v->yTopOs_ = this->yTopOs_;
  if (this->cutPattern_)
    v->cutPattern_ = strdup(this->cutPattern_);
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
  this->hasGenerated_ = 0;
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
  if (this->viaRuleName_)
    lefFree(this->viaRuleName_);
  this->viaRuleName_ = 0;
  this->xSize_ = 0;
  this->ySize_ = 0;
  if (this->botLayer_)
    lefFree(this->botLayer_);
  if (this->cutLayer_)
    lefFree(this->cutLayer_);
  if (this->topLayer_)
    lefFree(this->topLayer_);
  this->botLayer_ = 0;
  this->cutLayer_ = 0;
  this->topLayer_ = 0;
  this->xSpacing_ = 0;
  this->ySpacing_ = 0;
  this->xBotEnc_ = 0;
  this->yBotEnc_ = 0;
  this->xTopEnc_ = 0;
  this->yTopEnc_ = 0;
  this->numRows_ = 0;
  this->numCols_ = 0;
  this->xOffset_ = 0;
  this->yOffset_ = 0;
  this->xBotOs_ = 0;
  this->yBotOs_ = 0;
  this->xTopOs_ = 0;
  this->yTopOs_ = 0;
  if (this->cutPattern_)
    lefFree(this->cutPattern_);
  this->cutPattern_ = 0;
}

void 
lefiVia_setName(lefiVia * this,
		const char *name,
		int viaType)
{
  int     len;

  lefiVia_clear(this);
  switch (viaType) {
  case 1:
    this->hasDefault_ = 1;
    break;
  case 2:
    this->hasGenerated_ = 1;
    break;
  }
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

void 
lefiVia_addPolyToLayer(lefiVia * this,
		       lefiGeometries * geom)
{
  lefiViaLayer_addPoly(this->layers_[this->numLayers_ - 1],
		       geom);
}

void 
lefiVia_setViaRule(lefiVia * this,
		   const char *viaRuleName,
		   double xSize,
		   double ySize,
		   const char *botLayer,
		   const char *cutLayer,
		   const char *topLayer,
		   double xCut,
		   double yCut,
		   double xBotEnc,
		   double yBotEnc,
		   double xTopEnc,
		   double yTopEnc)
{
  this->viaRuleName_ = strdup(viaRuleName);
  this->xSize_ = xSize;
  this->ySize_ = ySize;
  this->botLayer_ = strdup(botLayer);
  this->cutLayer_ = strdup(cutLayer);
  this->topLayer_ = strdup(topLayer);
  this->xSpacing_ = xCut;
  this->ySpacing_ = yCut;
  this->xBotEnc_ = xBotEnc;
  this->yBotEnc_ = yBotEnc;
  this->xTopEnc_ = xTopEnc;
  this->yTopEnc_ = yTopEnc;
}

void 
lefiVia_setRowCol(lefiVia * this,
		  int numRows,
		  int numCols)
{
  this->numRows_ = numRows;
  this->numCols_ = numCols;
}

void 
lefiVia_setOrigin(lefiVia * this,
		  double xOffset,
		  double yOffset)
{
  this->xOffset_ = xOffset;
  this->yOffset_ = yOffset;
}

void 
lefiVia_setOffset(lefiVia * this,
		  double xBot,
		  double yBot,
		  double xTop,
		  double yTop)
{
  this->xBotOs_ = xBot;
  this->yBotOs_ = yBot;
  this->xTopOs_ = xTop;
  this->yTopOs_ = yTop;
}

void 
lefiVia_setPattern(lefiVia * this,
		   const char *cutPattern)
{
  this->cutPattern_ = strdup(cutPattern);
}

int 
lefiVia_hasDefault(const lefiVia * this)
{
  return this->hasDefault_;
}

int 
lefiVia_hasGenerated(const lefiVia * this)
{
  return this->hasGenerated_;
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

/*
 * 5.6
 */ int 
lefiVia_hasViaRule(const lefiVia * this)
{
  return this->viaRuleName_ ? 1 : 0;
}

/*
 * 5.6
 */ int 
lefiVia_hasRowCol(const lefiVia * this)
{
  if (this->numRows_ != 0 || this->numCols_ != 0)
    return 1;
  return 0;
}

/*
 * 5.6
 */ int 
lefiVia_hasOrigin(const lefiVia * this)
{
  if (this->xOffset_ != 0 || this->yOffset_ != 0)
    return 1;
  return 0;
}

/*
 * 5.6
 */ int 
lefiVia_hasOffset(const lefiVia * this)
{
  if (this->xBotOs_ != 0 || this->yBotOs_ != 0 || this->xTopOs_ != 0 || this->yTopOs_ != 0)
    return 1;
  return 0;
}

/*
 * 5.6
 */ int 
lefiVia_hasCutPattern(const lefiVia * this)
{
  return this->cutPattern_ ? 1 : 0;
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

int 
lefiVia_numPolygons(const lefiVia * this,
		    int layerNum)
{
  lefiViaLayer *vl;

  if (layerNum < 0 || layerNum >= this->numLayers_) {
    lefiError("ERROR: bad layer number index");
    return 0;
  }
  vl = this->layers_[layerNum];
  return lefiViaLayer_numPolygons(vl);
}

struct lefiGeomPolygon 
lefiVia_getPolygon(const lefiVia * this,
		   int layerNum,
		   int polyNum)
{
  lefiViaLayer *vl;

  struct lefiGeomPolygon tempPoly;

  tempPoly.numPoints = 0;
  tempPoly.x = 0;
  tempPoly.y = 0;
  if (layerNum < 0 || layerNum >= this->numLayers_) {
    lefiError("ERROR: bad layer number index");
    return tempPoly;
  }
  vl = this->layers_[layerNum];
  return *(lefiViaLayer_getPolygon(vl,
				   polyNum));
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
 * 5.6
 */ const char *
lefiVia_viaRuleName(const lefiVia * this)
{
  return this->viaRuleName_;
}

double 
lefiVia_xCutSize(const lefiVia * this)
{
  return this->xSize_;
}

double 
lefiVia_yCutSize(const lefiVia * this)
{
  return this->ySize_;
}

const char *
lefiVia_botMetalLayer(const lefiVia * this)
{
  return this->botLayer_;
}

const char *
lefiVia_cutLayer(const lefiVia * this)
{
  return this->cutLayer_;
}

const char *
lefiVia_topMetalLayer(const lefiVia * this)
{
  return this->topLayer_;
}

double 
lefiVia_xCutSpacing(const lefiVia * this)
{
  return this->xSpacing_;
}

double 
lefiVia_yCutSpacing(const lefiVia * this)
{
  return this->ySpacing_;
}

double 
lefiVia_xBotEnc(const lefiVia * this)
{
  return this->xBotEnc_;
}

double 
lefiVia_yBotEnc(const lefiVia * this)
{
  return this->yBotEnc_;
}

double 
lefiVia_xTopEnc(const lefiVia * this)
{
  return this->xTopEnc_;
}

double 
lefiVia_yTopEnc(const lefiVia * this)
{
  return this->yTopEnc_;
}

int 
lefiVia_numCutRows(const lefiVia * this)
{
  return this->numRows_;
}

int 
lefiVia_numCutCols(const lefiVia * this)
{
  return this->numCols_;
}

double 
lefiVia_xOffset(const lefiVia * this)
{
  return this->xOffset_;
}

double 
lefiVia_yOffset(const lefiVia * this)
{
  return this->yOffset_;
}

double 
lefiVia_xBotOffset(const lefiVia * this)
{
  return this->xBotOs_;
}

double 
lefiVia_yBotOffset(const lefiVia * this)
{
  return this->yBotOs_;
}

double 
lefiVia_xTopOffset(const lefiVia * this)
{
  return this->xTopOs_;
}

double 
lefiVia_yTopOffset(const lefiVia * this)
{
  return this->yTopOs_;
}

const char *
lefiVia_cutPattern(const lefiVia * this)
{
  return this->cutPattern_;
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
