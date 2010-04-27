/*
 * Copyright (c) 2005 Martin Decky
 * Copyright (c) 2006 Jakub Jermar
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * $Id$
 */

#include "main.h"
#include "asm.h"
#include <balloc.h>
#include <ofw.h>
#include <ofw_tree.h>
#include "ofwarch.h"
#include <align.h>


bootinfo_t bootinfo;

char *release = "0.0.0.1";

#ifdef REVISION
	char *revision = ", revision " REVISION;
#else
	char *revision = "";
#endif

#ifdef TIMESTAMP
	char *timestamp = "\nBuilt on " TIMESTAMP;
#else
	char *timestamp = "";
#endif

/** UltraSPARC subarchitecture - 1 for US, 3 for US3, 0 for other */
uint8_t subarchitecture = 0;

/**
 * mask of the MID field inside the ICBUS_CONFIG register shifted by
 * MID_SHIFT bits to the right
 */
uint16_t mid_mask;

/** Print version information. */
static void version_print(void)
{
	printk("HelenOS SPARC64 Bootloader\nRelease %s%s%s\n"
	    "Copyright (c) 2006 HelenOS project\n",
	    release, revision, timestamp);
}

/* the lowest ID (read from the VER register) of some US3 CPU model */
#define FIRST_US3_CPU 	0x14

/* the greatest ID (read from the VER register) of some US3 CPU model */
#define LAST_US3_CPU 	0x19

/* UltraSPARC IIIi processor implementation code */
#define US_IIIi_CODE	0x15

/* max. length of the "compatible" property of the root node */
#define COMPATIBLE_PROP_MAXLEN	64

/*
 * HelenOS bootloader will use these constants to distinguish particular
 * UltraSPARC architectures
 */
#define COMPATIBLE_SUN4U	10
#define COMPATIBLE_SUN4V	20

/** US architecture. COMPATIBLE_SUN4U for sun4v, COMPATIBLE_SUN4V for sun4u */
static uint8_t architecture;

/**
 * Detects the UltraSPARC architecture (sun4u and sun4v currently supported)
 * by inspecting the property called "compatible" in the OBP root node.
 */
static void detect_architecture(void)
{
	phandle root = ofw_find_device("/");
	char compatible[COMPATIBLE_PROP_MAXLEN];

	if (ofw_get_property(root, "compatible", compatible,
			COMPATIBLE_PROP_MAXLEN) <= 0) {
		printk("Unable to determine architecture, default: sun4u.\n");
		architecture = COMPATIBLE_SUN4U;
		return;
	}

	if (strcmp(compatible, "sun4v") == 0) {
		architecture = COMPATIBLE_SUN4V;
	} else {
		/*
		 * As not all sun4u machines have "sun4u" in their "compatible"
		 * OBP property (e.g. Serengeti's OBP "compatible" property is
		 * "SUNW,Serengeti"), we will by default fallback to sun4u if
		 * an unknown value of the "compatible" property is encountered.
		 */
		architecture = COMPATIBLE_SUN4U;
	}
}

/**
 * Detects the subarchitecture (US, US3) of the sun4u
 * processor. Sets the global variables "subarchitecture" and "mid_mask" to
 * correct values.
 */
static void detect_subarchitecture(void)
{
/*	uint64_t v;
	asm volatile ("rdpr %%ver, %0\n" : "=r" (v));

	v = (v << 16) >> 48;
	if ((v >= FIRST_US3_CPU) && (v <= LAST_US3_CPU)) {
		subarchitecture = SUBARCH_US3;
		if (v == US_IIIi_CODE)
			mid_mask = (1 << 5) - 1;
		else
			mid_mask = (1 << 10) - 1;
	} else if (v < FIRST_US3_CPU) {
		subarchitecture = SUBARCH_US;
		mid_mask = (1 << 5) - 1;
	} else {
		printk("\nThis CPU is not supported by HelenOS.");
	}
*/
}

/**
 * Performs sun4u-specific initialization. The components are expected
 * to be already copied and boot allocator initialized.
 *
 * @param base	kernel base virtual address
 * @param top	virtual address above which the boot allocator
 * 		can make allocations
 */
static void bootstrap_sun4u(void *base, unsigned int top)
{
	void *balloc_base;

	/*
	 * Claim and map the physical memory for the boot allocator.
	 * Initialize the boot allocator.
	 */
	balloc_base = base + ALIGN_UP(top, PAGE_SIZE);
	(void) ofw_claim_phys(bootinfo.physmem_start + balloc_base,
	    BALLOC_MAX_SIZE);
	(void) ofw_map(balloc_base, balloc_base, BALLOC_MAX_SIZE, -1);
	balloc_init(&bootinfo.ballocs, (uintptr_t)balloc_base);

	printk("\nCanonizing OpenFirmware device tree...");
	bootinfo.ofw_root = ofw_tree_build();
	printk("done.\n");

	detect_subarchitecture();

#ifdef CONFIG_SMP
	printk("\nChecking for secondary processors...");
	if (!ofw_cpu())
		printk("Error: unable to get CPU properties\n");
	printk("done.\n");
#endif

	setup_palette();
}

/**
 * Performs sun4v-specific initialization. The components are expected
 * to be already copied and boot allocator initialized.
 */
static void bootstrap_sun4v(void)
{
	/*
	 * When SILO booted, the OBP had established a virtual to physical
	 * memory mapping. This mapping is not an identity (because the
	 * physical memory starts on non-zero address) - this is not
	 * surprising. But! The mapping even does not map virtual address
	 * 0 onto the starting address of the physical memory, but onto an
	 * address which is 0x400000 bytes higher. The reason is that the
	 * OBP had already used the memory just at the beginning of the
	 * physical memory, so that memory cannot be used by SILO (nor
	 * bootloader). As for now, we solve it by a nasty workaround:
	 * we pretend that the physical memory starts 0x400000 bytes further
	 * than it actually does (and hence pretend that the physical memory
	 * is 0x400000 bytes smaller). Of course, the value 0x400000 will most
	 * probably depend on the machine and OBP version (the workaround now
	 * works on Simics). A solution would be to inspect the "available"
	 * property of the "/memory" node to find out which parts of memory
	 * are used by OBP and redesign the algorithm of copying
	 * kernel/init tasks/ramdisk from the bootable image to memory
	 * (which we must do anyway because of issues with claiming the memory
	 * on Serengeti).
 	 */
	bootinfo.physmem_start += 0x400000;
	bootinfo.memmap.zones[0].start += 0x400000;
	bootinfo.memmap.zones[0].size -= 0x400000;
}

void bootstrap(void)
{
	void *base = (void *) KERNEL_VIRTUAL_ADDRESS;
	unsigned int top = 0;
	int i, j;

	//printk("RTEMS System starting");

	detect_architecture();

	if (!ofw_get_physmem_start(&bootinfo.physmem_start)) {
		printk("Error: unable to get start of physical memory.\n");
		halt();
	}

	if (!ofw_memmap(&bootinfo.memmap)) {
		printk("Error: unable to get memory map, halting.\n");
		halt();
	}

	if (bootinfo.memmap.total == 0) {
		printk("Error: no memory detected, halting.\n");
		halt();
	}

	/*
	 * SILO for some reason adds 0x400000 and subtracts
	 * bootinfo.physmem_start to/from silo_ramdisk_image.
	 * We just need plain physical address so we fix it up.
	 */
	if (silo_ramdisk_image) {
		silo_ramdisk_image += bootinfo.physmem_start;
		silo_ramdisk_image -= 0x400000;
		/* Install 1:1 mapping for the ramdisk. */
		if (ofw_map((void *)((uintptr_t)silo_ramdisk_image),
		    (void *)((uintptr_t)silo_ramdisk_image),
		    silo_ramdisk_size, -1) != 0) {
			printk("Failed to map ramdisk.\n");
			halt();
		}
	}

	//printk("\n\rSystem info\n\r");
	//printk(" memory: %dM starting at %x\n\r",
	//    bootinfo.memmap.total >> 20, bootinfo.physmem_start);

}

