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
    bool link(const char* filename);
    void addElfFile(char* filename);
    void allocAddr();
    void collectInfo();
    bool symValid();
    void symParser();
    void relocate();
    void assemExe();
    void exportElf(const char* filename);
    void printInfo();

    std::vector<string> getSegNames();
    std::vector<SymLink*> getSymDef();
    std::map<string,SegList*> getSegLists();
private:

    std::vector<string> segNames;
    std::vector<ElfFile*> fileList;
    std::map<string,SegList*> segLists;

    std::vector<SymLink*> symLinks; // 引用符号集
    std::vector<SymLink*> symDef;   // 定义符号集

    ElfFile* genFile;               // 用于生成最后可执行文件的Elf类
};