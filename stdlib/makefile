OBJ = start.o lscore.o lscio.o lsclib.o

CA = ../ass/lsca
CC = ../compile/lscc


lib:$(OBJ)

start.o:start.s
	$(CA) start.s
lscio.o: lscio.c
	$(CC) lscio.c
	$(CA) lscio.s	
lsclib.o: lsclib.s
	$(CA) lsclib.s
lscore.o: lscore.s
	$(CA) lscore.s
clear:
	rm -rf *.o
	rm -rf lscio.s
