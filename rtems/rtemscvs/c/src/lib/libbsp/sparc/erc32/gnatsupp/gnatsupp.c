/*
 *
 * Support for gnat/rtems interrupts and exception handling.
 * Jiri Gaisler, ESA/ESTEC, 17-02-1999.
 *
 *  $Id: gnatsupp.c,v 1.6 2008/08/18 11:25:27 ralf Exp $
 */

void __gnat_install_handler_common( int, int );

void
__gnat_install_handler (void)
{
  __gnat_install_handler_common (0x1d, 0x15);
}
