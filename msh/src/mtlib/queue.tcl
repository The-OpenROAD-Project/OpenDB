package provide mtlib 1.0

#       Class: SQUEUE
# Description: A simple FIFO QUEUE
#      Author: Manolis M. Tsangaris  -- mtlib@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
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
	schedule $this Event <<new>>
    }
    public method Deq {} {
        set item [lindex $QUEUE 0]
        set QUEUE [lrange $QUEUE 1 end]
        if { ($QUEUE == "") && ($task1 == "") } {
            set task1 [after idle $this eEvent1 <<empty>>]
        }
        return $item
    }
    public method SystemStatus {} {
        format "%20s %s items" $name [llength $QUEUE]
    }
    # -- peek the top of the queue
    public method Peek {} {
	lindex $QUEUE 0
    }
    # -- reque a task
    public method Requeue {item} {
	set QUEUE [concat $item $QUEUE]
	schedule $this Event <<new>>
	return ""
    }
    # -- queue size
    public method Size {} { llength $QUEUE }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable QUEUE ""
    protected variable task1 ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor {
        if { $task1 != "" } {
            after cancel $task1
        }
    }
    # -------------------------------------------------- Private Methods ---
    protected method schedule {args} {
	after idle $args
    }
    # ----------------------------------------------- Interface Implementation
    # -- delayed event
    public method eEvent1 {ev} {
        set task1 ""
        Event $ev
    }
    # -------------------------------------------------- Common Initialization
}
