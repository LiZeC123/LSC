#include <lscio.h>
#include <lsclib.h>

#define SIZE 26

int main()
{
    char* arr = lscmalloc(SIZE+2);

    int i = 0;
    while(i < SIZE){
        arr[i] = 'a'+i;
        i++;
    }
    arr[SIZE]   = '\n';
    arr[SIZE+1] = '\0';

    lscPrintStr(arr);

    lscfree(arr);

    return 0;
}