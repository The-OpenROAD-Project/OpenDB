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


package provide ade::apps 1.0

namespace eval apps { 
}

namespace eval apps::turborc {

variable VERSION "TurboRC 1.0"
variable AE_PW "MYAEPW"

#
# TURBORC application
#

# -- Initialize the turborc application
# --    Context: This call is made on the master interpeter
# --     Inputs: interp (the slave interpeter to be initialized)
# --    Outputs: "" if ok, "message" on error
# -- Exceptions: raise errors

proc init {slave} {
    # -- expose the read command
    package require ade::turborc 1.0
    TurboRC ::apps::turborc::trc
    interp alias $slave trc "" apps::turborc::trc
    interp alias $slave rx "" apps::turborc::rnode
    #interp alias $slave puts "" apps::turborc::puts
    interp alias $slave puts "" puts
    interp alias $slave exit "" exit


    $slave expose source

    $slave expose pwd
    $slave expose cd
    set cwd [file normalize .]
    set twd [file join $::top "../lib/apps"]
    set cmd " cd $twd; source history.tcl; cd $cwd"

    $slave eval $cmd
    #$slave hide cd

    set cmd "
            proc ::parray {a {pattern *}} {
                upvar 1 \$a array
                if {!\[array exists array]} {
                    puts \"\\\"\$a\\\" isn't an array\"
                }
                set maxl 0
                foreach name \[lsort \[array names array \$pattern]] {
                if {\[string length \$name] > \$maxl} {
                    set maxl \[string length \$name]
                }
                }
                set maxl \[expr {\$maxl + \[string length \$a] + 2}]
                foreach name \[lsort \[array names array \$pattern]] {
                set nameString \[format %s(%s) \$a \$name]
                puts \[format \"%-*s = %s\" \$maxl \$nameString \$array(\$name)]
                }
            }"

    $slave eval $cmd

    #set cmd "proc ::!! { } {
        #set nid \[expr \[history nextid] - 2]
        #history redo \$nid
    #}"
    #$slave eval $cmd

    return ""
}

proc banner { } {
    trc banner
    return 0
}


proc putx { str args } {
    set len [llength $args]
    if { $len == 0 } {
        ::zr.zlog putl $str
    } elseif  { $len == 1 } {
        ::zr.zlog putl $str [lindex $args 0]
    } elseif  { $len == 2 } {
        ::zr.zlog putl $str [lindex $args 0] [lindex $args 1]
    }
}

proc rx { b args } {
    package require ade::turborc 1.0
    set cmd "TurboRC::rnode $b $args"
    eval $cmd
}


# -- end of namespace apps
}
