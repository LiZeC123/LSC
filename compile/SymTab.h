#pragma once
#include <map>
#include <vector>
#include <string>

class Var
{

};

class Fun
{

};

class SymTab
{
public:
    SymTab();
    void enter();
    void leave();



private:
    std::map<std::string,std::vector<Var*>*> varTab;
    std::map<std::string,Var*> strTab;
    std::map<std::string,Fun*> funTab;
    Fun* currFun;
    int scopeId;
    std::vector<int> scopePath;
};