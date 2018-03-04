#include "SymTab.h"
#include "Scanner.h"
using namespace std;

SymTab::SymTab()
{
    dataLen = 0;
    relLabel = nullptr;
}

bool SymTab::hasName(std::string name)
{
    return symTab.find(name) != symTab.end();
}

void SymTab::addLabel(Label* label)
{
    if(Scanner::ScanLoop != 1){
        // 第一次扫描才添加符号,否则直接退出
        delete label;
        return;
    }

    if(hasName(label->getName())){
        // 对于extern属性,会被新的符号覆盖
        // global属性从原有符号属性继承
        Label* old =  symTab[label->getName()];
        if(old->isGlobal()){
            label->setGlobal(true);
        }
        delete old;
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

void SymTab::begSeg(std::string segName)
{
    Label::currSegName = segName;
    Label::currAddr = 0;
}

void SymTab::endSeg()
{
    if(Scanner::ScanLoop == 1){
        dataLen += (4 - dataLen % 4) % 4;
        elfile.addShdr(Label::currSegName,Label::currAddr,dataLen);
        dataLen += Label::currAddr;
    }
}

void SymTab::exportSyms()
{
    for(auto it = symTab.begin();it != symTab.end();++it){
        Label* label = it->second;
        if(!label->isEqu()){
            elfile.addSym(label);
        }
    }
}

void SymTab::printSymbolTable()
{
    for(auto& i:symTab){
        i.second->printSelf();
    }
}

ElfFile& SymTab::getFile()
{
    return elfile;
}

void SymTab::setRelLabel(Label* label)
{
    relLabel = label;
}

Label* SymTab::getRelLabel()
{
    return relLabel;
}

int SymTab::getDataLen()
{
    return dataLen;
}

void SymTab::writeToFile(FILE* fin,FILE* fout)
{
    exportSyms();
    elfile.assemObj(dataLen);
    elfile.writeElfHead(fout);
    writeData(fout);
    elfile.writeElfTail(fin,fout);
}

void SymTab::writeData(FILE* fout)
{
    for(unsigned int i=0;i<defTab.size();i++){
        defTab[i]->write(fout);
    }
}