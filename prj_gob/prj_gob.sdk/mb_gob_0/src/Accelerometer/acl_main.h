/*
 * acl_main.h
 *
 *  Created on: Nov 25, 2017
 *      Author: gokul
 */

#ifndef SRC_ACCELEROMETER_ACL_MAIN_H_
#define SRC_ACCELEROMETER_ACL_MAIN_H_

#include "../Accelerometer/ADXL362.h"
#include "../Accelerometer/spi.h"

enum aclMoves {
	MOVE_ERROR = 0,
	MOVE_HANDLED,
	MOVE_DEST_REACHED,
	MOVE_LIFE_OVER
};

static int SENSITIVITY = 200;	//500 - half sliding; 1000 - full sliding;

static int XDEFAULT, YDEFAULT;

void acl_init();
void acl_calibrate();
int do_acl_access();

#endif /* SRC_ACCELEROMETER_ACL_MAIN_H_ */
