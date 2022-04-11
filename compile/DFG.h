#pragma once
#include <list>
#include <vector>

#include "InterInst.h"

using std::list;
using std::vector;

class Block {
 public:
  Block(vector<InterInst*> codes);

 public:
  list<InterInst*> insts;
  list<Block*> prevs;
  list<Block*> succs;
};

class DFG {
 public:
  //TODO: 构造函数
  void createBlocks();
  void linkBlocks();

 public:
  vector<InterInst*> codeList;
  vector<Block*> blocks;
};


// eax ebx 是最常用寄存器，不使用
// ecx 通常用于逻辑判断，因此不使用
// edx 仅在除法中使用，因此可以考虑执行除法前入栈保存
// esi edi 完全没有使用，可以直接分配