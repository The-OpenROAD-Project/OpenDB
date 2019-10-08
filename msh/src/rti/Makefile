include ../Makefile.defs
#
#
SRCS= \
	server.tcl client.tcl ui.tcl ui_css.tcl

PROGS=rti wrti

all: pkgIndex.tcl

install: $(SRCS) pkgIndex.tcl $(PROGS)
	@mkdir   -p $(LIBDIR)/rti $(BINDIR)
	@echo "installing:	libs in $(LIBDIR)/rti"
	@install -c $(SRCS) pkgIndex.tcl $(LIBDIR)/rti
	install -c $(PROGS) $(BINDIR)

tclcompile: install
	cd $(LIBDIR)/rti && tbc $(SRCS)
	cd $(BINDIR) && tbc $(PROGS)

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
