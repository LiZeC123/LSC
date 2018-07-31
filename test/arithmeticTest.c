#include <lscio.h>


void exit()
{
    // 使用除零错误来强制结束程序
    // 后续考虑使用系统调用实现此函数
    int d = 3/0;
}

// TODO: 添加行号宏,从而实现定位
void checkEquals(int a,int b)
{
    if(a != b){
        lscPrintStr("checkEquals:");lscPrintInt(a);lscPrintStr(" != ");lscPrintInt(b);lscPrintStr("\n");
        exit();
    }
}

int main()
{
    checkEquals(8,8);

    // 测试加法
    int a = 3; int b = 5;

    int c,d;

    c = a + b;
    d = 3 + 5;
    
    checkEquals(c,8);
    checkEquals(d,8);
    
    // 测试乘法
    a = 233; b = 332;
    c = a * b;
    d = 233 * 332;

    checkEquals(c,77356);
    checkEquals(d,77356);


    // 测试除法
    a = 6331; b = 3;
    c = a / b;
    d = 6331 / 3;

    checkEquals(c,2110);
    checkEquals(d,2110);

    // 测试混合运算
    a = 155; b = 486;
    int e = 983; int f = 283;

    c = e + f*(a+b) - a + e/f;
    d = 983 + 283*(155+486) - 155 + 983 / 283;

    checkEquals(c,182234);
    checkEquals(d,182234);
    
}