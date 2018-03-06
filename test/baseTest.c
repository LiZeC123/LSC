char* globalChar;


int lscPrints(char* str,int n);
void int2str(char* buf,int i);
int testPrintNum();

int main()
{
    globalChar = "Hello World!\n";
    lscPrints(globalChar,14);
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
    int2str(str,fsum);
	lscPrints(hit,13);
    lscPrints(str,3);
    lscPrints(rtn,2);
}

void int2str(char* buf,int i)
{
	char tmp[10];
    int count = 0;
    while(i != 0){
        int d = i % 10;
		tmp[count] = d + 48;
        *(buf+count) = d + 48;
        count++;
        i = i / 10;
    }
	
    int idx = 0;
	while(count >= 0){
		*(buf+idx) = tmp[count];
		--count;
        ++idx;
	}
    *(buf+idx) = '\0';
}
		


