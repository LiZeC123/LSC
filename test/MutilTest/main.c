#include "fun.h"
#include <lscio.h>

int main(int argc, char* argv)
{
    int c = funAdd(argc,10);
    int d = funMul(c,1000);
    lscPrintStr("(argv+10)*1000=");
    lscPrintInt(d);
    lscPrintStr("\n");
}