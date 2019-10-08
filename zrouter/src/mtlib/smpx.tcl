package provide mtlib 1.0

#
# Theory of Operation
# SMPX is a scheduler server serving collaborating clients.
#

#
#       Class: LSMPXServer
# Description:
#
#
itcl::class LSMPXServer {
    inherit LObject LInteractor
    # -------------------------------------------------- Public Variables --
    public variable address 7679@
    # -------------------------------------------------- Public Methods ----
    # -- register a resource of $type on $address
    public method Register {type address} {
	upvar peer peer
	if ![info exists POOL($type)] {
	    set POOL($type) [ServerPool #auto -type $type]
	    $POOL($type) Bind <<reserved>> $this eReserved
	    $POOL($type) Bind <<released>> $this eReleased
	}
	$POOL($type) Add $type $address
    }
    # -- unregister a resource
    public method UnRegister {type address} {
	upvar peer peer
	$POOL($type) Remove $type $address
    }
    public method Reserve {type} {
	upvar peer peer
	$POOL($type) Reserve $peer $type
    }
    public method Release {type address} {
	upvar peer peer
	$POOL($type) Release $peer $address
    }
    public method Info {} {
	list address [$rpc Address]
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable POOL  ;# server pool by type
    protected variable rpc ""
    protected variable qrpc ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set rpc [LRPC2 $this.srv -address $address -server 1\
	    -prefix "eval $this"]
	set qrpc [LAsyncEval $this.async]
	$qrpc Bind <<error>>  $this eAsyncError
	$qrpc Bind <<return>> $this eAsyncReturn
    }
    destructor {
	foreach p [array names POOL] {
	    $POOL($p) delete
	}
	$qrpc delete
	$rpc delete
    }
    # -------------------------------------------------- Private Methods ---
    # -- run a command asyncrounsly and send a reply to the peer
    protected method async_reply {peer cmd} {
	set seq [$qrpc Run [list $rpc Send $peer $cmd] $peer]
	set seq
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -- called after idle
    public method eReturn {peer cmd argv} {
	notice smpx "async return $cmd <$argv>"
	set cmd [concat $cmd $argv]
	$rpc Send $peer $cmd
    }
    public method eReserved {peer status address} {
	async_reply $peer [list eReserved $status $address]
    }
    public method eReleased {peer status address} {
	async_reply $peer [list eReleased $status $address]
    }

    # -- handle errors
    public method eAsyncError {seq tags rc errorInfo} {
	notice smpx "command: $seq: $tags: $rc"
	foreach line [split $errorInfo \n] {
	    notice smpx "$line"
	}
	notice smpx "----"
    }
    # -- handle return value
    public method eAsyncReturn {seq tags rc} {
	notice smpx "command: $seq: $tags: $rc"
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: ServerPool
# Description: manage a server pool
#
itcl::class ServerPool {
    inherit LObject LBinder
    # Event <<reserved>> status address
    # Event <<released>> status address
    # -------------------------------------------------- Public Variables --
    public variable type ""
    # -------------------------------------------------- Public Methods ----
    public method Add {type address} {
	if [info exists SERVER($address)] {
	    error "server $address already registered"
	}
	set srv [Server #auto -address $address -type $type]
	set SERVER($address) $srv
	notice smpx "adding $srv as $address $type"
	return ok
    }
    public method Remove {type address} {
	if ![info exists SERVER($address)] {
	    error "server $type/$address not registered"
	}
	if { [$SERVER($address) cget -busy] } { 
	    error "server $type/$address is busy now"
	}
	$SERVER($address) delete
	unset SERVER($address)
	return ok
    }

    # -- queue the request to reserve a server
    public method Reserve {peer type} {
	notice smpx "reserve $peer $type (queued)"
	$queue Enq [list reserve $peer $type]
	return ok
    }
    # -- release a server
    public method Release {peer address} {
	notice smpx "release $peer $type"
	foreach tag [array names SERVER] {
	    set srv $SERVER($tag)
	    if { [$srv cget -address] == $address } { 
		$srv configure -busy 0 -client ""
		eNewQueueItem
		return ok
	    }
	}
	return error
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable queue
    protected variable SERVER
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set queue [LSQUEUE $this.queue]
	$queue Bind <<new>> $this eNewQueueItem
    }
    destructor {
	foreach tag [array names SERVER] {
	    $SERVER($tag) delete
	}
	$queue delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eNewQueueItem {} {
	set item [$queue Peek]
	if { $item == "" } { return }
	foreach {cmd peer type} $item { break }
	switch $cmd {
	    reserve {
		foreach tag [array names SERVER] {
		    set srv $SERVER($tag) 
		    if { [$srv cget -busy] } { continue }
		    # -- remove item from the queue
		    $queue Deq
		    set ad [$srv cget -address]
		    # -- assign this peer to this server
		    notice smpx "reserving $ad ($tag) for $peer"
		    $srv configure -busy 1 -client $peer
		    Event <<reserved>> $peer ok $ad
		    return
		}
	    }
	}
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: Server
# Description: Manage a server
#      Author: Manolis M. Tsangaris
#
itcl::class Server {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable address ""
    public variable type ""
    public variable busy 0 ;# is it busy?
    public variable client "" ;# who is using it?
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}


#
#       Class: LSMPXClient
# Description:
#
itcl::class LSMPXClient {
    inherit LObject LBinder
    # -------------------------------------------------- Public Variables --
    public variable address 7679@
    # -------------------------------------------------- Public Methods ----
    public method Register {type address} {
	$rpc Send $peer [list Register $type $address]
    }
    public method Reserve {tag} {
	$rpc Send $peer [list Reserve $tag]
    }
    public method Release {type tag} {
	$rpc Send $peer [list Release $type $tag]
    }
    public method UnRegister {type tag} {
	$rpc Send $peer [list UnRegister $type $tag]
    }
    public method Wait {st} {
	upvar $st status
	catch {unset status}
	notice smpx-client waiting
	set r [$sema Wait]
	array set status $STATUS
	set r
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable CONF
    protected variable rpc ""
    protected variable peer ""
    protected variable sema ""
    protected variable STATUS ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set sema [LSemaphore $this.sema]
	set rpc  [LRPC2 $this.srv -address $address -prefix "eval $this"]
	set peer [$rpc Connect $address]
    }
    destructor {
	$sema delete
	$rpc Disconnect $peer
	$rpc delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eReserved {status {address ""}} {
	set STATUS [list status $status address $address cmd reserved]
	$sema Signal
    }
    public method eReleased {status address} {
	set STATUS [list status $status cmd released address $address]
	$sema Signal
    }
    # -------------------------------------------------- Common Initialization
}
