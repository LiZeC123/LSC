#include "GenIR.h"
#include "SymTab.h"

int GenIR::lbNum = 0;

GenIR::GenIR(SymTab tab): symtab(tab)
{
    symtab.setGenIR(this);
}