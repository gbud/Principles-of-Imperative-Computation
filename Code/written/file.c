#include <stdio.h>
#include <stdlib.h>

int main() {
    int *n = malloc(sizeof(int));
    *n = 15122;

    // int *A;
    // *A = 6;

    if (*n == 15122) printf("Yay!\n");
    else printf("Blasphemy\n");

    free(n+1);
    return 0;
}
