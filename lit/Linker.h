#pragma once
#include <vector>
#include "ElfFile.h"

class Linker
{
public:
    void addElfFile(char* filename);



    void printInfo();
private:
    std::vector<ElfFile*> fileList;

};