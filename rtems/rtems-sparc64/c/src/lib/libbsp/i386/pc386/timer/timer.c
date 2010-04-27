/*-------------------------------------------------------------------------+
| timer.c v1.1 - PC386 BSP - 1997/08/07
+--------------------------------------------------------------------------+
| This file contains the PC386 timer package.
+--------------------------------------------------------------------------+
| NOTE: It is important that the timer start/stop overhead be determined
|       when porting or modifying this code.
+--------------------------------------------------------------------------+
| (C) Copyright 1997 -
| - NavIST Group - Real-Time Distributed Systems and Industrial Automation
|
| http://pandora.ist.utl.pt
|
| Instituto Superior Tecnico * Lisboa * PORTUGAL
+--------------------------------------------------------------------------+
| Disclaimer:
|
| This file is provided "AS IS" without warranty of any kind, either
| expressed or implied.
+--------------------------------------------------------------------------+
| This code is base on:
|   timer.c,v 1.7 1995/12/19 20:07:43 joel Exp - go32 BSP
|
| Rosimildo daSilva -ConnectTel, Inc - Fixed infinite loop in the Calibration
| routine. I've seen this problems with faster machines ( pentiums ). Sometimes
| RTEMS just hangs at startup.
|
| With the following copyright notice:
| **************************************************************************
| *  COPYRIGHT (c) 1989-1999.
| *  On-Line Applications Research Corporation (OAR).
| *
| *  The license and distribution terms for this file may be
| *  found in found in the file LICENSE in this distribution or at
| *  http://www.rtems.com/license/LICENSE.
| **************************************************************************
|
|  $Id: timer.c,v 1.28 2008/12/03 17:28:10 joel Exp $
+--------------------------------------------------------------------------*/

#include <stdlib.h>

#include <bsp.h>
#include <bsp/irq.h>
#include <libcpu/cpuModel.h>

/*-------------------------------------------------------------------------+
| Constants
+--------------------------------------------------------------------------*/
#define AVG_OVERHEAD  0              /* 0.1 microseconds to start/stop timer. */
#define LEAST_VALID   1              /* Don't trust a value lower than this.  */
#define SLOW_DOWN_IO  0x80     	/* io which does nothing */

#define TWO_MS	(uint32_t)(2000)     /* TWO_MS = 2000us (sic!) */

#define MSK_NULL_COUNT 0x40	/* bit counter available for reading */

#define CMD_READ_BACK_STATUS 0xE2   /* command read back status */
/*-------------------------------------------------------------------------+
| Global Variables
+--------------------------------------------------------------------------*/
volatile uint32_t         Ttimer_val;
bool    	          benchmark_timer_find_average_overhead = true;
volatile unsigned int     fastLoop1ms, slowLoop1ms;

void     (*benchmark_timer_initialize_function)(void) = 0;
uint32_t (*benchmark_timer_read_function)(void) = 0;
void     (*Timer_exit_function)(void) = 0;

/*-------------------------------------------------------------------------+
| External Prototypes
+--------------------------------------------------------------------------*/
extern void timerisr(void);
       /* timer (int 08h) Interrupt Service Routine (defined in 'timerisr.s') */

/*
 * forward declarations
 */

void Timer_exit(void);

/*-------------------------------------------------------------------------+
| Pentium optimized timer handling.
+--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------+
|         Function: Timer_exit
|      Description: Timer cleanup routine at RTEMS exit. NOTE: This routine is
|                   not really necessary, since there will be a reset at exit.
| Global Variables: None.
|        Arguments: None.
|          Returns: Nothing.
+--------------------------------------------------------------------------*/
void
tsc_timer_exit(void)
{
} /* tsc_timer_exit */

/*-------------------------------------------------------------------------+
|         Function: benchmark_timer_initialize
|      Description: Timer initialization routine.
| Global Variables: Ttimer_val.
|        Arguments: None.
|          Returns: Nothing.
+--------------------------------------------------------------------------*/
void
tsc_timer_initialize(void)
{
  static bool First = true;

  if (First)
  {
    First = false;

    atexit(Timer_exit); /* Try not to hose the system at exit. */
  }
  Ttimer_val = rdtsc(); /* read starting time */
} /* tsc_timer_initialize */

/*-------------------------------------------------------------------------+
|         Function: benchmark_timer_read
|      Description: Read hardware timer value.
| Global Variables: Ttimer_val, benchmark_timer_find_average_overhead.
|        Arguments: None.
|          Returns: Nothing.
+--------------------------------------------------------------------------*/
uint32_t
tsc_read_timer(void)
{
  register uint32_t         total;

  total =  (uint32_t)(rdtsc() - Ttimer_val);

  if (benchmark_timer_find_average_overhead)
    return total;
  else if (total < LEAST_VALID)
    return 0; /* below timer resolution */
  else
    return (total - AVG_OVERHEAD);
} /* tsc_read_timer */

/*-------------------------------------------------------------------------+
| Non-Pentium timer handling.
+--------------------------------------------------------------------------*/
#define US_PER_ISR   250  /* Number of micro-seconds per timer interruption */

/*-------------------------------------------------------------------------+
|         Function: Timer_exit
|      Description: Timer cleanup routine at RTEMS exit. NOTE: This routine is
|                   not really necessary, since there will be a reset at exit.
| Global Variables: None.
|        Arguments: None.
|          Returns: Nothing.
+--------------------------------------------------------------------------*/
static void
timerOff(const rtems_raw_irq_connect_data* used)
{
    /*
     * disable interrrupt at i8259 level
     */
     BSP_irq_disable_at_i8259s(used->idtIndex - BSP_IRQ_VECTOR_BASE);
     /* reset timer mode to standard (DOS) value */
     outport_byte(TIMER_MODE, TIMER_SEL0|TIMER_16BIT|TIMER_RATEGEN);
     outport_byte(TIMER_CNTR0, 0);
     outport_byte(TIMER_CNTR0, 0);
} /* Timer_exit */

static void
timerOn(const rtems_raw_irq_connect_data* used)
{
     /* load timer for US_PER_ISR microsecond period */
     outport_byte(TIMER_MODE, TIMER_SEL0|TIMER_16BIT|TIMER_RATEGEN);
     outport_byte(TIMER_CNTR0, US_TO_TICK(US_PER_ISR) >> 0 & 0xff);
     outport_byte(TIMER_CNTR0, US_TO_TICK(US_PER_ISR) >> 8 & 0xff);
    /*
     * enable interrrupt at i8259 level
     */
     BSP_irq_enable_at_i8259s(used->idtIndex - BSP_IRQ_VECTOR_BASE);
}

static int
timerIsOn(const rtems_raw_irq_connect_data *used)
{
     return BSP_irq_enabled_at_i8259s(used->idtIndex - BSP_IRQ_VECTOR_BASE);
}

static rtems_raw_irq_connect_data timer_raw_irq_data = {
  BSP_PERIODIC_TIMER + BSP_IRQ_VECTOR_BASE,
  timerisr,
  timerOn,
  timerOff,
  timerIsOn
};

/*-------------------------------------------------------------------------+
|         Function: Timer_exit
|      Description: Timer cleanup routine at RTEMS exit. NOTE: This routine is
|                   not really necessary, since there will be a reset at exit.
| Global Variables: None.
|        Arguments: None.
|          Returns: Nothing.
+--------------------------------------------------------------------------*/
void
i386_timer_exit(void)
{
  i386_delete_idt_entry (&timer_raw_irq_data);
} /* Timer_exit */

/*-------------------------------------------------------------------------+
|         Function: benchmark_timer_initialize
|      Description: Timer initialization routine.
| Global Variables: Ttimer_val.
|        Arguments: None.
|          Returns: Nothing.
+--------------------------------------------------------------------------*/
void
i386_timer_initialize(void)
{
  static bool First = true;

  if (First)
  {
    First = false;

    atexit(Timer_exit); /* Try not to hose the system at exit. */
    if (!i386_set_idt_entry (&timer_raw_irq_data)) {
      printk("raw handler connexion failed\n");
      rtems_fatal_error_occurred(1);
    }
  }
  /* wait for ISR to be called at least once */
  Ttimer_val = 0;
  while (Ttimer_val == 0)
    continue;
  Ttimer_val = 0;
} /* benchmark_timer_initialize */

/*-------------------------------------------------------------------------+
|         Function: benchmark_timer_read
|      Description: Read hardware timer value.
| Global Variables: Ttimer_val, benchmark_timer_find_average_overhead.
|        Arguments: None.
|          Returns: Nothing.
+--------------------------------------------------------------------------*/
uint32_t
i386_read_timer(void)
{
  register uint32_t         total, clicks;
  register uint8_t          lsb, msb;

  outport_byte(TIMER_MODE, TIMER_SEL0|TIMER_LATCH);
  inport_byte(TIMER_CNTR0, lsb);
  inport_byte(TIMER_CNTR0, msb);
  clicks = (msb << 8) | lsb;
  total  = (Ttimer_val * US_PER_ISR) + (US_PER_ISR - TICK_TO_US(clicks));

  if (benchmark_timer_find_average_overhead)
    return total;
  else if (total < LEAST_VALID)
    return 0; /* below timer resolution */
  else
    return (total - AVG_OVERHEAD);
}

/*
 * General timer functions using either TSC-based implementation
 * or interrupt-based implementation
 */

void
benchmark_timer_initialize(void)
{
    static bool First = true;

    if (First) {
        if (x86_has_tsc()) {
#if defined(DEBUG)
            printk("TSC: timer initialization\n");
#endif /* DEBUG */
            benchmark_timer_initialize_function = &tsc_timer_initialize;
            benchmark_timer_read_function = &tsc_read_timer;
            Timer_exit_function = &tsc_timer_exit;
        }
        else {
#if defined(DEBUG)
            printk("ISR: timer initialization\n");
#endif /* DEBUG */
            benchmark_timer_initialize_function = &i386_timer_initialize;
            benchmark_timer_read_function = &i386_read_timer;
            Timer_exit_function = &i386_timer_exit;
        }
        First = false;
    }
    (*benchmark_timer_initialize_function)();
}

uint32_t
benchmark_timer_read(void)
{
    return (*benchmark_timer_read_function)();
}

void
Timer_exit(void)
{
    return (*Timer_exit_function)();
}

/*-------------------------------------------------------------------------+
|         Function: benchmark_timer_disable_subtracting_average_overhead
|      Description: Set internal benchmark_timer_find_average_overhead flag value.
| Global Variables: benchmark_timer_find_average_overhead.
|        Arguments: find_flag - new value of the flag.
|          Returns: Nothing.
+--------------------------------------------------------------------------*/
void
benchmark_timer_disable_subtracting_average_overhead(bool find_flag)
{
  benchmark_timer_find_average_overhead = find_flag;
} /* benchmark_timer_disable_subtracting_average_overhead */

static unsigned short lastLoadedValue;

/*-------------------------------------------------------------------------+
| Description: Loads timer 0 with value passed as arguemnt.
| Returns: Nothing. Loaded value must be a number of clock bits...
+--------------------------------------------------------------------------*/
void loadTimerValue( unsigned short loadedValue )
{
  lastLoadedValue = loadedValue;
  outport_byte(TIMER_MODE, TIMER_SEL0|TIMER_16BIT|TIMER_SQWAVE);
  outport_byte(TIMER_CNTR0, loadedValue & 0xff);
  outport_byte(TIMER_CNTR0, (loadedValue >> 8) & 0xff);
}

/*-------------------------------------------------------------------------+
| Description: Reads the current value of the timer, and converts the
|			   number of ticks to micro-seconds.
| Returns: number of clock bits elapsed since last load.
+--------------------------------------------------------------------------*/
unsigned int readTimer0(void)
{
  unsigned short lsb, msb;
  unsigned char  status;
  unsigned int	 count;

  outport_byte(TIMER_MODE, (TIMER_RD_BACK | (RB_COUNT_0 & ~(RB_NOT_STATUS | RB_NOT_COUNT))));
  inport_byte(TIMER_CNTR0, status);
  inport_byte(TIMER_CNTR0, lsb);
  inport_byte(TIMER_CNTR0, msb);
  count = ( msb << 8 ) | lsb ;
  if (status & RB_OUTPUT )
  	count += lastLoadedValue;

  return (2*lastLoadedValue - count);
}

void Timer0Reset(void)
{
  loadTimerValue(0xffff);
  readTimer0();
}

void fastLoop (unsigned int loopCount)
{
  unsigned int i;
  for( i=0; i < loopCount; i++ )outport_byte( SLOW_DOWN_IO, 0 );
}

void slowLoop (unsigned int loopCount)
{
  unsigned int j;
  for (j=0; j <100 ;  j++) {
    fastLoop (loopCount);
  }
}

/*
 * #define DEBUG_CALIBRATE
 */
void
Calibrate_loop_1ms(void)
{
  unsigned int offset, offsetTmp, emptyCall, emptyCallTmp, res, i, j;
  unsigned int targetClockBits, currentClockBits;
  unsigned int slowLoopGranularity, fastLoopGranularity;
  rtems_interrupt_level  level;

#ifdef DEBUG_CALIBRATE
  printk( "Calibrate_loop_1ms is starting,  please wait ( but not too loooong. )\n" );
#endif
  targetClockBits = US_TO_TICK(1000);

  rtems_interrupt_disable(level);
  /*
   * Fill up the cache to get a correct offset
   */
  Timer0Reset();
  readTimer0();
  /*
   * Compute the minimal offset to apply due to read counter register.
   */
  offset = 0xffffffff;
  for (i=0; i <1000; i++) {
    Timer0Reset();
    offsetTmp = readTimer0();
    offset += offsetTmp;
  }
  offset = offset / 1000; /* compute average */
  /*
   * calibrate empty call
   */
  fastLoop (0);
  emptyCall = 0;
  j = 0;
  for (i=0; i <10; i++) {
    Timer0Reset();
    fastLoop (0);
    res =  readTimer0();
    /* res may be inferior to offset on fast
     * machine because we took an average for offset
     */
    if (res >  offset) {
      ++j;
      emptyCallTmp = res - offset;
      emptyCall += emptyCallTmp;
    }
  }
  if (j == 0) emptyCall = 0;
  else emptyCall = emptyCall / j; /* compute average */
  /*
   * calibrate fast loop
   */
  Timer0Reset();
  fastLoop (10000);
  res = readTimer0() - offset;
  if (res < emptyCall) {
     printk("Problem #1 in offset computation in Calibrate_loop_1ms in file libbsp/i386/pc386/timer/timer.c\n");
    while (1);
  }
  fastLoopGranularity = (res - emptyCall) / 10000;
  /*
   * calibrate slow loop
   */
  Timer0Reset();
  slowLoop(10);
  res = readTimer0();
  if (res < offset + emptyCall) {
     printk("Problem #2 in offset computation in Calibrate_loop_1ms in file libbsp/i386/pc386/timer/timer.c\n");
    while (1);
  }
  slowLoopGranularity = (res - offset - emptyCall)/ 10;

  if (slowLoopGranularity == 0) {
    printk("Problem #3 in Calibrate_loop_1ms in file libbsp/i386/pc386/timer/timer.c\n");
    while (1);
  }

  targetClockBits += offset;
#ifdef DEBUG_CALIBRATE
  printk("offset = %u, emptyCall = %u, targetClockBits = %u\n",
	 offset, emptyCall, targetClockBits);
  printk("slowLoopGranularity = %u fastLoopGranularity =  %u\n",
	 slowLoopGranularity, fastLoopGranularity);
#endif
  slowLoop1ms = (targetClockBits - emptyCall) / slowLoopGranularity;
  if (slowLoop1ms != 0) {
    fastLoop1ms = targetClockBits % slowLoopGranularity;
    if (fastLoop1ms > emptyCall) fastLoop1ms -= emptyCall;
  }
  else
    fastLoop1ms = targetClockBits - emptyCall / fastLoopGranularity;

  if (slowLoop1ms != 0) {
    /*
     * calibrate slow loop
     */

    while(1)
      {
	int previousSign = 0; /* 0 = unset, 1 = incrementing,  2 = decrementing */
	Timer0Reset();
	slowLoop(slowLoop1ms);
	currentClockBits = readTimer0();
	if (currentClockBits > targetClockBits) {
	  if ((currentClockBits - targetClockBits) < slowLoopGranularity) {
	    /* decrement loop counter anyway to be sure slowLoop(slowLoop1ms) < targetClockBits */
	    --slowLoop1ms;
	    break;
	  }
	  else {
	    --slowLoop1ms;
	    if (slowLoop1ms == 0) break;
	    if (previousSign == 0) previousSign = 2;
	    if (previousSign == 1) break;
	  }
	}
	else {
	  if ((targetClockBits - currentClockBits) < slowLoopGranularity) {
	    break;
	  }
	  else {
	    ++slowLoop1ms;
	    if (previousSign == 0) previousSign = 1;
	    if (previousSign == 2) break;
	  }
	}
      }
  }
  /*
   * calibrate fast loop
   */

  if (fastLoopGranularity != 0 ) {
    while(1) {
      int previousSign = 0; /* 0 = unset, 1 = incrementing,  2 = decrementing */
      Timer0Reset();
      if (slowLoop1ms != 0) slowLoop(slowLoop1ms);
      fastLoop(fastLoop1ms);
      currentClockBits = readTimer0();
      if (currentClockBits > targetClockBits) {
	if ((currentClockBits - targetClockBits) < fastLoopGranularity)
	  break;
	else {
	  --fastLoop1ms;
	  if (previousSign == 0) previousSign = 2;
	  if (previousSign == 1) break;
	}
      }
      else {
	if ((targetClockBits - currentClockBits) < fastLoopGranularity)
	  break;
	else {
	  ++fastLoop1ms;
	  if (previousSign == 0) previousSign = 1;
	  if (previousSign == 2) break;
	}
      }
    }
  }
#ifdef DEBUG_CALIBRATE
  printk("slowLoop1ms = %u, fastLoop1ms = %u\n", slowLoop1ms, fastLoop1ms);
#endif
  rtems_interrupt_enable(level);

}

/*-------------------------------------------------------------------------+
|         Function: Wait_X_1ms
|      Description: loop which waits at least timeToWait ms
| Global Variables: loop1ms
|        Arguments: timeToWait
|          Returns: Nothing.
+--------------------------------------------------------------------------*/
void
Wait_X_ms( unsigned int timeToWait){

  unsigned int j;

  for (j=0; j<timeToWait ; j++) {
    if (slowLoop1ms != 0) slowLoop(slowLoop1ms);
    fastLoop(fastLoop1ms);
  }

}
