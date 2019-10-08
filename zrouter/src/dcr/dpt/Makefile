#
#
#
include ../../Makefile.defs

DEFINES= 
LIBNAME=dpt
TCLIF=ZDB.ti
SRCS=dpt.cpp
CXXFLAGS+=-O6
#CXXFLAGS+= -g

dtest: dtest.o dpt.o
	$(CXX) $(CFLAGS) -o $@ $^

clean::
	rm -f *.o *.a dtest

##############################################
# Add custom targets below the following line.
include ../../Makefile.rules

