#pragma once
#include <vector>
#include <map>
#include <string>
#include "ElfFile.h"
#include "SegList.h"

using std::string;
class Linker
{
public:


    Linker();
    void addElfFile(char* filename);
    void allocAddr();
    void collectInfo();
    bool symValid();
    void symParser();
    void relocate();
    void printInfo();
private:

    std::vector<string> segNames;
    std::vector<ElfFile*> fileList;
    std::map<string,SegList*> segLists;

    std::vector<SymLink*> symLinks; // 引用符号集
    std::vector<SymLink*> symDef;   // 定义符号集
};