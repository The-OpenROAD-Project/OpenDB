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

## Testing Tool module - util.tcl
## Author: Mattias Hembruch


if { [info var _Util_true] != "" } {
    return
}
set _Util_true 1

proc usage { } {
    global ttcmds
    puts "Help for the Testing Tool (tt)"
    puts "Input: \[test_selection | option] \[test_selection | option] ... "
    puts ""
    puts "Options:"
    puts "-d		Debug mode - show lots of extra (not always useful) info"
    puts "-h		This help message and then exit"
    puts "-l <file>	copy output to <file> instead of tt.log"
    puts "-n		Show Only mode - show what would be done without doing it"
    puts "-s		Show only a summary of the results"
    puts "-t <dir>	Redefine the \"top\" level directory where tests are located"
    puts "-u <path>	use this ade instead of the one in PATH"
    puts "-v		Verbose mode - show more information"
    puts "-z <args>	Specify arguments to ade"
    puts ""
    puts "Test selection format:"
    puts ""
    puts "test_all	Execute all tests in the system"
    puts ""
    puts "General Form:"
    puts "test\[_cmd]_<package_list>_\[test_range]"
    puts ""
    puts "cmd is one of: [join $ttcmds]"
    puts "    only ONE cmd is allowed, if omitted, _clean _run _diff are executed in order"
    puts ""
    puts "package: a top-level package or a testing group to test. "
    puts "    multiple packages/groups can be separated by commas"
    puts ""
    puts "test_to_run: which specific test(s) to execute."
    puts "    tests can be separated by commas, or a range specified with -"
    puts ""
    puts "Examples:"
    puts "test_show		Show all packages and groups in the system"
    puts "test_show_all		Show all tests in the system"
    puts "test_diff_abc,def	Diff tests in packages abc and def"
    puts "test_clean_abc_1-5	Clean tests 1-5 of package abc"
    puts "test_sanity,build	Run tests in groups sanity and build"
    puts ""
}

## Rename puts to allow for logging!
rename puts tcl::puts
rename exit tcl::exit
set logfd 0
proc puts {args } {
    if { [lindex $args 0] == "-nonewline" } {
        set cmd "tcl::puts -nonewline stdout [concat [lrange $args 1 end]]"
    } else {
        set cmd "tcl::puts stdout [concat $args]"
    }

    if {[catch {eval $cmd} txt]} {
        exit
    }

    if { $::logfd != 0 } {
        if { [lindex $args 0] == "-nonewline" } {
            set cmd "tcl::puts -nonewline $::logfd [concat [lrange $args 1 end]]"
        } else {
            set cmd "tcl::puts $::logfd [concat $args]"
        }
        if {[catch {eval $cmd}]} {
            exit
        }
        flush $::logfd
    }
}

set log "tt.log"
proc openlog { } {
    global logfd log
    set logfd [open $log "w"]
}

#proc reopenlog { } {
    #global logfd log
    #set logfd [open $log {WRONLY CREAT APPEND}]
#}

proc closelog { } {
    global logfd
    if { $logfd != 0 } {
        catch { close $logfd }
        set logfd 0
    }
}

proc exit { {rc 0} } {
    global logfd
    closelog
    tcl::exit $rc
}

## Return the unique elements of a list
#
proc luniq {myList} {
  set result [list]
  foreach elem $myList {
    if {![info exists temp($elem)]} {
      lappend result $elem
      set temp($elem) 1
    }
  }
  return $result
} 

## For debugging
set debug 0
proc putd { args } {
    global debug
    set ll [llength $args]
    if { $ll == 2 } {
        if { $debug >= [lindex $args 1] } {
            set arg [lindex $args 0]
            puts $arg
        } 
    } elseif { $ll == 1 } {
        if { $debug } {
            eval puts $args
        }
    }
}

## For verbose
proc putv { args } {
    global verbose
    set ll [llength $args]
    if { $verbose > 0 } {
        eval puts $args
    }
}
