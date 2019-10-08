
package provide mtlib 1.0

#       Class: LCrypter
# Description: Simple crypt using tcl::des
#
itcl::class LCrypter {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable mode ecb
    public variable key "\\0\\1\\2\\3\\4\\5\\6\\7" {key_set $key} ;# key to use
    # -------------------------------------------------- Public Methods ----
    public method Encrypt {data} {
	::DES::Encrypt $des_key $data
    }
    public method Decrypt {data} {
	::DES::Decrypt $des_key $data
    }
    # -- reset the state
    public method Reset {} {
	# -- reinitialize the key
	key_set $key
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable des_key
    protected variable iv
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	package require des 1.0
	set des_key ""
	set iv [string repeat \\0 8]
	key_set $key
    }
    destructor {
	if { $des_key !=  "" } { ::DES::Final $des_key }
    }
    # -------------------------------------------------- Private Methods ---
    protected method key_set {k} {
	if ![info exists des_key] { return }
	if { $des_key !=  "" } { ::DES::Final $des_key }
	set des_key [::DES::Init $mode $key $iv]
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
    # -------------------------------------------------- DEMO
    public method Demo {} {
	LCrypter cr
	set crypted ""
	foreach n "one two three" {
	    append crypted [cr Encrypt "line: $n\n"]
	}
	cr Reset
	puts [cr Decrypt $crypted]
	cr delete
    }
}
