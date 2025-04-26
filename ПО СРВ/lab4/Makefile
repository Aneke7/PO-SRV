.PHONY: clean

all: lab4

lab4.o: lab4.c
	gcc -c lab4.c -o lab4.o

lab4: lab4.o
	gcc lab4.o -o lab4
	
clean:
	rm -rf lab4 *.o