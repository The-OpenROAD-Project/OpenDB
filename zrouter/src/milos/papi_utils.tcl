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

## plugin.tcl - plugin methods
## System module for Milos
## Author: Mattias Hembruch

package require Itcl

package provide ade::milos::papi 0.1

itcl::class PAPI_UTILS {

    public method this { } {
        return $this
    }
    public method issocket { fp } {
        set rc [ catch { fconfigure $fp -sockname } ]
        #puts "ISS: $rc"
        return $rc
    }

    public method write_utils { fp proc_list } {
        foreach proc $proc_list { 
            #puts "Writing proc $proc"
            set pargs [$this info args $proc]
            set pdef [$this info body $proc]
            set cmd [list proc $proc $pargs $pdef]
            if { [issocket $fp] == 0 } {
                #puts "SOCKET!"
                puts $fp $cmd
                #puts $fp [list $cmd]
                puts $fp "."
                get_plugin_results $fp
            } else {
                #puts "not SOCKET!"
                puts $fp $cmd
            }
        }
    }

    public method write_code { fp proc_list } {
        foreach proc $proc_list { 
            puts "Writing proc $proc"
            set pdef [$this info body $proc]
            puts $fp $pdef
        }

    }

    protected method get_plugin_results { fp } {
        set result ""
        while { true } {
            gets $fp line
            if { $line eq "." } {
                break;
            } elseif { $line eq ".E." } {
                puts "ERROR in GPR: $result"
            } else {
                append result $line
            }
        }
        #puts "GPR results: $result"
    }
            

}
