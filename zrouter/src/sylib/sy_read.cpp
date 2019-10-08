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

#include <ctype.h>
#include "db.h"
using namespace ads;

#include "ztop.h"
#include "sy_lib.h"

extern FILE *liberty_parser2_in;
extern char *Sy_filename;
extern int Sy_lineno;

void read_liberty(int min_max,
                  const char *file_list,
                  const char *slew_def_hi,
                  const char *slew_def_lo)
{
  char file[4096];
  const char *pc;
  char *pf;
  for (pc=file_list; *pc; pc++) if (!isspace(*pc)) {
    pf = file;
    while (*pc && !isspace(*pc)) *(pf++) = *(pc++);
    *pf = '\0';
    --pc;
    if (pf==file) break;

    liberty_parser2_in = fopen(file,"r");
    if (!liberty_parser2_in) {
      notice(0,"cannot open %s\n",file);
    } else {
      sy_read_init();
      Sy_filename = file;
      Sy_lineno = 0;
      liberty_parser_parse();
      fclose(liberty_parser2_in);
      sy_library *sylib = sy_get_last_lib();
      if (sylib) sy_add_lib(min_max, sylib);
    }
  }
}
