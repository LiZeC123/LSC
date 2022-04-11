#include "DFG.h"

Block::Block(vector<InterInst*> codes) {
  for (auto code : codes) {
    code->setBlock(this);
    insts.push_back(code);
  }
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