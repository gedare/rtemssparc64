/*
 * Copyright (c) 2009 by
 * Ralf Corsépius, Ulm, Germany. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

#include <setjmp.h>

int
main (void)
{
  jmp_buf buf;
  setjmp (buf);
  return 0;
}
