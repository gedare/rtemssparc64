/*  cpu_asm.s
 *
 *  This file contains the basic algorithms for all assembly code used
 *  in an specific CPU port of RTEMS.  These algorithms must be implemented
 *  in assembly language. 
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  Ported to ERC32 implementation of the SPARC by On-Line Applications
 *  Research Corporation (OAR) under contract to the European Space 
 *  Agency (ESA).
 *
 *  ERC32 modifications of respective RTEMS file: COPYRIGHT (c) 1995. 
 *  European Space Agency.
 *
 *  $Id: cpu_asm.S,v 1.15 2009/03/12 14:16:50 joel Exp $
 */

#include <rtems/asm.h>


/* 
 *  The assembler needs to be told that we know what to do with 
 *  the global registers.
 */
.register %g2, #scratch
.register %g3, #scratch
.register %g6, #scratch
.register %g7, #scratch

#if (SPARC_HAS_FPU == 1)

/*
 *  void _CPU_Context_save_fp(
 *    void **fp_context_ptr
 *  )
 *
 *  This routine is responsible for saving the FP context
 *  at *fp_context_ptr.  If the point to load the FP context
 *  from is changed then the pointer is modified by this routine.
 *
 */

        .align 4
        PUBLIC(_CPU_Context_save_fp)
SYM(_CPU_Context_save_fp):
        save    %sp, -CPU_MINIMUM_STACK_FRAME_SIZE, %sp

        /*
         *  The following enables the floating point unit.
         */
  
        sparc64_enable_FPU(%l0)

        /*
         *  Although sun4v supports alternate register names for double-
         *  and quad-word floating point, SPARC v9 only uses f[#]
         *
         *  Because quad-word fp is not supported by the hardware in 
         *  many situations, we stick with double-word fp operations
         */
        ldx    [%i0], %l0		 
        std     %f0, [%l0]
        std     %f2, [%l0 + F2_OFFSET]
        std     %f4, [%l0 + F4_OFFSET]
        std     %f6, [%l0 + F6_OFFSET]
        std     %f8, [%l0 + F8_OFFSET]
        std     %f10, [%l0 + F1O_OFFSET]
        std     %f12, [%l0 + F12_OFFSET]
        std     %f14, [%l0 + F14_OFFSET]
        std     %f16, [%l0 + F16_OFFSET]
        std     %f18, [%l0 + F18_OFFSET]
        std     %f20, [%l0 + F2O_OFFSET]
        std     %f22, [%l0 + F22_OFFSET]
        std     %f24, [%l0 + F24_OFFSET]
        std     %f26, [%l0 + F26_OFFSET]
        std     %f28, [%l0 + F28_OFFSET]
        std     %f30, [%l0 + F3O_OFFSET]
        std     %f32, [%l0 + F32_OFFSET]
        std     %f34, [%l0 + F34_OFFSET]
        std     %f36, [%l0 + F36_OFFSET]
        std     %f38, [%l0 + F38_OFFSET]
        std     %f40, [%l0 + F4O_OFFSET]
        std     %f42, [%l0 + F42_OFFSET]
        std     %f44, [%l0 + F44_OFFSET]
        std     %f46, [%l0 + F46_OFFSET]
        std     %f48, [%l0 + F48_OFFSET]
        std     %f50, [%l0 + F5O_OFFSET]
        std     %f52, [%l0 + F52_OFFSET]
        std     %f54, [%l0 + F54_OFFSET]
        std     %f56, [%l0 + F56_OFFSET]
        std     %f58, [%l0 + F58_OFFSET]
        std     %f60, [%l0 + F6O_OFFSET]       
        std     %f62, [%l0 + F62_OFFSET]
        stx     %fsr, [%l0 + FSR_OFFSET]
        ret
        restore

/*
 *  void _CPU_Context_restore_fp(
 *    void **fp_context_ptr
 *  )
 *
 *  This routine is responsible for restoring the FP context
 *  at *fp_context_ptr.  If the point to load the FP context
 *  from is changed then the pointer is modified by this routine.
 *
 */

        .align 4
        PUBLIC(_CPU_Context_restore_fp)
SYM(_CPU_Context_restore_fp):
        save    %sp, -CPU_MINIMUM_STACK_FRAME_SIZE , %sp

        /*
         *  The following enables the floating point unit.
         */
   
        sparc64_enable_FPU(%l0)

        ldx     [%i0], %l0
        ldd     [%l0 + FO_OFFSET], %f0
        ldd     [%l0 + F2_OFFSET], %f2
        ldd     [%l0 + F4_OFFSET], %f4
        ldd     [%l0 + F6_OFFSET], %f6
        ldd     [%l0 + F8_OFFSET], %f8
        ldd     [%l0 + F1O_OFFSET], %f10
        ldd     [%l0 + F12_OFFSET], %f12
        ldd     [%l0 + F14_OFFSET], %f14
        ldd     [%l0 + F16_OFFSET], %f16
        ldd     [%l0 + F18_OFFSET], %f18
        ldd     [%l0 + F2O_OFFSET], %f20
        ldd     [%l0 + F22_OFFSET], %f22
        ldd     [%l0 + F24_OFFSET], %f24
        ldd     [%l0 + F26_OFFSET], %f26
        ldd     [%l0 + F28_OFFSET], %f28
        ldd     [%l0 + F3O_OFFSET], %f30
        ldd     [%l0 + F32_OFFSET], %f32
        ldd     [%l0 + F34_OFFSET], %f34
        ldd     [%l0 + F36_OFFSET], %f36
        ldd     [%l0 + F38_OFFSET], %f38
        ldd     [%l0 + F4O_OFFSET], %f40
        ldd     [%l0 + F42_OFFSET], %f42
        ldd     [%l0 + F44_OFFSET], %f44
        ldd     [%l0 + F46_OFFSET], %f46
        ldd     [%l0 + F48_OFFSET], %f48
        ldd     [%l0 + F5O_OFFSET], %f50
        ldd     [%l0 + F52_OFFSET], %f52
        ldd     [%l0 + F54_OFFSET], %f54
        ldd     [%l0 + F56_OFFSET], %f56
        ldd     [%l0 + F58_OFFSET], %f58
        ldd     [%l0 + F6O_OFFSET], %f60
        ldd     [%l0 + F62_OFFSET], %f62
        ldx     [%l0 + FSR_OFFSET], %fsr
        ret
        restore

#endif /* SPARC_HAS_FPU */

/*
 *  void _CPU_Context_switch(
 *    Context_Control  *run,
 *    Context_Control  *heir
 *  )
 *
 *  This routine performs a normal non-FP context switch.
 */

        .align 4
        PUBLIC(_CPU_Context_switch)
SYM(_CPU_Context_switch):
        ! skip g0
        stx     %g1, [%o0 + G1_OFFSET]       ! save the global registers
        stx     %g2, [%o0 + G2_OFFSET]
        stx     %g3, [%o0 + G3_OFFSET]       
        stx     %g4, [%o0 + G4_OFFSET]
        stx     %g5, [%o0 + G5_OFFSET]       
        stx     %g6, [%o0 + G6_OFFSET]
        stx     %g7, [%o0 + G7_OFFSET]

        ! load the address of the ISR stack nesting prevention flag
	setx	SYM(_CPU_ISR_Dispatch_disable), %g1, %g2
	lduw	[%g2], %g2
	
	! save it a bit later so we do not waste a couple of cycles

        stx     %l0, [%o0 + L0_OFFSET]       ! save the local registers
        stx     %l1, [%o0 + L1_OFFSET]
        stx     %l2, [%o0 + L2_OFFSET]
        stx     %l3, [%o0 + L3_OFFSET]
        stx     %l4, [%o0 + L4_OFFSET]
        stx     %l5, [%o0 + L5_OFFSET]
        stx     %l6, [%o0 + L6_OFFSET]
        stx     %l7, [%o0 + L7_OFFSET]

        ! Now actually save ISR stack nesting prevention flag
        stuw     %g2, [%o0 + ISR_DISPATCH_DISABLE_STACK_OFFSET]

        stx     %i0, [%o0 + I0_OFFSET]       ! save the input registers
        stx     %i1, [%o0 + I1_OFFSET]
        stx     %i2, [%o0 + I2_OFFSET]
        stx     %i3, [%o0 + I3_OFFSET]
        stx     %i4, [%o0 + I4_OFFSET]
        stx     %i5, [%o0 + I5_OFFSET]
        stx     %i6, [%o0 + I6_FP_OFFSET]
        stx     %i7, [%o0 + I7_OFFSET]

        stx     %o0, [%o0 + O0_OFFSET]       ! save the output registers
        stx     %o1, [%o0 + O1_OFFSET]
        stx     %o2, [%o0 + O2_OFFSET]
        stx     %o3, [%o0 + O3_OFFSET]
        stx     %o4, [%o0 + O4_OFFSET]
        stx     %o5, [%o0 + O5_OFFSET]
        stx     %o6, [%o0 + O6_SP_OFFSET]
        stx     %o7, [%o0 + O7_OFFSET]       ! o7 is the PC

        rdpr    %pil, %o2
        stuw    %o2, [%o0 + PIL_OFFSET] ! save pil

        rdpr    %pstate, %o2
        stx     %o2, [%o0 + PSTATE_OFFSET]      ! save status register

        /*
         *  This is entered from _CPU_Context_restore with:
         *    o1 = context to restore
         *    o2 = pstate
         */

        PUBLIC(_CPU_Context_restore_heir)
SYM(_CPU_Context_restore_heir):

	flushw



        ! skip g0
        ldx     [%o1 + G1_OFFSET], %g1        ! restore the global registers
        ldx     [%o1 + G2_OFFSET], %g2
        ldx     [%o1 + G3_OFFSET], %g3
        ldx     [%o1 + G4_OFFSET], %g4
        ldx     [%o1 + G5_OFFSET], %g5
        ldx     [%o1 + G6_OFFSET], %g6
        ldx     [%o1 + G7_OFFSET], %g7

        ! Load thread specific ISR dispatch prevention flag
        ldx    [%o1 + ISR_DISPATCH_DISABLE_STACK_OFFSET], %o2
	setuw	SYM(_CPU_ISR_Dispatch_disable), %o3
        ! Store it to memory later to use the cycles

        ldx     [%o1 + L0_OFFSET], %l0        ! restore the local registers
        ldx     [%o1 + L1_OFFSET], %l1
        ldx     [%o1 + L2_OFFSET], %l2
        ldx     [%o1 + L3_OFFSET], %l3
        ldx     [%o1 + L4_OFFSET], %l4
        ldx     [%o1 + L5_OFFSET], %l5
        ldx     [%o1 + L6_OFFSET], %l6
        ldx     [%o1 + L7_OFFSET], %l7

        ! Now restore thread specific ISR dispatch prevention flag
        stx	%o2, [%o3]

        ldx     [%o1 + I0_OFFSET], %i0        ! restore the input registers
        ldx     [%o1 + I1_OFFSET], %i1
        ldx     [%o1 + I2_OFFSET], %i2
        ldx     [%o1 + I3_OFFSET], %i3
        ldx     [%o1 + I4_OFFSET], %i4
        ldx     [%o1 + I5_OFFSET], %i5
        ldx     [%o1 + I6_FP_OFFSET], %i6
        ldx     [%o1 + I7_OFFSET], %i7

        ldx     [%o1 + O0_OFFSET], %o0
        ldx     [%o1 + O2_OFFSET], %o2        ! restore the output registers
        ldx     [%o1 + O3_OFFSET], %o3
        ldx     [%o1 + O4_OFFSET], %o4
        ldx     [%o1 + O5_OFFSET], %o5
        ldx     [%o1 + O6_SP_OFFSET], %o6
        ldx     [%o1 + O7_OFFSET], %o7       ! PC
        ! do o1 last to avoid destroying heir context pointer
        ldx     [%o1 + O1_OFFSET], %o1        ! overwrite heir pointer

        ! on a hunch... we should be able to use some of the %o regs
        lduw    [%o1 + PIL_OFFSET], %o2
        wrpr    %g0, %o2, %pil

        ldx     [%o1 + PSTATE_OFFSET], %o2
        wrpr    %g0, %o2, %pstate

 	retl
	nop

/*
 *  void _CPU_Context_restore(
 *    Context_Control *new_context
 *  )
 *
 *  This routine is generally used only to perform restart self.
 *
 *  NOTE: It is unnecessary to reload some registers.
 */
/* GAB: if _CPU_Context_restore_heir does not flushw, then do it here */
        .align 4
        PUBLIC(_CPU_Context_restore)
SYM(_CPU_Context_restore):
        save    %sp, -CPU_MINIMUM_STACK_FRAME_SIZE, %sp
        rdpr    %pstate, %o2
        ba      SYM(_CPU_Context_restore_heir)
        mov     %i0, %o1                      ! in the delay slot

/*
 *  void _ISR_Handler()
 *
 *  This routine provides the RTEMS interrupt management.
 *
 *  We enter this handler from the 8 instructions in the trap table with
 *  the following registers assumed to be set as shown:
 *
 *    g4 = tstate (old l0)
 *    g2 = trap type (vector) (old l3)
 *
 *  NOTE: By an executive defined convention, trap type is between 0 and 255 if
 *        it is an asynchonous trap and 256 and 511 if it is synchronous.
 */

        .align 4
        PUBLIC(_ISR_Handler)
SYM(_ISR_Handler):

/* 
 * The ISR is called at TL = 1. On sun4v, we can use the globals at this
 * trap level (GL[1]).  Back-porting to sun4u would involve ensuring 
 * compatability with the alternate global set. 
 *
 * On entry:
 *   g4 = tstate (from trap table)
 *   g2 = trap vector #
 * 
 * In either case, note that trap handlers share a register window with 
 * the interrupted context, unless we explicitly enter a new window. This 
 * differs from Sparc v8, in which a dedicated register window is saved 
 * for trap handling.  This means we have to avoid overwriting any registers
 * that we don't save.
 *
 */


/*
 *  save some or all context on stack
 */

        /*
         *  Save the state of the interrupted task -- especially the global
         *  registers -- in the Interrupt Stack Frame.  Note that the ISF
         *  includes a regular minimum stack frame which will be used if
         *  needed by register window overflow and underflow handlers.
	 *
	 *  This is slightly wasteful, since the stack already has the window
	 *  overflow space reserved, but there is no obvious way to ensure 
	 *  we can store the interrupted state and still handle window 
	 *  spill/fill correctly, since there is no room for the ISF.
         *
         */

	/* first store the sp of the interrupted task temporarily in g1 */
	mov 	%sp, %g1

        sub     %sp, CONTEXT_CONTROL_INTERRUPT_FRAME_SIZE, %sp
                                           ! make space for Stack_Frame||ISF

	/* save tstate, tpc, tnpc, pil */
	stx     %g4, [%sp + STACK_BIAS + ISF_TSTATE_OFFSET]	
	rdpr	%pil, %g3
	rdpr	%tpc, %g4
	rdpr	%tnpc, %g5
	stx	%g3, [%sp + STACK_BIAS + ISF_PIL_OFFSET]
	stx	%g4, [%sp + STACK_BIAS + ISF_TPC_OFFSET]
	stx	%g5, [%sp + STACK_BIAS + ISF_TNPC_OFFSET]
	stx	%g2, [%sp + STACK_BIAS + ISF_TVEC_NUM]

	rd	%y, %g4				! save y
	stx 	%g4, [%sp + STACK_BIAS + ISF_Y_OFFSET]
	
	! save interrupted frame's output regs
        stx     %o0, [%sp + STACK_BIAS + ISF_O0_OFFSET]     ! save o0
        stx     %o1, [%sp + STACK_BIAS + ISF_O1_OFFSET]     ! save o1
        stx     %o2, [%sp + STACK_BIAS + ISF_O2_OFFSET]     ! save o2
        stx     %o3, [%sp + STACK_BIAS + ISF_O3_OFFSET]     ! save o3
        stx     %o4, [%sp + STACK_BIAS + ISF_O4_OFFSET]     ! save o4
        stx     %o5, [%sp + STACK_BIAS + ISF_O5_OFFSET]     ! save o5
        stx     %g1, [%sp + STACK_BIAS + ISF_O6_SP_OFFSET]  ! save o6/sp
        stx     %o7, [%sp + STACK_BIAS + ISF_O7_OFFSET]     ! save o7

	mov	%g1, %o5		! hold the old sp here for now
	mov	%g2, %o1		! we'll need trap # later

	/* switch to TL[0], GL[0] get pstate to a known state */
	wrpr	%g0, 0, %tl
	wrpr	%g0, 0, %gl
	wrpr	%g0, SPARC_PSTATE_PRIV_MASK | SPARC_PSTATE_PEF_MASK, %pstate

	! save globals
        stx     %g1, [%sp + STACK_BIAS + ISF_G1_OFFSET]     ! save g1
        stx     %g2, [%sp + STACK_BIAS + ISF_G2_OFFSET]     ! save g2
        stx     %g3, [%sp + STACK_BIAS + ISF_G3_OFFSET]     ! save g3
        stx     %g4, [%sp + STACK_BIAS + ISF_G4_OFFSET]     ! save g4
        stx     %g5, [%sp + STACK_BIAS + ISF_G5_OFFSET]     ! save g5
        stx     %g6, [%sp + STACK_BIAS + ISF_G6_OFFSET]     ! save g6
        stx     %g7, [%sp + STACK_BIAS + ISF_G7_OFFSET]     ! save g7
	

	mov	%o1, %g2	! get the trap #
	mov	%o5, %g3	! store the interrupted %sp
        mov     %sp, %o1        ! 2nd arg to ISR Handler = address of ISF

        /*
         *  Increment ISR nest level and Thread dispatch disable level.
         *
         *  Register usage for this section: (note, these are used later)
         *
         *    g7 = _Thread_Dispatch_disable_level pointer
         *    g5 = _Thread_Dispatch_disable_level value (uint32_t)
         *    g6 = _ISR_Nest_level pointer
         *    g4 = _ISR_Nest_level value (uint32_t)
	 *    o5 = temp
         *
         *  NOTE: It is assumed that g6 - g7 will be preserved until the ISR
         *        nest and thread dispatch disable levels are unnested.
         */

	setx	SYM(_Thread_Dispatch_disable_level), %o5, %g7
	lduw	[%g7], %g5
        setx	SYM(_ISR_Nest_level), %o5, %g6
	lduw	[%g6], %g4

        add      %g5, 1, %g5
        stx      %g5, [%g7]

        add      %g4, 1, %g4
        stx      %g4, [%g6]

        /*
         *  If ISR nest level was zero (now 1), then switch stack.
         */

        subcc    %g4, 1, %g4             ! outermost interrupt handler?
        bnz      dont_switch_stacks      ! No, then do not switch stacks

	setx	SYM(_CPU_Interrupt_stack_high), %o5, %g1
	ldx	[%g1], %sp

        /*
         *  Make sure we have a place on the stack for the window overflow
         *  trap handler to write into.  At this point it is safe to
         *  enable traps again.
         */

        sub      %sp, CPU_MINIMUM_STACK_FRAME_SIZE, %sp

dont_switch_stacks:
        /*
         *  Check if we have an external interrupt (trap 0x41 - 0x4f). If so,
         *  set the PIL to mask off interrupts with lower priority.
         *
	 *  The original PIL is not modified since it will be restored
         *  when the interrupt handler returns.
         */

	and	 %g2, 0x0ff, %g1 ! is bottom byte of vector number [0x41,0x4f]?

        subcc    %g1, 0x41, %g0
        bl       dont_fix_pil
        subcc    %g1, 0x4f, %g0
        bg       dont_fix_pil
	nop
	wrpr	%g0, %g1, %pil

dont_fix_pil:
	/* We need to be careful about enabling traps here.
	 *
	 * We already stored off the tstate, tpc, and tnpc, and switched to
	 * TL = 0, so it should be safe.
	 */

	/* zero out g4 so that printf works */
	mov	%g0, %g4

	! **** ENABLE TRAPS ****
	wrpr	%g0, SPARC_PSTATE_PRIV_MASK | SPARC_PSTATE_PEF_MASK | \
			SPARC_PSTATE_IE_MASK, %pstate 

        /*
         *  Vector to user's handler.
         *
         *  NOTE: TBR may no longer have vector number in it since
         *        we just enabled traps.  It is definitely in g2.
         */
	setx	SYM(_ISR_Vector_table), %o5, %g1
	ldx	[%g1], %g1
	and      %g2, 0x1FF, %o5        ! remove synchronous trap indicator
        sll      %o5, 2, %o5            ! o5 = offset into table
        ldx      [%g1 + %o5], %g1       ! g1 = _ISR_Vector_table[ vector ]


                                        ! o1 = 2nd arg = address of the ISF
                                        !   WAS LOADED WHEN ISF WAS SAVED!!!
        mov      %g2, %o0               ! o0 = 1st arg = vector number
        call     %g1, 0
        nop                             ! delay slot

        /*
         *  Redisable traps so we can finish up the interrupt processing.
         *  This is a VERY conservative place to do this.
         */
	! **** DISABLE TRAPS ****
	wrpr	%g0, SPARC_PSTATE_PRIV_MASK, %pstate

	/* 
	 * We may safely use any of the %o and %g registers, because 
	 * we saved them earlier (and any other interrupt that uses 
	 * them will also save them).  Right now, the state of those
	 * registers are as follows:
	 *	%o registers: unknown (user's handler may have destroyed)
	 *	%g1,g4,g5: scratch
	 *	%g2: unknown: was trap vector
	 *	%g3: uknown: was interrupted task's sp
	 *	%g6: _ISR_Nest_level
	 *	%g7: _Thread_Dispatch_disable_level
	 */

        /*
         *  Increment ISR nest level and Thread dispatch disable level.
         *
         *  Register usage for this section: (note: as used above)
         *
         *    g7 = _Thread_Dispatch_disable_level pointer
         *    g5 = _Thread_Dispatch_disable_level value
         *    g6 = _ISR_Nest_level pointer
         *    g4 = _ISR_Nest_level value
	 *    o5 = temp
         */

	/* We have to re-load the values from memory, because there are
	 * not enough registers that we know will be preserved across the
	 * user's handler. If this is a problem, we can create a register
	 * window for _ISR_Handler.
	 */

	lduw	[%g7],%g5
	lduw	[%g6],%g4
        sub      %g5, 1, %g5
	stuw	%g5, [%g7]
	sub	%g4, 1, %g4
	stuw	%g4, [%g6]

        /*
         *  If dispatching is disabled (includes nested interrupt case),
         *  then do a "simple" exit.
         */

        orcc     %g5, %g0, %g0   ! Is dispatching disabled?
        bnz      simple_return   ! Yes, then do a "simple" exit
        ! NOTE: Use the delay slot
	mov	%g0, %g4	! clear g4 for printf

        ! Are we dispatching from a previous ISR in the interrupted thread?
	setx	SYM(_CPU_ISR_Dispatch_disable), %o5, %g5
        ld       [%g5], %o5
        orcc     %o5, %g0, %g0   ! Is this thread already doing an ISR?
        bnz      simple_return   ! Yes, then do a "simple" exit
        nop

	setx    SYM(_Context_Switch_necessary), %o5, %g7
        

        /*
         *  If a context switch is necessary, then do fudge stack to
         *  return to the interrupt dispatcher.
         */

        ldub     [%g7], %o5

        orcc     %o5, %g0, %g0   ! Is thread switch necessary?
        bnz      SYM(_ISR_Dispatch) ! yes, then invoke the dispatcher
	nop

        /*
         *  Finally, check to see if signals were sent to the currently
         *  executing task.  If so, we need to invoke the interrupt dispatcher.
         */
	setx SYM(_ISR_Signals_to_thread_executing), %o5, %g5
	ldub	[%g5], %o5

        orcc     %o5, %g0, %g0   ! Were signals sent to the currently
                                 !   executing thread?
        bz       simple_return   ! yes, then invoke the dispatcher
                                 ! use the delay slot to clear the signals
                                 !   to the currently executing task flag
        stb       %g0, [%g5]
                                 

        /*
         *  Invoke interrupt dispatcher.
         */
        PUBLIC(_ISR_Dispatch)
SYM(_ISR_Dispatch):
        ! Set ISR dispatch nesting prevention flag
        mov      1, %o1
        setx    SYM(_CPU_ISR_Dispatch_disable), %o5, %o2
        st       %o1, [%o2]

        /*
	 *  This is the outermost interrupt handler. Need to get off the
	 *  CPU Interrupt Stack and back to the tasks stack.
	 *
         *  The following subtract should get us back on the interrupted
         *  tasks stack and add enough room to invoke the dispatcher.
         *  When we enable traps, we are mostly back in the context
         *  of the task and subsequent interrupts can operate normally.
	 *
	 *  Now %sp points to the bottom of the ISF.
	 *  
         */

        sub      %fp, 	CONTEXT_CONTROL_INTERRUPT_FRAME_SIZE, %sp

  	!  **** ENABLE TRAPS ****
	wrpr	%g0, SPARC_PSTATE_PRIV_MASK | SPARC_PSTATE_PEF_MASK | \
			SPARC_PSTATE_IE_MASK, %pstate
isr_dispatch:
        call    SYM(_Thread_Dispatch), 0
        nop

	/*
	 *  We invoked _Thread_Dispatch in a state similar to the interrupted
	 *  task.  In order to safely be able to tinker with the register
	 *  windows and get the task back to its pre-interrupt state, 
	 *  we need to disable interrupts disabled so we can safely tinker
	 *  with the register windowing.  In particular, the CWP in the PSR
	 *  is fragile during this period. (See PR578.)
	 */
	mov	2,%g4				! syscall (disable interrupts)
	ta	0				! syscall (disable interrupts)
	mov	0,%g4
        /*
         *  While we had ISR dispatching disabled in this thread,
         *  did we miss anything.  If so, then we need to do another
         *  _Thread_Dispatch before leaving this ISR Dispatch context.
         */

        setx	SYM(_Context_Switch_necessary), %o5, %o1
        ldub     [%o1], %o2

        ! NOTE: Use some of delay slot to start loading this
        setx    SYM(_ISR_Signals_to_thread_executing), %o5, %o1
        ldub     [%o1], %o3

        orcc     %o2, %g0, %g0   ! Is thread switch necessary?
        bnz      dispatchAgain   ! yes, then invoke the dispatcher AGAIN
        ! NOTE: Use the delay slot to catch the orcc below

        /*
         *  Finally, check to see if signals were sent to the currently
         *  executing task.  If so, we need to invoke the interrupt dispatcher.
         */

        ! NOTE: Delay slots above were used to perform the load AND
        !       this orcc falls into the delay slot for bnz above
        orcc     %o3, %g0, %g0   ! Were signals sent to the currently
                                 !   executing thread?
        bz       allow_nest_again ! No, then clear out and return
	nop

        ! Yes, then invoke the dispatcher
dispatchAgain:
	mov	3, %g4				! syscall (enable interrupts)
	ta	0				! syscall (enable interrupts)
        ba      isr_dispatch
        mov	0, %g4

allow_nest_again:

        ! Zero out ISR stack nesting prevention flag
        setx	SYM(_CPU_ISR_Dispatch_disable), %o5, %o1
        st       %g0,[%o1]

        /*
         *  The CWP in place at this point may be different from
         *  that which was in effect at the beginning of the ISR if we
         *  have been context switched between the beginning of this invocation
         *  of _ISR_Handler and this point.  Thus the CWP and WIM should
         *  not be changed back to their values at ISR entry time.  Any
         *  changes to the PSR must preserve the CWP.
         */

simple_return:
	flushw

	! **** DISABLE TRAPS ****
	wrpr	%g0, SPARC_PSTATE_PRIV_MASK, %pstate

	ldx     [%sp + STACK_BIAS + ISF_Y_OFFSET], %o1      ! restore y
        wr      %o1, 0, %y



	ldx	[%sp + STACK_BIAS + ISF_TSTATE_OFFSET], %g1
	
	! see if cwp is proper (tstate.cwp == cwp)
	and	%g1, 0x1F, %g6
	rdpr	%cwp, %g7
	cmp	%g6, %g7
	bz	good_window
	nop

	/*
	 * Fix the CWP. Need the cwp to be the proper cwp that
	 * gets restored when returning from the trap via retry/done. Do 
	 * this before reloading the task's output regs. Basically fake a
	 * window spill/fill.
	 *
	 * TODO: Is this necessary/correct? Why not just re-write 
	 * tstate.cwp to be equal to the current cwp?
	 */
	mov	%sp, %g3
	stx	%l0, [%sp + STACK_BIAS + CPU_STACK_FRAME_L0_OFFSET]
	stx	%l1, [%sp + STACK_BIAS + CPU_STACK_FRAME_L1_OFFSET]
	stx	%l2, [%sp + STACK_BIAS + CPU_STACK_FRAME_L2_OFFSET]
	stx	%l3, [%sp + STACK_BIAS + CPU_STACK_FRAME_L3_OFFSET]
	stx	%l4, [%sp + STACK_BIAS + CPU_STACK_FRAME_L4_OFFSET]
	stx	%l5, [%sp + STACK_BIAS + CPU_STACK_FRAME_L5_OFFSET]
	stx	%l6, [%sp + STACK_BIAS + CPU_STACK_FRAME_L6_OFFSET]
	stx	%l7, [%sp + STACK_BIAS + CPU_STACK_FRAME_L7_OFFSET]
	stx	%i0, [%sp + STACK_BIAS + CPU_STACK_FRAME_I0_OFFSET]
	stx	%i1, [%sp + STACK_BIAS + CPU_STACK_FRAME_I1_OFFSET]
	stx	%i2, [%sp + STACK_BIAS + CPU_STACK_FRAME_I2_OFFSET]
	stx	%i3, [%sp + STACK_BIAS + CPU_STACK_FRAME_I3_OFFSET]
	stx	%i4, [%sp + STACK_BIAS + CPU_STACK_FRAME_I4_OFFSET]
	stx	%i5, [%sp + STACK_BIAS + CPU_STACK_FRAME_I5_OFFSET]
	stx	%i6, [%sp + STACK_BIAS + CPU_STACK_FRAME_I6_FP_OFFSET]
	stx	%i7, [%sp + STACK_BIAS + CPU_STACK_FRAME_I7_OFFSET]
	wrpr	%g0, %g6, %cwp
	mov	%g3, %sp
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_L0_OFFSET], %l0
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_L1_OFFSET], %l1
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_L2_OFFSET], %l2
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_L3_OFFSET], %l3
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_L4_OFFSET], %l4
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_L5_OFFSET], %l5
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_L6_OFFSET], %l6
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_L7_OFFSET], %l7
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_I0_OFFSET], %i0
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_I1_OFFSET], %i1
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_I2_OFFSET], %i2
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_I3_OFFSET], %i3
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_I4_OFFSET], %i4
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_I5_OFFSET], %i5
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_I6_FP_OFFSET], %i6
	ldx	[%sp + STACK_BIAS + CPU_STACK_FRAME_I7_OFFSET], %i7


good_window:


        /*
         *  Restore tasks global and out registers
         */

        ldx     [%sp + STACK_BIAS + ISF_G1_OFFSET], %g1    ! restore g1
        ldx     [%sp + STACK_BIAS + ISF_G2_OFFSET], %g2    ! restore g2
        ldx     [%sp + STACK_BIAS + ISF_G3_OFFSET], %g3    ! restore g3
        ldx     [%sp + STACK_BIAS + ISF_G4_OFFSET], %g4    ! restore g4
        ldx     [%sp + STACK_BIAS + ISF_G5_OFFSET], %g5    ! restore g5
        ldx     [%sp + STACK_BIAS + ISF_G6_OFFSET], %g6    ! restore g6
        ldx     [%sp + STACK_BIAS + ISF_G7_OFFSET], %g7    ! restore g7



	! return to TL[1], GL[1], and restore TSTATE, TPC, and TNPC
	wrpr	%g0, 1, %tl
	wrpr	%g0, 1, %gl
	! now we can use global registers (at gl 1)
	ldx	[%sp + STACK_BIAS + ISF_PIL_OFFSET], %g3
	ldx	[%sp + STACK_BIAS + ISF_TPC_OFFSET], %g4
	ldx	[%sp + STACK_BIAS + ISF_TNPC_OFFSET], %g5
	ldx	[%sp + STACK_BIAS + ISF_TSTATE_OFFSET], %g1
	ldx	[%sp + STACK_BIAS + ISF_TVEC_NUM], %g2
	wrpr	%g0, %g3, %pil
	wrpr	%g0, %g4, %tpc
	wrpr	%g0, %g5, %tnpc

        ldx     [%sp + STACK_BIAS + ISF_O0_OFFSET], %o0    ! restore o0
        ldx     [%sp + STACK_BIAS + ISF_O1_OFFSET], %o1    ! restore o1
        ldx     [%sp + STACK_BIAS + ISF_O2_OFFSET], %o2    ! restore o2
        ldx     [%sp + STACK_BIAS + ISF_O3_OFFSET], %o3    ! restore o3
        ldx     [%sp + STACK_BIAS + ISF_O4_OFFSET], %o4    ! restore o4
        ldx     [%sp + STACK_BIAS + ISF_O5_OFFSET], %o5    ! restore o5
        ! sp is restored later
	ldx     [%sp + STACK_BIAS + ISF_O7_OFFSET], %o7    ! restore o7

        ldx     [%sp + STACK_BIAS + ISF_O6_SP_OFFSET], %o6 ! restore o6/sp
	
	/*
         *  Determine whether to re-execute the trapping instruction 
	 *  (asynchronous trap) or to skip the trapping instruction
	 *  (synchronous trap).
         */

        andcc   %g2, SPARC_SYNCHRONOUS_TRAP_BIT_MASK, %g0
                                      ! Is this a synchronous trap?
        be	not_synch             ! No, then skip trapping instruction
	mov	0, %g4
	retry				! re-execute trapping instruction
not_synch:
	done				! skip trapping instruction

/* end of file */
