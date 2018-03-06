char* globalChar;


void lscRead(char* buf,int maxLen);
void lscWrite(char* buf,int len);
int lscstr2int(char* str);
void lscint2str(char* buf,int n);
void lscPrintStr(char* str);
void lscPrintInt(int n);
int lscReadInt();

int testPrintNum();

int main()
{
    globalChar = "Hello World!\n";
    lscPrintStr(globalChar);
    testPrintNum();
    return 0;
}

int testAssgin()
{
    int tAs = 8;
    int* ptAs = &tAs;

    int re = *ptAs;
    re = re + 2010;
    *ptAs = re;

    return tAs;
}

int testPrintNum()
{
    char str[20];
	char* hit = "5 + 4 * 3 = ";
    char* rtn = "\n";
    int fi = 5,fj = 4,fk = 3;
    int fsum;
    fsum = fi+fj*fk;
    lscint2str(str,fsum);
	lscPrintStr(hit);
    lscPrintStr(str);
    lscPrintStr(rtn);
}

		


