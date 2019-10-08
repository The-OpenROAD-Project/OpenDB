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

package provide ade::ui 1.0

#       Class: ZUI
# Description:
#      Author: Manolis M. Tsangaris
#
itcl::class ZUI {
    inherit LProgram
    # -------------------------------------------------- Public Variables --
    public variable pprefix zroute ""      ;# -- prefix for zroute session info
    public variable top .                  ;# -- top installation directory
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables
    protected variable loader ""
    protected variable rpc ""
    protected variable zmgr ""
    protected variable lic  "" ;# license manager
    protected variable SALT "ADS2005"
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        option version     -v version "select ui version" ""
        option server      -s server "Server url to connect to (host@port)" ""
        option debug       -d debug  "debug level" 0
        option maxobjects  -l int    "max number of objects to show" 15000
        option automark    -am "generate auto marks based on object type"
        option cache       -c "cache objects in GUI"
        option esupport -em email   "email for support"  zroute@athenads.com
        option eserver  -es server  "support mail server" mail.athenads.com
        option help  -h "Help"
        option lic     -L  file  {license file (or $ADS_LICENSE)} ""

        global tcl_precision
        set tcl_precision 17
	LLogger::Auto

        set rpc      [LRPCClient $this.rpc  -debug 0]
        set loader   [ZLoader $this.loader  -rpc $rpc]
    }
    destructor {
        $loader delete
        $rpc delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method license_check {lpath {features ""}} {
        if { $lpath == "" } {
            global env
            if ![info exists env(ADS_LICENSE)] {
		set lapth ""
            } else {
                set lpath $env(ADS_LICENSE)
            }
        }
        # -- modify the env variable
        set env(ADS_LICENSE) $lpath
        set lic [LLicense $this.lic -salt $SALT]
        $lic Read $lpath
        set errors 0
        foreach f $features {
            set r [$lic Check $f]
            if { $r != "ok" } {
                warning ade:ui "license: feature: $f: $r"
                incr errors
            }
        }
        return $errors
    }
    # ----------------------------------------------- Interface Implementation
    protected method vRun {opt} {
        upvar $opt OPT
        if $OPT(help) {
            Usage
            Exit 0
            return
        }
        if { [license_check $OPT(lic) zui] } {
            Exit 100
            return
        }
        wm withdraw .

        $loader configure -debug $OPT(debug)
        set mgr [ZMGR $this.mgr -loader $loader -rpc $rpc -opts [array get OPT]]
        update idletasks ; update

        # -- important to do this after idle loop
        $mgr Run init
        $mgr Wait
        $mgr delete
        Exit 0
    }
    # -------------------------------------------------- Common Initialization
}
