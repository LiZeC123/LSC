#pragma once
#include <list>
#include <vector>

#include "InterInst.h"

using std::list;
using std::vector;

// 数据流图的基本块
class Block {
 public:
  Block(vector<InterInst*> codes);

  void printSelf();

 public:
  list<InterInst*> insts;
  list<Block*> prevs;
  list<Block*> succs;

  vector<double> inVals;   // 输入值集合
  vector<double> outVals;  // 输出值集合

  bool visited;
  bool reachable;
};

// 数据流图
class DFG {
 public:
  
  DFG(InterCode& code);

  void createBlocks();
  void linkBlocks();


  void toCode(InterCode& optCode);

  void printSelf();

 public:
  vector<InterInst*> codeList;
  vector<Block*> blocks;
};