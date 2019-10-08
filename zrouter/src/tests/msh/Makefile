
%.test : %.tcl
	ade < $*.tcl

clean:
	rm -rf .mds s1.db logs ade.log

profiles.test: profiles.tcl
	MSHCONFIG=`pwd`/msh.conf ade -P 2 profiles.tcl

log:
	tail -f logs/ade/`cd logs/ade; ls -1|tail -1`/msh/msh.log\
	    logs/ade/`cd logs/ade; ls -1|tail -1`/msh/*@*.log
