
#include "stdio.h"
#include "stdlib.h"



int main(){
    char *a1;
    char *b1;
    a1 = malloc(sizeof (char));
    b1 = malloc(sizeof (char));
    *a1 = random();
    free(a1);
    printf("%c",*a1);
    printf("%c",*b1);



}
