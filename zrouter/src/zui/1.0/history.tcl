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

#
#       Class: ZHistory
# Description: Keep the ZUI history
#      Author: Manolis M. Tsangaris
#
itcl::class ZHistory {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # push history
    public method Push {state} {
        set STATE([incr NOW]) $state
        # erase any forward history
        for {set n [expr $NOW + 1]} { [info exists STATE($n)] } { incr n } {
            unset STATE($n)
        }
    }
    # -- go back to the history line
    public method Back {pp} {
        upvar $pp P
        incr NOW -1
        if ![info exists STATE($NOW)] {
            incr NOW
            return 0
        }
        array set P $STATE($NOW)
        return 1
    }
    # -- go forward to history line
    public method Next {pp} {
        upvar $pp P
        incr NOW
        if ![info exists STATE($NOW)] {
            incr NOW -1
            return 0
        }
        array set P $STATE($NOW)
        return 1
    }
    public method Clear {} {
        set NOW 0
        catch {unset STATE}
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable STATE
    protected variable NOW 0
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
