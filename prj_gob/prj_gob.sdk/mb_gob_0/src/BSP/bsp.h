/*****************************************************************************
* bsp.h for Lab2A of ECE 153a at UCSB
* Date of the Last Update:  October 23,2014
*****************************************************************************/
#ifndef bsp_h
#define bsp_h

/* headers to inclue........................................................*/
#include "../QPNano/qpn_port.h"
#include "xintc.h"
#include "xil_cache.h"		// Cache Drivers
#include "xil_exception.h"
#include "../LCD/lcd_test.h"
#include "../Accelerometer/acl_main.h"
#include "../gob_top.h"


#define ENABLE_ROT_ENC 0	//Not using rot enc

#if DEBUG_MESSAGES
#define TICK_INTERVAL 150000000 //2.5s for letting the debug msg to print
#else
#define TICK_INTERVAL 10000000 //10ms 25000000 //250ms //250000000 //2.5s
#endif


#define TIMER0 0	//Timer Number
#define TIMER1 1	//Timer Number

/* bsp functions ..........................................................*/

#if ENABLE_ROT_ENC
void TwistHandler(void *CallbackRef);
void PbHandler(void *CallbackRef);
#endif

void interrupt_timer_handler(void *CallBackRef, u8 TmrCtrNumber);
void BtnHandler(void *CallbackRef);

void BSP_init(void);
void ISR_gpio(void);
void ISR_timer(void);
int generateTimeOut(int);
void clearInterruptStat(void);
void enablePbInterrupt(void);

//extern int twist_prev_state, twist_current_state;
//extern int pb_flag, twist_flag;
//extern int cw_count, ccw_count;

#define BSP_showState(prio_, state_) ((void)0)


#endif                                                             /* bsp_h */


