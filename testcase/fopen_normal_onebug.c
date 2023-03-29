
#include "stdio.h"
#include "stdlib.h"

int main(){
    char *a1, *b1;
    a1 = fopen("123", "w+");

    fclose(a1);

    printf("%s", a1);


}
