/*
 *  Copyright (c) 2010 Gedare Bloom.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <rtems.h>
#include <tmacros.h>
#include <rtems/rtems/clock.h>
#include <bsp.h>
#include <boot/ofw.h>

#define US_TO_SPIN (3)

#define CPU_FREQ (5000000)

static unsigned int get_Frequency(void)
{
	phandle root = ofw_find_device("/");
	unsigned int freq;
	if (ofw_get_property(root, "clock-frequency", &freq, sizeof(freq)) <= 0) {
		printk("Unable to determine frequency, default: 0x%x\n",CPU_FREQ);
		return CPU_FREQ;
	}

	return freq;
}

#define delay( microseconds, inst_per_us ) \
  do { \
    register uint32_t         _delay=(microseconds); \
    register uint32_t         _inst=(inst_per_us); \
    register int              _tmp=0; \
    asm volatile( "sub     %0, 4, %0; \
      0: \
        mov     %0, %1; \
      1: \
        brgz,a  %1, 1b; \
        sub     %1, 2, %1; \
        dec     %2; \
        brnz    %2, 0b; \
        nop " \
      : "=r" (_inst), "=r" (_tmp), "=r" (_delay) \
      : "0" (_inst), "1" (_tmp), "2" (_delay) ); \
  } while (0) 

rtems_task Init(
  rtems_task_argument argument
)
{
  uint32_t freq, inst; 

  uint64_t  tick_start, tick_stop;

  puts( "\n\n*** TEST SPINNING ***" );
  freq = get_Frequency( );
  inst = freq / 1000000;

  printf("frequency: %u\n", freq);

  if ( inst == 0 ) {
    puts( "error: CPU frequency is too low" );
    rtems_test_exit(0);
  }

  printf("Time to spin: %d us\n", US_TO_SPIN);

  sparc64_read_tick( tick_start );
  delay( US_TO_SPIN, inst );
  sparc64_read_tick( tick_stop );

  printf("#inst spun: %lu \n", tick_stop - tick_start);
  printf("Time spun: %d us\n", 
        (tick_stop - tick_start) /* instructions */ / 
        (freq / 1000000) /* instructions per us */ 
      );

  printf("\n\n");
  puts( "*** END OF TEST TICKS ***" );

  rtems_test_exit(0);
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MICROSECONDS_PER_TICK 10000

#define CONFIGURE_MAXIMUM_TASKS             1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE


#define CONFIGURE_INIT

#include <rtems/confdefs.h>
/* end of file */
