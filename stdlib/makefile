OBJ = start.o lscio.o lsclib.o

CC = ../ass/lsca


lib:$(OBJ)
	echo "Finish" > lib
start.o:start.s
	$(CC) start.s
lscio.o: lscio.s
	$(CC) lscio.s
lsclib.o: lsclib.s
	$(CC) lsclib.s
clear:
	rm -rf *.o