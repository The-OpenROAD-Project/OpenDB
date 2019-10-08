#
# BSD 3-Clause License
#
# Copyright (c) 2019, Nefelus Inc
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


package provide ade::main 1.0

#       Class: ZROUTE
# Description: ZROUTE instance
#      Author: Manolis M. Tsangaris
#
itcl::class ZROUTE {
    inherit LProgram
    # -------------------------------------------------- Public Variables --
    public variable ppath "zroute"                   ;# where to store prefs
    # -------------------------------------------------- Public Methods ----
    public method Cont {} {
        set r $REASON
        set REASON ""
        Signal 0
        return "continue from '$r'"
    }
    public method Pause {{reason pausing}} {
        set REASON $reason
        Wait
    }
    public method Post {args} {
	$EVMGR Post $args
    }
    ## called by zroute::quit 
    ## to simulate a cli quit
    public method Quit {{exit_code 0}} {
        $cli Event <<eof>> $exit_code
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables
    protected variable cli ""
    protected variable prefs ""           ;# preferences
    protected variable sid ""             ;# session id
    protected variable psid ""            ;# parent session id
    protected variable REASON ""          ;# reason for pausing (if any)
    protected variable DBG  ""            ;# the zroute debugger
    protected variable EVMGR ""           ;# the zroute event manager
    protected variable ELOG ""            ;# the zroute event logger
    protected variable lic ""             ;# license manager
    protected variable ISSLAVE 0          ;# true if we are slave
    protected variable PROGNAME           ;# PRODUCT NAME
    protected variable SLAVE_INTERP ""    ;# Slave interpreter 
    protected variable ZLOG ""            ;# 
    protected variable HTASK ""           ;# preferences save task
    protected variable HBEAT 1800         ;# how often to write hbeat info (sec)
    protected variable MAX_DTIME 24       ;# session is dead after so many hours
    protected variable mytimestamp 0      ;# Time Stamp of startup..

    public variable IDLE_LEVEL 2              ;# 0 for no idle update
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

	package require ade::platform
	::platform::init

        set prefs [LPrefs $this.prefs]

        global env
	# -- if ZROUTE_SID is set, we are in slave mode
        if [info exists env(ZROUTE_SID)] {
            set psid $env(ZROUTE_SID)
            set ppath [file join $ppath $psid]
            incr ISSLAVE
        } else {
            set psid ""
            $prefs PathSet $ppath .zr
        }

        option port -p port "port to use (number or range)" "9768-@"
        option rje  -rje auto|ssh|lsf "rje to use (defaults to auto)" ""
        option help  -h "Help"
        option dmsh  -dmsh {notice[:info]} "debug flags to msh" notice:warning
        option dmds  -dmds {notice[:info]} "debug flags to mds" notice:warning
        option cpath -cp   {dir:dir...}    "MDS cache path"       /tmp:.
        option mprofile -mp   profile  "MSH profile to use"    "regtest"
        option mshopts -msh   msh-options  "MSH options"       ""
        option mdsopts -mds   mds-options  "MDS options"       ""
        option nproc   -P   nproc "Number of processors to use" 1
        option mark    -m   "produce time stamps"
        option lic     -L  file  "license file" ""
        option logfile -l file  \
	    "where to log (use name_%n for numbered files)" ""
        option logdir -ld dir  "directory where to place subsystem logs" logs
        option keeplog -kl duration "how long to keep old logs" "30d"
        option dryrun -n "Act like `make -n' for ADE commands"
        option loglink -ll "create a symlink to logging directory" ""
        option nosession -xs "do not create session information" 

        global argv0
        set ps1 "[file tail $argv0]> "

        set mytimestamp [timestamp]
        set myseconds [seconds $mytimestamp]
        #puts "start timestamp: $mytimestamp"

        set SLAVE_INTERP [apps::init $argv0 $myseconds]
        set PROGNAME [file tail $argv0]

        set cli [LSCLI $this.cli -ps1 $ps1 -ps2 "> "\
	    -interp $SLAVE_INTERP -slave $ISSLAVE]
        $cli Bind <<eof>> $this eSessionDone

        # make both line buffering to avoid synchronization issues
        # with the C++ code which does not use the channel abstraction.
        fconfigure stdout -buffering line
        fconfigure stderr -buffering line

    }
    destructor {

	# this may fail if zroute -h is called
        catch {mark main/exit}
        zroute::cleanup
        if { $EVMGR != "" } {
            $EVMGR delete
            $DBG   delete
            $cli delete
            prefs_cleanup
            $prefs delete
        }
        if { $lic != "" } { $lic delete }
        if { $HTASK != "" } { $HTASK delete }
	if { $ZLOG != "" } {
	    $ZLOG delete
	}
    }
    protected method zroute_banner {} {
        global argv0
        set bdate [::sys::getCompileTime]
        #set bdate [file mtime [info nameofexecutable]]
        #set bdate [clock format $bdate -format "%Y/%m/%d"]
        set version $::apps::VERSION
        set appname $::apps::APPNAME

        set msg "NEFELUS Inc $appname $version"
        append msg " ([::sys::getBits]-bit build $bdate)"
        append msg "\nExecutable: $argv0"
        notice $msg
    }
    public method zroute_version { } {
        return $::zroute::VERSION
    }

    public method ade { event args } {
        #puts "ADE event ($event): $args"
        set args [join $args]
        set mod [lindex $args 0]
        set cmd [join [lindex $args 1]]
        set orig [lindex $args 2]
        set m2 [lindex $args 3]

        ## Need this, or many commands fail running in ade regression mode
        if { $SLAVE_INTERP eq "" } {
            if { [string range $cmd 0 12] eq "::apps::milos" } {
                warning "Milos not active - ignoring command $cmd"
                return 0
            }
        }
        return [namespace eval :: $cmd]
    }

    # -- run a slave command in the slave interpeter
    public method slave {args} {
	interp eval $SLAVE_INTERP $args
    }


    public method milos { event args } {
        #puts "Milos event ($event): $args"
        set args [join $args]
        set mod [lindex $args 0]
        set cmd [join [lindex $args 1]]
        set orig [lindex $args 2]
        set m2 [lindex $args 3]


        if { $SLAVE_INTERP != "" } {
            return [interp eval $SLAVE_INTERP $cmd]
        } else {
            warning "Milos not active - command $cmd ignored!"
        }
    }

    public method idle { args } {
        if { $IDLE_LEVEL > 0 } {
            debug ZR:I "idle callback: $args"
            if { $IDLE_LEVEL == 1 } {
                update idletasks
            } elseif { $IDLE_LEVEL == 2 } {
                update
            }
            debug ZR:I "exiting idle callback!"
        }
    }
    # -- initialize the license manager
    # -- return 0 if success, != 0 otherwise

    protected method lmtb_check_dimitris { } {
        #upvar $rr R
        # -- any expiration?
	set expires "3/31/2023"
	set warningtime [expr 5*24*3600]

        #puts "Expiration Day : $expires"
        if { $expires == "" } { return 1 }
        set now [clock seconds]
        set exp [clock scan $expires]
        set dt  [expr $exp - $now]
        if { $dt <= 0 } {
            puts "Nefelus RC Extraction license expired (at $expires)"
            return 1
        }
        if { ($warningtime) && ($dt <= $warningtime) } {
            puts "warning:Nefelus RC Extraction license will expire soon ($expires)"
            return -1
        }
        return 0
    }



    protected method lic_check {opt} {
	upvar $opt OPT
	if { [lmtb_check_dimitris]>0 } {
		return 1
	}
	return 0;
	if { ![::platform::licensing] } { return 0 }
        if { $lic == "" } {
            set lic [ADELicense $this.lic]
        }
        if { [$lic Init R] != 0 } {
            puts stderr "$R(msg)"
            return 1
        }
	if { $ISSLAVE } {
	    set facility $PROGNAME.slave
	} else {
	    set facility $PROGNAME.master
	}
	# -- checkout failed
        set r [$lic Checkout $facility R]
	if { $r != 0 } {
	    puts $R(msg)
	    return $r
        }
	# -- request the MPL
	set r  [$lic Checkout $PROGNAME.mpl R]
	if { $r != 0 } {
	    puts $R(msg)
	    return $r
	}
	set mpl $R(param)
	# -- do not allow MPL > than licensed
	if { $mpl < $OPT(nproc) } {
	    set OPT(nproc) $mpl
	}
        set r
    }
    # ----------------------------------------------- Interface Implementation
    protected method vRun {opt} {
        global argv0
        global argv
        global env
        upvar $opt OPT
        if $OPT(help) {
            Usage
            Exit 0
            return
        }
        # -- change the logfile configuration
        #if { $OPT(logfile) != "" } {
            #LLogConfig -logfile $OPT(logfile)
        #}

        # -- is our license ok?
        if  { [lic_check OPT] } { 

	set no_lic_token NO_LICENSE_TOKEN
	global env
	set no_lic_var ""
	if [info exists env($no_lic_token)] {
		set no_lic_var $env($no_lic_token)
        	puts "Special license will be used: $no_lic_var"
	} else {
        	puts "Special license Required!"
            	Exit 100 ; return
	}
	if { $no_lic_var != "dimitris_friend_expired" } {
        	puts "Special license token name doesn't match!"
            	Exit 100 ; return
	}

	}
        # -- Dimitris - end of license check?

        set EVMGR [ZEventMGR $this.evmgr]
        # -- allow a global zevent to post an event
        interp alias "" ::zevent "" $this Post

        if { $OPT(dryrun) == 1 } {
            puts "Dry Run mode: $OPT(dryrun)!"
            set ::env(DRYRUN) 1
        } else {
            set ::env(DRYRUN) 0
        }

        set OPT(timestamp) $mytimestamp

        if { !$ISSLAVE } {

            ZLog ::zr.zlog -logroot $OPT(logdir) \
                -timestamp $OPT(timestamp) -app [file tail $argv0]

            ::zr.zlog configure -evmgr $EVMGR -cli $cli

            if { $OPT(logfile) != "" } {
                # User has requested a specific name
                ::zr.zlog configure -userlogfile $OPT(logfile)
            } else {
                # Use default name
                ::zr.zlog configure -userlogfile "[file tail $argv0].log"
            }
            if { [info exists OPT(loglink)] == 1 } {
                if { [string trim $OPT(loglink)] != "" } {
                    ::zr.zlog configure -loglink $OPT(loglink)
                } else {
                    zr.zlog configure -loglink "./ldir"
                }
            }

            ::zr.zlog configure -keeplog $OPT(keeplog)
            ::zr.zlog prunelogs

            set OPT(zlog) ::zr.zlog

            set OPT(logdir) "[::zr.zlog getLogdir]/"

            zroute_banner

            if { [info exists ::env(MSHCONFIG)] == 1 } {
                notice "Using MSH Config file $::env(MSHCONFIG)"
            } else {
                ## check in startup directory
                set ms "./msh.conf"
                if { [file exists $ms] } {
                    set ::env(MSHCONFIG) $ms
                    notice "Using MSH Config file $::env(MSHCONFIG)"
                } else {
                    ##no MSHCONFIG set, AND no local file
                    #set ::env(MSHCONFIG) ""
                    notice "Using default MSH Config file"
                }
            }


        } else {

            ## Commands for slave mode - very few..
            set ld "./"
            if { [info exists ::env(ZR_LOGDIR)] } {
                set ld $::env(ZR_LOGDIR)
            }
            if { [::info exists ::env(MSHTID)] } {
                set appn "[file tail $argv0]_$::env(MSHTID)"
                ZLog ::zr.zlog -fulldir $ld  -timestamp $OPT(timestamp) \
		-app $appn
            } else {
                set appn "[file tail $argv0]"
                ZLog ::zr.zlog -timestamp $OPT(timestamp) -app $appn
            }

            ::zr.zlog configure -evmgr $EVMGR -cli $cli

            set OPT(zlog) ::zr.zlog
            set OPT(logdir) "[::zr.zlog getLogdir]/"

            zroute_banner

            $cli reset_bindings
        }

        ## set up user-available parameters
        set cmd "array set [string toupper $PROGNAME] {"
        foreach arg {keeplog logdir logfile loglink timestamp} {
            if { [info exists OPT($arg)] == 1 } {
                if { $OPT($arg) != "" } {
                    append cmd "$arg $OPT($arg) "
                } else {
                    append cmd "$arg \"\" "
                }
            }
        }
        append cmd "}"
        if { $SLAVE_INTERP != "" } {
            interp eval $SLAVE_INTERP $cmd
        } else {
            namespace eval :: $cmd
        }


        set bind "$this idle"
        $EVMGR Bind idle $bind
        set bind "$this milos"
        $EVMGR Bind milos $bind
        set bind "$this ade"
        $EVMGR Bind ade $bind


        set DBG   [ZDBGUI $this.ui -evmgr $EVMGR]
        set OPT(zroute) $this
        set OPT(dbg)   $DBG
        set OPT(evmgr) $EVMGR
        # -- note the session starting time (in sec and milliseconds)
        set OPT(sstarttime) [clock seconds]
        set OPT(starttime)  [clock clicks -milliseconds]
        #parray OPT

        if { [zroute::Init OPT] } {
            Exit 0
            return
        }
        set sid [lindex [split [info hostname] .] 0].[pid]
        set env(ZROUTE_SID) $sid

        if { $psid == "" } {
	    # -- cleanup old sessions
	    if [catch \
		{session_cleanup $ppath .zr [expr $MAX_DTIME*24*60*60]} rc] {
		warning ade "session cleanup failed: $rc"
	    }

            # -- make a note of session parameters
	    set me [lindex [split [info hostname] .] 0] ;# local host name
            $prefs Set host $me "host where it runs"
	    $prefs Set user [LPlatform user] "user"
	    $prefs Set argv0 $argv0         "program"
	    $prefs Set argv  $argv          "args"
	    $prefs Set cwd  [pwd]           "working directory"
            $prefs Set pid  [pid]           "pid"
            $prefs Set port $zroute::PORT   "listening port"
            $prefs Set stim [clock seconds] "starting time"
	    $prefs Set now  [clock seconds] "initial heartbeat"
            # -- and save prefs to the session file
            $prefs Save
	    # -- setup the periodic task heartbeat
	    set HTASK [LSTask $this.htask \
		-timeout [expr $HBEAT*1000] -cmd "$this eHeartBeat"]
        }

        namespace eval :: {
            namespace import zroute::*
        }

        if { $OPT(args) != "" } {
            set cmd "source $OPT(args)"
        } else {
            set cmd ""
        }
        
        mark main/start
        # if startup has failed, abort with exit code 1
        if ![$cli Start $cmd] {
            Signal 1
        }
    }
    protected method prefs_cleanup {} {
        # -- remove the session file
        set f [$prefs cget -file]
        if {$f != "" }  {
            file delete $f
            $prefs configure -file "" ;# avoid saving the file
        }
    }

    # -- cleanup old dead sessions that might have run on this host in the past
    # -- ssuffix is the session suffix
    # -- max_dtime is the time a session is considered to be dead
    # -- REMEMBER: a healthy ade, will refresh its session file every 
    # -- HBEAT seconds
    proc session_cleanup {pprefix {ssuffix .zr} {max_dtime ""}} {
        set base [::LPrefs::PrefDir $pprefix]
	set me [lindex [split [info hostname] .] 0] ;# local host name

	# -- time after which a session is considered "prehistoric!"
	set PTIME [expr 7 * 24 * 60 * 60]
        foreach file [glob -nocomplain [file join $base *$ssuffix]] {
            if ![file isfile $file] { continue }
	    catch {$pref delete}
	    # -- skip errors, just in case
	    if { [catch {LPrefs #auto -file $file} rc] } {
		notice zsession "$file: session read failed ($rc)"
		continue
	    }
	    set pref $rc ;
            catch {unset S} ; array set S [$pref Get] ; $pref delete
            # -- is it junk?
            if ![info exists S(pid)] {
		notice zsession "$file: removing bad signature file"
                catch {file delete $file}
                continue
            }
	    # -- is it a session still running on the same host?
	    set shost [lindex [split $S(host) . ]  0]

            if { $shost == $me } {
                # -- is the process running?
                if ![LProcInfo $S(pid) P] {
                    notice zsession \
			"removing local dead session $S(host):$S(pid)"
                    file delete $file
		    continue
		}
	    }
	    # -- does this session have a heartbeat?
	    if { ($max_dtime != "")  && [info exists S(now)]} {
		# -- how long this session has been alive
		# -- since its last heartbeat
		set dt [expr [clock seconds] - $S(now)]
		if { $dt > $max_dtime  } {
                    notice zsession\
			"$file: removing old session ([time_afmt $dt] inactive)"
                    catch {file delete $file}
		}
		continue
	    }
	    # -- we get here if we have old version zroute that did not
	    # -- refresh its session info
	    # -- we will use a different heuristic here, based on when 
	    # -- the session started
	    set dt [expr [clock seconds] - $S(stim)]
	    if { $dt < 0 } {
		# -- clocks are not synchronized
		continue 
	    }
	    if { $dt > $PTIME } {
		notice zsession\
		    "$file: removing prehistoric session (started [time_afmt $dt] ago)"
		catch {file delete $file}
	    }
        }
    }
    # -------------------------------------------------- CallBacks
    # cli is notifying us our session is done
    # check if the master (if any), wants us to exit
    public method eSessionDone {{exit_code 0}} {
        set res [zroute::session_unregister]
        switch $res {
            OK {  }
            BREAK {
                # requesting to wait for more commands
                set REASON "waiting remote access"
                notice zsession "waiting for remote access"
                return
            }
            default {
                notice zsession "unexpected unregister result: $res"
            }
        }
        Signal $exit_code
    }

    # -- save the prefs periodically
    public method eHeartBeat {} {
	# -- note the time we have saved the prefs, so clenup is possible
	$prefs Set now  [clock seconds] "heartbeat"
	$prefs Save
    }
    # -------------------------------------------------- Private Methods ---
    # -------------------------------------------------- Common Initialization
}

proc bgerror {err} {
    global errorInfo
    set msg "$err\n$errorInfo"
    if [catch {LLog ade warning bgerror:$msg} rc] {
        puts stderr "bgerror: ($msg) $rc"
        flush stderr
    }
}
