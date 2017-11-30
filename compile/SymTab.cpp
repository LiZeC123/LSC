#include "SymTab.h"


SymTab::SymTab()
{
    scopeId = 0;
    currFun = nullptr;
    scopePath.push_back(0);  // 初始为全局作用域
}

void SymTab::enter()
{
    scopeId++;
    scopePath.push_back(scopeId);
    //TODO
}

void SymTab::leave()
{
    scopePath.pop_back();
    //TODO
}