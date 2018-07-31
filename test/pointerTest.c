#include <lscio.h>

void exit()
{
    // 使用除零错误来强制结束程序
    // 后续考虑使用系统调用实现此函数
    int d = 3/0;
}

// TODO: 添加行号宏,从而实现定位
void checkEquals(int a,int b)
{
    if(a != b){
        lscPrintStr("checkEquals:");lscPrintInt(a);lscPrintStr(" != ");lscPrintInt(b);lscPrintStr("\n");
        exit();
    }
}

void checkStrEquals(char a,char b)
{
    if(a != b){
        lscPrintStr("checkStrEquals: char not equal\n");
        exit();
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
    checkEquals(value,v2);

    v2 = *pv;
    checkEquals(value,v2);


    
    c = 'c';
    pc = &c;
    ppc = &pc;

    char c2;
    c2 = **ppc;
    checkStrEquals(c2,c);

    c2 = *pc;
    checkStrEquals(c2,c);

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
    checkEquals(d,9843);

    *pd = 9721;
    checkEquals(d,9721);


    char c;
    char* pc = &c;
    char** ppc = &pc;

    **ppc = 'c';
    checkStrEquals(c,'c');

    *pc = 'e';
    checkStrEquals(c,'e');
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
