include ../Makefile.defs
#
#
SRCS= \
	msh.tcl vrc.tcl slave.tcl vrje.tcl vslave.tcl vtask.tcl \
	slavemgr.tcl lic.tcl

all: pkgIndex.tcl

install: $(SRCS) pkgIndex.tcl msh
	@mkdir   -p $(LIBDIR)/msh $(BINDIR)
	@echo "installing:	msh libs in $(LIBDIR)/msh"
	@install -c $(SRCS) pkgIndex.tcl $(LIBDIR)/msh
	install -c msh $(BINDIR)/msh

LIB=${LIBDIR}/msh

tclcompile: install
	cd $(LIB) && tbc $(SRCS)
	cd $(BINDIR) && tbc msh

tags: $(SRCS)
	ctags $(SRCS)

tclIndex: $(SRCS) Makefile
	@echo "making:	tclIndex"
	@echo "package require Itcl; auto_mkindex . $(SRCS)" | tclsh

pkgIndex.tcl: $(SRCS) Makefile
	@echo "making:	 pkgIndex.tcl"
	@echo "package require Itcl; pkg_mkIndex . $(SRCS)" | tclsh

clean:
	rm -f tclIndex pkgIndex.tcl
	rm -rf logs

test:
	MSHCONFIG=msh.conf.local ./msh -i -w

log:
	tail -f logs/msh/msh.log
