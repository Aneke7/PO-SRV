.PHONY: clean

all: primer1

primer1.o: primer1.c
	gcc -c primer1.c -o primer1.o

primer1: primer1.o
	gcc primer1.o -o primer1
	
clean:
	rm -rf primer1 *.o