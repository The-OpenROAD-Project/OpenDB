include ../Makefile.defs
#
#
TCL_SRCS= mdsc.tcl mdsc_classes.itcl
CLIBS=libmemobj.so
TCL_FILES= $(TCL_SRCS) $(CLIBS)
# CFLAGS=-m32
# CXXFLAGS=-m32
# 
LIB=$(LIBDIR)/mdsc
all: pkgIndex.tcl

#-----------------------------------------------------------------------------
# INSTALL
#-----------------------------------------------------------------------------
install: pkgIndex.tcl $(TCL_FILES)
	@mkdir   -p $(LIB) $(BINDIR)
	@echo "installing:  mdsc libs in $(LIB)"
	@cp pkgIndex.tcl $(TCL_FILES) $(LIB)

tclcompile: install
	cd $(LIB) && tbc $(TCL_FILES)

#-----------------------------------------------------------------------------
# libmemobj.so
#-----------------------------------------------------------------------------
# This is the C++ library. In theorty I should use the more complex
# and elaborate Makefile scheme of "zroute" but ....
#-----------------------------------------------------------------------------
OBJS= ObjTable.o Debug.o MemObj.o memobj_tcl.o Md5.o

libmemobj.so: $(OBJS) Makefile
	@echo "making $@"
	$(CXX) $(CXXFLAGS) -pipe -shared $(OBJS) -o $@

Debug.o: Debug.cpp Debug.h
ObjTable.o: ObjTable.cpp ObjTable.h Debug.h
MemObj.o: MemObj.cpp MemObj.h Debug.h Md5.h
memobj_tcl.o: memobj_tcl.cpp MemObj.h ObjTable.h Debug.h
Md5.o: Md5.cpp Md5.h

#-----------------------------------------------------------------------------
# pkgIndex.tcl
#-----------------------------------------------------------------------------
pkgIndex.tcl: $(TCL_FILES) Makefile
	@echo "making $@"
	echo "package require Itcl; pkg_mkIndex . $(TCL_FILES)" | tclsh


compile: install
	@echo "	compiling scripts in $(LIB)"
	@cd $(LIB) ; tbc $(TCL_SRCS)

#-----------------------------------------------------------------------------
# MISC
#-----------------------------------------------------------------------------
clean:
	rm -f tclIndex pkgIndex.tcl $(CLIBS) $(OBJS) .server

# ----------------------------------------------------------------------------
# TESTING
# ----------------------------------------------------------------------------
clean-logs:
	rm -f .server
	rm -f logs/*

server:
	mkdir -p logs
	hostname > .server
	-./tmdsc server -port 1234 -size 100m -log logs/server.log
	rm -f .server

client:
	./tmdsc client -server `cat .server` \
		-port 1234 -n 60 -p 10 -checksum no -log logs/cl-`hostname`.log
