#include <string>
#include "common.h"

Symbol oldtoken = M_NULL, token = M_NULL; //最近的两个符号
int isWait = 0;                           //指示nexttoken是否读取下一个符号,从而可以一定程度实现回退
int showToken = 0;                        //指示是否显示输出词法分析结果
#define BACL isWait=1

void printTokenInfo();
int nextToken();
int match(Symbol s);
void program();
void segment();
Symbol type();
void def();

int nextToken()
{
    if(isWait)
    {
        isWait = 0;
        return 0;
    }

    int flag = 0;

    while(1)
    {
        flag = getSym();
        if( sym == ERR || flag == -1){         // 无效的符号掠过,例如注释
            if(flag == -1){       // 到达文件末尾,结束
                oldtoken = token;
                token = M_NULL;
                return -1;
            }
        }
        else
        {
            if(showToken){
                printTokenInfo();
            }
            oldtoken = token;
            token = sym;
            return 0;
        }
    }
}


int match(Symbol s)
{
    return (token == s);
}


int isCompileFinish = 0;
// <program> -> <segment><program>
void program()
{
    if(nextToken() == -1)
    {
        isCompileFinish = 1;

        if(errorNum == 0){
            printf("编译完成! (");
        }
        else{
            printf("编译失败! (");
        }
        printf("%d 错误 %d 警告)\n",errorNum,warnNum);
  }
  else{
      segment();
      program();
  }
}

// <segment> -> extern <type><def>
// <segment> -> <type><def>
void segment()
{
    if(token == SEMICON)
    {
        return;
    }
    else if(token == KW_EXTERN)
    {
        Symbol ttype;
        char name[31];

        nextToken();
        ttype = type();
        nextToken();
        def();
    }
    else
    {
        Symbol ttype;
        ttype = type();
        nextToken();
        def();
    }
}

// <type>    -> int | void | char | double
Symbol type()
{
    switch(token)
    {
    case kW_INT:
        return kW_INT;
    case KW_VOID:
        return KW_VOID;
    case KW_CHAR:
        return KW_CHAR;
    case KW_DOUBLE:
        return KW_DOUBLE;
    case IDENT:
        // 匹配到标识符,可能遗漏了类型
        break;
    default:
        ;
        // 匹配到其他符号
    }
    return M_NULL;
}

void def()
{
    // TODO
}



const char *sym2Name(int s)
{
    static std::string names[] = {
        "ERR",                                       // 错误
        "END",                                       // 文件结束
        "IDENT",                                     // 标识符
        "NUM", "CH", "STR",                          //常量
        "!", "&",                                    // 单目运算符 ! & 
        "+", "-", "*", "/", "%%",                    //算数运算符
        "++", "--",                                  // ++ --
        ">", ">=", "<", "<=", "==", "!=",            // 关系运算符
        "&&", "||",                                  // 逻辑运算符
        "(", ")",                                    // ()
        "[", "]",                                    // []
        "{", "}",                                    // {}
        ",", ":", ";",                               // , : ;
        "=",                                         // 赋值
        " ",                                         // 空白,与符号声明对应
        "kW_INT", "KW_CHAR", "KW_VOID", "KW_DOUBLE", // 数据类型
        "KW_EXTERN",                                 //extern
        "KW_IF", "KW_ELSE",                          // if-else
        "KW_SWITCH", "KW_CASE", "KW_DEFAULT",        // switch-case
        "KW_WHIILE", "KW_DO", "KE_FOR",              // 循环语句
        "KW_BREAK", "KW_CONTINUE", "KW_RETURN",      // 流程控制
        "KW_IN", "KW_OUT", "KW_STRING"               // 其他关键字
        " ",                                         // 空白,与符号声明对应
        "IS_NULL"
    };
    return names[s].c_str();
}


void printTokenInfo()
{
    if (sym == IDENT)
    {
        printf("标识符\t\t%s\n", id);
    }
    else if (sym == STR)
    {
        printf("字符串\t\t%s\n", str);
    }
    else if (sym == NUM)
    {
        printf("数字\t\t%d\n", num);
    }
    else if (sym == CH)
    {
        printf("字符\t\t%c\n", letter);
    }
    else if(sym > _KW_BEGIN_ && sym < _KW_END_){
        printf("关键字\t\t%s\n",sym2Name(sym));
    }
    else
    {
        printf("界符\t\t%s\n", sym2Name(sym));
    }
}

