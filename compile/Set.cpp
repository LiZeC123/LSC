#include "Set.h"

#include <stdexcept>

using std::runtime_error;

Set::Set() : count(0) {}

Set::Set(int size, bool val) { init(size, val); }

void Set::init(int size, bool val) {
  count = size;
  size = size / 32 + (size % 32 != 0);
  unsigned int v = val ? 0xffffffff : 0x00000000;
  for (int i = 0; i < size; i++) {
    data.push_back(v);
  }
}

Set Set::operator&(Set val) {
  Set res(count, 0);
  for (unsigned int i = 0; i < data.size(); i++) {
    res.data[i] = this->data[i] & val.data[i];
  }
  return res;
}

Set Set::operator|(Set val) {
  Set res(count, 0);
  for (unsigned int i = 0; i < data.size(); i++) {
    res.data[i] = this->data[i] | val.data[i];
  }
  return res;
}

Set Set::operator-(Set val) {
  Set res(count, 0);
  for (unsigned int i = 0; i < data.size(); i++) {
    res.data[i] = this->data[i] & ~val.data[i];
  }
  return res;
}

Set Set::operator^(Set val) {
  Set res(count, 0);
  for (unsigned int i = 0; i < data.size(); i++) {
    res.data[i] = this->data[i] ^ val.data[i];
  }
  return res;
}

Set Set::operator~() {
  Set res(count, 0);
  for (unsigned int i = 0; i < data.size(); i++) {
    res.data[i] = ~this->data[i];
  }
  return res;
}

bool Set::operator==(Set& o) {
  if (count != o.count) return false;
  for (unsigned int i = 0; i < data.size(); i++) {
    if (data[i] != o.data[i]) {
      return false;
    }
  }
  return true;
}

bool Set::operator!=(Set& o) { return !(*this == o); }

bool Set::get(int i) {
  if (i < 0 || i > count) {
    throw runtime_error("Set Index Out Of Range");
  }

  return (bool)(data[i / 32] & (1 << (i % 32)));
}

void Set::set(int i) {
  if (i < 0 || i > count) {
    throw runtime_error("Set Index Out Of Range");
  }

  data[i / 32] |= (1 << (i % 32));
}

void Set::reset(int i) {
  if (i < 0 || i > count) {
    throw runtime_error("Set Index Out Of Range");
  }

  data[i / 32] &= ~(1 << (i % 32));
}

int Set::max() {
  for (int i = data.size() - 1; i >= 0; --i) {
    unsigned int n = 0x80000000;
    int index = 31;
    while (n) {
      if (data[i] & n) break;
      --index;
      n >>= 1;
    }
    if (index >= 0) {  //出现1
      return i * 32 + index;
    }
  }
  return -1;
}

void Set::printSelf() {
  int num = count;  //计数器
  for (unsigned int i = 0; i < data.size(); ++i) {
    unsigned int n = 0x1;
    while (n) {
      printf("%d ", !!(data[i] & n));
      if (!--num) break;
      n <<= 1;
    }
    if (!--num) break;
  }
  fflush(stdout);
}