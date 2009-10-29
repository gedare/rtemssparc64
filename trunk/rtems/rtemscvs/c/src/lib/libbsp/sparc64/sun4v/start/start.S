
# This code needs header before distribution~

#include <rtems/asm.h>

# GAB: check these numbers
#define STACK_ALIGNMENT			16
! sp and fp are biased 2047 bytes toward lower addresses
#define STACK_BIAS			2047	
#define STACK_WINDOW_SAVE_AREA_SIZE	(16*8)


#define PSTATE_IE_BIT   2
#define PSTATE_PRIV_BIT	4
#define PSTATE_AM_BIT   8


#define INITIAL_STACK_SIZE 	8192

#define NWINDOWS		8

.register %g2, #scratch
.register %g3, #scratch
.register %g6, #scratch
.register %g7, #scratch

.section BOOTSTRAP, "ax"

PUBLIC(start)
.global start
SYM(start):
start:
	b 1f
	nop

/*
 * This header forces SILO to load the image at 0x4000.
 * More precisely, SILO will think this is an old version of Linux.
 */
	.ascii "HdrS"
	.word 0
	.half 0
	.half 0
	.half 0
	.half 0
.global silo_ramdisk_image
silo_ramdisk_image:
	.word 0
.global silo_ramdisk_size
silo_ramdisk_size:
	.word 0

.align 8
1:	
	/*
	 * Disable interrupts and disable address masking.
	 */
	wrpr %g0, PSTATE_PRIV_BIT, %pstate
	
	wrpr %g0, NWINDOWS - 2, %cansave	! set maximum saveable windows
	wrpr %g0, 0, %canrestore		! get rid of windows we will never need again
	wrpr %g0, 0, %otherwin			! make sure the window state is consistent
	wrpr %g0, NWINDOWS - 1, %cleanwin	! prevent needless clean_window traps for kernel

	set initial_stack_top, %sp
	add %sp, -STACK_BIAS, %sp

!	set ofw_cif, %l0
 
!	call ofw_init		! initialize OpenFirmware
!	stx %o4, [%l0]

/* Before entering C code, we need to set up trap table */
!	sethi %hi(trap_table), %g1
!	wrpr %g1, $lo(trap_table),%tba

/* Do we need to do anything with the MMU? */

/* Don't need to copy initialized data to RAM--link puts all in RAM already */

/* GAB: enter C code */
	call SYM(boot_card)
	nop

.align STACK_ALIGNMENT
initial_stack:
	.space INITIAL_STACK_SIZE
initial_stack_top:
	.space STACK_WINDOW_SAVE_AREA_SIZE

.global _fini
_fini:
	nop
	nop
	
.global _init
_init:
	nop
	nop
