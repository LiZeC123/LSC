OBJ = main.o lexical.o error.o parser.o
WARNFLG = -Wall -Wextra
DEBUG = -g

all: $(OBJ) 
	gcc $(OBJ) -o lscc $(DEBUG) $(WARNFLG)
main.o:main.c common.h
	gcc main.c -c -o main.o $(DEBUG) $(WARNFLG)
lexical.o:lexical.c common.h
	gcc lexical.c -c -o lexical.o $(DEBUG) $(WARNFLG)
error.o:error.c common.h
	gcc error.c -c -o error.o $(DEBUG) $(WARNFLG)
parser.o:parser.c common.h
	gcc parser.c -c -o parser.o $(DEBUG) $(WARNFLG)
clear:
	rm *.o
help:
	echo "注意检查新添加的文件有没有加入编译"
