package provide mtlib 1.0

#       Class: LSNotebook
# Description: Simple tab based notebook
#
itcl::class LSNotebook {
    inherit LFrame LBinder
    # Event <<raise>> tag
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Add {tag title} {
        $nb insert end $tag -text $title -raisecmd "$this eRaise $tag"
    }
    public method Raise {{tag ""}} {
        if { $tag == "" }  { set tag [$nb page 0] }
	UnHighlight $tag
        $nb raise $tag
    }
    # -- add a file viewer with scrollbar
    public method AddFile {tag title file} {
        set w [Add $tag $title]
        set wv [LScrollableWidget $w text -width 40 -height 20]
        $wv insert end [exec cat $file]
        # $wv configure -mode disabled
    }
    public method Configure {tag args} {
	eval $nb itemconfigure $tag $args
    }
    # -- highlight a tab 
    public method Highlight {tag} {
	if { $active == "$tag" } { return }
	set txt [$nb itemcget $tag -text]
	regsub {^[\* ]} $txt "" txt
	$nb itemconfigure $tag -text *$txt
    }
    public method UnHighlight {tag} {
	set txt [$nb itemcget $tag -text]
	regsub {^[\* ]} $txt "" txt
	$nb itemconfigure $tag -text " $txt"
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable nb
    protected variable active ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        package require BWidget 1.4
        set nb [NoteBook $This.notebook -ibd 0]
        pack $nb -side top -expand 1 -fill both
        after idle "$this Raise"
    }
    destructor {
        destroy $nb
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eRaise {tag} {
	set active $tag
	UnHighlight $tag
        Event <<raise>> $tag
    }
    # -------------------------------------------------- Common Initialization
}
