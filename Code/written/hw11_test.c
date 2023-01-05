#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h> 
#include "lib/contracts.h"
#include "lib/xalloc.h"

void mirror(char *A, char *res) {
    REQUIRES(A != NULL);
    REQUIRES(res != NULL);

    int n = strlen(A);
    for (int i = 0; i < n; i++) {
        res[i] = A[i];
        res[n+i] = A[n-i-1];
    }
    res[2*n] = '\0';
}

int main() {
    // char A[] = "hi!";
    char B[] = {'h', 'i','!'};
    // char *C = "hi!";
    // char *D = malloc(sizeof(char) * 4);
    // strncpy(D, C, 4);
    // char *E = D;

    // char *a = malloc(sizeof(char) * 7);
    char b[7];
    // char c[] = "hi";
    // char *d = "goodbye";
    // char *e = malloc(sizeof(char) * 12);

    // mirror(A,a);
    mirror(B,b);
    // mirror(C,c);
    // mirror(D,d);
    // mirror(E,e);
    //mirror(A,b); free(b);
    // mirror(C,e); free(e);
    // free(d); mirror(E,c);
    // free(a); mirror(B,d);
    // mirror(D,a); free(D);

    printf("%s", b);
    return 0;
}