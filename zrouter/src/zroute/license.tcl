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

package provide ade::license 1.0

#       Class: ADELicense
# Description: ADE specific licensing policy
#      Author: Manolis M. Tsangaris
#
itcl::class ADELicense {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable envlicense "LM_LICENSE_FILE:ADS_LICENSE"  
						;# license server env
    public variable server      ""              ;# license server
    public variable VERSION       1.0           ;# license file version
    public variable DEFAULT_SERVER 6150@tatu1
    # -------------------------------------------------- Public Methods ----
    public method Init {rr} {
	upvar $rr R
	set rc [lmgr_init R]
	if { $rc != 0 } { return $rc }
	if { [$lmgr Features] == "" } { 
	    set R(msg) "license server '$server' has no features"
	    return 1
	}
	return 0
    }
    public method Checkout {feature rr} {
	upvar $rr R
	lmgr_checkout $feature R
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable lmgr ""
    protected variable LICENSE_MAP ""           ;# license mapping
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	map_licenses
    }
    destructor {
	if { $lmgr != "" } { $lmgr delete }
    }
    # -------------------------------------------------- Private Methods ---
    protected method lmgr_init {rr} {
        upvar $rr R
        if { $server == "" } {
            global env
            foreach var [split $envlicense :] {
                if [info exists env($var)] { break }
            }
            if ![info exists env($var)] {
                if { $DEFAULT_SERVER != "" } {
                    set env($var) $DEFAULT_SERVER
                    puts stderr "No license server specified"
                    puts stderr "Using $DEFAULT_SERVER (Athena Design only)"
                } else {
                    set R(msg) "no license set or env variable ($envlicense)"
                    return 1
                }
            }
            set server $env($var)
        }
        set lmgr [LLicenseManagerFlexLM $this.lm]
        $lmgr Init $server R
    }
    # ----------------------------------------------- Interface Implementation
    protected method lmgr_checkout {feature rr} {
	upvar $rr R
	foreach {feat lfeat type} $LICENSE_MAP {
	    if { $feat == $feature} {
            if { [info command ::zr.zlog] != "" } {
                debug LIC:M "Found feature $feat: $lfeat $type"
            }
            return [lmgr_checkout1 $feature $lfeat $type R]
	    }
	}
    if { [info command ::zr.zlog] != "" } {
        debug LIC:M "Unsupported feature $feature!"
    }
	set R(msg) "checkout: unsupported feature $feature"
	return 1
    }
    protected method lmgr_checkout1 {feature lfeat type rr} {
    if { [info command ::zr.zlog] != "" } {
        debug LIC:M "Checkout feature $feature: $lfeat $type!"
    }
	upvar $rr R
	switch $type {
	    count {
		set rc [$lmgr Acquire $lfeat $VERSION]
		if { $rc < 0 } {
            if { [info command ::zr.zlog] != "" } {
                debug LIC:M "Checkout $feature:$lfeat no more licenses!"
            }
		    set R(msg) "checkout $feature:$lfeat: no more licenses"
		    return 2
		}
        if { [info command ::zr.zlog] != "" } {
            debug LIC:M "Checkout $feature:$lfeat checkout ok!"
        }
		set R(msg) "checkout $feature: checked out ok"
		set R(lid) $rc
		return 0
	    }
	    exists {
		set r [$lmgr FeatureInfo $lfeat]
		if { $r == "" } {
            if { [info command ::zr.zlog] != "" } {
                debug LIC:M "Checkout $feature:$lfeat feature not available!"
            }
		    set R(msg) "checkout $feature:$lfeat: feature not available"
		    return 1
		}
        if { [info command ::zr.zlog] != "" } {
            debug LIC:M "Checkout $feature:$lfeat checkout ok!"
        }
		set R(msg) "checkout $feature: checked out ok"
		return 0
	    }
	    param {
		# -- scan all licenses for the parameter, specified in lfeat
		foreach feat [$lmgr Features] {
		    if { [scan $lfeat param] == 1 } {
			set R(param) $param
			set R(msg) "checkout $feature: checked out ok"
            if { [info command ::zr.zlog] != "" } {
                debug LIC:M "Checkout $feature:$lfeat checkout ok!"
            }
			return 0
		    }
		}
        if { [info command ::zr.zlog] != "" } {
            debug LIC:M "Checkout $feature:$lfeat feature not available!"
        }
		set R(msg) "checkout $feature: feature not available"
		return 2
	    }
	    value {
		# -- the parameter is equal to the license count
		set r [$lmgr FeatureInfo $lfeat]
		if { $r == "" } {
		    set R(msg) "checkout $feature:$lfeat: feature not available"
            if { [info command ::zr.zlog] != "" } {
                debug LIC:M "Checkout $feature:$lfeat feature not available!"
            }
		    return 1
		}
		set F(max) 0
		array set F $r
		set R(param) $F(max)
		set R(msg) "checkout $feature: checked out ok ($F(max))"
        if { [info command ::zr.zlog] != "" } {
            debug LIC:M "Checkout $feature:$lfeat checkout ok ($F(max))!"
        }
		return 0
	    }
	}
    if { [info command ::zr.zlog] != "" } {
        debug LIC:M "Checkout $feature:$lfeat invalid type"
    }
	set R(msg) "checkout $feature: $type: illegal type"
	return 2
    }

    protected method map_licenses {} {
	set LICENSE_MAP {
	    ade.master ADE_Master            count
	    ade.mpl    ADE_Multi_Processing  value
            ade.tools  ADE_Tools             exists
            ade.debug  ADE_Debug             exists
            ade.slave  ADE_Tools             exists
            ade.ui     ADE_Tools:TurboPro_Tools  exists

	    turbopro.master Milos_Master            count
	    turbopro.mpl    Milos_Multi_Processing  value
            turbopro.tools  Milos_Tools             exists
            turbopro.debug  Milos_Debug             exists
            turbopro.slave  Milos_Tools             exists

	    athext.master Milos_Master            count
	    athext.mpl    Milos_Multi_Processing  value
            athext.tools  Milos_Tools             exists
            athext.debug  Milos_Debug             exists

	    milos.master Milos_Master            count
	    milos.mpl    Milos_Multi_Processing  value
            milos.tools  Milos_Tools             exists
            milos.debug  Milos_Debug             exists
            milos.slave  Milos_Tools             exists


	    turborc.master TurboRC_Master            count
	    turborc.mpl    TurboRC_Multi_Processing  value
            turborc.tools  TurboRC_Tools             exists
            turborc.debug  TurboRC_Debug             exists
            turborc.slave  TurboRC_Tools             exists
	}
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
