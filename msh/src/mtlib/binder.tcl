package require Itcl

# -- NOTICE THIS FILE IS PART OF THE FREELY AVAILABLE OPENSOURCE
# -- STD LIBRARY MTLIB
package provide mtlib 1.0

#
# Object Name: LBinder
# Description: A class supporting bindings. Allows events to be bound
#            much like the tk widget binding mechanism
#      Author: Manolis M. Tsangaris -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
itcl::class LBinder {
	# ---------------------------------------------------- Public Variables ---
	# ---------------------------------------------------- Protected Variables 
	protected variable Bind
	# ---------------------------------------------------- Common Variables ---
	# ---------------------------------------------------- Configuration ------
	public method delete {} { itcl::delete object $this }
	# ---------------------------------------------------- Public Methods ------
	# -- Generate an event
	# -- call event handlers and handle wildcard * (any event handler)
	public method Event {event args} {
		if ![info exists Bind($event)] {
			if ![info exists Bind(*)] {
				return
			}
			foreach h $Bind(*) { set n [uplevel 1 [concat $h $event $args]] }
		} else {
			foreach h $Bind($event) {
				set cmd [concat $h $args]
				if [catch {uplevel 1 $cmd} n] {
					global errorInfo 
					LLog binder warning "event $event $args: $n"
					puts stderr $errorInfo
				}
			}
		}
		set n
	}

	# -- bind an event to a (global) function
	public method Bind {event args} {
		lappend Bind($event) $args
	}

	# -- remove all event bindings
	public method UnBind {event} { unset Bind($event) }

	# -- redirect an event to a different handler object
	public method Redirect {event binder} {
		lappend Bind($event) [list $binder Event $event]
	}

	# -- return the event handler
	public method Handler {event} {
		if ![info exists Bind($event)] {
			return ""
		}
		set Bind($event)
	}
	# -- return all events currently registered
	public method Events {} {
		array names Bind
	}
	# ---------------------------------------------------- Virtual Methods -----
	# ---------------------------------------------------- Private Methods -----
	# ---------------------------------------------------- Common Initialization
}

#
# Object Name: LPBinder
# Description: A class supporting structured events
#      Author: Manolis M. Tsangaris -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2003
#
#     The event field can be a file path and bindings can match the path
#     Handlers can also control the argument list they receive.
#     For a good example in See tests/binder.tcl
#
itcl::class LPBinder {
	# --------------------------------------------------- Public Variables ---
	# ---------------------------------------------------- Configuration ------
	public method delete {} { itcl::delete object $this }
	# ---------------------------------------------------- Public Methods ------
	# -- Generate an event
	# -- call event handlers and handle path expressions
	public method Event {event args} {
		$this bind_event $event $args
	}
	# -- bind an event to a function
	public method Bind {event handler} {
		$this bind_bind $event $handler
	}

	# -- remove all event bindings
	public method UnBind {event {handler *}} {
		$this bind_unbind $event $handler
	}
	public method UnCache {event} {
		$this bind_uncache $event
	}

	# ---------------------------------------------------- Protected Variables 
	private variable BIND
	private variable HANDLERS
	private variable CACHE
	# ---------------------------------------------------- Common Variables ---
	protected method bind_event {event arglist} {
		set handler [binder_handler_find $event]
		if { $handler == "" } { return "" }
		uplevel 1 $handler
	}
	protected method bind_bind  {event handler} {
		# -- invalidate the cache
        catch {unset CACHE}
		# -- remeber bindings
        set BIND($event) $handler
        lappend HANDLERS $event $handler
        return ""
	}
	protected method bind_unbind {event arglist} {
		error "unsupported"
	}
	protected method bind_uncache {event} {
		catch {unset CACHE($event)}
	}
    # -------------------------------------------------- 
	public method binder_handler_find {path} {
		if ![info exists CACHE($path)] {
			set handler [$this binder_handler_find1 $path]
			if { $handler == "" } { return "" }
			set handler [handler_expand $handler $path]
			set CACHE($path) $handler
		}
		set handler $CACHE($path)
	}
	# -- 
	protected method binder_handler_find1 {path} {
		# -- exact match ?
		if [info exists BIND($path)] {
			return $BIND($path)
		}
		# -- skip initial slash
		set lpath [lrange [split $path /] 1 end]
		foreach {pathspec handler} $HANDLERS {
			# -- skip initial slash
			set pathspec [lrange [split $pathspec /] 1 end]
			if [binder_handler_match $lpath $pathspec] {
				return $handler
			}
		}
		return ""
	}
	protected method handler_expand {handler path} {
		set ohandler $handler
		# %P matches the path
		# %A is all the arguments
		regsub -all %P $handler $path handler
		regsub -all %A $handler [lrange [split $path /] 1 end] handler
		set n 0
		# %N matches the N-th component of the path
		foreach p [split $path /] {
			regsub -all %$n $handler $p handler
			incr n
		}
		set handler
	}
	# -- *  matches a path component
	# -- ** matches the remainder of the path
	protected method binder_handler_match {path pattern} {
		set n -1
		# -- compare the two patterns
		foreach p $pattern {
			incr n ; set p1 [lindex $path $n]
			if { $p1 == ""   } { return 0 }
			if { $p1 == $p   } { continue }
			if { $p  == "*"  } { continue }
			if { $p  == "**" } { return 1 }
			return 0
		}
		incr n ; set p1 [lindex $path $n]
		if { $p1 != {} } { return 0 }
		return 1
	}
	# ------------------------------------------------------------------------- 
    # -------------------------------------------------- Common Initialization
}
