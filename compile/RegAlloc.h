#pragma once
#include "SymTab.h"

struct Node
{
    Var* var;
    int degree;
    int color;

};

// eax ebx 是最常用寄存器，不使用
// ecx 通常用于逻辑判断，因此不使用
// edx 仅在除法中使用，因此可以考虑执行除法前入栈保存
// esi edi 完全没有使用，可以直接分配


