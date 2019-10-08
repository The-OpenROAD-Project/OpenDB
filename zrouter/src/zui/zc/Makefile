#
# Makefile for building zcanvas
#
#
# -- can override in Makefile.local
MAKE_ZC_QT=1

include ../../Makefile.defs

  SRCS = zc.tcl zc_tk.tcl zc_ds.tcl
   LIB = $(LIBDIR)/zc-1.0

##############################################
# Add custom targets below the following line.

all::

## need all first for "ln"
install:: all $(SRCS) pkgIndex.tcl 
	@echo "	installing: zc libs in $(LIB)"
	@mkdir   -p $(LIB)
	@install -c $(SRCS) pkgIndex.tcl $(LIB) 


# -- compile tcl script in the installation directory
# -- caution! do not compile source files, since they will be replaced
# -- by the tbc code
tclcompile:: install
	@echo "	compiling tcl files in $(LIB)"
	@cd $(LIB) && tbc $(SRCS)

clean::
	rm -f set1.cmd tkqt.so zc_qt.tcl
	make -C qzc $(MFLAGS) $(MAKECMDGOALS)

set1.cmd : mkset
	ade <mkset

pkgIndex.tcl: $(SRCS) Makefile
	@echo " making $@ (due to $?)"
	@echo "package require Itcl; pkg_mkIndex . $(SRCS)" | $(TCLSH)

# -- conditional compilation if MAKE_ZC_QT is enabled
ifeq ($(MAKE_ZC_QT),1)
  SRCS = zc.tcl zc_tk.tcl zc_ds.tcl zc_qt.tcl
ZQSRCS = qzc/tkqt.so qzc/zc_qt.tcl

all::
	@ln -sf $(ZQSRCS) .
	make -C qzc $(MFLAGS) $(MAKECMDGOALS)

install::
	@echo "	installing zc/qt in $(LIB)"
	@ln -sf $(ZQSRCS) .
	@cp tkqt.so $(LIB)
	@install -c zc_qt.tcl $(LIB)

tclcompile:: install
	@echo "	compiling additional tcl files in $(LIB)"
	@cd $(LIB) && tbc zc_qt.tcl
endif
