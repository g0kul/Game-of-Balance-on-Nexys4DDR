/*****************************************************************************
* bsp.c for Gob of ECE 153a at UCSB
* Date of the Last Update:  October 23,2014
*****************************************************************************/

/**/
#include "bsp.h"

/*****************************/

/* Define all variables and Gpio objects here  */
#if ENABLE_ROT_ENC
XGpio Twist_gpio_inst, Pb_gpio_inst;
#endif
XGpio Btn_gpio_inst;		//, GpioInstance1;
XIntc sys_intc;
XTmrCtr sys_tmrctr;

#if ENABLE_ROT_ENC
unsigned int t_up_flag = 0, t_down_flag = 0;

static int twist_prev_state = 0b11, twist_current_state = 0b11;
static int pb_flag = 0; //, twist_flag = 0;
static int cw_count = 0, ccw_count = 0;
#endif

static int timer_interrupt_flag = 0;

#define GPIO_CHANNEL1 1

void debounceInterrupt(); // Write This function

// Create two interrupt controllers XIntc
// Create two static XGpio variables
// Suggest Creating two int's to use for determining the direction of twist

/*..........................................................................*/
void BSP_init(void)
{
	/* Setup LED's, etc */
	/* Setup interrupts and reference to interrupt handler function(s)  */

	/*
	 * Initialize the interrupt controller driver so that it's ready to use.
	 * specify the device ID that was generated in xparameters.h
	 *
	 * Initialize GPIO and connect the interrupt controller to the GPIO.
	 *
	 */

	XStatus Status;


	//Enable caches

	Xil_ICacheEnable();
	Xil_DCacheEnable();

	//---------------------
	//LCD Initialization
	lcd_spi_init();

	//Accelerometer initialization
	acl_init();
	//---------------------

	/* Initialize the GPIO driver. */
#if ENABLE_ROT_ENC
	Status = XGpio_Initialize(&Twist_gpio_inst, XPAR_TWIST_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("GPIO Initialization Failed!\r\n");
		//return XST_FAILURE;
	}
	#if DEBUG_MESSAGES
		xil_printf("GPIO Twist initialized!\r\n");
	#endif

	Status = XGpio_Initialize(&Pb_gpio_inst, XPAR_PUSH_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("GPIO Initialization Failed!\r\n");
		//return XST_FAILURE;
	}
	#if DEBUG_MESSAGES
		xil_printf("GPIO Pb initialized!\r\n");
	#endif
#endif

	Status = XGpio_Initialize(&Btn_gpio_inst, XPAR_AXI_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("GPIO Initialization Failed!\r\n");
		//return XST_FAILURE;
	}
	#if DEBUG_MESSAGES
		xil_printf("GPIO Btn initialized!\r\n");
	#endif

	//-------------------------
	/*
	 * Initialize the interrupt controller driver so that
	 * it is ready to use, specify the device ID that is generated in
	 * xparameters.h
	 */
	Status = XST_SUCCESS;
	Status = XIntc_Initialize(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);

	if ( Status != XST_SUCCESS )
	{
		if( Status == XST_DEVICE_NOT_FOUND )
		{
			xil_printf("XST_DEVICE_NOT_FOUND...\r\n");
		}
		else
		{
			xil_printf("a different error from XST_DEVICE_NOT_FOUND...\r\n");
		}


		xil_printf("Interrupt controller driver failed to be initialized...\r\n");
		//return XST_FAILURE;
	}

	#if DEBUG_MESSAGES
		xil_printf("Interrupt controller driver initialized!\r\n");
	#endif
	/*
	 * Connect the application handler that will be called when an interrupt
	 * for the timer occurs
	 */
	Status = XIntc_Connect(&sys_intc,XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_0_INTERRUPT_INTR,
			(XInterruptHandler)XTmrCtr_InterruptHandler, &sys_tmrctr);
	if ( Status != XST_SUCCESS )
	{
		xil_printf("Failed to connect the application handlers to the interrupt controller...\r\n");
		//return XST_FAILURE;
	}
	#if DEBUG_MESSAGES
		xil_printf("Connected to Interrupt Controller - Timer!\r\n");
	#endif

	//---------
#if ENABLE_ROT_ENC
	/* Hook up interrupt service routine to the GPIO*/
	XIntc_Connect(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_TWIST_IP2INTC_IRPT_INTR,
		      (Xil_ExceptionHandler)TwistHandler, &Twist_gpio_inst);
	if ( Status != XST_SUCCESS )
	{
		xil_printf("Failed to connect the application handlers to the interrupt controller...\r\n");
		//return XST_FAILURE;
	}
	#if DEBUG_MESSAGES
		xil_printf("Connected to Interrupt Controller - GPIO Tw!\r\n");
	#endif

	/* Hook up interrupt service routine to the GPIO*/
	XIntc_Connect(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_PUSH_IP2INTC_IRPT_INTR,
		      (Xil_ExceptionHandler)PbHandler, &Pb_gpio_inst);
	if ( Status != XST_SUCCESS )
	{
		xil_printf("Failed to connect the application handlers to the interrupt controller...\r\n");
		//return XST_FAILURE;
	}
	#if DEBUG_MESSAGES
		xil_printf("Connected to Interrupt Controller - GPIO Pb!\r\n");
	#endif
#endif

	/* Hook up interrupt service routine to the GPIO*/
	XIntc_Connect(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_GPIO_0_IP2INTC_IRPT_INTR,
		      (Xil_ExceptionHandler)BtnHandler, &Btn_gpio_inst);
	if ( Status != XST_SUCCESS )
	{
		xil_printf("Failed to connect the application handlers to the interrupt controller...\r\n");
		//return XST_FAILURE;
	}
	#if DEBUG_MESSAGES
		xil_printf("Connected to Interrupt Controller - GPIO Btn!\r\n");
	#endif

	//---------

	/*
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts.
	 */
	Status = XIntc_Start(&sys_intc, XIN_REAL_MODE);
	if ( Status != XST_SUCCESS )
	{
		xil_printf("Interrupt controller driver failed to start...\r\n");
		//return XST_FAILURE;
	}
	#if DEBUG_MESSAGES
		xil_printf("Started Interrupt Controller!\r\n");
	#endif

	//-------------------------
		
}

/*..........................................................................*/
void QF_onStartup(void)
{
	/* entered with interrupts locked */

	XStatus Status;

	/* Enable interrupts */
	#if DEBUG_MESSAGES
		xil_printf("\n\rQF_onStartup\n"); // Comment out once you are in your complete program
	#endif

	/*
	 * Enable the interrupt for the timer counter
	 */
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_0_INTERRUPT_INTR);

	//---------

	/*
	 * Enable the interrupt for the gpio
	 */
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_GPIO_0_IP2INTC_IRPT_INTR);
#if ENABLE_ROT_ENC
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_TWIST_IP2INTC_IRPT_INTR);
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_PUSH_IP2INTC_IRPT_INTR);
#endif

	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */
#if ENABLE_ROT_ENC
	XGpio_InterruptEnable(&Twist_gpio_inst, GPIO_CHANNEL1);
	XGpio_InterruptGlobalEnable(&Twist_gpio_inst);

	XGpio_InterruptEnable(&Pb_gpio_inst, GPIO_CHANNEL1);
	XGpio_InterruptGlobalEnable(&Pb_gpio_inst);
#endif

	XGpio_InterruptEnable(&Btn_gpio_inst, GPIO_CHANNEL1);
	XGpio_InterruptGlobalEnable(&Btn_gpio_inst);
	/*
	 * Initialize the exception table and register the interrupt
	 * controller handler with the exception table
	 */
	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 (Xil_ExceptionHandler)XIntc_InterruptHandler, &sys_intc);

	/* Enable non-critical exceptions */
	Xil_ExceptionEnable();

	//---------
	/*
	 * Initialize the timer counter so that it's ready to use,
	 * specify the device ID that is generated in xparameters.h
	 */
	Status = XTmrCtr_Initialize(&sys_tmrctr, XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);
	if ( Status != XST_SUCCESS )
	{
		xil_printf("Timer initialization failed...\r\n");
		//return XST_FAILURE;
	}
	#if DEBUG_MESSAGES
		xil_printf("Initialized Timer!\r\n");
	#endif
	/*
	 * Setup the handler for the timer counter that will be called from the
	 * interrupt context when the timer expires, specify a pointer to the
	 * timer counter driver instance as the callback reference so the handler
	 * is able to access the instance data
	 */
	XTmrCtr_SetHandler(&sys_tmrctr, interrupt_timer_handler, &sys_tmrctr);

	/*
	 * Enable the interrupt of the timer counter so interrupts will occur
	 * and use auto reload mode such that the timer counter will reload
	 * itself automatically and continue repeatedly, without this option
	 * it would expire once only
	 */
	XTmrCtr_SetOptions(&sys_tmrctr, TIMER0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);

	/*
	 * Enable the interrupt of the timer counter so interrupts will occur
	 * and use auto reload mode such that the timer counter will reload
	 * itself automatically and continue repeatedly, without this option
	 * it would expire once only
	 */
	XTmrCtr_SetOptions(&sys_tmrctr, TIMER1, XTC_INT_MODE_OPTION);// | XTC_AUTO_RELOAD_OPTION);
	/*
	 * Set a reset value for the timer counter such that it will expire
	 * eariler than letting it roll over from 0, the reset value is loaded
	 * into the timer counter when it is started
	 */
	XTmrCtr_SetResetValue(&sys_tmrctr, TIMER0, 0xFFFFFFFF-TICK_INTERVAL);		// 0x17D7840 = 25*10^6 clk cycles @ 50MHz = 500ms

	/*
	 * Start the timer counter such that it's incrementing by default,
	 * then wait for it to timeout a number of times
	 */
	XTmrCtr_Start(&sys_tmrctr, TIMER0);

	/*
	 * Register the intc device driver’s handler with the Standalone
	 * software platform’s interrupt table
	 */
	microblaze_register_handler((XInterruptHandler)XIntc_DeviceInterruptHandler,
			(void*)XPAR_MICROBLAZE_0_AXI_INTC_DEVICE_ID);

	#if DEBUG_MESSAGES
		xil_printf("Registers handled!\r\n");
	#endif

	/*
	 * Enable interrupts on MicroBlaze
	 */
	microblaze_enable_interrupts();
	#if DEBUG_MESSAGES
		xil_printf("Interrupts enabled!\r\n");
	#endif
	/*
	 * At this point, the system is ready to respond to interrupts from the timer
	 */


	// Variables for reading Microblaze registers to debug your interrupts.
//	{
//		u32 axi_ISR =  Xil_In32(intcPress.BaseAddress + XIN_ISR_OFFSET);
//		u32 axi_IPR =  Xil_In32(intcPress.BaseAddress + XIN_IPR_OFFSET);
//		u32 axi_IER =  Xil_In32(intcPress.BaseAddress + XIN_IER_OFFSET);
//		u32 axi_IAR =  Xil_In32(intcPress.BaseAddress + XIN_IAR_OFFSET);
//		u32 axi_SIE =  Xil_In32(intcPress.BaseAddress + XIN_SIE_OFFSET);
//		u32 axi_CIE =  Xil_In32(intcPress.BaseAddress + XIN_CIE_OFFSET);
//		u32 axi_IVR =  Xil_In32(intcPress.BaseAddress + XIN_IVR_OFFSET);
//		u32 axi_MER =  Xil_In32(intcPress.BaseAddress + XIN_MER_OFFSET);
//		u32 axi_IMR =  Xil_In32(intcPress.BaseAddress + XIN_IMR_OFFSET);
//		u32 axi_ILR =  Xil_In32(intcPress.BaseAddress + XIN_ILR_OFFSET) ;
//		u32 axi_IVAR = Xil_In32(intcPress.BaseAddress + XIN_IVAR_OFFSET);
//		u32 gpioTestIER  = Xil_In32(sw_Gpio.BaseAddress + XGPIO_IER_OFFSET);
//		u32 gpioTestISR  = Xil_In32(sw_Gpio.BaseAddress  + XGPIO_ISR_OFFSET ) & 0x00000003; // & 0xMASK
//		u32 gpioTestGIER = Xil_In32(sw_Gpio.BaseAddress  + XGPIO_GIE_OFFSET ) & 0x80000000; // & 0xMASK
//	}
}


void QF_onIdle(void) {        /* entered with interrupts locked */

    QF_INT_UNLOCK();                       /* unlock interrupts */

    {
    	// Write code to increment your interrupt counter here.
    	// QActive_postISR((QActive *)&AO_Gob, ENCODER_DOWN); is used to post an event to your FSM



// 			Useful for Debugging, and understanding your Microblaze registers.
//    		u32 axi_ISR =  Xil_In32(intcPress.BaseAddress + XIN_ISR_OFFSET);
//    	    u32 axi_IPR =  Xil_In32(intcPress.BaseAddress + XIN_IPR_OFFSET);
//    	    u32 axi_IER =  Xil_In32(intcPress.BaseAddress + XIN_IER_OFFSET);
//
//    	    u32 axi_IAR =  Xil_In32(intcPress.BaseAddress + XIN_IAR_OFFSET);
//    	    u32 axi_SIE =  Xil_In32(intcPress.BaseAddress + XIN_SIE_OFFSET);
//    	    u32 axi_CIE =  Xil_In32(intcPress.BaseAddress + XIN_CIE_OFFSET);
//    	    u32 axi_IVR =  Xil_In32(intcPress.BaseAddress + XIN_IVR_OFFSET);
//    	    u32 axi_MER =  Xil_In32(intcPress.BaseAddress + XIN_MER_OFFSET);
//    	    u32 axi_IMR =  Xil_In32(intcPress.BaseAddress + XIN_IMR_OFFSET);
//    	    u32 axi_ILR =  Xil_In32(intcPress.BaseAddress + XIN_ILR_OFFSET) ;
//    	    u32 axi_IVAR = Xil_In32(intcPress.BaseAddress + XIN_IVAR_OFFSET);
//
//    	    // Expect to see 0x00000001
//    	    u32 gpioTestIER  = Xil_In32(sw_Gpio.BaseAddress + XGPIO_IER_OFFSET);
//    	    // Expect to see 0x00000001
//    	    u32 gpioTestISR  = Xil_In32(sw_Gpio.BaseAddress  + XGPIO_ISR_OFFSET ) & 0x00000003;
//
//    	    // Expect to see 0x80000000 in GIER
//    		u32 gpioTestGIER = Xil_In32(sw_Gpio.BaseAddress  + XGPIO_GIE_OFFSET ) & 0x80000000;


    }
}

/* Do not touch Q_onAssert */
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_LOCK();
    for (;;) {
    }
}

/* Interrupt handler functions here.  Do not forget to include them in lab2a.h!
To post an event from an ISR, use this template:
QActive_postISR((QActive *)&AO_Gob, SIGNALHERE);
Where the Signals are defined in lab2a.h  */

/******************************************************************************
*
* interrupt handler routines
*
******************************************************************************/
#if ENABLE_ROT_ENC
void TwistHandler(void *CallbackRef)
{

	//perform the handling code here
	twist_current_state = XGpio_DiscreteRead(&Twist_gpio_inst, GPIO_CHANNEL1);

	switch(twist_current_state)
	{

	case 0b00:
		if(twist_prev_state == 0b01) //cw2
		{
			cw_count++;
			twist_prev_state = twist_current_state;
		}
		else if(twist_prev_state == 0b10) //ccw2
		{
			ccw_count++;
			twist_prev_state = twist_current_state;
		}
		else
			twist_prev_state = twist_prev_state;			//ignore the debounce
		break;

	case 0b01:
		if(twist_prev_state == 0b11) //cw1
		{
			cw_count++;
			twist_prev_state = twist_current_state;
		}
		else if(twist_prev_state == 0b00) //ccw3
		{
			ccw_count++;
			twist_prev_state = twist_current_state;
		}
		else
			twist_prev_state = twist_prev_state;			//ignore the debounce
		break;

	case 0b10:
		if(twist_prev_state == 0b00) //cw3
		{
			cw_count++;
			twist_prev_state = twist_current_state;
		}
		else if(twist_prev_state == 0b11) //ccw1
		{
			ccw_count++;
			twist_prev_state = twist_current_state;
		}
		else
			twist_prev_state = twist_prev_state;			//ignore the debounce
		break;

	case 0b11:
		if(twist_prev_state == 0b10) //cw
		{
			if(cw_count == 3)
			{
				t_up_flag = 1;
				QActive_postISR((QActive *)&AO_Gob, ENCODER_UP);
			}

			cw_count = 0;
			twist_prev_state = twist_current_state; //update state as prev may be an debounce
		}
		else if(twist_prev_state == 0b01) //ccw
		{
			if(ccw_count == 3)
			{
				t_down_flag = 1;
				QActive_postISR((QActive *)&AO_Gob, ENCODER_DOWN);
			}

			ccw_count = 0;
			twist_prev_state = twist_current_state; //update state as prev may be an debounce
		}
		else if(twist_prev_state == 0b11) //idle
		{
			twist_prev_state = twist_current_state;
			cw_count = 0;
			ccw_count = 0;
		}
		else
			twist_prev_state = twist_prev_state;			//ignore the debounce
		break;

	}
/*
	twist_flag = 1;

	if((t_up_flag == 1) | (t_down_flag == 1))
	{
		//Disable the twist interrupt from keep happening due to debounce, enabling it when timer expires
		XIntc_Disable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_TWIST_IP2INTC_IRPT_INTR);

		XTmrCtr_SetResetValue(&sys_tmrctr, 0, 0xFFFFFFFF-RESET_VALUE);
		XTmrCtr_Start(&sys_tmrctr, 0);
	}
*/
	/* Clear the Interrupt */
	XGpio_InterruptClear(&Twist_gpio_inst, GPIO_CHANNEL1);

}

void PbHandler(void *CallbackRef)
{
	unsigned int pb_rd = 0;

	//perform the handling code here

	pb_rd = XGpio_DiscreteRead(&Pb_gpio_inst, GPIO_CHANNEL1);

	//Disable the pushbutton interrupt from keep happening due to debounce, enabling it when timer expires
	//XIntc_Disable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_PUSH_IP2INTC_IRPT_INTR);

	//QActive_postISR((QActive *)&AO_Gob, ENCODER_CLICK);


	if(pb_rd == 1)
	{
		//Disable the pushbutton interrupt from keep happening due to debounce, enabling it when timer expires
		XIntc_Disable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_PUSH_IP2INTC_IRPT_INTR);

		XTmrCtr_SetResetValue(&sys_tmrctr, 1, 0xFFFFFFFF-RESET_VALUE);
		XTmrCtr_Start(&sys_tmrctr, 1);

		//pb_flag = 1;
		QActive_postISR((QActive *)&AO_Gob, ENCODER_CLICK);
	}


	//pb_count += 1;

	//if(pb_count >= 3)
	//{
	//	pb_flag = 1;
	//	pb_count = 0;
	//}

	/* Clear the Interrupt */
	XGpio_InterruptClear(&Pb_gpio_inst, GPIO_CHANNEL1);

}
#endif

void BtnHandler(void *CallbackRef)
{
	unsigned int btn_rd = 0;

	//perform the handling code here

	btn_rd = XGpio_DiscreteRead(&Btn_gpio_inst, GPIO_CHANNEL1);

	switch(btn_rd)
	{
	case 1:		//up
		QActive_postISR((QActive *)&AO_Gob, BTN_UP_CLICK);
		break;
	case 2:		//left
		QActive_postISR((QActive *)&AO_Gob, BTN_LEFT_CLICK);
		break;
	case 4:		//right
		QActive_postISR((QActive *)&AO_Gob, BTN_RIGHT_CLICK);
		break;
	case 8:		//down
		QActive_postISR((QActive *)&AO_Gob, BTN_DOWN_CLICK);
		break;
	case 16:	//center
		QActive_postISR((QActive *)&AO_Gob, BTN_CENTER_CLICK);
		break;
	}

/*
	{
		//Disable the pushbutton interrupt from keep happening due to debounce, enabling it when timer expires
		XIntc_Disable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_PUSH_IP2INTC_IRPT_INTR);

		XTmrCtr_SetResetValue(&sys_tmrctr, 0, 0xFFFFFFFF-RESET_VALUE_X10);
		XTmrCtr_Start(&sys_tmrctr, 0);

		btn_flag = 1;
		QActive_postISR((QActive *)&AO_Gob, ENCODER_CLICK);
	}
	*/

	/* Clear the Interrupt */
	XGpio_InterruptClear(&Btn_gpio_inst, GPIO_CHANNEL1);

}

void interrupt_timer_handler(void *CallBackRef, u8 TmrCtrNumber)
{
	// This is the interrupt handler function
	// Do not print inside of this function.
	Xuint32 ControlStatusReg;
	/*
	 * Read the new Control/Status Register content.
	 */
	//ControlStatusReg = XTimerCtr_ReadReg(sys_tmrctr.BaseAddress, 0, XTC_TCSR_OFFSET);

	if(TmrCtrNumber == TIMER0)
	{
		timer_interrupt_flag = 1;
		QActive_postISR((QActive *)&AO_Gob, USER_TIMEOUT);
	}

#if ENABLE_ROT_ENC
	if(TmrCtrNumber == TIMER1)
	{
		XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_PUSH_IP2INTC_IRPT_INTR);
	}
#endif

	//XTmrCtr_Stop(&sys_tmrctr, 0);

	//TODO: Debug
	//XGpio_DiscreteWrite(&GpioInstance1, GPIO_CHANNEL1, pb_debounce_wait<<15);

	//Handled in timer handler
	/*
	 * Acknowledge the interrupt by clearing the interrupt
	 * bit in the timer control status register
	 */
	//XTmrCtr_WriteReg(sys_tmrctr.BaseAddress, 0, XTC_TCSR_OFFSET, ControlStatusReg |XTC_CSR_INT_OCCURED_MASK);

}

void interrupt_timer_disable()
{
	XIntc_Disable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_0_INTERRUPT_INTR);
}

void interrupt_timer_enable()
{
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_AXI_TIMER_0_INTERRUPT_INTR);
}


//Returns 0 - if started, 1 - if there was a timer event already occurred and it started again
int generateTimeOut(int value_10ns)
{
	int isTimeoutPendingServe = 0;
	XTmrCtrStats StatsPtr;

	XTmrCtr_GetStats(&sys_tmrctr, &StatsPtr);
	isTimeoutPendingServe = StatsPtr.Interrupts;
	//xil_printf("\nisTPS = %d\n", isTimeoutPendingServe);

	//XTmrCtr_ClearStats(&sys_tmrctr);

	//if(timer_interrupt_flag)
	//{
	//	isdone = timer_interrupt_flag;
	//	timer_interrupt_flag = 0;
	//}

	//XTmrCtr_Reset(&sys_tmrctr,TIMER0);
	//XTmrCtr_SetOptions(&sys_tmrctr, TIMER0, XTC_INT_MODE_OPTION);
	//XTmrCtr_SetResetValue(&sys_tmrctr, TIMER0, 0xFFFFFFFF-value_10ns);
	//XTmrCtr_Start(&sys_tmrctr, TIMER0);

	return isTimeoutPendingServe;
}

void clearInterruptStat()
{
	XTmrCtr_ClearStats(&sys_tmrctr);
}

#if ENABLE_ROT_ENC
void enablePbInterrupt()
{
	XIntc_Enable(&sys_intc, XPAR_MICROBLAZE_0_AXI_INTC_PUSH_IP2INTC_IRPT_INTR);
}
#endif
