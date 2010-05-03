/**
 * @file rtems/traps.h
 *
 * This include file defines macros and code for the sparc64 trap table.
 * This file is not currently used.
 *
 * $Id$
 */


#ifndef _RTEMS_TRAPS_H
#define _RTEMS_TRAPS_H



#define TT_CLEAN_WINDOW                 0x24
#define TT_SPILL_0_NORMAL               0x80    /* base spill entry */
#define TT_FILL_0_NORMAL                0xc0    /* base fill entry */
#define TT_SOFT_TRAP			0x100	/* Trap Instruction */


#define REGWIN_HANDLER_SIZE             128

#define CLEAN_WINDOW_HANDLER_SIZE       REGWIN_HANDLER_SIZE
#define SPILL_HANDLER_SIZE              REGWIN_HANDLER_SIZE
#define FILL_HANDLER_SIZE               REGWIN_HANDLER_SIZE

/* These need to be consistent with the size of the functions in reg_win.S */
/* these can be at most (4 * 32) */
#define WIN_SPILL_CODE_SIZE (4 * 18) 
#define WIN_FILL_CODE_SIZE (4 * 18)
#define WIN_CLEAN_CODE_SIZE (4 * 20)

/* register window management functions are in libcpu/reg_win */
extern void win_spill_code(void);
extern void win_fill_code(void);
extern void win_clean_code(void);


#endif
