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

# TILER technology module TCL layer
#
# Provides all necessary code to support the TCL layer of the
# tiler technology module.
# 
# Note that this namespace lives inside the zroute namespace, since
# it makes no sense to use it externally.
# As a result, the tiler technology module author should bind the *_cmd
# commands to zroute::tiler:*_cmd

namespace eval zroute::tiler  {
# ---------------------------------------------------------- EXPORTED COMMANDS
    namespace export tiles_make tracks_assign pins_assign buses_make
    namespace export tile_maker
# ---------------------------------------------------- 
proc init {} {
    # -- define the usage of the inspect command
    namespace import [namespace parent]::args::*
}
# ---------------------------------------------------- 

proc tile_maker {chip size metH metV} {
    tiles_make $chip $size 0
    tracks_assign block $chip metH $metH metV $metV
    pins_assign block $chip metH $metH metV $metV
    buses_make block $chip
}

# proc tiles_make {chip x y}
# tiles_make_cmd $chip $x $y
proc tiles_make {args} {
    eval tiles_make_cmd $args
}

proc tracks_assign {args} {
    eval tracks_assign_cmd $args
}

proc pins_assign {args} {
    eval pins_assign_cmd $args
}

proc buses_make {args} {
    eval buses_make_cmd $args
}

# -------------------------------------------------------------- CLI related
}
