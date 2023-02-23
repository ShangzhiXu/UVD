#include "stdio.h"
#include "stdlib.h"

void delete(char* ptr){
    free(ptr);
}

int main(){
    char *a1, *b1;
    a1 = malloc(sizeof (char));
    b1 = malloc(sizeof (char));
    a1 = b1;
    delete(b1);
    printf("%c",*a1);

}