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

#include "definTypes.h"

class definINet
{
 public:
  /// Net interface methods
  virtual void begin(const char* name) {}
  virtual void beginMustjoin(const char* iname, const char* pname) {}
  virtual void connection(const char* iname, const char* pname) {}
  virtual void nonDefaultRule(const char* rulename) {}
  virtual void use(defSigType type) {}
  virtual void wire(defWireType type) {}
  virtual void path(const char* layer) {}
  virtual void pathStyle(int style) {}
  virtual void pathTaper(const char* layer) {}
  virtual void pathTaperRule(const char* layer, const char* rule) {}
  virtual void pathPoint(int x, int y) {}
  virtual void pathPoint(int x, int y, int ext) {}
  virtual void pathVia(const char* via) {}
  virtual void pathVia(const char* via, int def_orient) {}
  virtual void pathEnd() {}
  virtual void wireEnd() {}
  virtual void source(defSource source) {}
  virtual void weight(int weight) {}
  virtual void fixedbump() {}
  virtual void xtalk(int value) {}
  virtual void property(const char* name, const char* value) {}
  virtual void property(const char* name, int value) {}
  virtual void property(const char* name, double value) {}
  virtual void end() {}
};


