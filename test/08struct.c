#include "00testhelper.h"

struct StructA {
  int mA;
  char mB;
  char mC;
  int mD;
  char mE;
  int mF;
};

struct Point2D {
  int x;
  int y;
};

struct Point3D {
  int x;
  int y;
  int z;
};

struct Rect {
  struct Point2D A;
  struct Point3D B;
};

void BaseAccessTest() {
  struct Point2D p2d;
  p2d.x = 23213;
  p2d.y = 43234;

  checkEquals(p2d.x + p2d.y, 23213 + 43234, __LINE__);

  struct StructA sa;
  sa.mA = 0x12345432;
  sa.mB = 'A';
  sa.mC = 'B';
  sa.mD = 0x12345678;
  sa.mE = 'C';
  sa.mF = 0x0303fede;

  checkEquals(sa.mA, 0x12345432, __LINE__);
  checkCharEquals(sa.mB, 'A', __LINE__);
  checkCharEquals(sa.mC, 'B', __LINE__);
  checkEquals(sa.mD, 0x12345678, __LINE__);
  checkCharEquals(sa.mE, 'C', __LINE__);
  checkEquals(sa.mF, 0x0303fede, __LINE__);

  sa.mB = 'B';
  sa.mC = 'C';
  sa.mE = 'E';
  sa.mA = 1233;
  checkEquals(sa.mA, 1233, __LINE__);
  checkCharEquals(sa.mB, 'B', __LINE__);
  checkCharEquals(sa.mC, 'C', __LINE__);
  checkEquals(sa.mD, 0x12345678, __LINE__);
  checkCharEquals(sa.mE, 'E', __LINE__);
  checkEquals(sa.mF, 0x0303fede, __LINE__);

  struct StructA* p = &sa;
}

void BasePointAccessTest() {
  struct StructA sa;
  sa.mA = 0x12345432;
  sa.mB = 'A';
  sa.mC = 'B';
  sa.mD = 0x12345678;
  sa.mE = 'C';
  sa.mF = 0x0303fede;

  struct StructA* ps;

  checkEquals(ps->mA, 0x12345432, __LINE__);
  checkCharEquals(ps->mB, 'A', __LINE__);
  checkCharEquals(ps->mC, 'B', __LINE__);
  checkEquals(ps->mD, 0x12345678, __LINE__);
  checkCharEquals(ps->mE, 'C', __LINE__);
  checkEquals(ps->mF, 0x0303fede, __LINE__);
}

void MutilAccessTest() {
  struct Rect r;
  r.A.x = 1;
  r.A.y = 2;
  r.B.x = 3;
  r.B.y = 4;
  r.B.z = 5;

  int C = (r.B.x - r.A.x) + (r.B.y - r.A.y) + r.B.z;
  checkEquals(C, 9, __LINE__);

  struct Rect* pr = &r;
  C = (pr->B.y - pr->A.x) + (pr->B.x - pr->A.y) + r.B.z;
  checkEquals(C, 9, __LINE__);
}

void SizeOfTest() {
  checkEquals(sizeof(struct Point2D), 8, __LINE__);
  checkEquals(sizeof(struct Point3D), 12, __LINE__);
  checkEquals(sizeof(struct StructA), 20, __LINE__);
  checkEquals(sizeof(struct Rect), 20, __LINE__);
}

int main() {
  BaseAccessTest();

  MutilAccessTest();

  SizeOfTest();

  return 0;
}
