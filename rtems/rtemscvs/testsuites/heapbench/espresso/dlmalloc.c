/* ---------- To make a malloc.h, start cutting here ------------ */

/*
  ****************************************************************
  * THIS IS A PRERELEASE. It has not yet been tested adequately. *
  * If you use it, please send back comments, suggestions,       *
  * performance reports, etc.                                    *
  ****************************************************************
*/

/*
  A version of malloc/free/realloc written by Doug Lea and released to the
  public domain.  AKA dlmalloc

  Send questions/comments/complaints/performance data to
  dl@cs.oswego.edu


* VERSION 2.7.0pre5 Sat Oct 14 11:24:17 2000  Doug Lea  (dl at gee)

   Note: There may be an updated version of this malloc obtainable at
           ftp://gee.cs.oswego.edu/pub/misc/malloc.c
         Check before installing!

* Why use this malloc?

  This is not the fastest, most space-conserving, most portable, or
  most tunable malloc ever written. However it is among the fastest
  while also being among the most space-conserving, portable and tunable.
  Consistent balance across these factors results in a good general-purpose
  allocator.

  The main properties of the algorithm are:
  * For large (>= 512 bytes) requests, it is a pure best-fit allocator,
    with ties among equal fits decided via FIFO (i.e. least recently used).
  * For small (<= 64 bytes by default) requests, it is a caching
    allocator, that maintains pools of quickly recycled chunks.
  * In between, and for combinations of large and small requests, it does
    the best it can trying to meet both goals at once.

  Compared to 2.6.X versions, this version is generally faster,
  especially for programs that allocate and free many small chunks.

  For a longer but slightly out of date high-level description, see
     http://gee.cs.oswego.edu/dl/html/malloc.html

  You may already by default be using a c library containing a malloc
  that is somehow based on some version of this malloc (for example in
  linux). You might still want to use the current version in order to
  customize settings or to avoid overheads associated with library
  versions.

  This library is all in one file to simplify the most common use
  case: ftping, compiling (-O), and linking into another program. All
  of the compile-time options default to reasonable values for use on
  most unix platforms. Compile -DWIN32 for reasonable defaults on windows.

* Synopsis of public routines

  (Much fuller descriptions are contained in the program documentation below.)

  malloc(size_t n);
     Return a pointer to a newly allocated chunk of at least n bytes, or null
     if no space is available.
  free(Void_t* p);
     Release the chunk of memory pointed to by p, or no effect if p is null.
  realloc(Void_t* p, size_t n);
     Return a pointer to a chunk of size n that contains the same data
     as does chunk p up to the minimum of (n, p's size) bytes, or null
     if no space is available. The returned pointer may or may not be
     the same as p. If p is null, equivalent to malloc.  Unless the
     #define REALLOC_ZERO_BYTES_FREES below is set, realloc with a
     size argument of zero (re)allocates a minimum-sized chunk.
  memalign(size_t alignment, size_t n);
     Return a pointer to a newly allocated chunk of n bytes, aligned
     in accord with the alignment argument, which must be a power of
     two.
  valloc(size_t n);
     Equivalent to memalign(pagesize, n), where pagesize is the page
     size of the system (or as near to this as can be figured out from
     all the includes/defines below.)
  pvalloc(size_t n);
     Equivalent to valloc(minimum-page-that-holds(n)), that is,
     round up n to nearest pagesize.
  calloc(size_t unit, size_t quantity);
     Returns a pointer to quantity * unit bytes, with all locations
     set to zero.
  cfree(Void_t* p);
     Equivalent to free(p).
  malloc_trim(size_t pad);
     Release all but pad bytes of freed top-most memory back
     to the system. Return 1 if successful, else 0.
  malloc_usable_size(Void_t* p);
     Report the number usable allocated bytes associated with allocated
     chunk p. This may or may not report more bytes than were requested,
     due to alignment and minimum size constraints.
  malloc_stats();
     Prints brief summary statistics on stderr.
  mallinfo()
     Returns (by copy) a struct containing various summary statistics.
  mallopt(int parameter_number, int parameter_value)
     Changes one of the tunable parameters described below. Returns
     1 if successful in changing the parameter, else 0.

* Vital statistics:

  Alignment:                            8-byte
       8 byte alignment is currently hardwired into the design.  This
       seems to suffice for all current machines and C compilers.
       People have reported porting to wider alignments but they
       are unchecked by me.

  Assumed pointer representation:       4 or 8 bytes
       (Thanks to Wolfram Gloger for contributing most of the
       changes supporting dual 4/8.)

  Assumed size_t  representation:       4 or 8 bytes
       Note that size_t is allowed to be 4 bytes even if pointers are 8.

  Minimum overhead per allocated chunk: 4 or 8 bytes
       Each malloced chunk has a hidden overhead of 4 bytes holding size
       and status information.

  Minimum allocated size: 4-byte ptrs:  16 bytes    (including 4 overhead)
                          8-byte ptrs:  24/32 bytes (including, 4/8 overhead)

       When a chunk is freed, 12 (for 4byte ptrs) or 20 (for 8 byte
       ptrs but 4 byte size) or 24 (for 8/8) additional bytes are
       needed; 4 (8) for a trailing size field and 8 (16) bytes for
       free list pointers. Thus, the minimum allocatable size is
       16/24/32 bytes.

       Even a request for zero bytes (i.e., malloc(0)) returns a
       pointer to something of the minimum allocatable size.

  Maximum allocated size: 4-byte size_t: 2^31 minus about two pages
                          8-byte size_t: 2^63 minus about two pages

       It is assumed that (possibly signed) size_t values suffice
       to represent chunk sizes. `Possibly signed' is due to the fact
       that `size_t' may be defined on a system as either a signed or
       an unsigned type. The ISO C standard says that it must be
       unsigned, but a few systems are known not to adhere to this.
       Additionally, even when size_t is unsigned, sbrk (which is by
       default used to obtain memory from system) accepts signed
       arguments, and may not be able to handle size_t-wide arguments
       with negative sign bit.  To be conservative, values that would
       appear as negative after accounting for overhead and alignment
       are rejected.

       Requests for sizes outside this range will perform an optional
       failure action and then return null. (Large requests may also
       also fail because a system is out of memory.)

  Maximum overhead wastage per allocated chunk: normally 15 bytes

       Alignnment demands, plus the minimum allocatable size restriction
       make the normal worst-case wastage 15 bytes (i.e., up to 15
       more bytes will be allocated than were requested in malloc), with
       two exceptions:
         1. Because requests for zero bytes allocate non-zero space,
            the worst case wastage for a request of zero bytes is 16/24 bytes.
         2. For requests >= mmap_threshold that are serviced via
            mmap(), the worst case wastage is 8 bytes plus the remainder
            from a system page (the minimal mmap unit); typically 4096 bytes.

  Thread-safety: NOT thread-safe unless USE_PTHREAD_LOCKS defined

       When USE_PTHREAD_LOCKS is defined, wrappers are created to
       surround every public call with a pthread mutex. This is not
       very fast, and can be a major bottleneck in programs with many
       threads. It is designed only to provide minimal protection in
       concurrent environments, and to provide a basis for extensions.
       If you are using malloc in a concurrent program, you would be
       far better off obtaining ptmalloc, which is derived from a
       version of this malloc, and is well-tuned for concurrent
       programs.

  Compliance: I believe it is compliant with the 1997 Single Unix Specification

       (See http://www.opennc.org). Probably other standards as well.

* Limitations

    Here are some features that are NOT currently supported

    * No automated mechanism for fully checking that all accesses
      to malloced memory stay within their bounds.
    * No support for compaction.

* Synopsis of compile-time options:

    People have reported using previous versions of this malloc on all
    versions of Unix, sometimes by tweaking some of the defines
    below. It has been tested most extensively on Solaris and
    Linux. It is also reported to work on WIN32 platforms.
    People also report using it in stand-alone embedded systems.

    The implementation is in straight, hand-tuned ANSI C.  It is not
    at all modular. (Sorry!)  It uses a lot of macros.  To be at all
    usable, this code should be compiled using an optimizing compiler
    (for example gcc -O3) that can simplify expressions and control
    paths. (FAQ: some macros import variables as arguments rather than
    declare locals because people reported that some debuggers
    otherwise get confused.)

    OPTION                     DEFAULT VALUE

    Compilation Environment options:

    __STD_C                    derived from C compiler defines
    WIN32                      NOT defined
    HAVE_MEMCPY                defined
    USE_MEMCPY                 1 if HAVE_MEMCPY is defined
    HAVE_MMAP                  defined as 1 unless WIN32
    MMAP_AS_MORECORE_SIZE      (1024 * 1024) 
    HAVE_MREMAP                defined as 0 unless linux defined
    malloc_getpagesize         derived from system #includes, or 4096 if not
    HAVE_USR_INCLUDE_MALLOC_H  NOT defined
    LACKS_UNISTD_H             NOT defined unless WIN32
    LACKS_SYS_PARAM_H          NOT defined unless WIN32
    INTERNAL_SIZE_T            size_t

    Configuration and functionality options:

    USE_DL_PREFIX              NOT defined
    USE_PUBLIC_MALLOC_WRAPPERS NOT defined
    USE_PTHREAD_LOCKS          NOT defined
    DEBUG                      NOT defined
    REALLOC_ZERO_BYTES_FREES   NOT defined
    MALLOC_FAILURE_ACTION      errno = ENOMEM, if __STD_C defined, else no-op

    Options for customizing MORECORE:

    MORECORE                   sbrk
    MORECORE_CONTIGUOUS        1 

    Tuning options that are also dynamically changeable via mallopt:

    DEFAULT_MXFAST             64
    DEFAULT_TRIM_THRESHOLD     128 * 1024
    DEFAULT_TOP_PAD            0
    DEFAULT_MMAP_THRESHOLD     128 * 1024
    DEFAULT_MMAP_MAX           256

    There are several other #defined constants and macros that you
    probably don't want to touch unless you are extending or adapting malloc.

*/




/*
  WIN32 sets up defaults for MS environment and compilers.
  Otherwise defaults are for unix.
*/

/* #define WIN32 */

#ifdef WIN32
#define MORECORE wsbrk
#define MORECORE_CONTIGUOUS 1
#define HAVE_MMAP 0
#define LACKS_UNISTD_H
#define LACKS_SYS_PARAM_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
/*
  Could use 'GetSystemInfo' instead for pagesize, but 
  if wsbrk is used, then it doesn't matter anyway 
*/
#define malloc_getpagesize (4096) 
#endif

#define HAVE_MMAP 0


/*
  __STD_C should be nonzero if using ANSI-standard C compiler, a C++
  compiler, or a C compiler sufficiently close to ANSI to get away
  with it.
*/

#ifndef __STD_C
#ifdef __STDC__
#define __STD_C     1
#else
#if __cplusplus
#define __STD_C     1
#else
#define __STD_C     0
#endif /*__cplusplus*/
#endif /*__STDC__*/
#endif /*__STD_C*/




/*
  Void_t* is the pointer type that malloc should say it returns
*/

#ifndef Void_t
#if (__STD_C || defined(WIN32))
#define Void_t      void
#else
#define Void_t      char
#endif
#endif /*Void_t*/

#if __STD_C
#include <stddef.h>   /* for size_t */
#else
#include <sys/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* define LACKS_UNISTD_H if your system does not have a <unistd.h>. */

/* #define  LACKS_UNISTD_H */

#ifndef LACKS_UNISTD_H
#include <unistd.h>
#endif

/* define LACKS_SYS_PARAM_H if your system does not have a <sys/param.h>. */

/* #define  LACKS_SYS_PARAM_H */


#include <stdio.h>    /* needed for malloc_stats */
#include <errno.h>    /* needed for optional MALLOC_FAILURE_ACTION */


/*
  Debugging:

  Because freed chunks may be overwritten with bookkeeping fields, this
  malloc will often die when freed memory is overwritten by user
  programs.  This can be very effective (albeit in an annoying way)
  in helping track down dangling pointers.

  If you compile with -DDEBUG, a number of assertion checks are
  enabled that will catch more memory errors. You probably won't be
  able to make much sense of the actual assertion errors, but they
  should help you locate incorrectly overwritten memory.  The
  checking is fairly extensive, and will slow down execution
  noticeably. Calling malloc_stats or mallinfo with DEBUG set will
  attempt to check every non-mmapped allocated and free chunk in the
  course of computing the summmaries. (By nature, mmapped regions
  cannot be checked very much automatically.)

  Setting DEBUG may also be helpful if you are trying to modify
  this code. The assertions in the check routines spell out in more
  detail the assumptions and invariants underlying the algorithms.

*/

#if DEBUG
#include <assert.h>
#else
#define assert(x) ((void)0)
#endif


/*
  INTERNAL_SIZE_T is the word-size used for internal bookkeeping
  of chunk sizes.

  While not strictly necessary, it is best to define this as an
  unsigned type, even if size_t is a signed type. This may avoid some
  artificial size limitations on some systems.

  On a 64-bit machine, you can reduce malloc overhead by defining
  INTERNAL_SIZE_T to be a 32 bit `unsigned int' at the expense of not
  being able to handle requests greater than 2^31. This limitation is
  hardly ever a concern; you are encouraged to set this.

  However, the default version is the same as size_t.

  Note to implementors: To deal with all this, size comparisons among
  INTERNAL_SIZE_Ts should normally cast INTERNAL_SIZE_T's to long or
  unsigned long, as appropriate.

*/

#ifndef INTERNAL_SIZE_T
#define INTERNAL_SIZE_T size_t
#endif

/*
  REALLOC_ZERO_BYTES_FREES should be set if a call to
  realloc with zero bytes should be the same as a call to free.
  Some people think it should. Otherwise, since this malloc
  returns a unique pointer for malloc(0), so does realloc(p, 0).
*/

/*   #define REALLOC_ZERO_BYTES_FREES */


/*
  USE_DL_PREFIX will prefix all public routines with the string 'dl'.
  Useful when you only want to use this malloc in one part of a program.
*/

#define USE_DL_PREFIX


/*
  USE_PTHREAD_LOCKS causes wrapper functions to surround each
  callable routine with pthread mutex lock/unlock.

  USE_PTHREAD_LOCKS forces USE_PUBLIC_MALLOC_WRAPPERS to be defined
*/

/* #define USE_PTHREAD_LOCKS */


/*
  If USE_PUBLIC_MALLOC_WRAPPERS is defined, every public routine is
  actually a wrapper function that first calls MALLOC_PREACTION, then
  calls the internal routine, and follows it with
  MALLOC_POSTACTION. This is needed for locking, but you can also use
  this, without USE_PTHREAD_LOCKS, for purposes of interception,
  instrumentation, etc. It is a sad fact that using wrappers often
  noticeably degrades performance of malloc-intensive programs.
*/

#ifdef USE_PTHREAD_LOCKS
#define USE_PUBLIC_MALLOC_WRAPPERS
#else
/* #define USE_PUBLIC_MALLOC_WRAPPERS */
#endif




/*
  HAVE_MEMCPY should be defined if you are not otherwise using
  ANSI STD C, but still have memcpy and memset in your C library
  and want to use them in calloc and realloc. Otherwise simple
  macro versions are defined here.

  USE_MEMCPY should be defined as 1 if you actually want to
  have memset and memcpy called. People report that the macro
  versions are faster than libc versions on some systems.
  
  Even if USE_MEMCPY is set to 1, loops to copy/clear small chunks
  (of <= 36 bytes) are manually unrolled in realloc and calloc.
*/

#define HAVE_MEMCPY

#ifndef USE_MEMCPY
#ifdef HAVE_MEMCPY
#define USE_MEMCPY 1
#else
#define USE_MEMCPY 0
#endif
#endif


#if (__STD_C || defined(HAVE_MEMCPY))

#ifdef WIN32
  /*
    On Win32 platforms, 'memset()' and 'memcpy()' are already declared in
    'windows.h'
  */
#else
#if __STD_C
void* memset(void*, int, size_t);
void* memcpy(void*, const void*, size_t);
#else
Void_t* memset();
Void_t* memcpy();
#endif
#endif
#endif


/*
  MALLOC_FAILURE_ACTION is the action to take before "return 0" when
  malloc fails to be able to return memory, either because memory is
  exhausted or because of illegal arguments.
  
  By default, sets errno if running on STD_C platform, else does nothing.  
*/

#ifndef MALLOC_FAILURE_ACTION
#if __STD_C
#define MALLOC_FAILURE_ACTION \
   errno = ENOMEM;

#else

#define MALLOC_FAILURE_ACTION
#endif
#endif

/*
  Define HAVE_MMAP as true to optionally make malloc() use mmap() to
  allocate very large blocks.  These will be returned to the
  operating system immediately after a free(). Also, if mmap
  is available, it is used as a backup strategy in cases where
  MORECORE fails to provide space from system.
*/

#ifndef HAVE_MMAP
#define HAVE_MMAP 1
#endif

/* 
   MMAP_AS_MORECORE_SIZE is the minimum mmap size argument to use if
   sbrk fails, and mmap is used as a backup (which is done only if
   HAVE_MMAP).  The value must be a multiple of page size.  This
   backup strategy generally applies only when systems have "holes" in
   address space, so sbrk cannot perform contiguous expansion, but
   there is still space available on system.  On systems for which
   this is known to be useful (i.e. most linux kernels), this occurs
   only when programs allocate huge amounts of memory.  Between this,
   and the fact that mmap regions tend to be limited, the size should
   be large, to avoid too many mmap calls and thus avoid running out
   of kernel resources.
*/

#ifndef MMAP_AS_MORECORE_SIZE
#define MMAP_AS_MORECORE_SIZE (1024 * 1024)
#endif



/*
  Define HAVE_MREMAP to make realloc() use mremap() to re-allocate
  large blocks.  This is currently only possible on Linux with
  kernel versions newer than 1.3.77.
*/

#ifndef HAVE_MREMAP
#ifdef linux
#define HAVE_MREMAP 1
#else
#define HAVE_MREMAP 0
#endif

#endif /* HAVE_MMAP */


/*
  This version of malloc supports the standard SVID/XPG mallinfo
  routine that returns a struct containing the same kind of
  information you can get from malloc_stats. It should work on
  any SVID/XPG compliant system that has a /usr/include/malloc.h
  defining struct mallinfo. (If you'd like to install such a thing
  yourself, cut out the preliminary declarations as described above
  and below and save them in a malloc.h file. But there's no
  compelling reason to bother to do this.)

  The main declaration needed is the mallinfo struct that is returned
  (by-copy) by mallinfo().  The SVID/XPG malloinfo struct contains a
  bunch of field that are not even meaningful in this version of
  malloc.  These fields are are instead filled by mallinfo() with
  other numbers that might be of interest.

  HAVE_USR_INCLUDE_MALLOC_H should be set if you have a
  /usr/include/malloc.h file that includes a declaration of struct
  mallinfo.  If so, it is included; else an SVID2/XPG2 compliant
  version is declared below.  These must be precisely the same for
  mallinfo() to work.
*/

/* #define HAVE_USR_INCLUDE_MALLOC_H */

#if HAVE_USR_INCLUDE_MALLOC_H
#include "/usr/include/malloc.h"
#else

/* SVID2/XPG mallinfo structure */

struct mallinfo {
  int arena;    /* non-mmapped space allocated from system */
  int ordblks;  /* number of free chunks */
  int smblks;   /* number of fastbin blocks */
  int hblks;    /* number of mmapped regions */
  int hblkhd;   /* space in mmapped regions */
  int usmblks;  /* maximum total allocated space */
  int fsmblks;  /* space available in freed fastbin blocks */
  int uordblks; /* total allocated space */
  int fordblks; /* total free space */
  int keepcost; /* top-most, releasable (via malloc_trim) space */
};

/* SVID2/XPG mallopt options */

#define M_MXFAST  1    /* Set maximum fastbin size */
#define M_NLBLKS  2    /* UNUSED in this malloc */
#define M_GRAIN   3    /* UNUSED in this malloc */
#define M_KEEP    4    /* UNUSED in this malloc */


#endif


/* mallopt options supported in this malloc */

#ifndef M_TRIM_THRESHOLD
#define M_TRIM_THRESHOLD    -1
#endif

#ifndef M_TOP_PAD
#define M_TOP_PAD           -2
#endif

#ifndef M_MMAP_THRESHOLD
#define M_MMAP_THRESHOLD    -3
#endif

#ifndef M_MMAP_MAX
#define M_MMAP_MAX          -4
#endif


/*
  MXFAST is the maximum request size used for "fastbins", special bins
  that hold returned chunks without consolidating their spaces. This
  enables future requests for chunks of the same size to be handled
  very quickly, but can increase fragmentation (thus increasing overall
  memory footprint of a program).

  This malloc manages fastbins very conservatively yet still
  efficiently, so fragmentation is rarely a problem for values less
  than or equal to the default.  The maximum supported value of MXFAST
  is 80. You wouldn't want it any higher than this anyway.  Fastbins
  are designed especially for use with many small structs, objects or
  strings -- the default handles structs/objects/arrays with sizes up
  to 8 4byte fields, or strings representing words, tokens, etc. Using
  fastbins for larger, more variably sized objects normally worsens
  fragmentation without improving speed.

  MXFAST is set in REQUEST size units. It is internally used in
  chunksize units, which adds padding and alignment.  You can reduce
  MXFAST to 0 to disable all use of fastbins.  This causes the malloc
  algorithm to be a close approximation of fifo-best-fit in all cases,
  not just for larger requests.
*/

#ifndef DEFAULT_MXFAST
#define DEFAULT_MXFAST      64
#endif


/*
  M_TRIM_THRESHOLD is the maximum amount of unused top-most memory
  to keep before releasing via malloc_trim in free().

  Automatic trimming is mainly useful in long-lived programs.
  Because trimming via sbrk can be slow on some systems, and can
  sometimes be wasteful (in cases where programs immediately
  afterward allocate more large chunks) the value should be high
  enough so that your overall system performance would improve by
  releasing.

  The trim threshold and the mmap control parameters (see below)
  can be traded off with one another. Trimming and mmapping are
  two different ways of releasing unused memory back to the
  system. Between these two, it is often possible to keep
  system-level demands of a long-lived program down to a bare
  minimum. For example, in one test suite of sessions measuring
  the XF86 X server on Linux, using a trim threshold of 128K and a
  mmap threshold of 192K led to near-minimal long term resource
  consumption.

  If you are using this malloc in a long-lived program, it should
  pay to experiment with these values.  As a rough guide, you
  might set to a value close to the average size of a process
  (program) running on your system.  Releasing this much memory
  would allow such a process to run in memory.  Generally, it's
  worth it to tune for trimming rather tham memory mapping when a
  program undergoes phases where several large chunks are
  allocated and released in ways that can reuse each other's
  storage, perhaps mixed with phases where there are no such
  chunks at all.  And in well-behaved long-lived programs,
  controlling release of large blocks via trimming versus mapping
  is usually faster.

  However, in most programs, these parameters serve mainly as
  protection against the system-level effects of carrying around
  massive amounts of unneeded memory. Since frequent calls to
  sbrk, mmap, and munmap otherwise degrade performance, the default
  parameters are set to relatively high values that serve only as
  safeguards.

  The default trim value is high enough to cause trimming only in
  fairly extreme (by current memory consumption standards) cases.
  It must be greater than page size to have any useful effect.  To
  disable trimming completely, you can set to (unsigned long)(-1);

  Trim settings interact with fastbin (MXFAST) settings:
  Automatic trimming never takes place upon freeing a chunk
  with size less than or equal to MXFAST. Trimming is instead
  delayed until subsequent freeing of larger chunks. However, you
  can still force an attempted trim by calling malloc_trim.

  Also, trimming is not generally possible in cases where
  the main arena is obtained via mmap.

*/


#ifndef DEFAULT_TRIM_THRESHOLD
#define DEFAULT_TRIM_THRESHOLD (128 * 1024)
#endif


/*
  M_TOP_PAD is the amount of extra `padding' space to allocate or
  retain whenever sbrk is called. It is used in two ways internally:

  * When sbrk is called to extend the top of the arena to satisfy
  a new malloc request, this much padding is added to the sbrk
  request.

  * When malloc_trim is called automatically from free(),
  it is used as the `pad' argument.

  In both cases, the actual amount of padding is rounded
  so that the end of the arena is always a system page boundary.

  The main reason for using padding is to avoid calling sbrk so
  often. Having even a small pad greatly reduces the likelihood
  that nearly every malloc request during program start-up (or
  after trimming) will invoke sbrk, which needlessly wastes
  time.

  Automatic rounding-up to page-size units is normally sufficient
  to avoid measurable overhead, so the default is 0.  However, in
  systems where sbrk is relatively slow, it can pay to increase
  this value, at the expense of carrying around more memory than
  the program needs.

*/

#ifndef DEFAULT_TOP_PAD
#define DEFAULT_TOP_PAD        (0)
#endif

/*

  M_MMAP_THRESHOLD is the request size threshold for using mmap()
  to service a request. Requests of at least this size that cannot
  be allocated using already-existing space will be serviced via mmap.
  (If enough normal freed space already exists it is used instead.)

  Using mmap segregates relatively large chunks of memory so that
  they can be individually obtained and released from the host
  system. A request serviced through mmap is never reused by any
  other request (at least not directly; the system may just so
  happen to remap successive requests to the same locations).

  Segregating space in this way has the benefit that mmapped space
  can ALWAYS be individually released back to the system, which
  helps keep the system level memory demands of a long-lived
  program low. Mapped memory can never become `locked' between
  other chunks, as can happen with normally allocated chunks, which
  menas that even trimming via malloc_trim would not release them.

  However, it has the disadvantages that:

   1. The space cannot be reclaimed, consolidated, and then
      used to service later requests, as happens with normal chunks.
   2. It can lead to more wastage because of mmap page alignment
      requirements
   3. It causes malloc performance to be more dependent on host
      system memory management support routines which may vary in
      implementation quality and may impose arbitrary
      limitations. Generally, servicing a request via normal
      malloc steps is faster than going through a system's mmap.

  All together, these considerations should lead you to use mmap
  only for relatively large requests.

*/


#ifndef DEFAULT_MMAP_THRESHOLD
#define DEFAULT_MMAP_THRESHOLD (128 * 1024)
#endif

/*
  M_MMAP_MAX is the maximum number of requests to simultaneously
  service using mmap. This parameter exists because:

  1. Some systems have a limited number of internal tables for
     use by mmap.
  2. In most systems, overreliance on mmap can degrade overall
     performance.
  3. If a program allocates many large regions, it is probably
     better off using normal sbrk-based allocation routines that
     can reclaim and reallocate normal heap memory. 

  Setting to 0 disables use of mmap for servicing large requests.  If
  HAVE_MMAP is not set, the default value is 0, and attempts to set it
  to non-zero values in mallopt will fail.
*/



#ifndef DEFAULT_MMAP_MAX
#if HAVE_MMAP
#define DEFAULT_MMAP_MAX       (256)
#else
#define DEFAULT_MMAP_MAX       (0)
#endif
#endif


/*
  MORECORE-related declarations. By default, rely on sbrk
*/


#ifdef LACKS_UNISTD_H
#if __STD_C
extern Void_t*     sbrk(ptrdiff_t);
#else
extern Void_t*     sbrk();
#endif
#endif

/*
  MORECORE_FAILURE is the value returned upon failure of MORECORE
  as well as mmap. Since it cannot be an otherwise valid memory address,
  and must reflect values of standard sys calls, you probably ought not
  try to redefine it.
*/

#define MORECORE_FAILURE -1

/*
  MORECORE is The name of the routine to call to obtain more memory
  from the system.  See below for general guidance on writing
  alternative MORECORE functions, as well as a version for WIN32 and a
  sample version for pre-OSX macos.

*/

#ifndef MORECORE
#define MORECORE sbrk
#endif

/*
  If MORECORE_CONTIGUOUS is true, take advantage of fact that
  consecutive calls to MORECORE with positive arguments always return
  contiguous increasing addresses.  This is true of unix sbrk.  Even
  if not defined, when regions happen to be contiguous, malloc will
  permit allocations spanning regions obtained from different
  calls. But defining this when applicable enables some stronger
  consistency checks and space efficiencies.
*/


#ifndef MORECORE_CONTIGUOUS
#define MORECORE_CONTIGUOUS 1
#endif


/*
  The system page size. To the extent possible, this malloc manages
  memory from the system in page-size units.  Note that this value is
  cached during initialization into a field of malloc_state. So even
  if it is a function, it is only called once.

  The following mechanics for getpagesize were adapted from bsd/gnu
  getpagesize.h. If none of the system-probes here apply, a value of
  4096 is used, which should be OK: If they don't apply, then using
  the actual value probably doesn't impact performance.
*/

#ifndef malloc_getpagesize

#ifndef LACKS_UNISTD_H
#  include <unistd.h>
#endif

#  ifdef _SC_PAGESIZE         /* some SVR4 systems omit an underscore */
#    ifndef _SC_PAGE_SIZE
#      define _SC_PAGE_SIZE _SC_PAGESIZE
#    endif
#  endif

#  ifdef _SC_PAGE_SIZE
#    define malloc_getpagesize sysconf(_SC_PAGE_SIZE)
#  else
#    if defined(BSD) || defined(DGUX) || defined(HAVE_GETPAGESIZE)
       extern size_t getpagesize();
#      define malloc_getpagesize getpagesize()
#    else
#      ifdef WIN32
#        define malloc_getpagesize (4096) 
#      else
#        ifndef LACKS_SYS_PARAM_H
#          include <sys/param.h>
#        endif
#        ifdef EXEC_PAGESIZE
#          define malloc_getpagesize EXEC_PAGESIZE
#        else
#          ifdef NBPG
#            ifndef CLSIZE
#              define malloc_getpagesize NBPG
#            else
#              define malloc_getpagesize (NBPG * CLSIZE)
#            endif
#          else
#            ifdef NBPC
#              define malloc_getpagesize NBPC
#            else
#              ifdef PAGESIZE
#                define malloc_getpagesize PAGESIZE
#              else /* just guess */
#                define malloc_getpagesize (4096) 
#              endif
#            endif
#          endif
#        endif
#      endif
#    endif
#  endif
#endif


/* Two-phase Name mangling */

#ifndef USE_PUBLIC_MALLOC_WRAPPERS
#define cALLOc      public_cALLOc
#define fREe        public_fREe
#define cFREe       public_cFREe
#define mALLOc      public_mALLOc
#define mEMALIGn    public_mEMALIGn
#define rEALLOc     public_rEALLOc
#define vALLOc      public_vALLOc
#define pVALLOc     public_pVALLOc
#define mALLINFo    public_mALLINFo
#define mALLOPt     public_mALLOPt
#define mTRIm       public_mTRIm
#define mSTATs      public_mSTATs
#define mUSABLe     dlmalloc_usable_size  /* public_mUSABLe  (EDB) */
#endif


#ifdef USE_DL_PREFIX
#define public_cALLOc    dlcalloc
#define public_fREe      dlfree
#define public_cFREe     dlcfree
#define public_mALLOc    dlmalloc
#define public_mEMALIGn  dlmemalign
#define public_rEALLOc   dlrealloc
#define public_vALLOc    dlvalloc
#define public_pVALLOc   dlpvalloc
#define public_mALLINFo  dlmallinfo
#define public_mALLOPt   dlmallopt
#define public_mTRIm     dlmalloc_trim
#define public_mSTATs    dlmalloc_stats
#define public_mUSABLe   dlmalloc_usable_size
#else /* USE_DL_PREFIX */
#define public_cALLOc    calloc
#define public_fREe      free
#define public_cFREe     cfree
#define public_mALLOc    malloc
#define public_mEMALIGn  memalign
#define public_rEALLOc   realloc
#define public_vALLOc    valloc
#define public_pVALLOc   pvalloc
#define public_mALLINFo  mallinfo
#define public_mALLOPt   mallopt
#define public_mTRIm     malloc_trim
#define public_mSTATs    malloc_stats
#define public_mUSABLe   malloc_usable_size
#endif /* USE_DL_PREFIX */

#if __STD_C

Void_t* public_mALLOc(size_t);
void    public_fREe(Void_t*);
Void_t* public_rEALLOc(Void_t*, size_t);
Void_t* public_mEMALIGn(size_t, size_t);
Void_t* public_vALLOc(size_t);
Void_t* public_pVALLOc(size_t);
Void_t* public_cALLOc(size_t, size_t);
void    public_cFREe(Void_t*);
int     public_mTRIm(size_t);
size_t  public_mUSABLe(Void_t*);
void    public_mSTATs();
int     public_mALLOPt(int, int);
struct mallinfo public_mALLINFo(void);
#else
Void_t* public_mALLOc();
void    public_fREe();
Void_t* public_rEALLOc();
Void_t* public_mEMALIGn();
Void_t* public_vALLOc();
Void_t* public_pVALLOc();
Void_t* public_cALLOc();
void    public_cFREe();
int     public_mTRIm();
size_t  public_mUSABLe();
void    public_mSTATs();
int     public_mALLOPt();
struct mallinfo public_mALLINFo();
#endif


#ifdef __cplusplus
};  /* end of extern "C" */
#endif



/* ---------- To make a malloc.h, end cutting here ------------ */


/* Declarations of internal utilities defined below  */



#if __STD_C
static Void_t* sYSMALLOc(INTERNAL_SIZE_T nb);
#else
static Void_t* sYSMALLOc();
#endif

static void malloc_consolidate();

#ifdef USE_PUBLIC_MALLOC_WRAPPERS
#if __STD_C

static Void_t* mALLOc(size_t);
static void    fREe(Void_t*);
static Void_t* rEALLOc(Void_t*, size_t);
static Void_t* mEMALIGn(size_t, size_t);
static Void_t* vALLOc(size_t);
static Void_t* pVALLOc(size_t);
static Void_t* cALLOc(size_t, size_t);
static void    cFREe(Void_t*);
static int     mTRIm(size_t);
static size_t  mUSABLe(Void_t*);
static void    mSTATs();
static int     mALLOPt(int, int);
static struct mallinfo mALLINFo(void);
#else
static Void_t* mALLOc();
static void    fREe();
static Void_t* rEALLOc();
static Void_t* mEMALIGn();
static Void_t* vALLOc();
static Void_t* pVALLOc();
static Void_t* cALLOc();
static void    cFREe();
static int     mTRIm();
static size_t  mUSABLe();
static void    mSTATs();
static int     mALLOPt();
static struct mallinfo mALLINFo();
#endif
#endif



/* ---------- public wrappers --------------- */

#ifdef USE_PUBLIC_MALLOC_WRAPPERS

/*
  MALLOC_PREACTION and MALLOC_POSTACTION should be
  defined to return 0 on success, and nonzero on failure.
  The return value of MALLOC_POSTACTION is currently ignored
  in wrapper functions since there is no reasonable default
  action to take on failure.
*/


#ifdef USE_PTHREAD_LOCKS

#include <pthread.h>

static pthread_mutex_t mALLOC_MUTEx = PTHREAD_MUTEX_INITIALIZER;

#define MALLOC_PREACTION   pthread_mutex_lock(&mALLOC_MUTEx)
#define MALLOC_POSTACTION  pthread_mutex_unlock(&mALLOC_MUTEx)

#else

/* Substitute anything you like for these */

#define MALLOC_PREACTION   (0)
#define MALLOC_POSTACTION  (0)

#endif

Void_t* public_mALLOc(size_t bytes) {
  Void_t* m;
  if (MALLOC_PREACTION != 0) {
    return 0;
  }
  m = mALLOc(bytes);
  if (MALLOC_POSTACTION != 0) {
  }
  return m;
}

void public_fREe(Void_t* m) {
  if (MALLOC_PREACTION != 0) {
    return;
  }
  fREe(m);
  if (MALLOC_POSTACTION != 0) {
  }
}

Void_t* public_rEALLOc(Void_t* m, size_t bytes) {
  if (MALLOC_PREACTION != 0) {
    return 0;
  }
  m = rEALLOc(m, bytes);
  if (MALLOC_POSTACTION != 0) {
  }
  return m;
}

Void_t* public_mEMALIGn(size_t alignment, size_t bytes) {
  Void_t* m;
  if (MALLOC_PREACTION != 0) {
    return 0;
  }
  m = mEMALIGn(alignment, bytes);
  if (MALLOC_POSTACTION != 0) {
  }
  return m;
}

Void_t* public_vALLOc(size_t bytes) {
  Void_t* m;
  if (MALLOC_PREACTION != 0) {
    return 0;
  }
  m = vALLOc(bytes);
  if (MALLOC_POSTACTION != 0) {
  }
  return m;
}

Void_t* public_pVALLOc(size_t bytes) {
  Void_t* m;
  if (MALLOC_PREACTION != 0) {
    return 0;
  }
  m = pVALLOc(bytes);
  if (MALLOC_POSTACTION != 0) {
  }
  return m;
}

Void_t* public_cALLOc(size_t n, size_t elem_size) {
  Void_t* m;
  if (MALLOC_PREACTION != 0) {
    return 0;
  }
  m = cALLOc(n, elem_size);
  if (MALLOC_POSTACTION != 0) {
  }
  return m;
}

void public_cFREe(Void_t* m) {
  if (MALLOC_PREACTION != 0) {
    return;
  }
  cFREe(m);
  if (MALLOC_POSTACTION != 0) {
  }
}

int public_mTRIm(size_t s) {
  int result;
  if (MALLOC_PREACTION != 0) {
    return 0;
  }
  result = mTRIm(s);
  if (MALLOC_POSTACTION != 0) {
  }
  return result;
}


size_t public_mUSABLe(Void_t* m) {
  size_t result;
  if (MALLOC_PREACTION != 0) {
    return 0;
  }
  result = mUSABLe(m);
  if (MALLOC_POSTACTION != 0) {
  }
  return result;
}


void public_mSTATs() {
  if (MALLOC_PREACTION != 0) {
    return;
  }
  mSTATs();
  if (MALLOC_POSTACTION != 0) {
  }
}

struct mallinfo public_mALLINFo() {
  struct mallinfo m;
  if (MALLOC_PREACTION != 0) {
    return m;
  }
  m = mALLINFo();
  if (MALLOC_POSTACTION != 0) {
  }
  return m;
}

int public_mALLOPt(int p, int v) {
  int result;
  if (MALLOC_PREACTION != 0) {
    return 0;
  }
  result = mALLOPt(p, v);
  if (MALLOC_POSTACTION != 0) {
  }
  return result;
}

#endif



/* ------------- Optional versions of memcopy ---------------- */


#if USE_MEMCPY

#define MALLOC_COPY(dest, src, nbytes)  memcpy(dest, src, nbytes)
#define MALLOC_ZERO(dest, nbytes)       memset(dest, 0,   nbytes)

#else /* !USE_MEMCPY */

/* Use Duff's device for good zeroing/copying performance. */

#define MALLOC_ZERO(charp, nbytes)                                            \
do {                                                                          \
  INTERNAL_SIZE_T* mzp = (INTERNAL_SIZE_T*)(charp);                           \
  long mctmp = (nbytes)/sizeof(INTERNAL_SIZE_T), mcn;                         \
  if (mctmp < 8) mcn = 0; else { mcn = (mctmp-1)/8; mctmp %= 8; }             \
  switch (mctmp) {                                                            \
    case 0: for(;;) { *mzp++ = 0;                                             \
    case 7:           *mzp++ = 0;                                             \
    case 6:           *mzp++ = 0;                                             \
    case 5:           *mzp++ = 0;                                             \
    case 4:           *mzp++ = 0;                                             \
    case 3:           *mzp++ = 0;                                             \
    case 2:           *mzp++ = 0;                                             \
    case 1:           *mzp++ = 0; if(mcn <= 0) break; mcn--; }                \
  }                                                                           \
} while(0)

#define MALLOC_COPY(dest,src,nbytes)                                          \
do {                                                                          \
  INTERNAL_SIZE_T* mcsrc = (INTERNAL_SIZE_T*) src;                            \
  INTERNAL_SIZE_T* mcdst = (INTERNAL_SIZE_T*) dest;                           \
  long mctmp = (nbytes)/sizeof(INTERNAL_SIZE_T), mcn;                         \
  if (mctmp < 8) mcn = 0; else { mcn = (mctmp-1)/8; mctmp %= 8; }             \
  switch (mctmp) {                                                            \
    case 0: for(;;) { *mcdst++ = *mcsrc++;                                    \
    case 7:           *mcdst++ = *mcsrc++;                                    \
    case 6:           *mcdst++ = *mcsrc++;                                    \
    case 5:           *mcdst++ = *mcsrc++;                                    \
    case 4:           *mcdst++ = *mcsrc++;                                    \
    case 3:           *mcdst++ = *mcsrc++;                                    \
    case 2:           *mcdst++ = *mcsrc++;                                    \
    case 1:           *mcdst++ = *mcsrc++; if(mcn <= 0) break; mcn--; }       \
  }                                                                           \
} while(0)

#endif

/* ------------------ MMAP support ------------------  */


#if HAVE_MMAP

#include <fcntl.h>
#include <sys/mman.h>

#if !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
#define MAP_ANONYMOUS MAP_ANON
#endif


/* 
   Nearly all versions of mmap support MAP_ANONYMOUS, 
   so the following is unlikely to be needed, but is
   supplied just in case.
*/

#ifndef MAP_ANONYMOUS

static int dev_zero_fd = -1; /* Cached file descriptor for /dev/zero. */

#define MMAP(addr, size, prot, flags) ((dev_zero_fd < 0) ? \
 (dev_zero_fd = open("/dev/zero", O_RDWR), \
  mmap((addr), (size), (prot), (flags), dev_zero_fd, 0)) : \
   mmap((addr), (size), (prot), (flags), dev_zero_fd, 0))

#else

#define MMAP(addr, size, prot, flags) \
 (mmap((addr), (size), (prot), (flags)|MAP_ANONYMOUS, -1, 0))

#endif

#endif /* HAVE_MMAP */


/* ---------- Alternative MORECORE functions ------------ */


/*
  General Requirements for MORECORE.

  The MORECORE function must have the following properties:

  If MORECORE_CONTIGUOUS is false:

    * MORECORE must allocate in multiples of pagesize. It will
      only be called with arguments that are multiples of pagesize.

    * MORECORE must page-align. That is, MORECORE(0) must
      return an address at a page boundary.

  else (i.e. If MORECORE_CONTIGUOUS is true):

    * Consecutive calls to MORECORE with positive arguments
      return increasing addresses, indicating that space has been
      contiguously extended.

    * MORECORE need not allocate in multiples of pagesize.
      Calls to MORECORE need not have args of multiples of pagesize.

    * MORECORE need not page-align.

  In either case:

    * MORECORE must not allocate memory when given argument zero, but
      instead return one past the end address of memory from previous
      nonzero call. This malloc does NOT call MORECORE(0)
      until at least one call with positive arguments is made, so
      the initial value returned is not important.

    * Even though consecutive calls to MORECORE need not return contiguous
      addresses, it must be OK for malloc'ed chunks to span multiple
      regions in those cases where they do happen to be contiguous.

    * MORECORE need not handle negative arguments -- it may instead
      just return MORECORE_FAILURE when given negative arguments.
      Negative arguments are always multiples of pagesize. However,
      it must not misinterpret negative args as large positive unsigned
      args.

  There is some variation across systems about the type of the
  argument to sbrk/MORECORE. If size_t is unsigned, then it cannot
  actually be size_t, because sbrk supports negative args, so it is
  normally the signed type of the same width as size_t (sometimes
  declared as "intptr_t", and sometimes "ptrdiff_t").  It doesn't much
  matter though. Internally, we use "long" as arguments, which should
  work across all reasonable possibilities.

  Additionally, if MORECORE ever returns failure for a positive
  request, and HAVE_MMAP is true, then mmap is used as a noncontiguous
  system allocator. This is a useful backup strategy for systems with
  holes in address spaces -- in this case sbrk cannot contiguously
  expand the heap, but mmap may be able to map noncontiguous space.
  If you'd like mmap to ALWAYS be used, you can define MORECORE to be
  a function that always returns MORECORE_FAILURE.

  If you are using this malloc with something other than unix sbrk to
  supply memory regions, you probably want to set MORECORE_CONTIGUOUS
  as false.  As an example, here is a custom allocator kindly
  contributed for pre-OSX macOS.  It uses virtually but not
  necessarily physically contiguous non-paged memory (locked in,
  present and won't get swapped out).  You can use it by uncommenting
  this section, adding some #includes, and setting up the appropriate
  defines above:

      #define MORECORE osMoreCore
      #define MORECORE_CONTIGUOUS 0

  There is also a shutdown routine that should somehow be called for
  cleanup upon program exit.

  #define MAX_POOL_ENTRIES 100
  #define MINIMUM_MORECORE_SIZE  (64 * 1024)
  static int next_os_pool;
  void *our_os_pools[MAX_POOL_ENTRIES];

  void *osMoreCore(int size)
  {
    void *ptr = 0;
    static void *sbrk_top = 0;

    if (size > 0)
    {
      if (size < MINIMUM_MORECORE_SIZE)
         size = MINIMUM_MORECORE_SIZE;
      if (CurrentExecutionLevel() == kTaskLevel)
         ptr = PoolAllocateResident(size + RM_PAGE_SIZE, 0);
      if (ptr == 0)
      {
        return (void *) MORECORE_FAILURE;
      }
      // save ptrs so they can be freed during cleanup
      our_os_pools[next_os_pool] = ptr;
      next_os_pool++;
      ptr = (void *) ((((unsigned long) ptr) + RM_PAGE_MASK) & ~RM_PAGE_MASK);
      sbrk_top = (char *) ptr + size;
      return ptr;
    }
    else if (size < 0)
    {
      // we don't currently support shrink behavior
      return (void *) MORECORE_FAILURE;
    }
    else
    {
      return sbrk_top;
    }
  }

  // cleanup any allocated memory pools
  // called as last thing before shutting down driver

  void osCleanupMem(void)
  {
    void **ptr;

    for (ptr = our_os_pools; ptr < &our_os_pools[MAX_POOL_ENTRIES]; ptr++)
      if (*ptr)
      {
         PoolDeallocate(*ptr);
         *ptr = 0;
      }
  }

*/


/*
  Emulation of sbrk for WIN32
  All code within the ifdef WIN32 is untested by me.

  Thanks to Anonymous and others for supplying this.
*/


#ifdef WIN32

#define AlignPage(add) (((add) + (malloc_getpagesize-1)) & \
~(malloc_getpagesize-1))
#define AlignPage64KB(add) (((add) + (0x10000 - 1)) & ~(0x10000 - 1))

/* resrve 64MB to insure large contiguous space */
#define RESERVED_SIZE (1024*1024*64)
#define NEXT_SIZE (2048*1024)
#define TOP_MEMORY ((unsigned long)2*1024*1024*1024)

struct GmListElement;
typedef struct GmListElement GmListElement;

struct GmListElement
{
  GmListElement* next;
  void* base;
};

static GmListElement* head = 0;
static unsigned int gNextAddress = 0;
static unsigned int gAddressBase = 0;
static unsigned int gAllocatedSize = 0;

static
GmListElement* makeGmListElement(void* bas)
{
  GmListElement* this;
  this = (GmListElement*)(void*)LocalAlloc (0, sizeof (GmListElement));
  assert (this);
  if (this) {
    this->base = bas;
    this->next = head;
    head = this;
  }
  return this;
}

void gcleanup()
{
  BOOL rval;
  assert ( (head == NULL) || (head->base == (void*)gAddressBase));
  if (gAddressBase && (gNextAddress - gAddressBase)) {
    rval = VirtualFree ((void*)gAddressBase,
                        gNextAddress - gAddressBase,
                        MEM_DECOMMIT);
    assert (rval);
  }
  while (head) {
    GmListElement* next = head->next;
    rval = VirtualFree (head->base, 0, MEM_RELEASE);
    assert (rval);
    LocalFree (head);
    head = next;
  }
}

static void* findRegion(void* start_address, unsigned long size)
{
  MEMORY_BASIC_INFORMATION info;
  if (size >= TOP_MEMORY) return NULL;

  while ((unsigned long)start_address + size < TOP_MEMORY) {
    VirtualQuery (start_address, &info, sizeof (info));
    if ((info.State == MEM_FREE) && (info.RegionSize >= size))
      return start_address;
    else {
      /*
        Requested region is not available so see if the
        next region is available.  Set 'start_address'
        to the next region and call 'VirtualQuery()'
        again.
      */

      start_address = (char*)info.BaseAddress + info.RegionSize;

      /*
        The subsequent call to 'VirtualAlloc(...,
        MEM_RESERVE, ...)' in 'wsbrk()' may fail
        regardless of what 'VirtualQuery()' reports
        since 'VirtualAlloc(..., MEM_RESERVE, ...)'
        will round down the region's address to a 64KB
        page boundary which may not be available.  So,
        to avoid the rounding, make sure the next
        region to query is aligned on a 64KB page
        boundary.
      */

      start_address =
        (void *) AlignPage64KB((unsigned long) start_address);
    }
  }
  return NULL;

}


void* wsbrk (long size)
{
  void* tmp;
  if (size > 0) {
    if (gAddressBase == 0) {
      /*
        Since 'VirtualAlloc(..., MEM_RESERVE, ...)'
        reserves everything on a 64KB page boundary,
        align the reservation size on a 64KB page
        boundary so that all subsequent reservations
        are made as contiguous as possible.

        Additionally, if we don't align the reservation
        size on a 64KB page boundary, a subsequent
        reservation may start on a non-64KB page
        boundary.  Though the call to 'VirtualQuery()'
        in 'findRegion()' may consider this address to
        be available, the subsequent call to
        'VirtualAlloc(..., MEM_RESERVE, ...)' will
        round down the address to a 64KB page boundary
        which may not be available.
      */
      gAllocatedSize = AlignPage64KB (max (RESERVED_SIZE, size));
      gNextAddress = gAddressBase =
        (unsigned int)VirtualAlloc (NULL,
                                    gAllocatedSize, MEM_RESERVE,
                                    PAGE_NOACCESS);
    }
    else if (AlignPage (gNextAddress + size) > (gAddressBase +
                                                gAllocatedSize)) {
      /*
        Align the reservation size on a 64KB page
        boundary.  See commentary on previous call to
        'AlignPage64KB()' for further details.
      */

      long new_size = AlignPage64KB (max (NEXT_SIZE, size));
      void* new_address = (void*)(gAddressBase+gAllocatedSize);
      do {
        new_address = findRegion (new_address, new_size);

        if (new_address == 0)
          return (void*)MORECORE_FAILURE;

        gAddressBase = gNextAddress =
          (unsigned int)VirtualAlloc (new_address,
                                      new_size, MEM_RESERVE,
                                      PAGE_NOACCESS);
                                /*
                                  repeat in case of race condition
                                  The region that we found has been snagged
                                  by another thread
                                */
      }
      while (gAddressBase == 0);

      assert (new_address == (void*)gAddressBase);

      gAllocatedSize = new_size;

      if (!makeGmListElement ((void*)gAddressBase))
        return (void*)MORECORE_FAILURE;
    }
    if ((size + gNextAddress) > AlignPage (gNextAddress)) {
      void* res;
      res = VirtualAlloc ((void*)AlignPage (gNextAddress),
                          (size + gNextAddress -
                           AlignPage (gNextAddress)),
                          MEM_COMMIT, PAGE_READWRITE);
      if (res == 0)
        return (void*)MORECORE_FAILURE;
    }
    tmp = (void*)gNextAddress;
    gNextAddress = (unsigned int)tmp + size;
                return tmp;
  }
  else if (size < 0) {
    unsigned int alignedGoal = AlignPage (gNextAddress + size);
    /* Trim by releasing the virtual memory */
    if (alignedGoal >= gAddressBase) {
      VirtualFree ((void*)alignedGoal, gNextAddress - alignedGoal,
                   MEM_DECOMMIT);
      gNextAddress = gNextAddress + size;
      return (void*)gNextAddress;
    }
    else {
      VirtualFree ((void*)gAddressBase, gNextAddress - gAddressBase,
                   MEM_DECOMMIT);
      gNextAddress = gAddressBase;
      return (void*)MORECORE_FAILURE;
    }
  }
  else {
    return (void*)gNextAddress;
  }
}

#endif



/*
  -----------------------  Chunk representations -----------------------
*/

struct malloc_chunk {

  INTERNAL_SIZE_T      prev_size;  /* Size of previous chunk (if free).  */
  INTERNAL_SIZE_T      size;       /* Size in bytes, including overhead. */

  struct malloc_chunk* fd;         /* double links -- used only if free. */
  struct malloc_chunk* bk;
};


typedef struct malloc_chunk* mchunkptr;

/*

   malloc_chunk details:

    (The following includes lightly edited explanations by Colin Plumb.)

    Chunks of memory are maintained using a `boundary tag' method as
    described in e.g., Knuth or Standish.  (See the paper by Paul
    Wilson ftp://ftp.cs.utexas.edu/pub/garbage/allocsrv.ps for a
    survey of such techniques.)  Sizes of free chunks are stored both
    in the front of each chunk and at the end.  This makes
    consolidating fragmented chunks into bigger chunks very fast.  The
    size fields also hold bits representing whether chunks are free or
    in use.

    An allocated chunk looks like this:


    chunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Size of previous chunk, if allocated            | |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Size of chunk, in bytes                         |P|
      mem-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             User data starts here...                          .
            .                                                               .
            .             (malloc_usable_space() bytes)                     .
            .                                                               |
nextchunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Size of chunk                                     |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


    Where "chunk" is the front of the chunk for the purpose of most of
    the malloc code, but "mem" is the pointer that is returned to the
    user.  "Nextchunk" is the beginning of the next contiguous chunk.

    Chunks always begin on even word boundries, so the mem portion
    (which is returned to the user) is also on an even word boundary, and
    thus double-word aligned.

    Free chunks are stored in circular doubly-linked lists, and look like this:

    chunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Size of previous chunk                            |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    `head:' |             Size of chunk, in bytes                         |P|
      mem-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Forward pointer to next chunk in list             |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Back pointer to previous chunk in list            |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            |             Unused space (may be 0 bytes long)                .
            .                                                               .
            .                                                               |
nextchunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    `foot:' |             Size of chunk, in bytes                           |
            +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    The P (PREV_INUSE) bit, stored in the unused low-order bit of the
    chunk size (which is always a multiple of two words), is an in-use
    bit for the *previous* chunk.  If that bit is *clear*, then the
    word before the current chunk size contains the previous chunk
    size, and can be used to find the front of the previous chunk.
    The very first chunk allocated always has this bit set,
    preventing access to non-existent (or non-owned) memory. If
    prev_inuse is set for any given chunk, then you CANNOT determine
    the size of the previous chunk, and might even get a memory
    addressing fault when trying to do so.

    Note that the `foot' of the current chunk is actually represented
    as the prev_size of the NEXT chunk. (This makes it easier to
    deal with alignments etc).

    The two exceptions to all this are

     1. The special chunk `top' doesn't bother using the
        trailing size field since there is no next contiguous chunk
        that would have to index off it. After initialization, `top'
        is forced to always exist.  If it would become less than
        MINSIZE bytes long, it is replenished.

     2. Chunks allocated via mmap, which have the second-lowest-order
        bit (IS_MMAPPED) set in their size fields.  Because they are
        never merged or traversed from any other chunk, they have no
        foot size or inuse information.

*/



/*  sizes, alignments */

#define SIZE_SZ                (sizeof(INTERNAL_SIZE_T))
#define MALLOC_ALIGNMENT       (SIZE_SZ + SIZE_SZ)
#define MALLOC_ALIGN_MASK      (MALLOC_ALIGNMENT - 1)
#define MINSIZE                (sizeof(struct malloc_chunk))


/* Check if m has acceptable alignment */

#define aligned_OK(m)    (((unsigned long)((m)) & (MALLOC_ALIGN_MASK)) == 0)

/* conversion from malloc headers to user pointers, and back */

#define chunk2mem(p)   ((Void_t*)((char*)(p) + 2*SIZE_SZ))
#define mem2chunk(mem) ((mchunkptr)((char*)(mem) - 2*SIZE_SZ))


/* pad request bytes into a usable size */


#define request2size(req)                                          \
  (((req) < (MINSIZE - ((SIZE_SZ + MALLOC_ALIGN_MASK)))) ?         \
    MINSIZE :                                                      \
   ((req) + (SIZE_SZ + MALLOC_ALIGN_MASK)) & ~MALLOC_ALIGN_MASK)


/*
   Same, except check for negative/huge arguments.
   This lets through args that are positive but wrap into
   negatives when padded. However, these are trapped
   elsewhere.

*/

#define checked_request2size(req, sz) \
  if ((long)(req) < (long)(MINSIZE - (SIZE_SZ + MALLOC_ALIGN_MASK))) { \
    if ((long)(req) < 0)  {                                            \
      MALLOC_FAILURE_ACTION;                                           \
      return 0;                                                        \
    }                                                                  \
    else                                                               \
      (req) = (MINSIZE - (SIZE_SZ + MALLOC_ALIGN_MASK));               \
  }                                                                    \
  (sz) = ((req) + (SIZE_SZ + MALLOC_ALIGN_MASK)) & ~MALLOC_ALIGN_MASK;




/*
  Physical chunk operations
*/


/* size field is or'ed with PREV_INUSE when previous adjacent chunk in use */

#define PREV_INUSE 0x1

/* size field is or'ed with IS_MMAPPED if the chunk was obtained with mmap() */

#define IS_MMAPPED 0x2

/* Bits to mask off when extracting size */

#define SIZE_BITS (PREV_INUSE|IS_MMAPPED)


/* Ptr to next physical malloc_chunk. */

#define next_chunk(p) ((mchunkptr)( ((char*)(p)) + ((p)->size & ~PREV_INUSE) ))

/* Ptr to previous physical malloc_chunk */

#define prev_chunk(p) ((mchunkptr)( ((char*)(p)) - ((p)->prev_size) ))


/* Treat space at ptr + offset as a chunk */

#define chunk_at_offset(p, s)  ((mchunkptr)(((char*)(p)) + (s)))




/*
  Dealing with use bits

  Note: IS_MMAPPED is intentionally not masked off from size field in
  macros for which mmapped chunks should never be seen. This should
  cause helpful core dumps to occur if it is tried by accident by
  people extending or adapting this malloc.

*/


/* extract p's inuse bit */

#define inuse(p)\
((((mchunkptr)(((char*)(p))+((p)->size & ~PREV_INUSE)))->size) & PREV_INUSE)

/* extract inuse bit of previous chunk */

#define prev_inuse(p)       ((p)->size & PREV_INUSE)

/* check for mmap()'ed chunk */

#define chunk_is_mmapped(p) ((p)->size & IS_MMAPPED)

/* set/clear chunk as being inuse without otherwise disturbing */

#define set_inuse(p)\
((mchunkptr)(((char*)(p)) + ((p)->size & ~PREV_INUSE)))->size |= PREV_INUSE

#define clear_inuse(p)\
((mchunkptr)(((char*)(p)) + ((p)->size & ~PREV_INUSE)))->size &= ~(PREV_INUSE)

/* check/set/clear inuse bits in known places */

#define inuse_bit_at_offset(p, s)\
 (((mchunkptr)(((char*)(p)) + (s)))->size & PREV_INUSE)

#define set_inuse_bit_at_offset(p, s)\
 (((mchunkptr)(((char*)(p)) + (s)))->size |= PREV_INUSE)

#define clear_inuse_bit_at_offset(p, s)\
 (((mchunkptr)(((char*)(p)) + (s)))->size &= ~(PREV_INUSE))




/*
  Dealing with size fields
*/

/* Get size, ignoring use bits */

#define chunksize(p)         ((p)->size & ~(SIZE_BITS))

/* Set size at head, without disturbing its use bit */

#define set_head_size(p, s)  ((p)->size = (((p)->size & PREV_INUSE) | (s)))

/* Set size/use field */

#define set_head(p, s)       ((p)->size = (s))

/* Set size at footer (only when chunk is not in use) */

#define set_foot(p, s)       (((mchunkptr)((char*)(p) + (s)))->prev_size = (s))





/*
  ------------------  Internal data structures --------------------

   All internal state is held in an instance of malloc_state defined
   below. There are no other static variables, except in two optional
   cases: 
   * If USE_PTHREAD_LOCKS is defined, the mALLOC_MUTEx declared
      above. 
   * If HAVE_MMAP is true, but mmap doesn't support
     MAP_ANONYMOUS, a dummy file descriptor for mmap.

   Beware of lots of tricks that minimize the total space
   requirements. The result is a little over 1K bytes (for 4byte
   pointers and size_t.)

*/

/*

  Bins

    An array of bin headers for consolidated chunks. Each bin is
    doubly linked.  The bins are approximately proportionally (log)
    spaced.  There are a lot of these bins (128). This may look
    excessive, but works very well in practice.  All procedures
    maintain the invariant that no consolidated chunk physically
    borders another one, so each chunk in a list is known to be
    preceeded and followed by either inuse chunks or the ends of
    memory.

    Chunks in bins are kept in size order, with ties going to the
    approximately least recently used chunk. Ordering is irrelevant
    for the small bins, which all contain the same-sized chunks, but
    facilitates best-fit allocation for larger chunks. (These lists
    are just sequential. Keeping them in order almost never requires
    enough traversal to warrant using fancier ordered data
    structures.)  Chunks of the same size are linked with the most
    recently freed at the front, and allocations are taken from the
    back.  This results in LRU (FIFO) allocation order, which tends
    to give each chunk an equal opportunity to be consolidated with
    adjacent freed chunks, resulting in larger free chunks and less
    fragmentation.

    To simplify use in double-linked lists, each bin header acts
    as a malloc_chunk. This avoids special-casing for headers.
    But to conserve space and (mainly) improve locality, we allocate
    only the fd/bk pointers of bins, and then use repositioning tricks
    to treat these as the fields of a malloc_chunk*.  
*/

typedef struct malloc_chunk* mbinptr;

#define NBINS             128


/* addressing -- note that bin_at(0) does not exist */

#define bin_at(m, i)   ((mbinptr) ((char*)&((m)->bins[(i)<<1]) - (SIZE_SZ<<1)))


/* analog of ++bin */

#define next_bin(b)    ((mbinptr)((char*)(b) + (sizeof(mchunkptr)<<1)))


/* Reminders about list directionality within bins */

#define first(b)       ((b)->fd)
#define last(b)        ((b)->bk)

/*
   Take a chunk off a bin list
*/

#define unlink(P, BK, FD) {                                            \
  FD = P->fd;                                                          \
  BK = P->bk;                                                          \
  FD->bk = BK;                                                         \
  BK->fd = FD;                                                         \
}

/*
  Indexing bins

    Bins for sizes < 512 bytes contain chunks of all the same size, spaced
    8 bytes apart. Larger bins are approximately logarithmically spaced:

    64 bins of size       8
    32 bins of size      64
    16 bins of size     512
     8 bins of size    4096
     4 bins of size   32768
     2 bins of size  262144
     1 bin  of size what's left

    There is actually a little bit of slop in the numbers in bin_index
    for the sake of speed. This makes no difference elsewhere.
*/

/* The first NSMALLBIN bins (and fastbins) hold only one size */
#define NSMALLBINS         64
#define SMALLBIN_WIDTH      8
#define MIN_LARGE_SIZE    512

#define in_smallbin_range(sz)  ((sz) <  MIN_LARGE_SIZE)

#define smallbin_index(sz)     (((unsigned)(sz)) >> 3)

#define largebin_index(sz)                                                   \
(((((unsigned long)(sz)) >>  6) <= 32)?  56 + (((unsigned long)(sz)) >>  6): \
 ((((unsigned long)(sz)) >>  9) <= 20)?  91 + (((unsigned long)(sz)) >>  9): \
 ((((unsigned long)(sz)) >> 12) <= 10)? 110 + (((unsigned long)(sz)) >> 12): \
 ((((unsigned long)(sz)) >> 15) <=  4)? 119 + (((unsigned long)(sz)) >> 15): \
 ((((unsigned long)(sz)) >> 18) <=  2)? 124 + (((unsigned long)(sz)) >> 18): \
                                        126)

#define bin_index(sz) \
 ((in_smallbin_range(sz)) ? smallbin_index(sz) : largebin_index(sz))


/*
  Unsorted chunks

    All remainders from chunk splits, as well as all returned chunks,
    are first placed in the unsorted bin. They are then placed
    in regular bins after malloc gives them one chance to be used before
    binning.
*/


/* The otherwise unindexable 1-bin is used to hold unsorted chunks. */

#define unsorted_chunks(M)          (bin_at(M, 1))


/*
  Top

    The top-most available chunk (i.e., the one bordering the end of
    available memory) is treated specially. It is never included in
    any bin, is used only if no other chunk is available, and is
    released back to the system if it is very large (see
    M_TRIM_THRESHOLD).  `top' is never properly linked to its bin
    since it is always handled specially.  Because top initially
    points to its own bin with initial zero size, thus forcing
    extension on the first malloc request, we avoid having any special
    code in malloc to check whether it even exists yet. But we still
    need to do so when getting memory from system, so we make
    initial_top treat the bin as a legal but unusable chunk during the
    interval between initialization and the first call to
    sYSMALLOc. (This is somewhat delicate, since it relies on
    the 2 preceding words to be zero during this interval as well.)
*/


/* Conveniently, the unsorted bin can be used as dummy top on first call */
#define initial_top(M)              (unsorted_chunks(M))

/*
  Binmap

    To help compensate for the large number of bins, a one-level index
    structure is used for bin-by-bin searching.  `binmap' is a
    bitvector recording whether bins are definitely empty so they can
    be skipped over during during traversals.  The bits are NOT always
    cleared as soon as bins are empty, but instead only
    when they are noticed to be empty during traversal in malloc.

*/

/* Conservatively use 32 bits per map word, even if on 64bit system */
#define BINMAPSHIFT      5
#define BITSPERMAP       (1U << BINMAPSHIFT)
#define BINMAPSIZE       (NBINS / BITSPERMAP)

#define idx2block(i)     ((i) >> BINMAPSHIFT)
#define idx2bit(i)       ((1U << ((i) & ((1U << BINMAPSHIFT)-1))))

#define mark_bin(m,i)    ((m)->binmap[idx2block(i)] |=  idx2bit(i))
#define unmark_bin(m,i)  ((m)->binmap[idx2block(i)] &= ~(idx2bit(i)))
#define get_binmap(m,i)  ((m)->binmap[idx2block(i)] &   idx2bit(i))


/*
  Fastbins

    An array of bins holding recently freed small chunks.  Fastbins
    are kept in FIFO order, but are not doubly linked.  It is slightly
    faster to single-link them, and since chunks are never removed from the
    middles of these lists, double linking is not necessary.  While we
    don't bother double linking fastbins, we still want to preserve
    FIFOness, so use a head/tail representation:

*/

struct mfastbin {
  mchunkptr hd;
  mchunkptr tl;
};

typedef struct mfastbin* mfastbinptr;

/* offset 2 to use otherwise unindexable first 2 bins */
#define fastbin_index(sz)        ((((unsigned int)(sz)) >> 3) - 2)

/* The maximum fastbin request size we supprt */
#define MAX_FAST_SIZE     80

#define NFASTBINS  (fastbin_index(request2size(MAX_FAST_SIZE))+1)

/*
  Consolidation flag bit is held in max_fast. It is set true
  whenever fastbins should be consolidated upon next malloc, but
  there is no reason to do so now. The truth value is inverted
  so that it will trigger on first malloc call, and in turn
  trigger an initialization check.
*/

#define need_consolidation(M)       (((M)->max_fast & 1U) == 0)
#define set_consolidation_flag(M)   ((M)->max_fast &= ~1U)
#define clear_consolidation_flag(M) ((M)->max_fast |= 1)

/*
  Threshold value for consolidating fastbins, applied after
  encountering a case where a large free chunk is split, but there are
  at least this many bytes of fastbin chunks, as well as when freeing
  large chunks. This is mainly a safeguard against unchecked
  buildup of fastbin blocks that never become requested again.  Exact
  value is not too important, but should be about a page to lessen the
  chance of unnecessary sbrk.
*/

#define FASTBIN_CONSOLIDATION_THRESHOLD  4096

/*
  NONCONTIGUOUS_REGIONS is a special value for sbrk_base indicating that
  we might have non-contiguous regions. In this case, we do not check
  or assume that all chunks are at addresses >= sbrk_base.  Otherwise,
  contiguity is exploited in merging together, when possible, results
  from consecutive MORECORE calls.

  The possible values for sbrk_base are:
    MORECORE_FAILURE:   
       MORECORE has not yet been called, but we expect contiguous space
    NONCONTIGUOUS_REGIONS: 
       we don't expect contiguous space
    legal addresses: 
       the first address returned by MORECORE when contiguous
*/

#define NONCONTIGUOUS_REGIONS ((char*)(-3))


/*
   ----------- Internal state representation and initialization -----------
*/


struct malloc_state {

  /* The maximum chunk size to be eligible for fastbin */
  INTERNAL_SIZE_T  max_fast;   /* low bit used as consolidation flag */

  /* The total number of bytes of all chunks held in fastbins */
  INTERNAL_SIZE_T  fastbytes;

  /* Base of the topmost chunk -- not otherwise kept in a bin */
  mchunkptr        top;

  /* The remainder from the most recent split of a small request */
  mchunkptr        last_remainder;

  /* Fastbins */
  struct mfastbin  fastbins[NFASTBINS];

  /* Normal bins packed as described above */
  mchunkptr        bins[NBINS * 2];

  /* Bitmap of bins */
  unsigned int     binmap[BINMAPSIZE];

  /* Tunable parameters */
  unsigned long    trim_threshold;
  INTERNAL_SIZE_T  top_pad;
  INTERNAL_SIZE_T  mmap_threshold;

  /* Memory map support */
  int              n_mmaps;
  int              n_mmaps_max;
  int              max_n_mmaps;

  /* Bookkeeping for sbrk */
  unsigned int     pagesize;    /* Cache malloc_getpagesize */
  char*            sbrk_base;   /* first address returned by sbrk, 
                                   or NONCONTIGUOUS_REGIONS */
  /* Statistics */

  INTERNAL_SIZE_T  mmapped_mem;
  INTERNAL_SIZE_T  sbrked_mem;

  INTERNAL_SIZE_T  max_sbrked_mem;
  INTERNAL_SIZE_T  max_mmapped_mem;
  INTERNAL_SIZE_T  max_total_mem;
};

typedef struct malloc_state *mstate;

/* Exactly one instance of this struct in this malloc */

static struct malloc_state av_;  /* never directly referenced */

/*
   All uses of av_ are via get_malloc_state().
   This simplifies construction of multithreaded, etc extensions
*/

#define get_malloc_state() (&(av_))

/*
  Initialize a malloc_state struct.
  This is called only via a check in malloc_consolidate, which needs
  be called on first malloc call anyway.
*/

#if 1 /* __STD_C */
static void malloc_init_state(mstate av)
#else
static void malloc_init_state() mstate av;
#endif
{
  int     i;
  mbinptr bin;

  for (i = 1; i < NBINS; ++i) {
    bin = bin_at(av,i);
    bin->fd = bin->bk = bin;
  }

  av->max_fast       = (DEFAULT_MXFAST == 0)? 0: request2size(DEFAULT_MXFAST);
  av->trim_threshold = DEFAULT_TRIM_THRESHOLD;
  av->top_pad        = DEFAULT_TOP_PAD;
  av->n_mmaps_max    = DEFAULT_MMAP_MAX;
  av->mmap_threshold = DEFAULT_MMAP_THRESHOLD;

  av->top            = initial_top(av);

#if MORECORE_CONTIGUOUS
  av->sbrk_base      = (char*)MORECORE_FAILURE;
#else
  av->sbrk_base      = NONCONTIGUOUS_REGIONS;
#endif

  av->pagesize       = malloc_getpagesize;

  clear_consolidation_flag(av);
}


/*
  Debugging support

  These routines make a number of assertions about the states
  of data structures that should be true at all times. If any
  are not true, it's very likely that a user program has somehow
  trashed memory. (It's also possible that there is a coding error
  in malloc. In which case, please report it!)

*/

#if ! DEBUG

#define check_chunk(P)
#define check_free_chunk(P)
#define check_inuse_chunk(P)
#define check_remalloced_chunk(P,N)
#define check_malloced_chunk(P,N)
#define check_malloc_state()

#else
#define check_chunk(P) do_check_chunk(P)
#define check_free_chunk(P)  do_check_free_chunk(P)
#define check_inuse_chunk(P) do_check_inuse_chunk(P)
#define check_remalloced_chunk(P,N) do_check_remalloced_chunk(P,N)
#define check_malloced_chunk(P,N) do_check_malloced_chunk(P,N)
#define check_malloc_state() do_check_malloc_state()


/*
  Properties of all chunks
*/

#if __STD_C
static void do_check_chunk(mchunkptr p)
#else
static void do_check_chunk(p) mchunkptr p;
#endif
{
  mstate av = get_malloc_state();
  unsigned long sz = chunksize(p);

  if (!chunk_is_mmapped(p)) {
    
    /* Has legal address ... */
    if (av->sbrk_base != NONCONTIGUOUS_REGIONS) {
      assert((char*)p >= (char*)(av->sbrk_base));
    }

    if (p != av->top) {
      if (av->sbrk_base != NONCONTIGUOUS_REGIONS) {
        assert((char*)p + sz <= (char*)(av->top));
      }
    }
    else {
      if (av->sbrk_base != NONCONTIGUOUS_REGIONS) {
        assert((char*)p + sz <= (char*)(av->sbrk_base) + av->sbrked_mem);
      }
      /* top size is always at least MINSIZE */
      assert((long)(sz) >= (long)(MINSIZE));
      /* top predecessor always marked inuse */
      assert(prev_inuse(p));
    }
      
  }
  else {
#if HAVE_MMAP
    /* address is outside main heap  */
    /* unless mmap has been used as sbrk backup */
    if (av->sbrk_base != NONCONTIGUOUS_REGIONS) { 
      assert(! ((char*)p >= (char*)av->sbrk_base &&
                (char*)p <  (char*)(av->sbrk_base) + av->sbrked_mem));
    }
    /* chunk is page-aligned */
    assert(((p->prev_size + sz) & (av->pagesize-1)) == 0);
    /* mem is aligned */
    assert(aligned_OK(chunk2mem(p)));
#else
    /* force an appropriate assert violation if debug set */
    assert(!chunk_is_mmapped(p));
#endif
  }

}

/*
  Properties of free chunks
*/


#if __STD_C
static void do_check_free_chunk(mchunkptr p)
#else
static void do_check_free_chunk(p) mchunkptr p;
#endif
{
  mstate av = get_malloc_state();

  INTERNAL_SIZE_T sz = p->size & ~PREV_INUSE;
  mchunkptr next = chunk_at_offset(p, sz);

  do_check_chunk(p);

  /* Chunk must claim to be free ... */
  assert(!inuse(p));
  assert (!chunk_is_mmapped(p));

  /* Unless a special marker, must have OK fields */
  if ((long)sz >= (long)MINSIZE)
  {
    assert((sz & MALLOC_ALIGN_MASK) == 0);
    assert(aligned_OK(chunk2mem(p)));
    /* ... matching footer field */
    assert(next->prev_size == sz);
    /* ... and is fully consolidated */
    assert(prev_inuse(p));
    assert (next == av->top || inuse(next));

    /* ... and has minimally sane links */
    assert(p->fd->bk == p);
    assert(p->bk->fd == p);
  }
  else /* markers are always of size SIZE_SZ */
    assert(sz == SIZE_SZ);
}

/*
  Properties of inuse chunks
*/


#if __STD_C
static void do_check_inuse_chunk(mchunkptr p)
#else
static void do_check_inuse_chunk(p) mchunkptr p;
#endif
{
  mstate av = get_malloc_state();
  mchunkptr next;
  do_check_chunk(p);

  if (chunk_is_mmapped(p))
    return; /* mmapped chunks have no next/prev */

  /* Check whether it claims to be in use ... */
  assert(inuse(p));

  next = next_chunk(p);

  /* ... and is surrounded by OK chunks.
    Since more things can be checked with free chunks than inuse ones,
    if an inuse chunk borders them and debug is on, it's worth doing them.
  */
  if (!prev_inuse(p))  {
    /* Note that we cannot even look at prev unless it is not inuse */
    mchunkptr prv = prev_chunk(p);
    assert(next_chunk(prv) == p);
    do_check_free_chunk(prv);
  }

  if (next == av->top) {
    assert(prev_inuse(next));
    assert(chunksize(next) >= MINSIZE);
  }
  else if (!inuse(next))
    do_check_free_chunk(next);

}

/*
  Properties of chunks recycled from fastbins
*/

#if __STD_C
static void do_check_remalloced_chunk(mchunkptr p, INTERNAL_SIZE_T s)
#else
static void do_check_remalloced_chunk(p, s) mchunkptr p; INTERNAL_SIZE_T s;
#endif
{

  INTERNAL_SIZE_T sz = p->size & ~PREV_INUSE;
  long room = sz - s;

  do_check_inuse_chunk(p);

  /* Legal size ... */
  assert((long)sz >= (long)MINSIZE);
  assert((sz & MALLOC_ALIGN_MASK) == 0);
  assert(room >= 0);
  assert(room < (long)MINSIZE);

  /* ... and alignment */
  assert(aligned_OK(chunk2mem(p)));

}

/*
  Properties of nonrecycled chunks at the point they are malloced
*/

#if __STD_C
static void do_check_malloced_chunk(mchunkptr p, INTERNAL_SIZE_T s)
#else
static void do_check_malloced_chunk(p, s) mchunkptr p; INTERNAL_SIZE_T s;
#endif
{
  /* same as recycled case ... */
  do_check_remalloced_chunk(p, s);

  /*
    ... plus,  must obey implementation invariant that prev_inuse is
    always true of any allocated chunk; i.e., that each allocated
    chunk borders either a previously allocated and still in-use
    chunk, or the base of its memory arena. This is ensured
    by making all allocations from the the `lowest' part of any found
    chunk.  This does not necessarily hold however for chunks
    recycled via fastbins.
  */

  assert(prev_inuse(p));
}


/*
  Properties of malloc_state.

  This may be useful for debugging malloc, as well as detecting user
  programmer errors that somehow write into malloc_state.
*/

static void do_check_malloc_state()
{
  mstate av = get_malloc_state();
  int i;
  mchunkptr p;
  mchunkptr q;
  mbinptr b;
  mfastbinptr f;
  unsigned int biton;
  int empty;
  unsigned int idx;
  INTERNAL_SIZE_T size;
  unsigned long total = 0;
  int max_fast_bin;

  /* cannot run these checks until fully initialized */
  if (av->top == 0 || av->top == initial_top(av))
    return;

  /* internal size_t must be no wider than pointer type */
  assert(sizeof(INTERNAL_SIZE_T) <= sizeof(char*));

  /* pagesize is a power of 2 */
  assert((av->pagesize & (av->pagesize-1)) == 0);

  /* properties of fastbins */

  /* max_fast is in allowed range */
  assert(av->max_fast <= (MAX_FAST_SIZE & ~1));

  max_fast_bin = fastbin_index(av->max_fast);

  for (i = 0; i < NFASTBINS; ++i) {
    f = &(av->fastbins[i]);
    p = f->hd;

    /* all bins past max_fast are empty */
    if (i > max_fast_bin)
      assert(p == 0);

    /* hd and tl must agree about emptiness */
    assert((p == 0) == (f->tl == 0));

    while (p != 0) {
      /* each chunk claims to be inuse */
      do_check_inuse_chunk(p);
      total += chunksize(p);
      /* chunk belongs in this bin */
      assert(fastbin_index(chunksize(p)) == i);
      /* tail points to last chunk on list */
      if (p->fd == 0)
        assert(p == f->tl);
      p = p->fd;
    }
  }

  /* total size of fastbin chunks is correct */
  assert (total == av->fastbytes);

  /* check normal bins */
  for (i = 1; i < NBINS; ++i) {
    b = bin_at(av,i);

    /* binmap is accurate (except for bin 1 == unsorted_chunks) */
    if (i >= 2) {
      biton = get_binmap(av,i);
      empty = last(b) == b;
      if (!biton)
        assert(empty);
      else if (!empty)
        assert(biton);
    }

    for (p = last(b); p != b; p = p->bk) {
      /* each chunk claims to be free */
      do_check_free_chunk(p);
      size = chunksize(p);
      total += size;
      if (i >= 2) {
        /* chunk belongs in bin */
        idx = bin_index(size);
        assert(idx == i);
        /* lists are sorted */
        assert(p->bk == b || chunksize(p->bk) >= chunksize(p));
      }
      /* chunk is followed by a legal chain of inuse chunks */
      for (q = next_chunk(p);
           q != av->top && inuse(q) && (long)(chunksize(q)) >= (long)MINSIZE;
           q = next_chunk(q))
        do_check_inuse_chunk(q);

    }
  }

  /* top chunk is OK */
  check_chunk(av->top);

  /* sanity checks for statistics */

  assert(total <= (unsigned long)(av->max_total_mem));
  assert(av->n_mmaps >= 0);
  assert(av->n_mmaps <= av->n_mmaps_max);
  assert(av->n_mmaps <= av->max_n_mmaps);
  assert(av->max_n_mmaps <= av->n_mmaps_max);


  assert((unsigned long)(av->sbrked_mem) <=
         (unsigned long)(av->max_sbrked_mem));

  assert((unsigned long)(av->mmapped_mem) <=
         (unsigned long)(av->max_mmapped_mem));

  assert((unsigned long)(av->max_total_mem) >=
         (unsigned long)(av->mmapped_mem) + (unsigned long)(av->sbrked_mem));

}


#endif





/* ----------- Routines dealing with system allocation -------------- */

/*
  Handle malloc cases requiring more memory from system.
  malloc relays to sYSMALLOc if it cannot allocate out of
  existing memory.
*/

#if __STD_C
static Void_t* sYSMALLOc(INTERNAL_SIZE_T nb)
#else
static Void_t* sYSMALLOc(nb) INTERNAL_SIZE_T nb;
#endif
{
  mstate av = get_malloc_state(); /* av MUST be initialized on entry */

  long            size;           /* arg to first MORECORE/mmap call */
  long            correction;     /* arg to 2nd call */

  INTERNAL_SIZE_T front_misalign; /* unusable bytes at front of new space */
  INTERNAL_SIZE_T end_misalign;   /* partial page left at end of new space */

  char*           brk;            /* return value from MORECORE */
  char*           aligned_brk;    /* aligned offset into brk */
  char*           snd_brk;        /* 2nd return val */

  mchunkptr       p;              /* the allocated/returned chunk */
  mchunkptr       remainder;      /* remainder from allocation */
  long            remainder_size; /* its size */

  unsigned long   sum;            /* for updating stats */

  mchunkptr       old_top;        /* incoming value of av->top */
  INTERNAL_SIZE_T old_size;       /* its size */
  char*           old_end;        /* its end address */

  size_t          pagemask = av->pagesize - 1;
  

  /*
    If have mmap, and the request size meets the mmap threshold, and
    the system supports mmap, and there are few enough currently
    allocated mmapped regions, and a call to mmap succeeds, directly map
    this request rather than expanding top.
  */

#if HAVE_MMAP
  if ((unsigned long)nb >= (unsigned long)(av->mmap_threshold) &&
      (av->n_mmaps < av->n_mmaps_max)) {

    /*
      Round up size to nearest page.
      For mmapped chunks, the overhead is one SIZE_SZ unit larger, because
      there is no following chunk whose prev_size field could be used.
    */
    size = (nb + SIZE_SZ + pagemask) & ~pagemask;
    
    p = (mchunkptr)(MMAP(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE));

    if (p != (mchunkptr)(MORECORE_FAILURE)) {
      set_head(p, size|IS_MMAPPED);

      /*
        The offset to the start of the mmapped region is stored
        in the prev_size field of the chunk; normally it is zero,
        but that can be changed in memalign().
      */
      
      assert(p->prev_size == 0);
      check_chunk(p);
      
      /* update statistics */

      if (++av->n_mmaps > av->max_n_mmaps) 
        av->max_n_mmaps = av->n_mmaps;

      sum = av->mmapped_mem += size;
      if (sum > (unsigned long)(av->max_mmapped_mem)) 
        av->max_mmapped_mem = sum;
      sum += av->sbrked_mem;
      if (sum > (unsigned long)(av->max_total_mem)) 
        av->max_total_mem = sum;
      
      return chunk2mem(p);
    }
  }
#endif

  /* record incoming configuration */

  old_top  = av->top;
  old_size = chunksize(old_top);
  old_end  = (char*)(chunk_at_offset(old_top, old_size));

  brk = snd_brk = (char*)(MORECORE_FAILURE); 

  /* 
     If not the first time through, we require old_size to be
     at least MINSIZE and to have prev_inuse
  */

  assert(old_top == initial_top(av) || 
         ((unsigned long) (old_size) >= (unsigned long)(MINSIZE) &&
          prev_inuse(old_top)));

  /* Request enough space for nb + pad + overhead */

  size = nb + av->top_pad + MINSIZE;

  /*
    If contiguous, we can subtract out existing space that we hope to
    combine with new space. We add it back later in only if
    we don't actually get contiguous space.
  */

  if (av->sbrk_base != NONCONTIGUOUS_REGIONS)
    size -= old_size;

  /*
    Round to a multiple of page size.
    If MORECORE is not contiguous, this ensures that we only call it
    with whole-page arguments.  And if MORECORE is contiguous and
    this is not first time through, this preserves page-alignment of
    previous calls. Otherwise, we re-correct anyway to page-align below.
  */

  size = (size + pagemask) & ~pagemask;

  /*
    Don't try to call MORECORE if argument is so big as to appear
    negative. Note that since mmap takes size_t arg, it may succeed
    below even if we cannot call MORECORE.
  */

  if (size > 0) 
    brk = (char*)(MORECORE(size));

  /*
    If have mmap, try using it as a backup when MORECORE fails. This
    is worth doing on systems that have "holes" in address space, so
    sbrk cannot extend to give contiguous space, but space is available
    elsewhere.  Note that we ignore mmap max count and threshold limits,
    since there is no reason to artificially limit use here.
  */

#if HAVE_MMAP
  if (brk == (char*)(MORECORE_FAILURE)) {

    /* Cannot merge with old, so add back in */

    if (av->sbrk_base != NONCONTIGUOUS_REGIONS)
      size = (size + old_size + pagemask) & ~pagemask;

    /* If we are relying on mmap as backup, then use larger units */

    if ((unsigned long)size < (unsigned long)MMAP_AS_MORECORE_SIZE)
      size = MMAP_AS_MORECORE_SIZE;

    brk = (char*)(MMAP(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE));

    if (brk != (char*)(MORECORE_FAILURE)) {

      /* We don't need, and cannot use, another sbrk call to find end */
      snd_brk = brk + size;

      /* 
         We no longer have a base of contiguous region. 
         After the first time mmap is used as backup, we cannot
         ever rely on contiguous space.
      */
      av->sbrk_base = NONCONTIGUOUS_REGIONS; 
    }
  }
#endif

  if (brk != (char*)(MORECORE_FAILURE)) {

    av->sbrked_mem += size;

    /*
      If MORECORE extends previous space, we can likewise extend top size.
    */
    
    if (brk == old_end && snd_brk == (char*)(MORECORE_FAILURE)) {
      set_head(old_top, (size + old_size) | PREV_INUSE);
    }
    
    /*
      Otherwise, make adjustments, guided by the special values of 
      av->sbrk_base (MORECORE_FAILURE or NONCONTIGUOUS_REGIONS):
      
      * If the first time through or noncontiguous, we need to call sbrk
        just to find out where the end of memory lies.

      * We need to ensure that all returned chunks from malloc will meet
        MALLOC_ALIGNMENT

      * If there was an intervening foreign sbrk, we need to adjust sbrk
        request size to account for fact that we will not be able to
        combine new space with existing space in old_top.

      * Almost all systems internally allocate whole pages at a time, in
        which case we might as well use the whole last page of request.
        So we allocate enough more memory to hit a page boundary now,
        which in turn causes future contiguous calls to page-align.

    */
    
    else {
      front_misalign = 0;
      end_misalign = 0;
      correction = 0;
      aligned_brk = brk;
      
      /* handle contiguous cases */
      if (av->sbrk_base != NONCONTIGUOUS_REGIONS) { 
        
        /* Guarantee alignment of first new chunk made from this space */

        front_misalign = (INTERNAL_SIZE_T)chunk2mem(brk) & MALLOC_ALIGN_MASK;
        if (front_misalign > 0) {

          /*
            We don't need to specially mark these wasted front bytes.
            They will never be accessed anyway because
            prev_inuse of av->top (and any chunk created from its start)
            is always true after initialization.
          */

          correction = MALLOC_ALIGNMENT - front_misalign;
          aligned_brk += correction;
        }
        
        /*
          If this isn't adjacent to a previous sbrk, then we will not
          be able to merge with old_top space, so must add to 2nd request.
        */
        
        correction += old_size;
        
        /* Pad out to hit a page boundary */

        end_misalign = (INTERNAL_SIZE_T)(brk + size + correction);
        correction += ((end_misalign + pagemask) & ~pagemask) - end_misalign;
        
        assert(correction >= 0);
        
        snd_brk = (char*)(MORECORE(correction));
        
        /*
          If can't allocate correction, try to at least find out current
          brk.  It might be enough to proceed without failing.
          
          Note that if second sbrk did NOT fail, we assume that space
          is contiguous with first sbrk. This is a safe assumption unless
          program is multithreaded but doesn't use locks and a foreign sbrk
          occurred between our first and second calls.
        */
        
        if (snd_brk == (char*)(MORECORE_FAILURE)) {
          correction = 0;
          snd_brk = (char*)(MORECORE(0));
        }
      }
      
      /* handle non-contiguous cases */
      else { 
        
        /* MORECORE/mmap must correctly align etc */
        assert(((unsigned long)chunk2mem(brk) & MALLOC_ALIGN_MASK) == 0);
        
        /* Find out current end of memory */
        if (snd_brk == (char*)(MORECORE_FAILURE)) {
          snd_brk = (char*)(MORECORE(0));
        }
        
        /* This must lie on a page boundary */
        if (snd_brk != (char*)(MORECORE_FAILURE)) {
          assert(((INTERNAL_SIZE_T)(snd_brk) & pagemask) == 0);
        }
      }
      
      if (snd_brk != (char*)(MORECORE_FAILURE)) {
       
        av->top = (mchunkptr)aligned_brk;
        set_head(av->top, (snd_brk - aligned_brk + correction) | PREV_INUSE);
        
        av->sbrked_mem += correction;
        
        /* If first time through and contiguous, record base */
        if (old_top == initial_top(av)) {
          if (av->sbrk_base == (char*)(MORECORE_FAILURE)) 
            av->sbrk_base = brk;
        }
        
        /*
          Otherwise, we either have a gap or a non-contiguous region.
          Insert a double fencepost at old_top to prevent
          consolidation with space we don't own. These fenceposts are
          artificial chunks that are marked as inuse and are in any
          case too small to use.  We need two to make sizes and
          alignments work out.
        */

        else {
          
          /* 
             Shrink old_top to insert fenceposts, keeping size a
             multiple of MALLOC_ALIGNMENT. 
          */
          old_size = (old_size - 3*SIZE_SZ) & ~MALLOC_ALIGN_MASK;
          set_head(old_top, old_size | PREV_INUSE);
          
          /*
            Note that the following assignments overwrite old_top when
            old_size was previously MINSIZE.  This is intentional. We
            need the fencepost, even if old_top otherwise disappears.
          */
          chunk_at_offset(old_top, old_size          )->size =
            SIZE_SZ|PREV_INUSE;

          chunk_at_offset(old_top, old_size + SIZE_SZ)->size =
            SIZE_SZ|PREV_INUSE;
          
          /* If possible, release the rest. */
          if (old_size >= MINSIZE) 
            fREe(chunk2mem(old_top));

        }
      }
    }
    
    /* Update statistics */
    
    sum = av->sbrked_mem;
    if (sum > (unsigned long)(av->max_sbrked_mem))
      av->max_sbrked_mem = sum;
    
    sum += av->mmapped_mem;
    if (sum > (unsigned long)(av->max_total_mem))
      av->max_total_mem = sum;
    
    /* finally, do the allocation */
    
    p = av->top;
    size = chunksize(p);
    remainder_size = size - nb;
    
    /* check that one of the above allocation paths succeeded */
    if (remainder_size >= (long)MINSIZE) {
      remainder = chunk_at_offset(p, nb);
      av->top = remainder;
      set_head(p, nb | PREV_INUSE);
      set_head(remainder, remainder_size | PREV_INUSE);
      
      check_malloced_chunk(p, nb);
      return chunk2mem(p);
    }
  }

  /* catch all failure paths */
  MALLOC_FAILURE_ACTION;
  return 0;
}


/*
  Malloc_trim is an inverse of sorts to sYSMALLOc.
  It is also publically callable though.

  Malloc_trim gives memory back to the system (via negative
  arguments to sbrk) if there is unused memory at the `high' end of
  the malloc pool. You can call this after freeing large blocks of
  memory to potentially reduce the system-level memory requirements
  of a program. However, it cannot guarantee to reduce memory. Under
  some allocation patterns, some large free blocks of memory will be
  locked between two used chunks, so they cannot be given back to
  the system.
  
  The `pad' argument to malloc_trim represents the amount of free
  trailing space to leave untrimmed. If this argument is zero,
  only the minimum amount of memory to maintain internal data
  structures will be left (one page or less). Non-zero arguments
  can be supplied to maintain enough trailing space to service
  future expected allocations without having to re-obtain memory
  from the system.
  
  Malloc_trim returns 1 if it actually released any memory, else 0.
*/

#if __STD_C
int mTRIm(size_t pad)
#else
int mTRIm(pad) size_t pad;
#endif
{
  mstate av = get_malloc_state();

  long  top_size;        /* Amount of top-most memory */
  long  extra;           /* Amount to release */
  long  released;        /* Amount actually released */
  char* current_brk;     /* address returned by pre-check sbrk call */
  char* new_brk;         /* address returned by post-check sbrk call */
  size_t pagesz;

  /* Ensure initialization/consolidation */
  if (need_consolidation(av) || av->fastbytes != 0)
    malloc_consolidate();

  pagesz = av->pagesize;
  top_size = chunksize(av->top);

  /* Release in pagesize units, keeping at least one page */
  extra = ((top_size - pad - MINSIZE + (pagesz-1)) / pagesz - 1) * pagesz;

  if (extra > 0) {

    /*
       Only proceed if end of memory is where we last set it.
       This avoids problems if there were foreign sbrk calls.
    */
    current_brk = (char*)(MORECORE(0));
    if (current_brk == (char*)(av->top) + top_size) {

      /*
        Attempt to release memory. We ignore return value,
        and instead call again to find out where new end of memory is.
        This avoids problems if first call releases less than we asked,
        of if failure somehow altered brk value. (We could still
        encounter problems if it altered brk in some very bad way,
        but the only thing we can do is adjust anyway, which will cause
        some downstream failure.)
      */

      MORECORE(-extra);
      new_brk = (char*)(MORECORE(0));

      if (new_brk != (char*)MORECORE_FAILURE) {
        released = (long)(current_brk - new_brk);
        if (released != 0) {
          /* Adjust top. */
          av->sbrked_mem -= released;
          set_head(av->top, (top_size - released) | PREV_INUSE);
          check_malloc_state();
          return 1;
        }
      }
    }
  }

  return 0;
}

/* ----------------------- Main public routines ----------------------- */


/*
  Malloc routine. See running comments for algorithm description.
*/


#if __STD_C
Void_t* mALLOc(size_t bytes)
#else
  Void_t* mALLOc(bytes) size_t bytes;
#endif
{
  mstate av = get_malloc_state();

  INTERNAL_SIZE_T nb;               /* normalized request size */
  unsigned int    idx = 0;          /* associated bin index */
  mbinptr         bin;              /* associated bin */
  mfastbinptr     fb;               /* associated fastbin */

  mchunkptr       victim;           /* inspected/selected chunk */
  INTERNAL_SIZE_T size;             /* its size */
  int             victim_index;     /* its bin index */

  mchunkptr       remainder;        /* remainder from a split */
  long            remainder_size;   /* its size */

  unsigned int    block;            /* bit map traverser */
  unsigned int    bit;              /* bit map traverser */
  unsigned int    map;              /* current word of binmap */

  mchunkptr       fwd;              /* misc temp for linking */
  mchunkptr       bck;              /* misc temp for linking */


  /*
    Check request for legality and convert to internal form, nb.
    This rejects negative arguments when size_t is treated as
    signed. It also rejects arguments that are so large that the size
    becomes negative when aligned and padded.  The converted form
    adds SIZE_T bytes overhead plus possibly more to obtain 8-byte
    alignment and/or to obtain a size of at least MINSIZE, the
    smallest allocatable size.
  */

  checked_request2size(bytes, nb);

  /*
    If the size qualifies as a fastbin, check corresponding bin.
    (Note that this code is safe to execute even if av not yet initialized.)
  */

  if (nb <= av->max_fast) {
    fb = &(av->fastbins[(fastbin_index(nb))]);
    if ( (victim = fb->hd) != 0) {
      fwd = victim->fd;
      av->fastbytes -= nb;
      fb->hd = fwd;
      if (fwd == 0) fb->tl = 0;

      check_remalloced_chunk(victim, nb);
      return chunk2mem(victim);
    }
  }

  /*
    Before continuing, consolidate fastbins if it looks like their
    existence could degrade fits. This is done if:

    * The current request is >= MIN_LARGE_SIZE (512 bytes)

    * A large chunk was freed since last consolidation

    * av has not yet been intitialized.

    * There was at some point space in fastbins, but a a request could
      not be satisfied without splitting space in much larger bins or
      top.

    The rules try to adapt to different (sometimes very transient)
    modes of use. The main idea is to kill off fastbins as soon as
    they look problematic -- early enough so that it is still cheap
    (especially wrt locality) and effective to do so, but to still
    give them a chance to improve performance when in a mode where
    many small chunks are being continually allocated and freed. (512
    is used as a cutoff because it is unusually big for a small chunk,
    but unusually small for a big one.)

  */

  if (need_consolidation(av) ||
      (!in_smallbin_range(nb) && av->fastbytes != 0))
    malloc_consolidate();

  /*
    If a small request, check regular bin.  Since these "smallbins"
    hold one size each, no searching is necessary.
    
    (If a large request, we need to wait until unsorted chunks are
    processed to find best fit. But for small ones, fits are exact
    anyway, so we can check now, which is faster.)

  */

  if (in_smallbin_range(nb)) {
    idx = smallbin_index(nb);
    bin = bin_at(av,idx);
    if ( (victim = last(bin)) != bin) {
      bck = victim->bk;
      set_inuse_bit_at_offset(victim, nb);
      bin->bk = bck;
      bck->fd = bin;

      check_malloced_chunk(victim, nb);
      return chunk2mem(victim);
    }
  }

  /*
    Scan through recently freed or remaindered chunks, placing
    unusable ones in bins.  Note that this step is the only place in
    any routine where chunks are placed in bins.
    
    For small requests, use chunk if an exact fit or if the only chunk
    left is the one that was last used to satisfy a previous small
    request.  This helps preserve locality in the absence of exact
    fits for small requests, and is often the best fitting chunk
    anyway.
    
    For large requests, ignore even exact fits for now,
    since a better (less recently used) choice may be in nb's bin.
  */

  while ( (victim = unsorted_chunks(av)->bk) != unsorted_chunks(av)) {
    bck = victim->bk;
    size = chunksize(victim);

    /* Try to use last remainder if it is the only chunk left in bin. */

    if (in_smallbin_range(nb) &&
        bck == unsorted_chunks(av) &&
        victim == av->last_remainder &&
        (remainder_size = size - nb) >= (long)MINSIZE) {

      remainder = chunk_at_offset(victim, nb);

      /* reattach remainder */
      unsorted_chunks(av)->bk = unsorted_chunks(av)->fd = remainder;
      av->last_remainder = remainder;
      remainder->bk = remainder->fd = unsorted_chunks(av);

      set_head(victim, nb | PREV_INUSE);
      set_head(remainder, remainder_size | PREV_INUSE);
      set_foot(remainder, remainder_size);

      check_malloced_chunk(victim, nb);
      return chunk2mem(victim);
    }

    unsorted_chunks(av)->bk = bck;
    bck->fd = unsorted_chunks(av);

    /* place chunk in bin */

    if (in_smallbin_range(size)) {

      /* Take now instead of binning if small and exact fit */
      if (size == nb) {
        set_inuse_bit_at_offset(victim, size);
        check_malloced_chunk(victim, nb);
        return chunk2mem(victim);
      }

      victim_index = smallbin_index(size);
      bck = bin_at(av, victim_index);
      fwd = bck->fd;
    }
    else {
      victim_index = largebin_index(size);
      bck = bin_at(av, victim_index);
      fwd = bck->fd;

      /* maintain large bins in sorted order */
      if (fwd != bck) {
        while (fwd != bck && size < chunksize(fwd)) fwd = fwd->fd;
        bck = fwd->bk;
      }

    }

    mark_bin(av, victim_index);
    victim->bk = bck;
    victim->fd = fwd;
    fwd->bk = victim;
    bck->fd = victim;
  }

  /*
    If a large request, scan through the chunks in current bin in
    sorted order to find smallest that fits.  This is the only step
    where an unbounded number of chunks might be scanned without doing
    anything useful with them. However the lists tend to be very
    short.
  */

  if (!in_smallbin_range(nb)) {
    idx = largebin_index(nb);
    bin = bin_at(av, idx);

    for (victim = last(bin); victim != bin; victim = victim->bk) {
      size = chunksize(victim);
      remainder_size = size - nb;

      if (remainder_size >= 0)  {
        unlink(victim, bck, fwd);

        /* Exhaust */
        if (remainder_size < (long)MINSIZE)  {
          set_inuse_bit_at_offset(victim, size);
          check_malloced_chunk(victim, nb);
          return chunk2mem(victim);
        }
        /* Split */
        else {
          remainder = chunk_at_offset(victim, nb);
          unsorted_chunks(av)->bk = unsorted_chunks(av)->fd = remainder;
          remainder->bk = remainder->fd = unsorted_chunks(av);
          set_head(victim, nb | PREV_INUSE);
          set_head(remainder, remainder_size | PREV_INUSE);
          set_foot(remainder, remainder_size);
          check_malloced_chunk(victim, nb);
          return chunk2mem(victim);
        }
      }
    }
  }

  /*
    Search for a chunk by scanning bins, starting with next largest
    bin. This search is strictly by best-fit; i.e., the smallest
    (with ties going to approximately the least recently used) chunk
    that fits is selected.
    
    The bitmap avoids needing to check that most blocks are nonempty.
    The particular case of skipping all bins during warm-up phases
    when no chunks have been returned yet is faster than it might look.

  */

  ++idx;
  bin = bin_at(av,idx);
  block = idx2block(idx);
  map = av->binmap[block];
  bit = idx2bit(idx);

  for (;;) {
    /*
      Skip rest of block if there are no more set bits in this block.
    */

    if (bit > map || bit == 0) {
      for (;;) {
        if (++block >= BINMAPSIZE) /* out of bins */
          break;

        else if ( (map = av->binmap[block]) != 0) {
          bin = bin_at(av, (block << BINMAPSHIFT));
          bit = 1;
          break;
        }
      }
      /* Optimizers seem to like this double-break better than goto */
      if (block >= BINMAPSIZE)
          break;
    }

    /* Advance to bin with set bit. There must be one. */
    while ((bit & map) == 0) {
      bin = next_bin(bin);
      bit <<= 1;
    }

    victim = last(bin);

    /*  False alarm -- the bin is empty. Clear the bit. */
    if (victim == bin) {
      av->binmap[block] = map &= ~bit; /* Write through */
      bin = next_bin(bin);
      bit <<= 1;
    }

    /*  We know the first chunk in this bin is big enough to use. */
    else {
      size = chunksize(victim);
      remainder_size = size - nb;

      assert(remainder_size >= 0);

      /* unlink */
      bck = victim->bk;
      bin->bk = bck;
      bck->fd = bin;


      /* Exhaust */
      if (remainder_size < (long)MINSIZE) {
        set_inuse_bit_at_offset(victim, size);
        check_malloced_chunk(victim, nb);
        return chunk2mem(victim);
      }

      /* Split */
      else {
        remainder = chunk_at_offset(victim, nb);

        unsorted_chunks(av)->bk = unsorted_chunks(av)->fd = remainder;
        remainder->bk = remainder->fd = unsorted_chunks(av);
        if (in_smallbin_range(nb)) {

          /* advertise remainder as last_remainder */
          av->last_remainder = remainder;

          /*
            If we are splitting a large free chunk to satisfy a small
            request but there are lots of fastbin chunks, arrange to
            consolidate on next malloc. It isn't worth consolidating
            and restarting current malloc since this is just a
            safeguard against buildup of fastbin chunks that never
            become requested.
          */

          if (remainder_size >= MIN_LARGE_SIZE  &&
              av->fastbytes  >= FASTBIN_CONSOLIDATION_THRESHOLD)
            set_consolidation_flag(av);
        }


        set_head(victim, nb | PREV_INUSE);
        set_head(remainder, remainder_size | PREV_INUSE);
        set_foot(remainder, remainder_size);
        check_malloced_chunk(victim, nb);
        return chunk2mem(victim);
      }
    }
  }

  /*
    If large enough, split off the chunk bordering the end of memory
    (`top'). Note that this use of top is in accord with the best-fit search
    rule.  In effect, top is treated as larger (and thus less well
    fitting) than any other available chunk since it can be extended
    to be as large as necessary (up to system limitations).
  */

  victim = av->top;
  size = chunksize(victim);
  remainder_size = size - nb;

  /* top must always have a remainder, so it always exists after split */
  if (remainder_size >= (long)MINSIZE) {
    remainder = chunk_at_offset(victim, nb);
    av->top = remainder;
    set_head(victim, nb | PREV_INUSE);
    set_head(remainder, remainder_size | PREV_INUSE);
    
    check_malloced_chunk(victim, nb);
    return chunk2mem(victim);
  }

  /*
    If fastbins exist, retry after consolidation to avoid expansion.
  */
  
  else if (av->fastbytes != 0) {
    set_consolidation_flag(av);
    return mALLOc(nb - MALLOC_ALIGN_MASK);
  }

  /* get more memory from system */
  else 
    return sYSMALLOc(nb);
}



/*
  Free routine. See running comments for algorithm description.
*/


#if __STD_C
void fREe(Void_t* mem)
#else
void fREe(mem) Void_t* mem;
#endif
{
  mstate av = get_malloc_state();

  mchunkptr       p;           /* chunk corresponding to mem */
  INTERNAL_SIZE_T size;        /* its size */
  mfastbinptr     fb;          /* associated fastbin */
  mchunkptr       tail;        /* tail of bin */
  mchunkptr       nextchunk;   /* next contiguous chunk */
  INTERNAL_SIZE_T nextsize;    /* its size */
  int             nextinuse;   /* true if nextchunk is used */
  INTERNAL_SIZE_T prevsize;    /* size of previous contiguous chunk */
  mchunkptr       bck;         /* misc temp for linking */
  mchunkptr       fwd;         /* misc temp for linking */


  /* free(0) has no effect */
  if (mem != 0) {

    p = mem2chunk(mem);
    check_inuse_chunk(p);

    size = chunksize(p);

    /*
      If eligible, place chunk on a fastbin
    */

    if (size <= av->max_fast) {

      fb = &(av->fastbins[fastbin_index(size)]);
      tail = fb->tl;
      av->fastbytes += size;
      p->fd = 0;

      fb->tl = p;
      if (tail == 0)
        fb->hd = p;
      else
        tail->fd = p;
    }

    /*
       Consolidate non-mmapped chunks as they arrive.
    */

    else if (!chunk_is_mmapped(p)) {

      /*
        Possibly schedule to consolidate fastbins on next malloc. The
        threshold compares against the sum of current size plus
        fastbin bytes, which means to arrange consolidation if
        freeing a large chunk, and/or if there are a lot of fastbin
        chunks but we are now freeing a non-fast one).
      */

      if ((unsigned long)(size + av->fastbytes) >=
          (unsigned long)FASTBIN_CONSOLIDATION_THRESHOLD)
          set_consolidation_flag(av);

      nextchunk = chunk_at_offset(p, size);

      /* consolidate backward */
      if (!prev_inuse(p)) {
        prevsize = p->prev_size;
        size += prevsize;
        p = chunk_at_offset(p, -((long) prevsize));
        unlink(p, bck, fwd);
      }

      nextsize = chunksize(nextchunk);

      if (nextchunk != av->top) {

        /* get and clear inuse bit */
        nextinuse = inuse_bit_at_offset(nextchunk, nextsize);
        set_head(nextchunk, nextsize);

        /* consolidate forward */
        if (!nextinuse) {
          unlink(nextchunk, bck, fwd);
          size += nextsize;
        }

        /*
          Place chunk in unsorted chunk list. Chunks are
          not placed into regular bins until after they have
          been given one chance to be used in malloc.
        */

        bck = unsorted_chunks(av);
        fwd = bck->fd;
        p->bk = bck;
        p->fd = fwd;
        bck->fd = p;
        fwd->bk = p;

        set_head(p, size | PREV_INUSE);
        set_foot(p, size);

      }

      /*
         If the chunk borders the current high end of memory,
         consolidate into top
      */

      else {

        size += nextsize;
        av->top = p;
        set_head(p, size | PREV_INUSE);

        /*
          If the total unused topmost memory exceeds trim
          threshold, ask malloc_trim to reduce top. Note that
          the comparison doesn't (can't) take into account
          fastbin chunks that may be bordering top, so may be
          an underestimate. malloc_trim will consolidate before
          trying to trim, so these chunks will be released. But
          it would be self-defeating to consolidate fastbins
          on each free just to see if threshold is reached.
        */

        if ((unsigned long)(size) > (unsigned long)(av->trim_threshold))
          mTRIm(av->top_pad);

      }

    }

    /*
      If the chunk was allocated via mmap, release via munmap()
      Note that if HAVE_MMAP is false but chunk_is_mmapped is
      true, then user must have overwritten memory. There's nothing
      we can do to catch this unless DEBUG is set, in which case
      check_inuse_chunk (above) will hopefully have caught this.
    */

    else {
#if HAVE_MMAP
      int ret;
      av->n_mmaps--;
      av->mmapped_mem -= (size + p->prev_size);
      ret = munmap((char*)p - p->prev_size, size + p->prev_size);
      /* munmap returns non-zero on failure */
      assert(ret == 0);
#endif
    }
  }
}



/*
  malloc_consolidate is a specialized version of free() that tears
  down chunks held in fastbins.  Free itself cannot be used for this
  purpose since, among other things, it might place chunks back onto
  fastbins.  So, instead, we need to use a minor variant of the same
  code.
  
  Also, because this routine needs to be called the first time through
  malloc anyway, it turns out to be the perfect place to bury
  initialization code.
*/

static void malloc_consolidate()
{
  mstate av = get_malloc_state();
  mfastbinptr     fb;
  mchunkptr       p;
  mchunkptr       nextp;

  /* These have same use as in free() */
  mchunkptr       nextchunk;
  INTERNAL_SIZE_T size;
  INTERNAL_SIZE_T nextsize;
  INTERNAL_SIZE_T prevsize;
  int             nextinuse;
  mchunkptr       bck;
  mchunkptr       fwd;
  mchunkptr       uns;
  mchunkptr       ufd;

  /*
    If top bin isn't set up, we know that malloc hasn't
    yet been initialized, in which case do so.
  */

  if (av->top == 0) {
    malloc_init_state(av);
    return;
  }

  /* Clear flags */
  clear_consolidation_flag(av);
  av->fastbytes = 0;

  uns = unsorted_chunks(av);

  /*
    Remove each chunk from fast bin and consolidate it, placing
    it then in unsorted bin. Placing in unsorted bin avoids
    needing to calculate actual bins until malloc is sure that chunks
    aren't immediately going to be reused anyway.
    
    Running through bins from largest to smallest seems to work a
    little better in practice than the other way around.
  */

  fb = &(av->fastbins[fastbin_index(av->max_fast)]);
  do {
    p = fb->hd;

    if (p != 0) {
      fb->hd = fb->tl = 0;

      do {
        check_inuse_chunk(p);
        nextp = p->fd;

        /* Slightly streamlined version of consolidation code in free() */
        size = p->size & ~PREV_INUSE;
        nextchunk = chunk_at_offset(p, size);

        if (!prev_inuse(p)) {
          prevsize = p->prev_size;
          size += prevsize;
          p = chunk_at_offset(p, -((long) prevsize));
          unlink(p, bck, fwd);
        }

        nextsize = chunksize(nextchunk);

        if (nextchunk != av->top) {

          nextinuse = inuse_bit_at_offset(nextchunk, nextsize);
          set_head(nextchunk, nextsize);

          if (!nextinuse) {
            size += nextsize;
            unlink(nextchunk, bck, fwd);
          }

          ufd = uns->fd;
          uns->fd = p;
          ufd->bk = p;

          set_head(p, size | PREV_INUSE);
          p->bk = uns;
          p->fd = ufd;
          set_foot(p, size);

        }

        else {
          size += nextsize;
          av->top = p;
          set_head(p, size | PREV_INUSE);
        }

      } while ( (p = nextp) != 0);

    }
  } while (fb-- != &(av->fastbins[0]));
}




/*
  Realloc algorithm cases:

  * Chunks that were obtained via mmap cannot be extended or shrunk
    unless HAVE_MREMAP is defined, in which case mremap is used.
    Otherwise, if their reallocation is for additional space, they are
    copied.  If for less, they are just left alone.

  * Otherwise, if the reallocation is for additional space, and the
    chunk can be extended, it is, else a malloc-copy-free sequence is
    taken.  There are several different ways that a chunk could be
    extended. All are tried:

       * Extending forward into following adjacent free chunk.
       * Shifting backwards, joining preceding adjacent space
       * Both shifting backwards and extending forward.
       * Extending into newly sbrked space

  * If the reallocation is for less space, the newly unused space is
    lopped off and freed. Unless the #define REALLOC_ZERO_BYTES_FREES
    is set, realloc with a size argument of zero (re)allocates a
    minimum-sized chunk.


  The old unix realloc convention of allowing the last-free'd chunk
  to be used as an argument to realloc is no longer supported.
  I don't know of any programs still relying on this feature,
  and allowing it would also allow too many other incorrect
  usages of realloc to be sensible.
*/


#if __STD_C
Void_t* rEALLOc(Void_t* oldmem, size_t bytes)
#else
Void_t* rEALLOc(oldmem, bytes) Void_t* oldmem; size_t bytes;
#endif
{
  mstate av = get_malloc_state();

  INTERNAL_SIZE_T  nb;              /* padded request size */

  mchunkptr        oldp;            /* chunk corresponding to oldmem */
  INTERNAL_SIZE_T  oldsize;         /* its size */

  mchunkptr        newp;            /* chunk to return */
  INTERNAL_SIZE_T  newsize;         /* its size (or a special flag value) */
  Void_t*          newmem;          /* corresponding user mem */

  mchunkptr        next;            /* next contiguous chunk after oldp */
  mchunkptr        prev;            /* previous contiguous chunk before oldp */

  mchunkptr        remainder;       /* extra space at end of newp */
  INTERNAL_SIZE_T  remainder_size;  /* its size */

  mchunkptr        bck;             /* misc temp for linking */
  mchunkptr        fwd;             /* misc temp for linking */

  INTERNAL_SIZE_T  copysize;        /* bytes to copy */
  int              ncopies;         /* INTERNAL_SIZE_T words to copy */
  INTERNAL_SIZE_T* s;               /* copy source */ 
  INTERNAL_SIZE_T* d;               /* copy destination */


#ifdef REALLOC_ZERO_BYTES_FREES
  if (bytes == 0) {
    fREe(oldmem);
    return 0;
  }
#endif

  /* realloc of null is supposed to be same as malloc */
  if (oldmem == 0) return mALLOc(bytes);

  checked_request2size(bytes, nb);

  /*
    If a large request, clobber fastbins first
  */
  if (!in_smallbin_range(nb) && av->fastbytes != 0)
    malloc_consolidate();

  oldp    = mem2chunk(oldmem);
  oldsize = chunksize(oldp);

  check_inuse_chunk(oldp);

  if (!chunk_is_mmapped(oldp)) {

    if ((unsigned long)(oldsize) >= (unsigned long)(nb)) {
      /* already big enough; split below */
      newp = oldp;
      newsize = oldsize;
    }

    else {
      newp = 0;
      newsize = 0;

      next = chunk_at_offset(oldp, oldsize);

      if (next == av->top) {            /* Expand forward into top */
        newsize = oldsize + chunksize(next);

        if ((unsigned long)(newsize) >= (unsigned long)(nb + MINSIZE)) {
          set_head_size(oldp, nb);
          av->top = chunk_at_offset(oldp, nb);
          set_head(av->top, (newsize - nb) | PREV_INUSE);
          return chunk2mem(oldp);
        }

        else if (!prev_inuse(oldp)) {   /* Shift backwards + top */
          prev = prev_chunk(oldp);
          newsize += chunksize(prev);

          if ((unsigned long)(newsize) >= (unsigned long)(nb + MINSIZE)) {
            newp = prev;
            unlink(prev, bck, fwd);
            set_head_size(newp, nb);
            av->top = chunk_at_offset(newp, nb);
            set_head(av->top, (newsize - nb) | PREV_INUSE);
            newsize = -2; /* signal not to trim remainder below */
          }
        }
      }

      else if (!inuse(next)) {          /* Forward into next chunk */
        newsize = oldsize + chunksize(next);
        
        if (((unsigned long)(newsize) >= (unsigned long)(nb))) {
          newp = oldp;
          unlink(next, bck, fwd);
        }
        
        else if (!prev_inuse(oldp)) {   /* Forward + backward */
          prev = prev_chunk(oldp);
          newsize += chunksize(prev);
          
          if (((unsigned long)(newsize) >= (unsigned long)(nb))) {
            newp = prev;
            unlink(prev, bck, fwd);
            unlink(next, bck, fwd);
          }
        }
      }
      
      else if (!prev_inuse(oldp)) {     /* Backward only */
        prev = prev_chunk(oldp);
        newsize = oldsize + chunksize(prev);
        
        if ((unsigned long)(newsize) >= (unsigned long)(nb)) {
          newp = prev;
          unlink(prev, bck, fwd);
        }
      }

      if (newp == 0) {                  /* Must allocate */
        newmem = mALLOc(nb - MALLOC_ALIGN_MASK);
        if (newmem == 0)
          return 0; /* propagate failure */

        newp = mem2chunk(newmem);

        /*
          Avoid copy if newp is next chunk after oldp.
        */
        if (newp == next) {
          newsize = oldsize + chunksize(newp);
          newp = oldp;
        }
        else
          newsize = -1; /* signal to free below */
      }

      /* copy if necessary */

      if (newp != oldp) {

        /*
          Unroll copy of <= 36 bytes (72 if 8byte sizes)
          We know that contents have an odd number of
          INTERNAL_SIZE_T-sized words; minimally 3.
        */

        copysize = oldsize - SIZE_SZ;
        s = (INTERNAL_SIZE_T*)oldmem;
        d = (INTERNAL_SIZE_T*)(chunk2mem(newp));
        ncopies = copysize / sizeof(INTERNAL_SIZE_T);
        assert(ncopies >= 3);

        if (ncopies > 9)
          MALLOC_COPY(d, s, copysize);

        else {
          *(d+0) = *(s+0);
          *(d+1) = *(s+1);
          *(d+2) = *(s+2);
          if (ncopies > 4) {
            *(d+3) = *(s+3);
            *(d+4) = *(s+4);
            if (ncopies > 6) {
              *(d+5) = *(s+5);
              *(d+6) = *(s+6);
              if (ncopies > 8) {
                *(d+7) = *(s+7);
                *(d+8) = *(s+8);
              }
            }
          }
        }

        /* newsize set to -1 above if need to free after copy */
        if (newsize == -1)
          fREe(oldmem);

        /* newsize set < 0 above if should not split after copy */
        if ((long)newsize < 0) {
          check_inuse_chunk(newp);
          return chunk2mem(newp);
        }
      }

    }

    assert((long)newsize >= (long)nb);

    /* If possible, free extra space in old or extended chunk */

    if (newsize - nb >= MINSIZE) { /* split off remainder */
      remainder = chunk_at_offset(newp, nb);
      remainder_size = newsize - nb;
      set_head_size(newp, nb);
      set_head(remainder, remainder_size | PREV_INUSE);
      set_inuse_bit_at_offset(remainder, remainder_size);
      fREe(chunk2mem(remainder)); /* let free() deal with it */
    }

    else { /* not enough extra to split off */
      set_head_size(newp, newsize);
      set_inuse_bit_at_offset(newp, newsize);
    }

    check_inuse_chunk(newp);
    return chunk2mem(newp);
  }

  /*
    Handle mmap cases
  */

  else {
#if HAVE_MMAP

    INTERNAL_SIZE_T offset = oldp->prev_size;
    int ret;

#if HAVE_MREMAP
    size_t pagemask = av->pagesize - 1;
    char *cp;
    unsigned long sum;
    
    /* Note the extra SIZE_SZ overhead */
    newsize = (nb + offset + SIZE_SZ + pagemask) & ~pagemask;

    /* don't need to remap if still within same page */
    if (oldsize == newsize - offset) 
      return oldmem;

    cp = (char*)mremap((char*)oldp - offset, oldsize + offset, newsize, 1);
    
    if (cp != (char*)MORECORE_FAILURE) {

      newp = (mchunkptr)(cp + offset);
      set_head(newp, (newsize - offset)|IS_MMAPPED);
      
      assert(aligned_OK(chunk2mem(newp)));
      assert((newp->prev_size == offset));
      
      /* update statistics */
      sum = av->mmapped_mem += newsize - oldsize;
      if (sum > (unsigned long)(av->max_mmapped_mem)) 
        av->max_mmapped_mem = sum;
      sum += av->sbrked_mem;
      if (sum > (unsigned long)(av->max_total_mem)) 
        av->max_total_mem = sum;
      
      return chunk2mem(newp);
    }

#endif

    /* Note the extra SIZE_SZ overhead. */
    if (oldsize - SIZE_SZ >= nb)
      return oldmem; /* do nothing */
    else {
      /* Must alloc, copy, free. */
      newmem = mALLOc(nb - MALLOC_ALIGN_MASK);
      if (newmem != 0) {
        MALLOC_COPY(newmem, oldmem, oldsize - 2*SIZE_SZ);
        av->n_mmaps--;
        av->mmapped_mem -= (oldsize + offset);
        ret = munmap((char*)oldp - offset, oldsize + offset);
        /* munmap returns non-zero on failure */
        assert(ret == 0);
      }
      return newmem;
    }

#else 
    /* If !HAVE_MMAP, but chunk_is_mmapped, user must have overwritten mem */
    check_malloc_state();
    MALLOC_FAILURE_ACTION;
    return 0;
#endif
  }

}




/*
  memalign requests more than enough space from malloc, finds a spot
  within that chunk that meets the alignment request, and then
  possibly frees the leading and trailing space.
  
  Alignments must be powers of two. If the argument is not a
  power of two, the nearest greater power is used.
  
  8-byte alignment is guaranteed by normal malloc calls, so don't
  bother calling memalign with an argument of 8 or less.
  
  Overreliance on memalign is a sure way to fragment space.
*/


#if __STD_C
Void_t* mEMALIGn(size_t alignment, size_t bytes)
#else
Void_t* mEMALIGn(alignment, bytes) size_t alignment; size_t bytes;
#endif
{
  INTERNAL_SIZE_T nb;             /* padded  request size */
  char*           m;              /* memory returned by malloc call */
  mchunkptr       p;              /* corresponding chunk */
  char*           brk;            /* alignment point within p */
  mchunkptr       newp;           /* chunk to return */
  INTERNAL_SIZE_T newsize;        /* its size */
  INTERNAL_SIZE_T leadsize;       /* leading space befor alignment point */
  mchunkptr       remainder;      /* spare room at end to split off */
  long            remainder_size; /* its size */


  /* If need less alignment than we give anyway, just relay to malloc */

  if (alignment <= MALLOC_ALIGNMENT) return mALLOc(bytes);

  /* Otherwise, ensure that it is at least a minimum chunk size */

  if (alignment <  MINSIZE) alignment = MINSIZE;

  /* Make sure alignment is power of 2. 
     (This check is needed anyway when MINSIZE is not a power of 2)
  */
  if ((alignment & (alignment - 1)) != 0) {
    size_t a = MALLOC_ALIGNMENT * 2;
    while ((unsigned long)a < (unsigned long)alignment) a <<= 1;
    alignment = a;
  }

  checked_request2size(bytes, nb);

  /* Call malloc with worst case padding to hit alignment. */

  m  = (char*)(mALLOc(nb + alignment + MINSIZE));

  if (m == 0) return 0; /* propagate failure */

  p = mem2chunk(m);

  if ((((unsigned long)(m)) % alignment) != 0) { /* misaligned */

    /*
      Find an aligned spot inside chunk.  Since we need to give back
      leading space in a chunk of at least MINSIZE, if the first
      calculation places us at a spot with less than MINSIZE leader,
      we can move to the next aligned spot -- we've allocated enough
      total room so that this is always possible.
    */

    brk = (char*)mem2chunk(((unsigned long)(m + alignment - 1)) &
                           -((signed) alignment));
    if ((long)(brk - (char*)(p)) < (long)MINSIZE)
      brk = brk + alignment;

    newp = (mchunkptr)brk;
    leadsize = brk - (char*)(p);
    newsize = chunksize(p) - leadsize;

    if (chunk_is_mmapped(p)) {
      newp->prev_size = p->prev_size + leadsize;
      set_head(newp, newsize|IS_MMAPPED);
      return chunk2mem(newp);
    }

    /* give back leader, use the rest */

    set_head(newp, newsize | PREV_INUSE);
    set_inuse_bit_at_offset(newp, newsize);
    set_head_size(p, leadsize);
    fREe(chunk2mem(p));
    p = newp;

    assert (newsize >= nb &&
            (((unsigned long)(chunk2mem(p))) % alignment) == 0);
  }

  /* Also give back spare room at the end */
  if (!chunk_is_mmapped(p)) {

    remainder_size = chunksize(p) - nb;

    if (remainder_size >= (long)MINSIZE) {
      remainder = chunk_at_offset(p, nb);
      set_head(remainder, remainder_size | PREV_INUSE);
      set_head_size(p, nb);
      fREe(chunk2mem(remainder));
    }
  }

  check_inuse_chunk(p);
  return chunk2mem(p);

}




/*
  calloc calls malloc, then zeroes out the allocated chunk.
*/

#if __STD_C
Void_t* cALLOc(size_t n_elements, size_t elem_size)
#else
Void_t* cALLOc(n_elements, elem_size) size_t n_elements; size_t elem_size;
#endif
{
  mchunkptr p;
  INTERNAL_SIZE_T clearsize;
  int nclears;
  INTERNAL_SIZE_T* d;

  Void_t* mem = mALLOc(n_elements * elem_size);

  if (mem != 0) {
    p = mem2chunk(mem);
    if (!chunk_is_mmapped(p)) {  /* don't need to clear mmapped space */

      /*
        Unroll clear of <= 36 bytes (72 if 8byte sizes)
        We know that contents have an odd number of
        INTERNAL_SIZE_T-sized words; minimally 3.
      */

      d = (INTERNAL_SIZE_T*)mem;
      clearsize = chunksize(p) - SIZE_SZ;
      nclears = clearsize / sizeof(INTERNAL_SIZE_T);
      assert(nclears >= 3);

      if (nclears > 9)
        MALLOC_ZERO(d, clearsize);

      else {
        *(d+0) = 0;
        *(d+1) = 0;
        *(d+2) = 0;
        if (nclears > 4) {
          *(d+3) = 0;
          *(d+4) = 0;
          if (nclears > 6) {
            *(d+5) = 0;
            *(d+6) = 0;
            if (nclears > 8) {
              *(d+7) = 0;
              *(d+8) = 0;
            }
          }
        }
      }
    }
  }
  return mem;
}


/*
  cfree just calls free. It is needed/defined on some systems
  that pair it with calloc, presumably for odd historical reasons
  (such as: cfree is used in example code in first edition of K&R).
*/

#if __STD_C
void cFREe(Void_t *mem)
#else
void cFREe(mem) Void_t *mem;
#endif
{
  fREe(mem);
}





/*
  valloc just invokes memalign with alignment argument equal
  to the page size of the system (or as near to this as can
  be figured out from all the includes/defines above.)
*/

#if __STD_C
Void_t* vALLOc(size_t bytes)
#else
Void_t* vALLOc(bytes) size_t bytes;
#endif
{
  /* Ensure initialization/consolidation */
  malloc_consolidate();
  return mEMALIGn(get_malloc_state()->pagesize, bytes);
}

/*
  pvalloc just invokes valloc for the nearest pagesize
  that will accommodate request
*/


#if __STD_C
Void_t* pVALLOc(size_t bytes)
#else
Void_t* pVALLOc(bytes) size_t bytes;
#endif
{
  size_t pagesz;

  /* Ensure initialization/consolidation */
  malloc_consolidate();

  pagesz = get_malloc_state()->pagesize;
  return mEMALIGn(pagesz, (bytes + pagesz - 1) & ~(pagesz - 1));
}



/*
  malloc_usable_size tells you how many bytes you can actually use in
  an allocated chunk, which may be more than you requested (although
  often not). You can use this many bytes without worrying about
  overwriting other allocated objects. Not a particularly great
  programming practice, but still sometimes useful.
*/

#if __STD_C
size_t dlmalloc_usable_size(Void_t* mem)
#else
size_t dlmalloc_usable_size(mem) Void_t* mem;
#endif
{
  mchunkptr p;
  if (mem != 0) {
    p = mem2chunk(mem);
    if (chunk_is_mmapped(p))
      return chunksize(p) - 2*SIZE_SZ;
    else if (inuse(p))
      return chunksize(p) - SIZE_SZ;
  }
  return 0;
}




/*
  mallinfo returns a copy of updated current mallinfo.
*/

struct mallinfo mALLINFo()
{
  mstate av = get_malloc_state();
  struct mallinfo mi;
  int i;
  mbinptr b;
  mchunkptr p;
  INTERNAL_SIZE_T avail;
  int navail;
  int nfastblocks;

  /* Ensure initialization */
  if (av->top == 0)  malloc_consolidate();

  check_malloc_state();

  avail = chunksize(av->top) + av->fastbytes;
  navail = 1;  /* for top */
  nfastblocks = 0;

  for (i = fastbin_index(av->max_fast); i >= 0; --i) {
    for (p = (av->fastbins[i]).hd; p != 0; p = p->fd) {
      ++nfastblocks;
    }
  }

  for (i = 1; i < NBINS; ++i) {
    b = bin_at(av, i);
    for (p = last(b); p != b; p = p->bk) {
      avail += chunksize(p);
      navail++;
    }
  }

  mi.smblks = nfastblocks;
  mi.ordblks = navail;
  mi.fordblks = avail;
  mi.uordblks = av->sbrked_mem - avail;
  mi.arena = av->sbrked_mem;
  mi.hblks = av->n_mmaps;
  mi.hblkhd = av->mmapped_mem;
  mi.fsmblks = av->fastbytes;
  mi.keepcost = chunksize(av->top);
  mi.usmblks = av->max_total_mem;
  return mi;
}



/*
  malloc_stats prints on stderr the amount of space obtained from the
  system (both via sbrk and mmap), the maximum amount (which may be
  more than current if malloc_trim and/or munmap got called), and the
  current number of bytes allocated via malloc (or realloc, etc) but
  not yet freed. Note that this is the number of bytes allocated, not
  the number requested. It will be larger than the number requested
  because of alignment and bookkeeping overhead. Because it includes
  alignment wastage as being in use, this figure may be greater than zero 
  even when no user-level chunks are allocated.

  The reported current and maximum system memory can be inaccurate if
  a program makes other calls to system memory allocation functions
  (normally sbrk) outside of malloc.

  malloc_stats prints only the most commonly interesting statistics.
  More information can be obtained by calling mallinfo.
*/

void mSTATs()
{
  struct mallinfo mi = mALLINFo();

  fprintf(stderr, "max system bytes = %10lu\n",
          (unsigned long)(mi.usmblks));
  fprintf(stderr, "system bytes     = %10lu\n",
          (unsigned long)(mi.arena + mi.hblkhd));
  fprintf(stderr, "in use bytes     = %10lu\n",
          (unsigned long)(mi.uordblks + mi.hblkhd));
}



/*
  mallopt is the general SVID/XPG interface to tunable parameters.
  The format is to provide a (parameter-number, parameter-value)
  pair.  mallopt then sets the corresponding parameter to the
  argument value if it can (i.e., so long as the value is
  meaningful), and returns 1 if successful else 0.  See descriptions
  of tunable parameters above for meanings.
*/

#if __STD_C
int mALLOPt(int param_number, int value)
#else
int mALLOPt(param_number, value) int param_number; int value;
#endif
{
  mstate av = get_malloc_state();
  /* Ensure initialization/consolidation */
  malloc_consolidate();

  switch(param_number) {
  case M_MXFAST:
    if (value >= 0 && value <= MAX_FAST_SIZE) {
      av->max_fast = (value == 0)? 0: request2size(value);
      clear_consolidation_flag(av);
      return 1;
    }
    else
      return 0;

  case M_TRIM_THRESHOLD:
    av->trim_threshold = value;
    return 1;

  case M_TOP_PAD:
    av->top_pad = value;
    return 1;

  case M_MMAP_THRESHOLD:
    av->mmap_threshold = value;
    return 1;

  case M_MMAP_MAX:
#if HAVE_MMAP
    av->n_mmaps_max = value;
    return 1;
#else
    if (value != 0)
      return 0;
    else {
      av->n_mmaps_max = value;
      return 1;
    }
#endif

  default:
    return 0;
  }
}

/*

History:

    V2.7.0
      * memalign: check alignment arg
      * Collect all cases in malloc requiring system memory into sYSMALLOc
      * Use mmap as backup to sbrk, if available; fold these mmap-related 
        operations into others.
      * Place all internal state in malloc_state
      * Introduce fastbins (although similar to 2.5.1)
      * Many minor tunings and cosmetic improvements
      * Introduce USE_PUBLIC_MALLOC_WRAPPERS, USE_PTHREAD_LOCKS 
      * Introduce MALLOC_FAILURE_ACTION, MORECORE_CONTIGUOUS
        Thanks to Tony E. Bennett <tbennett@nvidia.com> and others.
      * Adjust request2size to fit with MALLOC_FAILURE_ACTION.
      * Include errno.h to support default failure action.
      * Further improve WIN32 'sbrk()' emulation's 'findRegion()' routine
        to avoid infinite loop when allocating initial memory larger
        than RESERVED_SIZE and/or subsequent memory larger than
        NEXT_SIZE.  Thanks to Andreas Mueller <a.mueller at paradatec.de>
        for finding this one.

    V2.6.6 Sun Dec  5 07:42:19 1999  Doug Lea  (dl at gee)
      * return null for negative arguments
      * Added Several WIN32 cleanups from Martin C. Fong <mcfong at yahoo.com>
         * Add 'LACKS_SYS_PARAM_H' for those systems without 'sys/param.h'
          (e.g. WIN32 platforms)
         * Cleanup header file inclusion for WIN32 platforms
         * Cleanup code to avoid Microsoft Visual C++ compiler complaints
         * Add 'USE_DL_PREFIX' to quickly allow co-existence with existing
           memory allocation routines
         * Set 'malloc_getpagesize' for WIN32 platforms (needs more work)
         * Use 'assert' rather than 'ASSERT' in WIN32 code to conform to
           usage of 'assert' in non-WIN32 code
         * Improve WIN32 'sbrk()' emulation's 'findRegion()' routine to
           avoid infinite loop
      * Always call 'fREe()' rather than 'free()'

    V2.6.5 Wed Jun 17 15:57:31 1998  Doug Lea  (dl at gee)
      * Fixed ordering problem with boundary-stamping

    V2.6.3 Sun May 19 08:17:58 1996  Doug Lea  (dl at gee)
      * Added pvalloc, as recommended by H.J. Liu
      * Added 64bit pointer support mainly from Wolfram Gloger
      * Added anonymously donated WIN32 sbrk emulation
      * Malloc, calloc, getpagesize: add optimizations from Raymond Nijssen
      * malloc_extend_top: fix mask error that caused wastage after
        foreign sbrks
      * Add linux mremap support code from HJ Liu

    V2.6.2 Tue Dec  5 06:52:55 1995  Doug Lea  (dl at gee)
      * Integrated most documentation with the code.
      * Add support for mmap, with help from
        Wolfram Gloger (Gloger@lrz.uni-muenchen.de).
      * Use last_remainder in more cases.
      * Pack bins using idea from  colin@nyx10.cs.du.edu
      * Use ordered bins instead of best-fit threshhold
      * Eliminate block-local decls to simplify tracing and debugging.
      * Support another case of realloc via move into top
      * Fix error occuring when initial sbrk_base not word-aligned.
      * Rely on page size for units instead of SBRK_UNIT to
        avoid surprises about sbrk alignment conventions.
      * Add mallinfo, mallopt. Thanks to Raymond Nijssen
        (raymond@es.ele.tue.nl) for the suggestion.
      * Add `pad' argument to malloc_trim and top_pad mallopt parameter.
      * More precautions for cases where other routines call sbrk,
        courtesy of Wolfram Gloger (Gloger@lrz.uni-muenchen.de).
      * Added macros etc., allowing use in linux libc from
        H.J. Lu (hjl@gnu.ai.mit.edu)
      * Inverted this history list

    V2.6.1 Sat Dec  2 14:10:57 1995  Doug Lea  (dl at gee)
      * Re-tuned and fixed to behave more nicely with V2.6.0 changes.
      * Removed all preallocation code since under current scheme
        the work required to undo bad preallocations exceeds
        the work saved in good cases for most test programs.
      * No longer use return list or unconsolidated bins since
        no scheme using them consistently outperforms those that don't
        given above changes.
      * Use best fit for very large chunks to prevent some worst-cases.
      * Added some support for debugging

    V2.6.0 Sat Nov  4 07:05:23 1995  Doug Lea  (dl at gee)
      * Removed footers when chunks are in use. Thanks to
        Paul Wilson (wilson@cs.texas.edu) for the suggestion.

    V2.5.4 Wed Nov  1 07:54:51 1995  Doug Lea  (dl at gee)
      * Added malloc_trim, with help from Wolfram Gloger
        (wmglo@Dent.MED.Uni-Muenchen.DE).

    V2.5.3 Tue Apr 26 10:16:01 1994  Doug Lea  (dl at g)

    V2.5.2 Tue Apr  5 16:20:40 1994  Doug Lea  (dl at g)
      * realloc: try to expand in both directions
      * malloc: swap order of clean-bin strategy;
      * realloc: only conditionally expand backwards
      * Try not to scavenge used bins
      * Use bin counts as a guide to preallocation
      * Occasionally bin return list chunks in first scan
      * Add a few optimizations from colin@nyx10.cs.du.edu

    V2.5.1 Sat Aug 14 15:40:43 1993  Doug Lea  (dl at g)
      * faster bin computation & slightly different binning
      * merged all consolidations to one part of malloc proper
         (eliminating old malloc_find_space & malloc_clean_bin)
      * Scan 2 returns chunks (not just 1)
      * Propagate failure in realloc if malloc returns 0
      * Add stuff to allow compilation on non-ANSI compilers
          from kpv@research.att.com

    V2.5 Sat Aug  7 07:41:59 1993  Doug Lea  (dl at g.oswego.edu)
      * removed potential for odd address access in prev_chunk
      * removed dependency on getpagesize.h
      * misc cosmetics and a bit more internal documentation
      * anticosmetics: mangled names in macros to evade debugger strangeness
      * tested on sparc, hp-700, dec-mips, rs6000
          with gcc & native cc (hp, dec only) allowing
          Detlefs & Zorn comparison study (in SIGPLAN Notices.)

    Trial version Fri Aug 28 13:14:29 1992  Doug Lea  (dl at g.oswego.edu)
      * Based loosely on libg++-1.2X malloc. (It retains some of the overall
         structure of old version,  but most details differ.)

*/


