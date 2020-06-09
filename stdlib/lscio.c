#include "lscio.h"

int lscstr2int(char* buf,int maxLen)
{
	int count = 0;
	int asw = 0;
	int op = 1;
	if(buf[count] == '-'){
		op = -1;
		++count;
	}
	
	while(buf[count] != '\n' && buf[count] != '\0' && count < maxLen){
		int d = buf[count] - 48;
		asw = asw * 10 + d;
		++count;
	}

    
	return op * asw;
}

int lscReadInt()
{
    char buf[12];
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
    if(i == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    // 如果是负数, 则转成正数处理, 但是会导致-MAX错误, 影响不大, 暂时不处理这一情况
    int negitive = 0;
    if(i < 0) {
        negitive = 1;
        i = -i;
    }

    // 2^31 - 1 = 2147483647  
	char tmp[12];
    int count = 0;
    while(i != 0){
        int d = i % 10;
		tmp[count] = d + 48;
        count++;
        i = i / 10;
    }
	
    int idx = 0;

    if(negitive) {
        buf[idx++] = '-';
    }

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
    char buf[12];
    lscint2str(buf,n);
    lscPrintStr(buf);
}
