/*
 *  Blackfin Cache Manager Support
 *
 *  $Id: cache_.h,v 1.1 2008/08/15 20:18:40 joel Exp $
 */

#ifndef _cache__h_
#define _cache__h_

#define CPU_DATA_CACHE_ALIGNMENT          32
#define CPU_INSTRUCTION_CACHE_ALIGNMENT   32

#ifdef BSP_DATA_CACHE_CONFIG
#define LIBCPU_DATA_CACHE_CONFIG BSP_DATA_CACHE_CONFIG
#else
/* use 16K of each SRAM bank */
#define LIBCPU_DATA_CACHE_CONFIG (3 << DMEM_CONTROL_DMC_SHIFT)
#endif

#endif /* _cache__h_ */

