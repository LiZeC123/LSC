#include <lscio.h>
#include <lsclib.h>

void checkEquals(int a,int b,int line)
{
    if(a != b){
        lscPrintStr(__FILE__);
        lscPrintStr(" <第");lscPrintInt(line);lscPrintStr("行> ");
        lscPrintStr("checkEquals: ");lscPrintInt(a);lscPrintStr(" != ");lscPrintInt(b);lscPrintStr("\n");
        exit(1);
    }
}

void checkCharEquals(char a,char b,int line)
{
    if(a != b){
        lscPrintStr(__FILE__);
        lscPrintStr(" <第");lscPrintInt(line);lscPrintStr("行> ");
        lscPrintStr("checkCharEquals: char not equal\n");
        exit(1);
    }
}

void checkStringEquals(char* strA, char* strB, int line)
{
    char* strAp = strA;
    char* strBp = strB;
    int equal = 1;
    while (*strA != '\0' && *strB != '\0')
    {
        if (*strA != *strB)
        {
            equal = 0;
        }
        strA++;
        strB++;
    }
    
    if(equal) {
        equal = (*strA == '\0' && *strB == '\0');
    }
    

    if(!equal){
        lscPrintStr(__FILE__);
        lscPrintStr(" <第");lscPrintInt(line);lscPrintStr("行> ");
        lscPrintStr("checkStringEquals: ");lscPrintStr(strAp);lscPrintStr(" != ");lscPrintStr(strBp);
        lscPrintStr("\n");
        exit(1);
    }
}

void checkTrue(int value,int line)
{
    if(value){
        return;
    }
    else{
        lscPrintStr(__FILE__);
        lscPrintStr(" <第");lscPrintInt(line);lscPrintStr("行> ");
        lscPrintStr("checkTrue: expression is not True");
        exit(1);
    }
}

void checkFalse(int value,int line)
{
    if(value){
        lscPrintStr(__FILE__);
        lscPrintStr(" <第");lscPrintInt(line);lscPrintStr("行> ");
        lscPrintStr("checkFalse: expression is not False");
        exit(1);
    }
}

void checkUnreachable(int line)
{
    lscPrintStr(__FILE__);
    lscPrintStr(" <第");lscPrintInt(line);lscPrintStr("行> ");
    lscPrintStr("Unreachable Code Excuted");
    exit(1);
}
