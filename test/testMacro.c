#define MAX_SIZE 10
#define acd 12

void lscPrintStr(char* str);
void lscPrintInt(int n);

int main()
{
    int a[MAX_SIZE];
    a[0] = __LINE__;

    lscPrintInt(a[0]);lscPrintStr("\n");

    char* str = "MAX_SIZE";
    lscPrintStr(str);lscPrintStr("\n");

    int maybe = acd;
    lscPrintInt(maybe);lscPrintStr("\n");
    
    lscPrintStr("Finish!");
}