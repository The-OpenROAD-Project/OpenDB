package provide mtlib 1.0
#
#       Class: LShortMessage
# Description: Send a short message using email
#
itcl::class LShortMessage {
    inherit LTopLevelEditor LInteractor
    # -------------------------------------------------- Public Variables --
    public variable to ""       ;# recepient
    public variable subject  "" ;# subject
    public variable cc       "" ;# subject
    public variable body     "" ;# body
    public variable server   "localhost" ;# smtp server
    # -------------------------------------------------- Public Methods ----
    public method Send {} {
        msg_send
    }
    public method Attach {tag type name value} {
        form_attach $tag $type $name $value
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable FORM
    protected variable FIELD
    protected variable wh
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set wh [frame $This.header]
        pack $wh -side top -expand 1 -fill both

        Attach to param To $to
        Attach cc param Cc $cc
        Attach subject param Subject $subject
        Attach body text Body $body
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    protected method form_attach {tag type name value} {
        set N [array size FORM]
        set w1 $wh.l$N 
        set w2 $wh.fd$N 
        set FORM($tag) "$value"
        set FIELD($tag,type) $type
        set FIELD($tag,name) $name
        set FIELD($tag,win) $w2
        label $w1 -text $name: -justify right
        switch $type {
            param {
                entry $w2 -border 1 -relief sunken -bg gray70
                bind $w2 <Enter>           "focus $w2"
                bind $w2 <KeyPress-Return> "$this eSet $tag"
                bind $w2 <Leave>           "$this eSet $tag"
                $w2 insert 0 $value
                $this eSet $tag
            }
            text {
                text $w2 -border 1 \
                    -relief sunken -bg gray70 -width 40 -height 10
                bind $w2 <Enter>           "focus $w2"
                bind $w2 <Leave>           "$this eSet $tag"
                $w2 insert 1.0 $value
                $this eSet $tag
            }
            image {
                label $w2 -text $name -border 1 -relief sunken
                bind $w2 <1> "$this eShowImage $tag"
                $this eSet $tag
            }
        }
        grid $w1 $w2 -sticky ew
    }

    # send the message using smtp
    protected method msg_send {} {
        set m [LMimeMessage $this.msg#auto -server $server]
        $m configure -to $FORM(to) -cc $FORM(cc) -subject $FORM(subject)
        $m configure -body $FORM(body)
        foreach tag [array names FORM] {
            switch $tag {
                to - cc - subject - body { continue } 
            }
            $m Attach $tag $FIELD($tag,type) $FIELD($tag,name) $FORM($tag)
        }
        $m Send
        $m delete
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method vMakeButtons {w} {
        button_add $w.q -text Cancel -command "$this eCancel"
        button_add $w.s -text Send   -command "$this eSend"
        set w
    }
    public method eCancel {} {
        $this Signal 0
    }
    public method eSend {} {
        Send
        $this Signal 0
    }
    public method eSet {tag} {
        set val $FORM($tag)
        switch $FIELD($tag,type) {
            param  { set val [string trim [$FIELD($tag,win) get]] }
            text   { set val [string trim [$FIELD($tag,win) get 1.0 end]] }
        }
        set FORM($tag) $val
    }
    public method eShowImage {tag} {
        set im $FORM($tag)
        set sel [LSelectImage $this.img -image $im]
        if { [$sel Wait ] } {
            set FORM($tag) [$sel Get]
            puts img=[$sel Get]
        }
        $sel delete
    }
    # -------------------------------------------------- Common Initialization
}

package require Img

#
#       Class: LSelectImage
# Description:
#
itcl::class LSelectImage {
    inherit LTopLevelEditor LInteractor
    # -------------------------------------------------- Public Variables --
    public variable image ""
    public variable width 500
    public variable height 500
    # -------------------------------------------------- Public Methods ----
    public method Get {} {
puts status=$status,image=$image
        if { $status } { return $image }
        return ""
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable wim
    protected variable img ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set wim [label $This.im]
        pack $wim -side top -expand 1 -fill both
        winconfig -width $width -height $height
        image_show $image
    }
    destructor {
        if {$img != "" } { image delete $img }
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method image_show {im} {
        $wim configure -image "" -text "Loading Image" 
        $this configure -title "image: $im"
        update idletasks
        if { $img != "" } { 
            image delete $img
        }
        # is this alreay an internal image?
        if ![catch {image type $im}] {
            set img [image create photo]
            LImageMeta::RFit $im $img 300 300
            $wim configure -image $img
            return
        }
        if { $image != "" } {
            set im  [image create photo -file $image]
            set img [image create photo]
            LImageMeta::RFit $im $img $width $height
            #LImageMeta::Fit $im $img $width $height
            image delete $im
            $wim configure -image $img
        }
    }
    # ----------------------------------------------- CallBacks
    public method vMakeButtons {w} {
        button_add $w.c -text "Select" -command "$this Signal 1"
        button_add $w.q -text "Cancel" -command "$this Signal 0"
    }
    # -------------------------------------------------- Common Initialization
}

package require mime
package require smtp

#       Class: LMimeMessage
# Description:
#
itcl::class LMimeMessage {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable body ""      { }
    public variable to   ""      { }
    public variable cc   ""      { }
    public variable subject   "" { }
    public variable server "localhost"
    public variable queue 1                         ;# should we queue msg?
    # -------------------------------------------------- Public Methods ----
    public method Send {} {
        set rec $to

        set mp [mime::initialize -canonical text/plain -string $body ]
        set PARTS [concat $mp $PARTS]

        set token [mime::initialize -canonical "multipart/mixed" \
            -parts $PARTS]
        mime::setheader $token subject $subject
        smtp::sendmessage $token \
            -recipients "$rec" -servers $server -queue $queue
        mime::finalize $token
    }
    public method Attach {tag type name value} {
        switch -glob $type {
            image {
                set at [format {{attachment; filename="%s.jpg"}} $tag]
                set p [mime::initialize -canonical image/jpeg -file $value]
                mime::setheader $p  Content-Disposition $at -mode append
            }
            text - param {
                set p [mime::initialize -canonical text/plain -string $value]
            }
            default {
                puts "warning: $tag:$type not supported"
                return
            }
        }
        lappend PARTS $p
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable token ""
    protected variable PARTS ""
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
