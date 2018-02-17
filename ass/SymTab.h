#pragma once
#include <map>
#include <vector>
#include <string>
#include "Label.h"


class SymTab
{
public:
    static int dataLen;
    // SymTab();
    bool hasName(std::string name);
    void addLabel(Label* label);
    Label* getLabel(std::string name);
    void begSeg(std::string segName);
    void endSeg();
    // void switchSeg(std::string segName);
    void printSymbolTable();
private:
    std::map<std::string,Label*> symTab;
    std::vector<Label*> defTab;
};