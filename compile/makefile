OBJ = main.o Error.o Lexer.o Parser.o Scanner.o SymTab.o Token.o 
WARNFLG = -Wall -Wextra
DEBUG = -g
STD = -std=c++11
FLAG = $(DEBUG) $(WARNFLG) $(STD)
CC = g++

all: $(OBJ) 
	$(CC) $(OBJ) -o lscc $(FLAG)
main.o:main.cpp common.h
	$(CC) main.cpp -c -o main.o $(FLAG)
Error.o: Error.cpp Error.h Scanner.h
	$(CC) Error.cpp -c -o Error.o $(FLAG)
Lexer.o: Lexer.cpp Lexer.h common.h Scanner.h Token.h Error.h
	$(CC) Lexer.cpp -c -o Lexer.o $(FLAG)
Parser.o: Parser.cpp Parser.h common.h Token.h Lexer.h
	$(CC) Parser.cpp -c -o Parser.o $(FLAG)
Scanner.o: Scanner.cpp Scanner.h Error.h
	$(CC) Scanner.cpp -c -o Scanner.o $(FLAG)
SymTab.o: SymTab.cpp SymTab.h
	$(CC) SymTab.cpp -c -o SymTab.o $(FLAG)
Token.o: Token.cpp Token.h Error.h common.h
	$(CC) Token.cpp -c -o Token.o $(FLAG)
clear:
	rm *.o
help:
	echo "注意检查新添加的文件有没有加入编译"
