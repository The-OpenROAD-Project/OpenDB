package provide ade::ui 2.0

#       Class: ZHelp
# Description: ZUI HTML Help facility
#      Author: Manolis M. Tsangaris
#

itcl::class ZHelp {
    inherit LTopLevelEditor
    # -------------------------------------------------- Public Variables --
    public variable docroot "."
    # -------------------------------------------------- Public Methods ----
    public method Show {args} {
	incr WHERE
	set HISTORY($WHERE) $args
        set doc [doc_find $args]
        if { $doc == "" } {
            error_page "no documentation for $args"
            return
        }
        $viewer render $doc
	configure -title "$ftitle: [$viewer title]"
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable viewer ""
    protected variable ftitle ""
    protected variable HISTORY 
    protected variable WHERE 0
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        package require Iwidgets
        set viewer [iwidgets::scrolledhtml $This.html -width 6i -height 5i]
	$viewer configure -linkcommand "$this eLink" \
	    -width 600 -height 400
        pack $viewer -side top -expand 1 -fill both
	set ftitle "$title"
    }
    destructor {
        destroy $viewer
    }
    # -------------------------------------------------- Private Methods ---
    protected method error_page {msg} {
        set pg "<h2>ERROR</h2>\n"
        append pg "$msg\n"
        $viewer render $pg
    }
    # -- locate the help page that match the document keys
    # -- for now, it has to exist
    # FIXME:help: implement full keyword help
    protected method doc_find {keys} {
        set key [lindex $keys 0]
        #set f [file join $docroot/$key.html]
        #if ![file exists $f] { return "" }
	if [info exists HELP($key)] {
	    return $HELP($key)
	}
	return ""
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method vMakeMenus {w} {
        set w
    }
    public method vMakeToolbar {w} {
        toolbutton_add       $w.back    -text BACK    -command "$this eBack"
        toolbutton_add_right $w.dismiss -text DISMISS -command "$this Hide"
        set w
    }
    public method eLink {url} {
	Show $url
    }
    public method eBack {} {
	set N [expr $WHERE -1 ]
	if ![info exists HISTORY($N)] { bell ; return }
	set now $HISTORY($N)
	unset HISTORY($WHERE)
	incr WHERE -2
	Show $now
    }
    # -------------------------------------------------- Common Initialization
    # -------------------------------------------------- Common Initialization
    common HELP
    array set HELP {
/ {
    <h2>ADE / UI Help</h2>
<ul>
    <li><a href=/about>About ADE/UI</a>
    <li><a href=/news>UI developer news</a>
    <li><a href=/gcmds>UI commands</a>
    <li><a href=/ucmds>UI user commands</a>
    <li><a href=/ade/plotter>ADE plotter</a>
    <li><a href=/ade/note>ADE note</a> taking
</ul>
}

/about {
<title>About ADE/UI</title>
<br>
<br>
<br>
<center>
<h1>
    ADE/UI(tm) graphical user interface
</h1>
</center>
}
/gcmds {
<title> ADE/UI commands </title>
<h2>ADE/UI commands</h2>

The following keyboard/mouse shortcuts are recognized:<br>

<PRE>
<b>        KEY         Description</b>
               S: take a snapshot of the current view and put it in the album
           SPACE: zoom in
  CONTROL  SPACE: zoom out
      Arrow Keys: pan left, right, up and down
LEFT MOUSE
          BUTTON: Select an object
          DELETE: Clear Current Selection
SHIFT+LEFT
    MOUSE BUTTON: select an area to zoom in
RIGHT
    MOUSE BUTTON: Inspect an object
             ESC: Interrupt the drawing of large number of objects

<b>BOOKMARKS:</b>
In the overview window, users can define, activate, and delete bookmarks
               b: defines the current bbox as a bookmark
               d: deletes the bookmark under the cursor
    MOUSE BUTTON: over a bookmark, changes the bbox to that of the bookmark
</PRE>
}

/error {
<title> BAD PAGE </title>
    This page does not exist
}


/zdb {
<title>ZDB: ADE debugger</title>
<h2>ZDB: ADE debugger</h2>

<PRE>
It can be invoked from the ade command prompt, and will
accept additional arguments corresponding to the tcl script to run:

% zdb db ztest

This particular command will invoke the ztest routine will simulate
the effect of debugging c++ code.

At the zdb prompt, you can use help to see the available commands.

ZDB will allow stepping through any zevent posted by the C++ code,
with additional logic to stop when a condition is true.

}


/ucmds {
<title>ADE/UI user commands</title>
<h2>ADE/UI user commands</h2>
<P>
Commands can be entered from the command line window, at the
bottom of the UI screen:

<PRE>
show net id ID       : to show a net by id
show net name NAME   : to show a net by id
show id              : guess what id is, and then show the corresponding obj

select net id ID
select net name NAME : like the corresponding show, but will create a selection
+select net id ID

<i>(NAME or ID can be a list of items, or a single item)</i>

bbox x0 y0 x1 y1     : zoom to the corresponding bbox
bbox sel             : will zoom to selection (if any)

gfactor  INT         : grid size in pixels to sample/aggregate

source file          : run all commands from file

help                 : this help page
</PRE>
}

/news {
<title>Developer News</title>

<h2>ADE/UI 2.1.2 (delos)</h2>
<ol>
<li>control+click1 on the filter exculsively sets this filter
<li>Added balloon info  on/off in the command bar
<li>bbox sel: will zoom to selection (also from the view menu)
</ol>

<h2>ADE/UI 2.1.1 (delos)</h2>
<ol>
<li>Added Notepad Capability, and access from ADE
<li>Added Generic Visualization Capability, applied to milos
</ol>

<h2>ADE/UI 2.1 (delos)</h2>
<ol>
<li> Caching of datasets (datasets are the result of queries send to ADE)
<li> ADE-Album: the SNAP commands takes a picture of the main canvas and also generates a web album page, indexed by the command that has generated the picture.
<li> ADE to GUI communication (availabe through the 'gui' command on ADE), to make the GUI do things (like plotting).
</ol>
}

/ade/plotter {
<title>ADE/UI Plotter</title>
<h2>ADE/UI Plotter</h2>
<P>
ADE users can exploit the ADE/UI capabilities in order to plot
data values, right from their ade prompt, using the 
<code>gui plot data ?tag?</code> construct.
<br>
The following code fragment can plot a sequence of points:
</P>
<pre>
set a  {
        title "Demo plot"
        xtitle "time"
        ytitle "money"
        data {
            set1 {
                1970 10
                1975 20
                1980 30
                1985 40
                1990 20
                1995 40
                2000 45
                2005 55
            }
            set2 {
                1970 13
                1975 23
                1980 20
                1985 10
                1990 24
                1995 43
                2000 48
                2005 39
            }
        }
    }
<pre/>
Then, the following ade command will plot:
<code>
gui plot $a
</code>

The optional argument ?tag? will send the plot to a different tab 
on the plotter.
}

/ade/note {
<title>ADE/UI Notes</title>
<h2>ADE/UI Notes</h2>
<P>
ADE users can send a text list of names to the UI,
and subsequently use them to issue UI COMMANDS, like "find net id"

<code>gui note data ?tag?</code> construct.
The following command will send a list of net ids to the UI:
<pre>
<pre/>
Then, the following ade command will plot:
<code>
set a "2000 2001 2002"
gui note $a
</code>
}

}
}
