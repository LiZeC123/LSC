OBJ = main.o Label.o Lexer.o Parser.o Scanner.o SymTab.o Token.o 
WARNFLG = -Wall -Wextra
DEBUG = -g
STD = -std=c++11
FLAG = $(DEBUG) $(WARNFLG) $(STD)
CC = g++ $(FLAG)

all: $(OBJ) 
	$(CC) $(OBJ) -o lsca 
test: $(OBJ)
	$(CC) $(OBJ) -o lsca 
	./lsca ../test/baseTest.s
main.o:main.cpp common.h Scanner.h Lexer.h Token.h
	$(CC) main.cpp -c -o main.o 
Label.o: Label.cpp Label.h 
	$(CC) Label.cpp -c -o Label.o
Lexer.o: Lexer.cpp Lexer.h common.h Scanner.h Token.h 
	$(CC) Lexer.cpp -c -o Lexer.o 
Parser.o: Parser.cpp Parser.h common.h Token.h Lexer.h SymTab.h
	$(CC) Parser.cpp -c -o Parser.o
Scanner.o: Scanner.cpp Scanner.h common.h
	$(CC) Scanner.cpp -c -o Scanner.o
SymTab.o: SymTab.cpp SymTab.h Label.h
	$(CC) SymTab.cpp -c -o SymTab.o
Token.o: Token.cpp Token.h common.h
	$(CC) Token.cpp -c -o Token.o 
clear:
	rm -rf *.o
	rm -rf lsca