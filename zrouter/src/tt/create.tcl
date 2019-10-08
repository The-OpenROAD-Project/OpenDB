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

## Testing Tool module - create.tcl
### Author: Mattias Hembruch
#
# procedures used to create a new test

if { [info var _Create_true] != "" } {
    return
}
set _Create_true 1

proc get_single_input { name elist {default ""} } {
    global prompt

    set ell [llength $elist]

    set nvalue ""
    while { $nvalue == "" } {
        puts "Enter the $name for the test(s)."
        if { $default != "" } {
            puts "Press <ENTER> to accept the default value: \[$default]"
        }
        puts -nonewline "$prompt"
        flush stdout
        set ln [gets stdin line]
        set line [string trim $line]
        if { [string length $line] == 0 } { 
            return $default
        }

        if { ($line == "?") && ($ell > 0)} {
            puts "${name}s defined in other tests (enter the number to pick that $name): "
            set i 1
            foreach cp $elist {
                puts "$i. $cp"
                incr i
            }
            set nvalue ""
        } elseif { $line == "exit" } {
            exit -1
        } else {
            if { [string is integer $line] } {
                if { ($line >0) && ($line <= $ell) } {
                    incr line  -1
                    set nvalue [lindex $elist $line]
                } else {
                    set nvalue $line
                }
            } else {
                set nvalue $line
            }
        }
    }
    putd "New test $name: $nvalue"

    return $nvalue
}

proc get_form_input { name default} {
    global prompt

    set nvalue ""
    puts "Enter one or more ${name}(s) on separate lines. Enter . to finish."
    if { $default != "" } {
        puts "Press <ENTER> to accept the default value: \[$default]"
    }
    while { 1 } {
        puts -nonewline "$prompt"
        flush stdout
        set ln [gets stdin line]
        set line [string trim $line]
        if { [string length $line] == 0 } { 
            if { $nvalue == "" } {
                return $default
            }
        }

        if { $line == "." } {
            return $nvalue
        } else {
            lappend nvalue $line
            continue
        }
    }
    putd "New test $name: $nvalue"

    return $nvalue
}

proc get_multi_input { name limit elist edescr} {
    global prompt $edescr

    set ell [llength $elist]

    set nvalue ""
    puts "Enter 1 or more ${name}(s) separated by commas. "
    puts "Enter ? to show a list of ${name}(s), . to finish, -1 to reset, exit to quit."
    while { $nvalue == "" } {
        puts -nonewline "$prompt"
        flush stdout
        set ln [gets stdin line]
        set line [string trim $line]
        if { [string length $ln] == 0 } { 
            continue 
        }

        if { ($line == "?") && ($ell > 0)} {
            puts "${name}s defined in other tests (enter the number to pick that $name): "
            set i 1
            foreach cp $elist {
                set cmd "set ex \[info exists ${edescr}(\$cp)\]"
                eval $cmd
                if { $ex } {
                    set cmd "puts \"$i. $cp: 	\$${edescr}(\$cp)\""
                    eval $cmd
                } else {
                    puts "$i. $cp"
                }
                incr i
            }
            set nvalue ""
        } elseif { $line == "exit" } {
            exit -1
        } elseif { $line == "." } {
            return ""
        } else {
            set linel [split $line ","]

            if { ($limit == 2) && ($line == "-1") } {
                return "-1"
            }

            set nvalue {}
            foreach ln $linel {
                if { [string is integer $ln] } {
                    if { ($ln >0) && ($ln <= $ell) } {
                        incr ln  -1
                        lappend nvalue [lindex $elist $ln]
                    } else {
                        lappend nvalue $ln
                    }
                } else {
                    lappend nvalue [string trim $ln]
                }
            }
        }
    }
    putd "New test $name: $nvalue"

    return $nvalue
}

proc new_test { comp {num 1} } {
    global fdescr fplatform fruntime foption tcl_platform autocreate fzargs
    global top

    if { $autocreate == 0 } {
        puts "New Test creation!"
        puts "Please enter data for the new test to be created!"
        puts "At any prompt enter ? for a list of values (if applicable), enter exit to quit."
    }

    if { [string length $comp] > 0 } {
        set ncomp $comp
    } else {
        set ncomp [get_single_input "Package" [get_all_comps]]
    }
    puts "\nPackage selected: $ncomp\n"


    set st 1
    while { 1 } {
        if { [get_test_fname $ncomp $st] != "" } {
            incr st
        } else {
            break
        }
    }

    set def "source test.tcl"

    ## Create the test automatically - assume current directory
    if { $autocreate == 1 } {
        set nattrl { }
        set zcmds [list $def]
        set infol { }
        set tl [file split $top]
        set cname [lrange $tl end end]
        lappend infol "#$fdescr Test #$st of package $cname"
        lappend infol "#$fplatform $tcl_platform(platform)"
        lappend infol "#$foption none"
        lappend infol "#$fzargs none"
        set stn [file join $ncomp ]
        ## Only Create 1 test
        create_makefile $stn
        create_attr_file  $stn $infol
        create_exec_file $stn $zcmds
        puts "Successfully created test!"
        return
    } 


    set aal [get_all_attrs]
    set nattrl { }
    while { 1 } {
        set nattr [get_multi_input "Test Group" 2 [get_all_attrs] tgdescr]
        if { $nattr == "" } { break }
        if { $nattr == "-1" } { 
            set nattrl { } 
        } else {
            foreach nn $nattr {
                if { $nn != "" } {
                    lappend nattrl $nn
                }
            }
            set nattrl [luniq $nattrl]
            set nattrl [lsort $nattrl]
        }
        puts "\nCurrently defined Test Groups: "
        foreach at $nattrl { 
            puts "$at"
        }
        puts "\n"
    }
    puts "Test Groups selected: $nattrl\n"

    set zcmds [get_form_input "Ade commands" $def]
    if { $zcmds == $def } {
        set zcmds [list $def]
    }
    puts "Ade commands: $zcmds"
    puts "\n"

    set infol {}
    set value [get_single_input "Description" "" "Test #$st of package $ncomp"]
    if { $value != "" } {
        lappend infol "#$fdescr $value"
    }
    set value [get_single_input "Platform" "" $tcl_platform(platform)]
    if { $value != "" } {
        lappend infol "#$fplatform $value"
    }
    set value [get_single_input "Runtime (in seconds)" ""]
    if { $value != "" } {
        lappend infol "#$fruntime $value"
    }
    set value [get_single_input "Required Ade Compiled-in Options" "" "none"]
    if { $value != "" } {
        lappend infol "#$foption $value"
    }

    set value [get_single_input "Required Ade Startup Options" "" "none"]
    if { $value != "" } {
        lappend infol "#$fzargs $value"
    }
    puts "Ready to create $num new tests."
    if { [check_comp $ncomp] != 1 } {
        puts "Warning: Package $ncomp does not exist - it will be created."
    }

    putd "infol: $infol"
    foreach val $nattrl {
        lappend infol $val
    }
    set nattrl $infol

    
    for { set i 0 } { $i < $num } { incr i } {
        set stn [file join $ncomp "test$st"]
        puts "Creating new test $stn. Ok (y/n)?"
        set ln [gets stdin line]
        set line [string toupper $line]
        if { $line == "Y" } {
            ## Create test
            create_test_dir $stn
            create_makefile $stn
            create_attr_file  $stn $nattrl
            create_exec_file $stn $zcmds
            puts "Successfully created test $stn!"
        } else {
            exit -5
        }
        incr st
    }
}

proc create_makefile { tdir } {
    global makefile top makeinc
    set curd [pwd]
    set spath [file join $top $tdir]
    set mlev ""
    set curp [pwd]
    cd $spath
    while { 1 } {
        if { [file exists $makeinc] } {
            break
        } else {
            set mlev "../$mlev"
            cd ..
            if { [pwd] == "/" } {
                puts "Cannot find $makeinc in current directory tree - cannot proceed!"
                exit -6
            }
        }
    }
    cd $curp
    set fname [file join $top $tdir $makefile]
    set incfile "-include ${mlev}$makeinc"
    putd "test: [pwd] $spath $tdir $top $makefile"
    create_test_file $fname [list $incfile ""]
}

proc create_attr_file { tdir attrl } {
    global top attrfile
    set fname [file join $top $tdir $attrfile]
    create_test_file $fname $attrl
}

proc create_exec_file { tdir cmds } {
    global top testfile zprefix zcmd zpostfix
    set fname [file join $top $tdir $testfile]
    set fd [open $fname "w"]
    tcl::puts $fd "$zprefix"
    tcl::puts $fd "$zcmd"
    foreach c $cmds {
        tcl::puts $fd "        $c"
    }
    tcl::puts $fd "$zpostfix"
    close $fd
    file attributes $fname -permissions "ugo+x"
}


proc create_test_file { fname fcontents } {
    set fd [open $fname "w"]
    foreach c $fcontents {
        tcl::puts $fd $c
    }
    close $fd
}

proc create_test_dir { tdir } {
    global top

    set fn [file join $top $tdir]
    file mkdir $fn
}
