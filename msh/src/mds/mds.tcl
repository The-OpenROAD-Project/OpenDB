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
package require mtlib 1.0

package provide mds 1.0

#
# SIMPLE MDS SERVER
#    It implements the MDS publish/subscribe protocol
# --------------------------------------------------------------------------- 
#       Class: MDS
# Description: Multiprocessing Data Server
#      Author: Manolis M. Tsangaris
#
# This is the abstract interface to the MDS protocol
#
itcl::class MDS {
    inherit LObject LInteractor
    # -------------------------------------------------- Public Variables --
    public variable address "6370-@"
    public variable cachepath /tmp:.        ;# where to put cache
    public variable mdsc 0                  ;# should we use mdsc?
    # -------------------------------------------------- Public Methods ----
    # -- CALLS available to remote subscribers and publishers

    # PUBLISHER: declare the intention to publish
    # publish a data element
    # opts are unix like command line options (like -p2p 1)
    public method Publish {src {opts ""}} {
        $this mds_publish $src [$RPC Peer] $opts
    }
    # PUBLISHER: post the data to channel
    public method Post {dataset channel} {
        debug "post: $dataset -> $channel"
        $DV($dataset) Put $channel
    }

    # SUBSCRIBER: declare the intention to subscribe
    # subscribe to a data element
    # opts are unix like command line options (like -p2p 1)
    public method Subscribe {src {opts ""}} {
        $this mds_subscribe $src [$RPC Peer] $opts
    }

    # SUBSCRIBER: retrieve data to channel
    public method Get {dataset channel} {
        debug "get: $dataset <- $channel"
        $DV($dataset) Get $channel
    }

    # -- PUBLISHER: declare a data set no longer needed
    # -- the data set can now be removed from the cache and
    # -- the data set is marked as "invalid" (not ready)
    # -- in theory, another publish and subscribe cycle could start
    public method Unpublish {dataset} {
        $this mds_unpublish $dataset
    }

    # -- calls used by the local dataset objects
    # -- request for new data to be posted
    public method RequestPost {dataset pub} {
        if { [catch {$RPC Send $pub [list RequestPost $dataset]} rc ] } {
            LLog notice mds "request post failed: $dataset $pub"
            LLog notice mds "error:$rc"
            return 0
        }
        return $rc
    }

    # -- request for new data to be retrieved
    public method RequestGet {dataset pub} {
        $RPC Send $pub [list RequestGet $dataset]
    }

    # -- ask a subscriber to request a stream in of a data set from pub
    # -- called by var.tcl
    public method RequestStreamOut {dataset pub sub} {
	LLog notice mds "request stream out for $dataset $pub -> $sub"
        if [catch {$RPC Send $pub [list RequestStreamOut $dataset $sub]} rc] {
	    LLog warning mds "req streamout failed: $rc"
	}
	set rc
    }
    # -- ask a subscriber to receive a stream of a data set from pub
    # -- called by var.tcl
    public method RequestStreamIn {dataset pub sub} {
	LLog notice mds "request stream In for $dataset $pub -> $sub"
        if [catch {$RPC Send $sub [list RequestStreamIn $dataset $pub]} rc] {
	    LLog warning mds "streamin failed: $rc"
	}
	set rc
    }
    # -- register a publisher/subscriber
    public method Register {attrs} {
	mds_register [$RPC Peer] $attrs
    }

    # -- return our address
    public method Address {} {
        $RPC Address
    }
    # return code that for client access to MDS
    public proc API {} {
        global auto_path
        set f mdsclient.tcl
        foreach dir $auto_path {
            if [file exists $dir/$f] {
                return [exec cat $dir/$f]
            }
        }
    }

    public method Status {{dmask *}} {
        set res "MDS on [Address] \n"
        append res " -- DATA SETS\n"
        foreach ds [array names DV $dmask] {
            append res [$DV($ds) Status] \n
        }
        append res [pstats] \n
        #append res " -- SCHEDULER \n"
        set res
    }
    public method NextVersion {dset} {
	mds_next_version $dset
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private   variable N 0
    private   variable DV
    private   variable CACHEDIR ""
    protected variable RPC
    protected variable SCHED                 ;# SCHEDULER
    protected variable STAT                  ;# statistics
    protected variable VERSION               ;# keep track of ds versions
    protected variable REGISTRY              ;# peer registry
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set CACHEDIR [cache_dir_select $cachepath]

        set RPC [LRPC2 $this.rpc -address $address -server 1]
        $RPC configure -prefix "uplevel #0 $this"
        $RPC Bind <<logout>> $this eLogout
        LLog mdsd info "listening to port [$RPC cget -port]"
        set SCHED [LScheduler $this.sched]
	$SCHED Bind <<error>> $this eSchedulerError

        foreach s "xfers size dt recv dtrecv sent dtsent" {
            set STAT($s) 0
        }
    }

    destructor {
        LLog notice mds [pstats]
        $SCHED delete
        foreach ds [array names DV] {
            $DV($ds) delete
        }
    }
    # -------------------------------------------------- Private Methods ---
    private method cache_dir_select {cachepath} {
        foreach dir [split $cachepath :] {
            if ![file exists $dir] { continue }
            set t [file join $dir mds.[pid]]
            # writeable?
            if { [catch {exec touch $t} rc] } { continue }
            file delete $t
            return $dir
        }
        error "mds: no writable cache found in $cachepath"
    }
    # -------------------------------------------------------------------
    protected method do_bindings {dv name} {
        $dv Bind <<recv>> $this eRecv $name
        $dv Bind <<sent>> $this eSent $name
    }
    protected method debug {msg} {
        LLog mds info $msg
    }
    protected method notice {tag msg} {
        LLog mds notice [format "%8s %s" $tag $msg]
    }
    protected method mxfer {ds peer size dt} {
        set sz [expr $size / 1024.0 / 1024.0]
        set rt [expr $sz/($dt?$dt:1)]
	return [format "%10s %15s %5.1f MB %4.2f MB/sec" $ds $peer $sz $rt]
    }
    protected method pstats {} {
        set MB  [expr 1024.0*1024.0]
        set r            "TRANSFERS: $STAT(xfers)\n"
        foreach a "dt dtrecv dtsent" {
            if { $STAT($a) == 0 } {
                set S($a) 1
            } else {
                set S($a) $STAT($a)
            }
        }

        set rt  [expr $STAT(size)/$S(dt)/$MB]
        set rtr [expr $STAT(recv)/$S(dtrecv)/$MB]
        set rtx [expr $STAT(sent)/$S(dtsent)/$MB]

        append r [format "      TOT:%9.2f MB (%5.2f MB/sec)\n"\
                [expr $STAT(size)/$MB] $rt ]
        append r [format "       TX:%9.2f MB (%5.2f MB/sec)\n"\
                [expr $STAT(sent)/$MB] $rtx]
        append r [format "       RX:%9.2f MB (%5.2f MB/sec)\n"\
                [expr $STAT(recv)/$MB] $rtr]
        set r
    }
    # ----------------------------------------------- Interface Implementation
    protected method mds_publish {src pub opts} {
        if ![info exists DV($src)] {
            # -- generate the value object, if none exists
            set dv [MDSValue $this.mdv[incr N] -mdsc $mdsc\
                -name $src -mds $this -cachedir $CACHEDIR -sched $SCHED]
            set DV($src) $dv
            do_bindings $dv $src
        }
	# -- pass any registration
	if [info exists REGISTRY($pub)] {
	    lappend opts -publisher $REGISTRY($pub)
	}
        $DV($src) Publish $pub $opts
    }
    protected method mds_subscribe {src sub opts} {
        if ![info exists DV($src)] {
            set dv [MDSValue $this.mdv[incr N] -mdsc $mdsc\
                -name $src -mds $this -cachedir $CACHEDIR -sched $SCHED]
            set DV($src) $dv
            do_bindings $dv $src
        }
	# -- pass any registration
	if [info exists REGISTRY($sub)] {
	    lappend opts -publisher $REGISTRY($sub)
	}
        $DV($src) Subscribe $sub $opts
    }

    protected method mds_touch {src val} {
        $DV($src) Touch $val
    }

    protected method mds_unpublish {dset} {
	if ![info exists DV($dset)] {
	    return "unpublish: $dset: does not exist"
	}
        $DV($dset) Unpublish
    }

    # -- keep track of dataset name versions
    protected method mds_next_version {dset} {
	if ![regexp {^([^\.]*)\.(.*)$} $dset matchvar base suff] {
	    set base $dset
	    set suff ""
	}
	if ![info exists VERSION($base)] {
	    set VERSION($base) 0
	}
	incr VERSION($base)
	set p "$base.$VERSION($base)"
	if { $suff != "" } {
	    append p ".$suff"
	}
	set p
    }

    # -- register a peer
    protected method mds_register {peer attrs} {
	LLog notice mds "$peer: registered: '$attrs'"
	set REGISTRY($peer) $attrs
	return ok-$attrs
    }
    # ----------------------------------------------- CallBacks
    public method eLogout {fp} {
        # notify all data sets of the disconnection of a peer
        foreach ds [array names DV] {
            $DV($ds) Disconnected $fp
        }
    }
    # called at the end of the send transfer
    public method eSent {ds peer size dt} {
        notice sent "[mxfer $ds $peer $size $dt]"
        notice sent "$size bytes"
        incr STAT(xfers)
        incr STAT(sent) $size
        set  STAT(dtsent) [expr $STAT(dtsent) + $dt]
        set  STAT(size)   [expr $size + 0.0 + $STAT(size)]
        set  STAT(dt)     [expr $STAT(dt) + $dt]
    }
    public method eRecv {ds peer size dt} {
        notice recv "[mxfer $ds $peer $size $dt]"
        notice recv "$size bytes"
        incr STAT(xfers)
        incr STAT(recv) $size
        set  STAT(dtrecv) [expr $STAT(dtrecv) + $dt]
        set  STAT(size)   [expr $size + 0.0 + $STAT(size)]
        set  STAT(dt)     [expr $STAT(dt) + $dt]
    }
    # -- handle schedule errors
    public method eSchedulerError {tag cmd res} {
	LLog notice mds "scheduler: error: $tag $cmd: $res"
    }
    # -------------------------------------------------- Common Initialization
}
