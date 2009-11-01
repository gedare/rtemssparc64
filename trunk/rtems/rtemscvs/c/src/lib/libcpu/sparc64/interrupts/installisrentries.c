/* GAB */

#include <rtems.h>
#include <rtems/asm.h>
#include <rtems/sparc64/traps.h>
#include <rtems/score/sparc.h>

void sparc64_install_isr_entries( void )
{
	uint64_t tba;

	sparc64_get_tba(tba);


	memcpy( (void*)(tba+TT_SPILL_0_NORMAL), 
			win_spill_code, 
			WIN_SPILL_CODE_SIZE );




}
