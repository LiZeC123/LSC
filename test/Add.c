void lscRead(char* buf,int maxLen);
void lscWrite(char* buf,int len);
int lscstr2int(char* str);
void lscint2str(char* buf,int n);
void lscPrintStr(char* str);
void lscPrintInt(int n);
int lscReadInt();

int main()
{
   lscPrintStr("please input a:\n");
   int a = lscReadInt();
   lscPrintStr("please input b:\n");
   int b = lscReadInt();

   int c = a + b;
   lscPrintStr("the answer is ");
   lscPrintInt(c);
   lscPrintStr("\n");
    
}