#include "Error.h"

using namespace std;

static const char* errorTab[] = {
    "无法打开源文件",
    "无法打开目标文件",
    "编译预处理指令不能为空",
    "指定的文件名无效",
    "无效的编译预处理指令"
};


void Error::preError(PreError code, const string& filename, int rol)
{
    printf("%s 预处理错误<第%d行> %s\n",filename.c_str(),rol,errorTab[code]);
}