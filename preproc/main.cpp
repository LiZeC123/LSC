#include <iostream>
#include "PreProc.h"

using namespace std;
int main()
{
    string cfile = "../test/testMacro.c";
    string ifile = "./cii.i";
    PreProc proc(cfile,ifile);

    proc.anaylse();


    return 0;
}