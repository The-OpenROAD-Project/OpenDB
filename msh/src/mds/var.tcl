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
package require mtlib

package provide mds 1.0
# --------------------------------------------------------------------------
#       Class: MDSValue
# Description: MDS value object, representing a shared value, using MDSC
#      Author: Manolis M. Tsangaris
#
# P2P: in this mode, the subscriber will directly request the 
# data set from the publisher, but the rest of the protocol
# will go through correctly
#
itcl::class MDSValue {
    inherit LObject  LBinder
    # Event <<sent>> sock bytes time
    # Event <<recv>> sock bytes time
    # -------------------------------------------------- Public Variables --
    public variable name  ""        ;# data value name
    public variable mds "" 
    public variable cachetime 10    ;# how long to cache values (in sec)
    public variable cachedir /tmp   ;# where to cache values
    public  variable sched ""       ;# scheduler
    public  variable mdsc 1         ;# should we use mdsc?
    # -------------------------------------------------- Public Methods ----
    # -- note the publisher
    public method Publish {pub opts} {
        message $pub "publishing ($opts)"
        if { $PUB == $pub } {
            message $pub "already publishing"
            return "OK {already publishing $name}"
        }
	set p2p [option_get $opts -p2p 0]
	if { $P2P  == "" } {
	    set P2P $p2p
	} elseif { $P2P != $p2p } {
            message $pub "P2P mode already set to $P2P (requested $p2p)"
            return "OK {$name in P2P mode (ignoring p2p=$p2p) }"
	}
        set PUB $pub

	# -- collect publisher information (from its registration)
	set pi [option_get $opts -publisher]
	message $pub "publisher info: $pi"
	array set PUBINFO $pi

        # -- any subscribers?
        if { ![array size SUB] } { return "OK {no subscribers}" }

	if { $P2P } {
	    $sched Run req-stream-in.$name [list $this eRequestStreamIn]
	} else {
	    # -- request a value from publisher
	    $sched Run req-post.$name [list $this eRequestPost]
	}
        return "OK {publisher accepted for $name}"
    }

    # -- Put: accept data from socket and store them in the cache
    public method Put {channel} {
        set dob [clock seconds]
        set STATE loading
	if { $mdsc } {
	    message receiving "$channel to dset $dset"
	    mdsc::copy from $dset $channel \
		-callback "$this eMCopyPutDone $channel"
	} else {
	    set fp [open $cachefile w]
	    fconfigure $fp -buffering full -encoding binary -translation binary
	    message receiving "$channel to $cachefile"
	    fcopy $channel $fp -command "$this eCopyPutDone $channel $fp $dob"
	}
    }

    # -- note the subscriber
    public method Subscribe {sub opts} {
        message $sub "subscribing"
        if [info exists SUB($sub)] {
            message $sub "already a subscriber"
            return "OK {already subscribed to $name}"
        }
	set p2p [option_get $opts -p2p]
	if { $P2P  == "" } {
	    set P2P $p2p
	} elseif { $P2P != $p2p } {
            message $sub "P2P mode already set to $P2P (requested $p2p)"
            return "OK {$name in P2P mode (ignoring p2p=$p2p) }"
	}
        set SUB($sub) ""
        set SENT($sub) 0

        # schedule the value to arrive, if none exists
        switch $STATE {
            invalid {
		if { $P2P } {
		    return [p2p_handle $sub]
		}
                message $STATE "will request it"
		set STATE requesting
                $sched Run req-post.$name [list $this eRequestPost]
                return "OK {sub accepted. $name will be requested}"
            }
	    requesting {
                message $STATE "already requesting it"
                return "OK {sub accepted. $name is being requested}"
		return
	    }
            loading {
                message $STATE "being loaded"
                return "OK {sub accepted. $name is loading}"
            }
            ready {
                message $STATE "ready - will stream"
                $this eSendAll $sub
                return "OK {sub accepted. $name ready & will stream}"
            }
        }
    }
    public method Get {channel} {
        set now [clock seconds]
	if { $mdsc } {
	    message "sending" "$dset to $channel"
	    mdsc::copy to $dset $channel\
		-callback "$this eMCopyGetDone $channel"
	} else {
	    set fp [open $cachefile r]
	    fconfigure $fp -buffering full -encoding binary -translation binary
	    message "sending" "$cachefile to $channel"
	    fcopy $fp $channel -command "$this eCopyGetDone $channel $fp $now"
	}
    }

    # -- we should probably do more error checking here
    # -- this data set is no longer subscribable
    # -- returns an error message
    public method Unpublish {} {
	# -- no need to do much, in p2p mode
	if { $P2P } { 
	    cleanup
	    return ok
	}
	if { $STATE != "ready" } {
	    message "unpublish" "no data set loaded"
	    return "unpublish: $name: no data set loaded"
	}
	cleanup
	return "ok"
    }

    public method Clear {} {
        cleanup
    }

    public method Disconnected {peer} {
        if [info exists SUB($peer)] {
            # cancel a pending Send to this peer (if any)
            $sched Cancel send-$peer.$name
            unset SUB($peer)
        }
        if { $PUB == $peer } { set PUB "" }
    }
    public method Status {} {
        set out ""
        append out "Dataset: $name" \n
        append out "    DOB: [clock format $dob]" \n
	if { $mdsc } {
	    append out "  STATE: $STATE    DSET: $dset" \n
	} else {
	    append out "  STATE: $STATE    CACHE: $cachefile" \n
	}
        append out "$name ($STATE):" \n
        foreach pub $PUB {
            append out "[peer_info $pub]"
        }
        append out " -> "
        foreach sub [array names SUB] {
            append out "[peer_info $sub]"
            if {$SENT($sub)} {
                append out "(+)"
            } else {
                append out "(-)"
            }
        }
        append out "\n"
        set out
    }
    # -- update the dataset value (unimplemented)
    public method Touch {val} {
    }

    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable STATE invalid  ; # variable state
    protected variable PUB ""      ; # publisher
    protected variable SUB         ; # subscribers
    protected variable dob   0     ; # data of birth of data
    protected variable cachefile "" ;# file to save data 
    protected variable SENT         ;# have I sent variable to SUB?
    protected variable dset   ""    ;# memory based data set
    protected variable P2P  ""      ;# is this a p2p data set?
    protected variable P2P_ADDRESS "";# P2P publisher address
    protected variable PUBINFO      ;# publisher information
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set f [exec uname -n].[pid].[namespace tail $name].mds
	if { $mdsc } {
	    package require Mdsc 1.0
	    set dset [mdsc::memobj create]
	    notice "using multithreaded MDSC"
	} else {
	    notice "using soft MDSC"
	    set cachefile [file join $cachedir $f]
	}
    }
    destructor {
        cleanup
	if { $dset != "" } { $dset delete }
    }
    # -------------------------------------------------- Private Methods ---
    # cleanup the state
    protected method cleanup {} {
        if { [file exists $cachefile]} {
	    message "unpublish" "$cachefile: removing"
            file delete -force $cachefile
        }
	if { $dset != "" } { mdsc::memobj clear $dset }
        set STATE invalid
    }
    protected method notice {msg} {
        LLog mdsval notice "$name: $msg"
    }
    protected method message {m1 m2} {
        LLog mdsval notice [format "%8s %10s: %s" $name $m1 $m2]
    }
    # -- return peer information
    private method peer_info {peer} {
        if [catch {fconfigure $peer -peername} rc] { return $peer?  }
        foreach {_ host port} $rc {break}
        return $port@[lindex [split $host .] 0]
    }
    # -- handle a peer to peer subscription request
    protected method p2p_handle {sub} {
	if { $PUB == "" } { 
	    message P2P "no publisher yet"
	    return "OK {sub accepted to $name. Wating for publishers}"
	}
	$sched Run req-stream-in.$name [list $this eRequestStreamIn]
	return "OK {Sub accepted. Stream-in requested}"
    }
    # -- parse options and return opt
    protected method option_get {opts opt {default ""}} {
	foreach {op val} $opts {
	    if { "@$op" == "@$opt" } { return $val }
	}
	return $default
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # what to do at the end of a Put
    public method eCopyPutDone {channel fp stime size {rc ""}} {
        set dt [expr [clock seconds] - $stime]
        set STATE ready
        set pi [peer_info $channel]
        catch {close $fp}
        catch {close $channel}

        Event <<recv>> $pi $size $dt

        if { $rc != "" } {
            notice "   fcopy: err: $rc"
        }

        # schedule to send value to all of them
        $this eSendAll
        return ""
    }

    # what to do after a get is done
    public method eCopyGetDone {channel fp stime size {rc ""}} {
        set dt [expr [clock seconds] - $stime]

        set pi [peer_info $channel]
        catch {close $fp}
        catch {close $channel}

        Event <<sent>> $pi $size $dt

        if { $rc != "" } {
            notice "   fcopy: err: $rc"
        }
    }

    # ***************** MDSC CASE ****************************
    # -- what to do at the end of a put
    public method eMCopyPutDone {channel attrs} {
	array set A $attrs

	notice "mdsc: copy put done: $A(status)"

        set dt [expr $A(tlast) - $A(tstart)]
        set STATE ready
        set pi [peer_info $channel]
        catch {close $channel}

	set size $A(ncopied)

        Event <<recv>> $pi $size $dt

	switch $A(status) {
	    error { notice "mdsc copyget: error: $A(error)" ; return }
	}

        # schedule to send value to all of them
        $this eSendAll
        return ""
    }

    # what to do after a get is done
    public method eMCopyGetDone {channel attrs} {
	array set A $attrs

	notice "mdsc: copy get done: $A(status)"
        set dt [expr $A(tlast) - $A(tstart)]

        set pi [peer_info $channel]
        catch {close $channel}

        Event <<sent>> $pi $size $dt

	switch $A(status) {
	    error { notice "mdsc copyget: error: $A(error)" }
	}
    }
    # ---------------------------------------------------------------------

    # schedule all subscribers to retrieve the new value
    public method eSendAll {{match *}} {
        foreach sub [array names SUB $match] {
            $sched Run send-one-$name-$sub [list $this eSendOne $sub]
        }
    }

    # ask this particular subscriber to retrieve
    public method eSendOne {sub} {
        # check if this is still a subscriber ...
        if ![info exists SUB($sub)] { return }
        if { $SENT($sub) } { return }
        $mds RequestGet $name $sub
        incr SENT($sub)
    }

    # request a publisher to send us its content
    public method eRequestPost {} {
        set pub $PUB
        if { $pub == "" } {
            message requesting "no publisher yet"
            return
        }
        message requesting "from: $pub"
        $mds RequestPost $name $pub
    }

    # --- P2P
    # -- ask subscribers to directly receive a value from publishers
    public method eRequestStreamIn {} {
        if { $PUB == "" } {
            message streamIn "no publisher yet"
            return
        }
	set pub $PUBINFO(p2p)
        foreach sub [array names SUB] {
	    # -- subscriber address
            message streamIn "scheduling stream in $pub->$sub"
	    set tag stream-in-$name-$sub 
	    set cmd [list $mds RequestStreamIn $name $pub $sub]
            $sched Run $tag $cmd
        }
    }
    # -------------------------------------------------- Common Initialization
}
