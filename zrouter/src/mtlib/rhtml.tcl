package provide mtlib 1.0

namespace eval LHTML {
	variable HMevents
	variable HMinsert_map
	variable HMlist_elements
	variable HMtag_map
	variable _url
	variable dataArray
	variable paramArray
	variable UPCALL


# -- available events:
# -- <<link>> reference

proc Bind {event upcall} {
    variable UPCALL
    set UPCALL($event) $upcall
}

## ******************************************************** 
##                  ERRATA and PREAMBLE
## ******************************************************** 
#
# Simple HTML display library by:
#              Stephen Uhler (stephen.uhler@sun.com)
#
# This code was hacked almost beyond recognition by:
#              Phil Ehrens (pehrens@ligo.caltech.edu)
# during the summer of 1998.
#
# Neither Phil nor CalTech assumes any responsibility for
# damage caused by this code.  USE AT YOUR OWN RISK!!
#
# To use this package,  create a text widget (say, .text)
# and set a variable full of html, (say $html), and issue:
#     HMinit_win .text
#     HMparse_html $html "HMrender .text"
# To return a "used" text widget to its initialized state, call:
#   HMreset_win .text
#
# If you have a FILE you want to render, just call:
#      HMlink_callback .textwidget filename
# and everything will be handled for you.
#
## ********************************************************

##package provide html 1.0

## ********************************************************
# mapping of html tags to text tag properties
# properties beginning with "T" map directly to text tags

array set HMtag_map {
     b      {weight bold}
     blockquote  {style italic indent 1 Trindent rindent}
     bq     {style italic indent 1 Trindent rindent}
     cite   {style italic}
     code   {family courier}
     dfn    {style italic}     
     dir    {indent 1}
     dl     {indent 1}
     em     {style italic}
     h1     {size 24 weight bold}
     h2     {size 22}          
     h3     {size 20}     
     h4     {size 18}
     h5     {size 16}
     h6     {style italic}
     i      {style italic}
     kbd    {family courier weight bold}
     menu   {indent 1}
     ol     {indent 1}
     pre    {fill 0 family courier Tnowrap nowrap}
     samp   {family courier}          
     strong {weight bold}          
     tt     {family courier}
     u      {Tunderline underline}
     ul     {indent 1}
     var    {style italic}
     center {Tcenter center}
     strike {Tstrike strike}
}

# initialization values.
set HMtag_map(hmstart) {
     family default  weight normal  style roman  size 12
     Tcenter ""  Tlink ""  Tnowrap ""  Tunderline ""
     list list  fill 1  indent ""  counter 0  adjust 0
}
## ******************************************************** 

## ******************************************************** 
# html tags that insert vertical white space
array set HMinsert_map {
   blockquote "\n\n" /blockquote "\n"
   br    "\n"
   dd    "\n"   /dd   "\n"
   dl    "\n"   /dl   "\n"
   dt    "\n"
   form  "\n"   /form "\n"
   h1    "\n\n" /h1   "\n"
   h2    "\n\n" /h2   "\n"
   h3    "\n"   /h3   "\n"
   h4    "\n"   /h4   "\n"
   h5    "\n"   /h5   "\n"
   h6    "\n"   /h6   "\n"
   li    "\n"
   /dir  "\n"
   /ul   "\n"
   /ol   "\n"
   /menu "\n"
   p     "\n\n"
   pre   "\n"   /pre "\n"
}
## ******************************************************** 

## ******************************************************** 
# tags that are list elements, that support "compact" rendering
array set HMlist_elements {
     ol   1   
     ul   1   
     menu 1   
     dl   1   
     dir  1
}
## ******************************************************** 

## ******************************************************** 
# initialize the window and stack state
proc HMinit_win {win} {
     variable HM$win
     upvar #0 HM$win var
     HMinit_state $win
     $win tag configure underline -underline 1
     $win tag configure center    -justify center
     $win tag configure nowrap    -wrap none
     $win tag configure rindent   -rmargin $var(S_tab)c
     $win tag configure strike    -overstrike 1
     $win tag configure mark      -foreground red ;# list markers
     $win tag configure list      -spacing1 3p   -spacing3 3p
     $win tag configure compact   -spacing1 0p
     $win tag configure link      -borderwidth 2 -foreground blue
     
     HMset_indent $win $var(S_tab)
     
     $win configure -wrap word

     # configure the text insertion point
     $win mark set $var(S_insert) 1.0

     # for horizontal rules
     $win tag configure thin -font { default 2 roman }
     $win tag configure hr   -relief sunken \
                             -borderwidth 2 \
                             -wrap none \
                             -tabs [winfo width $win]
     bind $win <Configure> {
          %W tag configure hr -tabs %w
          %W tag configure last -spacing3 %h
     }
     # generic link callback
     $win tag bind link <1> "LHTML::HMlink_hit $win %x %y"
}
## ******************************************************** 

## ******************************************************** 
# set the indent spacing (in cm) for lists
# TK uses a "weird" tabbing model that causes \t to insert a single
# space if the current line position is past the tab setting

proc HMset_indent {win cm} {
     set tabs [expr $cm / 2.0]
     $win configure -tabs ${tabs}c
     foreach i {1 2 3 4 5 6 7 8 9} {
          set tab [expr $i * $cm]
          $win tag configure indent$i -lmargin1 ${tab}c \
                                      -lmargin2 ${tab}c \
          -tabs "[expr $tab + $tabs]c [expr $tab + 2*$tabs]c"
     }
}
## ******************************************************** 

## ******************************************************** 
# reset the state of window - get ready for the next page
# remove all but the font tags, and remove all form state

proc HMreset_win {win} {
     variable HM$win
     upvar #0 HM$win var
     regsub -all { +[^L ][^ ]*} " [$win tag names] " {} tags
     catch "$win tag delete $tags"
     eval $win mark unset [$win mark names]
     $win delete 0.0 end
     $win tag configure hr -tabs [winfo width $win]

     # configure the text insertion point
     catch { $win mark set $var(S_insert) 1.0 }

     # remove form state.  If any check/radio buttons still exists, 
     # their variables will be magically re-created, and never get
     # cleaned up.
     catch unset {[namespace eval [namespace current] info vars HM$win.form*]}
     # catch unset [info globals HM$win.form*]

     HMinit_state $win
     return HM$win
}
## ******************************************************** 

## ******************************************************** 
# initialize the window's state array
# Parameters beginning with S_ are NOT reset
#
#  adjust_size:  global font size adjuster
#      unknown:  character to use for unknown entities
#          tab:  tab stop (in cm)
#         stop:  enabled to stop processing
#       update:  how many tags between update calls
#         tags:  number of tags processed so far
#      symbols:  Symbols to use on un-ordered lists

proc HMinit_state {win} {
     variable HM$win
     upvar #0 HM$win var
     array set tmp [array get var S_*]
     catch {unset var}
     array set var {
          stop          0
          tags          0
          fill          0
          list          list
          S_adjust_size 0
          S_tab         1.0
          S_unknown     \xb7
          S_update      10
          S_symbols     O*=+-o\xd7\xb0>:\xb7
          S_insert      Insert
     }
     array set var [array get tmp]
}
## ******************************************************** 

## ******************************************************** 
# manage the display of html
# this can stand much improvement
# HMrender gets called for every html tag
#   win:   The name of the text widget to render into
#   tag:   The html tag (in arbitrary case)
#   not:   a "/" or the empty string
#   param: The un-interpreted parameter list
#   text:  The plain text until the next html tag

proc HMrender {win tag not param text} {
     variable HM$win
     upvar #0 HM$win var
     if {$var(stop)} return
     variable HMtag_map
	 variable HMinsert_map
	 variable HMlist_elements
     set tag [string tolower $tag]
     set text [HMmap_esc $text]

     # manage compact rendering of lists
     if {[info exists HMlist_elements($tag)]} {
        set list "list [expr {[HMextract_param $param compact] ? "compact" : "list"}]"
        } else {
        set list ""
        }

     # Allow text to be diverted to a different window (for tables)
     # this is not currently used
     if {[info exists var(divert)]} {
        set win $var(divert)
     	variable HM$win
        upvar #0 HM$win var
        }

     # adjust (push or pop) tag state
     catch {HMstack $win $not "$HMtag_map($tag) $list"}

     # insert white space (with current font)
     # adding white space can get a bit tricky.  This isn't quite right
     set bad [catch {$win insert $var(S_insert) $HMinsert_map($not$tag) "space $var(font)"}]
     if {!$bad && [lindex $var(fill) end]} {
        set text [string trimleft $text]
        }

     # to fill or not to fill
     if {[lindex $var(fill) end]} {
        set text [HMzap_white $text]
        }

     catch { XMLtag $not$tag $win $param text } err

     # add the text with proper tags
     set tags [HMcurrent_tags $win]
     $win insert $var(S_insert) $text $tags

     # We need to do an update every so often to insure
     # interactive response.
     if {!([incr var(tags)] % $var(S_update))} {
        update
        }
}
## ******************************************************** 

## ******************************************************** 
#  new generic tag handling, with some old holdouts
#  I just haven't decided to can quite yet.

proc XMLtag { tag win param text } {
     variable HM$win
     upvar #0 HM$win var
     upvar $text data
     variable dataArray
	 variable paramArray
     switch -exact -- $tag {
            xydata { ;## sample tag for xy data
                   array set paramArray [ split $param = ]
                   array set dataArray $data
                   set data "" ;## so it won't print     
                   }
             int2u { ;## LDAS 16 bit unsigned int array
                     ;## with size="num of elements"
                   list dataVector
                   set data ""
                   } 
             int2s { ;## LDAS 16 bit signed int
                   set data ""
                   }
             int4u { ;## LDAS 32 bit unsigned int
                   set data ""
                   }
             int4s { ;## LDAS 32 bit signed int
                   set data ""
                   }
             real4 { ;## LDAS 32 bit float
                   set data ""
                   }
             real8 { ;## LDAS 64 bit double
                   set data ""
                   }
            matrix { ;## LDAS 2d array with rows, cols, and type
                     ;## where type is a nested tag as above
                   set matrix(flag) true
                   array set paramArray [ split $param = ]
                   list dataArray $data
                   set n 0
                   while { n < $paramArray(cols) } {
                         lappend cols $n
                         incr n
                         }
                    foreach [ set cols ] $data {
                            }
                   }
           /matrix {
                   unset matrix(flag)
                   }
           hmstart { ;## tag internal to this lib
                   $win mark gravity $var(S_insert) left
                   $win insert end "\n " last
                   $win mark gravity $var(S_insert) right
                   }
          /hmstart { ;## tag internal to this lib
                   $win delete last.first end
                   }
             title { ;## set the help window title
                   wm title [winfo toplevel $win] $data
                   set data ""
                   }
                hr { ;## set a horizontal rule
                   $win insert $var(S_insert) "\n" space "\n" thin "\t" \
                                               "thin hr" "\n" thin
                   }
                ol { ;## numbered list
                   set var(count$var(level)) 0
                   }
                ul { ;## unnumbered list
                   catch {unset var(count$var(level))}
                   }
              menu { ;## unsupported, sorry
                   set var(menu) ->
                   set var(compact) 1
                   }
             /menu { ;## obvious
                   catch {unset var(menu)}
                   catch {unset var(compact)}
                   }
                dt { ;## definition term portion of dl tag
                   set level $var(level)
                   incr level -1
                   $win insert $var(S_insert) "$data" \
                   "hi [lindex $var(list) end] indent$level $var(font)"
                   set data ""
                   }
                li { ;## list item
                   set level $var(level)
                   incr level -1
                   set x [string index $var(S_symbols)+-+-+-+- $level]
                   catch {set x [incr var(count$level)]}
                   catch {set x $var(menu)}
                   $win insert $var(S_insert) \t$x\t \
                   "mark [lindex $var(list) end] indent$level $var(font)"
                   }
                 a { ;## href tag handling
                   ## for a source
                   if {[HMextract_param $param href]} {
                      set var(Tref) [list L:$href]
                      HMstack $win "" "Tlink link"
                      HMlink_setup $win $href
                      }
                   ## for a destination
                   if {[HMextract_param $param name]} {
                      set var(Tname) [list N:$name]
                      HMstack $win "" "Tanchor anchor"
                      $win mark set N:$name "$var(S_insert) - 1 chars"
                      $win mark gravity N:$name left
                      if {[info exists var(goto)] && $var(goto) == $name} {
                         unset var(goto)
                         set var(going) $name
                         }
                      }
                   }
                /a {
                   if {[info exists var(Tref)]} {
                      unset var(Tref)
                      HMstack $win / "Tlink link"
                      }

                   # goto this link, then invoke the call-back.
                   if {[info exists var(going)]} {
                      $win yview N:$var(going)
                      update
                      HMwent_to $win $var(going)
                      unset var(going)
                      }

                  if {[info exists var(Tname)]} {
                     unset var(Tname)
                     HMstack $win / "Tanchor anchor"
                     }
                   }
               img { ;## image tag handling
                   # get alternate text
                   set alt "image"
                   HMextract_param $param alt
                   set alt [HMmap_esc $alt]
                   set alt "<${alt}>"
                   set item $win.$var(tags)
                   set label $item
                   label $label 
                   $label configure -relief flat -bd 1 -fg red -text $alt
                   $win window create $var(S_insert) -window $item -pady 2 -padx 2

                   # add in all the current tags (this is overkill)
                   set tags [HMcurrent_tags $win]
                   foreach tag $tags { $win tag add $tag $item }

                   # now callback to the application
                   set src ""
                   HMextract_param $param src
                   HMset_image $win $label $src
                   }
           }
}

## ******************************************************** 

## ******************************************************** 
# The application should call here with the fragment name
# to cause the display to go to this spot.
# If the target exists, go there (and do the callback),
# otherwise schedule the goto to happen when we see the reference.
proc HMgoto {win where {callback HMwent_to}} {
     variable HM$win
     upvar #0 HM$win var
     if {[regexp N:$where [$win mark names]]} {
        $win see N:$where
        update
        eval $callback $win [list $where]
        return 1
        } else {
        set var(goto) $where
        return 0
        }
}

# We actually got to the spot, so highlight it!
proc HMwent_to {win where {count 0} {color orange}} {
     variable HM$win
     upvar #0 HM$win var
     if {$count > 5} return
     catch {$win tag configure N:$where -foreground $color}
     update
     after 200 [list HMwent_to $win $where [incr count] \
               [expr {$color=="orange" ? "" : "orange"}]]
}

## ******************************************************** 

## ******************************************************** 
# deal with a local bare gif file
proc HMbare_img { win href } {
     set img [ image create photo -file $href ]
     $win image create end -padx 8 -pady 8 -image $img
}

proc HMset_image {win handle src} {
     if { [ file exists [file join [pwd] help $src ]] } {
        set src [file join [pwd] help $src]
        } else {
        #get_file [file join [file dirname $url] $src]        
        #fileevent $src readable set src xxx
        #puts "$url $src"
        }
        set gif [ image create photo -file $src ]
        HMgot_image $handle $gif
}

# When the image is available, the application should call back here.
# If we have the image, put it in the label, otherwise display the error
# message.  If we don't get a callback, the "alt" text remains.
# if we have a clickable image, arrange for a callback

proc HMgot_image {win image_error} {
     # if we're in a frame turn on geometry propogation
     if {[winfo name $win] == "label"} {
        pack propagate [winfo parent $win] 1
        }
     if {[catch {$win configure -image $image_error}]} {
        $win configure -image {}
        $win configure -text $image_error
        }
}
## ******************************************************** 

## ******************************************************** 
# What should a hypertext link look like?
array set HMevents {
     Enter           {-borderwidth 2 -relief groove }
     Leave           {-borderwidth 2 -relief flat   }
     1               {-borderwidth 2 -relief sunken }
     ButtonRelease-1 {-borderwidth 2 -relief raised }
}
## ******************************************************** 

## ******************************************************** 
# We need to escape any %'s in the href tag name so the bind command
# doesn't try to substitute them.
proc HMlink_setup {win href} {
     variable HMevents
     regsub -all {%} $href {%%} href2
     foreach i [array names HMevents] {
          eval {$win tag bind  L:$href <$i>} \
               \{$win tag configure \{L:$href2\} $HMevents($i)\}
          }
}
## ******************************************************** 

## ******************************************************** 
# generic link-hit callback
# This gets called upon button hits on hypertext links
#   win:   The name of the text widget to render into
#   x,y:   The cursor position at the "click"

proc HMlink_hit {win x y} {
     set tags [$win tag names @$x,$y]
     set link [lindex $tags [lsearch -glob $tags L:*]]
     # regsub -all {[^L]*L:([^ ]*).*}  $tags {\1} link
     regsub L: $link {} link
     HMlink_callback $win $link
}
## ******************************************************** 

## ******************************************************** 
#    win:  The name of the text widget to render into
#   href:  The HREF link for this <a> tag.
#  We can only retrieve URL's with absolute pathnames
#  at present.
proc HMlink_callback {win href} {
  # if file is a help file
  foreach subdir [ glob * ] {
      if { [ file isdir $subdir ] } {
         if { [ file exists [file join [pwd] $subdir $href ]] } {
            set href [file join [pwd] $subdir $href]
            }
         }
  }
  # if file is a file in the current directory
  if { [ file exists $href ] } {
     set fid [ open "$href" r ]
     set txt [ read $fid ]
     close $fid
     } else {
       # we can't handle ftp, or some file formats
       if { ! [ regexp {^ftp:|^ftp\.|\.pdf$|\.jpg$|\.ps$|\.help$} $href ] } {
          if { [ regexp {^http:|\/|\.} $href ] } {
             set temp [ open "xxx" w ]           
             catch {
             set token [ http_get $href -channel $temp ]
             }
             close $temp
             set fid [ open "xxx" r ]
             set txt [ read $fid ]
             close $fid
             } 
          if { [ regexp {^#[^ ].+} $href ] } {     
             # a local tag in the current document
             regsub {^#} $href {} href
             HMgoto $win $href
             }
          if { [ regexp {<.+>+} $href ] } {
             set txt $href
             }
        }
   }   
   # if we have some content to render
   if { [ info exists txt ] } {
      $win configure -state normal
      HMreset_win $win
      HMinit_win $win
      # id a bare gif
      if { [ regexp {GIF8[79]a} $txt ] } {
         HMbare_img $win $href
         } else {
         HMparse_html $txt "HMrender $win"
         }
      $win configure -state disabled
      }
}
## ******************************************************** 

## ******************************************************** 
# extract a value from parameter list (this needs a re-do)
# returns "1" if the keyword is found, "0" otherwise
#   param:  A parameter list.  It should alredy have been processed to
#           remove any entity references
#   key:    The parameter name
#   val:    The variable to put the value into (use key as default)

proc HMextract_param {param key {val ""}} {
     if {$val == ""} {
        upvar $key result
        } else {
        upvar $val result
        }
     set ws "    \n\r"
    # look for name=value combinations.  Either (') or (") are valid delimeters
    if {
       [regsub -nocase [format {.*%s[%s]*=[%s]*"([^"]*).*} $key $ws $ws] $param {\1} value] ||
       [regsub -nocase [format {.*%s[%s]*=[%s]*'([^']*).*} $key $ws $ws] $param {\1} value] ||
       [regsub -nocase [format {.*%s[%s]*=[%s]*([^%s]+).*} $key $ws $ws $ws] $param {\1} value] } {
       set result $value
       return 1
       }

     # now look for valueless names
     # I should strip out name=value pairs, so we don't end up with "name"
     # inside the "value" part of some other key word - some day
     set bad \[^a-zA-Z\]+
     if {[regexp -nocase  "$bad$key$bad" -$param-]} {
        return 1
        } else {
        return 0
        }
}
## ******************************************************** 

## ******************************************************** 
# Push or pop tags to/from stack.
# Each orthogonal text property has its own stack, stored as a list.
# The current (most recent) tag is the last item on the list.
# Push is {} for pushing and {/} for popping

proc HMstack {win push list} {
     variable HM$win
     upvar #0 HM$win var
     array set tags $list
     if {$push == ""} {
        foreach tag [array names tags] {
             lappend var($tag) $tags($tag)
             }
        } else {
        foreach tag [array names tags] {
             # set cnt [regsub { *[^ ]+$} $var($tag) {} var($tag)]
             set var($tag) [lreplace $var($tag) end end]
             }
     }
}
## ******************************************************** 

## ******************************************************** 
# extract set of current text tags
# tags starting with T map directly to text tags, all others are
# handled specially.  There is an application callback, HMset_font
# to allow the application to do font error handling

proc HMcurrent_tags {win} {
     variable HM$win
     upvar #0 HM$win var
     set font font
     foreach i {family size weight style} {
          set $i [lindex $var($i) end]
          append font :[set $i]
          }
     set xfont [HMx_font $family $size $weight $style $var(S_adjust_size)]
     HMset_font $win $font $xfont
     set indent [llength $var(indent)]
     incr indent -1
     lappend tags $font indent$indent
     foreach tag [array names var T*] {
          lappend tags [lindex $var($tag) end]  ;# test
          }
     set var(font) $font
     set var(xfont) [$win tag cget $font -font]
     set var(level) $indent
     return $tags
}

# allow the application to do do better font management
# by overriding this procedure
proc HMset_font {win tag font} {
     catch {$win tag configure $tag -font $font} msg
}

# generate an X font name
proc HMx_font { { family default } { size 12 } { weight normal } \
                { style roman } {adjust_size 0} } {
     catch {incr size $adjust_size}
     set font "${family}${size}${weight}$style"
     if { [ catch { font configure $font } ] } {
        catch {
               font create $font -family $family \
                                 -size   $size   \
                                 -weight $weight \
                                 -slant  $style;
              }
     }
return $font
}
## ******************************************************** 

## ********************************************************
# Turn HTML into TCL commands
#   html  A string containing an html document
#   cmd   A command to run for each html tag found
#   start     The name of the dummy html start/stop tags

proc HMparse_html {html {cmd HMtest_parse} {start hmstart}} {
     regsub -all \{ $html {\&ob;} html
     regsub -all \} $html {\&cb;} html
     set w " \t\r\n"     ;# white space
     proc HMcl x {return "\[$x\]"}
     set exp <(/?)([HMcl ^$w>]+)[HMcl $w]*([HMcl ^>]*)>
     set sub "\}\n$cmd {\\2} {\\1} {\\3} \{"
     regsub -all $exp $html $sub html
     eval "$cmd {$start}   {} {} \{ $html \}"
     eval "$cmd {$start} / {} {}"
}

proc HMtest_parse { command tag slash text_after_tag } {
     puts "==> $command $tag $slash $text_after_tag"
}
## ******************************************************** 

## ******************************************************** 
# Convert multiple white space into a single space
proc HMzap_white { data } {
     regsub -all "\[ \t\r\n\]+" $data " " data
     return $data
}
## ******************************************************** 

## ******************************************************** 
# find HTML escape characters of the form &xxx;
proc HMmap_esc { text } {
     if { ![ regexp & $text ] } { return $text }
     regsub -all {([][$\\])} $text {\\\1} new
     regsub -all {&#([0-9][0-9]?[0-9]?);?} \
           $new {[format %c [scan \1 %d tmp;set tmp]]} new
     regsub -all {&([a-zA-Z]+);?} $new {[HMdo_map \1]} new
     return [ subst $new ]
}
## ******************************************************** 

## ******************************************************** 
# convert an HTML escape sequence into character
proc HMdo_map { text { unknown ? } } {
     variable HMesc_map
     set result $unknown
     catch { set result $HMesc_map($text) }
     return $result
}
## ******************************************************** 

## ******************************************************** 
# table of escape characters
array set HMesc_map {
     lt     <    gt     >    quot   \"   ob     \x7b  cb    \x7d
     nbsp   \xa0 iexcl  \xa1 cent   \xa2 pound  \xa3 curren \xa4
     yen    \xa5 brvbar \xa6 sect   \xa7 uml    \xa8 copy   \xa9
     ordf   \xaa laquo  \xab not    \xac shy    \xad reg    \xae
     hibar  \xaf deg    \xb0 plusmn \xb1 sup2   \xb2 sup3   \xb3
     acute  \xb4 micro  \xb5 para   \xb6 middot \xb7 cedil  \xb8
     sup1   \xb9 ordm   \xba raquo  \xbb frac14 \xbc frac12 \xbd
     frac34 \xbe iquest \xbf Agrave \xc0 Aacute \xc1 Acirc  \xc2
     Atilde \xc3 Auml   \xc4 Aring  \xc5 AElig  \xc6 Ccedil \xc7
     Egrave \xc8 Eacute \xc9 Ecirc  \xca Euml   \xcb Igrave \xcc
     Iacute \xcd Icirc  \xce Iuml   \xcf ETH    \xd0 Ntilde \xd1
     Ograve \xd2 Oacute \xd3 Ocirc  \xd4 Otilde \xd5 Ouml   \xd6
     times  \xd7 Oslash \xd8 Ugrave \xd9 Uacute \xda Ucirc  \xdb
     Uuml   \xdc Yacute \xdd THORN  \xde szlig  \xdf agrave \xe0
     aacute \xe1 acirc  \xe2 atilde \xe3 auml   \xe4 aring  \xe5
     aelig  \xe6 ccedil \xe7 egrave \xe8 eacute \xe9 ecirc  \xea
     euml   \xeb igrave \xec iacute \xed icirc  \xee iuml   \xef
     eth    \xf0 ntilde \xf1 ograve \xf2 oacute \xf3 ocirc  \xf4
     otilde \xf5 ouml   \xf6 divide \xf7 oslash \xf8 ugrave \xf9
     uacute \xfa ucirc  \xfb uuml   \xfc yacute \xfd thorn  \xfe
     yuml   \xff amp    &
}
## ******************************************************** 



###############
# start htmlbrow.tcl section
###############

# -- return path to text window from any widget in HTMLbrowser path
proc HB_textwin {w} {
	global level
	set path ""
	set i [string first $level(2) $w]
	if {-1 != $i} {
     		return "[string range $w 0 [expr $i + [string length $level(1)] - 1]].txt.win"
	} else {
		set i [string first $level(3) $w]
		if {$i == -1} {
			error "HB_textwin: could not figure out text widget for $w"
		}
		return "[string range $w 0 [expr $i - 1]]$level(4)"
	}
}

# -- build absolute path from url (relative/absolute/local)
proc HB_resolve_url {win url {dummy ""}} {
	variable _url
	global env
	set win $_url(textwidget)
	if {$_url($win~~lasturl) != "*"} {
		set _url($win~~url) [mergeurls $_url($win~~lasturl) $url]
	} else {
		set _url($win~~url) $url
	}
	return [extracttagkey $_url($win~~url)]
}

# -- display link url in entry when pointer is over it
proc HB_highlight_link {win href} {
	variable _url
	HB_init $win
	set win $_url(textwidget)
	set _url($win~~savedentry) [$_url($win~~entrywidget) get]
	$_url($win~~entrywidget) delete 0 end
    $_url($win~~entrywidget) insert end "$href"
	$_url($win~~entrywidget) configure -fg blue
	update
}

# -- restore url in entry when pointer leaves link
proc HB_unhighlight_link {win} {
	variable _url
	HB_init $win
	set win $_url(textwidget)
	if {![info exists _url($win~~savedentry)]} {
		set text ""
	} else {
		set text $_url($win~~savedentry)
	}
	$_url($win~~entrywidget) delete 0 end
	$_url($win~~entrywidget) insert end  "$text" 
	$_url($win~~entrywidget) configure -fg black
	update
}

# -- main load URL procedure
proc HB_load_url {win url {tag ""} {force 0}} {
	variable _url
	global env http
	HB_init $win
	set win $_url(textwidget)
	if {$url == "" && $tag == ""} {return}
	if {$_url($win~~lasturl) != "*"} {
		set url [mergeurls $_url($win~~lasturl) $url]
	}
	# -- get the status label widget
	if {"$tag" != ""} {
		$_url($win~~statuslabel) configure -text "fetching $url#$tag"
	} else {
		$_url($win~~statuslabel) configure -text "fetching $url"
	}
	set _url($win~~stopflag) 0
	$_url($win~~stopbut) configure -state normal
	update
	# -- for now assume anything that gets here is text or html
	# -- retrieve the URL to a local variable
	$win configure -cursor watch
	# -- put a final sanity check against getting .gif files
	if {[string last ".gif" $url] != -1} {return}
	# -- fetch the document
	if {$_url($win~~lasturl) != $url} {set force 1}
	set _url($win~~lasturl) $url
	if {$force} {
		set _url($win~~rawtext) [fetchURL -url $url]
		# -- reset url in case fetchURL was redirected
		set url $http(full)
	}
	$_url($win~~statuslabel) configure -text ""
	update
	# -- clear the window
	if {$url != $_url($win~~lasturl) || $force} {
		HMreset_win $win
	}
	# -- check for a stop prior to rendering
	if {$_url($win~~stopflag)} {
		$_url($win~~statuslabel) configure -text "Stopped" -fg red
		update
		after 1000 "$_url($win~~statuslabel) configure -text {} -fg White; update"
 	    $win configure -cursor arrow
		return
	}
	# -- update status label, entry area, and button for rendering time
	$_url($win~~statuslabel) configure -text "rendering $url"
	$_url($win~~entrywidget) delete 0 end
	if {$tag == ""} {
		$_url($win~~entrywidget) insert end  "$url" 
	} else {
		$_url($win~~entrywidget) insert end  "$url#$tag" 
	}		
	$_url($win~~entrywidget) configure -fg black
	$_url($win~~statuslabel) configure -text ""
	$_url($win~~gobut) configure -state normal
	$_url($win~~homebut) configure -state normal
	if {$_url($win~~histidx) < [expr [llength $_url($win~~history)] - 1]} {
		$_url($win~~backbut) configure -state normal
	} else {
		$_url($win~~backbut) configure -state disabled
	}
	if {$_url($win~~histidx) == 0} {
		$_url($win~~forwardbut) configure -state disabled
	} else {
		$_url($win~~forwardbut) configure -state normal
	}
	update
	# -- render only if a new document or forced to
	if {$force} {
		# -- loop through the rendering of the rawtext until done
		# -- find the start tag
		set start hmstart
		HMparse_html $_url($win~~rawtext) "HMrender $win"
		$_url($win~~stopbut) configure -state disabled
		update
	}
	# -- rendering done, jump to any internal link specified
	if {$tag != ""} {
		HMgoto $win "$tag"
	} else {
		# -- ensure that we're at top of window if no link jump performed
		tkTextSetCursor $win 1.0
	} 
	# -- update history
	incr _url($win~~histidx)
	if {$tag != ""} {
		set _url($win~~history) [join "$url#$tag $_url($win~~history)"]
	} else {
		set _url($win~~history) [join "$url $_url($win~~history)"]
	}
	incr _url($win~~histlen)
	$_url($win~~entrywidget) delete 0 end
	if {$tag == ""} {
		$_url($win~~entrywidget) insert end  "$url" 
	} else {
		$_url($win~~entrywidget) insert end  "$url#$tag" 
	}		
	$_url($win~~entrywidget) configure -fg black
	update
	# -- convert cursor back to arrow to show operation is complete
	$win configure -cursor arrow
	return
}	

# -- initialize 
proc HB_init {win} {
	variable _url
	global env
	set _url(textwidget) $win 
	if {[info exists _url($win~~inited)]} {return}
	set _url($win~~inited) 1
	set _url($win~~history) ""
	set _url($win~~histidx) -1
	set _url($win~~histlen) 0
	set _url($win~~lasturl) "*"
	if {[info exists env(TEMP)]} {
		set _url($win~~cachedir) "[eval file join [file split $env(TEMP)]]"
	} elseif {[info exists env(TMP)]} {
		set _url($win~~cachedir) "[eval file join [file split $env(TMP)]]"
	} elseif {[file exists "/tmp"]} {
		set _url($win~~cachedir) "/tmp"
	} elseif {[file exists "/temp"]} {
		set _url($win~~cachedir) "/temp"
	} elseif {[file exists "/usr/tmp"]} {
		set _url($win~~cachedir) "/usr/tmp"
	} else {
		set _url($win~~cachedir) "[pwd]"
	}
	set env(TEMP) $_url($win~~cachedir)

	if {![info exists _url($win~~statuslabel)]}  {
		set _url($win~~statuslabel) ".rouser.main.top.buttons.status"
		set _url($win~~entrywidget) ".rouser.main.top.ubar.url"
		set _url($win~~backbut)     ".rouser.main.top.buttons.back"
		set _url($win~~forwardbut)  ".rouser.main.top.buttons.forward"
		set _url($win~~homebut)     ".rouser.main.top.buttons.home"
		set _url($win~~gobut)       ".rouser.main.top.buttons.go"
		set _url($win~~stopbut)     ".rouser.main.top.buttons.stop"
	}

	# -- initialize GUI elements
	$_url($win~~statuslabel) configure -text ""
	$_url($win~~entrywidget) delete 0 end
	$_url($win~~entrywidget) configure -fg black
	$_url($win~~backbut) configure -state disabled
	$_url($win~~forwardbut) configure -state disabled
	$_url($win~~gobut) configure -state disabled
	$_url($win~~stopbut) configure -state disabled
	# -- setup entry widget bindings
	bind $_url($win~~entrywidget) <Return> {
	    variable _url
	    set win $_url(textwidget)
		HB_init $win
		set _url($win~~histidx) -1
		set _url($win~~lasturl) "*"
	    set urllist [HB_resolve_url $win [$_url($win~~entrywidget) get]]
		set turl "[lindex $urllist 0][lindex $urllist 2]"
                    HB_load_url $win $turl [lindex $urllist 1]
	}
	bind $_url($win~~entrywidget) <BackSpace> {tkEntryBackspace %W}
	bind $_url($win~~entrywidget) <Delete> {
		if {[%W selection present]} {
			%W delete sel.first sel.last
		} else {
			%W delete insert
		}
	}
	HMinit_win $win
}

#----------
proc HB_go_form {win} {
	variable _url
	global env
	set win $_url(textwidget)
	HB_init $win
	catch "destroy .history"
	if {[llength $_url($win~~history)] == 0} {return}
	toplevel .history 
	wm title .history {top0}
	wm geometry .history 400x400+[winfo pointerx $win]+[winfo pointery $win]
	wm minsize .history 1 1
	set Name .history.f3
	frame $Name  -background lightgray -borderwidth 2 -height 50 -highlightbackground lightgray -relief raised -width 50
	place $Name -x 0 -relx 0 -y -1 -rely 0 -width 392 -relwidth 0 -height 360 -relheight 0 -anchor nw

	set Name .history.f3.f17
	frame $Name  -background lightgray -height 50 -highlightbackground lightgray -width 50
	pack $Name -anchor center -expand 1 -fill both -ipadx 0 -ipady 0 -padx 0 -pady 0 -side top

	set Name .history.f3.f17.lb18
	listbox $Name  -background gray92 -foreground black -highlightbackground lightgray -selectbackground lightblue -selectforeground black -xscrollcommand ".history.f3.sb20 set" -yscrollcommand ".history.f3.f17.sb13 set"
	pack $Name -anchor center -expand 1 -fill both -ipadx 0 -ipady 0 -padx 5 -pady 5 -side left

	set Name .history.f3.f17.sb13
	scrollbar $Name  -activebackground gray92 -background lightgray -command ".history.f3.f17.lb18 yview" -highlightbackground lightgray -troughcolor gray92
	pack $Name -anchor center -expand 0 -fill y -ipadx 0 -ipady 0 -padx 5 -pady 5 -side right

	set Name .history.f3.f19
	frame $Name  -background lightgray -height 10 -highlightbackground lightgray -width 15
	pack $Name -anchor se -expand 0 -fill none -ipadx 0 -ipady 0 -padx 5 -pady 5 -side right

	set Name .history.f3.sb20
	scrollbar $Name  -activebackground gray92 -background lightgray -command ".history.f3.f17.lb18 xview" -highlightbackground lightgray -orient horizontal -troughcolor gray92
	pack $Name -anchor center -expand 0 -fill both -ipadx 0 -ipady 0 -padx 5 -pady 5 -side bottom
	
	set Name .history.b2
        button $Name  -text Close -command {catch {wm withdraw .history}}
	place $Name -x 0 -relx 0.3778 -y 0 -rely 0.9111 -width {} -relwidth {} -height {} -relheight {} -anchor nw

	update
	bind .history.f3.f17.lb18 <ButtonRelease-1> "HB_hist_select $win"
	foreach h $_url($win~~history) {
		.history.f3.f17.lb18 insert end $h
	}
	update
}	

#--------
proc HB_hist_select {win} {
	variable _url
	HB_Stop $win
	set win $_url(textwidget)
	catch "lindex [.history.f3.f17.lb18 curselection] 0" _url($win~~histidx)
	catch "wm withdraw .history"
	catch "HB_load_history $win"
}

#----------
proc HB_home {win} {
	variable _url
	HB_Stop $win
	set win $_url(textwidget)
	HB_init $win
	set _url($win~~histidx) [expr [llength $_url($win~~history)] - 1]
	$_url($win~~backbut) configure -state disabled
	update
	HB_load_history $win
}

#----------
proc HB_back {win} {
	variable _url
	HB_Stop $win
	set win $_url(textwidget)
	HB_init $win
	set last [expr [llength $_url($win~~history)] - 1]
	if {$_url($win~~histidx) >= $last} {
		$_url($win~~backbut) configure -state disabled
		update
		return
	}
	incr _url($win~~histidx)
	HB_load_history $win
}

#----------
proc HB_forward {win} {
	variable _url
	HB_Stop $win
	set win $_url(textwidget)
	HB_init $win
	if {$_url($win~~histidx) <= 0} {
		$_url($win~~forwardbut) configure -state disabled
		update
		return
	}
	incr _url($win~~histidx) -1
	HB_load_history $win
}

#----------
proc HB_Stop {win} {
	variable _url 
	set win $_url(textwidget)
    variable HM$win
	upvar #0 HM$win var
	set _url($win~~stopflag) 1
	set var(stop) 1
}

#----------
proc HB_load_history {win} {
	variable _url
	set win $_url(textwidget)
	HB_init $win
	set last [expr [llength $_url($win~~history)] - 1]
	if {$last < 0} {return}
	if {$_url($win~~histidx) < 0} {set _url($win~~histidx) 0}
	if {$_url($win~~histidx) > $last} {set _url($win~~histidx) $last}
	set url [lindex $_url($win~~history) $_url($win~~histidx)]
	set urllist [HB_resolve_url $win $url]
	$_url($win~~entrywidget) delete 0 end
	$_url($win~~entrywidget) insert end $url
	$_url($win~~entrywidget) configure -fg black
	$_url($win~~gobut) configure -state normal
	$_url($win~~homebut) configure -state normal
	if {$_url($win~~histidx) == 0} {
		$_url($win~~forwardbut) configure -state disabled
	} else {
		$_url($win~~forwardbut) configure -state normal
	}
	if {$_url($win~~histidx) < [expr [llength $_url($win~~history)] - 1]} {
		$_url($win~~backbut) configure -state normal
	} else {
		$_url($win~~backbut) configure -state disabled
	}
	update
	HB_load_url $win [lindex $urllist 0] [lindex $urllist 1]
}

#-------------------------------------------------------------------
# Replacement procedures for the html_library.tcl 0.3

# -- called when a link within a document is clicked on
proc XXX_HMlink_callback {win href} {
	# win:   text widget
	# href:  URL to link to
	variable _url
	if {[string index $href 0] == "#"} {
	}
	HB_Stop $win
	set win $_url(textwidget)
	# -- one last bit of tinkering for name only relative links
	set urllist [HB_resolve_url $win $href]
	set url "[lindex $urllist 0][lindex $urllist 2]"
	set tag [lindex $urllist 1]
	# -- load the URL
	variable _url
	set win $_url(textwidget)
	$_url($win~~forwardbut) configure -state disabled
	$_url($win~~backbut) configure -state normal
	$_url($win~~homebut) configure -state normal
	$_url($win~~gobut) configure -state normal
	$_url($win~~statuslabel) configure -text ""
	$_url($win~~entrywidget) delete 0 end
	if {$tag == ""} {
		$_url($win~~entrywidget) insert end $url
	} else {
		$_url($win~~entrywidget) insert end "$url#$tag"
	}
	$_url($win~~entrywidget) configure -fg black
	update
	set _url($win~~histidx) -1
	if {[catch "HB_load_url $win $url $tag"]} {return}
}
	
# -- set up bindings for <Enter>/<Leave> events to a link within a document
proc HMlink_setup {win href} {
	variable _url
	set win $_url(textwidget)
	# -- escape "%" in url so that bind doesn't interpret it
	regsub -all {%} $href {%%} href2
	$win tag bind L:$href <Enter> \
	    "catch [list "LHTML::HB_highlight_link $win $href2"]"
	$win tag bind L:$href <Leave> \
	    "catch [list "LHTML::HB_unhighlight_link $win"]"
}

# -- called to let applet know that a form has been reset 
proc HMreset_form {win form_id} {
	variable _url
	# -- no-op for now
}

# -- called when a font is to be loaded
proc XXX_HMset_font {win tag font} {
	variable _url
	set win $_url(textwidget)
	if {![info exists _url($font)]} {
		set _url($font) 1
		$_url($win~~statuslabel) configure -text "downloading font $font"
		update
	}
	if {[catch {$win tag configure $tag -font $font} fonterr]} {
		$_url($win~~statuslabel) configure -text "$fonterr" -fg red
		after 1000 "$_url($win~~statuslabel) configure -text {} -fg White; update"
	} else {
		$win tag lower $tag mark
		$_url($win~~statuslabel) configure -text {}
		update
	}
}

# -- obtain an image and load it
proc XXX_HMset_image {win handle src} {
	variable _url
	set win $_url(textwidget)
	$_url($win~~statuslabel) configure -text "fetching image $src"
	update
	# -- resolve image relative to current page
	if {$_url($win~~lasturl) != "*"} {
		set url [mergeurls $_url($win~~lasturl) $src]
	} else {
		set url $src
	}
	if {[string first "$url" "[image names]"] != -1} {
		HMgot_image $handle $url
		$_url($win~~statuslabel) configure -text ""
	} else {
		# -- image is remote, fetch it to a local file
		set urllist [parseurl $url]
		if {[lindex $urllist 0] == "http"} {
			set tempfile [HBuniquefile ".gif"]
			catch "fetchURL -url $url -outfile $tempfile"
		} else {
			# -- local file
			set tempfile [lindex $urllist 3]
			# -- check for dos drive specifier and get rid of any leading slash
			set clist [split $tempfile ""]
			if {[string index $tempfile 0] == "/" && \
					[string index $tempfile 2] == ":" && \
					[string index $tempfile 3] == "/"} {
				set tempfile [string range $tempfile 1 end]
			}
		}
		set type "photo"
		if {[file extension $url] == ".bmp"} {set type "bitmap"}
		catch "image create $type $url -file $tempfile" image
		HMgot_image $handle $image
	}
	update
}

# -- form submission callback
proc HMsubmit_form {win form_id param query} {
	variable _url
	# -- no-op for now
}

proc HBuniquefile {{ext ".txt"}} {
	variable _url
	global env
	if {![info exists _url(uniquenum)]} {
		set _url(uniquenum) [pid]
	}
#	incr _url(uniquenum)
	set f [join "SP $_url(uniquenum) $ext" ""]
	set f [eval file join [file split $env(TEMP)] $f]
	return $f
}
	
###############
# end htmlbrow.tcl section
###############


# procedure to install windows into the text widget
# - win:  name of the text widget
# - item: name of widget to install

proc HMwin_install {win item} {
	variable HM$win
	upvar #0 HM$win var
	$win window create $var(S_insert) -window $item -align bottom
	$win tag add indent$var(level) $item
	set focus [expr {[winfo class $item] != "Frame"}]
	$item configure -takefocus $focus
	bind $item <FocusIn> "$win see $item"
}



#============================================================================
# urlutil.tcl --
#        Procedures for parsing and manipulating URLs
#
# parseurl:
#        Breaks a URL into its components, returning these components in
#        a list:
#          index value:
#               0         protocol name (lowercase)
#               1         host name
#               2         port number
#               3         path
#               4         extension/MIME type ("." followed by lowercase)
#               5         tag found after "#"
#               6         key, including "?"
#
# mergeurls:
#         takes two urls and merges their paths.  The first argument
#         should be the current absolute path.  The second argument
#         can be any valid url or relative path.  The result is
#         an absolute url to the second argument.
#
# extracttagkey:
#         given an absolute URL, returns a list of three items:
#             absolute url without tag or key value
#             tag
#             key
#
#---------------------------------------------------------------------------

proc parseurl {url} {
    set protocol ""
    set host ""
    set port ""
    set path ""
    set ext ""
    set tag ""
    set key ""
    set full $url
    set i [string first "://" $url]
    if {$i != -1} {
        # -- parse out protocol
        set protocol [string tolower [string range $url 0 [expr $i - 1]]]
        set url [string range $url [expr $i + 3] end]
        # -- parse out host
        foreach c [split $url ""] {
            if {$c == "/" || $c == ":"} {break}
            append host $c
            set url [string range $url 1 end]
        }
        # -- parse out port number
        if {[string index $url 0] == ":"} {
            set url [string range $url 1 end]
            foreach c [split $url ""] {
                if {$c == "/"} {break}
                append port $c
                set url [string range $url 1 end]
            }
        }
    }
    # -- parse out path
    foreach c [split $url ""] {
        if {$c == "#" || $c == "?"} {break}
        append path $c
        set url [string range $url 1 end]
    }
    # -- parse out extension from path
    for {set i [expr [string length $path] - 1]} {$i>0} {incr i -1} {
        set c [string index $path $i]
        if {$c == "/"} {break}
        if {$c == "."} {
            set ext [string tolower [string range $path $i end]]
            break
        }
    }
    # -- parse out tag
    if {[string index $url 0] == "#"} {
        set url [string range $url 1 end]
        foreach c [split $url ""] {
            if {$c == "?"} {break}
            append tag $c
            set url [string range $url 1 end]
        }
    }
    # -- whatever remains must be a key
    set key $url
    return [list "$protocol" "$host" "$port" "$path" "$ext" "$tag" "$key"]
}

#----------
# -- merge urls into an absolute url
proc mergeurls {u1 u2} {
    set u1l [parseurl $u1]
    set u2l [parseurl $u2]
    set out ""
    # -- handle the case where u2 is absolute
    if {[lindex $u2l 0] != "" || [lindex $u2l 1] != "" || \
            [lindex $u2l 2] != ""} {
        if {[lindex $u2l 0] != ""} {
            append out "[lindex $u2l 0]://"
        }
        append out [lindex $u2l 1]
        if {[lindex $u2l 2] != ""} {
            append out ":[lindex $u2l 2]"
        }
        if {[string index [lindex $u2l 3] 0] != "/"} {
            append out "/"
        }
        append out [lindex $u2l 3]
        if {[lindex $u2l 5] != ""} {
            append out "#[lindex $u2l 5]"
        }
        if {[lindex $u2l 6] != ""} {
            append out [lindex $u2l 6]
        }
        return $out
    }
    # -- build URL prefix from the 1st URL
    if {[lindex $u1l 0] != ""} {
        append out "[lindex $u1l 0]://"
    }
    append out [lindex $u1l 1]
    if {[lindex $u1l 2] != ""} {
        append out ":[lindex $u1l 2]"
    }
    # -- merge the paths
    set p1 [lindex $u1l 3]
    set p2 [lindex $u2l 3]
	if {[string index $u2 0] == "#"} {
		# -- handle special case that relative link is only a tag
		append out [lindex $u1l 3]
		append out "#[lindex $u2l 5]"
		if {[lindex $u2l 6] != ""} {
			append out [lindex $u2l 6]
		}
		return $out
	}
    if {[string index $p1 [expr [string length $p1] - 1]] == "/"} {
        append p1 "xyz"
    }
    if {[string index $p2 0] == "/"} {
        set p1 $p2
    } else {
        set outp ""
        set p1 [split [file dirname $p1] "/"]
        set p1last [expr [llength $p1] - 1]
        set p2 [split $p2 "/"]
        set p2last [expr [llength $p2] - 1]
        foreach item $p2 {
            switch -exact -- $item {
                .. {
                    incr p1last -1
                    if {$p1last < -1} {
                        set p1last -1
                        set p1 ""
                    } else {
                        set p1 [lrange $p1 0 $p1last]
                    }
                }
                . {
                }
                default {
                    incr p1last
                    lappend p1 $item
                }
            }
        }
        if {[llength $p1] > 0 && [lindex $p1 0] != ""} {
            set p1 "{} $p1"
        }
        set p1 [join $p1 "/"]
        regsub -all {//} $p1 "/" p1
        regsub -all {//} $p1 "/" p1
        if {$p1 == ""} {set p1 "/"}
    }
    append out $p1
    if {[lindex $u2l 5] != ""} {
        append out "#[lindex $u2l 5]"
    }
    if {[lindex $u2l 6] != ""} {
        append out [lindex $u2l 6]
    }
    return $out
}

#----------
# -- extract tag and key from absolute url
proc extracttagkey {url} {
	set l [parseurl $url]
	if {[lindex $l 5] == "" && [lindex $l 6] == ""} {
		return [list "$url" "" ""]
	}
	set out ""
    if {[lindex $l 0] != ""} {
        append out "[lindex $l 0]://"
    }
    append out [lindex $l 1]
    if {[lindex $l 2] != ""} {
        append out ":[lindex $l 2]"
    }
	append out [lindex $l 3]
	return [list "$out" "[lindex $l 5]" "[lindex $l 6]"]
}


}
