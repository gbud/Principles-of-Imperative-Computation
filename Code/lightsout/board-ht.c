#include <stdlib.h>        // Standard C library: free(), NULL...  
#include <stdbool.h>       // Standard true, false, and bool type  
#include <string.h>        // Standard C version of lib/cstring.c0
#include <limits.h>
#include "lib/contracts.h" // Our contracts library  
#include "lib/xalloc.h"    // Our allocation library  
#include "lib/bitvector.h"
#include "lib/hdict.h"
#include "board-ht.h"

void *entry_key(void *e) {
    struct board_data *entry = (struct board_data*)e;
    void *key = (void*)&entry->board;
    return key;
}

bool key_equal(void *k1, void *k2) {
    REQUIRES(k1 != NULL && k2 != NULL);
    return bitvector_equal(*(bitvector*)k1, *(bitvector*)k2);
}

size_t key_hash(void *k) {
    size_t result = BITVECTOR_LIMIT;
    for (uint8_t i = 0; i < BITVECTOR_LIMIT; i++) {
        if (bitvector_get(*(bitvector*)k, i)) {
            result = result >> (BITVECTOR_LIMIT/4+i);
        }
    }
    return result;
}

void entry_free(hdict_entry e) {
    free((struct board_data*)e);
}

/* Initializes a new hash table with the given capacity */
hdict_t ht_new(size_t capacity) {
    REQUIRES(capacity > 0);
    entry_key_fn *ekfn = &entry_key;
    key_equal_fn *kefn = &key_equal;
    key_hash_fn *khfn = &key_hash;
    entry_free_fn *effn = &entry_free;
    hdict_t H = hdict_new(capacity, ekfn, kefn, khfn, effn);
    ENSURES(H != NULL);
    return H;
}

/* ht_lookup(H,B) returns
 * NULL if no struct containing the board B exists in H
 * A struct containing the board B if one exists in H.
 */
struct board_data *ht_lookup(hdict_t H, bitvector B) {
    /*@requires H != NULL; @*/
    REQUIRES(H != NULL);
    hdict_entry *e = (void*)&B;
    return hdict_lookup(H, e);
}

/* ht_insert(H,e) has no return value, because it should have as
 * a precondition that no struct currently in the hashtable contains
 * the same board as DAT->board.
 */
void ht_insert(hdict_t H, struct board_data *DAT) {
    REQUIRES(H != NULL && DAT != NULL);
    REQUIRES(ht_lookup(H, DAT->board) == NULL);
    void *insert = (void*)DAT;
    hdict_insert(H, insert);
}