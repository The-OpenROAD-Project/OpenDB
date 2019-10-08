package require Itcl
package provide mtlib 1.0

#
# Class:       LToplevel
# Description: Standard Toplevel Itcl widget
#
itcl::class LTopLevel {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable Class     "" 
    public variable title     "toplevel" 
    public variable icontitle "toplevel"
    public variable resize    0  { ifwin if $resize { wm minsize $This 0 0 } }
    public variable stitle    "" 
    public variable geometry  "" 
    # remote screen
    public variable screen    "" 
    # -------------------------------------------------- Protected Variables 
    protected variable This
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    public method Hide     {} { wm withdraw $This }
    public method UnHide   {} { wm deiconify $This }
    public method Iconify  {} { wm iconify $This }
    public method Show     {} { wm deiconify $This }
    public method IsMapped {} { winfo ismapped $This }
    public method Window   {} { set This }
    public method Raise    {} { raise $This }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Private Methods ---
    constructor {args} {
        eval configure $args
        if { $Class == "" } {
            set Class [$this info class]
            regsub {^::} $Class  "" Class
        }
        set This $this ; regsub {^::[^\.]*} $This  "" This
        if { $screen != ""  } {
            itcl_toplevel $this $This -screen $screen -class $Class 
        } else {
            itcl_toplevel $this $This -class $Class 
            global env
            if [info exists env(DISPLAY)] {
                set screen $env(DISPLAY)
            } else {
                set screen :0.0
            }
        }
        LBridge title     "wm title $This "
        LBridge icontitle "wm iconname $This "
        LBridge geometry  "wm geometry $This "
        LBridge stitle    "wm iconname $This "
    }
    destructor {
        itcl_cleanup $This
    }
    # -------------------------------------------------- 
    public method winconfig {args} { eval $This-win- config $args }
    public method wincget {args}   { eval $This-win- cget $args }
    public method ifwin {args} {
        if ![winfo exists $This] {return}
        uplevel #0 $args
    }
    # -------------------------------------------------- Common Initialization
}

#
# Class:       LTopLevelEditor
# Description: Editor Like toplevel window (with menus and buttons)
#
itcl::class LTopLevelEditor {
    inherit LTopLevel
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Protected Variables 
    protected variable _children {}
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    public method vMakeMenus   {w} { }
    public method vMakeButtons {w} { }
    public method vMakeToolbar {w} { }
    public method eChildDone {w} { $w delete }
    # -------------------------------------------------- Private Methods ---
    constructor {args} {
        eval configure $args
        pack [mk_menus   $This.menus]   -side top -fill x 
        pack [mk_toolbar $This.toolbar]   -side top -fill x 
        pack [mk_buttons $This.buttons] -side bottom -fill x
    }
    destructor {
        foreach c $_children {
            if { [itcl_info object $c ] == $c } {
                # puts "[info class]: deleting object $c"
                catch {itcl::delete object $c}
            } else {
                catch {destroy $c}
            }
        } 
    }
    protected method mk_menus {w} {
        frame $w -class Menus
        after idle "$this vMakeMenus $w"
        set w
    }
    protected method mk_buttons {w} {
        frame $w -class Buttons
        after idle "$this vMakeButtons $w"
        set w
    }
    protected method mk_toolbar {w} {
        frame $w -class Toolbar -border 1 -relief sunken
        after idle "$this vMakeToolbar $w"
        set w
    }
    protected method menu_add {w args} {
        set m $w.m
        eval menubutton $w $args -menu $m
        pack $w -side left
        menu $m -tearoff 0
        set m
    }
    protected method menu_add_right {w args} {
        set m $w.m
        eval menubutton $w $args -menu $m
        pack $w -side right
        menu $m -tearoff 0
        set m
    }
    protected method button_add {w args} {
        eval button $w $args
        pack $w -side left
    }
    protected method button_add_right {w args} {
        eval button $w $args
        pack $w -side right
    }
    protected method toolbutton_add {w args} {
        eval button $w $args
        pack $w -side left
    }
    protected method toolbutton_add_right {w args} {
        eval button $w $args
        pack $w -side right
    }
    # add a child to the list of subordinate windows
    protected method child_add {{w ""}} {
        if { $w == "" } {
            set w [itcl_gensym $This.ch]
        }
        lappend _children $w
        set w
    }
    # -------------------------------------------------- USEFUL PROCEDURES
    proc get_file {fb msg} {
        LPlaceParent $fb
        LWithFocus $fb {
            update idletasks ; update
            $fb config -msg $msg
            $fb Show ; $fb Refresh
            if { [$fb Wait] == "ok" } {
                set f [$fb Selection]
            } else {
                set f ""
            }
            $fb Hide
        }
        return $f
    }
    # -------------------------------------------------- Common Initialization
}

#
# Class:       LFrame
# Description: Standard Toplevel Itcl widget
#
itcl::class LFrame {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable Class  ""
    public variable width  0
    public variable height 0
    # -------------------------------------------------- Protected Variables 
    protected variable This
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    # -------------------------------------------------- Public Methods ----
    method Window {} { set This }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Private Methods ---
    constructor {args} {
        eval configure $args
        if { $Class == "" } {
            set Class [$this info class]
            regsub {^::} $Class  "" Class
        }
        set This $this ; regsub {^::[^\.]*} $This  "" This
        itcl_frame $this $This -class $Class

        LBridge width  "$This-win- configure -width "
        LBridge height "$This-win- configure -height "
    }
    destructor {
        destroy $This
    }
    public method winconfig {args} { eval $This-win- config $args }
    public method   wincget {args} { eval $This-win- cget $args }
    public method ifwin {args} {
        if ![winfo exists $This] {return}
        uplevel #0 $args
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: LApplication
# Description: Graphical Application
#
itcl::class LApplication {
    inherit LTopLevelEditor LInteractor
    # -------------------------------------------------- Public Variables --
    public variable rcfile   ""
    # -------------------------------------------------- Public Methods ----
    method Run {argv} {
    }
    # -------------------------------------------------- Virtual Methods ---
    # -- this method is called on idle after initialization, to 
    # -- do any of the necessary post initialization bookeeping
    public method eAfterInit {} {
        if { $rcfile == "" } { return }
        if { $rcfile == "-" } {
            global env
            set rcfile $env(HOME)/.${progname}rc
        }
        $prefs configure -file $rcfile
        $this eRestore
    }
    # -- this method is called by eAfterInit (after idle and all constructors) 
    # -- to do any restoration
    protected method eRestore {} {
        # -- just setup the geometry of the main application window
        set geom [$prefs Get $This.geometry]
        if { $geom != "" } {
            wm geometry $This $geom
        }
    }
    # -- this method is called before the destructor, to do a cleanup
    protected method eCleanup {} {
        $prefs Set $This.geometry [wm geometry $This] "Top level geometry"
    }
    # -------------------------------------------------- Protected Variables 
    protected variable prefs ""
    protected variable progname "application"
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set prefs [LPrefs $This.prefs]

        global argv0
        set progname [file root [file tail $argv0]]

        after idle "$this eAfterInit"
    }
    destructor {
        $this eCleanup
        $prefs delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method file_save_dialog {args} {
        eval tk_getSaveFile -parent $This $args
    }
    protected method file_open_dialog {args} {
        eval tk_getOpenFile -parent $This $args
    }
    public method mFileOpen  {} { }
    public method mFileSave  {} { }
    public method mFileClose {} { }
    public method mFileQuit  {} {
        Signal 0
    }
    public method mEditClear {} {}

    public method vMakeMenus {w} {
        set m [menu_add $w.file -text File]
            $m add command -label "Open ..." -command "$this mFileOpen"
            $m add command -label "Save ..." -command "$this mFileSave"
            $m add command -label  Close     -command "$this mFileClose"
            $m add separator
            $m add command -label  Quit      -command "$this mFileQuit"

        set m [menu_add $w.edit -text Edit]
            $m add command -label Clear  -command "$this mEditClear"

        set w
    }
    # ---------------------------------------------------- 
    protected method warning {msg} {
        puts stderr "warning:$msg"
    }
    protected method dinfo   {msg} {
        puts stderr "info:$msg"
    }
    # -------------------------------------------------- Common Initialization
}
