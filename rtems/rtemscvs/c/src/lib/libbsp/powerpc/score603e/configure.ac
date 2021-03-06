## Process this file with autoconf to produce a configure script.
##
## $Id: configure.ac,v 1.34 2009/11/28 06:28:40 ralf Exp $

AC_PREREQ(2.60)
AC_INIT([rtems-c-src-lib-libbsp-powerpc-ep1a],[_RTEMS_VERSION],[http://www.rtems.org/bugzilla])
AC_CONFIG_SRCDIR([bsp_specs])
RTEMS_TOP(../../../../../..)

RTEMS_CANONICAL_TARGET_CPU
AM_INIT_AUTOMAKE([no-define nostdinc foreign 1.10])
RTEMS_BSP_CONFIGURE

RTEMS_PROG_CC_FOR_TARGET
RTEMS_CANONICALIZE_TOOLS
RTEMS_PROG_CCAS

## FIXME: This should be a 1 out of 3 selection
## and is somehow coupled to USE_DINK (cf. include/gen2.h)
RTEMS_BSPOPTS_SET([SCORE603E_USE_SDS],[*],[0])
RTEMS_BSPOPTS_HELP([SCORE603E_USE_SDS],
[use SDS ROM monitor])

RTEMS_BSPOPTS_SET([SCORE603E_OPEN_FIRMWARE],[*],[0])
RTEMS_BSPOPTS_HELP([SCORE603E_OPEN_FIRMWARE],
[use Open Firmware ROM monitor])

RTEMS_BSPOPTS_SET([SCORE603E_USE_NONE],[*],[0])
RTEMS_BSPOPTS_HELP([SCORE603E_USE_NONE],
[use no ROM monitor])

RTEMS_BSPOPTS_SET([SCORE603E_USE_DINK],[*],[1])
RTEMS_BSPOPTS_HELP([SCORE603E_USE_DINK],
[FIXME: Missing explanation.])

RTEMS_BSPOPTS_SET([CONSOLE_USE_INTERRUPTS],[*],[0])
RTEMS_BSPOPTS_HELP([CONSOLE_USE_INTERRUPTS],
[whether using console interrupts])

RTEMS_BSPOPTS_SET([HAS_PMC_PSC8],[*],[1])
RTEMS_BSPOPTS_HELP([HAS_PMC_PSC8],
[whether has a PSC8 PMC board attached to PMC slot])

RTEMS_BSPOPTS_SET([INITIALIZE_COM_PORTS],[*],[0])
RTEMS_BSPOPTS_HELP([INITIALIZE_COM_PORTS],
[FIXME: Missing explanation])

RTEMS_BSPOPTS_SET([PPC_USE_SPRG],[*],[0])
RTEMS_BSPOPTS_HELP([PPC_USE_SPRG],
[If defined, then the PowerPC specific code in RTEMS will use some
of the special purpose registers to slightly optimize interrupt
response time.  The use of these registers can conflict with
other tools like debuggers.])

RTEMS_BSPOPTS_SET([PPC_USE_DATA_CACHE],[*],[0])
RTEMS_BSPOPTS_HELP([PPC_USE_DATA_CACHE],
[If defined, then the PowerPC specific code in RTEMS will use
 data cache instructions to optimize the context switch code.
 This code can conflict with debuggers or emulators.  It is known
 to break the Corelis PowerPC emulator with at least some combinations
 of PowerPC 603e revisions and emulator versions.
 The BSP actually contains the call that enables this.])

RTEMS_BSPOPTS_SET([PPC_VECTOR_FILE_BASE],[*],[0x0100])
RTEMS_BSPOPTS_HELP([PPC_VECTOR_FILE_BASE],
[This defines the base address of the exception table.
 NOTE: Vectors are actually at 0xFFF00000 but file starts at offset.])

RTEMS_CHECK_NETWORKING
AM_CONDITIONAL(HAS_NETWORKING,test "$HAS_NETWORKING" = "yes")

RTEMS_BSP_CLEANUP_OPTIONS(0, 0)

# Explicitly list all Makefiles here
AC_CONFIG_FILES([Makefile])

RTEMS_PPC_EXCEPTIONS

AC_OUTPUT
