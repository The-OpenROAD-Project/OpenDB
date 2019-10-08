package provide mtlib 1.0

#       Class: SQUEUE
# Description: A simple FIFO QUEUE
#
itcl::class LSQUEUE {
    inherit LBinder
    # Event <<empty>> ;# -- empty queue
    # Event <<new>>   ;# -- new item in the queue
    # -------------------------------------------------- Public Variables --
    public variable policy fifo     ;# -- policy of the queue
    public variable name "default"  ;# -- queue name
    # -------------------------------------------------- Public Methods ----
    public method Enq {item} {
        lappend QUEUE $item
        after idle $this Event <<new>>
    }
    public method Deq {} {
        set item [lindex $QUEUE 0]
        set QUEUE [lrange $QUEUE 1 end]
        if { ($QUEUE == "") && ($task == "") } {
            set task [after idle $this eEvent <<empty>>]
        }
        return $item
    }
    public method SystemStatus {} {
        format "%20s %s items" $name [llength $QUEUE]
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable QUEUE ""
    protected variable task ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor {
	if { $task != "" } {
	    after cancel $task
	}
    }
    # -------------------------------------------------- Private Methods ---
    # -- delayed event
    public method eEvent {ev} {
	set task ""
	Event $ev
    }
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}

