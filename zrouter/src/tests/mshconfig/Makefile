# test the mshconfig capabilities

ENV=export MSHCONFIG=`pwd`/msh.conf

test:
	$(ENV);ade  -mp test1 test.tcl

wroute:
	$(ENV);ade  -mp wroute test.tcl

log:
	less logs/ade/`cd logs/ade; ls -1tr| tail -1`/msh/msh.log

clean:
	rm -rf logs ade.log
