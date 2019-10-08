package provide smpx 1.0
#source foo.tcl
#
#       Class: SMPX
# Description:
#      Author: Manolis M. Tsangaris  -- mmt@performance.enet.gr
#   Copyright: Manolis M. Tsangaris, Sinastria, 2003
#
itcl::class SMPX {
    inherit LProgram
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable smpx ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        option help -help "command help"
	option address  -address port@host "smpx address" 7679@
	option client   -client  "client mode"
	option print    -info    "print smpx config info on stdout"
	option wait     -w       "wait for EOF in stdin to shutdown"
	option test     -demo    "test smpx with a simple demo"
	option logfile  -log  file "where to put the log (stderr)" ""
    }
    destructor {
	if { $smpx != "" } { $smpx delete } 
    }
    # -------------------------------------------------- Private Methods ---
    protected method monitor_eof {channel} {
	fileevent $channel r "$this eChannelCheck $channel"
    }
    # ----------------------------------------------- CallBacks
    # check for end of file status and exit on EOF
    public method eChannelCheck {fp} {
        if { [gets $fp line] == -1 } {
            LLog smpx notice "peer closed channel. quiting"
            catch {close $fp}
	    $smpx Signal 0
            Signal 0
	    # -- make sure you exit (just in case)
	    after 10000 "exit"
        }
    }
    # ----------------------------------------------- Interface Implementation
    protected method vRun {opt} {
        upvar $opt OPT
        if { $OPT(help) } {
            Usage
            return
        }
	if { $OPT(logfile) != "" } { 
	    LLogger::Auto -logfile $OPT(logfile)
	} else {
	    LLogger::Auto
	}
	if { $OPT(client) } {
	    set smpx [LSMPXClient $this.smpx -address $OPT(address)]
	} elseif { $OPT(test) } {
	    set demo [SMPXDemo $this.demo -address $OPT(address)]
	    $demo Test
	    $demo delete
	} else {
	    if { $OPT(wait) } { monitor_eof stdin }
	    set smpx [LSMPXServer $this.smpx -address $OPT(address)]
	    LLog smpx notice "smpx server starting ([$smpx Info])"
	    if { $OPT(print) } {
		puts stdout [$smpx Info]
		flush stdout
	    }
	    $smpx Wait
	    LLog smpx notice "smpx server exiting"
	}
    }
    # -------------------------------------------------- Common Initialization
}
