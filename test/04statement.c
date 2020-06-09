#include "00testhelper.h"

int main()
{
    int acc = 0;
    int count = 10;
    while (count-- > 0)
    {
        acc = acc + 1;
    }
    checkEquals(acc, 10, __LINE__);
    
/*    
    for (int i = 1; i <= 12; i++)
    {
        acc = acc + i;
    }
    checkEquals(acc,5050,__LINE__);

*/  
    return 0;
}