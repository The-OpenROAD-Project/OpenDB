#CFLAGS=-g
CFLAGS=
LIBS=-lpthread

all: client server

server: server.c
	$(CC) $(CFLAGS) -o server server.c $(LIBS)

client: client.c
	$(CC) $(CFLAGS) -o client client.c $(LIBS)

clean:
	rm -f client.o server.o client server

package:
	(fname=foo.tgz ; tar cvfz $$fname server.c client.c Makefile; \
	echo "FILE NAME IS $$fname")

