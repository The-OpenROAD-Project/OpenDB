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

## plugin.tcl - plugin methods
## System module for Milos
## Author: Mattias Hembruch

package require Itcl

package provide ade::milos::papi 0.1

itcl::class PAPI_BASE {

    inherit PAPI_UTILS

    ## SET list of procs to be sent remotely
    public variable init_list { init_local }
    public variable server_proc_list { Server exit_remote eco open_eco close_eco close_debug close_pdebug pdebug set_si debug open_debug open_pdebug bgerror}
    public variable plugin_proc_list { }
    public variable plugin_api_list { }

    ## for remote start
    #public variable proc_list { Server send_sock init_remote }
    public method write_server_utils { fp } {
        write_utils $fp $server_proc_list
        write_code $fp $init_list
    }
    public method write_timing_functions { fp } {
        puts "write timing $plugin_proc_list"
        write_utils $fp $plugin_proc_list
        write_utils $fp $plugin_api_list
    }

    ### actual methods sent to remotes

    public proc Server { channel clientaddr clientport } {
        puts "Connection from $clientaddr registered"
        fconfigure $channel -buffering line
        fconfigure $channel -buffersize 1000000
        while { true } {
            ## Read 1 command
            gets $channel line
            set cmd ""
            while { $line != "." } {
                lappend cmd "$line \n"
                gets $channel line
            }
    
            ## execute command
    
            ## check for exit
            if { $cmd == "exit" } {
                puts $channel "."
                break;
            }
    
            ## execute command

            #regsub -all "\\\\" $cmd "\\\\\\\\" cmd

            #puts "CMD: $cmd"
            debug "CMD: $cmd"
            debug "END OF CMD"

            set ::cur_channel $channel
            
            set result ""
            set output ""
            set rc [ catch { set result [uplevel #0 [join $cmd]] } output ]
            flush $channel
            puts $channel $output
            debug "RESULT: $result/$output"
            if { $rc != 0 } { 
                puts $channel ".E."
                debug ".E."
            } else {
                puts $channel "."
                debug "."
            }
            #puts "END OF CMD: $cmd"
        }
        close $channel
        exit
    }

    public proc send_sock { rsock mysock } {
        set myaddr [ lindex [fconfigure $mysock -sockname] 2]
        puts "myaddr: $myaddr"
        set myhost [ exec hostname]
        puts "myhost: $myhost"
        set sl [ split $rsock ":"]
        set rhost [ lindex $sl 0]
        set raddr [ lindex $sl 1]
        puts "rhost: $rhost"
        puts "raddr: $raddr"
        set sendsock -1
        set sendsock [socket $rhost $raddr]
        puts "ss: $sendsock"
        if { $sendsock == -1 } {
            error "Cannot connect to main host!"
        }
        fconfigure $sendsock -buffering line
        puts "sending: $myhost $myaddr"
        puts $sendsock "socket $myhost $myaddr"
        puts $sendsock "."
        flush $sendsock
        close $sendsock
        puts "Exiting SS!"
    }
    
    public proc init_remote { } {
        set idx 0
        set rsocket 0
        if { [info exists env(ADE_PLUGIN_SOCKET)] } {
            set rsocket $env(ADE_PLUGIN_SOCKET)
        } else {
            puts "NO remote socket name!"
            exit
        }
            puts "I AM HERE!"
        set mysock [socket -server Server 0]
        send_sock $rsocket $mysock
        vwait forever
    }

    public proc open_eco { fname } {
        set ::eco_fp [open $fname w]
    }

    public proc close_eco { } {
        if { $::eco_fp != "" } {
            flush $::eco_fp
            close $::eco_fp
            set ::eco_fp ""
        }
    }
    public proc bgerror { message } {
        puts "BG ERROR: $message "
        return 0
    }

    public proc open_debug { } {
        global dpref
        set ::tmp_fp [open "${dpref}pcmd.log" w]
    }

    public proc close_debug { } {
        if { $::tmp_fp != "" } {
            flush $::tmp_fp
            close $::tmp_fp
            set ::tmp_fp ""
        }
    }

    public proc debug { cmd } {
        if { $::tmp_fp != "" } {
            puts $::tmp_fp $cmd
            flush $::tmp_fp
        }
    }
    public proc eco { cmd } {
        if { $::eco_fp != "" } {
            puts $::eco_fp $cmd
            flush $::eco_fp
        }
    }


    public proc exit_remote { } {
        close_debug
        close_pdebug
        close_eco
        if { $::tmp_fp != "" } {
            flush $::tmp_fp
            close $::tmp_fp
        }
        close $::cur_channel
        exit
    }

    public proc open_pdebug { } {
        global dpref
        set ::tmp_deb [open "${dpref}pdebug.log" w]
    }

    public proc close_pdebug { } {
        if { $::tmp_deb != "" } {
            flush $::tmp_deb
            close $::tmp_deb
            set ::tmp_deb ""
        }
    }

    public proc pdebug { str } {
        if { $::tmp_deb != "" } {
            puts $::tmp_deb $str
            flush $::tmp_deb
        }
    }

    public proc set_si { val } {
        set ::milos_si $val
    }

    public proc init_local { } {
        set tmp_fp ""
        set tmp_deb ""
        set cur_channel ""
        set eco_fp ""
        set milos_si 0
        set dpref ""
        set dlevel 0

        ## normal code
    
        set socketid 9000
        if { [info exists env(ADE_PLUGIN_SOCKET)] } {
            set socketid $env(ADE_PLUGIN_SOCKET)
        }
        socket -server Server $socketid
        vwait forever
    }

}
