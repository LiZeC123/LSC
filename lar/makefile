OBJ = main.o BinTools.o Coder.o Huffman.o 
WARNFLG = -Wall -Wextra
DEBUG = -g
STD = -std=c++11
CXXFLAGS += $(DEBUG) $(WARNFLG) $(STD)
CC = g++

lar: $(OBJ) 
	$(CC) $(CXXFLAGS) $(OBJ) -o lar
test: lar
	./lscc ../test/baseTest.c
clear:
	rm -rf *.o
	rm -rf *.lar
	rm -rf lar
