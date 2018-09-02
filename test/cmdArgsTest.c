#include <lscio.h>

int main(int argc, char* argv[]){
	lscPrintStr("The Number of Args is ");
	lscPrintInt(argc); lscPrintStr(" ");

	lscPrintStr("The Args are: ");
	int i=0;
	while(i<argc){
		lscPrintStr(argv[i]);
		lscPrintStr(" ");
		i++;
	}
	lscPrintStr("\n");
	return 0;
}
