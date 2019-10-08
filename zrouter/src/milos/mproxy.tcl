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

## proxy.tcl
## Proxy classes used by Milos
## Author: Mattias Hembruch

package require Itcl

package provide ade::milos::mproxy 0.1

itcl::class MProxy {
    inherit ADSProxy

    public variable callback;
    public variable icallback;
    public variable acb;
    #public variable async 0;

    public method app_init {app } {
        Send "app_init $app" <<init>>
    }

    public method mdebug { {file ""} } {
        Send "mdebug $file" <<dummy>>
    }

    ## ADE command
    public method acmd { args } {
        #puts "MPA: [join $args]"
        ncmd acmd [join $args]
    }

    ## CMD (milos cmd)
    public method cmd { args } {
        #puts "MPC: [join $args]"
        ncmd cmd [join $args]
    }

    public method ncmd { cmd args } {
        #puts "MP: Sending: $cmd [join $args]"
        set rc [ catch { Send "$cmd $args" <<cmd>> } output ]
        #puts "SRC: $rc, $output"
    
    }

    ## just in case not redirected
    public method cb { type args } {
        if { $type eq "return" } {
            puts "$this Response: $args"
        } else {
            puts "$this ERROR from remote: $type, $args"
        }
    }

    constructor { args } {
        #puts "mproxy constructor"
        set callback "$this cb"
        set icallback "$this cb"
        eval configure $args
        #Bind <<cmd>> $callback
        set cmd "Bind <<cmd>> $callback"
        ::eval $cmd
        set cmd "Bind <<init>> $icallback"
        ::eval $cmd
        set cmd "Bind <<dummy>> $this cb"
        if { $async == 1 } {
            set cmd "Bind <<connected>> $acb"
            #set cmd "Bind <<connected>> $this cb"
            ::eval $cmd
        }
    }
}

itcl::class MSlave {
    inherit ADSSlave

    private variable myinterp "";
    private variable fp "";

    ## should we do debugging?
    private variable dodebug 0;

        
    public method cmd { args } {
        set output "output variable not set - milos"
        #if { $dodebug == 1 } {
        #    set rc [ catch {mydebug "CRECEIVED: $args"} output ]
        #    if { $rc != 0 } {
        #        mydebug "CMDR: $rc, $output"
        #    }
        #}
        set mycmd "[lrange $args 0 end]"
        if { $dodebug == 1 } {
            mydebug "MCMD: [set mycmd]"
        }
        if { $myinterp != "" } {
            #mydebug "MILOS $mycmd"
            set rc [ catch { interp eval $myinterp $mycmd } output ]
        } else {
            #mydebug "ADE $mycmd"
            set rc [ catch { uplevel 0 "namespace eval :: {$mycmd}" } ]
        }
        if { $rc == 0 } {
            if { $dodebug == 1 } {
                mydebug "DONE: $rc"
            }
            return $output
        } else {
            if { $dodebug == 1 } {
                mydebug "ERROR: {$output}"
            }
            return -code error "{$output}"
        }
    }

    public method acmd { args } {
        set output "output variable not set - ade"
        ## changed 0 to 1, to avoid double-eval of acmds..
        #set mycmd "[lrange $args 0 end]"
        #if { $dodebug == 1 } {
            #set rc [ catch {mydebug "ARECEIVED: acmd $args"} ]
            #if { $rc != 0 } {
                #mydebug "ACMDR: $rc, $output"
            #}
        #}
        set mycmd "[lrange $args 1 end]"
        if { $dodebug == 1 } {
            mydebug "ACMD: $mycmd"
        }

        set rc [ catch { ::eval $mycmd } output ]
        if { $rc == 0 } {
            if { $dodebug == 1 } {
                mydebug "DONE: $rc"
            }
            return $output
        } else {
            if { $dodebug == 1 } {
                mydebug "ERROR: {$output} {$::errorInfo}"
            }
            return -code error  "{$output}"
        }

        #return [::eval $mycmd]
    }

    public method app_init { app } {
        package require ade::$app
        set myinterp [::apps::init $app]
        ::zr.cli reset_bindings
    }

    constructor { args } { 
        eval configure $args
        msh::init
    }

    private method mydebug { msg } {
        ## moved dodebug check inline, in case of LONG command lines - want to
        ## avoid evaluating a 1 MB commandline twice when not debugging
        puts $fp $msg
        flush $fp
    }

    public method mdebug { {file ""}} {
        if { $file == "" } {
            set file "mproxy.log.[pid]"
        }
        set fp [open $file w]
        set dodebug 1
        return 0
    }

}
