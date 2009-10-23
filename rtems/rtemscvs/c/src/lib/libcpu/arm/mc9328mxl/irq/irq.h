/*
 * Interrupt handler Header file
 *
 * Copyright (c) 2004 by Jay Monkman <jtm@lopingdog.com>
 *	
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *
 *  http://www.rtems.com/license/LICENSE.
 *
 *
 *  $Id: irq.h,v 1.6 2007/03/12 11:17:07 joel Exp $
 */

#ifndef __IRQ_H__
#define __IRQ_H__

#include <rtems/irq.h>

#ifdef __cplusplus
extern "C" {
#endif

  /* define that can be useful (the values are just examples) */
#ifndef __asm__

/*
 * Include some preprocessor value also used by assember code
 */
  
#include <rtems.h>
#include <mc9328mxl.h>

extern void default_int_handler();
/***********************************************************************
 * Constants
 **********************************************************************/

/* possible interrupt sources on the MC9328MXL */
#define BSP_INT_UART3_PFERR       0 
#define BSP_INT_UART3_RTS         1     
#define BSP_INT_UART3_DTR         2     
#define BSP_INT_UART3_UARTC       3       
#define BSP_INT_UART3_TX          4    
#define BSP_INT_PEN_UP            5  
#define BSP_INT_CSI               6
#define BSP_INT_MMA_MAC           7    
#define BSP_INT_MMA               8
#define BSP_INT_COMP              9
#define BSP_INT_MSIRQ            10  
#define BSP_INT_GPIO_PORTA       11       
#define BSP_INT_GPIO_PORTB       12       
#define BSP_INT_GPIO_PORTC       13       
#define BSP_INT_LCDC             14 
#define BSP_INT_SIM_IRQ          15    
#define BSP_INT_SIM_DATA         16     
#define BSP_INT_RTC              17
#define BSP_INT_RTC_SAM          18    
#define BSP_INT_UART2_PFERR      19        
#define BSP_INT_UART2_RTS        20      
#define BSP_INT_UART2_DTR        21      
#define BSP_INT_UART2_UARTC      22        
#define BSP_INT_UART2_TX         23     
#define BSP_INT_UART2_RX         24     
#define BSP_INT_UART1_PFERR      25        
#define BSP_INT_UART1_RTS        26      
#define BSP_INT_UART1_DTR        27      
#define BSP_INT_UART1_UARTC      28        
#define BSP_INT_UART1_TX         29     
#define BSP_INT_UART1_RX         30     
#define BSP_INT_RES31            31 
#define BSP_INT_RES32            32 
#define BSP_INT_PEN_DATA         33    
#define BSP_INT_PWM              34
#define BSP_INT_MMC_IRQ          35   
#define BSP_INT_SSI2_TX          36   
#define BSP_INT_SSI2_RX          37   
#define BSP_INT_SSI2_ERR         38    
#define BSP_INT_I2C              39
#define BSP_INT_SPI2             40
#define BSP_INT_SPI1             41
#define BSP_INT_SSI_TX           42  
#define BSP_INT_SSI_TX_ERR       43      
#define BSP_INT_SSI_RX           44   
#define BSP_INT_SSI_RX_ERR       45      
#define BSP_INT_TOUCH            46 
#define BSP_INT_USBD0            47 
#define BSP_INT_USBD1            48 
#define BSP_INT_USBD2            49 
#define BSP_INT_USBD3            50 
#define BSP_INT_USBD4            51 
#define BSP_INT_USBD5            52 
#define BSP_INT_USBD6            53 
#define BSP_INT_UART3_RX         54    
#define BSP_INT_BTSYS            55 
#define BSP_INT_BTTIM            56 
#define BSP_INT_BTWUI            57 
#define BSP_INT_TIMER2           58  
#define BSP_INT_TIMER1           59   
#define BSP_INT_DMA_ERR          60    
#define BSP_INT_DMA              61
#define BSP_INT_GPIO_PORTD       62      
#define BSP_INT_WDT              63
#define BSP_MAX_INT              64
              
typedef struct {
    rtems_irq_hdl       vector;
    rtems_irq_hdl_param data;
} mc9328mxl_irq_info_t;



extern mc9328mxl_irq_info_t bsp_vector_table[BSP_MAX_INT];

/*
 * function to initialize the interrupt for a specific BSP
 */
void BSP_rtems_irq_mngt_init();


#endif /* __asm__ */

#ifdef __cplusplus
}
#endif

#endif /* __IRQ_H__ */
