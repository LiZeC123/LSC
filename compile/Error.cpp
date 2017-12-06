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

void Error::synError(int code, Token* token)
{
    static const char * synErrorTable[] = 
    {
        "类型",
		"标识符",
		"数组长度",
		"常量",
		"逗号",
		"分号",
		"=",
		"冒号",
		"while",
		"(",
		")",
		"[",
		"]",
		"{",
		"}"
    };

    errorNum++;

    if(code % 2 == 0){
        printf("%s<第%d行> 语法错误 : 在 %s 之前丢失 %s .\n",scanner->getFilename(),
            scanner->getRow(),token->toString().c_str(),synErrorTable[code/2]);
    }
    else{
		printf("%s<第%d行> 语法错误 : 在 %s 处没有正确匹配 %s .\n",scanner->getFilename(),
            scanner->getRow(),token->toString().c_str(),synErrorTable[code/2]);
    }

}