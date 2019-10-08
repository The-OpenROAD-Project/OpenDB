
all:

%.test : %.tcl
	ade -dmds notice:info $*.tcl

test1: mds1.tcl
	ade mds1.tcl

.PHONY: logs

logs:
	#tail -f logs/ade/*/msh/mds.log logs/ade/*/msh/msh.log
	tail -f logs/ade/*/msh/mds.log

test:
	 ade -dmds notice  mds1.tcl

var:
	 ade -dmds notice  var.tcl

unpub:
	 ade -dmds notice  unpub.tcl

file:
	 ade -dmds notice  file.tcl

client:
	ade client.tcl

clean:
	rm -rf .mds s1.db logs file1 tiles ade.log

