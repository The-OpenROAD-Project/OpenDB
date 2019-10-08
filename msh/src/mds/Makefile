include ../Makefile.defs
#
#
SRCS= \
	mds.tcl var.tcl mdsclient.tcl

LIB=$(LIBDIR)/mds
all: pkgIndex.tcl

install: $(SRCS) pkgIndex.tcl mdsd
	@mkdir   -p $(LIB) $(BINDIR)
	@echo "installing:	mds libs in $(LIB)"
	@cp pkgIndex.tcl $(SRCS) $(LIB)
	@echo "installing:	mdsd in $(BINDIR)"
	@install mdsd $(BINDIR)

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
