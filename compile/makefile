OBJ = main.o Error.o InterInst.o Lexer.o Parser.o Scanner.o SymTab.o Token.o 
WARNFLG = -Wall -Wextra
DEBUG = -g
STD = -std=c++11
FLAG = $(DEBUG) $(WARNFLG) $(STD)
CC = g++ $(FLAG)

all: $(OBJ) 
	$(CC) $(OBJ) -o lscc 
main.o:main.cpp common.h Scanner.h Lexer.h Parser.h
	$(CC) main.cpp -c -o main.o 
Error.o: Error.cpp Error.h Scanner.h Token.h
	$(CC) Error.cpp -c -o Error.o 
InterInst.o: InterInst.h common.h SymTab.h
	$(CC) InterInst.cpp -c -o InterInst.o
Lexer.o: Lexer.cpp Lexer.h common.h Scanner.h Token.h Error.h
	$(CC) Lexer.cpp -c -o Lexer.o 
Parser.o: Parser.cpp Parser.h common.h Token.h Lexer.h Error.h
	$(CC) Parser.cpp -c -o Parser.o 
Scanner.o: Scanner.cpp Scanner.h Error.h
	$(CC) Scanner.cpp -c -o Scanner.o 
SymTab.o: SymTab.cpp SymTab.h Token.h Error.h
	$(CC) SymTab.cpp -c -o SymTab.o 
Token.o: Token.cpp Token.h Error.h common.h
	$(CC) Token.cpp -c -o Token.o 
clear:
	rm -rf *.o
	rm -rf lscc
help:
	echo "注意检查新添加的文件有没有加入编译"
