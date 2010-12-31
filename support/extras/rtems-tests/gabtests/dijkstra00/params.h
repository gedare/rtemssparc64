/* params.h
 *
 * This is a generated file.
 * Use the gen-headers.lua script to create this file.
 */

/* This file was generated with the following parameters: 
-T 1,0.037,0 -T 3,0.003,0 -T 5,0.041,0 -T 6,0.007,0 -T 8,0.060,0 -T 9,0.053,0 -T 9,0.019,0 -T 10,0.031,0 -T 16,0.057,0 -T 19,0.017,0 -T 21,0.029,0 -T 28,0.044,0 -T 32,0.016,0 -T 33,0.024,0 -T 35,0.034,0 -T 44,0.043,0 -T 46,0.031,0 -T 47,0.018,0 -T 47,0.026,0 -T 50,0.010,0 
*/

#ifndef __PARAMS_H_
#define __PARAMS_H_

#define  HP_LENGTH                  (-1)
#define  MAX_PERIOD                 (47)
#define  NUM_PERIODIC_TASKS         (9)
#define  NUM_APERIODIC_TASKS        (1)
#define  NUM_TASKS                  ( NUM_PERIODIC_TASKS + NUM_APERIODIC_TASKS )
uint32_t  periodic_tasks            = NUM_PERIODIC_TASKS;

rtems_task_priority Priorities[1+NUM_TASKS]= { 0, 1, 3, 5, 6, 8, 9, 9, 10, 16, 19};

uint32_t  Periods[1+NUM_PERIODIC_TASKS]    = { 0, 1, 3, 5, 6, 8, 9, 9, 10, 16};

uint32_t  Execution_us[1+NUM_TASKS]        = {
                                             0*CONFIGURE_MICROSECONDS_PER_TICK,
                                             1*0.037*CONFIGURE_MICROSECONDS_PER_TICK,
                                             3*0.003*CONFIGURE_MICROSECONDS_PER_TICK,
                                             5*0.041*CONFIGURE_MICROSECONDS_PER_TICK,
                                             6*0.007*CONFIGURE_MICROSECONDS_PER_TICK,
                                             8*0.06*CONFIGURE_MICROSECONDS_PER_TICK,
                                             9*0.053*CONFIGURE_MICROSECONDS_PER_TICK,
                                             9*0.019*CONFIGURE_MICROSECONDS_PER_TICK,
                                             10*0.031*CONFIGURE_MICROSECONDS_PER_TICK,
                                             16*0.057*CONFIGURE_MICROSECONDS_PER_TICK,
                                             19*0.017*CONFIGURE_MICROSECONDS_PER_TICK                                             };

uint32_t  Phases[1+NUM_TASKS]           = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#define build_task_name() do { \
Task_name[ 1 ] =  rtems_build_name( 'P', 'T', '0', '1' );\
Task_name[ 2 ] =  rtems_build_name( 'P', 'T', '0', '2' );\
Task_name[ 3 ] =  rtems_build_name( 'P', 'T', '0', '3' );\
Task_name[ 4 ] =  rtems_build_name( 'P', 'T', '0', '4' );\
Task_name[ 5 ] =  rtems_build_name( 'P', 'T', '0', '5' );\
Task_name[ 6 ] =  rtems_build_name( 'P', 'T', '0', '6' );\
Task_name[ 7 ] =  rtems_build_name( 'P', 'T', '0', '7' );\
Task_name[ 8 ] =  rtems_build_name( 'P', 'T', '0', '8' );\
Task_name[ 9 ] =  rtems_build_name( 'P', 'T', '0', '9' );\
Task_name[ 10 ] =  rtems_build_name( 'D', 'T', '1', '0' );\
} while(0)

#endif
