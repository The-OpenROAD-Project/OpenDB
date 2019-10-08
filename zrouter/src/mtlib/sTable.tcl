#

package provide mtlib 1.0

#
#       Class: LScrollableTable
# Description: Scrollable table with multiple columns
#      Author: Manolis M. Tsangaris
#
# It accepts tagged data, and displays them in multi-column format,
# each on on its column. All usual selection capabilities are avaliable.
#
itcl::class LScrollableTable {
    inherit LFrame LBinder 
    # Event <<double-click>> $index
    # Event <<click>>        $index
    # -------------------------------------------------- Public Variables --
    public variable bg white
    public variable maxlines 10
    public variable scroll 1  { config_sb $scroll }
    # -------------------------------------------------- Protected Variables 
    protected variable column
    protected variable wcolumn
    protected variable format
    protected variable w
    protected variable items
    protected variable tags
    protected variable selection
    protected variable view ""
    protected variable Title
    protected variable Table
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        ::option add *STable*Label.background gray60
        # option add *STable*Listbox.font [LFont Helvetica 12]
        ::option add *STable*Scrollbar.background gray
        ::option add *STable*Scrollbar.foreground gray
        LScrollableTable::init
    }
    destructor {
    }
    # -------------------------------------------------- Public Methods ----
    # -- Add a new column(s) to the table
    #
    # The definition is supposed to be a list of {tag title format} attributes.
    # Format is for now the width of the column
    #
    public method NewColumns {Columns} {
        foreach c $Columns {
            add_column $c
        }
    }
    public method AddColumn {tag title fmt} {
        add_column [list $tag $title $fmt]
    }
    #
    # Append a tuple to the table
    # NOTE: this uses the old format
    public method Append {t} { append_tuple $t }

    # NOTE: this uses the new (array set) format
    public method Add {tuple} { add $tuple }

    #
    public method Size {} { return $items }
    #
    # View a subset of columns (in the order specified)
    #
    public method View {columns} { view $columns }
    #
    # Retrieve a particular tuple
    #
    public method Get { index } { set Table($index) }
    public method GetAll { } {
        set s ""
        for {set i 0} { $i < $items} { incr i} {
            lappend s $Table($i)
        }
        set s
    }
    #
    # Get the indices of the current selection
    #
    public method Selection  { } { $column([lindex $tags 0]) curselection }
    public method Selections { } { $column([lindex $tags 0]) curselection }
    #
    # Select a range of tuples ...
    # (well, at some point will be a list of indices!)
    #
    public method Select {list} {
        set from [lindex $list 0]
        set to   [lindex $list 1]
        if { $to == "" } { set to $from }
        if { $from == "all" } { 
            set from 0
            set to   [expr $items - 1]
        }
        ## Log sTable "Select $list -> $from $to"
        foreach t $tags {
            $column($t) select set $from ;
            $column($t) select anchor $to
        }
        set selection(new) 1
    }
    #
    # Unselect
    #
    public method UnSelect { } {
        foreach t $tags {
            $column($t) select clear
        }
        set selection(new)   1
    }
    #
    # NextSelectionValue: Navigate through the current list of selections
    # subsequent calls to this method will receive each  selection item
    #
    public method NextSelectionValue {} {
        # check if selection has been updated ...
        if { $selection(new) } {
            set selection(new) 0
            set selection(list) [Selection]
        }
        # check if selection list has been exhausted
        if { $selection(list) == {} } {
            set selection(list) [Selection]
            ## Log STable "No selection! (resetting to $selection(list))"
            return {}
        }
        # update the selection list
        ## Log STable "selection $selection(list)"
        set r               [lindex $selection(list) 0]
        set selection(list) [lrange $selection(list) 1 end]
        return [Get $r]
    }
    #
    # Delete a range of tuples (last can be end)
    #
    public method Delete { first {last ""}} {
        if { $last == "" } { set last $first }
        foreach t $tags {
            $column($t) delete $first $last
        }
        if { $last == "end" } { set last [expr $items - 1 ] }
        set delta [expr $last + 1]
        for {set i $first} {$i <= $last} {incr i} {
            set s {} ; catch {set s $Table($delta)}
            set Table($i) $s
            incr delta
        }
        if {!$scroll && ($items < $maxlines)} { config_sb 0 }
        set selection(new) 1
    }
    public method Clear { } {
        foreach t $tags { $column($t) delete 0 end }
        set items 0
        unset Table ; array set Table {}
        if !$scroll { config_sb 0 }
    }
    # force a selection, if the user pointed to window w @ x,y
    # return the index of the selected item
    public method SelectXY {W x y} {
        foreach t $tags {
            if { $column($t) == $W } {
                return [eSelect $W set $y]
            }
        }
        return [eSelect $column($t) set $y]
    }
    # Select & Show a line
    public method See {n} {
        foreach t $tags { $column($t) see $n }
    }
    # Dump table contents in text format
    public method DumpPage {lpp title } { dump_page $lpp $title }

    #
    # ----------------------------------------------------------------------
    # Event handling
    # ----------------------------------------------------------------------
    # called by scrollbar
    public method eYView {args} {
        foreach l $tags {
            uplevel #0 $column($l) yview $args
        }
        update idletasks
    }
    # called by text
    public method eYScroll {args} {
        uplevel #0 $w(vsb) set $args
        update idletasks
        set selection(new) 1
    }
    public method eScan { W how X Y } {
        foreach l $tags {
            $column($l) scan $how $X $Y
        }
        update idletasks
        set selection(new) 1
    }
    public method eSelect { W how Y } {
        set index [$W nearest $Y]
        if { $how == "set" } {
            foreach l $tags {
                $column($l) select clear 0 end
                $column($l) select set $index $index
            }
        } else {
            foreach l $tags {
                $column($l) select $how $index
            }
        }
        update idletasks
        if { $how == "set" } { Event <<click>> $index }
        set index
    }
    public method eDouble { W Y } {
        set index [$W nearest $Y]
        Event <<double-click>> $index
    }
    # -------------------------------------------------- Private Methods ---
    protected method init {} {
        frame $This.tbl 
        set w(vsb) $This.vs
        scrollbar $This.vs -relief sunken -border 1 \
            -command "$this eYView" -width 10
        pack $This.tbl -side left -expand 1 -fill both
        config_sb $scroll

        set tags {}
        set selection(list) {}
        set selection(new)  0
        array set Table {}
    }
    protected method config_sb {sb} {
        if ![winfo exists $This.vs] { return }
        if { $sb } {
            if ![winfo ismapped $This.vs] {
                pack $This.vs  -side left -fill y -in $This
            }
        } else {
            if [winfo ismapped $This.vs] {
                pack unpack $This.vs
            }
        }
    }
    protected method add_column {c} {
        set n [llength $tags]
        set Tag    [lindex $c 0] ; if { $Tag    == "" } { set Tag col$n }
        set Name   [lindex $c 1] ; if { $Name   == "" } { set Name   $tag }
        set Format [lindex $c 2] ; if { $Format == "" } { set Format 10 }
        set column($Tag)  [set f $This.tbl.c$n].l
        set wcolumn($Tag) $f
        set format($Tag) $Format
        set Title($Tag) $Name
        lappend tags $Tag

        frame $f -border 2 -relief groove -class Table
        label $f.t -text $Name -anchor c 
        listbox $f.l  -setgrid 1  -export false -bg $bg
        $f.l config -width ${Format} -height $maxlines
        if { !$n } {
               $f.l config -yscroll "$this eYScroll"
        }
        bind $f.l <Button-1>       "$this eSelect %W set   %y"
        bind $f.l <Button1-Motion> "$this eSelect %W anchor     %y"
        bind $f.l <Shift-ButtonPress-1>  "$this eSelect %W adjust  %y"
        bind $f.l <Shift-Button1-Motion> "$this eSelect %W adjust  %y"
        bind $f.l <Button-2>       "$this eScan   %W mark   %x %y"
        bind $f.l <Button2-Motion> "$this eScan   %W dragto %x %y"
        bind $f.l <Double-Button-1> "$this eDouble %W %y"

        pack $f.t -side top -fill x
        pack $f.l -side top -fill both -expand 1
        pack $f   -side left -expand 1 -fill both
        set items 0
        incr n
    }
    protected method view {columns} {
        if { $view == ""  } { set view $tags }
        foreach t $view {
            pack unpack $wcolumn($t)
        }
        foreach t $columns {
            pack $wcolumn($t) -side left -expand 1 -fill y
        }
        set view $columns
    }

    protected method append_tuple { Values } {
        array set inserted {}
        foreach c $Values {
            set Tag   [lindex $c 0]
            set Value [lindex $c 1]
            set inserted($Tag) 1
            catch { $column($Tag) insert $items \
                [format "%-$format($Tag)s" $Value] }
        }
        foreach Tag $tags {
            if ![info exists inserted($Tag)] {
                $column($Tag) insert $items [format "%$format($Tag)s" " "]
            }
        }
        set Table($items) $Values
        incr items
        if { $items >= $maxlines } {
            config_sb 1
        }
    }
    # Values is in array set format
    protected method add { Values } {
        array set V $Values
        array set inserted {}
        foreach c [array names V] {
            set Tag   $c
            set Value $V($c)
            set inserted($Tag) 1
            catch { $column($Tag) insert $items \
                [format "%-$format($Tag)s" $Value] }
        }
        foreach Tag $tags {
            if ![info exists inserted($Tag)] {
                $column($Tag) insert $items [format "%$format($Tag)s" " "]
            }
        }
        set Table($items) $Values
        incr items
        if { $items >= $maxlines } {
            config_sb 1
        }
    }
    #
    # ---------------------------------------------------------------------
    # Convert the current contents of the table, to a set of strings
    # suitable for producing a multi page output
    # Take into account the names of the columns and the formats.
    #
    protected method dump_text {a} {
        upvar $a A
        if { $view != "" } { 
            set cols $view
        } else {
            set cols $tags
        }
        set line ""
        foreach v $cols {
            append line "+"
            for {set i 0} { $i < $format($v) } {incr i } { append line "-" }
        }
        append line "+"
        set A(line) $line

        # printing titles may be tricky, since they may contain newlines
        set cont 1 ; set i 0; set s ""
        while {$cont != "" } {
            set cont "" ; set ln ""
            foreach v $cols {
                set tl  [lindex [split $Title($v) \n] $i]
                if { $tl != "" } { set cont $tl }
                append ln [format "|%$format($v)s" $tl]
            }
            if { $cont != "" } {
                if { $s != "" } { append s \n }
                append s "$ln|"
            }
            incr i
        }
        set A(title) $s
        for {set i 0} { $i < $items} { incr i} {
            set s ""
            array set P $Table($i)
            # take care of extra long columns (by chopping them off!)
            foreach v $cols {
                set ff $P($v) ; set n [string length $ff]
                if { $n > $format($v) } {
                    set ff "[string range $ff 0 [expr $format($v) - 3]].."
                }
                append s [format "|%$format($v)s" $ff]
            }
            set A($i) "$s|"
            unset P
        }
        set A(rows)  $s
        set A(items) $items
    }
    protected method dump_page {lpp title} {
        incr lpp -5
        if { $lpp < 0 } { set lpp 4 }
        dump_text A
        set out ""
        set page 1
        for {set i 0} { $i < $A(items) } { incr i $lpp } {
            append out $title\n\n
            append out $A(line)\n
            append out $A(title)\n
            append out $A(line)\n
            set j1 [expr $i + $lpp]
            if { $j1 > $A(items) } { set j1 $A(items) }
            for {set j $i} {$j < $j1} {incr j} {
                append out $A($j)\n
            }
            append out $A(line)\n
            append out "\n"
            append out "\n"
            append out "-PAGE $page --- \n"
            append out ""
            incr page
        }
        set out
    }
    # ------------------------------------------------------------------------
    # -------------------------------------------------- Common Initialization
}
