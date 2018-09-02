#include <iostream>
#include "Linker.h"

using namespace std;

class Args{
public:
    bool printInfo = false;
};

void analyseOptions(int argc, char* argv[], Args& args, Linker& linker)
{
    for(int i=1; i < argc; i++){
        string option(argv[i]);
        if(option == "--printLinkInfo"){
            args.printInfo = true;
        }
        else{
            linker.addElfFile(argv[i]);
        }
    }
}

void printInfo(Args& args, Linker& linker)
{
    if(args.printInfo){
        linker.printInfo();
    }
}



int main(int argc,char* argv[])
{
    if(argc == 1){
        printf("lscl: 输入文件不能为空");
        return 0;
    }
    
    Linker linker = Linker();
    Args args;

    analyseOptions(argc,argv,args,linker);

    const char* outName = "z.out";
    linker.link(outName);

    printInfo(args,linker);

    return 0;
}