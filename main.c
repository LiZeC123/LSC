#include <stdio.h>
#include "compile/common.h"


int main(){
    FILE* file = fopen("/home/lizec/CWorkSpace/lsc/main.c","r");
    char c;
    while((c = scan(file)) != -1){
        printf("%c",c);
    }
    return 0;
}
