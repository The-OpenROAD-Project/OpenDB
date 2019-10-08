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

#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "lefiLayer.hpp"
#include "lefiDebug.hpp"

////////////////////////////////////////////////////
////////////////////////////////////////////////////
//
//    lefiAntennaPWL
//
////////////////////////////////////////////////////
////////////////////////////////////////////////////

lefiAntennaPWL::lefiAntennaPWL() {
  this->lefiAntennaPWL::Init();
}

void lefiAntennaPWL::Init(){
  this->numAlloc_ = 2;
  this->d_ = (double*)lefMalloc(sizeof(double)*2);
  this->r_ = (double*)lefMalloc(sizeof(double)*2);
  this->lefiAntennaPWL::clear();
}

lefiAntennaPWL::~lefiAntennaPWL() {
  this->lefiAntennaPWL::Destroy();
}

void lefiAntennaPWL::Destroy() {
  if (this->d_)
     lefFree((char*)(this->d_));
  if (this->r_)
     lefFree((char*)(this->r_));
}

// Clear will reset the numPWL_ to zero but keep array allocated
void lefiAntennaPWL::clear() {
  this->numPWL_ = 0;
}

void lefiAntennaPWL::addAntennaPWL(double d, double r) {
  if (this->numPWL_ == this->numAlloc_) {
    int     i, len;
    double* nd;
    double* nr;

    if (this->numAlloc_ == 0)
      len = this->numAlloc_ = 2;
    else
      len = this->numAlloc_ *= 2;
    nd = (double*)lefMalloc(sizeof(double)*len);
    nr = (double*)lefMalloc(sizeof(double)*len);

    for (i = 0; i < this->numPWL_; i++) {
      nd[i] = this->d_[i];
      nr[i] = this->r_[i];
    }
    lefFree((char*)(this->d_));
    lefFree((char*)(this->r_));
    this->d_ = nd;
    this->r_ = nr;
  }
  this->d_[this->numPWL_] = d;
  this->r_[this->numPWL_] = r;
  this->numPWL_ += 1;
}

int lefiAntennaPWL::numPWL() const {
  return this->numPWL_;
}

double lefiAntennaPWL::PWLdiffusion(int index) {
  if (index < 0 || index >= this->numPWL_)
    return 0;
  return this->d_[index];
}

double lefiAntennaPWL::PWLratio(int index) {
  if (index < 0 || index >= this->numPWL_)
    return 0;
  return this->r_[index];
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
//
//    lefiLayerDensity
//
////////////////////////////////////////////////////
////////////////////////////////////////////////////

lefiLayerDensity::lefiLayerDensity() {
}

void lefiLayerDensity::Init(const char* type) {
  int len = strlen(type) + 1;
  this->type_ = (char*)lefMalloc(len);
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

void lefiLayerDensity::Destroy() {
  if (this->type_) lefFree((char*)(this->type_));
  if (this->frequency_) lefFree((char*)(this->frequency_));
  if (this->widths_) lefFree((char*)(this->widths_));
  if (this->tableEntries_) lefFree((char*)(this->tableEntries_));
  if (this->cutareas_) lefFree((char*)(this->cutareas_));
}

lefiLayerDensity::~lefiLayerDensity() {
  this->lefiLayerDensity::Destroy();
}

void lefiLayerDensity::setOneEntry(double entry) {
  this->oneEntry_ = entry;
}

void lefiLayerDensity::addFrequency(int num, double* frequency) {
  this->numFrequency_ = num;
  this->frequency_ = frequency;
}

void lefiLayerDensity::addWidth(int num, double* width) {
  this->numWidths_ = num;
  this->widths_ = width;
}

void lefiLayerDensity::addTableEntry(int num, double* entry) {
  this->numTableEntries_ = num;
  this->tableEntries_ = entry;
}

void lefiLayerDensity::addCutarea(int num, double* cutarea) {
  this->numCutareas_ = num;
  this->cutareas_ = cutarea;
}

int lefiLayerDensity::hasOneEntry() {
  return this->oneEntry_ ? 1 : 0 ;
}

char* lefiLayerDensity::type() {
  return this->type_;
}

double lefiLayerDensity::oneEntry() {
  return this->oneEntry_;
}

int lefiLayerDensity::numFrequency() {
  return this->numFrequency_;
}

double lefiLayerDensity::frequency(int index) {
  return this->frequency_[index];
}

int lefiLayerDensity::numWidths() {
  return this->numWidths_;
}

double lefiLayerDensity::width(int index) {
  return this->widths_[index];
}

int lefiLayerDensity::numTableEntries() {
  return this->numTableEntries_;
}

double lefiLayerDensity::tableEntry(int index) {
  return this->tableEntries_[index];
}

int lefiLayerDensity::numCutareas() {
  return this->numCutareas_;
}

double lefiLayerDensity::cutArea(int index) {
  return this->cutareas_[index];
}


////////////////////////////////////////////////////
////////////////////////////////////////////////////
//
//    lefiParallel
//
////////////////////////////////////////////////////
////////////////////////////////////////////////////

lefiParallel::lefiParallel() {
  this->lefiParallel::Init();
}

void lefiParallel::Init() {
  this->numLength_ = 0;
  this->numWidth_ = 0;
  this->numWidthAllocated_ = 0;
}

void lefiParallel::Destroy() {

  if (this->numLength_)
     lefFree((char*)(this->length_));
  if (this->numWidth_) {
     lefFree((char*)(this->width_));
     lefFree((char*)(this->widthSpacing_));
  }
  this->lefiParallel::Init();
}

lefiParallel::~lefiParallel() {
  this->lefiParallel::Destroy();
}

void lefiParallel::addParallelLength(int numLength, double* lengths) {
  this->numLength_ = numLength;
  this->length_ = lengths;  
}

void lefiParallel::addParallelWidth(double width) {
  if (this->numWidth_ == this->numWidthAllocated_) {
     int        i;
     double*    nw;
     double*    nws;

     this->numWidthAllocated_ = this->numWidthAllocated_ ?
                                this->numWidthAllocated_ * 2 : 2;
     nw = (double*)lefMalloc(sizeof(double)*this->numWidthAllocated_);
     nws = (double*)lefMalloc(sizeof(double)*(this->numWidthAllocated_ *
                                             this->numLength_));

     for (i = 0; i < this->numWidth_; i++)
        nw[i] = this->width_[i];
     for (i = 0; i < (this->numWidth_ * this->numLength_); i++)
        nws[i] = this->widthSpacing_[i];
     if (this->numWidth_) {
        lefFree((char*)(this->width_));
        lefFree((char*)(this->widthSpacing_));
     }
     this->width_ = nw;
     this->widthSpacing_= nws;
  }
  this->width_[this->numWidth_] = width;
  this->numWidth_ += 1;
}

void lefiParallel::addParallelWidthSpacing(int numSpacing, double* spacings) {
  int i;
  for (i = 0; i < numSpacing; i++)
      this->widthSpacing_[(this->numWidth_-1) * this->numLength_ + i] = 
            spacings[i];
}

int lefiParallel::numLength() const {
  return this->numLength_;
}

int lefiParallel::numWidth() const {
  return this->numWidth_;
}

double lefiParallel::length(int index) const {
  return this->length_[index];
}

double lefiParallel::width(int index) const {
  return this->width_[index];
}

double lefiParallel::widthSpacing(int iWidth, int iWidthSpacing) const {
  return this->widthSpacing_[iWidth * this->numLength_ + iWidthSpacing];
}


////////////////////////////////////////////////////
////////////////////////////////////////////////////
//
//    lefiInfluence
//
////////////////////////////////////////////////////
////////////////////////////////////////////////////

lefiInfluence::lefiInfluence() {
  this->lefiInfluence::Init();
}

void lefiInfluence::Init() {
  this->numAllocated_ = 0;
  this->numWidth_ = 0;
  this->numDistance_ = 0;
  this->numSpacing_ = 0;
}

void lefiInfluence::Destroy() {
  if (this->numWidth_) {
     lefFree((char*)(this->width_));
     lefFree((char*)(this->distance_));
     lefFree((char*)(this->spacing_));
  }
  this->lefiInfluence::Init();
}

lefiInfluence::~lefiInfluence() {
  this->lefiInfluence::Destroy();
}

void lefiInfluence::addInfluence(double width, double distance, double spacing)
{
  int i;
  double* nw;
  double* nd;
  double* ns;

  if (this->numWidth_ == this->numAllocated_) {
     this->numAllocated_ = this->numAllocated_ ?
                           this->numAllocated_ * 2 : 2;
     nw = (double*)lefMalloc(sizeof(double)*this->numAllocated_);
     nd = (double*)lefMalloc(sizeof(double)*this->numAllocated_);
     ns = (double*)lefMalloc(sizeof(double)*this->numAllocated_);

     for (i = 0; i < this->numWidth_; i++) {
        nw[i] = this->width_[i];
        nd[i] = this->distance_[i];
        ns[i] = this->spacing_[i];
     }
     if (this->numWidth_ > 0) {
        lefFree((char*)(this->width_));
        lefFree((char*)(this->distance_));
        lefFree((char*)(this->spacing_));
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

int lefiInfluence::numInfluenceEntry() const {
  return this->numWidth_;
}

double lefiInfluence::width(int index) const {
  return this->width_[index];
}

double lefiInfluence::distance(int index) const {
  return this->distance_[index];
}

double lefiInfluence::spacing(int index) const {
  return this->spacing_[index];
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
//
//    lefiSpacingTable
//
////////////////////////////////////////////////////
////////////////////////////////////////////////////

lefiSpacingTable::lefiSpacingTable() {
  this->lefiSpacingTable::Init();
}

void lefiSpacingTable::Init() {
  this->hasInfluence_ = 0;
  this->parallel_ = 0;
  this->influence_ = 0;
}

void lefiSpacingTable::Destroy() {
  if ((this->hasInfluence_) && (this->influence_))
     this->influence_->lefiInfluence::Destroy();
  else if (this->parallel_) {
     this->parallel_->lefiParallel::Destroy();
     lefFree((char*)this->parallel_);
  }
  if (this->influence_)
     lefFree((char*)this->influence_);
  this->lefiSpacingTable::Init();
}

lefiSpacingTable::~lefiSpacingTable() {
  this->lefiSpacingTable::Destroy();
}

void lefiSpacingTable::addParallelLength(int numLength, double* lengths) {
  lefiParallel* parallel;

  if (this->parallel_ == 0) {
     parallel = (lefiParallel*)lefMalloc(sizeof(lefiParallel));
     parallel->lefiParallel::Init();
     this->parallel_ = parallel;
  }
  else
     parallel = this->parallel_;
  parallel->lefiParallel::addParallelLength(numLength, lengths);
}

void lefiSpacingTable::addParallelWidth(double width) {
  lefiParallel* parallel;

  parallel = this->parallel_;
  parallel->lefiParallel::addParallelWidth(width);
}

void lefiSpacingTable::addParallelWidthSpacing(int numSpacing, double* spacings)
{
  lefiParallel* parallel;

  parallel = this->parallel_;
  parallel->lefiParallel::addParallelWidthSpacing(numSpacing, spacings);
}

void lefiSpacingTable::setInfluence() {
  lefiInfluence* influence;

  influence = (lefiInfluence*)lefMalloc(sizeof(lefiInfluence));
  influence->lefiInfluence::Init();
  this->influence_ = influence;
  this->hasInfluence_ = 1;
}

void lefiSpacingTable::addInfluence(double width, double distance,
                                    double spacing) {
  lefiInfluence* influence;

  influence = this->influence_;
  influence->lefiInfluence::addInfluence(width, distance, spacing);
}

int lefiSpacingTable::isInfluence() const {
  return this->hasInfluence_ ? 1 : 0;
}

lefiInfluence* lefiSpacingTable::influence() const {
  return this->influence_;
}

lefiParallel* lefiSpacingTable::parallel() const {
  return this->parallel_;
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
//
//    lefiAntennaModel
//
////////////////////////////////////////////////////
////////////////////////////////////////////////////

lefiAntennaModel::lefiAntennaModel() {
  this->lefiAntennaModel::Init();
}

void lefiAntennaModel::Init() {
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
  this->antennaCumDiffSideAreaRatioPWL_= 0;
  this->oxide_ = 0;
}

void lefiAntennaModel::Destroy() {
  if (this->oxide_)
    lefFree((char*)(this->oxide_));

  if (this->antennaDiffAreaRatioPWL_) {
    this->antennaDiffAreaRatioPWL_->lefiAntennaPWL::Destroy();
    lefFree((char*)(this->antennaDiffAreaRatioPWL_));
    this->antennaDiffAreaRatioPWL_ = 0;
  }
  if (this->antennaCumDiffAreaRatioPWL_) {
    this->antennaCumDiffAreaRatioPWL_->lefiAntennaPWL::Destroy();
    lefFree((char*)(this->antennaCumDiffAreaRatioPWL_));
    this->antennaCumDiffAreaRatioPWL_ = 0;
  }
  if (this->antennaDiffSideAreaRatioPWL_) {
    this->antennaDiffSideAreaRatioPWL_->lefiAntennaPWL::Destroy();
    lefFree((char*)(this->antennaDiffSideAreaRatioPWL_));
    this->antennaDiffSideAreaRatioPWL_ = 0;
  }
  if (this->antennaCumDiffSideAreaRatioPWL_) {
    this->antennaCumDiffSideAreaRatioPWL_->lefiAntennaPWL::Destroy();
    lefFree((char*)(this->antennaCumDiffSideAreaRatioPWL_));
    this->antennaCumDiffSideAreaRatioPWL_ = 0;
  }
  this->lefiAntennaModel::Init();
}

lefiAntennaModel::~lefiAntennaModel() {
  this->lefiAntennaModel::Destroy();
}

// 5.5
void lefiAntennaModel::setAntennaModel(int aOxide) {
  switch (aOxide) {
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
           this->oxide_ = strdup("OXIDE1"); 
           break;
  }
}

// 3/23/2000 -- Wanda da Rosa.  The following are for 5.4 syntax
void lefiAntennaModel::setAntennaAreaRatio(double value) {
  this->antennaAreaRatio_ = value;
  this->hasAntennaAreaRatio_ = 1;
}

void lefiAntennaModel::setAntennaCumAreaRatio(double value) {
  this->antennaCumAreaRatio_ = value;
  this->hasAntennaCumAreaRatio_ = 1;
}

void lefiAntennaModel::setAntennaAreaFactor(double value) {
  this->antennaAreaFactor_ = value;
  this->hasAntennaAreaFactor_ = 1;
}

void lefiAntennaModel::setAntennaSideAreaRatio(double value) {
  this->antennaSideAreaRatio_ = value;
  this->hasAntennaSideAreaRatio_ = 1;
}

void lefiAntennaModel::setAntennaCumSideAreaRatio(double value) {
  this->antennaCumSideAreaRatio_ = value;
  this->hasAntennaCumSideAreaRatio_ = 1;
}

void lefiAntennaModel::setAntennaSideAreaFactor(double value) {
  this->antennaSideAreaFactor_ = value;
  this->hasAntennaSideAreaFactor_ = 1;
}

void lefiAntennaModel::setAntennaValue(lefiAntennaEnum antennaType, double value) {
  switch (antennaType) {
    case lefiAntennaDAR:
         this->antennaDiffAreaRatio_ = value;
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

void lefiAntennaModel::setAntennaDUO(lefiAntennaEnum antennaType) {
  switch (antennaType) {
    case lefiAntennaAF:
         this->hasAntennaAreaFactorDUO_ = 1;
         break;
    case lefiAntennaSAF:
         this->hasAntennaSideAreaFactorDUO_ = 1;
         break;
    default:
         break;
  }
}

void lefiAntennaModel::setAntennaPWL(lefiAntennaEnum antennaType, lefiAntennaPWL* pwl)
{
  switch (antennaType) {
    case lefiAntennaDAR:
         this->antennaDiffAreaRatioPWL_ = pwl;
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

int lefiAntennaModel::hasAntennaAreaRatio() const {
  return this->hasAntennaAreaRatio_;
}

int lefiAntennaModel::hasAntennaDiffAreaRatio() const {
  return this->hasAntennaDiffAreaRatio_;
}

int lefiAntennaModel::hasAntennaCumAreaRatio() const {
  return this->hasAntennaCumAreaRatio_;
}

int lefiAntennaModel::hasAntennaCumDiffAreaRatio() const {
  return this->hasAntennaCumDiffAreaRatio_;
}

int lefiAntennaModel::hasAntennaAreaFactor() const {
  return this->hasAntennaAreaFactor_;
}

int lefiAntennaModel::hasAntennaSideAreaRatio() const {
  return this->hasAntennaSideAreaRatio_;
}

int lefiAntennaModel::hasAntennaDiffSideAreaRatio() const {
  return this->hasAntennaDiffSideAreaRatio_;
}

int lefiAntennaModel::hasAntennaCumSideAreaRatio() const {
  return this->hasAntennaCumSideAreaRatio_;
}

int lefiAntennaModel::hasAntennaCumDiffSideAreaRatio() const {
  return this->hasAntennaCumDiffSideAreaRatio_;
}

int lefiAntennaModel::hasAntennaSideAreaFactor() const {
  return this->hasAntennaSideAreaFactor_;
}

int lefiAntennaModel::hasAntennaDiffAreaRatioPWL() const {
  return this->antennaDiffAreaRatioPWL_ ? 1 : 0 ;
}

int lefiAntennaModel::hasAntennaCumDiffAreaRatioPWL() const {
  return this->antennaCumDiffAreaRatioPWL_ ? 1 : 0 ;
}

int lefiAntennaModel::hasAntennaDiffSideAreaRatioPWL() const {
  return this->antennaDiffSideAreaRatioPWL_ ? 1 : 0 ;
}

int lefiAntennaModel::hasAntennaCumDiffSideAreaRatioPWL() const {
  return this->antennaCumDiffSideAreaRatioPWL_ ? 1 : 0 ;
}

int lefiAntennaModel::hasAntennaAreaFactorDUO() const {
  return this->hasAntennaAreaFactorDUO_;
}

int lefiAntennaModel::hasAntennaSideAreaFactorDUO() const {
  return this->hasAntennaSideAreaFactorDUO_;
}

// 5.5
char* lefiAntennaModel::antennaOxide() const {
  return this->oxide_;
}

double lefiAntennaModel::antennaAreaRatio() const {
  return this->antennaAreaRatio_;
}

double lefiAntennaModel::antennaDiffAreaRatio() const {
  return this->antennaDiffAreaRatio_;
}

double lefiAntennaModel::antennaCumAreaRatio() const {
  return this->antennaCumAreaRatio_;
}

double lefiAntennaModel::antennaCumDiffAreaRatio() const {
  return this->antennaCumDiffAreaRatio_;
}

double lefiAntennaModel::antennaAreaFactor() const {
  return this->antennaAreaFactor_;
}

double lefiAntennaModel::antennaSideAreaRatio() const {
  return this->antennaSideAreaRatio_;
}

double lefiAntennaModel::antennaDiffSideAreaRatio() const {
  return this->antennaDiffSideAreaRatio_;
}

double lefiAntennaModel::antennaCumSideAreaRatio() const {
  return this->antennaCumSideAreaRatio_;
}

double lefiAntennaModel::antennaCumDiffSideAreaRatio() const {
  return this->antennaCumDiffSideAreaRatio_;
}

double lefiAntennaModel::antennaSideAreaFactor() const {
  return this->antennaSideAreaFactor_;
}

lefiAntennaPWL* lefiAntennaModel::antennaDiffAreaRatioPWL() const {
  return this->antennaDiffAreaRatioPWL_;
}

lefiAntennaPWL* lefiAntennaModel::antennaCumDiffAreaRatioPWL() const {
  return this->antennaCumDiffAreaRatioPWL_;
}

lefiAntennaPWL* lefiAntennaModel::antennaDiffSideAreaRatioPWL() const {
  return this->antennaDiffSideAreaRatioPWL_;
}

lefiAntennaPWL* lefiAntennaModel::antennaCumDiffSideAreaRatioPWL() const {
  return this->antennaCumDiffSideAreaRatioPWL_;
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
//
//    lefiLayer
//
////////////////////////////////////////////////////
////////////////////////////////////////////////////

lefiLayer::lefiLayer() {
  this->lefiLayer::Init();
}


void lefiLayer::Init() {
  this->name_ = (char*)lefMalloc(16);
  this->nameSize_ = 16;
  this->type_ = (char*)lefMalloc(16);
  this->typeSize_ = 16;
  this->numSpacings_ = 0;
  this->spacingsAllocated_ = 0;
  this->numMinimumcut_ = 0;
  this->minimumcutAllocated_ = 0;
  this->numMinenclosedarea_ = 0;
  this->minenclosedareaAllocated_ = 0;
  this->numCurrentPoints_ = 0;
  this->currentPointsAllocated_ = 2;
  this->currentWidths_ = (double*)lefMalloc(sizeof(double)*2);
  this->current_ = (double*)lefMalloc(sizeof(double)*2);
  this->numResistancePoints_ = 0;
  this->resistancePointsAllocated_ = 2;
  this->resistanceWidths_ = (double*)lefMalloc(sizeof(double)*2);
  this->resistances_ = (double*)lefMalloc(sizeof(double)*2);
  this->numCapacitancePoints_ = 0;
  this->capacitancePointsAllocated_ = 2;
  this->capacitanceWidths_ = (double*)lefMalloc(sizeof(double)*2);
  this->capacitances_ = (double*)lefMalloc(sizeof(double)*2);
  this->numProps_ = 0;
  this->propsAllocated_ = 1;
  this->names_ = (char**)lefMalloc(sizeof(char*));
  this->values_ = (char**)lefMalloc(sizeof(char*));
  this->dvalues_ = (double*)lefMalloc(sizeof(double));
  this->types_ = (char*)lefMalloc(sizeof(char));
  this->numAccurrents_ = 0;
  this->accurrentAllocated_ = 0;
  this->numDccurrents_ = 0;
  this->dccurrentAllocated_ = 0;
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->numSpacingTable_ = 0;
  this->spacingTableAllocated_ = 0;
  this->lefiLayer::clear();
}


void lefiLayer::Destroy() {

  this->lefiLayer::clear();
  lefFree(this->name_);
  lefFree(this->type_);
  if (this->spacing_) lefFree((char*)(this->spacing_));
  if (this->spacingName_) lefFree((char*)(this->spacingName_));
  if (this->spacingAdjacentCuts_) lefFree((char*)(this->spacingAdjacentCuts_));
  if (this->spacingAdjacentWithin_) lefFree((char*)(this->spacingAdjacentWithin_));
  if (this->hasSpacingName_) lefFree((char*)(this->hasSpacingName_));
  if (this->hasSpacingAdjacent_) lefFree((char*)(this->hasSpacingAdjacent_));
  if (this->rangeMin_) lefFree((char*)(this->rangeMin_));
  if (this->rangeMax_) lefFree((char*)(this->rangeMax_));
  if (this->rangeInfluence_) lefFree((char*)(this->rangeInfluence_));
  if (this->rangeInfluenceRangeMin_)
      lefFree((char*)(this->rangeInfluenceRangeMin_));
  if (this->rangeInfluenceRangeMax_)
      lefFree((char*)(this->rangeInfluenceRangeMax_));
  if (this->rangeRangeMin_) lefFree((char*)(this->rangeRangeMin_));
  if (this->rangeRangeMax_) lefFree((char*)(this->rangeRangeMax_));
  if (this->lengthThreshold_) lefFree((char*)(this->lengthThreshold_));
  if (this->lengthThresholdRangeMin_)
      lefFree((char*)(this->lengthThresholdRangeMin_));
  if (this->lengthThresholdRangeMax_) lefFree((char*)(this->lengthThresholdRangeMax_));
  if (this->hasSpacingRange_)
      lefFree((char*)(this->hasSpacingRange_));
  if (this->hasSpacingUseLengthThreshold_)
      lefFree((char*)(this->hasSpacingUseLengthThreshold_));
  if (this->hasSpacingLengthThreshold_)
      lefFree((char*)(this->hasSpacingLengthThreshold_));
  lefFree((char*)(this->currentWidths_));
  lefFree((char*)(this->current_));
  lefFree((char*)(this->resistanceWidths_));
  lefFree((char*)(this->resistances_));
  lefFree((char*)(this->capacitanceWidths_));
  lefFree((char*)(this->capacitances_));
  lefFree((char*)(this->names_));
  lefFree((char*)(this->values_));
  lefFree((char*)(this->dvalues_));
  lefFree((char*)(this->types_));
}


lefiLayer::~lefiLayer() {
  this->lefiLayer::Destroy();
}


void lefiLayer::clear() {
  int i;
  lefiLayerDensity* p;
  lefiSpacingTable* sp;
  lefiAntennaModel* aO;         // 5.5

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
  for (i = 0; i < this->numSpacings_; i++)
    if (this->spacingName_[i])
       lefFree(this->spacingName_[i]);
  for (i = 0; i < this->numProps_; i++) {
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
     for (i = 0; i < this->numAccurrents_; i++) {
       p = this->accurrents_[i];
       p->lefiLayerDensity::Destroy();
       lefFree((char*)p);
     }
     this->numAccurrents_ = 0;
     this->accurrentAllocated_ = 0;
     lefFree((char*)(this->accurrents_));
     this->accurrents_ = 0;
  }
  if (this->numDccurrents_) {
     for (i = 0; i < this->numDccurrents_; i++) {
       p = this->dccurrents_[i];
       p->lefiLayerDensity::Destroy();
       lefFree((char*)p);
     }
     this->numDccurrents_ = 0;
     this->dccurrentAllocated_ = 0;
     lefFree((char*)(this->dccurrents_));
     this->dccurrents_ = 0;
  }
  // 8/29/2001 - Wanda da Rosa.  The following are 5.4 enhancements
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
  // 5.5
  if (this->numMinimumcut_ > 0) {
     // Has allocated memories
     lefFree((char*)(this->minimumcut_));
     lefFree((char*)(this->minimumcutWidth_));
     lefFree((char*)(this->hasMinimumcutConnection_));
     lefFree((char*)(this->hasMinimumcutNumCuts_));
     lefFree((char*)(this->minimumcutLength_));
     lefFree((char*)(this->minimumcutDistance_));
     for (i = 0; i < this->numMinimumcut_; i++) {
        if (this->minimumcutConnection_[i])
           lefFree((char*)(this->minimumcutConnection_[i]));
     }
     lefFree((char*)(this->minimumcutConnection_));
     this->numMinimumcut_ = 0;
     this->minimumcutAllocated_ = 0;
  }
  this->maxwidth_ = -1;
  this->minwidth_ = -1;
  if (this->numMinenclosedarea_ > 0) {
     lefFree((char*)(this->minenclosedarea_));
     lefFree((char*)(this->minenclosedareaWidth_));
     this->numMinenclosedarea_ = 0;
     this->minenclosedareaAllocated_ = 0;
  }
  this->minstep_ = -1;
  this->protrusionWidth1_ = -1;
  this->protrusionLength_ = -1;
  this->protrusionWidth2_ = -1;
  if (this->numSpacingTable_) {
     for (i = 0; i < this->numSpacingTable_; i++) {
       sp = this->spacingTable_[i];
       sp->lefiSpacingTable::Destroy();
       lefFree((char*)sp); 
     }
  }
  this->numSpacingTable_ = 0;
  this->spacingTableAllocated_ = 0;

  for (i = 0; i < this->numAntennaModel_; i++) {  // 5.5
     aO = this->antennaModel_[i];
     aO->lefiAntennaModel::Destroy();
  }
  for (i = 0; i < this->antennaModelAllocated_; i++) { // 5.5
     lefFree((char*)this->antennaModel_[i]);
  }
     
  if (this->antennaModel_)                        // 5.5
     lefFree((char*)(this->antennaModel_));
  this->antennaModel_ = 0;
  this->numAntennaModel_ = 0;
  this->antennaModelAllocated_ = 0;

  if (this->nums_) lefFree((char*)(this->nums_));
}
  

void lefiLayer::setName(const char* name) {
  int len = strlen(name) + 1;
  this->lefiLayer::clear();
  if (len > this->nameSize_) {
    lefFree(this->name_);
    this->name_ = (char*)lefMalloc(len);
    this->nameSize_ = len;
  }
  strcpy(this->name_, CASE(name));
}


void lefiLayer::setType(const char* typ) {
  int len = strlen(typ) + 1;
  if (len > this->typeSize_) {
    lefFree(this->type_);
    this->type_ = (char*)lefMalloc(len);
    this->typeSize_ = len;
  }
  strcpy(this->type_, CASE(typ));
}


void lefiLayer::setPitch(double num) {
  this->hasPitch_ = 1;
  this->pitch_ = num;
}


void lefiLayer::setOffset(double num) {
  this->hasOffset_ = 1;
  this->offset_ = num;
}


void lefiLayer::setWidth(double num) {
  this->hasWidth_ = 1;
  this->width_ = num;
}


void lefiLayer::setArea(double num) {
  this->hasArea_ = 1;
  this->area_ = num;
}


void lefiLayer::setWireExtension(double num) {
  this->hasWireExtension_ = 1;
  this->wireExtension_ = num;
}

// 5.5
void lefiLayer::setMaxwidth(double width) {
  this->maxwidth_ = width;
}

// 5.5
void lefiLayer::setMinwidth(double width) {
  this->minwidth_ = width;
}

// 5.5
void lefiLayer::addMinenclosedarea(double area) {
  if (this->numMinenclosedarea_ == this->minenclosedareaAllocated_) {
    double* na;
    double* nw;
    int     i, lim;

    if (this->minenclosedareaAllocated_ == 0) {
       lim = this->minenclosedareaAllocated_ = 2;
       na = (double*)lefMalloc(sizeof(double)*lim);
       nw = (double*)lefMalloc(sizeof(double)*lim);
    } else {
       lim = this->minenclosedareaAllocated_ * 2;
       this->minenclosedareaAllocated_ = lim;
       na = (double*)lefMalloc(sizeof(double)*lim);
       nw = (double*)lefMalloc(sizeof(double)*lim);
       lim /= 2;
       for (i = 0; i < lim; i++) {
          na[i] = this->minenclosedarea_[i];
          nw[i] = this->minenclosedareaWidth_[i];
       }
       lefFree((char*)(this->minenclosedarea_));
       lefFree((char*)(this->minenclosedareaWidth_));
    }
    this->minenclosedarea_ = na;
    this->minenclosedareaWidth_ = nw;
  }
  this->minenclosedarea_[this->numMinenclosedarea_] = area;
  this->minenclosedareaWidth_[this->numMinenclosedarea_] = -1;
  this->numMinenclosedarea_ += 1;
}

// 5.5
void lefiLayer::addMinenclosedareaWidth(double width) {
  this->minenclosedareaWidth_[this->numMinenclosedarea_ - 1] = width;
}

// 5.5
void lefiLayer::addMinimumcut(int mincut, double width) {
  if (this->numMinimumcut_ == this->minimumcutAllocated_) {
    int*    nc;
    double* nw;
    int*    hm;
    char**  nud;
    int*    hc;
    double* nl;
    double* nd;
    int i, lim;

    if (this->minimumcutAllocated_ == 0) {
       lim = this->minimumcutAllocated_ = 2;
       nc = (int*)lefMalloc(sizeof(int)*lim);
       nw = (double*)lefMalloc(sizeof(double)*lim);
       hm = (int*)lefMalloc(sizeof(int)*lim);
       nud = (char**)lefMalloc(sizeof(char*)*lim);
       hc = (int*)lefMalloc(sizeof(int)*lim);
       nl = (double*)lefMalloc(sizeof(double)*lim);
       nd = (double*)lefMalloc(sizeof(double)*lim);
    } else {
       lim = this->minimumcutAllocated_ * 2;
       this->minimumcutAllocated_ = lim;
       nc = (int*)lefMalloc(sizeof(int)*lim);
       nw = (double*)lefMalloc(sizeof(double)*lim);
       hm = (int*)lefMalloc(sizeof(int)*lim);
       nud = (char**)lefMalloc(sizeof(char*)*lim);
       hc = (int*)lefMalloc(sizeof(int)*lim);
       nl = (double*)lefMalloc(sizeof(double)*lim);
       nd = (double*)lefMalloc(sizeof(double)*lim);
       lim /= 2;
       for (i = 0; i < lim; i++) {
          nc[i] = this->minimumcut_[i];
          nw[i] = this->minimumcutWidth_[i];
          hm[i] = this->hasMinimumcutConnection_[i];
          nud[i] = this->minimumcutConnection_[i];
          hc[i] = this->hasMinimumcutNumCuts_[i];
          nl[i] = this->minimumcutLength_[i];
          nd[i] = this->minimumcutDistance_[i];
       }
       lefFree((char*)(this->minimumcut_));
       lefFree((char*)(this->minimumcutWidth_));
       lefFree((char*)(this->hasMinimumcutConnection_));
       lefFree((char*)(this->minimumcutConnection_));
       lefFree((char*)(this->hasMinimumcutNumCuts_));
       lefFree((char*)(this->minimumcutLength_));
       lefFree((char*)(this->minimumcutDistance_));
    }
    this->minimumcut_ = nc;
    this->minimumcutWidth_ = nw;
    this->hasMinimumcutConnection_ = hm;
    this->minimumcutConnection_ = nud;
    this->hasMinimumcutNumCuts_ = hc;
    this->minimumcutLength_ = nl;
    this->minimumcutDistance_ = nd;
  }
  this->minimumcut_[this->numMinimumcut_]= mincut;
  this->minimumcutWidth_[this->numMinimumcut_] = width;
  this->hasMinimumcutConnection_[this->numMinimumcut_] = 0;
  this->minimumcutConnection_[this->numMinimumcut_] = 0;
  this->hasMinimumcutNumCuts_[this->numMinimumcut_] = 0;
  this->minimumcutLength_[this->numMinimumcut_] = 0;
  this->minimumcutDistance_[this->numMinimumcut_] = 0;
  this->numMinimumcut_ += 1;
}


// 5.5
void lefiLayer::addMinimumcutConnect(const char* connectType) {
  if (connectType && (strcmp(connectType,"") != 0)) {
     this->hasMinimumcutConnection_[this->numMinimumcut_ - 1] = 1;
     this->minimumcutConnection_[this->numMinimumcut_ - 1] =
         strdup(connectType); 
  }
}

// 5.5
void lefiLayer::addMinimumcutLengDis(double length, double width) {
  this->hasMinimumcutNumCuts_[this->numMinimumcut_ - 1] = 1;
  this->minimumcutLength_[this->numMinimumcut_ - 1] = length;
  this->minimumcutDistance_[this->numMinimumcut_ - 1] = width;
}

// 5.5
void lefiLayer::setMinstep(double distance) {
  this->minstep_ = distance;
}

// 5.5
void lefiLayer::setProtrusion(double width1, double length, double width2) {
  this->protrusionWidth1_ = width1;
  this->protrusionLength_ = length;
  this->protrusionWidth2_ = width2;
}


// wmd -- pcr 282799, need to make left_, right_ as arrays
//        when bumping to new list, need to copy also hasUseLengthThreshold_
//        and lengthThreshold_
void lefiLayer::setSpacingMin(double dist) {
  if (this->numSpacings_ == this->spacingsAllocated_) {
    double* nd;
    char** nn;             // Also set up the spacing name
    int    *nsn;           // hasSpacingName_
    int    *nsa;           // hasSpacingAdjacent_
    int    *nr;            // hasSpacingRange_
    int    *nac;           // adjacentCuts_
    double *naw;           // adjacentWithin_
    double *nrmin, *nrmax; // rangeMin_, rangeMax_
    double *nri, *nrimin, *nrimax; // rangeInfluence_, rangeInfluenceRangeMin_,
                                   // rangeInfluenceRangeMax_
    double *nrrmin, *nrrmax;       // rangeRangeMin_, rangeRangeMax_
    int    *ht;            // hasSpacingUseLengthThreshold_
    int    *nl;            // hasSpacingLengthThreshold_
    double *nt, *ntmin, *ntmax; // lengthThreshold_, lengthThresholdMin_
                                // lengthThresholdMax_
    
    int i, lim;
    if (this->spacingsAllocated_ == 0) {
       lim = this->spacingsAllocated_ = 2;
       nd = (double*)lefMalloc(sizeof(double)*lim);   
       nn = (char**)lefMalloc(sizeof(char*)*lim);
       nac = (int*)lefMalloc(sizeof(int)*lim);
       naw = (double*)lefMalloc(sizeof(double)*lim);
       nsn = (int*)lefMalloc(sizeof(int)*lim);
       nsa = (int*)lefMalloc(sizeof(int)*lim);
       nr = (int*)lefMalloc(sizeof(int)*lim);
       nrmin = (double*)lefMalloc(sizeof(double)*lim);   
       nrmax = (double*)lefMalloc(sizeof(double)*lim);   
       nri = (double*)lefMalloc(sizeof(double)*lim);   
       nrimin = (double*)lefMalloc(sizeof(double)*lim);   
       nrimax = (double*)lefMalloc(sizeof(double)*lim);   
       nrrmin = (double*)lefMalloc(sizeof(double)*lim);   
       nrrmax = (double*)lefMalloc(sizeof(double)*lim);   
       ht = (int*)lefMalloc(sizeof(int)*lim);
       nl = (int*)lefMalloc(sizeof(int)*lim);
       nt = (double*)lefMalloc(sizeof(double)*lim);   
       ntmin = (double*)lefMalloc(sizeof(double)*lim);   
       ntmax = (double*)lefMalloc(sizeof(double)*lim);   
    } else {
       lim = this->spacingsAllocated_ * 2;
       this->spacingsAllocated_ = lim;
       nd = (double*)lefMalloc(sizeof(double)*lim);
       nn = (char**)lefMalloc(sizeof(char*)*lim);
       nac = (int*)lefMalloc(sizeof(int)*lim);
       naw = (double*)lefMalloc(sizeof(double)*lim);
       nsn = (int*)lefMalloc(sizeof(int)*lim);
       nsa = (int*)lefMalloc(sizeof(int)*lim);
       nr = (int*)lefMalloc(sizeof(int)*lim);
       nrmin = (double*)lefMalloc(sizeof(double)*lim);
       nrmax = (double*)lefMalloc(sizeof(double)*lim);
       nri = (double*)lefMalloc(sizeof(double)*lim);
       nrimin = (double*)lefMalloc(sizeof(double)*lim);
       nrimax = (double*)lefMalloc(sizeof(double)*lim);
       nrrmin = (double*)lefMalloc(sizeof(double)*lim);
       nrrmax = (double*)lefMalloc(sizeof(double)*lim);
       ht = (int*)lefMalloc(sizeof(int)*lim);
       nl = (int*)lefMalloc(sizeof(int)*lim);
       nt = (double*)lefMalloc(sizeof(double)*lim);
       ntmin = (double*)lefMalloc(sizeof(double)*lim);
       ntmax = (double*)lefMalloc(sizeof(double)*lim);
       lim /= 2;
       for (i = 0; i < lim; i++) {
         nd[i] = this->spacing_[i];
         if (this->spacingName_[i])  // is null if is not CUT layer
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
       lefFree((char*)(this->spacing_));
       lefFree((char*)(this->spacingName_));
       lefFree((char*)(this->spacingAdjacentCuts_));
       lefFree((char*)(this->spacingAdjacentWithin_));
       lefFree((char*)(this->hasSpacingName_));
       lefFree((char*)(this->hasSpacingAdjacent_));
       lefFree((char*)(this->hasSpacingRange_));
       lefFree((char*)(this->rangeMin_));
       lefFree((char*)(this->rangeMax_));
       lefFree((char*)(this->rangeInfluence_));
       lefFree((char*)(this->rangeInfluenceRangeMin_));
       lefFree((char*)(this->rangeInfluenceRangeMax_));
       lefFree((char*)(this->rangeRangeMin_));
       lefFree((char*)(this->rangeRangeMax_));
       lefFree((char*)(this->hasSpacingUseLengthThreshold_));
       lefFree((char*)(this->hasSpacingLengthThreshold_));
       lefFree((char*)(this->lengthThreshold_));
       lefFree((char*)(this->lengthThresholdRangeMin_));
       lefFree((char*)(this->lengthThresholdRangeMax_));
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


void lefiLayer::setSpacingRange(double left, double right) {
  this->rangeMin_[this->numSpacings_-1] = left;
  this->rangeMax_[this->numSpacings_-1] = right;
  this->hasSpacingRange_[this->numSpacings_-1] = 1;
}


void lefiLayer::setSpacingName(const char* spacingName) {
  if (spacingName) {
     int len = strlen(spacingName) + 1;
     this->spacingName_[this->numSpacings_-1] = (char*)lefMalloc(len);
     strcpy(this->spacingName_[this->numSpacings_-1], CASE(spacingName));
     this->hasSpacingName_[this->numSpacings_-1] = 1;
  }
}


void lefiLayer::setSpacingAdjacent(int numCuts, double distance) {
  this->spacingAdjacentCuts_[this->numSpacings_-1] = numCuts;
  this->spacingAdjacentWithin_[this->numSpacings_-1] = distance;
  this->hasSpacingAdjacent_[this->numSpacings_-1] = 1;
}


void lefiLayer::setSpacingRangeUseLength() {
  this->hasSpacingUseLengthThreshold_[this->numSpacings_-1] = 1;
}


void lefiLayer::setSpacingRangeInfluence(double infLength) {
  this->rangeInfluence_[this->numSpacings_-1] = infLength;
}


void lefiLayer::setSpacingRangeInfluenceRange(double min, double max) {
  this->rangeInfluenceRangeMin_[this->numSpacings_-1] = min;
  this->rangeInfluenceRangeMax_[this->numSpacings_-1] = max;
}


void lefiLayer::setSpacingRangeRange(double min, double max) {
  this->rangeRangeMin_[this->numSpacings_-1] = min;
  this->rangeRangeMax_[this->numSpacings_-1] = max;
}


void lefiLayer::setSpacingLength(double num) {
  this->lengthThreshold_[this->numSpacings_-1] = num;
  this->hasSpacingLengthThreshold_[this->numSpacings_-1] = 1;
}


void lefiLayer::setSpacingLengthRange(double min, double max) {
  this->lengthThresholdRangeMin_[this->numSpacings_-1] = min;
  this->lengthThresholdRangeMax_[this->numSpacings_-1] = max;
}


void lefiLayer::setDirection(const char* dir) {
  this->direction_ = (char*)dir;
  this->hasDirection_ = 1;
}


void lefiLayer::setResistance(double num) {
  this->hasResistance_ = 1;
  this->resistance_ = num;
}


void lefiLayer::setCapacitance(double num) {
  this->hasCapacitance_ = 1;
  this->capacitance_ = num;
}


void lefiLayer::setHeight(double num) {
  this->hasHeight_ = 1;
  this->height_ = num;
}


void lefiLayer::setThickness(double num) {
  this->hasThickness_ = 1;
  this->thickness_ = num;
}


void lefiLayer::setShrinkage(double num) {
  this->hasShrinkage_ = 1;
  this->shrinkage_ = num;
}


void lefiLayer::setCapMultiplier(double num) {
  this->hasCapMultiplier_ = 1;
  this->capMultiplier_ = num;
}


void lefiLayer::setEdgeCap(double num) {
  this->hasEdgeCap_ = 1;
  this->edgeCap_ = num;
}


void lefiLayer::setAntennaLength(double num) {
  this->hasAntennaLength_ = 1;
  this->antennaLength_ = num;
}


void lefiLayer::setAntennaArea(double num) {
  this->hasAntennaArea_ = 1;
  this->antennaArea_ = num;
}


void lefiLayer::setCurrentDensity(double num) {
  this->hasCurrentDensityPoint_ = 1;
  this->currentDensity_ = num;
}


void lefiLayer::setCurrentPoint(double width, double current) {
  if (this->numCurrentPoints_ == this->currentPointsAllocated_) {
    int max = this->numCurrentPoints_;
    int len;
    int i;
    double* nc;
    double* nw;

    if (this->currentPointsAllocated_ == 0)
      len = this->currentPointsAllocated_ = 2;
    else
      len = this->currentPointsAllocated_ *= 2;
    nc = (double*)lefMalloc(sizeof(double)*len);
    nw = (double*)lefMalloc(sizeof(double)*len);

    for (i = 0; i < max; i++) {
      nc[i] = this->current_[i];
      nw[i] = this->currentWidths_[i];
    }
    lefFree((char*)(this->current_));
    lefFree((char*)(this->currentWidths_));
    this->current_ = nc;
    this->currentWidths_ = nw;
  }
  this->current_[this->numCurrentPoints_] = current;
  this->currentWidths_[this->numCurrentPoints_] = width;
  this->numCurrentPoints_ += 1;
}


void lefiLayer::setResistancePoint(double width, double resistance) {
  if (this->numResistancePoints_ == this->resistancePointsAllocated_) {
    int max = this->numResistancePoints_;
    int len;
    int i;
    double* nc;
    double* nw;

    if (this->resistancePointsAllocated_ == 0)
      len = this->resistancePointsAllocated_ = 2;
    else
      len = this->resistancePointsAllocated_ *= 2;
    nc = (double*)lefMalloc(sizeof(double)*len);
    nw = (double*)lefMalloc(sizeof(double)*len);
    for (i = 0; i < max; i++) {
      nc[i] = this->resistances_[i];
      nw[i] = this->resistanceWidths_[i];
    }
    lefFree((char*)(this->resistances_));
    lefFree((char*)(this->resistanceWidths_));
    this->resistances_ = nc;
    this->resistanceWidths_ = nw;
  }
  this->resistances_[this->numResistancePoints_] = resistance;
  this->resistanceWidths_[this->numResistancePoints_] = width;
  this->numResistancePoints_ += 1;
}


void lefiLayer::setCapacitancePoint(double width, double capacitance) {
  if (this->numCapacitancePoints_ == this->capacitancePointsAllocated_) {
    int max = this->numCapacitancePoints_;
    int len;
    int i;
    double* nc;
    double* nw;

    if (this->capacitancePointsAllocated_ == 0)
      len = this->capacitancePointsAllocated_ = 2;
    else
      len = this->capacitancePointsAllocated_ *= 2;
    nc = (double*)lefMalloc(sizeof(double)*len);
    nw = (double*)lefMalloc(sizeof(double)*len);
    for (i = 0; i < max; i++) {
      nc[i] = this->capacitances_[i];
      nw[i] = this->capacitanceWidths_[i];
    }
    lefFree((char*)(this->capacitances_));
    lefFree((char*)(this->capacitanceWidths_));
    this->capacitances_ = nc;
    this->capacitanceWidths_ = nw;
  }
  this->capacitances_[this->numCapacitancePoints_] = capacitance;
  this->capacitanceWidths_[this->numCapacitancePoints_] = width;
  this->numCapacitancePoints_ += 1;
}

int lefiLayer::hasType() const {
  return (this->type_[0] != '\0') ? 1 : 0 ;
}


int lefiLayer::hasPitch() const {
  return this->hasPitch_;
}


int lefiLayer::hasOffset() const {
  return this->hasOffset_;
}


int lefiLayer::hasWidth() const {
  return this->hasWidth_;
}


int lefiLayer::hasArea() const {
  return this->hasArea_;
}


int lefiLayer::hasWireExtension() const {
  return this->hasWireExtension_;
}


int lefiLayer::hasSpacingNumber() const {
  return ((this->hasSpacing_ != 0) && (this->numSpacings_ > 0)) ? 1 : 0;
}


int lefiLayer::hasSpacingName(int index) const {
  return ((this->hasSpacing_ != 0) &&
          (this->hasSpacingName_[index] != 0)) ? 1 : 0;
}


int lefiLayer::hasSpacingAdjacent(int index) const {
  return ((this->hasSpacing_ != 0) &&
          (this->hasSpacingAdjacent_[index] != 0)) ? 1 : 0;
}


int lefiLayer::hasSpacingRange(int index) const {
  return ((this->hasSpacing_ != 0) &&
          (this->hasSpacingRange_[index] != 0)) ? 1 : 0;
}


int lefiLayer::hasSpacingRangeUseLengthThreshold(int index) const {
  return (this->hasSpacingUseLengthThreshold_[index]);
}


int lefiLayer::hasSpacingRangeInfluence(int index) const {
  return ((this->hasSpacing_ != 0) && (this->hasSpacingRange_[index] != 0) &&
          (this->rangeInfluence_[index])) ? 1 : 0;
}


int lefiLayer::hasSpacingRangeInfluenceRange(int index) const {
  return ((this->hasSpacing_ != 0) && (this->hasSpacingRange_[index] != 0) &&
          (this->rangeInfluenceRangeMin_[index] != -1) &&
          (this->rangeInfluenceRangeMax_[index] != -1)) ? 1 : 0;
}


int lefiLayer::hasSpacingRangeRange(int index) const {
  return ((this->hasSpacing_ != 0) && (this->hasSpacingRange_[index] != 0) &&
          (this->rangeRangeMin_[index] != -1) &&
          (this->rangeRangeMax_[index] != -1)) ? 1 : 0;
}


int lefiLayer::hasSpacingLengthThreshold(int index) const {
  return (this->hasSpacingLengthThreshold_[index]) ? 1 : 0;
}


int lefiLayer::hasSpacingLengthThresholdRange(int index) const{
  return ((this->hasSpacing_ != 0) &&
          (this->hasSpacingLengthThreshold_[index] != 0) &&
          (this->lengthThresholdRangeMin_[index] != -1) &&
          (this->lengthThresholdRangeMax_[index] != -1)) ? 1 : 0;
}


int lefiLayer::hasDirection() const {
  return this->hasDirection_;
}


int lefiLayer::hasResistance() const {
  return this->hasResistance_;
}


int lefiLayer::hasCapacitance() const {
  return this->hasCapacitance_;
}


int lefiLayer::hasHeight() const {
  return this->hasHeight_;
}


int lefiLayer::hasThickness() const {
  return this->hasThickness_;
}


int lefiLayer::hasShrinkage() const {
  return this->hasShrinkage_;
}


int lefiLayer::hasCapMultiplier() const {
  return this->hasCapMultiplier_;
}


int lefiLayer::hasEdgeCap() const {
  return this->hasEdgeCap_;
}


int lefiLayer::hasAntennaArea() const {
  return this->hasAntennaArea_;
}


int lefiLayer::hasAntennaLength() const {
  return this->hasAntennaLength_;
}


int lefiLayer::hasCurrentDensityPoint() const {
  return this->hasCurrentDensityPoint_;
}


int lefiLayer::hasCurrentDensityArray() const {
  return this->numCurrentPoints_ ? 1 : 0 ;
}


int lefiLayer::hasResistanceArray() const {
  return this->numResistancePoints_ ? 1 : 0 ;
}


int lefiLayer::hasCapacitanceArray() const {
  return this->numCapacitancePoints_ ? 1 : 0 ;
}


void lefiLayer::currentDensityArray(int* numPoints,
	   double** widths, double** current) const {
  *numPoints = this->numCurrentPoints_;
  *widths = this->currentWidths_;
  *current = this->current_;
}


void lefiLayer::resistanceArray(int* numPoints,
	   double** widths, double** res) const {
  *numPoints = this->numResistancePoints_;
  *widths = this->resistanceWidths_;
  *res = this->resistances_;
}


void lefiLayer::capacitanceArray(int* numPoints,
	   double** widths, double** cap) const {
  *numPoints = this->numCapacitancePoints_;
  *widths = this->capacitanceWidths_;
  *cap = this->capacitances_;
}


int lefiLayer::numSpacing() const {
  return this->numSpacings_;
}


char* lefiLayer::name() const {
  return this->name_;
}


const char* lefiLayer::type() const {
  return this->type_;
}


double lefiLayer::pitch() const {
  return this->pitch_;
}


double lefiLayer::offset() const {
  return this->offset_;
}


double lefiLayer::width() const {
  return this->width_;
}


double lefiLayer::area() const {
  return this->area_;
}


double lefiLayer::wireExtension() const {
  return this->wireExtension_;
}


double lefiLayer::spacing(int index) const {
  return this->spacing_[index];
}


char* lefiLayer::spacingName(int index) const {
  return this->spacingName_[index];
}


int lefiLayer::spacingAdjacentCuts(int index) const {
  return this->spacingAdjacentCuts_[index];
}


double lefiLayer::spacingAdjacentWithin(int index) const {
  return this->spacingAdjacentWithin_[index];
}


double lefiLayer::spacingRangeMin(int index) const {
  return this->rangeMin_[index];
}


double lefiLayer::spacingRangeMax(int index) const {
  return this->rangeMax_[index];
}


double lefiLayer::spacingRangeInfluence(int index) const {
  return this->rangeInfluence_[index];
}


double lefiLayer::spacingRangeInfluenceMin(int index) const {
  return this->rangeInfluenceRangeMin_[index];
}


double lefiLayer::spacingRangeInfluenceMax(int index) const {
  return this->rangeInfluenceRangeMax_[index];
}


double lefiLayer::spacingRangeRangeMin(int index) const {
  return this->rangeRangeMin_[index];
}


double lefiLayer::spacingRangeRangeMax(int index) const {
  return this->rangeRangeMax_[index];
}


double lefiLayer::spacingLengthThreshold(int index) const {
  return this->lengthThreshold_[index];
}


double lefiLayer::spacingLengthThresholdRangeMin(int index) const {
  return this->lengthThresholdRangeMin_[index];
}


double lefiLayer::spacingLengthThresholdRangeMax(int index) const {
  return this->lengthThresholdRangeMax_[index];
}


const char* lefiLayer::direction() const {
  return this->direction_;
}


double lefiLayer::currentDensityPoint() const {
  return this->currentDensity_;
}


double lefiLayer::resistance() const {
  return this->resistance_;
}


double lefiLayer::capacitance() const {
  return this->capacitance_;
}


double lefiLayer::height() const {
  return this->height_;
}


double lefiLayer::thickness() const {
  return this->thickness_;
}


double lefiLayer::shrinkage() const {
  return this->shrinkage_;
}


double lefiLayer::capMultiplier() const {
  return this->capMultiplier_;
}


double lefiLayer::edgeCap() const {
  return this->edgeCap_;
}


double lefiLayer::antennaLength() const {
  return this->antennaLength_;
}

double lefiLayer::antennaArea() const {
  return this->antennaArea_;
}

// 5.5
int lefiLayer::numMinimumcut() const {
  return this->numMinimumcut_;
}

// 5.5
int lefiLayer::minimumcut(int index) const {
  return this->minimumcut_[index];
}

// 5.5
double lefiLayer::minimumcutWidth(int index) const {
  return this->minimumcutWidth_[index];
}

// 5.5
int lefiLayer::hasMinimumcutConnection(int index) const {
  return this->hasMinimumcutConnection_[index];
}

// 5.5
const char* lefiLayer::minimumcutConnection(int index) const {
  return this->minimumcutConnection_[index];
}

// 5.5
int lefiLayer::hasMinimumcutNumCuts(int index) const {
  return this->hasMinimumcutNumCuts_[index];
}

// 5.5
double lefiLayer::minimumcutLength(int index) const {
  return this->minimumcutLength_[index];
}

// 5.5
double lefiLayer::minimumcutDistance(int index) const {
  return this->minimumcutDistance_[index];
}

// 5.5
int lefiLayer::hasMaxwidth() const {
  return this->maxwidth_ == -1 ? 0 : 1;
}

// 5.5
double lefiLayer::maxwidth() const {
  return this->maxwidth_ ;
}

// 5.5
int lefiLayer::hasMinwidth() const {
  return this->minwidth_ == -1 ? 0 : 1;
}

// 5.5
double lefiLayer::minwidth() const {
  return this->minwidth_ ;
}

// 5.5
int lefiLayer::numMinenclosedarea() const {
  return this->numMinenclosedarea_;
}

// 5.5
int lefiLayer::hasMinenclosedareaWidth(int index) const {
  return this->minenclosedareaWidth_[index] == -1 ? 0 : 1;
}

// 5.5
double lefiLayer::minenclosedarea(int index) const {
  return this->minenclosedarea_[index] ;
}

// 5.5
double lefiLayer::minenclosedareaWidth(int index) const {
  return this->minenclosedareaWidth_[index] ;
}

// 5.5
int lefiLayer::hasMinstep() const {
  return this->minstep_ == -1 ? 0 : 1;
}

// 5.5
double lefiLayer::minstep() const {
  return this->minstep_ ;
}

// 5.5
int lefiLayer::hasProtrusion() const {
  return this->protrusionWidth1_ == -1 ? 0 : 1;
}

// 5.5
double lefiLayer::protrusionWidth1() const {
  return this->protrusionWidth1_ ;
}

// 5.5
double lefiLayer::protrusionLength() const {
  return this->protrusionLength_ ;
}

// 5.5
double lefiLayer::protrusionWidth2() const {
  return this->protrusionWidth2_ ;
}

void lefiLayer::print(FILE* f) const {
  int i,max;
  double* j;
  double* k;
  fprintf(f, "Layer %s:\n", this->lefiLayer::name());
  if (this->lefiLayer::hasType())
     fprintf(f, "  type %s\n", this->lefiLayer::type());
  if (this->lefiLayer::hasPitch())
     fprintf(f, "  pitch %g\n", this->lefiLayer::pitch());
  if (this->lefiLayer::hasWireExtension())
     fprintf(f, "  wireextension %g\n", this->lefiLayer::wireExtension());
  if (this->lefiLayer::hasWidth())
     fprintf(f, "  width %g\n", this->lefiLayer::width());
  if (this->lefiLayer::hasArea())
     fprintf(f, "  area %g\n", this->lefiLayer::area());
  if (this->lefiLayer::hasSpacingNumber()) {
     for (i = 0; i < this->lefiLayer::numSpacing(); i++) {
        fprintf(f, "  spacing %g\n", this->lefiLayer::spacing(i));
        if (this->lefiLayer::hasSpacingRange(i)) {
           fprintf(f, "  range %g %g\n",
                   this->lefiLayer::spacingRangeMin(i),
                   this->lefiLayer::spacingRangeMax(i));
           if (this->lefiLayer::hasSpacingRangeUseLengthThreshold(i))
              fprintf(f, "    uselengththreshold\n");
           else if (this->lefiLayer::hasSpacingRangeInfluence(i)) {
              fprintf(f, "    influence %g\n",
                 this->lefiLayer::spacingRangeInfluence(i));
              if (this->lefiLayer::hasSpacingRangeInfluenceRange(i))
                 fprintf(f, "      Range %g %g\n",
                    this->lefiLayer::spacingRangeInfluenceMin(i),
                    this->lefiLayer::spacingRangeInfluenceMax(i));
           } else if (this->lefiLayer::hasSpacingRangeRange(i))
               fprintf(f, "    range %g %g\n",
                 this->lefiLayer::spacingRangeRangeMin(i),
                 this->lefiLayer::spacingRangeRangeMax(i));
        } else if (this->lefiLayer::hasSpacingLengthThreshold(i)) {
           fprintf(f, "  lengththreshold %g\n",
              this->lefiLayer::spacingLengthThreshold(i));
           if (this->lefiLayer::hasSpacingLengthThresholdRange(i))
              fprintf(f, "  range %g %g\n",
                 this->lefiLayer::spacingLengthThresholdRangeMin(i),
                 this->lefiLayer::spacingLengthThresholdRangeMax(i));
        }
     }
  }
  if (this->lefiLayer::hasDirection())
     fprintf(f, "  direction %s\n", this->lefiLayer::direction());

  if (this->lefiLayer::hasResistance())
     fprintf(f, "  resistance %g\n", this->lefiLayer::resistance());
  if (this->lefiLayer::hasResistanceArray()) {
     this->lefiLayer::resistanceArray(&max,&j,&k);
     fprintf(f, "  resistance PWL");
     for (i = 0; i < max; i++)
       fprintf(f, " %g %g", j[i], k[i]);
     fprintf(f, "\n");
    }
  if (this->lefiLayer::hasCapacitance())
     fprintf(f, "  capacitance %g\n", this->lefiLayer::capacitance());
  if (this->lefiLayer::hasCapacitanceArray()) {
     this->lefiLayer::capacitanceArray(&max,&j,&k);
     fprintf(f, "  capacitance PWL");
     for (i = 0; i < max; i++)
       fprintf(f, " %g %g", j[i], k[i]);
     fprintf(f, "\n");
    }

  if (this->lefiLayer::hasHeight())
     fprintf(f, "  height %g\n", this->lefiLayer::height());
  if (this->lefiLayer::hasThickness())
     fprintf(f, "  thickness %g\n", this->lefiLayer::thickness());
  if (this->lefiLayer::hasShrinkage())
     fprintf(f, "  shrinkage %g\n", this->lefiLayer::shrinkage());
  if (this->lefiLayer::hasCapMultiplier())
     fprintf(f, "  cap muptiplier %g\n", this->lefiLayer::capMultiplier());
  if (this->lefiLayer::hasEdgeCap())
     fprintf(f, "  edge cap %g\n", this->lefiLayer::edgeCap());

  if (this->lefiLayer::hasCurrentDensityPoint())
     fprintf(f, "  currentden %g\n", this->lefiLayer::currentDensityPoint());
  if (this->lefiLayer::hasCurrentDensityArray()) {
     this->lefiLayer::currentDensityArray(&max,&j,&k);
     fprintf(f, "  currentden PWL");
     for (i = 0; i < max; i++)
       fprintf(f, " %g %g", j[i], k[i]);
     fprintf(f, "\n");
    }
}


void lefiLayer::addProp(const char* name, const char* value, const char type) {
  int len = strlen(name) + 1;
//  char*  tvalue;
//  int    vlen, i;
  if (this->numProps_ == this->propsAllocated_) {
    int i;
    int max;
    int lim = this->numProps_;
    char**  nn;
    char**  nv;
    double* nd;
    char*   nt;

    if (this->propsAllocated_ == 0)
      max = this->propsAllocated_ = 2;
    else
      max = this->propsAllocated_ *= 2;
    nn = (char**)lefMalloc(sizeof(char*)*max);
    nv = (char**)lefMalloc(sizeof(char*)*max);
    nd = (double*)lefMalloc(sizeof(double)*max);
    nt = (char*)lefMalloc(sizeof(char)*max);
    for (i = 0; i < lim; i++) {
      nn[i] = this->names_[i];
      nv[i] = this->values_[i];
      nd[i] = this->dvalues_[i];
      nt[i] = this->types_[i];
    }
    lefFree((char*)(this->names_));
    lefFree((char*)(this->values_));
    lefFree((char*)(this->dvalues_));
    lefFree((char*)(this->types_));
    this->names_   = nn;
    this->values_  = nv;
    this->dvalues_ = nd;
    this->types_   = nt;
  }
  this->names_[this->numProps_] = (char*)lefMalloc(sizeof(char)*len);
  strcpy(this->names_[this->numProps_],name);
  len = strlen(value) + 1;
  this->values_[this->numProps_] = (char*)lefMalloc(sizeof(char)*len);
  strcpy(this->values_[this->numProps_],value);
  this->dvalues_[this->numProps_] = 0;
  //if (type == 'N') {
    // it is a number, don't know if it is an integer or real
    // Look for . in the value
    //tvalue = (char*)value;
    //vlen = strlen(value);
    //for (i = 0; i < vlen; i++) { 
      //if (*tvalue == '.') {
          //this->types_[this->numProps_] = 'R';
          //break;
      //}
      //++tvalue;
      //this->types_[this->numProps_] = type;
    //}
  //} else
    this->types_[this->numProps_] = type;
  this->numProps_ += 1;
}


void lefiLayer::addNumProp(const char* name, const double d,
                           const char* value, const char type) {
  int len = strlen(name) + 1;
  if (this->numProps_ == this->propsAllocated_) {
    int i;
    int max;
    int lim = this->numProps_;
    char**  nn;
    char**  nv;
    double* nd;
    char*   nt;

    max = this->propsAllocated_ *= 2;
    nn = (char**)lefMalloc(sizeof(char*)*max);
    nv = (char**)lefMalloc(sizeof(char*)*max);
    nd = (double*)lefMalloc(sizeof(double)*max);
    nt = (char*)lefMalloc(sizeof(char)*max);
    for (i = 0; i < lim; i++) {
      nn[i] = this->names_[i];
      nv[i] = this->values_[i];
      nd[i] = this->dvalues_[i];
      nt[i] = this->types_[i];
    }
    lefFree((char*)(this->names_));
    lefFree((char*)(this->values_));
    lefFree((char*)(this->dvalues_));
    lefFree((char*)(this->types_));
    this->names_   = nn;
    this->values_  = nv;
    this->dvalues_ = nd;
    this->types_   = nt;
  }
  this->names_[this->numProps_] = (char*)lefMalloc(sizeof(char)*len);
  strcpy(this->names_[this->numProps_],name);
  len = strlen(value) + 1;
  this->values_[this->numProps_] = (char*)lefMalloc(sizeof(char)*len);
  strcpy(this->values_[this->numProps_],value);
  this->dvalues_[this->numProps_] = d;
  this->types_[this->numProps_] = type;
  this->numProps_ += 1;
}

int lefiLayer::numProps() const {
  return this->numProps_;
}


const char* lefiLayer::propName(int i) const {
  if (i < 0 || i >= this->numProps_) {
    lefiError ("bad index for layer property");
    return 0;
  }
  return this->names_[i];
}


const char* lefiLayer::propValue(int i) const {
  if (i < 0 || i >= this->numProps_) {
    lefiError ("bad index for layer property");
    return 0;
  }
  return this->values_[i];
}


double lefiLayer::propNumber(int i) const {
  if (i < 0 || i >= this->numProps_) {
    lefiError ("bad index for layer property");
    return 0;
  }
  return this->dvalues_[i];
}


const char lefiLayer::propType(int i) const {
  if (i < 0 || i >= this->numProps_) {
    lefiError ("bad index for layer property");
    return 0;
  }
  return this->types_[i];
}


int lefiLayer::propIsNumber(int i) const {
  if (i < 0 || i >= this->numProps_) {
    lefiError ("bad index for layer property");
    return 0;
  }
  return this->dvalues_[i] ? 1 : 0;
}


int lefiLayer::propIsString(int i) const {
  if (i < 0 || i >= this->numProps_) {
    lefiError ("bad index for layer property");
    return 0;
  }
  return this->dvalues_[i] ? 0 : 1;
}


void lefiLayer::addAccurrentDensity(const char* type) {
  lefiLayerDensity* density;
  if (this->numAccurrents_ == this->accurrentAllocated_) {
    lefiLayerDensity** array;
    int i;
    this->accurrentAllocated_ = this->accurrentAllocated_ ?
                                this->accurrentAllocated_ * 2 : 2 ;
    array = (lefiLayerDensity**)lefMalloc(sizeof(lefiLayerDensity*)
                                *this->accurrentAllocated_);
    for (i = 0; i < this->numAccurrents_; i++)
       array[i] = this->accurrents_[i];
    if (this->accurrents_) lefFree((char*)(this->accurrents_));
    this->accurrents_ = array;
  }
  density = this->accurrents_[this->numAccurrents_] =
            (lefiLayerDensity*)lefMalloc(sizeof(lefiLayerDensity));
  this->numAccurrents_ += 1;
  density->lefiLayerDensity::Init(type);
}


void lefiLayer::setAcOneEntry(double num) {
  lefiLayerDensity* density;
  density = this->accurrents_[this->numAccurrents_-1];
  density->lefiLayerDensity::setOneEntry(num);
}


void lefiLayer::addAcFrequency() {
  lefiLayerDensity* density;
  density = this->accurrents_[this->numAccurrents_-1];
  density->lefiLayerDensity::addFrequency(this->numNums_, this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}


void lefiLayer::addAcCutarea() {
  lefiLayerDensity* density;
  density = this->accurrents_[this->numAccurrents_-1];
  density->lefiLayerDensity::addCutarea(this->numNums_, this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}


void lefiLayer::addAcTableEntry() {
  lefiLayerDensity* density;
  density = this->accurrents_[this->numAccurrents_-1];
  density->lefiLayerDensity::addTableEntry(this->numNums_, this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}


void lefiLayer::addAcWidth() {
  lefiLayerDensity* density;
  density = this->accurrents_[this->numAccurrents_-1];
  density->lefiLayerDensity::addWidth(this->numNums_, this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}


void lefiLayer::setDcOneEntry(double num) {
  lefiLayerDensity* density;
  density = this->dccurrents_[this->numDccurrents_-1];
  density->lefiLayerDensity::setOneEntry(num);
}


void lefiLayer::addDccurrentDensity(const char* type) {
  lefiLayerDensity* density;
  if (this->numDccurrents_ == this->dccurrentAllocated_) {
    lefiLayerDensity** array;
    int i;
    this->dccurrentAllocated_ = this->dccurrentAllocated_ ?
                                this->dccurrentAllocated_ * 2 : 2 ;
    array = (lefiLayerDensity**)lefMalloc(sizeof(lefiLayerDensity*)
                                *this->dccurrentAllocated_);
    for (i = 0; i < this->numDccurrents_; i++)
       array[i] = this->dccurrents_[i];
    if (this->dccurrents_) lefFree((char*)(this->dccurrents_));
    this->dccurrents_ = array;
  }
  density = this->dccurrents_[this->numDccurrents_] =
            (lefiLayerDensity*)lefMalloc(sizeof(lefiLayerDensity));
  this->numDccurrents_ += 1;
  density->lefiLayerDensity::Init(type);
}

void lefiLayer::addDcCutarea() {
  lefiLayerDensity* density;
  density = this->dccurrents_[this->numDccurrents_-1];
  density->lefiLayerDensity::addCutarea(this->numNums_, this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}


void lefiLayer::addDcTableEntry() {
  lefiLayerDensity* density;
  density = this->dccurrents_[this->numDccurrents_-1];
  density->lefiLayerDensity::addTableEntry(this->numNums_, this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}


void lefiLayer::addDcWidth() {
  lefiLayerDensity* density;
  density = this->dccurrents_[this->numDccurrents_-1];
  density->lefiLayerDensity::addWidth(this->numNums_, this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}


void lefiLayer::addNumber(double num) {
  if (this->numNums_ == this->numAllocated_) {
    double* array;
    int i;
    this->numAllocated_ = this->numAllocated_ ?
                          this->numAllocated_ * 2 : 2 ;
    array = (double*)lefMalloc(sizeof(double) *this->numAllocated_);
    for (i = 0; i < this->numNums_; i++)
       array[i] = this->nums_[i];
    if (this->nums_) lefFree((char*)(this->nums_));
    this->nums_ = array;
  }
  this->nums_[this->numNums_++] = num;
}


int lefiLayer::getNumber() {
  return this->numNums_ - 1;
}

int lefiLayer::hasAccurrentDensity() const {
  return this->numAccurrents_ ? 1 : 0 ;
}


int lefiLayer::hasDccurrentDensity() const {
  return this->numDccurrents_ ? 1 : 0 ;
}


int lefiLayer::numAccurrentDensity() const {
  return(this->numAccurrents_);
}


int lefiLayer::numDccurrentDensity() const {
  return(this->numDccurrents_);
}


lefiLayerDensity* lefiLayer::accurrent(int index) const {
  if (index >= this->numAccurrents_)
     return 0;
  return(this->accurrents_[index]);
}


lefiLayerDensity* lefiLayer::dccurrent(int index) const {
  if (index >= this->numDccurrents_)
     return 0;
  return(this->dccurrents_[index]);
}

// 5.5
void lefiLayer::addAntennaModel(int aOxide) {
  // For version 5.5 only OXIDE1, OXIDE2, OXIDE3 & OXDIE4
  // are defined within a layer
  lefiAntennaModel* amo;
  int i;

  if (this->numAntennaModel_ == 0) {   // does not have antennaModel
     this->antennaModel_ = (lefiAntennaModel**)
              lefMalloc(sizeof(lefiAntennaModel*)*4); 
     this->antennaModelAllocated_ = 4;
     for (i = 0; i < 4; i++) {
        this->antennaModel_[i] = (lefiAntennaModel*)
                                 lefMalloc(sizeof(lefiAntennaModel)); 
     }
     this->numAntennaModel_++;
     this->antennaModelAllocated_ = 4;
     amo = this->antennaModel_[0];
  } else {
     amo = this->antennaModel_[this->numAntennaModel_];
     this->numAntennaModel_++;
  }
  amo->lefiAntennaModel::Init();
  amo->lefiAntennaModel::setAntennaModel(aOxide);
  return;
}

// 5.5
int lefiLayer::numAntennaModel() const {
  return this->numAntennaModel_;
}

// 5.5
lefiAntennaModel* lefiLayer::antennaModel(int index) const {
  return this->antennaModel_[index];
}

// 3/23/2000 -- Wanda da Rosa.  The following are for 5.4 syntax
void lefiLayer::setAntennaAreaRatio(double value) {
  if (this->numAntennaModel_ == 0)   // haven't created any antannaModel yet
     this->lefiLayer::addAntennaModel(1);
  this->antennaModel_[this->numAntennaModel_-1]->lefiAntennaModel::setAntennaAreaRatio(value);
}

void lefiLayer::setAntennaCumAreaRatio(double value) {
  if (this->numAntennaModel_ == 0)   // haven't created any antannaModel yet
     this->lefiLayer::addAntennaModel(1);
  this->antennaModel_[this->numAntennaModel_-1]->lefiAntennaModel::setAntennaCumAreaRatio(value);
}

void lefiLayer::setAntennaAreaFactor(double value) {
  if (this->numAntennaModel_ == 0)   // haven't created any antannaModel yet
     this->lefiLayer::addAntennaModel(1);
  this->antennaModel_[this->numAntennaModel_-1]->lefiAntennaModel::setAntennaAreaFactor(value);
}

void lefiLayer::setAntennaSideAreaRatio(double value) {
  if (this->numAntennaModel_ == 0)   // haven't created any antannaModel yet
     this->lefiLayer::addAntennaModel(1);
  this->antennaModel_[this->numAntennaModel_-1]->lefiAntennaModel::setAntennaSideAreaRatio(value);
}

void lefiLayer::setAntennaCumSideAreaRatio(double value) {
  if (this->numAntennaModel_ == 0)   // haven't created any antannaModel yet
     this->lefiLayer::addAntennaModel(1);
  this->antennaModel_[this->numAntennaModel_-1]->lefiAntennaModel::setAntennaCumSideAreaRatio(value);
}

void lefiLayer::setAntennaSideAreaFactor(double value) {
  if (this->numAntennaModel_ == 0)   // haven't created any antannaModel yet
     this->lefiLayer::addAntennaModel(1);
  this->antennaModel_[this->numAntennaModel_-1]->lefiAntennaModel::setAntennaSideAreaFactor(value);
}

void lefiLayer::setAntennaValue(lefiAntennaEnum antennaType, double value) {
  if (this->numAntennaModel_ == 0)   // haven't created any antannaModel yet
     this->lefiLayer::addAntennaModel(1);
  this->antennaModel_[this->numAntennaModel_-1]->lefiAntennaModel::setAntennaValue(antennaType, value);
}

void lefiLayer::setAntennaDUO(lefiAntennaEnum antennaType) {
  if (this->numAntennaModel_ == 0)   // haven't created any antannaModel yet
     this->lefiLayer::addAntennaModel(1);
  this->antennaModel_[this->numAntennaModel_-1]->lefiAntennaModel::setAntennaDUO(antennaType);
}

void lefiLayer::setAntennaPWL(lefiAntennaEnum antennaType, lefiAntennaPWL* pwl) {
  if (this->numAntennaModel_ == 0)   // haven't created any antannaModel yet
     this->lefiLayer::addAntennaModel(1);
  this->antennaModel_[this->numAntennaModel_-1]->lefiAntennaModel::setAntennaPWL(antennaType, pwl);
}

// 8/29/2001 -- Wanda da Rosa.  The following are for 5.4 enhancements

void lefiLayer::setSlotWireWidth(double num) {
  this->hasSlotWireWidth_ = 1;
  this->slotWireWidth_ = num;
}

void lefiLayer::setSlotWireLength(double num) {
  this->hasSlotWireLength_ = 1;
  this->slotWireLength_ = num;
}

void lefiLayer::setSlotWidth(double num) {
  this->hasSlotWidth_ = 1;
  this->slotWidth_ = num;
}

void lefiLayer::setSlotLength(double num) {
  this->hasSlotLength_ = 1;
  this->slotLength_ = num;
}

void lefiLayer::setMaxAdjacentSlotSpacing(double num) {
  this->hasMaxAdjacentSlotSpacing_ = 1;
  this->maxAdjacentSlotSpacing_ = num;
}

void lefiLayer::setMaxCoaxialSlotSpacing(double num) {
  this->hasMaxCoaxialSlotSpacing_ = 1;
  this->maxCoaxialSlotSpacing_ = num;
}

void lefiLayer::setMaxEdgeSlotSpacing(double num) {
  this->hasMaxEdgeSlotSpacing_ = 1;
  this->maxEdgeSlotSpacing_ = num;
}

void lefiLayer::setSplitWireWidth(double num) {
  this->hasSplitWireWidth_ = 1;
  this->splitWireWidth_ = num;
}

void lefiLayer::setMinimumDensity(double num) {
  this->hasMinimumDensity_ = 1;
  this->minimumDensity_ = num;
}

void lefiLayer::setMaximumDensity(double num) {
  this->hasMaximumDensity_ = 1;
  this->maximumDensity_ = num;
}

void lefiLayer::setDensityCheckWindow(double length, double width) {
  this->hasDensityCheckWindow_ = 1;
  this->densityCheckWindowLength_ = length;
  this->densityCheckWindowWidth_ = width;
}

void lefiLayer::setDensityCheckStep(double num) {
  this->hasDensityCheckStep_ = 1;
  this->densityCheckStep_ = num;
}

void lefiLayer::setFillActiveSpacing(double num) {
  this->hasFillActiveSpacing_ = 1;
  this->fillActiveSpacing_ = num;
}

int lefiLayer::hasSlotWireWidth() const {
  return this->hasSlotWireWidth_;
}

int lefiLayer::hasSlotWireLength() const {
  return this->hasSlotWireLength_;
}

int lefiLayer::hasSlotWidth() const {
  return this->hasSlotWidth_;
}

int lefiLayer::hasSlotLength() const {
  return this->hasSlotLength_;
}

int lefiLayer::hasMaxAdjacentSlotSpacing() const {
  return this->hasMaxAdjacentSlotSpacing_;
}

int lefiLayer::hasMaxCoaxialSlotSpacing() const {
  return this->hasMaxCoaxialSlotSpacing_;
}

int lefiLayer::hasMaxEdgeSlotSpacing() const {
  return this->hasMaxEdgeSlotSpacing_;
}

int lefiLayer::hasSplitWireWidth() const {
  return this->hasSplitWireWidth_;
}

int lefiLayer::hasMinimumDensity() const {
  return this->hasMinimumDensity_;
}

int lefiLayer::hasMaximumDensity() const {
  return this->hasMaximumDensity_;
}

int lefiLayer::hasDensityCheckWindow() const {
  return this->hasDensityCheckWindow_;
}

int lefiLayer::hasDensityCheckStep() const {
  return this->hasDensityCheckStep_;
}

int lefiLayer::hasFillActiveSpacing() const {
  return this->hasFillActiveSpacing_;
}


double lefiLayer::slotWireWidth() const {
  return this->slotWireWidth_;
}

double lefiLayer::slotWireLength() const {
  return this->slotWireLength_;
}

double lefiLayer::slotWidth() const {
  return this->slotWidth_;
}

double lefiLayer::slotLength() const {
  return this->slotLength_;
}

double lefiLayer::maxAdjacentSlotSpacing() const {
  return this->maxAdjacentSlotSpacing_;
}

double lefiLayer::maxCoaxialSlotSpacing() const {
  return this->maxCoaxialSlotSpacing_;
}

double lefiLayer::maxEdgeSlotSpacing() const {
  return this->maxEdgeSlotSpacing_;
}

double lefiLayer::splitWireWidth() const {
  return this->splitWireWidth_;
}

double lefiLayer::minimumDensity() const {
  return this->minimumDensity_;
}

double lefiLayer::maximumDensity() const {
  return this->maximumDensity_;
}

double lefiLayer::densityCheckWindowLength() const {
  return this->densityCheckWindowLength_;
}

double lefiLayer::densityCheckWindowWidth() const {
  return this->densityCheckWindowWidth_;
}

double lefiLayer::densityCheckStep() const {
  return this->densityCheckStep_;
}

double lefiLayer::fillActiveSpacing() const {
  return this->fillActiveSpacing_;
}

// 5.5 SPACINGTABLE

void lefiLayer::addSpacingTable() {
  lefiSpacingTable* sp;
  if (this->numSpacingTable_ == this->spacingTableAllocated_) {
    lefiSpacingTable** array;
    int i;
    this->spacingTableAllocated_ = this->spacingTableAllocated_ ?
                                this->spacingTableAllocated_ * 2 : 2 ;
    array = (lefiSpacingTable**)lefMalloc(sizeof(lefiSpacingTable*)
                                *this->spacingTableAllocated_);
    for (i = 0; i < this->numSpacingTable_; i++)
       array[i] = this->spacingTable_[i];
    if (this->spacingTable_) lefFree((char*)(this->spacingTable_));
    this->spacingTable_ = array;
  }
  sp = this->spacingTable_[this->numSpacingTable_] =
            (lefiSpacingTable*)lefMalloc(sizeof(lefiSpacingTable));
  this->numSpacingTable_ += 1;
  sp->lefiSpacingTable::Init();
}

void lefiLayer::addSpParallelLength() {
  lefiSpacingTable* sp;
  sp = this->spacingTable_[this->numSpacingTable_-1];
  sp->lefiSpacingTable::addParallelLength(this->numNums_, this->nums_);
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}

void lefiLayer::addSpParallelWidth(double width) {
  lefiSpacingTable* sp;
  sp = this->spacingTable_[this->numSpacingTable_-1];
  sp->lefiSpacingTable::addParallelWidth(width);
}

void lefiLayer::addSpParallelWidthSpacing() {
  lefiSpacingTable* sp;
  sp = this->spacingTable_[this->numSpacingTable_-1];
  sp->lefiSpacingTable::addParallelWidthSpacing(this->numNums_, this->nums_);
  // Since inside addParallelWidthSpacing copy the nums_, we can free it
  // here
  lefFree((char*)(this->nums_)); 
  this->numNums_ = 0;
  this->numAllocated_ = 0;
  this->nums_ = 0;
}

void lefiLayer::setInfluence() {
  lefiSpacingTable* sp;
  sp = this->spacingTable_[this->numSpacingTable_-1];
  sp->lefiSpacingTable::setInfluence();
}

void lefiLayer::addSpInfluence(double width, double distance, double spacing) {
  lefiSpacingTable* sp;
  sp = this->spacingTable_[this->numSpacingTable_-1];
  sp->lefiSpacingTable::addInfluence(width, distance, spacing);
}

int lefiLayer::numSpacingTable() {
  return this->numSpacingTable_;
}

lefiSpacingTable* lefiLayer::spacingTable(int index) {
  return this->spacingTable_[index];
}
