package require Itcl
package provide mtlib 1.0

#
# Object Name: LInteractor
# Description: Basic Interactor, to be used as an inherited class
#
itcl::class LInteractor {
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Protected Variables 
    protected variable sema
    protected variable status 0
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
