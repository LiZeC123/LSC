// #include "00testhelper.h"
void lscPrintInt(int n);
void lscPrintStr(char *s);
int lscp2i(int *point);

// int main(int argc, char *argvs)
// {
//     char *s = argvs;
//     int i = 0;
//     char *argv[4];
//     //lscPrintStr("argvs=");
//     //lscPrintStr(argvs);

//     argv[0] = s;
//     while (i < argc)
//     {
//         while (*s != '\0')
//         {
//             ++s;
//         }
//         ++s;
//         argv[i] = s;
//         ++i;
//     }
    
//     lscPrintStr(argv[0]);lscPrintStr("\n");
//     lscPrintStr(argv[1]);lscPrintStr("\n");
//     lscPrintStr(argv[2]);lscPrintStr("\n");
    

//     return 0;
// }

int main(int argc, char* argv[]) 
{
    for(int i=0;i<argc;i++){
        lscPrintStr(argv[i]);
        lscPrintStr("\n");
    }
    return 0;
}

