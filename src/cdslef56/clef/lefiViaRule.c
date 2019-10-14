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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "lefiViaRule.h"
#include "lefiDebug.h"

/*
 * lefiViaRuleLayer
 */ lefiViaRuleLayer *
lefiViaRuleLayer_CreateUninitialized()
{
  lefiViaRuleLayer *this = (lefiViaRuleLayer *) malloc(sizeof(lefiViaRuleLayer));

  lefiViaRuleLayer_Init(this);
  return (this);
}

void 
lefiViaRuleLayer_Init(lefiViaRuleLayer * this)
{
  this->name_ = 0;
  this->overhang1_ = -1;
  this->overhang2_ = -1;
}

void 
lefiViaRuleLayer_clearLayerOverhang(lefiViaRuleLayer * this)
{
  this->overhang1_ = -1;
  this->overhang2_ = -1;
}

void 
lefiViaRuleLayer_setName(lefiViaRuleLayer * this,
			 const char *name)
{
  int     len = strlen(name) + 1;

  if (this->name_)
    lefFree(this->name_);
  this->name_ = (char *) lefMalloc(len);
  strcpy(this->name_, CASE(name));
  this->direction_ = '\0';
  this->hasWidth_ = 0;
  this->hasResistance_ = 0;
  this->hasOverhang_ = 0;
  this->hasMetalOverhang_ = 0;
  this->hasSpacing_ = 0;
  this->hasRect_ = 0;
}

void 
lefiViaRuleLayer_Destroy(lefiViaRuleLayer * this)
{
  if (this->name_)
    lefFree(this->name_);
}

void
lefiViaRuleLayer_Delete(lefiViaRuleLayer * this)
{
  if (this == NULL)
    return;
  free(this);
}

void 
lefiViaRuleLayer_setHorizontal(lefiViaRuleLayer * this)
{
  this->direction_ = 'H';
}

void 
lefiViaRuleLayer_setVertical(lefiViaRuleLayer * this)
{
  this->direction_ = 'V';
}

/*
 * 5.5
 */ void 
lefiViaRuleLayer_setEnclosure(lefiViaRuleLayer * this,
			      double overhang1,
			      double overhang2)
{
  this->overhang1_ = overhang1;
  this->overhang2_ = overhang2;
}

void 
lefiViaRuleLayer_setWidth(lefiViaRuleLayer * this,
			  double minW,
			  double maxW)
{
  this->hasWidth_ = 1;
  this->widthMin_ = minW;
  this->widthMax_ = maxW;
}

void 
lefiViaRuleLayer_setOverhang(lefiViaRuleLayer * this,
			     double d)
{
  this->hasOverhang_ = 1;
  this->overhang_ = d;
}

/*
 * 5.6
 */ void 
lefiViaRuleLayer_setOverhangToEnclosure(lefiViaRuleLayer * this,
					double d)
{
  if ((this->overhang1_ != -1) && (this->overhang2_ != -1))
    return;
  if (this->overhang1_ == -1)
    this->overhang1_ = d;
  else
    this->overhang2_ = d;
  return;
}

/*
 * 5.5
 */ void 
lefiViaRuleLayer_setMetalOverhang(lefiViaRuleLayer * this,
				  double d)
{
  this->hasMetalOverhang_ = 1;
  this->metalOverhang_ = d;
}

void 
lefiViaRuleLayer_setResistance(lefiViaRuleLayer * this,
			       double d)
{
  this->hasResistance_ = 1;
  this->resistance_ = d;
}

void 
lefiViaRuleLayer_setSpacing(lefiViaRuleLayer * this,
			    double x,
			    double y)
{
  this->hasSpacing_ = 1;
  this->spacingStepX_ = x;
  this->spacingStepY_ = y;
}

void 
lefiViaRuleLayer_setRect(lefiViaRuleLayer * this,
			 double xl,
			 double yl,
			 double xh,
			 double yh)
{
  this->hasRect_ = 1;
  this->xl_ = xl;
  this->yl_ = yl;
  this->xh_ = xh;
  this->yh_ = yh;
}

int 
lefiViaRuleLayer_hasRect(const lefiViaRuleLayer * this)
{
  return this->hasRect_;
}

int 
lefiViaRuleLayer_hasDirection(const lefiViaRuleLayer * this)
{
  return this->direction_ ? 1 : 0;
}

/*
 * 5.5
 */ int 
lefiViaRuleLayer_hasEnclosure(const lefiViaRuleLayer * this)
{
  return this->overhang1_ == -1 ? 0 : 1;
}

int 
lefiViaRuleLayer_hasWidth(const lefiViaRuleLayer * this)
{
  return this->hasWidth_;
}

int 
lefiViaRuleLayer_hasResistance(const lefiViaRuleLayer * this)
{
  return this->hasResistance_;
}

int 
lefiViaRuleLayer_hasOverhang(const lefiViaRuleLayer * this)
{
  return this->hasOverhang_;
}

int 
lefiViaRuleLayer_hasMetalOverhang(const lefiViaRuleLayer * this)
{
  return this->hasMetalOverhang_;
}

int 
lefiViaRuleLayer_hasSpacing(const lefiViaRuleLayer * this)
{
  return this->hasSpacing_;
}

char   *
lefiViaRuleLayer_name(const lefiViaRuleLayer * this)
{
  return this->name_;
}

int 
lefiViaRuleLayer_isHorizontal(const lefiViaRuleLayer * this)
{
  return this->direction_ == 'H' ? 1 : 0;
}

int 
lefiViaRuleLayer_isVertical(const lefiViaRuleLayer * this)
{
  return this->direction_ == 'V' ? 1 : 0;
}

/*
 * 5.5
 */ double 
lefiViaRuleLayer_enclosureOverhang1(const lefiViaRuleLayer * this)
{
  return this->overhang1_;
}

/*
 * 5.5
 */ double 
lefiViaRuleLayer_enclosureOverhang2(const lefiViaRuleLayer * this)
{
  return this->overhang2_;
}

double 
lefiViaRuleLayer_widthMin(const lefiViaRuleLayer * this)
{
  return this->widthMin_;
}

double 
lefiViaRuleLayer_widthMax(const lefiViaRuleLayer * this)
{
  return this->widthMax_;
}

double 
lefiViaRuleLayer_overhang(const lefiViaRuleLayer * this)
{
  return this->overhang_;
}

double 
lefiViaRuleLayer_metalOverhang(const lefiViaRuleLayer * this)
{
  return this->metalOverhang_;
}

double 
lefiViaRuleLayer_resistance(const lefiViaRuleLayer * this)
{
  return this->resistance_;
}

double 
lefiViaRuleLayer_spacingStepX(const lefiViaRuleLayer * this)
{
  return this->spacingStepX_;
}

double 
lefiViaRuleLayer_spacingStepY(const lefiViaRuleLayer * this)
{
  return this->spacingStepY_;
}

double 
lefiViaRuleLayer_xl(const lefiViaRuleLayer * this)
{
  return this->xl_;
}

double 
lefiViaRuleLayer_yl(const lefiViaRuleLayer * this)
{
  return this->yl_;
}

double 
lefiViaRuleLayer_xh(const lefiViaRuleLayer * this)
{
  return this->xh_;
}

double 
lefiViaRuleLayer_yh(const lefiViaRuleLayer * this)
{
  return this->yh_;
}

void 
lefiViaRuleLayer_print(const lefiViaRuleLayer * this,
		       FILE * f)
{
  fprintf(f, " Layer %s", this->name_);
  if (lefiViaRuleLayer_isHorizontal(this))
    fprintf(f, " HORIZONTAL");
  if (lefiViaRuleLayer_isVertical(this))
    fprintf(f, " VERTICAL");
  fprintf(f, "\n");
  if (lefiViaRuleLayer_hasWidth(this))
    fprintf(f, " WIDTH %g %g\n", lefiViaRuleLayer_widthMin(this), lefiViaRuleLayer_widthMax(this));
  if (lefiViaRuleLayer_hasResistance(this))
    fprintf(f, " RESISTANCE %g\n", lefiViaRuleLayer_resistance(this));
  if (lefiViaRuleLayer_hasOverhang(this))
    fprintf(f, " OVERHANG %g\n", lefiViaRuleLayer_overhang(this));
  if (lefiViaRuleLayer_hasMetalOverhang(this))
    fprintf(f, " METALOVERHANG %g\n", lefiViaRuleLayer_metalOverhang(this));
  if (lefiViaRuleLayer_hasSpacing(this))
    fprintf(f, " SPACING %g %g\n", lefiViaRuleLayer_spacingStepX(this), lefiViaRuleLayer_spacingStepY(this));
  if (lefiViaRuleLayer_hasRect(this))
    fprintf(f, " RECT %g,%g %g,%g\n", lefiViaRuleLayer_xl(this), lefiViaRuleLayer_yl(this), lefiViaRuleLayer_xh(this), lefiViaRuleLayer_yh(this));
}

/*
 * lefiViaRule
 */ lefiViaRule *
lefiViaRule_CreateUninitialized()
{
  lefiViaRule *this = (lefiViaRule *) malloc(sizeof(lefiViaRule));

  lefiViaRule_Init(this);
  return (this);
}

void 
lefiViaRule_Init(lefiViaRule * this)
{
  this->nameSize_ = 16;
  this->name_ = (char *) lefMalloc(16);
  this->viasAllocated_ = 2;
  this->vias_ = (char **) lefMalloc(sizeof(char *) * 2);
  lefiViaRuleLayer_Init(&(this->layers_[0]));
  lefiViaRuleLayer_Init(&(this->layers_[1]));
  lefiViaRuleLayer_Init(&(this->layers_[2]));
  this->numLayers_ = 0;
  this->numVias_ = 0;
  this->numProps_ = 0;
  this->propsAllocated_ = 1;
  this->names_ = (char **) lefMalloc(sizeof(char *));
  this->values_ = (char **) lefMalloc(sizeof(char *));
  this->dvalues_ = (double *) lefMalloc(sizeof(double));
  this->types_ = (char *) lefMalloc(sizeof(char));
}

void 
lefiViaRule_clear(lefiViaRule * this)
{
  int     i;

  this->hasGenerate_ = 0;
  this->hasDefault_ = 0;
  for (i = 0;
       i < this->numProps_;
       i++) {
    lefFree(this->names_[i]);
    lefFree(this->values_[i]);
    this->dvalues_[i] = 0;
  }
  this->numProps_ = 0;
  this->numLayers_ = 0;
  for (i = 0;
       i < this->numVias_;
       i++) {
    lefFree(this->vias_[i]);
  }
  this->numVias_ = 0;
}

void 
lefiViaRule_clearLayerOverhang(lefiViaRule * this)
{
  lefiViaRuleLayer_clearLayerOverhang(&(this->layers_[0]));
  lefiViaRuleLayer_clearLayerOverhang(&(this->layers_[1]));
}

void 
lefiViaRule_setName(lefiViaRule * this,
		    const char *name)
{
  int     len = strlen(name) + 1;

  if (len > this->nameSize_) {
    lefFree(this->name_);
    this->name_ = (char *) lefMalloc(len);
    this->nameSize_ = len;
  }
  strcpy(this->name_, CASE(name));
  lefiViaRule_clear(this);
}

void 
lefiViaRule_Destroy(lefiViaRule * this)
{
  lefiViaRule_clear(this);
  lefFree(this->name_);
  lefFree((char *) (this->vias_));
  lefFree((char *) (this->names_));
  lefFree((char *) (this->values_));
  lefFree((char *) (this->dvalues_));
  lefFree((char *) (this->types_));
  lefiViaRuleLayer_Destroy(&(this->layers_[0]));
  lefiViaRuleLayer_Destroy(&(this->layers_[1]));
  lefiViaRuleLayer_Destroy(&(this->layers_[2]));
}

void
lefiViaRule_Delete(lefiViaRule * this)
{
  if (this == NULL)
    return;
  lefiViaRule_Destroy(this);
  free(this);
}

void 
lefiViaRule_setGenerate(lefiViaRule * this)
{
  this->hasGenerate_ = 1;
}

void 
lefiViaRule_setDefault(lefiViaRule * this)
{
  this->hasDefault_ = 1;
}

void 
lefiViaRule_addViaName(lefiViaRule * this,
		       const char *name)
{
  int     len = strlen(name) + 1;

  if (this->numVias_ == this->viasAllocated_) {
    int     i;

    char  **nn;

    if (this->viasAllocated_ == 0)
      this->viasAllocated_ = 2;
    else
      this->viasAllocated_ *= 2;
    nn = (char **) lefMalloc(sizeof(char *) * this->viasAllocated_);
    for (i = 0;
	 i < this->numVias_;
	 i++)
      nn[i] = this->vias_[i];
    lefFree((char *) (this->vias_));
    this->vias_ = nn;
  }
  this->vias_[this->numVias_] = (char *) lefMalloc(len);
  strcpy(this->vias_[this->numVias_], CASE(name));
  this->numVias_ += 1;
}

void 
lefiViaRule_setRect(lefiViaRule * this,
		    double xl,
		    double yl,
		    double xh,
		    double yh)
{
  lefiViaRuleLayer_setRect(&(this->layers_[this->numLayers_ - 1]),
			   xl,
			   yl,
			   xh,
			   yh);
}

void 
lefiViaRule_setSpacing(lefiViaRule * this,
		       double x,
		       double y)
{
  lefiViaRuleLayer_setSpacing(&(this->layers_[this->numLayers_ - 1]),
			      x,
			      y);
}

void 
lefiViaRule_setWidth(lefiViaRule * this,
		     double x,
		     double y)
{
  lefiViaRuleLayer_setWidth(&(this->layers_[this->numLayers_ - 1]),
			    x,
			    y);
}

void 
lefiViaRule_setResistance(lefiViaRule * this,
			  double d)
{
  lefiViaRuleLayer_setResistance(&(this->layers_[this->numLayers_ - 1]),
				 d);
}

void 
lefiViaRule_setOverhang(lefiViaRule * this,
			double d)
{
  lefiViaRuleLayer_setOverhang(&(this->layers_[this->numLayers_ - 1]),
			       d);
}

/*
 * 5.6, try to set value to layers_[0] & layers_[1]
 */ void 
lefiViaRule_setOverhangToEnclosure(lefiViaRule * this,
				   double d)
{
  lefiViaRuleLayer_setOverhangToEnclosure(&(this->layers_[0]),
					  d);
  lefiViaRuleLayer_setOverhangToEnclosure(&(this->layers_[1]),
					  d);
}

void 
lefiViaRule_setMetalOverhang(lefiViaRule * this,
			     double d)
{
  lefiViaRuleLayer_setMetalOverhang(&(this->layers_[this->numLayers_ - 1]),
				    d);
}

void 
lefiViaRule_setVertical(lefiViaRule * this)
{
  lefiViaRuleLayer_setVertical(&(this->layers_[this->numLayers_ - 1]));
}

void 
lefiViaRule_setHorizontal(lefiViaRule * this)
{
  lefiViaRuleLayer_setHorizontal(&(this->layers_[this->numLayers_ - 1]));
}

void 
lefiViaRule_setEnclosure(lefiViaRule * this,
			 double overhang1,
			 double overhang2)
{
  lefiViaRuleLayer_setEnclosure(&(this->layers_[this->numLayers_ - 1]),
				overhang1,
				overhang2);
}

void 
lefiViaRule_setLayer(lefiViaRule * this,
		     const char *name)
{
  if (this->numLayers_ == 3) {
    lefiError("ERROR: too many via rule layers");
    return;
  }
  lefiViaRuleLayer_setName(&(this->layers_[this->numLayers_]),
			   name);
  this->numLayers_ += 1;
}

int 
lefiViaRule_hasGenerate(const lefiViaRule * this)
{
  return this->hasGenerate_;
}

int 
lefiViaRule_hasDefault(const lefiViaRule * this)
{
  return this->hasDefault_;
}

int 
lefiViaRule_numLayers(const lefiViaRule * this)
{
  return this->numLayers_;
}

lefiViaRuleLayer *
lefiViaRule_layer(lefiViaRule * this,
		  int index)
{
  if (index < 0 || index > 2)
    return 0;
  return &(this->layers_[index]);
}

char   *
lefiViaRule_name(const lefiViaRule * this)
{
  return this->name_;
}

void 
lefiViaRule_print(const lefiViaRule * this,
		  FILE * f)
{
  int     i;

  fprintf(f, "VIA RULE %s", lefiViaRule_name(this));
  if (lefiViaRule_hasGenerate(this))
    fprintf(f, " GENERATE");
  fprintf(f, "\n");
  for (i = 0;
       i < lefiViaRule_numLayers(this);
       i++) {
    lefiViaRuleLayer_print(&(this->layers_[i]),
			   f);
  }
  for (i = 0;
       i < lefiViaRule_numVias(this);
       i++) {
    fprintf(f, " Via %s\n", lefiViaRule_viaName(this,
						i));
  }
}

int 
lefiViaRule_numVias(const lefiViaRule * this)
{
  return this->numVias_;
}

char   *
lefiViaRule_viaName(const lefiViaRule * this,
		    int index)
{
  if (index < 0 || index >= this->numVias_)
    return 0;
  return this->vias_[index];
}

int 
lefiViaRule_numProps(const lefiViaRule * this)
{
  return this->numProps_;
}

void 
lefiViaRule_addProp(lefiViaRule * this,
		    const char *name,
		    const char *value,
		    const char type)
{
  int     len = strlen(name) + 1;

  if (this->numProps_ == this->propsAllocated_) {
    int     i;

    int     max;

    int     lim;

    char  **nn;

    char  **nv;

    double *nd;

    char   *nt;

    if (this->propsAllocated_ == 0)
      this->propsAllocated_ = 1;
    max = this->propsAllocated_ *= 2;
    lim = this->numProps_;
    nn = (char **) lefMalloc(sizeof(char *) * max);
    nv = (char **) lefMalloc(sizeof(char *) * max);
    nd = (double *) lefMalloc(sizeof(double) * max);
    nt = (char *) lefMalloc(sizeof(char) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nn[i] = this->names_[i];
      nv[i] = this->values_[i];
      nd[i] = this->dvalues_[i];
      nt[i] = this->types_[i];
    }
    lefFree((char *) (this->names_));
    lefFree((char *) (this->values_));
    lefFree((char *) (this->dvalues_));
    lefFree((char *) (this->types_));
    this->names_ = nn;
    this->values_ = nv;
    this->dvalues_ = nd;
    this->types_ = nt;
  }
  this->names_[this->numProps_] = (char *) lefMalloc(sizeof(char) * len);
  strcpy(this->names_[this->numProps_], name);
  len = strlen(value) + 1;
  this->values_[this->numProps_] = (char *) lefMalloc(sizeof(char) * len);
  strcpy(this->values_[this->numProps_], value);
  this->dvalues_[this->numProps_] = 0;
  this->types_[this->numProps_] = type;
  this->numProps_ += 1;
}

void 
lefiViaRule_addNumProp(lefiViaRule * this,
		       const char *name,
		       const double d,
		       const char *value,
		       const char type)
{
  int     len = strlen(name) + 1;

  if (this->numProps_ == this->propsAllocated_) {
    int     i;

    int     max;

    int     lim;

    char  **nn;

    char  **nv;

    double *nd;

    char   *nt;

    if (this->propsAllocated_ == 0)
      this->propsAllocated_ = 1;
    max = this->propsAllocated_ *= 2;
    lim = this->numProps_;
    nn = (char **) lefMalloc(sizeof(char *) * max);
    nv = (char **) lefMalloc(sizeof(char *) * max);
    nd = (double *) lefMalloc(sizeof(double) * max);
    nt = (char *) lefMalloc(sizeof(char) * max);
    for (i = 0;
	 i < lim;
	 i++) {
      nn[i] = this->names_[i];
      nv[i] = this->values_[i];
      nd[i] = this->dvalues_[i];
      nt[i] = this->types_[i];
    }
    lefFree((char *) (this->names_));
    lefFree((char *) (this->values_));
    lefFree((char *) (this->dvalues_));
    lefFree((char *) (this->types_));
    this->names_ = nn;
    this->values_ = nv;
    this->dvalues_ = nd;
    this->types_ = nt;
  }
  this->names_[this->numProps_] = (char *) lefMalloc(sizeof(char) * len);
  strcpy(this->names_[this->numProps_], name);
  len = strlen(value) + 1;
  this->values_[this->numProps_] = (char *) lefMalloc(sizeof(char) * len);
  strcpy(this->values_[this->numProps_], value);
  this->dvalues_[this->numProps_] = d;
  this->types_[this->numProps_] = type;
  this->numProps_ += 1;
}

const char *
lefiViaRule_propName(const lefiViaRule * this,
		     int i)
{
  if (i < 0 || i >= this->numProps_) {
    lefiError("bad index for viarule property");
    return 0;
  }
  return this->names_[i];
}

const char *
lefiViaRule_propValue(const lefiViaRule * this,
		      int i)
{
  if (i < 0 || i >= this->numProps_) {
    lefiError("bad index for viarule property");
    return 0;
  }
  return this->values_[i];
}

double 
lefiViaRule_propNumber(const lefiViaRule * this,
		       int i)
{
  if (i < 0 || i >= this->numProps_) {
    lefiError("bad index for viarule property");
    return 0;
  }
  return this->dvalues_[i];
}

const char 
lefiViaRule_propType(const lefiViaRule * this,
		     int i)
{
  if (i < 0 || i >= this->numProps_) {
    lefiError("bad index for viarule property");
    return 0;
  }
  return this->types_[i];
}

int 
lefiViaRule_propIsNumber(const lefiViaRule * this,
			 int i)
{
  if (i < 0 || i >= this->numProps_) {
    lefiError("bad index for viarule property");
    return 0;
  }
  return this->dvalues_[i] ? 1 : 0;
}

int 
lefiViaRule_propIsString(const lefiViaRule * this,
			 int i)
{
  if (i < 0 || i >= this->numProps_) {
    lefiError("bad index for viarule property");
    return 0;
  }
  return this->dvalues_[i] ? 0 : 1;
}
