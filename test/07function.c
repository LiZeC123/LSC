#include "00testhelper.h"

int Final_Answer = 42;
char X_Char = 'X';
int printf(char *str, int values, ...);

int f1()
{
    return Final_Answer;
}

int f2(int x, int y)
{
    return Final_Answer - x - y;
}

int f3(int op, int x, int y)
{
    if (op == 0)
    {
        return x + y;
    }

    if (op == 1)
    {
        return x - y;
    }

    if (op == 2)
    {
        return x * y;
    }
}

char f4()
{
    return X_Char;
}

void BaseTest()
{
    int f = f1();
    int g = f2(42, -1);
    checkEquals(f, 42, __LINE__);
    checkEquals(g, 1, __LINE__);
    checkEquals(f + g, 42 + 1, __LINE__);

    checkEquals(f3(0, 100, 2), 102, __LINE__);
    checkEquals(f3(1, 100, 2), 98, __LINE__);
    checkEquals(f3(2, 100, 2), 200, __LINE__);

    checkCharEquals(f4(), X_Char, __LINE__);
}

int partSum(int a[], int n)
{
    int sum = 0;
    while (n-- > 0)
    {
        sum = sum + a[n];
    }
    return sum;
}

int partSumP(int *a, int n)
{
    int sum = 0;
    while (n-- > 0)
    {
        sum = sum + a[n];
    }
    return sum;
}

void TestArray()
{
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    checkEquals(partSum(a, 10), 55, __LINE__);
    checkEquals(partSumP(a, 10), 55, __LINE__);
}

int countStrLen(char *str)
{
    int count = 0;
    while (*str++ != '\0')
    {
        count++;
    }
    return ++count;
}

int countStrArrLen(char *strings[], int n)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        count = count + countStrLen(strings[i]);
    }
    return count;
}

void TestStringArray()
{
    char *strings[3] = {"Lands apart, sky shared.", "Covid-19", "Tomorrow is another day."};
    checkEquals(countStrArrLen(strings, 3), 59, __LINE__);
}

#define NULL 0
int *findMax(int a[], int n)
{
    int max = 0;
    int *loc = a;
    for (int i = 0; i < n; i++)
    {
        if (a[i] > max)
        {
            max = a[i];
            loc = &a[i];
        }
    }
    return loc;
}

void TestReturnPoint()
{
    int a[3] = {0, 1, 2};
    int *loc = findMax(a, 3);
    //checkEquals(lscp2i(loc), lscp2i(&a[1]), __LINE__);
}

int gcd(int x, int y)
{
    if (x % y == 0)
    {
        return y;
    }
    else
    {
        return gcd(y, x % y);
    }
}

void TestRecursive()
{
    checkEquals(gcd(49, 28), 7, __LINE__);
}

int main()
{
    BaseTest();

    TestArray();
    TestStringArray();

    TestReturnPoint();

    TestRecursive();

    return 0;
}

int printf(char *str, int values, ...)
{
    int *p = &values;
    int begin = 0;
    int end = 0;
    int vCount = 0;
    while (str[end] != '\0')
    {
        if (str[end] == '{')
        {

            str[end] = '\0';
            lscPrintStr(str + begin);
            lscPrintInt(*(p + vCount));
            str[end] = '{'; // 字符串是引用, 不能随便修改, 否则会影响外部
            end = end + 2;
            begin = end;
            vCount++;
        }
        else
        {
            end++;
        }
    }

    if (begin != end)
    {
        // 输出最后一部分的字符
        lscPrintStr(str + begin);
    }
}