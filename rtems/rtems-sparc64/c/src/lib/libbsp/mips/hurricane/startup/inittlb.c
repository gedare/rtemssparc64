/*
 * $Id: inittlb.c,v 1.2 2006/03/17 10:34:12 ralf Exp $
 */

#include <rtems/mips/idtcpu.h>

extern void resettlb( int i );

void init_tlb(void)
{
  int i;

        for (i = 0; i < N_TLB_ENTRIES; i++ )
                resettlb(i);
}
