/* GAB */

#include <rtems.h>
#include <rtems/asm.h>
#include <rtems/sparc64/traps.h>
#include <rtems/score/sparc.h>
#include <rtems/bspIo.h>

void sparc64_install_isr_entries( void )
{
	uint64_t tba;
	sparc64_get_tba(tba);

	printf("sizeof(float)=%d\n",sizeof(float));

  /* GAB: Apparently, we can use the default trap handlers */
/*	
  memcpy( (void*)(tba+TT_SPILL_0_NORMAL), 
			win_spill_code, 
			WIN_SPILL_CODE_SIZE );

  memcpy( (void*)(tba+TT_FILL_0_NORMAL),
      win_fill_code,
      WIN_FILL_CODE_SIZE );

  memcpy( (void*)(tba+TT_CLEAN_WINDOW),
      win_clean_code,
      WIN_CLEAN_CODE_SIZE );
*/

}
