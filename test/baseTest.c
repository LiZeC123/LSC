char* globalChar;

int printf(char* s);

int main()
{
    char* str = "Hello World!\n";
    int n = printf(str);
    int fi = 5,fj = 4,fk = 3;
    int fsum;
    fsum = fi+fj*fk;
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