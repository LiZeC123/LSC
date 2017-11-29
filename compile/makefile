OBJ = main.o lexical.o error.o parser.o
WARNFLG = -Wall -Wextra
DEBUG = -g
CC = g++

all: $(OBJ) 
	$(CC) $(OBJ) -o lscc $(DEBUG) $(WARNFLG)
main.o:main.c common.h
	$(CC) main.c -c -o main.o $(DEBUG) $(WARNFLG)
lexical.o:lexical.c common.h
	$(CC) lexical.c -c -o lexical.o $(DEBUG) $(WARNFLG)
error.o:error.c common.h
	$(CC) error.c -c -o error.o $(DEBUG) $(WARNFLG)
parser.o:parser.cpp common.h
	$(CC) parser.cpp -c -o parser.o $(DEBUG) $(WARNFLG)
clear:
	rm *.o
help:
	echo "注意检查新添加的文件有没有加入编译"
