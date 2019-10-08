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
#include "lefiMisc.h"
#include <string.h>
#include <stdlib.h>
#include "lex.h"
#include "lefiDebug.h"
#include "FEF_stringhash.h"
#include "lefiUtil.h"

/*
 * lefiSpacing
 */ lefiSpacing *
lefiSpacing_CreateUninitialized()
{
  lefiSpacing *this = (lefiSpacing *) malloc(sizeof(lefiSpacing));

  lefiSpacing_Init(this);
  return (this);
}

void 
lefiSpacing_Init(lefiSpacing * this)
{
  this->name1_ = (char *) lefMalloc(16);
  this->name2_ = (char *) lefMalloc(16);
  this->name1Size_ = 16;
  this->name2Size_ = 16;
  this->distance_ = 0;
  this->hasStack_ = 0;
}

void 
lefiSpacing_Destroy(lefiSpacing * this)
{
  if (this->name1_)
    lefFree(this->name1_);
  if (this->name2_)
    lefFree(this->name2_);
}

void
lefiSpacing_Delete(lefiSpacing * this)
{
  if (this == NULL)
    return;
  lefiSpacing_Destroy(this);
  free(this);
}

lefiSpacing *
lefiSpacing_clone(lefiSpacing * this)
{
  lefiSpacing *sp = (lefiSpacing *) lefMalloc(sizeof(lefiSpacing));

  sp->name1Size_ = strlen(this->name1_) + 1;
  sp->name1_ = (char *) lefMalloc(sp->name1Size_);
  strcpy(sp->name1_, this->name1_);
  sp->name2Size_ = strlen(this->name2_) + 1;
  sp->name2_ = (char *) lefMalloc(sp->name2Size_);
  strcpy(sp->name2_, this->name2_);
  sp->distance_ = this->distance_;
  sp->hasStack_ = this->hasStack_;
  return sp;
}

void 
lefiSpacing_set(lefiSpacing * this,
		const char *name1,
		const char *name2,
		double d,
		int hasStack)
{
  int     len = strlen(name1) + 1;

  if (len > this->name1Size_) {
    lefFree(this->name1_);
    this->name1_ = (char *) lefMalloc(len);
    this->name1Size_ = len;
  }
  len = strlen(name2) + 1;
  if (len > this->name2Size_) {
    lefFree(this->name2_);
    this->name2_ = (char *) lefMalloc(len);
    this->name2Size_ = len;
  }
  strcpy(this->name1_, CASE(name1));
  strcpy(this->name2_, CASE(name2));
  this->distance_ = d;
  this->hasStack_ = hasStack;
}

const char *
lefiSpacing_name1(const lefiSpacing * this)
{
  return this->name1_;
}

const char *
lefiSpacing_name2(const lefiSpacing * this)
{
  return this->name2_;
}

double 
lefiSpacing_distance(const lefiSpacing * this)
{
  return this->distance_;
}

int 
lefiSpacing_hasStack(const lefiSpacing * this)
{
  return this->hasStack_;
}

void 
lefiSpacing_print(const lefiSpacing * this,
		  FILE * f)
{
  fprintf(f, "SPACING %s %s %g", lefiSpacing_name1(this), lefiSpacing_name2(this), lefiSpacing_distance(this));
  if (lefiSpacing_hasStack(this))
    fprintf(f, " STACK");
  fprintf(f, "\n");
}

/*
 * lefiIRDrop
 */ lefiIRDrop *
lefiIRDrop_CreateUninitialized()
{
  lefiIRDrop *this = (lefiIRDrop *) malloc(sizeof(lefiIRDrop));

  lefiIRDrop_Init(this);
  return (this);
}

void 
lefiIRDrop_Init(lefiIRDrop * this)
{
  this->nameSize_ = 16;
  this->value1Size_ = 16;
  this->value2Size_ = 16;
  this->name_ = (char *) lefMalloc(16);
  this->numValues_ = 0;
  this->valuesAllocated_ = 2;
  this->value1_ = (double *) lefMalloc(sizeof(double) * 2);
  this->value2_ = (double *) lefMalloc(sizeof(double) * 2);
}

void 
lefiIRDrop_Destroy(lefiIRDrop * this)
{
  lefFree(this->name_);
  lefiIRDrop_clear(this);
  lefFree((char *) (this->value1_));
  lefFree((char *) (this->value2_));
}

void
lefiIRDrop_Delete(lefiIRDrop * this)
{
  if (this == NULL)
    return;
  lefiIRDrop_Destroy(this);
  free(this);
}

void 
lefiIRDrop_clear(lefiIRDrop * this)
{
  this->numValues_ = 0;
}

void 
lefiIRDrop_setTableName(lefiIRDrop * this,
			const char *name)
{
  int     len = strlen(name) + 1;

  if (len > this->nameSize_) {
    lefFree(this->name_);
    this->name_ = (char *) lefMalloc(len);
    this->nameSize_ = len;
  }
  strcpy(this->name_, CASE(name));
  lefiIRDrop_clear(this);
}

void 
lefiIRDrop_setValues(lefiIRDrop * this,
		     double value1,
		     double value2)
{
  if (this->numValues_ == this->valuesAllocated_) {
    int     i;

    double *v1;

    double *v2;

    if (this->valuesAllocated_ == 0) {
      this->valuesAllocated_ = 2;
      v1 = (double *) lefMalloc(sizeof(double) * this->valuesAllocated_);
      v2 = (double *) lefMalloc(sizeof(double) * this->valuesAllocated_);
    }
    else {
      this->valuesAllocated_ *= 2;
      v1 = (double *) lefMalloc(sizeof(double) * this->valuesAllocated_);
      v2 = (double *) lefMalloc(sizeof(double) * this->valuesAllocated_);
      for (i = 0;
	   i < this->numValues_;
	   i++) {
	v1[i] = this->value1_[i];
	v2[i] = this->value2_[i];
      }
      lefFree((char *) (this->value1_));
      lefFree((char *) (this->value2_));
    }
    this->value1_ = v1;
    this->value2_ = v2;
  }
  this->value1_[this->numValues_] = value1;
  this->value2_[this->numValues_] = value2;
  this->numValues_ += 1;
}

const char *
lefiIRDrop_name(const lefiIRDrop * this)
{
  return this->name_;
}

int 
lefiIRDrop_numValues(const lefiIRDrop * this)
{
  return this->numValues_;
}

double 
lefiIRDrop_value1(const lefiIRDrop * this,
		  int index)
{
  if (index < 0 || index >= this->numValues_) {
    lefiError("bad index for IRDrop current");
    return 0;
  }
  return this->value1_[index];
}

double 
lefiIRDrop_value2(const lefiIRDrop * this,
		  int index)
{
  if (index < 0 || index >= this->numValues_) {
    lefiError("bad index for IRDrop volts");
    return 0;
  }
  return this->value2_[index];
}

void 
lefiIRDrop_print(const lefiIRDrop * this,
		 FILE * f)
{
  int     i;

  fprintf(f, "IRDROP %s ", lefiIRDrop_name(this));
  for (i = 0;
       i < lefiIRDrop_numValues(this);
       i++) {
    fprintf(f, "%g %g ", lefiIRDrop_value1(this,
					   i), lefiIRDrop_value2(this,
								 i));
  }
  fprintf(f, "\n");
  fprintf(f, "END IRDrop\n");
}

/*
 * lefitMinFeature
 */ lefiMinFeature *
lefiMinFeature_CreateUninitialized()
{
  lefiMinFeature *this = (lefiMinFeature *) malloc(sizeof(lefiMinFeature));

  lefiMinFeature_Init(this);
  return (this);
}

void 
lefiMinFeature_Init(lefiMinFeature * this)
{
}

void 
lefiMinFeature_Destroy(lefiMinFeature * this)
{
}

void
lefiMinFeature_Delete(lefiMinFeature * this)
{
  if (this == NULL)
    return;
  lefiMinFeature_Destroy(this);
  free(this);
}

void 
lefiMinFeature_set(lefiMinFeature * this,
		   double one,
		   double two)
{
  this->one_ = one;
  this->two_ = two;
}

double 
lefiMinFeature_one(const lefiMinFeature * this)
{
  return this->one_;
}

double 
lefiMinFeature_two(const lefiMinFeature * this)
{
  return this->two_;
}

void 
lefiMinFeature_print(const lefiMinFeature * this,
		     FILE * f)
{
  fprintf(f, "MINfEATURE %g %g\n", lefiMinFeature_one(this), lefiMinFeature_two(this));
}

/*
 * lefiSite
 */ lefiSite *
lefiSite_CreateUninitialized()
{
  lefiSite *this = (lefiSite *) malloc(sizeof(lefiSite));

  lefiSite_Init(this);
  return (this);
}

void 
lefiSite_Init(lefiSite * this)
{
  this->nameSize_ = 16;
  this->name_ = (char *) lefMalloc(16);
}

void 
lefiSite_Destroy(lefiSite * this)
{
  lefFree(this->name_);
}

void
lefiSite_Delete(lefiSite * this)
{
  if (this == NULL)
    return;
  lefiSite_Destroy(this);
  free(this);
}

void 
lefiSite_setName(lefiSite * this,
		 const char *name)
{
  int     len = strlen(name) + 1;

  if (len > this->nameSize_) {
    lefFree(this->name_);
    this->name_ = (char *) lefMalloc(len);
    this->nameSize_ = len;
  }
  strcpy(this->name_, CASE(name));
  this->hasClass_ = 0;
  *(this->siteClass_) = 0;
  this->hasSize_ = 0;
  this->symmetry_ = 0;
}

void 
lefiSite_setClass(lefiSite * this,
		  const char *cls)
{
  strcpy(this->siteClass_, cls);
  this->hasClass_ = 1;
}

void 
lefiSite_setSize(lefiSite * this,
		 double x,
		 double y)
{
  this->hasSize_ = 1;
  this->sizeX_ = x;
  this->sizeY_ = y;
}

void 
lefiSite_setXSymmetry(lefiSite * this)
{
  this->symmetry_ |= 1;
}

void 
lefiSite_setYSymmetry(lefiSite * this)
{
  this->symmetry_ |= 2;
}

void 
lefiSite_set90Symmetry(lefiSite * this)
{
  this->symmetry_ |= 4;
}

const char *
lefiSite_name(const lefiSite * this)
{
  return this->name_;
}

int 
lefiSite_hasClass(const lefiSite * this)
{
  return this->hasClass_;
}

const char *
lefiSite_siteClass(const lefiSite * this)
{
  return this->siteClass_;
}

double 
lefiSite_sizeX(const lefiSite * this)
{
  return this->sizeX_;
}

double 
lefiSite_sizeY(const lefiSite * this)
{
  return this->sizeY_;
}

int 
lefiSite_hasSize(const lefiSite * this)
{
  return this->hasSize_;
}

int 
lefiSite_hasXSymmetry(const lefiSite * this)
{
  return (this->symmetry_ & 1) ? 1 : 0;
}

int 
lefiSite_hasYSymmetry(const lefiSite * this)
{
  return (this->symmetry_ & 2) ? 1 : 0;
}

int 
lefiSite_has90Symmetry(const lefiSite * this)
{
  return (this->symmetry_ & 4) ? 1 : 0;
}

void 
lefiSite_print(const lefiSite * this,
	       FILE * f)
{
  fprintf(f, "SITE %s", lefiSite_name(this));
  if (lefiSite_hasClass(this))
    fprintf(f, " CLASS %s", lefiSite_siteClass(this));
  if (lefiSite_hasSize(this))
    fprintf(f, " SIZE %g %g", lefiSite_sizeX(this), lefiSite_sizeY(this));
  if (lefiSite_hasXSymmetry(this))
    fprintf(f, " SYMMETRY X");
  if (lefiSite_hasYSymmetry(this))
    fprintf(f, " SYMMETRY Y");
  if (lefiSite_has90Symmetry(this))
    fprintf(f, " SYMMETRY R90");
  fprintf(f, "\n");
}

/*
 * lefiSitePattern
 */ lefiSitePattern *
lefiSitePattern_CreateUninitialized()
{
  lefiSitePattern *this = (lefiSitePattern *) malloc(sizeof(lefiSitePattern));

  lefiSitePattern_Init(this);
  return (this);
}

void 
lefiSitePattern_Init(lefiSitePattern * this)
{
  this->nameSize_ = 16;
  this->name_ = (char *) lefMalloc(16);
}

void 
lefiSitePattern_Destroy(lefiSitePattern * this)
{
  lefFree(this->name_);
}

void
lefiSitePattern_Delete(lefiSitePattern * this)
{
  if (this == NULL)
    return;
  lefiSitePattern_Destroy(this);
  free(this);
}

void 
lefiSitePattern_set(lefiSitePattern * this,
		    const char *name,
		    double x,
		    double y,
		    int orient,
		    double xStart,
		    double yStart,
		    double xStep,
		    double yStep)
{
  int     len = strlen(name) + 1;

  if (len > this->nameSize_) {
    lefFree(this->name_);
    this->name_ = (char *) lefMalloc(len);
    this->nameSize_ = len;
  }
  strcpy(this->name_, CASE(name));
  this->x_ = x;
  this->y_ = y;
  this->xStep_ = xStep;
  this->yStep_ = yStep;
  this->xStart_ = xStart;
  this->yStart_ = yStart;
  this->orient_ = orient;
}

const char *
lefiSitePattern_name(const lefiSitePattern * this)
{
  return this->name_;
}

int 
lefiSitePattern_orient(const lefiSitePattern * this)
{
  return this->orient_;
}

const char *
lefiSitePattern_orientStr(const lefiSitePattern * this)
{
  return (lefiOrientStr(this->orient_));
}

double 
lefiSitePattern_x(const lefiSitePattern * this)
{
  return this->x_;
}

double 
lefiSitePattern_y(const lefiSitePattern * this)
{
  return this->y_;
}

double 
lefiSitePattern_xStart(const lefiSitePattern * this)
{
  return this->xStart_;
}

double 
lefiSitePattern_yStart(const lefiSitePattern * this)
{
  return this->yStart_;
}

double 
lefiSitePattern_xStep(const lefiSitePattern * this)
{
  return this->xStep_;
}

double 
lefiSitePattern_yStep(const lefiSitePattern * this)
{
  return this->yStep_;
}

void 
lefiSitePattern_print(const lefiSitePattern * this,
		      FILE * f)
{
  fprintf(f, " SITE Pattern %s %g,%g %s\n", lefiSitePattern_name(this), lefiSitePattern_x(this), lefiSitePattern_y(this), lefiSitePattern_orientStr(this));
  fprintf(f, " %g,%g step %g,%g\n", lefiSitePattern_xStart(this), lefiSitePattern_yStart(this), lefiSitePattern_xStep(this), lefiSitePattern_yStep(this));
}

/*
 * lefiTrackPattern
 */ lefiTrackPattern *
lefiTrackPattern_CreateUninitialized()
{
  lefiTrackPattern *this = (lefiTrackPattern *) malloc(sizeof(lefiTrackPattern));

  lefiTrackPattern_Init(this);
  return (this);
}

void 
lefiTrackPattern_Init(lefiTrackPattern * this)
{
  this->nameSize_ = 16;
  this->name_ = (char *) lefMalloc(16);
  this->start_ = 0;
  this->numTracks_ = 0;
  this->space_ = 0;
  this->numLayers_ = 0;
  this->layerAllocated_ = 2;
  this->layerNames_ = (char **) lefMalloc(sizeof(char *) * 2);
  lefiTrackPattern_clear(this);
}

void 
lefiTrackPattern_Destroy(lefiTrackPattern * this)
{
  if (this->name_)
    lefFree(this->name_);
  lefiTrackPattern_clear(this);
  this->name_ = 0;
  this->start_ = 0;
  this->numTracks_ = 0;
  this->space_ = 0;
  lefFree((char *) (this->layerNames_));
}

void 
lefiTrackPattern_clear(lefiTrackPattern * this)
{
  int     i;

  for (i = 0;
       i < this->numLayers_;
       i++) {
    lefFree(this->layerNames_[i]);
  }
}

void
lefiTrackPattern_Delete(lefiTrackPattern * this)
{
  if (this == NULL)
    return;
  lefiTrackPattern_Destroy(this);
  free(this);
}

void 
lefiTrackPattern_set(lefiTrackPattern * this,
		     const char *name,
		     double start,
		     int numTracks,
		     double space)
{
  int     len = strlen(name) + 1;

  if (len > this->nameSize_) {
    lefFree(this->name_);
    this->name_ = (char *) lefMalloc(len);
    this->nameSize_ = len;
  }
  strcpy(this->name_, CASE(name));
  this->start_ = start;
  this->numTracks_ = numTracks;
  this->space_ = space;
}

void 
lefiTrackPattern_addLayer(lefiTrackPattern * this,
			  const char *name)
{
  int     len;

  if (this->numLayers_ == this->layerAllocated_) {
    int     i;

    char  **nn;

    if (this->layerAllocated_ == 0)
      this->layerAllocated_ = 2;
    else
      this->layerAllocated_ *= 2;
    nn = (char **) lefMalloc(sizeof(char *) * this->layerAllocated_);
    for (i = 0;
	 i < this->numLayers_;
	 i++)
      nn[i] = this->layerNames_[i];
    lefFree((char *) (this->layerNames_));
    this->layerNames_ = nn;
  }
  len = strlen(name) + 1;
  this->layerNames_[this->numLayers_] = (char *) lefMalloc(len);
  strcpy(this->layerNames_[this->numLayers_], CASE(name));
  this->numLayers_ += 1;
}

const char *
lefiTrackPattern_name(const lefiTrackPattern * this)
{
  return this->name_;
}

double 
lefiTrackPattern_start(const lefiTrackPattern * this)
{
  return this->start_;
}

int 
lefiTrackPattern_numTracks(const lefiTrackPattern * this)
{
  return this->numTracks_;
}

double 
lefiTrackPattern_space(const lefiTrackPattern * this)
{
  return this->space_;
}

int 
lefiTrackPattern_numLayers(const lefiTrackPattern * this)
{
  return this->numLayers_;
}

const char *
lefiTrackPattern_layerName(const lefiTrackPattern * this,
			   int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("ERROR: bad layer number index");
    return 0;
  }
  return this->layerNames_[index];
}

void 
lefiTrackPattern_print(const lefiTrackPattern * this,
		       FILE * f)
{
  int     i;

  fprintf(f, " TRACK Pattern %s %g DO %d STEP %g\n", lefiTrackPattern_name(this), lefiTrackPattern_start(this), lefiTrackPattern_numTracks(this), lefiTrackPattern_space(this));
  if (lefiTrackPattern_numLayers(this) > 0) {
    fprintf(f, " LAYER ");
    for (i = 0;
	 i < lefiTrackPattern_numLayers(this);
	 i++)
      fprintf(f, "%s ", lefiTrackPattern_layerName(this,
						   i));
    fprintf(f, "\n");
  }
}

/*
 * lefiGcellPattern
 */ lefiGcellPattern *
lefiGcellPattern_CreateUninitialized()
{
  lefiGcellPattern *this = (lefiGcellPattern *) malloc(sizeof(lefiGcellPattern));

  lefiGcellPattern_Init(this);
  return (this);
}

void 
lefiGcellPattern_Init(lefiGcellPattern * this)
{
  this->nameSize_ = 16;
  this->name_ = (char *) lefMalloc(16);
  this->start_ = 0;
  this->numCRs_ = 0;
  this->space_ = 0;
}

void 
lefiGcellPattern_Destroy(lefiGcellPattern * this)
{
  if (this->name_)
    lefFree(this->name_);
  this->name_ = 0;
  this->start_ = 0;
  this->numCRs_ = 0;
  this->space_ = 0;
}

void
lefiGcellPattern_Delete(lefiGcellPattern * this)
{
  if (this == NULL)
    return;
  lefiGcellPattern_Destroy(this);
  free(this);
}

void 
lefiGcellPattern_set(lefiGcellPattern * this,
		     const char *name,
		     double start,
		     int numCRs,
		     double space)
{
  int     len = strlen(name) + 1;

  if (len > this->nameSize_) {
    lefFree(this->name_);
    this->name_ = (char *) lefMalloc(len);
    this->nameSize_ = len;
  }
  strcpy(this->name_, CASE(name));
  this->start_ = start;
  this->numCRs_ = numCRs;
  this->space_ = space;
}

const char *
lefiGcellPattern_name(const lefiGcellPattern * this)
{
  return this->name_;
}

double 
lefiGcellPattern_start(const lefiGcellPattern * this)
{
  return this->start_;
}

int 
lefiGcellPattern_numCRs(const lefiGcellPattern * this)
{
  return this->numCRs_;
}

double 
lefiGcellPattern_space(const lefiGcellPattern * this)
{
  return this->space_;
}

void 
lefiGcellPattern_print(const lefiGcellPattern * this,
		       FILE * f)
{
  fprintf(f, " TRACK Pattern %s %g DO %d STEP %g\n", lefiGcellPattern_name(this), lefiGcellPattern_start(this), lefiGcellPattern_numCRs(this), lefiGcellPattern_space(this));
}

/*
 * lefiAlias_itr
 */ lefiAlias_itr *
lefiAlias_itr_CreateUninitialized()
{
  lefiAlias_itr *this = (lefiAlias_itr *) malloc(sizeof(lefiAlias_itr));

  lefiAlias_itr_Init(this);
  return (this);
}

void 
lefiAlias_itr_Init(lefiAlias_itr * this)
{
  this->num_ = -1;
}

void 
lefiAlias_itr_Destroy(lefiAlias_itr * this)
{
}

void
lefiAlias_itr_Delete(lefiAlias_itr * this)
{
  if (this == NULL)
    return;
  lefiAlias_itr_Destroy(this);
  free(this);
}

int 
lefiAlias_itr_Next(lefiAlias_itr * this)
{
  while (1) {
    this->num_ += 1;
    if (this->num_ <= -1 || this->num_ >= aliasLimit())
      return 0;
    if (aliasKey(this->num_))
      break;
  }
  return 1;
}

const char *
lefiAlias_itr_Key(lefiAlias_itr * this)
{
  if (this->num_ <= -1 || this->num_ >= aliasLimit())
    return 0;
  return aliasKey(this->num_);
}

const char *
lefiAlias_itr_Data(lefiAlias_itr * this)
{
  if (this->num_ <= -1 || this->num_ >= aliasLimit())
    return 0;
  return aliasData(this->num_);
}

/*
 * lefiUseMinSpacing
 */ lefiUseMinSpacing *
lefiUseMinSpacing_CreateUninitialized()
{
  lefiUseMinSpacing *this = (lefiUseMinSpacing *) malloc(sizeof(lefiUseMinSpacing));

  lefiUseMinSpacing_Init(this);
  return (this);
}

void 
lefiUseMinSpacing_Init(lefiUseMinSpacing * this)
{
  this->name_ = 0;
  this->value_ = 0;
}

void 
lefiUseMinSpacing_Destroy(lefiUseMinSpacing * this)
{
  if (this->name_)
    lefFree(this->name_);
}

void
lefiUseMinSpacing_Delete(lefiUseMinSpacing * this)
{
  if (this == NULL)
    return;
  lefiUseMinSpacing_Destroy(this);
  free(this);
}

void 
lefiUseMinSpacing_set(lefiUseMinSpacing * this,
		      const char *name,
		      int value)
{
  lefiUseMinSpacing_Destroy(this);
  this->name_ = (char *) lefMalloc(strlen(name) + 1);
  strcpy(this->name_, CASE(name));
  this->value_ = value;
}

const char *
lefiUseMinSpacing_name(const lefiUseMinSpacing * this)
{
  return this->name_;
}

int 
lefiUseMinSpacing_value(const lefiUseMinSpacing * this)
{
  return this->value_;
}

void 
lefiUseMinSpacing_print(const lefiUseMinSpacing * this,
			FILE * f)
{
  fprintf(f, "USEMINSPACING %s %d\n", lefiUseMinSpacing_name(this), lefiUseMinSpacing_value(this));
}

/*
 * lefiMaxStackVia
 */ lefiMaxStackVia *
lefiMaxStackVia_CreateUninitialized()
{
  lefiMaxStackVia *this = (lefiMaxStackVia *) malloc(sizeof(lefiMaxStackVia));

  lefiMaxStackVia_Init(this);
  return (this);
}

void 
lefiMaxStackVia_Init(lefiMaxStackVia * this)
{
  this->value_ = 0;
  this->hasRange_ = 0;
  this->bottomLayer_ = 0;
  this->topLayer_ = 0;
}

void 
lefiMaxStackVia_Destroy(lefiMaxStackVia * this)
{
  if (this->bottomLayer_)
    lefFree(this->bottomLayer_);
  if (this->topLayer_)
    lefFree(this->topLayer_);
  this->hasRange_ = 0;
}

void
lefiMaxStackVia_Delete(lefiMaxStackVia * this)
{
  if (this == NULL)
    return;
  lefiMaxStackVia_Destroy(this);
  free(this);
}

void 
lefiMaxStackVia_setMaxStackVia(lefiMaxStackVia * this,
			       int value)
{
  this->value_ = value;
}

void 
lefiMaxStackVia_setMaxStackViaRange(lefiMaxStackVia * this,
				    const char *bottomLayer,
				    const char *topLayer)
{
  this->hasRange_ = 1;
  this->bottomLayer_ = (char *) lefMalloc(strlen(bottomLayer) + 1);
  strcpy(this->bottomLayer_, CASE(bottomLayer));
  this->topLayer_ = (char *) lefMalloc(strlen(topLayer) + 1);
  strcpy(this->topLayer_, CASE(topLayer));
}

int 
lefiMaxStackVia_maxStackVia(const lefiMaxStackVia * this)
{
  return this->value_;
}

int 
lefiMaxStackVia_hasMaxStackViaRange(const lefiMaxStackVia * this)
{
  return this->hasRange_ ? 1 : 0;
}

const char *
lefiMaxStackVia_maxStackViaBottomLayer(const lefiMaxStackVia * this)
{
  return this->bottomLayer_;
}

const char *
lefiMaxStackVia_maxStackViaTopLayer(const lefiMaxStackVia * this)
{
  return this->topLayer_;
}

void 
lefiMaxStackVia_print(const lefiMaxStackVia * this,
		      FILE * f)
{
  fprintf(f, "MAXVIASTACK %d", lefiMaxStackVia_maxStackVia(this));
  if (lefiMaxStackVia_hasMaxStackViaRange(this))
    fprintf(f, " RANGE %s %s", lefiMaxStackVia_maxStackViaBottomLayer(this), lefiMaxStackVia_maxStackViaTopLayer(this));
  fprintf(f, "\n");
}
