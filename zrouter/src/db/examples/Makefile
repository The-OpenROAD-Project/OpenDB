all: dump

dump: dump.cpp  print.cpp ../libdb.a
	c++ -Wall -pedantic -g -I../../include dump.cpp print.cpp -o dump ../libdb.a

clean:
	rm -f dump
