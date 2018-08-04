OBJ = main.o Error.o GenIR.o InterInst.o Lexer.o Parser.o Scanner.o SymTab.o Token.o 
WARNFLG = -Wall -Wextra
DEBUG = -g
STD = -std=c++11
CXXFLAGS += $(DEBUG) $(WARNFLG) $(STD)
CC = g++

lscc: $(OBJ) 
	$(CC) $(CXXFLAGS) $(OBJ) -o lscc
test: lscc
	./lscc ../test/baseTest.c
clear:
	rm -rf *.o
	rm -rf lscc
