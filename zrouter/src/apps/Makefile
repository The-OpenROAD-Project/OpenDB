#
# Makefile for mtlib
#
include ../Makefile.defs
LIB=$(LIBDIR)/apps

## ESRCS: encrypted sources
ESRCS =  milos.tcl version_milos.tcl athext.tcl version_athext.tcl nRCX.tcl version_nRCX.tcl help.tcl
## USRCS: UNencrypted sources
USRCS = history.tcl

SRCS=$(ESRCS) $(USRCS)

APPS=milos athext nRCX

all:: pkgIndex.tcl tclIndex

apps:
	@echo "$(APPS): installing in $(BINDIR)"
	@cd $(BINDIR) ; for app in $(APPS) ; do ln -sf ade $$app ; done

pkgIndex.tcl: $(SRCS) Makefile $(SRCS)
	@echo "	making pkgIndex.tcl from: $(SRCS)"
	@echo "package require Itcl; pkg_mkIndex . $(SRCS)" | $(TCLSH)

tclIndex: $(SRCS) Makefile
	@echo "	creating $@ (changes in '$?')"
	@echo "package require Itcl; auto_mkindex . $(SRCS)" | $(TCLSH)

tclcompile:: install
	@echo "compiling tcl files in $(LIB)"
	@cd $(LIB) && tbc $(ESRCS)

clean::
	rm -f tclIndex pkgIndex.tcl

install:: all $(SRCS) apps
	@echo "	installing library in $(LIB)"
	@mkdir -p $(LIB)
	@cp $(SRCS) pkgIndex.tcl $(LIB)

include ../Makefile.rules
