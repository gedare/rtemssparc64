/*  bspinit.c
 *
 *  This file provides for BSP-specific initialization within the sparc64 
 *  boot-up sequence.
 *
 *  sun4u
 *
 *  Copyright (c) 2010 Gedare Bloom.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <rtems.h>
#include <bsp.h>
#include <boot/ofw.h>
#include <rtems/score/cpu.h>
#include <traptable.h>

extern void (*syscall)();

void* ofw_maplow(const void *phys, const unsigned int size)
{
  ofw_arg_t retaddr[1];
  if(ofw_call("call-method", 4, 2, retaddr, "map-low", ofw_mmu, phys, size)) {
    puts("Error: unable to map-low\n");
  }
  return (void*) retaddr[0];
}

int ofw_modify(const void *virt, const unsigned int size, const int mode)
{
  return ofw_call("call-method", 5, 1, NULL, "modify", ofw_mmu, 
      virt, size, mode);
}

int ofw_unmap(const void *virt, const unsigned int size)
{
  return ofw_call("call-method", 4, 1, NULL, "unmap", ofw_mmu, virt, size);
}

int ofw_release(const void *virt, const unsigned int size)
{
  return ofw_call("call-method", 4, 1, NULL, "release", ofw_mmu, virt, size);
}

unsigned int ofw_get_mode(const void* virt) {
  uint64_t mask = 0;
  void *phys = 0;

  phys = ofw_translate(virt);

  if (sizeof(unative_t) == 8)
    mask = ~(0x0FFFFFFFUL);
  else
    mask = ~(0x3);

  printk("phys: %x\n", phys);
  printk("mask: %x\n", mask);
  return (((uint64_t)phys) & mask);
}

void _BSP_init() {

  void* curr_tba_v; /* virtual address of current tba */
  void* curr_tba_p; /* physical address of current tba */

  void *ignore = 0;
  unsigned int mode = 0;
  unsigned int mask = 0;
  void *translation = 0;

  sparc64_get_tba(curr_tba_v);
  curr_tba_p = ofw_translate(curr_tba_v);

#if 0
  tba_copy =  ofw_maplow(curr_tba_p, TABLE_SIZE);
#endif

#if 0
  mode = ofw_get_mode(&syscall);
  printk("mode: %x\n", mode);
  
  mode = ofw_get_mode(curr_tba_v + 0x2000);

  printk("mode: %x\n", mode);

  if(ofw_modify(curr_tba_v, TABLE_SIZE, -1)) {
    printk("failed to modify translation\n");
  }

  mode = ofw_get_mode(curr_tba_v + 0x2000);
  printk("mode: %x\n", mode);
#endif 

#if 0
  /*
  if(ofw_unmap(&trap_table[0], TABLE_SIZE)) {
    printk("Failed to unmap trap table\n");
  }
  */
  /*
  if(ofw_release(&trap_table[0], TABLE_SIZE)) {
    printk("Failed to release trap table\n");
  }
  */
  (void) ofw_claim_virt(&trap_table[0], TABLE_SIZE);
  if(ofw_map(curr_tba_p, &trap_table[0], TABLE_SIZE, -1)) {
    printk("Failed to map trap table\n");
  }
#endif


  printk("curr_tba: v: %x, p: %x, content: %x\n", curr_tba_v, curr_tba_p,
      *(uint64_t*)curr_tba_v);

  _CPU_ISR_install_raw_handler(0x100, &syscall, &ignore);
}
