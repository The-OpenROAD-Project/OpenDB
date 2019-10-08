package provide ade::ui 2.0

#       Class: ZConsole
# Description: Console to talk to the zroute
#      Author: Manolis M. Tsangaris
#
itcl::class ZConsole {
    inherit LTopLevelEditor
    # -------------------------------------------------- Public Variables --
    public variable rpc ""       ;# server to connect to
    public variable hfile "hist" ;# suffix of the history file
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable hist ""
    protected variable con  ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        configure -title "zroute console"
        set hist [LConsoleHistory $this.hist]
        set con  [LConsole $This.con -hist $hist -width 80 -height 15]
        $con Bind <<cmd>> $this eRunCmd
        pack $con -side top -fill both -expand 1
    }
    destructor {
        $hist delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    public method vMakeButtons {w} {
        button_add $w.hide  -text Hide  -command "$this Hide"
        button_add $w.clear -text Clear -command "[namespace code $con] Clear"
    }
    # -------------------------------------------------- CallBacks       ---
    public method eRunCmd {cmd} {
        $con Show cmd $cmd
        # is it a local command?
        if { [string match !* $cmd] } {
            set cm "uplevel #0 [string range $cmd 1 end]"
        } else {
            set cm "uplevel #0 $rpc Run $cmd"
        }
        if { [catch $cm rc] } {
            $con Show err $rc
        } else {
            $con Show out $rc
        }
    }
    # -------------------------------------------------- Common Initialization
}
