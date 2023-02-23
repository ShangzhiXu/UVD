
#include "stdio.h"
#include "stdlib.h"



int main(){
    char *a1;
    char *b1;
    char *c1;
    a1 = malloc(sizeof (char));
    b1 = malloc(sizeof (char));
    c1 = malloc(sizeof (char));
    *a1 = random();
    b1 = a1;
    c1 = b1;
    free(b1);
    printf("%c",*c1);



}
