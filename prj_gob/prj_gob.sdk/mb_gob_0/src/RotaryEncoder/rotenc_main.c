#include "rotenc_main.h"

#include "../gob_top.h"

XGpio GpioInstance; /* The Instance of the GPIO Driver used for LED 0 */

static int t_count = 0;

void ledDisplay(int action) {

	int display_val = 0;

	XGpio_Initialize(&GpioInstance, XPAR_GPIO_1_DEVICE_ID);

	if(action == VOLUME_UP)
	{
		t_count = ((t_count << 1) | 1) & 0xFFFF;
		display_val = t_count;
	}
	else if(action == VOLUME_DOWN)
	{
		t_count = t_count >> 1;
		display_val = t_count;
	}
	else if(action == MUTE)
	{
		t_count = t_count;
		display_val = 0;
	}
	else if(action == UNMUTE)
	{
		t_count = t_count;
		display_val = t_count;
	}

	XGpio_DiscreteWrite(&GpioInstance, LED_CHANNEL, t_count);
}
