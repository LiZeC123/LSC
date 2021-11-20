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


struct LinkedNode
{
    int data;
    struct LinkedNode *next;
};

struct TreeNode
{
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
};


int main(int argc, char *argv)
{
    struct Point2D p2d;
    p2d.x = 3;
    p2d.y = 4;
    int x = p2d.x;
    int y = p2d.y;
    int abs = p2d.x * p2d.x + p2d.y * p2d.y;
    lscPrintInt(abs);
    return 0;
}

// 未定义的函数和变量会直接导致Core Dump, 可能需要排查具体原因





// typedef struct
// {
//     int X;
//     int Y;
// } C;







// int L2Len(struct Point2D p)
// {
//     return p.x * p.x + p.y * p.y;
// }

// void baseTest()
// {
//     struct Point2D p2d;
//     p2d.x = 3;
//     p2d.y = 4;
//     int L = L2Len(p2d);
//     checkEquals(L, 25, __LINE__);
// }




// 实现链表数据结构
// 实现二叉树数据结构
// 翻转链表并比对输出结果是否一致
// 翻转二叉树并比对结果是否一致
// 层序输出二叉树

