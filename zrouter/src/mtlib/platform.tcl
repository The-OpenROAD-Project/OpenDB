#
#
#
#
package provide mtlib 1.0

proc LPlatform {attribute} {
    global env
    set pl $::tcl_platform(platform)
    switch -glob -- $pl/$attribute {
        unix/user    { set env(USER) }
        windows/user { set env(USERNAME) }

        unix/home    { set env(HOME) }
        windows/home { 
            if [info exists env(USERPROFILE)] {
                return $env(USERPROFILE)
            } else {
                return C:\\
            }
        }

        unix/top - windows/top {
            file dirname [file dirname [info nameofexecutable]]
        }

        default {
            error " LPlatform: $attribute: unsupported platform $pl"
        }
    }
}
