all: server.o tunnel.o
	gcc -o cnc server.o

server.o: src/server.c
	gcc -c src/server.c

tunnel.o: include/tunnel.h src/tunnel.c
	gcc -c src/tunnel.c

certificate.o: src/certificate.c include/certificate.h
	gcc -c src/certificate.c

clean:
	rm -f *.o cnc
