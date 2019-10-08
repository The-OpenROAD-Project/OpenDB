package provide ade::ui 2.0

#
#       Class: ZLoader
# Description: Load a stream
#      Author: Manolis M. Tsangaris
#
# ZLoader is responsible for loading a design into the zui canvas
#
itcl::class ZLoader {
    inherit LBinder
    # Event <<stats>>            ;# interesting performance stats
    # Event <<ade-event>> args   ;# pass a remote ADE event
    # -------------------------------------------------- Public Variables --
    public variable cv ""
    public variable echo 1               ;# echo remote ade command?
    public variable debug 0
    public variable nlines 1000
    public variable rpc    "" {
	$rpc Bind * $this Event <<ade-event>>
    }
    # -------------------------------------------------- Public Methods ----
    # -- make sure we register ...
    public method Init {} {
	if [catch {$rpc Run GUI::Check} rc] {
	    warning "zui" "failed to register: $rc"
	    # -- it is possible the backend does not support that yet
	    return
	}
	set user [LPlatform user]
	set reg [list user $user host [info hostname]]
	# -- obtain the registration ID
	set RID [$rpc Run gui Register $reg]
	set crs [$rpc Run gui Crawlers]
	if { $crs == "" } { return }
	CrawlerSet [lindex $crs 0]
    }
    public method Load {chip} {
    }
    # -- get all objects
    # -- pipe the output to streaming call upcall, if specified
    public method Get {chip bbox filters samples gx gy {upcall ""}} {
        chip_get $chip $bbox $filters $samples $gx $gy $upcall
    }
    public method GetCursor {chip bbox filters} {
        chip_get_cursor $chip $bbox $filters
    }
    public method GetAttr {chip} {
        CRAWL chip_attr_get -def_types $chip
    }
    public method GetTiles {chip} {
        get_tiles $chip
    }
    public method Connected {status} {
        set connected $status
    }
    public method ObjInspectChoices {chip obj} {
        CRAWL inspect -chip $chip $obj
    }
    # inspect an object
    public method ObjInspect {chip obj info bbox filters} {
        # -- flatten filters
        array set F "layer {} hierarchy {} type {}"
        set filters [filter_flatten $chip $filters] ; array set F $filters

        CRAWL inspect -chip $F(block) -info $info \
                -bbox $bbox -layer $F(layer) -type $F(type) -attr $F(class) $obj
    }
    public method ObjInspectCursor {chip obj info bbox filters} {
        # -- flatten filters
        array set F "layer {} hierarchy {} type {}"
        set filters [filter_flatten $chip $filters] ; array set F $filters

        CRAWL inspect -chip $F(block) -info $info \
                -bbox $bbox -layer $F(layer) -noclip -q -attr $F(class)\
		-type $F(type) $obj
    }

    # -- return object information
    public method ObjInfo {chip obj} {
        lindex [CRAWL inspect -chip $chip -info self $obj] 0
    }

    # -- return a message describing an object
    public method ObjMessage {chip bbox filters obj} {
        # flatten filters
        array set F "layer {} hierarchy {} type {}"
        set filters [filter_flatten $chip $filters] ; array set F $filters

        lindex [CRAWL inspect -chip $F(block) \
            -info msg/msg $obj ] 0
    }

    # -- return a message describing an object for peeking (mouseover)
    public method ObjPeek {chip bbox filters obj} {
        # flatten filters
        array set F "layer {} hierarchy {} type {}"
        set filters [filter_flatten $chip $filters] ; array set F $filters

        lindex [CRAWL inspect -chip $F(block) \
            -info msg/msg $obj ] 0
    }

    public method Find {chip a b c bbox filters} {
        array set F "layer {} hierarchy {} type {}"
        set filters [filter_flatten $chip $filters] ; array set F $filters

	set res ""
	foreach c1 $c {
	    set r [CRAWL inspect -chip $F(block) \
		-bbox $bbox -layer $F(layer) -type $F(type) -attr $F(class)\
		-s $a/$b $c1]
	    set res [concat $res $r]
	}
	set res
    }

    public method MSH {args} {
        eval RPC ::msh::$args
    }
    #
    # given a group return its members
    #
    public method GetGroup {chip obj {bbox ""}} {
        CRAWL inspect -chip $chip -info conn $obj
    }
    # return all available crawlers
    # a list of crawler {crawler attrs}
    public method Crawlers {} {
	return [RPC gui Crawlers]
        set out ""
        #lappend out default [list title Default tm "" cr ""]
        #lappend out test    [list title Test    tm "" cr ""]
	return [concat $out [RPC gui Crawlers]]

        foreach cr [RPC itcl::find objects -class Dcr] {
            set crn [namespace tail $cr]
            lappend out $cr [list title $crn tm "" cr ""]
        }
        set out
    }
    # -- set the default crawler to use subsequently
    public method CrawlerSet {cr} {
        switch @$cr {
            @default - @test { set CR "" }
            default {
                set CR $cr
            }   
        }
    }
    public method Whoami {} {
        RPC whoami
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable connected 0
    protected variable CR ""             ;# default crawler to use
    protected variable RID ""            ;# registration ID
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        LBridge debug "$rpc configure -debug"
    }
    destructor {
	if { $RID != "" } {
	    $rpc Run gui UnRegister $RID
	}
    }
    # -------------------------------------------------- Private Methods ---
    # get a number of objects under bbox ...
    private method chip_get {chip bbox filters samples gx gy upcall} {
        set filters [filter_flatten $chip $filters]
        set bbox [fixbbox $bbox]
        array set F "layer {} hierarchy {} type {} class {}"
        array set F $filters
        SCRAWL $upcall chip_get -chip $F(block) -bbox $bbox \
            -layer $F(layer) -hierarchy $F(hierarchy) -type $F(type) \
	    -attr $F(class) -gx $gx -gy $gy -maxobjects $samples
	 # -- add those to get via display ok
	 # -noclip -q
    }

    # get a part of the chip under the cursor
    private method chip_get_cursor {chip bbox filters} {
        set filters [filter_flatten $chip $filters]
        set bbox [fixbbox $bbox]
        array set F "layer {} hierarchy {} type {}"
        array set F $filters
        CRAWL chip_get -chip $F(block) -bbox $bbox \
                -layer $F(layer) -hierarchy $F(hierarchy) -type $F(type) \
                -noclip -q
    }
    private method get_tiles {chip} {
        CRAWL tiles_get $chip
    }
    protected method RPC {args} {
        set res ""
        set now [clock clicks -milliseconds]
        if { $connected } {
	    # -- echo the command on the remote ade
	    if { $echo } { $rpc Run puts $args }
	    # -- run the actual command
            set res [eval $rpc Run $args]
        } else {
            notice rpc "[lindex $args 0]: not connected"
        }
        set dt [expr ([clock clicks -milliseconds ]- $now)/1000.0]
        if { $dt } {
	    Event <<stats>> \
		[format "request: %s: %.2f second(s)" [lindex $args 1] $dt]
        }
        set res
    }
    # -- run a command, possibly using a crawler
    # -- will go way when the default crawler is implemented
    protected method CRAWL {cmd args} {
        # -- if no crawler, use the old style
        if { $CR == "" } { return [eval RPC $cmd $args] }

        set av  $args
        switch $cmd {
            inspect {
                # -- in case of inspect, we need to change the last free
                # -- argument to -obj
                set av [concat [lrange $av 0 end-1] -obj [lindex $av end]]
            }
        }
        eval RPC $CR $cmd $av
    }
    # -- run a command, using a crawler, possibly in streaming mode
    protected method SCRAWL {upcall args} {
	if { $upcall == "" } { 
	    return [eval CRAWL $args]
	}
        if { $CR == "" } {
	    error "no streaming is supported for legacy crawler"
	}
	# -- pass "-channel" to enable streaming
	set rcmd [concat $CR $args -channel]
	# -- use this one to test streaming
	#set rcmd [concat rstream $CR $args]
	$rpc Run puts "$rcmd"
	#puts LOCAL:$upcall ; puts REMOTE:$rcmd

	# -- start streaming - run upcall to receive the data
        $rpc Stream $rcmd $upcall
    }
    proc fixbbox {bbox} {
        if { $bbox == "" } { return "" }
        foreach {x0 y0 x1 y1} $bbox {break} 
        if { $x0 > $x1 } { set x $x1 ; set x1 $x0 ; set x0 $x } 
        if { $y0 > $y1 } { set y $y1 ; set y1 $y0 ; set y0 $y } 
        list $x0 $y0 $x1 $y1
    }
    # -------------------------------------------------------- filter handling
    # -- flatten a filter expression
    # -- if block is not defined, just pass $chip
    protected method filter_flatten {chip filter} {
        if { $filter == "" } {
	    return [list block $chip]
	}
        array set F {}
        foreach {class vclass} $filter {
            array set A {}
            foreach {attr val} $vclass {
                if { $val == 1 } {
                    set A($attr) 1
                }
            }
            set F($class) [array names A]
            unset A
        }
	# -- if no block is defined, revert to chip mode
	if { $F(block) == "" } { set F(block) $chip }
        array get F
    }
    # -------------------------------------------------- Common Initialization
}
