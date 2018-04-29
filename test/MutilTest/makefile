CC = ../../lsc


all: main.o fun.o
	$(CC) main.o fun.o -o print
main.o: main.c
	$(CC) main.c -C
fun.o: fun.c fun.h
	$(CC) fun.c -C
clear:
	rm -f *.i
	rm -f *.o
	rm -f *.s
	rm -f print