## Process this file with autoconf to produce a configure script.
##
## $Id: configure.ac,v 1.15 2009/11/28 06:28:40 ralf Exp $

AC_PREREQ(2.60)
AC_INIT([rtems-c-src-lib-libbsp-powerpc-gen5200],[_RTEMS_VERSION],[http://www.rtems.org/bugzilla])
AC_CONFIG_SRCDIR([bsp_specs])
RTEMS_TOP(../../../../../..)

RTEMS_CANONICAL_TARGET_CPU
AM_INIT_AUTOMAKE([no-define nostdinc foreign 1.10])
RTEMS_BSP_CONFIGURE

RTEMS_PROG_CC_FOR_TARGET
RTEMS_CANONICALIZE_TOOLS
RTEMS_PROG_CCAS

RTEMS_BSPOPTS_SET([DATA_CACHE_ENABLE],[*],[1])
RTEMS_BSPOPTS_HELP([DATA_CACHE_ENABLE],
[If defined, the data cache will be enabled after address translation
 is turned on.])

RTEMS_BSPOPTS_SET([INSTRUCTION_CACHE_ENABLE],[*],[1])
RTEMS_BSPOPTS_HELP([INSTRUCTION_CACHE_ENABLE],
[If defined, the instruction cache will be enabled after address translation
 is turned on.])

RTEMS_BSPOPTS_SET([BENCHMARK_IRQ_PROCESSING],[*],[0])
RTEMS_BSPOPTS_HELP([BENCHMARK_IRQ_PROCESSING],
[If defined, enable code to benchmark IRQ processing.])

RTEMS_BSPOPTS_SET([ALLOW_IRQ_NESTING],[icecube],[0])
RTEMS_BSPOPTS_SET([ALLOW_IRQ_NESTING],[*],[1])
RTEMS_BSPOPTS_HELP([ALLOW_IRQ_NESTING],
[If defined, allow nested IRQ processing.])

RTEMS_BSPOPTS_SET([BSP_PRESS_KEY_FOR_RESET],[icecube],[1])
RTEMS_BSPOPTS_SET([BSP_PRESS_KEY_FOR_RESET],[*],[0])
RTEMS_BSPOPTS_HELP([BSP_PRESS_KEY_FOR_RESET],
[If defined, print a message and wait until pressed before resetting
 board when application exits.])

RTEMS_BSPOPTS_SET([BSP_RESET_BOARD_AT_EXIT],[icecube],[1])
RTEMS_BSPOPTS_SET([BSP_RESET_BOARD_AT_EXIT],[*],[0])
RTEMS_BSPOPTS_HELP([BSP_RESET_BOARD_AT_EXIT],
[If defined, reset the board when the application exits.])

RTEMS_BSPOPTS_SET([PM520_ZE30],[pm520_ze30],[1])
RTEMS_BSPOPTS_HELP([PM520_ZE30],
[If defined, use custom settings of for the pm520_ze30 BSP])

RTEMS_BSPOPTS_SET([PM520_CR825],[pm520_cr825],[1])
RTEMS_BSPOPTS_HELP([PM520_CR825],
[If defined, use custom settings of for the pm520_cr825 BSP])

RTEMS_BSPOPTS_SET([icecube],[icecube],[1])
RTEMS_BSPOPTS_HELP([icecube],
[If defined, use custom settings of for the icecube BSP])

RTEMS_BSPOPTS_SET([BRS5L],[brs5l],[1])
RTEMS_BSPOPTS_HELP([BRS5L],
[If defined, use custom settings of for the brs5l BSP])


RTEMS_CHECK_NETWORKING
AM_CONDITIONAL(HAS_NETWORKING,test "$HAS_NETWORKING" = "yes")

# Explicitly list all Makefiles here
AC_CONFIG_FILES([Makefile])

RTEMS_BSP_CLEANUP_OPTIONS(0, 0)
RTEMS_BSP_LINKCMDS

RTEMS_PPC_EXCEPTIONS

AC_OUTPUT