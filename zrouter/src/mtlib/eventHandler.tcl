package provide mtlib 1.0
#
#       Class: LEventHandler
# Description: A friendier way to handle events
#
itcl::class LEventHandler {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable debug 0
    # -------------------------------------------------- Public Methods ----
    public method OnEvent {BODY} {
	addHandlers $BODY
    }
    # -- attach to a binder to receive events
    public method Attach {binder {eventlist *}} {
	attachToBinder $binder $eventlist
    }
    # -- can you handle an event?
    public method IsHandled {event} {
	if [info exists HANDLER($event)] { return true }
	return false
    }
    # -- handle an event
    public method Handle {event args} {
	eval [concat $this eHandleEvent $event $args]
    }
    # -- dump handlers
    public method Dump {} {
	parray HANDLER
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable HANDLER
    # -------------------------------------------------- Components
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	array set HANDLER {}
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # -- handler is an inline body of TCL code
    # -- before evaluating it we bind the following variables:
    # -- event: to event name
    # -- argv:  to the event arguments
    protected method dispatchEvent {event body argv} {
	dlog "dispatch $event ($argv) $body"
	set proc proc[clock clicks]
	proc $proc {event argv} $body
	global errorInfo
	set ei ""
	if [catch {$proc $event $argv} rc] {
	    set ei $errorInfo
	}
	rename $proc ""
	if { $ei != "" } { 
	    error $ei
	}
    }
    protected method dlog {msg} {
	if {!$debug} { return }
	puts stderr "EventHandler: $msg"
    }
    # ----------------------------------------------- Interface Implementation
    protected method addHandlers {body} {
	foreach {ev handler} $body {
	    set HANDLER($ev) $handler
	}
    }
    protected method attachToBinder {binder eventlist} {
	foreach event $eventlist {
	    if { $event != "*" } {
		$binder Bind $event $this eHandleEvent $event
	    } else {
		$binder Bind $event $this eHandleEvent
	    }
	}
    }
    # ----------------------------------------------- CallBacks
    public method eHandleEvent {event args} {
	dlog "handle: $event $args"
	set handler ""
	if [info exists HANDLER($event)] {
	    set handler $HANDLER($event)
	} else {
	    foreach tag [array names HANDLER] {
		if [string match $tag $event] {
		    set handler $HANDLER($tag)
		    break
		}
	    }
	}
	if { $handler == "" } { return }
	dispatchEvent $event $handler $args
    }
    # -------------------------------------------------- Common Initialization
    # -------------------------------------------------- TESTING
}
