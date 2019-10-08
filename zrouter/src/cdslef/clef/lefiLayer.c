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
#include "lefiLayer.h"
#include "lefiDebug.h"

/*
 * lefiAntennaPWL
 */ lefiAntennaPWL *
lefiAntennaPWL_CreateUninitialized()
{
  lefiAntennaPWL *this = (lefiAntennaPWL *) malloc(sizeof(lefiAntennaPWL));

  lefiAntennaPWL_Init(this);
  return (this);
}

void 
lefiAntennaPWL_Init(lefiAntennaPWL * this)
{
  this->numAlloc_ = 2;
  this->d_ = (double *) lefMalloc(sizeof(double) * 2);
  this->r_ = (double *) lefMalloc(sizeof(double) * 2);
  lefiAntennaPWL_clear(this);
}

void
lefiAntennaPWL_Delete(lefiAntennaPWL * this)
{
  if (this == NULL)
    return;
  lefiAntennaPWL_Destroy(this);
  free(this);
}

void 
lefiAntennaPWL_Destroy(lefiAntennaPWL * this)
{
  if (this->d_)
    lefFree((char *) (this->d_));
  if (this->r_)
    lefFree((char *) (this->r_));
}

/*
 * Clear will reset the numPWL_ to zero but keep array allocated
 */ void 
lefiAntennaPWL_clear(lefiAntennaPWL * this)
{
  this->numPWL_ = 0;
}

void 
lefiAntennaPWL_addAntennaPWL(lefiAntennaPWL * this,
			     double d,
			     double r)
{
  if (this->numPWL_ == this->numAlloc_) {
    int     i, len;

    double *nd;

    double *nr;

    if (this->numAlloc_ == 0)
      len = this->numAlloc_ = 2;
    else
      len = this->numAlloc_ *= 2;
    nd = (double *) lefMalloc(sizeof(double) * len);
    nr = (double *) lefMalloc(sizeof(double) * len);
    for (i = 0;
	 i < this->numPWL_;
	 i++) {
      nd[i] = this->d_[i];
      nr[i] = this->r_[i];
    }
    lefFree((char *) (this->d_));
    lefFree((char *) (this->r_));
    this->d_ = nd;
    this->r_ = nr;
  }
  this->d_[this->numPWL_] = d;
  this->r_[this->numPWL_] = r;
  this->numPWL_ += 1;
}

int 
lefiAntennaPWL_numPWL(const lefiAntennaPWL * this)
{
  return this->numPWL_;
}

double 
lefiAntennaPWL_PWLdiffusion(lefiAntennaPWL * this,
			    int index)
{
  if (index < 0 || index >= this->numPWL_)
    return 0;
  return this->d_[index];
}

double 
lefiAntennaPWL_PWLratio(lefiAntennaPWL * this,
			int index)
{
  if (index < 0 || index >= this->numPWL_)
    return 0;
  return this->r_[index];
}

/*
 * lefiLayerDensity
 */ lefiLayerDensity *
lefiLayerDensity_CreateUninitialized()
{
  lefiLayerDensity *this = (lefiLayerDensity *) malloc(sizeof(lefiLayerDensity));

  return (this);
}

void 
lefiLayerDensity_Init(lefiLayerDensity * this,
		      const char *type)
{
  int     len = strlen(type) + 1;

  this->type_ = (char *) lefMalloc(len);
  strcpy(this->type_, CASE(type));
  this->oneEntry_ = 0;
  this->numFrequency_ = 0;
  this->frequency_ = 0;
  this->numWidths_ = 0;
  this->widths_ = 0;
  this->numTableEntries_ = 0;
  this->tableEntries_ = 0;
  this->numCutareas_ = 0;
  this->cutareas_ = 0;
}

void 
lefiLayerDensity_Destroy(lefiLayerDensity * this)
{
  if (this->type_)
    lefFree((char *) (this->type_));
  if (this->frequency_)
    lefFree((char *) (this->frequency_));
  if (this->widths_)
    lefFree((char *) (this->widths_));
  if (this->tableEntries_)
    lefFree((char *) (this->tableEntries_));
  if (this->cutareas_)
    lefFree((char *) (this->cutareas_));
}

void
lefiLayerDensity_Delete(lefiLayerDensity * this)
{
  if (this == NULL)
    return;
  lefiLayerDensity_Destroy(this);
  free(this);
}

void 
lefiLayerDensity_setOneEntry(lefiLayerDensity * this,
			     double entry)
{
  this->oneEntry_ = entry;
}

void 
lefiLayerDensity_addFrequency(lefiLayerDensity * this,
			      int num,
			      double *frequency)
{
  this->numFrequency_ = num;
  this->frequency_ = frequency;
}

void 
lefiLayerDensity_addWidth(lefiLayerDensity * this,
			  int num,
			  double *width)
{
  this->numWidths_ = num;
  this->widths_ = width;
}

void 
lefiLayerDensity_addTableEntry(lefiLayerDensity * this,
			       int num,
			       double *entry)
{
  this->numTableEntries_ = num;
  this->tableEntries_ = entry;
}

void 
lefiLayerDensity_addCutarea(lefiLayerDensity * this,
			    int num,
			    double *cutarea)
{
  this->numCutareas_ = num;
  this->cutareas_ = cutarea;
}

int 
lefiLayerDensity_hasOneEntry(lefiLayerDensity * this)
{
  return this->oneEntry_ ? 1 : 0;
}

char   *
lefiLayerDensity_type(lefiLayerDensity * this)
{
  return this->type_;
}

double 
lefiLayerDensity_oneEntry(lefiLayerDensity * this)
{
  return this->oneEntry_;
}

int 
lefiLayerDensity_numFrequency(lefiLayerDensity * this)
{
  return this->numFrequency_;
}

double 
lefiLayerDensity_frequency(lefiLayerDensity * this,
			   int index)
{
  return this->frequency_[index];
}

int 
lefiLayerDensity_numWidths(lefiLayerDensity * this)
{
  return this->numWidths_;
}

double 
lefiLayerDensity_width(lefiLayerDensity * this,
		       int index)
{
  return this->widths_[index];
}

int 
lefiLayerDensity_numTableEntries(lefiLayerDensity * this)
{
  return this->numTableEntries_;
}

double 
lefiLayerDensity_tableEntry(lefiLayerDensity * this,
			    int index)
{
  return this->tableEntries_[index];
}

int 
lefiLayerDensity_numCutareas(lefiLayerDensity * this)
{
  return this->numCutareas_;
}

double 
lefiLayerDensity_cutArea(lefiLayerDensity * this,
			 int index)
{
  return this->cutareas_[index];
}

/*
 * lefiParallel
 */ lefiParallel *
lefiParallel_CreateUninitialized()
{
  lefiParallel *this = (lefiParallel *) malloc(sizeof(lefiParallel));

  lefiParallel_Init(this);
  return (this);
}

void 
lefiParallel_Init(lefiParallel * this)
{
  this->numLength_ = 0;
  this->numWidth_ = 0;
  this->numWidthAllocated_ = 0;
}

void 
lefiParallel_Destroy(lefiParallel * this)
{
  if (this->numLength_)
    lefFree((char *) (this->length_));
  if (this->numWidth_) {
    lefFree((char *) (this->width_));
    lefFree((char *) (this->widthSpacing_));
  }
  lefiParallel_Init(this);
}

void
lefiParallel_Delete(lefiParallel * this)
{
  if (this == NULL)
    return;
  lefiParallel_Destroy(this);
  free(this);
}

void 
lefiParallel_addParallelLength(lefiParallel * this,
			       int numLength,
			       double *lengths)
{
  this->numLength_ = numLength;
  this->length_ = lengths;
}

void 
lefiParallel_addParallelWidth(lefiParallel * this,
			      double width)
{
  if (this->numWidth_ == this->numWidthAllocated_) {
    int     i;

    double *nw;

    double *nws;

    this->numWidthAllocated_ = this->numWidthAllocated_ ? this->numWidthAllocated_ * 2 : 2;
    nw = (double *) lefMalloc(sizeof(double) * this->numWidthAllocated_);
    nws = (double *) lefMalloc(sizeof(double) * (this->numWidthAllocated_ * this->numLength_));
    for (i = 0;
	 i < this->numWidth_;
	 i++)
      nw[i] = this->width_[i];
    for (i = 0;
	 i < (this->numWidth_ * this->numLength_);
	 i++)
      nws[i] = this->widthSpacing_[i];
    if (this->numWidth_) {
      lefFree((char *) (this->width_));
      lefFree((char *) (this->widthSpacing_));
    }
    this->width_ = nw;
    this->widthSpacing_ = nws;
  }
  this->width_[this->numWidth_] = width;
  this->numWidth_ += 1;
}

void 
lefiParallel_addParallelWidthSpacing(lefiParallel * this,
				     int numSpacing,
				     double *spacings)
{
  int     i;

  for (i = 0;
       i < numSpacing;
       i++)
    this->widthSpacing_[(this->numWidth_ - 1) * this->numLength_ + i] = spacings[i];
}

int 
lefiParallel_numLength(const lefiParallel * this)
{
  return this->numLength_;
}

int 
lefiParallel_numWidth(const lefiParallel * this)
{
  return this->numWidth_;
}

double 
lefiParallel_length(const lefiParallel * this,
		    int index)
{
  return this->length_[index];
}

double 
lefiParallel_width(const lefiParallel * this,
		   int index)
{
  return this->width_[index];
}

double 
lefiParallel_widthSpacing(const lefiParallel * this,
			  int iWidth,
			  int iWidthSpacing)
{
  return this->widthSpacing_[iWidth * this->numLength_ + iWidthSpacing];
}

/*
 * lefiInfluence
 */ lefiInfluence *
lefiInfluence_CreateUninitialized()
{
  lefiInfluence *this = (lefiInfluence *) malloc(sizeof(lefiInfluence));

  lefiInfluence_Init(this);
  return (this);
}

void 
lefiInfluence_Init(lefiInfluence * this)
{
  this->numAllocated_ = 0;
  this->numWidth_ = 0;
  this->numDistance_ = 0;
  this->numSpacing_ = 0;
}

void 
lefiInfluence_Destroy(lefiInfluence * this)
{
  if (this->numWidth_) {
    lefFree((char *) (this->width_));
    lefFree((char *) (this->distance_));
    lefFree((char *) (this->spacing_));
  }
  lefiInfluence_Init(this);
}

void
lefiInfluence_Delete(lefiInfluence * this)
{
  if (this == NULL)
    return;
  lefiInfluence_Destroy(this);
  free(this);
}

void 
lefiInfluence_addInfluence(lefiInfluence * this,
			   double width,
			   double distance,
			   double spacing)
{
  int     i;

  double *nw;

  double *nd;

  double *ns;

  if (this->numWidth_ == this->numAllocated_) {
    this->numAllocated_ = this->numAllocated_ ? this->numAllocated_ * 2 : 2;
    nw = (double *) lefMalloc(sizeof(double) * this->numAllocated_);
    nd = (double *) lefMalloc(sizeof(double) * this->numAllocated_);
    ns = (double *) lefMalloc(sizeof(double) * this->numAllocated_);
    for (i = 0;
	 i < this->numWidth_;
	 i++) {
      nw[i] = this->width_[i];
      nd[i] = this->distance_[i];
      ns[i] = this->spacing_[i];
    }
    if (this->numWidth_ > 0) {
      lefFree((char *) (this->width_));
      lefFree((char *) (this->distance_));
      lefFree((char *) (this->spacing_));
    }
    this->width_ = nw;
    this->distance_ = nd;
    this->spacing_ = ns;
  }
  this->width_[this->numWidth_] = width;
  this->distance_[this->numWidth_] = distance;
  this->spacing_[this->numWidth_] = spacing;
  this->numWidth_ += 1;
}

int 
lefiInfluence_numInfluenceEntry(const lefiInfluence * this)
{
  return this->numWidth_;
}

double 
lefiInfluence_width(const lefiInfluence * this,
		    int index)
{
  return this->width_[index];
}

double 
lefiInfluence_distance(const lefiInfluence * this,
		       int index)
{
  return this->distance_[index];
}

double 
lefiInfluence_spacing(const lefiInfluence * this,
		      int index)
{
  return this->spacing_[index];
}

/*
 * lefiSpacingTable
 */ lefiSpacingTable *
lefiSpacingTable_CreateUninitialized()
{
  lefiSpacingTable *this = (lefiSpacingTable *) malloc(sizeof(lefiSpacingTable));

  lefiSpacingTable_Init(this);
  return (this);
}

void 
lefiSpacingTable_Init(lefiSpacingTable * this)
{
  this->hasInfluence_ = 0;
  this->parallel_ = 0;
  this->influence_ = 0;
}

void 
lefiSpacingTable_Destroy(lefiSpacingTable * this)
{
  if ((this->hasInfluence_) && (this->influence_))
    lefiInfluence_Destroy(this->influence_);
  else if (this->parallel_) {
    lefiParallel_Destroy(this->parallel_);
    lefFree((char *) this->parallel_);
  }
  if (this->influence_)
    lefFree((char *) this->influence_);
  lefiSpacingTable_Init(this);
}

void
lefiSpacingTable_Delete(lefiSpacingTable * this)
{
  if (this == NULL)
    return;
  lefiSpacingTable_Destroy(this);
  free(this);
}

void 
lefiSpacingTable_addParallelLength(lefiSpacingTable * this,
				   int numLength,
				   double *lengths)
{
  lefiParallel *parallel;

  if (this->parallel_ == 0) {
    parallel = (lefiParallel *) lefMalloc(sizeof(lefiParallel));
    lefiParallel_Init(parallel);
    this->parallel_ = parallel;
  }
  else
    parallel = this->parallel_;
  lefiParallel_addParallelLength(parallel,
				 numLength,
				 lengths);
}

void 
lefiSpacingTable_addParallelWidth(lefiSpacingTable * this,
				  double width)
{
  lefiParallel *parallel;

  parallel = this->parallel_;
  lefiParallel_addParallelWidth(parallel,
				width);
}

void 
lefiSpacingTable_addParallelWidthSpacing(lefiSpacingTable * this,
					 int numSpacing,
					 double *spacings)
{
  lefiParallel *parallel;

  parallel = this->parallel_;
  lefiParallel_addParallelWidthSpacing(parallel,
				       numSpacing,
				       spacings);
}

void 
lefiSpacingTable_setInfluence(lefiSpacingTable * this)
{
  lefiInfluence *influence;

  influence = (lefiInfluence *) lefMalloc(sizeof(lefiInfluence));
  lefiInfluence_Init(influence);
  this->influence_ = influence;
  this->hasInfluence_ = 1;
}

void 
lefiSpacingTable_addInfluence(lefiSpacingTable * this,
			      double width,
			      double distance,
			      double spacing)
{
  lefiInfluence *influence;

  influence = this->influence_;
  lefiInfluence_addInfluence(influence,
			     width,
			     distance,
			     spacing);
}

int 
lefiSpacingTable_isInfluence(const lefiSpacingTable * this)
{
  return this->hasInfluence_ ? 1 : 0;
}

lefiInfluence *
lefiSpacingTable_influence(const lefiSpacingTable * this)
{
  return this->influence_;
}

lefiParallel *
lefiSpacingTable_parallel(const lefiSpacingTable * this)
{
  return this->parallel_;
}

/*
 * lefiAntennaModel
 */ lefiAntennaModel *
lefiAntennaModel_CreateUninitialized()
{
  lefiAntennaModel *this = (lefiAntennaModel *) malloc(sizeof(lefiAntennaModel));

  lefiAntennaModel_Init(this);
  return (this);
}

void 
lefiAntennaModel_Init(lefiAntennaModel * this)
{
  this->hasAntennaAreaRatio_ = 0;
  this->hasAntennaDiffAreaRatio_ = 0;
  this->hasAntennaDiffAreaRatioPWL_ = 0;
  this->hasAntennaCumAreaRatio_ = 0;
  this->hasAntennaCumDiffAreaRatio_ = 0;
  this->hasAntennaCumDiffAreaRatioPWL_ = 0;
  this->hasAntennaAreaFactor_ = 0;
  this->hasAntennaAreaFactorDUO_ = 0;
  this->hasAntennaSideAreaRatio_ = 0;
  this->hasAntennaDiffSideAreaRatio_ = 0;
  this->hasAntennaDiffSideAreaRatioPWL_ = 0;
  this->hasAntennaCumSideAreaRatio_ = 0;
  this->hasAntennaCumDiffSideAreaRatio_ = 0;
  this->hasAntennaCumDiffSideAreaRatioPWL_ = 0;
  this->hasAntennaSideAreaFactor_ = 0;
  this->hasAntennaSideAreaFactorDUO_ = 0;
  this->antennaDiffAreaRatioPWL_ = 0;
  this->antennaCumDiffAreaRatioPWL_ = 0;
  this->antennaDiffSideAreaRatioPWL_ = 0;
  this->antennaCumDiffSideAreaRatioPWL_ = 0;
  this->oxide_ = 0;
}

void 
lefiAntennaModel_Destroy(lefiAntennaModel * this)
{
  if (this->oxide_)
    lefFree((char *) (this->oxide_));
  if (this->antennaDiffAreaRatioPWL_) {
    lefiAntennaPWL_Destroy(this->antennaDiffAreaRatioPWL_);
    lefFree((char *) (this->antennaDiffAreaRatioPWL_));
    this->antennaDiffAreaRatioPWL_ = 0;
  }
  if (this->antennaCumDiffAreaRatioPWL_) {
    lefiAntennaPWL_Destroy(this->antennaCumDiffAreaRatioPWL_);
    lefFree((char *) (this->antennaCumDiffAreaRatioPWL_));
    this->antennaCumDiffAreaRatioPWL_ = 0;
  }
  if (this->antennaDiffSideAreaRatioPWL_) {
    lefiAntennaPWL_Destroy(this->antennaDiffSideAreaRatioPWL_);
    lefFree((char *) (this->antennaDiffSideAreaRatioPWL_));
    this->antennaDiffSideAreaRatioPWL_ = 0;
  }
  if (this->antennaCumDiffSideAreaRatioPWL_) {
    lefiAntennaPWL_Destroy(this->antennaCumDiffSideAreaRatioPWL_);
    lefFree((char *) (this->antennaCumDiffSideAreaRatioPWL_));
    this->antennaCumDiffSideAreaRatioPWL_ = 0;
  }
  lefiAntennaModel_Init(this);
}

void
lefiAntennaModel_Delete(lefiAntennaModel * this)
{
  if (this == NULL)
    return;
  lefiAntennaModel_Destroy(this);
  free(this);
}

/*
 * 5.5
 */ void 
lefiAntennaModel_setAntennaModel(lefiAntennaModel * this,
				 int aOxide)
{
  switch (aOxide) {
    case 2:this->oxide_ = strdup("OXIDE2");
    break;
  case 3:
    this->oxide_ = strdup("OXIDE3");
    break;
  case 4:
    this->oxide_ = strdup("OXIDE4");
    break;
  default:
    this->oxide_ = strdup("OXIDE1");
    break;
  }
}

/*
 * 3/23/2000 -- Wanda da Rosa.  The following are for 5.4 syntax
 */ void 
lefiAntennaModel_setAntennaAreaRatio(lefiAntennaModel * this,
				     double value)
{
  this->antennaAreaRatio_ = value;
  this->hasAntennaAreaRatio_ = 1;
}

void 
lefiAntennaModel_setAntennaCumAreaRatio(lefiAntennaModel * this,
					double value)
{
  this->antennaCumAreaRatio_ = value;
  this->hasAntennaCumAreaRatio_ = 1;
}

void 
lefiAntennaModel_setAntennaAreaFactor(lefiAntennaModel * this,
				      double value)
{
  this->antennaAreaFactor_ = value;
  this->hasAntennaAreaFactor_ = 1;
}

void 
lefiAntennaModel_setAntennaSideAreaRatio(lefiAntennaModel * this,
					 double value)
{
  this->antennaSideAreaRatio_ = value;
  this->hasAntennaSideAreaRatio_ = 1;
}

void 
lefiAntennaModel_setAntennaCumSideAreaRatio(lefiAntennaModel * this,
					    double value)
{
  this->antennaCumSideAreaRatio_ = value;
  this->hasAntennaCumSideAreaRatio_ = 1;
}

void 
lefiAntennaModel_setAntennaSideAreaFactor(lefiAntennaModel * this,
					  double value)
{
  this->antennaSideAreaFactor_ = value;
  this->hasAntennaSideAreaFactor_ = 1;
}

void 
lefiAntennaModel_setAntennaValue(lefiAntennaModel * this,
				 lefiAntennaEnum antennaType,
				 double value)
{
  switch (antennaType) {
    case lefiAntennaDAR:this->antennaDiffAreaRatio_ = value;
    this->hasAntennaDiffAreaRatio_ = 1;
    break;
  case lefiAntennaCDAR:
    this->antennaCumDiffAreaRatio_ = value;
    this->hasAntennaCumDiffAreaRatio_ = 1;
    break;
  case lefiAntennaDSAR:
    this->antennaDiffSideAreaRatio_ = value;
    this->hasAntennaDiffSideAreaRatio_ = 1;
    break;
  case lefiAntennaCDSAR:
    this->antennaCumDiffSideAreaRatio_ = value;
    this->hasAntennaCumDiffSideAreaRatio_ = 1;
    break;
  default:
    break;
  }
}

void 
lefiAntennaModel_setAntennaDUO(lefiAntennaModel * this,
			       lefiAntennaEnum antennaType)
{
  switch (antennaType) {
    case lefiAntennaAF:this->hasAntennaAreaFactorDUO_ = 1;
    break;
  case lefiAntennaSAF:
    this->hasAntennaSideAreaFactorDUO_ = 1;
    break;
  default:
    break;
  }
}

void 
lefiAntennaModel_setAntennaPWL(lefiAntennaModel * this,
			       lefiAntennaEnum antennaType,
			       lefiAntennaPWL * pwl)
{
  switch (antennaType) {
    case lefiAntennaDAR:this->antennaDiffAreaRatioPWL_ = pwl;
    break;
  case lefiAntennaCDAR:
    this->antennaCumDiffAreaRatioPWL_ = pwl;
    break;
  case lefiAntennaDSAR:
    this->antennaDiffSideAreaRatioPWL_ = pwl;
    break;
  case lefiAntennaCDSAR:
    this->antennaCumDiffSideAreaRatioPWL_ = pwl;
    break;
  default:
    break;
  }
}

int 
lefiAntennaModel_hasAntennaAreaRatio(const lefiAntennaModel * this)
{
  return this->hasAntennaAreaRatio_;
}

int 
lefiAntennaModel_hasAntennaDiffAreaRatio(const lefiAntennaModel * this)
{
  return this->hasAntennaDiffAreaRatio_;
}

int 
lefiAntennaModel_hasAntennaCumAreaRatio(const lefiAntennaModel * this)
{
  return this->hasAntennaCumAreaRatio_;
}

int 
lefiAntennaModel_hasAntennaCumDiffAreaRatio(const lefiAntennaModel * this)
{
  return this->hasAntennaCumDiffAreaRatio_;
}

int 
lefiAntennaModel_hasAntennaAreaFactor(const lefiAntennaModel * this)
{
  return this->hasAntennaAreaFactor_;
}

int 
lefiAntennaModel_hasAntennaSideAreaRatio(const lefiAntennaModel * this)
{
  return this->hasAntennaSideAreaRatio_;
}

int 
lefiAntennaModel_hasAntennaDiffSideAreaRatio(const lefiAntennaModel * this)
{
  return this->hasAntennaDiffSideAreaRatio_;
}

int 
lefiAntennaModel_hasAntennaCumSideAreaRatio(const lefiAntennaModel * this)
{
  return this->hasAntennaCumSideAreaRatio_;
}

int 
lefiAntennaModel_hasAntennaCumDiffSideAreaRatio(const lefiAntennaModel * this)
{
  return this->hasAntennaCumDiffSideAreaRatio_;
}

int 
lefiAntennaModel_hasAntennaSideAreaFactor(const lefiAntennaModel * this)
{
  return this->hasAntennaSideAreaFactor_;
}

int 
lefiAntennaModel_hasAntennaDiffAreaRatioPWL(const lefiAntennaModel * this)
{
  return this->antennaDiffAreaRatioPWL_ ? 1 : 0;
}

int 
lefiAntennaModel_hasAntennaCumDiffAreaRatioPWL(const lefiAntennaModel * this)
{
  return this->antennaCumDiffAreaRatioPWL_ ? 1 : 0;
}

int 
lefiAntennaModel_hasAntennaDiffSideAreaRatioPWL(const lefiAntennaModel * this)
{
  return this->antennaDiffSideAreaRatioPWL_ ? 1 : 0;
}

int 
lefiAntennaModel_hasAntennaCumDiffSideAreaRatioPWL(const lefiAntennaModel * this)
{
  return this->antennaCumDiffSideAreaRatioPWL_ ? 1 : 0;
}

int 
lefiAntennaModel_hasAntennaAreaFactorDUO(const lefiAntennaModel * this)
{
  return this->hasAntennaAreaFactorDUO_;
}

int 
lefiAntennaModel_hasAntennaSideAreaFactorDUO(const lefiAntennaModel * this)
{
  return this->hasAntennaSideAreaFactorDUO_;
}

/*
 * 5.5
 */ char *
lefiAntennaModel_antennaOxide(const lefiAntennaModel * this)
{
  return this->oxide_;
}

double 
lefiAntennaModel_antennaAreaRatio(const lefiAntennaModel * this)
{
  return this->antennaAreaRatio_;
}

double 
lefiAntennaModel_antennaDiffAreaRatio(const lefiAntennaModel * this)
{
  return this->antennaDiffAreaRatio_;
}

double 
lefiAntennaModel_antennaCumAreaRatio(const lefiAntennaModel * this)
{
  return this->antennaCumAreaRatio_;
}

double 
lefiAntennaModel_antennaCumDiffAreaRatio(const lefiAntennaModel * this)
{
  return this->antennaCumDiffAreaRatio_;
}

double 
lefiAntennaModel_antennaAreaFactor(const lefiAntennaModel * this)
{
  return this->antennaAreaFactor_;
}

double 
lefiAntennaModel_antennaSideAreaRatio(const lefiAntennaModel * this)
{
  return this->antennaSideAreaRatio_;
}

double 
lefiAntennaModel_antennaDiffSideAreaRatio(const lefiAntennaModel * this)
{
  return this->antennaDiffSideAreaRatio_;
}

double 
lefiAntennaModel_antennaCumSideAreaRatio(const lefiAntennaModel * this)
{
  return this->antennaCumSideAreaRatio_;
}

double 
lefiAntennaModel_antennaCumDiffSideAreaRatio(const lefiAntennaModel * this)
{
  return this->antennaCumDiffSideAreaRatio_;
}

double 
lefiAntennaModel_antennaSideAreaFactor(const lefiAntennaModel * this)
{
  return this->antennaSideAreaFactor_;
}

lefiAntennaPWL *
lefiAntennaModel_antennaDiffAreaRatioPWL(const lefiAntennaModel * this)
{
  return this->antennaDiffAreaRatioPWL_;
}

lefiAntennaPWL *
lefiAntennaModel_antennaCumDiffAreaRatioPWL(const lefiAntennaModel * this)
{
  return this->antennaCumDiffAreaRatioPWL_;
}

lefiAntennaPWL *
lefiAntennaModel_antennaDiffSideAreaRatioPWL(const lefiAntennaModel * this)
{
  return this->antennaDiffSideAreaRatioPWL_;
}

lefiAntennaPWL *
lefiAntennaModel_antennaCumDiffSideAreaRatioPWL(const lefiAntennaModel * this)
{
  return this->antennaCumDiffSideAreaRatioPWL_;
}

/*
 * lefiLayer
 */ lefiLayer *
lefiLayer_CreateUninitialized()
{
  lefiLayer *this = (lefiLayer *) malloc(sizeof(lefiLayer));

  lefiLayer_Init(this);
  return (this);
}

void 
lefiLayer_Init(lefiLayer * this)
{
  this->name_ = (char *) lefMalloc(16);
  this->nameSize_ = 16;
  this->type_ = (char *) lefMalloc(16);
  this->typeSize_ = 16;
  this->numSpacings_ = 0;
  this->spacingsAllocated_ = 0;
  this->numMinimumcut_ = 0;
  this->minimumcutAllocated_ = 0;
  this->numMinenclosedarea_ = 0;
  this->minenclosedareaAllocated_ = 0;
  this->numCurrentPoints_ = 0;
  this->currentPointsAllocated_ = 2;
  this->currentWidths_ = (double *) lefMalloc(sizeof(double) * 2);
  this->current_ = (double *) lefMalloc(sizeof(double) * 2);
  this->numResistancePoints_ = 0;
  this->resistancePointsAllocated_ = 2;
  this->resistanceWidths_ = (double *) lefMalloc(sizeof(double) * 2);
  this->resistances_ = (double *) lefMalloc(sizeof(double) * 2);
  this->numCapacitancePoints_ = 0;
  this->capacitancePointsAllocated_ = 2;
  this->capacitanceWidths_ = (double *) lefMalloc(sizeof(double) * 2);
  this->capacitances_ = (double *) lefMalloc(sizeof(double) * 2);
  this->numProps_ = 0;
  this->propsAllocated_ = 1;
  this->names_ = (char **) lefMalloc(sizeof(char *));
  this->values_ = (char **) lefMalloc(sizeof(char *));
  this->dvalues_ = (double *) lefMalloc(sizeof(double));
  this->types_ = (char *) lefMalloc(sizeof(char));
  this->numAccurrents_ = 0;
  this->accurrentAllocated_ = 0;
  this->numDccurrents_ = 0;
  this->dccurrentAllocated_ = 0;
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->numSpacingTable_ = 0;
  this->spacingTableAllocated_ = 0;
  lefiLayer_clear(this);
}

void 
lefiLayer_Destroy(lefiLayer * this)
{
  lefiLayer_clear(this);
  lefFree(this->name_);
  lefFree(this->type_);
  if (this->spacing_)
    lefFree((char *) (this->spacing_));
  if (this->spacingName_)
    lefFree((char *) (this->spacingName_));
  if (this->spacingAdjacentCuts_)
    lefFree((char *) (this->spacingAdjacentCuts_));
  if (this->spacingAdjacentWithin_)
    lefFree((char *) (this->spacingAdjacentWithin_));
  if (this->hasSpacingName_)
    lefFree((char *) (this->hasSpacingName_));
  if (this->hasSpacingAdjacent_)
    lefFree((char *) (this->hasSpacingAdjacent_));
  if (this->rangeMin_)
    lefFree((char *) (this->rangeMin_));
  if (this->rangeMax_)
    lefFree((char *) (this->rangeMax_));
  if (this->rangeInfluence_)
    lefFree((char *) (this->rangeInfluence_));
  if (this->rangeInfluenceRangeMin_)
    lefFree((char *) (this->rangeInfluenceRangeMin_));
  if (this->rangeInfluenceRangeMax_)
    lefFree((char *) (this->rangeInfluenceRangeMax_));
  if (this->rangeRangeMin_)
    lefFree((char *) (this->rangeRangeMin_));
  if (this->rangeRangeMax_)
    lefFree((char *) (this->rangeRangeMax_));
  if (this->lengthThreshold_)
    lefFree((char *) (this->lengthThreshold_));
  if (this->lengthThresholdRangeMin_)
    lefFree((char *) (this->lengthThresholdRangeMin_));
  if (this->lengthThresholdRangeMax_)
    lefFree((char *) (this->lengthThresholdRangeMax_));
  if (this->hasSpacingRange_)
    lefFree((char *) (this->hasSpacingRange_));
  if (this->hasSpacingUseLengthThreshold_)
    lefFree((char *) (this->hasSpacingUseLengthThreshold_));
  if (this->hasSpacingLengthThreshold_)
    lefFree((char *) (this->hasSpacingLengthThreshold_));
  lefFree((char *) (this->currentWidths_));
  lefFree((char *) (this->current_));
  lefFree((char *) (this->resistanceWidths_));
  lefFree((char *) (this->resistances_));
  lefFree((char *) (this->capacitanceWidths_));
  lefFree((char *) (this->capacitances_));
  lefFree((char *) (this->names_));
  lefFree((char *) (this->values_));
  lefFree((char *) (this->dvalues_));
  lefFree((char *) (this->types_));
}

void
lefiLayer_Delete(lefiLayer * this)
{
  if (this == NULL)
    return;
  lefiLayer_Destroy(this);
  free(this);
}

void 
lefiLayer_clear(lefiLayer * this)
{
  int     i;

  lefiLayerDensity *p;

  lefiSpacingTable *sp;

  lefiAntennaModel *aO;

  if (this->name_)
    *(this->name_) = 0;
  if (this->type_)
    *(this->type_) = 0;
  this->hasPitch_ = 0;
  this->hasOffset_ = 0;
  this->hasWidth_ = 0;
  this->hasArea_ = 0;
  this->hasWireExtension_ = 0;
  this->hasSpacing_ = 0;
  this->hasDirection_ = 0;
  this->hasResistance_ = 0;
  this->hasCapacitance_ = 0;
  this->hasHeight_ = 0;
  this->hasThickness_ = 0;
  this->hasShrinkage_ = 0;
  this->hasCapMultiplier_ = 0;
  this->hasEdgeCap_ = 0;
  this->hasAntennaArea_ = 0;
  this->hasAntennaLength_ = 0;
  this->hasCurrentDensityPoint_ = 0;
  for (i = 0;
       i < this->numSpacings_;
       i++)
    if (this->spacingName_[i])
      lefFree(this->spacingName_[i]);
  for (i = 0;
       i < this->numProps_;
       i++) {
    if (this->names_[i])
      lefFree(this->names_[i]);
    if (this->values_[i])
      lefFree(this->values_[i]);
    this->dvalues_[i] = 0;
  }
  this->numProps_ = 0;
  this->numSpacings_ = 0;
  this->numCurrentPoints_ = 0;
  if (this->numAccurrents_) {
    for (i = 0;
	 i < this->numAccurrents_;
	 i++) {
      p = this->accurrents_[i];
      lefiLayerDensity_Destroy(p);
      lefFree((char *) p);
    }
    this->numAccurrents_ = 0;
    this->accurrentAllocated_ = 0;
    lefFree((char *) (this->accurrents_));
    this->accurrents_ = 0;
  }
  if (this->numDccurrents_) {
    for (i = 0;
	 i < this->numDccurrents_;
	 i++) {
      p = this->dccurrents_[i];
      lefiLayerDensity_Destroy(p);
      lefFree((char *) p);
    }
    this->numDccurrents_ = 0;
    this->dccurrentAllocated_ = 0;
    lefFree((char *) (this->dccurrents_));
    this->dccurrents_ = 0;
  }
  this->hasSlotWireWidth_ = 0;
  this->hasSlotWireLength_ = 0;
  this->hasSlotWidth_ = 0;
  this->hasSlotLength_ = 0;
  this->hasMaxAdjacentSlotSpacing_ = 0;
  this->hasMaxCoaxialSlotSpacing_ = 0;
  this->hasMaxEdgeSlotSpacing_ = 0;
  this->hasSplitWireWidth_ = 0;
  this->hasMinimumDensity_ = 0;
  this->hasMaximumDensity_ = 0;
  this->hasDensityCheckWindow_ = 0;
  this->hasDensityCheckStep_ = 0;
  this->hasFillActiveSpacing_ = 0;
  if (this->numMinimumcut_ > 0) {
    lefFree((char *) (this->minimumcut_));
    lefFree((char *) (this->minimumcutWidth_));
    lefFree((char *) (this->hasMinimumcutConnection_));
    lefFree((char *) (this->hasMinimumcutNumCuts_));
    lefFree((char *) (this->minimumcutLength_));
    lefFree((char *) (this->minimumcutDistance_));
    for (i = 0;
	 i < this->numMinimumcut_;
	 i++) {
      if (this->minimumcutConnection_[i])
	lefFree((char *) (this->minimumcutConnection_[i]));
    }
    lefFree((char *) (this->minimumcutConnection_));
    this->numMinimumcut_ = 0;
    this->minimumcutAllocated_ = 0;
  }
  this->maxwidth_ = -1;
  this->minwidth_ = -1;
  if (this->numMinenclosedarea_ > 0) {
    lefFree((char *) (this->minenclosedarea_));
    lefFree((char *) (this->minenclosedareaWidth_));
    this->numMinenclosedarea_ = 0;
    this->minenclosedareaAllocated_ = 0;
  }
  this->minstep_ = -1;
  this->protrusionWidth1_ = -1;
  this->protrusionLength_ = -1;
  this->protrusionWidth2_ = -1;
  if (this->numSpacingTable_) {
    for (i = 0;
	 i < this->numSpacingTable_;
	 i++) {
      sp = this->spacingTable_[i];
      lefiSpacingTable_Destroy(sp);
      lefFree((char *) sp);
    }
  }
  this->numSpacingTable_ = 0;
  this->spacingTableAllocated_ = 0;
  for (i = 0;
       i < this->numAntennaModel_;
       i++) {
    aO = this->antennaModel_[i];
    lefiAntennaModel_Destroy(aO);
  }
  for (i = 0;
       i < this->antennaModelAllocated_;
       i++) {
    lefFree((char *) this->antennaModel_[i]);
  }
  if (this->antennaModel_)
    lefFree((char *) (this->antennaModel_));
  this->antennaModel_ = 0;
  this->numAntennaModel_ = 0;
  this->antennaModelAllocated_ = 0;
  if (this->nums_)
    lefFree((char *) (this->nums_));
}

void 
lefiLayer_setName(lefiLayer * this,
		  const char *name)
{
  int     len = strlen(name) + 1;

  lefiLayer_clear(this);
  if (len > this->nameSize_) {
    lefFree(this->name_);
    this->name_ = (char *) lefMalloc(len);
    this->nameSize_ = len;
  }
  strcpy(this->name_, CASE(name));
}

void 
lefiLayer_setType(lefiLayer * this,
		  const char *typ)
{
  int     len = strlen(typ) + 1;

  if (len > this->typeSize_) {
    lefFree(this->type_);
    this->type_ = (char *) lefMalloc(len);
    this->typeSize_ = len;
  }
  strcpy(this->type_, CASE(typ));
}

void 
lefiLayer_setPitch(lefiLayer * this,
		   double num)
{
  this->hasPitch_ = 1;
  this->pitch_ = num;
}

void 
lefiLayer_setOffset(lefiLayer * this,
		    double num)
{
  this->hasOffset_ = 1;
  this->offset_ = num;
}

void 
lefiLayer_setWidth(lefiLayer * this,
		   double num)
{
  this->hasWidth_ = 1;
  this->width_ = num;
}

void 
lefiLayer_setArea(lefiLayer * this,
		  double num)
{
  this->hasArea_ = 1;
  this->area_ = num;
}

void 
lefiLayer_setWireExtension(lefiLayer * this,
			   double num)
{
  this->hasWireExtension_ = 1;
  this->wireExtension_ = num;
}

/*
 * 5.5
 */ void 
lefiLayer_setMaxwidth(lefiLayer * this,
		      double width)
{
  this->maxwidth_ = width;
}

/*
 * 5.5
 */ void 
lefiLayer_setMinwidth(lefiLayer * this,
		      double width)
{
  this->minwidth_ = width;
}

/*
 * 5.5
 */ void 
lefiLayer_addMinenclosedarea(lefiLayer * this,
			     double area)
{
  if (this->numMinenclosedarea_ == this->minenclosedareaAllocated_) {
    double *na;

    double *nw;

    int     i, lim;

    if (this->minenclosedareaAllocated_ == 0) {
      lim = this->minenclosedareaAllocated_ = 2;
      na = (double *) lefMalloc(sizeof(double) * lim);
      nw = (double *) lefMalloc(sizeof(double) * lim);
    }
    else {
      lim = this->minenclosedareaAllocated_ * 2;
      this->minenclosedareaAllocated_ = lim;
      na = (double *) lefMalloc(sizeof(double) * lim);
      nw = (double *) lefMalloc(sizeof(double) * lim);
      lim /= 2;
      for (i = 0;
	   i < lim;
	   i++) {
	na[i] = this->minenclosedarea_[i];
	nw[i] = this->minenclosedareaWidth_[i];
      }
      lefFree((char *) (this->minenclosedarea_));
      lefFree((char *) (this->minenclosedareaWidth_));
    }
    this->minenclosedarea_ = na;
    this->minenclosedareaWidth_ = nw;
  }
  this->minenclosedarea_[this->numMinenclosedarea_] = area;
  this->minenclosedareaWidth_[this->numMinenclosedarea_] = 0;
  this->numMinenclosedarea_ += 1;
}

/*
 * 5.5
 */ void 
lefiLayer_addMinenclosedareaWidth(lefiLayer * this,
				  double width)
{
  this->minenclosedareaWidth_[this->numMinenclosedarea_ - 1] = width;
}

/*
 * 5.5
 */ void 
lefiLayer_addMinimumcut(lefiLayer * this,
			int mincut,
			double width)
{
  if (this->numMinimumcut_ == this->minimumcutAllocated_) {
    int    *nc;

    double *nw;

    int    *hm;

    char  **nud;

    int    *hc;

    double *nl;

    double *nd;

    int     i, lim;

    if (this->minimumcutAllocated_ == 0) {
      lim = this->minimumcutAllocated_ = 2;
      nc = (int *) lefMalloc(sizeof(int) * lim);
      nw = (double *) lefMalloc(sizeof(double) * lim);
      hm = (int *) lefMalloc(sizeof(int) * lim);
      nud = (char **) lefMalloc(sizeof(char *) * lim);
      hc = (int *) lefMalloc(sizeof(int) * lim);
      nl = (double *) lefMalloc(sizeof(double) * lim);
      nd = (double *) lefMalloc(sizeof(double) * lim);
    }
    else {
      lim = this->minimumcutAllocated_ * 2;
      this->minimumcutAllocated_ = lim;
      nc = (int *) lefMalloc(sizeof(int) * lim);
      nw = (double *) lefMalloc(sizeof(double) * lim);
      hm = (int *) lefMalloc(sizeof(int) * lim);
      nud = (char **) lefMalloc(sizeof(char *) * lim);
      hc = (int *) lefMalloc(sizeof(int) * lim);
      nl = (double *) lefMalloc(sizeof(double) * lim);
      nd = (double *) lefMalloc(sizeof(double) * lim);
      lim /= 2;
      for (i = 0;
	   i < lim;
	   i++) {
	nc[i] = this->minimumcut_[i];
	nw[i] = this->minimumcutWidth_[i];
	hm[i] = this->hasMinimumcutConnection_[i];
	nud[i] = this->minimumcutConnection_[i];
	hc[i] = this->hasMinimumcutNumCuts_[i];
	nl[i] = this->minimumcutLength_[i];
	nd[i] = this->minimumcutDistance_[i];
      }
      lefFree((char *) (this->minimumcut_));
      lefFree((char *) (this->minimumcutWidth_));
      lefFree((char *) (this->hasMinimumcutConnection_));
      lefFree((char *) (this->minimumcutConnection_));
      lefFree((char *) (this->hasMinimumcutNumCuts_));
      lefFree((char *) (this->minimumcutLength_));
      lefFree((char *) (this->minimumcutDistance_));
    }
    this->minimumcut_ = nc;
    this->minimumcutWidth_ = nw;
    this->hasMinimumcutConnection_ = hm;
    this->minimumcutConnection_ = nud;
    this->hasMinimumcutNumCuts_ = hc;
    this->minimumcutLength_ = nl;
    this->minimumcutDistance_ = nd;
  }
  this->minimumcut_[this->numMinimumcut_] = mincut;
  this->minimumcutWidth_[this->numMinimumcut_] = width;
  this->hasMinimumcutConnection_[this->numMinimumcut_] = 0;
  this->minimumcutConnection_[this->numMinimumcut_] = 0;
  this->hasMinimumcutNumCuts_[this->numMinimumcut_] = 0;
  this->minimumcutLength_[this->numMinimumcut_] = 0;
  this->minimumcutDistance_[this->numMinimumcut_] = 0;
  this->numMinimumcut_ += 1;
}

/*
 * 5.5
 */ void 
lefiLayer_addMinimumcutConnect(lefiLayer * this,
			       const char *connectType)
{
  if (connectType && (strcmp(connectType, "") != 0)) {
    this->hasMinimumcutConnection_[this->numMinimumcut_ - 1] = 1;
    this->minimumcutConnection_[this->numMinimumcut_ - 1] = strdup(connectType);
  }
}

/*
 * 5.5
 */ void 
lefiLayer_addMinimumcutLengDis(lefiLayer * this,
			       double length,
			       double width)
{
  this->hasMinimumcutNumCuts_[this->numMinimumcut_ - 1] = 1;
  this->minimumcutLength_[this->numMinimumcut_ - 1] = length;
  this->minimumcutDistance_[this->numMinimumcut_ - 1] = width;
}

/*
 * 5.5
 */ void 
lefiLayer_setMinstep(lefiLayer * this,
		     double distance)
{
  this->minstep_ = distance;
}

/*
 * 5.5
 */ void 
lefiLayer_setProtrusion(lefiLayer * this,
			double width1,
			double length,
			double width2)
{
  this->protrusionWidth1_ = width1;
  this->protrusionLength_ = length;
  this->protrusionWidth2_ = width2;
}

/*
 * wmd -- pcr 282799, need to make left_, right_ as arrays
 *//* when bumping to new list, need to copy also hasUseLengthThreshold_ *//* a
    nd lengthThreshold_ */ void 
lefiLayer_setSpacingMin(lefiLayer * this,
			double dist)
{
  if (this->numSpacings_ == this->spacingsAllocated_) {
    double *nd;

    char  **nn;

    int    *nsn;

    int    *nsa;

    int    *nr;

    int    *nac;

    double *naw;

    double *nrmin, *nrmax;

    double *nri, *nrimin, *nrimax;

    double *nrrmin, *nrrmax;

    int    *ht;

    int    *nl;

    double *nt, *ntmin, *ntmax;

    int     i, lim;

    if (this->spacingsAllocated_ == 0) {
      lim = this->spacingsAllocated_ = 2;
      nd = (double *) lefMalloc(sizeof(double) * lim);
      nn = (char **) lefMalloc(sizeof(char *) * lim);
      nac = (int *) lefMalloc(sizeof(int) * lim);
      naw = (double *) lefMalloc(sizeof(double) * lim);
      nsn = (int *) lefMalloc(sizeof(int) * lim);
      nsa = (int *) lefMalloc(sizeof(int) * lim);
      nr = (int *) lefMalloc(sizeof(int) * lim);
      nrmin = (double *) lefMalloc(sizeof(double) * lim);
      nrmax = (double *) lefMalloc(sizeof(double) * lim);
      nri = (double *) lefMalloc(sizeof(double) * lim);
      nrimin = (double *) lefMalloc(sizeof(double) * lim);
      nrimax = (double *) lefMalloc(sizeof(double) * lim);
      nrrmin = (double *) lefMalloc(sizeof(double) * lim);
      nrrmax = (double *) lefMalloc(sizeof(double) * lim);
      ht = (int *) lefMalloc(sizeof(int) * lim);
      nl = (int *) lefMalloc(sizeof(int) * lim);
      nt = (double *) lefMalloc(sizeof(double) * lim);
      ntmin = (double *) lefMalloc(sizeof(double) * lim);
      ntmax = (double *) lefMalloc(sizeof(double) * lim);
    }
    else {
      lim = this->spacingsAllocated_ * 2;
      this->spacingsAllocated_ = lim;
      nd = (double *) lefMalloc(sizeof(double) * lim);
      nn = (char **) lefMalloc(sizeof(char *) * lim);
      nac = (int *) lefMalloc(sizeof(int) * lim);
      naw = (double *) lefMalloc(sizeof(double) * lim);
      nsn = (int *) lefMalloc(sizeof(int) * lim);
      nsa = (int *) lefMalloc(sizeof(int) * lim);
      nr = (int *) lefMalloc(sizeof(int) * lim);
      nrmin = (double *) lefMalloc(sizeof(double) * lim);
      nrmax = (double *) lefMalloc(sizeof(double) * lim);
      nri = (double *) lefMalloc(sizeof(double) * lim);
      nrimin = (double *) lefMalloc(sizeof(double) * lim);
      nrimax = (double *) lefMalloc(sizeof(double) * lim);
      nrrmin = (double *) lefMalloc(sizeof(double) * lim);
      nrrmax = (double *) lefMalloc(sizeof(double) * lim);
      ht = (int *) lefMalloc(sizeof(int) * lim);
      nl = (int *) lefMalloc(sizeof(int) * lim);
      nt = (double *) lefMalloc(sizeof(double) * lim);
      ntmin = (double *) lefMalloc(sizeof(double) * lim);
      ntmax = (double *) lefMalloc(sizeof(double) * lim);
      lim /= 2;
      for (i = 0;
	   i < lim;
	   i++) {
	nd[i] = this->spacing_[i];
	if (this->spacingName_[i])
	  nn[i] = this->spacingName_[i];
	else
	  nn[i] = 0;
	nac[i] = this->spacingAdjacentCuts_[i];
	naw[i] = this->spacingAdjacentWithin_[i];
	nsn[i] = this->hasSpacingName_[i];
	nsa[i] = this->hasSpacingAdjacent_[i];
	nr[i] = this->hasSpacingRange_[i];
	nrmin[i] = this->rangeMin_[i];
	nrmax[i] = this->rangeMax_[i];
	nri[i] = this->rangeInfluence_[i];
	nrimin[i] = this->rangeInfluenceRangeMin_[i];
	nrimax[i] = this->rangeInfluenceRangeMax_[i];
	nrrmin[i] = this->rangeRangeMin_[i];
	nrrmax[i] = this->rangeRangeMax_[i];
	ht[i] = this->hasSpacingUseLengthThreshold_[i];
	nl[i] = this->hasSpacingLengthThreshold_[i];
	nt[i] = this->lengthThreshold_[i];
	ntmin[i] = this->lengthThresholdRangeMin_[i];
	ntmax[i] = this->lengthThresholdRangeMax_[i];
      }
      lefFree((char *) (this->spacing_));
      lefFree((char *) (this->spacingName_));
      lefFree((char *) (this->spacingAdjacentCuts_));
      lefFree((char *) (this->spacingAdjacentWithin_));
      lefFree((char *) (this->hasSpacingName_));
      lefFree((char *) (this->hasSpacingAdjacent_));
      lefFree((char *) (this->hasSpacingRange_));
      lefFree((char *) (this->rangeMin_));
      lefFree((char *) (this->rangeMax_));
      lefFree((char *) (this->rangeInfluence_));
      lefFree((char *) (this->rangeInfluenceRangeMin_));
      lefFree((char *) (this->rangeInfluenceRangeMax_));
      lefFree((char *) (this->rangeRangeMin_));
      lefFree((char *) (this->rangeRangeMax_));
      lefFree((char *) (this->hasSpacingUseLengthThreshold_));
      lefFree((char *) (this->hasSpacingLengthThreshold_));
      lefFree((char *) (this->lengthThreshold_));
      lefFree((char *) (this->lengthThresholdRangeMin_));
      lefFree((char *) (this->lengthThresholdRangeMax_));
    }
    this->spacing_ = nd;
    this->spacingName_ = nn;
    this->spacingAdjacentCuts_ = nac;
    this->spacingAdjacentWithin_ = naw;
    this->hasSpacingName_ = nsn;
    this->hasSpacingAdjacent_ = nsa;
    this->hasSpacingRange_ = nr;
    this->rangeMin_ = nrmin;
    this->rangeMax_ = nrmax;
    this->rangeInfluence_ = nri;
    this->rangeInfluenceRangeMin_ = nrimin;
    this->rangeInfluenceRangeMax_ = nrimax;
    this->rangeRangeMin_ = nrrmin;
    this->rangeRangeMax_ = nrrmax;
    this->hasSpacingUseLengthThreshold_ = ht;
    this->hasSpacingLengthThreshold_ = nl;
    this->lengthThreshold_ = nt;
    this->lengthThresholdRangeMin_ = ntmin;
    this->lengthThresholdRangeMax_ = ntmax;
  }
  this->hasSpacing_ = 1;
  this->spacing_[this->numSpacings_] = dist;
  this->spacingName_[this->numSpacings_] = 0;
  this->hasSpacingName_[this->numSpacings_] = 0;
  this->hasSpacingAdjacent_[this->numSpacings_] = 0;
  this->hasSpacingRange_[this->numSpacings_] = 0;
  this->rangeMin_[this->numSpacings_] = -1;
  this->rangeMax_[this->numSpacings_] = -1;
  this->rangeInfluence_[this->numSpacings_] = 0;
  this->rangeInfluenceRangeMin_[this->numSpacings_] = -1;
  this->rangeInfluenceRangeMax_[this->numSpacings_] = -1;
  this->rangeRangeMin_[this->numSpacings_] = -1;
  this->rangeRangeMax_[this->numSpacings_] = -1;
  this->hasSpacingUseLengthThreshold_[this->numSpacings_] = 0;
  this->hasSpacingLengthThreshold_[this->numSpacings_] = 0;
  this->lengthThreshold_[this->numSpacings_] = 0;
  this->lengthThresholdRangeMin_[this->numSpacings_] = -1;
  this->lengthThresholdRangeMax_[this->numSpacings_] = -1;
  this->numSpacings_ += 1;
}

void 
lefiLayer_setSpacingRange(lefiLayer * this,
			  double left,
			  double right)
{
  this->rangeMin_[this->numSpacings_ - 1] = left;
  this->rangeMax_[this->numSpacings_ - 1] = right;
  this->hasSpacingRange_[this->numSpacings_ - 1] = 1;
}

void 
lefiLayer_setSpacingName(lefiLayer * this,
			 const char *spacingName)
{
  if (spacingName) {
    int     len = strlen(spacingName) + 1;

    this->spacingName_[this->numSpacings_ - 1] = (char *) lefMalloc(len);
    strcpy(this->spacingName_[this->numSpacings_ - 1], CASE(spacingName));
    this->hasSpacingName_[this->numSpacings_ - 1] = 1;
  }
}

void 
lefiLayer_setSpacingAdjacent(lefiLayer * this,
			     int numCuts,
			     double distance)
{
  this->spacingAdjacentCuts_[this->numSpacings_ - 1] = numCuts;
  this->spacingAdjacentWithin_[this->numSpacings_ - 1] = distance;
  this->hasSpacingAdjacent_[this->numSpacings_ - 1] = 1;
}

void 
lefiLayer_setSpacingRangeUseLength(lefiLayer * this)
{
  this->hasSpacingUseLengthThreshold_[this->numSpacings_ - 1] = 1;
}

void 
lefiLayer_setSpacingRangeInfluence(lefiLayer * this,
				   double infLength)
{
  this->rangeInfluence_[this->numSpacings_ - 1] = infLength;
}

void 
lefiLayer_setSpacingRangeInfluenceRange(lefiLayer * this,
					double min,
					double max)
{
  this->rangeInfluenceRangeMin_[this->numSpacings_ - 1] = min;
  this->rangeInfluenceRangeMax_[this->numSpacings_ - 1] = max;
}

void 
lefiLayer_setSpacingRangeRange(lefiLayer * this,
			       double min,
			       double max)
{
  this->rangeRangeMin_[this->numSpacings_ - 1] = min;
  this->rangeRangeMax_[this->numSpacings_ - 1] = max;
}

void 
lefiLayer_setSpacingLength(lefiLayer * this,
			   double num)
{
  this->lengthThreshold_[this->numSpacings_ - 1] = num;
  this->hasSpacingLengthThreshold_[this->numSpacings_ - 1] = 1;
}

void 
lefiLayer_setSpacingLengthRange(lefiLayer * this,
				double min,
				double max)
{
  this->lengthThresholdRangeMin_[this->numSpacings_ - 1] = min;
  this->lengthThresholdRangeMax_[this->numSpacings_ - 1] = max;
}

void 
lefiLayer_setDirection(lefiLayer * this,
		       const char *dir)
{
  this->direction_ = (char *) dir;
  this->hasDirection_ = 1;
}

void 
lefiLayer_setResistance(lefiLayer * this,
			double num)
{
  this->hasResistance_ = 1;
  this->resistance_ = num;
}

void 
lefiLayer_setCapacitance(lefiLayer * this,
			 double num)
{
  this->hasCapacitance_ = 1;
  this->capacitance_ = num;
}

void 
lefiLayer_setHeight(lefiLayer * this,
		    double num)
{
  this->hasHeight_ = 1;
  this->height_ = num;
}

void 
lefiLayer_setThickness(lefiLayer * this,
		       double num)
{
  this->hasThickness_ = 1;
  this->thickness_ = num;
}

void 
lefiLayer_setShrinkage(lefiLayer * this,
		       double num)
{
  this->hasShrinkage_ = 1;
  this->shrinkage_ = num;
}

void 
lefiLayer_setCapMultiplier(lefiLayer * this,
			   double num)
{
  this->hasCapMultiplier_ = 1;
  this->capMultiplier_ = num;
}

void 
lefiLayer_setEdgeCap(lefiLayer * this,
		     double num)
{
  this->hasEdgeCap_ = 1;
  this->edgeCap_ = num;
}

void 
lefiLayer_setAntennaLength(lefiLayer * this,
			   double num)
{
  this->hasAntennaLength_ = 1;
  this->antennaLength_ = num;
}

void 
lefiLayer_setAntennaArea(lefiLayer * this,
			 double num)
{
  this->hasAntennaArea_ = 1;
  this->antennaArea_ = num;
}

void 
lefiLayer_setCurrentDensity(lefiLayer * this,
			    double num)
{
  this->hasCurrentDensityPoint_ = 1;
  this->currentDensity_ = num;
}

void 
lefiLayer_setCurrentPoint(lefiLayer * this,
			  double width,
			  double current)
{
  if (this->numCurrentPoints_ == this->currentPointsAllocated_) {
    int     max = this->numCurrentPoints_;

    int     len;

    int     i;

    double *nc;

    double *nw;

    if (this->currentPointsAllocated_ == 0)
      len = this->currentPointsAllocated_ = 2;
    else
      len = this->currentPointsAllocated_ *= 2;
    nc = (double *) lefMalloc(sizeof(double) * len);
    nw = (double *) lefMalloc(sizeof(double) * len);
    for (i = 0;
	 i < max;
	 i++) {
      nc[i] = this->current_[i];
      nw[i] = this->currentWidths_[i];
    }
    lefFree((char *) (this->current_));
    lefFree((char *) (this->currentWidths_));
    this->current_ = nc;
    this->currentWidths_ = nw;
  }
  this->current_[this->numCurrentPoints_] = current;
  this->currentWidths_[this->numCurrentPoints_] = width;
  this->numCurrentPoints_ += 1;
}

void 
lefiLayer_setResistancePoint(lefiLayer * this,
			     double width,
			     double resistance)
{
  if (this->numResistancePoints_ == this->resistancePointsAllocated_) {
    int     max = this->numResistancePoints_;

    int     len;

    int     i;

    double *nc;

    double *nw;

    if (this->resistancePointsAllocated_ == 0)
      len = this->resistancePointsAllocated_ = 2;
    else
      len = this->resistancePointsAllocated_ *= 2;
    nc = (double *) lefMalloc(sizeof(double) * len);
    nw = (double *) lefMalloc(sizeof(double) * len);
    for (i = 0;
	 i < max;
	 i++) {
      nc[i] = this->resistances_[i];
      nw[i] = this->resistanceWidths_[i];
    }
    lefFree((char *) (this->resistances_));
    lefFree((char *) (this->resistanceWidths_));
    this->resistances_ = nc;
    this->resistanceWidths_ = nw;
  }
  this->resistances_[this->numResistancePoints_] = resistance;
  this->resistanceWidths_[this->numResistancePoints_] = width;
  this->numResistancePoints_ += 1;
}

void 
lefiLayer_setCapacitancePoint(lefiLayer * this,
			      double width,
			      double capacitance)
{
  if (this->numCapacitancePoints_ == this->capacitancePointsAllocated_) {
    int     max = this->numCapacitancePoints_;

    int     len;

    int     i;

    double *nc;

    double *nw;

    if (this->capacitancePointsAllocated_ == 0)
      len = this->capacitancePointsAllocated_ = 2;
    else
      len = this->capacitancePointsAllocated_ *= 2;
    nc = (double *) lefMalloc(sizeof(double) * len);
    nw = (double *) lefMalloc(sizeof(double) * len);
    for (i = 0;
	 i < max;
	 i++) {
      nc[i] = this->capacitances_[i];
      nw[i] = this->capacitanceWidths_[i];
    }
    lefFree((char *) (this->capacitances_));
    lefFree((char *) (this->capacitanceWidths_));
    this->capacitances_ = nc;
    this->capacitanceWidths_ = nw;
  }
  this->capacitances_[this->numCapacitancePoints_] = capacitance;
  this->capacitanceWidths_[this->numCapacitancePoints_] = width;
  this->numCapacitancePoints_ += 1;
}

int 
lefiLayer_hasType(const lefiLayer * this)
{
  return (this->type_[0] != '\0') ? 1 : 0;
}

int 
lefiLayer_hasPitch(const lefiLayer * this)
{
  return this->hasPitch_;
}

int 
lefiLayer_hasOffset(const lefiLayer * this)
{
  return this->hasOffset_;
}

int 
lefiLayer_hasWidth(const lefiLayer * this)
{
  return this->hasWidth_;
}

int 
lefiLayer_hasArea(const lefiLayer * this)
{
  return this->hasArea_;
}

int 
lefiLayer_hasWireExtension(const lefiLayer * this)
{
  return this->hasWireExtension_;
}

int 
lefiLayer_hasSpacingNumber(const lefiLayer * this)
{
  return ((this->hasSpacing_ != 0) && (this->numSpacings_ > 0)) ? 1 : 0;
}

int 
lefiLayer_hasSpacingName(const lefiLayer * this,
			 int index)
{
  return ((this->hasSpacing_ != 0) && (this->hasSpacingName_[index] != 0)) ? 1 : 0;
}

int 
lefiLayer_hasSpacingAdjacent(const lefiLayer * this,
			     int index)
{
  return ((this->hasSpacing_ != 0) && (this->hasSpacingAdjacent_[index] != 0)) ? 1 : 0;
}

int 
lefiLayer_hasSpacingRange(const lefiLayer * this,
			  int index)
{
  return ((this->hasSpacing_ != 0) && (this->hasSpacingRange_[index] != 0)) ? 1 : 0;
}

int 
lefiLayer_hasSpacingRangeUseLengthThreshold(const lefiLayer * this,
					    int index)
{
  return (this->hasSpacingUseLengthThreshold_[index]);
}

int 
lefiLayer_hasSpacingRangeInfluence(const lefiLayer * this,
				   int index)
{
  return ((this->hasSpacing_ != 0) && (this->hasSpacingRange_[index] != 0) && (this->rangeInfluence_[index])) ? 1 : 0;
}

int 
lefiLayer_hasSpacingRangeInfluenceRange(const lefiLayer * this,
					int index)
{
  return ((this->hasSpacing_ != 0) && (this->hasSpacingRange_[index] != 0) && (this->rangeInfluenceRangeMin_[index] != -1) && (this->rangeInfluenceRangeMax_[index] != -1)) ? 1 : 0;
}

int 
lefiLayer_hasSpacingRangeRange(const lefiLayer * this,
			       int index)
{
  return ((this->hasSpacing_ != 0) && (this->hasSpacingRange_[index] != 0) && (this->rangeRangeMin_[index] != -1) && (this->rangeRangeMax_[index] != -1)) ? 1 : 0;
}

int 
lefiLayer_hasSpacingLengthThreshold(const lefiLayer * this,
				    int index)
{
  return (this->hasSpacingLengthThreshold_[index]) ? 1 : 0;
}

int 
lefiLayer_hasSpacingLengthThresholdRange(const lefiLayer * this,
					 int index)
{
  return ((this->hasSpacing_ != 0) && (this->hasSpacingLengthThreshold_[index] != 0) && (this->lengthThresholdRangeMin_[index] != -1) && (this->lengthThresholdRangeMax_[index] != -1)) ? 1 : 0;
}

int 
lefiLayer_hasDirection(const lefiLayer * this)
{
  return this->hasDirection_;
}

int 
lefiLayer_hasResistance(const lefiLayer * this)
{
  return this->hasResistance_;
}

int 
lefiLayer_hasCapacitance(const lefiLayer * this)
{
  return this->hasCapacitance_;
}

int 
lefiLayer_hasHeight(const lefiLayer * this)
{
  return this->hasHeight_;
}

int 
lefiLayer_hasThickness(const lefiLayer * this)
{
  return this->hasThickness_;
}

int 
lefiLayer_hasShrinkage(const lefiLayer * this)
{
  return this->hasShrinkage_;
}

int 
lefiLayer_hasCapMultiplier(const lefiLayer * this)
{
  return this->hasCapMultiplier_;
}

int 
lefiLayer_hasEdgeCap(const lefiLayer * this)
{
  return this->hasEdgeCap_;
}

int 
lefiLayer_hasAntennaArea(const lefiLayer * this)
{
  return this->hasAntennaArea_;
}

int 
lefiLayer_hasAntennaLength(const lefiLayer * this)
{
  return this->hasAntennaLength_;
}

int 
lefiLayer_hasCurrentDensityPoint(const lefiLayer * this)
{
  return this->hasCurrentDensityPoint_;
}

int 
lefiLayer_hasCurrentDensityArray(const lefiLayer * this)
{
  return this->numCurrentPoints_ ? 1 : 0;
}

int 
lefiLayer_hasResistanceArray(const lefiLayer * this)
{
  return this->numResistancePoints_ ? 1 : 0;
}

int 
lefiLayer_hasCapacitanceArray(const lefiLayer * this)
{
  return this->numCapacitancePoints_ ? 1 : 0;
}

void 
lefiLayer_currentDensityArray(const lefiLayer * this,
			      int *numPoints,
			      double **widths,
			      double **current)
{
  *numPoints = this->numCurrentPoints_;
  *widths = this->currentWidths_;
  *current = this->current_;
}

void 
lefiLayer_resistanceArray(const lefiLayer * this,
			  int *numPoints,
			  double **widths,
			  double **res)
{
  *numPoints = this->numResistancePoints_;
  *widths = this->resistanceWidths_;
  *res = this->resistances_;
}

void 
lefiLayer_capacitanceArray(const lefiLayer * this,
			   int *numPoints,
			   double **widths,
			   double **cap)
{
  *numPoints = this->numCapacitancePoints_;
  *widths = this->capacitanceWidths_;
  *cap = this->capacitances_;
}

int 
lefiLayer_numSpacing(const lefiLayer * this)
{
  return this->numSpacings_;
}

char   *
lefiLayer_name(const lefiLayer * this)
{
  return this->name_;
}

const char *
lefiLayer_type(const lefiLayer * this)
{
  return this->type_;
}

double 
lefiLayer_pitch(const lefiLayer * this)
{
  return this->pitch_;
}

double 
lefiLayer_offset(const lefiLayer * this)
{
  return this->offset_;
}

double 
lefiLayer_width(const lefiLayer * this)
{
  return this->width_;
}

double 
lefiLayer_area(const lefiLayer * this)
{
  return this->area_;
}

double 
lefiLayer_wireExtension(const lefiLayer * this)
{
  return this->wireExtension_;
}

double 
lefiLayer_spacing(const lefiLayer * this,
		  int index)
{
  return this->spacing_[index];
}

char   *
lefiLayer_spacingName(const lefiLayer * this,
		      int index)
{
  return this->spacingName_[index];
}

int 
lefiLayer_spacingAdjacentCuts(const lefiLayer * this,
			      int index)
{
  return this->spacingAdjacentCuts_[index];
}

double 
lefiLayer_spacingAdjacentWithin(const lefiLayer * this,
				int index)
{
  return this->spacingAdjacentWithin_[index];
}

double 
lefiLayer_spacingRangeMin(const lefiLayer * this,
			  int index)
{
  return this->rangeMin_[index];
}

double 
lefiLayer_spacingRangeMax(const lefiLayer * this,
			  int index)
{
  return this->rangeMax_[index];
}

double 
lefiLayer_spacingRangeInfluence(const lefiLayer * this,
				int index)
{
  return this->rangeInfluence_[index];
}

double 
lefiLayer_spacingRangeInfluenceMin(const lefiLayer * this,
				   int index)
{
  return this->rangeInfluenceRangeMin_[index];
}

double 
lefiLayer_spacingRangeInfluenceMax(const lefiLayer * this,
				   int index)
{
  return this->rangeInfluenceRangeMax_[index];
}

double 
lefiLayer_spacingRangeRangeMin(const lefiLayer * this,
			       int index)
{
  return this->rangeRangeMin_[index];
}

double 
lefiLayer_spacingRangeRangeMax(const lefiLayer * this,
			       int index)
{
  return this->rangeRangeMax_[index];
}

double 
lefiLayer_spacingLengthThreshold(const lefiLayer * this,
				 int index)
{
  return this->lengthThreshold_[index];
}

double 
lefiLayer_spacingLengthThresholdRangeMin(const lefiLayer * this,
					 int index)
{
  return this->lengthThresholdRangeMin_[index];
}

double 
lefiLayer_spacingLengthThresholdRangeMax(const lefiLayer * this,
					 int index)
{
  return this->lengthThresholdRangeMax_[index];
}

const char *
lefiLayer_direction(const lefiLayer * this)
{
  return this->direction_;
}

double 
lefiLayer_currentDensityPoint(const lefiLayer * this)
{
  return this->currentDensity_;
}

double 
lefiLayer_resistance(const lefiLayer * this)
{
  return this->resistance_;
}

double 
lefiLayer_capacitance(const lefiLayer * this)
{
  return this->capacitance_;
}

double 
lefiLayer_height(const lefiLayer * this)
{
  return this->height_;
}

double 
lefiLayer_thickness(const lefiLayer * this)
{
  return this->thickness_;
}

double 
lefiLayer_shrinkage(const lefiLayer * this)
{
  return this->shrinkage_;
}

double 
lefiLayer_capMultiplier(const lefiLayer * this)
{
  return this->capMultiplier_;
}

double 
lefiLayer_edgeCap(const lefiLayer * this)
{
  return this->edgeCap_;
}

double 
lefiLayer_antennaLength(const lefiLayer * this)
{
  return this->antennaLength_;
}

double 
lefiLayer_antennaArea(const lefiLayer * this)
{
  return this->antennaArea_;
}

/*
 * 5.5
 */ int 
lefiLayer_numMinimumcut(const lefiLayer * this)
{
  return this->numMinimumcut_;
}

/*
 * 5.5
 */ int 
lefiLayer_minimumcut(const lefiLayer * this,
		     int index)
{
  return this->minimumcut_[index];
}

/*
 * 5.5
 */ double 
lefiLayer_minimumcutWidth(const lefiLayer * this,
			  int index)
{
  return this->minimumcutWidth_[index];
}

/*
 * 5.5
 */ int 
lefiLayer_hasMinimumcutConnection(const lefiLayer * this,
				  int index)
{
  return this->hasMinimumcutConnection_[index];
}

/*
 * 5.5
 */ const char *
lefiLayer_minimumcutConnection(const lefiLayer * this,
			       int index)
{
  return this->minimumcutConnection_[index];
}

/*
 * 5.5
 */ int 
lefiLayer_hasMinimumcutNumCuts(const lefiLayer * this,
			       int index)
{
  return this->hasMinimumcutNumCuts_[index];
}

/*
 * 5.5
 */ double 
lefiLayer_minimumcutLength(const lefiLayer * this,
			   int index)
{
  return this->minimumcutLength_[index];
}

/*
 * 5.5
 */ double 
lefiLayer_minimumcutDistance(const lefiLayer * this,
			     int index)
{
  return this->minimumcutDistance_[index];
}

/*
 * 5.5
 */ int 
lefiLayer_hasMaxwidth(const lefiLayer * this)
{
  return this->maxwidth_ == -1 ? 0 : 1;
}

/*
 * 5.5
 */ double 
lefiLayer_maxwidth(const lefiLayer * this)
{
  return this->maxwidth_;
}

/*
 * 5.5
 */ int 
lefiLayer_hasMinwidth(const lefiLayer * this)
{
  return this->minwidth_ == -1 ? 0 : 1;
}

/*
 * 5.5
 */ double 
lefiLayer_minwidth(const lefiLayer * this)
{
  return this->minwidth_;
}

/*
 * 5.5
 */ int 
lefiLayer_numMinenclosedarea(const lefiLayer * this)
{
  return this->numMinenclosedarea_;
}

/*
 * 5.5
 */ int 
lefiLayer_hasMinenclosedareaWidth(const lefiLayer * this,
				  int index)
{
  return this->minenclosedareaWidth_[index] == -1 ? 0 : 1;
}

/*
 * 5.5
 */ double 
lefiLayer_minenclosedarea(const lefiLayer * this,
			  int index)
{
  return this->minenclosedarea_[index];
}

/*
 * 5.5
 */ double 
lefiLayer_minenclosedareaWidth(const lefiLayer * this,
			       int index)
{
  return this->minenclosedareaWidth_[index];
}

/*
 * 5.5
 */ int 
lefiLayer_hasMinstep(const lefiLayer * this)
{
  return this->minstep_ == -1 ? 0 : 1;
}

/*
 * 5.5
 */ double 
lefiLayer_minstep(const lefiLayer * this)
{
  return this->minstep_;
}

/*
 * 5.5
 */ int 
lefiLayer_hasProtrusion(const lefiLayer * this)
{
  return this->protrusionWidth1_ == -1 ? 0 : 1;
}

/*
 * 5.5
 */ double 
lefiLayer_protrusionWidth1(const lefiLayer * this)
{
  return this->protrusionWidth1_;
}

/*
 * 5.5
 */ double 
lefiLayer_protrusionLength(const lefiLayer * this)
{
  return this->protrusionLength_;
}

/*
 * 5.5
 */ double 
lefiLayer_protrusionWidth2(const lefiLayer * this)
{
  return this->protrusionWidth2_;
}

void 
lefiLayer_print(const lefiLayer * this,
		FILE * f)
{
  int     i, max;

  double *j;

  double *k;

  fprintf(f, "Layer %s:\n", lefiLayer_name(this));
  if (lefiLayer_hasType(this))
    fprintf(f, " type %s\n", lefiLayer_type(this));
  if (lefiLayer_hasPitch(this))
    fprintf(f, " pitch %g\n", lefiLayer_pitch(this));
  if (lefiLayer_hasWireExtension(this))
    fprintf(f, " wireextension %g\n", lefiLayer_wireExtension(this));
  if (lefiLayer_hasWidth(this))
    fprintf(f, " width %g\n", lefiLayer_width(this));
  if (lefiLayer_hasArea(this))
    fprintf(f, " area %g\n", lefiLayer_area(this));
  if (lefiLayer_hasSpacingNumber(this)) {
    for (i = 0;
	 i < lefiLayer_numSpacing(this);
	 i++) {
      fprintf(f, " spacing %g\n", lefiLayer_spacing(this,
						    i));
      if (lefiLayer_hasSpacingRange(this,
				    i)) {
	fprintf(f, " range %g %g\n", lefiLayer_spacingRangeMin(this,
					 i), lefiLayer_spacingRangeMax(this,
								       i));
	if (lefiLayer_hasSpacingRangeUseLengthThreshold(this,
							i))
	  fprintf(f, " uselengththreshold\n");
	else if (lefiLayer_hasSpacingRangeInfluence(this,
						    i)) {
	  fprintf(f, " influence %g\n", lefiLayer_spacingRangeInfluence(this,
									i));
	  if (lefiLayer_hasSpacingRangeInfluenceRange(this,
						      i))
	    fprintf(f, " Range %g %g\n", lefiLayer_spacingRangeInfluenceMin(this,
				i), lefiLayer_spacingRangeInfluenceMax(this,
								       i));
	}
	else if (lefiLayer_hasSpacingRangeRange(this,
						i))
	  fprintf(f, " range %g %g\n", lefiLayer_spacingRangeRangeMin(this,
				    i), lefiLayer_spacingRangeRangeMax(this,
								       i));
      }
      else if (lefiLayer_hasSpacingLengthThreshold(this,
						   i)) {
	fprintf(f, " lengththreshold %g\n", lefiLayer_spacingLengthThreshold(this,
									i));
	if (lefiLayer_hasSpacingLengthThresholdRange(this,
						     i))
	  fprintf(f, " range %g %g\n", lefiLayer_spacingLengthThresholdRangeMin(this,
			  i), lefiLayer_spacingLengthThresholdRangeMax(this,
								       i));
      }
    }
  }
  if (lefiLayer_hasDirection(this))
    fprintf(f, " direction %s\n", lefiLayer_direction(this));
  if (lefiLayer_hasResistance(this))
    fprintf(f, " resistance %g\n", lefiLayer_resistance(this));
  if (lefiLayer_hasResistanceArray(this)) {
    lefiLayer_resistanceArray(this,
			      &max,
			      &j,
			      &k);
    fprintf(f, " resistance PWL");
    for (i = 0;
	 i < max;
	 i++)
      fprintf(f, " %g %g", j[i], k[i]);
    fprintf(f, "\n");
  }
  if (lefiLayer_hasCapacitance(this))
    fprintf(f, " capacitance %g\n", lefiLayer_capacitance(this));
  if (lefiLayer_hasCapacitanceArray(this)) {
    lefiLayer_capacitanceArray(this,
			       &max,
			       &j,
			       &k);
    fprintf(f, " capacitance PWL");
    for (i = 0;
	 i < max;
	 i++)
      fprintf(f, " %g %g", j[i], k[i]);
    fprintf(f, "\n");
  }
  if (lefiLayer_hasHeight(this))
    fprintf(f, " height %g\n", lefiLayer_height(this));
  if (lefiLayer_hasThickness(this))
    fprintf(f, " thickness %g\n", lefiLayer_thickness(this));
  if (lefiLayer_hasShrinkage(this))
    fprintf(f, " shrinkage %g\n", lefiLayer_shrinkage(this));
  if (lefiLayer_hasCapMultiplier(this))
    fprintf(f, " cap muptiplier %g\n", lefiLayer_capMultiplier(this));
  if (lefiLayer_hasEdgeCap(this))
    fprintf(f, " edge cap %g\n", lefiLayer_edgeCap(this));
  if (lefiLayer_hasCurrentDensityPoint(this))
    fprintf(f, " currentden %g\n", lefiLayer_currentDensityPoint(this));
  if (lefiLayer_hasCurrentDensityArray(this)) {
    lefiLayer_currentDensityArray(this,
				  &max,
				  &j,
				  &k);
    fprintf(f, " currentden PWL");
    for (i = 0;
	 i < max;
	 i++)
      fprintf(f, " %g %g", j[i], k[i]);
    fprintf(f, "\n");
  }
}

void 
lefiLayer_addProp(lefiLayer * this,
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
lefiLayer_addNumProp(lefiLayer * this,
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

int 
lefiLayer_numProps(const lefiLayer * this)
{
  return this->numProps_;
}

const char *
lefiLayer_propName(const lefiLayer * this,
		   int i)
{
  if (i < 0 || i >= this->numProps_) {
    lefiError("bad index for layer property");
    return 0;
  }
  return this->names_[i];
}

const char *
lefiLayer_propValue(const lefiLayer * this,
		    int i)
{
  if (i < 0 || i >= this->numProps_) {
    lefiError("bad index for layer property");
    return 0;
  }
  return this->values_[i];
}

double 
lefiLayer_propNumber(const lefiLayer * this,
		     int i)
{
  if (i < 0 || i >= this->numProps_) {
    lefiError("bad index for layer property");
    return 0;
  }
  return this->dvalues_[i];
}

const char 
lefiLayer_propType(const lefiLayer * this,
		   int i)
{
  if (i < 0 || i >= this->numProps_) {
    lefiError("bad index for layer property");
    return 0;
  }
  return this->types_[i];
}

int 
lefiLayer_propIsNumber(const lefiLayer * this,
		       int i)
{
  if (i < 0 || i >= this->numProps_) {
    lefiError("bad index for layer property");
    return 0;
  }
  return this->dvalues_[i] ? 1 : 0;
}

int 
lefiLayer_propIsString(const lefiLayer * this,
		       int i)
{
  if (i < 0 || i >= this->numProps_) {
    lefiError("bad index for layer property");
    return 0;
  }
  return this->dvalues_[i] ? 0 : 1;
}

void 
lefiLayer_addAccurrentDensity(lefiLayer * this,
			      const char *type)
{
  lefiLayerDensity *density;

  if (this->numAccurrents_ == this->accurrentAllocated_) {
    lefiLayerDensity **array;

    int     i;

    this->accurrentAllocated_ = this->accurrentAllocated_ ? this->accurrentAllocated_ * 2 : 2;
    array = (lefiLayerDensity **) lefMalloc(sizeof(lefiLayerDensity *) * this->accurrentAllocated_);
    for (i = 0;
	 i < this->numAccurrents_;
	 i++)
      array[i] = this->accurrents_[i];
    if (this->accurrents_)
      lefFree((char *) (this->accurrents_));
    this->accurrents_ = array;
  }
  density = this->accurrents_[this->numAccurrents_] = (lefiLayerDensity *) lefMalloc(sizeof(lefiLayerDensity));
  this->numAccurrents_ += 1;
  lefiLayerDensity_Init(density,
			type);
}

void 
lefiLayer_setAcOneEntry(lefiLayer * this,
			double num)
{
  lefiLayerDensity *density;

  density = this->accurrents_[this->numAccurrents_ - 1];
  lefiLayerDensity_setOneEntry(density,
			       num);
}

void 
lefiLayer_addAcFrequency(lefiLayer * this)
{
  lefiLayerDensity *density;

  density = this->accurrents_[this->numAccurrents_ - 1];
  lefiLayerDensity_addFrequency(density,
				this->numNums_,
				this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}

void 
lefiLayer_addAcCutarea(lefiLayer * this)
{
  lefiLayerDensity *density;

  density = this->accurrents_[this->numAccurrents_ - 1];
  lefiLayerDensity_addCutarea(density,
			      this->numNums_,
			      this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}

void 
lefiLayer_addAcTableEntry(lefiLayer * this)
{
  lefiLayerDensity *density;

  density = this->accurrents_[this->numAccurrents_ - 1];
  lefiLayerDensity_addTableEntry(density,
				 this->numNums_,
				 this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}

void 
lefiLayer_addAcWidth(lefiLayer * this)
{
  lefiLayerDensity *density;

  density = this->accurrents_[this->numAccurrents_ - 1];
  lefiLayerDensity_addWidth(density,
			    this->numNums_,
			    this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}

void 
lefiLayer_setDcOneEntry(lefiLayer * this,
			double num)
{
  lefiLayerDensity *density;

  density = this->dccurrents_[this->numDccurrents_ - 1];
  lefiLayerDensity_setOneEntry(density,
			       num);
}

void 
lefiLayer_addDccurrentDensity(lefiLayer * this,
			      const char *type)
{
  lefiLayerDensity *density;

  if (this->numDccurrents_ == this->dccurrentAllocated_) {
    lefiLayerDensity **array;

    int     i;

    this->dccurrentAllocated_ = this->dccurrentAllocated_ ? this->dccurrentAllocated_ * 2 : 2;
    array = (lefiLayerDensity **) lefMalloc(sizeof(lefiLayerDensity *) * this->dccurrentAllocated_);
    for (i = 0;
	 i < this->numDccurrents_;
	 i++)
      array[i] = this->dccurrents_[i];
    if (this->dccurrents_)
      lefFree((char *) (this->dccurrents_));
    this->dccurrents_ = array;
  }
  density = this->dccurrents_[this->numDccurrents_] = (lefiLayerDensity *) lefMalloc(sizeof(lefiLayerDensity));
  this->numDccurrents_ += 1;
  lefiLayerDensity_Init(density,
			type);
}

void 
lefiLayer_addDcCutarea(lefiLayer * this)
{
  lefiLayerDensity *density;

  density = this->dccurrents_[this->numDccurrents_ - 1];
  lefiLayerDensity_addCutarea(density,
			      this->numNums_,
			      this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}

void 
lefiLayer_addDcTableEntry(lefiLayer * this)
{
  lefiLayerDensity *density;

  density = this->dccurrents_[this->numDccurrents_ - 1];
  lefiLayerDensity_addTableEntry(density,
				 this->numNums_,
				 this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}

void 
lefiLayer_addDcWidth(lefiLayer * this)
{
  lefiLayerDensity *density;

  density = this->dccurrents_[this->numDccurrents_ - 1];
  lefiLayerDensity_addWidth(density,
			    this->numNums_,
			    this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}

void 
lefiLayer_addNumber(lefiLayer * this,
		    double num)
{
  if (this->numNums_ == this->numAllocated_) {
    double *array;

    int     i;

    this->numAllocated_ = this->numAllocated_ ? this->numAllocated_ * 2 : 2;
    array = (double *) lefMalloc(sizeof(double) * this->numAllocated_);
    for (i = 0;
	 i < this->numNums_;
	 i++)
      array[i] = this->nums_[i];
    if (this->nums_)
      lefFree((char *) (this->nums_));
    this->nums_ = array;
  }
  this->nums_[this->numNums_++] = num;
}

int 
lefiLayer_getNumber(lefiLayer * this)
{
  return this->numNums_ - 1;
}

int 
lefiLayer_hasAccurrentDensity(const lefiLayer * this)
{
  return this->numAccurrents_ ? 1 : 0;
}

int 
lefiLayer_hasDccurrentDensity(const lefiLayer * this)
{
  return this->numDccurrents_ ? 1 : 0;
}

int 
lefiLayer_numAccurrentDensity(const lefiLayer * this)
{
  return (this->numAccurrents_);
}

int 
lefiLayer_numDccurrentDensity(const lefiLayer * this)
{
  return (this->numDccurrents_);
}

lefiLayerDensity *
lefiLayer_accurrent(const lefiLayer * this,
		    int index)
{
  if (index >= this->numAccurrents_)
    return 0;
  return (this->accurrents_[index]);
}

lefiLayerDensity *
lefiLayer_dccurrent(const lefiLayer * this,
		    int index)
{
  if (index >= this->numDccurrents_)
    return 0;
  return (this->dccurrents_[index]);
}

/*
 * 5.5
 */ void 
lefiLayer_addAntennaModel(lefiLayer * this,
			  int aOxide)
{
  lefiAntennaModel *amo;

  int     i;

  if (this->numAntennaModel_ == 0) {
    this->antennaModel_ = (lefiAntennaModel **) lefMalloc(sizeof(lefiAntennaModel *) * 4);
    this->antennaModelAllocated_ = 4;
    for (i = 0;
	 i < 4;
	 i++) {
      this->antennaModel_[i] = (lefiAntennaModel *) lefMalloc(sizeof(lefiAntennaModel));
    }
    this->numAntennaModel_++;
    this->antennaModelAllocated_ = 4;
    amo = this->antennaModel_[0];
  }
  else {
    amo = this->antennaModel_[this->numAntennaModel_];
    this->numAntennaModel_++;
  }
  lefiAntennaModel_Init(amo);
  lefiAntennaModel_setAntennaModel(amo,
				   aOxide);
  return;
}

/*
 * 5.5
 */ int 
lefiLayer_numAntennaModel(const lefiLayer * this)
{
  return this->numAntennaModel_;
}

/*
 * 5.5
 */ lefiAntennaModel *
lefiLayer_antennaModel(const lefiLayer * this,
		       int index)
{
  return this->antennaModel_[index];
}

/*
 * 3/23/2000 -- Wanda da Rosa.  The following are for 5.4 syntax
 */ void 
lefiLayer_setAntennaAreaRatio(lefiLayer * this,
			      double value)
{
  if (this->numAntennaModel_ == 0)
    lefiLayer_addAntennaModel(this,
			      1);
  lefiAntennaModel_setAntennaAreaRatio(this->antennaModel_[this->numAntennaModel_ - 1],
				       value);
}

void 
lefiLayer_setAntennaCumAreaRatio(lefiLayer * this,
				 double value)
{
  if (this->numAntennaModel_ == 0)
    lefiLayer_addAntennaModel(this,
			      1);
  lefiAntennaModel_setAntennaCumAreaRatio(this->antennaModel_[this->numAntennaModel_ - 1],
					  value);
}

void 
lefiLayer_setAntennaAreaFactor(lefiLayer * this,
			       double value)
{
  if (this->numAntennaModel_ == 0)
    lefiLayer_addAntennaModel(this,
			      1);
  lefiAntennaModel_setAntennaAreaFactor(this->antennaModel_[this->numAntennaModel_ - 1],
					value);
}

void 
lefiLayer_setAntennaSideAreaRatio(lefiLayer * this,
				  double value)
{
  if (this->numAntennaModel_ == 0)
    lefiLayer_addAntennaModel(this,
			      1);
  lefiAntennaModel_setAntennaSideAreaRatio(this->antennaModel_[this->numAntennaModel_ - 1],
					   value);
}

void 
lefiLayer_setAntennaCumSideAreaRatio(lefiLayer * this,
				     double value)
{
  if (this->numAntennaModel_ == 0)
    lefiLayer_addAntennaModel(this,
			      1);
  lefiAntennaModel_setAntennaCumSideAreaRatio(this->antennaModel_[this->numAntennaModel_ - 1],
					      value);
}

void 
lefiLayer_setAntennaSideAreaFactor(lefiLayer * this,
				   double value)
{
  if (this->numAntennaModel_ == 0)
    lefiLayer_addAntennaModel(this,
			      1);
  lefiAntennaModel_setAntennaSideAreaFactor(this->antennaModel_[this->numAntennaModel_ - 1],
					    value);
}

void 
lefiLayer_setAntennaValue(lefiLayer * this,
			  lefiAntennaEnum antennaType,
			  double value)
{
  if (this->numAntennaModel_ == 0)
    lefiLayer_addAntennaModel(this,
			      1);
  lefiAntennaModel_setAntennaValue(this->antennaModel_[this->numAntennaModel_ - 1],
				   antennaType,
				   value);
}

void 
lefiLayer_setAntennaDUO(lefiLayer * this,
			lefiAntennaEnum antennaType)
{
  if (this->numAntennaModel_ == 0)
    lefiLayer_addAntennaModel(this,
			      1);
  lefiAntennaModel_setAntennaDUO(this->antennaModel_[this->numAntennaModel_ - 1],
				 antennaType);
}

void 
lefiLayer_setAntennaPWL(lefiLayer * this,
			lefiAntennaEnum antennaType,
			lefiAntennaPWL * pwl)
{
  if (this->numAntennaModel_ == 0)
    lefiLayer_addAntennaModel(this,
			      1);
  lefiAntennaModel_setAntennaPWL(this->antennaModel_[this->numAntennaModel_ - 1],
				 antennaType,
				 pwl);
}

/*
 * 8/29/2001 -- Wanda da Rosa.  The following are for 5.4 enhancements
 */ void 
lefiLayer_setSlotWireWidth(lefiLayer * this,
			   double num)
{
  this->hasSlotWireWidth_ = 1;
  this->slotWireWidth_ = num;
}

void 
lefiLayer_setSlotWireLength(lefiLayer * this,
			    double num)
{
  this->hasSlotWireLength_ = 1;
  this->slotWireLength_ = num;
}

void 
lefiLayer_setSlotWidth(lefiLayer * this,
		       double num)
{
  this->hasSlotWidth_ = 1;
  this->slotWidth_ = num;
}

void 
lefiLayer_setSlotLength(lefiLayer * this,
			double num)
{
  this->hasSlotLength_ = 1;
  this->slotLength_ = num;
}

void 
lefiLayer_setMaxAdjacentSlotSpacing(lefiLayer * this,
				    double num)
{
  this->hasMaxAdjacentSlotSpacing_ = 1;
  this->maxAdjacentSlotSpacing_ = num;
}

void 
lefiLayer_setMaxCoaxialSlotSpacing(lefiLayer * this,
				   double num)
{
  this->hasMaxCoaxialSlotSpacing_ = 1;
  this->maxCoaxialSlotSpacing_ = num;
}

void 
lefiLayer_setMaxEdgeSlotSpacing(lefiLayer * this,
				double num)
{
  this->hasMaxEdgeSlotSpacing_ = 1;
  this->maxEdgeSlotSpacing_ = num;
}

void 
lefiLayer_setSplitWireWidth(lefiLayer * this,
			    double num)
{
  this->hasSplitWireWidth_ = 1;
  this->splitWireWidth_ = num;
}

void 
lefiLayer_setMinimumDensity(lefiLayer * this,
			    double num)
{
  this->hasMinimumDensity_ = 1;
  this->minimumDensity_ = num;
}

void 
lefiLayer_setMaximumDensity(lefiLayer * this,
			    double num)
{
  this->hasMaximumDensity_ = 1;
  this->maximumDensity_ = num;
}

void 
lefiLayer_setDensityCheckWindow(lefiLayer * this,
				double length,
				double width)
{
  this->hasDensityCheckWindow_ = 1;
  this->densityCheckWindowLength_ = length;
  this->densityCheckWindowWidth_ = width;
}

void 
lefiLayer_setDensityCheckStep(lefiLayer * this,
			      double num)
{
  this->hasDensityCheckStep_ = 1;
  this->densityCheckStep_ = num;
}

void 
lefiLayer_setFillActiveSpacing(lefiLayer * this,
			       double num)
{
  this->hasFillActiveSpacing_ = 1;
  this->fillActiveSpacing_ = num;
}

int 
lefiLayer_hasSlotWireWidth(const lefiLayer * this)
{
  return this->hasSlotWireWidth_;
}

int 
lefiLayer_hasSlotWireLength(const lefiLayer * this)
{
  return this->hasSlotWireLength_;
}

int 
lefiLayer_hasSlotWidth(const lefiLayer * this)
{
  return this->hasSlotWidth_;
}

int 
lefiLayer_hasSlotLength(const lefiLayer * this)
{
  return this->hasSlotLength_;
}

int 
lefiLayer_hasMaxAdjacentSlotSpacing(const lefiLayer * this)
{
  return this->hasMaxAdjacentSlotSpacing_;
}

int 
lefiLayer_hasMaxCoaxialSlotSpacing(const lefiLayer * this)
{
  return this->hasMaxCoaxialSlotSpacing_;
}

int 
lefiLayer_hasMaxEdgeSlotSpacing(const lefiLayer * this)
{
  return this->hasMaxEdgeSlotSpacing_;
}

int 
lefiLayer_hasSplitWireWidth(const lefiLayer * this)
{
  return this->hasSplitWireWidth_;
}

int 
lefiLayer_hasMinimumDensity(const lefiLayer * this)
{
  return this->hasMinimumDensity_;
}

int 
lefiLayer_hasMaximumDensity(const lefiLayer * this)
{
  return this->hasMaximumDensity_;
}

int 
lefiLayer_hasDensityCheckWindow(const lefiLayer * this)
{
  return this->hasDensityCheckWindow_;
}

int 
lefiLayer_hasDensityCheckStep(const lefiLayer * this)
{
  return this->hasDensityCheckStep_;
}

int 
lefiLayer_hasFillActiveSpacing(const lefiLayer * this)
{
  return this->hasFillActiveSpacing_;
}

double 
lefiLayer_slotWireWidth(const lefiLayer * this)
{
  return this->slotWireWidth_;
}

double 
lefiLayer_slotWireLength(const lefiLayer * this)
{
  return this->slotWireLength_;
}

double 
lefiLayer_slotWidth(const lefiLayer * this)
{
  return this->slotWidth_;
}

double 
lefiLayer_slotLength(const lefiLayer * this)
{
  return this->slotLength_;
}

double 
lefiLayer_maxAdjacentSlotSpacing(const lefiLayer * this)
{
  return this->maxAdjacentSlotSpacing_;
}

double 
lefiLayer_maxCoaxialSlotSpacing(const lefiLayer * this)
{
  return this->maxCoaxialSlotSpacing_;
}

double 
lefiLayer_maxEdgeSlotSpacing(const lefiLayer * this)
{
  return this->maxEdgeSlotSpacing_;
}

double 
lefiLayer_splitWireWidth(const lefiLayer * this)
{
  return this->splitWireWidth_;
}

double 
lefiLayer_minimumDensity(const lefiLayer * this)
{
  return this->minimumDensity_;
}

double 
lefiLayer_maximumDensity(const lefiLayer * this)
{
  return this->maximumDensity_;
}

double 
lefiLayer_densityCheckWindowLength(const lefiLayer * this)
{
  return this->densityCheckWindowLength_;
}

double 
lefiLayer_densityCheckWindowWidth(const lefiLayer * this)
{
  return this->densityCheckWindowWidth_;
}

double 
lefiLayer_densityCheckStep(const lefiLayer * this)
{
  return this->densityCheckStep_;
}

double 
lefiLayer_fillActiveSpacing(const lefiLayer * this)
{
  return this->fillActiveSpacing_;
}

/*
 * 5.5 SPACINGTABLE
 */ void 
lefiLayer_addSpacingTable(lefiLayer * this)
{
  lefiSpacingTable *sp;

  if (this->numSpacingTable_ == this->spacingTableAllocated_) {
    lefiSpacingTable **array;

    int     i;

    this->spacingTableAllocated_ = this->spacingTableAllocated_ ? this->spacingTableAllocated_ * 2 : 2;
    array = (lefiSpacingTable **) lefMalloc(sizeof(lefiSpacingTable *) * this->spacingTableAllocated_);
    for (i = 0;
	 i < this->numSpacingTable_;
	 i++)
      array[i] = this->spacingTable_[i];
    if (this->spacingTable_)
      lefFree((char *) (this->spacingTable_));
    this->spacingTable_ = array;
  }
  sp = this->spacingTable_[this->numSpacingTable_] = (lefiSpacingTable *) lefMalloc(sizeof(lefiSpacingTable));
  this->numSpacingTable_ += 1;
  lefiSpacingTable_Init(sp);
}

void 
lefiLayer_addSpParallelLength(lefiLayer * this)
{
  lefiSpacingTable *sp;

  sp = this->spacingTable_[this->numSpacingTable_ - 1];
  lefiSpacingTable_addParallelLength(sp,
				     this->numNums_,
				     this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}

void 
lefiLayer_addSpParallelWidth(lefiLayer * this,
			     double width)
{
  lefiSpacingTable *sp;

  sp = this->spacingTable_[this->numSpacingTable_ - 1];
  lefiSpacingTable_addParallelWidth(sp,
				    width);
}

void 
lefiLayer_addSpParallelWidthSpacing(lefiLayer * this)
{
  lefiSpacingTable *sp;

  sp = this->spacingTable_[this->numSpacingTable_ - 1];
  lefiSpacingTable_addParallelWidthSpacing(sp,
					   this->numNums_,
					   this->nums_);
  lefFree((char *) (this->nums_));
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}

void 
lefiLayer_setInfluence(lefiLayer * this)
{
  lefiSpacingTable *sp;

  sp = this->spacingTable_[this->numSpacingTable_ - 1];
  lefiSpacingTable_setInfluence(sp);
}

void 
lefiLayer_addSpInfluence(lefiLayer * this,
			 double width,
			 double distance,
			 double spacing)
{
  lefiSpacingTable *sp;

  sp = this->spacingTable_[this->numSpacingTable_ - 1];
  lefiSpacingTable_addInfluence(sp,
				width,
				distance,
				spacing);
}

int 
lefiLayer_numSpacingTable(lefiLayer * this)
{
  return this->numSpacingTable_;
}

lefiSpacingTable *
lefiLayer_spacingTable(lefiLayer * this,
		       int index)
{
  return this->spacingTable_[index];
}
