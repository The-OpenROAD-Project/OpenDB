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

package provide ade::help 1.0

namespace eval apps { 
    proc mod_init { } {
        variable MOD_DESC
        set MOD_DESC(ext) "Extraction Commands"
        set MOD_DESC(pdb) "Physical/DB Commands: load, save, export"
        set MOD_DESC(opt) "Optimization Commands"
        set MOD_DESC(tmg) "Timing Commands"
        set MOD_DESC(clk) "Clockworks Commands"
        set MOD_DESC(rte) "Routing Commands"
        set MOD_DESC(rpt) "Reporting Commands"
        set MOD_DESC(mme) "Multimode Commands"
        set MOD_DESC(sys) "System/Utility Commands"
    }

    proc mod_help { {args ""} } {
        variable cur_app
        variable ${cur_app}::VERSION
        variable ${cur_app}::APPNAME
        variable ${cur_app}::MODS
        variable MOD_DESC

        if { $args ne "" } {
            array set ml {}
            set cnt 0
            foreach mod $MODS {
                set ml($mod) 0
                foreach arg $args {
                    if { [ string match -nocase $arg $mod] == 1 } {
                        incr ml($mod)
                        incr cnt
                        break;
                    }
                }
            }
            if { $cnt > 0 } {
                puts "$APPNAME $VERSION help"
                foreach mod [array names ml] {
                    if { $ml($mod) > 0 } {
                        puts "    $mod: $MOD_DESC($mod)"
                    }
                }
                return
            }
        }
        puts "$APPNAME $VERSION help"
        puts "    Available command modules: "
        foreach mod $MODS {
            puts "        $mod: $MOD_DESC($mod)"
        }
        puts ""
        puts "    For additional help:"
        puts "    Command list: <module> help"
        puts "    Command detail: <module> help <cmd>"
        puts "    Full Command detail: <module> help <cmd> -long"
        puts "    Note: <cmd> may contain wildcards ? * \[] \\"
        puts "    Examples: ext help"
        puts "              pdb help load_design"
        puts "              pdb help write_spef -long"
        puts "              pdb help write* "

    }
    mod_init
}


