char* globalChar;

int printf(char* s);

int main()
{
    char* str = "Hello World!\n";
    printf(str);
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