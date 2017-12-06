#include "SymTab.h"

Fun::Fun()
{
    
}

void Fun::enterScope()
{
    scopeEsp.push_back(0);
}

void Fun::leaveScope()
{
    maxDepth = (curEsp > maxDepth)?curEsp:maxDepth;     // 更新最大栈帧值
    curEsp -= scopeEsp.back();                          // 恢复栈指针
    scopeEsp.pop_back();
}

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
    // 如果当前分析的是函数,则让函数执行进入作用域操作
    if(currFun){
        currFun->enterScope();
    }
}

void SymTab::leave()
{
    scopePath.pop_back();
    // 如果当前分析的是函数,则让函数执行离开作用域操作
    if(currFun){
        currFun->leaveScope();
    }
}