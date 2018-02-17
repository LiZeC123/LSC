#pragma once
#include "SymTab.h"

class Generator
{
public:
    Generator(SymTab& tab);
    bool processRel(int type);
private:
    SymTab& symtab;
};