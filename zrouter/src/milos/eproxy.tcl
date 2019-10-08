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

## Extraction Proxy Class

package require Itcl
package require ade::milos::vob 0.1

package provide ade::milos::ext 0.1

## Extraction proxy class
itcl::class EXTProxy {
    inherit VOB

    public variable callback "";
    public variable block "";
    public variable ext_model_file "";
    public variable tile_script "";
    public variable ext_args "";
    public variable tile_lefs "";
    public variable exec_name "";
    public variable exec_parms "";

    protected variable db  ""
    protected variable mydebug 0

    ### Restore "configure" - overrides "VOB"'s hidden configure
    public method configure { option args } \
        @itcl-builtin-configure

    public method cget { var } {
        return [ set [string range $var 1 end]]
    }

    public method cmd { args } {
        if { $args eq "" } {
            return
        }
        _send [join $args]
    }

    public method start { args } {
        set db [get_tm DB]
        _start [join $args]
    }

    public method get_tile { } {
        _getTile
    }
    public method send_tile { } {
        _sendTile
    }

    public method extract { args } {
        _extract
    }

    public method assemble { } {
        _assemble
    }

    public method exit { } {
        set callback ""
        _exit
    }
    
    public method this { } {
        return $this
    }

    public method set_debug { val } {
        set mydebug $val
        _debug $val
    }

    public method status { } {
        return [_status]
    }

    protected method mydebug { args } {
        if { $mydebug > 0 } {
            puts "$args"
        }
    }
    protected method _status { } {
    }
        
    protected method _debug { val } {
    }
    protected method _extract { } {
    }
    protected method _assemble { } {
    }

    protected method _exit { } {
    }

    protected method _send { args } {
        return
    }
    protected method _start { args } {
        return -code return
    }

    protected method _getTile { } {
    }

    protected method _sendTile { } {
    }

}


## Internal extraction (on remote)
itcl::class EXTProxy_internal {
    protected variable _rmt;
    protected variable _init 0;
    protected common RMT_CNT 0;

    inherit EXTProxy 
    protected method _start { args } {
        new_rmt R$RMT_CNT 1 athext
        set _rmt [state ns]::R$RMT_CNT
        incr RMT_CNT
        $_rmt configure -mycallback [::itcl::code $this _callBack]
        $_rmt start
    }

    protected method _status { } {
        return [$_rmt status]
    }

    protected method _doInit { } {
        ## do initialization stuff here
        mydebug "INTERNAL doInit 1 $this, $_rmt"
        $_rmt send_command acmd "Subscribe chip.tech"
        $_rmt send_command acmd "Subscribe chip.libs"
        $_rmt send_command acmd "eval {::zr.zlog enable_debug_log -module ZTC}"
    }

    protected method _getTile { } {
        Subscribe -dbargs "-parasitics" ${block}.res.block
    }

    protected method _sendTile { } {
        $db find -block $block -object= ::BN
        if { $mydebug > 0 } {
            file mkdir tiles/$block
            $db write -block $::BN -file tiles/$block/remote_in.db
        }
        Publish $block.mblock
    }

    protected method _callBack { } {
        mydebug "EPROXY CALLBACK!!"
        if { $_init == 0 } {
            mydebug "EPROXY CALLBACK doinit!!"
            _doInit
            if { $mydebug > 0 } {
                $_rmt mdebug
            }
            if { $mydebug > 1 } {
                $_rmt send_command sys "eval  { gdb n14:21.0 -x ~/.c}"
            }
            set _init 1
            mydebug "EPROXY CALLBACK doinit DONE!!"
        } else {
            mydebug "EPROXY CALLBACK $callback!!"
            if { $callback ne "" } {
                ::eval $callback
            }
        }
    }

    protected method _exit { } {
        itcl::delete object $_rmt
    }

    protected method _extract { } {
        if { ($block eq "") } {
            error "Cannot extract - block: $block" 
        }
        if { $mydebug > 0 } {
            mydebug "Internal extraction of block $block!"
        }

        $_rmt send_command acmd "Subscribe $block.mblock"
        $_rmt send_command acmd "set \[::apps::milos::cur_ns]::STATE(db_loaded) 1"
        if { $tile_script ne "" } {
            $_rmt send_command "source $tile_script"
        }
        $_rmt send_command ext "extract $ext_args"
        $_rmt send_command pdb "write_spef -outfile $block/int.spef.gz"
        $_rmt send_command acmd "Publish -dbargs -parasitics $block.res.block"
        $_rmt send_command acmd "\$::apps::milos::mycf del_tm Ext"
    }

    protected method _assemble { } {
        set te [get_tm TE]
        $te assemble -relative_c_threshold 0.01 -tile [string range $block 1 end]
    }

}

## External extraction (on remote)
itcl::class EXTProxy_external {
    inherit EXTProxy_internal

    protected method _doInit { } {
        ## do initialization stuff here
        mydebug "EXTERNAL doInit 1 $this, $_rmt"
        $_rmt send_command acmd "Subscribe chip.tech"
        $_rmt send_command acmd "Subscribe chip.libs"
        mydebug "EXTERNAL doInit 2 $this"
        $_rmt send_command acmd "eval {::zr.zlog enable_debug_log -module ZTC}"
        $_rmt send_command acmd "cd tiles"
        mydebug "EXTERNAL doInit 3 $this"
        $_rmt send_command acmd "array unset ::env MSHTID"
        mydebug "EXTERNAL doInit 4 $this"
    }

    protected method _extract { } {
        if { $block eq "" } {
            error "Cannot extract - missing block"
        }
        if { ($exec_name eq "") || ($tile_script eq "") } { 
            error "Cannot extract - missing exec_name or tile_script!"
        }
        mydebug "External extraction of block $block!"

        $_rmt send_command acmd "file mkdir $block"
        $_rmt send_command acmd "cd $block"
        $_rmt send_command acmd "Subscribe $block.mblock"
        $_rmt send_command acmd "set \[::apps::milos::cur_ns]::STATE(db_loaded) 1"
        $_rmt send_command acmd "set ::tnum $block"
        if { $tile_lefs eq "" } {
            $_rmt send_command pdb "save_design -def_out tile.def -lef_out tile.lef"
        } else {
            $_rmt send_command pdb "save_design -def_out tile.def"
        }
        $_rmt send_command acmd "set ::tcmd \"exec $exec_name $exec_parms $tile_script\""
        $_rmt send_command acmd "puts \"exec cmd: \$::tcmd\""
        $_rmt send_command acmd "::eval \$::tcmd"
        $_rmt send_command pdb "read_spef -file tile.spef.gz"

        ## write debugging db
        $_rmt send_command acmd "::db find -block $block -object= ::BN"
        $_rmt send_command acmd "::db get -top -object= ::BN"
        if { $mydebug > 0 } {
            $_rmt send_command acmd "::db write -block \$::BN -parasitics -file ${block}_remote_out.db"
            $_rmt send_command acmd "::db write -file full_remote_out.db"
        }
        $_rmt send_command acmd "Publish -dbargs -parasitics $block.res.block"
        $_rmt send_command acmd "\$::apps::milos::mycf del_tm Ext"
        $_rmt send_command acmd "cd .."
    }
}

## Threaded extraction (on main)
itcl::class EXTProxy_threaded {
    inherit EXTProxy;

    protected variable _id -1;
    protected common THREAD_CNT 0;

    protected method _send { args } {
        if { $_id == -1 } {
            error "Thread has not been initialized!"
        }
        thread::send $_id [join $args]
    }

    protected method _start { args } {
        package require Thread
        set _id [thread::create -joinable {
            thread::wait
        }]
        thread::configure $_id -unwindonerror 1

        if { $THREAD_CNT == 0 } {
            tsv::set milos main [thread::id]
            incr THREAD_CNT
        }
        thread::send -async [list set ::callback [::itcl::code $this _callBack]]

    }

    protected method _callBack  { } {
        puts "Threaded callback $this"
    }

    protected method _extract { } {

        thread::send $_id { 
            
            
            
            thread::wait 
        }
    }

    protected method _exit { } {
        thread::send $_id { thread::release }
        thread::join $_id
        set _id -1
    }


}


