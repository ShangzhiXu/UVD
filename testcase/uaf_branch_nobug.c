#include "stdio.h"
#include "stdlib.h"

int main(){
    char *a1;
    a1 = malloc(sizeof (char));
    int con = 1;

    if(con == 1)
        free(a1);
    else
        printf("%c",*a1);

}