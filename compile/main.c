#include <stdio.h>
#include <stdlib.h>
#include "common.h"

FILE *fin;
char * filename = "main.c";
int main()
{
    //fin = fopen("/home/lizec/CWorkSpace/lsc/errorTest.c", "r");
    fin = fopen("/home/lizec/CWorkSpace/lsc/test/main.c","r");
    showToken = 1; 
    program();

    return 0;
}

