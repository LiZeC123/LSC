#include <iostream>
#include <elf.h>
#include "Parser.h"
#include "Token.h"

#define firstIs(C) look->sym == C       // 判断向前读取的一个符号是不是指定的符号
#define _OR_(T) || look->sym == T       // 与上面的宏连用,组成更复杂的条件语句

using namespace std;

Parser::Parser(Lexer& lex,SymTab& tab) : 
    lexer(lex),
    symtab(tab)
{ }

void Parser::analyse()
{
    while(Scanner::ScanLoop <= 2){
        move();
        program();
    }
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
    type(name,cont,size);
    valtail(name,cont,size);
    symtab.addLabel(new Label(name,times,size,cont));
}

// <type> -> <NUM> | <off> <NUM> | STR | IDENT
void Parser::type(string name,std::vector<int>& cont,int size)
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
                SymTab::elfile.addRel(
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
        type(name,cont, size);
        valtail(name,cont, size);
    }
}

// <inst> -> <doubleop> <oprand> , <oprand>
// <inst> -> <singleop> <oprand>
// <inst> -> <noneop>
void Parser::inst()
{
    //TODO: 代码生成
    if(look->sym >= I_MOV && look->sym<=I_LEA){
        int regNum; OpType dstType,srcType; int len;
        move();
        oprand(dstType,len,true);
        match(COMMA);
        oprand(srcType,len,false);
    }
    else if(look->sym >= I_CALL && look->sym <= I_POP){
        int regNum; OpType type; int len;
        move();
        oprand(type,len,true);
    }
    else{
        move();
    }
}

// <oprand> -> <NUM> | <IDENT> | <reg> | <men>
// 操作数类型,长度,已经识别的寄存器数量
void Parser::oprand(OpType& type,int& len,bool isDstReg)
{
    int regCode;
    switch (look->sym)
    {
    case NUM:
        type = IMMEDIATE;
        move();
        break;
    case IDENT:
        type = IMMEDIATE;
        //string name = ((ID*)look)->name;
        move();
        break;
    case LBRAC:
        type = MEMORY;      
        men();
        break;
    case SUB:
        type = IMMEDIATE;
        move();
        //((Num*)look)->val;
        break;
    default:
        type = REGISTER;
        regCode = reg(len); // 除了寄存器类型,其他类型都不涉及长度
        if(isDstReg){
            // 目标寄存器写入reg
            break;
        }
        else{
            // 源寄存器写入rm
            break;
        }
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
        move();
    }
    else if(look->sym == IDENT){
        move();
    }
    else{
        int len = 0;
        int regCode = reg(len);
        regaddr();
    }
}

// <regaddr> -> <off> <regaddrtail> | e
void Parser::regaddr()
{
    Symbol s = look->sym;
    if(s == ADD || s == SUB){
        move();
        regaddrtail();
    }
}

// <regaddrtail> -> <NUM> | <reg>
void Parser::regaddrtail()
{
    move();
}