#
#
#
include ../Makefile.defs

all:: ti

install:: all

ti_parser.cpp: ti.ypp
	bison -o ti_parser.cpp -p Ti -d ti.ypp

ti_scanner.cpp: ti.l ti_parser.cpp
	flex -oti_scanner.cpp -B -PTi -f ti.l

ti: ti.cpp ti_scanner.cpp ti_parser.cpp
	$(CXX) $(CXXFLAGS) -g -o ti ti.cpp ti_scanner.cpp  ti_parser.cpp -lstdc++

Test: test.ti
	ti -p Test_tcl test.ti
	$(CXX) $(CXXFLAGS) -c Test_tcl.cpp

clean::
	rm -rf ti ti_scanner.hpp ti_scanner.cpp ti_parser.cpp ti_parser.hpp
	rm -rf *.o .deps Test_tcl.h Test_tcl.cpp

##############################################
# Add custom targets below the following line.
include ../Makefile.rules
