#include <lscio.h>

int main(int argc, char* argv[]){
	lscPrintStr("The Number of Args is ");
	lscPrintInt(argc); lscPrintStr("\n");

	lscPrintStr("The Args are:\n");
	int i=0;
	while(i<argc){
		lscPrintStr(argv[i]);
		lscPrintStr("\n");
		i++;
	}

	return 0;
}
