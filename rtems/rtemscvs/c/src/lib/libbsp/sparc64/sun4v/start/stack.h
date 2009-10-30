

#ifndef _STACK_H
#define _STACK_H

#ifdef __cplusplus
extern "C" {
#endif

/* GAB: check these numbers */

/* sp and fp are biased 2047 bytes toward lower addresses */
#define STACK_BIAS			2047	
#define STACK_WINDOW_SAVE_AREA_SIZE	(16*8)

#define INITIAL_STACK_SIZE 	8192

#ifdef __cplusplus
}
#endif

#endif
