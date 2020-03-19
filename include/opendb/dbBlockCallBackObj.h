///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019, Nefelus Inc
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <list>
#include "odb.h"

namespace odb {

class dbBlock;
class dbInst;
class dbMaster;
class dbNet;
class dbITerm;
class dbRegion;
class dbWire;

///////////////////////////////////////////////////////////////////////////////
///
/// dbBlockCallBackObj - An object comprising a list of stub routines
/// invoked by dbBlock.
/// Derived classes may implement these routines, causing external code
/// to be invoked from inside dbBlock methods.
///
///////////////////////////////////////////////////////////////////////////////

class dbBlockCallBackObj
{
 public:
  virtual void inDbInstCreate(dbInst*) {}
  virtual void inDbInstCreate(dbInst*, dbRegion*) {}
  virtual void inDbInstDestroy(dbInst*) {}
  virtual void inDbInstSwapMasterBefore(dbInst*, dbMaster*) {}
  virtual void inDbInstSwapMasterAfter(dbInst*) {}
  virtual void inDbNetCreate(dbNet*) {}
  virtual void inDbNetDestroy(dbNet*) {}
  virtual void inDbITermCreate(dbITerm*) {}
  virtual void inDbITermDestroy(dbITerm*) {}  // Bugzilla #7 - payam
  virtual void inDbITermDisconnect(dbITerm*) {}
  virtual void inDbITermConnect(dbITerm*) {}
  virtual void inDbBlockStreamOutBefore(dbBlock*) {}
  virtual void inDbBlockStreamOutAfter(dbBlock*) {}
  virtual void inDbBlockReadNetsBefore(dbBlock*) {}
  virtual void inDbMoveInst(dbInst*) {}
  virtual void inDbWireUpdate(dbWire*) {}

  // allow ECO client initialization - payam
  virtual dbBlockCallBackObj& operator()() { return *this; }

  // Manipulate _callback list of owner -- in journal.cpp
  void addOwner(dbBlock* new_owner);
  bool hasOwner() const { return (_owner != NULL); }
  void removeOwner();

  dbBlockCallBackObj() { _owner = NULL; }
  virtual ~dbBlockCallBackObj() { removeOwner(); }

 private:
  dbBlock* _owner;
};

}  // namespace odb
