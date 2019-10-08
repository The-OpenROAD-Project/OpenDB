package provide ade::ui 2.0

#       Class: ZUI
# Description:
#      Author: Manolis M. Tsangaris
#
itcl::class ZUI {
    inherit LProgram
    # -------------------------------------------------- Public Variables --
    public variable pprefix zroute ""      ;# -- prefix for zroute session info
    public variable top "."                ;# -- top installation dir
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables
    protected variable loader ""
    protected variable rpc ""
    protected variable zmgr ""
    protected variable lic  "" ;# license manager
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        option version     -v version "1.0|2.0 (gui version)" "2.0"
        option server      -s server "Server url to connect to (port@host)" ""
        option debug       -d debug  "debug level" 0
        option maxobjects  -l int    "max number of objects to show" 1000000
        option automark    -am "generate auto marks based on object type"
        option cache       -c "cache objects in GUI"
        option esupport -em email   "email for support"   mmt@athenads.com
        option eserver  -es server  "support mail server" smtp.athenadesign.com
        option help  -h "Help"
        option lic     -L  file  "license file" ""
        option tk  -tk "Use tk instead of qt"
        option nostreaming  -ns "do not use streaming"
        option zcdebug  -dzc level "debug dzc (use ? for help)" ""

	package require ade::platform 1.0
	package require ade::license 1.0
	package require ade::media 1.0
	package require ade::ui::apps 1.0
	::platform::init

	LLogger::Auto

        set rpc      [LRPCClient $this.rpc  -debug 0]
        set loader   [ZLoader $this.loader  -rpc $rpc]
    }
    destructor {
        $loader delete
        $rpc delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method lic_check {opt} {
        upvar $opt OPT
	set facility ade.ui

        if { ![::platform::licensing] } { return 0 }
        if { $lic == "" } {
            set lic [ADELicense $this.lic]
        }
        if { [$lic Init R] != 0 } {
            puts stderr "$R(msg)"
            return 1
        }
        # -- checkout failed
        set r [$lic Checkout $facility R]
        if { $r != 0 } {
            puts $R(msg)
            return $r
        }
        set r
    }
    # ----------------------------------------------- Interface Implementation
    protected method vRun {opt} {
        upvar $opt OPT
        if $OPT(help) {
            Usage
            Exit 0
            return
        }
        wm withdraw .

        # -- is our license ok?
        if  { [lic_check OPT] } {
            Exit 100 ; return
        }


	if { $::platform::ZC_TK } {
	    set OPT(tk) 1
	}
	
        $loader configure -debug $OPT(debug)
        set zmgr [ZMGR $this.mgr -loader $loader -rpc $rpc \
		-opts [array get OPT]]
        update idletasks ; update

        # -- important to do this after idle loop
        $zmgr Run init
        $zmgr Wait
        $zmgr delete
        Exit 0
    }
    proc idleinfo {} {
	foreach id [after info] {
	    puts "$id [after info $id]"
	}
    }
    # -------------------------------------------------- Common Initialization
}
