#pragma once
#include <map>
#include <vector>
#include <string>
#include "Label.h"


class SymTab
{
public:
    static int scanLop;
    // SymTab();
    bool hasName(std::string name);
    void addLabel(Label* label);
    Label* getLabel(std::string name);


private:
    std::map<std::string,Label*> symTab;
    std::vector<Label*> defTab;
};