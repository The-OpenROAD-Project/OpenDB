package provide mtlib 1.0

# --------------------------------------------------------------------------
# -- simple text file digitally signed license
package require md5

#       Class: LLicense
# Description: Simple License scheme
#      Author: Manolis M. Tsangaris  -- mtlib@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2005
#
# Read/Write a license file from an encrypted file
# Use mklicense program to make one
itcl::class LLicense {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable salt ""                ;# salt used for md5
    # -------------------------------------------------- Public Methods ----
    # -- check if the feature is present. Return its value
    # -- or ok, if everything checks out correctly
    public method Check {feature} {
        lic_check $feature
    }
    # -- read license from file
    public method Read {file rr} {
	upvar $rr R
        lic_read $file R
    }
    # -- write license to a file
    public method Write {file} {
        lic_write $file LIC
    }
    # -- validate and write a license to a file
    # -- add digital signature, use salt
    public method Make {file lic} {
        array set L $lic
        if ![info exists L(info)] {
            set dt [clock format [clock seconds] -format "%Y-%m-%d"]
            set L(info) "issued on [info hostname] at $dt"
        }
        lic_write $file L
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable LIC              ;# license file info
    protected variable VALID 0          ;# is loaded license valid?
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # -- try to read license from the license path
    protected method lic_read {path rr} {
	upvar $rr R
	set fp ""
	set errors ""
	foreach file [split $path :] {
	    if { $file == "" } { continue }
	    if { $file == "-" } {
		set fp "stdin"
		break
	    }
	    if [file exists $file] {
		if { [catch {open $file r} rc] } {
		    lappend errors "$file: $fp"
		    continue
		}
		set fp $rc
		break
	    }
	}
	if { $fp == "" } {
	    set R(msg) [list "no valid license in $path"]
	    return -2
	}
	set R(msg) [list "$file: reading license"]
	lic_read1 $file $fp R
    }
    protected method lic_read1 {file fp rr} {
	upvar $rr R
        catch {unset LIC}
        set rc [read $fp]
        close $fp
        set LIC(ck) ""
        if [catch {array set LIC $rc} ] {
	    lappend R(msg) "parsing $file: malformed license file"
        }
        set md $LIC(ck) ; unset LIC(ck)
        set ck [mksum LIC]
        if { $ck != $md } {
	    lappend R(msg) "$file: invalid license"
            set VALID 0
	    return -3
        } else {
            set VALID 1
        }
        return 0
    }
    protected method lic_write {file lic} {
        upvar $lic LIC
        if { $file == "-" } {
            set fp stdout
        } else {
            set fp [open [file normalize $file] w]
        }
        set LIC(ck) [mksum LIC]
        foreach a [lsort [array names LIC]] {
            puts $fp [list $a $LIC($a)]
        }
        close $fp
    }
    protected method mksum {a} {
        upvar $a A
        set out "$salt"
        foreach a [lsort [array names A]] {
            append out "$a $A($a)\n"
        }
        ::md5::md5 -hex $out
    }
    # ----------------------------------------------------------- CHECK
    protected method lic_check {feature} {
        if ![info exists LIC] { return "no license file" }
        if { !$VALID } { return "no valid license" }
        set r [lic_check_user LIC]
        if {$r != "" } { return $r}
        set r [lic_check_expires LIC]
        if {$r != "" } { return $r}
        set r [lic_check_feature  LIC $feature]
        if {$r != "" } { return $r}
        return "ok"
    }
    # -- check user
    protected method lic_check_user {lic} {
        upvar $lic LIC
        if ![info exists LIC(users)] { return "" }
        set me [exec whoami]
        foreach u $LIC(users) {
            if { $u == $me } { return "" }
        }
        return "$me: user not licensed"
    }
    protected method lic_check_expires {lic} {
        upvar $lic LIC
        if ![info exists LIC(expires)] { return "" }
        set dt [string trim $LIC(expires)]
        set now [clock seconds]
        set nl  [clock scan $dt]
        if { $now > $nl } { return "$dt: licensed expired" }
        return ""
    }
    protected method lic_check_feature {lic feature} {
        upvar $lic LIC
        if ![info exists LIC(features)] {
            return "$feature: no features available"
        }
        foreach f [split $LIC(features) \n] {
            set f [string trim $f]
            if { $f == "" } { continue }
            foreach {fname title} $f { break }
            if { $fname == $feature } { return "" }
            unset fname title
        }
        return "$feature: feature not available"
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}


# --------------------------------------------------------------------------
#       Class: LLicenseManager
# Description: Base line class
#      Author: Manolis M. Tsangaris  -- mtlib@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2006
# Simple License Manager
itcl::class LLicenseManager {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -- initialize the server. Return non 0 on error. Fill in R(msg)
    public method Init {serverlist rr} {
	upvar $rr R
	$this lm_init $serverlist R
    }
    public method Features {} {
	$this lm_features
    }
    public method FeatureInfo {feature} {
	$this lm_feature_info $feature
    }
    # -- checkout a license - return an identifier
    # -- >=0 if ok, -1 if errror
    public method Acquire {feature {version 1.0}} {
	$this lm_acquire $feature $version
    }
    # -- return all checked out licenses
    public method Licenses {} {
	$this lm_licenses
    }
    # -- return information about a particular (checked out) license
    public method LicenseInfo {licenseid} {
	$this lm_license_info $licenseid
    }
    public method Release {licenseid} {
	$this lm_release $licenseid
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -- initialize the server connection, if necessary
    protected method lm_init {serverlist rr} {
	return 0
    }
    # -- return all features
    protected method lm_features { } {
    }
    # -- return feature information
    protected method lm_feature_info {feature} {
    }
    # -- acquire a version of license features
    protected method lm_acquire {feature version} {
    }
    # -- release a license
    protected method lm_release {licenseid} {
    }
    # -- return all active licenses
    protected method lm_licenses {} {
    }
    # -- return information about a particular (checked out) license
    protected method lm_license_info {licenseid} {
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}

#       Class: LLicenseManagerFlexLM
# Description: FlexLM
#      Author: Manolis M. Tsangaris  -- mtlib@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2006
# Simple License Manager using FLEXLM
itcl::class LLicenseManagerFlexLM {
    inherit LLicenseManager
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable flexlm ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor {
	# -- cleanup
	if { $flexlm != "" } { 
	    delete_FlexLM $flexlm
	}
    }
    # -------------------------------------------------- Private Methods ---
    # -- setup the flexlm connection
    protected method lmf_init {srv rr} {
	upvar $rr R
	if { $flexlm == "" } { 
	    if [catch {package require tclflexlm 1.0} rc] {
		set R(msg) "no FlexLM support: $rc"
		return 2
	    }
	    set flexlm [new_FlexLM]
	}
	if [catch {$flexlm init 16 $srv} rc] {
	    set R(msg) "FlexLM error: $rc"
	    return 1
	}
	set R(server) $srv
	set R(msg) "connected to $srv ($rc)"
	set R(features) [Features]
	if { $R(features) == "" } {
	    set R(msg) "no features found"
	    return 1
	}
	return 0
    }
    # ----------------------------------------------- Interface Implementation
    # -- initialize the server connection, if necessary
    protected method lm_init {serverlist rr} {
	upvar $rr R
	if [string match *:* $serverlist] {
	    set serverlist [split $serverlist :]
	}
	set emsg ""
	foreach srv $serverlist {
	    set n [lmf_init $srv R]
	    if { $n == 0 } {
		return 0
	    }
	    append emsg "\n$srv: $R(msg)"
	}
	set R(msg) "failed to connect to any license server ($serverlist)"
	append R(msg) "$emsg"
	return 1
    }
    # -- return all features
    protected method lm_features { } {
	 $flexlm query
    }
    # -- return feature info (could be tab separated features)
    protected method lm_feature_info {features} {
	foreach feature [split $features :] {
	    set max ""
	    foreach {max inuse} [$flexlm getFeatureInfo $feature] { break }
	    if { $max != "" } { break }
	}
	if { $max == "" } { return "" }
	list feature $feature max $max inuse $inuse
    }
    # -- acquire a version of license features (could be tab separated features)
    protected method lm_acquire {features version} {
	set F(feature) $features
	array set F [lm_feature_info $features]
	$flexlm get $F(feature) $version
    }
    # -- release a feature
    protected method lm_release {licenseid} {
	$flexlm release $licenseid
    }
    # -- return all active licenses
    protected method lm_licenses {} {
	$flexlm getJobList
    }
    # -- return information about a particular (checked out) license
    protected method lm_license_info {licenseid} {
	foreach {max inuse} [$flexlm getJobInfo $licenseid] { break }
	list max $max inuse $inuse
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}


#
#       Class: LLicenseManagerTimeBomb
# Description: Time bomb license (based on a date)
#      Author: Manolis M. Tsangaris
#
itcl::class LLicenseManagerTimeBomb {
    inherit LLicenseManager
    # -------------------------------------------------- Public Variables --
    public variable expires ""
    public variable warningtime [expr 5*24*3600] ;# when to warn
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor {
    }
    # -------------------------------------------------- Private Methods ---
    protected method lmtb_check {rr} {
	upvar $rr R
	# -- any expiration?
	if { $expires == "" } { return 1 }
	set now [clock seconds]
	set exp [clock scan $expires]
	set dt  [expr $exp - $now]
	if { $dt <= 0 } {
	    set R(msg) "license expired (at $expires)"
	    return 1
	}
	if { ($warningtime) && ($dt <= $warningtime) } {
	    set R(msg) "warning:license will expire soon ($expires)"
	    return -1
	}
	return 0
    }
    # ----------------------------------------------- Interface Implementation
    protected method lm_init {_ rr} {
	upvar $rr R
	lmtb_check R
    }
    # -- fake feature info. Always accept!
    protected method lm_feature_info {feature} {
	set n [lmtb_check R]
	switch -- $n {
	    0 - -1 {
		return "inuse 0 max 10 feature $feature"
	    }
	}
	error "license expired: $R(msg)"
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}

