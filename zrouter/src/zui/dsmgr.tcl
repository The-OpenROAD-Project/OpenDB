package provide ade::ui 2.0

#       Class: ZCDSFactory
# Description: ZCanvas Data Set Factory
#      Author: Manolis M. Tsangaris
#
# Manage data sets

itcl::class ZCDSFactory {
    inherit LObject LBinder
    # Event <<dataset-updated>> ds
    # Event <<dataset-deleted>> ds
    # Event <<dataset-cache>> miss attrs
    # Event <<dataset-cache>> hit attrs
    # -------------------------------------------------- Public Variables --
    public variable type list          ;# type of datasets to create
                                       ;# list or qtlist
    public variable loader ""          ;# loader to talk to backend
    public variable debug 0            ;# debug
    # -------------------------------------------------- Public Methods ----
    public method New {{tag ""} args} {
        $this zm_new $tag $args
    }
    # -- create a dataset
    # -- return all known tags
    public method Tags {} {
        array names DSET
    }
    # -- print out statistics
    public method Stats {{tags "all"}} {
	if { $tags == "all" } {
	    set tags [array names DSET]
	}
	foreach tag $tags {
	    array set SUM [set STAT($tag) [$DSET($tag) Stats]]
	}
	foreach n [array names SUM] { set SUM($n) 0 }

	foreach tag $tags {
	    array set S [$DSET($tag) Stats]
	    foreach t [array names SUM] {
		if ![info exists S($t)] { continue }
		set SUM($t) [expr $SUM($t) + $S($t)]
	    }
	}
	set out ""
	append out "[array size DSET] data set(s)\n" 
	foreach t [array names SUM] {
	    if { $t == "total"} { continue }
	    append out [format "%10s %8d\n" $t $SUM($t)]
	}
	set t total
	append out [format "%10s %8d\n" $t $SUM($t)]
	set out
    }
    # -- get a set of objects from loader.
    # -- or from an existing, sufficiently loaded dataset.
    # -- stream is set, if loader streaming is to be used.
    public method Get {chip bbox filters samples gx gy stream} {
	$cache Get $chip $bbox $filters $samples $gx $gy $stream
    }

    # -- retrieve a group of objects
    public method GetGroup {chip obj bbox} {
	$dset Clear
	set objects [$loader GetGroup $chip $obj $bbox]
	$dset AddCADObjects $objects
	set dset
    }

    # -- find a bunch of objects
    public method Find {chip a b c bb filters} {
	$cache Find $chip $a $b $c $bb $filters
    }

    # -- do an object inspection
    # -- subject to caching
    public method ObjInspect {chip start mode bbox filters} {
	$cache Inspect $chip $start $mode $bbox $filters
    }

    # -- return object information in a degenerate data set
    public method ObjInfo {chip obj} {
	$dset Clear
	set objects [$loader ObjInfo $chip $obj]
	$dset AddCADObjects $objects
	set dset
    }

    # -- return object definition from the cache
    public method ObjDef {obj} {
        if ![info exists OBJCACHE($obj)] {
            bell
            puts "****** cannot find $obj in my cache"
            return ""
        }
        set OBJCACHE($obj)
    }
    # -- add object definition in the object cache
    public method AddObjCache {obj attrs} {
        set OBJCACHE($obj) $attrs
    }

    # -- refresh an object
    public method Refresh {ds} {
	$cache Refresh $ds
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable OBJCACHE             ;# object cache
    protected variable DSET                 ;# the dataset
    protected variable ID 0
    protected variable dset ""
    protected variable cache ""             ;# data set cache
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set dset [New]
	set cache [DSCache $this.cache -loader $loader -debug $debug]

	# -- this event is supposed to generate a new data set
	$cache Bind <<new-dset>> $this New
	$cache Redirect <<dataset-updated>> $this
	$cache Redirect <<dataset-deleted>> $this
	$cache Redirect <<dataset-cache>> $this
    }
    destructor {
        foreach tag [array names DSET] {
            $DSET($tag) delete
	    unset DSET($tag)
        }
	$cache delete
    }
    # -------------------------------------------------- Private Methods ---
    # -- generate and register a dataset of the proper type
    private method zm_new {tag argv} {
        switch $type {
            list     { set class ZCDS_List }
            qtlist   { set class ZCDS_QTList   }
            default {
                error "unknown type: $type"
            }
        }
	incr ID
	if { $tag == "" } { 
	    set tag set$ID
	}
        set obj [eval $class $this.zds$ID -tag $tag -mgr $this $argv]
        set DSET($tag) $obj
        $obj Bind <<used>>      $this eDSStatus $tag used
        $obj Bind <<released>>  $this eDSStatus $tag released
        set obj
    }

    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -- cleanup when a dataset is deleted
    public method eDSStatus {tag status count} {
	return
	$DSET($tag) delete
	if [info exists DIR($DSET($tag)] {
	    unset DIR($DSET($tag))
	}
        unset DSET($tag)
    }
    # -------------------------------------------------- Common Initialization
}


#
# =======================================================================
#

#       Class: DSCache
# Description: DSet cache
#      Author: Manolis M. Tsangaris
#
# We keep a cache of the recently requested data sets
# I a new request is "covered" by a cached data set, we return that one.
# (see cache_cover)
#
itcl::class DSCache {
    inherit LObject LBinder
    # Event <<new-dset>> ;# request a new data set
    # Event <<dataset-updated>> $ds      ;# when datasets are updated
    # Event <<dataset-deleted>> $ds      ;# when datasets are deleted
    # Event <<dataset-cache>>  hit|miss attrs ;# datasets hit/miss
    # -------------------------------------------------- Public Variables --
    public variable loader ""
    public variable max_dsets 10
    public variable debug 0
    # -------------------------------------------------- Public Methods ----
    public method Get {chip bbox filters samples gx gy stream} {
	do_get $chip $bbox $filters $samples $gx $gy $stream
    }

    public method Inspect {args} {
	eval do_inspect $args
    }
    public method Find {chip a b c bbox filters} {
    	do_find chip $a $b $c $bbox $filters
    }
    public method List {} {
	puts ""
	puts "Data Set Cache"
	foreach ds [array names DIR] {
	    array set D $DIR($ds)
	    puts [format "%20s %12s %s" $ds $D(mode) $D(start)]
	}
	puts ""
    }

    # -- force a data set to refresh
    public method Refresh {dset} {
	set cmd [$dset cget -command]
	# -- force reload of the data set
	eval $cmd 1
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable DIR              ;# directory of DSets by tag
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
	array set DIR {}
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # -- Chip get
    public method do_get {chip bbox filters samples gx gy stream {force 0}} {
	# -- we will ignore samples, gx and gy for now

	set start "/"
	set mode "chip"
	set ds [cache_find chip $chip start $start \
	    mode $mode bbox $bbox filters $filters]

	# -- do we force reload?
	if { !$force && ($ds != "") } {
	    # -- cache hit
	    return $ds
	}
	if { $ds == "" } {
	    # -- request a new data set
	    set ds [cache_replace]
	}

	$ds Clear

	# -- stream objects to the dataset?

	if { $stream } {
	    set upcall [list $ds AddCADObjectStream]
	    $loader Get $chip $bbox $filters $samples $gx $gy $upcall
	} else {
	    set objects [$loader Get $chip $bbox $filters $samples $gx $gy]
	    $ds AddCADObjects $objects
	}
	$ds configure -info [list bbox $bbox] \
	    -command [list do_get $chip $bbox $filters $samples $gx $gy $stream]
	    
	cache_set $ds $chip $start $mode $bbox $filters
	set ds
    }

    # -- attempt to locate a suitable data set from the cache, if any
    protected method do_inspect {chip start mode bbox filters {force 0}} {
	set ds [cache_find chip $chip start $start \
	    mode $mode bbox $bbox filters $filters]

	# -- do we force reload?
	if { !$force && ($ds != "") } {
	    # -- cache hit
	    return $ds
	}
	if { $ds == "" } {
	    # -- request a new data set
	    set ds [cache_replace]
	}


	$ds Clear
	set objects [$loader ObjInspect $chip $start $mode $bbox $filters]
	$ds AddCADObjects $objects

	$ds configure -info [list inspect $start $mode $bbox] \
	    -command [list do_inspect $chip $start $mode $bbox $filters]

	cache_set $ds $chip $start $mode $bbox $filters
	set ds
    }

    # -- find an object, return a data set
    public method do_find {chip a b c bbox filters {force 0}} {
	set start $a.$b.$c
	set mode find
	set ds [cache_find chip $chip start $start\
	    mode $mode bbox $bbox filters $filters]

	# -- do we force reload?
	if { !$force && ($ds != "") } {
	    # -- cache hit
	    return $ds
	}
	if { $ds == "" } {
	    # -- request a new data set
	    set ds [cache_replace]
	}

	$ds Clear
	$ds AddCADObjects [$loader Find $chip $a $b $c $bbox $filters]
	debug "$ds: found [$ds Size] objects"

	$ds configure -info [list show $a $b $c] \
	    -command [list do_find $chip $a $b $c $bbox $filters]

	cache_set $ds $chip $start $mode $bbox $filters
	set ds
    }

    # ----------------------------------------------------------- Cache Mngt
    protected method cache_set {ds chip start mode bbox filters} {
	set attrs [list chip $chip start $start mode $mode \
	    bbox $bbox filters $filters key $chip-$start-$mode ]
	set DIR($ds) $attrs

	# -- set the dataset attributes
	$ds configure -attrs $attrs
	
	# -- generate an event
	Event <<dataset-updated>> $ds
    }

    protected method cache_delete {ds} {
	unset DIR($ds)
	$ds delete
    }

    # -- look in the cache, to find a suitable data set
    protected method cache_find {args} {
	array set C $args

	set C(key) $C(chip)-$C(start)-$C(mode)
	array set A {}

	foreach ds [array names DIR] {
	    unset A
	    array set A [$ds cget -attrs]
	    set A(ds) $ds
	    if [cache_covers A C] {
		Event <<dataset-cache>> hit $args
		debug "*** HIT $ds ([$ds Size] objects)"
		return $ds
	    }
	}
	Event <<dataset-cache>> miss $args
	return ""
    }

    # -- check if data set A covers dataset B (i.e. overlaps)
    protected method cache_covers {aa bb} {
	upvar $aa A
	upvar $bb B
	debug "examining if $A(ds) covers the request"
	# -- different chip+start+mode?
	if { $A(key) != $B(key)  } { return 0 }

	# -- we are in chip mode?
	# -- is the requested bbox a subset of the master's?
	# -- we will silently assume there is no clipping
	# -- when on inspect/search mode (so there is no need to check bbox)
	if { ($A(mode) == "chip") && ($A(bbox) != "") } {
	    foreach {ax0 ay0 ax1 ay1} $A(bbox) { break }
	    if { $ax0 > $ax1 } { swap ax0 ax1 }
	    if { $ay0 > $ay1 } { swap ay0 ay1 }

	    foreach {bx0 by0 bx1 by1} $B(bbox) { break }
	    if { $bx0 > $bx1 } { swap bx0 bx1 }
	    if { $by0 > $by1 } { swap by0 by1 }

	    # -- is NOT (A.bbox out of B.bbox)?
	    if {! (($ax0 <= $bx0) && ($ay0 <= $by0) &&
	         ($bx1 <= $ax1) && ($by1 <= $ay1)) } {
		debug "$A(ds): different bbox from req"
		return 0
	    }
	}
	array set FA $A(filters)
	array set FB $B(filters)
	# -- do the filters overlap?

	foreach filter [array names FB] {
	    if ![info exists FA($filter)] {
		debug "$A(ds) is missing $filter"
		return 0
	    }
	    debug "A Filter($filter): $FA($filter)"
	    debug "B Filter($filter): $FB($filter)"
	    array set fa $FA($filter)
	    array set fb $FB($filter)
	    # -- check if all components of B are covered
	    foreach f [array names fb] {
		# -- ignore .
		if { $f == "." } { continue }
		# -- covered?
		if { $fa($f)  } { continue }
		if { !$fb($f)  } { continue }
		debug "$A(ds): $filter/$f: A:$fa($f) REQ:$fb($f): no cover"
		# -- filters do not cover
		return 0
	    }
	    unset fa
	    unset fb
	}
	debug "$A(ds): covers the request"
	return 1
    }
    # -- manage the cache. Use LRU
    protected method cache_replace {} {
	# -- any need to replace?
	if { [array size DIR] < $max_dsets } {
	    set ds [Event <<new-dset>>]
	    debug "$ds: allocated"
	    return $ds
	}
	set now [clock seconds]
	set victim ""
	set max ""
	foreach ds [array names DIR] {
	    set dt [expr $now - [$ds Atime]]
	    debug "$ds:last used [time_afmt $dt] ago"
	    if { ($max == "") || ($dt > $max) } {
		set max $dt ; set victim $ds
	    }
	}
	debug "$victim: reused (last used [time_afmt $max] ago)"
	return $victim
    }
    # ----------------------------------------------- Interface Implementation
    protected method debug {msg} {
	if { $debug } { puts stderr "    > dscache $msg"}
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
