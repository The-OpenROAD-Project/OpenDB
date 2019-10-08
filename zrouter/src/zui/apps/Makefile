#
# Makefile for building zui adapters
#
include ../../Makefile.defs

   LIB = $(LIBDIR)/uiapps
  SRCS = uiviz.tcl

##############################################
# Add custom targets below the following line.

all:: pkgIndex.tcl

install:: $(SRCS) all pkgIndex.tcl
	@echo "	installing: UI adapter libs in $(LIB)"
	@mkdir   -p $(LIB)
	@install -c $(SRCS) pkgIndex.tcl $(LIB)


# -- compile tcl script in the installation directory
# -- caution! do not compile source files, since they will be replaced
# -- by the tbc code
tclcompile:: install
	@echo "	compiling tcl files in $(LIB)"
	@cd $(LIB) && tbc $(SRCS)

clean::
	rm -f pkgIndex.tcl

pkgIndex.tcl: $(SRCS) Makefile
	@echo " making $@ (due to $?)"
	@echo "package require Itcl; pkg_mkIndex . $(SRCS)" | $(TCLSH)
