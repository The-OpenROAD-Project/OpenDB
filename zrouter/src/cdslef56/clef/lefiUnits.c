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
#include "lefiUnits.h"
#include "lefiDebug.h"

/*
 * lefiUnits
 */ lefiUnits *
lefiUnits_CreateUninitialized()
{
  lefiUnits *this = (lefiUnits *) malloc(sizeof(lefiUnits));

  lefiUnits_Init(this);
  return (this);
}

void 
lefiUnits_Init(lefiUnits * this)
{
  lefiUnits_clear(this);
}

void 
lefiUnits_Destroy(lefiUnits * this)
{
  lefiUnits_clear(this);
}

void
lefiUnits_Delete(lefiUnits * this)
{
  if (this == NULL)
    return;
  lefiUnits_Destroy(this);
  free(this);
}

void 
lefiUnits_setDatabase(lefiUnits * this,
		      const char *name,
		      double num)
{
  int     len = strlen(name) + 1;

  this->databaseName_ = (char *) lefMalloc(len);
  strcpy(this->databaseName_, CASE(name));
  this->databaseNumber_ = num;
  this->hasDatabase_ = 1;
}

void 
lefiUnits_clear(lefiUnits * this)
{
  if (this->databaseName_)
    lefFree(this->databaseName_);
  this->hasTime_ = 0;
  this->hasCapacitance_ = 0;
  this->hasResistance_ = 0;
  this->hasPower_ = 0;
  this->hasCurrent_ = 0;
  this->hasVoltage_ = 0;
  this->hasDatabase_ = 0;
  this->hasFrequency_ = 0;
  this->databaseName_ = 0;
}

void 
lefiUnits_setTime(lefiUnits * this,
		  double num)
{
  this->hasTime_ = 1;
  this->time_ = num;
}

void 
lefiUnits_setCapacitance(lefiUnits * this,
			 double num)
{
  this->hasCapacitance_ = 1;
  this->capacitance_ = num;
}

void 
lefiUnits_setResistance(lefiUnits * this,
			double num)
{
  this->hasResistance_ = 1;
  this->resistance_ = num;
}

void 
lefiUnits_setPower(lefiUnits * this,
		   double num)
{
  this->hasPower_ = 1;
  this->power_ = num;
}

void 
lefiUnits_setCurrent(lefiUnits * this,
		     double num)
{
  this->hasCurrent_ = 1;
  this->current_ = num;
}

void 
lefiUnits_setVoltage(lefiUnits * this,
		     double num)
{
  this->hasVoltage_ = 1;
  this->voltage_ = num;
}

void 
lefiUnits_setFrequency(lefiUnits * this,
		       double num)
{
  this->hasFrequency_ = 1;
  this->frequency_ = num;
}

int 
lefiUnits_hasDatabase(lefiUnits * this)
{
  return this->hasDatabase_;
}

int 
lefiUnits_hasCapacitance(lefiUnits * this)
{
  return this->hasCapacitance_;
}

int 
lefiUnits_hasResistance(lefiUnits * this)
{
  return this->hasResistance_;
}

int 
lefiUnits_hasPower(lefiUnits * this)
{
  return this->hasPower_;
}

int 
lefiUnits_hasCurrent(lefiUnits * this)
{
  return this->hasCurrent_;
}

int 
lefiUnits_hasVoltage(lefiUnits * this)
{
  return this->hasVoltage_;
}

int 
lefiUnits_hasFrequency(lefiUnits * this)
{
  return this->hasFrequency_;
}

int 
lefiUnits_hasTime(lefiUnits * this)
{
  return this->hasTime_;
}

const char *
lefiUnits_databaseName(lefiUnits * this)
{
  return this->databaseName_;
}

double 
lefiUnits_databaseNumber(lefiUnits * this)
{
  return this->databaseNumber_;
}

double 
lefiUnits_capacitance(lefiUnits * this)
{
  return this->capacitance_;
}

double 
lefiUnits_resistance(lefiUnits * this)
{
  return this->resistance_;
}

double 
lefiUnits_power(lefiUnits * this)
{
  return this->power_;
}

double 
lefiUnits_current(lefiUnits * this)
{
  return this->current_;
}

double 
lefiUnits_time(lefiUnits * this)
{
  return this->time_;
}

double 
lefiUnits_voltage(lefiUnits * this)
{
  return this->voltage_;
}

double 
lefiUnits_frequency(lefiUnits * this)
{
  return this->frequency_;
}

void 
lefiUnits_print(lefiUnits * this,
		FILE * f)
{
  fprintf(f, "Units:\n");
  if (lefiUnits_hasTime(this))
    fprintf(f, " %g nanoseconds\n", lefiUnits_time(this));
  if (lefiUnits_hasCapacitance(this))
    fprintf(f, " %g picofarads\n", lefiUnits_capacitance(this));
  if (lefiUnits_hasResistance(this))
    fprintf(f, " %g ohms\n", lefiUnits_resistance(this));
  if (lefiUnits_hasPower(this))
    fprintf(f, " %g milliwatts\n", lefiUnits_power(this));
  if (lefiUnits_hasCurrent(this))
    fprintf(f, " %g milliamps\n", lefiUnits_current(this));
  if (lefiUnits_hasVoltage(this))
    fprintf(f, " %g volts\n", lefiUnits_voltage(this));
  if (lefiUnits_hasFrequency(this))
    fprintf(f, " %g frequency\n", lefiUnits_frequency(this));
  if (lefiUnits_hasDatabase(this))
    fprintf(f, " %s %g\n", lefiUnits_databaseName(this), lefiUnits_databaseNumber(this));
}
