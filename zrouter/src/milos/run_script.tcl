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


public method run_tmg_remote_script { {undo 0} } {
	mydebug "--------------------------> run_tmg_remote_script: BEGIN\n"
	if { [llength $rmt_list] == 0 } {
		return


#cterms ctlf

	Event <<TIMING_UPDATE_START>> $INCR_CNT

	update_incr_full_timing $rmt_tmg_info $suffix

	set num_scenarios 0
	if { $BBD_OPT eq "ON" } {
		set num_scenarios 1
	}
	foreach rmt $rmt_list {
		mydebug "\t<<<$rmt run_remote_tmg_cmd $rmt_tmg_info 0 $num_scenarios"
		#$rmt run_remote_tmg_cmd $rmt_tmg_info 0 $num_scenarios
		$rmt run_remote_tmg_cmd "example" {a b c d fgh 2}
		mydebug "\t>>>$rmt run_remote_tmg_cmd "
	}
        #if { $BBD_OPT ne "ON" } {
        #foreach rmt $rmt_list {
            ##020810D set dstring "[$rmt getAlias].$INCR_CNT.atfl"
            #set dstring "[$rmt getAlias].$INCR_CNT.$suffix"
                #mydebug "\tPublish $dstring"
            #$rmt publish $dstring
                #mydebug "\tSubscribe $dstring!"
            #Subscribe $dstring
            #mydebug "\tDone Subscribe $dstring!"
            #$rmt unpublish $dstring
        #}
        #} elseif { $rmt_tmg_info eq "aterms" } {
                #foreach rmt $rmt_list {
                        #$rmt store_timing_cache $num_scenarios
                #}
        #}

	check_multi_error
	incr INCR_CNT

	
	Event <<TIMING_UPDATE_END>> $INCR_CNT

	mydebug "--------------------------> run_tmg_remote_script: END\n"
}
