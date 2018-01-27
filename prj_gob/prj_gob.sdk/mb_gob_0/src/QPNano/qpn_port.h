/*****************************************************************************
* Microblaze Qnano port qpn_port.h
* Last modified July 15, 2010 at 1:44 PM
*****************************************************************************/
#ifndef qpn_port_h
#define qpn_port_h


#define Q_PARAM_SIZE            0
#define QF_TIMEEVT_CTR_SIZE     1

/* maximum # active objects--must match EXACTLY the QF_active[] definition  */
#define QF_MAX_ACTIVE           1

                              /* interrupt locking policy for XPS compiler */
#define QF_INT_LOCK()           microblaze_disable_interrupts();
#define QF_INT_UNLOCK()         microblaze_enable_interrupts();
#define Q_ROM_PTR(rom_var_)     rom_var_
#define Q_ROM_BYTE(rom_var_)    rom_var_
                                  
#define QF_ISR_NEST
#define Q_ROM
#define Q_ROM_VAR
#define Q_REENTRANT
#define Q_ISR_NEST

#include <stdint.h>       /* IAR provides C99-standard exact-width integers */
#include "qepn.h"         /* QEP-nano platform-independent public interface */
#include "qfn.h"           /* QF-nano platform-independent public interface */



#include "xparameters.h"
#include "xgpio.h"
#include "xtmrctr_l.h"
#include "xintc_l.h"
#include "mb_interface.h"
#include "xbasic_types.h"
#include "xio.h"
#include "xtmrctr.h"


#endif                                                        /* qpn_port_h */
