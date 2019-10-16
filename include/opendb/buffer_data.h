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

namespace odb {
enum Dir {X=0,Y};
typedef struct _Bufloc {
  dbMaster *mas; 
  Point loc;
  extTnode *node;
} Bufloc;
int is_a_buf_or_inv(dbMaster *master);
typedef struct _Buflist {
  Bufloc *val;
  _Buflist *next;
} Buflist;
typedef struct _Bufllist {
  Buflist *val;
  _Bufllist *next;
} Bufllist;
typedef struct _Loadlist {
  int val;
  _Loadlist *next;
} Loadlist;
typedef struct _Loadllist {
  Loadlist *val;
  _Loadllist *next;
} Loadllist;
typedef struct _Solution {
  Buflist *buffers; //buffers and locations for this solution
  double total_area;
  double fanout;
  Bufllist *driven_bufs; // for a given buffer, buffers that it drives directly
  Loadllist *driven_loads; //for a given buffer, the loads it drives
  Buflist *frontier_bufs; //bufs seen from a given extrc node, directly
  Loadlist *frontier_loads;    // loads seen from a extrc node directly
  double cap; // total capacitance till the frontier buffers
  double required; //required time at this extrc node
  double delay;    // max elmore delay from this node to the first load (could be a frontier buffer or a original load
//  double slew;
  _Solution *next;
} Solution;
typedef struct _Solutions {
  Darr<Solution*> pos_solutions;
  Darr<Solution*> neg_solutions;
  _Solutions *next;
} Solutions;
typedef struct _Solslack {
  Solution *sol;
  double slack;
} Solslack; 
typedef struct _RmvbParasitics {
  dbRSeg *rtrseg;
  dbRSeg *lastrrseg;
  dbCapNode *lastrcapnd;
  uint ricapndCnt;
  dbRSeg *dtrseg;
  dbRSeg *fstdrseg;
  dbCapNode *fstdcapnd;
  dbRSeg *bridgeRseg;
  std::vector<dbCCSeg*> * gndcc;
} RmvbParasitics;

typedef struct _Miscdata {
  int idr;
  Hash<int, int> *polarity; //polarity of the receivers once the orig buf_tree is removed
  Darr<dbITerm*> loads;
  int driver_pol;
  Darr<dbMaster*> *bufs;
  Darr<dbMaster*> *invs;
  Darr<Bufloc*> buflocar; //free Bufloc* after use
  dbBlock *block;
  Kdtree *site_tree;
  Kdtree *inst_tree;
  int maxwid;
  Darr<Site*> sites;
  dbNet *net;
  extRcTree *ptree;
  uint totnod;
  double max_cap;
  double max_grid;
  Hash<dbMaster*, int> bufinv;
  Hash<dbMTerm*, double> maxcaptable;
  Hash<Rect*, int> moved_rect;
  Darr<Rect*> movedrect;
  Darr<dbInst*> moved_inst;
  Hash<Rect*, int> sized;
  int rowheight;
  Hash<dbITerm*, double> arrival;
  Hash<void*, double> req_table;
  Hash<dbNet*, int> destroyed_nets;
  dbITerm *worst_in_pin;
  int    worst_in_rf;
  int    worst_out_rf;
  double worst_in_arv;
  double worst_in_slew;
  double worst_slack;
  Darr<dbITerm*> removed_bufs;
  int level;
  double drivercap;
  bool for_slew;
  bool for_glitch;
  double slew_limit;
  double slack_target;
  bool verbose;
  bool new_buffer_timing;
  bool respect_fanout;
  double mcf;
  extTnode *drivernode;
  Hash<Bufloc*, dbInst*> bufinsttable; 
  Hash<dbInst*, extTnode*> new_inst_nd_table;
  Darr<dbInst*> new_insts;
  Hash<dbInst*, extTnode*> old_inst_nd_table;
  int iterations; //iterations of buffering
  Hash<int, extTnode*> load_node;
  Darr<dbNet*> bad_nets;
  Hash<dbITerm*, double> bad_slack_table;
  Hash<dbNet*, int> bad_net_table;
  Hash<dbITerm*, int> affected_iterms;
  Hash<dbNet*, int> buffered_nets;
  Darr<dbITerm*> affected_iterms_ar;
  Darr<dbNet*> *pnetar;
  char **destroyedNetName;
  double fuzz; //user given additive slack
  int gr; // placement grid for a row
  bool dual_incr_spef;
  bool coupled_rc;
  bool reuse_wires;
  bool no_undo;
  bool test_undo;
  bool min_area; // buffering for min_area
  bool multi_mode;
  Darr<dbNet*> destroyed_netar; 
  Darr<dbNet*> oldnets;
  Hash<dbNet*, Darr<dbITerm*>*> netterms;
  Hash<dbNet*, rcmodel**> oldrcmodels;
  Hash<dbNet*, int> boundary_nets;
  Darr<dbNet*> spef_nets;
  Hash<dbITerm*, int> active_terms;
  bool ignore_max_cap;
  int levels;
  bool preserve_setup; 
  bool preserve_hold; 
  int numcorners; // number of extraction corners
  int cornersPerBlock; // number of extraction corners in the block
  bool independentExtCorners;
  bool do_not_legalize; // for buffering
  bool opt_tns;
  Hash<Rect*, dbInst*> legalized_rects;
} Miscdata;
int find_driver_cap(Miscdata *data, dbITerm *tr,bool fixMaxCap);

void getPlacedObjectsInDesign(dbBlock *block, Darr<Rect *> &instar,Hash<Rect*,dbInst *> &instTable,int rowh, int minh, dbSite *rwst = NULL);
void create_sites_for_bbox(int x1, int y1,int x2, int y2,Miscdata *data);
int create_sites_for_block(Miscdata *data, int rowht, dbSiteClass siteclass, dbSite *site = NULL);

int place(Darr<dbInst*> &iar, Darr<Rect*> &sites, Kdtree &tr, double &totmov, int &maxmov, dbInst* &maxinst,double &totrat, int cell_fits_site(dbInst *inst, Site *site),bool doOneInst=false);
int cell_fit_site(dbInst *inst, Site *site);
double getSiteUtilization(dbBlock *block,bool rem_fillers);
extern void find_bufs_and_invs(Miscdata *data, dbLib *lib, Darr<dbMaster*> *bufs, Darr<dbMaster*> *invs, int *maxwid);
void find_max_cap(Miscdata *data,bool fixMaxCap);
int is_a_filler(dbInst* inst);
int cmpdcrbufs(const void *a, const void *b);
int create_sites_for_block(Miscdata *data);
//extern int verify_timing(dbNet *net, Miscdata *data);
void set_arrivals_at_inputs(dbInst *inst, Miscdata *data);
dbITerm *get_output_iterm(dbNet *net);
void print_sol_cap(extTnode *driver, Solution *sol, Miscdata *data);
extTnode *find_real_loads(dbNet *net, Miscdata *data);
void is_a_tree(Miscdata *data, extTnode *node);
int buffer_dp(extTnode *node , Miscdata *data,  Solutions *sols);
void implement_sol(Miscdata *data, Solution *sol);
void free_exttree(extTnode *driver);
int free_sol(Solutions *sol, bool point=true);
dbInst *get_inst_from_bufloc(Miscdata *data, Bufloc *bl);
void makeApproxRcmodel(Miscdata *data, Solution *sol, extTnode *drv_node, dbITerm *drv_buf_iterm);
void remove_filler_cells(dbBlock *block);
int has_bterms(dbNet *net);
extern int buffer_net_for_slew_maxcap(dbNet *net, dbITerm *worst_in, Miscdata *data, Darr<dbInst*> *iar,int *nbuf,int *ninv,bool verbose,bool fixMaxCap, Darr<dbNet*> &cnets);
extern bool init_buffer(dbDatabase *db,dbBlock *block,Miscdata &data,bool fixMaxCap);


} // namespace
