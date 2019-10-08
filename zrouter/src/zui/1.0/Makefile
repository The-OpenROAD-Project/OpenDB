#
#
include ../../Makefile.defs

SRCS= \
	bview.tcl css.tcl \
	mshviewer.tcl csel.tcl\
	history.tcl mgr.tcl zconsole.tcl gtcl.tcl zcontext.tcl help.tcl \
	mzui.tcl zscope.tcl \
	prog.tcl zui.tcl cmd.tcl canvas.tcl loader.tcl cpanel.tcl

##############################################
# Add custom targets below the following line.

all:: tclIndex pkgIndex.tcl
LIB=$(LIBDIR)/ui-1.0

install:: tclIndex zui start.tcl $(SRCS) pkgIndex.tcl
	@echo "	installing: zui 1.0 in $(LIB)"
	@mkdir   -p $(BINDIR)
	@mkdir   -p $(LIB)
	@install -c $(SRCS) tclIndex pkgIndex.tcl $(LIB)

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


clean::
	rm -f tclIndex pkgIndex.tcl

