#include "../stdlib/lscio.h"
#include "../stdlib/lsclib.h"


void checkTrue(int value,int line)
{
    if(value){
        return;
    }
    else{
        lscPrintStr(__FILE__);
        lscPrintStr(" <第");lscPrintInt(line);lscPrintStr("行> ");
        lscPrintStr("checkTrue: is not True");
        exit(1);
    }
}

void checkFalse(int value,int line)
{
    if(value){
        lscPrintStr(__FILE__);
        lscPrintStr(" <第");lscPrintInt(line);lscPrintStr("行> ");
        lscPrintStr("checkFalse: is not False");
        exit(1);
    }
}

int main()
{
    int true = 1;
    int false = 0;

    checkTrue(true,__LINE__);
    checkFalse(false,__LINE__);

    checkTrue(true&&true,__LINE__);
    checkFalse(true&&false,__LINE__);

    checkTrue(true||false,__LINE__);
    checkFalse(false||false,__LINE__);

    checkTrue(!false,__LINE__);
    checkFalse(!true,__LINE__);

    return 0;
}


