/**
 * @file rtems/termiostypes.h
 */

/*
 *  RTEMS termios device support internal data structures
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: termiostypes.h,v 1.13 2009/09/30 02:51:32 ralf Exp $
 */

#ifndef  __TERMIOSTYPES_H
#define  __TERMIOSTYPES_H

#include <rtems.h>
#include <rtems/libio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Wakeup callback data structure
 */
struct ttywakeup {
  void      (*sw_pfn)(struct termios *tty, void *arg);
  void      *sw_arg;
};

/*
 * Variables associated with the character buffer
 */
struct rtems_termios_rawbuf {
  char *theBuf;
  volatile unsigned int  Head;
  volatile unsigned int  Tail;
  volatile unsigned int  Size;
  rtems_id    Semaphore;
};
/*
 * Variables associated with each termios instance.
 * One structure for each hardware I/O device.
 */
struct rtems_termios_tty {
  /*
   * Linked-list of active TERMIOS devices
   */
  struct rtems_termios_tty  *forw;
  struct rtems_termios_tty  *back;

  /*
   * How many times has this device been opened
   */
  int    refcount;

  /*
   * This device
   */
  rtems_device_major_number  major;
  rtems_device_minor_number  minor;

  /*
   * Mutual-exclusion semaphores
   */
  rtems_id  isem;
  rtems_id  osem;

  /*
   * The canonical (cooked) character buffer
   */
  char    *cbuf;
  int    ccount;
  int    cindex;

  /*
   * Keep track of cursor (printhead) position
   */
  int    column;
  int    read_start_column;

  /*
   * The ioctl settings
   */
  struct termios  termios;
  rtems_interval  vtimeTicks;

  /*
   * Raw input character buffer
   */
  struct rtems_termios_rawbuf rawInBuf;
  uint32_t                    rawInBufSemaphoreOptions;
  rtems_interval              rawInBufSemaphoreTimeout;
  rtems_interval              rawInBufSemaphoreFirstTimeout;
  unsigned int                rawInBufDropped;  /* Statistics */

  /*
   * Raw output character buffer
   */
  struct rtems_termios_rawbuf rawOutBuf;
  int  t_dqlen; /* count of characters dequeued from device */
  enum {rob_idle, rob_busy, rob_wait }  rawOutBufState;

  /*
   * Callbacks to device-specific routines
   */
  rtems_termios_callbacks  device;
  volatile unsigned int    flow_ctrl;
  unsigned int             lowwater,highwater;

  /*
   * I/O task IDs (for task-driven drivers)
   */
  rtems_id                rxTaskId;
  rtems_id                txTaskId;

  /*
   * line discipline related stuff
   */
  int t_line;   /* id of line discipline                       */
  void *t_sc;   /* hook for discipline-specific data structure */

  /*
   * Wakeup callback variables
   */
  struct ttywakeup tty_snd;
  struct ttywakeup tty_rcv;
  int              tty_rcvwakeup;
};

struct rtems_termios_linesw {
  int (*l_open) (struct rtems_termios_tty *tp);
  int (*l_close)(struct rtems_termios_tty *tp);
  int (*l_read )(struct rtems_termios_tty *tp,rtems_libio_rw_args_t *args);
  int (*l_write)(struct rtems_termios_tty *tp,rtems_libio_rw_args_t *args);
  int (*l_rint )(int c,struct rtems_termios_tty *tp);
  int (*l_start)(struct rtems_termios_tty *tp);
  int (*l_ioctl)(struct rtems_termios_tty *tp,rtems_libio_ioctl_args_t *args);
  int (*l_modem)(struct rtems_termios_tty *tp,int flags);
};

/*
 * FIXME: this should move to libio.h!
 * values for rtems_termios_callbacks.outputUsesInterrupts
 */
#define TERMIOS_POLLED      0
#define TERMIOS_IRQ_DRIVEN  1
#define TERMIOS_TASK_DRIVEN 2

/*
 * FIXME: this should move to termios.h!
 */
void rtems_termios_rxirq_occured(struct rtems_termios_tty *tty);

/*
 * FIXME: this should move to termios.h!
 * put a string to output ring buffer
 */
void rtems_termios_puts (
  const void               *buf,
  int                       len,
  struct rtems_termios_tty *tty
);

/*
 * global hooks for line disciplines
 */
extern struct rtems_termios_linesw rtems_termios_linesw[];
extern int   rtems_termios_nlinesw;

#define TTYDISC   0    /* termios tty line discipline */
#define TABLDISC  3    /* tablet discipline */
#define SLIPDISC  4    /* serial IP discipline */
#define PPPDISC   5    /* PPP discipline */
#define MAXLDISC  8

/* baudrate xxx integer type */
typedef int32_t rtems_termios_baud_t;

/* convert xxx integer to equivalent Bxxx constant */
int  rtems_termios_number_to_baud(rtems_termios_baud_t baud);

/* convert Bxxx constant to xxx integer */
rtems_termios_baud_t rtems_termios_baud_to_number(int termios_baud);

/* convert Bxxx constant to index */
int  rtems_termios_baud_to_index(rtems_termios_baud_t termios_baud);

/*
 *  This method is used by a driver to tell termios its
 *  initial baud rate.  This is especially important when
 *  the device driver does not set the baud to the default
 *  of B9600.
 */
int  rtems_termios_set_initial_baud(
  struct rtems_termios_tty *ttyp,
  rtems_termios_baud_t      baud
);

#ifdef __cplusplus
}
#endif

#endif  /* TERMIOSTYPES_H */
