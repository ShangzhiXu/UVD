#include "stdio.h"
#include "stdlib.h"
static char* c1;
int main(){
    char *a1;
    a1 = malloc(sizeof (char));
    c1 = a1;
    free(c1);
    printf("%c",*a1);

}