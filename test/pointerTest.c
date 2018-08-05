#include "../stdlib/lscio.h"
#include "../stdlib/lsclib.h"

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

void stringArrayTest()
{
    // 字符串有关运算测试
    char* name1 = "Lsc ";
    char* name2 = "iS Compile\n";
    char* name3 = "He";

    char* nameArray[3];

    nameArray[0] = name1;
    nameArray[1] = name2;
    nameArray[3] = name3;

    char** arr = nameArray;

    char* p1 = *arr;
    lscPrintStr(p1);

    char* p2 = *(arr+1);
    lscPrintStr(p2);
}


void mutilPointerReadTest(){
    int value;
    int* pv;
    int** ppv;

    char c;
    char* pc;
    char** ppc;

    pv = &value;
    ppv  = &pv;

    int v2;
    v2 = **ppv;
    checkEquals(value,v2,__LINE__);

    v2 = *pv;
    checkEquals(value,v2,__LINE__);


    
    c = 'c';
    pc = &c;
    ppc = &pc;

    char c2;
    c2 = **ppc;
    checkCharEquals(c2,c,__LINE__);

    c2 = *pc;
    checkCharEquals(c2,c,__LINE__);

    pc = *ppc;

    c = *pc;
    c = **ppc;

    char d;
    *pc = d;
    **ppc = d;
}


void mutilPointerWriteTest()
{
    int d;
    int* pd = &d;
    int** ppd = &pd;

    **ppd = 9843;
    checkEquals(d,9843,__LINE__);

    *pd = 9721;
    checkEquals(d,9721,__LINE__);


    char c;
    char* pc = &c;
    char** ppc = &pc;

    **ppc = 'c';
    checkCharEquals(c,'c',__LINE__);

    *pc = 'e';
    checkCharEquals(c,'e',__LINE__);
}

int arrayTest() {
    char* arr[5];
    char* pc;
    arr[0] = pc;
    arr[1] = *(&pc);


    char* pc2;
    pc2 = arr[0];

    *(arr+2) = pc2;
}


int main(int argc, char* argv[]){

    stringArrayTest();

    mutilPointerReadTest();

    mutilPointerWriteTest();

    arrayTest();

    return 0;
}
