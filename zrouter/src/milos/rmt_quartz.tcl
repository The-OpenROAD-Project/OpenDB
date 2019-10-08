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

## rmt.tcl
## Remote module/proxy used by Milos
## Author: Mattias Hembruch

package require Itcl
package require ade::milos::mproxy
package require ade::milos::rmt_base

package provide ade::milos::rmt 0.1


itcl::class RMT_quartz {
    inherit RMT_psta 

    public method set_plugin_status { } {
        set plugins_local { }
        set plugins_remote { CELTIC TE }
        set plugins_ignore { }
        set plugins_invalid { }

        init_plugin_object TE PLUGIN_quartz
    }

    public proc new { {name ""} {async 0} {rmt_app "milos"} } {
        if { $name eq "" } {
            set r [RMT_quartz #auto $async $rmt_app]
        } else {
            set r [RMT_quartz $name $async $rmt_app]
        }
        return $r
    }


    public method get_db { name } {

        ## Write files (can only be done on main - not sure of having DB)
        file mkdir [$sobj cget -tmp_dir]

        set sname [plugin_fname $sobj "quartz" "spef"]
        UPOBJ enable_eco_log quartz

        if { [$sobj cget -spef_file] eq "" } {
            set ec [$corner cget -ext_corner]
            call_milos "pdb write_spef -prime_time 1 -outfile $sname -ext_corner_name $ec"
            $sobj configure -spef_file "${sname}[get_spef_ext]"
        }
        
        if { [$sobj cget -verilog_file] eq "" } {
            write_verilog $sobj
        }

        check_files [$sobj cget -spef_file] [$sobj cget -verilog_file]
        plugin_api TE set_timing_engine
        copy_data_to_plugin
    }


    public method start_db { } {
        ## Set up plugin
        incr startsocket 100
        plugin_api TE start_plugin $startsocket
    }

}
