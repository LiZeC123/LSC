char* globalChar;


int lscPrints(char* str,int n);
void int2str(char* buf,int i);
int testPrintNum();

int main()
{
    globalChar = "Hello World!";
    lscPrints(globalChar,12);
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
    int fi = 5,fj = 4,fk = 3;
    int fsum;
    fsum = fi+fj*fk;
    int2str(str,fsum);
    lscPrints(str,2);
}

void int2str(char* buf,int i)
{
    int count = 0;
    while(i != 0){
        int d = i % 10;
        *(buf+count) = d + 48;
        count++;
        i = i / 10;
    }
}