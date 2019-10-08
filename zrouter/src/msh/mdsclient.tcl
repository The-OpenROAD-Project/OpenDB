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
# MDS interface 
#
package require Itcl
package require mtlib 1.0

package provide LMSH 1.0

#
#       Class: MDSClient
# Description: MDS client
#      Author: Manolis M. Tsangaris
#
itcl::class MDSClient {
    inherit LObject LBinder
    # -------------------------------------------------- Public Variables --
    public variable address 6370@localhost
    # -------------------------------------------------- Public Methods ----
    # -- CALLS available to remote subscribers and publishers
    # declare the intention to publish
    # -- args is really the options passed to publish (like -p2p 1)
    public method Publish {dataset {callback ""} args} {
        $this mds_publish $dataset $callback $args
    }
    public method Unpublish {dataset} {
        $this mds_unpublish $dataset
    }

    # declare the intention to subscribe
    # -- args is really the options passed to publish (like -p2p 1)
    public method Subscribe {dataset {callback ""} args} {
        $this mds_subscribe $dataset $callback $args
    }
    # unsubscribe
    public method UnSubscribe {dataset} {
        $this mds_unsubscribe $dataset
    }

    # Wait for the dataset to arrive
    public method Wait {dataset} {
        $this mds_wait $dataset
    }

    # A post has been requested
    public method RequestPost {dataset} {
        $this mds_request_post $dataset
    }

    # A get has been requested
    # -- request for new data to be retrieved
    public method RequestGet {dataset} {
        $this mds_request_get $dataset
    }

    # -- request a client to receive data from a p2p connection
    public method RequestStreamOut {dataset p2p} {
        $this mds_request_stream_out $dataset $p2p
    }
    # -- request a client to receive data from a p2p connection
    public method RequestStreamIn {dataset p2p} {
        $this mds_request_stream_in $dataset $p2p
    }

    # -- P2P stream out. This command is called by the subscriver
    # -- which opens another tcp connection to the current socket
    # -- and asks for stream out. RPC2 appends the channel to the command
    public method StreamOut {dataset channel} {
	if [catch {eSend $dataset $channel} rc] {
            warning mdsclient "StreamOut: $rc"
	}
	set rc
    }

    public method Status {arg} {
        $rpc Send $peer "Status $arg"
    }
    public method NextVersion {ds} {
        $rpc Send $peer "NextVersion $ds"
    }
    public method CurVersion {ds} {
        $rpc Send $peer "CurVersion $ds"
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable rpc            ;# rpc instance (to talk to MDS)
    private variable rpc1 ""        ;# rpc p2p server instance
    private variable rpc2 ""        ;# rpc p2p client instance
    private variable peer           ;# peer connected to (mds)
    private variable PUB            ;# Publisher callbacks
    private variable SUB            ;# Subscriber callbacks
    private variable SEMA           ;# Semaphore to wait on datasets 
    private variable SCHED          ;# Scheduler
    private variable ELOG           ;# enhanced logger
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set ELOG [ELogger #auto]

        set rpc [LRPC2 $this.rpc -server 0 -prefix "uplevel #0 $this"]
        set peer [$rpc Connect $address]

#$rpc configure -debug *
	# -- two sockets in case we have peer2peer
        set rpc1 [LRPC2 $this.rpc1 -server 1 -prefix "uplevel #0 $this"]
        set rpc2 [LRPC2 $this.rpc2 -server 0 -prefix "uplevel #0 $this"]
	set reg [list p2p [$rpc1 Address]]
	$rpc Send $peer [list Register $reg]
	message registering "$reg"

        set SCHED [LScheduler $this.sched]
	$SCHED Bind <<error>> $this eSchedulerError

        message connected "to $address"
        array set SEMA {}
    }
    destructor {
        $SCHED delete
        foreach ds [array names SEMA] {
            $SEMA($ds) delete
        }
        $rpc delete
	foreach r "rpc1 rpc2" {
	    if { [set $r] != "" } { [set $r] delete }
	}
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -- publish a dataset
    protected method mds_publish {dataset callback opts} {
        set PUB($dataset) $callback
        if ![info exists SEMA($dataset)] {
            set SEMA($dataset) [LSemaphore $this.pub_$dataset]
        } else {
            #warning mdsclient "$dataset: multiple publish/subscribe"
            #warning mdsclient "$dataset: Assumes data has not changed"
        }
	message $dataset "publish $dataset $opts"
        $rpc Send $peer [list Publish $dataset $opts]
    }

    # -- declare the intention to subscribe
    protected method mds_subscribe {dataset callback opts} {
        set SUB($dataset) $callback
        if { ![info exists SEMA($dataset)] } { 
            set SEMA($dataset) [LSemaphore $this.sub_$dataset]
        } else {
            #warning mdsclient "$dataset: multiple subscribe/publish"
            #warning mdsclient "$dataset: Assumes data has not changed"
        }
	message $dataset "subscribe $dataset $opts"
        $rpc Send $peer [list Subscribe $dataset $opts]
    }
    # -- unsubscribe (INCOMPLETE!)
    protected method mds_unsubscribe {dataset} {
        set SUB($dataset) ""
    }

    # -- unpublish (see msh/src/mds/var.tcl)
    protected method mds_unpublish {dataset} {
        $rpc Send $peer [list Unpublish $dataset]
    }

    # -- Wait for the dataset to arrive
    protected method mds_wait {dataset} {
	if ![info exists SEMA($dataset)] {
	    error "Wait($dataset): data set does not exist!"
	}
        $SEMA($dataset) Wait
    }

    # -- A post has been requested
    protected method mds_request_post {dataset} {
        message post-req $dataset
        $SCHED Run post-req-$dataset [list $this eMDSPost $dataset]
        return "OK {$dataset will be posted}"
    }

    # -- A get has been requested
    # -- request for new data to be retrieved
    protected method mds_request_get {dataset} {
        message get-req $dataset
        $SCHED Run get-req-$dataset [list $this eMDSGet $dataset]
        return "OK {$dataset will be retrieved}"
    }
    # -- mds requests subscriber to rx data directly from a publisher
    protected method mds_request_stream_in {dataset p2peer} {
        message stream-in "$dataset $p2peer"
        $SCHED Run stream-in-req-$dataset\
	    [list $this eP2PStreamIn $dataset $p2peer]
        return "OK {$dataset will be received in stream mode by $p2peer}"
    }
    # ----------------------------------------------------------------------
    protected method message {m1 m2} {
        $ELOG debug "MDSC:A" [format "%10s: %s" $m1 $m2]
        # LLog mdsclient notice [format "%10s: %s" $m1 $m2]
    }
    # ----------------------------------------------- CallBacks
    # -- send the dataset to MDS
    public method eMDSPost {dataset} {
        $rpc SendStream $peer [list $this eSend $dataset] [list Post $dataset]
    }
    # -- send a dataset to the server using channel
    public method eSend {dataset channel} {
        set cmd $PUB($dataset) ; lappend cmd $dataset $channel
        message sending "$dataset => $channel ($cmd)"

        fconfigure $channel -buffering full -encoding binary -translation binary

        if {[catch {uplevel #0 $cmd} rc]} {
            warning mdsclient "sending $dataset: $rc"
            message sent "$dataset => $channel (with errors)"
        } else {
            message sent "$dataset => $channel"
        }
        catch {close $channel}

        # note that the dataset is ok and notify waiters
        $SEMA($dataset) Signal
    }

    # -- request this dataset from the MDS server
    public method eMDSGet {dataset} {
        $rpc SendStream $peer [list $this eRecv $dataset] [list Get $dataset]
    }

    # -- schedule stream in to happen from p2peer (publisher) to us (sub)
    public method eP2PStreamIn {dataset p2peer} {
	message P2PStreamIn "initiating stream of $dataset from $p2peer"
	# -- initiate stream
	if [catch {$rpc GetStreamFrom $p2peer \
		[list StreamOut $dataset] [list $this eRecv $dataset]} rc] {
	    puts stderr "$rc"
	}
    }

    # -- receiver for this dataset from the server using channel
    public method eRecv {dataset channel} {
        set cmd $SUB($dataset) ; lappend cmd $dataset $channel
        message receiving "$dataset <= $channel ($cmd)"
        fconfigure $channel -buffering full -encoding binary -translation binary

        if {[catch {uplevel #0 $cmd} rc]} {
            warning mdsclient "receiving $dataset: $rc"
            message recvd "$dataset <= $channel (with errors)"
        } else {
            message recvd "$dataset <= $channel"
        }
        catch {close $channel}

        # note that the dataset is ok and notify waiters
        $SEMA($dataset) Signal
    }
    public method eSchedulerError {tag cmd error errorinfo} {
	message mdsclient "scheduler($tag): $error $errorInfo"
	warning mdsclient "scheduler($tag): $error $errorInfo"
    }
    # -------------------------------------------------- Common Initialization
}
