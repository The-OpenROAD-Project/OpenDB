
package provide mtlib 1.0
#       Class: LSCLI
# Description: Simple CLI
#
itcl::class LSCLI {
    inherit ELogger  LBinder
    # Event <<cmd>> cmd : to run a command & return results
    # Event <<eof>>                ;# end of file
    # -------------------------------------------------- Public Variables --
    public variable ps1 ">"
    public variable ps2 ">>"
    public variable fin stdin
    public variable fout stdout
    public variable ferr stderr
    public variable interp ""      ;# interpeter to use
    public variable in  ::         ;# where to evaluate
    public variable interactive "" ;# are we interactive?
    public variable oneof  exit    ;# what to do on eof
    public variable slave 1        ;# are we a slave?
    # -------------------------------------------------- Public Methods ----
    # start the CLI running an optional initcmd
    # if that command has failed return false
    public method Start {{initcmd ""}} {
        cli_setup $initcmd
    }
    # -------------------------------------------------- Virtual Methods ---
    # -- read a line of command from the file & eval
    public method eCmd {fp} {
        if { [gets $fp line] == -1 } { 
            close $fp
            Event <<eof>>
            return
        }
        append CMD $line \n
        if ![info complete $CMD] {
            prompt
            return
        }
        # reset async events until we finish this command
        # just in case the command enables the idle loop
        fileevent $fp r ""

        # run the command -- catch internal errors
        if [catch {runcmd $CMD} rc] {
	    global errorInfo
	    puts stderr "CLI: warning: internal error: $rc"
	    puts stderr "$errorInfo"
	} elseif { $rc } {
            if !$interactive {
                close $fp
                Event <<eof>>
                return
            }
        }

        # reenable the async read
        fileevent $fp r "$this eCmd $fp"
        set CMD ""
        prompt
    }

    public method reset_bindings { } {
        ## reset bindings
        Export
    }
    
    # -------------------------------------------------- Protected Variables 
    protected variable CMD ""          ;# the command so far
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

        global tcl_platform
        switch $tcl_platform(platform) {
            unix {
                # determine interactive mode
                if { $interactive == "" } {
                    if [catch {exec tty -s <@ $fin}] {
                        set interactive 0
                    } else {
                        set interactive 1
                    }
                }
            }
            default {
                # play it safe on windows
                set interactive 1
            }
        }
        ## Export global logging commands
        Export
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # return false if the initial cmd has failed
    protected method cli_setup {initcmd} {
        if { ($initcmd != "") && ([runcmd $initcmd]) } {
            # errors in initcmd
            return 0
        }
        fileevent $fin r "$this eCmd $fin"
        fconfigure $fout -buffering line
        prompt
        return 1
    }

    protected method prompt {} {
        if { !$interactive } { 
            update idletasks
            return
        }
        if {$CMD == "" } {
            puts -nonewline $fout $ps1
            cmdPost "prompt $ps1"
        } else {
            puts -nonewline $fout $ps2
            cmdPost "prompt $ps2"
        }
        flush $fout
        update idletasks
    }
    
    protected method cli_out {msg} {
        if { $interactive } {
            puts $fout $msg
            cmdPost "cmdo {$msg}"
        }
    }
    # print out a message
    # in interactive mode, just print the message and ignore
    # in non interactive mode, report an exeption and stop processing
    protected method cli_error {msg} {
        if { $interactive } {
            puts $fout $msg
        } else {
            puts $ferr $msg
        }
        cmdPost "clierror {$msg}"
    }
    protected method append_file { fname args } {
        set fp [open $fname "a"]
        puts $fp [join $args]
        close $fp
    }

    protected method rm_module { args } {
        mydebug ERRDEB:A "CM: $args"
        set cnt 0
        set have_mod 0
        set have_instr 0
        set brace 0
        set al [split $args " "]
        foreach arg $al {
            mydebug ERRDEB:A  "CM($cnt): $arg"
            if { $cnt == 0 } {
                if { ([string range $arg 0 2] == "\{::") && ([string range $arg end end] == ":") } {
                    mydebug ERRDEB:A "ERRDEB:A have mod!"
                    incr have_mod
                }
            }
            if { ($cnt == 1) && ($have_mod == 1) } {
                if { [string range $arg end end] == ":" } {
                    mydebug ERRDEB:A "ERROR:A have instr!"
                    incr have_instr
                }
            }
            incr cnt
        }
        set cargs {}
        if { $have_instr == 1 } {
            set cargs [join [lrange $al 2 end]]
        } else {
            set cargs [join $args]
        }

        set cl [split $cargs "\n"]
        set ll [llength $cl]
        set cnt 0

        set sidx 1
        set eidx 1
        set line [lindex $cl $sidx]

        ## if no "while executing line", add it - get rid of all "invoked" lines
        if { !($line eq "    while executing") } {
            set cl [linsert $cl $sidx "    while executing"]
        }
        incr sidx
        set we_start $sidx

        set ns [find_block $sidx $ll $we_start $cl]
        while { [lindex $ns 0] != -1 } {
            set eidx [lindex $ns 1]
            set ns [find_block [expr {$eidx+1}] $ll $we_start $cl]
        }

        mydebug ERRDEB:A "CA: $cargs"
        #mydebug ERRDEB:A "FB: $sidx, $eidx"
        set cl2 [lreplace $cl $sidx $eidx]
        foreach line $cl2 {
            #mydebug ERRDEB:A "CLA: $line"
        }
        set rs ""
        foreach line $cl2 {
            append rs "${line}\n"
        }
        return $rs
    }

    protected method find_block { sidx ll we_start cl } {
        set flag 0
        #mydebug ERRDEB:A "    FB: $sidx, $ll, $we_start"
        foreach line $cl {
            #mydebug ERRDEB:A "        FB: $line"
        }
        while { $sidx < $ll } {
            set line [lindex $cl $sidx]
            #mydebug ERRDEB:A "    FB: test line: $line"
            set tofind $::apps::cur_app
            if { [string first "(object \"$tofind" $line] >= 0 } {
                #mydebug ERRDEB:A "    FB: Found object: $line"
                set eidx [expr {$sidx + 1}]
                if { $sidx != $we_start } {
                    incr sidx -1
                }
                set flag 1
                break
            }
            incr sidx
        }
        if { $flag == 1 } {
            #mydebug ERRDEB:A "    FB: Return: $sidx, $eidx"
            return [list $sidx $eidx]
        } else { 
            #mydebug ERRDEB:A "    FB: Return: -1, -1"
            return [list -1 -1]
        }
    }


    protected method mydebug { mod args } {
        set ml [split $mod ":"]
        set m2 [lindex $ml 0]
        if { [::zr.zlog get_debug_log_config -module $m2] } {
            puts "$mod [join $args]"
        }
    }
        
    
    public method runcmd {cmd} {
        ## get rid of initial "source" for logging
        set einfo ""
        set ecode 0
        set stack [lindex [info level [expr [info level] -1]] 0]
        if { $stack != "cli_setup" } {
            cmdPost "cmdi ${cmd}"
        }

        set cmd [string trim $cmd]
        set ecmd ""
        if { $cmd == "" } { return 0}
        # is there a binding?
        if { [Handler <<cmd>>] == "" } {
            if { ([string length $cmd] > 1) && ([string index $cmd 0] == "!")} {
                ## History manipulation command
                set mod [string index $cmd 1]
                if { ($mod == "!") || [string is alnum $mod]  } {

                    if { $interp != "" } {
                        set nid [expr [interp eval $interp "history nextid"] - 1]
                    } else {
                        set nid [expr [history nextid] - 1]
                    }
                    set ei [string first " " $cmd]
                    if { $ei < 0 } {
                        set rem ""
                        set ei [expr [string length $cmd] -1]
                    } else {
                        set rem [string range $cmd [expr $ei + 1] end]
                    }
                    if { [string index $cmd 1] != "!" } {
                        set offset [string range $cmd 1 $ei]
                        if { $offset < 0 } {
                            set nid [expr $nid + $offset + 1]
                        } elseif { $offset <= $nid } {
                            set nid [expr int($offset)]
                        } else {
                            set nid -1
                        }
                    }
                    if { [string is integer $nid] && ($nid > 0) } {
                        if { $interp != "" } {
                            set cmd [interp eval $interp "history event $nid"]
                            append cmd $rem
                        } else {
                            set cmd [history event $nid]
                            append cmd $rem
                        }
                    } else { 
                        set cmd ""
                    }
                } else {
                    set cmd ""
                }
                #cli_out "final cmd: $cmd"
                cli_out "$cmd"
            } 
            if { $interp != "" } {
                interp eval $interp "history add {$cmd}"
                set n [catch {interp eval $interp [list uplevel #0 $cmd]} rc]

                if { $n } { 
                    set einfo $::errorInfo 
                    set ecode $::errorCode 
                    set ecmd [list uplevel #0 $cmd]
                    mydebug "ERROR:A ETest 1: $ecode\nEI: $einfo\nEC: $ecmd\nN:$n"
                    set einfo [rm_module $einfo]
                }
            } else {
                if { $cmd != "history" } {
                    history add [concat $cmd]
                }
                set n [catch {namespace eval $in [list uplevel #0 $cmd]} rc]
                if { $n } { 
                    set einfo $::errorInfo 
                    set ecode $::errorCode 
                    mydebug ERROR:A "ETest 2: $ecode"
                    set ecmd [list uplevel #0 $cmd]
                }
            }
        } else {
            set n [catch {Event <<cmd>> $cmd} rc]
            if { $n } { 
                set einfo $::errorInfo
                set ecode $::errorCode 
                mydebug ERROR:A "ETest 3: [llength $einfo], $ecode"
            }
        }
        if { $n } {
            mydebug ERRDEB:A "Starting einfo: $einfo"
            mydebug ERRDEB:A "EEIN: $ecode\n\n"

            ## strip off last "uplevel"
            set idx [string first $ecmd $einfo]
            set idx2 [string first "...\"\n" $einfo]
            if { $idx > 0 } {
                set einfo [string range $einfo 0 [expr $idx - 2]]
                mydebug ERRDEB:A "NE ($idx, $idx2): $einfo\nXXX\n" 
                set idx [string last "invoked from within" $einfo]
                if { $idx > 0 } {
                    set einfo [string range $einfo 0 [expr $idx - 2]]

                    set idx [string last "uplevel" $einfo]
                    if { $idx > 0 } {
                        set einfo [string range $einfo 0 [expr $idx - 7]]
                    }
                }
                mydebug ERRDEB:A "ECT: $ecode"
                ## if -2, remove consecutive 
                ## "while executing" and "invoked from within"
                if { $ecode == -2 } {
                    ## special, invalid command sequence
                    set el [split $einfo "\n"]
                    set idx [string first "while executing" [lindex $el 1]]
                    set idx2 [string first "invoked from within" [lindex $el 2]]
                    mydebug ERRDEB:A "EL: $idx, $idx2"
                    if { ($idx >= 0) && ($idx2 >=0 ) } {
                        ## found it - remove
                        set el [lreplace $el 2 2]
                        set einfo [join $el "\n"]
                    }
                    mydebug ERRDEB:A "ELN: [join $el "\n"]"

                }
            } elseif {$idx2 > 0 } {
                mydebug ERRDEB:A "Not found!"
                ## Try new approach
                set cmpstr "\"interp eval \$interp \[list uplevel #0 \$cmd]\""
                set eshort ""
                set  el [split $einfo "\n"] 
                set idx 0
                set flag 0
                foreach arg $el {
                    if { [string compare $arg $cmpstr] == 0 } {
                        set flag 1
                        break
                    }
                    incr idx
                }
                if { $flag == 1 } {
                    ## found - remove 5 lines..
                    set einfo ""
                    set rmidx [expr $idx - 5]
                    for { set i 0 } { $i <= $rmidx } { incr i } {
                        append einfo [lindex $el $i]
                        append einfo "\n"
                    }
                }
            }
            cli_error "Error: $einfo" ; flush stdout
        } else {
            cli_out "$rc" ; flush stdout
        }
        return $n
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: LSafeCLI
# Description: Safe CLI using a safe interpeter
#
itcl::class LSafeCLI {
    inherit LObject  LBinder
    # -------------------------------------------------- Public Variables --
    public variable ps1 ">"
    public variable ps2 ">>"
    public variable fin stdin
    public variable fout stdout
    public variable interactive "" ;# are we interactive?
    public variable oneof  exit   ;# what to do on eof
    # -------------------------------------------------- Public Methods ----
    public method Start {{initcmd ""}} {
        cli_setup $initcmd
    }
    public method Bind {local global} {
        set cmd [list interp alias $INTERP $local {}]
        foreach n $global {
            lappend cmd $n
        }
        eval $cmd
    }
    public method Alias {a b} {
        set cmd [list interp alias $INTERP $a $INTERP]
        foreach n $b {
            lappend cmd $n
        }
        eval $cmd
    }
    public method UnBind {local} {
        interp alias {} $local ""
    }
    # -------------------------------------------------- Virtual Methods ---
    # -- read a line of command from the file & eval
    public method eCmd {fp} {
        if { [gets $fp line] == -1 } { 
            fileevent $fp r ""
            runcmd $oneof
            return
        }
        append CMD $line \n
        if ![info complete $CMD] {
            prompt
            return
        }
        # reset async events until we finish this command
        # just in case the command enables the idle loop
        fileevent $fp r ""

        # run the command
        runcmd $CMD

        # reenable the async read
        fileevent $fp r "$this eCmd $fp"
        set CMD ""
        prompt
    }
    
    # -------------------------------------------------- Protected Variables 
    protected variable CMD ""          ;# the command so far
    protected variable INTERP ""       ;# slave interpeter
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

        global tcl_platform
        switch $tcl_platform(platform) {
            unix {
                # determine interactive mode
                if { $interactive == "" } {
                    if [catch {exec tty -s <@ $fin}] {
                        set interactive 0
                    } else {
                        set interactive 1
                    }
                }
            }
            default {
                # play it safe on windows
                set interactive 1
            }
        }
        set INTERP [interp create -safe]
        # remove all commands from the interpeter
        foreach cmd [interp eval $INTERP info commands] {
            switch $cmd {
                rename - info { continue } 
            }
            interp eval $INTERP rename $cmd {{}}
        }
        interp eval $INTERP rename rename {{}}
    }
    destructor {
        interp delete $INTERP
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method cli_setup {initcmd} {
        fileevent $fin r "$this eCmd $fin"
        fconfigure $fout -buffering line
        if { $initcmd != "" } {
            runcmd $initcmd
        }
        prompt
    }

    protected method prompt {} {
        if { !$interactive } { 
            update idletasks
            return
        }
        if {$CMD == "" } {
            puts -nonewline $fout $ps1
        } else {
            puts -nonewline $fout $ps2
        }
        flush $fout
        update idletasks
    }
    
    protected method cli_out {msg} {
        if { $interactive } {
            puts $fout $msg
        }
    }
    
    protected method runcmd {cmd} {
        set cmd [string trim $cmd]
        if { $cmd == "" } { return }
        # is there a binding?
        set n [catch [list uplevel #0 [list interp eval $INTERP $cmd]] rc] 
        if { $n } {
            cli_error "error: $rc" ; flush stdout
        } else {
            cli_out "$rc" ; flush stdout
        }
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: LSafeInterp
# Description: Safe interpeter
#
itcl::class LInterp {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable safe 0                   ;# is it safe ?
    # -------------------------------------------------- Public Methods ----
    public method Run {cmd} {
        interp eval $INTERP $cmd
    }
    public method Bind {local global} {
        set cmd [list interp alias $INTERP $local {}]
        foreach n $global {
            lappend cmd $n
        }
        eval $cmd
    }
    public method Alias {a b} {
        set cmd [list interp alias $INTERP $a $INTERP]
        foreach n $b {
            lappend cmd $n
        }
        eval $cmd
    }
    public method UnBind {local} {
        interp alias {} $local ""
    }
    public method Exists {alias} {
	if { [interp alias $INTERP $alias] != "" } {
	    return 1
	}
	return 0
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable INTERP ""       ;# slave interpeter
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

        set INTERP [interp create -safe]
        # remove all commands from the interpeter
        if { $safe } {
            foreach cmd [interp eval $INTERP info commands] {
                switch $cmd {
                    rename - info { continue } 
                }
                interp eval $INTERP rename $cmd {{}}
            }
        }
        interp eval $INTERP rename rename {{}}
    }
    destructor {
        interp delete $INTERP
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
