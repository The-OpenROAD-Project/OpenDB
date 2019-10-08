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

## Testing Tool - module group.tcl
## Author: Mattias Hembruch

source util.tcl
source globals.tcl

if { [info var _Group_true] != "" } {
    return
}
set _Group_true 1

proc readfile { fname } {
    set fd [open $fname "r"]
    set fcont [read $fd]
    close $fd
    return $fcont
}

proc get_comp_groups { comp } {
    global top groupfile testfile
    set gpath [file join $top $comp]
    set gname [file join $gpath $groupfile]

    set retlist { }
    if { [file exists $gname ] } {
        set glist [readfile $gname]

        set tests [split $glist "\n"]
        foreach i $tests {
            set i [string trim $i]
            if { [string length $i] == 0 } {
                continue
            }
            if { [string index $i end] == ":" } {
                set i [string range $i 0 [expr [string length $i] - 2] ]
                lappend retlist $i
            }
        }
    }
    return $retlist
}



proc read_group { comp args } {
    global top groupfile testfile

    set gpath [file join $top $comp]
    set gname [file join $gpath $groupfile]
    set comptests(0,0) 0
    set groups {}
    if { [file exists $gname ] } {
        set glist [readfile $gname]

        set tests [split $glist "\n"]

        set cnt 0
        set curg ""
        foreach i $tests {
            incr cnt
            set i [string trim $i]
            if { [string length $i] == 0 } {
                continue
            }
            putd "Test: $i"

            if { [string index $i end] == ":" } {
                set i [string range $i 0 [expr [string length $i] - 2] ]
                putd "Group: $i"
                ## New group declaration
                if { [info exists comptests($i,0) ] } {
                    ## Duplicate group
                    puts "Duplicate definition of group $i on line $cnt of file $gname"
                } else {
                    putd "New Group definition: $i"
                    set curg $i
                    set comptests($curg,0) 0
                    set comptests($curg,1) {}
                    lappend groups $i
                }
            } elseif { [string index $i 0] == ":" } {
                ## Group reference - copy members
                set i [string range $i 1 end]
                putd "Reference to group: $i"
                if { [info exists comptests($i,0)] } {
                    foreach j $comptests($i,1) {
                        lappend comptests($curg,1) $j
                    }
                } else {
                    puts "Cannot find referenced group $i at line $cnt in file $gname"
                    continue
                }

            } else {
                ## Must be a new member

                set tf1 [file join $gpath $i]
                set tf2 [file join $gpath "test$i"]
                set tf ""
                if { [file exists $tf1 ] } {
                    set tf $tf1
                } elseif { [file exists $tf2 ] } {
                    set tf $tf2
                } else {
                    puts "Unable to resolve reference to test directory $i/test$i on line $cnt of file $gname"
                    continue
                }
                set tname [file join $tf $testfile]
                if { [file exists $tname] } {
                    putd "Adding test: $tname"
                    lappend comptests($curg,1) $tname
                } else {
                    putd "Test $tf: $testfile not found!"
                }
            }

        }
    }
    #parray comptests
    foreach i $groups {
        set comptests($i,1) [luniq $comptests($i,1)]
    }

    if { [llength $args] == 0 } {
        set args $groups
    }

    set retlist { }
    foreach arg $args {
        foreach t $comptests($arg,1) {
            lappend retlist $t
        }
    }
    set retlist [luniq $retlist]
    if { [llength $retlist] > 0 } {
        return $retlist
    } else {
        return ""
    }

}

proc check_comp { comp } {
    global top
    if { [string length $comp] == 0 } {
        return 0
    }
    set cname [file join $top $comp]
    set rc [file exists $cname]
    putd "CC: $rc ($cname)"
    return $rc
}

proc create_comp { comp } {
    global top
    if { $comp == "" } {
        return 
    }
    set cname [file join $top $comp]
    set rc [file mkdir $cname]
}


proc get_test_attrib { fname attrib } {
    global attrfile
    set fp [get_test_path $fname]
    set fa [file join $fp $attrfile]
    set rc [catch {exec grep -i $attrib $fa} msg]
    if { $rc == 0 } {
        set idx [expr [string first $attrib $msg] + [string length $attrib] +1 ]
        return [string trim [string range $msg $idx end]]
    } else {
        return ""
    }
}

proc get_test_descr { fname } {
    global fdescr
    return [get_test_attrib $fname $fdescr]
}

proc get_test_platform { fname } {
    global fplatform
    return [get_test_attrib $fname $fplatform]
}

proc get_test_runtime { fname } {
    global fruntime
    return [get_test_attrib $fname $fruntime]
}

proc get_test_options { fname } {
    global foption
    return [get_test_attrib $fname $foption]
}

proc get_test_zargs { fname } {
    global fzargs
    set rt [get_test_attrib $fname $fzargs]
    if { $rt == "none" } {
        return ""
    } else {
        return $rt
    }
}




proc get_test_comp { fname } {
    global top testfile 
    set t1 [expr [string length $top ] + 1]
    set t2 [string range $fname $t1 end]
    set tl [file split $t2]
    return [lindex $tl 0]
}

proc get_test_name { comp fname } {
    global top testfile 
    set t1 [expr [string length [file join $top $comp]] + 1]
    set t2 [string length $testfile]
    set t3 [expr [string length $fname] - $t2 -2]
    return [string range $fname $t1 $t3]
}

proc get_test_path { fname } {
    global testfile
    set t2 [expr [string length $testfile] + 1]
    return [string range $fname 0 end-$t2]
}

proc get_test_file { fname } {
    set fl [file split $fname]

    set ll [expr [llength $fl] - 1]
    return [lindex $fl $ll]
}

set comp_shown 0
set last_comp ""
proc show_comp_header { comp } {
    global comp_shown last_comp
    if { ($comp_shown == 0) || ($comp != $last_comp) } {
        puts "\nPackage $comp Test List"
        set last_comp $comp
        set comp_shown 1
    }
}

set header_shown 0
proc show_header { } {
    global header_shown
    if { $header_shown == 0 } {
        puts ""
        puts "Test Name           Description"
        set header_shown 1
    }
}

set gheader_shown 0
proc show_groupheader { } {
    global gheader_shown
    if { $gheader_shown == 0 } {
        puts ""
        puts "Test Name                               Description"
        set gheader_shown 1
    }
}

proc format_test { comp fname } {
    set fs [format "%-20s%-60s" [get_test_name $comp $fname] [get_test_descr $fname] ]
    return $fs
}

proc show_group_test { group fname } {
    global verbose
    show_groupheader
    puts [format "%-40s%-40s" [get_test_path $fname] [get_test_descr $fname] ]
    if { $verbose > 0 } {
        set platform [get_test_platform $fname]
        if { [string length $platform] > 0 } {
            puts [format %-40s%-40s "" "Platform: $platform"]
        }
        set runtime [get_test_runtime $fname]
        if { [string length $runtime] > 0 } {
            puts [format %-40s%-40s "" "Runtime: $runtime"]
        }
        set option [get_test_options $fname]
        if { [string length $option] > 0 } {
            puts [format %-40s%-40s "" "Option: $option"]
        }
        set zarg [get_test_zargs $fname]
        if { [string length $zarg] > 0 } {
            puts [format %-40s%-40s "" "Ade Arguments: $zarg"]
        }
    }
}

proc show_comp_test { comp fname } {
    global verbose
    show_header
    puts [format_test $comp $fname]
    if { $verbose > 0 } {
        set platform [get_test_platform $fname]
        if { [string length $platform] > 0 } {
            puts [format %-20s%-60s "" "Platform: $platform"]
        }
        set runtime [get_test_runtime $fname]
        if { [string length $runtime] > 0 } {
            puts [format %-20s%-60s "" "Runtime: $runtime"]
        }
        set option [get_test_options $fname]
        if { [string length $option] > 0 } {
            puts [format %-20s%-60s "" "Option: $option"]
        }
        set zarg [get_test_zargs $fname]
        if { [string length $zarg] > 0 } {
            puts [format %-20s%-60s "" "Ade Arguments: $zarg"]
        }
    }
}

proc show_attr_list { } {
    set cl [get_all_attrs]
    puts "Available test groups:"
    foreach cp $cl {
        puts "$cp"
    }
    puts ""
}
proc show_comp_list { } {
    set cl [get_all_comps]
    puts "Available Packages:" 
    foreach cp $cl {
        puts "$cp"
    }
    puts ""
}

proc show_comp_tests { comp } {
    global testfile
    show_header 
    foreach j [get_comp_files $comp $testfile] {
        show_comp_test $comp $j
    }
}

proc show_comp_group { comp gp } {
    show_header
    puts "Group $gp:"
    foreach j [read_group $comp $gp] {
        show_comp_test $comp $j
    }
}
proc show_comp { comp } {
    global groups top testfile

    set groups [get_comp_groups $comp]

    show_header
    show_comp_header $comp
    foreach gp $groups {
        show_comp_group $comp $gp
        puts ""
    }

    puts "Individual Tests:"
    show_comp_tests $comp
    puts "\n"
}

proc get_test_fname { comp name } {
    global top testfile
    set fname [file join $top $comp $name $testfile]
    putd "GTF: Trying file: $fname"
    if { [file exists $fname] } {
        putd "GTF: found"
        return $fname
    }

    ## Try "test+name"
    set n2 "test$name"
    set fname [file join $top $comp $n2 $testfile]
    putd "GTF: Trying file: $fname"
    if { [file exists $fname] } {
        putd "GTF: found"
        return $fname
    }

    putd "GTF: NOT found"
    return ""
}

proc get_comp_files { comp filepatt } {
    global top find

    set tdir [file join $top $comp]
    set rc [catch { set slist [exec $find $tdir -name $filepatt -print -follow] } msg ]

    if { $rc == 0 } {
        return [lsort $slist]
    } else {
        ## Had an error, get partial info
        putd "error: $msg"
        set sl [split $msg "\n"]
        set rl { }
        foreach tn $sl {
            if { [string first "No such file or directory" $tn] < 0 } {
                lappend rl $tn
            }
        }
        return $rl
    }
}


proc get_all_files { filepatt } {
    global top find

    set rc [catch { set slist [exec $find $top -name $filepatt -print -follow] } msg ]

    if { $rc == 0 } {
        return [lsort $slist]
    } else {
        putd "error: $msg"
        set sl [split $msg "\n"]
        set rl { }
        foreach tn $sl {
            if { [string first "No such file or directory" $tn] < 0 } {
                lappend rl $tn
            }
        }
        return $rl
    }
}

proc get_all_files_by_attr { attr } {
    global attrfile testfile

    set attrlist [get_all_files $attrfile]
    set atfl { }
    foreach attrf $attrlist {
        putd "Testing file: $attrf"
        set atl [read_attrib_file $attrf]
        foreach at $atl {
            if { [string compare -nocase $attr $at] == 0 } {
                set tn [expr [string length $testfile] + 1]
                set fp [string range $attrf 0 end-$tn]
                set fp "${fp}$testfile"
                lappend atfl $fp
            }
        }
    }
    return $atfl
}

proc read_attrib_file { fname } {

    set atl {}

    set acont [readfile $fname]
    set contlist [split $acont "\n"]
    foreach ct $contlist {
        set ct [string trim $ct]
        if { $ct != "" } {
            if { [string index $ct 0] != "#" } {
                lappend atl $ct
            }
        }
    }
    return $atl
}


proc get_all_attrs { } {
    global attrfile

    set attrlist [get_all_files $attrfile]
    set atl { }
    putd "Test: $atl"
    foreach attrf $attrlist {
        set acont [read_attrib_file $attrf]
        foreach ac $acont {
            lappend atl $ac
        }
    }
    putd "Test: $atl"
    set atl [luniq $atl]
    set atl [lsort $atl]

    return $atl
}

proc get_all_comps { } {
    global top testfile
    set tl [get_all_files $testfile]
    putd "GAC: $tl"
    set clist {}
    set tlen [expr [string length $top] ]
    if { [string index $top end ] != "/" } {
        incr tlen
    }
    set flen [expr [string length $testfile] + 1]
    foreach t $tl {
        set cn [string range $t $tlen end-$flen]
        set cl [file split $cn]
        putd "tt: [lindex $cl 0]"
        lappend clist [lindex $cl 0]
    }
    set clist [luniq $clist]

    return $clist
}
    
proc show_test_list { } {
    set cl [get_all_comps]
    foreach comp $cl {
        show_comp $comp
    }
}

