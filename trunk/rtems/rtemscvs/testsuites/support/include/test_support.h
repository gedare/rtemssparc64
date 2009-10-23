/*
 *  Support routines for test code.
 *
 *  $Id: test_support.h,v 1.1 2008/02/04 19:40:33 jennifer Exp $
 */

#ifndef __TEST_SUPPORT_h
#define __TEST_SUPPORT_h

#ifdef __cplusplus
extern "C" {
#endif

void Allocate_majority_of_workspace( int smallest );
const char *Get_Too_Long_Name();
const char *Get_Longest_Name();

#ifdef __cplusplus
};
#endif

#endif
