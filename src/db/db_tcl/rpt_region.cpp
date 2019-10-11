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

#include "db.h"
#include <logger.h>

BEGIN_NAMESPACE_ADS

void
db_report_regions(dbBlock *blk)
{
  dbSet<dbRegion> regions = blk->getRegions();
  dbSet<dbRegion>::iterator rit;
  for (rit = regions.begin(); rit != regions.end(); ++rit) {
    dbRegion *re = *rit;
    if (re->getParent()) continue;
    notice(0,"Region %s  %s\n",re->getName().c_str(),
                               re->getRegionType().getString());
    dbSet<dbBox> boxes = re->getBoundaries();
    dbSet<dbBox>::iterator bxit;
    int rj, rn = boxes.size();
    if (!rn) continue;
    adsRect *rr = new adsRect [rn];
    rn = 0;
    for (bxit = boxes.begin(); bxit != boxes.end(); ++bxit) {
      dbBox *box = *bxit;
      notice(0,"  (%d %d) (%d %d)\n",box->xMin(),box->yMin(),box->xMax(),box->yMax());
      box->getBox(rr[rn]);
      rn++;
    }
    if (re->getRegionType() == dbRegionType::EXCLUSIVE) {
      int assigned = 0;
      int not_contained = 0;
      int not_outside = 0;
      dbSet<dbInst> insts = blk->getInsts();
      dbSet<dbInst>::iterator iit;
      for (iit = insts.begin(); iit != insts.end(); ++iit) {
        dbInst *inst = *iit;
        bool belongs = false;
        dbRegion *rgi = inst->getRegion();
        if (rgi) {
          while (rgi->getParent()) rgi = rgi->getParent();
          if (rgi == re) {
            belongs = true;
            assigned++;
          }
        }
        adsRect ri;
        inst->getBBox()->getBox(ri);
        if (belongs) {
          for (rj = 0; rj<rn; rj++) {
            if ((rr+rj)->contains(ri)) break;
          }
          if (rj==rn) {
            not_contained++;
          }
        } else {
          for (rj = 0; rj<rn; rj++) {
            if ((rr+rj)->overlaps(ri)) break;
          }
          if (rj<rn) {
            not_outside++;
          }
        }
      }
      notice(0,"  %d instances assigned to this region\n",assigned);
      notice(0,"  %d instances are assigned but are not inside the region\n",not_contained);
      notice(0,"  %d instances are not assigned to, but are inside the region\n",not_outside);
    }
    else if (re->getRegionType() == dbRegionType::INCLUSIVE) {
      int assigned = 0;
      int not_contained = 0;
      dbSet<dbInst> insts = blk->getInsts();
      dbSet<dbInst>::iterator iit;
      for (iit = insts.begin(); iit != insts.end(); ++iit) {
        dbInst *inst = *iit;
        dbRegion *rgi = inst->getRegion();
        if (!rgi) continue;
        while (rgi->getParent()) rgi = rgi->getParent();
        if (rgi != re) continue;
        assigned++;
        adsRect ri;
        inst->getBBox()->getBox(ri);
        for (rj = 0; rj<rn; rj++) {
          if ((rr+rj)->contains(ri)) break;
        }
        if (rj==rn) {
          not_contained++;
        }
      }
      notice(0,"  %d instances assigned to this region\n",assigned);
      notice(0,"  %d instances are assigned but are not inside the region\n",not_contained);
    }
    delete [] rr;
  }
}

END_NAMESPACE_ADS
