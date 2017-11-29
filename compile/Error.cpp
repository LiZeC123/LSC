#include "Error.h"
#include "Scanner.h"

Scanner* Error::scanner = nullptr;
int Error::warnNum = 0;
int Error::errorNum = 0;

Error::Error(Scanner * sc)
{
    scanner = sc;
}

int Error::getWarnNum()
{
    return Error::warnNum;
}

int Error::getErrorNUm()
{
    return errorNum;
}

void Error::lexError(int code)
{
    //词法错误信息串
	static const char *lexErrorTable[]=
	{
		"字符串丢失右引号",
		"二进制数没有实体数据",
		"十六进制数没有实体数据",
		"字符丢失右单引号",
		"不支持空字符",
		"错误的或运算符",
		"多行注释没有正常结束",
		"词法记号不存在"
	};
	errorNum++;
    printf("%s<%d行,%d列> 词法错误 : %s.\n",
        scanner->getFilename(),
        scanner->getRow(),scanner->getCol(),
        lexErrorTable[code]);
}

void Error::lexWarm(int code)
{
    //词法警告信息串
    static const char* lexWarnTable[] =
    {
        "文件未扫描结束"
    };
    warnNum++;
    printf("%s<%d行,%d列> 词法警告 : %s.\n",
        scanner->getFilename(),
        scanner->getRow(),scanner->getCol(),
        lexWarnTable[code]);
}