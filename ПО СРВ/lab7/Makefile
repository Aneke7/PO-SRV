.PHONY: clean

all: lab7

lab7.o: lab7.c
	gcc -c lab7.c -o lab7.o

lab7: lab7.o
	gcc lab7.o -o lab7
	
clean:
	rm -rf lab7 *.o