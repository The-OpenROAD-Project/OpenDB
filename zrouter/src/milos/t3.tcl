#
# BSD 3-Clause License
#
# Copyright (c) 2019, Nefelus Inc
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#catch {exec lsfkill}

if { [file exists ex.db] } {
    pdb load_design -dbname ex.db
} else {
    pdb load_design -lefs /fs/designs/e/e.lef -def /fs/designs/e/e.def
    pdb save_db -dbname ex.db
}

tmg gui_start

set d  /fs/designs/nec2/libs
set d2 /fs/designs/nec_sigmodup/time_LIBS

set parms(ext_rules)  /fs/designs/extLibs/nec90mm.extRules
set parms(maxlibs) [ list \
  $d/CB130M_CMOS_12V_H9_MAX_DRIVE.lib \
  $d/CB130M_CMOS_12V_H9_MAX_SCAN.lib \
  $d/CB130M_CMOS_12V_H9_MAX_GATING.lib \
  $d/CB130M_CMOS_12V_H9_MAX_SPECIAL.lib \
  $d/CB130M_CMOS_12V_H9_MAX_IO.lib \
  $d/CB130M_CMOS_12V_H9_MAX_PRIM.lib \
  $d2/CB130M_CMOS_12V_M9_MAX_TESTACT.lib  \
  $d2/CB130M_CMOS_12V-M_COM_MAX.lib \
  $d2/CB130M_FAKE_MAX.lib \
]

set parms(minlibs) [ list \
  $d/CB130M_CMOS_12V_H9_MIN_DRIVE.lib \
  $d/CB130M_CMOS_12V_H9_MIN_SCAN.lib \
  $d/CB130M_CMOS_12V_H9_MIN_GATING.lib \
  $d/CB130M_CMOS_12V_H9_MIN_SPECIAL.lib \
  $d/CB130M_CMOS_12V_H9_MIN_IO.lib \
  $d/CB130M_CMOS_12V_H9_MIN_PRIM.lib \
  $d2/CB130M_CMOS_12V_M9_MAX_TESTACT.lib  \
  $d2/CB130M_CMOS_12V-M_COM_MAX.lib \
  $d2/CB130M_FAKE_MAX.lib \
]

## enable "disassembly" debug
#pdb e "::zr.zlog enable_debug_log -module ZTM"
#pdb e "::zr.zlog enable_debug_log -module MILOS_TMG_MM"
#pdb e "::zr.zlog enable_debug_log -module MILOS_RMT"


## start multimode setup
tmg init_multimode 

tmg reset_threshold -upper_pct 59 -lower_pct 41 -derate 0.18

tmg define_corner -corner SETUP \
-min_derate 1.0 -max_derate 1.22 \
-lib $parms(maxlibs)

tmg define_corner -corner SETUP2 \
-min_derate 1.0 -max_derate 1.5 \
-lib $parms(maxlibs)

tmg define_corner -corner CLONE \
-min_derate 1.0 -max_derate 1.22 \
-lib $parms(maxlibs)

tmg define_corner -corner HOLD \
-min_derate 0.8 -max_derate 1.7 \
-lib $parms(minlibs)



## this will be the local node (priority 1)
tmg create_scenario -name SETUP -corner SETUP -sdc_file { e2.sdc } -ext_rules /fs/designs/extLibs/nec90mm.extRules -priority 1

## now define remote/other nodes/corners
tmg create_scenario -name CLONE  -corner CLONE  -sdc_file { e2.sdc } -ext_rules /fs/designs/extLibs/nec90mm.extRules 

## now define remote/other nodes/corners
tmg create_scenario -name SETUP2  -corner SETUP2  -sdc_file { e2.sdc } -ext_rules /fs/designs/extLibs/nec90mm.extRules 

## now define remote/other nodes/corners
tmg create_scenario -name HOLD  -corner HOLD  -sdc_file { e2.sdc } -ext_rules /fs/designs/extLibs/nec90mm.extRules 

## finish multimode setup
tmg apply_multimode -name "dac"


## run some optimization command
opt size -change_limit 50000 -setup_incr 1 -slack_target 0.8

tmg finish_multimode
#exit
