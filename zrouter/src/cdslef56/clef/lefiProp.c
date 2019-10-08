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
#include "lefiProp.h"
#include "lefiDebug.h"
lefiProp *
lefiProp_CreateUninitialized()
{
  lefiProp *this = (lefiProp *) malloc(sizeof(lefiProp));

  lefiProp_Init(this);
  return (this);
}

void 
lefiProp_Init(lefiProp * this)
{
  this->stringLength_ = 16;
  this->stringData_ = (char *) lefMalloc(16);
  this->nameSize_ = 16;
  this->propName_ = (char *) lefMalloc(16);
  lefiProp_clear(this);
}

void 
lefiProp_Destroy(lefiProp * this)
{
  lefFree(this->stringData_);
  lefFree(this->propName_);
}

void
lefiProp_Delete(lefiProp * this)
{
  if (this == NULL)
    return;
  lefiProp_Destroy(this);
  free(this);
}

void 
lefiProp_setPropType(lefiProp * this,
		     const char *typ,
		     const char *string)
{
  int     len;

  this->propType_ = (char *) typ;
  if ((len = strlen(string) + 1) > this->nameSize_)
    lefiProp_bumpName(this,
		      len);
  strcpy(this->propName_, CASE(string));
}

void 
lefiProp_setRange(lefiProp * this,
		  double left,
		  double right)
{
  this->hasRange_ = 1;
  this->left_ = left;
  this->right_ = right;
}

void 
lefiProp_setNumber(lefiProp * this,
		   double d)
{
  this->hasNumber_ = 1;
  this->d_ = d;
}

void 
lefiProp_setPropInteger(lefiProp * this)
{
  this->dataType_ = 'I';
}

void 
lefiProp_setPropReal(lefiProp * this)
{
  this->dataType_ = 'R';
}

void 
lefiProp_setPropString(lefiProp * this)
{
  this->dataType_ = 'S';
}

void 
lefiProp_setPropQString(lefiProp * this,
			const char *string)
{
  int     len;

  this->dataType_ = 'Q';
  if ((len = strlen(string) + 1) > this->stringLength_)
    lefiProp_bumpSize(this,
		      len);
  strcpy(this->stringData_, CASE(string));
}

void 
lefiProp_setPropNameMapString(lefiProp * this,
			      const char *string)
{
  int     len;

  this->dataType_ = 'N';
  this->hasNameMapString_ = 1;
  if ((len = strlen(string) + 1) > this->stringLength_)
    lefiProp_bumpSize(this,
		      len);
  strcpy(this->stringData_, CASE(string));
}

const char *
lefiProp_string(const lefiProp * this)
{
  return this->stringData_;
}

const char *
lefiProp_propType(const lefiProp * this)
{
  return this->propType_;
}

int 
lefiProp_hasNumber(const lefiProp * this)
{
  return (int) (this->hasNumber_);
}

int 
lefiProp_hasRange(const lefiProp * this)
{
  return (int) (this->hasRange_);
}

double 
lefiProp_number(const lefiProp * this)
{
  return this->d_;
}

double 
lefiProp_left(const lefiProp * this)
{
  return this->left_;
}

double 
lefiProp_right(const lefiProp * this)
{
  return this->right_;
}

void 
lefiProp_bumpSize(lefiProp * this,
		  int size)
{
  lefFree(this->stringData_);
  this->stringData_ = (char *) lefMalloc(size);
  this->stringLength_ = size;
  *(this->stringData_) = '\0';
}

void 
lefiProp_bumpName(lefiProp * this,
		  int size)
{
  lefFree(this->propName_);
  this->propName_ = (char *) lefMalloc(size);
  this->nameSize_ = size;
  *(this->propName_) = '\0';
}

void 
lefiProp_clear(lefiProp * this)
{
  if (this->stringData_)
    *(this->stringData_) = '\0';
  if (this->stringData_)
    *(this->propName_) = '\0';
  this->propType_ = 0;
  this->hasRange_ = 0;
  this->hasNumber_ = 0;
  this->hasNameMapString_ = 0;
  this->dataType_ = 'B';
  this->d_ = this->left_ = this->right_ = 0.0;
}

int 
lefiProp_hasString(const lefiProp * this)
{
  return *(this->stringData_) ? 1 : 0;
}

const char *
lefiProp_propName(const lefiProp * this)
{
  return (this->propName_);
}

char 
lefiProp_dataType(const lefiProp * this)
{
  return (this->dataType_);
}

void 
lefiProp_print(const lefiProp * this,
	       FILE * f)
{
  fprintf(f, "Prop type '%s'\n", lefiProp_propType(this));
  if (lefiProp_hasString(this)) {
    fprintf(f, " string '%s'\n", lefiProp_string(this));
  }
  if (lefiProp_hasNumber(this)) {
    fprintf(f, " number %5.2f\n", lefiProp_number(this));
  }
  if (lefiProp_hasRange(this)) {
    fprintf(f, " range %5.2f - %5.2f\n", lefiProp_left(this), lefiProp_right(this));
  }
}
