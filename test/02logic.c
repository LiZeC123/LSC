#include "00testhelper.h"

int main()
{
    int true = 1;
    int false = 0;

    checkTrue(true, __LINE__);
    checkFalse(false, __LINE__);

    checkTrue(true && true, __LINE__);
    checkFalse(true && false, __LINE__);

    checkTrue(true || false, __LINE__);
    checkFalse(false || false, __LINE__);

    checkTrue(!false, __LINE__);
    checkFalse(!true, __LINE__);

    int a = 1;
    int b = -1;
    checkTrue(1 == 1, __LINE__);
    checkFalse(1 == 2, __LINE__);
    checkTrue(a == a, __LINE__);
    checkFalse(a == b, __LINE__);

    checkTrue(a > b, __LINE__);
    checkTrue(a >= b, __LINE__);
    checkFalse(a > a, __LINE__);
    checkTrue(a >= a, __LINE__);

    checkTrue(1 == 1 && 2 == 2 && 2 > 1, __LINE__);

    return 0;
}
