/*
 *  Copyright (c) 2006 Kolja Waschk rtemsdev/ixo.de
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: clocks.h,v 1.1 2006/08/09 21:05:32 joel Exp $
 */

#ifndef __NIOS2GEN_CLOCKS_H
#define __NIOS2GEN_CLOCKS_H 1

#include "ptf.h"

typedef struct clockdsc
{
    char *name;
    char *cfgname;
    unsigned long freq;
    struct clockdsc *next;
}
clock_desc;

clock_desc *find_clocks( struct ptf *ptf, struct ptf *cfg );

#endif
