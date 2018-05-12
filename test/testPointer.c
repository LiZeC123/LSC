
int f(){
    char c;
    char* pc;
    char** ppc;

    c = 'c';
    pc = &c;
    ppc = &pc;

    pc = *ppc;

    c = *pc;
    c = **ppc;

    char d;
    *pc = d;
    **ppc = d;
}


int g() {
    char* arr[5];
    char* pc;
    arr[0] = pc;
    arr[1] = *(&pc);


    char* pc2;
    pc2 = arr[0];

    *(arr+2) = pc2;
}


int main(int argc){
    f();
    g();
}
