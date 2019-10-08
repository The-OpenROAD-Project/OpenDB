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

#!/usr/bin/env tclsh
##
## Testing Tool module - parse.tcl
## Author: Mattias Hembruch

source globals.tcl
source util.tcl
source group.tcl
source create.tcl

if { [info var _Parse_true] != "" } {
    return
}
set _Parse_true 1


proc parse_env { } {
    global cmdsep
    if { [info exists ::env(TT_SEP)] } {
        set cmdsep $::env(TT_SEP)
        #puts "Using $cmdsep as command separator"
    }
}

## Pick up any command-line options
proc parse_options { args } {
    global show_only debug summary verbose top autocreate log zargs 

    set retlist {}
    set resettop 0
    set resetlog 0
    set resetzarg 0
    set resetzr 0
    set usage 0
    set twopart 0
    foreach argm $args { 
        ## Split for arguments from "make"
        set al [split $argm " "]
        foreach arg $al {

            if { $twopart == 0 } {

                if { [string range $arg 0 0] == "-" } {
                    if { $arg == "-autocreate" } {
                        set autocreate 1
                    } elseif { $arg == "-d" } {
                        incr debug
                        putd "Debug level: $debug"
                    } elseif { $arg == "-n" } {
                        puts "-n mode - Showing what would be done\n"
                        set show_only 1
                    } elseif { $arg == "-h" } {
                        set usage 1
                        break
                    } elseif { $arg == "-l" } {
                        set resetlog 1
                        set twopart 1
                    } elseif { $arg == "-s" } {
                        set summary 2
                        putd "Show Test Summary"
                    } elseif { $arg == "-t" } {
                        set resettop 1
                        set twopart 1
                    } elseif { $arg == "-u" } {
                        set resetzr 1
                        set twopart 1
                    } elseif { $arg == "-v" } {
                        set verbose 1
                        putd "Show Verbose Test Results"
                        if { $summary == 1 } {
                            set summary 0
                        }
                    } elseif { $arg == "-z" } {
                        set resetzarg 1
                        set twopart 1
                    } else {
                        puts "TT: Unknown command-line option: $arg"
                        exit -11
                    }
                } else {
                    lappend retlist $arg
                }
            } else {
                if { $resettop == 1 } {
                    set top [string trim $arg]
                    putd "Setting top to: $top"
                    set resettop 0
                } elseif { $resetlog == 1 } {
                    set log [string trim $arg]
                    set resetlog 0
                } elseif { $resetzr == 1 } {
                    set adepath [string trim $arg]
                    ::cfg set_zr $adepath
                    set resetzr 0
                } elseif { $resetzarg == 1 } {
                    set zargs [string trim $al]
                    set ::env(ZARGS) $zargs
                    set resetzarg 0
                    set twopart 0
                    break
                }
                set twopart 0
            }
        }
    }

    if { $resettop == 1 } {
        puts "-t command missing new top level directory"
    }
    if { $resetlog == 1 } {
        puts "-l command missing log file name"
    }
    if { $resetzarg == 1 } {
        puts "-z command missing ade arguments"
    }

    if { [file pathtype $log] == "relative" } {
        set log "$top/$log"
    }

    openlog 

    if { $usage == 1 } {
        usage
        exit
    }

    return $retlist
}

proc seg_test { arg } {
    global cmdsep

    set cmd_list [split $arg $cmdsep]
    set cmd [lindex $cmd_list 0]
    set cmdcomp [string toupper [lindex $cmd_list 1]]
    set cmdnum [lindex $cmd_list 2]
    if { [llength $cmd_list] > 3 } {
        if { ($cmdcomp == "CLEAN") 
          || ($cmdcomp == "DIFF")
          || ($cmdcomp == "RUN")
          || ($cmdcomp == "NEW")
          || ($cmdcomp == "UPCVS")
          || ($cmdcomp == "UPGOLD")
          || ($cmdcomp == "SHOW") } {
              ## possibly valid 4-segment command - shift over
              return 1
          } else {
            puts "Don't know how to process this [llength $cmd_list]-segment command: $arg"
            usage
            return -1
          }
    }
    return 0
}
## Parse occurrences of -
proc parse_minus { args } {
    global cmdsep

    set retlist {}

    foreach arg $args {
        if { [string first "-" $arg] == -1 } {
            lappend retlist $arg
            continue
        }
        set cmd_list [split $arg $cmdsep]

        set rc [seg_test $arg]
        putd "Rcm = $rc"
        if { $rc  == 1 } {
            set cmdbase "[lindex $cmd_list 0]${cmdsep}[lindex $cmd_list 1]${cmdsep}[lindex $cmd_list 2]"
            set comp [lindex $cmd_list 2]
            set cmdnum [lindex $cmd_list 3]
        } elseif { $rc == 0 } {
            set cmdbase "[lindex $cmd_list 0]${cmdsep}[lindex $cmd_list 1]"
            set comp [lindex $cmd_list 1]
            set cmdnum [lindex $cmd_list 2]
        } else {
            putd "Fatal!"
            exit -1
        }

        ## have the right package to expand now
        set nlist [split $cmdnum "-"]

        ## check for integers on both sides..
        if { [llength $nlist] != 2 } {
            puts "Unable to handle this range: $cmdnum"
            continue
        }

        putd "Checking for ranges"

        set left [lindex $nlist 0]
        set right [lindex $nlist 1]

        if { ([string is integer $left]) && ([string is integer $right]) } {
            for {set i $left} { $i <= $right } {incr i } {
                set ncmd "${cmdbase}_${i}"
                lappend retlist $ncmd
            }
            continue
        } else { 
            ## figure out non-integer ranges

            putd "Not pure integer range! $left/$right"
            ## try ending with integers on both sides

            set li ""
            set ri ""
            regexp {[0-9]*$} $left li
            regexp {[0-9]*$} $right ri 
            set ll [string length $li]
            set rl [string length $ri]

            putd "$li/$ri"
            if { ($ll > 0) && ($rl > 0) } {
                ## try to do something..

                putd "Trying it!"

                set sleft [string range $left 0 end-$ll]
                set sright [string range $right 0 end-$rl]

                if { $sleft == $sright } { 
                    ## same
                    for {set i $li} { $i <= $ri } {incr i } {
                        set ncmd "${cmdbase}${cmdsep}$sleft${i}"
                        lappend retlist $ncmd
                    }
                    continue
                }
            }


            ## 
            putd "Not mixed integer range - try groups ($left <=> $right)" 

            if { ( [string is alpha $left] == 1 ) && 
                 ( [string is alpha $right] == 1 ) &&
                 ( [string compare -nocase $left $right] != 1 ) } {

                 putd "HERE: $comp!"
                set gl [get_comp_groups $comp]
                foreach gp $gl {
                    putd "comparing $left, $gp, $right"
                    if { ([string compare -nocase $left $gp] != 1) &&
                         ([string compare -nocase $gp $right] != 1) } {

                        set ncmd "${cmdbase}${cmdsep}$gp"
                        lappend retlist $ncmd
                    }
                }
            }

        }

    }

    return $retlist
}


## Parse occurrences of ,
proc parse_comma { args } {
    global cmdsep
    ## break args up by commas into multiple commands

    set retlist {}
    foreach arg $args {
        set cmd_list [split $arg $cmdsep]
        set cmd [lindex $cmd_list 0]
        set cmdcomp [lindex $cmd_list 1]
        set cmdnum [lindex $cmd_list 2]


        set rc [seg_test $arg]
        putd "Rcc = $rc"
        if { $rc  == 1 } {
            putd "PC: shift!"
            set cmd "${cmd}${cmdsep}${cmdcomp}"
            set cmdcomp $cmdnum
            set cmdnum [lindex $cmd_list 3]
        } elseif { $rc == -1 } {
            putd "$arg is not a valid command!"
            exit 1
        }
        putd "PC: process!"

        if { [string first "," $arg] == -1 } {
            ## No splits necesary
            lappend retlist $arg
            continue 
        }
        putd "PC: process 2!"


        if { [string first "," [lindex $cmd_list 0]] != -1 } {
            ## Not sure how to handle a comma in the first line
            puts "Unsure how to handle \",\" in: $cmd of $arg"
            continue
        }

        ## Check for package splits
        lappend tmplist [list ${cmd}${cmdsep}${cmdcomp}] 
        if { [string first "," [lindex $cmd_list 0]] == -1 } {
            ## Have a package split
            set complist [split $cmdcomp ","]
            set tmplist { }
            foreach cp $complist {
                lappend tmplist "${cmd}${cmdsep}${cp}"
            }
        }
        putd "PC: process 4: $tmplist!"

        foreach num $tmplist {
            set numlist [split $cmdnum ","]
            if { [llength $numlist] > 0 } {
                foreach nl $numlist {
                    lappend retlist "${num}${cmdsep}${nl}" 
                }
            } else {
                lappend retlist $num 
            }
        }
        putd "PC: process 5: $retlist!"
    }

    return $retlist
}

## Check for certain invalid command forms and alert the user
proc parse_valid_command { args } {
    global ctype testfile ttcmds cmdsep

    set clist { }


    set curcmd ""
    foreach arg $args {
        set all 0
        set cmdlist [split $arg $cmdsep]

        set tcmd [lindex $cmdlist 0]

        if { [string toupper $tcmd] != "TEST" } {
            puts "Invalid command: $arg"
            usage
            exit 1
        }

        set cmd [string toupper [lindex $cmdlist 1]]

        foreach tc $ttcmds {
            set ta [string toupper $tc]
            if { $ta == $cmd } {
                set curcmd $tc
            }
        }
        if { $curcmd == "" } {
            set curcmd "exec"
        }

        putd "Resolved command: $curcmd"

        lappend clist $curcmd

        set cl [llength $cmdlist]
        for { set i 0 } {$i < $cl} { incr i} {
            set cmd [lindex $cmdlist $i]
            set cmdu [string toupper $cmd]
            set idx [string first "ALL" $cmdu]

            ## All IS part of the command
            if { $idx != -1 } {
                ## See if all is the LAST part of the command
                putd "AL: $i, $cl"

                ## See if all is combined with another option
                #  packages like wall are ok, just not one called all
                if { $cmdu != "ALL" } {
                    set brk ""
                    if { $idx == 0 } {
                        ## first part of the command
                        set brk [string range $cmd 3 3]
                    } else {
                        incr idx -1
                        set brk [string range $cmd $idx $idx]
                    }
                    if { ($brk == ",") } {
                        puts "Option \"ALL\" cannot be combined with any other option in command: $arg"
                        exit -5
                    }
                } else {
                    ## All only makes sense once
                    if { $curcmd == "new" } {
                        puts "Option all cannot be combined with option new"
                        exit -4
                    }
                    if { [expr $cl - 1] != $i } {
                        puts "Option \"ALL\" can only be specified as the last part of a command."
                        exit -4
                    }
                    incr all
                    putd "incr for: $cmdu ($all)"
                    if {$all > 1 } {
                        puts "Option \"ALL\" can only appear once in command: $arg"
                        exit -6
                    }
                }
            }
        }
    }

    set clist [luniq $clist]
    putd "MT: $clist"
    if { [llength $clist] > 1 } {
        puts "Cannot mix commands [join $clist /] in one run"
        exit -7
    } 

    set ctype $clist
    putd "Ctype set to: $ctype"
}

proc get_cmd_text { ctype } {
    global ttcmds ttdesc
    if { $ctype == "exec" } {
        return "executed"
    } else {
        set idx [lsearch $ttcmds $ctype]
        return [lindex $ttdesc $idx]
    }
}

proc parse { args } {
    global ctype testfile cmdsep

    set cmd_list ""
    set numproc 0

    set al { }
    foreach arg $args {
        lappend al [string trim $arg]
    }
    set args $al

    ## check for command-line options
    set args [eval parse_options $args]
    putd "Post-PA: $args"

    puts "\nTT Testing Tool at [exec date]\n"

    eval parse_valid_command $args

    ## get all comma splits
    set args [eval parse_comma $args]
    putd "Post-PC: $args"

    ## get number splits
    set args [eval parse_minus $args]
    putd "Post-PM: $args"

    ## Have commands with all commas and minuses expanded
    

    set retlist { }
    foreach arg $args {

        putd "Parsing command: $arg"
        set cmd_list [split $arg $cmdsep]

        # has to be a valid command at this point

        if { $ctype == "exec" } {
            set main "test"
            set comp [lindex $cmd_list 1]
            set tests [lindex $cmd_list 2]
            putd "Main command: $main"
            putd "Package:    $comp"
            putd "Tests to run: $tests"
        } else {
            set main "test"
            set comp [lindex $cmd_list 2]
            set tests [lindex $cmd_list 3]
            putd "Main command (subcommand): $main ($ctype)"
            putd "Package:    $comp"
            putd "Tests to run: $tests"
        }

        if { $ctype == "new" } {

            if { [string length $tests] > 0 } {
                putd "NT1:"
                new_test $comp $tests
            } else {
                putd "NT2:"
                new_test $comp
            }
            incr numproc
            continue
        }

        if { [string length $comp] == 0 } {
            ## an "all" command
            puts "Need a package or test group to operate on!"
            show_comp_list
            show_attr_list
            incr numproc
            continue
        }

        if { [string compare -nocase $comp "all" ] == 0 } {
            ## test_all - trumps everything else
            if { $ctype != "show" } {
                return [get_all_files $testfile]
            } else {
                show_test_list 
                incr numproc
                continue
            }
        }



        if { [check_comp $comp] == 0 } {
            ## See if the attribute-group exists
            set attrl [get_all_attrs]
            set match 0
            foreach attr $attrl {
                putd "Testing attr $attr"
                if { [string compare -nocase $comp $attr] == 0 } {
                    ## Found matching group
                    set match 1
                    if { $ctype == "show" } {
                        show_groupheader
                        puts "\nAll tests in group $attr"
                    }
                    putd "Unknown package matches attribute: $attr"
                    set fl [get_all_files_by_attr $attr]
                    putd "Matching files: $fl"
                    foreach rt $fl {
                        if { $ctype == "show" } {
                            #puts "Show: $attr-$rt"
                            show_group_test $attr $rt
                        } else {
                            lappend retlist $rt
                        }
                    }
                    if { $ctype == "show" } {
                        puts ""
                    }
                }
            }
            if { $match == 1 } {
                incr numproc
                continue
            }

            puts "Neither package $comp nor test group $comp exist.\n"
            show_comp_list
            show_attr_list
            incr numproc
            continue
        }


        if { ([string length $tests] == 0) ||
             ([string compare -nocase "ALL" $tests ] == 0) } {
            # No tests - get all available for package, just as if all
            putd "in Package Default: $comp"
            if { $ctype != "show" } {
                #set clist [read_group $comp]
                set clist [get_comp_files $comp $testfile]
                set retlist [concat $retlist $clist]
            } else {
                show_comp $comp
            }
            incr numproc
            continue
        }

        ## Check for package groups
        set cgroups [get_comp_groups $comp]
        putd "Groups for package $comp: $cgroups"
        set match 0
        foreach gp $cgroups {
            if { $tests == $gp } {
                set cglist [read_group $comp $gp]
                putd "Matched package group: $gp-$cglist"
                if { $ctype != "show" } {
                    set retlist [concat $retlist $cglist]
                } else {
                    show_comp_group $comp $gp
                }
                incr numproc
                set match 1
                break
            }
        }

        if { $match == 1 } {
            continue
        }


        ## Must be individual test
        putd "Individual test: $arg"
        ## use get_test_fname to try test prefix alternative
        set nfn [get_test_fname $comp $tests]
        if { [string length $nfn] > 0 } {
            if { $ctype != "show" } {
                lappend retlist [get_test_fname $comp $tests]
            } else {
                show_header
                show_comp_header $comp
                show_comp_test $comp $nfn
            }
            incr numproc
        }

    }

    if {$numproc == 0} { 
        usage 
        return
    }

    set retlist [luniq $retlist]
    return $retlist
}

