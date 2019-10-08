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

## rpt.tcl
## Reporting module used by Milos
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::rpt 0.1

itcl::class RPT {

    inherit VOB

    ## Public Vars
    ##

    ## End Public Vars

    ## Protected Vars
    ##

    ## End Protected Vars

    protected method constructor { parent } { 
        VOB::constructor $parent
        set NAME "Report"
        set SELFNAME "Report$this"
        ## ONLY for public vars
        set_state rpt init
    }

    ## used to "close the hole"
    protected method info { } 


    public method help { {cmd ""} {param ""} {long "" } } {
        if { $cmd != "" && $cmd != "-long" } {
            cmd_help RPT $cmd $param $long
        } else {
            mod_help RPT $cmd
        }
    }


    VOB::add_args RPT_REPORT_TOTAL_CAP {
        { file ofile 0 } 
        { ref  ifile 0 } 
    }

    VOB::add_help RPT_REPORT_TOTAL_CAP {
        { file "Filename to save total capacitance report to" }
        { ref "Filename containing reference resistance/capacitance values for comparison" }
    }

    public method report_total_cap { args } {
        parse_args
        parasitics_check

        echo [cmdline]
        #echo [settings]

        set ext [get_tm Ext]
        $ext report_total_cap -file [get_arg file]
        cmd_rlog
    }


    public method get_power { } {
        if { [state ttype] == "sta" } {
            set pwr [get_tm Power]
            set ::design_power ""
            set ::power_unit ""
            $pwr get_power -value_list= ::design_power
            $pwr get_unit -unit= ::power_unit
            return [list $::design_power $::power_unit]
        }
    }


    VOB::add_args RPT_GET_TOTAL_SLACK {
        { delay_type nstr 0 "max" }
        { merged bool 0 1 }
    }

    VOB::add_help RPT_GET_TOTAL_SLACK {
        { delay_type "Which mode to return total slack for: min or max" }
        { merged "Return merged slack or local slack" }
    }

    public method get_total_slack { args } {

        parse_args
        get_all_args
        echo [cmdline]

        if { [state rmt_local] eq "" } {
            set tmg [get_tm Tmg]

            set ::mytns ""
            set rc [$tmg get_total_negative_slack -delay_type $delay_type -tns= ::mytns]

            return $::mytns
        } else {
            if { $merged == 0 } {
                set nname [ [[state rmt_local] get_sobj] cget -name]
                if { $nname eq "NULL" } {
                    set nname [ [state nulled_scenario] cget -name]
                }
                set retval [UPOBJ find_node_slack $nname]
                if { $delay_type eq "max" } {
                    return [lindex [lindex $retval 0] 2]
                } else {
                    return [lindex [lindex $retval 1] 2]
                }
            } else {
                [get_tm Tmg] get_total_negative_slack -delay_type $delay_type -tns= ::mytns
                return $::mytns
            }

        }
    }


    VOB::add_args RPT_GET_WORST_SLACK {
        { delay_type nstr 0 "max"}
        { viol bool 0 0}
        { merged bool 0 1 }
    }

    VOB::add_help RPT_GET_WORST_SLACK {
        { delay_type "Which mode to return worst slack for: min or max" }
        { viol "Return number of violations" }
        { merged "Return merged slack or local slack" }
    }

    public method get_worst_slack { args } {

        parse_args
        echo [cmdline]
        get_all_args

        if { [state rmt_local] eq "" } {
            ## standalone mode
            set tmg [get_tm Tmg]

            set ::myws -1
            set trc [$tmg get_worst_slack -delay_type $delay_type -slack= ::myws -viol= ::myviol]

            if { $viol == 0 } {
                return $::myws
            } else {
                return "$::myws $::myviol"
            }
        } else {
            if { $merged == 0 } {
                set nname [ [[state rmt_local] get_sobj] cget -name]
                if { $nname eq "NULL" } {
                    set nname [ [state nulled_scenario] cget -name]
                }
                set retval [UPOBJ find_node_slack $nname]
                if { $delay_type eq "max" } {
                    set rv [lindex $retval 0]
                } else {
                    set rv [lindex $retval 1]
                }
                set retval [lindex $rv 1]
                if { $viol == 1 } {
                    lappend retval [lindex $rv 3]
                }
                return $retval
            } else {
                [get_tm Tmg] get_worst_slack -delay_type $delay_type -slack= ::myws -viol= ::myviol
                if { $viol == 1 } {
                    return [list $::myws $::myviol]
                } else {
                    return $::myws
                }
            }

        }
    }

    public method get_all_slacks { {min 1} {max 1} } {
        if { [state rmt_local] eq "" } {
            set retval {}
            if { $min == 1 } {
                    foreach {min_slack min_viol} [get_worst_slack -delay_type min -viol 1] { break; }
                    lappend retval $min_slack
                    set min_tslack [get_total_slack -delay_type min]
                    lappend retval $min_tslack
                    lappend retval $min_viol
            }
            if { $max == 1 } {
                    foreach {max_slack max_viol} [get_worst_slack -delay_type max -viol 1] { break; }
                    lappend retval $max_slack
                    set max_tslack [get_total_slack -delay_type max]
                    lappend retval $max_tslack
                    lappend retval $max_viol
            }

            return $retval
        } else {
            return [UPOBJ find_node_slack ALL]
        }
    }

    VOB::add_args RPT_REPORT_SLEW_VIOLATIONS {
        {slew_limit num 0}
        {clock bool 0 0}
        {file ofile 0 }
    }

    VOB::add_help RPT_REPORT_SLEW_VIOLATIONS {
        {slew_limit "Maximum slew limit to report" }
        {clock "If on (1), report violations for clock nets." }
        {file "Output file for debugging" }
    }

    public method report_slew_violations { args } {
        parse_args
        parasitics_check
        echo [cmdline]

        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg report_slew_violations]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc during report_slew_violations!" "" -1
        }
        cmd_rlog

    }

    VOB::add_args RPT_REPORT_MAX_CAP_VIOLATIONS {
        {file ofile 0}
    }

    VOB::add_help RPT_REPORT_MAX_CAP_VIOLATIONS {
        {file "File to save max_cap violations to."}
    }

    public method report_max_cap_violations { args } {
        parse_args
        parasitics_check
        echo [cmdline]

        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "report_max_cap_violations"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc during report_max_cap_violations" "" -1
        }
        cmd_rlog
    }


}
