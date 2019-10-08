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

# provides zdb



namespace eval zroute::db  {
# ---------------------------------------------------------- EXPORTED COMMANDS
    namespace export read_lef read_def read_macro_lef read_block_def
    namespace export chip_get chip_attr_get tiles_get
    namespace export files_list files_pwd rsample
    namespace export db_read db_save
    namespace export inspect
    namespace export tiles_get
    namespace export db_schema tblocks_make write_def search_make
# ---------------------------------------------------- 
proc init {} {
    # -- define the usage of the inspect command
    namespace import [namespace parent]::args::*

    # define the usage of calls

    usage read_lef {
        -file name          "LEF file name"     lef
        -skip_geoms value       "Skip macro geometries"     ""
    }
    usage read_def {
        -file name          "DEF file name"     def
        -skip_shields value     "Skip VSS/VDD shield paths"     ""
    }
    usage db_schema {
        -v version          "0:dimitrisDB 1:tomDB"             0
    }
    usage db_read {
        -block block_name       "the name of the block to use"   ALL
        -mode mode          "file(0) stream(1)"            0
        -file name          "DB file name or stream handle"     DB
    }
    usage db_save {
        -block block_name       "the name of the block to use"   ALL
        -mode mode          "file(0) stream(1)"            0
        -file name          "DB file name or stream handle"     DB
    }
    usage inspect {
        obj     object     "name of the object to inspect" ""
        -chip   chipname   "the name of the chip to use"   ALL
        -info   info       "type of info to inspect"       ""
        -bbox   "x0 y0 x1 y1" "bbox to clip the results"   ""
        -layer  layer-list    "list of layers to include" ""
        -type   type-list     "list of obj types to include" ""
        -s      type-obj     "type object to select" ""
    } {
        name {
            inspect -- inspect any object in the chip database
        }
        synopsis {
            inspect obj
            inspect -info mode [flags] obj
        }
        description {
            In the first form (if -info is ommitted) the call
            returns all possible modes of inspection for obj.

            Otherwise, it will return all other objects related to obj
        }
    }

    usage chip_get {
        -chip    chip     "name of the chip"    "ALL"
        -bbox    bbox      "bounding box"       ""
        -layer       layer-list       "list of layers to include" ""
        -hierarchy   hierarchy-list   "list of hierarchy to include" ""
        -type        type-list        "list of obj types to include" ""
        -gx      float   "GUI grid width in microns"      "0.0"
        -gy      float   "GUI grid height in microns"     "0.0"
        -maxobjects int  "maximum number of objects to return" 0
        -noclip          "do not clip the output"
        -q               "this is a cursor query, so do your magic"
    } {
        name chip_get

        synopsis {
            chip_get -q [flags] -- cursor query mode
            chip_get [flags]    -- normal query mode
        }

        description {
            Scan all objects in the chip database.
            Eliminate the ones not in the bbox.
            Exclude layers not in layer-list.
            Exclude objects not in type-list.
            Remove multiple objects mapping into the same gui cell (gx,gy)
            Sample objects to limit output to about maxobjects

            if -noclip is specified, then all objects that hit the bbox
            will be returned, but the will not be clipped to the bbox,
            which is the default.

            if -q is specified, the algorithm can optimize the
            search, since this query will be used for determining
            which objects might be below the cursor.
        }
    }
    usage tblocks_make {
        -block block_name       "the name of the block to use"   ALL
    }
    usage write_def {
        -file name      "the name def file"   "def"
        -use_ids value  "output names using ids"   ""
    }
    usage search_make {
        -block block_name       "the name of the block to use"   ALL
        -force force_flag       "1 overides previous DB"       1
        -top top_block_only     "makes only top block"       0
        -isdb bool_0_1     "Use Search DB Interface from DB"       0
    }
}

# ---------------------------------------------------- 
proc search_make {args} {
    parse search_make $args C
    search_make_cmd C
}
proc tblocks_make {args} {
    parse tblocks_make $args C
    tblocks_make_cmd C
}
proc db_schema {args} {
    parse db_schema $args C
    db_schema_cmd C
}
proc db_read {args} {
    parse db_read $args C
    db_read_cmd C
}
proc db_save {args} {
    parse db_save $args C
    db_save_cmd C
}
proc read_def {args} {
    parse read_def $args C
    read_def_cmd C
}
proc read_lef {args} {
    parse read_lef $args C
    read_lef_cmd C
}
proc read_macro_lef {file} {
    lef_macros_read_cmd $file
}


proc read_block_def {file} {
    blocks_read_def_cmd $file
}

proc write_def {args} {
    parse write_def $args C
    write_def_cmd C
}

#
# ---------------------------------------------------- CHIP RETRIEVAL 
#
proc chip_attr_get {name} {
    chip_attr_get_cmd $name
}

# -- get a  part of the chip
proc chip_get {args} {
    parse chip_get $args C
    chip_get_cmd C
}

#
# produce up to $samples samples
# We should probably do stratified sampling
#
proc rsample {samples args} {
    set res [uplevel $args]
    if { $samples == 1 } { return $res }
    set out ""
    set len [llength $res]
    set e [expr int(1.0 * $len / $samples)]
    if { ($e == 0) || ($e == 1) } {
        puts "*** NO SAMPLING NECESSARY"
        return $res
    }
    puts "*** SAMPLING 1/$e"
    set S 1
    # sample the results
    foreach l $res {
        if { ![incr S -1] } {
            lappend out $l
            set S $e
        }
    }
    set out
}

proc inspect {args} {
    parse inspect $args C
    inspect_cmd C
}

proc tiles_get {chip} {
    tiles_get_cmd $chip {} {}
}
# -------------------------------------------------------------- CLI related
}
