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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef TMG_STA_H
#define TMG_STA_H

#include "ads.h"
#include "db.h"
#include "arnoldi1.h"
#include "darr.h"

namespace sta
{
  class TransRiseFall;
  class Pin;
  class Vertex;
  class LibertyLibrary;
  class LibertyCell;
}

namespace odb {

class tmg_db;
struct tmg_corner;

struct tmg_ctf {
  double max_r,max_f,min_r,min_f;
};

struct tmg_pin_list {
  void *pin;
  tmg_pin_list *next;
};
struct tmg_adj_clk_schedule {
  double max_r,max_f,min_r,min_f;
  Darr<dbITerm*> exclude;
  bool set;
  Darr<dbMaster*> bufs;
};

class tmg_sta {
 public:
  tmg_db *_parent;
  dbBlock *_block;
  int _brefN;
  int *_brefV;
  char _pinName[4096];
  bool _has_extra_delay;
  tmg_ctf *_ctfV;
  int _ctfN;
  tmg_adj_clk_schedule *_adj_clk_scheduleV;
  int _adj_clk_scheduleN;
 private:
  int _altPinN;
  dbITerm *_altPinI[2];
  dbBTerm *_altPinB[2];
  char _altPinName[4096];
  double *_delayV;
  double *_slewV;
  int _delayNmax;

 public:
  tmg_sta(tmg_db *parent);
  ~tmg_sta();

  enum FlagType { TmgActivePinFlag, TmgTouchedPinFlag, TmgEndPinFlag, TmgNewPinFlag };

                          void setFlag(const sta::Pin* const pin, const FlagType flagType, const bool flag) const;
  template<class dbTerm>  void setFlag(dbTerm* const term, const FlagType flagType, const bool flag) const;
                          bool isFlagSet(const sta::Pin* const pin, const FlagType flag) const;
  template<class dbTerm>  bool isFlagSet(const dbTerm* const term, const FlagType flag) const;

                          bool vertices(const sta::Pin* const pin, sta::Vertex* &vertex, sta::Vertex* &vertexb) const;
  template<class dbTerm>  bool vertices(dbTerm* const term, sta::Vertex* &vertex, sta::Vertex* &vertexb) const;

        sta::LibertyCell*    const libertyCell(dbMaster* const master) const;
        sta::LibertyCell*    const libertyCell(dbInst* const inst) const;
  const sta::LibertyLibrary* const library(dbMaster* const master) const;
  const sta::LibertyLibrary* const library(dbInst* const inst) const;
  const long                       uniqueId(const sta::Pin*) const;
  const long                       uniqueId(dbITerm* const iterm) const;
  const long                       uniqueId(dbBTerm* const bterm) const;
  const std::string                name(dbITerm* const iterm) const;
  const std::string                name(dbBTerm* const bterm) const;
  const std::string                name(const sta::Pin* const pin) const;
  const std::string                name(dbNet* const net) const;
  const std::string                name(dbMaster* const master) const;
  const std::string                name(dbInst* const inst) const;
  const sta::Pin*            const pin(dbITerm* const iterm) const;
  const sta::Pin*            const pin(dbBTerm* const bterm) const;
  const sta::Pin*            const pin(const long uniqueId) const;
        void                       ibterm(const sta::Pin* const pin, dbITerm* &iterm, dbBTerm* &bterm) const;
  const bool                       isClock(dbITerm* const iterm) const;
  const bool                       isClock(dbBTerm* const bterm) const;
  const dbInst*              const pin_inst(const sta::Pin* const pin) const;
        dbNet*                     pin_net(const void* pin) const;

  void *getStaCell(dbMaster *master);
  void *getStaPort(dbMTerm *mterm);
  double getTotalCap(dbNet *net, const sta::TransRiseFall* const tr, bool is_min);

  double getExtPinCap(dbBTerm *bterm, const sta::TransRiseFall* const tr);
  double getMaxCap(dbITerm *iterm, const sta::TransRiseFall* const tr);
  double getCap(dbITerm *iterm, const sta::TransRiseFall* const tr, tmg_corner *tc, bool is_min);

  bool  gateWireDelay(dbITerm *itermDrv,double in_slew,
             void *in_sypin,
             const sta::TransRiseFall* const tr_in, const sta::TransRiseFall* const tr_out,
             bool is_min,
             const void **pinV,double *delayV,double *slewV,int *pinN);

  void findAlternatePinNames(const char *pattern);
  const char *getAlternatePinName();
  int ceffMethod();
  arnoldi1 *getRcmod(const sta::Pin* const pin, const sta::TransRiseFall* const tr);
  arnoldi1 *getOrMakeRcmod(const sta::Pin* const pin, const sta::TransRiseFall* const tr, bool is_min);
  void getRcmodPins(arnoldi1 *mod1,const void **pinV);
  void getVectors(double **dv,double **sv,int n);
  void readCtf(const char *filename);
  void readClockSchedule(const char *filename);
  void implementClockSchedule(const char *filename, bool max);
  void clearCtf();
  double getExtraDelay(const sta::Pin* const drvr_pin, const sta::TransRiseFall* const tr, const sta::MinMax* const min_max);
  double getExtraDelay2(const sta::Pin* const recv_pin, const sta::TransRiseFall* const tr, const sta::MinMax* const min_max);
  double getExtraCalibDelay(const sta::Pin* const recv_pin, const sta::TransRiseFall* const tr, const sta::MinMax* const min_max);
  double getExtraClockScheduleDelay(const sta::Pin* const recv_pin, const sta::TransRiseFall* const tr, const sta::MinMax* const min_max);
};

#include "../tmg/TmgSta.T"

} // namespace

#endif

