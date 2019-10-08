package provide ade::ui 2.0

#       Class: BookMarkMGR
# Description: Manage Bookmarks
#      Author: Manolis M. Tsangaris
#
itcl::class BookMarkMGR {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable cpanel ""
    # -------------------------------------------------- Public Methods ----
    public method Add {x0 y0 x1 y1 {descr ""}} {
	set id [array size BOOKMARK]
	set BOOKMARK($id) [list x0 $x0 y0 $y0 x1 $x1 y1 $y1 descr $descr]
	set id
    }
    public method Get {bid} {
	set BOOKMARK($bid)
    }
    public method Remove {bid} {
	unset BOOKMARK($bid)
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable BOOKMARK
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
