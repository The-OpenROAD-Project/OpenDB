#
#
#
include ../Makefile.defs

LIBNAME=sylib

SRCS= sy_lib.cpp sy_func.cpp sy_pi.cpp liberty_parser.cpp \
      liberty_front_lex.cpp token.cpp sy_palloc.cpp \
      attr_lookup.cpp group_lookup.cpp

YACC_SRC=liberty_parser.y
YACC_PREFIX=liberty_parser_
LEX_SRC=token.l

##############################################
# Add custom targets below the following line.
include ../Makefile.rules

sytest:	sylib_main.cpp $(OBJS)
	g++ $(CXXFLAGS) -o sytest sylib_main.cpp $(OBJS) -lm

