package require Itcl

# -- NOTICE THIS FILE IS PART OF THE FREELY AVAILABLE OPENSOURCE
# -- STD LIBRARY MTLIB
package provide mtlib 1.0


#
# Object Name: LInteractor
# Description: Basic Interactor, to be used as an inherited class
#      Author: Manolis M. Tsangaris -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
itcl::class LInteractor {
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Protected Variables 
    protected variable sema
    protected variable status 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    destructor { itcl::delete object $sema }
    # -------------------------------------------------- Public Methods ----
    public method Wait {} { $sema Wait ; return $status }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Private Methods ---
	constructor {args} {
		eval configure $args
    	set sema $this.sema
    	Semaphore $sema
    }
    public method Signal {st} {set status $st;  $sema Signal}
    # -------------------------------------------------- Common Initialization
}
