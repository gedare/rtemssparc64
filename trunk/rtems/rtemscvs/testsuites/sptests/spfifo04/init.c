/*
 *  COPYRIGHT (c) 2010
 *  Bharath Suri<bharath.s.jois@gmail.com>.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: init.c,v 1.2 2010/06/10 14:44:27 jennifer Exp $
 */

/* Includes */
#include <bsp.h>
#include <tmacros.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <rtems.h>
#include <rtems/libio.h>

#define NUM_OPEN_REQ        26

void test_main(void)
{

  int status = -1;
  int fd = 0;
  int index = 0;
  int num_opens = 0;

  puts("\n\n*** FIFO / PIPE OPEN TEST - 4 ***");
  puts(
"\n\nConfiguration: Pipes configured, \
but number of barriers configured = 2\n"
       );

  puts("\n\nCreating directory /tmp");
  status = mkdir("/tmp", 0777);
  rtems_test_assert(status == 0);

  puts("\n\nCreating fifo /tmp/fifo");
  status = mkfifo("/tmp/fifo01", 0777);
  rtems_test_assert(status == 0);

  puts("\n\nAttempt to open the fifo file in RDWR mode\n");
  puts("Should be successful and non-negative\
file descriptor expected");

  fd = open("/tmp/fifo01", O_RDWR);
  rtems_test_assert(fd > 0);
  ++num_opens;

  puts("\n\nClosing the fifo file");
  status = close(fd);
  rtems_test_assert(status == 0);

  puts("\n\nAttempt to open the fifo file in \
RDONLY and NONBLOCK mode\n");
  puts("Should be successful and non-negative\
file descriptor expected");

  fd = open("/tmp/fifo01", O_RDONLY | O_NONBLOCK);
  rtems_test_assert(fd > 0);
  ++num_opens;

  puts("\n\nClosing the fifo file");
  status = close(fd);
  rtems_test_assert(status == 0);
  
  puts("\n\nAttempt to open the fifo file in \
WRONLY and NONBLOCK mode\n");
  puts("Should return with an error ENXIO");

  fd = open("/tmp/fifo01", O_WRONLY | O_NONBLOCK);
  rtems_test_assert(fd == -1);
  rtems_test_assert(errno == ENXIO);
  ++num_opens;

  // Number of attempts to open fifo till now = 3
  // Number of attempts to move the static char
  // from 'a' -> 'z' = 26
  // => Number of opens required = 23
  puts("\n\nMultiple opens\n");
  for(index = 0; index < NUM_OPEN_REQ - num_opens; ++index) {

    fd = open("/tmp/fifo01", O_RDONLY | O_NONBLOCK);
    rtems_test_assert(fd > 0);

    status = close(fd);
    rtems_test_assert(status == 0);

    printf("%d... ", index+1);
  }

  puts("\n\nRemove the entry /tmp/fifo01");
  status = unlink("/tmp/fifo01");
  rtems_test_assert(status == 0);

  puts("\n\nRemove directory /tmp");
  status = rmdir("/tmp");
  rtems_test_assert(status == 0);

  puts("\n\n*** END OF FIFO / PIPE OPEN TEST - 4 ***");
}

rtems_task Init(
  rtems_task_argument not_used
)
{
  test_main();
  rtems_test_exit(0);
}

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 6

#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_PIPES_ENABLED
#define CONFIGURE_INIT
#define CONFIGURE_MAXIMUM_BARRIERS               2

#include <rtems/confdefs.h>

/* end of file */
