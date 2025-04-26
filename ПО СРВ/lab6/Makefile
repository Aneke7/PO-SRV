.PHONY: clean

all: lab6

lab6.o: lab6.c
	gcc -c lab6.c -o lab6.o

lab6: lab6.o
	gcc lab6.o -o lab6
	
clean:
	rm -rf lab6 *.o