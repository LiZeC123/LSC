
#include "common.h"
#include <stdio.h>

static const char *ErrorTable[] =
    {
        "字符串缺少右引号",
        "二进制数字缺少数据实体",
        "十六进制数字缺少数据实体",
        "字符缺少右引号",
        "不支持空字符",
        "不支持的运算符,是否应该是\"||\"",
        "多行注释缺少结束符",
        "未定义的符号",
        "标识符过长",
        "数字常量过长",
        "字符串常量过长"
    };


void lexError(ErrCode code)
{
    //printf("code = %d\n",code);
    printf("%s <%d %d> 词法错误:%s\n", filename, lineNum, colNum, ErrorTable[code]);
}