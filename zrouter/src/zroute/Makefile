#
#
#
include ../Makefile.defs
# -- those will go away, superseded by Tech Modules
OBSOLETE=db.tcl tiler.tcl ext.tcl

SRCS= \
	apps.tcl zevent.tcl zargs.tcl gdb.tcl\
	zsession.tcl zroute.tcl ztm.tcl tile_cmd.tcl version.tcl\
	zlog.tcl gui.tcl guiadapter.tcl zr.tcl license.tcl\
	$(OBSOLETE)

all:: pkgIndex.tcl

zroute1: zroute.in Makefile
	sed 's@%LDPATH%@$(TCLLIB)@' zroute.in > $@
	chmod +x $@

install:: pkgIndex.tcl zroute gzroute nzroute ade
	@echo "	installing zroute progs in $(BINDIR)"
	@ln -sf zroute zroute.zri
	@mkdir -p $(BINDIR)
	@install -c ade        $(BINDIR)
	@install -c zroute     $(BINDIR)
	@install -c gzroute    $(BINDIR)
	@install -c nzroute    $(BINDIR)
	@install -c zroute     $(BINDIR)/zroute.zri
	@mkdir -p $(LIBDIR)/zroute
	@install -c $(SRCS) pkgIndex.tcl $(LIBDIR)/zroute

pkgIndex.tcl: $(SRCS) Makefile
	@echo "	making pkgIndex.tcl from: $(SRCS)"
	@echo "package require Itcl; pkg_mkIndex . $(SRCS)" | $(TCLSH)

# -- compile tcl script in the installation directory
# -- caution! do not compile source files, since they will be replaced
# -- by the tbc code
LIB=$(LIBDIR)/zroute
tclcompile:: install
	@echo "	compiling tcl files in $(LIB)"
	cd $(LIB) && tbc $(SRCS)
	cd $(BINDIR) && tbc zroute

clean::
	rm -f tclIndex pkgIndex.tcl zroute.zri ade.log 
	rm -rf logs

include ../Makefile.rules
