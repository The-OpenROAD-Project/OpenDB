package provide mtlib 1.0

package require Itcl
#
#       Class: LPSession
# Description: provide support for 'persistent' session regoistry
#      Author: Manolis M. Tsangaris
#
itcl::class LPSession {
    inherit LObject LSLog
    # -------------------------------------------------- Public Variables --
    public variable dir "psession" {dir_set $dir}     ;# where to store sessions
    public variable suffix session                    ;# session file suffix
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -- register a session
    public method Register {sessionid attrs} {
        ps_register $sessionid $attrs
    }
    # -- remove a session from the registry
    public method UnRegister {sessionid} {
        $this ps_unregister $sessionid
    }
    # -- cleanup registry. Remove any files belong to dead localhost
    # -- processes
    public method Cleanup {} {
        $this ps_cleanup
    }

    # -- make sessionid
    public method NewSession {} {
        $this ps_newsession
    }
    # -- list all sessions 
    public method Sessions {} {
    	$this ps_sessions
    }
    # -- tell me the session status
    public method Status {sessionid} {
	ps_status $sessionid
    }

    # -- get a session state
    public method Get {sessionid} {
	ps_get $sessionid
    }
    # -------------------------------------------------- Protected Variables 
    protected variable DIR ""                           ;#
    protected variable interp ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        if { $DIR == "" } { set DIR $dir }
	set interp [interp create -safe]
    }
    destructor {
	interp delete $interp
    }
    # -------------------------------------------------- Private Methods ---
    # -- expand the dir expression to the right file
    protected method dir_set {dir} {
        set DIR [file normalize $dir]
    }
    # -- given the sessionid produce the file in the registry
    protected method sessionfile {sessionid} {
        file join $DIR $sessionid.$suffix
    }
    # -- collect all session files
    protected method sessionfiles {} {
        set list [glob -nocomplain "$DIR/*.$suffix"]
    }
    # -- write session information 
    protected method write_session {sessionid attrs} {
	set sfile [sessionfile $sessionid]
	set dir [file dirname $sfile]
	if ![file exists $dir] { file mkdir $dir }
	set fp [open $sfile w]
	array set A $attrs
	puts $fp "# -- session information [info hostname] [pid]"
	puts $fp "# -- saved at [clock format [clock seconds]]"
	puts $fp "set SESSION {"
	foreach a [lsort [array names A]] {
	    puts $fp "	[list $a $A($a)]"
	}
	puts $fp "}"
	puts $fp "# -- end of session information"
	close $fp
	return 1
    }
    protected method read_session {sessionid} {
	set sfile [sessionfile $sessionid]
	read_session_file $sfile
    }
    protected method read_session_file {sfile} {
	if ![file exists $sfile] {
	    error "$sfile: session file does not exist"
	}
	# -- well, this should be done, using a safe interpeter at some point!
	catch {interp eval $interp unset SESSION}
	set fp [open $sfile r]
	if [catch {interp eval $interp [read $fp]} rc] {
	    close $fp
	    error "$sfile: failed to read session information ($rc)"
	}
	close $fp
	if [catch {interp eval $interp set SESSION} rc] {
	    error "$sfile: does not contain legal session information"
	}
	set rc
    }
    # -- what is the session status with respect to is host/pid?
    protected method session_status {ss} {
	# only for this host
	upvar $ss S
	# -- is it a remote session?
	if { $S(hostname) != [info hostname] } { return remote }
	# -- is it still alive?
	if ![LProcInfo $S(pid) P] { return dead }
	return alive
    }
    # ----------------------------------------------- Interface Implementation
    # -- register a new session
    protected method ps_register {sessionid attrs} {
        lappend attrs pid [pid] hostname [info hostname] sessionid $sessionid
        lappend attrs wtime [clock seconds]
        write_session $sessionid $attrs
    }

    # -- delete the session file from the registry
    protected method ps_unregister {sessionid} {
        set sfile [sessionfile $sessionid]
        if ![file exists $sfile] { return 0 }
        file delete $sfile
        return 1
    }

    # -- cleanup possibly dead sessions
    # -- nothing to do with remote sessions here
    protected method ps_cleanup {} {
        foreach sfile [sessionfiles] {
            array set S [read_session_file $sfile]
	    switch [Status $S(sessionid)] {
		remote { }
		dead   { UnRegister $S(sessionid) }
		alive  { }
	    }
            unset S
        }
    }
    # -- generate a new 'unique' sessionid
    protected method ps_newsession {} {
        set sessionid [lindex [split [info hostname] .] 0].[pid]
        set sf  [sessionfile $sessionid]
        if ![file exists $sf] { return $sessionid }
        # -- produce a unique file
        for {set i 0} {100} {incr i } {
            set sd1 $sd$i
            set sf  [sessionfile $sessionid]
            if ![file exists $sf] { return $sessionid }
        }
        error "could not create a new sessionid"
    }
    # -- list all sessions with their attributes
    protected method ps_sessions {} {
	set out ""
	foreach f [sessionfiles] {
	    lappend out $f [read_session_file $f]
	}
	set out
    }
    protected method ps_status {sessionid} {
	array set S [Get $sessionid]
	session_status S
    }
    # -- get the session state
    protected method ps_get {sessionid} {
	set sfile [sessionfile $sessionid]
    	read_session $sessionid
    }

    # ----------------------------------------------- TESTS
    public method TEST {} {
	LPSession lp -dir sessiontest
	set sid [lp NewSession]
	puts sid=$sid
	lp Register $sid "mitsos 1 kitsos 2"
	puts [lp Sessions]
	puts [lp Cleanup]
	lp delete
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
