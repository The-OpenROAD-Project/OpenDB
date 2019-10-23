LEF_TABNAME     = lef
LEF_BISON_SRCS  = lef.y

FAKE_ALL: all

LIBTARGET =	libclef.a

HEADERS =	FEF_malloc.h \
		    FEF_stringhash.h \
			FEF_util.h \
			crypt.h \
			lef.tab.h \
			lex.h \
			lex.cpph

PUBLIC_HDRS = \
			lefiArray.h \
			lefiCrossTalk.h \
			lefiDebug.h \
			lefiDefs.h \
			lefiKRDefs.h \
			lefiLayer.h \
			lefiMacro.h \
			lefiMisc.h \
			lefiNonDefault.h \
			lefiProp.h \
			lefiPropType.h \
			lefiUnits.h \
			lefiUser.h \
			lefiUtil.h \
			lefiVia.h \
			lefiViaRule.h \
			lefrCallBacks.h \
			lefrReader.h \
			lefwWriter.h \
			lefwWriterCalls.h

LIBSRCS =	FEF_malloc.c \
			FEF_stringhash.c \
			crypt.c \
			lef.tab.c \
			lef_keywords.c \
			lefiArray.c \
			lefiCrossTalk.c \
			lefiDebug.c \
			lefiLayer.c \
			lefiMacro.c \
			lefiMisc.c \
			lefiNonDefault.c \
			lefiProp.c \
			lefiPropType.c \
			lefiTBExt.c \
			lefiUnits.c \
			lefiVia.c \
			lefiViaRule.c \
			lefrReader.c \
			lefwWriter.c \
			lefwWriterCalls.c

# For lef, create '.cpp' object
lef.tab.c : $(LEF_BISON_SRCS)
	bison -v -p$(LEF_TABNAME)yy -d $(LEF_BISON_SRCS)

include ../template.mk
