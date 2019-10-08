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

package require Itcl

source globals.tcl
source util.tcl
source timer.tcl

if { [info var _Config_true] != "" } {
    return
}
set _Config_true 1

itcl::class Config {
	private variable zr_path ""
    private variable zr_name "ade";
    private variable zr_opts "";
    private variable zr_found;
    private variable platform $::tcl_platform(platform);
    private variable top;
    private variable testlist;
    private variable total 0;
    private variable success 0;
    private variable execerred 0;
    private variable execerrlist "";
    private variable failure 0;
    private variable faillist "";
    private variable unknown 0;
    private variable unknownlist "";
    private variable skipped 0;
    private variable skiplist "";


    private variable timer;

	constructor {args} {
		eval configure $args
        set timer [Timer #auto]
	}

	public method set_zr { path } {
        if { [file isdirectory $path] } {
            putd "set_zr: dir"
            set ::env(PATH) "$path:$::env(PATH)"
        } elseif { [file executable $path] } {
            set ::env(PATH) "[file dirname $path]:$::env(PATH)"
            putd "set_zr: exe"
        } else { 
            putd "set_zr: ??"
            return
        }
        find_zr
	}

    public method get_zr { } {
        return $zr_name
    }

    public method get_platform { } {
        return $platform
    }

    public method get_zr_opts { } {
        if { $zr_opts == "" } {
            find_zr_opts 
        }
        return $zr_opts
    }

    private method find_zr_opts { } {
        if { $zr_opts != "" } {
            ## Already found - optimization
            return
        }

        if { $zr_found != 1 } {
            puts "Ade not found - cannot get ade options"
            return
        }

        set zr_opts ""
        set rc [catch {exec $zr_name get_zr_options.tcl} zr_opts]
        putd "zr_opts: $zr_opts"
    }

	public method find_zr {} {

        set rc [catch {exec which $zr_name } msg]

        if { $rc != 0 } {
            putd "ade not found in current PATH!"
            set zr_found 0
        } else {
            set zr_found 1
            putd "ade found!"
            set zr_path $msg
        }
	}

    public method set_tests { tl } {
        set testlist { }
        foreach t $tl {
            lappend testlist [Test #auto $t]
        }
    }

    public method show_tests { } {
        if { [llength $testlist] > 0 } {
            foreach t $testlist {
                $t show
            }
        } else {
            puts "No tests defined!"
        }
    }

    public method execute_tests { } {
        global ctype
        
        if { $zr_found == 0 } {
            puts "Cannot Execute tests - no ade in \$PATH!"
            exit -1
        } 


        if { [llength $testlist] > 0 } {

            if { ($ctype == "exec") || ($ctype == "run") } {
                set rt 0
                foreach t $testlist {
                    set r [$t get_runtime]
                    if { ([string is integer $r]) && ($r > 0) } {
                        incr rt $r
                    }
                }
                puts "Estimated total runtime: $rt s"

                find_zr_opts
            }  else {
                set zr_opts "N/A"
            }


            puts "\nStart of Testing"
            puts "================\n"

            puts "Using $zr_path to run tests"
            $timer start
            foreach t $testlist {
                set res [$t execute $zr_opts $ctype]
                if { [string is integer $res] } {
                    if { $res == 0 } {
                        incr success
                    } else {
                        if { [$t get_execresult] != 0 } {
                            incr execerred
                            set execerrlist "$execerrlist\n\t[$t get_path]"
                        } elseif { [$t get_diffresult] != 0 } {
                            incr failure
                            set faillist "$faillist\n\t[$t get_path]"
                        } else {
                            incr unknown
                            set unknownlist "$unknownlist\n\t[$t get_path]"
                        }
                    }
                    incr total
                } else {
                    ## Noninteger result
                    puts "Test [$t get_name] skipped due to $res"
                    incr skipped
                    set skiplist "$skiplist\n\t[$t get_path]"
                }
            }
            $timer stop
        } else {
            puts "No tests defined!"
        }
    }

    public method show_results { } {
        global show_only verbose ctype
        if { $show_only != 0 } { return }

        if { [llength $testlist] > 0 } {
            if { $verbose == 1 } {
                foreach t $testlist {
                    $t show_result
                }
            }
            
            puts "\nTesting Summary"
            puts "===============\n"

            set tm [$timer get_difftime]
            if { $tm >= 500 } {
                set ts " in [$timer get_time]s"
            }  else {
                set ts ""
            }
            puts "$total tests [get_cmd_text $ctype]$ts."
            puts "$success successful."
            puts "$execerred execution error(s)."
            if { $execerred > 0 } {
                puts "Tests unable to execute: $execerrlist"
            }
            puts "$failure diff error(s)."
            if { $failure > 0 } {
                puts "Tests that failed diff: $faillist"
            }
            if { $unknown > 0 } {
                puts "$unknown unknown error(s)"
                puts "Tests with unknown errors:\n$unknownlist"
            }
            if { $skipped > 0 } {
                puts "$skipped skipped."
                puts "Skipped tests:\n$skiplist"
            }
                
        } else {
            puts "No tests defined!"
        }
    }

}

