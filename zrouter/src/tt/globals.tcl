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

## Testing Tool module - globals.tcl
### Author: Mattias Hembruch
#
# Global variables used by the program

if { [info var _Globals_true] != "" } {
    return
}

set _Globals_true 1

set top "../../test"
set zargs ""
set rcglobal "~/.ttrc"
set rclocal "./.ttrc"

set prompt "> "
set find "/usr/bin/find"
set groupfile "groups.dat"
set testfile "test.exe"
#set testfile "test_run.tcl"
set difffile "test.diff"
set cleanfile "test_clean.tcl"
set attrfile "test.attr"
set fdescr "::Description::"
set fplatform "::Platform::"
set fruntime "::Runtime::"
set foption "::Option::"
set fzargs "::Zargs::"
set testlist {}
set gtestlist(0,0) ""
set groups {}
set ctype "exec"
set makefile "Makefile"
set makeinc "Makefile.rules"

## Options
set show_only 0
set summary 1
set verbose 0
set autocreate 0

set cmdsep "_"

set zp1 "#!/bin/bash --noprofile\n##Automatically generated test file\n##Do not Edit\n"
set zp2 "Z=`which ade 2> /dev/null`"
#set zp3 "ZR=\$\{Z:-\$ZR}\n\n"
set zp3 ""
set zp4 "if \[\[ -f \$Z ]]\nthen"
set zprefix "$zp1\n$zp2\n$zp3$zp4"
set zcmd "    \$\{Z} \$\{ZARGS\} << 'EOF'\n    catch \{"
set zps0 "    \} _testoutput_\n    puts \$_testoutput_\n"
set zps1 "    exit 0\n"
set zps2 "EOF\n\nelse"
set zps3 {    echo "ade not found in current PATH"}
set zps4 "    exit 1\nfi"
set zpostfix "$zps0$zps1\n$zps2\n$zps3\n$zps4"

set ttcmds {clean run diff show new upcvs upgold }
set ttdesc {"cleaned" "executed only" "diffed" "showed" "created" "updated cvs" "updated gold"}

## Command targets for make
set makeupgold "make upgold"
set makeupcvs "make upcvs"
set makerun "make test.log"
set makeclean "make cleantest"
set makediff "make cleandiff test.diff"

## Group descriptions
set tgdescr(athena) "Tests internal to Athena"
set tgdescr(toshiba) "Tests required to ship to Toshiba"
set tgdescr(sanity) "Sanity Tests - make sure it works"
set tgdescr(basic) "Basic Tests - a little more involved than sanity"
set tgdescr(extreme) "Extreme tests - long-running, stress tests"
set tgdescr(installation) "Tests shipped for testing of a ade install"

