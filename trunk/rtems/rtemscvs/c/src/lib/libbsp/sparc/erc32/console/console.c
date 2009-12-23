/*
 *  This file contains the TTY driver for the serial ports on the erc32.
 *
 *  This driver uses the termios pseudo driver.
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: console.c,v 1.30 2009/12/10 14:43:52 ralf Exp $
 */

#include <bsp.h>
#include <rtems/libio.h>
#include <stdlib.h>
#include <assert.h>

/*
 *  Should we use a polled or interrupt drived console?
 *
 *  NOTE: This is defined in the custom/erc32.cfg file.
 *
 *  WARNING:  In sis 1.6, it did not appear that the UART interrupts
 *            worked in a desirable fashion.  Immediately upon writing
 *            a character into the TX buffer, an interrupt was generated.
 *            This did not allow enough time for the program to put more
 *            characters in the buffer.  So every character resulted in
 *            "priming" the transmitter.   This effectively results in
 *            in a polled console with a useless interrupt per character
 *            on output.  It is reasonable to assume that input does not
 *            share this problem although it was not investigated.
 *
 */

/*
 *  console_outbyte_polled
 *
 *  This routine transmits a character using polling.
 */

void console_outbyte_polled(
  int  port,
  char ch
);

/* body is in debugputs.c */

/*
 *  console_inbyte_nonblocking
 *
 *  This routine polls for a character.
 */

int console_inbyte_nonblocking( int port );

/* body is in debugputs.c */

/*
 *  Interrupt driven console IO
 */

#if (CONSOLE_USE_INTERRUPTS)

/*
 *  Buffers between task and ISRs
 */

#include <rtems/ringbuf.h>

Ring_buffer_t  TX_Buffer[ 2 ];
bool           Is_TX_active[ 2 ];

void *console_termios_data[ 2 ];

/*
 *  console_isr_a
 *
 *  This routine is the console interrupt handler for Channel A.
 *
 *  Input parameters:
 *    vector - vector number
 *
 *  Output parameters: NONE
 *
 *  Return values:     NONE
 */

rtems_isr console_isr_a(
  rtems_vector_number vector
)
{
  char ch;
  int UStat;

  if ( (UStat = ERC32_MEC.UART_Status) & ERC32_MEC_UART_STATUS_DRA ) {
    if (UStat & ERC32_MEC_UART_STATUS_ERRA) {
      ERC32_MEC.UART_Status = ERC32_MEC_UART_STATUS_CLRA;
      ERC32_MEC.Control = ERC32_MEC.Control;
    }
    ch = ERC32_MEC.UART_Channel_A;

    rtems_termios_enqueue_raw_characters( console_termios_data[ 0 ], &ch, 1 );
  }

  if ( ERC32_MEC.UART_Status & ERC32_MEC_UART_STATUS_THEA ) {
    if ( !Ring_buffer_Is_empty( &TX_Buffer[ 0 ] ) ) {
      Ring_buffer_Remove_character( &TX_Buffer[ 0 ], ch );
      ERC32_MEC.UART_Channel_A = (uint32_t) ch;
    } else
     Is_TX_active[ 0 ] = false;
  }

  ERC32_Clear_interrupt( ERC32_INTERRUPT_UART_A_RX_TX );
}

/*
 *  console_isr_b
 *
 *  This routine is the console interrupt handler for Channel B.
 *
 *  Input parameters:
 *    vector - vector number
 *
 *  Output parameters: NONE
 *
 *  Return values:     NONE
 */

rtems_isr console_isr_b(
  rtems_vector_number vector
)
{
  char ch;
  int UStat;

  if ( (UStat = ERC32_MEC.UART_Status) & ERC32_MEC_UART_STATUS_DRB ) {
    if (UStat & ERC32_MEC_UART_STATUS_ERRB) {
      ERC32_MEC.UART_Status = ERC32_MEC_UART_STATUS_CLRB;
      ERC32_MEC.Control = ERC32_MEC.Control;
    }
    ch = ERC32_MEC.UART_Channel_B;
    rtems_termios_enqueue_raw_characters( console_termios_data[ 1 ], &ch, 1 );

  }

  if ( ERC32_MEC.UART_Status & ERC32_MEC_UART_STATUS_THEB ) {
    if ( !Ring_buffer_Is_empty( &TX_Buffer[ 1 ] ) ) {
      Ring_buffer_Remove_character( &TX_Buffer[ 1 ], ch );
      ERC32_MEC.UART_Channel_B = (uint32_t) ch;
    } else
     Is_TX_active[ 1 ] = false;
  }

  ERC32_Clear_interrupt( ERC32_INTERRUPT_UART_B_RX_TX );
}

/*
 *  console_exit
 *
 *  This routine allows the console to exit by masking its associated interrupt
 *  vectors.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters: NONE
 *
 *  Return values:     NONE
 */

void console_exit(void)
{
  uint32_t         port;
  uint32_t         ch;

  /*
   *  Although the interrupts for the UART are unmasked, the PIL is set to
   *  disable all external interrupts.  So we might as well do this first.
   */

  ERC32_Mask_interrupt( ERC32_INTERRUPT_UART_A_RX_TX );
  ERC32_Mask_interrupt( ERC32_INTERRUPT_UART_B_RX_TX );

  for ( port=0 ; port <= 1 ; port++ ) {
    while ( !Ring_buffer_Is_empty( &TX_Buffer[ port ] ) ) {
      Ring_buffer_Remove_character( &TX_Buffer[ port ], ch );
      console_outbyte_polled( port, ch );
    }
  }

  /*
   *  Now wait for all the data to actually get out ... the send register
   *  should be empty.
   */

  while ( (ERC32_MEC.UART_Status & ERC32_MEC_UART_STATUS_THEA) !=
          ERC32_MEC_UART_STATUS_THEA );

  while ( (ERC32_MEC.UART_Status & ERC32_MEC_UART_STATUS_THEB) !=
          ERC32_MEC_UART_STATUS_THEB );

}

#define CONSOLE_UART_A_TRAP  ERC32_TRAP_TYPE( ERC32_INTERRUPT_UART_A_RX_TX )
#define CONSOLE_UART_B_TRAP  ERC32_TRAP_TYPE( ERC32_INTERRUPT_UART_B_RX_TX )

/*
 *  console_initialize_interrupts
 *
 *  This routine initializes the console's receive and transmit
 *  ring buffers and loads the appropriate vectors to handle the interrupts.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters: NONE
 *
 *  Return values:     NONE
 */

#ifdef RDB_BREAK_IN
  extern uint32_t   trap_table[];
#endif

void console_initialize_interrupts( void )
{
  Ring_buffer_Initialize( &TX_Buffer[ 0 ] );
  Ring_buffer_Initialize( &TX_Buffer[ 1 ] );

  Is_TX_active[ 0 ] = false;
  Is_TX_active[ 1 ] = false;

  atexit( console_exit );

  set_vector( console_isr_a, CONSOLE_UART_A_TRAP, 1 );
#ifdef RDB_BREAK_IN
  if (trap_table[0x150/4] == 0x91d02000)
#endif
  set_vector( console_isr_b, CONSOLE_UART_B_TRAP, 1 );
}

/*
 *  console_outbyte_interrupt
 *
 *  This routine transmits a character out.
 *
 *  Input parameters:
 *    port - port to transmit character to
 *    ch  - character to be transmitted
 *
 *  Output parameters:  NONE
 *
 *  Return values:      NONE
 */

void console_outbyte_interrupt(
  int   port,
  char  ch
)
{
  /*
   *  If this is the first character then we need to prime the pump
   */

  if ( Is_TX_active[ port ] == false ) {
    Is_TX_active[ port ] = true;
    console_outbyte_polled( port, ch );
    return;
  }

  while ( Ring_buffer_Is_full( &TX_Buffer[ port ] ) );

  Ring_buffer_Add_character( &TX_Buffer[ port ], ch );
}

#endif /* CONSOLE_USE_INTERRUPTS */

/*
 *  Console Termios Support Entry Points
 *
 */

ssize_t console_write_support (int minor, const char *buf, size_t len)
{
  int nwrite = 0;

  while (nwrite < len) {
#if (CONSOLE_USE_INTERRUPTS)
    console_outbyte_interrupt( minor, *buf++ );
#else
    console_outbyte_polled( minor, *buf++ );
#endif
    nwrite++;
  }
  return nwrite;
}

/*
 *  Console Device Driver Entry Points
 *
 */

rtems_device_driver console_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void                      *arg
)
{
  rtems_status_code status;

  /*
   *  Ensure Termios is initialized
   */
  rtems_termios_initialize();

  /*
   *  Register Device Names
   */
  status = rtems_io_register_name( "/dev/console", major, 0 );
  if (status != RTEMS_SUCCESSFUL)
    rtems_fatal_error_occurred(status);

  status = rtems_io_register_name( "/dev/console_b", major, 1 );
  if (status != RTEMS_SUCCESSFUL)
    rtems_fatal_error_occurred(status);

  /*
   *  Initialize Hardware
   */
  #if (CONSOLE_USE_INTERRUPTS)
    console_initialize_interrupts();
  #endif

  return RTEMS_SUCCESSFUL;
}

rtems_device_driver console_open(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void                    * arg
)
{
  rtems_status_code sc;
#if (CONSOLE_USE_INTERRUPTS)
  rtems_libio_open_close_args_t *args = arg;
  static const rtems_termios_callbacks intrCallbacks = {
    NULL,                        /* firstOpen */
    NULL,                        /* lastClose */
    NULL,                        /* pollRead */
    console_write_support,       /* write */
    NULL,                        /* setAttributes */
    NULL,                        /* stopRemoteTx */
    NULL,                        /* startRemoteTx */
    0                            /* outputUsesInterrupts */
  };
#else
  static const rtems_termios_callbacks pollCallbacks = {
    NULL,                        /* firstOpen */
    NULL,                        /* lastClose */
    console_inbyte_nonblocking,  /* pollRead */
    console_write_support,       /* write */
    NULL,                        /* setAttributes */
    NULL,                        /* stopRemoteTx */
    NULL,                        /* startRemoteTx */
    0                            /* outputUsesInterrupts */
  };
#endif

  assert( minor <= 1 );
  if ( minor > 2 )
    return RTEMS_INVALID_NUMBER;

#if (CONSOLE_USE_INTERRUPTS)
  sc = rtems_termios_open (major, minor, arg, &intrCallbacks);

  console_termios_data[ minor ] = args->iop->data1;
#else
  sc = rtems_termios_open (major, minor, arg, &pollCallbacks);
#endif

  return RTEMS_SUCCESSFUL;
}

rtems_device_driver console_close(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void                    * arg
)
{
  return rtems_termios_close (arg);
}

rtems_device_driver console_read(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void                    * arg
)
{
  return rtems_termios_read (arg);
}

rtems_device_driver console_write(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void                    * arg
)
{
  return rtems_termios_write (arg);
}

rtems_device_driver console_control(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void                    * arg
)
{
  return rtems_termios_ioctl (arg);
}
