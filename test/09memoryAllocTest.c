#include "00testhelper.h"

#define SIZE 26

int main()
{

    char *arr = lscmalloc(SIZE + 2);
    char *base = "abcdefghijklmnopqrstuvwxyz";

    int i = 0;
    while (i < SIZE)
    {
        arr[i] = 'a' + i;
        i++;
    }
    arr[SIZE] = '\0';

    checkStringEquals(arr,base,__LINE__);

    lscfree(arr);


    // 批量分配测试, 例如分配 1024 * 1KB 空间

    return 0;
}