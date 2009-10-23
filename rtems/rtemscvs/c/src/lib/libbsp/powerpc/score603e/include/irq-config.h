/**
 * @file
 *
 * @ingroup bsp_interrupt
 *
 * @brief BSP interrupt support configuration.
 *
 * $Id: irq-config.h,v 1.2 2009/05/05 16:24:04 jennifer Exp $
 */

/*
 * Copyright (c) 2008
 * Embedded Brains GmbH
 * Obere Lagerstr. 30
 * D-82178 Puchheim
 * Germany
 * rtems@embedded-brains.de
 *
 * The license and distribution terms for this file may be found in the file
 * LICENSE in this distribution or at http://www.rtems.com/license/LICENSE.
 */

#ifndef LIBBSP_POWERPC_SCORE603E_IRQ_CONFIG_H
#define LIBBSP_POWERPC_SCORE603E_IRQ_CONFIG_H

#include <bsp/irq.h>

/**
 * @addtogroup bsp_interrupt
 *
 * @{
 */

/**
 * @brief Minimum vector number.
 */
#define BSP_INTERRUPT_VECTOR_MIN BSP_LOWEST_OFFSET

/**
 * @brief Maximum vector number.
 */
#define BSP_INTERRUPT_VECTOR_MAX BSP_MAX_OFFSET

/** @} */

#endif /* LIBBSP_POWERPC_SCORE603E_IRQ_CONFIG_H */
