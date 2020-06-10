#include "00testhelper.h"

void TestLoopBase()
{
    int acc = 0;
    int count = 10;
    while (count-- > 0)
    {
        acc = acc + 1;
    }
    checkEquals(acc, 10, __LINE__);

    acc = 0;
    for (int i = 1; i <= 100; i++)
    {
        acc = acc + i;
    }
    checkEquals(acc, 5050, __LINE__);
}

void TestIfBase()
{
    int acc = 0;
    if (acc == 0)
    {
        checkTrue(acc == 0, __LINE__);
    }

    if (acc == 0)
    {
        checkTrue(acc == 0, __LINE__);
    }
    else
    {
        checkUnreachable(__LINE__);
    }

    if (acc == 1)
    {
        checkUnreachable(__LINE__);
    }
    else
    {
        checkTrue(acc == 0, __LINE__);
    }

    acc = 1;
    if (acc == 0)
    {
        checkUnreachable(__LINE__);
    }
    else if (acc == 1)
    {
        checkEquals(acc, 1, __LINE__);
    }
    else if (acc == 2)
    {
        checkUnreachable(__LINE__);
    }
    else
    {
        checkUnreachable(__LINE__);
    }

    acc = 12;
    if (acc == 0)
    {
        checkUnreachable(__LINE__);
    }
    else if (acc == 1)
    {
        checkUnreachable(__LINE__);
    }
    else if (acc == 2)
    {
        checkUnreachable(__LINE__);
    }
    else
    {
        checkEquals(acc, 12, __LINE__);
    }
}

void TestExpression()
{
    int i = 0, acc = 0;

    for (; i < 10; i++)
    {
        acc = acc + 1;
    }
    checkEquals(acc, 10, __LINE__);

    i = 0;
    for (; i < 10;)
    {
        acc = acc + 1;
        i++;
    }
    checkEquals(acc, 20, __LINE__);

    int count = 0;
    for (i = 0, acc = 0; i <= 10; i++, count++)
    {
        acc = acc + count;
    }
    checkEquals(acc, 55, __LINE__);
}

void TestControl()
{
    for (int i = 0; i < 10; i++)
    {
        if (i == 0)
        {
            continue;
        }
        if (i == 3)
        {
            break;
        }
        // 如果没有及时continue, 则下面的条件会成立
        checkTrue(i != 0, __LINE__);
        // 如果没有及时break, 则下面的条件会成立
        checkTrue(i < 3, __LINE__);
    }

    int i = 0;
    while (i++ < 10)
    {
        if (i == 0)
        {
            continue;
        }
        if (i == 3)
        {
            break;
        }
        // 如果没有及时continue, 则下面的条件会成立
        checkTrue(i != 0, __LINE__);
        // 如果没有及时break, 则下面的条件会成立
        checkTrue(i < 3, __LINE__);
    }
}

void TestMultiLoop()
{
    int count = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            count++;
        }
    }
    checkEquals(count, 100, __LINE__);

    int i = 0, j = 0;
    count = 0;
    while (i < 10)
    {
        j = 0;
        while (j < 10)
        {
            count++;
            j++;
        }
        i++;
    }
    checkEquals(count, 100, __LINE__);
}

void TestMutilLoopControl()
{
    int count = 0;
    for (int i = 0; i < 10; i++)
    {
        if (i == 2)
        {
            continue;
        }

        for (int j = 0; j < 10; j++)
        {
            if (j % 2 == 0)
            {
                continue;
            }
            if (j == 5)
            {
                break;
            }

            checkTrue(j % 2 != 0, __LINE__);
            checkTrue(j < 5, __LINE__);
        }

        if (i == 8)
        {
            break;
        }

        checkTrue(i != 8, __LINE__);
        checkTrue(i < 8, __LINE__);
    }
}

int main()
{
    TestLoopBase();
    TestIfBase();
    TestExpression();
    TestControl();
    TestMultiLoop();
    TestMutilLoopControl();

    return 0;
}