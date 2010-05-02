/*
 *  Install trap handlers for handling software interrupts.
 *  This file is deprecated, as the trap handlers are needed before this 
 *  function is called. We still use this as for debugging purposes.
 *
 *  Copyright 2010 Gedare Bloom.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <rtems.h>
#include <rtems/asm.h>
#include <rtems/sparc64/traps.h>
#include <rtems/score/sparc.h>
#include <rtems/bspIo.h>

extern int syscall(int tstate); /* 
				 * does the decl actually matter?
				 * other than to compile?
				 */

void sparc64_install_isr_entries( void )
{
	uint64_t tba;

	uint32_t isr_cookie = 0;

	sparc64_get_tba(tba);

	proc_ptr ignored;

/*
	_CPU_ISR_install_raw_handler(0x100,syscall,&ignored);
*/
#ifdef GICADEBUG
	printk("Trap base address = %lx\n\r", tba);
	printk("ISR cookie = %x\n\r", isr_cookie);
#endif
	isr_cookie = sparc_disable_interrupts();
#ifdef GICADEBUG
	printk("ISR cookie = %x\n\r", isr_cookie);
#endif

	printk("START_TOKEN\n\r");
	return;
}
