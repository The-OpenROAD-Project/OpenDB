
package provide mtlib 1.0
#       Class: LSCLI
# Description: Simple CLI
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris
#
itcl::class LSCLI {
    inherit LObject  LBinder
    # Event <<cmd>> cmd : to run a command & return results
    # -------------------------------------------------- Public Variables --
    public variable ps1 ">"
    public variable ps2 ">>"
    public variable fin stdin
    public variable fout stdout
    public variable in  ::         ;# where to evaluate
    public variable interactive "" ;# are we interactive?
    public variable oneof  exit   ;# what to do on eof
    # -------------------------------------------------- Public Methods ----
    public method Start {{initcmd ""}} {
        cli_setup $initcmd
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
    }
    destructor { }
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
        if { [Handler <<cmd>>] == "" } {
            set n [catch {namespace eval $in [list uplevel #0 $cmd]} rc]
        } else {
            set n [catch {Event <<cmd>> $cmd} rc]
        }
        if { $n } {
            cli_out "error: $rc" ; flush stdout
        } else {
            cli_out "$rc" ; flush stdout
        }
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: LSafeCLI
# Description: Safe CLI using a safe interpeter
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris
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
	    global errorInfo
            # cli_out "error: $rc" ; flush stdout
            cli_out "$errorInfo" ; flush stdout
        } else {
            cli_out "$rc" ; flush stdout
        }
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: LSafeInterp
# Description: Safe interpeter
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris
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
