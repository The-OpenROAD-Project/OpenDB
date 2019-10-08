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

#
# LMDS implements the publish and subscribe methods of data exchange
# and interprocess communication
#
package provide LMSH 1.0
package require mtlib 1.0

#
#       Class: ZMDSClient
# Description: Local MDS client -- adapted for zroute
#      Author: Manolis M. Tsangaris
#
#
itcl::class ZMDSClient {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable address ""    ;# where to find mds server
    public variable db "db"       ;# database manager to use
    # -------------------------------------------------- Public Methods ----

    public method Publish {args} {
        LArgs::Parse $args OPT {
            args "args"  "" "datasets to publish" ""
            wait   -wait      "wait for publishing to happen"
            p2p    -p2p  0|1    "operate in peer to peer mode" 0
            dbargs -dbargs "args" "list of arguments to pass to db" {}
        }
        if { $OPT(args) == "" } { error "Publish: no datasets specified" } 
        publish OPT
    }
    public method Subscribe {args} {
        LArgs::Parse $args OPT {
            args    "args"  "" "datasets to subscribe to" ""
            async -async      "do not wait for subscription to happen"
            p2p    -p2p  0|1    "operate in peer to peer mode" 0
            dbargs -dbargs "args" "list of arguments to pass to db" {}
        }
        if { $OPT(args) == "" } { error "Subscribe: no datasets specified" } 
        subscribe OPT
    }
    # -- note: Waiting for multiple datasets, is currently very
    # -- simplistic (we wait in order). It should be replaced by $mds Wait args
    # -- but this is not reliable yet.
    public method Wait {args} {
        foreach ds $args {
            $mds Wait $ds
        }
    }
    # -- unpublish a data set
    # -- returns ok or an error message
    public method Unpublish {dset} {
	$mds Unpublish $dset
    }

    public method mstatus {{ds *}} {
        $mds Status $ds
    }
    # -- obtain the next version of the dataset
    public method NextVersion {dset} {
	if [catch {$mds NextVersion $dset} rc] {
	    warning "mdsclient" "$dset:NextVersion: failed: $rc"
	    return $dset
	}
	set rc
    }
    public method CurVersion {dset} {
	if [catch {$mds CurVersion $dset} rc] {
	    warning "mdsclient" "$dset:CurVersion: failed: $rc"
	    return $dset
	}
	set rc
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable mds ""       ;# mds client object
    private variable ELOG           ;# enhanced logger
    protected variable DBTOP ""       ;#DB TOP object - used for mblocks

    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set mds [MDSClient $this.mds -address $address]
        set ELOG [ELogger #auto]
    }
    destructor {
        $mds delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method message {m1 m2} {
        $ELOG debug "LMDS:A" [format "%10s: %s" $m1 $m2]
        #LLog lmds notice  [format "%10s: %s" $m1 $m2]
    }
    # -- do an eval, by doing a special concatenation of arguments
    # -- so possible empty values are preserved
    # -- we assume that only the last argument is to be concatenated
    # -- to the rest
    protected method leval {args} {
	set last  [lindex $args end]
	set first [lrange $args 0 end-1]
	set cmd [concat $first $last]
	# -- do an eval on the caller frame
	uplevel $cmd
    }
    # 
    # stream in an abstract object
    #     block, lib, or tech
    # the hacks on block will be eliminated in the future
    # opts will be appended to the database command
    private method ds_stream_in {db dataset channel opts} {
        set suff [file extension $dataset]
        set base [file rootname $dataset]
        switch $suff {
            .mblock   {
                set base [block_norm $base]
                if { $DBTOP == "" } {
                    $db create -chip
                    if [$db create -block $base -object= ::BN] {
                        error "$base: block could not be created"
                    }
                    set DBTOP $::BN
                }
                message "Subscribe"  "$db -block $DBTOP -channel $channel $opts"
                leval $db read   -block $DBTOP -channel $channel $opts
            }
            .block   {
                set base [block_norm $base]
                # is the block there? If not, create a fake chip
                if [$db find   -block $base -object= BN] {
                    $db create -chip
                    if [$db create -block $base -object= BN] {
                        error "$base: block could not be created"
                    }
                }
                message "Subscribe"  "$db -block $BN -channel $channel $opts"
                leval $db read   -block $BN -channel $channel $opts
            }
            .subblock {
                set base [block_norm $base]
                set BN ""
                $db find -block $base -object= ::BN
                if { $::BN eq "" } {
                    db get -top -object= ::top
                    db create -block $base -parent $::top
                    $db find -block $base -object= ::BN
                }
                leval $db read -block $::BN -channel $channel $opts
            }
            .db   {
                message "Subscribe"  "$db -block $base -channel $channel $opts"
                leval $db read -channel $channel $opts
            }
            .eco {
                ## Assume top-level db for now..
                $db get -top -object= BN
                message "Subscribe eco" \
		    "db read_eco -block $BN -channel $channel $opts"
                leval $db read_eco -block $BN -channel $channel $opts
                leval $db commit_eco -block $BN
            }
            .tim {
                ## Assume top-level db for now..
                $db get -top -object= BN
                message "Subscribe timing" "db read_timing -block $BN \
		    -channel $channel $opts"
                leval $db read_timing -block $BN -channel $channel $opts
            }
            .tmo {
                ## Assume top-level db for now..
                $db get -top -object= BN
                message "Subscribe timing ECO" "db read_tmg_eco -block $BN \
		    -channel $channel $opts"
                leval $db read_tmg_eco -block $BN -channel $channel $opts
            }
            .id {
                message "Reading ID list " ""
                eval [$::apps::milos::mycf get_tm Tmg] read_all_terms -stream $channel"
            }
            .ep {
                message "Reading Endpoint list " ""
                eval [$::apps::milos::mycf get_tm Tmg] read_end_terms -stream $channel"
            }
            .atil {
                message "Reading Active iterm id list" ""
                eval [$::apps::milos::mycf get_tm Tmg] read_activeterms -stream $channel
            }
            .atnl {
                message "Reading Active iterm name list" ""
                eval $::apps::milos::mycf read_iterm_names $channel
            }
            .atdl {
                message "Reading Active iterm data list" ""
                eval [$::apps::milos::mycf get_tm Tmg] merge_activeterms_values -ichan $channel
            }
            .atsl {
                message "Reading Active iterm stream" ""
                # replaced set_idlist_values by merge_list_timing_data - payam
                #eval [$::apps::milos::mycf get_tm Tmg] merge_list_timing_data -ichan $channel
                eval [$::apps::milos::mycf get_tm Tmg] merge_timing_cache -stream $channel
            }
            .atfl {
                message "Reading Active iterm stream" ""
                #eval [$::apps::milos::mycf get_tm Tmg] merge_activeterms_values -ichan $channel
                eval [$::apps::milos::mycf get_tm Tmg] merge_timing_cache -stream $channel -scenario
            }
            .ctfl {
                message "Reading Clock Term stream" ""
                eval [$::apps::milos::mycf get_tm Tmg] merge_clock_structure -stream $channel 
            }

            .libs {
                # currently, libs are global
                leval $db read  -libs -channel $channel $opts
            }
            .tech {
                # currently, techs are global
                leval $db read  -tech -channel $channel $opts
            }
            .file {
		# -- stream in to a file
                set base [block_norm $base]
                # treat this data set as a file
                set fp [open $base w]
		fconfigure $fp -buffering full -encoding binary \
		    -translation binary
                while { [set nb [read $channel 65536]] != "" } {
                    puts -nonewline $fp $nb
                } 
                close $fp
            }
            .var {
		# -- stream to a variable
                set base [block_norm $base]
                # treat this data set as a global data value
                upvar #0 $base V
                set V [read $channel]
                message "streamed in" \
		    "[string length $V] chars - $V, name: $base"
            }
            default {
                error "$dataset: I do not know how to handle '$suff'"
            }
        }
    }

    private method ds_stream_out {db dataset channel opts} {
        set suff [file extension $dataset]
        set base [file rootname $dataset]

        switch $suff {
            .mblock   {
                set base [block_norm $base]
                # if the block is not found there .. assume it is toplevel
                if [$db find   -block $base -object= BN] {
                    $db get -top -object= BN
                }
                message "Publish" "db write -block $BN -channel $channel $opts"
                leval $db write  -block $BN -channel $channel $opts
            }
            .block   {
                set base [block_norm $base]
                # if the block is not found there .. assume it is toplevel
                if [$db find   -block $base -object= BN] {
                    $db get -top -object= BN
                }
                message "Publish" "db write -block $BN -channel $channel $opts"
                leval $db write  -block $BN -channel $channel $opts
            }
            .subblock   {
                set base [block_norm $base]
                # if the block is not found there .. assume it is toplevel
                if [$db find   -block $base -object= BN] {
                    $db get -top -object= BN
                }
                message "Publish" "db write -block $BN -channel $channel $opts"
                leval $db write  -block $BN -channel $channel $opts
            }
            .db   {
                message "Publish obj" "db write -channel $channel $opts"
                leval $db write -channel $channel $opts
            }
            .eco   {
                $db get -top -object= BN
                message "Publish eco" \
		    "db write_eco -block $BN -channel $channel $opts"
                leval $db write_eco  -block $BN -channel $channel $opts
            }
            .tim   {
                $db get -top -object= BN
                message "Publish timing" \
		    "db write_timing -block $BN -channel $channel $opts"

                leval $db write_timing  -block $BN -channel $channel 
            }
            .tmo   {
                $db get -top -object= BN
                message "Publish timing ECO" \
		    "db write_tmg_eco -block $BN -channel $channel $opts"

                leval $db write_tmg_eco  -block $BN -channel $channel 
            }
            .id {
                message "Publish ID list " ""
                eval "[$::apps::milos::mycf get_tm Tmg] write_all_terms -stream $channel"
            }
            .ep {
                message "Publish Endpoint list " ""
                eval "[$::apps::milos::mycf get_tm Tmg] write_end_terms -stream $channel"
            }
            .atil {
                message "Publish Active iterm id list" ""
                eval [$::apps::milos::mycf get_tm Tmg] write_activeterms -stream $channel
            }
            .atnl {
                message "Publish Active iterm name list" ""
                eval [$::apps::milos::mycf get_tm Tmg] get_name_activeterms -ochan $channel
            }
            .atdl {
                message "Publish Active iterm data list" ""
                eval $::apps::milos::mycf publish_iterm_values $channel
            }
            .atsl {
                message "Publish Active iterm stream" ""
                # replaced get_activeterms_values by get_list_timing_data - payam
                #eval [$::apps::milos::mycf get_tm Tmg] get_list_timing_data -ochan $channel
                eval [$::apps::milos::mycf get_tm Tmg] write_timing_cache -stream $channel -scenario 1
            }
            .atfl {
                message "Publish Active iterm data list" ""
                #eval $::apps::milos::mycf publish_iterm_values $channel
                # changed get_activeterms_values to get_list_timing_data - payam
                #eval [$::apps::milos::mycf get_tm Tmg] get_list_timing_data -ochan $channel
                eval [$::apps::milos::mycf get_tm Tmg] write_timing_cache -stream $channel -scenario $::scen_idx
            }
            .ctfl {
                message "Publish Clock iterm list" ""
                eval [$::apps::milos::mycf get_tm Tmg] write_clock_structure -stream $channel -scenario $::scen_idx
            }
            .libs {
                # currently, libs are global
                leval $db write  -libs -channel $channel $opts
            }
            .tech {
                # currently, techs are global
                message "Publish" "db write -tech -channel $channel $opts"
                leval $db write  -tech -channel $channel $opts
            }
            .file {
		# -- stream out a file
                set base [block_norm $base]
                # treat this data set as a file
                set fp [open $base r]
		fconfigure $fp -buffering full -encoding binary \
		    -translation binary
                while { [set nb [read $fp 65536]] != "" } {
                    puts -nonewline $channel $nb
                }
                close $fp
            }
            .var {
                set base [block_norm $base]
                # treat this data set as a global data value
                upvar #0 $base V
                puts -nonewline $channel $V
                message "streamed out" "[string length $V] chars - value: $V"
            }
            default {
                error "$dataset: I do not know how to handle '$suff'"
            }
        }
    }
    private method check_type {dsets} {
        set DSETS "*.block, *.mblock, *.subblock, *.libs, *.tech *.file *.var .eco .obj .db .tim .tmo .atnl .atil .itvl .id .ep .atdl .atsl .atfl ctfl only"
        foreach ds $dsets {
            set suff [file extension $ds]
            switch $suff {
                .var - .file - .block - .mblock - .subblock - .libs - .tech  - \
		    .eco - .obj - .db - .tim  - .tmo  - .atnl - .atil - .itvl - .id - .ep - .atdl  - .atsl  - .atfl  - .ctfl { }
                default {
                    error "$ds: unsupported dataset (not in $DSETS)"
                }
            }
        }
    }
    # -- a data set has the following syntax:
    # -- NAME[.VERSION][.res]
    # -- strip off both of them, if present
    # -- NOTE: the order matters foo.1.res -> foo , but foo.res.1 -> foo.res
    private method block_norm {base} {
	foreach suff {\\.res \\.[0-9]+} {
	    regsub "$suff\$" $base "" base
	}
	set base
    }
    # ----------------------------------------------- Interface Implementation
    public method publish {opt} {
        upvar $opt OPT
        check_type $OPT(args)
        $mds Publish $OPT(args) [list $this eSend $OPT(dbargs)] -p2p $OPT(p2p)
        if { $OPT(wait) } { $mds Wait $OPT(args) }
    }
    protected method subscribe {opt} {
        upvar $opt OPT
        check_type $OPT(args)
        $mds Subscribe $OPT(args) [list $this eRecv $OPT(dbargs)] -p2p $OPT(p2p)
        if !$OPT(async) {
            $mds Wait $OPT(args)
        }
    }
    # ----------------------------------------------- CallBacks
    # -- called when a data set should be streamed out
    public method eSend {opts ds fp} {
	# -- paranoid check
        if [eof $fp] {
            warning ade-mds "send: $ds: socket '$fp' is closed"
            return
        }
        message sending $ds
        ds_stream_out $db $ds $fp $opts
        close $fp
        message sent $ds
    }
    # -- called when a data set should be streamed in
    public method eRecv {opts ds fp} {
	# -- paranoid check
        if [eof $fp] {
            warning ade-mds "recv: $ds: socket '$fp' is closed"
            return
        }
        message  receiving $ds
        ds_stream_in $db $ds $fp $opts
        close $fp
        message  received $ds
    }
    # -------------------------------------------------- Common Initialization
}
