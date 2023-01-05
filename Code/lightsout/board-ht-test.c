/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2022                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "lib/contracts.h"
#include "lib/xalloc.h"
#include "lib/bitvector.h"
#include "board-ht.h"

// You are welcome to define helper functions for your tests

int main() {
  hdict_t ht1 = ht_new(8);
  bitvector bv1 = bitvector_new();
  bitvector bv2 = bitvector_flip(bv1, 5);
  bitvector bv3 = 0x04;
  bitvector bv4 = 0x40;

  struct board_data *test1 = xmalloc(sizeof(struct board_data));
  test1->board = bv2;
  ht_insert(ht1, test1);
  assert(ht_lookup(ht1, bv2) != NULL);
  assert(ht_lookup(ht1, bv3) != NULL);

  struct board_data *test2 = xmalloc(sizeof(struct board_data));
  test2->board = bv4;
  ht_insert(ht1, test2);
  assert(ht_lookup(ht1, bv4) != NULL);

  hdict_t *ht1_free = (hdict_t*)ht1;
  free(ht1_free);
  
  // Using them, test the functions you wrote in file board-ht.c



  printf("All tests passed!\n");
  return 0;
}
