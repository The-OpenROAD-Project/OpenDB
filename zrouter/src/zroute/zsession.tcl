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

package provide ade::main 1.0
#
#       Class: ZSession
# Description: Session Controller for zroute
#      Author: Manolis M. Tsangaris
#
# The session holds all necessary state of a zroute session 
#
itcl::class ZSession {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable profile "default"
    public variable ns      "::"        ;# which namespace to run
    public variable evmgr   ""          ;# event manager
    # -------------------------------------------------- Public Methods ----
    public method ProfileSet {prof} {
        set cmd $PROFILE($prof)

        # expand the profile
        set cmd [subst -nocomm $cmd]

        # evaluate it
        namespace eval $ns $cmd
    }
    public method Register {var value} {
        set CONF($var) $value
    }
    public method Resolve {var} {
        if [info exists CONF($var)] {
            return $CONF($var)
        } else {
            return ""
        }
    }
    # -- make a note of a new tm object
    public method NewTM {tm} {
        # provide tm with its event manager
        $tm configure -evmgr $evmgr

        # keep a note for it
        lappend ZTMS $tm
    }

    # -- info
    public method Info {} {
        puts "Configuration Variables"
        foreach var [array names CONF] {
            puts "$var $CONF($var)"
        }
        puts "Technology Modules"
        foreach tm $ZTMS {
            puts "$tm"
        }
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable PROFILE
    protected variable CONF
    protected variable ZTMS ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        # initialize profiles
        profile_init

        # initalize technology modules
        zroute::ztm::init

        # configure the zsession static variable on all TM classes
        # just change the super class:
        set ZTM::zsession $this
        if { [itcl::find class ::ZTMSession] == "" } {
            warning ztm "ZTMSession: class not defined"
            warning ztm "there will be no tmodule access"
        } else {
            ProfileSet $profile
        }
    }
    destructor {
        foreach tm $ZTMS {
            $tm delete
        }
    }
    # -------------------------------------------------- Private Methods ---
    # load in the profiles
    protected method profile_init {} {
        array set PROFILE {
            default {
                ZTMSession zs
                DB         db
                LTimer     timer
            }
        }
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
