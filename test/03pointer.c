#include "00testhelper.h"

void stringArrayTest()
{
    // 字符串数组有关运算测试
    char *name1 = "Hello ";
    char *name2 = "World ";
    char *name3 = "LSC   ";

    char *nameArray[3];

    nameArray[0] = name1;
    nameArray[1] = name2;
    nameArray[2] = name3;

    char **arr = nameArray;

    char *p1 = *arr;
    checkStringEquals(p1, "Hello ", __LINE__);

    char *p2 = *(arr + 1);
    checkStringEquals(p2, "World ", __LINE__);

    char *p3 = nameArray[2];
    checkStringEquals(p3, "LSC   ", __LINE__);
}

void mutilPointerReadTest()
{
    int value = 1234;
    int *pv = &value;
    int **ppv = &pv;

    int v2;
    v2 = **ppv;
    checkEquals(v2, 1234, __LINE__);

    value = 4321;
    v2 = *pv;
    checkEquals(v2, 4321, __LINE__);


    char c = 'c';
    char *pc = &c;
    char **ppc = &pc;

    char c2;
    c2 = **ppc;
    checkCharEquals(c2, 'c', __LINE__);

    c = 'd';
    c2 = *pc;
    checkCharEquals(c2, 'd', __LINE__);
}

void mutilPointerWriteTest()
{
    int d;
    int *pd = &d;
    int **ppd = &pd;

    **ppd = 9843;
    checkEquals(d, 9843, __LINE__);

    *pd = 9721;
    checkEquals(d, 9721, __LINE__);

    char c;
    char *pc = &c;
    char **ppc = &pc;

    **ppc = 'c';
    checkCharEquals(c, 'c', __LINE__);

    *pc = 'e';
    checkCharEquals(c, 'e', __LINE__);
}

int arrayTest()
{
    char *arr[3];
    char base = 'a';
    char *pc = &base;
    
    arr[0] = pc;
    arr[1] = *(&pc);
     *(arr + 2) = pc;

    checkCharEquals(*arr[0],'a',__LINE__);
    
    base = 'b';
    checkCharEquals(*(*(arr+1)),'b',__LINE__);
    
    base = 'c';
    checkCharEquals(*arr[2],'c',__LINE__);
}

void sizeOfTest()
{
    sizeof(int);    // 空sizeof语句测试

    char arr[7];
    checkEquals(sizeof("123456"), 7, __LINE__);
    checkEquals(sizeof(arr), 7, __LINE__);
}

int main(int argc, char *argv[])
{

    stringArrayTest();

    mutilPointerReadTest();

    mutilPointerWriteTest();

    arrayTest();

    sizeOfTest();

    return 0;
}
