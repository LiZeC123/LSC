#include <iostream>
#include <stdexcept>
#include "Parser.h"
#include "Token.h"
#include "Generator.h"
 
#define firstIs(C) look->sym == C       // 判断向前读取的一个符号是不是指定的符号
#define _OR_(T) || look->sym == T       // 与上面的宏连用,组成更复杂的条件语句

using namespace std;


Parser::Parser(Lexer& lex,SymTab& tab,FILE* fout) : 
    lexer(lex),
    symtab(tab),
    gen(new Generator(tab,fout))
{ }

void Parser::analyse()
{
    while(Scanner::ScanLoop <= 2){
        move();
        program();
    }
}

Parser::~Parser()
{
    delete gen;
}

void Parser::move()
{
    look = lexer.nextToken();
}

bool Parser::match(Symbol need)
{
    if(look->sym == need){
        move();
        return true;
    }
    else{
        return false;
    }
}

// <program> -> section <IDENT> <program>
// <program> -> global <IDENT> <program>
// <program> -> <IDENT> <lbtail> <program>
// <program> -> <inst> <program>
// <program> -> e
void Parser::program()
{
    while (true)
    {
        switch (look->sym)
        {
        case END:
            symtab.endSeg();
            lexer.reset();  // 重置词法分析器,进行第二次读取
            ++Scanner::ScanLoop;
            return;
        case KW_SEC:
            move();
            symtab.endSeg();
            symtab.begSeg(((ID *)look)->name);
            move();
            break;
        case KW_GLB:
            move();
            symtab.addLabel(new Label(((ID*)look)->name,true,true));
            match(IDENT);
            break;
        case IDENT:
            lbtail(((ID *)look)->name);
            break;
        default:
            inst();
        }
    }
}


// <lbtail> -> COLON
// <lbtail> -> equ <NUM>
// <lbtail> -> <basetail>
// <lbtail> -> times <NUM> <basetail>
void Parser::lbtail(string name)
{
    move();
    switch(look->sym){
    case COLON:
        symtab.addLabel(new Label(name));
        move();
        break;
    case KW_EQU:
        move();
        symtab.addLabel(new Label(name, ((Num *)look)->val));
        move();
        break;
    case KW_TIMES:
        move();
        basetail(name, ((Num *)look)->val);
        break;
    default:
        basetail(name, 1);
    }
}

// <basetail> -> <len> <value>
void Parser::basetail(string name,int times)
{
    int size = len();
    value(name,times,size);
}

// <len> -> db | dw | dd
int Parser::len()
{
    switch(look->sym){
    case KW_DB:
        return 1;
    case KW_DW:
        return 2;
    case KW_DD:
        return 4;
    default:
        return 0;
    }
}

// <value> -> <type> <valtail>
void Parser::value(string name,int times,int size)
{
    move();
    vector<int> cont;
    type(name,cont);
    valtail(name,cont,size);
    symtab.addLabel(new Label(name,times,size,cont));
}

// <type> -> <NUM> | <off> <NUM> | STR | IDENT
void Parser::type(string name,std::vector<int>& cont)
{
    switch (look->sym){
    case NUM:
        cont.push_back(((Num *)look)->val);
        break;
    case STR:
        for(unsigned int i=0;i<((Str*)look)->str.size();i++){
            cont.push_back(((Str*)look)->str[i]);
        }
        break;
    case IDENT:
        if(Scanner::ScanLoop == 2){
            Label* label = symtab.getLabel(name);
            if(!label->isEqu()){
                symtab.getFile().addRel(
                    Label::currSegName,Label::currAddr+cont.size()*label->getLen(),
                    name,R_386_32);
            }
        }
        cont.push_back(
            symtab.getLabel(
                ((Str*)look)->str
            )->getAddr()
        );
        break;
    default:
        break;
    }
    move();
}

// <valtail> -> , <type><valtail> | e
void Parser::valtail(string name,std::vector<int>& cont,int size)
{
    if (match(COMMA)){
        type(name,cont);
        valtail(name,cont, size);
    }
}

// <inst> -> <doubleop> <oprand> , <oprand>
// <inst> -> <singleop> <oprand>
// <inst> -> <noneop>
void Parser::inst()
{
    if(look->sym >= I_MOV && look->sym<=I_LEA){
        Symbol s = look->sym;
        OpType dstType,srcType;
        int len; int regNum = 0;
        move();
        oprand(dstType,len,regNum);
        match(COMMA);
        oprand(srcType,len,regNum);
        gen->genTwoOp(s,dstType,srcType,len);
    }
    else if(look->sym >= I_CALL && look->sym <= I_POP){
        Symbol s = look->sym;
        OpType type; int len; int regNum = 0;
        move();
        oprand(type,len,regNum);
        gen->genOneOp(s,type,len);
    }
    else{
        Symbol s = look->sym;
        gen->genNonOp(s);
        move();
    }
}

// <oprand> -> <NUM> | <IDENT> | <reg> | <men>
// 操作数类型,长度,已经识别的寄存器数量
void Parser::oprand(OpType& type,int& len,int& regNum)
{
    int regCode;string name;Label* label;
    switch (look->sym)
    {
    case NUM:
        modrm.mod = -1;
        type = IMMEDIATE;
        instr.imm32 = ((Num*)look)->val;
        move();
        break;
    case IDENT:
        modrm.mod = -1;
        type = IMMEDIATE;
        name = ((ID*)look)->name;
        label = symtab.getLabel(name);
        instr.imm32 = label->getAddr();
        tempSave(label);
        move();
        break;
    case LBRAC:
        type = MEMORY;      // 由于不能同时操作两个内存地址,因此长度由另一个参数决定
        men();
        break;
    case SUB:
        modrm.mod = -1;
        type = IMMEDIATE;
        move();
        instr.imm32 = - ((Num*)look)->val;  // 取负数
        break;
    default:
        type = REGISTER;
        regCode = reg(len); // 除了寄存器类型,其他类型都不涉及长度
        if(regNum != 0){
            // 第二次读取到寄存器,将源寄存器写入rm
            modrm.mod = 3;
            modrm.rm = regCode;
        }
        else{
            // 第一次读取寄存器,将此寄存器临时保存在reg
            // 可能是 mov eax,ebx 也有可能是mov [ebp-4],eax
            modrm.reg = regCode;
        }
        regNum++;
        break;
    }

}

// <men> -> [ <addr> ]
void Parser::men()
{
    match(LBRAC);
    addr();
    match(RBRAC);
}


int Parser::reg(int& len)
{
    Symbol s = look->sym;
    int code = 0;
    if(s >= BR_AL && s <= BR_BH){
        len = 1;
        code =  s - BR_AL;
    }
    else if(s >= DR_EAX && s <= DR_EDI){
        len = 4;
        code = s - DR_EAX;
    }

    move();
    return code;
}

// <addr> -> <NUM> | <ID> | <reg> <regaddr>
void Parser::addr()
{
    if(look->sym == NUM){
        // 直接寻址
        modrm.mod = 0;
        modrm.rm = 5;
        instr.disp = ((Num*)look)->val;
        instr.dispLen = 4;
        move();
    }
    else if(look->sym == IDENT){
        // 直接寻址
        modrm.mod = 0;
        modrm.rm = 5;
        string name = ((ID *)look)->name;
        Label *label = symtab.getLabel(name);
        instr.disp = label->getAddr();
        instr.dispLen = 4;
        tempSave(label);
        move();
    }
    else{
        // 寄存器寻址
        int len = 0;
        int regCode = reg(len);
        regaddr(regCode,len);
    }
}

// <regaddr> -> <off> <regaddrtail> | e
void Parser::regaddr(int regCode,int len)
{
    Symbol s = look->sym;
    if(s == ADD || s == SUB){
        bool isSub = off();
        regaddrtail(regCode,isSub);
    }
    else{
        if(regCode == 4 && len == 4) {  //特殊寄存器 esp
            modrm.mod = 0;
            modrm.rm = 4;               //引导SIB
            sib.scale = 0;
            sib.index = 4;
            sib.base = 4;
        }
        else if(regCode == 5 && len == 4) { //特殊寄存器 ebp
            modrm.mod = 1;                  //8-bit 0 disp
            modrm.rm = 5;
            instr.disp = 0;
            instr.dispLen = 1;
        }
        else {                              // 普通寄存器寻址
            modrm.mod = 0;
            modrm.rm = regCode;
        }
    }
}

bool Parser::off()
{
    Symbol s = look->sym;
    bool isSub = false;
    if(s == ADD){
        isSub = false;
        move();
    }
    else if(s == SUB){
        isSub = true;
        move();
    }

    return isSub;
}


// <regaddrtail> -> <NUM> | <reg>
void Parser::regaddrtail(int regCode,bool isSub)
{
    if(look->sym == NUM){
        int num = ((Num*)look)->val;
        if(isSub){
            num = - num;
        }

        if(num >= -128 && num <= 127){
            modrm.mod = 1;
            instr.disp = num;
            instr.dispLen = 1;
        }
        else{
            modrm.mod = 2;
            instr.disp = num;
            instr.dispLen = 4;
        }
        modrm.rm = regCode;

        if(regCode == 4){   // 特殊寄存器 [esp+8bit] [esp+32bit]需要特殊处理
            modrm.rm = 4; //引导SIB
            sib.scale = 0;      // 指数为0
            sib.index = 4;      // index为4表示不存在index项
            sib.base = 4;       // base为4表示esp
        }
    }
    else{
        // 编译器不生成此类型代码
        throw std::logic_error("不支持的寻址类型!");
    }
    move();
}

// 对符号的引用可能需要重定位,先保存
// 在后续代码生成中,进行进一步的处理
void Parser::tempSave(Label* label)
{
    if (Scanner::ScanLoop == 2 && !label->isEqu()){
        symtab.setRelLabel(label);
    }
}