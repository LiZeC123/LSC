#include <stdio.h>
#include "common.h"
#include <string.h>

#define BUF_LEN 80     // 缓冲区最大长度
int lineLen = 0;       // 当前行长度
int readPos = -1;      // 但前字符的位置
char line[BUF_LEN];    // 字符缓冲区
int lineNum = 1;       // 行号
int colNum = 0;        // 列号
char ch;               // 当前字符
char lastch;           // 上一个字符
//需要全局定义的文件指针fin
char scan()
{
    //没有指向文件,直接返回
    if(!fin){
        return -1;
    }

    if(readPos == lineLen-1){
        lineLen = fread(line,1,BUF_LEN,fin);
        if(lineLen == 0){
            lineLen = 1;
            line[0] = -1;
        }
        readPos = -1;
    }
    readPos++;

    ch = line[readPos];
    if(lastch == '\n'){
        lineNum++;
        colNum=0;
    }

    if(ch == -1){
        fclose(fin);
        fin = NULL;
    }
    else if(ch != '\n'){
        colNum++;
    }

    lastch = ch;
    return ch;
}



Symbol sym;           // 待返回的词法记号
char id[ID_LEN+1];    // 标识符数组
int num;              // 存放数字
char str[STR_LEN+1];  // 存放字符串常量
char letter;          // 存放字符常量
double dnum;          // 存放浮点数常量
int getSym()
{
    // 掠过空白字符
    while(ch==' '||ch==10||ch==9)
    {
      scan();
    }

    if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch=='_')){
        int idcount = 0;
        int realCount = 0;
        int f;

        do{
            if(idcount < ID_LEN){
                id[idcount] = ch;
                idcount++;
            }
            realCount++;
            f = scan();

        }while((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch=='_')||(ch>='0'&&ch<='9'));

        id[idcount] = '\0';

        if(realCount > ID_LEN){
            // 标识符过长
        }
        checkKeyWord();
        return f;
    }
    else if((ch>='0')&&(ch<='9')){
        sym = NUM;
        int f = getNumber();
        return f;
    }
    else if(ch=='"'){
        sym = STR;
        int strCount = 0;
        int realCount = 0;

        checkedScan;
        while(ch != '"'){
            if(strCount < STR_LEN){
                if(ch=='\\'){
                    checkedScan;
                    switch(ch){
                        case 't':
                            str[strCount] = '\t';
                            break;
                        case 'n':
                            str[strCount] = '\n';
                            break;
                        case '"':
                            str[strCount] = '"';
                            break;
                        case '0':
                            str[strCount] = '0';
                            break;
                        default:
                            str[strCount] = ch;
                    }
                }
                else{
                    str[strCount] = ch;
                }
                strCount++;
            }
            realCount++;
            checkedScan;
        }
        //实际上,还需要检测是否有换行符和文件结束,否则会导致换行的字符串
        // 最后读取掉结束的"
        scan();
        str[strCount] = '\0';
        if(realCount > STR_LEN){
            // 字符串过长
        }
    }
    else if(ch=='\''){
        sym = CH;
        char c;
        scan();
        if(ch=='\\'){
            scan();
            switch(ch){
                case 't':
                    c = '\t';
                    break;
                case 'n':
                    c = '\n';
                    break;
                case '"':
                    c = '"';
                    break;
                case '0':
                    c = '0';
                    break;
                default:
                    c = ch;
            }
            letter = c; 
        }
        else if(ch=='\n' || ch==-1){
            sym = ERR;
            // 缺少右引号
        }
        else if(ch=='\''){
            sym = ERR;
            // 空字符
            //读取掉此引号,从而不影响之后的分析
            scan();
        }
        else{
            //其他普通字符
            letter = ch;
        }

        if(sym != ERR){
            scan();
            if(ch == '\''){
                sym = CH;
                //匹配到右引号,读取掉此符号
                checkedScan;
            }
            else{
                sym = ERR;
                // 缺少右括号
            }
        }
    }
    else{
        switch(ch){
            case '+':
                sym = ADD;
                checkedScan;
                if(ch=='+'){
                    sym = INC;
                    checkedScan;
                }
                break;
            case '-':
                sym = SUB;
                checkedScan;
                if(ch=='-'){
                    sym = DEC;
                    checkedScan;
                }
                break;
            case '*':
                sym = MUL;
                checkedScan;
                break;
            case '%':
                sym = MOD;
                checkedScan;
                break;
            case '>':
                sym = GT;
                checkedScan;
                if(ch=='='){
                    sym = GE;
                    checkedScan;
                }
                else if(ch==">"){
                    sym = KW_IN;
                    checkedScan;
                }
                break;
            case '<':
                sym = LT;
                checkedScan;
                if(ch=='='){
                    sym = LE;
                    checkedScan;
                }
                else if(ch=='<'){
                    sym = KW_OUT;
                    checkedScan;
                }
                break;
            case '=':
                sym = ASSIGN;
                checkedScan;
                if(ch=='='){
                    sym = EQU;
                    checkedScan;
                }
                break;
            case '&':
                sym = LEA;
                checkedScan;
                if(ch=='&'){
                    sym = AND;
                    checkedScan;
                }
                break;
            case '!':
                sym = NOT;
                checkedScan;
                if(ch=='='){
                    sym = NEQU;
                    checkedScan;
                }
                break;
            case ',':
                sym = COMMA;
                checkedScan;
                break;
            case ':':
                sym = COLON;
                checkedScan;
                break;
            case ';':
                sym = SEMICON;
                checkedScan;
                break;
            case '(':
                sym = LPAREN;
                checkedScan;
                break;
            case ')':
                sym = RPAREN;
                checkedScan;
                break;
            case '[':
                sym = LBRACK;
                checkedScan;
                break;
            case ']':
                sym = RBRACK;
                checkedScan;
                break;
            case '{':
                sym = LBRACE;
                checkedScan;
                break;
            case '}':
                sym = RBRACE;
                checkedScan;
                break;
            case '|':
                sym = ERR;
                checkedScan;
                if(ch=='|'){
                    sym = OR;
                }
                else{
                    // 错误的符号 |
                }
                break;
            case '/':
                // 注释都视为无效的词
                sym = ERR;
                // 已经读入一个/,可能是除号,因此不能因为下一个字符是文件尾就退出
                scan();
                // 是单行注释
                if(ch='/'){
                    while(ch!='\n'){
                        checkedScan;
                    }

                }
                // 是多行注释
                else if(ch='*'){
                    do{
                        checkedScan;
                        while(ch=='*'){
                            checkedScan;
                            if(ch=='/'){
                                break;
                            }
                        }
                    }while(1);
                }
                // 只是除号
                else{
                    sym = DIV;
                }
                break;
            case -1:
                // 匹配到此情况说明在上一轮的最后读取到文件末尾
                // 只使用-1表示文件尾,不产生词法符号
                return -1;
            default:
                //TODO: 未定义的符号
                

                checkedScan;
                
        }
    }







    //TODO: 其他类型的判断
    return 0;
}



#define KEYWORD_NUM 16
static char keyWordTable[KEYWORD_NUM][ID_LEN]={"break","case","char","continue","default","double","else","extern","if","in","int","out","return","string","void","while"};
static Symbol keyWordSymbol[KEYWORD_NUM]={KW_BREAK,KW_CASE,KW_CHAR,KW_CONTINUE,KW_DEFAULT,KW_DOUBLE,KW_ELSE,KW_EXTERN,KW_IF,KW_IN,kW_INT,KW_OUT,KW_RETURN,KW_STRING,KW_VOID,KW_WHIILE};

void checkKeyWord()
{
  int i=0,j=KEYWORD_NUM-1,k=0;
  do
  {
    k=(i+j)/2;//折半查找
    if(strcmp(id,keyWordTable[k])<0)
    {
      j=k-1;
    }
    else if(strcmp(id,keyWordTable[k])>0)
    {
      i=k+1;
    }
    else
    {
      //找到了,是关键字
      sym = keyWordSymbol[k];
      break;
    }
  }
  while(i<=j);
  if(i>j){
      //搜索失败，是标识符
      sym=IDENT;
  }
}

#define BIN_NUM_LEN 32
#define OCT_NUM_LEN 10
#define DEC_NUM_LEN 9
#define HEX_NUM_LEN 8

int getNumber()
{
    num = 0;
    int numCount = 0;
    int realCount = 0;
    int f;

    if(ch=='0'){
        f = scan();
        if(ch=='b'||ch=='B'){
            // 2进制
            f = scan();
            do{
                if(numCount < BIN_NUM_LEN){
                    if(ch>='0'&&ch<='1'){
                        num = num*2+(ch-'0');
                    }
                    else{
                        // 数字格式错误
                        // 由于循环条件的检测,只有第一次进入循环体时,有可能发生这一情况
                    }

                    numCount++;
                }
                realCount++;
                f = scan();
            }while(ch>='0'&&ch<='1');

            if(realCount > BIN_NUM_LEN){
                // 数字过长
            }
            return f;
        }
        else if(ch=='x'||ch=='X'){
            // 16进制
            f = scan();
            do{
                if(numCount < HEX_NUM_LEN){
                    if(ch>='0'&&ch<='9'){
                        num = num*16+(ch-'0');
                    }
                    else if(ch>='a'&&ch<='f'){
                        num = num*16+(ch-'a');
                    }
                    else if(ch>='A'&&ch<='F'){
                        num = num*16+(ch-'A');
                    }
                    else {
                        // 数字格式错误
                        // 由于循环条件的检测,只有第一次进入循环体时,有可能发生这一情况
                    }
                    numCount++;
                }
                realCount++;
                f = scan();
            }while((ch>='0'&&ch<='9')||(ch>='a'&&ch<='f')||(ch>='A'&&ch<='F'));

            if(realCount > HEX_NUM_LEN){
                // 数字过长
            }
            return f;
        }
        else{
             //八进制
            do{
                if(numCount < OCT_NUM_LEN){
                    if(ch>='0'&&ch<='7'){
                        num = num*8+(ch-'0');
                    }
                    else{
                        // 数字格式错误
                        // 由于循环条件的检测,只有第一次进入循环体时,有可能发生这一情况
                    }

                    numCount++;
                }
                realCount++;
                f = scan();
            }while(ch>='0'&&ch<='7');

            if(realCount > OCT_NUM_LEN){
                // 数字过长
            }
            return f;
        }
    }
    else{
        // 10进制
        do {
            if(numCount < DEC_NUM_LEN){
                num = num * 10 + (ch - '0');
                numCount++;
            }
            realCount++;
            f = scan();
        } while (ch>='0'&&ch<='9');

        if(realCount > DEC_NUM_LEN){
            // 数字过长
        }
        return f;
    }
}


/*****************************测试用主函数***********************************************/
char* sym2Name(int s)
{
    static char* names[] = {
        "ERR",                                // 错误
        "END",                                // 文件结束
        "IDENT",                              // 标识符
        "kW_INT","KW_CHAR","KW_VOID","KW_DOUBLE",   // 数据类型
        "KW_EXTERN",                          //extern
        "NUM","CH","STR",                         //常量
        "NOT","LEA",                            // 单目运算符 ! & - *
        "ADD","SUB","MUL","DIV","MOD",                //算数运算符
        "INC","DEC",                            // ++ --
        "GT","GE","LT","LE","EQU","NEQU",               // 关系运算符
        "AND","OR",                             // 逻辑运算符
        "LPAREN","RPAREN",                      // ()
        "LBRACK","RBRACK",                      // []
        "LBRACE","RBRACE",                      // {}
        "COMMA","COLON","SEMICON",                // , : ;
        "ASSIGN",                             // 赋值
        "KW_IF","KW_ELSE",                      // if-else
        "KW_SWITCH","KW_CASE","KW_DEFAULT",       // switch-case
        "KW_WHIILE","KW_DO","KE_FOR",             // 循环语句
        "KW_BREAK","KW_CONTINUE","KW_RETURN",     // 流程控制
        "KW_IN","KW_OUT","KW_STRIN"              // 其他关键字
    };
    return names[s];
}


FILE * fin;
int main(){
    fin = fopen("/home/lizec/CWorkSpace/lsc/main.c","r");
    
    scan();
    while(getSym() != -1){
        if(sym == IDENT){
            printf("id=%s\n",id);
        }
        else if(sym == STR){
            printf("str=%s\n",str);
        }
        else if(sym == NUM){
            printf("num=%d\n",num);
        }
        else if(sym == CH){
            printf("ch=%c\n",letter);
        }
        else{
            printf("type=%s\n",sym2Name(sym));
        }
    }

    return 0;
}