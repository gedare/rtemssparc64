/*
 *  Support routines for test code.
 *
 *  $Id: test_support.h,v 1.2 2009/10/26 05:33:50 ralf Exp $
 */

#ifndef __TEST_SUPPORT_h
#define __TEST_SUPPORT_h

#ifdef __cplusplus
extern "C" {
#endif

extern void Allocate_majority_of_workspace( int smallest );
extern const char *Get_Too_Long_Name(void);
extern const char *Get_Longest_Name(void);

#ifdef __cplusplus
};
#endif

#endif
