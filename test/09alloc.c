#include "00testhelper.h"

#define SIZE 26

void BaseTest() {
  char *arr = lscmalloc(SIZE + 2);
  char *base = "abcdefghijklmnopqrstuvwxyz";

  for (int i = 0; i < SIZE; i++) {
    arr[i] = 'a' + i;
  }
  arr[SIZE] = '\0';

  checkStringEquals(arr, base, __LINE__);

  lscfree(arr);
}

#define T_SIZE 1024

void LargeAllocTest() {
  char *arr[T_SIZE];

  for (int i = 0; i < T_SIZE; i++) {
    // 测试大批量分配
    arr[i] = lscmalloc(T_SIZE);
  }

  for (int i = 0; i < T_SIZE; i++) {
    // 测试对分配的内存写入数据
    char *m = arr[i];
    m[i] = i % 256;
  }

  for (int i = 0; i < T_SIZE; i++) {
    // 测试从分配的内存读取数据
    char *m = arr[i];
    char v = i % 256;
    checkCharEquals(m[i], v, __LINE__);
  }

  for (int i = 0; i < T_SIZE; i++) {
    // 测试释放
    lscfree(arr[i]);
  }
}

char *AllocFunction() {
  char *arr = lscmalloc(SIZE + 2);
  for (int i = 0; i < SIZE; i++) {
    arr[i] = 'a' + i;
  }
  arr[SIZE] = '\0';
  return arr;
}

void CrossFunctionTest() {
  // 测试离开作用域后内存是否能够使用
  char *arr = AllocFunction();
  char *base = "abcdefghijklmnopqrstuvwxyz";
  checkStringEquals(arr, base, __LINE__);
  lscfree(arr);
}


struct Point2D {
  int x;
  int y;
};

struct Rect {
  struct Point2D* A;
  int b;
};

void StructAllocTest() {
  struct Rect* r;
  r = (struct Rect*)lscmalloc(sizeof(struct Rect));
  r->A = (struct Point2D*)lscmalloc(sizeof(struct Point2D));
  r->b = 4;
  r->A->x = 9;
  r->A->y = 29;

  checkEquals(r->A->x + r->A->y + r->b, 42, __LINE__);
}

int main() {
  BaseTest();
  LargeAllocTest();
  CrossFunctionTest();
  return 0;
}