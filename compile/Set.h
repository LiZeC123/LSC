#pragma once
#include <vector>

using std::vector;

class Set {
 public:
  Set();
  Set(int size, bool val);
  void init(int size, bool val);

  Set operator&(Set other);
  Set operator|(Set other);
  Set operator-(Set other);
  Set operator^(Set other);
  Set operator~();
  bool operator==(Set& other);
  bool operator!=(Set& other);

  bool get(int index);
  void set(int index);
  void reset(int index);

  int max();
  bool empty();

  void printSelf();

 private:
  vector<unsigned int> data;  //每个bit位存储一个元素
  int count;                  // 实际使用的位数
};