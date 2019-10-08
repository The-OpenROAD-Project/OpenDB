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
set name [file tail $argv0]
set name [string range $name 4 end]

if { $argc == 0 } {
    puts "Usage:"
    puts "$argv0 \[-options] \[input file]"
    puts "Valid options: -convert_par : convert ( ) into \[ ]"
    puts "               -old_magma   : use older magma syntax"
    exit
}

switch $name {
    magma - pt - psta { 
    }
    default {
        error "Conversion to $name format is not supported"
    }
}

    
set psta(milos_createCell) {puts "create_cell [arg_0] [arg_1]"}
set psta(milos_removeCell) {puts "remove_cell [arg_0]"}
set psta(milos_moveCell) {puts "## moveCell not supported in psta "}
set psta(milos_swapCell) {puts "swap_cell [arg_0] [arg_1]"}

set psta(milos_createNet) {puts "create_net [arg_0]"}
set psta(milos_removeNet) {puts "remove_net [arg_0]"}
set psta(milos_connectNet) { puts "connect_net [arg_3] [arg_0]/[arg_1]" }
set psta(milos_disconnectNet) { puts "disconnect_net [arg_3] [arg_0]/[arg_1]" }

## PT is almost the same as psta
array set pt [array get psta]
set pt(milos_moveCell) {puts "## moveCell not supported in pt"}
set pt(milos_swapCell) {puts "size_cell [arg_0] [arg_1]"}



## MAGMA is quite different from others..
set magma(milos_removeNet) {puts " data delete object \$m/net:[argc_0]"}
set magma(milos_removeCell) {puts " data delete object \$m/cell:[arg_0]"}

set magma(milos_swapCell) {puts " data attach \$m/cell:[arg_0] cell_model \[find_model [arg_1]]" }
set magma(milos_createCell) {puts " data create cell \[find_model [arg_1]] \$m [argc_0]"}
set magma(milos_createNet) {puts " data create net \$m [argc_0]"}

set magma(milos_connectNet) {puts " data attach \$m/net:[argc_3] net_pin \$m/[arg_0]/[arg_1] "}

## New magma
set magma(milos_moveCell) { puts " data move cell \$m/[arg_0] \"[expr [arg_4]/$::dbu]n [expr [arg_5]/$::dbu]n\" $::magma_orient([arg_2])" }; 
## Old magma

set magma(milos_disconnectNet) {puts " data detach \[data only pin_net \$m/[arg_0]/pin:[arg_1]] net_pin \$m/[arg_0]/[arg_1] "}

#set magma(milos_disconnectNet) {puts " data detach \$m/net:[arg_3] net_pin [arg_0]/pin:[arg_1] "}


set magma_orient(R0) "NORTH"
set magma_orient(R90) "WEST"
set magma_orient(R180) "SOUTH"
set magma_orient(R270) "EAST"
set magma_orient(MY) "FLIP_NORTH"
set magma_orient(MX) "FLIP_SOUTH"
set magma_orient(MYR90) "FLIP_EAST"
set magma_orient(MXR90) "FLIP_WEST"

## create arg procs
for { set i 0 } { $i < 10 } { incr i } {
    set cmd "proc arg_$i { } { return \[lindex \[lrange \[info level -1] 1 end] $i]}"
    ::eval $cmd
}

## create argc procs
for { set i 0 } { $i < 10 } { incr i } {
    set cmd "proc argc_$i { } { return \[pconv \[lindex \[lrange \[info level -1] 1 end] $i]]}"
    ::eval $cmd
}

proc pconv { arg } {
    global conv_par
    if { $conv_par == 1 } {
        regsub -all "\[(]" $arg "\[" arg
        regsub -all "\[)]" $arg "]" arg
    }
    return $arg
}


## create procs
proc create_procs { } {
    global $::name
    global magma_orient
    foreach {op body} [array get $::name] {
        set cmd "proc $op { args } { "
        append cmd "    $body "
        append cmd " }"
        ::eval $cmd
    }
}

## Escape the input
## put {} around each part of the line to preserve backslashes. 
proc escape_line { line } {
    
    set sl [string trim $line]

    if { [string index $sl 0] eq "#" } {
        puts $line
        return ""
    }
    if { $sl eq "" } {
        return ""
    }

    regsub -all " " $line "\} {" nl
    regsub "^" $nl "\{" nl
    regsub "$" $nl "}" nl
    return $nl
}

proc my_source { file } {
    set fp [open $file r]
    while { [gets $fp line] >= 0 } {
        set cmd [escape_line $line]
        eval $cmd
    }
}

set conv_par 0
set dbu 1
proc set_args { arg } {
    #puts "set_args $arg"
    switch [string range $arg 1 end] { 
        convert_par {
            set ::conv_par 1
        }
        old_magma {
            set ::magma(milos_moveCell) { puts " run plan move cell \$m/[arg_0] \"[arg_4]n [arg_5]n\" $::magma_orient([arg_2])" }; 
            create_procs
        }
        dbu_2000 { 
            set ::dbu 2
        }
    }
}

create_procs

foreach f $argv {
    if { [string index $f 0] == "-" } {
        set_args $f
    } else {
        my_source $f
    }
}
