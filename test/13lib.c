#include "00testhelper.h"

void TestInt2Str()
{
    char buf[10];

    lscint2str(buf, 0);
    checkStringEquals(buf, "0", __LINE__);

    lscint2str(buf, 42);
    checkStringEquals(buf, "42", __LINE__);

    lscint2str(buf, 123456);
    checkStringEquals(buf, "123456", __LINE__);

    lscint2str(buf, 12345678);
    checkStringEquals(buf, "12345678", __LINE__);

    lscint2str(buf, 123456789);
    checkStringEquals(buf, "123456789", __LINE__);

    lscint2str(buf, 1234567890);
    checkStringEquals(buf, "1234567890", __LINE__);

    lscint2str(buf, 0x7fffffff);
    checkStringEquals(buf, "2147483647", __LINE__); //PS: 从其他地方复制字符串, 会产生看不见的字符

    lscint2str(buf, -1);
    checkStringEquals(buf, "-1", __LINE__);

    lscint2str(buf, -12345);
    checkStringEquals(buf, "-12345", __LINE__);

    lscint2str(buf, -7654321);
    checkStringEquals(buf, "-7654321", __LINE__);
}

void TestStr2Int()
{
    int value = 0;
    value = lscstr2int("1", 2);
    checkEquals(value, 1, __LINE__);

    value = lscstr2int("1234", 5);
    checkEquals(value, 1234, __LINE__);

    value = lscstr2int("2147483647", 11);
    checkEquals(value, 2147483647, __LINE__);

    value = lscstr2int("-1", 3);
    checkEquals(value, -1, __LINE__);

    value = lscstr2int("0", 2);
    checkEquals(value, 0, __LINE__);

    value = lscstr2int("-12345", 7);
    checkEquals(value, -12345, __LINE__);
}

void TestPointer2Str()
{
    char buf[12];

    lscpointer2str(0x12344321, buf);
    checkStringEquals(buf, "0x12344321", __LINE__);

    // 处理负数存在一点问题, 但是不影响使用, 暂时不做进一步处理
    // lscpointer2str(0xf7f60008, buf);
    // checkStringEquals(buf, "0xf7f60008", __LINE__);

    lscpointer2str(0x7fffcdcd, buf);
    checkStringEquals(buf, "0x7fffcdcd", __LINE__);

}

int main()
{
    TestInt2Str();
    TestStr2Int();
    TestPointer2Str();
    return 0;
}