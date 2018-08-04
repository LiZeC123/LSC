OBJ = main.o ElfFile.o Linker.o SegList.o
WARNFLG = -Wall -Wextra
DEBUG = -g
STD = -std=c++11
CXXFLAGS += $(DEBUG) $(WARNFLG) $(STD)
CC = g++

lscl: $(OBJ) 
	$(CC) $(CXXFLAGS) $(OBJ) -o lscl 
test: $(OBJ)
	./lscl ../test/baseTest.o ../stdlib/stdlib.o ../stdlib/start.o
clear:
	rm -rf *.o
	rm -rf lscl