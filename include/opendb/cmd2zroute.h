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

#ifdef _WIN32
#include "direct.h"
#endif

#include "layout.h"
#include "z2tcl.h"

void  Ath__zroute_init(uint layerCnt, uint masterCnt);
int   Ath__readLef(char* fileName);
int   Ath__readLefMacros(char* fileName);
int   Ath__readDef(char* fileName);
int   Ath__readDefTiles(char* fileName);
int   Ath__readDefBlocks(char* fileName);
int   Ath__makeTiles(char* list, int rows, int cols, char* topH, char* topV);
int   Ath__zui_get(char* filter);
uint  Ath__getTclBoxCnt();
char* Ath__getTclBox(int ii);
uint  Ath__getTclBoxCntV();
char* Ath__getTclBoxV(uint ii);
int   Ath__assign_tracks(char* list, char* topH, char* topV);
int   Ath__chip_get(char* bb, char* filter);
int   Ath__tiles_get(char* bb);

int Ath__chipAttr_get(int*  x1,
                      int*  y1,
                      int*  x2,
                      int*  y2,
                      char* layers,
                      char* objects,
                      char* hierarchy);

int Ath__navigate(char* tileName, char* bb);

uint Ath__print_routing_layers();

uint Ath__pins_assign(char* blkName,
                      char* tile,
                      int   connType,
                      int   priority,
                      int   style,
                      int   space,
                      int   space_units,
                      int   width,
                      int   width_units,
                      int   length,
                      int   length_units,
                      char* top_h_layer,
                      char* top_v_layer);

uint Ath__bus_make(char* blkName, char* tile, int opt, int width, int space);
uint Ath__db_write(char* blkName, char* dir, int format);
uint Ath__db_read(char* blkName, char* dir, int format);

uint Ath__tiles_route(char* blkName, char* tile);
