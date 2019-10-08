#
#
#
include ../Makefile.defs

YACC_SRCXX=def.ypp
YACC_PREFIX=def
YACC_OUTPUT=def_parser
LEX_SRC=def.l
LEX_PREFIX=def
LEX_OUTPUT=def_scanner
LIBNAME=defin

SRCS= definNet.cpp \
      definSNet.cpp \
      definComponent.cpp \
      definVia.cpp \
      definPin.cpp \
      definBlockage.cpp \
      definFill.cpp \
      definTracks.cpp \
      definGCell.cpp \
      definRow.cpp \
      definRegion.cpp \
      definNonDefaultRule.cpp \
      definReader.cpp \
      definBase.cpp \
      create_box.cpp \
      def_parser.cpp \
      def_scanner.cpp \
      defin.cpp \
      definPolygon.cpp \
      definPropDefs.cpp \
      definPinProps.cpp \

##############################################
# Add custom targets below the following line.
include ../Makefile.rules

#
# Build a test parser that does nothing but parse a DEF file
#
deftest: deftest.o def_parser.o def_scanner.o 
	$(CXX) -m32 -o deftest deftest.o def_parser.o def_scanner.o  -lstdc++
