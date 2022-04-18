#pragma once

#include <vector>

#include "DFG.h"
#include "SymTab.h"

using std::vector;

class ConstPropagation {
 public:
  ConstPropagation(DFG* g, SymTab* tab, vector<Var*>& paraVars);
  void propagation();

 private:
  void analyse();          //常量传播分析
  void algebraSimplify();  //代数化简
  void condJmpOpt();  //条件跳转优化，同时进行不可达代码消除

  void join(Block* block);
  double join(double left, double right);

  bool translate(Block* block);  //传递函数fB
  void translate(InterInst* inst, vector<double>& in,
                 vector<double>& out);  //单指令传递函数

 private:
  DFG* dfg;
  SymTab* tab;
  vector<Var*> vars;     // 局部变量集合
  vector<Var*> glbVars;  // 全局变量集合
  vector<double> boundVals;   // 边界值集合
  vector<double> initVals;

  static double UNDEF;
  static double NAC;
};

class CopyPropagation {
 public:
  CopyPropagation(DFG* g);
  void propagation();

 private:
  DFG* dfg;
};