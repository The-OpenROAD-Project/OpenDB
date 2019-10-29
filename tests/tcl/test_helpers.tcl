if {[package vcompare [package present Tcl] 8.6] == -1} {
    proc lmap {args} {
        set result {}
        set var [lindex $args 0]
        foreach item [lindex $args 1] {
            uplevel 1 "set $var $item"
            lappend result [uplevel 1 [lindex $args end]]
        }
        return $result
    }
}

set ::failing_checks 0
set ::passing_checks 0

proc check {description test expected_value} {
    if {[catch {set return_value [uplevel 1 $test]} msg]} {
        incr ::failing_checks
        error "ERROR: $description: Command \{$test\}\n$msg"
    } elseif {$return_value != $expected_value} {
        incr ::failing_checks
        error "ERROR: $description: Expected $expected_value, got $return_value"
    } else {
        incr ::passing_checks
    }
}

proc exit_summary {} {
    set total_checks [expr $::passing_checks + $::failing_checks]
    if {$total_checks > 0} {
        puts "Summary $::passing_checks / $total_checks ([expr round(100.0 * $::passing_checks / $total_checks)]% pass)"
    } else { 
        puts "Summary 0 checks run"
    }
    exit $::failing_checks
}