/*****************************************************************************
* gob_top.h for Gob of ECE 153a at UCSB
* Date of the Last Update:  October 23,2014
* Created by: Gokul Prasath Nallasami
*****************************************************************************/

#ifndef gob_top_h
#define gob_top_h

#include "QPNano/qpn_port.h"
#include "QPNano/qepn.h"
#include "BSP/bsp.h"
#include "RotaryEncoder/rotenc_main.h"
#include "LCD/lcd_test.h"
#include "xtmrctr.h"
#include "LCD/lcd.h"
#include "Accelerometer/acl_main.h"
#include "Maze/maze.h"

enum GobSignals {
	//ENCODER_UP = Q_USER_SIG,
	//ENCODER_DOWN,
	//ENCODER_CLICK,
	BTN_UP_CLICK = Q_USER_SIG,
	BTN_LEFT_CLICK,
	BTN_RIGHT_CLICK,
	BTN_DOWN_CLICK,
	BTN_CENTER_CLICK,
	USER_TIMEOUT
};

enum Volume_Actions {
	VOLUME_UP = 0,
	VOLUME_DOWN,
	MUTE,
	UNMUTE
};

//0-wall; 1-space; 2-player; 3-destination;
enum maze_arr_legend {
	WALL = 0,
	WAY = 1,
	PLAYER = 2,
	DESTINATION = 3,
	STAR = 4,
	HOLE = 5,
	NOWAY = 6
};

enum maze_difficulty {
	EASY = 0,
	MEDIUM = 1,
	HARD = 2
};

extern struct GobTag AO_Gob;


#define ENABLE_ROT_ENC 0	//Not using rot enc

#if ENABLE_ROT_ENC
void TwistHandler(void *CallbackRef);
void PbHandler(void *CallbackRef);
#define DISPLAY_TIMEOUT					200000000	//2seconds
#endif

void Gob_ctor(void);
void interrupt_timer_handler(void *CallBackRef, u8 TmrCtrNumber);
void BtnHandler(void *CallbackRef);


#define DEBUG_MESSAGES					0	//1-print debug, 0 - no general debug but error cases still prints

//---------Maze---------//

#define MAZE_X_SIZE 23	//23*10 = 230; remaining 10 for the outer border (Width)
#define MAZE_Y_SIZE 27  //31*10 = 310; remaining 10 for the outer border (Height)
#define MAZE_CELL_SIZE 10 //each cell of maze in the array
#define MAZE_BORDER_X_SIZE 5 //5 on x
#define MAZE_BORDER_Y_SIZE 25 //5 on y
#define BALL_SIZE 10

#define MAZE_X_SIZE_M1 MAZE_X_SIZE-1
#define MAZE_Y_SIZE_M1 MAZE_Y_SIZE-1
#define MAZE_CELL_SIZE_M1 MAZE_CELL_SIZE-1
#define MAZE_BORDER_SIZE_M1 MAZE_BORDER_SIZE-1
#define BALL_SIZE_M1 BALL_SIZE-1

#define PLAYER_LIFE_COUNT 3	//number of life for the player
#define PLAYER_LIFE_OVER 0	//when to tell player is out of life

#define PLAYER_STARS_COUNT 3	//to track whether taken or not
#define PLAYER_HOLES_COUNT 5	//max holes to use
//Defines for printing text score and life
#define SCORE_X DISP_X_SIZE-OFSET-OFSET2
#define SCORE_Y 10
#define LIFE_X OFSET2
#define LIFE_Y 10
#define OFSET 50   //Genric text offset for printing
#define OFSET2 35 // offset for the small maze
//..
extern uint8_t maze_arr[MAZE_Y_SIZE][MAZE_X_SIZE];

extern uint8_t player_x, player_y, dest_x, dest_y;
extern uint8_t player_score, player_life;


//Small cache to keep track of stars obtained in a game
typedef struct GobStars {
	uint8_t x[PLAYER_STARS_COUNT];
	uint8_t y[PLAYER_STARS_COUNT];
	uint8_t index;
} GobStarsTrack;

extern struct GobStars GobStarsCache;	//Small cache to keep track of stars obtained in a game

#endif
