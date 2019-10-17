LEF_TABNAME     = lef
LEF_BISON_SRCS  = lef.y

FAKE_ALL: all

DIRNAME = lef
LEF_BISON_SRCS  = lef.y

LIBTARGET =	liblef.a

HEADERS =	FEF_malloc.h \
		    FEF_stringhash.h \
		    FEF_util.h \
		    lef.tab.h \
		    lex.h \
			crypt.hpp \
			lex.cpph

PUBLIC_HDRS = \
			lefiArray.hpp \
			lefiCrossTalk.hpp \
			lefiDebug.hpp \
			lefiDefs.hpp \
			lefiKRDefs.hpp \
			lefiLayer.hpp \
			lefiMacro.hpp \
			lefiMisc.hpp \
			lefiNonDefault.hpp \
			lefiProp.hpp \
			lefiPropType.hpp \
			lefiUnits.hpp \
			lefiUser.hpp \
			lefiUtil.hpp \
			lefiVia.hpp \
			lefiViaRule.hpp \
			lefrCallBacks.hpp \
			lefrReader.hpp \
			lefwWriter.hpp \
			lefwWriterCalls.hpp

LIBSRCS =	    FEF_malloc.cpp \
		    FEF_stringhash.cpp \
		    crypt.cpp \
		    lef.tab.cpp \
		    lef_keywords.cpp \
		    lefiArray.cpp \
		    lefiCrossTalk.cpp \
		    lefiDebug.cpp \
		    lefiLayer.cpp \
		    lefiMacro.cpp \
		    lefiMisc.cpp \
		    lefiNonDefault.cpp \
		    lefiProp.cpp \
		    lefiPropType.cpp \
		    lefiTBExt.cpp \
		    lefiUnits.cpp \
		    lefiVia.cpp \
		    lefiViaRule.cpp \
		    lefrReader.cpp \
		    lefwWriter.cpp \
		    lefwWriterCalls.cpp \

# For lef, create '.cpp' object
lef.tab.cpp : $(LEF_BISON_SRCS)
	bison -v -p$(LEF_TABNAME)yy -d $(LEF_BISON_SRCS)
	mv $(LEF_TABNAME).tab.c $(LEF_TABNAME).tab.cpp ;

include ../template.mk
