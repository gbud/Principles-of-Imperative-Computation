// gbudhija strbuf.c

#include <stdlib.h>        // Standard C library: free(), NULL...  
#include <stdbool.h>       // Standard true, false, and bool type  
#include <string.h>        // Standard C version of lib/cstring.c0
#include <limits.h>
#include "lib/contracts.h" // Our contracts library  
#include "lib/xalloc.h"    // Our allocation library  
#include "strbuf.h"        // The string buffer interface

// struct strbuf_header {
//   int limit;
//   int len;
//   char* buf;
// };
// typedef struct strbuf_header strbuf;

// bool is_strbuf_expected_length(char* sb, int limit) {  
//   //@assert \length(sb) == limit;  
//   return true;  
// } 

bool strbuf_terminated(char* buf, size_t len)
{
  if (buf[len] != '\0') return false;
  for (size_t i = 0; i < len; i++) {
    if (buf[i] == '\0') return false;
  }
  return true;
}

bool is_strbuf(strbuf* sb)
{
  return sb != NULL
      && sb->limit > 0
      // && is_strbuf_expec#include <stdlib.h>        // Standard C library: free(), NULL...  
#include <stdbool.h>       // Standard true, false, and bool type  
#include <string.h>        // Standard C version of lib/cstring.c0
#include <limits.h>
#include "lib/contracts.h" // Our contracts library  
#include "lib/xalloc.h"    // Our allocation library  
#include "strbuf.h"        // The string buffer interface


ted_length(sb->buf, sb->limit)
      && sb->limit > sb->len
      && strbuf_terminated(sb->buf, sb->len);
      // && is_cstring(sb->buf, 0, sb->len);
      // && string_terminated(sb->buf, sb->len);
}

strbuf *strbuf_new(size_t init_limit) // *Strictly* positive  
//@requires init_limit > 0;
//@ensures is_strbuf(\result);
{
  strbuf* new = xmalloc(sizeof(strbuf));
  new->limit = init_limit;
  new->buf = xcalloc(init_limit, sizeof(char));
  new->buf[0] = '\0';
  ENSURES(is_strbuf(new));
  return new;
}

char* strbuf_str(strbuf* sb)
//@requires is_strbuf(sb);
{
  REQUIRES(is_strbuf(sb));
  char* new = strncpy(xcalloc(sb->len + 1, sizeof(char)), sb->buf, sb->len);
  ENSURES(new[sb->len] == '\0');
  ENSURES(strncmp(sb->buf, new, sb->len) == 0);
  return new;
}

void resize_strbuf(strbuf* sb, size_t len)
//@requires (sb->len + len) <= int_max() / 2;
{
  REQUIRES((sb->len + len) <= (INT_MAX /2));
  size_t new_limit = (sb->len + len) * 2;
  assert(new_limit > sb->limit);

  char* new_buf = xcalloc(new_limit, sizeof(char));
  sb->limit = new_limit;
  for (size_t i = 0; i <= sb->len; i++) 
  //@loop_invariant 0 <= i && i <= sb->limit;
  {
    ASSERT(i <= sb->limit);
    new_buf[i] = sb->buf[i];
  }
  free(sb->buf);
  sb->buf = new_buf;
  ENSURES(is_strbuf(sb));
}

void strbuf_add(strbuf *sb, char *str, size_t len)
//@requires is_strbuf(sb);
//@requires strlen(str, 0) == str_len;
//@ensures is_strbuf(sb);
{
  REQUIRES(is_strbuf(sb));
  REQUIRES(strlen(str) == len);
  if (len + sb->len >= sb->limit) {
    resize_strbuf(sb, len);
  }
  for (size_t i = 0; i < len; i++) {
    sb->buf[sb->len + i] = str[i];
  }
  sb->len = sb->len + len;
  sb->buf[sb->len] = '\0';
  ENSURES(is_strbuf(sb));
}

void strbuf_addstr(strbuf* sb, char* str)
//@requires is_strbuf(sb);
//@ensures is_strbuf(sb);
{
  REQUIRES(is_strbuf(sb));
  strbuf_add(sb, str, strlen(str));
  ENSURES(is_strbuf(sb));
}

char *strbuf_free(strbuf *sb)
{
  REQUIRES(is_strbuf(sb));
  char* return_buf = strbuf_str(sb);
  free(sb->buf);
  free(sb);
  return return_buf;
}