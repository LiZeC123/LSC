void lscRead(char* buf,int maxLen);
void lscWrite(char* buf,int len);
int lscstr2int(char* str);
void lscint2str(char* buf,int n);
void lscPrintStr(char* str);
void lscPrintInt(int n);
int lscReadInt();

int main()
{
    lscPrintStr("Please input a num:\n");
    int n = lscReadInt();
    lscPrintStr("this num is ");
    lscPrintInt(n);
    lscPrintStr("\n");
}