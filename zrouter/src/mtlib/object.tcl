# -- NOTICE THIS FILE IS PART OF THE FREELY AVAILABLE OPENSOURCE
# -- STD LIBRARY MTLIB
package provide mtlib 1.0

package require Itcl

#       Class: LObject
# Description: Generic Father of all objects
#      Author: Manolis M. Tsangaris  -- mmt@performance.enet.gr
# Object Archetype
# It supplies the delete method, as well as other interesting common methods
itcl::class LObject {
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    public method delete {} { itcl::delete object $this }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}

#       Class: LNObject
# Description: Beefed up LObject to include support for debugging

# Object Archetype
# It supplies the delete method, as well as other interesting common methods
itcl::class LNObject {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable verbose ""  {verbosity_set $verbose } ; # verbosity level
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable VERBOSE
    protected variable PROG ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        global argv0
        set PROG [file tail $argv0]
    }
    # -------------------------------------------------- Private Methods ---
    # builtin reporting & debugging methods
    protected method warning {msg} {
        if ![info exists VERBOSE(w)] { return }
        puts stderr "warning:$PROG:[$this info class]: $msg"
    }
    protected method notice {msg} {
        if ![info exists VERBOSE(n)] { return }
        puts stderr "notice:$PROG:[$this info class]: $msg"
    }
    protected method perror {msg} {
        puts stderr "error:$PROG:[$this info class]: $msg"
    }
    private method verbosity_set {verbosity} {
        catch {unset VERBOSE}
        if { $verbosity == "*" } {
            set verbosity "notice warning"
        }
        foreach v $verbosity {
            set VERBOSE([string range $v 0 0]) 1
        }
    }
    # ----------------------------------------------- Private Methods
    # -------------------------------------------------- Common Initialization
}
