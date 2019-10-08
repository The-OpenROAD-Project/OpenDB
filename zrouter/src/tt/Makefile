include ../Makefile.defs

TARGET=run_test

SRCC= config.tcl \
	create.tcl \
	get_zr_options.tcl \
	globals.tcl \
	group.tcl \
	parse.tcl \
	test.tcl \
	timer.tcl \
	util.tcl \

SRC = tt $(SRCC)

install::
	cp $(TARGET) $(PBINDIR)

installc::
	mkdir -p $(LIBDIR)/tt
	install -c  $(SRC) $(LIBDIR)/tt

tclcompile:: installc
	cd $(LIBDIR)/tt && tbc $(SRCC)


include ../Makefile.rules
