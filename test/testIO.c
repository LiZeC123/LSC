void lscRead(char* buf,int maxLen);
void lscWrite(char* buf,int len);

int lscstr2int(char* str);
void lscint2str(char* buf,int n);

void lscPrintStr(char* str);
void lscPrintInt(int n);

int lscReadInt();


int lscstr2int(char* buf)
{
	int count = 0;
	int asw = 0;
	int op = 1;
	if(buf[count] == '-'){
		op = -1;
		++count;
	}
	
	while(buf[count] != '\n'){
		int d = buf[count] - 48;
		asw = asw * 10 + d;
		++count;
	}
	
	return asw;
}

int lscReadInt()
{
    char buf[20];
    lscRead(buf,20);
    return lscstr2int(buf);
}

void lscPrintStr(char* str)
{
    int count = 0;
    while(*(str+count) != '\0'){
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
		*(buf+idx) = tmp[count];
		--count;
        ++idx;
	}
    *(buf+idx) = '\0';
}

void lscPrintInt(int n)
{
    char buf[20];
    lscint2str(buf,n);
    lscPrintStr(buf);
}