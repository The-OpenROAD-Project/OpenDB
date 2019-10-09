#
#
#
include ../Makefile.defs

DEFINES= -DDB_PRIVATE=public

LIBNAME=SHAPES

################### Tcl to C++ APIs
SUBDIRS=PDN_tcl  
####################################

SRCS=  \
        SHAPES.cpp \
        pdn_info.cpp \

EXCLUDE_SRC= 
##############################################
# Add custom targets below the following line.
include ../Makefile.rules

#xtest: xtest.cpp
#	$(CXX) $(CXXFLAGS) xtest.cpp -DDB_TRANSFORM_TEST dbTransform.cpp dbTypes.cpp
