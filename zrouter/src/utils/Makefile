#
#
include ../Makefile.defs

## remove ca if it exists here for now - MH, Nov 28/06
A=$(wildcard $(PBINDIR)/ca)
ifneq '$(A)' ""
	ifeq '$(shell rm $(A))' ""
	endif
endif

all:: 



install:: mproxy-zrouter 
	@echo "	installing: mproxy-zrouter & links in $(PBINDIR)"
## PBINDIR stuff
	@mkdir -p $(PBINDIR)
	@cp mproxy-zrouter $(PBINDIR)
	@(cd $(PBINDIR); ln -sf mproxy-zrouter milos)
	@(cd $(PBINDIR); ln -sf mproxy-zrouter milos32)
	@(cd $(PBINDIR); ln -sf mproxy-zrouter milos64)
	@(cd $(PBINDIR); ln -sf mproxy-zrouter ade)
	@(cd $(PBINDIR); ln -sf mproxy-zrouter ade32)
	@(cd $(PBINDIR); ln -sf mproxy-zrouter ade64)
	@(cd $(PBINDIR); ln -sf mproxy-zrouter nRCX)
	@(cd $(PBINDIR); ln -sf mproxy-zrouter athext)
	@(cd $(PBINDIR); ln -sf mproxy-zrouter athext32)
	@(cd $(PBINDIR); ln -sf mproxy-zrouter athext64)
	@(cd $(PBINDIR); ln -sf mproxy-zrouter zui)
	@(cd $(PBINDIR); ln -sf mproxy-zrouter ademed)

tclcompile: install

clean::

win32_sln:

clean_cmake:
