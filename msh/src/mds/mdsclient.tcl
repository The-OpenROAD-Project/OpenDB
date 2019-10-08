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

package provide mdsclient 1.0

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
	public method Publish {dataset {callback ""}} {
		$this mds_publish $dataset $callback
	}
	public method UnPublish {dataset} {
		$this mds_unpublish $dataset
	}

	# declare the intention to subscribe
	public method Subscribe {dataset {callback ""}} {
		$this mds_subscribe $dataset $callback
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
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
	private variable rpc            ;# rpc instance
	private variable peer           ;# peer connected to
	private variable PUB            ;# Publisher callbacks
	private variable SUB            ;# Subscriber callbacks
	private variable SEMA           ;# Semaphore to wait on datasets 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
		eval configure $args
		set rpc [LRPC2 $this.rpc -server 0 -prefix "uplevel #0 $this"]
		set peer [$rpc Connect $address]
		LLog mdsclient debug "connected to $address"
		array set SEMA {}
	}
    destructor {
		foreach ds [array names SEMA] {
			$SEMA($ds) delete
		}
		$rpc delete
	}
    # -------------------------------------------------- Private Methods ---
	# ----------------------------------------------- Interface Implementation
	# -- publish a dataset
	protected method mds_publish {dataset callback} {
		set PUB($dataset) $callback
		if ![info exists SEMA($dataset)] {
			set SEMA($dataset) [LSemaphore $this.$dataset]
		}
		$rpc Send $peer [list Publish $dataset]
	}
	# -- unpublish a dataset
	protected method mds_unpublish {dataset} {
		set PUB($dataset) ""
	}

	# -- declare the intention to subscribe
	protected method mds_subscribe {dataset callback} {
		set SUB($dataset) $callback
		set SEMA($dataset) [LSemaphore $this.$dataset]
		$rpc Send $peer [list Subscribe $dataset]
	}
	# -- unsubscribe
	protected method mds_unsubscribe {dataset} {
		set SUB($dataset) ""
	}

	# -- Wait for the dataset to arrive
	protected method mds_wait {dataset} {
		$SEMA($dataset) Wait
	}

	# -- A post has been requested
	protected method mds_request_post {dataset} {
		after idle $this eMDSPost $dataset
		return "OK {$dataset will be posted}"
	}

	# -- A get has been requested
	# -- request for new data to be retrieved
	protected method mds_request_get {dataset} {
		after idle $this eMDSGet $dataset
		return "OK {$dataset will be retrieved}"
	}
	# ----------------------------------------------- CallBacks
	# -- send the dataset to MDS
	public method eMDSPost {dataset} {
		$rpc SendStream $peer [list $this eSend $dataset] [list Post $dataset]
	}
	# -- send a dataset to the server using channel
	public method eSend {dataset channel} {
		LLog mdsc info "sending: $dataset => $channel"
		set cmd $PUB($dataset) ; lappend cmd $channel
        fconfigure $channel -buffering full -encoding binary -translation binary
		uplevel #0 $cmd
		LLog mdsc info "sent: $dataset => $channel"
		catch {close $channel}

		$SEMA($dataset) Signal
	}

	# -- request this dataset from the MDS server
	public method eMDSGet {dataset} {
		$rpc SendStream $peer [list $this eRecv $dataset] [list Get $dataset]
	}

	# -- receiver for this dataset from the server using channel
	public method eRecv {dataset channel} {
		LLog mdsc info "$this: [info hostname]"
		LLog mdsc info "$this: receiving: $dataset <= $channel"
		set cmd $SUB($dataset) ; lappend cmd $channel
        fconfigure $channel -buffering full -encoding binary -translation binary
		uplevel #0 $cmd
		LLog mdsc info "$this: recvd: $dataset <= $channel"
        catch {close $channel}

		$SEMA($dataset) Signal
	}
    # -------------------------------------------------- Common Initialization
}
