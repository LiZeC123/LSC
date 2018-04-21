#include <iostream>
#include "PreProc.h"

using namespace std;
int main(int argc, const char* argv[])
{
    if(argc >= 2){
        string cfile(argv[1]);
        string ifile = "./cii.i";
        PreProc proc(cfile,ifile);
        proc.anaylse();
    }

    return 0;
}