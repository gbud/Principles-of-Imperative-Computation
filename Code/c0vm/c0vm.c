/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2022                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include "lib/xalloc.h"
#include "lib/stack.h"
#include "lib/contracts.h"
#include "lib/c0v_stack.h"
#include "lib/c0vm.h"
#include "lib/c0vm_c0ffi.h"
#include "lib/c0vm_abort.h"

/* call stack frames */
typedef struct frame_info frame;
struct frame_info {
  c0v_stack_t S;   /* Operand stack of C0 values */
  ubyte *P;        /* Function body */
  size_t pc;       /* Program counter */
  c0_value *V;     /* The local variables */
};

int pop_int(c0v_stack_t S) {
  return val2int(c0v_pop(S));
}

void push_int(c0v_stack_t S, int i) {
  c0v_push(S, int2val(i));
}

void *pop_ptr(c0v_stack_t S) {
  return val2ptr(c0v_pop(S));
}

void push_ptr(c0v_stack_t S, void *i) {
  c0v_push(S, ptr2val(i));
}

int execute(struct bc0_file *bc0) {
  REQUIRES(bc0 != NULL);

  /* Variables */

  /* Operand stack of C0 values */
  c0v_stack_t S = c0v_stack_new();
  /* Array of bytes that make up the current function */
  ubyte *P = bc0->function_pool[0].code;
  /* Current location within the current byte array P */
  size_t pc = 0;
  /* Local variables (you won't need this till Task 2) */     
  c0_value *V = xcalloc(sizeof(c0_value), bc0->function_pool[0].num_vars);
  // silences compilation errors about V being currently unused   
  (void) V;         

  /* The call stack, a generic stack that should contain pointers to frames */
  /* You won't need this until you implement functions. */
  gstack_t callStack = stack_new();
  // silences compilation errors about callStack being currently unused
  (void) callStack; 

  while (true) {

#ifdef DEBUG
    /* You can add extra debugging information here */
    fprintf(stderr, "Opcode %x -- Stack size: %zu -- PC: %zu\n",
            P[pc], c0v_stack_size(S), pc);
#endif

    switch (P[pc]) {

    /* Additional stack operation: */

    case POP: {
      pc++;
      c0v_pop(S);
      break;
    }

    case DUP: {
      pc++;
      c0_value v = c0v_pop(S);
      c0v_push(S,v);
      c0v_push(S,v);
      break;
    }

    case SWAP: {
      pc++;
      c0_value a = c0v_pop(S);
      c0_value b = c0v_pop(S);
      c0v_push(S,a);
      c0v_push(S,b);
      break;
    }


    /* Returning from a function.
     * This currently has a memory leak! You will need to make a slight
     * change for the initial tasks to avoid leaking memory.  You will
     * need to revise it further when you write INVOKESTATIC. */
 
    case RETURN: {
      int retval = val2int(c0v_pop(S));
      assert(c0v_stack_empty(S));
      // Another way to print only in DEBUG mode
      IF_DEBUG(fprintf(stderr, "Returning %d from execute()\n", retval));
      // Free everything before returning from the execute function!
      c0v_stack_free(S);
      free(V);
      if (!stack_empty(callStack)) {
        frame *F = (frame*)pop(callStack);
        S  = F->S;
        P  = F->P;
        pc = F->pc;
        V  = F->V;
        free(F);
        push_int(S, retval);
        break;
      } else {
        stack_free(callStack, NULL);
        return retval;
      }        
    }


    /* Arithmetic and Logical operations */

    case IADD: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      push_int(S,(x + y));
      break;
    }

    case ISUB: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      push_int(S,(x - y));
      break;
    }

    case IMUL: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      push_int(S,(x * y));
      break;
    }

    case IDIV: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if (y == 0) c0_arith_error("Division by 0 Error");
      else if (x == INT_MIN && y == -1) {
        c0_arith_error("Overflow Error");
      }
      else push_int(S,(x / y));
      break;
    }

    case IREM: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if (y == 0) c0_arith_error("Division by 0 Error");
      else if (x == INT_MIN && y == -1) {
        c0_arith_error("Overflow Error");
      }
      else push_int(S,(x % y));
      break;
    }

    case IAND: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      push_int(S,(x & y));
      break;
    }

    case IOR: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      push_int(S,(x | y));
      break;
    }

    case IXOR: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      push_int(S,(x ^ y));
      break;
    }

    case ISHR: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if (y < 0 || y >= 32) c0_arith_error("Invalid Shift");
      else push_int(S,(x >> y));
      break;
    }

    case ISHL: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if (y < 0 || y >= 32) c0_arith_error("Invalid Shift");
      else push_int(S,(x << y));
      break;
    }

    /* Pushing constants */

    case BIPUSH: {
      pc++;
      int8_t x = (int8_t)P[pc];
      push_int(S, ((int32_t)x));
      pc++;
      break;
    }

    case ILDC: {
      pc++;
      uint32_t c1 = (uint32_t)(P[pc]) << 8;
      pc++;
      uint32_t c2 = (uint32_t)(P[pc]);
      push_int(S, bc0->int_pool[(c1|c2)]);
      pc++;
      break;
    }

    case ALDC: {
      pc++;
      uint32_t c1 = (uint32_t)(P[pc]) << 8;
      pc++;
      uint32_t c2 = (uint32_t)(P[pc]);
      push_ptr(S, &bc0->string_pool[(c1|c2)]);
      pc++;
      break;
    }

    case ACONST_NULL: {
      pc++;
      push_ptr(S, NULL);
      break;
    }


    /* Operations on local variables */

    case VLOAD: {
      pc++;
      c0_value v = V[P[pc]];
      c0v_push(S, v);
      pc++;
      break;
    }

    case VSTORE: {
      pc++;
      V[P[pc]] = c0v_pop(S);
      pc++;
      break;
    }


    /* Assertions and errors */

    case ATHROW: {
      pc++;
      c0_user_error(pop_ptr(S));
      break;
    }

    case ASSERT: {
      pc++;
      void* error = pop_ptr(S);
      int32_t x = pop_int(S);
      if (x == 0) c0_assertion_failure(error);
      break;
    }


    /* Control flow operations */

    case NOP: {
      pc++;
      break;
    }

    case IF_CMPEQ: {
      pc++;
      c0_value v2 = c0v_pop(S);
      c0_value v1 = c0v_pop(S);
      if (val_equal(v1, v2)) {
        uint16_t o1 = (uint16_t)(P[pc]) << 8;
        pc++;
        uint16_t o2 = (uint16_t)(P[pc]);
        int16_t i = (int16_t)(o1|o2);
        pc = pc + i - 2;
        break;
      } else {
        pc = pc + 2;
        break;
      }
    }

    case IF_CMPNE: {
      pc++;
      c0_value v2 = c0v_pop(S);
      c0_value v1 = c0v_pop(S);
      if (!val_equal(v1, v2)) {
        uint16_t o1 = (uint16_t)(P[pc]) << 8;
        pc++;
        uint16_t o2 = (uint16_t)(P[pc]);
        int16_t i = (int16_t)(o1|o2);
        pc = pc + i - 2;
        break;
      } else {
        pc = pc + 2;
        break;
      }
    }

    case IF_ICMPLT: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if (x < y) {
        uint16_t o1 = (uint16_t)(P[pc]) << 8;
        pc++;
        uint16_t o2 = (uint16_t)(P[pc]);
        int16_t i = (int16_t)(o1|o2);
        pc = pc + i - 2;
        break;
      } else {
        pc = pc + 2;
        break;
      }
    }

    case IF_ICMPGE: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if (x >= y) {
        uint16_t o1 = (uint16_t)(P[pc]) << 8;
        pc++;
        uint16_t o2 = (uint16_t)(P[pc]);
        int16_t i = (int16_t)(o1|o2);
        pc = pc + i - 2;
        break;
      } else {
        pc = pc + 2;
        break;
      }
    }

    case IF_ICMPGT: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if (x > y) {
        uint16_t o1 = (uint16_t)(P[pc]) << 8;
        pc++;
        uint16_t o2 = (uint16_t)(P[pc]);
        int16_t i = (int16_t)(o1|o2);
        pc = pc + i - 2;
        break;
      } else {
        pc = pc + 2;
        break;
      }
    }

    case IF_ICMPLE: {
      pc++;
      int32_t y = pop_int(S);
      int32_t x = pop_int(S);
      if (x <= y) {
        uint16_t o1 = (uint16_t)(P[pc]) << 8;
        pc++;
        uint16_t o2 = (uint16_t)(P[pc]);
        int16_t i = (int16_t)(o1|o2);
        pc = pc + i - 2;
        break;
      } else {
        pc = pc + 2;
        break;
      }
    }

    case GOTO: {
      pc++;
      uint16_t o1 = (uint16_t)(P[pc]) << 8;
      pc++;
      uint16_t o2 = (uint16_t)(P[pc]);
      int16_t i = (int16_t)(o1|o2);
      pc = pc + i - 2;
      break;
    }

////////////////////////////////////////////////////////////////////////////////
    /* Function call operations: */

    case INVOKESTATIC: {
      pc++;
      uint16_t c1 = (uint16_t)(P[pc]) << 8;
      pc++;
      uint16_t c2 = (uint16_t)(P[pc]);
      pc++;
      uint16_t args = bc0->function_pool[(c1|c2)].num_args;

      frame *calling_function = xmalloc(sizeof(frame));
      calling_function->S  = S;
      calling_function->P  = P;
      calling_function->pc = pc;
      calling_function->V  = V;
      void *calling_function_generic = (void*)calling_function;
      push(callStack, calling_function_generic);

      S = c0v_stack_new();
      P = bc0->function_pool[(c1|c2)].code;
      pc = 0;
      V = xcalloc(sizeof(c0_value), (bc0->function_pool[(c1|c2)].num_vars));
      for (int i = args - 1; i >= 0; i--) {
        V[i] = c0v_pop(calling_function->S);
      }
      break;
    }

    case INVOKENATIVE: {
      pc++;
      uint16_t c1 = (uint16_t)(P[pc]) << 8;
      pc++;
      uint16_t c2 = (uint16_t)(P[pc]);
      pc++;
      uint16_t args = bc0->native_pool[(c1|c2)].num_args;

      c0_value native[args];
      for (int i = args - 1; i >= 0; i--) {
        native[i] = c0v_pop(S);
      }

      uint16_t fti = bc0->native_pool[(c1|c2)].function_table_index;
      c0_value result = native_function_table[fti](native);
      c0v_push(S, result);
      break;
    }


    /* Memory allocation and access operations: */

    case NEW: {
      pc++;
      void *new = xcalloc(sizeof(char), P[pc]);
      push_ptr(S, new);
      pc++;
      break;
    }

    case IMLOAD: {
      int *a = (int*)pop_ptr(S);
      if (a == NULL) c0_memory_error("Segmentation Fault");
      else push_int(S, *a);
      pc++;
      break;
    }

    case IMSTORE: {
      int x = pop_int(S);
      int *a = (int*)pop_ptr(S);
      if (a == NULL) c0_memory_error("Segmentation Fault");
      else *a = x;
      pc++;
      break;
    }

    case AMLOAD: {
      void **a = pop_ptr(S);
      if (a == NULL) c0_memory_error("Segmentation Fault");
      else {
        void *b = *a;
        push_ptr(S, b);
      }
      pc++;
      break;
    }

    case AMSTORE: {
      void *b = pop_ptr(S);
      void **a = pop_ptr(S);
      if (a == NULL || b == NULL ) c0_memory_error("Segmentation Fault");
      else {
        *a = b;
      }
      pc++;
      break;
    }

    case CMLOAD: {
      char *a = (char*)pop_ptr(S);
      if (a == NULL) c0_memory_error("Segmentation Fault");
      else {
        char x = (char)(int32_t)*a;
        push_int(S, x);
      }
      pc++;
      break;
    }

    case CMSTORE: {
      char x = (char)pop_int(S);
      char *a = (char*)pop_ptr(S);
      if (a == NULL) c0_memory_error("Segmentation Fault");
      else {
        *a = x & 0x7F;
      }
      pc++;
      break;
    }

    case AADDF: {
      pc++;
      char *a = (char*)pop_ptr(S);
      if (a == NULL) c0_memory_error("Segmentation Fault");
      else {
        push_ptr(S, (a+P[pc]));
      }
      pc++;
      break;
    }


    /* Array operations: */

    case NEWARRAY: {
      pc++;
      int n = pop_int(S);
      c0_array *a = xmalloc(sizeof(c0_array));
      a->count = n;
      a->elt_size = P[pc];
      a->elems = xmalloc(a->count * a->elt_size);
      void *array = (void*)a;
      push_ptr(S, array);
      pc++;
      break;
    }

    case ARRAYLENGTH: {
      c0_array *a = (c0_array*)pop_ptr(S);
      if (a == NULL) c0_memory_error("Segmentation Fault");
      else {
        push_int(S, a->count);
      }
      pc++;
      break;
    }

    case AADDS: {
      int i = pop_int(S);
      c0_array *a = (c0_array*)pop_ptr(S);
      if (a == NULL) c0_memory_error("Segmentation Fault");
      else if (i < 0 || i >= a->count) c0_memory_error("Segmentation Fault");
      else {
        int s = a->elt_size;
        char *result = (char*)a->elems + s * i;
        push_ptr(S, result);
      }
      pc++;
      break;
    }


    /* BONUS -- C1 operations */

    case CHECKTAG:

    case HASTAG:

    case ADDTAG:

    case ADDROF_STATIC:

    case ADDROF_NATIVE:

    case INVOKEDYNAMIC:

    default:
      fprintf(stderr, "invalid opcode: 0x%02x\n", P[pc]);
      abort();
    }
  }

  /* cannot get here from infinite loop */
  assert(false);
}
