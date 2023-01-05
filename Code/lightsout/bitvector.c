#include <stdlib.h>        // Standard C library: free(), NULL...  
#include <stdbool.h>       // Standard true, false, and bool type  
#include <string.h>        // Standard C version of lib/cstring.c0
#include <limits.h>
#include "lib/contracts.h" // Our contracts library  
#include "lib/xalloc.h"    // Our allocation library  
#include "lib/bitvector.h"

#if (BITVECTOR_LIMIT <= 8)
#define bitvector_bits uint8_t

#elif (BITVECTOR_LIMIT > 8 && BITVECTOR_LIMIT <= 16)
#define bitvector_bits uint16_t 

#elif (BITVECTOR_LIMIT > 16 && BITVECTOR_LIMIT <= 32)
#define bitvector_bits uint32_t

#elif (BITVECTOR_LIMIT > 32)
#define bitvector_bits uint64_t 
#endif

/* Get a new bitvector with everything set to 'false'. */
bitvector bitvector_new() {
    bitvector_bits result = 0;
    bitvector result_bv = (bitvector)result;
    return result_bv;
}

/* Get the ith bit of the bitvector n. */
bool bitvector_get(bitvector B, uint8_t i) {
    REQUIRES(i < BITVECTOR_LIMIT);
    bitvector_bits B_flexible = (bitvector_bits)B;
    bitvector_bits shift = BITVECTOR_LIMIT - i - 1;
    bitvector_bits cmp = (B_flexible >> shift) & 1;
    return cmp != 0;
}

/* Toggle the ith bit of the bitvector n, returning a new bitvector. */
/* The old bitvector remains unchanged. */
bitvector bitvector_flip(bitvector B, uint8_t i) {
    REQUIRES(i < BITVECTOR_LIMIT);
    bitvector_bits B_flexible = (bitvector_bits)B;
    bitvector_bits shift = BITVECTOR_LIMIT - i - 1;
    bitvector_bits new = B_flexible ^ (1 << shift);
    bitvector result_bv = (bitvector)new;
    return result_bv;
}

/* Compare two bitvectors for equality. */
bool bitvector_equal(bitvector B1, bitvector B2) {
    return (bitvector_bits) B1 == (bitvector_bits) B2;
}
