/*
 * Cogent CSB337 - AT91RM9200 Startup code
 *
 * Copyright (c) 2004 by Cogent Computer Systems
 * Written by Jay Monkman <jtm@lopingdog.com>
 *	
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.rtems.com/license/LICENSE.
 *
 *
 *  $Id: bspstart.c,v 1.16 2009/06/04 18:37:54 joel Exp $
*/
#include <bsp.h>
#include <at91rm9200.h>
#include <at91rm9200_pmc.h>
#include <at91rm9200_emac.h>

/* Function prototypes */
extern void rtems_irq_mngt_init(void);
void bsp_libc_init( void *, uint32_t, int );
static void fix_mac_addr(void);

/*
 * bsp_start_default - BSP initialization function
 *
 * This function is called before RTEMS is initialized and used
 * adjust the kernel's configuration.
 *
 * This function also configures the CPU's memory protection unit.
 *
 * RESTRICTIONS/LIMITATIONS:
 *   Since RTEMS is not configured, no RTEMS functions can be called.
 */
void bsp_start_default( void )
{
  /* disable interrupts */
  AIC_CTL_REG(AIC_IDCR) = 0xffffffff;

  /* 
   * Some versions of the bootloader have the MAC address
   * reversed. This fixes it, if necessary.
   */
  fix_mac_addr();

  /*
   * Init rtems exceptions management
   */
  rtems_exception_init_mngt();
  
  /*
   * Init rtems interrupt management
   */
  rtems_irq_mngt_init();
    
} /* bsp_start */

/* 
 * Some versions of the bootloader shipped with the CSB337
 * reverse the MAC address. This function tests for that,
 * and fixes the MAC address.
 */
static void fix_mac_addr(void)
{
  uint8_t addr[6];

  /* Read the MAC address */
  addr[0] = (EMAC_REG(EMAC_SA1L) >>  0) & 0xff;
  addr[1] = (EMAC_REG(EMAC_SA1L) >>  8) & 0xff;
  addr[2] = (EMAC_REG(EMAC_SA1L) >> 16) & 0xff;
  addr[3] = (EMAC_REG(EMAC_SA1L) >> 24) & 0xff;
  addr[4] = (EMAC_REG(EMAC_SA1H) >>  0) & 0xff;
  addr[5] = (EMAC_REG(EMAC_SA1H) >>  8) & 0xff;

  /* Check which 3 bytes have Cogent's OUI */
  if ((addr[5] == 0x00) && (addr[4] == 0x23) && (addr[3] == 0x31)) {
      EMAC_REG(EMAC_SA1L) = ((addr[5] <<  0) |
                             (addr[4] <<  8) |
                             (addr[3] << 16) |
                             (addr[2] << 24));

      EMAC_REG(EMAC_SA1H) = ((addr[1] <<  0) |
                             (addr[0] <<  8));
  }
}


/*
 *  By making this a weak alias for bsp_start_default, a brave soul
 *  can override the actual bsp_start routine used.
 */
void bsp_start (void) __attribute__ ((weak, alias("bsp_start_default")));
