#pragma once
#include "SymTab.h"
#include "Parser.h"
#include "OpInfo.h"

class Generator
{
public:
    Generator(SymTab& tab,Parser& p,FILE* fout);
    bool processRel(int type);
    void genTwoOp(Symbol s,OpType dstType,OpType srcType,int len);
    void genOneOp(Symbol s,OpType type,int len);
    void genNonOp(Symbol s);
private:
    int getOpCode(Symbol s,OpType dstType,OpType srcType,int len);
    void writeBytes(int value,int len);
    void writeModRM();
    void writeSIB();
    void writeDisp();
    SymTab& symtab;
    FILE* fout;
};


extern Inst instr;
extern ModRM modrm;
extern SIB sib;