OBJ = main.o Error.o GenIR.o InterInst.o Lexer.o Parser.o Scanner.o SymTab.o Token.o 
WARNFLG = -Wall -Wextra
DEBUG = -g
STD = -std=c++11
FLAG = $(DEBUG) $(WARNFLG) $(STD)
CC = g++ $(FLAG)

all: $(OBJ) 
	$(CC) $(OBJ) -o lscc 
test: $(OBJ)
	$(CC) $(OBJ) -o lscc 
	./lscc ../test/baseTest.c
main.o:main.cpp common.h Scanner.h Lexer.h Parser.h
	$(CC) main.cpp -c -o main.o 
Error.o: Error.cpp Error.h Scanner.h Token.h
	$(CC) Error.cpp -c -o Error.o 
GenIR.o: GenIR.cpp GenIR.h SymTab.h Error.h common.h
	$(CC) GenIR.cpp -c -o GenIR.o
InterInst.o: InterInst.cpp InterInst.h common.h SymTab.h GenIR.h
	$(CC) InterInst.cpp -c -o InterInst.o
Lexer.o: Lexer.cpp Lexer.h common.h Scanner.h Token.h Error.h
	$(CC) Lexer.cpp -c -o Lexer.o 
Parser.o: Parser.cpp Parser.h common.h Token.h Lexer.h Error.h
	$(CC) Parser.cpp -c -o Parser.o 
Scanner.o: Scanner.cpp Scanner.h Error.h
	$(CC) Scanner.cpp -c -o Scanner.o 
SymTab.o: SymTab.cpp SymTab.h Token.h Error.h InterInst.h GenIR.h
	$(CC) SymTab.cpp -c -o SymTab.o 
Token.o: Token.cpp Token.h Error.h common.h
	$(CC) Token.cpp -c -o Token.o 
clear:
	rm -rf *.o
	rm -rf lscc
