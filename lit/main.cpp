#include <iostream>
#include "Linker.h"


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

    linker.printInfo();

    return 0;
}