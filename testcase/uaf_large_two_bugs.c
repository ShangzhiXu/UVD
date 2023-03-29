#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(){
    char *a1;
    char *b1;
    char *c1;
    char *d1;
    int con = 1;
    a1 = malloc(sizeof (char));
    b1 = malloc(sizeof (char));
    c1 = malloc(sizeof (char));
    d1 = malloc(sizeof (char));
    free(a1);
    strlen(a1);

    if (con == 1){
        free(b1);
    }else{
        strlen(b1);
    }
    free(c1);
    if (con == 1)
        strlen(c1);
    else
        con = 2;

    strlen(d1);


}