package provide mtlib 1.0

#----------------------------------------------------------------------------
# Simple Inspector package (to visually inspect Itcl objects and classes)
#
# -- VERSION INFO
# $Id: inspector.tcl,v 1.1 2006/01/10 21:17:41 mmt Exp $
# -- MODIFICATION HISTORY
# $Log: inspector.tcl,v $
# Revision 1.1  2006/01/10 21:17:41  mmt
# initial
#
# Revision 1.4  2005/11/28 13:25:12  mmt
# added tree.tcl (simple tree drawing program)
#
# Revision 1.3  2005/09/14 15:45:23  mmt
# fixes
#
# Revision 1.2  2005/09/14 09:53:43  mmt
# window packing changes
#
# Revision 1.1  2005/08/01 13:41:33  mmt
# added inspector
#
#----------------------------------------------------------------------------

#       Class: LInspectorUI
# Description: Object Inspector
#
itcl::class LInspectorUI {
    inherit LTopLevelEditor LBinder
    # Event <<quit>>
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable insp
    protected variable tree
    protected variable text
    protected variable ecmd           ;# command window
    protected variable SELECTION ""   ;# remember the last object selected
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set insp [LObjInspector $this.insp]
        set w [mktree $This.tr]
        pack $w -side top -expand 1 -fill both
        global argv argv0
        configure -title "Inspector: $argv0 $argv"

        set ecmd [LInspectorCMD $This.ecmd]
        pack $ecmd -side top -fill x
        $ecmd Bind <<cmd>> $this eEval
    }
    destructor {
        $insp delete
        $tree delete
        $ecmd delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method mktree {w} {
        frame $w
        set tree [LDirTree $w.tree]
        $tree Bind <<expand>> $this eExpand

        $tree Add /objects -type objects -id objects
        set text [LScrollableWidget $w.txt text -width 40 -height 20]
        pack $tree -side left -fill both -expand 1
        pack $w.txt -side left -fill both -expand 1
        set tree [namespace current]::$tree

        $insp Bind <<info>>    $this eInfoShow
        $insp Bind <<link>>    $this eLink
        $insp Bind <<object>>  $this eSelectObject

        set w
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method vMakeButtons {w} {
        button_add $w.inspect -text Quit -command "$this Event <<quit>>"
        set w
    }
    # -- user has expanded a node
    public method eExpand {path} {
        set r [$insp Expand $path $tree]
        $tree Open $path
    }
    # ----------------------------------------------INSPECTOR EVENTS
    # -- show inforomation on the text screen
    public method eInfoShow {info} {
        $text delete 1.0 end
        $text insert 1.0 $info
    }
    # -- user has selected a node
    public method eLink {path} {
	if { $path == "" } { return }
        $tree Select $path
    }
    # -- usr has selected an object
    public method eSelectObject {obj} {
        set SELECTION $obj
    }
    # ---------------------------------------------- COMMAND
    # -- user has requested to run a command on an object
    public method eEval {cmd} {
        if { [string match !* $cmd]} {
            set cmd [string range $cmd 1 end]
        } elseif { $SELECTION != "" } {
            set cmd "$SELECTION $cmd"
        } else {
            set cmd "$cmd"
        }
        if [catch {uplevel #0 $cmd} rc] {
            eInfoShow $cmd\n$rc
        } else {
            eInfoShow $cmd\n$rc
        }
    }
    # ------------------------------------------------------
    # -------------------------------------------------- Common Initialization
}

#       Class: LInspectorCMD
# Description: Inspector command window
#
itcl::class LInspectorCMD {
    inherit LFrame LBinder
    # Event <<cmd>> cmd
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -- change the entry contents to cmd
    public method Set {cmd} {
        $This.e delete 0 end
        $This.e insert 0 $cmd
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable e
    protected variable HIST
    protected variable HCOUNT 0
    protected variable THCOUNT 0         ;# temporary history count
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set e [entry $This.e -bg black -fg red]
        pack $e -side left -fill x -expand 1
        bind $e <KeyPress-Return> "$this eCMD $e"
        bind $e <Control-p> "$this eHist -1"
        bind $e <Control-n> "$this eHist  1"
        bind $e <Enter> "focus $e"
	LBindEmacs $e
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eCMD {w} {
        set cmd [string trim [$e get]]
	$e delete 0 end
	set HIST($HCOUNT) $cmd
	incr HCOUNT
	set THCOUNT $HCOUNT
        Event <<cmd>> $cmd
    }
    public method eHist {pos} {
	set n [expr $THCOUNT + $pos]
	if ![info exists HIST($n)] { return }
	set THCOUNT $n
	$e delete 0 end
	$e insert 0 $HIST($n)
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: LObjInspector
# Description: Object Inspector
#
itcl::class LObjInspector {
    inherit LObject LBinder
    # Event <<info>> info   (info of a tree node)
    # Event <<link>> path   (reselect a new path)
    # Event <<object>> obj  (an object has been selected)
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Expand {path tree} {
        expand $path $tree
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method expand {path tree} {
        switch [$tree Get $path type] {
            objects {
                # -- produce a list of objects
                foreach obj [itcl::find objects] {
                    obj_add $tree  $obj $obj
                }
            }
            object {
                set obj [$tree Get $path obj]
                Event <<object>> $obj
                $tree Add $path/vars -type object/vars  -obj $obj
                $tree Add $path/objs -type object/objs  -obj $obj
            }
            object/vars {
                set obj [$tree Get $path obj]
                Event <<object>> $obj
                set vals ""
                foreach v [lsort [$obj info variable]] {
                    $tree Add $path/$v -type object/var \
                        -obj $obj -var $v -title "[vtitle $obj $v]"
                    append vals [format "%15s=%s\n"\
                        [namespace tail $v] [$obj info var $v -value]]
                }
                Event <<info>> $vals
            }
            object/var {
                set obj [$tree Get $path obj]
                Event <<object>> $obj
                set var [$tree Get $path var]
                Event <<info>> [$obj info variable $var -value]
            }
            object/objs {
                # show embedded objects
                set obj [$tree Get $path obj]
                Event <<object>> $obj
                # -- collect all objects stored in object variables
                foreach v [lsort [$obj info var]] {
                    # -- skil self references
                    if { $v == "this" } { continue }
                    if { $v == "This" } { continue }

                    set val [$obj info var $v -val]
                    # -- skip null values
                    if { $val == "" } { continue }
                    # -- skip no object values
                    if { [itcl::find object $val] == "" } { 
                        continue
                    }
                    # -- embedded object
                    set label [namespace tail $v]
                    $tree Add $path/$label -type object/ref \
                        -link /objects/$val -title "$label ($val)"
                }
            }
            object/ref {
                Event <<object>> ""
                Event <<link>> [$tree Get $path link]
            }
            default {
            }
        }
    }
    protected method obj_add {tree path obj} {
        $tree Add /objects/$path -type object -obj $obj \
            -title [obj_title $obj]
    }
    protected method obj_title {obj} {
        set o [namespace tail $obj]
        return "$o ([$obj info class])"
    }
    protected method vtitle {obj var} {
        set tl [namespace tail $var]
        return "$tl ([$obj info var $var -protection])"
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
