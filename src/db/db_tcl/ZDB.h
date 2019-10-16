///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019, Nefelus Inc
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef ADS_DB_DB_H
#define ADS_DB_DBH
#endif

#ifndef ADS_TM_H
#include "tm.h"
#endif

namespace odb {

class dbDatabase;
class dbTechNonDefaultRule;

class ZDB : public ZTechModule<ZDB>
{
    dbDatabase * _db;
    int          _io_bufsize;

    bool read_layer_aliases( const char * file );
    bool _parseNonDefaultRule(const char  *inrule, dbTechNonDefaultRule * &out_ndrule);

  public:
     std::string bufsize;

    bool bufsizeChange( const std::string & s );
    
    ZDB( ZArgs * in, ZArgs * out );
    ~ZDB();
    
    // TCL methods
    int clear( ZArgs * in, ZArgs * out );
    int find( ZArgs * in, ZArgs * out );
    int get( ZArgs * in, ZArgs * out );
    int get_children( ZArgs * in, ZArgs * out );
    int bind_block( ZArgs * in, ZArgs * out );
    int reset_inst_hier( ZArgs * in, ZArgs * out );
    int timing_info( ZArgs * in, ZArgs * out );
    int check_lef_sites( ZArgs * in, ZArgs * out );
    int default_class_site( ZArgs * in, ZArgs * out );
    int load_lef( ZArgs * in, ZArgs * out );
    int load_def( ZArgs * in, ZArgs * out );
    int load_def_eco( ZArgs * in, ZArgs * out );
    int save_lef( ZArgs * in, ZArgs * out );
    int save_def( ZArgs * in, ZArgs * out );
    int load_oa_lib( ZArgs * in, ZArgs * out );
    int save_oa_lib( ZArgs * in, ZArgs * out );
    int load_oa_design( ZArgs * in, ZArgs * out );
    int save_oa_design( ZArgs * in, ZArgs * out );
    int read( ZArgs * in, ZArgs * out );
    int read_timing( ZArgs * in, ZArgs * out );
    int write( ZArgs * in, ZArgs * out );
    int write_timing( ZArgs * in, ZArgs * out );
    int init_timing( ZArgs * in, ZArgs * out );
    int clear_timing( ZArgs * in, ZArgs * out );
    int create( ZArgs * in, ZArgs * out );
    int destroy( ZArgs * in, ZArgs * out );
    int duplicate( ZArgs * in, ZArgs * out );
    int remove_wires( ZArgs * in, ZArgs * out );
    int diff( ZArgs * in, ZArgs * out );
    int differences( ZArgs * in, ZArgs * out );
    int names( ZArgs * in, ZArgs * out );
    int connect_ztop( ZArgs * in, ZArgs * out );
    int test_wires( ZArgs * in, ZArgs * out );
    int test_wires2( ZArgs * in, ZArgs * out );
    int test_error( ZArgs * in, ZArgs * out );
    int test_wire_graph( ZArgs * in, ZArgs * out );
    int order_wires( ZArgs * in, ZArgs * out );
    int find_disconnects( ZArgs * in, ZArgs * out );
    int ext_info( ZArgs * in, ZArgs * out );
    int translate_sdc( ZArgs * in, ZArgs * out );
    int get_extraction_corners( ZArgs * in, ZArgs * out );
    int get_block_stats( ZArgs * in, ZArgs * out );
    int begin_eco( ZArgs * in, ZArgs * out );
    int end_eco( ZArgs * in, ZArgs * out );
    int eco_empty( ZArgs * in, ZArgs * out );
    int read_eco( ZArgs * in, ZArgs * out );
    int write_eco( ZArgs * in, ZArgs * out );
    int check_eco( ZArgs * in, ZArgs * out );
    int commit_eco( ZArgs * in, ZArgs * out );
    int begin_tmg_eco( ZArgs * in, ZArgs * out );
    int end_tmg_eco( ZArgs * in, ZArgs * out );
    int eco_tmg_empty( ZArgs * in, ZArgs * out );
    int read_tmg_eco( ZArgs * in, ZArgs * out );
    int write_tmg_eco( ZArgs * in, ZArgs * out );
    int check_tmg_eco( ZArgs * in, ZArgs * out );
    int commit_tmg_eco( ZArgs * in, ZArgs * out );
    int make_area_search( ZArgs * in, ZArgs * out );
    int test_timing_modes( ZArgs * in, ZArgs * out );
    int attach_gui( ZArgs * in, ZArgs * out );
    int test_list( ZArgs * in, ZArgs * out );
    int test_swap_master( ZArgs * in, ZArgs * out );
    int test_ui_region_callback( ZArgs * in, ZArgs * out );
    int set_attribute_for_signal_nets( ZArgs * in, ZArgs * out );
    int count_attribute_for_signal_nets( ZArgs * in, ZArgs * out );
    int rlog( ZArgs * in, ZArgs * out );
    int check_layer( ZArgs * in, ZArgs * out );
    int check_backslash( ZArgs * in, ZArgs * out );
    int print_wire( ZArgs * in, ZArgs * out );
    int print_rseg( ZArgs * in, ZArgs * out );
    int print_capn( ZArgs * in, ZArgs * out );
    int print_wnp( ZArgs * in, ZArgs * out );
    int create_net( ZArgs * in, ZArgs * out );
    int delete_net( ZArgs * in, ZArgs * out );
    int create_inst( ZArgs * in, ZArgs * out );
    int delete_inst( ZArgs * in, ZArgs * out );
    int connect_iterm( ZArgs * in, ZArgs * out );
    int disconnect_iterm( ZArgs * in, ZArgs * out );
    int swap_master( ZArgs * in, ZArgs * out );
    int test_eco( ZArgs * in, ZArgs * out );
    int begin_iterm_missing_scenarios( ZArgs * in, ZArgs * out );


    int next_iterm_missing_scenarios( ZArgs * in, ZArgs * out );
    int begin_bterm_missing_scenarios( ZArgs * in, ZArgs * out );
    int next_bterm_missing_scenarios( ZArgs * in, ZArgs * out );
    int update_iterm_missing_scenario( ZArgs * in, ZArgs * out );
    int update_bterm_missing_scenario( ZArgs * in, ZArgs * out );
    int set_number_of_scenarios( ZArgs * in, ZArgs * out );
    int testlist( ZArgs * in, ZArgs * out );

    int begin_scenario_missing_iterms( ZArgs * in, ZArgs * out );
    int get_scenario_missing_iterms( ZArgs * in, ZArgs * out );
    int set_scenario_missing_iterms( ZArgs * in, ZArgs * out );
    int get_iterm_timing_data( ZArgs * in, ZArgs * out );

    int report_regions( ZArgs * in, ZArgs * out );
    int set_rebuffer_altered_nets( ZArgs * in, ZArgs * out );
    int no_rebuffer_altered_nets( ZArgs * in, ZArgs * out );
    int print_control( ZArgs * in, ZArgs * out );
    int flatten( ZArgs * in, ZArgs * out );
    int create_property( ZArgs * in, ZArgs * out );
    int find_property( ZArgs * in, ZArgs * out );

    int encrypt_file( ZArgs * in, ZArgs *out);
    int decrypt_file( ZArgs * in, ZArgs *out);

    int diff_net_cap( ZArgs * in, ZArgs * out );
    int eco_update_wires( ZArgs * in, ZArgs * out );
};

} // namespace
