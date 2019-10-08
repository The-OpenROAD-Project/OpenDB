FAKE_ALL: all

all: test

install: test

dotest: test

clean doclean:
	rm -f run.output run.stderr diff.out

TEST_FILE = complete.5.5.lef

TEST_GOLD = complete.5.5.lef.au

test: ../bin/lefrw ../bin/lefdiff
	../bin/lefrw $(TEST_FILE) > run.output 2> run.stderr
	diff run.output $(TEST_GOLD)
