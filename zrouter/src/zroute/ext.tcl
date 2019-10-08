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

package provide ade::main 1.0
#
# ZDB technology module TCL layer
#
# Provides all necessary code to support the TCL layer of the
# db technology module.
# 
# Note that this namespace lives inside the zroute namespace, since
# it makes no sense to use it externally.
# As a result, the zdb technology module author should bind the *_cmd
# commands to zroute::zdb:*_cmd

# provides ext

namespace eval zroute::ext  {
# ---------------------------------------------------------- EXPORTED COMMANDS
    namespace export ext_make_model ext_extract ext_write_spef ext_init ext_clean

# ---------------------------------------------------- 
proc init {} {
    # -- define the usage of the inspect command
    namespace import [namespace parent]::args::*

    # define the usage of calls

    usage ext_init {
        -all flag "Delete all extract objects" "0"
        -model_only ext-flag "Delete only recent extract object" "1"
    }
    usage ext_clean {
        -all flag "Delete all extract objects" "1"
        -ext_only ext-flag "Delete only recent extract object" "0"
    }
    usage ext_make_model {
        -name       model-name      "model name for a process corner" "TYP"
        -from_db    lef-model       "read values from DB" "1"
        -file       process-file    "process parameters" ""
    }
    usage ext_extract {
        -name   model-name      "model name for a process corner" "TYP"
        -net    net-list        "list of net Ids to extract" "ALL"
    }
    usage ext_write_spef {
        -bbox   "x0 y0 x1 y1" "Area to extract"   ""
        -net    net-list    "list of net Ids to extract" "ALL"
        -file   filename    "output SPEF file" ""
    }
}

# ---------------------------------------------------- 
proc ext_init {args} {
    parse ext_init  $args C
    ext_init_cmd C
}
proc ext_make_model {args} {
    parse ext_make_model $args C
    ext_make_model_cmd C
}
proc ext_extract {args} {
    parse ext_extract $args C
    ext_extract_cmd C
}
proc ext_write_spef {args} {
    parse ext_write_spef $args C
    ext_write_spef_cmd C
}
proc ext_clean {args} {
    parse ext_clean $args C
    ext_clean_cmd C
}
# -------------------------------------------------------------- CLI related
}

