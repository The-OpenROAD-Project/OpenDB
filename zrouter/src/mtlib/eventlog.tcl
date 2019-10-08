package require Itcl
package provide mtlib 1.0

#
#       Class: ELogger
# Description: Turns Log output into Events for the zroute event manager
#      Author: Mattias Hembruch
#
itcl::class ELogger {
    inherit LObject

    protected variable elogged 1;
    protected variable proc_error "::_zlogsave::error"
    
    public method warning { {msg "" } args} {
        if { [llength $args] == 0 } {
            genevent "warning" "$msg"
        } else {
            genevent "warning" "$msg $args"
        }
    }

    public method notice {{msg "" } args} {
        if { [llength $args] == 0 } {
            genevent "notice" "$msg"
        } else {
            genevent "notice" "$msg $args"
        }
    }

    public method verbose {{msg "" } args} {
        if { [llength $args] == 0 } {
            genevent "verbose" "$msg"
        } else {
            genevent "verbose" "$msg $args"
        }
    }

    public method debug {msg args} {
        set mgr [ itcl::find objects -isa ZEventMGR "::zr.evmgr"]
        if { $mgr != "" } {
            ::zr.evmgr Post "debug $msg {[join $args]\n}"
        } else {
            puts -nonewline "*** debug: $msg, $args\n"
        }
    }

    public method Post { level args } {
        genevent "$level" "$args"
    }

    public method cmdPost { level args } {
        set mgr [ itcl::find objects -isa ZEventMGR "::zr.evmgr"]
        if { $mgr != "" } {
            ::zr.evmgr Post "$level $args"
        }
    }

    protected method genevent {level msg} {
        set mgr [ itcl::find objects -isa ZEventMGR "::zr.evmgr"]
        if { $mgr != "" } {
            ::zr.evmgr Post "$level 4 {\"$msg\n\"}"
        } else {
            puts -nonewline "$level: $msg\n"
        }
        set elogged 1
    }

    # export some Log methods as flat commands
    public method Export {} {
        LExportMethods $this warning 
        LExportMethods $this debug 
        LExportMethods $this notice 
        LExportMethods $this verbose 
    }

    constructor {args} {
    }
    destructor {
    }
}
