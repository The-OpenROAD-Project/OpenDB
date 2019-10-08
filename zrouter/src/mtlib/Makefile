#
# Makefile for mtlib
#
include ../Makefile.defs
LIB=$(LIBDIR)/mtlib

SRCS = object.tcl eventlog.tcl slog.tcl misc.tcl misctk.tcl semaphore.tcl\
	itcl.tcl binder.tcl dotty.tcl interactor.tcl\
	toplevel.tcl prefs.tcl rpc.tcl program.tcl squares.tcl rslave.tcl\
	menu.tcl toolbar.tcl man.tcl popup.tcl platform.tcl tree.tcl cli.tcl \
	dirtree.tcl license.tcl psession.tcl sTable.tcl balloonhelp.tcl\
	colors.tcl\
	shistory.tcl shvar.tcl procwalk.tcl console.tcl rpc2.tcl\
	async-eval.tcl smpx.tcl\
	do.tcl album.tcl plotter.tcl task.tcl\
	form.tcl field.tcl settings.tcl\
	pmonitor.tcl queue.tcl scheduler.tcl devel.tcl args.tcl plugin.tcl\
	wbrowser.tcl rhtml.tcl smessage.tcl image.tcl notebook.tcl timer.tcl \
	timestamp.tcl file.tcl inspector.tcl sdm.tcl viz-event.tcl \
	crypt.tcl \
	p2p.tcl aslave.tcl eventHandler.tcl

all:: tclIndex pkgIndex.tcl

tclIndex: $(SRCS) Makefile
	@echo "	creating $@ (changes in '$?')"
	@echo "package require Itcl; auto_mkindex . $(SRCS)" | $(TCLSH)

pkgIndex.tcl: $(SRCS) Makefile
	@echo " making $@"
	@echo "package require Itcl ; pkg_mkIndex . $(SRCS)" | tclsh


tclcompile:: install
	@echo "compiling tcl files in $(LIB)"
	@cd $(LIB) && tbc $(SRCS)

clean::
	rm -f tclIndex pkgIndex.tcl

install:: tclIndex pkgIndex.tcl $(SRCS)
	@echo "	installing library in $(LIB)"
	@mkdir -p $(LIB)
	@cp $(SRCS) tclIndex pkgIndex.tcl $(LIB)

include ../Makefile.rules
