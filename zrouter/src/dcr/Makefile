#
#
#
include ../Makefile.defs

LIBNAME=dcr

TCLIF=Crawler.ti

SRCS= grids.cpp zui.cpp Sdb.cpp Zdcr.cpp Crawler.cpp \
	spreadSearch.cpp Crawler_Tcl.cpp dpt.cpp asa.cpp 

##############################################
# Add custom targets below the following line.
include ../Makefile.rules

# -- pass the USEJPEG option to c++
ifneq ($(USEJPEG),)
CXXFLAGS +=-DUSEJPEG=1
endif
ifneq ($(USEPNG),)
CXXFLAGS +=-DUSEPNG=1
endif

CXXFLAGS += -I.
asatest: asa.o asa.h asatest.cpp
	$(CXX) $(CXXFLAGS) asatest.cpp -o asatest asa.o -L/opt/tcl8/lib -ltcl
