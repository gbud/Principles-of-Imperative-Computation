#include <stdlib.h>        // Standard C library: free(), NULL...  
#include <stdbool.h>       // Standard true, false, and bool type  
#include <string.h>        // Standard C version of lib/cstring.c0
#include <limits.h>
#include <stdint.h> 
#include "lib/contracts.h" // Our contracts library  
#include "lib/xalloc.h"    // Our allocation library  
#include "lib/bitvector.h"
#include "lib/hdict.h"
#include "board-ht.h"
#include "lib/queue.h"
#include "lib/boardutil.h"

bitvector press_button(int row, int col, uint8_t height, 
                       uint8_t width, bitvector B) {
    REQUIRES(is_valid_boardsize(width, height));
    REQUIRES(is_valid_pos(row, col, width, height));
    int center = get_index(row, col, width, height);
    bitvector flip = bitvector_flip(B, center);

    //check if directions are valid flips
    if (is_valid_pos(row - 1, col, width, height)) {
        int up = get_index(row - 1, col, width, height);
        flip = bitvector_flip(flip, up);
    }
    if (is_valid_pos(row + 1, col, width, height)) {
        int down = get_index(row + 1, col, width, height);
        flip = bitvector_flip(flip, down);
    }
    if (is_valid_pos(row, col - 1, width, height)) {
        int left = get_index(row, col - 1, width, height);
        flip = bitvector_flip(flip, left);
    }
    if (is_valid_pos(row, col + 1, width, height)) {
        int right = get_index(row, col + 1, width, height);
        flip = bitvector_flip(flip, right);
    }
    return flip;
}

void free_Q_and_H(queue_t Q, hdict_t H) {
    queue_free(Q, NULL);
    hdict_free(H);
}

void insert_Q_and_H(queue_t Q, hdict_t H, bitvector B) {
    struct board_data *insert = xmalloc(sizeof(struct board_data));
    insert->board = B;
    ht_insert(H, insert);
    void *insert_generic = (void*)insert;
    enq(Q, insert_generic);
}


int main(int argc, char **argv) {  
    if (argc != 2) {  
        fprintf(stderr, "Usage: lightsout <board name>\n");  
        return 1;  
    }  
    
    char *board_filename = argv[1];
    queue_t Q = queue_new();
    hdict_t H = ht_new((size_t)420);
    bitvector newboard = bitvector_new();
    uint8_t height;
    uint8_t width;
    
    bool read_file = file_read(board_filename,&newboard,&width,&height);
    if (read_file == false) {
        fprintf(stderr, "board unreadable\n");
        free_Q_and_H(Q, H);
        return 1;
    }
    insert_Q_and_H(Q, H, newboard);

    while (!queue_empty(Q)) {  
    // Find a board that we haven't looked at yet from the queue  
        struct board_data *B = (struct board_data*)deq(Q);  
        newboard = B->board;

        // Consider all the moves  
        for (int row = 0; row < height; row++) {  
            for (int col = 0; col < width; col++) {  
                newboard = press_button(row, col, height, width, B->board); 
                if (bitvector_equal(newboard, bitvector_new())) {
                    fprintf(stderr, "board has solution!!\n");
                    free_Q_and_H(Q, H);
                    return 0; 
                }  
                if (ht_lookup(H, newboard) == NULL) {
                    insert_Q_and_H(Q, H, newboard);
                }  
            }  
        }
    }
    fprintf(stderr, "board has no solution\n");
    free_Q_and_H(Q, H); 
    return 1;    
}