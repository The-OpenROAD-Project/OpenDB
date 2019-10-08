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
#       Class: GTCL
# Description: Gui TCL language
#      Author: Manolis M. Tsangaris
#
# Interpet user plain commands as zroute commands. Visualize output.
#
# GTCL works using a slave safe interpeter bound to some of its own
# public methods.
#
itcl::class GTCL {
    inherit LObject LBinder
    # Event <<result>>    value         ;# value to be visualized
    # Event <<command>>   cmd args      ;# command to run in the backend
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # show any type of object
    # to be more specific, the user can enter a hint (like -net or -name)
    public method show {args} {
        eval command find_show $args
    }

    public method inspect {obj} {
        show_obj $obj
        # -- now inspect it
        result [command inspect $obj]
    }
    # select an object
    public method select {args} {
        eval command find_select $args
    }

    # allow the user to change the bbox
    public method bbox {args} {
        eval command view_set $args user
    }

    # -- set the grid fatcor (i.e. how many pixels to aggregate/sample)
    public method gfactor {gf} {
        command gfactor $gf
    }
    # ------------------------------------------------------------------------
    public method Run {args} {
        $interp Run $args
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable interp
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set interp [LInterp $this.interp]
        set cmds "show inspect select bbox gfactor"
        foreach cmd $cmds {
            $interp Bind $cmd "$this $cmd"
        }
    }
    destructor {
        $interp delete
    }
    # -------------------------------------------------- Private Methods ---
    private method result {res} {
        Event <<result>> $res
    }
    private method command {args} {
        eval Event <<command>> $args
    }
    # ----------------------------------------------- Interface Implementation
    # the user has asked us to show a chip object name or chip object id
    protected method show_object {obj type} {
        if { $type == "" } {
            set type [type_of $obj]
        }
        switch $type {
            id      {
                # is the object there already? just select it
                if { [result [command select $obj]] } {
                    return ""
                }
                # -- center the object
                result [command center $obj]

                # -- select it
                result [command select $obj]
            }
            default {
                error "cannot show `$obj' yet (of type `$type')"
            }
        }
        return ""
    }
    # in general, chip object ids have that form
    protected method type_of {name} {
        if { [regexp {[a-zA-Z][0-9]+\.[a-zA-Z][0-9]+.*} $name] } {
            return id
        }
        return "unknown"
    }

    # find objects and make the results a selection
    protected method gt_select {arg} {
        set matches [obj_find select $arg]
        set res "" 
        foreach obj $matches {
            array set P $obj
            lappend res $P(id)
        }
        result [list type selection obj $res]
    }

    # find objects and display the results 
    protected method gt_show {arg} {
        set res [obj_find select $arg]
        if { $res == "" } { bell ; return }
        array set A [lindex $res 0]
        result [list type inspection obj $A(id) \
            info set/group inspection $res bbox ""]
    }

    # find an object
    protected method obj_find {name arg} {
        if { ( $arg == "" ) } { 
            error [find_usage $name]
        }
        foreach {a b c} $arg { break }
        if { ($a == "") || ($b == "") || ($c == "") } {
            error [find_usage $name]
        }
        command find $a $b $c
    }
    protected method find_usage {name} {
        set list {net name "net by name" net id "net by id"}
        set msg "Usage:\n"
        append msg "  gui-id (to $name any gui object)\n" 
        foreach {a b c} $list {
            append msg "  $name $a $b string (to $name $c)\n" 
        }
        set msg
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
