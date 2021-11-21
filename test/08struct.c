#include "00testhelper.h"

struct Point2D
{
    int x;
    int y;
};

struct Point3D
{
    int x;
    int y;
    int z;
};

struct StructA
{
    int mA;
    char mB;
    char mC;
    int mD;
    char mE;
    int mF;
};


struct Rect
{
    struct Point2D A;
    struct Point2D B;
};


void baseReadWriteTest()
{
    struct Point2D p2d;


    struct Rect r;
    r.A.x = 1;
    r.A.y = 1;
    r.B.x = 4;
    r.B.y = 5;

    int C = (r.B.x - r.A.x) + (r.B.y - r.A.y);
    checkEquals(C, 7, __LINE__);

    // sizeof(struct Point2D);

}


void checkSizeOf() {
    checkEquals(sizeof(struct Point2D), 8, __LINE__);
    checkEquals(sizeof(struct Point3D), 12, __LINE__);
    checkEquals(sizeof(struct StructA), 20, __LINE__);
    checkEquals(sizeof(struct Rect), 16, __LINE__);
}


int main()
{
    baseReadWriteTest();
    
    checkSizeOf();



    return 0;
}
