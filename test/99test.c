// #include "00testhelper.h"

int main() {
  int a = 1;
  int b = a + 2;
  
  if (b) {
    b = b + 1;
  }
  
  b = a * b;

  return b;
}
