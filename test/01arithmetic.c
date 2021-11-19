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

    c = e - b - a;
    d = e + b + a;
    checkEquals(c, 342, __LINE__);
    checkEquals(d, 1624, __LINE__);

    c = e + f * (a + b) - a + e / f;
    d = 983 + 283 * (155 + 486) - 155 + 983 / 283;

    checkEquals(c, 182234, __LINE__);
    checkEquals(d, 182234, __LINE__);
}
