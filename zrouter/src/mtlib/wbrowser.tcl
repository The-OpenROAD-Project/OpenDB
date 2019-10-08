package require Itcl
package provide mtlib 1.0

#       Class: LWBrowser
# Description: Simple Web Browser
#
itcl::class LWBrowser {
    inherit LTopLevelEditor
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Show {url} {
        wb_show $url
    }
    public method ShowPage {page} {
        wb_show_page $page
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable wpage
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set wpage [LWPage $This.page]
        pack $wpage -side top -expand 1 -fill both
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method wb_show {url} {
        set page [exec cat $url]
        $wpage RenderText $page
    }
    protected method wb_show_page {page} {
        $wpage Render $page
    }
    # ----------------------------------------------- CallBacks
    public method vMakeButtons {w} {
        button_add_right $w.hide -text "Hide"     -command "$this Hide"
        set w
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: LWPage
# Description: Simple Web Page Renderer
#
itcl::class LWPage {
    inherit LFrame LBinder
    # Event <<image>> url               ;# request an inline image
    # Event <<link>> text url           ;# user selected a link
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Render {html} {
        wp_render_html $html
    }
    public method RenderText {page} {
        wp_render $page
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable wtext             ;# text widget
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set wtext [LScrollableWidget $This.ftxt text -width 100 -height 40] 
	pack $This.ftxt -side top -expand 1 -fill both
        LHTML::HMinit_win $wtext
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method wp_render {page} {
        $wtext delete 0.0 end
        $wtext insert 0.0 $page
    }
    protected method wp_render_html {html} {
        LHTML::HMreset_win $wtext
        LHTML::HMparse_html $html "HMrender $wtext"
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
