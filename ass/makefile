OBJ = main.o Generator.o ElfFile.o Label.o Lexer.o Parser.o Scanner.o SymTab.o Token.o 
WARNFLG = -Wall -Wextra
DEBUG = -g
STD = -std=c++11
CXXFLAGS += $(DEBUG) $(WARNFLG) $(STD)
CC = g++

lsca: $(OBJ) 
	$(CC) $(CXXFLAGS) $(OBJ) -o lsca
test: lsca
	./lsca ../test/baseTest.s
clear:
	rm -rf *.o
	rm -rf lsca