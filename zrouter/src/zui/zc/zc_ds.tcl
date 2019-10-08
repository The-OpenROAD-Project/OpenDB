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

package provide ade::ui::zc 1.0

#       Class: ZCDS
# Description: ZCanvas Data Set
#      Author: Manolis M. Tsangaris
#
# Data sets are implemented based on zc canvas, for the shake of efficiency.
# Logically they belong to the data layer of the GUI.
# Depending on the implementation, the zui data set manager will instantiate
# the right one.

itcl::class ZCDS {
    inherit LObject LBinder
    # Event <<used>>     count                  ;# ds cas been used by count
    # Event <<released>> count                  ;# ds has been released by count
    # -------------------------------------------------- Public Variables --
    public variable tag set                   ;# its tag name
    public variable mgr ""                    ;# data set manager
    public variable debug 0
    public variable attrs ""                  ;# attributes generated this DS
    public variable info ""                   ;# other data set information
    public variable command ""                ;# command that created ds
    # -------------------------------------------------- Public Methods ----
    # -- clear a dataset
    public method Clear {} {
	$this zcds_clear
    }
    # -- add a set of objects
    public method AddCADObjects {objlist} {
	$this zcds_cad_objects_add $objlist
    }
    # -- add an object stream
    public method AddCADObjectStream {channel} {
	$this zcds_cad_objects_add_stream $channel
    }
    # -- return dataset bbox
    public method BBox {} {
	$this zcds_bbox
    }
    # -- return all dataset marks
    public method Marks {} {
	$this zcds_marks
    }
    # -- return data set statistics
    public method Stats {} {
	$this zcds_stats
    }

    # -- Note the use or the release of a data set (for garbage collection)
    public method Use {} {
	set ATIME [clock seconds]
	$this zcds_use
    }
    public method Release {} {
	$this zcds_release 
    }
    public method Changed {} {
	set DIRTY
    }
    # -- data set size
    public method Size {} {
	$this zcds_size 
    }

    # -- last time used
    public method Atime {} {
	set ATIME
    }

    # -- return a data set attribute
    public method GetAttr {attr} {
	array set A $attrs
	set A($attr)
    }

    # -- return a range of the data set
    public method Range {start end} {
	$this zcds_range $start $end
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable INUSE 0
    protected variable DIRTY 0       ;# has the dataset changed?
    protected variable ATIME 0       ;# last time used
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set ATIME [clock seconds]
    }
    destructor {
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -- clear a data set
    protected method zcds_clear {} {
    }
    # -- add objects to the data set 
    protected method zcds_cad_objects_add {objlist} {
    }
    # -- add objects to the data set 
    protected method zcds_cad_objects_add_stream {channel} {
    }
    # -- return bbox
    protected method zcds_bbox {} {
    }
    # -- return stats
    protected method zcds_stats {} {
    }
    # -- this dataset is being used/released
    protected method zcds_use {} {
	incr INUSE
	Event <<used>> $INUSE
    }
    # -- this data set is no longer used
    protected method zcds_release {} {
	incr INUSE -1
	Event <<released>> $INUSE
    }
    # -- return all marks
    protected method zcds_marks {} {
    }
    # -- return dataset size
    protected method zcds_size  {} {
	return -1
    }
    protected method zcds_range {start end} { }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}

#
#       Class: ZCDS_List
# Description: Tcl based data set
#      Author: Manolis M. Tsangaris
#
itcl::class ZCDS_List {
    inherit ZCDS
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -- return all objects stored (needed for list only)
    public method Get {} {
	return $SET
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable MARK                     ;# marks of this dataset
    protected variable BBOX   ""                ;# BBOX
    protected variable SET ""                   ;# data set
    protected variable OBJECTCOUNT              ;# object count stats
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	array set MARK {}
	array set OBJECTCOUNT {}
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -- clear a data set
    protected method zcds_clear {} {
	unset MARK
	array set MARK {}
	unset OBJECTCOUNT
	array set OBJECTCOUNT {}
	set BBOX ""
	set SET ""
	set DIRTY 0
    }
    # -- add a list of objects
    protected method zcds_cad_objects_add {objlist} {
	incr DIRTY
        array set M {}
        foreach obj $objlist {
            if [catch {array set S $obj} rc] {
                warning zc_ds "add_objects: $rc: $obj"
                continue
            }
            lappend SET $obj
	    if ![info exists OBJECTCOUNT($S(type))] {
		set OBJECTCOUNT($S(type)) 1
	    } else {
		incr OBJECTCOUNT($S(type))
	    }
            if [info exists S(mark)] { foreach m $S(mark) { set M($m) "" } }
            unset S
        }
        set BBOX ""
        array set MARK [array get M]
    }

    # -- add object stream
    protected method zcds_cad_objects_add_stream {channel} {
	incr DIRTY
        array set M {}
        while { [gets $channel line] != -1 } {
                #warning zc_ds "RECEIVING LINE: <$line>"
            if { $line == "." } { break }
            if { $line == "" } { continue }
            set S(mark) ""
            foreach {S(layer) S(hierarchy) S(type) x0 y0 dx dy} $line {
                break
            }
            if { $x0 == "" } {
                warning zc_ds "add_stream: ignoring <$line>"
                continue
            }
	    if ![info exists OBJECTCOUNT($S(type))] {
		set OBJECTCOUNT($S(type)) 1
	    } else {
		incr OBJECTCOUNT($S(type))
	    }

            if ![info exists S(layer)] {
                warning zc_ds "add_stream: ignoring <$line>"
                continue
            }
            set S(id) 0
            set S(bb) [list $x0 $y0 [expr $x0 + $dx] [expr $y0 + $dy]]
            lappend SET [array get S]
            foreach m $S(mark) { set M($m) "" }
            unset S
        }
        set BBOX ""
        array set MARK [array get M]
    }
    protected method zcds_bbox {} {
        if { $BBOX != "" } { 
            return $BBOX
        }
        set ax0 ""
        foreach obj $SET {
            array set S $obj
            foreach {x0 y0 x1 y1} $S(bb) { break }
            if { $x0 > $x1 } { swap x0 x1 }
            if { $y0 > $y1 } { swap y0 y1 }
            if { $ax0 == "" } {
                foreach {ax0 ay0 ax1 ay1} [list $x0 $y0 $x1 $y1] { break }
            } else {
                if { $x0 < $ax0 } { set ax0 $x0 }
                if { $y0 < $ay0 } { set ay0 $y0 }
                if { $x1 > $ax1 } { set ax1 $x1 }
                if { $y1 > $ay1 } { set ay1 $y1 }
            }
            unset S
        }
	if { $ax0 == "" } { 
	    set BBOX ""
	} else {
	    set BBOX [list $ax0 $ay0 $ax1 $ay1]
	}
    }
    protected method zcds_stats {} {
        # array set S "slices [array size SLICE]"
        array set S [array get OBJECTCOUNT]
        set sum 0
        foreach n [array names OBJECTCOUNT] {
            set sum [expr $sum + $OBJECTCOUNT($n)]
        }
        set S(total) $sum
        array get S
    }
    protected method zcds_marks {} {
	array names MARK
    }
    protected method zcds_size  {} {
	llength $SET
    }
    protected method zcds_range {start end} {
	lrange $SET $start $end
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}

#
#       Class: ZCDS_QTList
# Description: MEMORY (QT) based data set
#      Author: Manolis M. Tsangaris
#
itcl::class ZCDS_QTList {
    inherit ZCDS
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable prefix  "::"              ;# where the .so is loaded
    protected variable mname   "tkqt"       ;# module name
    protected variable qt      "::tkqt"     ;# full access name
    protected variable SIZE 0               ;# data set size
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    protected method qt {cmd args} {
        dprint qt.$cmd "$args"
        set rc [eval ${qt}::$cmd $args]
        dprint qt.$cmd "=$rc"
        return $rc
    }
    # -- return all tags of a given type
    protected method qtzc_tags {type} {
        set nt [qt getTagSize $type]
        set out ""
        for { set i 0 } { $i < $nt } { incr i } {
           set m [qt getTagName $type $i]
           if { $m == "" } { continue }
           lappend out $m
        }
        set out
    }
    proc dprint {tag msg} {
	# if { $debug } { puts stderr "$tag: $msg" }
    }
    # ----------------------------------------------- Interface Implementation
    # -- clear a data set
    protected method zcds_clear {} {
	set DIRTY 0
	set SIZE 0
	qt eraseDataset $tag
    }
    # -- add a list of objects
    protected method zcds_cad_objects_add {objlist} {
	incr DIRTY
	incr SIZE [expr $SIZE + [llength $objlist]]
        qt addObjects $tag $objlist
    }
    # -- add object stream
    protected method zcds_cad_objects_add_stream {channel} {
	incr DIRTY
        qt readFile $tag $channel
    }
    protected method zcds_bbox {} {
	 lindex [qt getDataBBox $tag] 0
    }
    protected method zcds_stats {} {
	list total 0
    }
    protected method zcds_marks {} {
	qtzc_tags "marks-$tag"
    }
    # -- return dataset size
    protected method zcds_size  {} {
        return [qt getDatasetStats $tag]
    }
    # -- return a range of values staring from start and including end
    # -- 0 is the first item, SIZE is the last
    protected method zcds_range {start end} {
        return [qt getDatasetRange $tag $start $end]
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
