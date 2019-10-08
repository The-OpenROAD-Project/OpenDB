#
#
#
DTEST=../../dcr/dpt/dtest

test:
	zroute < zpt-test.tcl
	./ztest set0.jpg

show:
	./ztest set0.jpg

speed:
	time ./gtest | $(DTEST) > /dev/null

clean:
	rm -f *.ppm *.jpg *.db
