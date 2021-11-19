#include <stdio.h>
#include <stdlib.h>

void checkEquals(int a, int b, int line)
{
    if (a != b)
    {
        printf("%s<第%d行> checkEquals:%d!=%d\n", __FILE__, line, a, b);
        exit(1);
    }
}

void checkCharEquals(char a, char b, int line)
{
    if (a != b)
    {
        printf("%s<第%d行> checkCharEquals: char not equal\n\n", __FILE__, line);
        exit(1);
    }
}

void checkStringEquals(char *strA, char *strB, int line)
{
    char *strAp = strA;
    char *strBp = strB;
    int equal = 1;
    while (*strA != '\0' && *strB != '\0')
    {
        if (*strA != *strB)
        {
            equal = 0;
        }
        strA++;
        strB++;
    }

    if (equal)
    {
        equal = (*strA == '\0' && *strB == '\0');
    }

    if (!equal)
    {
        printf("%s<第%d行> checkStringEquals: %s!=%s\n", __FILE__, line, strA, strB);
        exit(1);
    }
}

void checkTrue(int value, int line)
{
    if (value)
    {
        return;
    }
    else
    {
        printf("%s<第%d行> checkTrue: expression is not True\n", __FILE__, line);
        exit(1);
    }
}

void checkFalse(int value, int line)
{
    if (value)
    {
        printf("%s<第%d行> checkFalse: expression is not False\n", __FILE__, line);
        exit(1);
    }
}

void checkUnreachable(int line)
{
    printf("%s<第%d行> Unreachable Code Excuted\n", __FILE__, line);
    exit(1);
}
