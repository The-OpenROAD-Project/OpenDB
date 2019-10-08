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

#
# provide applications
# 
package provide ade::main 1.0

#
# User Application Handling
#
# This file provides all functionality to support the reduced user
# access to ade internals, and only expose a particular application
#

namespace eval apps  {
    # Initialize application, if any
    # Create a slave interpeter, and initialize the application in there
    variable VERSION "";# the version of the app loaded
    variable APPNAME "";

    proc init {argv0 args} {
        variable VERSION
        variable APPNAME

        ## initialize to ADE settings
        set APPNAME [set ::zroute::APPNAME]
        set VERSION [set ::zroute::VERSION]

        set argv0 [file tail $argv0]
        if { $argv0 == "ade" } { return "" }
        if { $argv0 == "zroute" } { return "" }
        if { [info command ${argv0}::init] == "" } { 
            error "no application $argv0"
        }

        ## reset to app settings
        set APPNAME [set ${argv0}::APPNAME]
        set VERSION [set ${argv0}::VERSION]

        set ai [interp create -safe]
        set m [${argv0}::init $ai $args]
        if { $m == "" } {
            set VERSION [set ${argv0}::VERSION]
            return $ai
        }
        interp delete $ai
        error "$argv0: failed to initialize application:\n$m"
    }

    # -- return a banner to print
    proc banner {} {
        variable APPNAME
        variable VERSION 
        if { [info procs ${APPNAME}::banner] != "" } {
            ${APPNAME}::banner
        } elseif { $VERSION != "" } {
            return "\n$VERSION"
        }
    }

    proc rx { args } {
        package require ade::milos::rte 0.1
        set cmd "RTE::rnode $args"
        eval $cmd
    }

    proc rexec { dir name script log log2 sdf2 } {
        cd $dir
        puts "REXEC! [pwd] [exec hostname] ($name $script $log)"
        set ::env(MSHCONFIG) "./msh.conf"
        set cmd "exec $name $script > $log"
        eval $cmd
        exec cp $log $log2
        if { [file exists celtic.sdf] == 0 } {
            error "Celtic.sdf not found! Celtic did not run!"
        } else {
            exec cp celtic.sdf $sdf2
        }
    }
}
