#==============================================================================
# FILE: mdsc.tcl
#==============================================================================
# This file provides the Tcl code for the "Mdsc" package.
#==============================================================================
package provide Mdsc 1.0
package require Thread

#------------------------------------------------------------------------------
# NAMESPACE: mdsc
#------------------------------------------------------------------------------
#if {[namespace exists mdsc]} {
#    namespace delete mdsc
#}

namespace eval mdsc {

    # _NextId : used to generate unique ids for the copy operations
    #
    variable _NextId
    catch {unset _NextId}
    set _NextId 0

    # _CopyInfo : an array where we store information about the
    #		copy operations in progress.
    #		Indexed by "copy id". Value is an attribute/value
    #		list.
    #
    variable _CopyInfo
    catch {unset _CopyInfo}
    array set _CopyInfo {}

    set KLUDGE 0	;# Channel transferring between threads kludge
}

#------------------------------------------------------------------------------
# mdsc::copy
#------------------------------------------------------------------------------
# This is the main entry point for the "copy" functionality between
# memory objects and channels
#
# There are 3 subcommands:
#
# mdsc::copy from $mobj $channel ?options?
#	Async copy from a channel into a memory object.
#	Return a handle that identifies this copy operation.
#	Valid options are:
#		-start <num> : skip that many bytes from the channel
#			before starting copying to the file
#		-nbytes <num>: Write that many bytes (max) to the object.
#			(or less if EOF is encountered first)
#		-buffersize <num>
#		-chunksize <num>
#		-callback <script>
#			The callbac to call when we are done
#		-statusevery <num>
#			Call the callback every time that many
#			bytes are copied
#		-autoforget <boolean>
#
# mdsc::copy to $mobj $channel ?options?
#
#
# mdsc::copy info ?$handler?
#	Return information about this handler (or if no args return
#	a list of all handlers)
#
# mdsc::copy forget $handler
#
#	
#------------------------------------------------------------------------------
proc mdsc::copy {cmd args} {
    _dodebug 1 ">mdsc::copy $cmd $args"
    if {$cmd == "from" || $cmd == "to"} {
	set res [_copy_fromto $cmd $args]
    } elseif {$cmd == "info"} {
	set res [_copy_info $args]
    } elseif {$cmd == "forget"} {
	set res [_copy_forget $args]
    } else {
	set msg "Invalid subcommand \"$cmd\". Must be one of"
	append msg " from, to, info or forget."
	_dodebug 1 "<mdsc::copy $cmd $args error=$msg"
	error $msg
    }
    _dodebug 1 "<mdsc::copy $cmd $args return=$res"
    return $res
}

#------------------------------------------------------------------------------
# mdsc::_copy_fromto
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
proc mdsc::_copy_fromto {direction arglist} {

    _dodebug 1 ">mdsc::_copy_fromto $direction $arglist"
    # NOTE: "$direction" is either "from" or "to".
    # Parse the remaining arguments.
    # The first two are the memory object handle and the channel
    # The remaining the options
    #
    set nargs [llength $arglist]
    if {$nargs < 2} {
	set msg ""
	append msg "wrong # args: should be \"mdsc::copy" \
			" $direction memobjhandle channel ?options?\""
	_dodebug 1 "<mdsc::_copy_fromto $direction $arglist error=$msg"
	error $msg
    }
    set mobj [lindex $arglist 0]
    set channel [lindex $arglist 1]
    if {![mdsc::memobj exists $mobj]} {
	set msg "Invalid memory object handle \"$mobj\"."
	_dodebug 1 "<mdsc::_copy_fromto $direction $arglist error=$msg"
	error $msg
    }

    set inf(copyid)		"XXX"	;# TBA later on
    set inf(tid)		""	;# TBA later on
    set inf(parent)		[thread::id]
    set inf(direction)		$direction
    set inf(channel)		$channel
    set inf(mobj)		$mobj
    set inf(callback)		""
    set inf(channelconf)	""
    set inf(chunksize)		0
    set inf(buffersize)		0
    set inf(start)		0
    set inf(nbytes)		-1
    set inf(statusevery)	0
    set inf(status)		"run"
    set inf(error)		""
    set inf(errorinfo)		""
    set inf(nread)		0
    set inf(ncopied)		0
    set inf(autoforget)		1
    # Absolute time (second granularity)
    set inf(atstart)		[clock seconds]
    set inf(atlast)		$inf(atstart)
    # The following are used for relative time measurements (millisecond
    # granularity. The number is in seconds but has 3 decimal digits
    set inf(tstart)		[expr {[clock clicks -milliseconds] / 1000.0}]
    set inf(tlast)		$inf(tstart)


    # Parse options
    for {set i 2} {$i < $nargs} {incr i} {
	set o [lindex $arglist $i]
	# All the options below this point require an argument
	#
	incr i
	if {$i >= $nargs} {
	    set msg "value for \"$o\" missing"
	    _dodebug 1 "<mdsc::_copy_fromto $direction $arglist error=$msg"
	    error $msg
	}
	set val [lindex $arglist $i]
	if {$o == "-callback"} {
	    set inf(callback) $val
	} elseif {$o == "-chunksize"} {
	    set val [_parse_numbytes $val 0]
	    set inf(chunksize) $val
	} elseif {$o == "-buffersize"} {
	    set val [_parse_numbytes $val 0]
	    set inf(buffersize) $val
	} elseif {$o == "-start"} {
	    set val [_parse_numbytes $val 0]
	    set inf(start) $val
	} elseif {$o == "-nbytes"} {
	    set val [_parse_numbytes $val -1]
	    set inf(nbytes) $val
	} elseif {$o == "-statusevery"} {
	    set val [_parse_numbytes $val 0]
	    set inf(statusevery) $val
	} elseif {$o == "-autoforget"} {
	    set val [_parse_boolean $val]
	    set inf(autoforget) $val
	} else {
	    set msg "Invalid option \"$o\"."
	    append msg " Valid options are: -callback," \
	    	" -chunksize, -buffersize, -start, -nbytes," \
		" -statusevery, -autoforget."
	    _dodebug 1 "<mdsc::_copy_fromto $direction $arglist error=$msg"
	    error $msg
	}
    }
    # Give some default values
    if {$inf(chunksize) <= 0} {
	set inf(chunksize) [expr {1024*1024}]
    }
    if {$inf(buffersize) <= 0} {
	set inf(buffersize) [expr {1024*1024}]
    }

    # If we arrive here, we are ready to go!
    # Generate a new handle for this copy operation
    #
    variable _NextId
    variable _CopyInfo
    incr _NextId
    set copyid [format "copy%d" $_NextId]
    set inf(copyid) $copyid

    set _CopyInfo($copyid) [array get inf]

    # NOTE: Due to the internal Tcl core implementation and the
    # restriction on transferring shared channels, we will create
    # the thread and trasnfer the channel in the idle loop
    # (see man page for "thread::transfer"
    # 
    after idle [list mdsc::_copy_fromto_idle $copyid]

    # Return the copyid
    _dodebug 1 "<mdsc::_copy_fromto $direction $arglist return=$copyid"
    return $copyid
}

#------------------------------------------------------------------------------
# mdsc::_copy_fromto_idle
#------------------------------------------------------------------------------
# This is the last part of the "_copy_fromto" method.
# Normally this code should have been there, but there are
# some restrictions on transferring shared channels in the current
# Tcl implementation, so we have to do it in the idle loop.
#------------------------------------------------------------------------------
proc mdsc::_copy_fromto_idle {copyid} {
    variable _CopyInfo
    global auto_path
    if {![info exists _CopyInfo($copyid)]} {
	# Probably we were already forgotten!!!!!
	return
    }

    array set inf $_CopyInfo($copyid)
    set inf(tid) [thread::create]
    thread::send $inf(tid) [list set auto_path $auto_path]
    thread::send $inf(tid) {package require Mdsc}
    thread::transfer $inf(tid) $inf(channel)
    thread::send -async $inf(tid) [list mdsc::_thread_copy [array get inf]]
}

#------------------------------------------------------------------------------
# mdsc::_copy_info
#------------------------------------------------------------------------------
#
#------------------------------------------------------------------------------
proc mdsc::_copy_info {arglist} {
    variable _CopyInfo

    # Easy case first.
    if {$arglist == ""} {
	return [array names _CopyInfo]
    }
    set nargs [llength $arglist]
    if {$nargs == 1} {
	set copyid [lindex $arglist 0]
	if {![info exists _CopyInfo($copyid)]} {
	    error "Invalid copy handle \"$copyid\"."
	}
	return $_CopyInfo($copyid)
    } else {
	set msg "wrong # args: should be \"mdsc::copy info ?copyhandle?\""
	error $msg
    }
}

#------------------------------------------------------------------------------
# mdsc::_copy_forget
#------------------------------------------------------------------------------
#
#------------------------------------------------------------------------------
proc mdsc::_copy_forget {arglist} {
    variable _CopyInfo

    set nargs [llength $arglist]
    if {$nargs != 1} {
	error "wrong # args: should be \"mdsc::copy forget copyhandle\""
    }
    set copyid [lindex $arglist 0]
    if {![info exists _CopyInfo($copyid)]} {
	error "Invalid copy handle \"$copyid\"."
    }


    # XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    # WHAT SHALL WE DO HERE????
    # SHOULD WE NOTIFY THE THREAD AND INTERRUPT THE COPY?
    # SHOULD WE REFUSE TO FORGET A RUNNING THREAD
    # UNLESS THE "-force" OPTION IS GIVEN ????
    # XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    #array set inf $_CopyInfo($copyid)

    unset _CopyInfo($copyid)
}

#------------------------------------------------------------------------------
# mdsc::_parse_numbytes
#------------------------------------------------------------------------------
# Parse a "number of bytes" specification.
# This is an integer optionally followed by the letters "k" or "K" (kilobytes)
# "m" or "M" (megabytes) and "b" or "B" (bytes - the default)
#
# ARGUMENTS:
#	str : the string
#	minval : a minimum value for the number.
#		 If equal to "" then there is no minimum lmit
#	maxval : a max val for the number.
#		 If equal to "" then there is no maximum lmit
#
# RETURNS:
#	The number of bytes (an integer)
# ERRORS:
#	If the string is invalid or out of bounds, an appropriate
#	error is generated.
#------------------------------------------------------------------------------
proc mdsc::_parse_numbytes {str {minval ""} {maxval ""}} {

    set last_char [string index $str end]
    set last_char [string tolower $last_char]	;# ignore case
    if {$last_char == "m"} {
	set multiplier [expr {1024*1024}]
	set num [string range $str 0 end-1]
    } elseif {$last_char == "k"} {
	set multiplier 1024
	set num [string range $str 0 end-1]
    } elseif {$last_char == "b"} {
	set multiplier 1
	set num [string range $str 0 end-1]
    } else {
	set multiplier 1
	set num $str
    }

    if {[string is integer -strict $num]} {
	set val [expr {$num * $multiplier}]
	if {($minval == "" || $val >= $minval) \
		&& ($maxval == "" || $val <= $maxval)} {
	    # We are done!
	    return $val
	}
    }

    # If we arrive here the string was invalid or out of bounds.
    # Construct an appropriate error message and throw an error
    #
    set msg "Invalid value \"$str\". Must be an integer optionally"
    append msg " followed by the letters \"m\" (megabytes), \"k\"" \
    		" (kilobytes) or \"b\" (bytes - the default)"

    if {$minval == "" && $maxval == ""} {
	append msg "."
    } elseif {$minval != "" && $maxval == ""} {
	append msg " greater or equal to $minval."
    } elseif {$minval == "" && $maxval != ""} {
	append msg " less or equal to $maxval."
    } else {
	# {$minval != "" && $maxval != ""}
	append msg " greater or equal to $minval"
	append msg " and less or equal to $maxval."
    }
    error $msg
}

#------------------------------------------------------------------------------
# mdsc::_parse_boolean
#------------------------------------------------------------------------------
# Parse a string representing a boolen value
# ARGUMENTS:
#	str : the string
# RETURNS:
#	0 or 1 depending on whether the value was false or true
# ERRORS:
#	Generates an error if the string is invalid
#------------------------------------------------------------------------------
proc mdsc::_parse_boolean {str} {
    if {[string is integer -strict $str]} {
	if {$str} {
	    return 1
	} else {
	    return 0
	}
    }
    switch -exact -- $str {
	"true"	-
	"t"	-
	"on"	-
	"y"	-
	"yes"	{return 1}
	"false"	-
	"f"	-
	"off"	-
	"n"	-
	"no"	{return 0}
    }

    set msg "Invalid boolean value \"$str\"."
    error $msg
}

#------------------------------------------------------------------------------
# mdsc::_sendproc
#------------------------------------------------------------------------------
# This is a small utility function that "sends" a proc
# to a thread, i.e. using the "thread::send" command it sends
# the definition of a proc that the current thread knows about
#
# ARGUMENTS:
#       tid     : the thread id where we will "send" the procedure
#       procname: the name of the procedure to send.
#	newprocname: An optional new name that the new proc will be known
#		us. Defaults to "$procname"
#------------------------------------------------------------------------------
proc mdsc::_sendproc {tid procname {newprocname ""}} {

    if {$newprocname == ""} {
	set newprocname $procname
    }
    set procscript [list "proc" $newprocname \
	    [info args $procname] \
	    [info body $procname] ]
    thread::send $tid $procscript
}

#------------------------------------------------------------------------------
# mdsc::_callback
#------------------------------------------------------------------------------
# This is the callback that the various threads call in order to
# notify us about completion of file copy operations (either successful
# or with an error) or with intermediate status information.
#------------------------------------------------------------------------------
proc mdsc::_callback {arglist} {
    _dodebug 1 ">mdsc::_callback $arglist"
    variable _CopyInfo

    #puts "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
    #puts "XXX CALLBACK: $arglist"
    #puts "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

    array set inf $arglist
    set copyid $inf(copyid)
    if {![info exists _CopyInfo($copyid)]} {
	# Probably we were "forgotten"!
	_dodebug 1 "<mdsc::_callback (NO SUCH COPY ID) $arglist"
	return
    }
    set _CopyInfo($copyid) $arglist

    # Extract the information we need to pass back to the user's
    # callback
    set cback $inf(callback)
    if {$cback != ""} {
	if {[catch {eval $cback [list $arglist]} res]} {
	    global errorInfo
	    set msg "Callback error:\n$errorInfo"
	    _dodebug 1 "=mdsc::_callback error=$res"
	    puts stderr $msg
	} else {
	    _dodebug 1 "=mdsc::_callback ok res=$res"
	}
    }

    # If the job has finished, and the "autoforget" flag is true,
    # forget the job.
    #
    set status $inf(status)
    if {$status == "ok" || $status == "error"} {
	if {$inf(autoforget)} {
	    _dodebug 1 "=mdsc::_callback autoforgetting $copyid"
	    unset _CopyInfo($copyid)
	}
    }
    _dodebug 1 "<mdsc::_callback $arglist"
}

#------------------------------------------------------------------------------
# mdsc::_thread_copy
#------------------------------------------------------------------------------
# This is the main function that executes in each thread
#
# ARGUMENTS:
#	arglist : the attribute/value list that contains all the
#		information we need about our copy.
#------------------------------------------------------------------------------
proc mdsc::_thread_copy {arglist} {
    
    _dodebug 1 ">mdsc::_thread_copy $arglist"

    # Extract al the information into an array
    # NOTE: no checks made! Make sure that the information
    # is complete and correct!!!!
    #
    array set inf $arglist

    if {$inf(chunksize) <= 0} {
	set inf(chunksize) [expr {1024*1024}]
    }
    if {$inf(buffersize) <= 0} {
	set inf(buffersize) [expr {1024*1024}]
    }

    # Depending on the "direction" of the copy, call the
    # appropriate function. Catch any errors
    #
    if {$inf(direction) == "from"} {
	set status [catch {_thread_copyfrom inf} res]
    } else {
	set status [catch {_thread_copyto inf} res]
    }

    ################################################
    ### STRANGE RACE CODNDITION?????
    ### sjp 04/04/2005
    ### It seems that the "transfer" of channel back to the
    ### main thread does not always work for some mysterious
    ### reason. So.. (I am just guessing here...) try to do the
    ### whole thing in the idle loop
    #################################################

variable KLUDGE
if {!$KLUDGE} {
#-------- ORIGINAL CODE
    # Transfer back the channel to the parent thread
    # *before* calling the callback
    #
    catch { thread::transfer $inf(parent) $inf(channel) }
    if {$status} {
	# Ooops! There was an error
	global errorInfo
	set inf(errorinfo) $errorInfo
	set inf(error) $res
	set inf(status) "error"
	_thread_cback "error" inf

	# NOTE: try to restore the channel to its original configuration
	# Just in case this was not done due to the error
	# Ignore any errors
	catch {_thread_channel_restore inf}
    } else {
	set inf(status) "ok"
	_thread_cback "ok" inf
    }

    # Commit suicide...
    #
    thread::release
} else {
#-------- KLUDGED CODE
    after idle [list mdsc::_thread_KLUDGE1 $status [array get inf]]
}
    _dodebug 1 "<mdsc::_thread_copy $arglist"
}

#------------------------------------------------------------------------------
# _thread_KLUDGE1
# _thread_KLUDGE2
#------------------------------------------------------------------------------
# These are the kludges (soon to become official code!)
# that have to do with the mysterious problems with the channel
# transferring
#------------------------------------------------------------------------------
proc mdsc::_thread_KLUDGE1 {status inf_array_vals} {
    array set inf $inf_array_vals

    # Transfer back the channel to the parent thread
    # *before* calling the callback
    #
    catch { thread::transfer $inf(parent) $inf(channel) }

    after idle [list mdsc::_thread_KLUDGE2 $status [array get inf]]
}
proc mdsc::_thread_KLUDGE2 {status inf_array_vals} {
    array set inf $inf_array_vals
    if {$status} {
	# Ooops! There was an error
	global errorInfo
	set inf(errorinfo) $errorInfo
	set inf(error) $res
	set inf(status) "error"
	_thread_cback "error" inf

	# NOTE: try to restore the channel to its original configuration
	# Just in case this was not done due to the error
	# Ignore any errors
	catch {_thread_channel_restore inf}
    } else {
	set inf(status) "ok"
	_thread_cback "ok" inf
    }
    after idle [list mdsc::_thread_KLUDGE3 $status [array get inf]]
}
proc mdsc::_thread_KLUDGE3 {status inf_array_vals} {
    thread::release
}



#------------------------------------------------------------------------------
# mdsc::_thread_channel_reconfig
#------------------------------------------------------------------------------
# Configure the given channel so that we can transmit the data to
# be copied. Remember though the original channel configuration
# so that we can restore it later on with a call to "_thread_channel_restore"
#------------------------------------------------------------------------------
proc mdsc::_thread_channel_reconfig {infovar} {
    upvar $infovar inf

    set fp $inf(channel)
    set inf(channelconf) [fconfigure $fp]
    fconfigure $fp -blocking 1 -encoding binary -eofchar {} \
	    -translation binary -buffersize $inf(buffersize)
      
}

#------------------------------------------------------------------------------
# mdsc::_thread_channel_restore
#------------------------------------------------------------------------------
# Restore the original channel configuration as it was saved from
# a previous call to "_thread_channel_reconfig"
#------------------------------------------------------------------------------

proc mdsc::_thread_channel_restore {infovar} {
    upvar $infovar inf

    if {![info exists inf(channelconf)] || $inf(channelconf) == ""} {
	return
    }
    array set conf $inf(channelconf)
    set attrs  {
	  -blocking -buffering -buffersize -encoding -eofchar -translation
    }
    foreach a $attrs {
	if {![info exists conf($a)]} {
	    continue
	}
	fconfigure $inf(channel) $a $conf($a)
    }

} 

#------------------------------------------------------------------------------
# mdsc::_thread_copyfrom
#------------------------------------------------------------------------------
# Copy from a channel to a memory object
#------------------------------------------------------------------------------

proc mdsc::_thread_copyfrom {infovar} {
    upvar $infovar inf
    mdsc::_thread_copyfrom_2 inf
}

# OLD VERSION
proc mdsc::_thread_copyfrom_1 {infovar} {

    upvar $infovar inf

    # Get misc information from "inf" array
    #
    set mobj $inf(mobj)
    set fp $inf(channel)

    set inf(status) "run"

    # Ok, first try to re-configure the channel.
    #
    _thread_channel_reconfig inf

    # Start reading the file. The following arguments are given to us:
    # 	nskip : how many initial bytes to skip from the channel
    #	nbytes: max number of bytes to copy (a negative number means
    #		that we copy all bytes up to an EOF)
    # We then calculate the following:
    #	nmaxread: the max number of bytes to read.
    #		If "$nbytes >= 0" then "nmaxread = $nskip + $nbytes"
    #		Else, "nmaxread" is -1 to indicate that we read up to EOF
    #
    set nskip $inf(start)
    set nbytes $inf(nbytes)
    if {$nbytes < 0} {
	set nmaxread -1
    } else {
	set nmaxread [expr {$nskip + $nbytes}]
    }

    # We also use the following:
    #	nread   : number of bytes read so far
    #	ncopied : number of bytes copied so far to the object.
    #		  (this is between 0 and "$nread - $nskip")
    set inf(nread) 0
    set inf(ncopied) 0
    set status_indx ""
    while {1} {
	# Exit the loop if we have reached EOF, or if we have
	# copied more than the specified number of bytes.
	#
	if {[eof $fp]} { break }
	if {$nbytes >= 0 && $inf(ncopied) >= $nbytes} { break }

	# Read a whole chunk of bytes, but on the other hande
	# do not read more bytes than we are supposed to consume
	#
	set n1 $inf(chunksize)
	if {$nmaxread >= 0 && [expr {$inf(nread) + $n1}] > $nmaxread} {
	    set n1 [expr {$nmaxread - $inf(nread)}]
	}

	# Do the actual read. Find out how many bytes we have read
	# (this could be less than $n1 if we found EOF)
	#
	set data [read $fp $n1]
	#set n2 [string bytelength $data]
	set n2 [string length $data]
	incr inf(nread) $n2

	# Now copy the data bytes to the memory object
	# (but only if we are not in the "skipping" phase)
	#
	set n_to_copy [expr {$inf(nread) - $nskip - $inf(ncopied)}]
	if {$n_to_copy > 0} {
	    mdsc::memobj append $inf(mobj) $data [expr {$n2 - $n_to_copy}]
	    incr inf(ncopied) $n_to_copy
	}
	set inf(atlast) [clock seconds]
	set inf(tlast) [expr {[clock clicks -milliseconds] / 1000.0}]

	# Should we call the "status" callback
	#
	if {$inf(statusevery) > 0} {
	    set new_status_indx [expr {$inf(ncopied) / $inf(statusevery)}]
	    if {$new_status_indx != $status_indx} {
		set status_indx $new_status_indx
		_thread_cback "status" inf
	    }
	}
    }

    # We are done! Reset channel configuration
    _thread_channel_restore inf
}

# NEW VERSION (uses "mdsc::memobj copyfromfile")
proc mdsc::_thread_copyfrom_2 {infovar} {

    upvar $infovar inf

    # Get misc information from "inf" array
    #
    set mobj $inf(mobj)
    set fp $inf(channel)

    set inf(status) "run"

    # Ok, first try to re-configure the channel.
    #
    _thread_channel_reconfig inf

    # Start reading the file. The following arguments are given to us:
    # 	nskip : how many initial bytes to skip from the channel
    #	nbytes_to_copy: max number of bytes to copy (a negative number means
    #		that we copy all bytes up to an EOF)
    #
    set nskip $inf(start)
    set nbytes_to_copy $inf(nbytes)

    # We also use the following:
    #	inf(nread)   : number of bytes read so far
    #	inf(ncopied) : number of bytes copied so far to the object.
    #		  (this is between 0 and "$nread - $nskip")
    set inf(nread) 0
    set inf(ncopied) 0
    set status_indx ""

    # If we have to "skip" some bytes, then do it now
    #
    while {$inf(nread) < $nskip} {
	# Check for EOF, just in case...
	if {[eof $fp]} { break }

	# Read a whole chunk of bytes, but on the other hande
	# do not read more bytes than we are supposed to skip
	#
	set n1 [expr {$nskip - $inf(nread)}]
	if {$n1 > $inf(chunksize)} {
	    set n1 $inf(chunksize)
	}
	set data [read $fp $n1]
	#set n2 [string bytelength $data]
	set n2 [string length $data]
	incr inf(nread) $n2
    }

    # Ok, now all the bytes to be read from the channel should go to
    # the object...
    while {1} {
	# Exit the loop if we have reached EOF, or if we have
	# copied more than the specified number of bytes.
	#
	if {[eof $fp]} { break }
	if {$nbytes_to_copy >= 0 && $inf(ncopied) >= $nbytes_to_copy} { break }

	# Read a whole chunk of bytes, but on the other hande
	# do not read more bytes than we are supposed to consume
	#
	if {$nbytes_to_copy >= 0} {
	    set n1 [expr {$nbytes_to_copy - $inf(ncopied)}]
	    if {$n1 > $inf(chunksize)} {
		set n1 $inf(chunksize)
	    }
	} else {
	    set n1 $inf(chunksize)
	}

	# Do the copy from channel to object
	# NOTE: "$n2" is the actual number of bytes copied.
	#
	set n2 [mdsc::memobj copyfromfile $inf(mobj) $fp $n1]

	# Update number of bytes read/copied + time of last copy
	incr inf(nread) $n2
	incr inf(ncopied) $n2
	set inf(atlast) [clock seconds]
	set inf(tlast) [expr {[clock clicks -milliseconds] / 1000.0}]

	# Should we call the "status" callback
	#
	if {$inf(statusevery) > 0} {
	    set new_status_indx [expr {$inf(ncopied) / $inf(statusevery)}]
	    if {$new_status_indx != $status_indx} {
		set status_indx $new_status_indx
		_thread_cback "status" inf
	    }
	}
    }

    # We are done! Reset channel configuration
    _thread_channel_restore inf
}

#------------------------------------------------------------------------------
# mdsc::_thread_copyto
#------------------------------------------------------------------------------
# Copy from a memory object to a channel
#------------------------------------------------------------------------------
proc mdsc::_thread_copyto {infovar} {
    upvar $infovar inf
    mdsc::_thread_copyto_2 inf
}

# OLD VERSION
proc mdsc::_thread_copyto_1 {infovar} {

    upvar $infovar inf
    _dodebug 2 ">mdsc::_thread_copyto infovar=$infovar inf=[array get inf]"


    # Get misc information from "inf" array
    #
    set mobj $inf(mobj)
    set fp $inf(channel)

    set inf(status) "run"

    # Ok, first try to re-configure the channel.
    #
    _thread_channel_reconfig inf


    # Start reading bytes from the object and copying them to the
    # channel
    #
    if {$inf(nbytes) < 0} {
	set maxoffset [mdsc::memobj size $inf(mobj)]
    } else {
	set maxoffset [expr {$inf(start) + $inf(nbytes)}]
    }
    set inf(nread) 0
    set inf(ncopied) 0
    set offset $inf(start)
    set status_indx ""
    while {$offset < $maxoffset} {
	# Read the next chunk of data from the object
	#
	set n $inf(chunksize)
	if {[expr {$n + $offset}] > $maxoffset} {
	    set n [expr {$maxoffset - $offset}]
	}
	set data [mdsc::memobj get $inf(mobj) $offset $n]
	#set n2 [string bytelength $data]
	set n2 [string length $data]
	incr offset $n2
	incr inf(nread) $n2

	# Copy to channel
	puts -nonewline $inf(channel) $data
	flush $inf(channel)
	incr inf(ncopied) $n2
	set inf(atlast) [clock seconds]
	set inf(tlast) [expr {[clock clicks -milliseconds] / 1000.0}]

	# Should we call the "status" callback
	#
	if {$inf(statusevery) > 0} {
	    set new_status_indx [expr {$inf(ncopied) / $inf(statusevery)}]
	    if {$new_status_indx != $status_indx} {
		set status_indx $new_status_indx
		_thread_cback "status" inf
	    }
	}
    }

    # We are done! Reset channel configuration
    _thread_channel_restore inf
}

# NEW VERSION (uses "mdsc::memobj copytofile")
proc mdsc::_thread_copyto_2 {infovar} {

    upvar $infovar inf
    _dodebug 2 ">mdsc::_thread_copyto infovar=$infovar inf=[array get inf]"


    # Get misc information from "inf" array
    #
    set mobj $inf(mobj)
    set fp $inf(channel)

    set inf(status) "run"

    # Ok, first try to re-configure the channel.
    #
    _thread_channel_reconfig inf


    # Start reading bytes from the object and copying them to the
    # channel
    #
    if {$inf(nbytes) < 0} {
	set maxoffset [mdsc::memobj size $inf(mobj)]
    } else {
	set maxoffset [expr {$inf(start) + $inf(nbytes)}]
    }
    set inf(nread) 0
    set inf(ncopied) 0
    set offset $inf(start)
    set status_indx ""
    while {$offset < $maxoffset} {
	# Copy the next chunk of data from the object
	# to the channel
	#
	set n $inf(chunksize)
	if {[expr {$n + $offset}] > $maxoffset} {
	    set n [expr {$maxoffset - $offset}]
	}
	set n2 [mdsc::memobj copytofile $inf(mobj) $inf(channel) $offset $n]
	_dodebug 2 \
	"=mdsc::_thread_copyto_2 copytofile $inf(channel) $offset $n res=$n2"
	flush $inf(channel)

	# Increment the offset and number of bytes read/copied
	# Update the time of the last copy
	#
	incr offset $n2
	incr inf(nread) $n2
	incr inf(ncopied) $n2
	set inf(atlast) [clock seconds]
	set inf(tlast) [expr {[clock clicks -milliseconds] / 1000.0}]

	# Should we call the "status" callback
	#
	if {$inf(statusevery) > 0} {
	    set new_status_indx [expr {$inf(ncopied) / $inf(statusevery)}]
	    if {$new_status_indx != $status_indx} {
		set status_indx $new_status_indx
		_thread_cback "status" inf
	    }
	}
    }

    # We are done! Reset channel configuration
    _thread_channel_restore inf
}

#------------------------------------------------------------------------------
# mdsc::_thread_cback
#------------------------------------------------------------------------------
# Call a callback to the main thread
#------------------------------------------------------------------------------
proc mdsc::_thread_cback {what infovar} {
    upvar $infovar inf
    _dodebug 1 \
    	">mdsc::_thread_cback: what=$what infovar=$infovar inf=[array get inf]"
    set parent $inf(parent)
    set script [list mdsc::_callback [array get inf]]
    thread::send -async $parent $script
    _dodebug 1 \
    	"<mdsc::_thread_cback: what=$what infovar=$infovar"
}

#------------------------------------------------------------------------------
# mdsc::_dodebug
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
proc mdsc::_dodebug {level args} {
    set msg [format "DEBUG: %s: %s" [thread::id] [concat $args]]
    mdsc::debug $level $msg
}
