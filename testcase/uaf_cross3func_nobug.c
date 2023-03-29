
#include "stdio.h"
#include "stdlib.h"

void swap(char* a1, char* b1){
    b1 = a1;
}
void FREE(char * a){
    free(a);
}

int main(){
    char *a1, *b1;
    a1 = malloc(sizeof(char));
    swap(a1,b1);
    FREE(a1);

}
