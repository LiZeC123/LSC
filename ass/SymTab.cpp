#include "SymTab.h"
using namespace std;

int SymTab::scanLop = 0; // TODO: 设置初始值

bool SymTab::hasName(std::string name)
{
    return symTab.find(name) != symTab.end();
}

void SymTab::addLabel(Label* label)
{
    if(scanLop != 1){
        // 第一次扫描才添加符号,否则直接退出
        return;
    }

    if(hasName(label->getName())){
        delete symTab[label->getName()];
        symTab[label->getName()] = label;
    }
    else{
        symTab[label->getName()] = label;
    }

    if(label->getTimes() != 0 && label->getSegName() == ".data"){
        defTab.push_back(label);
    }
}

Label* SymTab::getLabel(std::string name)
{
    Label* label = nullptr;
    if(hasName(name)){
        label = symTab[name];
    }
    else{
        label = new Label(name,true);
        symTab[name] = label;
    }
    return label;
}