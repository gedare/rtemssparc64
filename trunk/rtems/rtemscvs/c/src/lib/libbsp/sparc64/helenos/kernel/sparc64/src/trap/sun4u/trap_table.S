#
# Copyright (c) 2005 Jakub Jermar
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# - Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# - The name of the author may not be used to endorse or promote products
#   derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

/**
 * @file
 * @brief This file contains kernel trap table.
 */

/*
 * This file is heavily modified to support bootstrapping sun4u on RTEMS.
 * Most traps will simply spin in place. This isn't particularly great, 
 * but shouldn't be too hard to debug problems
 *
 */

#include <rtems/asm.h>

.register %g2, #scratch
.register %g3, #scratch

#define BAD_TRAP \
0:  nop; \
  ba 0b; \
  nop; 


.macro PREEMPTIBLE_HANDLER f
  call \f
  nop
.endm

/* from syscall.h */
#define TT_TRAP_INSTRUCTION_0   0x100

.text

#include <arch/trap/exception.h>
#include <arch/trap/interrupt.h>
#include <arch/trap/regwin.h>
#include <arch/trap/mmu.h>
#include <arch/mm/mmu.h>
#include <arch/mm/page.h>
#include <arch/stack.h>
#include <arch/regdef.h>

#include <traptable.h>

/*
 * Kernel trap table.
 */
.align TABLE_SIZE
.global trap_table
trap_table:

/* TT = 0x08, TL = 0, instruction_access_exception */
.org trap_table + TT_INSTRUCTION_ACCESS_EXCEPTION*ENTRY_SIZE
.global instruction_access_exception_tl0
instruction_access_exception_tl0:
  BAD_TRAP

/* TT = 0x0a, TL = 0, instruction_access_error */
.org trap_table + TT_INSTRUCTION_ACCESS_ERROR*ENTRY_SIZE
.global instruction_access_error_tl0
instruction_access_error_tl0:
  BAD_TRAP

/* TT = 0x10, TL = 0, illegal_instruction */
.org trap_table + TT_ILLEGAL_INSTRUCTION*ENTRY_SIZE
.global illegal_instruction_tl0
illegal_instruction_tl0:
  BAD_TRAP

/* TT = 0x11, TL = 0, privileged_opcode */
.org trap_table + TT_PRIVILEGED_OPCODE*ENTRY_SIZE
.global privileged_opcode_tl0
privileged_opcode_tl0:
	BAD_TRAP

/* TT = 0x12, TL = 0, unimplemented_LDD */
.org trap_table + TT_UNIMPLEMENTED_LDD*ENTRY_SIZE
.global unimplemented_LDD_tl0
unimplemented_LDD_tl0:
	BAD_TRAP

/* TT = 0x13, TL = 0, unimplemented_STD */
.org trap_table + TT_UNIMPLEMENTED_STD*ENTRY_SIZE
.global unimplemented_STD_tl0
unimplemented_STD_tl0:
  BAD_TRAP	

/* TT = 0x20, TL = 0, fb_disabled handler */
.org trap_table + TT_FP_DISABLED*ENTRY_SIZE
.global fb_disabled_tl0
fp_disabled_tl0:
	BAD_TRAP

/* TT = 0x21, TL = 0, fb_exception_ieee_754 handler */
.org trap_table + TT_FP_EXCEPTION_IEEE_754*ENTRY_SIZE
.global fb_exception_ieee_754_tl0
fp_exception_ieee_754_tl0:
	BAD_TRAP

/* TT = 0x22, TL = 0, fb_exception_other handler */
.org trap_table + TT_FP_EXCEPTION_OTHER*ENTRY_SIZE
.global fb_exception_other_tl0
fp_exception_other_tl0:
	BAD_TRAP

/* TT = 0x23, TL = 0, tag_overflow */
.org trap_table + TT_TAG_OVERFLOW*ENTRY_SIZE
.global tag_overflow_tl0
tag_overflow_tl0:
	BAD_TRAP

/* TT = 0x24, TL = 0, clean_window handler */
.org trap_table + TT_CLEAN_WINDOW*ENTRY_SIZE
.global clean_window_tl0
clean_window_tl0:
	CLEAN_WINDOW_HANDLER

/* TT = 0x28, TL = 0, division_by_zero */
.org trap_table + TT_DIVISION_BY_ZERO*ENTRY_SIZE
.global division_by_zero_tl0
division_by_zero_tl0:
	BAD_TRAP

/* TT = 0x30, TL = 0, data_access_exception */
.org trap_table + TT_DATA_ACCESS_EXCEPTION*ENTRY_SIZE
.global data_access_exception_tl0
data_access_exception_tl0:
  BAD_TRAP

/* TT = 0x32, TL = 0, data_access_error */
.org trap_table + TT_DATA_ACCESS_ERROR*ENTRY_SIZE
.global data_access_error_tl0
data_access_error_tl0:
	BAD_TRAP

/* TT = 0x34, TL = 0, mem_address_not_aligned */
.org trap_table + TT_MEM_ADDRESS_NOT_ALIGNED*ENTRY_SIZE
.global mem_address_not_aligned_tl0
mem_address_not_aligned_tl0:
	BAD_TRAP

/* TT = 0x35, TL = 0, LDDF_mem_address_not_aligned */
.org trap_table + TT_LDDF_MEM_ADDRESS_NOT_ALIGNED*ENTRY_SIZE
.global LDDF_mem_address_not_aligned_tl0
LDDF_mem_address_not_aligned_tl0:
	BAD_TRAP

/* TT = 0x36, TL = 0, STDF_mem_address_not_aligned */
.org trap_table + TT_STDF_MEM_ADDRESS_NOT_ALIGNED*ENTRY_SIZE
.global STDF_mem_address_not_aligned_tl0
STDF_mem_address_not_aligned_tl0:
	BAD_TRAP

/* TT = 0x37, TL = 0, privileged_action */
.org trap_table + TT_PRIVILEGED_ACTION*ENTRY_SIZE
.global privileged_action_tl0
privileged_action_tl0:
	BAD_TRAP

/* TT = 0x38, TL = 0, LDQF_mem_address_not_aligned */
.org trap_table + TT_LDQF_MEM_ADDRESS_NOT_ALIGNED*ENTRY_SIZE
.global LDQF_mem_address_not_aligned_tl0
LDQF_mem_address_not_aligned_tl0:
	BAD_TRAP

/* TT = 0x39, TL = 0, STQF_mem_address_not_aligned */
.org trap_table + TT_STQF_MEM_ADDRESS_NOT_ALIGNED*ENTRY_SIZE
.global STQF_mem_address_not_aligned_tl0
STQF_mem_address_not_aligned_tl0:
	BAD_TRAP

/* TT = 0x41, TL = 0, interrupt_level_1 handler */
.org trap_table + TT_INTERRUPT_LEVEL_1*ENTRY_SIZE
.global interrupt_level_1_handler_tl0
interrupt_level_1_handler_tl0:
	BAD_TRAP

/* TT = 0x42, TL = 0, interrupt_level_2 handler */
.org trap_table + TT_INTERRUPT_LEVEL_2*ENTRY_SIZE
.global interrupt_level_2_handler_tl0
interrupt_level_2_handler_tl0:
	BAD_TRAP

/* TT = 0x43, TL = 0, interrupt_level_3 handler */
.org trap_table + TT_INTERRUPT_LEVEL_3*ENTRY_SIZE
.global interrupt_level_3_handler_tl0
interrupt_level_3_handler_tl0:
	BAD_TRAP

/* TT = 0x44, TL = 0, interrupt_level_4 handler */
.org trap_table + TT_INTERRUPT_LEVEL_4*ENTRY_SIZE
.global interrupt_level_4_handler_tl0
interrupt_level_4_handler_tl0:
	BAD_TRAP

/* TT = 0x45, TL = 0, interrupt_level_5 handler */
.org trap_table + TT_INTERRUPT_LEVEL_5*ENTRY_SIZE
.global interrupt_level_5_handler_tl0
interrupt_level_5_handler_tl0:
	BAD_TRAP

/* TT = 0x46, TL = 0, interrupt_level_6 handler */
.org trap_table + TT_INTERRUPT_LEVEL_6*ENTRY_SIZE
.global interrupt_level_6_handler_tl0
interrupt_level_6_handler_tl0:
	BAD_TRAP

/* TT = 0x47, TL = 0, interrupt_level_7 handler */
.org trap_table + TT_INTERRUPT_LEVEL_7*ENTRY_SIZE
.global interrupt_level_7_handler_tl0
interrupt_level_7_handler_tl0:
	BAD_TRAP

/* TT = 0x48, TL = 0, interrupt_level_8 handler */
.org trap_table + TT_INTERRUPT_LEVEL_8*ENTRY_SIZE
.global interrupt_level_8_handler_tl0
interrupt_level_8_handler_tl0:
	BAD_TRAP

/* TT = 0x49, TL = 0, interrupt_level_9 handler */
.org trap_table + TT_INTERRUPT_LEVEL_9*ENTRY_SIZE
.global interrupt_level_9_handler_tl0
interrupt_level_9_handler_tl0:
	BAD_TRAP

/* TT = 0x4a, TL = 0, interrupt_level_10 handler */
.org trap_table + TT_INTERRUPT_LEVEL_10*ENTRY_SIZE
.global interrupt_level_10_handler_tl0
interrupt_level_10_handler_tl0:
	BAD_TRAP

/* TT = 0x4b, TL = 0, interrupt_level_11 handler */
.org trap_table + TT_INTERRUPT_LEVEL_11*ENTRY_SIZE
.global interrupt_level_11_handler_tl0
interrupt_level_11_handler_tl0:
  BAD_TRAP

/* TT = 0x4c, TL = 0, interrupt_level_12 handler */
.org trap_table + TT_INTERRUPT_LEVEL_12*ENTRY_SIZE
.global interrupt_level_12_handler_tl0
interrupt_level_12_handler_tl0:
	BAD_TRAP

/* TT = 0x4d, TL = 0, interrupt_level_13 handler */
.org trap_table + TT_INTERRUPT_LEVEL_13*ENTRY_SIZE
.global interrupt_level_13_handler_tl0
interrupt_level_13_handler_tl0:
	BAD_TRAP

/* TT = 0x4e, TL = 0, interrupt_level_14 handler */
.org trap_table + TT_INTERRUPT_LEVEL_14*ENTRY_SIZE
.global interrupt_level_14_handler_tl0
interrupt_level_14_handler_tl0:
	BAD_TRAP

/* TT = 0x4f, TL = 0, interrupt_level_15 handler */
.org trap_table + TT_INTERRUPT_LEVEL_15*ENTRY_SIZE
.global interrupt_level_15_handler_tl0
interrupt_level_15_handler_tl0:
	BAD_TRAP

/* TT = 0x60, TL = 0, interrupt_vector_trap handler */
.org trap_table + TT_INTERRUPT_VECTOR_TRAP*ENTRY_SIZE
.global interrupt_vector_trap_handler_tl0
interrupt_vector_trap_handler_tl0:
	BAD_TRAP

/* TT = 0x64, TL = 0, fast_instruction_access_MMU_miss */
.org trap_table + TT_FAST_INSTRUCTION_ACCESS_MMU_MISS*ENTRY_SIZE
.global fast_instruction_access_mmu_miss_handler_tl0
fast_instruction_access_mmu_miss_handler_tl0:
/*  call fast_instruction_access_mmu_miss */
  BAD_TRAP

/* TT = 0x68, TL = 0, fast_data_access_MMU_miss */
.org trap_table + TT_FAST_DATA_ACCESS_MMU_MISS*ENTRY_SIZE
.global fast_data_access_mmu_miss_handler_tl0
fast_data_access_mmu_miss_handler_tl0:
  /* TODO: fast_data_access_mmu_miss */
  FAST_DATA_ACCESS_MMU_MISS_HANDLER 0

/* TT = 0x6c, TL = 0, fast_data_access_protection */
.org trap_table + TT_FAST_DATA_ACCESS_PROTECTION*ENTRY_SIZE
.global fast_data_access_protection_handler_tl0
fast_data_access_protection_handler_tl0:
/*	call fast_data_access_protection */
  BAD_TRAP

/* TT = 0x80, TL = 0, spill_0_normal handler */
.org trap_table + TT_SPILL_0_NORMAL*ENTRY_SIZE
.global spill_0_normal_tl0
spill_0_normal_tl0:
	SPILL_ANY

/* TT = 0x84, TL = 0, spill_1_normal handler */
.org trap_table + TT_SPILL_1_NORMAL*ENTRY_SIZE
.global spill_1_normal_tl0
spill_1_normal_tl0:
	SPILL_ANY

/* TT = 0x88, TL = 0, spill_2_normal handler */
.org trap_table + TT_SPILL_2_NORMAL*ENTRY_SIZE
.global spill_2_normal_tl0
spill_2_normal_tl0:
	SPILL_ANY

/* TT = 0xa0, TL = 0, spill_0_other handler */
.org trap_table + TT_SPILL_0_OTHER*ENTRY_SIZE
.global spill_0_other_tl0
spill_0_other_tl0:
	SPILL_ANY

/* TT = 0xc0, TL = 0, fill_0_normal handler */
.org trap_table + TT_FILL_0_NORMAL*ENTRY_SIZE
.global fill_0_normal_tl0
fill_0_normal_tl0:
	FILL_ANY

/* TT = 0xc4, TL = 0, fill_1_normal handler */
.org trap_table + TT_FILL_1_NORMAL*ENTRY_SIZE
.global fill_1_normal_tl0
fill_1_normal_tl0:
	FILL_ANY

/* TT = 0x100 - 0x17f, TL = 0, trap_instruction_0 - trap_instruction_7f */
.irp cur, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,\
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,\
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,\
    58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,\
    77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,\
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,\
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,\
    127
.org trap_table + (TT_TRAP_INSTRUCTION_0+\cur)*ENTRY_SIZE
.global trap_instruction_\cur\()_tl0
trap_instruction_\cur\()_tl0:
	BAD_TRAP
.endr

/*
 * Handlers for TL>0.
 */

/* TT = 0x08, TL > 0, instruction_access_exception */
.org trap_table + (TT_INSTRUCTION_ACCESS_EXCEPTION+512)*ENTRY_SIZE
.global instruction_access_exception_tl1
instruction_access_exception_tl1:
  BAD_TRAP

/* TT = 0x0a, TL > 0, instruction_access_error */
.org trap_table + (TT_INSTRUCTION_ACCESS_ERROR+512)*ENTRY_SIZE
.global instruction_access_error_tl1
instruction_access_error_tl1:
  BAD_TRAP

/* TT = 0x10, TL > 0, illegal_instruction */
.org trap_table + (TT_ILLEGAL_INSTRUCTION+512)*ENTRY_SIZE
.global illegal_instruction_tl1
illegal_instruction_tl1:
  BAD_TRAP

/* TT = 0x24, TL > 0, clean_window handler */
.org trap_table + (TT_CLEAN_WINDOW+512)*ENTRY_SIZE
.global clean_window_tl1
clean_window_tl1:
	CLEAN_WINDOW_HANDLER

/* TT = 0x28, TL > 0, division_by_zero */
.org trap_table + (TT_DIVISION_BY_ZERO+512)*ENTRY_SIZE
.global division_by_zero_tl1
division_by_zero_tl1:
	BAD_TRAP

/* TT = 0x30, TL > 0, data_access_exception */
.org trap_table + (TT_DATA_ACCESS_EXCEPTION+512)*ENTRY_SIZE
.global data_access_exception_tl1
data_access_exception_tl1:
	BAD_TRAP

/* TT = 0x32, TL > 0, data_access_error */
.org trap_table + (TT_DATA_ACCESS_ERROR+512)*ENTRY_SIZE
.global data_access_error_tl1
data_access_error_tl1:
	BAD_TRAP

/* TT = 0x34, TL > 0, mem_address_not_aligned */
.org trap_table + (TT_MEM_ADDRESS_NOT_ALIGNED+512)*ENTRY_SIZE
.global mem_address_not_aligned_tl1
mem_address_not_aligned_tl1:
	BAD_TRAP

/* TT = 0x68, TL > 0, fast_data_access_MMU_miss */
.org trap_table + (TT_FAST_DATA_ACCESS_MMU_MISS+512)*ENTRY_SIZE
.global fast_data_access_mmu_miss_handler_tl1
fast_data_access_mmu_miss_handler_tl1:
/*	call fast_data_access_mmu_miss*/
  BAD_TRAP

/* TT = 0x6c, TL > 0, fast_data_access_protection */
.org trap_table + (TT_FAST_DATA_ACCESS_PROTECTION+512)*ENTRY_SIZE
.global fast_data_access_protection_handler_tl1
fast_data_access_protection_handler_tl1:
/*  call fast_data_access_protection	*/
  BAD_TRAP

/* TT = 0x80, TL > 0, spill_0_normal handler */
.org trap_table + (TT_SPILL_0_NORMAL+512)*ENTRY_SIZE
.global spill_0_normal_tl1
spill_0_normal_tl1:
	SPILL_ANY

/* TT = 0x88, TL > 0, spill_2_normal handler */
.org trap_table + (TT_SPILL_2_NORMAL+512)*ENTRY_SIZE
.global spill_2_normal_tl1
spill_2_normal_tl1:
	SPILL_ANY

/* TT = 0xa0, TL > 0, spill_0_other handler */
.org trap_table + (TT_SPILL_0_OTHER+512)*ENTRY_SIZE
.global spill_0_other_tl1
spill_0_other_tl1:
	SPILL_ANY

/* TT = 0xc0, TL > 0, fill_0_normal handler */
.org trap_table + (TT_FILL_0_NORMAL+512)*ENTRY_SIZE
.global fill_0_normal_tl1
fill_0_normal_tl1:
	FILL_ANY

