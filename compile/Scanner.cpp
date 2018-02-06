#include "common.h"
#include "Scanner.h"
#include "Error.h"
Scanner::Scanner(const char* name)
{
    this->filename = name;
    this->fin = fopen(filename,"r");
}

int Scanner::scan()
{
    //没有指向文件,直接返回
    if (!fin)
    {
        return -1;
    }

    if (readPos == lineLen - 1)
    {
        lineLen = fread(line, 1, BUF_LEN, fin);
        if (lineLen == 0)
        {
            lineLen = 1;
            line[0] = -1;
        }
        readPos = -1;
    }
    readPos++;

    ch = line[readPos];
    if (lastch == '\n')
    {
        lineNum++;
        colNum = 0;
    }

    if (ch == -1)
    {
        fclose(fin);
        fin = NULL;
    }
    else if (ch != '\n')
    {
        colNum++;
    }

    lastch = ch;
    return ch;
}

const char* Scanner::getFilename()
{
    return filename;
}

int Scanner::getRow()
{
    return lineNum;
}

int Scanner::getCol()
{
    return colNum;
}

Scanner::~Scanner()
{
    if(fin)
	{
        Error::lexWarm(FILE_NOT_FINISH);
		fclose(fin);
	}
}

void Scanner::showChar()
{
    if(ch==-1)printf("EOF");
	else if(ch=='\n')printf("\\n");
	else if(ch=='\t')printf("\\t");
	else if(ch==' ')printf("<blank>");
	else printf("%c",ch);	
	printf("\t\t<%d>\n",ch);
}