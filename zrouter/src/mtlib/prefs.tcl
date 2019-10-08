package provide mtlib 1.0
package require Itcl

#
# Manage user preferences ...
#
# User preferences are stored in a file as tcl variables
#
# The class will automatically read the prefs on construction
# NOTE: prefs need to be explicitly saved using Save
#
#
itcl::class LPrefs {
    inherit LObject
    # ---------------------------------------------------- Public Variables ---
    public variable file    ""         ;# where to save prefs
    public variable version "1.0"
    # ---------------------------------------------------- Protected Variables 
    protected variable Pref
    # ---------------------------------------------------- Common Variables ---
    # ---------------------------------------------------- Configuration ------
    constructor {args} {
        eval configure $args
        array set Pref {}
        LBridge file "$this eLoad"
    }
    destructor  {
    }
    # ---------------------------------------------------- Public Methods ------
    method Save {} { prefs_save }
    method Get {{key ""}} {$this vMatch $key}
    method Set {key pref {comment ""}} {
        set Pref($key) $pref
        set Comment($key) $comment
    }
    method Print {} {
        parray Pref
    }
    public method SaveGeometry {tag w} {
        #set    g =[winfo width $w]x[winfo height $w]
        set     g +[winfo rootx $w]+[winfo rooty $w]
        Set $tag $g
        set g
    }
    public method GetGeometry {tag w} {
        set g [Get $tag]
        if { $g == "" } { return ""} 
        wm geometry $w $g
        set g
    }
    public method PathSet {prefix {suff pref}} {
        set fdir [PrefDir $prefix]
        if { $fdir == "" } { return "" }
        set file [path_make $fdir $suff]
    }
    # generate prefs directory using hostname and pid and prefix
    public proc PrefDir {prefix} {
        global ::tcl_platform
        global env
        set pathdir ""
        switch $::tcl_platform(platform) {
            windows {
                if [info exists env(USERPROFILE)] {
                    set pathdir [file join $env(USERPROFILE) $prefix]
                }
            }
            unix    {
                set pathdir [file join [glob ~] .$prefix]
            }
            default {
                error "unsupported platform: $::tcl_platform(platform)"
            }
        }
        set pathdir
    }
    # ---------------------------------------------------- Virtual Methods -----
    public method vMatch {key} {
        if { $key == "" } { return [array get Pref] }
        if ![info exists Pref($key)] {
            return ""
        }
        return $Pref($key)
    }
    public method eLoad {file} {
        if { ($file != "") && [file exists $file]} {
            source $file
        }
    }
    # ---------------------------------------------------- Private Methods -----
    method prefs_save {} {
        set f $file
        if { $f == "" } { return 1 }
        set fp [open $f w]
        puts $fp "# Start Preferences"
        puts $fp "# "
        puts $fp "# saved at [clock format [clock seconds]]"
        puts $fp "# "
        puts $fp "version $version"
        foreach k [array names Pref] {
            var_dump $fp $k $Pref($k)
        }
        puts $fp "# -- end of preferences"
        close $fp
        return 0
    }
    method addpref {key pref {comment ""}} {
        set Pref($key)    $pref
        set Comment($key) $comment
    }
    method version {v} {
        if { $v != $version } {
            puts stderr "warning: preferences with different version $v"
        }
    }
    protected method var_dump {fp k pref} {
        if { [string match "*\n*" $pref] } {
            puts $fp [list addpref $k $pref]
            return
        }
        if { [expr [llength $pref] % 2] != 0 } {
            puts $fp [list addpref $k $pref]
            return
        }
        puts $fp ""
        puts $fp "addpref $k {"
        foreach {var val} $pref {
            puts $fp [list $var $val]
        }
        puts $fp "}"
    }
    # generate the full pathname for the prefs
    # the suffix will be added to the end of the file
    protected method path_make {fdir {suff ""}} {
        set sf "[lindex [split [info hostname] . ] 0].[pid]$suff"
        file mkdir $fdir
        set fpath [file join $fdir $sf]
        set fpath
    }

    # ---------------------------------------------------- Tests
    method test {} {
        set file /tmp/tclprefs
        catch {ReadPrefs $file}
        addpref foo bar "mr foo"
        addpref goo baz "mr bar"
        prefs_save
    }
    # ---------------------------------------------------- Common Initialization
}
