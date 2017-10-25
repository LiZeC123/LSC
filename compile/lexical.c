#include <stdio.h>
#include "common.h"
#include <stdbool.h>
#include <string.h>
#define BUF_LEN 80     // 缓冲区最大长度
int lineLen = 0;      // 当前行长度
int readPos = -1;     // 但前字符的位置
char line[BUF_LEN];    // 字符缓冲区
int lineNum = 1;      // 行号
int colNum = 0;       // 列号
char ch;              // 当前字符
char lastch;          // 上一个字符
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
        int f = getNumber();
        return f;
    }



    //TODO: 其他类型的判断
    return 0;
}



#define KEYWORD_NUM 14
static char keyWordTable[KEYWORD_NUM][ID_LEN]={"break","char","continue","double","else","extern","if","in","int","out","return","string","void","while"};
static Symbol keyWordSymbol[14]={KW_BREAK,KW_CHAR,KW_CONTINUE,KW_DOUBLE,KW_ELSE,KW_EXTERN,KW_IF,KW_IN,kW_INT,KW_OUT,KW_RETURN,KW_OUT,KW_VOID,KW_WHIILE};

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
    sym = NUM;
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
