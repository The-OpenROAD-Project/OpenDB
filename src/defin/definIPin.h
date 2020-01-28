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

#include <vector>

class definIPin
{
 public:
  // Pin interface methods
  virtual void pinsBegin(int n) {}
  virtual void pinBegin(const char* name, const char* net) {}
  virtual void pinSpecial() {}
  virtual void pinUse(defSigType type) {}
  virtual void pinDirection(defIoType type) {}
  virtual void pinPlacement(defPlacement status, int x, int y, defOrient orient)
  {
  }
  virtual void pinMinSpacing(int spacing) {}
  virtual void pinEffectiveWidth(int width) {}
  virtual void pinRect(const char* layer, int x1, int y1, int x2, int y2) {}
  virtual void pinPolygon(std::vector<defPoint>& points) {}
  virtual void pinGroundPin(const char* groundPin) {}
  virtual void pinSupplyPin(const char* supplyPin) {}
  virtual void pinEnd() {}
  virtual void pinsEnd() {}
};


