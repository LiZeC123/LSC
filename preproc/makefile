OBJ = main.o Error.o PreProc.o
WARNFLG = -Wall -Wextra
DEBUG = -g
STD = -std=c++11
CXXFLAGS += $(DEBUG) $(WARNFLG) $(STD)
CC = g++

lscp: $(OBJ) 
	$(CC) $(CXXFLAGS) $(OBJ) -o lscp 
test: lscp
	./lscp ../test/testMacro.c
clear:
	rm -rf *.o
	rm -rf lscp
