include ../Makefile.defs
#
#
#
SRCS= \
	object.tcl misc.tcl rpc.tcl binder.tcl cli.tcl exec.tcl \
	interactor.tcl \
	slog.tcl semaphore.tcl rje.tcl shvar.tcl rpc2.tcl smpx.tcl do.tcl \
	itcl.tcl program.tcl pmonitor.tcl queue.tcl scheduler.tcl \
	license.tcl procwalk.tcl task.tcl async-eval.tcl\
	toplevel.tcl dirtree.tcl misctk.tcl console.tcl prefs.tcl squares.tcl\
	sTable.tcl

all: pkgIndex.tcl

install: tclIndex $(SRCS) pkgIndex.tcl
	mkdir   -p $(LIBDIR)/mtlib
	@echo installing mtlib to $(LIBDIR)/mtlib
	@install -c $(SRCS) pkgIndex.tcl $(LIBDIR)/mtlib

LIB=$(LIBDIR)/mtlib
tclcompile: install
	cd $(LIB) && tbc $(SRCS)

tclIndex: $(SRCS) Makefile
	@echo "making $@ (changes in $?)"
	@echo "package require Itcl; auto_mkindex . $(SRCS)" | tclsh

pkgIndex.tcl: $(SRCS) Makefile
	@echo "making $@ (changes in $?)"
	@echo "package require Itcl; pkg_mkIndex . $(SRCS)" | tclsh

clean:
	rm -f tclIndex pkgIndex.tcl
