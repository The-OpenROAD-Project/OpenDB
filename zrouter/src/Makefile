#
# PLEASE SEE NOTE ABOUT CONDITIONAL COMPILATION AT THE END
#
# BOOTDIRS - Standalone binaries that must be built before anything else

BOOTDIRS= ti
# STA=sta

#ifeq ($(MAKECMDGOALS),win32_dsp)
#STA=
#endif

#ifeq ($(MAKECMDGOALS),win32_sln)
#STA=
#endif

#ifeq ($(MAKECMDGOALS),clean_cmake)
#STA=
#endif

ifneq ($(BUILD_ARCH),32)
	BUILD_ARCH=64
endif
export BUILD_ARCH

# -- change this to get more noise
ifneq ($(BUILD_NOISE),1) 
# Do not print "Entering directory ..."
	MAKEFLAGS += --no-print-directory
endif


#
# LIBDIRS - Libraries to be built. There should NO depedancies between 
#           libraries.
#
LIBDIRS= \
 	SHAPES \
	cdslef56/lef \
	tm \
	zlib \
	$(STA) \
	db \
	lefin \
	lefout\
	defin \
	defout \
	zutil \
	sylib \
	sver \
	apps \
	milos \
	media \
	utils \
	ModuleTemplate


#
# BINDIRS = Directories which build executables
#
BINDIRS= main

#
# TCLDIRS = Directories which build tcl packages
#
TCLDIRS= zroute msh mtlib zui tt

SUBDIRS=$(BOOTDIRS) $(LIBDIRS) $(BINDIRS) $(TCLDIRS)

ifeq ($(PURIFY),1)
DO_STATIC_LINK=static_link
DO_RATIONAL=purify
endif

ifeq ($(QUANTIFY),1)
DO_STATIC_LINK=static_link
DO_RATIONAL=quantify
endif

TT=$(shell echo $@ | cut -c 1-4)

TESTDIR="$(shell pwd)/tt/tests"
TTOPT:="-t ${TESTDIR}"

ifneq ($(strip $(TFLAGS)),)
	TTOPT+=" ${TFLAGS} "
endif
ifneq ($(strip $(MAKEFLAGS)), )
    TTOPT+="-${MAKEFLAGS}"
endif

%::

ifeq '$(strip $(shell echo $(MAKECMDGOALS) | cut -c 1-4))' "test"
	+(cd tt && ./tt $@ ${TTOPT} )
else ifeq '$(strip $(shell echo $(MAKECMDGOALS) | cut -c 1-3))' "rpm"
include Makefile.defs
rpm:: Makefile.defs
	echo $(DIST)
	ls $(BINDIR)
	ls $(LIBDIR)
else
$(MAKECMDGOALS):: Makefile.defs $(SUBDIRS)
endif

.PHONY: $(SUBDIRS)

Makefile.defs: utils/mkconfig
	utils/mkconfig $(DO_STATIC_LINK) $(DO_RATIONAL) > $@

$(SUBDIRS):
	make -C $@ $(MFLAGS) $(MAKECMDGOALS) 

clean::
	rm -rf Makefile.defs CMake* *.cmake cmake.*

clean_cmake::
	rm -rf CMake* *.cmake cmake.*

CMAKE="c:/Program Files/CMake 2.4/bin/cmake"

win32_dsp::
	@rm -rf CMakeLists.txt
	@echo "PROJECT(zrouter)" >> CMakeLists.txt
	@echo "SUBDIRS($(LIBDIRS) $(BINDIRS))" >> CMakeLists.txt
	@$(CMAKE) -G"Visual Studio 6" .

win32_sln::
	@rm -rf CMakeLists.txt
	@echo "SET(CMAKE_CONFIGURATION_TYPES DEBUG RELEASE)" >> CMakeLists.txt
	@echo "PROJECT(zrouter)" >> CMakeLists.txt
	@echo "SUBDIRS($(LIBDIRS) $(BINDIRS))" >> CMakeLists.txt
	@$(CMAKE) -G"Visual Studio 8 2005" .

# ---------------------------------------------------------------------------
# ADE can be compiled with several options that can aid development
# Here is a list, we would like to encourage developers to maintain
# Please use Makefile.local to override those

# MAKEZUI1=0                : do not make the OLD zui
# BUILD_ARCH=32             : use default build architecture (32 bit)
# USEJPEG=0                 : use jpeg library for dpt
# USEPNG=0                  : use png library for dpt
# MAKE_ZC_QT=0              : disable ZUI QT canvas
# ---------------------------------------------------------------------------
