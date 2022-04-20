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
  dfg->blocks[0]->outVals = boundVals;  //入口块只有out值
  for (unsigned int i = 1; i < dfg->blocks.size(); i++) {
    Block *block = dfg->blocks[i];
    block->inVals = initVals;
    block->outVals = initVals;
  }

  bool changed = true;
  while (changed) {
    changed = false;
    // 从第1块开始依次执行join和translate操作
    for (unsigned int i = 1; i < dfg->blocks.size(); i++) {
      Block *block = dfg->blocks[i];
      join(block);
      if (translate(block)) {
        changed = true;
      }
    }
  }
}

void ConstPropagation::algebraSimplify() {
  for (Block *block : dfg->blocks) {
    for (InterInst *inst : block->insts) {
      simplifyInst(inst);
    }
  }
}

void ConstPropagation::simplifyInst(InterInst *inst) {
  Operator op = inst->getOp();
  Var *result = inst->getResult();
  Var *arg1 = inst->getArg1();
  Var *arg2 = inst->getArg2();

  if (inst->isExpr()) {
    double rs = inst->outVals[result->getIndex()];
    if (rs != UNDEF && rs != NAC) {
      Var *newVar = new Var((int)rs);
      tab->addVar(newVar);
      inst->replace(OP_AS, result, newVar);
    } else if (op >= OP_ADD && op <= OP_OR &&
               !(op == OP_AS || op == OP_NEG || op == OP_NOT)) {
      simplifyExpr(inst);
    }
  } else if (op == OP_ARG || op == OP_RETV) {
    // 如果函数参数或者返回值为常量，则直接替换为对应的常量值
    if (arg1->getIndex() != -1) {
      double rs = inst->outVals[arg1->getIndex()];
      if (rs != UNDEF && rs != NAC) {
        Var *newVar = new Var((int)rs);
        tab->addVar(newVar);
        //替换新的操作符与常量操作数
        inst->replace(op, result, newVar, arg2);
      }
    }
  }
}

void ConstPropagation::simplifyExpr(InterInst *inst) {
  Operator op = inst->getOp();
  Var *result = inst->getResult();
  Var *arg1 = inst->getArg1();
  Var *arg2 = inst->getArg2();

  //常量传播不能处理的情况由代数化简完成
  double lp, rp;  //左右操作数
  if (arg1->getIndex() == -1) {
    if (arg1->isBase()) lp = arg1->getVal();
  } else
    lp = inst->inVals[arg1->getIndex()];  //左操作数

  if (arg2->getIndex() == -1) {
    if (arg2->isBase()) rp = arg2->getVal();
  } else
    rp = inst->inVals[arg2->getIndex()];  //右操作数

  int left, right;                //记录操作数值
  bool dol = false, dor = false;  //处理哪一个操作数
  if (lp != UNDEF && lp != NAC) {
    left = lp;
    dol = true;
  } else if (rp != UNDEF && rp != NAC) {
    right = rp;
    dor = true;
  } else {
    //都不是常量不进行处理！
    return;
  }

  Var *newArg1 = nullptr;  //记录有效的操作数
  Var *newArg2 = nullptr;  //可选的操作数

  Operator newOp = OP_AS;  //化简成功后默认为赋值运算
  if (op == OP_ADD) {      // z=0+y z=x+0
    if (dol && left == 0) {
      newArg1 = arg2;
    }
    if (dor && right == 0) {
      newArg1 = arg1;
    }
  } else if (op == OP_SUB) {  // z=0-y z=x-0
    if (dol && left == 0) {
      newOp = OP_NEG;
      newArg1 = arg2;
    }
    if (dor && right == 0) {
      newArg1 = arg1;
    }
  } else if (op == OP_MUL) {  // z=0*y z=x*0 z=1*y z=x*1
    if ((dol && left == 0) || (dor && right == 0)) {
      newArg1 = SymTab::zero;
    }
    if (dol && left == 1) {
      newArg1 = arg2;
    }
    if (dor && right == 1) {
      newArg1 = arg1;
    }
  } else if (op == OP_DIV) {  // z=0/y z=x/1
    if (dol && left == 0) {
      newArg1 = SymTab::zero;
    }
    if (dor && right == 1) {
      newArg1 = arg1;
    }
  } else if (op == OP_MOD) {  // z=0%y z=x%1
    if ((dol && left == 0) || (dor && right == 1)) {
      newArg1 = SymTab::zero;
    }
  } else if (op == OP_AND) {  // z=0&&y z=x&&0 z=1&&y z=x&&1
    if ((dol && left == 0) || (dor && right == 0)) {
      newArg1 = SymTab::zero;
    }
    if (dol && left != 0) {  // z=y!=0
      newOp = OP_NE;
      newArg1 = arg2;
      newArg2 = SymTab::zero;
    }
    if (dor && right != 0) {  // z=x!=0
      newOp = OP_NE;
      newArg1 = arg1;
      newArg2 = SymTab::zero;
    }
  } else if (op == OP_OR) {  // z=0||y z=x||0 z=1||y z=x||1
    if ((dol && left != 0) || (dor && right != 0)) {
      newArg1 = SymTab::one;
    }
    if (dol && left == 0) {  // z=y!=0
      newOp = OP_NE;
      newArg1 = arg2;
      newArg2 = SymTab::zero;
    }
    if (dor && right == 0) {  // z=x!=0
      newOp = OP_NE;
      newArg1 = arg1;
      newArg2 = SymTab::zero;
    }
  }

  if (newArg1) {
    // 如果能够进行代数简化，则替换相应的指令
    inst->replace(newOp, result, newArg1, newArg2);
  } else {
    // 否则使用已知的常量信息替换指令中的变量
    if (dol) {
      newArg1 = new Var(left);
      tab->addVar(newArg1);
      newArg2 = arg2;
    } else if (dor) {
      newArg2 = new Var(right);
      tab->addVar(newArg2);
      newArg1 = arg1;
    }
    inst->replace(op, result, newArg1, newArg2);
  }
}

void ConstPropagation::condJmpOpt() {}

void ConstPropagation::join(Block *block) {
  // 首先判断特殊情况
  int prevCount = block->prevs.size();
  if (prevCount == 0) {
    // 没有前驱，则in集合为默认值
    block->inVals = initVals;
    return;
  }
  if (prevCount == 1) {
    // 只有一个前驱，则in集合直接等于前驱的out集合
    block->inVals = block->prevs.front()->outVals;
    return;
  }
  // 否则针对多个前驱，合并不同前驱的计算结果
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

  if (inst->isExpr()) {  //基本运算表达式
    double tmp;
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
      if (lp == NAC || rp == NAC) {
        //有一个NAC结果必是NAC
        tmp = NAC;
      } else if (lp == UNDEF || rp == UNDEF) {
        //都不是NAC，有一个是UNDEF，结果UNDEF
        tmp = UNDEF;
      } else {
        //都是常量，直接计算结果
        tmp = execOp(op, (int)lp, (int)rp);
      }
    } else if (op == OP_GET) {  //破坏运算x=*y
      tmp = NAC;
    }
    out[rst->getIndex()] = tmp;  //更新out集合值
  } else if (op == OP_SET || (op == OP_ARG && !arg1->isBase())) {
    // 如果产生指针操作，则默认所有变量都可能被修改
    for (unsigned int i = 0; i < out.size(); ++i) out[i] = NAC;
  } else if (op == OP_PROC) {
    // 默认函数调用可能导致任意全局变量被修改
    for (unsigned int i = 0; i < glbVars.size(); ++i)
      out[glbVars[i]->getIndex()] = NAC;
  } else if (op == OP_CALL) {
    for (unsigned int i = 0; i < glbVars.size(); ++i)
      out[glbVars[i]->getIndex()] = NAC;
    out[rst->getIndex()] = NAC;
  }

  //拷贝信息指令的in，out信息，方便代数化简，条件跳转优化和不可达代码消除
  inst->inVals = in;
  inst->outVals = out;
}

double ConstPropagation::execOp(Operator op, int left, int right) {
  double tmp = 0;
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

  return tmp;
}

CopyPropagation::CopyPropagation(DFG *g) : dfg(g) {
  dfg->toCode(optCode);

  for (InterInst *inst : optCode.getCode()) {
    if (inst->getOp() == OP_AS) {
      copyExpr.push_back(inst);
    }
  }

  U.init(copyExpr.size(), true);
  E.init(copyExpr.size(), false);

  for (InterInst *inst : optCode.getCode()) {
    inst->copyInfo.gen = E;
    inst->copyInfo.kill = E;

    Var *rs = inst->getResult();
    Operator op = inst->getOp();

    if (inst->isUncertainOp()) {
      inst->copyInfo.kill = U;
    } else if (op >= OP_AS && op <= OP_GET) {
      for (unsigned int i = 0; i < copyExpr.size(); i++) {
        if (copyExpr[i] == inst) {
          inst->copyInfo.gen.set(i);
        } else if (rs == copyExpr[i]->getResult() ||
                   rs == copyExpr[i]->getArg1()) {
          inst->copyInfo.kill.set(i);
        }
      }
    }
  }
}

void CopyPropagation::propagation() {
  analyse();

  for (InterInst *inst : optCode.getCode()) {
    Var *rs = inst->getResult();
    Operator op = inst->getOp();
    Var *arg1 = inst->getArg1();
    Var *arg2 = inst->getArg2();

    if (op == OP_SET) {
      //取值运算，检查 *arg1=result
      //找到result赋值源头，肯定不是空
      Var *newRs = find(inst->copyInfo.in, rs);
      inst->replace(op, newRs, arg1);
    } else if (op >= OP_AS && op <= OP_GET && op != OP_LEA) {
      //一般表达式，排除p=&x运算，检查arg1/2
      Var *newArg1 = find(inst->copyInfo.in, arg1);
      Var *newArg2 = find(inst->copyInfo.in, arg2);
      inst->replace(op, rs, newArg1, newArg2);
    } else if (op == OP_JT || op == OP_JF || op == OP_ARG || op == OP_RETV) {
      //条件表达式,参数表达式，返回表达式
      Var *newArg1 = find(inst->copyInfo.in, arg1);
      inst->replace(op, rs, newArg1);
    }
  }
}

void CopyPropagation::analyse() {
  dfg->blocks[0]->copyInfo.out = E;
  for (unsigned int i = 1; i < dfg->blocks.size(); i++) {
    dfg->blocks[i]->copyInfo.out = U;
  }

  bool change = true;
  while (change) {
    change = false;
    for (unsigned int i = 1; i < dfg->blocks.size(); i++) {
      Block *block = dfg->blocks[i];
      // TODO: 不可达判定
      Set tmp = U;
      for (Block *p : block->prevs) {
        tmp = tmp & p->copyInfo.out;
      }

      block->copyInfo.in = tmp;
      if (translate(block)) {
        change = true;
      }
    }
  }
}

bool CopyPropagation::translate(Block *block) {
  Set tmp = block->copyInfo.in;

  for (InterInst *inst : block->insts) {
    Set &in = inst->copyInfo.in;
    Set &out = inst->copyInfo.out;
    in = tmp;
    out = (in - inst->copyInfo.kill) | (inst->copyInfo.gen);
    tmp = out;
  }

  bool flag = (tmp != block->copyInfo.out);
  block->copyInfo.out = tmp;
  return flag;
}

Var *CopyPropagation::find(Set &in, Var *var) { return _find(in, var, var); }

Var *CopyPropagation::_find(Set &in, Var *var, Var *src) {
  if (!var) {
    return nullptr;
  }

  for (unsigned int i = 0; i < copyExpr.size(); i++) {
    if (in.get(i) && var == copyExpr[i]->getResult()) {
      var = copyExpr[i]->getArg1();
      if (src == var) {
        break;
      }
      return _find(in, var, src);
    }
  }
  return var;
}
