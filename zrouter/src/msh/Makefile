#
#
#
include ../Makefile.defs
SRCS=mshc.tcl msh.tcl mdsclient.tcl lmds.tcl rslave.tcl
    LIB=$(LIBDIR)/lmsh
 OLDLIB=$(LIBDIR)/msh

all:: pkgIndex.tcl tclIndex

install:: tclIndex $(SRCS) pkgIndex.tcl
# -- cleanup a leftover lib/msh directory
	@(test -f $(OLDLIB)/mdsclient.tcl && rm -f $(OLDLIB)/*.tcl)||true
	@echo   "	install msh support libs in $(LIB)"
	@mkdir   -p $(LIB)
	@install -c $(SRCS) tclIndex pkgIndex.tcl $(LIB)

tclIndex: $(SRCS) Makefile
	@echo "	remaking $@  (changes in $?)"
	@echo "package require Itcl; auto_mkindex . $(SRCS)" | $(TCLSH)

pkgIndex.tcl: $(SRCS) Makefile
	@echo "	remaking $@  (changes in $?)"
	@echo "package require Itcl; pkg_mkIndex . $(SRCS)" | $(TCLSH)

# -- compile tcl script in the installation directory
# -- caution! do not compile source files, since they will be replaced
# -- by the tbc code
LIB=$(LIBDIR)/lmsh
tclcompile:: install
	@echo "	compiling tcl files in $(LIB)"
	@cd $(LIB) && tbc $(SRCS)

clean::
	rm -f tclIndex pkgIndex.tcl

include ../Makefile.rules
