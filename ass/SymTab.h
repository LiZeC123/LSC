#pragma once
#include <map>
#include <vector>
#include <string>
#include "Label.h"


class SymTab
{
public:
    // SymTab();
    bool hasName(std::string name);
    void addLabel(Label* label);
    Label* getLabel(std::string name);

    void printSymbolTable();
private:
    std::map<std::string,Label*> symTab;
    std::vector<Label*> defTab;
};