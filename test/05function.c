#include "00testhelper.h"

int Final_Answer = 42;

int f1()
{
    return Final_Answer;
}

int f2(int op, int x, int y)
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

// fixme: 目前不支持数组初始化, 因此这个接口不好用
void printf(char *s, int *arr, int num)
{
    return;
}

int partSum(int *a, int n)
{
    int sum = 0;
    while (n-- > 0)
    {
        sum = sum + a[n];
    }

    return sum;
}

int main()
{
    int a[100];
    int idx = 0;
    while (idx < 100)
    {
        a[idx] = idx + 1;
        idx++;
    }

    checkEquals(partSum(a, 100), 5050, __LINE__);

    checkEquals(f1(), 42, __LINE__);

    return 0;
}