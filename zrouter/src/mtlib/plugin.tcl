package provide mtlib 1.0

#       Class: LPlugin
# Description: Spawn an external process and monitor it and send commands
#	cloned from pmonitor
#
itcl::class LPlugin {
    inherit LObject LBinder
    # Event <<done>>   fp             ;# notify if server has died
    # Event <<notice>> level msg
    # Event <<result>> fp result      ;# result of a command
    # -------------------------------------------------- Public Variables --
    public variable disabled 0               ;# disabled?
    # -------------------------------------------------- Public Methods ----
    # -- start a server. Return its pipe stream
    public method Start {cmd} {
        proc_setup $cmd
    }
    # -- send a command to a running server
    public method Send {fp command} {
	sendCommand $fp $command
    }

    # -- terminate a command
    public method Cleanup {fp} {
	cleanup $fp
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable CMD
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        array set CMD {}
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
            notice 0 "'$pl: no support for monitoring $cmd"
	    return
        }
	notice 2 "running: $cmd"
        set fp [open "| $cmd 2>@stderr" r+]
	fconfigure $fp -buffering line
        set CMD($fp) $cmd
	fileevent $fp r "$this eProcOutput $fp"
        set fp
    }
    # -- check one line from $fp
    protected method check_input {fp line} {
        upvar $line ln
        if {[eof $fp] || ([gets $fp ln] == -1) } {
            cleanup $fp
	    Event <<done>> $fp
            return 0
        }
        return 1
    }
    protected method cleanup {fp} {
        if [info exists CMD($fp)] { unset CMD($fp) } 
	catch {fconfigure $fp -blocking 0}
	if [catch {pid $fp} pid] { 
	    return
	}
        if { [catch {close $fp} rc] } {
            notice 1 "$fp: close: $rc\n"
        }
	after 500
	catch {exec kill -HUP $pid}
	catch {exec kill -INT $pid}
    }
    protected method notice {level msg} {
	Event <<notice>> $level $msg
    }
    protected method sendCommand {fp command} {
	notice 1 "$fp: cmd: $command"
	puts $fp $command
	flush $fp
    }
    # ----------------------------------------------- CallBacks
    public method eProcOutput {fp} {
        if { [gets $fp line] == -1 } {
            notice 1 "$fp: server died"
            cleanup $fp
	    Event <<done>> $fp
            return
        }
        notice 2 "output: $fp -> $line"
	Event <<result>> $fp $line
    }
    # -------------------------------------------------- Common Initialization
}
