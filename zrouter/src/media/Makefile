#
# This is the makefile for zui (ade user interface)
#
# -- install media files

include ../Makefile.defs

LIB=$(LIBDIR)/media

MEDIA=gray25.bmp

SRCS=media.tcl
##############################################
# Add custom targets below the following line.

all::  pkgIndex.tcl

install:: pkgIndex.tcl $(SRCS) $(MEDIA)
	@echo installing media in $(LIB)
	@mkdir -p $(LIB)
	@cp $(SRCS) pkgIndex.tcl $(MEDIA) $(LIB)

pkgIndex.tcl: $(SRCS) Makefile
	@echo " making $@ (due to $?)"
	@echo "package require Itcl; pkg_mkIndex . $(SRCS)" | $(TCLSH)

tclcompile:: install
	@echo "compiling $(SRCS) in $(LIB)"
	@cd $(LIB) && tbc $(SRCS)

clean::
	rm -f pkgIndex.tcl

win32_sln:

clean_cmake:
