/*
 *     This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
 *  Distribution,  Product Version 5.6, and is subject to the Cadence
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

#ifndef lefiUnits_h
#define lefiUnits_h

#include <stdio.h>
#include "lefiKRDefs.hpp"

class lefiUnits {
public:
  lefiUnits();
  void Init();

  void Destroy();
  ~lefiUnits();

  void setDatabase(const char* name, double num);
  void clear();
  void setTime(double num);
  void setCapacitance(double num);
  void setResistance(double num);
  void setPower(double num);
  void setCurrent(double num);
  void setVoltage(double num);
  void setFrequency(double num);

  int hasDatabase();
  int hasCapacitance();
  int hasResistance();
  int hasTime();
  int hasPower();
  int hasCurrent();
  int hasVoltage();
  int hasFrequency();

  const char* databaseName();
  double databaseNumber();
  double capacitance();
  double resistance();
  double time();
  double power();
  double current();
  double voltage();
  double frequency();

  // Debug print
  void print(FILE* f);

protected:
  int hasDatabase_;
  int hasCapacitance_;
  int hasResistance_;
  int hasTime_;
  int hasPower_;
  int hasCurrent_;
  int hasVoltage_;
  int hasFrequency_;
  char* databaseName_;
  double databaseNumber_;
  double capacitance_;
  double resistance_;
  double power_;
  double time_;
  double current_;
  double voltage_;
  double frequency_;
};

#endif
