all: server.o tunnel.o
	gcc -o cnc server.o

server.o: src/server.c
	gcc -c src/server.c

tunnel.o: include/tunnel.h src/tunnel.c
	gcc -c src/tunnel.c

clean:
	rm -f *.o
