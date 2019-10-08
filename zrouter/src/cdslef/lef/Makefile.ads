#
#
#
include ../../Makefile.defs

YACC_SRC=lef.y
YACC_OUTPUT=lef_parser
YACC_PREFIX=lefyy
LIBNAME=lef
SRCS =  FEF_malloc.cpp \
                    FEF_stringhash.cpp \
                    crypt.cpp \
                    lef_parser.cpp \
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

include ../../Makefile.rules
