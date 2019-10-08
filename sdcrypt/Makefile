#
#/*****************************************************************
# * Copyright (c) 2003 - Synchronous Design Automation            *
# * Distribution,Modification or Use without a license agreement  *
# * with Synchronous Design Automation is not permitted.          * 
# * Contact: info@synchronous-da.com                              *
# *****************************************************************/
#
## ---------Package level Makefile-----------------------
## ---------Template in $SYNCDA_HOME/etc/Makefile.package---
## Example values are given after the '=' sign on each variable

# Specify target executable for this <group>/<package>
##MODE := optimze
TARGET = gen_token

CFLAGS =  
# Specify headers to be exported to $(SYNCDA_HOME)/include
TARGET_HEADERS = Blowfish.h 

# Specify headers local group headers to be exported to <group>/include
GROUP_HEADERS  = 

# Specify library to be installed in <group>/lib
GROUP_LIBS    =  libblowfish.a

# Specify shared library to be installed in <group>/lib
GROUP_SHARED_LIBS    = ##

# Header files 
HEADERS =	

# Source files
SOURCES =	Blowfish.cpp Test.cpp sdcrtpt.cpp

# Objects that will make up the library GROUP_LIBS
LIB_OBJS =      Blowfish.o

# Objects that will make TARGET
OBJECTS =	Blowfish.o Test.o sdcrypt.o

CLEAN =		$(OBJECTS) $(GROUP_LIBS)

# Specify additional Include directories with e.g. -I../include
INCLUDES = 

## Libs to link to make TARGET
LIBS =		#-L$(SYNCDA_HOME)/lib 



# Include package makefile template
# *****************************************************************/
#
# Package rules, to be included in <group>/<package> directories.


#>		--- do not remove this line ---
AR =		ar
ARFLAGS =	cr
OS_TYPE := $(shell uname)
CFLAGS += -fPIC


PURE := purify
QUANTIFY := quantify

ifeq ($(strip $(BITS)),32)
CFLAGS := -m32 $(CFLAGS)
endif


ifeq ($(strip $(MODE)),debug)
OPTMODE := debug
endif

ifeq ($(strip $(MODE)),dbg)
OPTMODE := debug
endif

ifeq ($(strip $(MODE)),opt)
OPTMODE := optimize
endif 
ifeq ($(strip $(MODE)),optimize)
OPTMODE := optimize
endif 
ifeq ($(strip $(MODE)),)
OPTMODE := optimize
endif 

ifeq ($(strip $(MODE)),prof)
OPTMODE=debug
endif


DEBUG_FLAGS := -g -Wall 
OPTIMIZE_FLAGS := -O3

ifeq ($(OS_TYPE),Linux)
CC = g++
OPTIMIZE_FLAGS :=  -O3
endif


ifeq ($(OS_TYPE),Solaris)
CC = CC
OPTIMIZE_FLAGS := -xO5
endif

ifneq ($(strip $(CFLAGS)),)
CFLAGS :=  $(CFLAGS)
else
CFLAGS = 
endif

ifneq ($(strip $(CPPFLAGS)),)
CPPFLAGS := $(CPPFLAGS) 
else
CPPFLAGS =  
endif



ifneq ($(strip $(INCLUDES)),)
INCLUDES := $(INCLUDES) -I. -I../include -I$(SYNCDA_HOME)/include
else 
INCLUDES = -I. -I../include -I$(SYNCDA_HOME)/include
endif




## Define these installation
TCLDIR = $(SYNCDA_HOME)/lib
SWIG = $(SYNCDA_HOME)/bin/swig
JAVAC  =     /usr/local/java/bin/javac

DISTCLEAN =
EXEEXT =
INSTALL =	install -c
INSTALL_DATA =	$(INSTALL) -m 644
INSTALL_PROGRAM =	$(INSTALL) -s
LDFLAGS =
LEX =		flex

LIBTOOL =	libtool $(LTFLAGS)
LPR =		lpr -h
PROGRAMS =	$(OBJECTS:.o= )
RANLIB =	ranlib
RM =		rm -f
VERSION =	0
YACC =		bison

prefix =	$(SYNCDA_HOME)
bindir =	$(prefix)/bin
incdir =        $(prefix)/include
libdir =	$(prefix)/lib
mandir =	$(prefix)/man
classdir  =     $(prefix)/lib/class 
tcllibdir =    $(prefix)/lib/sdtcl
grpincdir =     ../include
grplibdir =     ../lib



ifeq ($(strip $(OPTMODE)),optimize)
COMPILE :=	$(CC) $(OPTIMIZE_FLAGS) -c $(CFLAGS) $(DEFS)\
		 $(INCLUDES) $(CPPFLAGS) 
else 
COMPILE :=	$(CC) -c $(CFLAGS) $(DEBUG_FLAGS) $(DEFS)\
		 $(INCLUDES) $(CPPFLAGS) $(PROFFLAGS)

endif

ifeq ($(strip $(MODE)),prof) 
COMPILE += -pg
LDFLAGS += -pg
endif

ECHO            = /bin/echo
SECHO           = @-$(ECHO)


.SUFFIXES:	.l .y .m .cc .cpp .java
.c.o:
		$(COMPILE) $< -o $@
.m.o:
		$(COMPILE) $< -o $@
.cc.o:
		$(COMPILE) $< -o $@
.cpp.o:
		$(COMPILE) $< -o $@
.l.c:
		$(LEX) $(LFLAGS) -t $< >$@
.y.c:
		$(YACC) $(YFLAGS) -y -d $<
		mv y.tab.c $*.c
		cmp -s y.tab.h $*.h || cp y.tab.h $*.h
.y.h:
		$(YACC) $(YFLAGS) -y -d $<
		mv y.tab.c $*.c
		cmp -s y.tab.h $*.h || cp y.tab.h $*.h

.class:
		echo "Test"
		$(JAVAC)  $<
.java:
		echo "Test"
		$(JAVAC)  $<

SHELL =		/bin/sh


all: $(TARGET) $(ADDLIBS) grp_libs grp_shared_libs java_libs


ifneq ($(strip $(TARGET)),)
$(TARGET):	$(OBJECTS) $(ADDLIBS)
ifeq ($(OPTMODE),optimize)
		$(CC) $(LDFLAGS) $(OBJECTS) $(ADDLIBS) $(LIBS) -o $@
else
		$(CC) $(LDFLAGS) $(OBJECTS) $(ADDLIBS) $(LIBS) -o $@
endif
else
$(TARGET):
endif


optimize: $(TARGET)
	echo "Optimize Specified"
DEBUG_FLAGS = 
OPTIMIZE_FLAGS = $(OPT_FLAG)

#$(OPTIMIZE):	$(OBJECTS) $(ADDLIBS)
#		$(CC) $(OPTIMIZE_FLAGS) $(LDFLAGS) $(OBJECTS) $(ADDLIBS) -static \
#		-L$(SYNCDA_HOME)/lib -ltclstub8.4 $(LIBS) -o $(TARGET)



#ifeq ($(strip $(OPTIMIZE)),)
#DEBUG = $(TARGET)
#$(DEBUG):	$(OBJECTS) $(ADDLIBS)
#		$(CC) -g $(LDFLAGS) $(OBJECTS) $(ADDLIBS) $(LIBS) -o $@
#else
#$(DEBUG):
#endif


## Static Executable

## Purify
$(TARGET).pure:	$(OBJECTS) $(ADDLIBS)
		$(PURE) $(CC) $(LDFLAGS) $(OBJECTS) $(ADDLIBS) -static $(LIBS) -o $@

$(TARGET).q:	$(OBJECTS) $(ADDLIBS)
		$(QUANTIFY) $(CC) $(LDFLAGS) $(OBJECTS) $(ADDLIBS) -static $(LIBS) -o $@


## Gprof
$(TARGET).prof:	COMPILE += -pg
$(TARGET).prof:	$(OBJECTS) $(ADDLIBS)
		$(CC) -pg $(LDFLAGS) $(OBJECTS) $(ADDLIBS) -static $(LIBS) -o $@

ifneq ($(strip $(GROUP_LIBS)),)
grp_libs: $(OBJECTS)
	rm -f $(GROUP_LIBS)
	$(AR) $(ARFLAGS)  $(GROUP_LIBS) $(LIB_OBJS)
else
grp_libs:
endif

ifneq ($(strip $(GROUP_SHARED_LIBS)),)
grp_shared_libs: $(OBJECTS)
else
grp_shared_libs:
endif


ifneq ($(strip $(JAVA_LIBS)),)
java_libs: $(JAVA_OBJECTS)
	rm -f $(JAVA_LIBS)
	$(JAVAC) $(CLASSPATH) $(JAVA_SOURCES)
$(JAVA_OBJECTS): $(JAVA_SOURCES)
else
java_libs:
endif




check:		$(TARGET)


clean:
		$(RM) $(CLEAN) $(TARGET) $(TARGET)$(EXEEXT) $(GROUP_LIBS) $(GROUP_SHARED_LIBS) core

distclean:	clean
		$(RM) $(DISTCLEAN)

print:
		$(LPR) $(HEADERS) $(SOURCES)

install:	$(TARGET) grp_libs grp_shared_libs install_target_headers\
		 install_grp_headers\
                   install_grp_libs install_grp_shared_libs install_java_libs\
		 install_tcl_libs install_target post_install

ifeq ($(strip $(POST_INSTALL_RULE)),)
post_install:
endif

ifneq ($(strip $(GROUP_HEADERS)),)
install_grp_headers: $(GROUP_HEADERS)
		$(SECHO) "Installing Group Headers.."
		$(INSTALL_DATA) $(GROUP_HEADERS)$(EXEEXT) $(grpincdir)
else 
install_grp_headers:
endif

ifneq ($(strip $(TARGET_HEADERS)),)
install_target_headers: $(TARGET_HEADERS)
		$(SECHO) "Installing Headers.."
		$(INSTALL_DATA) $(TARGET_HEADERS)$(EXEEXT) $(incdir)
else
install_target_headers: 
endif

ifneq ($(strip $(GROUP_LIBS)),)
install_grp_libs: $(GROUP_LIBS) grp_libs
	$(SECHO) "Installing Group Libraries.."
	$(INSTALL_DATA) $(GROUP_LIBS) $(grplibdir)	
else
install_grp_libs: 
endif

ifneq ($(strip $(GROUP_SHARED_LIBS)),)
install_grp_shared_libs: $(GROUP_SHARED_LIBS) grp_shared_libs
	$(SECHO) "Installing Group Shared Libraries.."
	$(INSTALL_DATA) $(GROUP_SHARED_LIBS) $(grplibdir)	
else
install_grp_shared_libs: 
endif

ifneq ($(strip $(JAVA_LIBS)),)
install_java_libs: java_libs
	$(SECHO) "Installing Java Classes..."
	$(INSTALL_DATA) $(JAVA_LIBS) $(classdir)
else
install_java_libs:
endif

ifneq ($(strip $(TCL_LIBS)),)
install_tcl_libs: $(TCL_LIBS)
	$(SECHO) "Installing TCL Libraries..."
	$(INSTALL_DATA) $(TCL_LIBS) $(tcllibdir)
else
install_tcl_libs:
endif


ifneq ($(strip $(INSTALL_TARGET)),)
install_target: $(INSTALL_TARGET)
	$(SECHO) "Installing Executable.."
	$(INSTALL_PROGRAM) $(INSTALL_TARGET) $(bindir)
else
install_target:
endif



uninstall:
		$(RM) $(bindir)/$(TARGET)$(EXEEXT)

#>		--- do not remove this line ---

