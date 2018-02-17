#pragma once
#include <map>
#include <vector>
#include <string>
#include "Label.h"
#include "ElfFile.h"

class SymTab
{
public:
    static int dataLen;
    static ElfFile elfile;
    // SymTab();
    bool hasName(std::string name);
    void addLabel(Label* label);
    Label* getLabel(std::string name);
    void begSeg(std::string segName);
    void endSeg();
    void exportSyms();
    void printSymbolTable();
private:
    std::map<std::string,Label*> symTab;
    std::vector<Label*> defTab; 
};