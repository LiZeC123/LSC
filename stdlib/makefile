OBJ = start.o stdlib.o

CC = ../ass/lsca


lib:$(OBJ)
	echo "Finish" > lib
start.o: start.s
	$(CC) start.s
stdlib.o: stdlib.s
	$(CC) stdlib.s
clear:
	rm -rf *.o