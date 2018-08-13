#include <lscio.h>
#include <lsclib.h>

int main()
{
    char* arr;
    lscPrintStr("Before malloc");
    arr = lscmalloc(10);
    lscPrintStr("After malloc");

    lscPrintStr("Before Write");
    int i = 0;
    while(i < 10){
        arr[i] = 'a'+i;
        i++;
    }
    arr[9] = '\0';
    lscPrintStr("After Write");

    lscPrintStr(arr);

    lscfree(arr);

    return 0;
}