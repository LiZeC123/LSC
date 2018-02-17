#include "Generator.h"
#include "Scanner.h"
#include "Parser.h"
#include "elf.h"

Generator::Generator(SymTab& tab) : symtab(tab)
{

}

bool Generator::processRel(int type)
{
    Label* relLabel = symtab.getRelLabel();
    if(Scanner::ScanLoop == 1 || relLabel == nullptr){
        symtab.setRelLabel(nullptr);
        return false;
    }

    bool flag = false;
    if(type == R_386_32){   // 绝对地址重定位
        symtab.getFile().addRel(Label::currSegName,Label::currAddr,relLabel->getName(),type);
        flag = true;
    }
    else if(type == R_386_PC32){    // 相对地址重定位
        if(relLabel->isExterned()){ // 外部跳转
            symtab.getFile().addRel(Label::currSegName,Label::currAddr,relLabel->getName(),type);
            flag = true;
        }
    }

    symtab.setRelLabel(nullptr);
    return flag;
}