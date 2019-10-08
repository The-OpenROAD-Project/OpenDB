#
#
#
include ../../Makefile.defs

DEFINES= 
SRCS=dpt.cpp

## 32-bit flags
CXXFLAGS=-I/u/mattias/work/latest/zrouter/src/include -Wall  -m32 -O6 -fexpensive-optimizations -I/opt/tcl8/include
LDFLAGS+=-L/u/mattias/lib -ljpeg  -lpng -lz -m32 

## 64-bit flags
#CXXFLAGS=-I/u/mattias/work/latest/zrouter/src/include -O6   -m64 -Wall -fexpensive-optimizations
#LDFLAGS+= -ljpeg  -m64 

CXXFLAGS+=-DUSEJPEG=1 -DUSEPNG=1 -I/u/mattias/include

## Debug flags
CXXFLAGS+=   -g
LDFLAGS+=  -g

dtest: dtest.o dpt.o
	$(CXX) $(LDFLAGS) -o $@ $^

dtest.o: dtest.cpp dpt.h
	$(CXX) $(CXXFLAGS) -c dtest.cpp

dpt.asm: dpt.cpp 
	$(CXX) $(CXXFLAGS) -S $^ -o $@

clean::
	rm -f *.o *.a dtest

##############################################
# Add custom targets below the following line.
include ../../Makefile.rules

