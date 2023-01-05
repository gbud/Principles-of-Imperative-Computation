#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h> 
#include "lib/contracts.h"
#include "lib/xalloc.h"
#include "lib/hdict.h"
#include "lib/queue.h"

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

