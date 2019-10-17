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
#include "lefiCrossTalk.h"
#include "lefiDebug.h"

/*
 * lefiNoiseVictim
 */ lefiNoiseVictim *
lefiNoiseVictim_Create(double d)
{
  lefiNoiseVictim *this = (lefiNoiseVictim *) malloc(sizeof(lefiNoiseVictim));

  lefiNoiseVictim_Init(this,
		       d);
  return (this);
}

void 
lefiNoiseVictim_Init(lefiNoiseVictim * this,
		     double d)
{
  this->length_ = d;
  this->numNoises_ = 0;
  this->noisesAllocated_ = 2;
  this->noises_ = (double *) lefMalloc(sizeof(double) * 2);
}

void 
lefiNoiseVictim_clear(lefiNoiseVictim * this)
{
  this->numNoises_ = 0;
}

void 
lefiNoiseVictim_Destroy(lefiNoiseVictim * this)
{
  lefiNoiseVictim_clear(this);
  lefFree((char *) (this->noises_));
}

void
lefiNoiseVictim_Delete(lefiNoiseVictim * this)
{
  if (this == NULL)
    return;
  lefiNoiseVictim_Destroy(this);
  free(this);
}

void 
lefiNoiseVictim_addVictimNoise(lefiNoiseVictim * this,
			       double d)
{
  if (this->numNoises_ == this->noisesAllocated_) {
    int     max;

    double *ne;

    int     i;

    if (this->noisesAllocated_ == 0) {
      max = this->noisesAllocated_ = 2;
      this->numNoises_ = 0;
    }
    else
      max = this->noisesAllocated_ = this->numNoises_ * 2;
    ne = (double *) lefMalloc(sizeof(double) * max);
    max /= 2;
    for (i = 0;
	 i < max;
	 i++)
      ne[i] = this->noises_[i];
    lefFree((char *) (this->noises_));
    this->noises_ = ne;
  }
  this->noises_[this->numNoises_] = d;
  this->numNoises_ += 1;
}

int 
lefiNoiseVictim_numNoises(lefiNoiseVictim * this)
{
  return this->numNoises_;
}

double 
lefiNoiseVictim_noise(lefiNoiseVictim * this,
		      int index)
{
  return this->noises_[index];
}

double 
lefiNoiseVictim_length(lefiNoiseVictim * this)
{
  return this->length_;
}

/*
 * lefiNoiseResistance
 */ lefiNoiseResistance *
lefiNoiseResistance_CreateUninitialized()
{
  lefiNoiseResistance *this = (lefiNoiseResistance *) malloc(sizeof(lefiNoiseResistance));

  lefiNoiseResistance_Init(this);
  return (this);
}

void 
lefiNoiseResistance_Init(lefiNoiseResistance * this)
{
  this->numNums_ = 0;
  this->numsAllocated_ = 1;
  this->nums_ = (double *) lefMalloc(sizeof(double) * 1);
  this->numVictims_ = 0;
  this->victimsAllocated_ = 2;
  this->victims_ = (lefiNoiseVictim **) lefMalloc(sizeof(lefiNoiseVictim *) * 2);
}

void 
lefiNoiseResistance_clear(lefiNoiseResistance * this)
{
  int     i;

  lefiNoiseVictim *r;

  int     max = this->numVictims_;

  for (i = 0;
       i < max;
       i++) {
    r = this->victims_[i];
    lefiNoiseVictim_Destroy(r);
    lefFree((char *) r);
  }
  this->numVictims_ = 0;
  this->numNums_ = 0;
}

void 
lefiNoiseResistance_Destroy(lefiNoiseResistance * this)
{
  lefiNoiseResistance_clear(this);
  lefFree((char *) (this->nums_));
  lefFree((char *) (this->victims_));
}

void
lefiNoiseResistance_Delete(lefiNoiseResistance * this)
{
  if (this == NULL)
    return;
  lefiNoiseResistance_Destroy(this);
  free(this);
}

void 
lefiNoiseResistance_addResistanceNumber(lefiNoiseResistance * this,
					double d)
{
  if (this->numNums_ == this->numsAllocated_) {
    int     max;

    double *ne;

    int     i;

    if (this->numsAllocated_ == 0) {
      max = this->numsAllocated_ = 2;
      this->numNums_ = 0;
    }
    else
      max = this->numsAllocated_ = this->numNums_ * 2;
    ne = (double *) lefMalloc(sizeof(double) * max);
    max /= 2;
    for (i = 0;
	 i < max;
	 i++)
      ne[i] = this->nums_[i];
    lefFree((char *) (this->nums_));
    this->nums_ = ne;
  }
  this->nums_[this->numNums_] = d;
  this->numNums_ += 1;
}

void 
lefiNoiseResistance_addVictimNoise(lefiNoiseResistance * this,
				   double d)
{
  lefiNoiseVictim *r = this->victims_[this->numVictims_ - 1];

  lefiNoiseVictim_addVictimNoise(r,
				 d);
}

void 
lefiNoiseResistance_addVictimLength(lefiNoiseResistance * this,
				    double d)
{
  lefiNoiseVictim *r;

  if (this->numVictims_ == this->victimsAllocated_) {
    int     max;

    lefiNoiseVictim **ne;

    int     i;

    if (this->victimsAllocated_ == 0) {
      max = this->victimsAllocated_ = 2;
      this->numVictims_ = 0;
    }
    else
      max = this->victimsAllocated_ = this->numVictims_ * 2;
    ne = (lefiNoiseVictim **) lefMalloc(sizeof(lefiNoiseVictim *) * max);
    max /= 2;
    for (i = 0;
	 i < max;
	 i++)
      ne[i] = this->victims_[i];
    lefFree((char *) (this->victims_));
    this->victims_ = ne;
  }
  r = (lefiNoiseVictim *) lefMalloc(sizeof(lefiNoiseVictim));
  lefiNoiseVictim_Init(r,
		       d);
  this->victims_[this->numVictims_] = r;
  this->numVictims_ += 1;
}

int 
lefiNoiseResistance_numVictims(lefiNoiseResistance * this)
{
  return this->numVictims_;
}

lefiNoiseVictim *
lefiNoiseResistance_victim(lefiNoiseResistance * this,
			   int index)
{
  return this->victims_[index];
}

int 
lefiNoiseResistance_numNums(lefiNoiseResistance * this)
{
  return this->numNums_;
}

double 
lefiNoiseResistance_num(lefiNoiseResistance * this,
			int index)
{
  return this->nums_[index];
}

/*
 * lefiNoiseEdge
 */ lefiNoiseEdge *
lefiNoiseEdge_CreateUninitialized()
{
  lefiNoiseEdge *this = (lefiNoiseEdge *) malloc(sizeof(lefiNoiseEdge));

  lefiNoiseEdge_Init(this);
  return (this);
}

void 
lefiNoiseEdge_Init(lefiNoiseEdge * this)
{
  this->edge_ = 0;
  this->numResistances_ = 0;
  this->resistancesAllocated_ = 2;
  this->resistances_ = (lefiNoiseResistance **) lefMalloc(sizeof(lefiNoiseResistance *) * 2);
}

void 
lefiNoiseEdge_clear(lefiNoiseEdge * this)
{
  int     i;

  lefiNoiseResistance *r;

  int     maxr = this->numResistances_;

  for (i = 0;
       i < maxr;
       i++) {
    r = this->resistances_[i];
    lefiNoiseResistance_Destroy(r);
    lefFree((char *) r);
  }
  this->edge_ = 0;
  this->numResistances_ = 0;
}

void 
lefiNoiseEdge_Destroy(lefiNoiseEdge * this)
{
  lefiNoiseEdge_clear(this);
  lefFree((char *) (this->resistances_));
}

void
lefiNoiseEdge_Delete(lefiNoiseEdge * this)
{
  if (this == NULL)
    return;
  lefiNoiseEdge_Destroy(this);
  free(this);
}

void 
lefiNoiseEdge_addEdge(lefiNoiseEdge * this,
		      double d)
{
  this->edge_ = d;
}

void 
lefiNoiseEdge_addResistanceNumber(lefiNoiseEdge * this,
				  double d)
{
  lefiNoiseResistance *r = this->resistances_[this->numResistances_ - 1];

  lefiNoiseResistance_addResistanceNumber(r,
					  d);
}

void 
lefiNoiseEdge_addResistance(lefiNoiseEdge * this)
{
  lefiNoiseResistance *r;

  if (this->numResistances_ == this->resistancesAllocated_) {
    int     max;

    lefiNoiseResistance **ne;

    int     i;

    if (this->resistancesAllocated_ == 0) {
      max = this->resistancesAllocated_ = 2;
      this->numResistances_ = 0;
    }
    else
      max = this->resistancesAllocated_ = this->numResistances_ * 2;
    ne = (lefiNoiseResistance **) lefMalloc(sizeof(lefiNoiseResistance *) * max);
    max /= 2;
    for (i = 0;
	 i < max;
	 i++)
      ne[i] = this->resistances_[i];
    lefFree((char *) (this->resistances_));
    this->resistances_ = ne;
  }
  r = (lefiNoiseResistance *) lefMalloc(sizeof(lefiNoiseResistance));
  lefiNoiseResistance_Init(r);
  this->resistances_[this->numResistances_] = r;
  this->numResistances_ += 1;
}

void 
lefiNoiseEdge_addVictimNoise(lefiNoiseEdge * this,
			     double d)
{
  lefiNoiseResistance *r = this->resistances_[this->numResistances_ - 1];

  lefiNoiseResistance_addVictimNoise(r,
				     d);
}

void 
lefiNoiseEdge_addVictimLength(lefiNoiseEdge * this,
			      double d)
{
  lefiNoiseResistance *r = this->resistances_[this->numResistances_ - 1];

  lefiNoiseResistance_addVictimLength(r,
				      d);
}

int 
lefiNoiseEdge_numResistances(lefiNoiseEdge * this)
{
  return this->numResistances_;
}

lefiNoiseResistance *
lefiNoiseEdge_resistance(lefiNoiseEdge * this,
			 int index)
{
  return this->resistances_[index];
}

double 
lefiNoiseEdge_edge(lefiNoiseEdge * this)
{
  return this->edge_;
}

/*
 * lefiNoiseTable
 */ lefiNoiseTable *
lefiNoiseTable_CreateUninitialized()
{
  lefiNoiseTable *this = (lefiNoiseTable *) malloc(sizeof(lefiNoiseTable));

  lefiNoiseTable_Init(this);
  return (this);
}

void 
lefiNoiseTable_Init(lefiNoiseTable * this)
{
  this->numEdges_ = 0;
  this->edgesAllocated_ = 2;
  this->edges_ = (lefiNoiseEdge **) lefMalloc(sizeof(lefiNoiseEdge *) * 2);
}

void 
lefiNoiseTable_clear(lefiNoiseTable * this)
{
  int     i;

  lefiNoiseEdge *r;

  int     max = this->numEdges_;

  for (i = 0;
       i < max;
       i++) {
    r = this->edges_[i];
    lefiNoiseEdge_Destroy(r);
    lefFree((char *) r);
  }
  this->numEdges_ = 0;
}

void 
lefiNoiseTable_Destroy(lefiNoiseTable * this)
{
  lefiNoiseTable_clear(this);
  lefFree((char *) (this->edges_));
}

void
lefiNoiseTable_Delete(lefiNoiseTable * this)
{
  if (this == NULL)
    return;
  lefiNoiseTable_Destroy(this);
  free(this);
}

void 
lefiNoiseTable_setup(lefiNoiseTable * this,
		     int i)
{
  this->num_ = i;
  lefiNoiseTable_clear(this);
}

void 
lefiNoiseTable_newEdge(lefiNoiseTable * this)
{
  lefiNoiseEdge *r;

  if (this->numEdges_ == this->edgesAllocated_) {
    int     max;

    lefiNoiseEdge **ne;

    int     i;

    if (this->edgesAllocated_ == 0) {
      max = this->edgesAllocated_ = 2;
      this->numEdges_ = 0;
    }
    else
      max = this->edgesAllocated_ = this->numEdges_ * 2;
    ne = (lefiNoiseEdge **) lefMalloc(sizeof(lefiNoiseEdge *) * max);
    max /= 2;
    for (i = 0;
	 i < max;
	 i++)
      ne[i] = this->edges_[i];
    lefFree((char *) (this->edges_));
    this->edges_ = ne;
  }
  r = (lefiNoiseEdge *) lefMalloc(sizeof(lefiNoiseEdge));
  lefiNoiseEdge_Init(r);
  this->edges_[this->numEdges_] = r;
  this->numEdges_ += 1;
}

void 
lefiNoiseTable_addEdge(lefiNoiseTable * this,
		       double d)
{
  lefiNoiseEdge *r = this->edges_[this->numEdges_ - 1];

  lefiNoiseEdge_addEdge(r,
			d);
}

void 
lefiNoiseTable_addResistance(lefiNoiseTable * this)
{
  lefiNoiseEdge *r = this->edges_[this->numEdges_ - 1];

  lefiNoiseEdge_addResistance(r);
}

void 
lefiNoiseTable_addResistanceNumber(lefiNoiseTable * this,
				   double d)
{
  lefiNoiseEdge *r = this->edges_[this->numEdges_ - 1];

  lefiNoiseEdge_addResistanceNumber(r,
				    d);
}

void 
lefiNoiseTable_addVictimLength(lefiNoiseTable * this,
			       double d)
{
  lefiNoiseEdge *r = this->edges_[this->numEdges_ - 1];

  lefiNoiseEdge_addVictimLength(r,
				d);
}

void 
lefiNoiseTable_addVictimNoise(lefiNoiseTable * this,
			      double d)
{
  lefiNoiseEdge *r = this->edges_[this->numEdges_ - 1];

  lefiNoiseEdge_addVictimNoise(r,
			       d);
}

int 
lefiNoiseTable_num(lefiNoiseTable * this)
{
  return this->num_;
}

int 
lefiNoiseTable_numEdges(lefiNoiseTable * this)
{
  return this->numEdges_;
}

lefiNoiseEdge *
lefiNoiseTable_edge(lefiNoiseTable * this,
		    int index)
{
  return this->edges_[index];
}

/*
 * lefiCorrectionVictim
 */ lefiCorrectionVictim *
lefiCorrectionVictim_Create(double d)
{
  lefiCorrectionVictim *this = (lefiCorrectionVictim *) malloc(sizeof(lefiCorrectionVictim));

  lefiCorrectionVictim_Init(this,
			    d);
  return (this);
}

void 
lefiCorrectionVictim_Init(lefiCorrectionVictim * this,
			  double d)
{
  this->length_ = d;
  this->numCorrections_ = 0;
  this->correctionsAllocated_ = 2;
  this->corrections_ = (double *) lefMalloc(sizeof(double) * 2);
}

void 
lefiCorrectionVictim_clear(lefiCorrectionVictim * this)
{
  this->numCorrections_ = 0;
}

void 
lefiCorrectionVictim_Destroy(lefiCorrectionVictim * this)
{
  lefiCorrectionVictim_clear(this);
  lefFree((char *) (this->corrections_));
}

void
lefiCorrectionVictim_Delete(lefiCorrectionVictim * this)
{
  if (this == NULL)
    return;
  lefiCorrectionVictim_Destroy(this);
  free(this);
}

void 
lefiCorrectionVictim_addVictimCorrection(lefiCorrectionVictim * this,
					 double d)
{
  if (this->numCorrections_ == this->correctionsAllocated_) {
    int     max;

    double *ne;

    int     i;

    if (this->correctionsAllocated_ == 0) {
      max = this->correctionsAllocated_ = 2;
      this->numCorrections_ = 0;
    }
    else
      max = this->correctionsAllocated_ = this->numCorrections_ * 2;
    ne = (double *) lefMalloc(sizeof(double) * max);
    max /= 2;
    for (i = 0;
	 i < max;
	 i++)
      ne[i] = this->corrections_[i];
    lefFree((char *) (this->corrections_));
    this->corrections_ = ne;
  }
  this->corrections_[this->numCorrections_] = d;
  this->numCorrections_ += 1;
}

int 
lefiCorrectionVictim_numCorrections(lefiCorrectionVictim * this)
{
  return this->numCorrections_;
}

double 
lefiCorrectionVictim_correction(lefiCorrectionVictim * this,
				int index)
{
  return this->corrections_[index];
}

double 
lefiCorrectionVictim_length(lefiCorrectionVictim * this)
{
  return this->length_;
}

/*
 * lefiCorrectionResistance
 */ lefiCorrectionResistance *
lefiCorrectionResistance_CreateUninitialized()
{
  lefiCorrectionResistance *this = (lefiCorrectionResistance *) malloc(sizeof(lefiCorrectionResistance));

  lefiCorrectionResistance_Init(this);
  return (this);
}

void 
lefiCorrectionResistance_Init(lefiCorrectionResistance * this)
{
  this->numNums_ = 0;
  this->numsAllocated_ = 1;
  this->nums_ = (double *) lefMalloc(sizeof(double) * 1);
  this->numVictims_ = 0;
  this->victimsAllocated_ = 2;
  this->victims_ = (lefiCorrectionVictim **) lefMalloc(sizeof(lefiCorrectionVictim *) * 2);
}

void 
lefiCorrectionResistance_clear(lefiCorrectionResistance * this)
{
  int     i;

  lefiCorrectionVictim *r;

  int     max = this->numVictims_;

  for (i = 0;
       i < max;
       i++) {
    r = this->victims_[i];
    lefiCorrectionVictim_Destroy(r);
    lefFree((char *) r);
  }
  this->numVictims_ = 0;
  this->numNums_ = 0;
}

void 
lefiCorrectionResistance_Destroy(lefiCorrectionResistance * this)
{
  lefiCorrectionResistance_clear(this);
  lefFree((char *) (this->nums_));
  lefFree((char *) (this->victims_));
}

void
lefiCorrectionResistance_Delete(lefiCorrectionResistance * this)
{
  if (this == NULL)
    return;
  lefiCorrectionResistance_Destroy(this);
  free(this);
}

void 
lefiCorrectionResistance_addResistanceNumber(lefiCorrectionResistance * this,
					     double d)
{
  if (this->numNums_ == this->numsAllocated_) {
    int     max;

    double *ne;

    int     i;

    if (this->numsAllocated_) {
      max = this->numsAllocated_ = 2;
      this->numNums_ = 0;
    }
    else
      max = this->numsAllocated_ = this->numNums_ * 2;
    ne = (double *) lefMalloc(sizeof(double) * max);
    max /= 2;
    for (i = 0;
	 i < max;
	 i++)
      ne[i] = this->nums_[i];
    lefFree((char *) (this->nums_));
    this->nums_ = ne;
  }
  this->nums_[this->numNums_] = d;
  this->numNums_ += 1;
}

void 
lefiCorrectionResistance_addVictimCorrection(lefiCorrectionResistance * this,
					     double d)
{
  lefiCorrectionVictim *r = this->victims_[this->numVictims_ - 1];

  lefiCorrectionVictim_addVictimCorrection(r,
					   d);
}

void 
lefiCorrectionResistance_addVictimLength(lefiCorrectionResistance * this,
					 double d)
{
  lefiCorrectionVictim *r;

  if (this->numVictims_ == this->victimsAllocated_) {
    int     max;

    lefiCorrectionVictim **ne;

    int     i;

    if (this->victimsAllocated_ == 0) {
      max = this->victimsAllocated_ = 2;
      this->numVictims_ = 0;
    }
    else
      max = this->victimsAllocated_ = this->numVictims_ * 2;
    ne = (lefiCorrectionVictim **) lefMalloc(sizeof(lefiCorrectionVictim *) * max);
    max /= 2;
    for (i = 0;
	 i < max;
	 i++)
      ne[i] = this->victims_[i];
    lefFree((char *) (this->victims_));
    this->victims_ = ne;
  }
  r = (lefiCorrectionVictim *) lefMalloc(sizeof(lefiCorrectionVictim));
  lefiCorrectionVictim_Init(r,
			    d);
  this->victims_[this->numVictims_] = r;
  this->numVictims_ += 1;
}

int 
lefiCorrectionResistance_numVictims(lefiCorrectionResistance * this)
{
  return this->numVictims_;
}

lefiCorrectionVictim *
lefiCorrectionResistance_victim(lefiCorrectionResistance * this,
				int index)
{
  return this->victims_[index];
}

int 
lefiCorrectionResistance_numNums(lefiCorrectionResistance * this)
{
  return this->numNums_;
}

double 
lefiCorrectionResistance_num(lefiCorrectionResistance * this,
			     int index)
{
  return this->nums_[index];
}

/*
 * lefiCorrectionEdge
 */ lefiCorrectionEdge *
lefiCorrectionEdge_CreateUninitialized()
{
  lefiCorrectionEdge *this = (lefiCorrectionEdge *) malloc(sizeof(lefiCorrectionEdge));

  lefiCorrectionEdge_Init(this);
  return (this);
}

void 
lefiCorrectionEdge_Init(lefiCorrectionEdge * this)
{
  this->edge_ = 0;
  this->numResistances_ = 0;
  this->resistancesAllocated_ = 2;
  this->resistances_ = (lefiCorrectionResistance **) lefMalloc(sizeof(lefiCorrectionResistance *) * 2);
}

void 
lefiCorrectionEdge_clear(lefiCorrectionEdge * this)
{
  int     i;

  lefiCorrectionResistance *r;

  int     maxr = this->numResistances_;

  for (i = 0;
       i < maxr;
       i++) {
    r = this->resistances_[i];
    lefiCorrectionResistance_Destroy(r);
    lefFree((char *) r);
  }
  this->edge_ = 0;
  this->numResistances_ = 0;
}

void 
lefiCorrectionEdge_Destroy(lefiCorrectionEdge * this)
{
  lefiCorrectionEdge_clear(this);
  lefFree((char *) (this->resistances_));
}

void
lefiCorrectionEdge_Delete(lefiCorrectionEdge * this)
{
  if (this == NULL)
    return;
  lefiCorrectionEdge_Destroy(this);
  free(this);
}

void 
lefiCorrectionEdge_addEdge(lefiCorrectionEdge * this,
			   double d)
{
  this->edge_ = d;
}

void 
lefiCorrectionEdge_addResistanceNumber(lefiCorrectionEdge * this,
				       double d)
{
  lefiCorrectionResistance *r = this->resistances_[this->numResistances_ - 1];

  lefiCorrectionResistance_addResistanceNumber(r,
					       d);
}

void 
lefiCorrectionEdge_addResistance(lefiCorrectionEdge * this)
{
  lefiCorrectionResistance *r;

  if (this->numResistances_ == this->resistancesAllocated_) {
    int     max;

    lefiCorrectionResistance **ne;

    int     i;

    if (this->resistancesAllocated_ == 0) {
      max = this->resistancesAllocated_ = 2;
      this->numResistances_ = 0;
    }
    else
      max = this->resistancesAllocated_ = this->numResistances_ * 2;
    ne = (lefiCorrectionResistance **) lefMalloc(sizeof(lefiCorrectionResistance *) * max);
    max /= 2;
    for (i = 0;
	 i < max;
	 i++)
      ne[i] = this->resistances_[i];
    lefFree((char *) (this->resistances_));
    this->resistances_ = ne;
  }
  r = (lefiCorrectionResistance *) lefMalloc(sizeof(lefiCorrectionResistance));
  lefiCorrectionResistance_Init(r);
  this->resistances_[this->numResistances_] = r;
  this->numResistances_ += 1;
}

void 
lefiCorrectionEdge_addVictimCorrection(lefiCorrectionEdge * this,
				       double d)
{
  lefiCorrectionResistance *r = this->resistances_[this->numResistances_ - 1];

  lefiCorrectionResistance_addVictimCorrection(r,
					       d);
}

void 
lefiCorrectionEdge_addVictimLength(lefiCorrectionEdge * this,
				   double d)
{
  lefiCorrectionResistance *r = this->resistances_[this->numResistances_ - 1];

  lefiCorrectionResistance_addVictimLength(r,
					   d);
}

int 
lefiCorrectionEdge_numResistances(lefiCorrectionEdge * this)
{
  return this->numResistances_;
}

lefiCorrectionResistance *
lefiCorrectionEdge_resistance(lefiCorrectionEdge * this,
			      int index)
{
  return this->resistances_[index];
}

double 
lefiCorrectionEdge_edge(lefiCorrectionEdge * this)
{
  return this->edge_;
}

/*
 * lefiCorrectionTable
 */ lefiCorrectionTable *
lefiCorrectionTable_CreateUninitialized()
{
  lefiCorrectionTable *this = (lefiCorrectionTable *) malloc(sizeof(lefiCorrectionTable));

  lefiCorrectionTable_Init(this);
  return (this);
}

void 
lefiCorrectionTable_Init(lefiCorrectionTable * this)
{
  this->numEdges_ = 0;
  this->edgesAllocated_ = 2;
  this->edges_ = (lefiCorrectionEdge **) lefMalloc(sizeof(lefiCorrectionEdge *) * 2);
}

void 
lefiCorrectionTable_clear(lefiCorrectionTable * this)
{
  int     i;

  lefiCorrectionEdge *r;

  int     max = this->numEdges_;

  for (i = 0;
       i < max;
       i++) {
    r = this->edges_[i];
    lefiCorrectionEdge_Destroy(r);
    lefFree((char *) r);
  }
  this->numEdges_ = 0;
}

void 
lefiCorrectionTable_Destroy(lefiCorrectionTable * this)
{
  lefiCorrectionTable_clear(this);
  lefFree((char *) (this->edges_));
}

void
lefiCorrectionTable_Delete(lefiCorrectionTable * this)
{
  if (this == NULL)
    return;
  lefiCorrectionTable_Destroy(this);
  free(this);
}

void 
lefiCorrectionTable_setup(lefiCorrectionTable * this,
			  int i)
{
  this->num_ = i;
  lefiCorrectionTable_clear(this);
}

void 
lefiCorrectionTable_newEdge(lefiCorrectionTable * this)
{
  lefiCorrectionEdge *r;

  if (this->numEdges_ == this->edgesAllocated_) {
    int     max;

    lefiCorrectionEdge **ne;

    int     i;

    if (this->edgesAllocated_ == 0) {
      max = this->edgesAllocated_ = 2;
      this->numEdges_ = 0;
    }
    else
      max = this->edgesAllocated_ = this->numEdges_ * 2;
    ne = (lefiCorrectionEdge **) lefMalloc(sizeof(lefiCorrectionEdge *) * max);
    max /= 2;
    for (i = 0;
	 i < max;
	 i++)
      ne[i] = this->edges_[i];
    lefFree((char *) (this->edges_));
    this->edges_ = ne;
  }
  r = (lefiCorrectionEdge *) lefMalloc(sizeof(lefiCorrectionEdge));
  lefiCorrectionEdge_Init(r);
  this->edges_[this->numEdges_] = r;
  this->numEdges_ += 1;
}

void 
lefiCorrectionTable_addEdge(lefiCorrectionTable * this,
			    double d)
{
  lefiCorrectionEdge *r = this->edges_[this->numEdges_ - 1];

  lefiCorrectionEdge_addEdge(r,
			     d);
}

void 
lefiCorrectionTable_addResistanceNumber(lefiCorrectionTable * this,
					double d)
{
  lefiCorrectionEdge *r = this->edges_[this->numEdges_ - 1];

  lefiCorrectionEdge_addResistanceNumber(r,
					 d);
}

void 
lefiCorrectionTable_addResistance(lefiCorrectionTable * this)
{
  lefiCorrectionEdge *r = this->edges_[this->numEdges_ - 1];

  lefiCorrectionEdge_addResistance(r);
}

void 
lefiCorrectionTable_addVictimLength(lefiCorrectionTable * this,
				    double d)
{
  lefiCorrectionEdge *r = this->edges_[this->numEdges_ - 1];

  lefiCorrectionEdge_addVictimLength(r,
				     d);
}

void 
lefiCorrectionTable_addVictimCorrection(lefiCorrectionTable * this,
					double d)
{
  lefiCorrectionEdge *r = this->edges_[this->numEdges_ - 1];

  lefiCorrectionEdge_addVictimCorrection(r,
					 d);
}

int 
lefiCorrectionTable_num(lefiCorrectionTable * this)
{
  return this->num_;
}

int 
lefiCorrectionTable_numEdges(lefiCorrectionTable * this)
{
  return this->numEdges_;
}

lefiCorrectionEdge *
lefiCorrectionTable_edge(lefiCorrectionTable * this,
			 int index)
{
  return this->edges_[index];
}
