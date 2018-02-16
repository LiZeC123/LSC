#include "SymTab.h"
#include "Scanner.h"
using namespace std;

int SymTab::dataLen = 0;

bool SymTab::hasName(std::string name)
{
    return symTab.find(name) != symTab.end();
}

void SymTab::addLabel(Label* label)
{
    if(Scanner::ScanLoop != 1){
        // 第一次扫描才添加符号,否则直接退出
        // 考虑是否需要在此处释放内存
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

void SymTab::switchSeg(std::string segName)
{
    // TODO: 需要进一步检查边界条件的正确性
    if(Scanner::ScanLoop == 1){
        dataLen += (4 - dataLen % 4) % 4;
        // TODO: 生成段
        dataLen += Label::currAddr;
    }
    Label::currSegName = segName;
    Label::currAddr = 0;
}

void SymTab::printSymbolTable()
{
    for(auto& i:symTab){
        i.second->printSelf();
    }
}