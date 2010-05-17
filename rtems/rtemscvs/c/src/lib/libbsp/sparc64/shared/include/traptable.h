/*
 * traptable.h
 *
 * Copyright 2010 Gedare Bloom
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 * $Id$
 */

/* This file can be included by assembly code */

#define TABLE_SIZE (1024*32)

#ifndef ASM
extern void* trap_table[TABLE_SIZE];
#endif

