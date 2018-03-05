#include <iostream>
#include "Linker.h"

using namespace std;

int main(int argc,char* argv[])
{
    if(argc == 1){
        return 0;
    }
    // 需要编译的文件是 argv[1] ~ argv[argc-1]
    
    Linker linker = Linker();

    for(int i=1;i<argc;i++){
        linker.addElfFile(argv[i]);
    }

    const char* outName = "z.out";
    bool success = linker.link(outName);

    linker.printInfo();

    if(success){
        cout << "链接成功" << endl;
    }
    else{
        cout << "链接失败" << endl;
    }

    return 0;
}