#pragma once
#include <map>
#include <vector>
#include <string>
#include "Label.h"
#include "ElfFile.h"

class SymTab
{
public:
    SymTab();
    bool hasName(std::string name);
    void addLabel(Label* label);
    Label* getLabel(std::string name);
    void begSeg(std::string segName);
    void endSeg();
    void exportSyms();
    void printSymbolTable();

    ElfFile& getFile();
    void setRelLabel(Label* label);
    Label* getRelLabel();
private:
    std::map<std::string,Label*> symTab;
    std::vector<Label*> defTab; 

    int dataLen;
    ElfFile elfile;
    Label* relLabel;    // 临时保存可能的重定位项
};