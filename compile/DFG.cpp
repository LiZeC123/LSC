#include "DFG.h"

Block::Block(vector<InterInst*> codes) {
  for (auto code : codes) {
    code->setBlock(this);
    insts.push_back(code);
  }
}

void Block::printSelf() {
  printf("-----------%p----------\n", this);
  printf("前驱：");
  for (list<Block*>::iterator i = prevs.begin(); i != prevs.end(); ++i) {
    printf("%p ", *i);
  }
  printf("\n");
  printf("后继：");
  for (list<Block*>::iterator i = succs.begin(); i != succs.end(); ++i) {
    printf("%p ", *i);
  }
  printf("\n代码\n");
  for (list<InterInst*>::iterator i = insts.begin(); i != insts.end(); ++i) {
    (*i)->printSelf();
  }
  printf("in值集合\n");
  for (auto v : inVals) {
    printf("%7.1f", v);
  }
  printf("\n");

  printf("out值集合\n");
  for (auto v : inVals) {
    printf("%7.1f", v);
  }
  printf("\n");
}

DFG::DFG(InterCode& code) {
  code.markFirstOp();
  this->codeList = code.getCode();
  createBlocks();
  linkBlocks();
}

void DFG::createBlocks() {
  vector<InterInst*> tmp;
  tmp.push_back(codeList[0]);
  for (unsigned int i = 0; i < codeList.size(); i++) {
    if (codeList[i]->isFirstOp()) {
      blocks.push_back(new Block(tmp));
      tmp.clear();
    }
    tmp.push_back(codeList[i]);
  }
  blocks.push_back(new Block(tmp));
}

void DFG::linkBlocks() {
  for (unsigned int i = 0; i < blocks.size(); i++) {
    Block* block = blocks[i];
    InterInst* last = block->insts.back();
    if (last->isJumpOp() || last->isCondOp()) {
      Block* target = last->getTarget()->getBlock();
      block->succs.push_back(target);
      target->prevs.push_back(block);
    }

    if (!last->isJumpOp() && i != blocks.size() - 1) {
      block->succs.push_back(blocks[i + 1]);
      blocks[i + 1]->prevs.push_back(block);
    }
  }
}

void DFG::toCode(InterCode& optCode) {
  optCode.clear();
  // TODO:
  // 1. 跳过不可达代码
  // 2. 删除死代码
  for (Block* block : blocks) {
    for (InterInst* inst : block->insts) {
      optCode.addInst(inst);
    }
  }
}

void DFG::printSelf() {
  for (Block* block : this->blocks) {
    block->printSelf();
  }
}