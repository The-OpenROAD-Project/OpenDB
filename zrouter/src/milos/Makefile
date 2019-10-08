#
#
#
include ../Makefile.defs
# -- those will go away, superseded by Tech Modules
BASE= vob.tcl pdb.tcl rte.tcl opt.tcl tmg.tcl rpt.tcl  \
	mme.tcl sys.tcl cf.tcl igui.tcl gtree.tcl clk.tcl ext.tcl dobj.tcl  \
	mproxy.tcl ngui.tcl ngui_proxy.tcl smpx.tcl eproxy.tcl

UPDATE= update.tcl

RMT_BASE=    rmt.tcl  rmt_local_null.tcl
RMT_MILOS= rmt_local_milos.tcl rmt_milos.tcl
RMT_PSTA= rmt_local_psta.tcl  rmt_psta.tcl
RMT_PT= rmt_local_pt.tcl rmt_pt.tcl
RMT_QUARTZ= rmt_local_quartz.tcl rmt_quartz.tcl

PLUGIN_BASE= plugin_base.tcl 
PLUGIN_PSTA= plugin_psta.tcl 
PLUGIN_PT= plugin_pt.tcl
PLUGIN_QUARTZ= plugin_quartz.tcl
PLUGIN_CELTIC= plugin_celtic.tcl

PAPI_BASE= papi_utils.tcl papi_base.tcl 
PAPI_PSTA= papi_psta.tcl
PAPI_PT= papi_pt.tcl
PAPI_QUARTZ= papi_quartz.tcl
PAPI_CELTIC=papi_celtic.tcl

FILTER=eco_filter.tcl
FILTER_LN=eco_psta eco_pt eco_magma

RMTS=    $(RMT_BASE) $(RMT_MILOS) $(RMT_PSTA) $(RMT_PT) $(RMT_QUARTZ) $(RMT_REMOTE_MILOS)
PAPI= $(PAPI_BASE) $(PAPI_PSTA) $(PAPI_PT) $(PAPI_QUARTZ) $(PAPI_CELTIC)
PLUGIN= $(PLUGIN_BASE) $(PLUGIN_PSTA) $(PLUGIN_PT) $(PLUGIN_QUARTZ) $(PLUGIN_CELTIC)

SRCS:= $(BASE) $(UPDATE) $(RMTS) $(PAPI) $(PLUGIN) $(FILTER)

FILTER_INSTALL_DIR:=$(wildcard $(PBINDIR))
ifeq ($(FILTER_INSTALL_DIR),)
	FILTER_INSTALL_DIR:=$(BINDIR)
endif


all:: pkgIndex.tcl

install:: pkgIndex.tcl $(FILTER)
	@mkdir -p $(LIBDIR)/milos
	@install -c $(SRCS) pkgIndex.tcl $(LIBDIR)/milos
	@mkdir -p $(FILTER_INSTALL_DIR)
	@cp $(FILTER) $(FILTER_INSTALL_DIR)
	@cd $(FILTER_INSTALL_DIR); for app in $(FILTER_LN); do ln -sf $(FILTER) $$app; done

pkgIndex.tcl: $(SRCS) Makefile
	@echo "	making pkgIndex.tcl from: $(SRCS)"
	@echo "package require Itcl; pkg_mkIndex . $(SRCS)" | $(TCLSH)

# -- compile tcl script in the installation directory
# -- caution! do not compile source files, since they will be replaced
# -- by the tbc code
LIB=$(LIBDIR)/milos
tclcompile:: install
	@echo "	compiling tcl files in $(LIB)"
	cd $(LIB) && tbc $(SRCS)

wc:
	wc $(SRCS)

clean::
	rm -f tclIndex pkgIndex.tcl 
	rm -rf logs

include ../Makefile.rules
