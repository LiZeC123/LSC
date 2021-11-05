#include "00testhelper.h"

#define SIZE 26

void TestBase()
{
    char *arr = lscmalloc(SIZE + 2);
    char *base = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < SIZE; i++)
    {
        arr[i] = 'a' + i;
    }
    arr[SIZE] = '\0';

    checkStringEquals(arr, base, __LINE__);

    lscfree(arr);
}

#define T_SIZE 1024

void TestMany()
{
    char *arr[T_SIZE];

    for (int i = 0; i < T_SIZE; i++)
    {
        // 测试大批量分配
        arr[i] = lscmalloc(T_SIZE);
    }

    for (int i = 0; i < T_SIZE; i++)
    {
        // 测试对分配的内存写入数据
        char *m = arr[i];
        m[i] = i % 256;
    }

    for (int i = 0; i < T_SIZE; i++)
    {
        // 测试从分配的内存读取数据
        char *m = arr[i];
        char v = i % 256;
        checkCharEquals(m[i], v, __LINE__);
    }

    for (int i = 0; i < T_SIZE; i++)
    {
        // 测试释放
        lscfree(arr[i]);
    }
}

char* AllocFunction()
{
    char *arr = lscmalloc(SIZE + 2);
    for (int i = 0; i < SIZE; i++)
    {
        arr[i] = 'a' + i;
    }
    arr[SIZE] = '\0';
    return arr;
}

void TestCrossFunction()
{
    // 测试离开作用域后内存是否能够使用
    char* arr = AllocFunction();
    char *base = "abcdefghijklmnopqrstuvwxyz";
    checkStringEquals(arr, base, __LINE__);
    lscfree(arr);
}

int main()
{
    TestBase();
    TestMany();
    TestCrossFunction();
    return 0;
}