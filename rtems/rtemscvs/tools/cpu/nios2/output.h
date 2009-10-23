/*
 *  Copyright (c) 2006 Kolja Waschk rtemsdev/ixo.de
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: output.h,v 1.2 2006/08/15 21:02:55 joel Exp $
 */

#ifndef __OUTPUT_H
#define __OUTPUT_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ptf.h"
#include "clocks.h"
#include "devices.h"

void fwrite_value(struct ptf_item *pi, void *arg);
void fwrite_header_file(FILE *file, struct ptf *cfg, device_desc *devices, clock_desc *clocks);

#endif


