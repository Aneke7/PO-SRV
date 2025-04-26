.PHONY: clean clean_logs

all: server client

server: server.o
	gcc server.o -o server 

server.o: server.c
	gcc -std=c99 -c server.c -o server.o 

client: client.o
	gcc client.o -o client 

client.o: client.c
	gcc -c client.c -o client.o

clean:
	rm -rf client server *.o

clean_logs:
	rm -rf *.log