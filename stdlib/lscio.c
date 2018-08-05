#include "lscio.h"

// 2^31 = 2147483648
int lscstr2int(char* buf,int maxLen)
{
	int count = 0;
	int asw = 0;
	int op = 1;
	if(buf[count] == '-'){
		op = -1;
		++count;
	}
	
	while(buf[count] != '\n' && count < maxLen){
		int d = buf[count] - 48;
		asw = asw * 10 + d;
		++count;
	}
	
	return asw;
}

int lscReadInt()
{
    char buf[10];
    lscRead(buf,10);
    return lscstr2int(buf,10);
}

void lscPrintStr(char* str)
{
    int count = 0;
    while(str[count] != '\0'){
        ++count;
    }
    ++count;

    lscWrite(str,count);
}


void lscint2str(char* buf,int i)
{
	char tmp[10];
    int count = 0;
    while(i != 0){
        int d = i % 10;
		tmp[count] = d + 48;
        count++;
        i = i / 10;
    }
	
    int idx = 0;
    --count;
	while(count >= 0){
        buf[idx] = tmp[count];
		--count;
        ++idx;
	}
    buf[idx] = '\0';
}

void lscPrintInt(int n)
{
    char buf[10];
    lscint2str(buf,n);
    lscPrintStr(buf);
}
