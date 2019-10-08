include ../Makefile.defs
#
#
SRCS= \
	prog.tcl demo.tcl

LIB=$(LIBDIR)/smpx
all: pkgIndex.tcl
PROG=smpx

install: $(SRCS) pkgIndex.tcl smpx
	@mkdir   -p $(LIB) $(BINDIR)
	@echo "installing:	smpx libs in $(LIB)"
	@cp pkgIndex.tcl $(SRCS) $(LIB)
	@echo "installing:	$(PROG) in $(BINDIR)"
	@install $(PROG) $(BINDIR)

tclIndex: $(SRCS) Makefile
	@echo "making:	tclIndex"
	@echo "package require Itcl; auto_mkindex . $(SRCS)" | tclsh

pkgIndex.tcl: $(SRCS) Makefile
	@echo "making:	 pkgIndex.tcl (changes in '$?') "
	@echo "package require Itcl; pkg_mkIndex . $(SRCS)" | tclsh

tclcompile: install
	cd $(LIB) && tbc $(SRCS) 
	cd $(BINDIR) && tbc mdsd

clean:
	rm -f tclIndex pkgIndex.tcl
	rm -rf logs .mds
