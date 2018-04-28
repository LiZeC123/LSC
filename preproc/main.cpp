#include <iostream>
#include "PreProc.h"

using namespace std;
int main(int argc, const char* argv[])
{
    if(argc == 2){
        string cfile(argv[1]);
        string ifile(argv[1]);
        std::size_t pos = ifile.rfind(".c");
        if (pos > 0 && pos == ifile.length() - 2)
        {
            ifile.replace(pos, 2, ".i");
        }
        else
        {
            ifile = ifile + ".i";
        }
        
        PreProc proc(cfile,ifile);
        proc.anaylse();
    }

    return 0;
}