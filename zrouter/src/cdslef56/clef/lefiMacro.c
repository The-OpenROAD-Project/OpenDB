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
#include <string.h>
#include <stdlib.h>
#include "lex.h"
#include "lefiMacro.h"
#include "lefiMisc.h"
#include "lefiDebug.h"
#include "lefiUtil.h"

/*
 * lefiObstruction
 */ lefiObstruction *
lefiObstruction_CreateUninitialized()
{
  lefiObstruction *this = (lefiObstruction *) malloc(sizeof(lefiObstruction));

  lefiObstruction_Init(this);
  return (this);
}

void 
lefiObstruction_Init(lefiObstruction * this)
{
  this->geometries_ = 0;
}

void
lefiObstruction_Delete(lefiObstruction * this)
{
  if (this == NULL)
    return;
  lefiObstruction_Destroy(this);
  free(this);
}

void 
lefiObstruction_Destroy(lefiObstruction * this)
{
  lefiObstruction_clear(this);
}

void 
lefiObstruction_clear(lefiObstruction * this)
{
  if (this->geometries_) {
    lefiGeometries_Destroy(this->geometries_);
    lefFree((char *) (this->geometries_));
  }
  this->geometries_ = 0;
}

void 
lefiObstruction_setGeometries(lefiObstruction * this,
			      lefiGeometries * g)
{
  lefiObstruction_clear(this);
  this->geometries_ = g;
}

lefiGeometries *
lefiObstruction_geometries(const lefiObstruction * this)
{
  return this->geometries_;
}

void 
lefiObstruction_print(const lefiObstruction * this,
		      FILE * f)
{
  lefiGeometries *g;

  fprintf(f, " Obstruction\n");
  g = this->geometries_;
  lefiGeometries_print(g,
		       f);
}

/*
 * lefiDensity
 */ lefiDensity *
lefiDensity_CreateUninitialized()
{
  lefiDensity *this = (lefiDensity *) malloc(sizeof(lefiDensity));

  lefiDensity_Init(this);
  return (this);
}

void 
lefiDensity_Init(lefiDensity * this)
{
  this->numLayers_ = 0;
  this->layersAllocated_ = 0;
}

void
lefiDensity_Delete(lefiDensity * this)
{
  if (this == NULL)
    return;
  lefiDensity_Destroy(this);
  free(this);
}

void 
lefiDensity_Destroy(lefiDensity * this)
{
  lefiDensity_clear(this);
}

void 
lefiDensity_clear(lefiDensity * this)
{
  int     i;

  for (i = 0;
       i < this->numLayers_;
       i++) {
    lefFree(this->layerName_[i]);
    lefFree((char *) this->rects_[i]);
    lefFree((char *) this->densityValue_[i]);
    this->numRects_[i] = 0;
    this->rectsAllocated_[i] = 0;
  }
  lefFree(this->layerName_);
  this->layerName_ = 0;
  this->numLayers_ = 0;
  this->layersAllocated_ = 0;
}

void 
lefiDensity_addLayer(lefiDensity * this,
		     const char *name)
{
  if (this->numLayers_ == this->layersAllocated_) {
    int     i;

    char  **ln;

    int    *nr;

    int    *ra;

    struct lefiGeomRect **rs;

    double **dv;

    this->layersAllocated_ = (this->layersAllocated_ == 0) ? 2 : this->layersAllocated_ * 2;
    ln = (char **) lefMalloc(sizeof(char *) * this->layersAllocated_);
    nr = (int *) lefMalloc(sizeof(int) * this->layersAllocated_);
    ra = (int *) lefMalloc(sizeof(int) * this->layersAllocated_);
    rs = (struct lefiGeomRect **) lefMalloc(sizeof(struct lefiGeomRect *) * this->layersAllocated_);
    dv = (double **) lefMalloc(sizeof(double *) * this->layersAllocated_);
    for (i = 0;
	 i < this->numLayers_;
	 i++) {
      ln[i] = this->layerName_[i];
      nr[i] = this->numRects_[i];
      ra[i] = this->rectsAllocated_[i];
      rs[i] = this->rects_[i];
      dv[i] = this->densityValue_[i];
    }
    lefFree((char *) (this->layerName_));
    lefFree((char *) (this->numRects_));
    lefFree((char *) (this->rectsAllocated_));
    lefFree((char *) (this->rects_));
    lefFree((char *) (this->densityValue_));
    this->layerName_ = ln;
    this->numRects_ = nr;
    this->rectsAllocated_ = ra;
    this->rects_ = rs;
    this->densityValue_ = dv;
  }
  this->layerName_[this->numLayers_] = strdup(name);
  this->numRects_[this->numLayers_] = 0;
  this->rectsAllocated_[this->numLayers_] = 0;
  this->numLayers_ += 1;
}

void 
lefiDensity_addRect(lefiDensity * this,
		    double x1,
		    double y1,
		    double x2,
		    double y2,
		    double value)
{
  struct lefiGeomRect *rs, *ors;

  double *dv, *odv;

  int     i;

  struct lefiGeomRect p;

  if (this->numRects_[this->numLayers_ - 1] == this->rectsAllocated_[this->numLayers_ - 1]) {
    this->rectsAllocated_[this->numLayers_ - 1] = (this->rectsAllocated_[this->numLayers_ - 1] == 0) ? 2 : this->rectsAllocated_[this->numLayers_ - 1] * 2;
    rs = (struct lefiGeomRect *) lefMalloc(sizeof(struct lefiGeomRect) * this->rectsAllocated_[this->numLayers_ - 1]);
    dv = (double *) lefMalloc(sizeof(double) * this->rectsAllocated_[this->numLayers_ - 1]);
    ors = this->rects_[this->numLayers_ - 1];
    odv = this->densityValue_[this->numLayers_ - 1];
    for (i = 0;
	 i < this->numRects_[this->numLayers_ - 1];
	 i++) {
      rs[i] = ors[i];
      dv[i] = odv[i];
    }
    if (this->numRects_[this->numLayers_ - 1] > 0) {
      lefFree((char *) this->rects_[this->numLayers_ - 1]);
      lefFree((char *) this->densityValue_[this->numLayers_ - 1]);
    }
    this->rects_[this->numLayers_ - 1] = rs;
    this->densityValue_[this->numLayers_ - 1] = dv;
  }
  else {
    rs = this->rects_[this->numLayers_ - 1];
    dv = this->densityValue_[this->numLayers_ - 1];
  }
  p.xl = x1;
  p.yl = y1;
  p.xh = x2;
  p.yh = y2;
  rs[this->numRects_[this->numLayers_ - 1]] = p;
  dv[this->numRects_[this->numLayers_ - 1]] = value;
  this->numRects_[this->numLayers_ - 1] += 1;
}

int 
lefiDensity_numLayer(const lefiDensity * this)
{
  return this->numLayers_;
}

char   *
lefiDensity_layerName(const lefiDensity * this,
		      int index)
{
  return this->layerName_[index];
}

int 
lefiDensity_numRects(const lefiDensity * this,
		     int index)
{
  return this->numRects_[index];
}

struct lefiGeomRect 
lefiDensity_getRect(const lefiDensity * this,
		    int index,
		    int rectIndex)
{
  struct lefiGeomRect *rs;

  rs = this->rects_[index];
  return rs[rectIndex];
}

double 
lefiDensity_densityValue(const lefiDensity * this,
			 int index,
			 int rectIndex)
{
  double *dv;

  dv = this->densityValue_[index];
  return dv[rectIndex];
}

void 
lefiDensity_print(const lefiDensity * this,
		  FILE * f)
{
  int     i, j;

  fprintf(f, " DENSITY\n");
  for (i = 0;
       i < this->numLayers_;
       i++) {
    fprintf(f, " LAYER %s\n", this->layerName_[i]);
    for (j = 0;
	 j < this->numRects_[i];
	 j++) {
      fprintf(f, " RECT %g %g %g %g ", this->rects_[i][j].xl, this->rects_[i][j].yl, this->rects_[i][j].xh, this->rects_[i][j].yh);
      fprintf(f, "%g\n", this->densityValue_[i][j]);
    }
  }
}

/*
 * lefiPinAntennaModel
 */ lefiPinAntennaModel *
lefiPinAntennaModel_CreateUninitialized()
{
  lefiPinAntennaModel *this = (lefiPinAntennaModel *) malloc(sizeof(lefiPinAntennaModel));

  lefiPinAntennaModel_Init(this);
  return (this);
}

void 
lefiPinAntennaModel_Init(lefiPinAntennaModel * this)
{
  this->numAntennaGateArea_ = 0;
  this->antennaGateAreaAllocated_ = 1;
  this->antennaGateArea_ = (double *) lefMalloc(sizeof(double));
  this->antennaGateAreaLayer_ = (char **) lefMalloc(sizeof(char *));
  this->numAntennaMaxAreaCar_ = 0;
  this->antennaMaxAreaCarAllocated_ = 1;
  this->antennaMaxAreaCar_ = (double *) lefMalloc(sizeof(double));
  this->antennaMaxAreaCarLayer_ = (char **) lefMalloc(sizeof(char *));
  this->numAntennaMaxSideAreaCar_ = 0;
  this->antennaMaxSideAreaCarAllocated_ = 1;
  this->antennaMaxSideAreaCar_ = (double *) lefMalloc(sizeof(double));
  this->antennaMaxSideAreaCarLayer_ = (char **) lefMalloc(sizeof(char *));
  this->numAntennaMaxCutCar_ = 0;
  this->antennaMaxCutCarAllocated_ = 1;
  this->antennaMaxCutCar_ = (double *) lefMalloc(sizeof(double));
  this->antennaMaxCutCarLayer_ = (char **) lefMalloc(sizeof(char *));
  this->oxide_ = 0;
  this->hasReturn_ = 0;
}

void
lefiPinAntennaModel_Delete(lefiPinAntennaModel * this)
{
  if (this == NULL)
    return;
  lefiPinAntennaModel_Destroy(this);
  free(this);
}

void 
lefiPinAntennaModel_Destroy(lefiPinAntennaModel * this)
{
  lefiPinAntennaModel_clear(this);
}

void 
lefiPinAntennaModel_clear(lefiPinAntennaModel * this)
{
  int     i;

  if (this->oxide_)
    lefFree((char *) (this->oxide_));
  else
    return;
  this->oxide_ = 0;
  this->hasReturn_ = 0;
  for (i = 0;
       i < this->numAntennaGateArea_;
       i++) {
    if (this->antennaGateAreaLayer_[i])
      lefFree(this->antennaGateAreaLayer_[i]);
  }
  this->numAntennaGateArea_ = 0;
  for (i = 0;
       i < this->numAntennaMaxAreaCar_;
       i++) {
    if (this->antennaMaxAreaCarLayer_[i])
      lefFree(this->antennaMaxAreaCarLayer_[i]);
  }
  this->numAntennaMaxAreaCar_ = 0;
  for (i = 0;
       i < this->numAntennaMaxSideAreaCar_;
       i++) {
    if (this->antennaMaxSideAreaCarLayer_[i])
      lefFree(this->antennaMaxSideAreaCarLayer_[i]);
  }
  this->numAntennaMaxSideAreaCar_ = 0;
  for (i = 0;
       i < this->numAntennaMaxCutCar_;
       i++) {
    if (this->antennaMaxCutCarLayer_[i])
      lefFree(this->antennaMaxCutCarLayer_[i]);
  }
  this->numAntennaMaxCutCar_ = 0;
  lefFree((char *) (this->antennaGateArea_));
  lefFree((char *) (this->antennaGateAreaLayer_));
  lefFree((char *) (this->antennaMaxAreaCar_));
  lefFree((char *) (this->antennaMaxAreaCarLayer_));
  lefFree((char *) (this->antennaMaxSideAreaCar_));
  lefFree((char *) (this->antennaMaxSideAreaCarLayer_));
  lefFree((char *) (this->antennaMaxCutCar_));
  lefFree((char *) (this->antennaMaxCutCarLayer_));
}

/*
 * 5.5
 */ void 
lefiPinAntennaModel_setAntennaModel(lefiPinAntennaModel * this,
				    int aOxide)
{
  switch (aOxide) {
    case 1:this->oxide_ = strdup("OXIDE1");
    break;
  case 2:
    this->oxide_ = strdup("OXIDE2");
    break;
  case 3:
    this->oxide_ = strdup("OXIDE3");
    break;
  case 4:
    this->oxide_ = strdup("OXIDE4");
    break;
  default:
    this->oxide_ = NULL;
    break;
  }
}

void 
lefiPinAntennaModel_addAntennaGateArea(lefiPinAntennaModel * this,
				       double val,
				       const char *layer)
{
  int     len;

  if (this->numAntennaGateArea_ == this->antennaGateAreaAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAntennaGateArea_;

    double *nd;

    char  **nl;

    if (this->antennaGateAreaAllocated_ == 0)
      max = this->antennaGateAreaAllocated_ = 2;
    else
      max = this->antennaGateAreaAllocated_ *= 2;
    nd = (double *) lefMalloc(sizeof(double) * max);
    nl = (char **) lefMalloc(sizeof(double) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nl[i] = this->antennaGateAreaLayer_[i];
      nd[i] = this->antennaGateArea_[i];
    }
    lefFree((char *) (this->antennaGateAreaLayer_));
    lefFree((char *) (this->antennaGateArea_));
    this->antennaGateAreaLayer_ = nl;
    this->antennaGateArea_ = nd;
  }
  this->antennaGateArea_[this->numAntennaGateArea_] = val;
  if (layer) {
    len = strlen(layer) + 1;
    this->antennaGateAreaLayer_[this->numAntennaGateArea_] = (char *) lefMalloc(len);
    strcpy(this->antennaGateAreaLayer_[this->numAntennaGateArea_], layer);
  }
  else
    this->antennaGateAreaLayer_[this->numAntennaGateArea_] = NULL;
  this->numAntennaGateArea_ += 1;
}

void 
lefiPinAntennaModel_addAntennaMaxAreaCar(lefiPinAntennaModel * this,
					 double val,
					 const char *layer)
{
  int     len;

  if (this->numAntennaMaxAreaCar_ == this->antennaMaxAreaCarAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAntennaMaxAreaCar_;

    double *nd;

    char  **nl;

    if (this->antennaMaxAreaCarAllocated_ == 0)
      max = this->antennaMaxAreaCarAllocated_ = 2;
    else
      max = this->antennaMaxAreaCarAllocated_ *= 2;
    nd = (double *) lefMalloc(sizeof(double) * max);
    nl = (char **) lefMalloc(sizeof(double) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nl[i] = this->antennaMaxAreaCarLayer_[i];
      nd[i] = this->antennaMaxAreaCar_[i];
    }
    lefFree((char *) (this->antennaMaxAreaCarLayer_));
    lefFree((char *) (this->antennaMaxAreaCar_));
    this->antennaMaxAreaCarLayer_ = nl;
    this->antennaMaxAreaCar_ = nd;
  }
  this->antennaMaxAreaCar_[this->numAntennaMaxAreaCar_] = val;
  if (layer) {
    len = strlen(layer) + 1;
    this->antennaMaxAreaCarLayer_[this->numAntennaMaxAreaCar_] = (char *) lefMalloc(len);
    strcpy(this->antennaMaxAreaCarLayer_[this->numAntennaMaxAreaCar_], layer);
  }
  else
    this->antennaMaxAreaCarLayer_[this->numAntennaMaxAreaCar_] = NULL;
  this->numAntennaMaxAreaCar_ += 1;
}

void 
lefiPinAntennaModel_addAntennaMaxSideAreaCar(lefiPinAntennaModel * this,
					     double val,
					     const char *layer)
{
  int     len;

  if (this->numAntennaMaxSideAreaCar_ == this->antennaMaxSideAreaCarAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAntennaMaxSideAreaCar_;

    double *nd;

    char  **nl;

    if (this->antennaMaxSideAreaCarAllocated_ == 0)
      max = this->antennaMaxSideAreaCarAllocated_ = 2;
    else
      max = this->antennaMaxSideAreaCarAllocated_ *= 2;
    nd = (double *) lefMalloc(sizeof(double) * max);
    nl = (char **) lefMalloc(sizeof(double) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nl[i] = this->antennaMaxSideAreaCarLayer_[i];
      nd[i] = this->antennaMaxSideAreaCar_[i];
    }
    lefFree((char *) (this->antennaMaxSideAreaCarLayer_));
    lefFree((char *) (this->antennaMaxSideAreaCar_));
    this->antennaMaxSideAreaCarLayer_ = nl;
    this->antennaMaxSideAreaCar_ = nd;
  }
  this->antennaMaxSideAreaCar_[this->numAntennaMaxSideAreaCar_] = val;
  if (layer) {
    len = strlen(layer) + 1;
    this->antennaMaxSideAreaCarLayer_[this->numAntennaMaxSideAreaCar_] = (char *) lefMalloc(len);
    strcpy(this->antennaMaxSideAreaCarLayer_[this->numAntennaMaxSideAreaCar_], layer);
  }
  else
    this->antennaMaxSideAreaCarLayer_[this->numAntennaMaxSideAreaCar_] = NULL;
  this->numAntennaMaxSideAreaCar_ += 1;
}

void 
lefiPinAntennaModel_addAntennaMaxCutCar(lefiPinAntennaModel * this,
					double val,
					const char *layer)
{
  int     len;

  if (this->numAntennaMaxCutCar_ == this->antennaMaxCutCarAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAntennaMaxCutCar_;

    double *nd;

    char  **nl;

    if (this->antennaMaxCutCarAllocated_ == 0)
      max = this->antennaMaxCutCarAllocated_ = 2;
    else
      max = this->antennaMaxCutCarAllocated_ *= 2;
    nd = (double *) lefMalloc(sizeof(double) * max);
    nl = (char **) lefMalloc(sizeof(double) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nl[i] = this->antennaMaxCutCarLayer_[i];
      nd[i] = this->antennaMaxCutCar_[i];
    }
    lefFree((char *) (this->antennaMaxCutCarLayer_));
    lefFree((char *) (this->antennaMaxCutCar_));
    this->antennaMaxCutCarLayer_ = nl;
    this->antennaMaxCutCar_ = nd;
  }
  this->antennaMaxCutCar_[this->numAntennaMaxCutCar_] = val;
  if (layer) {
    len = strlen(layer) + 1;
    this->antennaMaxCutCarLayer_[this->numAntennaMaxCutCar_] = (char *) lefMalloc(len);
    strcpy(this->antennaMaxCutCarLayer_[this->numAntennaMaxCutCar_], layer);
  }
  else
    this->antennaMaxCutCarLayer_[this->numAntennaMaxCutCar_] = NULL;
  this->numAntennaMaxCutCar_ += 1;
}

void 
lefiPinAntennaModel_setAntennaReturnFlag(lefiPinAntennaModel * this,
					 int flag)
{
  this->hasReturn_ = flag;
  return;
}

int 
lefiPinAntennaModel_hasAntennaGateArea(const lefiPinAntennaModel * this)
{
  return this->numAntennaGateArea_ ? 1 : 0;
}

int 
lefiPinAntennaModel_hasAntennaMaxAreaCar(const lefiPinAntennaModel * this)
{
  return this->numAntennaMaxAreaCar_ ? 1 : 0;
}

int 
lefiPinAntennaModel_hasAntennaMaxSideAreaCar(const lefiPinAntennaModel * this)
{
  return this->numAntennaMaxSideAreaCar_ ? 1 : 0;
}

int 
lefiPinAntennaModel_hasAntennaMaxCutCar(const lefiPinAntennaModel * this)
{
  return this->numAntennaMaxCutCar_ ? 1 : 0;
}

/*
 * 5.5
 */ char *
lefiPinAntennaModel_antennaOxide(const lefiPinAntennaModel * this)
{
  return this->oxide_;
}

const char *
lefiPinAntennaModel_antennaGateAreaLayer(const lefiPinAntennaModel * this,
					 int i)
{
  return this->antennaGateAreaLayer_[i];
}

const char *
lefiPinAntennaModel_antennaMaxAreaCarLayer(const lefiPinAntennaModel * this,
					   int i)
{
  return this->antennaMaxAreaCarLayer_[i];
}

const char *
lefiPinAntennaModel_antennaMaxSideAreaCarLayer(const lefiPinAntennaModel * this,
					       int i)
{
  return this->antennaMaxSideAreaCarLayer_[i];
}

const char *
lefiPinAntennaModel_antennaMaxCutCarLayer(const lefiPinAntennaModel * this,
					  int i)
{
  return this->antennaMaxCutCarLayer_[i];
}

int 
lefiPinAntennaModel_numAntennaGateArea(const lefiPinAntennaModel * this)
{
  return this->numAntennaGateArea_;
}

int 
lefiPinAntennaModel_numAntennaMaxAreaCar(const lefiPinAntennaModel * this)
{
  return this->numAntennaMaxAreaCar_;
}

int 
lefiPinAntennaModel_numAntennaMaxSideAreaCar(const lefiPinAntennaModel * this)
{
  return this->numAntennaMaxSideAreaCar_;
}

int 
lefiPinAntennaModel_numAntennaMaxCutCar(const lefiPinAntennaModel * this)
{
  return this->numAntennaMaxCutCar_;
}

double 
lefiPinAntennaModel_antennaGateArea(const lefiPinAntennaModel * this,
				    int i)
{
  return this->antennaGateArea_[i];
}

double 
lefiPinAntennaModel_antennaMaxAreaCar(const lefiPinAntennaModel * this,
				      int i)
{
  return this->antennaMaxAreaCar_[i];
}

double 
lefiPinAntennaModel_antennaMaxSideAreaCar(const lefiPinAntennaModel * this,
					  int i)
{
  return this->antennaMaxSideAreaCar_[i];
}

double 
lefiPinAntennaModel_antennaMaxCutCar(const lefiPinAntennaModel * this,
				     int i)
{
  return this->antennaMaxCutCar_[i];
}

int 
lefiPinAntennaModel_hasReturn(const lefiPinAntennaModel * this)
{
  return this->hasReturn_;
}

/*
 * lefiPin
 */ lefiPin *
lefiPin_CreateUninitialized()
{
  lefiPin *this = (lefiPin *) malloc(sizeof(lefiPin));

  lefiPin_Init(this);
  return (this);
}

void 
lefiPin_Init(lefiPin * this)
{
  this->nameSize_ = 16;
  this->name_ = (char *) lefMalloc(16);
  this->portsAllocated_ = 2;
  this->ports_ = (lefiGeometries **) lefMalloc(sizeof(lefiGeometries *) * 2);
  this->numPorts_ = 0;
  this->numProperties_ = 0;
  this->propertiesAllocated_ = 0;
  this->propNames_ = 0;
  this->propValues_ = 0;
  this->propTypes_ = 0;
  this->foreign_ = 0;
  this->LEQ_ = 0;
  this->mustjoin_ = 0;
  this->lowTable_ = 0;
  this->highTable_ = 0;
  this->taperRule_ = 0;
  this->antennaModel_ = 0;
  this->numAntennaModel_ = 0;
  this->netEpxr_ = 0;
  this->ssPinName_ = 0;
  this->gsPinName_ = 0;
  lefiPin_bump(this,
	       &(this->LEQ_), 16, &(this->LEQSize_));
  lefiPin_bump(this,
	       &(this->mustjoin_), 16, &(this->mustjoinSize_));
  lefiPin_bump(this,
	       &(this->lowTable_), 16, &(this->lowTableSize_));
  lefiPin_bump(this,
	       &(this->highTable_), 16, &(this->highTableSize_));
  this->numAntennaSize_ = 0;
  this->antennaSizeAllocated_ = 1;
  this->antennaSize_ = (double *) lefMalloc(sizeof(double));
  this->antennaSizeLayer_ = (char **) lefMalloc(sizeof(char *));
  this->numAntennaMetalArea_ = 0;
  this->antennaMetalAreaAllocated_ = 1;
  this->antennaMetalArea_ = (double *) lefMalloc(sizeof(double));
  this->antennaMetalAreaLayer_ = (char **) lefMalloc(sizeof(char *));
  this->numAntennaMetalLength_ = 0;
  this->antennaMetalLengthAllocated_ = 1;
  this->antennaMetalLength_ = (double *) lefMalloc(sizeof(double));
  this->antennaMetalLengthLayer_ = (char **) lefMalloc(sizeof(char *));
  this->numAntennaPartialMetalArea_ = 0;
  this->antennaPartialMetalAreaAllocated_ = 1;
  this->antennaPartialMetalArea_ = (double *) lefMalloc(sizeof(double));
  this->antennaPartialMetalAreaLayer_ = (char **) lefMalloc(sizeof(char *));
  this->numAntennaPartialMetalSideArea_ = 0;
  this->antennaPartialMetalSideAreaAllocated_ = 1;
  this->antennaPartialMetalSideArea_ = (double *) lefMalloc(sizeof(double));
  this->antennaPartialMetalSideAreaLayer_ = (char **) lefMalloc(sizeof(char *));
  this->numAntennaPartialCutArea_ = 0;
  this->antennaPartialCutAreaAllocated_ = 1;
  this->antennaPartialCutArea_ = (double *) lefMalloc(sizeof(double));
  this->antennaPartialCutAreaLayer_ = (char **) lefMalloc(sizeof(char *));
  this->numAntennaDiffArea_ = 0;
  this->antennaDiffAreaAllocated_ = 1;
  this->antennaDiffArea_ = (double *) lefMalloc(sizeof(double));
  this->antennaDiffAreaLayer_ = (char **) lefMalloc(sizeof(char *));
}

void
lefiPin_Delete(lefiPin * this)
{
  if (this == NULL)
    return;
  lefiPin_Destroy(this);
  free(this);
}

void 
lefiPin_Destroy(lefiPin * this)
{
  lefiPin_clear(this);
  lefFree(this->name_);
  lefFree((char *) (this->ports_));
  lefFree(this->LEQ_);
  lefFree(this->mustjoin_);
  lefFree(this->lowTable_);
  lefFree(this->highTable_);
  if (this->propNames_)
    lefFree((char *) (this->propNames_));
  this->propNames_ = 0;
  if (this->propValues_)
    lefFree((char *) (this->propValues_));
  this->propValues_ = 0;
  if (this->propNums_)
    lefFree((char *) (this->propNums_));
  this->propNums_ = 0;
  if (this->propTypes_)
    lefFree((char *) (this->propTypes_));
  this->propTypes_ = 0;
  lefFree((char *) (this->antennaSize_));
  lefFree((char *) (this->antennaSizeLayer_));
  lefFree((char *) (this->antennaMetalArea_));
  lefFree((char *) (this->antennaMetalAreaLayer_));
  lefFree((char *) (this->antennaMetalLength_));
  lefFree((char *) (this->antennaMetalLengthLayer_));
  lefFree((char *) (this->antennaPartialMetalArea_));
  lefFree((char *) (this->antennaPartialMetalAreaLayer_));
  lefFree((char *) (this->antennaPartialMetalSideArea_));
  lefFree((char *) (this->antennaPartialMetalSideAreaLayer_));
  lefFree((char *) (this->antennaPartialCutArea_));
  lefFree((char *) (this->antennaPartialCutAreaLayer_));
  lefFree((char *) (this->antennaDiffArea_));
  lefFree((char *) (this->antennaDiffAreaLayer_));
  if (this->foreignAllocated_) {
    lefFree((char *) (this->hasForeignOrient_));
    lefFree((char *) (this->hasForeignPoint_));
    lefFree((char *) (this->foreignOrient_));
    lefFree((char *) (this->foreignX_));
    lefFree((char *) (this->foreignY_));
    lefFree((char *) (this->foreign_));
    this->foreignAllocated_ = 0;
  }
}

void 
lefiPin_clear(lefiPin * this)
{
  int     i;

  lefiGeometries *g;

  lefiPinAntennaModel *aModel;

  for (i = 0;
       i < this->numPorts_;
       i++) {
    g = this->ports_[i];
    lefiGeometries_Destroy(g);
    lefFree((char *) g);
  }
  this->numPorts_ = 0;
  this->portsAllocated_ = 0;
  this->hasLEQ_ = 0;
  this->hasDirection_ = 0;
  this->hasUse_ = 0;
  this->hasShape_ = 0;
  this->hasMustjoin_ = 0;
  this->hasOutMargin_ = 0;
  this->hasOutResistance_ = 0;
  this->hasInMargin_ = 0;
  this->hasPower_ = 0;
  this->hasLeakage_ = 0;
  this->hasMaxload_ = 0;
  this->hasMaxdelay_ = 0;
  this->hasCapacitance_ = 0;
  this->hasResistance_ = 0;
  this->hasPulldownres_ = 0;
  this->hasTieoffr_ = 0;
  this->hasVHI_ = 0;
  this->hasVLO_ = 0;
  this->hasRiseVoltage_ = 0;
  this->hasFallVoltage_ = 0;
  this->hasRiseThresh_ = 0;
  this->hasFallThresh_ = 0;
  this->hasRiseSatcur_ = 0;
  this->hasFallSatcur_ = 0;
  this->hasCurrentSource_ = 0;
  this->hasRiseSlewLimit_ = 0;
  this->hasFallSlewLimit_ = 0;
  this->hasTables_ = 0;
  strcpy(this->use_, "SIGNAL");
  for (i = 0;
       i < this->numForeigns_;
       i++) {
    this->hasForeignOrient_[i] = 0;
    this->hasForeignPoint_[i] = 0;
    this->foreignOrient_[i] = -1;
    lefFree((char *) (this->foreign_[i]));
  }
  this->numForeigns_ = 0;
  for (i = 0;
       i < this->numAntennaSize_;
       i++) {
    if (this->antennaSizeLayer_[i])
      lefFree(this->antennaSizeLayer_[i]);
  }
  this->numAntennaSize_ = 0;
  for (i = 0;
       i < this->numAntennaMetalLength_;
       i++) {
    if (this->antennaMetalLengthLayer_[i])
      lefFree(this->antennaMetalLengthLayer_[i]);
  }
  this->numAntennaMetalLength_ = 0;
  for (i = 0;
       i < this->numAntennaMetalArea_;
       i++) {
    if (this->antennaMetalAreaLayer_[i])
      lefFree(this->antennaMetalAreaLayer_[i]);
  }
  this->numAntennaMetalArea_ = 0;
  for (i = 0;
       i < this->numAntennaPartialMetalArea_;
       i++) {
    if (this->antennaPartialMetalAreaLayer_[i])
      lefFree(this->antennaPartialMetalAreaLayer_[i]);
  }
  this->numAntennaPartialMetalArea_ = 0;
  for (i = 0;
       i < this->numAntennaPartialMetalSideArea_;
       i++) {
    if (this->antennaPartialMetalSideAreaLayer_[i])
      lefFree(this->antennaPartialMetalSideAreaLayer_[i]);
  }
  this->numAntennaPartialMetalSideArea_ = 0;
  for (i = 0;
       i < this->numAntennaPartialCutArea_;
       i++) {
    if (this->antennaPartialCutAreaLayer_[i])
      lefFree(this->antennaPartialCutAreaLayer_[i]);
  }
  this->numAntennaPartialCutArea_ = 0;
  for (i = 0;
       i < this->numAntennaDiffArea_;
       i++) {
    if (this->antennaDiffAreaLayer_[i])
      lefFree(this->antennaDiffAreaLayer_[i]);
  }
  this->numAntennaDiffArea_ = 0;
  if (this->numAntennaModel_ > 0) {
    for (i = 0;
	 i < 4;
	 i++) {
      aModel = this->antennaModel_[i];
      lefiPinAntennaModel_Destroy(aModel);
    }
  }
  for (i = 0;
       i < this->antennaModelAllocated_;
       i++) {
    lefFree((char *) this->antennaModel_[i]);
  }
  if (this->antennaModel_)
    lefFree((char *) this->antennaModel_);
  this->antennaModel_ = 0;
  this->numAntennaModel_ = 0;
  this->curAntennaModelIndex_ = 0;
  this->antennaModelAllocated_ = 0;
  for (i = 0;
       i < this->numProperties_;
       i++) {
    lefFree(this->propNames_[i]);
    lefFree(this->propValues_[i]);
  }
  this->numProperties_ = 0;
  this->propertiesAllocated_ = 0;
  if (this->taperRule_) {
    lefFree(this->taperRule_);
    this->taperRule_ = 0;
  }
  if (this->netEpxr_) {
    lefFree(this->netEpxr_);
    this->netEpxr_ = 0;
  }
  if (this->ssPinName_) {
    lefFree(this->ssPinName_);
    this->ssPinName_ = 0;
  }
  if (this->gsPinName_) {
    lefFree(this->gsPinName_);
    this->gsPinName_ = 0;
  }
}

void 
lefiPin_bump(lefiPin * this,
	     char **array,
	     int len,
	     int *size)
{
  if (*array)
    lefFree(*array);
  *array = (char *) lefMalloc(len);
  *size = len;
}

void 
lefiPin_setName(lefiPin * this,
		const char *name)
{
  int     len = strlen(name) + 1;

  lefiPin_clear(this);
  if (len > this->nameSize_) {
    lefFree(this->name_);
    this->name_ = (char *) lefMalloc(len);
    this->nameSize_ = len;
  }
  strcpy(this->name_, CASE(name));
}

void 
lefiPin_addPort(lefiPin * this,
		lefiGeometries * g)
{
  if (this->numPorts_ == this->portsAllocated_) {
    int     i;

    lefiGeometries **ng;

    if (this->portsAllocated_ == 0)
      this->portsAllocated_ = 2;
    else
      this->portsAllocated_ *= 2;
    ng = (lefiGeometries **) lefMalloc(sizeof(lefiGeometries *) * this->portsAllocated_);
    for (i = 0;
	 i < this->numPorts_;
	 i++)
      ng[i] = this->ports_[i];
    lefFree((char *) (this->ports_));
    this->ports_ = ng;
  }
  this->ports_[this->numPorts_++] = g;
}

void 
lefiPin_addForeign(lefiPin * this,
		   const char *name,
		   int hasPnt,
		   double x,
		   double y,
		   int orient)
{
  int     i;

  int    *hfo;

  int    *hfp;

  int    *fo;

  double *fx;

  double *fy;

  char  **f;

  if (this->foreignAllocated_ == this->numForeigns_) {
    if (this->foreignAllocated_ == 0)
      this->foreignAllocated_ = 16;
    else
      this->foreignAllocated_ *= 2;
    hfo = (int *) lefMalloc(sizeof(int) * this->foreignAllocated_);
    hfp = (int *) lefMalloc(sizeof(int) * this->foreignAllocated_);
    fo = (int *) lefMalloc(sizeof(int) * this->foreignAllocated_);
    fx = (double *) lefMalloc(sizeof(double) * this->foreignAllocated_);
    fy = (double *) lefMalloc(sizeof(double) * this->foreignAllocated_);
    f = (char **) lefMalloc(sizeof(char *) * this->foreignAllocated_);
    if (this->numForeigns_ != 0) {
      for (i = 0;
	   i < this->numForeigns_;
	   i++) {
	hfo[i] = this->hasForeignOrient_[i];
	hfp[i] = this->hasForeignPoint_[i];
	fo[i] = this->foreignOrient_[i];
	fx[i] = this->foreignX_[i];
	fy[i] = this->foreignY_[i];
	f[i] = this->foreign_[i];
      }
      lefFree((char *) (this->hasForeignOrient_));
      lefFree((char *) (this->hasForeignPoint_));
      lefFree((char *) (this->foreignOrient_));
      lefFree((char *) (this->foreignX_));
      lefFree((char *) (this->foreignY_));
      lefFree((char *) (this->foreign_));
    }
    this->hasForeignOrient_ = hfo;
    this->hasForeignPoint_ = hfp;
    this->foreignOrient_ = fo;
    this->foreignX_ = fx;
    this->foreignY_ = fy;
    this->foreign_ = f;
  }
  if (orient != -1)
    this->hasForeignOrient_[this->numForeigns_] = 1;
  else
    this->hasForeignOrient_[this->numForeigns_] = -1;
  this->hasForeignPoint_[this->numForeigns_] = hasPnt;
  this->foreignOrient_[this->numForeigns_] = orient;
  this->foreignX_[this->numForeigns_] = x;
  this->foreignY_[this->numForeigns_] = y;
  this->foreign_[this->numForeigns_] = (char *) lefMalloc(strlen(name) + 1);
  strcpy(this->foreign_[this->numForeigns_], CASE(name));
  this->numForeigns_ += 1;
}

void 
lefiPin_setLEQ(lefiPin * this,
	       const char *name)
{
  int     len = strlen(name) + 1;

  if (len > this->LEQSize_)
    lefiPin_bump(this,
		 &(this->LEQ_), len, &(this->LEQSize_));
  strcpy(this->LEQ_, CASE(name));
  this->hasLEQ_ = 1;
}

void 
lefiPin_setDirection(lefiPin * this,
		     const char *name)
{
  strcpy(this->direction_, CASE(name));
  this->hasDirection_ = 1;
}

void 
lefiPin_setUse(lefiPin * this,
	       const char *name)
{
  strcpy(this->use_, CASE(name));
  this->hasUse_ = 1;
}

void 
lefiPin_setShape(lefiPin * this,
		 const char *name)
{
  strcpy(this->shape_, CASE(name));
  this->hasShape_ = 1;
}

void 
lefiPin_setMustjoin(lefiPin * this,
		    const char *name)
{
  int     len = strlen(name) + 1;

  if (len > this->mustjoinSize_)
    lefiPin_bump(this,
		 &(this->mustjoin_), len, &(this->mustjoinSize_));
  strcpy(this->mustjoin_, CASE(name));
  this->hasMustjoin_ = 1;
}

void 
lefiPin_setOutMargin(lefiPin * this,
		     double high,
		     double low)
{
  this->outMarginH_ = high;
  this->outMarginL_ = low;
  this->hasOutMargin_ = 1;
}

void 
lefiPin_setOutResistance(lefiPin * this,
			 double high,
			 double low)
{
  this->outResistanceH_ = high;
  this->outResistanceL_ = low;
  this->hasOutResistance_ = 1;
}

void 
lefiPin_setInMargin(lefiPin * this,
		    double high,
		    double low)
{
  this->inMarginH_ = high;
  this->inMarginL_ = low;
  this->hasInMargin_ = 1;
}

void 
lefiPin_setPower(lefiPin * this,
		 double power)
{
  this->power_ = power;
  this->hasPower_ = 1;
}

void 
lefiPin_setLeakage(lefiPin * this,
		   double current)
{
  this->leakage_ = current;
  this->hasLeakage_ = 1;
}

void 
lefiPin_setMaxload(lefiPin * this,
		   double capacitance)
{
  this->maxload_ = capacitance;
  this->hasMaxload_ = 1;
}

void 
lefiPin_setMaxdelay(lefiPin * this,
		    double dtime)
{
  this->maxdelay_ = dtime;
  this->hasMaxdelay_ = 1;
}

void 
lefiPin_setCapacitance(lefiPin * this,
		       double capacitance)
{
  this->capacitance_ = capacitance;
  this->hasCapacitance_ = 1;
}

void 
lefiPin_setResistance(lefiPin * this,
		      double resistance)
{
  this->resistance_ = resistance;
  this->hasResistance_ = 1;
}

void 
lefiPin_setPulldownres(lefiPin * this,
		       double resistance)
{
  this->pulldownres_ = resistance;
  this->hasPulldownres_ = 1;
}

void 
lefiPin_setTieoffr(lefiPin * this,
		   double resistance)
{
  this->tieoffr_ = resistance;
  this->hasTieoffr_ = 1;
}

void 
lefiPin_setVHI(lefiPin * this,
	       double voltage)
{
  this->VHI_ = voltage;
  this->hasVHI_ = 1;
}

void 
lefiPin_setVLO(lefiPin * this,
	       double voltage)
{
  this->VLO_ = voltage;
  this->hasVLO_ = 1;
}

void 
lefiPin_setRiseVoltage(lefiPin * this,
		       double voltage)
{
  this->riseVoltage_ = voltage;
  this->hasRiseVoltage_ = 1;
}

void 
lefiPin_setFallVoltage(lefiPin * this,
		       double voltage)
{
  this->fallVoltage_ = voltage;
  this->hasFallVoltage_ = 1;
}

void 
lefiPin_setFallSlewLimit(lefiPin * this,
			 double num)
{
  this->fallSlewLimit_ = num;
  this->hasFallSlewLimit_ = 1;
}

void 
lefiPin_setRiseSlewLimit(lefiPin * this,
			 double num)
{
  this->riseSlewLimit_ = num;
  this->hasRiseSlewLimit_ = 1;
}

void 
lefiPin_setRiseThresh(lefiPin * this,
		      double capacitance)
{
  this->riseThresh_ = capacitance;
  this->hasRiseThresh_ = 1;
}

void 
lefiPin_setTaperRule(lefiPin * this,
		     const char *name)
{
  int     len = strlen(name) + 1;

  this->taperRule_ = (char *) lefMalloc(len);
  strcpy(this->taperRule_, name);
}

void 
lefiPin_setNetExpr(lefiPin * this,
		   const char *name)
{
  this->netEpxr_ = strdup(name);
}

void 
lefiPin_setSupplySensitivity(lefiPin * this,
			     const char *pinName)
{
  this->ssPinName_ = strdup(pinName);
}

void 
lefiPin_setGroundSensitivity(lefiPin * this,
			     const char *pinName)
{
  this->gsPinName_ = strdup(pinName);
}

void 
lefiPin_setFallThresh(lefiPin * this,
		      double capacitance)
{
  this->fallThresh_ = capacitance;
  this->hasFallThresh_ = 1;
}

void 
lefiPin_setRiseSatcur(lefiPin * this,
		      double current)
{
  this->riseSatcur_ = current;
  this->hasRiseSatcur_ = 1;
}

void 
lefiPin_setFallSatcur(lefiPin * this,
		      double current)
{
  this->fallSatcur_ = current;
  this->hasFallSatcur_ = 1;
}

void 
lefiPin_setCurrentSource(lefiPin * this,
			 const char *name)
{
  strcpy(this->currentSource_, CASE(name));
  this->hasCurrentSource_ = 1;
}

void 
lefiPin_setTables(lefiPin * this,
		  const char *highName,
		  const char *lowName)
{
  int     len = strlen(highName) + 1;

  if (len > this->highTableSize_)
    lefiPin_bump(this,
		 &(this->highTable_), len, &(this->highTableSize_));
  strcpy(this->highTable_, CASE(highName));
  len = strlen(lowName) + 1;
  if (len > this->lowTableSize_)
    lefiPin_bump(this,
		 &(this->lowTable_), len, &(this->lowTableSize_));
  strcpy(this->lowTable_, CASE(lowName));
  this->hasTables_ = 1;
}

void 
lefiPin_setProperty(lefiPin * this,
		    const char *name,
		    const char *value,
		    const char type)
{
  int     len;

  if (this->numProperties_ == this->propertiesAllocated_)
    lefiPin_bumpProps(this);
  len = strlen(name) + 1;
  this->propNames_[this->numProperties_] = (char *) lefMalloc(len);
  strcpy(this->propNames_[this->numProperties_], CASE(name));
  len = strlen(value) + 1;
  this->propValues_[this->numProperties_] = (char *) lefMalloc(len);
  strcpy(this->propValues_[this->numProperties_], CASE(value));
  this->propNums_[this->numProperties_] = 0;
  this->propTypes_[this->numProperties_] = type;
  this->numProperties_ += 1;
}

void 
lefiPin_setNumProperty(lefiPin * this,
		       const char *name,
		       double d,
		       const char *value,
		       const char type)
{
  int     len;

  if (this->numProperties_ == this->propertiesAllocated_)
    lefiPin_bumpProps(this);
  len = strlen(name) + 1;
  this->propNames_[this->numProperties_] = (char *) lefMalloc(len);
  strcpy(this->propNames_[this->numProperties_], CASE(name));
  len = strlen(value) + 1;
  this->propValues_[this->numProperties_] = (char *) lefMalloc(len);
  strcpy(this->propValues_[this->numProperties_], CASE(value));
  this->propNums_[this->numProperties_] = d;
  this->propTypes_[this->numProperties_] = type;
  this->numProperties_ += 1;
}

void 
lefiPin_bumpProps(lefiPin * this)
{
  int     lim = this->propertiesAllocated_;

  int     news;

  char  **newpn;

  char  **newpv;

  double *newd;

  char   *newt;

  news = lim ? lim + lim : 2;
  newpn = (char **) lefMalloc(sizeof(char *) * news);
  newpv = (char **) lefMalloc(sizeof(char *) * news);
  newd = (double *) lefMalloc(sizeof(double) * news);
  newt = (char *) lefMalloc(sizeof(char) * news);
  lim = this->propertiesAllocated_ = news;
  if (lim > 2) {
    int     i;

    for (i = 0;
	 i < this->numProperties_;
	 i++) {
      newpn[i] = this->propNames_[i];
      newpv[i] = this->propValues_[i];
      newd[i] = this->propNums_[i];
      newt[i] = this->propTypes_[i];
    }
  }
  if (this->propNames_)
    lefFree((char *) (this->propNames_));
  if (this->propValues_)
    lefFree((char *) (this->propValues_));
  if (this->propNums_)
    lefFree((char *) (this->propNums_));
  if (this->propTypes_)
    lefFree((char *) (this->propTypes_));
  this->propNames_ = newpn;
  this->propValues_ = newpv;
  this->propNums_ = newd;
  this->propTypes_ = newt;
}

int 
lefiPin_hasForeign(const lefiPin * this)
{
  return (this->numForeigns_) ? 1 : 0;
}

int 
lefiPin_hasForeignOrient(const lefiPin * this,
			 int index)
{
  return (this->hasForeignOrient_[index] == -1) ? 0 : 1;
}

int 
lefiPin_hasForeignPoint(const lefiPin * this,
			int index)
{
  return this->hasForeignPoint_[index];
}

int 
lefiPin_hasLEQ(const lefiPin * this)
{
  return this->hasLEQ_;
}

int 
lefiPin_hasDirection(const lefiPin * this)
{
  return this->hasDirection_;
}

int 
lefiPin_hasUse(const lefiPin * this)
{
  return this->hasUse_;
}

int 
lefiPin_hasShape(const lefiPin * this)
{
  return this->hasShape_;
}

int 
lefiPin_hasMustjoin(const lefiPin * this)
{
  return this->hasMustjoin_;
}

int 
lefiPin_hasOutMargin(const lefiPin * this)
{
  return this->hasOutMargin_;
}

int 
lefiPin_hasOutResistance(const lefiPin * this)
{
  return this->hasOutResistance_;
}

int 
lefiPin_hasInMargin(const lefiPin * this)
{
  return this->hasInMargin_;
}

int 
lefiPin_hasPower(const lefiPin * this)
{
  return this->hasPower_;
}

int 
lefiPin_hasLeakage(const lefiPin * this)
{
  return this->hasLeakage_;
}

int 
lefiPin_hasMaxload(const lefiPin * this)
{
  return this->hasMaxload_;
}

int 
lefiPin_hasMaxdelay(const lefiPin * this)
{
  return this->hasMaxdelay_;
}

int 
lefiPin_hasCapacitance(const lefiPin * this)
{
  return this->hasCapacitance_;
}

int 
lefiPin_hasResistance(const lefiPin * this)
{
  return this->hasResistance_;
}

int 
lefiPin_hasPulldownres(const lefiPin * this)
{
  return this->hasPulldownres_;
}

int 
lefiPin_hasTieoffr(const lefiPin * this)
{
  return this->hasTieoffr_;
}

int 
lefiPin_hasVHI(const lefiPin * this)
{
  return this->hasVHI_;
}

int 
lefiPin_hasVLO(const lefiPin * this)
{
  return this->hasVLO_;
}

int 
lefiPin_hasFallSlewLimit(const lefiPin * this)
{
  return this->hasFallSlewLimit_;
}

int 
lefiPin_hasRiseSlewLimit(const lefiPin * this)
{
  return this->hasRiseSlewLimit_;
}

int 
lefiPin_hasRiseVoltage(const lefiPin * this)
{
  return this->hasRiseVoltage_;
}

int 
lefiPin_hasFallVoltage(const lefiPin * this)
{
  return this->hasFallVoltage_;
}

int 
lefiPin_hasRiseThresh(const lefiPin * this)
{
  return this->hasRiseThresh_;
}

int 
lefiPin_hasFallThresh(const lefiPin * this)
{
  return this->hasFallThresh_;
}

int 
lefiPin_hasRiseSatcur(const lefiPin * this)
{
  return this->hasRiseSatcur_;
}

int 
lefiPin_hasFallSatcur(const lefiPin * this)
{
  return this->hasFallSatcur_;
}

int 
lefiPin_hasCurrentSource(const lefiPin * this)
{
  return this->hasCurrentSource_;
}

int 
lefiPin_hasTables(const lefiPin * this)
{
  return this->hasTables_;
}

int 
lefiPin_hasAntennaSize(const lefiPin * this)
{
  return this->numAntennaSize_ ? 1 : 0;
}

int 
lefiPin_hasAntennaMetalLength(const lefiPin * this)
{
  return this->numAntennaMetalLength_ ? 1 : 0;
}

int 
lefiPin_hasAntennaMetalArea(const lefiPin * this)
{
  return this->numAntennaMetalArea_ ? 1 : 0;
}

int 
lefiPin_hasAntennaPartialMetalArea(const lefiPin * this)
{
  return this->numAntennaPartialMetalArea_ ? 1 : 0;
}

int 
lefiPin_hasAntennaPartialMetalSideArea(const lefiPin * this)
{
  return this->numAntennaPartialMetalSideArea_ ? 1 : 0;
}

int 
lefiPin_hasAntennaPartialCutArea(const lefiPin * this)
{
  return this->numAntennaPartialCutArea_ ? 1 : 0;
}

int 
lefiPin_hasAntennaDiffArea(const lefiPin * this)
{
  return this->numAntennaDiffArea_ ? 1 : 0;
}

int 
lefiPin_hasTaperRule(const lefiPin * this)
{
  return this->taperRule_ ? 1 : 0;
}

int 
lefiPin_hasNetExpr(const lefiPin * this)
{
  return this->netEpxr_ ? 1 : 0;
}

int 
lefiPin_hasSupplySensitivity(const lefiPin * this)
{
  return this->ssPinName_ ? 1 : 0;
}

int 
lefiPin_hasGroundSensitivity(const lefiPin * this)
{
  return this->gsPinName_ ? 1 : 0;
}

const char *
lefiPin_name(const lefiPin * this)
{
  return this->name_;
}

const char *
lefiPin_taperRule(const lefiPin * this)
{
  return this->taperRule_;
}

const char *
lefiPin_netExpr(const lefiPin * this)
{
  return this->netEpxr_;
}

const char *
lefiPin_supplySensitivity(const lefiPin * this)
{
  return this->ssPinName_;
}

const char *
lefiPin_groundSensitivity(const lefiPin * this)
{
  return this->gsPinName_;
}

int 
lefiPin_numPorts(const lefiPin * this)
{
  return this->numPorts_;
}

lefiGeometries *
lefiPin_port(const lefiPin * this,
	     int index)
{
  if (index < 0 || index > this->numPorts_) {
    lefiError("bad pin number index");
    return 0;
  }
  return this->ports_[index];
}

int 
lefiPin_numForeigns(const lefiPin * this)
{
  return this->numForeigns_;
}

const char *
lefiPin_foreignName(const lefiPin * this,
		    int index)
{
  return this->foreign_[index];
}

int 
lefiPin_foreignOrient(const lefiPin * this,
		      int index)
{
  return this->foreignOrient_[index];
}

const char *
lefiPin_foreignOrientStr(const lefiPin * this,
			 int index)
{
  return (lefiOrientStr(this->foreignOrient_[index]));
}

double 
lefiPin_foreignX(const lefiPin * this,
		 int index)
{
  return this->foreignX_[index];
}

double 
lefiPin_foreignY(const lefiPin * this,
		 int index)
{
  return this->foreignY_[index];
}

const char *
lefiPin_LEQ(const lefiPin * this)
{
  return this->LEQ_;
}

const char *
lefiPin_direction(const lefiPin * this)
{
  return this->direction_;
}

const char *
lefiPin_use(const lefiPin * this)
{
  return this->use_;
}

const char *
lefiPin_shape(const lefiPin * this)
{
  return this->shape_;
}

const char *
lefiPin_mustjoin(const lefiPin * this)
{
  return this->mustjoin_;
}

double 
lefiPin_outMarginHigh(const lefiPin * this)
{
  return this->outMarginH_;
}

double 
lefiPin_outMarginLow(const lefiPin * this)
{
  return this->outMarginL_;
}

double 
lefiPin_outResistanceHigh(const lefiPin * this)
{
  return this->outResistanceH_;
}

double 
lefiPin_outResistanceLow(const lefiPin * this)
{
  return this->outResistanceL_;
}

double 
lefiPin_inMarginHigh(const lefiPin * this)
{
  return this->inMarginH_;
}

double 
lefiPin_inMarginLow(const lefiPin * this)
{
  return this->inMarginL_;
}

double 
lefiPin_power(const lefiPin * this)
{
  return this->power_;
}

double 
lefiPin_leakage(const lefiPin * this)
{
  return this->leakage_;
}

double 
lefiPin_maxload(const lefiPin * this)
{
  return this->maxload_;
}

double 
lefiPin_maxdelay(const lefiPin * this)
{
  return this->maxdelay_;
}

double 
lefiPin_capacitance(const lefiPin * this)
{
  return this->capacitance_;
}

double 
lefiPin_resistance(const lefiPin * this)
{
  return this->resistance_;
}

double 
lefiPin_pulldownres(const lefiPin * this)
{
  return this->pulldownres_;
}

double 
lefiPin_tieoffr(const lefiPin * this)
{
  return this->tieoffr_;
}

double 
lefiPin_VHI(const lefiPin * this)
{
  return this->VHI_;
}

double 
lefiPin_VLO(const lefiPin * this)
{
  return this->VLO_;
}

double 
lefiPin_fallSlewLimit(const lefiPin * this)
{
  return this->fallSlewLimit_;
}

double 
lefiPin_riseSlewLimit(const lefiPin * this)
{
  return this->riseSlewLimit_;
}

double 
lefiPin_riseVoltage(const lefiPin * this)
{
  return this->riseVoltage_;
}

double 
lefiPin_fallVoltage(const lefiPin * this)
{
  return this->fallVoltage_;
}

double 
lefiPin_riseThresh(const lefiPin * this)
{
  return this->riseThresh_;
}

double 
lefiPin_fallThresh(const lefiPin * this)
{
  return this->fallThresh_;
}

double 
lefiPin_riseSatcur(const lefiPin * this)
{
  return this->riseSatcur_;
}

double 
lefiPin_fallSatcur(const lefiPin * this)
{
  return this->fallSatcur_;
}

const char *
lefiPin_currentSource(const lefiPin * this)
{
  return this->currentSource_;
}

const char *
lefiPin_tableHighName(const lefiPin * this)
{
  return this->highTable_;
}

const char *
lefiPin_tableLowName(const lefiPin * this)
{
  return this->lowTable_;
}

const char *
lefiPin_antennaSizeLayer(const lefiPin * this,
			 int i)
{
  return this->antennaSizeLayer_[i];
}

const char *
lefiPin_antennaMetalAreaLayer(const lefiPin * this,
			      int i)
{
  return this->antennaMetalAreaLayer_[i];
}

const char *
lefiPin_antennaMetalLengthLayer(const lefiPin * this,
				int i)
{
  return this->antennaMetalLengthLayer_[i];
}

const char *
lefiPin_antennaPartialMetalAreaLayer(const lefiPin * this,
				     int i)
{
  return this->antennaPartialMetalAreaLayer_[i];
}

const char *
lefiPin_antennaPartialMetalSideAreaLayer(const lefiPin * this,
					 int i)
{
  return this->antennaPartialMetalSideAreaLayer_[i];
}

const char *
lefiPin_antennaPartialCutAreaLayer(const lefiPin * this,
				   int i)
{
  return this->antennaPartialCutAreaLayer_[i];
}

const char *
lefiPin_antennaDiffAreaLayer(const lefiPin * this,
			     int i)
{
  return this->antennaDiffAreaLayer_[i];
}

int 
lefiPin_numAntennaSize(const lefiPin * this)
{
  return this->numAntennaSize_;
}

int 
lefiPin_numAntennaMetalArea(const lefiPin * this)
{
  return this->numAntennaMetalArea_;
}

int 
lefiPin_numAntennaMetalLength(const lefiPin * this)
{
  return this->numAntennaMetalLength_;
}

int 
lefiPin_numAntennaPartialMetalArea(const lefiPin * this)
{
  return this->numAntennaPartialMetalArea_;
}

int 
lefiPin_numAntennaPartialMetalSideArea(const lefiPin * this)
{
  return this->numAntennaPartialMetalSideArea_;
}

int 
lefiPin_numAntennaPartialCutArea(const lefiPin * this)
{
  return this->numAntennaPartialCutArea_;
}

int 
lefiPin_numAntennaDiffArea(const lefiPin * this)
{
  return this->numAntennaDiffArea_;
}

double 
lefiPin_antennaSize(const lefiPin * this,
		    int i)
{
  return this->antennaSize_[i];
}

double 
lefiPin_antennaMetalArea(const lefiPin * this,
			 int i)
{
  return this->antennaMetalArea_[i];
}

double 
lefiPin_antennaMetalLength(const lefiPin * this,
			   int i)
{
  return this->antennaMetalLength_[i];
}

double 
lefiPin_antennaPartialMetalArea(const lefiPin * this,
				int i)
{
  return this->antennaPartialMetalArea_[i];
}

double 
lefiPin_antennaPartialMetalSideArea(const lefiPin * this,
				    int i)
{
  return this->antennaPartialMetalSideArea_[i];
}

double 
lefiPin_antennaPartialCutArea(const lefiPin * this,
			      int i)
{
  return this->antennaPartialCutArea_[i];
}

double 
lefiPin_antennaDiffArea(const lefiPin * this,
			int i)
{
  return this->antennaDiffArea_[i];
}

void 
lefiPin_addAntennaMetalLength(lefiPin * this,
			      double val,
			      const char *layer)
{
  int     len;

  if (this->numAntennaMetalLength_ == this->antennaMetalLengthAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAntennaMetalLength_;

    double *nd;

    char  **nl;

    if (this->antennaMetalLengthAllocated_ == 0)
      max = this->antennaMetalLengthAllocated_ = 2;
    else
      max = this->antennaMetalLengthAllocated_ *= 2;
    nd = (double *) lefMalloc(sizeof(double) * max);
    nl = (char **) lefMalloc(sizeof(double) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nl[i] = this->antennaMetalLengthLayer_[i];
      nd[i] = this->antennaMetalLength_[i];
    }
    lefFree((char *) (this->antennaMetalLengthLayer_));
    lefFree((char *) (this->antennaMetalLength_));
    this->antennaMetalLengthLayer_ = nl;
    this->antennaMetalLength_ = nd;
  }
  this->antennaMetalLength_[this->numAntennaMetalLength_] = val;
  if (layer) {
    len = strlen(layer) + 1;
    this->antennaMetalLengthLayer_[this->numAntennaMetalLength_] = (char *) lefMalloc(len);
    strcpy(this->antennaMetalLengthLayer_[this->numAntennaMetalLength_], layer);
  }
  else
    this->antennaMetalLengthLayer_[this->numAntennaMetalLength_] = NULL;
  this->numAntennaMetalLength_ += 1;
}

/*
 * 5.5
 */ void 
lefiPin_addAntennaModel(lefiPin * this,
			int oxide)
{
  lefiPinAntennaModel *amo;

  int     i;

  if (this->numAntennaModel_ == 0) {
    this->antennaModel_ = (lefiPinAntennaModel **) lefMalloc(sizeof(lefiPinAntennaModel *) * 4);
    this->antennaModelAllocated_ = 4;
    for (i = 0;
	 i < 4;
	 i++) {
      this->antennaModel_[i] = (lefiPinAntennaModel *) lefMalloc(sizeof(lefiPinAntennaModel));
      lefiPinAntennaModel_setAntennaModel(this->antennaModel_[i],
					  0);
    }
    this->numAntennaModel_++;
    this->antennaModelAllocated_ = 4;
    amo = this->antennaModel_[0];
    this->curAntennaModelIndex_ = 0;
  }
  else {
    amo = this->antennaModel_[oxide - 1];
    this->curAntennaModelIndex_ = oxide - 1;
    if (!(lefiPinAntennaModel_antennaOxide(amo)))
      this->numAntennaModel_++;
    else {
      lefiPinAntennaModel_clear(amo);
    }
  }
  lefiPinAntennaModel_Init(amo);
  lefiPinAntennaModel_setAntennaModel(amo,
				      oxide);
  return;
}

/*
 * 5.5
 */ int 
lefiPin_numAntennaModel(const lefiPin * this)
{
  return this->numAntennaModel_;
}

/*
 * 5.5
 */ lefiPinAntennaModel *
lefiPin_antennaModel(const lefiPin * this,
		     int index)
{
  int     j = index;

  lefiPinAntennaModel *amo;

  if (index == 0) {
    int     i;

    for (i = 0;
	 i < 4;
	 i++)
      lefiPinAntennaModel_setAntennaReturnFlag(this->antennaModel_[i],
					       0);
  }
  while (j < 4) {
    amo = this->antennaModel_[j];
    if (!(lefiPinAntennaModel_antennaOxide(amo)) && (lefiPinAntennaModel_hasReturn(amo) == 0))
      j++;
    else
      break;
    if (j == 4) {
      lefiError("There is an unexpected lef parser bug which cause it unable to retrieve ANTENNAMODEL data with the given index.");
      return 0;
    }
  }
  lefiPinAntennaModel_setAntennaReturnFlag(this->antennaModel_[j],
					   1);
  return this->antennaModel_[j];
}

void 
lefiPin_addAntennaSize(lefiPin * this,
		       double val,
		       const char *layer)
{
  int     len;

  if (this->numAntennaSize_ == this->antennaSizeAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAntennaSize_;

    double *nd;

    char  **nl;

    if (this->antennaSizeAllocated_ == 0)
      max = this->antennaSizeAllocated_ = 2;
    else
      max = this->antennaSizeAllocated_ *= 2;
    nd = (double *) lefMalloc(sizeof(double) * max);
    nl = (char **) lefMalloc(sizeof(double) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nl[i] = this->antennaSizeLayer_[i];
      nd[i] = this->antennaSize_[i];
    }
    lefFree((char *) (this->antennaSizeLayer_));
    lefFree((char *) (this->antennaSize_));
    this->antennaSizeLayer_ = nl;
    this->antennaSize_ = nd;
  }
  this->antennaSize_[this->numAntennaSize_] = val;
  if (layer) {
    len = strlen(layer) + 1;
    this->antennaSizeLayer_[this->numAntennaSize_] = (char *) lefMalloc(len);
    strcpy(this->antennaSizeLayer_[this->numAntennaSize_], layer);
  }
  else
    this->antennaSizeLayer_[this->numAntennaSize_] = NULL;
  this->numAntennaSize_ += 1;
}

void 
lefiPin_addAntennaMetalArea(lefiPin * this,
			    double val,
			    const char *layer)
{
  int     len;

  if (this->numAntennaMetalArea_ == this->antennaMetalAreaAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAntennaMetalArea_;

    double *nd;

    char  **nl;

    if (this->antennaMetalAreaAllocated_ == 0)
      max = this->antennaMetalAreaAllocated_ = 2;
    else
      max = this->antennaMetalAreaAllocated_ *= 2;
    nd = (double *) lefMalloc(sizeof(double) * max);
    nl = (char **) lefMalloc(sizeof(double) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nl[i] = this->antennaMetalAreaLayer_[i];
      nd[i] = this->antennaMetalArea_[i];
    }
    lefFree((char *) (this->antennaMetalAreaLayer_));
    lefFree((char *) (this->antennaMetalArea_));
    this->antennaMetalAreaLayer_ = nl;
    this->antennaMetalArea_ = nd;
  }
  this->antennaMetalArea_[this->numAntennaMetalArea_] = val;
  if (layer) {
    len = strlen(layer) + 1;
    this->antennaMetalAreaLayer_[this->numAntennaMetalArea_] = (char *) lefMalloc(len);
    strcpy(this->antennaMetalAreaLayer_[this->numAntennaMetalArea_], layer);
  }
  else
    this->antennaMetalAreaLayer_[this->numAntennaMetalArea_] = NULL;
  this->numAntennaMetalArea_ += 1;
}

void 
lefiPin_addAntennaPartialMetalArea(lefiPin * this,
				   double val,
				   const char *layer)
{
  int     len;

  if (this->numAntennaPartialMetalArea_ == this->antennaPartialMetalAreaAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAntennaPartialMetalArea_;

    double *nd;

    char  **nl;

    if (this->antennaPartialMetalAreaAllocated_ == 0)
      max = this->antennaPartialMetalAreaAllocated_ = 2;
    else
      max = this->antennaPartialMetalAreaAllocated_ *= 2;
    nd = (double *) lefMalloc(sizeof(double) * max);
    nl = (char **) lefMalloc(sizeof(double) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nl[i] = this->antennaPartialMetalAreaLayer_[i];
      nd[i] = this->antennaPartialMetalArea_[i];
    }
    lefFree((char *) (this->antennaPartialMetalAreaLayer_));
    lefFree((char *) (this->antennaPartialMetalArea_));
    this->antennaPartialMetalAreaLayer_ = nl;
    this->antennaPartialMetalArea_ = nd;
  }
  this->antennaPartialMetalArea_[this->numAntennaPartialMetalArea_] = val;
  if (layer) {
    len = strlen(layer) + 1;
    this->antennaPartialMetalAreaLayer_[this->numAntennaPartialMetalArea_] = (char *) lefMalloc(len);
    strcpy(this->antennaPartialMetalAreaLayer_[this->numAntennaPartialMetalArea_], layer);
  }
  else
    this->antennaPartialMetalAreaLayer_[this->numAntennaPartialMetalArea_] = NULL;
  this->numAntennaPartialMetalArea_ += 1;
}

void 
lefiPin_addAntennaPartialMetalSideArea(lefiPin * this,
				       double val,
				       const char *layer)
{
  int     len;

  if (this->numAntennaPartialMetalSideArea_ == this->antennaPartialMetalSideAreaAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAntennaPartialMetalSideArea_;

    double *nd;

    char  **nl;

    if (this->antennaPartialMetalSideAreaAllocated_ == 0)
      max = this->antennaPartialMetalSideAreaAllocated_ = 2;
    else
      max = this->antennaPartialMetalSideAreaAllocated_ *= 2;
    nd = (double *) lefMalloc(sizeof(double) * max);
    nl = (char **) lefMalloc(sizeof(double) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nl[i] = this->antennaPartialMetalSideAreaLayer_[i];
      nd[i] = this->antennaPartialMetalSideArea_[i];
    }
    lefFree((char *) (this->antennaPartialMetalSideAreaLayer_));
    lefFree((char *) (this->antennaPartialMetalSideArea_));
    this->antennaPartialMetalSideAreaLayer_ = nl;
    this->antennaPartialMetalSideArea_ = nd;
  }
  this->antennaPartialMetalSideArea_[this->numAntennaPartialMetalSideArea_] = val;
  if (layer) {
    len = strlen(layer) + 1;
    this->antennaPartialMetalSideAreaLayer_[this->numAntennaPartialMetalSideArea_] = (char *) lefMalloc(len);
    strcpy(this->antennaPartialMetalSideAreaLayer_[this->numAntennaPartialMetalSideArea_], layer);
  }
  else
    this->antennaPartialMetalSideAreaLayer_[this->numAntennaPartialMetalSideArea_] = NULL;
  this->numAntennaPartialMetalSideArea_ += 1;
}

void 
lefiPin_addAntennaPartialCutArea(lefiPin * this,
				 double val,
				 const char *layer)
{
  int     len;

  if (this->numAntennaPartialCutArea_ == this->antennaPartialCutAreaAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAntennaPartialCutArea_;

    double *nd;

    char  **nl;

    if (this->antennaPartialCutAreaAllocated_ == 0)
      max = this->antennaPartialCutAreaAllocated_ = 2;
    else
      max = this->antennaPartialCutAreaAllocated_ *= 2;
    nd = (double *) lefMalloc(sizeof(double) * max);
    nl = (char **) lefMalloc(sizeof(double) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nl[i] = this->antennaPartialCutAreaLayer_[i];
      nd[i] = this->antennaPartialCutArea_[i];
    }
    lefFree((char *) (this->antennaPartialCutAreaLayer_));
    lefFree((char *) (this->antennaPartialCutArea_));
    this->antennaPartialCutAreaLayer_ = nl;
    this->antennaPartialCutArea_ = nd;
  }
  this->antennaPartialCutArea_[this->numAntennaPartialCutArea_] = val;
  if (layer) {
    len = strlen(layer) + 1;
    this->antennaPartialCutAreaLayer_[this->numAntennaPartialCutArea_] = (char *) lefMalloc(len);
    strcpy(this->antennaPartialCutAreaLayer_[this->numAntennaPartialCutArea_], layer);
  }
  else
    this->antennaPartialCutAreaLayer_[this->numAntennaPartialCutArea_] = NULL;
  this->numAntennaPartialCutArea_ += 1;
}

void 
lefiPin_addAntennaDiffArea(lefiPin * this,
			   double val,
			   const char *layer)
{
  int     len;

  if (this->numAntennaDiffArea_ == this->antennaDiffAreaAllocated_) {
    int     i;

    int     max;

    int     lim = this->numAntennaDiffArea_;

    double *nd;

    char  **nl;

    if (this->antennaDiffAreaAllocated_ == 0)
      max = this->antennaDiffAreaAllocated_ = 2;
    else
      max = this->antennaDiffAreaAllocated_ *= 2;
    nd = (double *) lefMalloc(sizeof(double) * max);
    nl = (char **) lefMalloc(sizeof(double) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nl[i] = this->antennaDiffAreaLayer_[i];
      nd[i] = this->antennaDiffArea_[i];
    }
    lefFree((char *) (this->antennaDiffAreaLayer_));
    lefFree((char *) (this->antennaDiffArea_));
    this->antennaDiffAreaLayer_ = nl;
    this->antennaDiffArea_ = nd;
  }
  this->antennaDiffArea_[this->numAntennaDiffArea_] = val;
  if (layer) {
    len = strlen(layer) + 1;
    this->antennaDiffAreaLayer_[this->numAntennaDiffArea_] = (char *) lefMalloc(len);
    strcpy(this->antennaDiffAreaLayer_[this->numAntennaDiffArea_], layer);
  }
  else
    this->antennaDiffAreaLayer_[this->numAntennaDiffArea_] = NULL;
  this->numAntennaDiffArea_ += 1;
}

void 
lefiPin_addAntennaGateArea(lefiPin * this,
			   double val,
			   const char *layer)
{
  if (this->numAntennaModel_ == 0)
    lefiPin_addAntennaModel(this,
			    1);
  lefiPinAntennaModel_addAntennaGateArea(this->antennaModel_[this->curAntennaModelIndex_],
					 val,
					 layer);
}

void 
lefiPin_addAntennaMaxAreaCar(lefiPin * this,
			     double val,
			     const char *layer)
{
  if (this->numAntennaModel_ == 0)
    lefiPin_addAntennaModel(this,
			    1);
  lefiPinAntennaModel_addAntennaMaxAreaCar(this->antennaModel_[this->curAntennaModelIndex_],
					   val,
					   layer);
}

void 
lefiPin_addAntennaMaxSideAreaCar(lefiPin * this,
				 double val,
				 const char *layer)
{
  if (this->numAntennaModel_ == 0)
    lefiPin_addAntennaModel(this,
			    1);
  lefiPinAntennaModel_addAntennaMaxSideAreaCar(this->antennaModel_[this->curAntennaModelIndex_],
					       val,
					       layer);
}

void 
lefiPin_addAntennaMaxCutCar(lefiPin * this,
			    double val,
			    const char *layer)
{
  if (this->numAntennaModel_ == 0)
    lefiPin_addAntennaModel(this,
			    1);
  lefiPinAntennaModel_addAntennaMaxCutCar(this->antennaModel_[this->curAntennaModelIndex_],
					  val,
					  layer);
}

int 
lefiPin_numProperties(const lefiPin * this)
{
  return this->numProperties_;
}

const char *
lefiPin_propName(const lefiPin * this,
		 int index)
{
  if (index < 0 || index >= this->numProperties_) {
    lefiError("bad index for pin property");
    return 0;
  }
  return this->propNames_[index];
}

const char *
lefiPin_propValue(const lefiPin * this,
		  int index)
{
  if (index < 0 || index >= this->numProperties_) {
    lefiError("bad index for pinr property");
    return 0;
  }
  return this->propValues_[index];
}

double 
lefiPin_propNum(const lefiPin * this,
		int index)
{
  if (index < 0 || index >= this->numProperties_) {
    lefiError("bad index for pinr property");
    return 0;
  }
  return this->propNums_[index];
}

const char 
lefiPin_propType(const lefiPin * this,
		 int index)
{
  if (index < 0 || index >= this->numProperties_) {
    lefiError("bad index for pinr property");
    return 0;
  }
  return this->propTypes_[index];
}

int 
lefiPin_propIsNumber(const lefiPin * this,
		     int index)
{
  if (index < 0 || index >= this->numProperties_) {
    lefiError("bad index for pinr property");
    return 0;
  }
  return this->propNums_[index] ? 1 : 0;
}

int 
lefiPin_propIsString(const lefiPin * this,
		     int index)
{
  if (index < 0 || index >= this->numProperties_) {
    lefiError("bad index for pinr property");
    return 0;
  }
  return this->propNums_[index] ? 0 : 1;
}

void 
lefiPin_print(const lefiPin * this,
	      FILE * f)
{
  int     i;

  lefiGeometries *g;

  fprintf(f, " Pin %s\n", lefiPin_name(this));
  for (i = 0;
       i < lefiPin_numPorts(this);
       i++) {
    fprintf(f, " Port %d ", i);
    g = lefiPin_port(this,
		     i);
    lefiGeometries_print(g,
			 f);
  }
}

/*
 * lefiMacro
 */ lefiMacro *
lefiMacro_CreateUninitialized()
{
  lefiMacro *this = (lefiMacro *) malloc(sizeof(lefiMacro));

  lefiMacro_Init(this);
  return (this);
}

void 
lefiMacro_Init(lefiMacro * this)
{
  this->name_ = 0;
  this->generator_ = 0;
  this->EEQ_ = 0;
  this->LEQ_ = 0;
  this->gen1_ = 0;
  this->gen2_ = 0;
  this->foreign_ = 0;
  this->siteName_ = 0;
  this->clockType_ = 0;
  this->propNames_ = 0;
  this->propValues_ = 0;
  this->propTypes_ = 0;
  lefiMacro_bump(this,
		 &(this->name_), 16, &(this->nameSize_));
  lefiMacro_bump(this,
		 &(this->generator_), 16, &(this->generatorSize_));
  lefiMacro_bump(this,
		 &(this->EEQ_), 16, &(this->EEQSize_));
  lefiMacro_bump(this,
		 &(this->LEQ_), 16, &(this->LEQSize_));
  lefiMacro_bump(this,
		 &(this->gen1_), 16, &(this->gen1Size_));
  lefiMacro_bump(this,
		 &(this->gen2_), 16, &(this->gen2Size_));
  lefiMacro_bump(this,
		 &(this->siteName_), 16, &(this->siteNameSize_));
  lefiMacro_bump(this,
		 &(this->clockType_), 16, &(this->clockTypeSize_));
  this->propertiesAllocated_ = 2;
  this->numProperties_ = 0;
  this->propNames_ = (char **) lefMalloc(sizeof(char *) * 2);
  this->propValues_ = (char **) lefMalloc(sizeof(char *) * 2);
  this->propNums_ = (double *) lefMalloc(sizeof(double) * 2);
  this->propTypes_ = (char *) lefMalloc(sizeof(char) * 2);
  this->numSites_ = 0;
  this->sitesAllocated_ = 0;
  this->pattern_ = 0;
  this->numForeigns_ = 0;
  this->foreignAllocated_ = 0;
  lefiMacro_clear(this);
}

void 
lefiMacro_Destroy(lefiMacro * this)
{
  lefiMacro_clear(this);
  lefFree(this->name_);
  lefFree(this->generator_);
  lefFree(this->EEQ_);
  lefFree(this->LEQ_);
  lefFree(this->gen1_);
  lefFree(this->gen2_);
  lefFree(this->siteName_);
  lefFree(this->clockType_);
  lefFree((char *) (this->propNames_));
  lefFree((char *) (this->propValues_));
  lefFree((char *) (this->propNums_));
  lefFree((char *) (this->propTypes_));
  if (this->foreignAllocated_) {
    lefFree((char *) (this->hasForeignOrigin_));
    lefFree((char *) (this->hasForeignPoint_));
    lefFree((char *) (this->foreignOrient_));
    lefFree((char *) (this->foreignX_));
    lefFree((char *) (this->foreignY_));
    lefFree((char *) (this->foreign_));
    this->foreignAllocated_ = 0;
  }
}

void
lefiMacro_Delete(lefiMacro * this)
{
  if (this == NULL)
    return;
  lefiMacro_Destroy(this);
  free(this);
}

void 
lefiMacro_clear(lefiMacro * this)
{
  int     i;

  this->hasClass_ = 0;
  this->hasGenerator_ = 0;
  this->hasGenerate_ = 0;
  this->hasPower_ = 0;
  this->hasOrigin_ = 0;
  this->hasSource_ = 0;
  this->hasEEQ_ = 0;
  this->hasLEQ_ = 0;
  this->hasSymmetry_ = 0;
  this->hasSiteName_ = 0;
  this->hasClockType_ = 0;
  this->hasSize_ = 0;
  this->isInverter_ = 0;
  this->isBuffer_ = 0;
  for (i = 0;
       i < this->numForeigns_;
       i++) {
    this->hasForeignOrigin_[i] = 0;
    this->hasForeignPoint_[i] = 0;
    this->foreignOrient_[i] = -1;
    lefFree((char *) (this->foreign_[i]));
  }
  this->numForeigns_ = 0;
  if (this->pattern_) {
    for (i = 0;
	 i < this->numSites_;
	 i++) {
      lefiSitePattern_Destroy(this->pattern_[i]);
      lefFree((char *) (this->pattern_[i]));
    }
    this->numSites_ = 0;
    this->sitesAllocated_ = 0;
    lefFree((char *) (this->pattern_));
    this->pattern_ = 0;
  }
  for (i = 0;
       i < this->numProperties_;
       i++) {
    lefFree(this->propNames_[i]);
    lefFree(this->propValues_[i]);
  }
  this->numProperties_ = 0;
}

void 
lefiMacro_bump(lefiMacro * this,
	       char **array,
	       int len,
	       int *size)
{
  if (*array)
    lefFree(*array);
  *array = (char *) lefMalloc(len);
  *size = len;
}

void 
lefiMacro_setName(lefiMacro * this,
		  const char *name)
{
  int     len = strlen(name) + 1;

  if (len > this->nameSize_)
    lefiMacro_bump(this,
		   &(this->name_), len, &(this->nameSize_));
  strcpy(this->name_, CASE(name));
}

void 
lefiMacro_setGenerate(lefiMacro * this,
		      const char *name,
		      const char *n2)
{
  int     len = strlen(name) + 1;

  if (len > this->gen1Size_)
    lefiMacro_bump(this,
		   &(this->gen1_), len, &(this->gen1Size_));
  strcpy(this->gen1_, CASE(name));
  len = strlen(n2) + 1;
  if (len > this->gen2Size_)
    lefiMacro_bump(this,
		   &(this->gen2_), len, &(this->gen2Size_));
  strcpy(this->gen2_, n2);
}

void 
lefiMacro_setGenerator(lefiMacro * this,
		       const char *name)
{
  int     len = strlen(name) + 1;

  if (len > this->generatorSize_)
    lefiMacro_bump(this,
		   &(this->generator_), len, &(this->generatorSize_));
  strcpy(this->generator_, CASE(name));
  this->hasGenerator_ = 1;
}

void 
lefiMacro_setInverter(lefiMacro * this)
{
  this->isInverter_ = 1;
}

void 
lefiMacro_setBuffer(lefiMacro * this)
{
  this->isBuffer_ = 1;
}

void 
lefiMacro_setSource(lefiMacro * this,
		    const char *name)
{
  strcpy(this->source_, CASE(name));
  this->hasSource_ = 1;
}

void 
lefiMacro_setClass(lefiMacro * this,
		   const char *name)
{
  strcpy(this->macroClass_, CASE(name));
  this->hasClass_ = 1;
}

void 
lefiMacro_setOrigin(lefiMacro * this,
		    double x,
		    double y)
{
  this->originX_ = x;
  this->originY_ = y;
  this->hasOrigin_ = 1;
}

void 
lefiMacro_setPower(lefiMacro * this,
		   double p)
{
  this->power_ = p;
  this->hasPower_ = 1;
}

void 
lefiMacro_setEEQ(lefiMacro * this,
		 const char *name)
{
  int     len = strlen(name) + 1;

  if (len > this->EEQSize_)
    lefiMacro_bump(this,
		   &(this->EEQ_), len, &(this->EEQSize_));
  strcpy(this->EEQ_, CASE(name));
  this->hasEEQ_ = 1;
}

void 
lefiMacro_setLEQ(lefiMacro * this,
		 const char *name)
{
  int     len = strlen(name) + 1;

  if (len > this->LEQSize_)
    lefiMacro_bump(this,
		   &(this->LEQ_), len, &(this->LEQSize_));
  strcpy(this->LEQ_, CASE(name));
  this->hasLEQ_ = 1;
}

void 
lefiMacro_setProperty(lefiMacro * this,
		      const char *name,
		      const char *value,
		      const char type)
{
  int     len;

  if (this->numProperties_ == this->propertiesAllocated_)
    lefiMacro_bumpProps(this);
  len = strlen(name) + 1;
  this->propNames_[this->numProperties_] = (char *) lefMalloc(len);
  strcpy(this->propNames_[this->numProperties_], CASE(name));
  len = strlen(value) + 1;
  this->propValues_[this->numProperties_] = (char *) lefMalloc(len);
  strcpy(this->propValues_[this->numProperties_], CASE(value));
  this->propNums_[this->numProperties_] = 0.0;
  this->propTypes_[this->numProperties_] = type;
  this->numProperties_ += 1;
}

void 
lefiMacro_setNumProperty(lefiMacro * this,
			 const char *name,
			 double d,
			 const char *value,
			 const char type)
{
  int     len;

  if (this->numProperties_ == this->propertiesAllocated_)
    lefiMacro_bumpProps(this);
  len = strlen(name) + 1;
  this->propNames_[this->numProperties_] = (char *) lefMalloc(len);
  strcpy(this->propNames_[this->numProperties_], CASE(name));
  len = strlen(value) + 1;
  this->propValues_[this->numProperties_] = (char *) lefMalloc(len);
  strcpy(this->propValues_[this->numProperties_], CASE(value));
  this->propNums_[this->numProperties_] = d;
  this->propTypes_[this->numProperties_] = type;
  this->numProperties_ += 1;
}

void 
lefiMacro_bumpProps(lefiMacro * this)
{
  int     lim = this->propertiesAllocated_;

  int     news = lim + lim;

  char  **newpn = (char **) lefMalloc(sizeof(char *) * news);

  char  **newpv = (char **) lefMalloc(sizeof(char *) * news);

  double *newd = (double *) lefMalloc(sizeof(double) * news);

  char   *newt = (char *) lefMalloc(sizeof(char) * news);

  int     i;

  this->propertiesAllocated_ = news;
  for (i = 0;
       i < lim;
       i++) {
    newpn[i] = this->propNames_[i];
    newpv[i] = this->propValues_[i];
    newd[i] = this->propNums_[i];
    newt[i] = this->propTypes_[i];
  }
  lefFree((char *) (this->propNames_));
  lefFree((char *) (this->propValues_));
  lefFree((char *) (this->propNums_));
  lefFree((char *) (this->propTypes_));
  this->propNames_ = newpn;
  this->propValues_ = newpv;
  this->propNums_ = newd;
  this->propTypes_ = newt;
}

void 
lefiMacro_addForeign(lefiMacro * this,
		     const char *name,
		     int hasPnt,
		     double x,
		     double y,
		     int orient)
{
  int     i;

  int    *hfo;

  int    *hfp;

  int    *fo;

  double *fx;

  double *fy;

  char  **f;

  if (this->foreignAllocated_ == this->numForeigns_) {
    if (this->foreignAllocated_ == 0)
      this->foreignAllocated_ = 16;
    else
      this->foreignAllocated_ *= 2;
    hfo = (int *) lefMalloc(sizeof(int) * this->foreignAllocated_);
    hfp = (int *) lefMalloc(sizeof(int) * this->foreignAllocated_);
    fo = (int *) lefMalloc(sizeof(int) * this->foreignAllocated_);
    fx = (double *) lefMalloc(sizeof(double) * this->foreignAllocated_);
    fy = (double *) lefMalloc(sizeof(double) * this->foreignAllocated_);
    f = (char **) lefMalloc(sizeof(char *) * this->foreignAllocated_);
    if (this->numForeigns_ != 0) {
      for (i = 0;
	   i < this->numForeigns_;
	   i++) {
	hfo[i] = this->hasForeignOrigin_[i];
	hfp[i] = this->hasForeignPoint_[i];
	fo[i] = this->foreignOrient_[i];
	fx[i] = this->foreignX_[i];
	fy[i] = this->foreignY_[i];
	f[i] = this->foreign_[i];
      }
      lefFree((char *) (this->hasForeignOrigin_));
      lefFree((char *) (this->hasForeignPoint_));
      lefFree((char *) (this->foreignOrient_));
      lefFree((char *) (this->foreignX_));
      lefFree((char *) (this->foreignY_));
      lefFree((char *) (this->foreign_));
    }
    this->hasForeignOrigin_ = hfo;
    this->hasForeignPoint_ = hfp;
    this->foreignOrient_ = fo;
    this->foreignX_ = fx;
    this->foreignY_ = fy;
    this->foreign_ = f;
  }
  this->hasForeignOrigin_[this->numForeigns_] = orient;
  this->hasForeignPoint_[this->numForeigns_] = hasPnt;
  this->foreignOrient_[this->numForeigns_] = orient;
  this->foreignX_[this->numForeigns_] = x;
  this->foreignY_[this->numForeigns_] = y;
  this->foreign_[this->numForeigns_] = (char *) lefMalloc(strlen(name) + 1);
  strcpy(this->foreign_[this->numForeigns_], CASE(name));
  this->numForeigns_ += 1;
}

void 
lefiMacro_setXSymmetry(lefiMacro * this)
{
  this->hasSymmetry_ |= 1;
}

void 
lefiMacro_setYSymmetry(lefiMacro * this)
{
  this->hasSymmetry_ |= 2;
}

void 
lefiMacro_set90Symmetry(lefiMacro * this)
{
  this->hasSymmetry_ |= 4;
}

void 
lefiMacro_setSiteName(lefiMacro * this,
		      const char *name)
{
  int     len = strlen(name) + 1;

  if (len > this->siteNameSize_)
    lefiMacro_bump(this,
		   &(this->siteName_), len, &(this->siteNameSize_));
  strcpy(this->siteName_, CASE(name));
  this->hasSiteName_ = 1;
}

void 
lefiMacro_setClockType(lefiMacro * this,
		       const char *name)
{
  int     len = strlen(name) + 1;

  if (len > this->clockTypeSize_)
    lefiMacro_bump(this,
		   &(this->clockType_), len, &(this->clockTypeSize_));
  strcpy(this->clockType_, CASE(name));
  this->hasClockType_ = 1;
}

void 
lefiMacro_setSitePattern(lefiMacro * this,
			 lefiSitePattern * p)
{
  if (this->numSites_ == this->sitesAllocated_) {
    lefiSitePattern **np;

    int     i, lim;

    if (this->sitesAllocated_ == 0) {
      lim = this->sitesAllocated_ = 4;
      np = (lefiSitePattern **) lefMalloc(sizeof(lefiSitePattern *) * lim);
    }
    else {
      lim = this->sitesAllocated_ * 2;
      this->sitesAllocated_ = lim;
      np = (lefiSitePattern **) lefMalloc(sizeof(lefiSitePattern *) * lim);
      lim /= 2;
      for (i = 0;
	   i < lim;
	   i++)
	np[i] = this->pattern_[i];
      lefFree((char *) (this->pattern_));
    }
    this->pattern_ = np;
  }
  this->pattern_[this->numSites_] = p;
  this->numSites_ += 1;
}

void 
lefiMacro_setSize(lefiMacro * this,
		  double x,
		  double y)
{
  this->hasSize_ = 1;
  this->sizeX_ = x;
  this->sizeY_ = y;
}

int 
lefiMacro_hasClass(const lefiMacro * this)
{
  return this->hasClass_;
}

int 
lefiMacro_hasSiteName(const lefiMacro * this)
{
  return this->hasSiteName_;
}

int 
lefiMacro_hasGenerator(const lefiMacro * this)
{
  return this->hasGenerator_;
}

int 
lefiMacro_hasPower(const lefiMacro * this)
{
  return this->hasPower_;
}

int 
lefiMacro_hasOrigin(const lefiMacro * this)
{
  return this->hasOrigin_;
}

int 
lefiMacro_hasSource(const lefiMacro * this)
{
  return this->hasSource_;
}

int 
lefiMacro_hasEEQ(const lefiMacro * this)
{
  return this->hasEEQ_;
}

int 
lefiMacro_hasLEQ(const lefiMacro * this)
{
  return this->hasLEQ_;
}

int 
lefiMacro_hasXSymmetry(const lefiMacro * this)
{
  return (this->hasSymmetry_ & 1) ? 1 : 0;
}

int 
lefiMacro_hasYSymmetry(const lefiMacro * this)
{
  return (this->hasSymmetry_ & 2) ? 1 : 0;
}

int 
lefiMacro_has90Symmetry(const lefiMacro * this)
{
  return (this->hasSymmetry_ & 4) ? 1 : 0;
}

int 
lefiMacro_hasSitePattern(const lefiMacro * this)
{
  return (this->pattern_) ? 1 : 0;
}

int 
lefiMacro_hasSize(const lefiMacro * this)
{
  return this->hasSize_;
}

int 
lefiMacro_hasForeign(const lefiMacro * this)
{
  return (this->numForeigns_) ? 1 : 0;
}

int 
lefiMacro_hasForeignOrigin(const lefiMacro * this,
			   int index)
{
  return this->hasForeignOrigin_[index];
}

int 
lefiMacro_hasForeignOrient(const lefiMacro * this,
			   int index)
{
  return (this->foreignOrient_[index] == -1) ? 0 : 1;
}

int 
lefiMacro_hasForeignPoint(const lefiMacro * this,
			  int index)
{
  return this->hasForeignPoint_[index];
}

int 
lefiMacro_hasClockType(const lefiMacro * this)
{
  return this->hasClockType_;
}

int 
lefiMacro_numSitePattern(const lefiMacro * this)
{
  return this->numSites_;
}

int 
lefiMacro_numProperties(const lefiMacro * this)
{
  return this->numProperties_;
}

const char *
lefiMacro_propName(const lefiMacro * this,
		   int index)
{
  if (index < 0 || index >= this->numProperties_) {
    lefiError("bad index for macro property");
    return 0;
  }
  return this->propNames_[index];
}

const char *
lefiMacro_propValue(const lefiMacro * this,
		    int index)
{
  if (index < 0 || index >= this->numProperties_) {
    lefiError("bad index for macro property");
    return 0;
  }
  return this->propValues_[index];
}

double 
lefiMacro_propNum(const lefiMacro * this,
		  int index)
{
  if (index < 0 || index >= this->numProperties_) {
    lefiError("bad index for macro property");
    return 0;
  }
  return this->propNums_[index];
}

const char 
lefiMacro_propType(const lefiMacro * this,
		   int index)
{
  if (index < 0 || index >= this->numProperties_) {
    lefiError("bad index for macro property");
    return 0;
  }
  return this->propTypes_[index];
}

int 
lefiMacro_propIsNumber(const lefiMacro * this,
		       int index)
{
  if (index < 0 || index >= this->numProperties_) {
    lefiError("bad index for macro property");
    return 0;
  }
  return this->propNums_[index] ? 1 : 0;
}

int 
lefiMacro_propIsString(const lefiMacro * this,
		       int index)
{
  if (index < 0 || index >= this->numProperties_) {
    lefiError("bad index for macro property");
    return 0;
  }
  return this->propNums_[index] ? 0 : 1;
}

const char *
lefiMacro_name(const lefiMacro * this)
{
  return this->name_;
}

const char *
lefiMacro_macroClass(const lefiMacro * this)
{
  return this->macroClass_;
}

const char *
lefiMacro_generator(const lefiMacro * this)
{
  return this->generator_;
}

const char *
lefiMacro_EEQ(const lefiMacro * this)
{
  return this->EEQ_;
}

const char *
lefiMacro_LEQ(const lefiMacro * this)
{
  return this->LEQ_;
}

const char *
lefiMacro_source(const lefiMacro * this)
{
  return this->source_;
}

double 
lefiMacro_originX(const lefiMacro * this)
{
  return this->originX_;
}

double 
lefiMacro_originY(const lefiMacro * this)
{
  return this->originY_;
}

double 
lefiMacro_power(const lefiMacro * this)
{
  return this->power_;
}

void 
lefiMacro_generate(const lefiMacro * this,
		   char **name1,
		   char **name2)
{
  if (name1)
    *name1 = this->gen1_;
  if (name2)
    *name2 = this->gen2_;
}

lefiSitePattern *
lefiMacro_sitePattern(const lefiMacro * this,
		      int index)
{
  return this->pattern_[index];
}

const char *
lefiMacro_siteName(const lefiMacro * this)
{
  return this->siteName_;
}

double 
lefiMacro_sizeX(const lefiMacro * this)
{
  return this->sizeX_;
}

double 
lefiMacro_sizeY(const lefiMacro * this)
{
  return this->sizeY_;
}

int 
lefiMacro_numForeigns(const lefiMacro * this)
{
  return this->numForeigns_;
}

int 
lefiMacro_foreignOrient(const lefiMacro * this,
			int index)
{
  return this->foreignOrient_[index];
}

const char *
lefiMacro_foreignOrientStr(const lefiMacro * this,
			   int index)
{
  return (lefiOrientStr(this->foreignOrient_[index]));
}

double 
lefiMacro_foreignX(const lefiMacro * this,
		   int index)
{
  return this->foreignX_[index];
}

double 
lefiMacro_foreignY(const lefiMacro * this,
		   int index)
{
  return this->foreignY_[index];
}

const char *
lefiMacro_foreignName(const lefiMacro * this,
		      int index)
{
  return this->foreign_[index];
}

const char *
lefiMacro_clockType(const lefiMacro * this)
{
  return this->clockType_;
}

int 
lefiMacro_isBuffer(const lefiMacro * this)
{
  return this->isBuffer_;
}

int 
lefiMacro_isInverter(const lefiMacro * this)
{
  return this->isInverter_;
}

void 
lefiMacro_print(const lefiMacro * this,
		FILE * f)
{
  char   *c1;

  char   *c2;

  lefiSitePattern *sp;

  int     i;

  fprintf(f, "MACRO %s\n", lefiMacro_name(this));
  if (lefiMacro_hasClass(this))
    fprintf(f, " Class %s\n", lefiMacro_macroClass(this));
  if (lefiMacro_hasGenerator(this))
    fprintf(f, " Generator %s\n", lefiMacro_generator(this));
  if (lefiMacro_hasGenerator(this)) {
    lefiMacro_generate(this,
		       &c1,
		       &c2);
    fprintf(f, " Generate %s %s\n", c1, c2);
  }
  if (lefiMacro_hasPower(this))
    fprintf(f, " Power %g\n", lefiMacro_power(this));
  if (lefiMacro_hasOrigin(this))
    fprintf(f, " Origin %g,%g\n", lefiMacro_originX(this), lefiMacro_originY(this));
  if (lefiMacro_hasEEQ(this))
    fprintf(f, " EEQ %s\n", lefiMacro_EEQ(this));
  if (lefiMacro_hasLEQ(this))
    fprintf(f, " LEQ %s\n", lefiMacro_LEQ(this));
  if (lefiMacro_hasSource(this))
    fprintf(f, " Source %s\n", lefiMacro_source(this));
  if (lefiMacro_hasXSymmetry(this))
    fprintf(f, " Symmetry X\n");
  if (lefiMacro_hasYSymmetry(this))
    fprintf(f, " Symmetry Y\n");
  if (lefiMacro_has90Symmetry(this))
    fprintf(f, " Symmetry R90\n");
  if (lefiMacro_hasSiteName(this))
    fprintf(f, " Site name %s\n", lefiMacro_siteName(this));
  if (lefiMacro_hasSitePattern(this)) {
    for (i = 0;
	 i < lefiMacro_numSitePattern(this);
	 i++) {
      sp = lefiMacro_sitePattern(this,
				 i);
      fprintf(f, " Site pattern ");
      lefiSitePattern_print(sp,
			    f);
    }
  }
  if (lefiMacro_hasSize(this))
    fprintf(f, " Size %g,%g\n", lefiMacro_sizeX(this), lefiMacro_sizeY(this));
  if (lefiMacro_hasForeign(this)) {
    for (i = 0;
	 i < lefiMacro_numForeigns(this);
	 i++) {
      fprintf(f, " Foreign %s", lefiMacro_foreignName(this,
						      i));
      if (lefiMacro_hasForeignOrigin(this,
				     i))
	fprintf(f, " %g,%g", lefiMacro_foreignX(this,
						i), lefiMacro_foreignY(this,
								       i));
      if (lefiMacro_hasForeignOrient(this,
				     i))
	fprintf(f, " orient %s", lefiMacro_foreignOrientStr(this,
							    i));
      fprintf(f, "\n");
    }
  }
  if (lefiMacro_hasClockType(this))
    fprintf(f, " Clock type %s\n", lefiMacro_clockType(this));
  fprintf(f, "END MACRO %s\n", lefiMacro_name(this));
}

/*
 * lefiTiming
 */ lefiTiming *
lefiTiming_CreateUninitialized()
{
  lefiTiming *this = (lefiTiming *) malloc(sizeof(lefiTiming));

  lefiTiming_Init(this);
  return (this);
}

void 
lefiTiming_Init(lefiTiming * this)
{
  this->numFrom_ = 0;
  this->from_ = (char **) lefMalloc(sizeof(char *));
  this->fromAllocated_ = 1;
  this->numTo_ = 0;
  this->to_ = (char **) lefMalloc(sizeof(char *));
  this->toAllocated_ = 1;
  this->numOfAxisNumbers_ = 0;
  this->axisNumbers_ = (double *) lefMalloc(sizeof(double));
  this->axisNumbersAllocated_ = 1;
  this->numOfTableEntries_ = 0;
  this->tableEntriesAllocated_ = 1;
  this->table_ = (double *) lefMalloc(sizeof(double) * 3);
  lefiTiming_clear(this);
}

void 
lefiTiming_Destroy(lefiTiming * this)
{
  lefiTiming_clear(this);
  lefFree((char *) (this->from_));
  lefFree((char *) (this->to_));
  lefFree((char *) (this->axisNumbers_));
  lefFree((char *) (this->table_));
}

void
lefiTiming_Delete(lefiTiming * this)
{
  if (this == NULL)
    return;
  lefiTiming_Destroy(this);
  free(this);
}

void 
lefiTiming_addRiseFall(lefiTiming * this,
		       const char *risefall,
		       double one,
		       double two)
{
  if (*risefall == 'r' || *risefall == 'R') {
    this->hasRiseIntrinsic_ = 1;
    this->nowRise_ = 1;
    this->riseIntrinsicOne_ = one;
    this->riseIntrinsicTwo_ = two;
  }
  else {
    this->nowRise_ = 0;
    this->hasFallIntrinsic_ = 1;
    this->fallIntrinsicOne_ = one;
    this->fallIntrinsicTwo_ = two;
  }
}

void 
lefiTiming_addRiseFallVariable(lefiTiming * this,
			       double one,
			       double two)
{
  if (this->nowRise_ == 1) {
    this->riseIntrinsicThree_ = one;
    this->riseIntrinsicFour_ = two;
  }
  else {
    this->fallIntrinsicThree_ = one;
    this->fallIntrinsicFour_ = two;
  }
}

void 
lefiTiming_setRiseRS(lefiTiming * this,
		     double one,
		     double two)
{
  this->hasRiseRS_ = 1;
  this->riseRSOne_ = one;
  this->riseRSTwo_ = two;
}

void 
lefiTiming_setFallRS(lefiTiming * this,
		     double one,
		     double two)
{
  this->hasFallRS_ = 1;
  this->fallRSOne_ = one;
  this->fallRSTwo_ = two;
}

void 
lefiTiming_setRiseCS(lefiTiming * this,
		     double one,
		     double two)
{
  this->hasRiseCS_ = 1;
  this->riseCSOne_ = one;
  this->riseCSTwo_ = two;
}

void 
lefiTiming_setFallCS(lefiTiming * this,
		     double one,
		     double two)
{
  this->hasFallCS_ = 1;
  this->fallCSOne_ = one;
  this->fallCSTwo_ = two;
}

void 
lefiTiming_setRiseAtt1(lefiTiming * this,
		       double one,
		       double two)
{
  this->hasRiseAtt1_ = 1;
  this->riseAtt1One_ = one;
  this->riseAtt1Two_ = two;
}

void 
lefiTiming_setFallAtt1(lefiTiming * this,
		       double one,
		       double two)
{
  this->hasFallAtt1_ = 1;
  this->fallAtt1One_ = one;
  this->fallAtt1Two_ = two;
}

void 
lefiTiming_setRiseTo(lefiTiming * this,
		     double one,
		     double two)
{
  this->hasRiseTo_ = 1;
  this->riseToOne_ = one;
  this->riseToTwo_ = two;
}

void 
lefiTiming_setFallTo(lefiTiming * this,
		     double one,
		     double two)
{
  this->hasFallTo_ = 1;
  this->fallToOne_ = one;
  this->fallToTwo_ = two;
}

void 
lefiTiming_addUnateness(lefiTiming * this,
			const char *typ)
{
  this->hasUnateness_ = 1;
  this->unateness_ = (char *) typ;
}

void 
lefiTiming_setStable(lefiTiming * this,
		     double one,
		     double two,
		     const char *typ)
{
  this->hasStableTiming_ = 1;
  this->stableSetup_ = one;
  this->stableHold_ = two;
  this->stableRiseFall_ = (char *) typ;
}

void 
lefiTiming_addTableEntry(lefiTiming * this,
			 double one,
			 double two,
			 double three)
{
  int     i;

  double *n;

  if (this->numOfTableEntries_ >= this->tableEntriesAllocated_) {
    int     lim;

    lim = this->tableEntriesAllocated_ *= 2;
    n = (double *) lefMalloc(sizeof(double) * 3 * lim);
    lim = this->numOfTableEntries_ * 3;
    for (i = 0;
	 i < lim;
	 i++) {
      n[i] = this->table_[i];
    }
    lefFree((char *) (this->table_));
    this->table_ = n;
  }
  i = this->numOfTableEntries_ * 3;
  this->table_[i++] = one;
  this->table_[i++] = two;
  this->table_[i] = three;
  this->numOfTableEntries_ += 1;
}

void 
lefiTiming_addTableAxisNumber(lefiTiming * this,
			      double one)
{
  if (this->numOfAxisNumbers_ == this->axisNumbersAllocated_) {
    int     i;

    int     lim;

    double *n;

    if (this->axisNumbersAllocated_ == 0)
      lim = this->axisNumbersAllocated_ = 2;
    else
      lim = this->axisNumbersAllocated_ *= 2;
    n = (double *) lefMalloc(sizeof(double) * lim);
    lim = this->numOfAxisNumbers_;
    for (i = 0;
	 i < lim;
	 i++)
      n[i] = this->axisNumbers_[i];
    if (this->axisNumbersAllocated_ > 2)
      lefFree((char *) (this->axisNumbers_));
    this->axisNumbers_ = n;
  }
  this->axisNumbers_[(this->numOfAxisNumbers_)++] = one;
}

void 
lefiTiming_addRiseFallSlew(lefiTiming * this,
			   double one,
			   double two,
			   double three,
			   double four)
{
  if (this->nowRise_) {
    this->hasRiseSlew_ = 1;
    this->riseSlewOne_ = one;
    this->riseSlewTwo_ = two;
    this->riseSlewThree_ = three;
    this->riseSlewFour_ = four;
  }
  else {
    this->hasFallSlew_ = 1;
    this->fallSlewOne_ = one;
    this->fallSlewTwo_ = two;
    this->fallSlewThree_ = three;
    this->fallSlewFour_ = four;
  }
}

void 
lefiTiming_addRiseFallSlew2(lefiTiming * this,
			    double one,
			    double two,
			    double three)
{
  if (this->nowRise_) {
    this->hasRiseSlew2_ = 1;
    this->riseSlewFive_ = one;
    this->riseSlewSix_ = two;
    this->riseSlewSeven_ = three;
  }
  else {
    this->hasFallSlew2_ = 1;
    this->fallSlewFive_ = one;
    this->fallSlewSix_ = two;
    this->fallSlewSeven_ = three;
  }
}

void 
lefiTiming_addFromPin(lefiTiming * this,
		      const char *name)
{
  if (this->numFrom_ == this->fromAllocated_) {
    int     lim;

    int     i;

    char  **n;

    if (this->fromAllocated_ == 0)
      lim = this->fromAllocated_ = 2;
    else
      lim = this->fromAllocated_ *= 2;
    n = (char **) lefMalloc(sizeof(char *) * lim);
    lim = this->numFrom_;
    for (i = 0;
	 i < lim;
	 i++)
      n[i] = this->from_[i];
    lefFree((char *) (this->from_));
    this->from_ = n;
  }
  this->from_[(this->numFrom_)++] = (char *) name;
}

void 
lefiTiming_addToPin(lefiTiming * this,
		    const char *name)
{
  if (this->numTo_ == this->toAllocated_) {
    int     lim;

    int     i;

    char  **n;

    if (this->toAllocated_ == 0)
      lim = this->toAllocated_ = 2;
    else
      lim = this->toAllocated_ *= 2;
    n = (char **) lefMalloc(sizeof(char *) * lim);
    lim = this->numTo_;
    for (i = 0;
	 i < lim;
	 i++)
      n[i] = this->to_[i];
    lefFree((char *) (this->to_));
    this->to_ = n;
  }
  this->to_[(this->numTo_)++] = (char *) name;
}

void 
lefiTiming_addDelay(lefiTiming * this,
		    const char *risefall,
		    const char *unateness,
		    double one,
		    double two,
		    double three)
{
  this->hasDelay_ = 1;
  this->delayRiseOrFall_ = (char *) risefall;
  this->delayUnateness_ = (char *) unateness;
  this->delayTableOne_ = one;
  this->delayTableTwo_ = two;
  this->delayTableThree_ = three;
}

void 
lefiTiming_addTransition(lefiTiming * this,
			 const char *risefall,
			 const char *unateness,
			 double one,
			 double two,
			 double three)
{
  this->hasTransition_ = 1;
  this->transitionRiseOrFall_ = (char *) risefall;
  this->transitionUnateness_ = (char *) unateness;
  this->transitionTableOne_ = one;
  this->transitionTableTwo_ = two;
  this->transitionTableThree_ = three;
}

void 
lefiTiming_addSDF1Pin(lefiTiming * this,
		      const char *trigType,
		      double one,
		      double two,
		      double three)
{
  this->hasSDFonePinTrigger_ = 1;
  this->SDFtriggerType_ = (char *) trigType;
  this->SDFtriggerTableOne_ = one;
  this->SDFtriggerTableTwo_ = two;
  this->SDFtriggerTableThree_ = three;
}

void 
lefiTiming_addSDF2Pins(lefiTiming * this,
		       const char *trigType,
		       const char *fromTrig,
		       const char *toTrig,
		       double one,
		       double two,
		       double three)
{
  this->hasSDFtwoPinTrigger_ = 1;
  this->SDFtriggerType_ = (char *) trigType;
  this->SDFfromTrigger_ = (char *) fromTrig;
  this->SDFtoTrigger_ = (char *) toTrig;
  this->SDFtriggerTableOne_ = one;
  this->SDFtriggerTableTwo_ = two;
  this->SDFtriggerTableThree_ = three;
}

void 
lefiTiming_setSDFcondStart(lefiTiming * this,
			   const char *condStart)
{
  this->SDFcondStart_ = (char *) condStart;
}

void 
lefiTiming_setSDFcondEnd(lefiTiming * this,
			 const char *condEnd)
{
  this->SDFcondEnd_ = (char *) condEnd;
}

void 
lefiTiming_setSDFcond(lefiTiming * this,
		      const char *cond)
{
  this->SDFcond_ = (char *) cond;
}

int 
lefiTiming_hasData(lefiTiming * this)
{
  return ((this->numFrom_) ? 1 : 0);
}

void 
lefiTiming_clear(lefiTiming * this)
{
  this->numFrom_ = 0;
  this->numTo_ = 0;
  this->numOfAxisNumbers_ = 0;
  this->numOfTableEntries_ = 0;
  this->nowRise_ = 0;
  this->hasTransition_ = 0;
  this->hasDelay_ = 0;
  this->hasFallSlew_ = 0;
  this->hasFallSlew2_ = 0;
  this->hasRiseSlew_ = 0;
  this->hasRiseSlew2_ = 0;
  this->hasRiseIntrinsic_ = 0;
  this->hasFallIntrinsic_ = 0;
  this->hasRiseSlew_ = 0;
  this->hasFallSlew_ = 0;
  this->hasRiseSlew2_ = 0;
  this->hasFallSlew2_ = 0;
  this->hasRiseRS_ = 0;
  this->hasRiseCS_ = 0;
  this->hasFallRS_ = 0;
  this->hasFallCS_ = 0;
  this->hasUnateness_ = 0;
  this->hasFallAtt1_ = 0;
  this->hasRiseAtt1_ = 0;
  this->hasFallTo_ = 0;
  this->hasRiseTo_ = 0;
  this->hasStableTiming_ = 0;
  this->hasSDFonePinTrigger_ = 0;
  this->hasSDFtwoPinTrigger_ = 0;
  this->hasSDFcondStart_ = 0;
  this->hasSDFcondEnd_ = 0;
  this->hasSDFcond_ = 0;
}

int 
lefiTiming_numFromPins(lefiTiming * this)
{
  return this->numFrom_;
}

const char *
lefiTiming_fromPin(lefiTiming * this,
		   int index)
{
  return this->from_[index];
}

int 
lefiTiming_numToPins(lefiTiming * this)
{
  return this->numTo_;
}

const char *
lefiTiming_toPin(lefiTiming * this,
		 int index)
{
  return this->to_[index];
}

int 
lefiTiming_hasTransition(lefiTiming * this)
{
  return this->hasTransition_;
}

int 
lefiTiming_hasDelay(lefiTiming * this)
{
  return this->hasDelay_;
}

int 
lefiTiming_hasRiseSlew(lefiTiming * this)
{
  return this->hasRiseSlew_;
}

int 
lefiTiming_hasRiseSlew2(lefiTiming * this)
{
  return this->hasRiseSlew2_;
}

int 
lefiTiming_hasFallSlew(lefiTiming * this)
{
  return this->hasFallSlew_;
}

int 
lefiTiming_hasFallSlew2(lefiTiming * this)
{
  return this->hasFallSlew2_;
}

int 
lefiTiming_hasRiseIntrinsic(lefiTiming * this)
{
  return this->hasRiseIntrinsic_;
}

int 
lefiTiming_hasFallIntrinsic(lefiTiming * this)
{
  return this->hasFallIntrinsic_;
}

int 
lefiTiming_hasSDFonePinTrigger(lefiTiming * this)
{
  return this->hasSDFonePinTrigger_;
}

int 
lefiTiming_hasSDFtwoPinTrigger(lefiTiming * this)
{
  return this->hasSDFtwoPinTrigger_;
}

int 
lefiTiming_hasSDFcondStart(lefiTiming * this)
{
  return this->hasSDFcondStart_;
}

int 
lefiTiming_hasSDFcondEnd(lefiTiming * this)
{
  return this->hasSDFcondEnd_;
}

int 
lefiTiming_hasSDFcond(lefiTiming * this)
{
  return this->hasSDFcond_;
}

int 
lefiTiming_numOfAxisNumbers(lefiTiming * this)
{
  return this->numOfAxisNumbers_;
}

double *
lefiTiming_axisNumbers(lefiTiming * this)
{
  return this->axisNumbers_;
}

int 
lefiTiming_numOfTableEntries(lefiTiming * this)
{
  return this->numOfTableEntries_;
}

void 
lefiTiming_tableEntry(lefiTiming * this,
		      int num,
		      double *one,
		      double *two,
		      double *three)
{
  num *= 3;
  *one = this->table_[num];
  num++;
  *two = this->table_[num];
  num++;
  *three = this->table_[num];
}

const char *
lefiTiming_delayRiseOrFall(lefiTiming * this)
{
  return this->delayRiseOrFall_;
}

const char *
lefiTiming_delayUnateness(lefiTiming * this)
{
  return this->delayUnateness_;
}

double 
lefiTiming_delayTableOne(lefiTiming * this)
{
  return this->delayTableOne_;
}

double 
lefiTiming_delayTableTwo(lefiTiming * this)
{
  return this->delayTableTwo_;
}

double 
lefiTiming_delayTableThree(lefiTiming * this)
{
  return this->delayTableThree_;
}

const char *
lefiTiming_transitionRiseOrFall(lefiTiming * this)
{
  return this->transitionRiseOrFall_;
}

const char *
lefiTiming_transitionUnateness(lefiTiming * this)
{
  return this->transitionUnateness_;
}

double 
lefiTiming_transitionTableOne(lefiTiming * this)
{
  return this->transitionTableOne_;
}

double 
lefiTiming_transitionTableTwo(lefiTiming * this)
{
  return this->transitionTableTwo_;
}

double 
lefiTiming_transitionTableThree(lefiTiming * this)
{
  return this->transitionTableThree_;
}

double 
lefiTiming_riseIntrinsicOne(lefiTiming * this)
{
  return this->riseIntrinsicOne_;
}

double 
lefiTiming_riseIntrinsicTwo(lefiTiming * this)
{
  return this->riseIntrinsicTwo_;
}

double 
lefiTiming_riseIntrinsicThree(lefiTiming * this)
{
  return this->riseIntrinsicThree_;
}

double 
lefiTiming_riseIntrinsicFour(lefiTiming * this)
{
  return this->riseIntrinsicFour_;
}

double 
lefiTiming_fallIntrinsicOne(lefiTiming * this)
{
  return this->fallIntrinsicOne_;
}

double 
lefiTiming_fallIntrinsicTwo(lefiTiming * this)
{
  return this->fallIntrinsicTwo_;
}

double 
lefiTiming_fallIntrinsicThree(lefiTiming * this)
{
  return this->fallIntrinsicThree_;
}

double 
lefiTiming_fallIntrinsicFour(lefiTiming * this)
{
  return this->fallIntrinsicFour_;
}

double 
lefiTiming_riseSlewOne(lefiTiming * this)
{
  return this->riseSlewOne_;
}

double 
lefiTiming_riseSlewTwo(lefiTiming * this)
{
  return this->riseSlewTwo_;
}

double 
lefiTiming_riseSlewThree(lefiTiming * this)
{
  return this->riseSlewThree_;
}

double 
lefiTiming_riseSlewFour(lefiTiming * this)
{
  return this->riseSlewFour_;
}

double 
lefiTiming_riseSlewFive(lefiTiming * this)
{
  return this->riseSlewFive_;
}

double 
lefiTiming_riseSlewSix(lefiTiming * this)
{
  return this->riseSlewSix_;
}

double 
lefiTiming_riseSlewSeven(lefiTiming * this)
{
  return this->riseSlewSeven_;
}

double 
lefiTiming_fallSlewOne(lefiTiming * this)
{
  return this->fallSlewOne_;
}

double 
lefiTiming_fallSlewTwo(lefiTiming * this)
{
  return this->fallSlewTwo_;
}

double 
lefiTiming_fallSlewThree(lefiTiming * this)
{
  return this->fallSlewThree_;
}

double 
lefiTiming_fallSlewFour(lefiTiming * this)
{
  return this->fallSlewFour_;
}

double 
lefiTiming_fallSlewFive(lefiTiming * this)
{
  return this->fallSlewFive_;
}

double 
lefiTiming_fallSlewSix(lefiTiming * this)
{
  return this->fallSlewSix_;
}

double 
lefiTiming_fallSlewSeven(lefiTiming * this)
{
  return this->fallSlewSeven_;
}

int 
lefiTiming_hasRiseRS(lefiTiming * this)
{
  return this->hasRiseRS_;
}

double 
lefiTiming_riseRSOne(lefiTiming * this)
{
  return this->riseRSOne_;
}

double 
lefiTiming_riseRSTwo(lefiTiming * this)
{
  return this->riseRSTwo_;
}

int 
lefiTiming_hasRiseCS(lefiTiming * this)
{
  return this->hasRiseCS_;
}

double 
lefiTiming_riseCSOne(lefiTiming * this)
{
  return this->riseCSOne_;
}

double 
lefiTiming_riseCSTwo(lefiTiming * this)
{
  return this->riseCSTwo_;
}

int 
lefiTiming_hasFallRS(lefiTiming * this)
{
  return this->hasFallRS_;
}

double 
lefiTiming_fallRSOne(lefiTiming * this)
{
  return this->fallRSOne_;
}

double 
lefiTiming_fallRSTwo(lefiTiming * this)
{
  return this->fallRSTwo_;
}

int 
lefiTiming_hasFallCS(lefiTiming * this)
{
  return this->hasFallCS_;
}

double 
lefiTiming_fallCSOne(lefiTiming * this)
{
  return this->fallCSOne_;
}

double 
lefiTiming_fallCSTwo(lefiTiming * this)
{
  return this->fallCSTwo_;
}

int 
lefiTiming_hasUnateness(lefiTiming * this)
{
  return this->hasUnateness_;
}

const char *
lefiTiming_unateness(lefiTiming * this)
{
  return this->unateness_;
}

int 
lefiTiming_hasFallAtt1(lefiTiming * this)
{
  return this->hasFallAtt1_;
}

double 
lefiTiming_fallAtt1One(lefiTiming * this)
{
  return this->fallAtt1One_;
}

double 
lefiTiming_fallAtt1Two(lefiTiming * this)
{
  return this->fallAtt1Two_;
}

int 
lefiTiming_hasRiseAtt1(lefiTiming * this)
{
  return this->hasRiseAtt1_;
}

double 
lefiTiming_riseAtt1One(lefiTiming * this)
{
  return this->riseAtt1One_;
}

double 
lefiTiming_riseAtt1Two(lefiTiming * this)
{
  return this->riseAtt1Two_;
}

int 
lefiTiming_hasFallTo(lefiTiming * this)
{
  return this->hasFallTo_;
}

double 
lefiTiming_fallToOne(lefiTiming * this)
{
  return this->fallToOne_;
}

double 
lefiTiming_fallToTwo(lefiTiming * this)
{
  return this->fallToTwo_;
}

int 
lefiTiming_hasRiseTo(lefiTiming * this)
{
  return this->hasRiseTo_;
}

double 
lefiTiming_riseToOne(lefiTiming * this)
{
  return this->riseToOne_;
}

double 
lefiTiming_riseToTwo(lefiTiming * this)
{
  return this->riseToTwo_;
}

int 
lefiTiming_hasStableTiming(lefiTiming * this)
{
  return this->hasStableTiming_;
}

double 
lefiTiming_stableSetup(lefiTiming * this)
{
  return this->stableSetup_;
}

double 
lefiTiming_stableHold(lefiTiming * this)
{
  return this->stableHold_;
}

const char *
lefiTiming_stableRiseFall(lefiTiming * this)
{
  return this->stableRiseFall_;
}

const char *
lefiTiming_SDFonePinTriggerType(lefiTiming * this)
{
  return this->SDFtriggerType_;
}

const char *
lefiTiming_SDFtwoPinTriggerType(lefiTiming * this)
{
  return this->SDFtriggerType_;
}

const char *
lefiTiming_SDFfromTrigger(lefiTiming * this)
{
  return this->SDFfromTrigger_;
}

const char *
lefiTiming_SDFtoTrigger(lefiTiming * this)
{
  return this->SDFtoTrigger_;
}

double 
lefiTiming_SDFtriggerOne(lefiTiming * this)
{
  return this->SDFtriggerTableOne_;
}

double 
lefiTiming_SDFtriggerTwo(lefiTiming * this)
{
  return this->SDFtriggerTableTwo_;
}

double 
lefiTiming_SDFtriggerThree(lefiTiming * this)
{
  return this->SDFtriggerTableThree_;
}

const char *
lefiTiming_SDFcondStart(lefiTiming * this)
{
  return this->SDFcondStart_;
}

const char *
lefiTiming_SDFcondEnd(lefiTiming * this)
{
  return this->SDFcondEnd_;
}

const char *
lefiTiming_SDFcond(lefiTiming * this)
{
  return this->SDFcond_;
}
