#include "00testhelper.h"

// 1. 素数测试 ==> 一百以内的素数
#define true 1
#define false 0
#define bool int

int primeTest();

int main()
{
    primeTest();
    return 0;
}

int square(int x)
{
    return x * x;
}

int sqrt_i(int n)
{
    int left = 1;
    int right = n;
    int mid = (left + right) / 2;

    while (left < right)
    {
        int s = square(mid);
        if (s > n)
        {
            right = mid - 1;
        }
        else if (s < n)
        {
            left = mid + 1;
        }
        else
        {
            return mid;
        }
        mid = (left + right) / 2;
    }

    return mid; 
}

bool isPrime(int n)
{
    for (int i = 2; i <= sqrt_i(n); i++)
    {
        if (n % i == 0)
        {
            return false;
        }
    }

    return true;
}

int primeTest()
{
    int total = 1229;
    int count = 1;
    for (int i = 3; i < 10000; i++)
    {
        if (isPrime(i))
        {
            ++count;
        }
    }
    checkEquals(total, count, __LINE__);
    
}
