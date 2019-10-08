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
source timer.tcl
source util.tcl


if { [info var _Test_true] != "" } {
    return
}
set _Test_true 1

itcl::class Test {
    protected variable comp "package";
    protected variable name ".";
    protected variable fname "";
    protected variable path "";
    protected variable file $::testfile

    protected variable description "Generic Test";
    protected variable platform "all";
    protected variable runtime "unknown";
    protected variable options "none";
    protected variable zstartopts "none";
    protected variable optmatch 0;
    protected variable optmissing {};
    protected variable type "tcl";

    protected variable execplatform $tcl_platform(platform);
    protected variable platmatch 0;
    protected variable execsname "";
    protected variable execresult 1;
    protected variable execrun -1;
    protected variable execoutput "";

    protected variable diffresult -1;
    protected variable diffrun -1;
    protected variable diffoutput "";
    protected variable diffcmds "";

    protected variable cleanresult 1;
    protected variable cleanrun -1;
    protected variable cleanoutput "";

    protected variable cvsresult 1;
    protected variable cvsoutput "";
    protected variable cvscmds "";

    protected variable goldresult 1;
    protected variable goldoutput "";
    protected variable goldcmds "";

    private variable timer;

    private method check_platform { } {
        set platmatch 1
        if { $platform == "" } { 
            return
        }
        if { ([string compare -nocase $platform "all"] != 0) } {
            set platmatch 0
            foreach pt [split $platform " ,"] {
                if { [string compare -nocase $pt $execplatform] == 0 } {
                    set platmatch 1
                    break
                }
            }
        }
    }

    private method check_options { zol } {

        if { [string compare -nocase $options "none"] ==0 } {
            set optmatch 1
            return
        }

        set ol [split $options " ,"]
        set optmatch 0
        set optmissing {}
        foreach o $ol {
            foreach zo $zol {
                set o [string trim $o]
                set zo [string trim $zo]
                if { [string compare -nocase $o $zo] == 0 } {
                    set optmatch 1
                    break;
                }
            }
            if { $optmatch == 0 } {
                lappend optmissing $o
            } else {
                set optmatch 0
            }
        }
        if { [llength $optmissing] == 0 } {
            set optmatch 1
        }
    }

    public method set_finfo { fn } { 
        set fname $fn

        global top
        set comp [get_test_comp $fname]
        set name [get_test_name $comp $fname]
        set path [get_test_path $fname]
        set file [get_test_file $fname]


        set description [get_test_descr $fname]
        set platform [get_test_platform $fname]
        set runtime [get_test_runtime $fname]
        set options [get_test_options $fname]
        set zstartopts [get_test_zargs $fname]
    }

    constructor { args } {
        set_finfo $args
        set timer [Timer #auto]
    }


    public method get_execresult { } {
        return $execresult
    }
    public method get_diffresult { } {
        return $diffresult
    }

    public method get_name { } {
        return $name
    }

    public method get_path { } {
        return $path
    }

    public method get_fname { } {
        return $fname
    }

    public method get_runtime { } {
        return $runtime
    }

    public method show { } {
        puts ""
        puts "Test Information for test [file join $comp $name]"
        puts "Description: $description"
        if { $platform != "" } {
            puts "Platform: $platform"
        }
        if { $runtime != "" } {
            puts "Expected Runtime: $runtime"
        }
    }

    public method show_clean_result { } {
        if { $cleanrun == -1 } { return }
        puts "\nClean Result for test: ${comp}/$name"
        if { $cleanresult == 0 } {
            puts "Clean Status: Passed"
        } else {
            puts "Clean Status: Failed ($cleanresult)"
        }

        putv "Clean Output: \n$cleanoutput"
        puts ""
    }

    public method show_run_result { } {
        if { $execrun == -1 } { return }
        puts "\nExecution Result for test: ${comp}/$name"
        if { $execresult == 0 } {
            puts "Execution Status: Passed"
        } else {
            puts "Execution Status: Failed ($execresult)"
        }
        puts "Execution Runtime: [$timer get_time]"
        puts "Execution Platform: $execplatform"

        putv "Execution Output: \n$execoutput"
        puts ""
    }

    public method show_diff_result { } {
        if { $diffrun == -1 } { return }
        puts "\nDiff Result for test: ${comp}/$name"
        if { $diffresult == 0 } {
            puts "Diff Status: Passed"
        } else {
            puts "Diff Status: Failed ($diffresult)"
        }

        putv "Diff Commands: \n$diffcmds"
        putv "Diff Output: \n$diffoutput"
        puts ""
    }

    public method show_upgold_result { } {
        puts "\nUpdating Gold for test: ${comp}/$name"
        if { $goldresult == 0 } {
            puts "Updating Gold Status: Passed"
        } else {
            puts "Updating Gold Status: Failed ($goldresult)"
        }

        putv "Updating Gold Commands: \n$goldcmds"
        putv "Updating Gold Output: \n$goldoutput"
        puts ""
    }

    public method show_upcvs_result { } {
        puts "\nUpdating CVS for test: ${comp}/$name"
        if { $cvsresult == 0 } {
            puts "Updating CVS Status: Passed"
        } else {
            puts "Updating CVS Status: Failed ($cvsresult)"
        }

        putv "Updating CVS Commands: \n$cvscmds"
        putv "Updating CVS Output: \n$cvsoutput"
        puts ""
    }



    public method show_result { } {
        global ctype

        if { ($ctype == "exec") } {
            show_clean_result
            show_run_result
            show_diff_result
        } else {
            set cmd "show_${ctype}_result"
            eval $cmd
        }
    }

    private method exec_test_clean { zopts show_only } {
        global makeclean

        putv "Executing $makeclean to prepare test"

        if { $show_only  == 0 } {
            set cleanresult 0
            set cleanrun 0
            set cmd "catch \{exec $makeclean\} cleanoutput"
            set cleanresult [eval $cmd]
            if { $cleanresult != 0 } {
                puts "\nError running make clean: $cleanresult"
                puts "Output:\n"
                puts "$cleanoutput\n"
                set execresult $cleanresult
                return
            } else {
                putv "Clean Finished."
            }
            return $cleanresult
        }
    }

    private method exec_test_run { zopts show_only } {
        global makerun zargs

        check_platform 

        if { $platmatch == 0 } {
            puts "Test $name can only execute on the following platforms: $platform"
            puts "Skipping test"
            return "incompatible Platform"
        }

        check_options $zopts

        if { $optmatch == 0 } {
            puts "Test $name requires ade compiled-in option(s): $options"
            puts "Option(s) $optmissing were not compiled into ade"
            return "missing ade compiled-in option(s) $optmissing"
        }

        set ::env(ZARGS) "$zstartopts $zargs"

        putv "Executing $makerun to execute test"
        set execrun 0
        if { ([string is integer $runtime]) && ($runtime > 0) } {
            putv " (estimated runtime: $runtime s)"
        } else {
            putv ""
        }
        if { $show_only == 0 } {
            set execresult 0

            $timer start
            
            set cmd "catch \{exec $makerun\} execoutput"
            set execresult [eval $cmd]

            putv "Execution result: $execresult\nEnd of Execution result"
            $timer stop

            if { $execresult != 0 } {
                if { [file exists test.templog] } {
                    puts [exec cat test.templog]
                }
            }

            putv "Exec Finished (result=$execresult)."
            putv "Exec output: $execoutput"


            set ::env(ZARGS) "$zargs"
            return $execresult
        }
    }

    private method exec_test_diff { zopts show_only } {
        global makediff difffile

        putv "Executing $makediff to analyze test results"

        if { $show_only  == 0 } {
            set diffresult 0
            set diffrun 0
            set cmd "catch \{exec $makediff 2>/dev/null\} diffoutput"
            set diffresult [eval $cmd]
            if { $diffresult != 0 } {
                puts "\nError running $makediff: $diffresult"
                puts "Output:\n"
                puts "$diffoutput\n"
                if { [ file exists $difffile] } {
                    set fd [open $difffile "r"]
                    set diffoutput [read $fd]
                    close $fd
                    puts "$diffoutput\n"
                } 
            } else {
                putv "Diff Finished."
            }

            return $diffresult
        }
    }

    private method exec_test_upcvs { zopts show_only } {
        global makeupcvs

        putv "Executing $makeupcvs to update the test files FROM CVS."

        if { $show_only  == 0 } {
            set cvsresult 0
            set cmd "catch \{exec $makeupcvs\} cvsoutput"
            set cvsresult [eval $cmd]
            if { $cvsresult != 0 } {
                puts "\nError running $makeupcvs: $cvsresult"
                puts "Output:\n"
                puts "$cvsoutput\n"
                return
            } else {
                putv "Update CVS Finished."
            }
            return $cvsresult
        }
    }
    private method exec_test_upgold { zopts show_only } {
        global makeupgold

        putv "Executing $makeupgold to update the GOLD test files."

        if { $show_only  == 0 } {
            set goldresult 0
            set cmd "catch \{exec $makeupgold\} goldoutput"
            set goldresult [eval $cmd]
            if { $goldresult != 0 } {
                puts "\nError running $makeupgold: $goldresult"
                puts "Output:\n"
                puts "$goldoutput\n"
                return
            } else {
                putv "Update GOLD Finished."
            }
            return $goldresult
        }
    }


    public method execute { zopts ctype } {
        global show_only top
        set curdir [ pwd ]
        puts "\nRunning test [file join $top $comp $name]."
        cd $path
        if {$ctype == "exec" } {
            exec_test_clean $zopts $show_only 
            set res [exec_test_run $zopts $show_only]
            if { $res == 0 } {
                set res [exec_test_diff $zopts $show_only]
            } 
        } else {
            set cmd "exec_test_${ctype} $zopts $show_only"
            set res [eval $cmd]
        }
        puts "Finished test [file join $top $comp $name]."

        cd $curdir

        return $res
    }

}

