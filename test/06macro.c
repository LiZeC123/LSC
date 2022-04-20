#include "00testhelper.h"

#define MAX_SIZE 10
#define LESS_THAN <=
#define C1 (1 + 2)
#define C3 (C2 * C2 * C1)
#define C2 (C1 + C1)
#define FALSE_CONDITION 0
#define WELCOME "Welcome to LSC"

int strlen(char *str)
{
    char *p = str;
    int count = 0;
    while (*p++ != '\0')
    {
        count++;
    }

    return ++count;
}

#define LEN strlen
#define LEN_TEST strlen("123")

int main()
{
    int a[MAX_SIZE];
    a[0] = __LINE__;

    checkTrue(a[0] LESS_THAN __LINE__, __LINE__);

    checkEquals(C1 + C1, 6, __LINE__);
    checkEquals(C1 * C1, 9, __LINE__);
    checkEquals(C3, 108, __LINE__);
    checkEquals(C3 + C2 + C1, 108 + 6 + 3, __LINE__);

    if (FALSE_CONDITION)
    {
        // 如果代码被执行, 则条件判断出现错误
        checkUnreachable(__LINE__);
    }

    char *str = WELCOME;
    checkEquals(strlen(str), 15, __LINE__);
    checkEquals(LEN(WELCOME), 15, __LINE__);
    checkEquals(LEN_TEST, 4, __LINE__);
}
