/* GAB */

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

	printk("Trap base address = %lx\n\r", tba);
/*
	_CPU_ISR_install_raw_handler(0x100,syscall,&ignored);
*/

	printk("ISR cookie = %x\n\r", isr_cookie);
	isr_cookie = sparc_disable_interrupts();
	printk("ISR cookie = %x\n\r", isr_cookie);

	printk("\n\r");
	return;
}
