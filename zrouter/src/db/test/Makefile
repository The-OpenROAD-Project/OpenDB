all: test_transform

test_transform: test_transform.cpp  ../dbTransform.cpp ../../include/dbTransform.h
	c++ -Wall -pedantic -O2 -I../../include test_transform.cpp -o test_transform ../dbTransform.cpp ../dbTypes.o

test_strings: test_strings.cpp  ../libdb.a
	c++ -Wall -pedantic -O2 -I../../include test_strings.cpp -o test_strings ../libdb.a

test_wire: test_wire.cpp  ../libdb.a
	c++ -Wall -pedantic -g -I../../include test_wire.cpp -o test_wire ../libdb.a

clean:
	rm -f test_strings name_server.flat name_server.hier test_transform
