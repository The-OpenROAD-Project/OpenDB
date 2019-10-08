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

## dobj.tcl
## General Data objects for Milos
## Author: Mattias Hembruch

package require Itcl

package provide ade::milos::dobj 0.1

itcl::class DOBJ {
    private variable _my_cmd
    private variable _my_flag ""

    public method this { } {
        return $this
    }

    public method set_cmd { c } {
        set _my_cmd $c
    }

    public method get_cmd { } {
        return $_my_cmd
    }

    public method get_flag { } {
        return $_my_flag
    }

    public method set_flag { val } {
        set _my_flag $val
    }

    public method get_conf { } {
        set conf [$this configure]
        set cmd ""
        foreach val $conf {
            foreach {var old new} $val { 
                if { $old ne $new } {
                    append cmd "$var {$new} "
                }
            }
        }
        return $cmd
    }

    public method reset { } {
        set conf [$this configure]
        set cmd ""
        foreach val $conf {
            foreach {var old new} $val {
                if { $old ne $new } {
                    $this configure $var $old
                }
            }
        }
    }

    public method copy_to { obj } {
        foreach { option val } [get_conf] {
            $obj configure $option $val
        }
        return $obj
    }
}


itcl::class THRESH {
    inherit DOBJ

    public variable upper_rise_pct;
    public variable upper_fall_pct;
    public variable upper_pct;
    public variable lower_rise_pct;
    public variable lower_fall_pct;
    public variable lower_pct;
    public variable derate;
}
    
itcl::class EXT_CORNER {
    inherit DOBJ

    public variable model_idx -1;
    public variable name "";
}

itcl::class DER_CORNER {
    inherit DOBJ

    public variable ext_corner "";
    public variable name "";
    public variable res_factor 1.0;
    public variable cc_factor 1.0;
    public variable gndc_factor 1.0;
}

itcl::class CORNER {

    inherit DOBJ

    public variable corner ""
    public variable min_derate 1.0
    public variable max_derate 1.0
    public variable ext_corner ""
    public variable min_ext_corner ""
    public variable max_ext_corner ""
    public variable op_cond ""
    public variable min_op_cond ""
    public variable max_op_cond ""
    public variable lib ""
    public variable min_lib ""
    public variable max_lib ""
    public variable sdf_file ""

    ## Scenario objects using this corner..
    public variable sobj_list {}


    ## variables for NEC's CW flow
    ## has sdf processing been done for this corner?
    public variable sdf_done 0
    public variable delay_active 0
    public variable delay_exec_name ""
    public variable delay_parameter_string ""
}   



itcl::class NODE_SLACK {
    inherit DOBJ

    constructor { } {
        #puts "NODE_SLACK $this!!!"
    }
    public variable worst_min_slack -99999.9;
    public variable native_worst_min_slack -99999.9;
    public variable total_min_slack -99999.9;
    public variable num_min_violations -1;
    public variable worst_min_term "none";

    public variable worst_max_slack -99999.9;
    public variable native_worst_max_slack -99999.9;
    public variable total_max_slack -99999.9;
    public variable num_max_violations -1;
    public variable worst_max_term "none";

    public variable sized_cells_min 0;
    public variable new_cells_min 0;
    public variable cells_removed_min 0;
    public variable opt_method_min "none";
    public variable time_elaps_min 0;

    public variable sized_cells_max 0;
    public variable new_cells_max 0;
    public variable cells_removed_max 0;
    public variable opt_method_max "none";
    public variable time_elaps_max 0;

    public variable plugin_time -1;
}

itcl::class PLUGIN_DATA {
    inherit DOBJ
    public variable server_socket -1;
    public variable server_hostname "localhost";
    public variable plugin_socket -1;
    public variable plugin_hostname "localhost";
    public variable plugin_string ""
    public variable active 0;
    public variable result "";
    public variable error "";
    public variable license_type;
    public variable engine;
    public variable all_terms 0;
    public variable plugin_cmd_pending 0;
    public variable plog;
    public variable translate 0;

    public variable timer -1;
}

itcl::class SCENARIO {
    inherit DOBJ

    ## used for gui
    public variable gname "";
    ## general scenario data
    public variable name "";
    public variable ignore_main_tmg 0;
    public variable block "";
    public variable corner "";
    public variable sdc_file "";
    public variable ext_model_file "";
    public variable priority 2;
    public variable engine "milos";
    public variable translate 0;
    public variable replace_string "";

    ## plugin data
    public variable schedule_style 1;
    public variable start_name "";
    public variable set_units "";
    public variable preinit_file "";
    public variable postinit_file "";
    public variable tmp_dir "";
    public variable verilog_module "";
    public variable verilog_file "";
    public variable verilog_original_file "";
    public variable spef_file "";
    public variable min_spef_file "";
    public variable max_spef_file "";
    public variable db "";
    public variable ltype "transient";
    public variable restore_dir "";
    public variable save_dir "";
    public variable timeout 30;

    ## returned from celtic
    public variable sdf_name "";

    ## celtic data

    public variable celtic_on 0;
    public variable celtic_start_name "ca celtic";
    public variable celtic_noiselib "";
    public variable celtic_vdd_value -99.99;
    public variable celtic_vdd_signals "VDD";
    public variable celtic_gnd_value 0.0;
    public variable celtic_gnd_signals "GND";
    public variable celtic_settings_file "";
    public variable celtic_report_file "";
    public variable celtic_process "130nm";
    public variable celtic_spef_file "";
    public variable celtic_copy_lib 1;

    private variable rmt "";
    private variable sidx 0;

    public method set_remote { r } {
        set rmt $r
    }

    public method get_remote { } {
        return $rmt
    }

    public method set_sidx { s } {
        set sidx $s
    }

    public method get_sidx { } {
        return $sidx
    }


}
