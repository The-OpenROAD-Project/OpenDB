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


itcl::class RMT_milos {
    inherit RMT RMT_LOCAL_milos
    ### PUBLIC PROCS - NEW
    public proc new { {name ""} {async 0} {rmt_app "milos"} } {
        #puts "NRM!! $name $async $rmt_app"
        if { $name eq "" } {
        #    puts "NRM 1 $async $rmt_app"
            set r [RMT_milos #auto $async $rmt_app]
        } else {
        #    puts "NRM 2 $name $async $rmt_app"
            set r [RMT_milos $name $async $rmt_app]
        }
        return $r
    }

    constructor { async rmt_app } {
        RMT::constructor $async $rmt_app 
    } {
        #puts "Derived const!"
    }

    public method update_node_slack { req_slacks } {
        set newret $SLACK_UPDATE
        send_command acmd "\$apps::milos::mycf get_slacks $req_slacks"
    }

    public method update_eco { str } {
        send_command acmd "Subscribe $str"
        send_command tmg update_timing -force 0
    }

    public method reinit_db { dbs } {
        get_db $dbs
    }

    public method get_db { dbs } {
        send_command acmd "Subscribe $dbs"
        send_command acmd { set ::ns [::apps::milos::cur_ns] }
        send_command acmd "set \${::ns}::STATE(db_loaded) 1"
        send_command acmd "set \${::ns}::STATE(db_parasitics) 1"
        send_command acmd "set \${::ns}::STATE(tmg_rcmodels) 1"
    }

    public method start_db { } {
    }

    public method update_timing { {scenario 0} {num_scenarios 1} } {
        send_command acmd {[$::apps::milos::mycf get_tm Tmg] reset_timing_cache -scenario 0 -num_scenarios 1}
    }

    public method publish { name } {
        send_command acmd "Publish $name"
    }

    public method unpublish { name } {
        send_command acmd "Unpublish $name"
    }

    public method subscribe { name } {
        send_command acmd "Subscribe $name"
    }

    public method set_update_method { method } {
        send_command acmd "\[\$::apps::milos::mycf get_tm Tmg] set_update_method -method $method"
    }

    public method send_activeterms { string } {
        send_command acmd "Subscribe $string"
    }

    public method set_si { num } {
        send_command tmg "set_si -si $num"
    }

    public method set_sobj { obj } {
        set sobj $obj
        set corner [find_obj corner_list corner [$sobj cget -corner]]
        ## initialize remote CF/plugin object from here..
    }
}
