#include "Generator.h"
#include "Scanner.h"
#include "Parser.h"
#include "elf.h"

Inst instr;
ModRM modrm;
SIB sib;

static int TwoOpCode[][2][4] = 
{
    //          8bit                     32bit 
    // r,r  r,rm  rm,r  r,imm   r,r   r,rm  rm,r  r,imm
    {{0x88, 0x8a, 0x88, 0xb0}, {0x89, 0x8b, 0x89, 0xb8}}, //mov
    {{0x38, 0x3a, 0x38, 0x80}, {0x39, 0x3b, 0x39, 0x81}}, //cmp
    {{0x28, 0x2a, 0x28, 0x80}, {0x29, 0x2b, 0x29, 0x81}}, //sub
    {{0x00, 0x02, 0x00, 0x80}, {0x01, 0x03, 0x01, 0x81}}, //add
    {{0x22, 0x22, 0x20, 0x80}, {0x23, 0x23, 0x21, 0x81}}, //and
    {{0x0a, 0x0a, 0x08, 0x80}, {0x0b, 0x0b, 0x09, 0x81}}, //or
    {{0x00, 0x00, 0x00, 0x00}, {0x00, 0x8d, 0x00, 0x00}}  //lea
};

static int OneOpCode[] =
{
//  call  int   imul  idiv  neg   inc   dec   jmp
    0xe8, 0xcd, 0xf7, 0xf7, 0xf7, 0x40, 0x48, 0xe9,
//  je    jne
    0x84, 0x85,
//  sete  setne setg  setge setl setle
    0x94, 0x95, 0x9f, 0x9d, 0x9c, 0x9e,
//  push  pop
    0x50, 0x58
};

static int NonOpCodep[] = 
{
    //ret
    0xc3
};

Generator::Generator(SymTab & tab, Parser& p) : symtab(tab)
{
    p.setGen(this);
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

void Generator::genTwoOp(Symbol s,OpType dstType,OpType srcType,int len)
{
    int opCode = getOpCode(s,dstType,srcType,len);
    switch(modrm.mod){
        //TODO: 1. 检查这三个全局变量是否正常
        //      2. 继续代码生成的函数
    }
}

void Generator::genOneOp(Symbol s,OpType type,int len)
{

}

void Generator::genNonOp(Symbol s)
{

}

int Generator::getOpCode(Symbol sym,OpType dstType,OpType srcType,int len)
{
    int index = 0;
    switch (srcType)
    {
    case IMMEDIATE:
        index = 3;
        break;
    case REGISTER:
        index = 2 * (dstType != IMMEDIATE);
        break;
    case MEMORY:
        index = 1;
        break;
    default:
        break;
    }

    return TwoOpCode[sym-I_MOV][(len!=8)][index];
}