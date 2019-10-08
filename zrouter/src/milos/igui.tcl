#
# BSD 3-Clause License
#
# Copyright (c) 2019, Nefelus Inc
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## igui.tcl (GUI modules)
## System module for Milos
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::igui 0.1


## graphical tree representation
itcl::class  IGUI {

    inherit VOB

    protected method constructor { parent } { 
        VOB::constructor $parent
        set NAME "IGUI CF"
        set SELFNAME "IGUI$this"
        set_state ep_slacks_updated 0
        set_state have_epslacks 0
        ## ONLY for public vars
    }
    
    ###############################################################
    ## GUI STUFF
    ###############################################################

    private variable active 0;
    private common MYNS "::ugui"
    private common MYPF "${MYNS}::"
    private variable nb
    private common MODS { mme opt pdb rpt rte tmg }

    private common MOD_CMDS
    private common SKIP_CMDS {help show_defaults}
    #set MOD_CMDS(pdb) { load_design save_design diff_spef read_spef write_spef needs_extraction needs_routing }
    #set MOD_CMDS(pdb) { load_design save_design diff_spef read_spef write_spef needs_extraction needs_routing }

    private variable last_cmd 

    private variable cs_mode "setup";
    private variable cs_node "main";
    private variable cs_pane "sum"
    private variable cur_gi "gimainsetup";

    private variable ct_sum "";
    private variable ct_hist "";
    private variable ct_path "";
    private variable ct_up "";

    private variable ds_sum 0
    private variable ds_hist 0
    private variable ds_path 0
    private variable ds_up 0

    private variable numticks 20.0;
    private variable numticks2 30.0;
    private variable barsize 100;
    private variable baroffset 50;

    private variable all_nodes {};
    public method show { {window stats} } {
        if { $active == 0 } {

            package require BWidget 
            package require BLT
            namespace eval :: { namespace import ::blt::* }
            namespace eval :: { namespace import -force ::blt::tile::* }

            package require ade::milos::gtree 0.1

            set active 1
            set tnb [tabnotebook .tn]
            pack $tnb

            namespace eval $MYNS { set dummy "" }

            #build_update_nb ""
            build_update_nb $tnb 

            ## build STATS notebook
            build_stats_nb $tnb 


            ### build COMMAND notebook..
            #build_command_nb $tnb $ct

            set_state gui_slacks_callback [itcl::code $this updateSlacks]
            set_state gui_ep_callback [itcl::code $this update_epslacks]

            #pack .
            #puts "DONE!"
            update
        }
    }



    ##################################################################
    ### Update (show slacks) related methods
    ##################################################################

    private method build_update_nb { top } {
        set tab [$top insert end -text "Update"]
        frame $top.uf 
        #pack $top.uf -fill both -expand 1
        pack $top.uf 
        update
        if { [state gui_css] == "" } {
            return
        }
        #puts "BUN: [state gui_css]"
        set ww [build_nodes $top.uf]
        $top tab configure $tab -window $top.uf -windowwidth $ww
    }

    private method build_nodes { top } {

        array set CSS [state gui_css]
        set wwidth 100
        foreach {node name css} $CSS(nodes) {
            set node [string tolower $node]
            #puts "node: $node"
            #puts "name: $name"
            #puts "css: $css"

            set tf [frame $top.$node ]
            Label $tf.title -text $name
            set cmd "table $tf $tf.title 0,0 -cspan 2 "

            set idx 1
            foreach {field label } $css {
                if { $field eq "-" } {
                    if { $label eq "-" } {
                        Label $tf.field$idx 
                    } else {
                        Label $tf.field$idx -text $label -fg white -bg black
                    }
                    append cmd "$idx,0 $tf.field$idx -cspan 2 "
                } else {
                    set ${MYNS}::FIELDS(${node}.$field) ""
                    set ${MYNS}::field ""
                    Label $tf.label$idx -text $label
                    Entry $tf.field$idx -textvariable ${MYNS}::FIELDS(${node}.$field) -width 20
                    append cmd "$idx,0 $tf.label$idx -anchor e $idx,1 $tf.field$idx -anchor w "
                }
                incr idx
            }
            #puts "NC: $cmd"
            ::eval $cmd
            incr wwidth 200
        }

        set cmd "table $top "
        foreach {node row col} $CSS(layout) {
            set node [string tolower $node]
            #append cmd "$row,$col $top.$node -fill both "
            append cmd "$row,$col $top.$node "
        }
        #puts "TC: $cmd"
        ::eval $cmd

        return $wwidth
    }


    ##################################################################
    ### Command (interative milos) related methods
    ##################################################################

    private method build_command_nb { tnb } {
        namespace eval $MYNS { set l1 "Milos Commands:" }
        set tab [$tnb insert end -text "Commands"]
        set f [frame $tnb.f] 
        label $f.l -textvar ${MYPF}l1
        pack $f.l

        $tnb tab configure $tab -window $f -windowwidth 800

        set nb [tabnotebook $tnb.f.nb]

        set idx 0
        foreach mod $MODS {
            set pane [$nb insert end -text $mod -windowwidth 800]

            build_tab $nb $idx $mod

            incr idx
        }
        pack $tnb.f.nb -fill both -expand 1

        $nb select 4
        $tnb.f.nb configure -tearoff yes
    }

    private method exe_cmd { mod } {
        puts "Try to run command: $mod, $last_cmd($mod)!"
        exec_gui_cmd $mod $last_cmd($mod)
    }

    private method build_tab { pane idx mod } {

        #set top [frame $pane.all$idx]
        #set top [ScrollableFrame $pane.all$idx]
        set top [ScrolledWindow $pane.all$idx -scrollbar both -auto both]

        label $top.text -text "MODULE [string toupper $mod]"
        button $top.exe -text "Run Command" -command [itcl::code $this exe_cmd $mod]
        #scrollbar $top.vs -orient vertical -command "$top yview"
        #scrollbar $top.hs -orient horizontal -command "$top xview"


        frame $top.f1
        frame $top.f2
        #ScrollableFrame $top.f1
        #ScrollableFrame $top.f2
        #ScrolledWindow $top.f1 -auto both
        #ScrolledWindow $top.f2 -auto both

        table $top \
            0,0 $top.text -anchor w -fill x\
            0,1 $top.exe -anchor e \
            1,0 $top.f1 -fill x -anchor n\
            1,1 $top.f2 -fill both  

        table configure $top r0 -height 0.5i -resize none


        set cmdx [build_cmds $top $mod]

        $pane tab configure $idx -window $top -anchor nw

        puts "cmdx: $cmdx"
        eval $cmdx
    }


    private method build_cmds { pane mod } {
        set cmdx ""
        set bn 0
        set top $pane.f1
        set ${MYNS}::rb_$mod 0
        foreach cmd [public_commands $mod] {
            #puts "cmd: $cmd"
            if { $cmd eq "help" } { continue }
            radiobutton $top.b$bn -value $bn -variable ${MYNS}::rb_$mod -text $cmd -command [itcl::code $this get_cmd $pane $mod $cmd]
            if { $bn == 0 } {
                set cmdx "$this get_cmd $pane $mod $cmd"
            }
            
            #puts "table $top $bn,0 $top.b$bn"
            table $top \
                $bn,0 $top.b$bn -anchor w 
               
            incr bn
        }
        return $cmdx
    }

    private method get_cmd { pane mod cmd } {
        #puts "get_cmd: $mod $cmd"
        set last_cmd($mod) $cmd
        gui_help [string toupper $mod] $cmd
        set cidx "[string toupper $mod]_[string toupper $cmd]"
        #parray GUI_PARMS
        set top $pane.f2
        destroy $top
        #frame $top
        ScrollableFrame $top
        if { ![info exists GUI_PARMS($cidx,name0)] } {
            label $top.text -text "Command $cmd takes no parameters."
            table $top 0,0 $top.text
        } else {
            build_form $top $cidx
        }
        table $pane 1,1 $top -anchor n
    }

    private method exec_gui_cmd { mod cmd } {
        set idx "[string toupper $mod]_[string toupper $cmd]"
        set cmd "$mod $cmd"
        if { [info exists GUI_PARMS($idx,name0)] } {
            set max [llength [array names GUI_PARMS ${idx},name*] ]


            for { set i 0 } { $i < $max } { incr i } {
                foreach arg { name type required default desc } {
                    set $arg $GUI_PARMS($idx,${arg}${i})
                    set varname "${MYPF}$idx,default$i"
                }
                set val [set $varname]
                if { [set $varname] != "" } {
                    append cmd " -$name [set $varname]"
                }
            }
        }
        #puts "$cmd"
        set rc [call_milos $cmd]
        notice "$rc"
        notice "milos> "
    }

    private method build_form { top idx } {
        set max [llength [array names GUI_PARMS ${idx},name*] ]


        #puts "BF: $top"
        ## loop through each parameter
        for { set i 0 } { $i < $max } { incr i } {
            foreach arg { name type required default desc } {
                set $arg $GUI_PARMS($idx,${arg}${i})
                #puts "results: $arg - [set $arg]"
            }
            #puts "type: $type"
            #puts "Entry!"
            Label $top.lab$name -text "${name}:" -helptext $desc
            set varname "${MYPF}$idx,default$i"
            if { ![info exists $varname] } {
                set $varname $default
            }
            #puts "varname: $varname [set $varname]"
            switch -glob $type {
                *file -
                *list -
                num -
                *str { 
                    Entry $top.$name -helptext $GUI_PARMS($idx,desc$i)  -textvariable $varname
                }
                bool {
                    checkbutton $top.$name -variable $varname
                }

            }
            #puts "BF: $top, $top.lab$name, $top.$name"
            table $top \
                $i,0 $top.lab$name -anchor w \
                $i,1 $top.$name -anchor w
        }
    }

    ##################################################################
    ### Stats (detailed info) related methods
    ##################################################################

    private variable stree
    private method build_stats_nb { top } {
        set tab [$top insert end -text "Stats"]
        frame $top.sf
        $top tab configure $tab -window $top.sf -windowwidth 1000



        frame $top.sf.lf
        frame $top.sf.rf

        set stree [GTree #auto $top.sf.lf]
        get_navi_nodes 

        build_stats_subnb $top.sf.rf

        pack $top.sf.lf -side left -anchor n -expand 1 -fill y
        pack $top.sf.rf -side left -anchor nw

    }

    private method get_navi_nodes { } {
        ## in theory, loop through defined nodes here
        $stree insert "main" "main"
        $stree insert "main Setup" "main_setup"
        $stree insert "main Hold" "main_hold"
        lappend all_nodes "main_setup"
        lappend all_nodes "main_hold"

        if { [state multi_remotes] != "" } {
            foreach [state MSA] [state multi_scenarios] {
                $stree insert $name $name
                $stree insert "$name Setup" "${name}_setup"
                $stree insert "$name Hold" "${name}_hold"
                lappend all_nodes "${name}_setup"
                lappend all_nodes "${name}_hold"
            }
        }
        $stree bind [itcl::code $this select_node] 0
    }

    private method select_node { id name tag } {
        #puts "Selected: $name, $id, $tag"
        foreach {node mode} $name { break; }
        if { $mode == "" } {
            return;
        }
        if { ([string tolower $mode] eq $cs_mode)  && $node eq $cs_mode } {
            ## nothing has changed
            return
        }
        set cs_mode [string tolower $mode]
        set cs_node $node
        set cur_gi "gi$cs_node$cs_mode"

        build_stats_$cs_pane [set ct_$cs_pane]

        ## to update tree selection, just in case..
        populate_sum_gtab
    }


    private method select_stats_pane { func } {
        #puts "Selected tab $func"
        set cs_pane $func
        build_stats_$cs_pane [set ct_$cs_pane]
    }

    private method build_stats_subnb { top } {
        set snb [tabnotebook $top.sn]
        pack $snb -side top -fill both -expand 1

        foreach {lab func} { Summary sum Path path Histogram hist Update up } {
            set tab [$snb insert end -text $lab -windowwidth 1000 -windowheight 5i]
            frame $top.sn.$func 
            pack $top.sn.$func -fill both -expand 1
            $snb tab configure $tab -window $top.sn.$func  -windowwidth 1000
            $snb tab configure -fill both
            $snb bind $tab <Button-1> [itcl::code $this select_stats_pane $func]
            build_stats_$func $top.sn.$func

            set ct_$func $top.sn.$func
        }
    }

    private method check_update { } {
        set_title
        if { [state multi] eq "OFF" } {
            update_epslacks 1
            set_title
        }
    }
    private method set_title { } {
        if { [set ${MYNS}::req_up] == 1 } {
            wm title . "milos (update requested)"
        } else {
            wm title . "milos"
        }
    }

    private method build_stats_up { top } {
        if { $ds_up == 0 } {
            frame $top.f -width 6i -height 6i
            checkbutton $top.f.cb -text "update" -variable ${MYNS}::req_up \
                -command [itcl::code $this check_update]
            set ${MYNS}::req_up 0
            pack $top.f
            pack $top.f.cb
            set_title
        }
        set ds_up 1
    }

    private method build_stats_sum { top } {
        if { $ds_sum == 0 } {
            #puts "H: $top"
            frame $top.lf -width 4i -height 4i
            frame $top.rf -width 3i -height 6i
            pack $top.lf -side left -anchor w -fill y -expand 1
            pack $top.rf -side left -anchor w 
        }

        build_sum_text $top.lf
        build_sum_graph $top.rf

        set ds_sum 1
    }

    private variable myslacks;

    private method updateSlacks { } {
        #puts "updateSlacks!"
        if { $ds_sum == 1 } {
            #puts "rebuild updateSlacks!"
            build_stats_$cs_pane [set ct_$cs_pane]
        }
    }

    private method getSlacks { } {
        set st "${cs_node}_${cs_mode}_slack"
        #puts "gs: $st"
        #puts "gs2: [state $st]"
        if { [state $st] != "" } {
            set rval ""
            append rval [state $st]
            set st "${cs_node}_${cs_mode}_tslack"
            append rval " [state $st]"
            set st "${cs_node}_${cs_mode}_viol"
            append rval " [state $st]"
            #puts "gs: $rval"
            return $rval
        }
        #puts "gs: none!"
        return "-99999 -999999 -99999"
    }

    private method build_sum_text { top } {
        if { $ds_sum == 0 } {
            label $top.tl -text "Timing Summary"
            pack $top.tl -side top
        }

        foreach widget [list $top.sl $top.tns $top.fn] {
            if { [info command $widget] != "" } {
                destroy $widget
            }
        }

        #puts "bst 1"
        if { [state all_slacks] == "" } {
            return
        }
        #puts "bst 2"
        foreach { sl tns viol } [getSlacks] { break; }
        #puts "bst 2: $sl $tns $viol"

        if { $sl < 0 } {
            radiobutton $top.sl -text "Your design fails $cs_mode timing. The worst slack is $sl."
            radiobutton $top.tns -text "Total negative slack is $tns."
            radiobutton $top.fn -text "There are $viol failing endpoint(s)."
            pack $top.sl $top.tns $top.fn -side top -anchor w
        } else {
            radiobutton $top.sl -selectcolor seagreen -text "Your design meets timing. The worst slack is $sl."
            pack $top.sl -side top -anchor w
            $top.sl invoke
        }
    }

    private variable ghist;
    private variable ghist2;
    private variable gtab;
    private variable gtab2;
    private variable gtrees;
    private variable num_gts 0;
    private variable gvecs;
    private variable num_gvc 0;


        
    private method build_sum_graph { top } {
        if { $ds_sum == 0 } {
            frame $top.tf
            frame $top.bf
            pack $top.tf $top.bf -side top -expand 1 -fill both

            build_sum_ghist $top.tf
            build_sum_gtab $top.bf
        }

        populate_sum_ghist
        populate_sum_gtab
    }


    private method get_epslacks { } {
        foreach cmode { setup hold } {
            if { $cmode eq "setup" } {
                set mode "max"
            } else {
                set mode "min"
            }

            #puts "Get EP: $mode"
            ## send command to remotes
            foreach [state MSA] [state multi_scenarios] {
                [state ns]::$node send_command acmd "$::apps::milos::mycf get_epslacks $mode"
            }

            ## get values from main
            set rl [$::apps::milos::mycf get_epslacks $mode]
            foreach {len nl sl} $rl { break; }
            set_state main_${cmode}_epslacks [join $sl]
            set_state main_${cmode}_epnames [join $nl]
            set_state main_${cmode}_eplen $len
            
            ## now process remotes
            sync_nodes 0.2
            foreach [state MSA] [state multi_scenarios] {
                foreach {len nl sl} [[state ns]::$node rc] { break; }
                set_state ${name}_${cmode}_epslacks [join $sl]
                set_state ${name}_${cmode}_epnames [join $nl]
                set_state ${name}_${cmode}_eplen $len
            }
            set_state have_epslacks 1
        }
    }


    private method update_epslacks { {force 0} } {
        if { [state tmg_rcmodels] == 0 } {
            ## nothing we can do
            return
        }
        if { ([state ep_slacks_updated] == 1 && [set ${MYNS}::req_up] == 1) || $force == 1 } {
            #puts "get EP!"
            get_epslacks
            populate_sum_ghist
            populate_sum_gtab
            #get_cpath
            set_state ep_slacks_updated 0
            set ${MYNS}::req_up 0
            set_title
        }
    }


    private method select_sum_ghist { x y } {
        $ghist element closest $x $y info
        #parray info
        set sslack [expr $info(x) - $baroffset]
        set gi [state "gi$cs_node$cs_mode"]
        set idx -1
        foreach {ix val} $gi {
            if { $ix == $info(x) } {
                set idx $val
                break;
            }
        }
        $gtab see $idx
    }

    private method select_hist_ghist { x y } {
        $ghist2 element closest $x $y info
        #parray info
        set sslack [expr $info(x) - $baroffset]
        set gi [state "gi$cs_node$cs_mode"]
        set idx -1
        foreach {ix val} $gi {
            if { $ix == $info(x) } {
                set idx $val
                break;
            }
        }
        $gtab2 see $idx
    }

    private method build_sum_ghist { top } {
        set ghist [barchart $top.g]
        pack $ghist -fill both -expand 1
        $ghist legend configure -hide 1
        $ghist configure -title "Endpoint Slack Distribution"
        $ghist configure -width 2i -height 3i
        $ghist xaxis configure -rotate 90
        $ghist xaxis configure -title "Slack Time \[ps]"
        $ghist yaxis configure -title "Num. of Endpoints"
        $ghist element bind all <ButtonPress-2> \
                "[itcl::code $this select_sum_ghist] %x %y"
        Blt_ZoomStack $ghist
    }

    private method build_hist_ghist { top } { 
        set ghist2 [barchart $top.g]
        pack $ghist2 -fill both -expand 1
        $ghist2 legend configure -hide 1
        $ghist2 configure -title "Endpoint Slack Distribution"
        $ghist2 configure -width 6i -height 3i
        $ghist2 xaxis configure -rotate 90
        $ghist2 xaxis configure -title "Slack Time \[ps]"
        $ghist2 yaxis configure -title "Num. of Endpoints"
        $ghist2 element bind all <ButtonPress-2> \
                "[itcl::code $this select_hist_ghist] %x %y"
        Blt_ZoomStack $ghist2
    }


    private method calc_ghist_data { } {
        #puts "cghd 1"
        #puts "cghd 2"
        set xn "xn$cs_node$cs_mode" 
        set xp "xp$cs_node$cs_mode" 
        set yn "yn$cs_node$cs_mode" 
        set yp "yp$cs_node$cs_mode" 

        #puts "start cgd"
        ## vectors must already exist
        $xn notify never
        $xp notify never
        $yn notify never
        $yp notify never
        $xn set { }
        $xp set { }
        $yn set { }
        $yp set { }
        #puts "cghd 3"

        set len [state ${cs_node}_${cs_mode}_eplen]
        if { $len == 0 } {
            return
        }
        #puts "cghd 4"

        set sl [state ${cs_node}_${cs_mode}_epslacks]
        vector tmp
        tmp set { }
        foreach { slack id } $sl {
            set it [expr {int($slack)}]
            if { $slack < 0 } {
                incr it -1
            }
            tmp append $it
        }
        #puts "cghd 5"

        ## tmp has slack values now
        
        set ticks [expr {round(($tmp(max) - $tmp(min)) / $numticks)} ]
        set ticks2 [expr {round(($tmp(max) - $tmp(min)) / $numticks2)} ]
        set_state ${cs_node}_${cs_mode}_ticks $ticks
        set_state ${cs_node}_${cs_mode}_ticks2 $ticks2
        set minx [ expr {floor($tmp(min) / $barsize) * $barsize}] 
        set maxx [ expr {ceil($tmp(max) / $barsize) * $barsize}] 

        tmp sort

        ## offset 1 - first element in tree has index 1
        set sum 1

        set start $minx
        set end [expr {$start + [expr $barsize - 0.1]}]

        array set gidx {}
        while { $start <= $maxx } {
            set vl [tmp search $start $end]
            set idx [expr { $start + $baroffset}] 
            set len [llength $vl]
            if { $end <= 0.0 } {
                $xn append $idx
                $yn append $len
            } else {
                $xp append $idx
                $yp append $len
            }
            set gidx($idx) $sum
            incr sum $len
            
            set start [expr $start + $barsize]
            set end [expr {$start + [expr $barsize - 0.1]}]
        }
        #puts "cghd 8"
        set_state "gi$cs_node$cs_mode" [array get gidx]
        #puts "Test: gi$cs_node$cs_mode [state gi$cs_node$cs_mode]"
        $xn notify now
        $xp notify now
        $yn notify now
        $yp notify now
        if { 0 } {
        if { [$xn length] > 0 } {
            puts "X neg ticks: [$xn range 0 end]"
            puts "Y neg values: [$yn range 0 end]"
        }
        if { [$xp length] > 0 } {
            puts "X pos ticks: [$xp range 0 end]"
            puts "Y pos values: [$yp range 0 end]"
        }
        }
    }



    private method populate_sum_ghist { } {
        #puts "psg 0"
        if { [state have_epslacks] == 0 } { return }

        #
        set xn "xn$cs_node$cs_mode" 
        set xp "xp$cs_node$cs_mode" 
        set yn "yn$cs_node$cs_mode" 
        set yp "yp$cs_node$cs_mode" 
        #puts "psg 1"
        if { ![info exists gvecs($cs_node,$cs_mode)] } {
            #puts "psg 1a"
            ## first time!
            set gvecs($cs_node,$cs_mode) 1
            vector $xn
            vector $xp
            vector $yn
            vector $yp
            #puts "psg 1a1"

            calc_ghist_data 

            $ghist element create $yn -xdata $xn -ydata $yn
            $ghist element create $yp -xdata $xp -ydata $yp
            $ghist element configure $yn -fg red
            $ghist element configure $yp -fg green
            $ghist configure -barwidth $barsize

            $ghist2 element create $yn -xdata $xn -ydata $yn
            $ghist2 element create $yp -xdata $xp -ydata $yp
            $ghist2 element configure $yn -fg red
            $ghist2 element configure $yp -fg green
            $ghist2 configure -barwidth $barsize

        } elseif { [state ep_slacks_updated] == 1 } {
            ## need to update axes, etc
            calc_ghist_data
        }
        $ghist xaxis configure -stepsize [state ${cs_node}_${cs_mode}_ticks]
        $ghist2 xaxis configure -stepsize [state ${cs_node}_${cs_mode}_ticks2]
        $ghist element show "$yn $yp"
        $ghist2 element show "$yn $yp"
    }

    private method build_sum_gtab { top } {
        #puts "BSG!"
        label $top.l -text "Worst Path Endpoints"
        pack $top.l -side top
        set gtab [GTree #auto $top]
        $gtab title "end point"
        $gtab addCol slack slack
        $gtab showCol slack
        $gtab addCol id id
        $gtab setSize 3i 2i
        $gtab separator " "
    }
    private method build_hist_gtab { top } {
        #puts "BHG!"
        label $top.l -text "Worst Path Endpoints"
        pack $top.l -side top
        set gtab2 [GTree #auto $top]
        $gtab2 share_tree ::IGUI::$gtab 1
        $gtab2 title "end point"
        $gtab2 addCol slack slack
        $gtab2 showCol slack
        $gtab2 setSize 6i 3i
        $gtab2 separator " "
        $gtab2 addCol sp "start point" 0
        $gtab2 addCol misc "random data"
        $gtab2 showCol sp
    }

    ## build list for names/slacks

    private method calc_gtab_data { } {

        set len [state ${cs_node}_${cs_mode}_eplen]
        if { $len == 0 } {
            $gtab clear
            return
        }

        $gtab clear
        #puts "start cgtab"
        set sl [state ${cs_node}_${cs_mode}_epslacks]
        set nl [state ${cs_node}_${cs_mode}_epnames]

        set cl {}
        for { set i 0 } { $i < $len } { incr i } {
            set j [expr $i*2]
            set name [lindex $nl $i]
            set slack [lindex $sl $j]
            incr j 1
            set id [lindex $sl $j]
            lappend cl [list $name $slack $id]
        }

        set cl [lsort -real -index 1 $cl]
        foreach arg $cl {
            foreach { name slack id } $arg {
                #puts "name: $name, slack: $slack, id: $id"
                $gtab insert $name $name "slack $slack id $id"
            }
        }
        #not currently needed
        #set_state ${cs_node}_${cs_mode}_cl $cl
        get_cpath
        #puts "end cgtab"

    }

    private method get_cpath { } {
        return
        puts "Get CP!"

        set idx 1
        foreach arg [state ${cs_node}_${cs_mode}_cl] {
            $gtab2 select_tree $gtrees(${cs_node}_${cs_mode})
            foreach { name slack id } $arg {
                set nl [split $name "/"]
                set rl [$::apps::milos::mycf get_cpath [lindex $nl 0]]
                #puts "rl: $rl"
                foreach a2 $rl {
                    foreach { nm2 model } $a2 {
                        #puts "[lindex $nl 0], nm2: $nm2, idx: $idx"
                        $gtab2 setData $idx "sp $nm2"
                        break;
                    }
                    break;
                }
                $gtab2 setData $idx "misc $rl"
            }
            incr idx
        }
    }

    private method populate_sum_gtab { } {
        ## get worst data later
        if { [state have_epslacks] == 0 } { return }

        if { ![info exists gtrees(${cs_node}_${cs_mode})] } {
            set nt 0
            if { $num_gts != 0 } {
                set nt [$gtab new_tree]
            }
            set gtrees(${cs_node}_${cs_mode}) $nt
            calc_gtab_data
            incr num_gts
        } elseif { [state ep_slacks_updated] == 1 } { 
            calc_gtab_data
        }
        #puts "PSG!"
        $gtab select_tree $gtrees(${cs_node}_${cs_mode})
    }

    private method populate_hist_gtab { } {
        ## get worst data later

        if { [state have_epslacks] == 0 } { return }

        #calc_gtab2_data
        #puts "PHG!"
        if { ![info exists gtrees(${cs_node}_${cs_mode})] } {
            populate_sum_gtab
        }
        #$gtab2 select_tree $gtrees(${cs_node}_${cs_mode})
    }




    private method build_stats_hist { top } {
        if { $ds_hist == 0 } {
            #puts "H: $top"
            frame $top.tf -width 6i -height 3i
            frame $top.bf -width 6i -height 3i
            pack $top.tf -side top -anchor w -fill y -expand 1
            pack $top.bf -side top -anchor w 

            build_hist_ghist $top.tf
            build_hist_gtab $top.bf
            set ds_hist 1
        }
        populate_sum_ghist

    }

    ########################################################
    # Methods for the PATH sub-panel
    ########################################################

    private method build_stats_path { top } {
        if { $ds_path == 0 } {
            #puts "H: $top"
            frame $top.lf -width 1i -height 4i
            frame $top.rf -width 7i -height 4i
            pack $top.lf -side left -anchor w 
            pack $top.rf -side left -anchor w -fill both -expand 1

            build_path_gnav $top.lf
            build_path_gtab $top.rf
        }

        set ds_path 1
    }

    private variable gpnav;
    private variable gptab;

    private method build_path_gnav { top } {
        set gpnav [GTree #auto $top]
        $gpnav share_tree ::IGUI::$gtab 1
        $gpnav title "Paths Found"
        $gpnav addCol slack slack
        $gpnav showCol slack
        $gpnav hideCol -1
        $gpnav setSize 1i 4i
        $gpnav separator " "
    }

    private method build_path_gtab { top } {
        set gptab [GTree #auto $top]
        $gptab title "pin name"
        $gptab addCol model "model name"
        $gptab addCol delay delay
        $gptab addCol at AT
        $gptab addCol edge edge
        $gptab addCol slack slack
        $gptab showCol model delay at edge slack
    }




}




