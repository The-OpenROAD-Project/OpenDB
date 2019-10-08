include ../Makefile.defs

E=/fs/designs/e

.PHONY: test test1

all:

install: rfarm rssh
	install -c  rfarm rssh $(BINDIR)

test:
	mkdir -p test
	rsync -a $E/e.lef $E/e.def test
	du -sh test
	cd test && ../rfarm "date > today"
	ls -l test

test1:
	@mkdir -p test1
	@echo "** this one should succeed"
	./rfarm -d test1 exit 0
	@echo "** this one should fail"
	-./rfarm -d test1 exit 1
	rmdir test1

tclcompile: install

clean:
	rm -rf test


