//header

#ifndef maze_h
#define maze_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "../gob_top.h"

#define START_OFFSET 1 //start player which width to place
#define STOP_OFFSET MAZE_Y_SIZE_M1-1 //stop player which width to place

#define LIFE_BASE 	5	//start from width 10*CELL_SIZE
#define LIFE_OFFSET	5	//offset b/w each star 5*CELL_SIZE

int maze_gen(uint16_t maze_width, uint16_t maze_height, uint16_t seed, uint16_t difficulty);

#endif
