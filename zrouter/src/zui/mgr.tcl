package provide ade::ui 2.0

#
#       Class: ZMGR
# Description: ZUI controller
#      Author: Manolis M. Tsangaris
#
# Manage the ZUI/ZCMD interactions
itcl::class ZMGR {
    inherit LObject LInteractor
    # -------------------------------------------------- Public Variables --
    public variable loader ""        ; # the cad object loader
    public variable rpc    ""        ; # remote server to connect to
    public variable opts    ""  {     ; # various parameters from the argv
            array set OPT $opts
    }     
    # -------------------------------------------------- Public Methods ----
    public method Run {cmd} {
        eval eUICmd $cmd
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable zcmd ""              ;# gui controller
    protected variable zui  ""
    protected variable zconsole  ""
    protected variable OPT 
    protected variable gcmd ""              ;# gui language interpeter
    protected variable zc ""                ;# zui context
    protected variable zhist ""             ;# zui history
    protected variable zhelp ""             ;# zui help
    protected variable dsmgr ""             ;# data set manager
    protected variable bmgr  ""             ;# bookmark manager
    protected variable bhelp ""             ;# balloon help
    protected variable amgr  ""             ;# adapter manager
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

        set zhist [ZHistory $this.zhist]

        set zconsole [ZConsole .zcon#auto -rpc $rpc]
        $zconsole configure -geometry -0-50
        $zconsole Hide

	set bhelp [LBalloonHelp .bhelp]
	set bhelp [namespace code $bhelp]

        set zhelp [namespace code [ZHelp .zhelp#auto -title "ADE/UI:Help"]]
        $zhelp Hide

	set bmgr [BookMarkMGR $this.bmgr]

        set zui  [namespace eval :: [list  \
            LZUI .zui#auto -rpc $rpc -opts $opts -bhelp $bhelp]
        ]
	if { $OPT(tk) } {
	    set dstype list
	} else {
	    set dstype qtlist
	}
	set dsmgr [ZCDSFactory $this.dsmgr -type $dstype \
	    -loader $loader -debug 0]

        $zui Bind <<show-console>> [namespace code $zconsole] UnHide

	set amgr [namespace eval :: AdapterManager $this.amgr]

        set zcmd [namespace eval :: [list \
            ZCMD $this.cmd -loader $loader -dsmgr $dsmgr -bmgr $bmgr\
                -zhist $zhist -rpc $rpc -opts $opts -zhelp $zhelp] \
        ]
	$zcmd Bind <<adapter-event>> $amgr Handle

        set gcmd [GTCL $this.gcmd]

        $loader Bind <<stats>>    $zui eLoaderStats

        $zui    Bind <<user-command>>     $gcmd Run 
        $zui    Bind <<quit>>             $this Signal 0
        $zui    Bind <<command>>          $this eUICmd 

	# -- pass data set caching events, used to update the overview
	$dsmgr Bind <<dataset-updated>> $zui eDataSetUpdated
	$dsmgr Bind <<dataset-cache>>   $zui eDataSetCache

        # -- GCMD bindings
        # -- pass all commands to zcmd run method
        $gcmd Bind <<command>> $zcmd

        # -- process results locally (and eventually pass them to viz)
        $gcmd Bind <<result>>  $zui Viz

        # bind cmd result events to zui viz
        $zcmd Bind <<result>> $zui Viz
    }
    destructor {
        $zui  delete
        $zcmd delete
        $zconsole delete
        $gcmd delete
        $zhist delete
        $zhelp delete
        $dsmgr delete
        $bmgr delete
	$bhelp delete
	$amgr delete
    }
    # -------------------------------------------------- Private Methods ---
    # -------------------------------------------------- CallBacks
    # -- interpet a UI command coming from the user window or the notepad
    public method eUICmd {cmd args} {
        switch $cmd {
            quit { Signal 0 }
            default {
                eval $zcmd $cmd $args
            }
        }
    }
    # -------------------------------------------------- Common Initialization
}
