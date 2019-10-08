## file.tcl - file related tcl routines
## Author: Mattias Hembruch
## (c) 2005 Athena Design Systems

package provide mtlib 1.0

proc common_path { file1 file2 } {
    set fn1 [file normalize $file1]
    set fn2 [file normalize $file2]

    set fl1 [file split $fn1]
    set fl2 [file split $fn2]

    set cp ""
    foreach arg1 $fl1 arg2 $fl2 {
        if { $arg1 != $arg2 } {
            break
        }
        set cp [file join $cp $arg1]
    }
    return $cp
}

## how to get from file1 to file2, when file1 is lower in the tree
proc path_up { file1 file2 } {

    set fn1 [file normalize $file1]
    set fn2 [file normalize $file2]

    if { ![file isdirectory $fn1] } {
        set fp1 [file dirname $fn1] 
    } else {
        set fp1 $fn1
    }

    if { ![file isdirectory $fn2] } {
        set fp2 [file dirname $fn2] 
    } else {
        set fp2 $fn2
    }

    set fpl1 [file split $fp1]
    set fpl2 [file split $fp2]

    set idx [llength $fpl2]
    set rpl [lrange $fpl1 $idx end]

    set rpath ""
    foreach arg $rpl {
        append rpath "../"
    }

    return $rpath
}

## how to get from file1 to file2, when file1 is higher in the tree
proc path_down { file1 file2 } {

    set fn1 [file normalize $file1]
    set fn2 [file normalize $file2]

    if { ![file isdirectory $fn1] } {
        set fp1 [file dirname $fn1] 
    } else {
        set fp1 $fn1
    }

    if { ![file isdirectory $fn2] } {
        set fp2 [file dirname $fn2] 
    } else {
        set fp2 $fn2
    }

    set fpl1 [file split $fp1]
    set fpl2 [file split $fp2]

    set idx [llength $fpl1]
    set rpl [lrange $fpl2 $idx end]

    set rpath ""
    foreach arg $rpl {
        append rpath "$arg/"
    }

    return $rpath
}

## link file1 to file 2 using minimal path
proc path_link { file1 file2 } {

    set cp [common_path $file1 $file2]

    ## if only common item is "/"
    if { ($cp == "/") || [string index $file1 0] == "/" } {
        set file1 [file normalize $file1]
        set file2 [file normalize $file2]
        set rc [ catch {exec ln -sf $file2 $file1} out]
        return [list $rc $out]
    }

    ## save current directory
    set cwd [pwd]

    set f1f [file tail $file1]

    set f2f ""
    if { ![file isdirectory $file2] } {
        set f2f [file tail $file2]
    }


    if { ![file isdirectory $file1] } {
        set fp1 [file dirname $file1] 
    } else {
        set fp1 $file1
    }


    ## find minimal path/location
    set pu [path_up $file1 $cp]
    set pd [path_down $cp $file2]

    set link [file join $pu $pd $f2f]

    ## go to "location"
    cd $fp1
    ## create link
    set rc [ catch {exec ln -sf $link $f1f} out]

    ## go back to starting directory
    cd $cwd

    return [list $rc $out]

}
    
