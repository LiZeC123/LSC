#include "Propagation.h"

double ConstPropagation::UNDEF = 0.5;
double ConstPropagation::NAC = -0.5;

ConstPropagation::ConstPropagation(DFG *g, SymTab *t, vector<Var *> &paraVars)
    : dfg(g), tab(t) {
  int index = 0;

  for (Var *var : tab->getGlobalVars()) {
    var->setIndex(index++);
    this->vars.push_back(var);

    // 全局变量根据初始状态设置，默认值为0
    double val = 0;
    if (!var->isBase()) {
      val = NAC;
    } else if (var->isInit()) {
      val = var->getVal();
    }

    this->boundVals.push_back(val);
  }

  for (Var *var : paraVars) {
    var->setIndex(index++);
    this->vars.push_back(var);
    this->boundVals.push_back(NAC);
  }

  for (auto code : dfg->codeList) {
    if (code->isDecOp()) {
      Var *var = code->getArg1();
      var->setIndex(index++);
      this->vars.push_back(var);

      // 局部变量根据初始状态设置值，默认为未定义状态
      double val = UNDEF;
      if (!var->isBase()) {
        val = NAC;
      } else if (var->isInit()) {
        val = var->getVal();
      }

      this->boundVals.push_back(val);
    }
  }

  while (index--) {
    // 所有变量初始为UNDEF
    this->initVals.push_back(UNDEF);
  }
}

void ConstPropagation::propagation() {
  analyse();
  algebraSimplify();
  condJmpOpt();
}

void ConstPropagation::analyse() {
  dfg->blocks[0]->outVals = boundVals;
  for (Block *block : dfg->blocks) {
    block->outVals = initVals;
  }

  bool changed = true;
  while (changed) {
    changed = false;
    for (Block *block : dfg->blocks) {
      join(block);
      if (translate(block)) {
        changed = true;
      }
    }
  }
}

void ConstPropagation::algebraSimplify() {}

void ConstPropagation::condJmpOpt() {}

void ConstPropagation::join(Block *block) {
  for (unsigned int i = 0; i < block->inVals.size(); i++) {
    double val = UNDEF;
    for (Block *pb : block->prevs) {
      val = join(val, pb->outVals[i]);
    }
    block->inVals[i] = val;
  }
}

double ConstPropagation::join(double left, double right) {
  if (left == NAC || right == NAC) {
    return NAC;
  } else if (left == UNDEF) {
    return right;
  } else if (right == UNDEF) {
    return left;
  } else if (left == right) {
    return left;
  } else {
    return NAC;
  }
}

bool ConstPropagation::translate(Block *block) {
  vector<double> in = block->inVals;
  vector<double> out = in;

  for (InterInst *inst : block->insts) {
    translate(inst, in, out);
    in = out;
  }

  bool changed = false;
  for (unsigned int i = 0; i < out.size(); i++) {
    if (block->outVals[i] != out[i]) {
      changed = true;
      break;
    }
  }

  block->outVals = out;
  return changed;
}

void ConstPropagation::translate(InterInst *inst, vector<double> &in,
                                 vector<double> &out) {
  out = in;
  Operator op = inst->getOp();
  Var *rst = inst->getResult();
  Var *arg1 = inst->getArg1();
  Var *arg2 = inst->getArg2();

  if (inst->isExpr()) {  //基本运算表达式x=?，计算新发现的传播值
    double tmp;          //存储临时值结果
    if (op == OP_AS || op == OP_NEG || op == OP_NOT) {  //一元运算x=y x=-y x=!y
      if (arg1->getIndex() == -1) {  //参数不在值列表，必是常量
        if (arg1->isBase()) tmp = arg1->getVal();  //排除字符串类型常量
      } else
        tmp = in[arg1->getIndex()];      //获取in集合值
      if (tmp != UNDEF && tmp != NAC) {  //处理常量
        if (op == OP_NEG)
          tmp = -tmp;
        else if (op == OP_NOT)
          tmp = !tmp;
      }
    } else if (op >= OP_ADD && op <= OP_OR) {  //二元运算x=y+z
      double lp, rp;                           //左右操作数
      if (arg1->getIndex() == -1) {  //参数不在值列表，必是常量
        if (arg1->isBase()) lp = arg1->getVal();
      } else
        lp = in[arg1->getIndex()];   //左操作数
      if (arg2->getIndex() == -1) {  //参数不在值列表，必是常量
        if (arg2->isBase()) rp = arg2->getVal();  //初始化的基本类型变量为const
      } else
        rp = in[arg2->getIndex()];  //右操作数
      if (lp == NAC || rp == NAC)
        tmp = NAC;  //有一个NAC结果必是NAC
      else if (lp == UNDEF || rp == UNDEF)
        tmp = UNDEF;  //都不是NAC，有一个是UNDEF，结果UNDEF
      else {          //都是常量，可以计算
        int left = lp, right = rp;
        if (op == OP_ADD)
          tmp = left + right;
        else if (op == OP_SUB)
          tmp = left - right;
        else if (op == OP_MUL)
          tmp = left * right;
        else if (op == OP_DIV) {
          if (!right)
            tmp = NAC;
          else
            tmp = left / right;
        }  //除数为0,特殊处理！
        else if (op == OP_MOD) {
          if (!right)
            tmp = NAC;
          else
            tmp = left % right;
        } else if (op == OP_GT)
          tmp = left > right;
        else if (op == OP_GE)
          tmp = left >= right;
        else if (op == OP_LT)
          tmp = left < right;
        else if (op == OP_LE)
          tmp = left <= right;
        else if (op == OP_EQU)
          tmp = left == right;
        else if (op == OP_NE)
          tmp = left != right;
        else if (op == OP_AND)
          tmp = left && right;
        else if (op == OP_OR)
          tmp = left || right;
      }
    } else if (op == OP_GET) {  //破坏运算x=*y
      tmp = NAC;
    }
    out[rst->getIndex()] = tmp;  //更新out集合值
  } else if (op == OP_SET || (op == OP_ARG && !arg1->isBase())) {
    //破坏运算*x=y 或者 arg x没有影响，arg p->p是指针，破坏！！！
    for (unsigned int i = 0; i < out.size(); ++i)
      out[i] = NAC;            // out全部置为NAC
  } else if (op == OP_PROC) {  //破坏运算call f()
    for (unsigned int i = 0; i < glbVars.size(); ++i)
      out[glbVars[i]->getIndex()] = NAC;  //全局变量全部置为NAC
  } else if (op == OP_CALL) {             //破坏运算call f()
    for (unsigned int i = 0; i < glbVars.size(); ++i)
      out[glbVars[i]->getIndex()] = NAC;  //全局变量全部置为NAC
    out[rst->getIndex()] = NAC;  //函数返回值失去常量性质！——保守预测
  }

  //拷贝信息指令的in，out信息，方便代数化简，条件跳转优化和不可达代码消除
  inst->inVals = in;
  inst->outVals = out;
}