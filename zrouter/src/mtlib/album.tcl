package provide mtlib 1.0

#
#       Class: LAlbum
# Description: Create and manage a web album
#
# -- FIXME: 
itcl::class LAlbum {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable pages  0                       ;# how many pages?
    public variable dir album   {set DIRSOK 0}
    public variable media media {set DIRSOK 0} 
    public variable attributes ""                  ;# page attributes
    public variable browsers "mozilla:firefox:opera:mfx:mfb"
    public variable quality 95
    public variable merge    0                     ;# merge an existing file
    # -------------------------------------------------- Public Methods ----
    public method AddTclImage {image} {
	mk_dirs
	set img  image[incr NIMAGES].jpg
	set file [file join $dir $media $img]
	$image write $file -format jpeg,quality=$quality
	set img
    }
    public method AddPage {page} {
	set PAGE($pages) $page
	incr pages
    }
    public method Render {} {
	render
    }
    public method ImagePath {img} {
	file join $dir $media $img
    }
    # -- point the browser to the album page
    public method BrowserView {} {
	browser_goto file://[file normalize $dir/index.html]
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable DIRSOK 0
    protected variable PAGE
    protected variable NIMAGES 0
    protected variable BROWSER ""
    common PROTO
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set BROWSER [browser_find]
	##FIX ME: if [file exists $dir/album.tcl] { source $dir/album.tcl }
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    protected method mk_dirs {} {
	if $DIRSOK { return }
        file mkdir $dir
	file mkdir [file join $dir $media]
	set DIRSOK 1
    }
    protected method render {} {
	if { [array size PAGE] == 0 } { return }
	mk_dirs

	set index ""
	foreach pageno [lsort -integer [array names PAGE]] {
	    array set P [list \
		title "Page $pageno" caption "" body "" image ""]
	    if [catch {array set P $PAGE($pageno)} err] {
		puts stderr "warning: page $pageno: $PAGE($pageno): $err"
	    }
	    mkpage $pageno P index
	    unset P
	}
	mkindex $index pages.html

	# -- generate the index.html page now
	set attr [list title "Web Album" startpage [lindex $index 0]\
	    controlpage pages.html]
	array set P $attr
	array set P $attributes
	page_expand $PROTO(index.html) P [file join $dir index.html]
	state_save $dir/album.tcl
    }
    # -- fix me
    protected method state_save {file} {
	set fp [open $file w]
	foreach p [array names PAGE] {
	    puts $fp [list AddPage $PAGE($p)]
	}
	close $fp
    }

    # -- generate a page & update the index variable
    # -- index holds the list of pages
    protected method mkpage {pageno pp ii} {
	upvar $pp P
	upvar $ii index

	# -- append this page to the index
	lappend index page$pageno.html $P(title) 

	set page [file join $dir page$pageno.html]

	set fp [open $page w]
	puts $fp "<html>"
	puts $fp "<head>"
	puts $fp "<title>$P(title)</title>"
	puts $fp "</head>"
	puts $fp "<body>"
	puts $fp "   <center>"
	puts $fp "   <h2>$P(title)</h2>"
	if { $P(image) != "" } {
	    puts $fp "   <img src=media/$P(image)><br>"
	}
	if { $P(caption) != "" } {
	    puts $fp "   <h2>$P(caption)</h2>"
	}
	puts $fp "   </center>"
	if { $P(body) != "" } {
	    puts $fp "$P(body)"
	}
	puts $fp "</body>"
	puts $fp "</html>"
	close $fp
	set page
    }

    # -- generate the index file
    protected method mkindex {index file} {
	set fp [open [file join $dir $file] w]
	puts $fp "<html>"
	foreach {fpage descr} $index {
	    puts $fp "<li><a href=$fpage target=main>$descr</a><br>"
	}
	puts $fp "</html>"
	close $fp
    }

    # -- expand a page from the templates
    protected proc page_expand {_proto _pp {_file ""}} {
	upvar $_pp _P
	foreach _var [array names _P] {
	    set $_var $_P($_var)
	}
	set _page [subst -nocommands $_proto]
	if { $_file != "" } {
	    set fp [open $_file w]
	    puts $fp $_page
	    close $fp
	}
	set _page
    }
    # -- locate one of the browsers in the path
    protected method browser_find {} {
	global env
	if [info exists env(BROWSER)] {
	    return $env(BROWSER)
	}
	set brs [split $browsers :]
	# -- see if any of the browsers can be found
	foreach b $brs {
	    foreach p [split $env(PATH) :] {
		set f [file join $p $b]
		if [file executable $f] {
		    return $f
		}
	    }
	}
	return mozilla
    }
    # -- point a running browser to url
    protected method browser_goto {url} {
        set fmt {%s -remote "OpenURL(%s)"}
        set cmd [format $fmt $BROWSER $url]
	eval exec $cmd
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization

    set PROTO(index.html) {
<html>
<head>
<title>
           $title
</title>
</head>

<body>
<table width=100% height=100% border=0 cellpadding=0 cellspacing=0>

<tr>
        <td colspan=2 align=center bgcolor=#cceecc>
            <h2>
            $title
            </h2>
        </td>
</tr>

<tr>
    <td bgcolor=#cccccc align=center><B>Requests</B></td>
    <td height=100% rowspan=2>
        <iframe name=main height=100% src="$startpage"
            width=100% frameborder=0>
        </iframe>
    </td>
</tr>
<tr>
    <td height=100% width=200 bgcolor=#cccccc>
        <iframe src=$controlpage height=100% width=200 frameborder=0>
        </iframe>
    </td>
</tr>
</table>
</body>
</html>
}
}
