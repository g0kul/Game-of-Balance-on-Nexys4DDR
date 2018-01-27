/*****************************************************************************
* lab2a.c for Gob of ECE 153a at UCSB
* Date of the Last Update:  October 23,2014
*****************************************************************************/

#define AO_LAB2A

#include "gob_top.h"


typedef struct GobTag  {               //Gob State machine
	QActive super;
}  Gob;

Gob AO_Gob;

/* Setup state machines */
/**********************************************************************/
static QState Gob_initial (Gob *me);
static QState Gob_Idle  (Gob *me);
static QState Gob_Play  (Gob *me);
static QState Gob_Over  (Gob *me);
void reset_data_create_maze(uint16_t difficulty);

/**********************************************************************/
//array for storing maze graph data
uint8_t maze_arr[MAZE_Y_SIZE][MAZE_X_SIZE];

//update player and destination co-ordinates - in display maze to enable automatic
uint8_t player_x = 0, player_y = 0, dest_x = 0, dest_y = 0;

//update score and life
uint8_t player_score = 0, player_life = 0;

//for generating random number to maze random generation
static uint16_t lfsr = 0xACE1u;         /* Any nonzero start state will work. */

//for keeping track of stars obtained in a game
GobStarsTrack GobStarsCache;

/**********************************************************************/

//static int ignore_timer_flag = 0;

void Gob_ctor(void)  {
	Gob *me = &AO_Gob;
	QActive_ctor(&me->super, (QStateHandler)&Gob_initial);
}


QState Gob_initial(Gob *me) {
	//xil_printf("\n\rInitialization");
	setFont(BigFont);
    return Q_TRAN(&Gob_Idle);
}

/* Create Gob_Idle state and do any initialization code if needed */
/******************************************************************/

QState Gob_Idle(Gob *me) {

	int bit;

	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			//xil_printf("Startup State A\n");
			//TODO: change it dynamically
			return Q_HANDLED();
		}

		//UP - Not Used
		case BTN_UP_CLICK: {
			//xil_printf("BTN_UP_CLICK from State A\n");
			return Q_HANDLED();
		}

		//Down - calibrate
		case BTN_DOWN_CLICK: {
			//xil_printf("BTN_DOWN_CLICK from State A\n");
			acl_calibrate();
			return Q_HANDLED();
		}

		//Left - Start easy
		case BTN_LEFT_CLICK: {
			//xil_printf("BTN_LEFT_CLICK from State A\n");
			//gob_create_maze();	//should update initial and final pos globally
			reset_data_create_maze(EASY);
			return Q_TRAN(&Gob_Play);
		}

		//Right - Stop Play (not here)
		case BTN_RIGHT_CLICK: {
			//xil_printf("BTN_RIGHT_CLICK from State A\n");
			return Q_HANDLED();
		}

		//Center - start hard
		case BTN_CENTER_CLICK: {
			//xil_printf("BTN_CENTER_CLICK from State A\n");
			reset_data_create_maze(HARD);
			return Q_TRAN(&Gob_Play);
		}

		case USER_TIMEOUT: {
			//xil_printf("USER_TIMEOUT from State A\n");

			//Compute Random Number generation in Idle
			//16-bit maximal-period Galois LFSR
			bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
			lfsr =  (lfsr >> 1) | (bit << 15);

			return Q_HANDLED();
		}

	}

	return Q_SUPER(&QHsm_top);

}

QState Gob_Play(Gob *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			//xil_printf("Startup State B\n");
			//TODO: Get position
			return Q_HANDLED();
		}

		case BTN_UP_CLICK: {
			//xil_printf("BTN_UP_CLICK from State B\n");
			return Q_HANDLED();
		}
		//Down - calibrate while playing too
		case BTN_DOWN_CLICK: {
			//xil_printf("BTN_DOWN_CLICK from State B\n");
			acl_calibrate();
			return Q_HANDLED();
		}

		case BTN_LEFT_CLICK: {
			//xil_printf("BTN_LEFT_CLICK from State B\n");
			return Q_HANDLED();
		}

		//Right - Stop
		case BTN_RIGHT_CLICK: {
			//xil_printf("BTN_RIGHT_CLICK from State B\n");
			//TODO: Call necessary display functions
			start_screen(1);
			return Q_TRAN(&Gob_Idle);	//go to idle on stop
		}

		case BTN_CENTER_CLICK: {
			//xil_printf("BTN_CENTER_CLICK from State B\n");
			//if(generateTimeOut(DISPLAY_TIMEOUT))
			//	ignore_timer_flag = 1;
			//textdisplay(BTN_CENTER_CLICK);
			return Q_HANDLED();
		}

		case USER_TIMEOUT: {
			//xil_printf("USER_TIMEOUT from State B\n");

			//Do the ACL rd and related processing
			switch(do_acl_access())
			{
				case MOVE_DEST_REACHED:
				{
					//TODO: Update Display score, can be done in move_ball?
					victory_screen();
					return Q_TRAN(&Gob_Over);	//go to idle on victory
				}
				case MOVE_LIFE_OVER:
				{
					//TODO: Display Game Over with score, can be done in move_ball?
					end_screen();
					return Q_TRAN(&Gob_Over);	//go to idle on victory
				}
			}

			//clear the status to tell that the interrupt event is served
			//clearInterruptStat();

			//Assuming only one previous timeout in the qp event queue as
			//timeout is 2 seconds which is higher than QP handling time
			//if(!ignore_timer_flag)
			//	clr_inact();
			//else
			//	ignore_timer_flag = 0;
			return Q_HANDLED();
		}

	}

	return Q_SUPER(&Gob_Idle);

}


QState Gob_Over(Gob *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			//xil_printf("Startup State A\n");
			return Q_HANDLED();
		}

		//UP - Not Used
		case BTN_UP_CLICK: {
			//xil_printf("BTN_UP_CLICK from State A\n");
			return Q_HANDLED();
		}

		//Down - calibrate
		case BTN_DOWN_CLICK: {
			//xil_printf("BTN_DOWN_CLICK from State A\n");
			acl_calibrate();
			return Q_HANDLED();
		}

		//Left - Start easy
		case BTN_LEFT_CLICK: {
			//xil_printf("BTN_LEFT_CLICK from State A\n");
			//gob_create_maze();	//should update initial and final pos globally
			reset_data_create_maze(EASY);
			return Q_TRAN(&Gob_Play);
		}

		//Right - Stop
		case BTN_RIGHT_CLICK: {
			//xil_printf("BTN_RIGHT_CLICK from State B\n");
			//TODO: Call necessary display functions
			start_screen(0);
			return Q_TRAN(&Gob_Idle);	//go to idle on stop
		}

		//Center - start hard
		case BTN_CENTER_CLICK: {
			//xil_printf("BTN_CENTER_CLICK from State A\n");
			reset_data_create_maze(HARD);
			return Q_TRAN(&Gob_Play);
			return Q_HANDLED();
		}

		case USER_TIMEOUT: {
			//xil_printf("USER_TIMEOUT from State A\n");
			return Q_HANDLED();
		}

	}

	return Q_SUPER(&Gob_Idle);

}

//reset player data and create new maze
void reset_data_create_maze(uint16_t difficulty)
{
	//reset globals
	//player_x = 1;
	//player_y = 1;
	player_score = 0;
	player_life = PLAYER_LIFE_COUNT;
	GobStarsCache.index = 0;

	//generate maze, send mode with random value
	maze_gen(MAZE_X_SIZE, MAZE_Y_SIZE, lfsr, difficulty);

	//update the display
	display_maze();

	//calibrate the current user acl position
	acl_calibrate();
}


/*-------------only comments below --------------//

 //Maze Generated from: Jacek Wieczorek's depth-first search maze generator
//0-wall; 1-space; 2-player; 3-destination; 4-star; 5-hole;
uint8_t maze_arr[MAZE_Y_SIZE][MAZE_X_SIZE] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 2, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0},
		{0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0},
		{0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 4, 0, 1, 1, 1, 0, 1, 1, 5, 0},
		{0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0},
		{0, 1, 0, 4, 1, 1, 0, 1, 1, 1, 1, 5, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
		{0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0},
		{0, 1, 1, 1, 1, 5, 0, 1, 1, 4, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
		{0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 3, 0, 5, 1, 1, 1, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0},
		{0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0},
		{0, 1, 1, 4, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0},
		{0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 5, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 1, 1, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
		};


 */
