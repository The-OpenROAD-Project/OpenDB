#
# This is the makefile for zui (ade user interface)
#
# -- make zui by default
MAKEZUI1=1

include ../Makefile.defs

SRCS= \
	platform.tcl bview.tcl css.tcl adapter.tcl\
	mshviewer.tcl csel.tcl version.tcl zsettings.tcl\
	history.tcl mgr.tcl zconsole.tcl gtcl.tcl help.tcl \
	mzui.tcl zscope.tcl bookmark.tcl dslist.tcl zplotter.tcl\
	prog.tcl zui.tcl cmd.tcl canvas.tcl loader.tcl cpanel.tcl dsmgr.tcl

LIB=$(LIBDIR)/ui-2.0

##############################################
# Add custom targets below the following line.

all:: tclIndex pkgIndex.tcl
	make -C zc $@
	make -C apps $@

install:: tclIndex zui start.tcl $(SRCS) pkgIndex.tcl
	@echo "	installing: zui 2.0 in $(LIB)"
	@mkdir   -p $(BINDIR)
	@mkdir   -p $(LIB)
	@install -c $(SRCS) tclIndex pkgIndex.tcl $(LIB)
	@install zui $(BINDIR)
	make -C zc $@
	make -C apps $@


tclIndex: $(SRCS) Makefile
	@echo "	making $@ (due to $?)"
	@echo "package require Itcl; auto_mkindex . $(SRCS)" | $(TCLSH)

pkgIndex.tcl: $(SRCS) Makefile
	@echo "	making $@ (due to $?)"
	@echo "package require Itcl; pkg_mkIndex . $(SRCS)" | $(TCLSH)

# -- compile tcl script in the installation directory
# -- caution! do not compile source files, since they will be replaced
# -- by the tbc code
tclcompile:: install
	@echo "	compiling tcl files in $(LIB)"
	@cd $(LIB) && tbc $(SRCS)
	@cd $(BINDIR) && tbc zui
	make -C zc $@
	make -C apps $@

clean::
	rm -f tclIndex pkgIndex.tcl
	make -C zc $@
	make -C apps $@


# ----------------------------------------------------------------------
# -- OLD ZUI compatibility. Define this in your Makefile.local
# -- to cause installation of the 1.0 zui

ifneq ($(MAKEZUI1),)
install::
	make -C 1.0 $@

clean::
	make -C 1.0 $@

tclcompile::
	make -C 1.0 $@
endif

win32_sln:

clean_cmake:
