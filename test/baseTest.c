char* globalChar;

int printf(char* s);
void int2str(char* buf,int i);

int main()
{
    char str[20];
    int fi = 5,fj = 4,fk = 3;
    int fsum;
    fsum = fi+fj*fk;
    int2str(str,fsum);
    printf(str);
    return fsum;
}

int testAssgin()
{
    int tAs = 10;
    int* ptAs = &tAs;

    int re = *ptAs;
    int* pother = &re;

    *pother = re;
}

int testArray()
{
    int array[10];

    array[1] = 5;
    array[2] = 0;

    int arrayRec;
    arrayRec = array[1] + array[2];

}

void testIf()
{
    int n = 5;

    if(n == 5){
        printf("true");
    }
    else{
        printf("false");
    }
}

void testWhile()
{
    int i = 0;
    while(i < 5){
        testIf();
        i++;
    }
}

void testBreak()
{
    int i = 0;
    while(i < 5){
        int j = 0;
        while(j < 5){
            if(j != 2){
                int n = 0;
                ++n;
                continue;
            }
            else{
                int m = 0;
                ++m;
                break;
            }
        }
    }
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