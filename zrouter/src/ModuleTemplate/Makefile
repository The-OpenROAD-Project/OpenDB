#
#
#
include ../Makefile.defs

DEFINES= -DDB_PRIVATE=public

LIBNAME=TEMPLATE

################### Tcl to C++ APIs
SUBDIRS=TMP_tcl  
####################################

SRCS=  \
        TEMPLATE.cpp \
        db_TEMPLATE.cpp \

EXCLUDE_SRC= 
##############################################
# Add custom targets below the following line.
include ../Makefile.rules

#xtest: xtest.cpp
#	$(CXX) $(CXXFLAGS) xtest.cpp -DDB_TRANSFORM_TEST dbTransform.cpp dbTypes.cpp
