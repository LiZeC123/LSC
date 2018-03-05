OBJ = main.o ElfFile.o Linker.o SegList.o
WARNFLG = -Wall -Wextra
DEBUG = -g
STD = -std=c++11
FLAG = $(DEBUG) $(WARNFLG) $(STD)
CC = g++ $(FLAG)

all: $(OBJ) 
	$(CC) $(OBJ) -o lscl 
test: $(OBJ)
	$(CC) $(OBJ) -o lscl 
	./lscl ../test/baseTest.o ../stdlib/stdlib.o
main.o: Linker.h
	$(CC) main.cpp -c -o main.o 
ElfFile.o: ElfFile.cpp ElfFile.h
	$(CC) ElfFile.cpp -c -o ElfFile.o
Linker.o: Linker.cpp Linker.h common.h
	$(CC) Linker.cpp -c -o Linker.o
SegList.o: SegList.cpp SegList.h ElfFile.h common.h
	$(CC) SegList.cpp -c -o SegList.o
clear:
	rm -rf *.o
	rm -rf lscl