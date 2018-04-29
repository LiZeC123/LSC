#include <iostream>
#include "PreProc.h"

using namespace std;
int main(int argc, const char* argv[])
{
    if(argc >= 2){
        // 获得待处理文件名
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

        // 获得标准库路径
        string stdPath(argv[2]);
        
        PreProc proc(cfile,ifile,stdPath);
        proc.anaylse();
    }

    return 0;
}