/*===============================================================*\
| Project: display driver for HCMS29xx                            |
+-----------------------------------------------------------------+
| File: font_hcms29xx.h                                           |
+-----------------------------------------------------------------+
|                    Copyright (c) 2008                           |
|                    Embedded Brains GmbH                         |
|                    Obere Lagerstr. 30                           |
|                    D-82178 Puchheim                             |
|                    Germany                                      |
|                    rtems@embedded-brains.de                     |
+-----------------------------------------------------------------+
| The license and distribution terms for this file may be         |
| found in the file LICENSE in this distribution or at            |
|                                                                 |
| http://www.rtems.com/license/LICENSE.                           |
|                                                                 |
+-----------------------------------------------------------------+
| This file declares the 5x7 bit font used in disp_hcms29xx       |
+-----------------------------------------------------------------+
|  $Id: font_hcms29xx.h,v 1.2 2008/07/17 14:39:34 thomas Exp $
\*===============================================================*/

#ifndef FONT_HCMS29XX_H
#define FONT_HCMS29XX_H

#include "disp_fonts.h"


#ifdef __cplusplus
extern "C" {
#endif

extern disp_font_t font_hcms29xx;

extern struct disp_font_base font_hcms29xx_base;

#ifdef __cplusplus
}
#endif

#endif /* not defined FONT_HCMS29XX_H */
