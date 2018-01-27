/*****************************************************************************
* Product: QF-nano implemenation
* Last Updated for Version: 4.0.05
* Date of the Last Update:  May 05, 2009
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2009 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qpn_port.h"                                       /* QP-nano port */

Q_DEFINE_THIS_MODULE(qfn_init)

/**
* \file
* \ingroup qfn_init
* QF-nano implementation.
*/

void QF_init(void) {
    QActive *a;
    uint8_t p;

    QF_readySet_ = (uint8_t)0;

#ifdef QK_PREEMPTIVE
    QK_currPrio_ = (uint8_t)(QF_MAX_ACTIVE + 1);

#ifdef QF_ISR_NEST
    QK_intNest_ = (uint8_t)0;
#endif

#ifdef QK_MUTEX
    QK_ceilingPrio_ = (uint8_t)0;
#endif

#endif                                              /* #ifdef QK_PREEMPTIVE */

                                  /* clear all registered active objects... */
    for (p = (uint8_t)1; p <= (uint8_t)QF_MAX_ACTIVE; ++p) {
        a = (QActive *)Q_ROM_PTR(QF_active[p].act);

        Q_ASSERT(a != (QActive *)0);    /* QF_active[p] must be initialized */
        a->head    = (uint8_t)0;
        a->tail    = (uint8_t)0;
        a->nUsed   = (uint8_t)0;
#if (QF_TIMEEVT_CTR_SIZE != 0)
        a->tickCtr = (QTimeEvtCtr)0;
#endif
    }
}
