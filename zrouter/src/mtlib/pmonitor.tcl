package require Itcl
package provide mtlib 1.0

#       Class: LProcMonitor
# Description: Spawn an external process and monitor it
#
itcl::class LProcMonitor {
    inherit LObject LBinder
    # Event <<done>> server ;# notify if server has died
    # -------------------------------------------------- Public Variables --
    public variable disabled 0               ;# disabled?
    public variable wait  1                  ;# wait for process attrs?
    # -------------------------------------------------- Public Methods ----
    public method Start {cmd} {
        proc_setup $cmd
    }
    public method Info {fp} {
        if ![info exists INFO($fp)] {
            return ""
        }
        set INFO($fp)
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable CMD
    protected variable INFO
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        array set CMD {}
        array set INFO {}
    }
    destructor {
        foreach fp [array names CMD] {
            cleanup $fp
        }
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method proc_setup {cmd} {
        global tcl_platform
        set pl $tcl_platform(platform) 
        if { $pl != "unix" } {
            set disabled 1
            warning procmonitor "'$pl: no support for monitoring $cmd"
        }
        set fp [open "|$cmd 2@>stderr" r+]
        set CMD($fp) $cmd
        set INFO($fp) ""
        if { $wait } {
            $this eProcMessage $fp
        } else {
            fileevent $fp r "$this eProcMessage $fp"
        }
        set fp
    }
    protected method check_input {fp line} {
        upvar $line ln
        if {[eof $fp] || ([gets $fp ln] == -1) } {
            cleanup $fp
            return 0
        }
        return 1
    }
    protected method cleanup {fp} {
        Event <<done>> $fp
        unset INFO($fp)
        if { [catch {close $fp} rc] } {
            warning procmonitor "$CMD($fp): $rc\n"
        }
        unset CMD($fp)
    }
    # ----------------------------------------------- CallBacks
    public method eProcMessage {fp} {
        if { ![check_input $fp line] } {
            warning procmonitor "server died before sending attributes"
            return
        }
        set INFO($fp) $line

        # setup a new handler to collect output and detect eof
        fileevent $fp r ""
        fileevent $fp r "$this eProcOutput $fp"
    }
    public method eProcOutput {fp} {
        if { [gets $fp line] == -1 } {
            notice procmonitor "$CMD($fp): server died"
            cleanup $fp
            return
        }
        warning procmonitor "$fp -> $line"
    }
    # -------------------------------------------------- Common Initialization
}
