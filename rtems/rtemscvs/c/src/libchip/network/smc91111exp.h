/*
 *  $Id: smc91111exp.h,v 1.2 2006/01/09 15:41:42 ralf Exp $
 */

#ifndef _SMC91111_EXP_H_
#define _SMC91111_EXP_H_

typedef struct scmv91111_configuration {
  void                     *baseaddr;
  unsigned int              vector;
  unsigned int              pio;
  unsigned int              ctl_rspeed;
  unsigned int              ctl_rfduplx;
  unsigned int              ctl_autoneg;
} scmv91111_configuration_t;

#endif  /* _SMC_91111_EXP_H_ */


