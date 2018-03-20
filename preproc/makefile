OBJ = main.o Lexer.o Scanner.o  Token.o 
WARNFLG = -Wall -Wextra
DEBUG = -g
STD = -std=c++11
FLAG = $(DEBUG) $(WARNFLG) $(STD)
CC = g++ $(FLAG)

all: $(OBJ) 
	$(CC) $(OBJ) -o lscp 
test: $(OBJ)
	$(CC) $(OBJ) -o lscp 
	./lscp ../test/baseTest.c
main.o:main.cpp common.h Scanner.h Lexer.h 
	$(CC) main.cpp -c -o main.o 
Lexer.o: Lexer.cpp Lexer.h common.h Scanner.h Token.h 
	$(CC) Lexer.cpp -c -o Lexer.o 
Scanner.o: Scanner.cpp Scanner.h 
	$(CC) Scanner.cpp -c -o Scanner.o 
Token.o: Token.cpp Token.h common.h
	$(CC) Token.cpp -c -o Token.o 
clear:
	rm -rf *.o
	rm -rf lscp
