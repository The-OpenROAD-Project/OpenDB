package require Itcl
package provide mtlib 1.0

#
# Object Name: LBinder
# Description: A class supporting bindings. Allows events to be bound
#            much like the tk widget binding mechanism
#
itcl::class LBinder {
    # ---------------------------------------------------- Public Variables ---
    # ---------------------------------------------------- Protected Variables 
    protected variable Bind
    protected variable debug 0
    # ---------------------------------------------------- Common Variables ---
    # ---------------------------------------------------- Configuration ------
    public method delete {} { itcl::delete object $this }
    # ---------------------------------------------------- Public Methods ------
    # -- Generate an event
    # -- call event handlers and handle wildcard * (any event handler)
    public method Event {event args} {
        global errorInfo
        if ![info exists Bind($event)] {
            if ![info exists Bind(*)] { return }
            foreach h $Bind(*) {
                set cmd [concat $h $event $args]
                if [catch {uplevel 1 $cmd} rc] {
                    global errorInfo
                    return -code error  -errorinfo $errorInfo "$rc"
                }
            }
        } else {
            foreach h $Bind($event) {
                set cmd [concat $h $args]
                if [catch {uplevel 1 $cmd} rc] {
                    return -code error  -errorinfo $errorInfo $rc
                }
            }
        }
        set rc
    }

    # -- bind an event to a (global) function - LIFO order
    public method Bind {event args} {
        if { [info exists Bind($event) ] } {
            set Bind($event) [linsert $Bind($event) 0 [list $args]]
        } else {
            set Bind($event) [list $args]
        }
    }
    # -- replace event handler -- return last bindings
    public method Bind! {event args} {
	set old $Bind($event) ; set Bind($event) ""
	lappend Bind($event) $args
	return $old
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
    # ---------------------------------------------------- Virtual Methods -----
    # ---------------------------------------------------- Private Methods -----
    # ---------------------------------------------------- Common Initialization
}
