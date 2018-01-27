/*****************************************************************************
* main.c for Gob
* Date of the Last Update:  Nov, 2017
*****************************************************************************/

#include "QPNano/qpn_port.h"                                       /* QP-nano port */
#include "BSP/bsp.h"                             /* Board Support Package (BSP) */
#include "gob_top.h"                               /* application interface */



static QEvent l_gobQueue[30];

QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
	{ (QActive *)0,            (QEvent *)0,          0                    },
	{ (QActive *)&AO_Gob,      l_gobQueue,           Q_DIM(l_gobQueue)    }
};

Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);

// Do not edit main, unless you have a really good reason
int main(void) {
	Gob_ctor(); // inside of lab2a.c
	BSP_init(); // inside of bsp.c, starts out empty!
	QF_run(); // inside of qfn.c
	return 0;
}
