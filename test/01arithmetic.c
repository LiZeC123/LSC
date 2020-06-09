#include "00testhelper.h"

int main()
{
    checkEquals(8, 8, __LINE__);

    // 测试加法
    int a = 3;
    int b = 5;

    int c, d;

    c = a + b;
    d = 3 + 5;

    checkEquals(c, 8, __LINE__);
    checkEquals(d, 8, __LINE__);

    // 测试乘法
    a = 233;
    b = 332;
    c = a * b;
    d = 233 * 332;

    checkEquals(c, 77356, __LINE__);
    checkEquals(d, 77356, __LINE__);

    // 测试除法
    a = 6331;
    b = 3;
    c = a / b;
    d = 6331 / 3;

    checkEquals(c, 2110, __LINE__);
    checkEquals(d, 2110, __LINE__);

    // 测试混合运算
    a = 155;
    b = 486;
    int e = 983;
    int f = 283;

    c = e + f * (a + b) - a + e / f;
    d = 983 + 283 * (155 + 486) - 155 + 983 / 283;

    checkEquals(c, 182234, __LINE__);
    checkEquals(d, 182234, __LINE__);
}

// 测试代码思路
// 根据类似进行测试
/*
1. 数值计算
2. 逻辑计算
3. 数组操作
4. 基本结构测试(for, while, switch, if-else)
5. 函数调用测试
6. 宏测试
7. 指针测试
8. 结构体测试
9. 动态内存分配测试
10.数据结构测试
11.综合测试 ==> 例如比较直接求和与公式求和结果是否一致


/home/lizec/C/LSC/./test/04statement.c <第18行> checkEquals: 5040 != 50401
==> 把.去了  ./ ==> 空
*/