package provide mtlib 1.0

#       Class: LConsole
# Description: Simple Console with command line editing
#              Following the Plan 9 approach you can execute past commands
#              but they will be inserted at the end of the session always.
#              You cannot run commands on the output
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
itcl::class LConsole {
    inherit LFrame LBinder
    # Event <<cmd>> cmd        : the user wants to run a command
    # Event <<eval>>           : the console wants to evaluate the command
    # -------------------------------------------------- Public Variables --
    public variable hist ""
    public variable rcfile ""
    public variable bg "gray70"
    public variable fg "black"
    public variable out 0               ;# do not show any output or errors
    public variable append 0            ;# append text at the end of console
    public variable autoscroll 0        ;# scroll to the appended text
    # -------------------------------------------------- Public Methods ----
    # run a command, push it to history
    # display results
    protected method Eval {cmd} {
        show cmd $cmd
        update idletasks

        global errorInfo
        if [catch {eval Event <<eval>> $cmd} rc] {
            show result err $rc
        } elseif { $output } {
            show result out $rc
        }
        $entry delete 0 end
    }
    # show cmd, out or err (append to messages)
    public method Show {type txt} {
        show $type $txt
    }
    # show replacing previous message
    public method ReShow {type txt} {
        show $type $txt 1
    }
    # -- just push a command to history stack & show it
    public method Push {cmd} {
        $hist Push $cmd
    }
    public method Clear {} {
        $text config -state normal
        $text delete 0.0 end
        $text config -state disabled
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable text ""
    protected variable entry ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set entry [entry $This.e]
        set text [mktext $This.ftxt -bg black -fg white]
        pack $This.ftxt -side top -expand 1 -fill both
        pack $entry -side top -fill x
        LBridge width  "$text configure -width"
        LBridge height "$text configure -height"
        LBridge bg     "$entry configure -bg"
        LBridge fg     "$entry configure -fg"
        #$entry config -font $font
        #$text  config -font $font

        LBridge rcfile "$hist configure -rcfile"
        do_bindings $entry
        $text tag configure cmd -foreground yellow
        $text tag configure out -foreground white
        $text tag configure err -foreground red
        $text tag configure inf -foreground green

        $text tag bind cmd <1> "$this eExpand $text"
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # apply emacs like bindings to the text entry
    protected method do_bindings {w} {
        set start 0
        bind $w <KeyPress-Delete> "
            if \[catch {$w delete sel.first sel.last}\] {
            $w delete \[expr \[$w index insert\]-1\]
        }"
        bind $w <KeyPress-Return>  "$this eCommand $w"
        bind $w <Enter> "focus $w"
        bind $w <Leave> "focus ."
        bind $w <Control-KeyPress-d> "$w delete \[$w index insert\]"
        bind $w <Control-KeyPress-k> "$w delete \[$w index insert\] end"
        bind $w <Control-KeyPress-f> "$w icursor \[expr \[$w index insert\]+1\]"
        bind $w <Control-KeyPress-b> "$w icursor \[expr \[$w index insert\]-1\]"
        bind $w <Control-KeyPress-a> "$w icursor $start"
        bind $w <Control-KeyPress-e> "$w icursor end"
        bind $w <Control-KeyPress-u> "$w delete $start end"

        bind $w <Control-KeyPress-p> "$this eHistoryGo $w -1"
        bind $w <KeyPress-Up>        "$this eHistoryGo $w -1"
        bind $w <Control-KeyPress-n> "$this eHistoryGo $w  1"
        bind $w <KeyPress-Down>      "$this eHistoryGo $w  1"
        bind $w <Control-KeyPress-l> "$this Clear"
    }
    protected method mktext {w args} {
        frame $w -class Text
        set box [eval text $w.box -relief sunken -border 1 $args]
        scrollbar $w.vsb -relief sunken -border 1 -width 10
        pack $w.box -side left -expand 1 -fill both
        pack $w.vsb -side left -fill y

        $w.vsb config -command "$w.box yview"
        $w.box config -yscroll "$w.vsb set"
        set box
    }
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- CallBacks       ---
    # ------------------------------------------------------- Execution
    public method eExpand {w} {
        set loc [$w index current]
        if { $loc == "" } { bell ; return }
        if { [scan $loc "%d" line] != 1 } {
            bell ; return
        }
        set cmd [$text get $line.0 $line.end]
        Event <<cmd>> $cmd
    }
    # ------------------------------------------------------- Line Editing
    # the user wants to execute a command. Just post an event
    public method eCommand {w} {
        set cmd [string trim [$w get]]
        $w delete 0 end
        Event <<cmd>> $cmd
    }

    public method eHistoryGo {w n} {
        set cmd [$hist Go $n]
        if { $cmd == "" } { bell ; return}
        $w delete 0 end 
        $w insert 0 $cmd
    }
    # --------------------------------------------------------- show
    protected method show {type txt {replace 0}} {
        case $type {
            cmd {
                $entry delete 0 end
                $entry insert 0 $txt
                $hist Push $txt
                insert 1.0 $txt\n cmd $replace
            }
            out {
                $entry delete 0 end
                insert 2.0 $txt\n out $replace
            }
            err {
                $entry delete 0 end
                insert 2.0 $txt\n err $replace
            }
            inf {
                insert 1.0 $txt\n inf $replace
            }
            default {
                error "$type: unknown type. Use cmd|out|err|inf"
            }
        }
    }
    protected method insert {sindex txt tags replace} {
        # crude!
        $text config -state normal
        if { $append } { 
            insert_end end $txt $tags
	    if { $autoscroll } { $text see end } 
            $text config -state disabled
            return
        }
        if { $replace } {
            foreach {a b} [split $sindex . ] { break }
            $text delete $sindex $a.end
        }
        set s [$text index $sindex]
        $text insert $sindex $txt
        set n [string length $txt]
        incr n -1
        set e [$text index "$s + $n char"]
        foreach tag $tags {
            $text tag add $tag $s $e
        }
	if { $autoscroll } { $text see $sindex } 
        $text config -state disabled
    }
    protected method tfind {tag} {
        set tags [$text tag names $tag]
        return $tags
    }
    # -- insert new text at the $sindex position
    # -- take care of the tags as well
    protected method insert_end {sindex txt tags} {
        set s [$text index insert]
        $text insert $s $txt
        set e [$text index "insert - 1 char"]
        foreach tag $tags {
            $text tag add $tag $s $e
        }
        # move window at the newly entered text
        $text see $s
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: LConsoleHistory
# Description:
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
itcl::class LConsoleHistory {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable rcfile "" { hist_load $rcfile }
    # -------------------------------------------------- Public Methods ----
    public method Push {cmd} {
        set HIST([array size HIST]) $cmd
        set HPOS [array size HIST]
        set cmd
    }
    public method HistNo {} {
        set HPOS
    }
    public method Go {n} {
        set n [expr $HPOS + $n] 
        if [info exists HIST($n)] {
            set HPOS $n
            return $HIST($n)
        }
        return ""
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable HIST
    protected variable HPOS 0 ;# current position
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor {
        if { $rcfile != "" } {
            hist_save $rcfile
        }
    }
    # -------------------------------------------------- Private Methods ---
    protected method hist_load {file} {
        if ![file exists $file] { return }
        set fp [open $file r]
        while { [gets $fp line] != -1 } {
            Push [string trim $line]
        }
        close $fp
    }
    protected method hist_save {file} {
        set fp [open $file w]
        foreach n [lsort -integer -increasing [array names HIST]] {
            puts $fp $HIST($n)
        }
        close $fp
    }
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- CallBacks       ---
    # -------------------------------------------------- Common Initialization
}
