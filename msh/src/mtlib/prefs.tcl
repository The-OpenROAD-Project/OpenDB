package provide mtlib 1.0
package require Itcl

#
# Manage user preferences ...
#
# User preferences are stored in a file as tcl variables
#
# The class will automatically read the prefs on construction
# and save them on destruction
# Preferences can be forced to the disk with Save
#
itcl::class LPrefs {
	inherit LObject
	# ---------------------------------------------------- Public Variables ---
	public variable profile  ""  {profile_set $profile}   ;# profile basename
	public variable file    ""                            ;# where to save prefs
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
		prefs_save 
	}
	# ---------------------------------------------------- Public Methods ------
	method Save {} { prefs_save }
	method Get {key} {$this vMatch $key}
	method Set {key pref {comment ""}} {
		set Pref($key) $pref
		set Comment($key) $comment
	}
	method Exists {key} {
		info exists Pref($key)
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
	# ---------------------------------------------------- Virtual Methods -----
	public method vMatch {key} {
		if { $key == "*" } { return [array get Pref] }
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
		catch { exec mv -f $f ${f}~ }
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
		puts $fp [list addpref $k $pref]
	}

	# 
	protected method profile_set {p} {
		configure -file [format [LPlatform appprofile] $p]
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
