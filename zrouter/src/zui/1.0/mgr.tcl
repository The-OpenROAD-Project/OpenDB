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

package provide ade::ui 1.0

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
    protected variable zcmd ""
    protected variable zui  ""
    protected variable zconsole  ""
    protected variable OPT 
    protected variable gcmd ""              ;# gui language interpeter
    protected variable zc ""                ;# zui context
    protected variable zhist ""             ;# zui history
    protected variable zhelp ""             ;# zui help
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

        set zhist [ZHistory $this.zhist]
        set zc [ZContext $this.zc]

        set zconsole [ZConsole .zcon#auto -rpc $rpc]
        $zconsole configure -geometry -0-50
        $zconsole Hide

        set zhelp [namespace code [ZHelp .zhelp#auto -title ZUI:Help]]
        $zhelp Hide

        set zui  [namespace eval :: [list  \
            LZUI .zui#auto -rpc $rpc -opts $opts] \
        ]

        $zui Bind <<show-console>> [namespace code $zconsole] UnHide

        set zcmd [namespace eval :: [list \
            ZCMD $this.cmd -loader $loader \
                -zhist $zhist -rpc $rpc -opts $opts -zc $zc -zhelp $zhelp] \
        ]

        set gcmd [GTCL $this.gcmd]

        $loader Bind <<info>>     $zui InfoShow
        $loader Bind <<progress>> $zui InfoShow

        $zui    Bind <<lcmd>>     $gcmd Run 
        $zui    Bind <<quit>>     $this Signal 0
        $zui    Bind <<command>>  $this eUICmd 


        # -- GCMD bindings
        # pass all commands to zcmd
        $gcmd Bind <<command>> $zcmd
        # process results locally (and eventually pass them to viz)
        $gcmd Bind <<result>>  $zui Viz

        # bind cmd result events to zui viz
        $zcmd Bind <<result>> $zui Viz
    }
    destructor {
        $zui  delete
        $zcmd delete
        $zconsole delete
        $gcmd delete
        $zc delete
        $zhist delete
        $zhelp delete
    }
    # -------------------------------------------------- Private Methods ---
    # -------------------------------------------------- CallBacks
    # interpet a UI command
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
