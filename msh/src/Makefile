#
# Makefile for msh
#
-include Makefile.defs
	
$(MAKECMDGOALS): Makefile.defs
	make -C mtlib $(MFLAGS) $(MAKECMDGOALS) 
	make -C msh   $(MFLAGS) $(MAKECMDGOALS) 
	make -C mds   $(MFLAGS) $(MAKECMDGOALS) 
	make -C rfarm   $(MFLAGS) $(MAKECMDGOALS) 
	make -C smpx   $(MFLAGS) $(MAKECMDGOALS) 

Makefile.defs: utils/mkconfig
	utils/mkconfig > $@
