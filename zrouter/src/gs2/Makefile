#
#
#
include ../Makefile.defs

DEFINES= 
SRCS=gs.cpp

## 32-bit flags
CXXFLAGS=-I/u/mattias/work/latest/zrouter/src/include -Wall  -m32  -O2 
#CXXFLAGS=-I/u/mattias/work/latest/zrouter/src/include -Wall  -m64 -O6 -fexpensive-optimizations
LDFLAGS+= -m32
#LDFLAGS+= -m64

## 64-bit flags
#CXXFLAGS=-I/u/mattias/work/latest/zrouter/src/include -O6   -m64 -Wall -fexpensive-optimizations
#LDFLAGS+= -ljpeg  -m64 

## Debug flags
CXXFLAGS+=   -g 
LDFLAGS+=  -g 

# gtest: gtest.o gs.o
	# $(CXX) $(LDFLAGS) -o $@ $^

# gtest.o: gtest.cpp gs.h
	# $(CXX) $(CXXFLAGS) -c gtest.cpp

clean::
	rm -f *.o *.a gtest

# test: gtest
	# cat gs.dat | ./gtest > g.ppm 2> g.seq

##############################################
# Add custom targets below the following line.
include ../Makefile.rules

