#include <lscio.h>
#include <lsclib.h>

int main()
{
    char* arr;

    arr = lscmalloc(28);

    int i = 0;
    while(i < 26){
        arr[i] = 'a'+i;
        i++;
    }
    arr[26] = '\n';
    arr[27] = '\0';

    lscPrintStr(arr);

    lscfree(arr);

    return 0;
}