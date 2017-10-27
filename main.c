int main(){
    int books = 3;
    int num = 754321;
    char c = '4';
    char f = '\n';
    int hexnum = 0x1fac0310; //531366672
    int tom = "Tom and Jerry";
    fopen("/home/lizec/CWorkSpace/lsc/main.c","r");
    //注释
    /****key*******/

    if(books >= 3){
        while(num <= 4){
            if(c != 4){
                do{
                    printf("Hello World");
                }while(tom == 4);
            }
        }
    }
    else 
    {
        books = 3;
    }

    switch(c){
        case '1':
            books++;
        case '2':
            num--;
        default:
            return -1;
    }

    return 0;
}

extern int fval;


void f(int a,int b){
    return a + b;
}
