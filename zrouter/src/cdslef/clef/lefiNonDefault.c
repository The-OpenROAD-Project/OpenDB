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
#include "lefiNonDefault.h"
#include "lefiDebug.h"
#include "lefrCallBacks.h"

/*
 * 6/16/2000 - Wanda da Rosa
 *//* Make these variables in globals.  Can't use those defined *//* in the
 * class because it generates warning when it casts. *//* Can't assign for
 * example lefrViaCbkFnType to oldViaCbk_ *//* in the class because it
 * requires to include lefrReader.hpp *//* in lefiNonDefault.hpp.  But in
 * lefrReader.hpp, it requires *//* include lefiNonDefault.hpp, this creates
    a loop and is *//* problematic... */ static lefrViaCbkFnType oldViaCbk;

static lefrSpacingCbkFnType oldSpacingCbk;

static lefrVoidCbkFnType oldSpacingBeginCbk;

static lefrVoidCbkFnType oldSpacingEndCbk;

/*
 * lefiNonDefault
 */ lefiNonDefault *
lefiNonDefault_CreateUninitialized()
{
  lefiNonDefault *this = (lefiNonDefault *) malloc(sizeof(lefiNonDefault));

  lefiNonDefault_Init(this);
  return (this);
}

void 
lefiNonDefault_Init(lefiNonDefault * this)
{
  this->nameSize_ = 16;
  this->name_ = (char *) lefMalloc(16);
  this->layersAllocated_ = 2;
  this->numLayers_ = 0;
  this->layerName_ = (char **) lefMalloc(sizeof(char *) * 2);
  this->width_ = (double *) lefMalloc(sizeof(double) * 2);
  this->spacing_ = (double *) lefMalloc(sizeof(double) * 2);
  this->wireExtension_ = (double *) lefMalloc(sizeof(double) * 2);
  this->resistance_ = (double *) lefMalloc(sizeof(double) * 2);
  this->capacitance_ = (double *) lefMalloc(sizeof(double) * 2);
  this->edgeCap_ = (double *) lefMalloc(sizeof(double) * 2);
  this->hasWidth_ = (char *) lefMalloc(sizeof(char) * 2);
  this->hasSpacing_ = (char *) lefMalloc(sizeof(char) * 2);
  this->hasWireExtension_ = (char *) lefMalloc(sizeof(char) * 2);
  this->hasResistance_ = (char *) lefMalloc(sizeof(char) * 2);
  this->hasCapacitance_ = (char *) lefMalloc(sizeof(char) * 2);
  this->hasEdgeCap_ = (char *) lefMalloc(sizeof(char) * 2);
  this->allocatedVias_ = 2;
  this->numVias_ = 0;
  this->viaRules_ = (lefiVia **) lefMalloc(sizeof(lefiVia *) * 2);
  this->allocatedSpacing_ = 2;
  this->numSpacing_ = 0;
  this->spacingRules_ = (lefiSpacing **) lefMalloc(sizeof(lefiSpacing *) * 2);
  this->numProps_ = 0;
  this->propsAllocated_ = 1;
  this->names_ = (char **) lefMalloc(sizeof(char *));
  this->values_ = (char **) lefMalloc(sizeof(char *));
  this->dvalues_ = (double *) lefMalloc(sizeof(double));
  this->types_ = (char *) lefMalloc(sizeof(char));
}

void 
lefiNonDefault_Destroy(lefiNonDefault * this)
{
  lefiNonDefault_clear(this);
  lefFree(this->name_);
  lefFree((char *) (this->layerName_));
  lefFree((char *) (this->width_));
  lefFree((char *) (this->spacing_));
  lefFree((char *) (this->wireExtension_));
  lefFree((char *) (this->resistance_));
  lefFree((char *) (this->capacitance_));
  lefFree((char *) (this->edgeCap_));
  lefFree(this->hasWidth_);
  lefFree(this->hasSpacing_);
  lefFree(this->hasWireExtension_);
  lefFree(this->hasResistance_);
  lefFree(this->hasCapacitance_);
  lefFree(this->hasEdgeCap_);
  lefFree((char *) (this->viaRules_));
  lefFree((char *) (this->spacingRules_));
  lefFree((char *) (this->names_));
  lefFree((char *) (this->values_));
  lefFree((char *) (this->dvalues_));
  lefFree((char *) (this->types_));
}

void
lefiNonDefault_Delete(lefiNonDefault * this)
{
  if (this == NULL)
    return;
  lefiNonDefault_Destroy(this);
  free(this);
}

void 
lefiNonDefault_clear(lefiNonDefault * this)
{
  int     i;

  lefiSpacing *sr;

  lefiVia *vr;

  for (i = 0;
       i < this->numProps_;
       i++) {
    lefFree(this->names_[i]);
    lefFree(this->values_[i]);
    this->dvalues_[i] = 0;
  }
  this->numProps_ = 0;
  for (i = 0;
       i < this->numLayers_;
       i++) {
    lefFree(this->layerName_[i]);
    this->layerName_[i] = 0;
  }
  this->numLayers_ = 0;
  for (i = 0;
       i < this->numVias_;
       i++) {
    vr = this->viaRules_[i];
    lefiVia_Destroy(vr);
    lefFree((char *) (this->viaRules_[i]));
    this->viaRules_[i] = 0;
  }
  this->numVias_ = 0;
  for (i = 0;
       i < this->numSpacing_;
       i++) {
    sr = this->spacingRules_[i];
    lefiSpacing_Destroy(sr);
    lefFree((char *) (this->spacingRules_[i]));
    this->spacingRules_[i] = 0;
  }
  this->numSpacing_ = 0;
}

void 
lefiNonDefault_addViaRule(lefiNonDefault * this,
			  lefiVia * v)
{
  if (this->numVias_ == this->allocatedVias_) {
    int     i;

    lefiVia **nv;

    if (this->allocatedVias_ == 0)
      this->allocatedVias_ = 2;
    else
      this->allocatedVias_ *= 2;
    nv = (lefiVia **) lefMalloc(sizeof(lefiVia *) * this->allocatedVias_);
    for (i = 0;
	 i < this->numVias_;
	 i++) {
      nv[i] = this->viaRules_[i];
    }
    lefFree((char *) (this->viaRules_));
    this->viaRules_ = nv;
  }
  this->viaRules_[this->numVias_++] = lefiVia_clone(v);
}

void 
lefiNonDefault_addSpacingRule(lefiNonDefault * this,
			      lefiSpacing * s)
{
  if (this->numSpacing_ == this->allocatedSpacing_) {
    int     i;

    lefiSpacing **ns;

    if (this->allocatedSpacing_ == 0)
      this->allocatedSpacing_ = 2;
    else
      this->allocatedSpacing_ *= 2;
    ns = (lefiSpacing **) lefMalloc(sizeof(lefiSpacing *) * this->allocatedSpacing_);
    for (i = 0;
	 i < this->numSpacing_;
	 i++) {
      ns[i] = this->spacingRules_[i];
    }
    lefFree((char *) (this->spacingRules_));
    this->spacingRules_ = ns;
  }
  this->spacingRules_[this->numSpacing_++] = lefiSpacing_clone(s);
}

int 
lefiNonDefaultViaCbk(lefrCallbackType_e t, lefiVia * v, lefiUserData ud)
{
  lefiNonDefault *nd = (lefiNonDefault *) ud;

  if (t != lefrViaCbkType)
    lefiError("Bad nondefaultspacing callback");
  lefiNonDefault_addViaRule(nd,
			    v);
  return 0;
}

int 
lefiNonDefaultSpacingCbk(lefrCallbackType_e t, lefiSpacing * s, lefiUserData ud)
{
  lefiNonDefault *nd = (lefiNonDefault *) ud;

  if (t != lefrSpacingCbkType)
    lefiError("Bad nondefaultspacing callback");
  lefiNonDefault_addSpacingRule(nd,
				s);
  return 0;
}

void 
lefiNonDefault_setName(lefiNonDefault * this,
		       const char *name)
{
  int     len = strlen(name) + 1;

  lefiNonDefault_clear(this);
  this->oldUserData_ = lefrGetUserData();
  oldViaCbk = lefrViaCbk;
  oldSpacingCbk = lefrSpacingCbk;
  oldSpacingBeginCbk = lefrSpacingBeginCbk;
  oldSpacingEndCbk = lefrSpacingEndCbk;
  lefrViaCbk = lefiNonDefaultViaCbk;
  lefrSpacingCbk = lefiNonDefaultSpacingCbk;
  lefrSpacingBeginCbk = 0;
  lefrSpacingEndCbk = 0;
  lefrSetUserData((lefiUserData) this);
  if (len > this->nameSize_) {
    lefFree(this->name_);
    this->name_ = (char *) lefMalloc(len);
    this->nameSize_ = len;
  }
  strcpy(this->name_, CASE(name));
}

void 
lefiNonDefault_addLayer(lefiNonDefault * this,
			const char *name)
{
  int     len = strlen(name) + 1;

  if (this->numLayers_ == this->layersAllocated_) {
    int     i;

    char  **newl;

    double *neww;

    double *news;

    double *newe;

    double *newc;

    double *newr;

    double *newec;

    char   *newhw;

    char   *newhs;

    char   *newhe;

    char   *newhc;

    char   *newhr;

    char   *newhec;

    if (this->layersAllocated_ == 0)
      this->layersAllocated_ = 2;
    else
      this->layersAllocated_ *= 2;
    newl = (char **) lefMalloc(sizeof(char *) * this->layersAllocated_);
    newe = (double *) lefMalloc(sizeof(double) * this->layersAllocated_);
    neww = (double *) lefMalloc(sizeof(double) * this->layersAllocated_);
    news = (double *) lefMalloc(sizeof(double) * this->layersAllocated_);
    newc = (double *) lefMalloc(sizeof(double) * this->layersAllocated_);
    newr = (double *) lefMalloc(sizeof(double) * this->layersAllocated_);
    newec = (double *) lefMalloc(sizeof(double) * this->layersAllocated_);
    newhe = (char *) lefMalloc(sizeof(char) * this->layersAllocated_);
    newhw = (char *) lefMalloc(sizeof(char) * this->layersAllocated_);
    newhs = (char *) lefMalloc(sizeof(char) * this->layersAllocated_);
    newhc = (char *) lefMalloc(sizeof(char) * this->layersAllocated_);
    newhr = (char *) lefMalloc(sizeof(char) * this->layersAllocated_);
    newhec = (char *) lefMalloc(sizeof(char) * this->layersAllocated_);
    for (i = 0;
	 i < this->numLayers_;
	 i++) {
      newl[i] = this->layerName_[i];
      neww[i] = this->width_[i];
      news[i] = this->spacing_[i];
      newe[i] = this->wireExtension_[i];
      newc[i] = this->capacitance_[i];
      newr[i] = this->resistance_[i];
      newec[i] = this->edgeCap_[i];
      newhe[i] = this->hasWireExtension_[i];
      newhw[i] = this->hasWidth_[i];
      newhs[i] = this->hasSpacing_[i];
      newhc[i] = this->hasCapacitance_[i];
      newhr[i] = this->hasResistance_[i];
      newhec[i] = this->hasEdgeCap_[i];
    }
    lefFree((char *) (this->layerName_));
    lefFree((char *) (this->width_));
    lefFree((char *) (this->spacing_));
    lefFree((char *) (this->wireExtension_));
    lefFree((char *) (this->capacitance_));
    lefFree((char *) (this->resistance_));
    lefFree((char *) (this->edgeCap_));
    lefFree((char *) (this->hasWireExtension_));
    lefFree((char *) (this->hasWidth_));
    lefFree((char *) (this->hasSpacing_));
    lefFree((char *) (this->hasCapacitance_));
    lefFree((char *) (this->hasResistance_));
    lefFree((char *) (this->hasEdgeCap_));
    this->layerName_ = newl;
    this->width_ = neww;
    this->spacing_ = news;
    this->wireExtension_ = newe;
    this->capacitance_ = newc;
    this->resistance_ = newr;
    this->edgeCap_ = newec;
    this->hasWidth_ = newhw;
    this->hasSpacing_ = newhs;
    this->hasWireExtension_ = newhe;
    this->hasCapacitance_ = newhc;
    this->hasResistance_ = newhr;
    this->hasEdgeCap_ = newhec;
  }
  this->layerName_[this->numLayers_] = (char *) lefMalloc(len);
  strcpy(this->layerName_[this->numLayers_], CASE(name));
  this->width_[this->numLayers_] = 0.0;
  this->spacing_[this->numLayers_] = 0.0;
  this->wireExtension_[this->numLayers_] = 0.0;
  this->capacitance_[this->numLayers_] = 0.0;
  this->resistance_[this->numLayers_] = 0.0;
  this->edgeCap_[this->numLayers_] = 0.0;
  this->hasWidth_[this->numLayers_] = '\0';
  this->hasSpacing_[this->numLayers_] = '\0';
  this->hasWireExtension_[this->numLayers_] = '\0';
  this->hasCapacitance_[this->numLayers_] = '\0';
  this->hasResistance_[this->numLayers_] = '\0';
  this->hasEdgeCap_[this->numLayers_] = '\0';
  this->numLayers_ += 1;
}

void 
lefiNonDefault_addWidth(lefiNonDefault * this,
			double num)
{
  this->width_[this->numLayers_ - 1] = num;
  this->hasWidth_[this->numLayers_ - 1] = 1;
}

void 
lefiNonDefault_addSpacing(lefiNonDefault * this,
			  double num)
{
  this->spacing_[this->numLayers_ - 1] = num;
  this->hasSpacing_[this->numLayers_ - 1] = 1;
}

void 
lefiNonDefault_addWireExtension(lefiNonDefault * this,
				double num)
{
  this->wireExtension_[this->numLayers_ - 1] = num;
  this->hasWireExtension_[this->numLayers_ - 1] = 1;
}

void 
lefiNonDefault_addCapacitance(lefiNonDefault * this,
			      double num)
{
  this->capacitance_[this->numLayers_ - 1] = num;
  this->hasCapacitance_[this->numLayers_ - 1] = 1;
}

void 
lefiNonDefault_addResistance(lefiNonDefault * this,
			     double num)
{
  this->resistance_[this->numLayers_ - 1] = num;
  this->hasResistance_[this->numLayers_ - 1] = 1;
}

void 
lefiNonDefault_addEdgeCap(lefiNonDefault * this,
			  double num)
{
  this->edgeCap_[this->numLayers_ - 1] = num;
  this->hasEdgeCap_[this->numLayers_ - 1] = 1;
}

void 
lefiNonDefault_end(lefiNonDefault * this)
{
  lefrSetViaCbk(oldViaCbk);
  lefrSetSpacingCbk(oldSpacingCbk);
  lefrSetSpacingBeginCbk(oldSpacingBeginCbk);
  lefrSetSpacingEndCbk(oldSpacingEndCbk);
  lefrSetUserData(this->oldUserData_);
}

int 
lefiNonDefault_numLayers(const lefiNonDefault * this)
{
  return this->numLayers_;
}

const char *
lefiNonDefault_layerName(const lefiNonDefault * this,
			 int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0;
  }
  return this->layerName_[index];
}

int 
lefiNonDefault_hasLayerWidth(const lefiNonDefault * this,
			     int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0;
  }
  return this->hasWidth_[index];
}

double 
lefiNonDefault_layerWidth(const lefiNonDefault * this,
			  int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0.0;
  }
  return this->width_[index];
}

int 
lefiNonDefault_hasLayerWireExtension(const lefiNonDefault * this,
				     int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0;
  }
  return this->hasWireExtension_[index];
}

int 
lefiNonDefault_hasLayerSpacing(const lefiNonDefault * this,
			       int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0;
  }
  return this->hasSpacing_[index];
}

double 
lefiNonDefault_layerWireExtension(const lefiNonDefault * this,
				  int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0.0;
  }
  return this->wireExtension_[index];
}

double 
lefiNonDefault_layerSpacing(const lefiNonDefault * this,
			    int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0.0;
  }
  return this->spacing_[index];
}

int 
lefiNonDefault_hasLayerResistance(const lefiNonDefault * this,
				  int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0;
  }
  return this->hasResistance_[index];
}

double 
lefiNonDefault_layerResistance(const lefiNonDefault * this,
			       int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0.0;
  }
  return this->resistance_[index];
}

int 
lefiNonDefault_hasLayerCapacitance(const lefiNonDefault * this,
				   int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0;
  }
  return this->hasCapacitance_[index];
}

double 
lefiNonDefault_layerCapacitance(const lefiNonDefault * this,
				int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0.0;
  }
  return this->capacitance_[index];
}

int 
lefiNonDefault_hasLayerEdgeCap(const lefiNonDefault * this,
			       int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0;
  }
  return this->hasEdgeCap_[index];
}

double 
lefiNonDefault_layerEdgeCap(const lefiNonDefault * this,
			    int index)
{
  if (index < 0 || index >= this->numLayers_) {
    lefiError("Bad index in non default");
    return 0.0;
  }
  return this->edgeCap_[index];
}

int 
lefiNonDefault_numVias(const lefiNonDefault * this)
{
  return this->numVias_;
}

lefiVia *
lefiNonDefault_viaRule(const lefiNonDefault * this,
		       int index)
{
  if (index < 0 || index >= this->numVias_) {
    lefiError("Bad index in non default");
    return 0;
  }
  return this->viaRules_[index];
}

int 
lefiNonDefault_numSpacingRules(const lefiNonDefault * this)
{
  return this->numSpacing_;
}

lefiSpacing *
lefiNonDefault_spacingRule(const lefiNonDefault * this,
			   int index)
{
  if (index < 0 || index >= this->numSpacing_) {
    lefiError("Bad index in non default");
    return 0;
  }
  return this->spacingRules_[index];
}

const char *
lefiNonDefault_name(const lefiNonDefault * this)
{
  return this->name_;
}

void 
lefiNonDefault_print(lefiNonDefault * this,
		     FILE * f)
{
  int     i;

  lefiSpacing *s;

  lefiVia *v;

  fprintf(f, "Nondefault rule %s\n", lefiNonDefault_name(this));
  fprintf(f, "%d layers %d vias %d spacing rules\n", lefiNonDefault_numLayers(this), lefiNonDefault_numVias(this), lefiNonDefault_numSpacingRules(this));
  for (i = 0;
       i < lefiNonDefault_numLayers(this);
       i++) {
    fprintf(f, " Layer %s\n", lefiNonDefault_layerName(this,
						       i));
    if (lefiNonDefault_hasLayerWidth(this,
				     i))
      fprintf(f, " WIDTH %g\n", lefiNonDefault_layerWidth(this,
							  i));
    if (lefiNonDefault_hasLayerSpacing(this,
				       i))
      fprintf(f, " SPACING %g\n", lefiNonDefault_layerSpacing(this,
							      i));
    if (lefiNonDefault_hasLayerWireExtension(this,
					     i))
      fprintf(f, " WIREEXTENSION %g", lefiNonDefault_layerWireExtension(this,
									i));
    if (lefiNonDefault_hasLayerResistance(this,
					  i))
      fprintf(f, " RESISTANCE RPERSQ %g\n", lefiNonDefault_layerResistance(this,
									i));
    if (lefiNonDefault_hasLayerCapacitance(this,
					   i))
      fprintf(f, " CAPACITANCE CPERSQDIST %g\n", lefiNonDefault_layerCapacitance(this,
									i));
    if (lefiNonDefault_hasLayerEdgeCap(this,
				       i))
      fprintf(f, " EDGECAPACITANCE %g\n", lefiNonDefault_layerEdgeCap(this,
								      i));
  }
  for (i = 0;
       i < lefiNonDefault_numVias(this);
       i++) {
    v = lefiNonDefault_viaRule(this,
			       i);
    lefiVia_print(v,
		  f);
  }
  for (i = 0;
       i < lefiNonDefault_numSpacingRules(this);
       i++) {
    s = lefiNonDefault_spacingRule(this,
				   i);
    lefiSpacing_print(s,
		      f);
  }
}

int 
lefiNonDefault_numProps(const lefiNonDefault * this)
{
  return this->numProps_;
}

void 
lefiNonDefault_addProp(lefiNonDefault * this,
		       const char *name,
		       const char *value,
		       const char type)
{
  int     len = strlen(name) + 1;

  if (this->numProps_ == this->propsAllocated_) {
    int     i;

    int     max;

    int     lim = this->numProps_;

    char  **nn;

    char  **nv;

    double *nd;

    char   *nt;

    if (this->propsAllocated_ == 0)
      max = this->propsAllocated_ = 2;
    else
      max = this->propsAllocated_ *= 2;
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
lefiNonDefault_addNumProp(lefiNonDefault * this,
			  const char *name,
			  const double d,
			  const char *value,
			  const char type)
{
  int     len = strlen(name) + 1;

  if (this->numProps_ == this->propsAllocated_) {
    int     i;

    int     max;

    int     lim = this->numProps_;

    char  **nn;

    char  **nv;

    double *nd;

    char   *nt;

    max = this->propsAllocated_ *= 2;
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
lefiNonDefault_propName(const lefiNonDefault * this,
			int index)
{
  if (index < 0 || index >= this->numProps_) {
    lefiError("bad index for nondefault property");
    return 0;
  }
  return this->names_[index];
}

const char *
lefiNonDefault_propValue(const lefiNonDefault * this,
			 int index)
{
  if (index < 0 || index >= this->numProps_) {
    lefiError("bad index for nondefault property");
    return 0;
  }
  return this->values_[index];
}

double 
lefiNonDefault_propNumber(const lefiNonDefault * this,
			  int index)
{
  if (index < 0 || index >= this->numProps_) {
    lefiError("bad index for nondefault property");
    return 0;
  }
  return this->dvalues_[index];
}

const char 
lefiNonDefault_propType(const lefiNonDefault * this,
			int index)
{
  if (index < 0 || index >= this->numProps_) {
    lefiError("bad index for nondefault property");
    return 0;
  }
  return this->types_[index];
}

int 
lefiNonDefault_propIsNumber(const lefiNonDefault * this,
			    int index)
{
  if (index < 0 || index >= this->numProps_) {
    lefiError("bad index for nondefault property");
    return 0;
  }
  return this->dvalues_[index] ? 1 : 0;
}

int 
lefiNonDefault_propIsString(const lefiNonDefault * this,
			    int index)
{
  if (index < 0 || index >= this->numProps_) {
    lefiError("bad index for nondefault property");
    return 0;
  }
  return this->dvalues_[index] ? 0 : 1;
}
