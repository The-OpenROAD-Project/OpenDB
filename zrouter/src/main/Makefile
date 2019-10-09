#
#
#
include ../Makefile.defs

# WIN32 libs
CMAKE_LIBS=C:/zlib/lib/zdll.lib

all:: zroutesh

BINNAME=zroutesh

EXCLUDE_SRCS=z2tcl.cpp blkHandle.cpp factories.cpp zroute_zui.cpp zroute_tiler.cpp tiler_pkg.c zui_pkg.c
SRCS=tclAppInit.c zroute_pkg.c tcl2cpp.cpp \
     ztop.cpp zroute_db.cpp \
     db_pkg.c modules.cpp logger.cpp

OBJS=tclAppInit.o zroute_pkg.o tcl2cpp.o \
     ztop.o zroute_db.o \
     db_pkg.o modules.o logger.o


QTLIBS=$(X11_LIB) -L$(QTDIR)/lib -lqt-mt 

ifdef PURIFY
LIBS= 

else
LIBS= \
      ../db/WireConn/libdb_conn.a \
      ../db/db_tcl/libdb_tcl.a \
      ../db/libdb.a \
      ../sylib/libsylib.a \
      ../sver/libsver.a \
      ../defout/libdefout.a \
      ../tm/libtm.a \
      ../lefin/liblefin.a \
      ../defin/libdefin.a \
      ../lefout/liblefout.a \
      ../cdslef56/lef/liblef.a \
      ../zlib/libzlib.a \
      ../zutil/libzutil.a \
      $(ZLIB) \
 	../SHAPES/PDN_tcl/libPDN_tcl.a \
 	../SHAPES/libSHAPES.a \
	 ../ModuleTemplate/libTEMPLATE.a \
	 ../ModuleTemplate/TMP_tcl/libTMP_tcl.a
endif

ifeq ($(MAKEDUI),1)
USEQT=1
CXXFLAGS+=-DMAKEDUI
endif

ifeq ($(MAKEUI),1)
USEQT=1
CXXFLAGS+=-DMAKEUI
endif

ifeq ($(USENEWQT),1)
#GLUT=/opt/glut
QTLIBS2=-L$(X11_LIB) -L$(QT)/lib -lqt-mt -lpthread  -lXmu -lXt -lSM -lICE -lXext -lX11 -lXi -lXext -lX11 -lm 
endif

ifeq ($(USEQT),1)
QT=/opt/qt3
GLUT=/opt/glut
QTLIBS=-L/usr/X11R6/lib -L$(QT)/lib -L$(GLUT)/lib -lqt-mt -lpthread -lglut -lGLU -lGL -lXmu -lXt -lSM -lICE -lXext -lX11 -lXi -lXext -lX11 -lm 
endif

STALIBS=

# SRCS+=zroute_sta.cpp
# OBJS+=zroute_sta.o

# zroute_sta.o:	zroute_sta.cpp
	# $(CXX) $(CXXFLAGS) $(STAINC) -c zroute_sta.cpp

STALIBS= 
#STALIBS= \
../sta/sta_z/libsta_z.a \
../sta/search/libsearch.a \
../sta/constraints/libconstraints.a \
../sta/dcalc/libdcalc.a \
../sta/liberty/libliberty.a \
../sta/graph/libgraph.a \
../sta/network/libnetwork.a \
../sta/parasitics/libparasitics.a \
../sta/sdf/libsdf.a \
../sta/util/libutil.a 

LIBS += $(STALIBS)

IMGLIBS=
ifeq ($(USEJPEG),1)
	IMGLIBS=-ljpeg
endif

ifeq ($(USEPNG),1)
	IMGLIBS:=$(IMGLIBS) -lpng
endif

TCLLIBS=-L$(TCLLIB) -ltcl8.5 -lpthread

zroutesh: $(OBJS) $(LIBS) $(STALIBS)
	-rm logger.o
	-rm cvs_version.h
	make logger.o
	#$(PURIFY) $(CXX) -m$(BUILD_ARCH) -o $@ $(OBJS) $(LIBS) $(STALIBS) $(TCLLIBS) $(QTLIBS) $(IMGLIBS) -lz -lstdc++ -lrt
	$(PURIFY) $(CXX) -m$(BUILD_ARCH) -o $@ $(OBJS) $(LIBS) $(TCLLIBS) $(QTLIBS) $(IMGLIBS) -lz -lstdc++ -lrt

logger.o: logger.cpp cvs_version.h

cvs_version.h:
	cp cvs_pref.h cvs_version.h
	exec ../utils/mkcvsversion ./Makefile  | sed -e "s/$$/\\\\n/" >> cvs_version.h
	cat cvs_suff.h >> cvs_version.h
	cat cvs_version.h | tr -d '\n' > tmp.h
	echo "" >> tmp.h
	mv -f tmp.h cvs_version.h

install:: zroutesh
	mkdir -p $(BINDIR)
	install -c zroutesh $(BINDIR)
#install::

clean::
	rm -rf .deps *.o zroutesh


CMAKE_INCLUDES=\
                ../sta/constraints \
                ../sta/dcalc \
                ../sta/graph \
                ../sta/liberty \
                ../sta/network \
                ../sta/parasitics \
                ../sta/sdf \
                ../sta/search \
                ../sta/sta_z \
                ../sta/util \

include ../Makefile.rules
