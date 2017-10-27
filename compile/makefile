OBJ = main.o lexical.o
WARNFLG = -Wall -Wextra
DEBUG = -g

all: $(OBJ) 
	gcc $(OBJ) -o lscc $(DEBUG) $(WARNFLG)
main.o:main.c common.h
	gcc main.c -c -o main.o $(DEBUG) $(WARNFLG)
lexical.o:lexical.c common.h
	gcc lexical.c -c -o lexical.o $(DEBUG) $(WARNFLG)
clear:
	rm *.o

