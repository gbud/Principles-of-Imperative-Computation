/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2022                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
/* Allocation utilities
 * Implement versions of malloc and free that abort
 * when allocation fails instead of returning NULL.
 *
 * 15-122 Principles of Imperative Computation, Fall 2010
 * Frank Pfenning
 */
#include <stdio.h>

#ifndef XALLOC_H
#define XALLOC_H

/* xcalloc(nobj, size) returns a non-NULL pointer to
 * array of nobj objects, each of size size and
 * exits if the allocation fails.  Like calloc, the
 * array is initialized with zeroes.
 */
void* xcalloc(size_t nobj, size_t size);

/* xmalloc(size) returns a non-NULL pointer to
 * an object of size size and exits if the allocation
 * fails.  Like malloc, no initialization is guaranteed.
 */
void* xmalloc(size_t size);

#endif
