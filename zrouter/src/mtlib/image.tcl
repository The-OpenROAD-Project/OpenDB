package require Itcl
package require Img
package require BLT

package provide mtlib 1.0
#
#       Class: LImageMeta 
# Description: Meta data image manager
#      Author: Manolis M. Tsangaris  -- mmt@performance.enet.gr
#
# Manage image Thumpnails and other metadata (in the metadir directory)
#
itcl::class LImageMeta {
    inherit LBinder
    # Event <<info>>    msg
    # Event <<warning>> msg
    # -------------------------------------------------- Public Variables --
    public variable metadir     .rsc     ; # dir to store metadata
    public variable meta        .inf     ; # metadata file suffix
    public variable iconwidth    64
    public variable iconheight   64
    # -------------------------------------------------- Public Methods ----
    # -- return a tk image that corresponds to the icon of this file
    # -- the caller should dispose the tk image
    public method IconGet {file} {
        icon_get $file
    }
    public method Get {file pp} {
        upvar $pp P
        array set P {}
        meta_get $file P
    }
    public method Put {file pp} {
        upvar $pp P
        meta_put $file P
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable IM
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        # -- create two scratch images
        set IM(1) [image create photo]
        set IM(2) [image create photo]
    }
    destructor {
        image delete $IM(1) 
        image delete $IM(2)
    }
    # -------------------------------------------------- Private Methods ---
    # Retrieve the metadata of a file stored in 
    protected method meta_get {file pp} {
        upvar $pp P
        if ![file exists $file] { return 0 }
        if [file isdirectory $file] { return 0 }
        set mt [file dirname $file]/$metadir/[file tail $file]$meta
        if ![file exists $mt] { 
            exec touch $mt
            return 1
        }
        array set P [exec cat $mt]
        return 1
    }
    # -- save metadata 
    protected method meta_put {file pp} {
        upvar $pp P
        if ![file exists $file] { return 0 }
        if [file isdirectory $file] { return 0 }

        set mt [file dirname $file]/$metadir/[file tail $file]$meta
        if ![file exists $mt] { 
            exec touch $mt
            return 1
        }
        set fp [open $mt w]
        foreach p [array names P] {
            puts $fp [list $p $P($p)]
        }
        close $fp
        return 1
    }
    # -- retrieve or generate the icon of a file
    protected method icon_get {f} {
        set icon [meta_get_icon $f]
        set im [image create photo]

        if { $icon == "" } {
            set icon "Image file-unknown"
        }
        # -- is it a builtin icon?
        if [string match file-* $icon] {
            $im copy [LICONS::Image $icon] -shrink
        } elseif [catch {$im config -file $icon -format jpeg} ] {
            $im copy [LICONS::Image file-unknown] -shrink
        }
        return $im
    }

    protected method meta_get_icon {f} {
        if ![file exists $f] { return "" }
        if [file isdirectory $f] {
            return file-folder
        }
        switch [file extension $f] {
            .c - .h - .c++ - .CC - .cpp  { return file-c }
            .html - .htm - .doc - .txt   { return file-doc }
        }
        set icons [file dirname $f]/$metadir
        set iconfile $icons/[file tail $f]

        # -- no icon file?
        if ![file exists $iconfile] {
            exec mkdir -p $icons
            if { ![meta_icon_generate $f $iconfile] } {
                return ""
            }
        }
        # -- has the file changed
        if { [file mtime $f] > [file mtime $iconfile] } {
            if {![meta_icon_generate $f $iconfile]} {
                return ""
            }
        }
        return $iconfile
    }

    # Generate an icon file for an image file f
    public method meta_icon_generate {f iconfile} {
        Event <<info>> "icon:gen $f"
        # -- read in the image
        $IM(1) blank
        if [catch {$IM(1) config -file $f} err] {
            Event <<warning>> "icon:read source: $f: $err"
            return 0
        }
        # -- scale the image to fit
        if [catch {Fit $IM(1) $IM(2) $iconwidth $iconheight} err] {
            Event <<warning>> "icon:scale: $f: $err"
            return 0
        }
        if { $err == 0 }  { return 0 }
        # -- write back the icon file
        $IM(2) write  $iconfile -format jpeg
        # -- now clean the image
        $IM(1) copy [LICONS::Image file-folder] -shrink
        return 1
    }

    # ------------------------------------------------------------------------
    # copy im1 to im2 and fit that to width & height -- maintain aspect ratio
    # use the BLT winop resample
    proc Fit {im1 im2 width height} {
        set w [image width  $im1]
        set h [image height $im1]

        # -- empty image?
        if { ($w == 0) || ($h == 0) } { 
            return 0
        }
        # -- scale factors
        set sx [expr 1.0*$width/$w]
        set sy [expr 1.0*$height/$h]
    
        # -- keep the smallest scale 
        if { $sx  > $sy } {
            set sx $sy
        }
        set w1 [expr int($sx * $w)]
        set h1 [expr int($sx * $h)]

        $im2 config -width $w1 -height $h1
        blt::winop resample $im1 $im2 box
        return 1
    }

    # ------------------------------------------------------------------------
    # copy im1 to im2 and fit that to width & height -- maintain aspect ratio
    # use the BLT winop resample
    proc RFit {im1 im2 width height} { ;# rough fit (integer scaling)
        set w [image width  $im1]
        set h [image height $im1]

        # -- empty image?
        if { ($w == 0) || ($h == 0) } { 
            return 0
        }
        # -- scale factors
        set sx [expr 1.0*$width/$w]
        set sy [expr 1.0*$height/$h]
    
        # -- keep the smallest scale 
        if { $sx  > $sy } {
            set sx $sy
        }
        set w1 [expr int($sx * $w)]
        set h1 [expr int($sx * $h)]

        if { $sx > 1.0 } {
            set f [expr int($sx)]
            $im2 copy $im1 -zoom $f $f
        } else {
            set f [expr int(1.0/$sx)]
            $im2 copy $im1 -subsample $f $f
        }
        $im2 config -width $w1 -height $h1
        return 1
    }

    # -------------------------------------------------- Common Initialization
    # -------------------------------------------------- Common Initialization
}
