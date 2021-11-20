#include <stdio.h>

int main() {
    for(int i=0;i<15;i++){
        printf("%d\n", i + (4 - i % 4) % 4);
    }
}