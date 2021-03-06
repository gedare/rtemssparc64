#
# Copyright (c) 2006 Martin Decky
# Copyright (c) 2006 Jakub Jermar 
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

#include <stack.h>
#include <register.h>

.register       %g2, #scratch
.register       %g3, #scratch

.text

.global halt
.global memcpy
.global jump_to_kernel

halt:
	b halt
	nop
	
memcpy:
	mov	%o0, %o3		! save dst
	add	%o1, 7, %g1
	and	%g1, -8, %g1
	cmp	%o1, %g1
	be,pn	%xcc, 3f
	add	%o0, 7, %g1
	mov	0, %g3
0:
	brz,pn	%o2, 2f
	mov	0, %g2
1:
	ldub	[%g3 + %o1], %g1
	add	%g2, 1, %g2
	cmp	%o2, %g2
	stb	%g1, [%g3 + %o0]
	bne,pt	%xcc, 1b
	mov	%g2, %g3
2:
	jmp	%o7 + 8			! exit point
	mov	%o3, %o0
3:
	and	%g1, -8, %g1
	cmp	%o0, %g1
	bne,pt	%xcc, 0b
	mov	0, %g3
	srlx	%o2, 3, %g4
	brz,pn	%g4, 5f
	mov	0, %g5
4:
	sllx	%g3, 3, %g2
	add	%g5, 1, %g3
	ldx	[%o1 + %g2], %g1
	mov	%g3, %g5
	cmp	%g4, %g3
	bne,pt	%xcc, 4b
	stx	%g1, [%o0 + %g2]
5:
	and	%o2, 7, %o2
	brz,pn	%o2, 2b
	sllx	%g4, 3, %g1
	mov	0, %g2
	add	%g1, %o0, %o0
	add	%g1, %o1, %g4
	mov	0, %g3
6:
	ldub	[%g2 + %g4], %g1
	stb	%g1, [%g2 + %o0]
	add	%g3, 1, %g2
	cmp	%o2, %g2
	bne,pt	%xcc, 6b
	mov	%g2, %g3

	jmp	%o7 + 8			! exit point
	mov	%o3, %o0

jump_to_kernel:
	/*
	 * We have copied code and now we need to guarantee cache coherence.
	 * 1. Make sure that the code we have moved has drained to main memory.
	 * 2. Invalidate I-cache.
	 * 3. Flush instruction pipeline.
	 */

#if defined (SUN4U)
	/*
	 * US3 processors have a write-invalidate cache, so explicitly
	 * invalidating it is not required. Whether to invalidate I-cache
	 * or not is decided according to the value of the global
	 * "subarchitecture" variable (set in the bootstrap).
	 */

	set subarchitecture, %g2
	ldub [%g2], %g2
	cmp %g2, 3
	be 1f
	nop
0:
	call icache_flush
	nop
#endif

1:
	membar #StoreStore
	
	/*
	 * Flush the instruction pipeline.
	 */
	flush	%i7

	mov %o0, %l1
	mov %o1, %o0
	mov %o2, %o1
	mov %o3, %o2
	jmp %l1				! jump to kernel
	nop

#define ICACHE_SIZE		8192
#define ICACHE_LINE_SIZE	32
#define ICACHE_SET_BIT		(1 << 13)
#define ASI_ICACHE_TAG		0x67

# Flush I-cache
icache_flush:
	set	((ICACHE_SIZE - ICACHE_LINE_SIZE) | ICACHE_SET_BIT), %g1
	stxa	%g0, [%g1] ASI_ICACHE_TAG
0:	membar	#Sync
	subcc	%g1, ICACHE_LINE_SIZE, %g1
	bnz,pt	%xcc, 0b
	stxa	%g0, [%g1] ASI_ICACHE_TAG
	membar	#Sync
	retl
	! SF Erratum #51
	nop
.global ofw
ofw:
	save %sp, -STACK_WINDOW_SAVE_AREA_SIZE, %sp
	set ofw_cif, %l0
	ldx [%l0], %l0

	rdpr  %pstate, %l1
	and  %l1, ~PSTATE_AM_BIT, %l2
	wrpr  %l2, 0, %pstate
    
	jmpl %l0, %o7
	mov %i0, %o0

	wrpr %l1, 0, %pstate

	ret
	restore %o0, 0, %o0
