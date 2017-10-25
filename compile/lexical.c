#include <stdio.h>

#define BUFLEN 80

int lineLen = 0;
int readPos = -1;
char line[BUFLEN];
int lineNum = 1;
int colNum = 0;
char ch;
char lastch;

char scan(FILE* file)
{
    //没有指向文件,直接返回
    if(!file){
        return -1;
    }

    if(readPos == lineLen-1){
        lineLen = fread(line,1,BUFLEN,file);
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
        fclose(file);
        file = NULL;
    }
    else if(ch != '\n'){
        colNum++;
    }

    lastch = ch;
    return ch;
}



enum Symbol sym;      // 待返回的词法记号
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
      getChar();
    }
}
