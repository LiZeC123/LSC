OBJ = lscore.o lscio.o

CA = ../ass/lsca
CC = ../compile/lscc
CFLAG = -O2


lib:$(OBJ)

lscio.o: lscio.c
	$(CC) lscio.c $(CFLAG)
	$(CA) lscio.s	
lscore.o: lscore.s
	$(CA) lscore.s
clear:
	rm -rf *.o
	rm -rf lscio.s
