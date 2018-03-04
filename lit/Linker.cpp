#include "Linker.h"
#include <string>

using namespace std;

void Linker::addElfFile(char* filename)
{
    ElfFile* file = new ElfFile();
    file->readElf(string(filename));
    fileList.push_back(file);
}

void Linker::printInfo()
{
    for(auto file:fileList){
        file->printInfo();
        printf("\n");
    }
}