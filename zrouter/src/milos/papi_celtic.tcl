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


itcl::class PAPI_CELTIC {

    inherit PAPI_BASE;

    ##### ACTUAL METHODS
    public method constructor { } {
        set init_list { }
        set plugin_proc_list { celtic_init celtic_analyze }
        set server_proc_list { Server exit_remote open_debug debug close_debug }
        set plugin_api_list { }
    }

    public proc celtic_init { dprefix top spef sdc libs noiseLib vddSignals vddValue gndSignals gndValue settingsFile { myprocess 130nm } } {

        global dpref

        set dpref $dprefix

        #if { $::dlevel & 1 } {
        #    open_debug
        #}

        ## set the run mode
        set_run_mode -process $myprocess

        ## To turn on the timer
        set_parm timing 1

        ## enable the message handler
        message_handler -set_msg_level ALL

        foreach lib $libs {
            read_dotlib $lib
        }
        
        ## Set supply voltage
        set_supply -vdd $vddValue -gnd $gndValue

        # load_netlist
        load_netlist -spef $spef -cdb $noiseLib -top $top \
                     -vdd "$vddSignals" -gnd "$gndSignals"

        process_netlist
        read_dc_script $sdc
        calculate_delay

        if { [file exists $settingsFile] } {
            source $settingsFile
        }

    }

    public proc celtic_analyze { sdfFile si { reportFile "" } } {

        if { $si > 0 } {
            set_tw_convergence -iterations $si -first_infinite_windows
        }
        ## get noise data
        analyze_noise -delay

        if { $reportFile != "" } {
            if { [file exists $reportFile] } {
                source $reportFile
            } else {
                error "Unable to find report file '$reportFile' - exiting"
            }
        }

        ## write sdf file
        generate_sdf -file $sdfFile
    }

    public proc exit_remote { } {
        exit
    }

}
