#include <lscio.h>


void exit()
{
    // 使用除零错误来强制结束程序
    // 后续考虑使用系统调用实现此函数
    int d = 3/0;
}


void checkTrue(int value)
{
    if(value){
        return;
    }
    else{
        lscPrintStr("checkTrue: is not True");
        exit();
    }
}

void checkFalse(int value)
{
    if(value){
        lscPrintStr("checkFalse: is not False");
        exit();
    }
}

int main()
{
    int true = 1;
    int false = 0;

    checkTrue(true);
    checkFalse(false);

    checkTrue(true&&true);
    checkFalse(true&&false);

    checkTrue(true||false);
    checkFalse(false||false);

    checkTrue(!false);
    checkFalse(!true);

    return 0;
}


