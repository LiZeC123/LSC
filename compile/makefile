OBJ = main.o lexical.o

all: $(OBJ) 
	gcc -g $(OBJ) -o lscc
main.o:main.c common.h
	gcc main.c -c -g -o main.o
lexical.o:lexical.c common.h
	gcc lexical.c -c -g -o lexical.o
clear:
	rm *.o

