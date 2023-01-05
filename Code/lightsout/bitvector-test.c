/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2022                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "lib/contracts.h"
#include "lib/bitvector.h"

// You are welcome to define helper functions for your tests

int main() {
  // Create a few bitvectors
  bitvector test1 = bitvector_new();
  assert(bitvector_get(test1, 0) == false);
  assert(bitvector_get(test1, 24) == false);
  assert(bitvector_get(test1, 47) == false);
  // printf("test1 before flip= %d \n", test1);
  bitvector test2 = 0x000000000400;
  assert(bitvector_get(test2, 36) == false);
  assert(bitvector_get(test2, 37) == true);
  assert(bitvector_get(test2, 38) == false);
  // printf("test2 before flip= %d \n", test2);
  // // printf("bitvector = %d \n", test1);
  // printf("BITVECTOR_LIMIT = %d \n", BITVECTOR_LIMIT);
  // printf("sizeof bitvector = %ld \n", sizeof(bitvector));
  // // printf("sizeof test1= %ld \n", sizeof(test1));
  // printf("sizeof unit8_t= %ld \n", sizeof(uint8_t));
  // printf("sizeof unit16_t= %ld \n", sizeof(uint16_t));
  // printf("sizeof unit32_t= %ld \n", sizeof(uint32_t));
  // printf("sizeof unit64_t= %ld \n", sizeof(uint64_t));
  // assert(bitvector_get(test1, 0) == false);
  // assert(bitvector_get(test1, BITVECTOR_LIMIT-1) == false);
  // assert(bitvector_get(test1, BITVECTOR_LIMIT/2) == false);
  bitvector test3 = bitvector_flip(test1, 37);
  // printf("test3 after flip= %d \n", test3);
  assert(bitvector_get(test3, 37) == true);
  assert(bitvector_equal(test2, test3));

  // Using them, test the functions you wrote in file bitvector.c



  printf("All tests passed!\n");
  return 0;
}
