#include "00testhelper.h"

void BaseTest()
{
    int arr[3];
    arr[0] = 101;
    arr[1] = 111;
    arr[2] = 121;

    checkEquals(arr[0] + arr[1] + arr[2], 333, __LINE__);
}

void TestRead()
{
    char crr[6] = {'a', 'b', 'c', 'd', 'e', 'f'};
    checkCharEquals(crr[0], 'a', __LINE__);
    checkCharEquals(crr[1], 'b', __LINE__);
    checkCharEquals(crr[2], 'c', __LINE__);
    checkCharEquals(crr[3], 'd', __LINE__);
    checkCharEquals(crr[4], 'e', __LINE__);
    checkCharEquals(crr[5], 'f', __LINE__);

    int irr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    checkEquals(irr[0], 0, __LINE__);
    checkEquals(irr[1], 1, __LINE__);
    checkEquals(irr[3], 3, __LINE__);
    checkEquals(irr[7], 7, __LINE__);
    checkEquals(irr[9], 9, __LINE__);
}

void testWrite()
{
    char crr[6] = {'a', 'b', 'c', 'd', 'e', 'f'};
    crr[0] = 'z';
    crr[1] = 'y';
    crr[4] = 'x';

    checkCharEquals(crr[0], 'z', __LINE__);
    checkCharEquals(crr[1], 'y', __LINE__);
    checkCharEquals(crr[2], 'c', __LINE__);
    checkCharEquals(crr[3], 'd', __LINE__);
    checkCharEquals(crr[4], 'x', __LINE__);
    checkCharEquals(crr[5], 'f', __LINE__);

    int irr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    irr[2] = 233;
    irr[5] = 1902;
    checkEquals(irr[0], 0, __LINE__);
    checkEquals(irr[2], 233, __LINE__);
    checkEquals(irr[3], 3, __LINE__);
    checkEquals(irr[5], 1902, __LINE__);
    checkEquals(irr[9], 9, __LINE__);
}

int main()
{
    BaseTest();
    TestRead();
    testWrite();

    // 添加对指针数组的测试


    return 0;
}