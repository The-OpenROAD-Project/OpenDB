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

## rmt.tcl
## Remote module/proxy used by Milos
## Author: Mattias Hembruch

package require Itcl
package require ade::milos::mproxy

package provide ade::milos::rmt_base 0.1

itcl::class RMT {

    protected common READY    0; ## Ready, and waiting for commands
    protected common READYQ   1; ## Ready, queued commands - probably not common
    protected common WAIT     2; ## Waiting for a command to finish executing
    protected common WAITQ    3; ## Waiting, with additional commands queued
    protected common WAITRMT  4; ## Waiting for some other task to finish
                               ## see WRMT array for which task
    protected common WAITRMTQ 5; ## Waiting for some other task to finish
                               ## with more commands queued

    protected common WAITINIT 6;

    protected common STAT_TXT { "Ready" "Ready - commands queued" 
        "Waiting on cmd" "Waiting on cmd - commands queued" 
        "Waiting on remote task(s)"
        "Waiting - commands queued - on remote task(s)"
        "Waiting for initialization to finish"
    }

    protected common DB_NUM 0;
    protected common BLOCK_NUM 0;
    protected common ECO_NUM 1;
    protected common TIMING_NUM 0;
    protected common TMG_ECO_NUM 0;
    protected common TIMING_ALL 0;

    protected common ourmutex "";

    protected common bench 0; ## if doing benchmarking

    ### UPDATE TYPES - special return values!
    protected common SLACK_UPDATE 1;

    protected variable publish_list {};

    protected variable engine "milos";

    protected variable status;
    protected variable name ""; ## corner name

    protected variable cmdq { };
    protected variable retq { };
    protected variable curcmd "";
    protected variable curret 0;
    protected variable newret 0;
    public variable proxy;
    protected variable cbrc 0;
    protected variable async 0;
    protected variable rmt_app "milos";

    protected variable cberror 0;
    protected variable cberrmsg 0;

    protected variable mymutex;

    protected variable license_type "static"

    protected variable nslack;

    protected proc ns { task } {
         return [string range $task 0 [expr {[string last ":" $task] -2} ]]
    }

    protected method mutex { } {
        return $mymutex
    }

    protected method constructor { {async 0} {rmt_app "milos"}} {
        debug MILOS_RMT:C "RMT CONSTRUCTOR: $async! $rmt_app"
        set_status WAITINIT
        if { $::msh::OPT(dryrun) == 0 } {
            if { $async == 0 } {
                set proxy [MProxy #auto  \
                -callback "RMT::call_proc $this callback "  \
                -icallback "RMT::call_proc $this initcb "  \
                -init "package require ade::milos::mproxy "]
                $proxy app_init $rmt_app
            } else {
                set async 1
                set proxy [MProxy #auto  \
                -callback "RMT::call_proc $this callback "  \
                -icallback "RMT::call_proc $this initcb "  \
                -init "package require ade::milos::mproxy " \
                -acb "RMT::call_proc $this connected " \
                -async 1]
            }
        }

        set mymutex "${this}cbmutex"
        upvar $mymutex mutex
        set mutex 0

        if { $ourmutex == "" } {
            set ourmutex "[ns $this]::cbmutex"
        }
        set nslack [[NODE_SLACK #auto] this];
        debug MILOS_RMT:C "T: $nslack"

    }

    protected method destructor { } {
        debug MILOS_RMT:D "RMT destructor!"
        $proxy Kill
        debug MILOS_RMT:D "RMT destroyed!"
    }

    protected method set_status { stat {task ""} } {
        if { $task eq "" } { 
            set task $this 
        }
        if { ($stat < 0) || ($stat > $WAITRMTQ) } { 
            return -1 
        }
        return [set [ns $task]::RSTATUS($task) $stat]
    }

    protected method set_special_result { args } {
        debug MILOS_RMT:C "SPECIAL ($curret)!!!"
        switch $curret {
            1 {
                debug MILOS_RMT:C "REMOTE SLACK UPDATE: $args"
                set cmd "$nslack configure [join $args]"
                debug MILOS_RMT:C "cmd: $cmd"
                set rc [ catch { ::eval $cmd } output ]
                debug MILOS_RMT:C "DONE cmd ($rc, $output) [$nslack configure]!"
                set rc [ catch {::eval $cmd} output]
                debug MILOS_RMT:C "EXIT REMOTE SLACK UPDATE: $args"
            }
        }
    }

    public method get_node_slack { slacks } {
        return $nslack
    }

    protected method callback { rc args } {
        debug MILOS_RMT:A "Callback in $this for $curcmd ($curret): $rc: $args"

        set l [llength $cmdq]
        set_status $READY

        debug MILOS_RMT:B "Step 00: $l, $cmdq"
        if { $rc == "return" } {
            debug MILOS_RMT:B "Step 0a, $curret"
            if { $curret != 0 } {
                set_special_result $args
            } else {
                set cbrc $args
            }
        } elseif { $rc == "error" } {
            debug MILOS_RMT:B "Step 0b"
            #notice "Callback error $this for $curcmd: $rc: $args"
            debug MILOS_RMT:B "Step 0b2"
            set cbrc $args
            debug MILOS_RMT:B "Step 0b3"
            set cberrmsg "CMD $curcmd: Error: [join $args]"
            debug MILOS_RMT:B "Step 0b4"
            set cberror 1
            debug MILOS_RMT:B "Step 0b5"
        }

        debug MILOS_RMT:B "Step 1"

        if { $l > 0 } {
            debug MILOS_RMT:B "Step 2"
            debug MILOS_RMT:A "Command waiting for $this, send next command"

            set curcmd [lindex $cmdq 0]
            set curret [lindex $retq 0]
            set newret $curret
            debug MILOS_RMT:C "Set CURRET 2: $curret ($curcmd)"
            set cmd [lindex $curcmd 0]

            set cmdq [lreplace $cmdq 0 0]
            set retq [lreplace $retq 0 0]

            debug MILOS_RMT:B "Step 3"

            send_command $cmd [join [lrange $curcmd 1 end]]
            debug MILOS_RMT:B "Step 4"
        } else {
            ## see if I'm one of the ones being waited on, if I'm done
            debug MILOS_RMT:B "Step 5"
            debug MILOS_RMT:A "No more commands waiting for $this!"
            set curcmd ""
            set curret 0
            debug MILOS_RMT:C "Set CURRET 3: $curret ($curcmd)"
            if { [llength [waitlist]] > 0 } {
                debug MILOS_RMT:B "Step 6a for $this"
                set idx [lsearch [waitlist] $this]
                if { $idx >= 0 } {
                    set [ns $this]::RMTWL [lreplace [waitlist] $idx $idx]
                    if { [llength [waitlist]] == 0 } {
                        ## everyone's done
                        debug MILOS_RMT:B "Step 6b for $this"
                        set $ourmutex 1
                    }
                }
                debug MILOS_RMT:B "Step 6c"
            } else {
                ## Only me, reset my mutex if anyone was waiting for it
                debug MILOS_RMT:B "Step 7 for $this"
                set $mymutex 1
                if { $mycallback != "" } { 
                    debug MILOS_RMT:C "$this: Calling my callback: $mycallback"
                    ::eval $mycallback
                }
            }
            debug MILOS_RMT:B "Step 8: $curcmd ($curret), $rc, $cbrc, $args"
        }
        return 0
    }

    protected method initcb { args } {
        debug MILOS_RMT:C "Initcb: $args"
        set_status READY
        if { $mycallback != "" } { 
            debug MILOS_RMT:C "$this: Calling my callback: $mycallback"
            ::eval $mycallback
        }
    }

    protected method unalias { alias } {
        foreach {name val} [array get [ns $this]::RSLAVES] {
            if { $val eq $alias } {
                return $name
            }
        }
        return ""
    }

    protected method set_waitlist { lst } {
        set [ns $this]::RMTWL $lst
    }

    protected method waitlist { } {
        return [set [ns $this]::RMTWL]
    }

    ### PUBLIC VARIABLES
    public variable mycallback "";

    ### PUBLIC PROCS - NEW
    public proc new { {name ""} {async 0} {rmt_app "milos"} } {
        if { $name eq "" } {
            return [RMT #auto $async $rmt_app]
        } else {
            return [RMT $name $async $rmt_app]
        }
    }

    ### PUBLIC METHODS

    ## used to get back into the object from the proxy
    public proc call_proc { obj cmd args } {
        set cmd "$obj $cmd [join $args]"
        ::eval $cmd
    }
    public method addwait { } {
        debug MILOS_RMT:W "AW1: [waitlist]"
        if { [status] != 0 } {
            debug MILOS_RMT:W "AW 2: $this, [waitlist]"
            lappend [ns $this]::RMTWL $this
        }
    }

    public method gwait { {msg ""} } {
        debug MILOS_RMT:A "SGW [waitlist] ([info level -1] [info level -2])!"
        foreach node [waitlist] {
            debug MILOS_RMT:A "node $node status: [$node status]"
            debug MILOS_RMT:B "node $node status: [$node scq]"
        }
        if { [waitlist] > 0 } {
            notice $msg
            vwait $ourmutex
        }
        debug MILOS_RMT:A "EGW!"
    }

    public method mwait { } {
        set mymutex 0
        debug MILOS_RMT:W "Waiting for $this!"
        vwait [mutex]
        debug MILOS_RMT:W "Finished Waiting for $this!"
    }

    public method restart { } {
        $proxy Kill
        if { $async == 0 } {
            set proxy [MProxy #auto  \
            -callback "RMT::call_proc $this callback "  \
            -icallback "RMT::call_proc $this initcb "  \
            -init "package require ade::milos::mproxy "]
            $proxy app_init $rmt_app

        } else {
            set proxy [MProxy #auto  \
            -callback "RMT::call_proc $this callback "  \
            -icallback "RMT::call_proc $this initcb "  \
            -init "package require ade::milos::mproxy " \
            -acb "RMT::call_proc $this connected " \
            -async 1]
        }
    }


    public method start { } {
        $proxy StartRemote
    }

    public method get_remote_host { } {
        set addr [$proxy cget -address]
        if { $addr eq "" } {
            return ""
        } else {
            set hl [split $addr "@"]
            return [lindex $hl end]
        }
    }

    public method connected {args } {
        debug MILOS_RMT:C "Connected: $this!! $args"
        $proxy app_init $rmt_app
        debug MILOS_RMT:C "Initialized!"
    }

    public method send_command { cmd args } {
        debug MILOS_RMT:S "send_command"

        if { $::msh::OPT(dryrun) == 0 } {
            set st [status]
            if { $st == $READY } {
                debug MILOS_RMT:S "SC ($this): command: $cmd, args: $args"
                set curcmd "$cmd [join $args]"
                debug MILOS_RMT:S "SET CURRET 1: $newret ($curcmd)!"
                set curret $newret
                set newret 0
                debug MILOS_RMT:S "SCcc: $curcmd ($curret)"
                if { $cmd eq "acmd" } {
                    ## special Ade command
                    debug MILOS_RMT:S "Using acmd!"
                    $proxy acmd "$curcmd"
                } else {
                    ## Normal method
                    $proxy cmd "$curcmd"
                }
                set_status $WAIT
            } else {
                debug MILOS_RMT:A "send_command: ($this) non-ready: $st"
                if { $st == $WAIT  || $st == $WAITQ } {
                    ## waiting..
                    set_status $WAITQ
                } elseif { $st == $READYQ } {
                    ## can we execute?
                } elseif { $st == $WAITRMT || $st == $WAITRMTQ } {
                    set_status $WAITRMTQ
                }
                lappend cmdq "$cmd $args"
                lappend retq $newret
                set newret 0
                scq
            }

        }
    }

    public method clear { } {
        set_status $READY
        set rc ""
        set cmdq {}
    }

    public method mdebug { {file ""} } {
        $proxy mdebug $file
    }

    public method set_name { myname } {
        set name $myname
    }

    public method get_name { } {
        return $name
    }

    ## Testing methods

    public method next_command { } {
        set cmd [lindex $cmdq 0]
        set cmdq [lreplace $cmdq 0 0]
        set retq [lreplace $retq 0 0]
        return $cmd
    }

    public method scq { } {
        foreach arg $cmdq {
            foreach {cmd opt} $arg {
                debug MILOS_RMT:C "Command: [join $cmd] [join $opt]"
            }
        }
        debug MILOS_RMT:C "RETQ: $retq"
    }


    public method waitme { {task ""} } {
        if { $task eq "" } {
            set task $this
        }
        return [set [ns $task]::WME($task)]
    }

    public method waitfor { {task ""} } {
        if { $task eq "" } {
            set task $this
        }
        return [set [ns $task]::WRMT($task)]
    }

    public method add_waitme { task } {
        lappend [ns $this]::WME($this) $task
    }

    public method add_waitfor { task } {
        lappend [ns $this]::WRMT($this) $task
    }

    public method tasklist { } {
        return [ lsort [array names [ns $this]::RSLAVES]]
    }

    public method getAlias { {task ""} } {
        if { $task eq "" } {
            set task $this
        }
        return [set [ns $task]::RSLAVES($task)]
    }

    ## Warning - CLEARS error status
    public method getErrorStatus { } {
        set lrc $cberror
        set cberror 0
        return $lrc
    }

    public method getErrorMessage { } {
        set lmsg $cberrmsg
        set cberrmsg ""
        return $lmsg
    }

    public method status { {task ""} } {
        if { $task eq "" } {
            set task $this
        }
        return [set [ns $task]::RSTATUS($task)]
    }


    public method pstatus { {task ""} } {
        set stat [status $task]
        set ps [lindex $STAT_TXT $stat]
        if { $stat == $WAITRMT || $stat == $WAITRMTQ} {
            foreach arg [waitfor $task] {
                append ps " [getAlias $arg]"
            }
        }
        return $ps
    }

    public method rc { } {
        return $cbrc
    }

    public method reset_unpublish { } {
        set publish_list {}
    }

    public method add_unpublish { name } {
        lappend publish_list $name
    }

    public method do_unpublish { } {
        foreach name $publish_list {
            unpublish $name
        }
    }

    public method unpublish { string } {
        send_command acmd "Unpublish $string"
    }
    public method publish { string } {
        send_command acmd "Publish $string"
    }

    public method subscribe { string } {
        send_command acmd "Subscribe $string"
    }

    foreach mname {ext mme pdb opt rpt rte sta sys tmg } {
        set cmd "public method $mname { args } { "
        #append cmd "puts \"RMT $mname: \$args\";\n"
        append cmd "send_command \"$mname \$args\" }"
        eval $cmd
    }
}

