#include <stdio.h>
#include "compile/common.h"

FILE * fin;

int main(){
    fin = fopen("/home/lizec/CWorkSpace/lsc/main.c","r");
    char c;
    while((c = scan()) != -1){
        printf("%c",c);
    }
    return 0;
}
