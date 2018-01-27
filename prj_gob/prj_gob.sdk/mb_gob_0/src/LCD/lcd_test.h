//header

#ifndef LCD_TEST_H_
#define LCD_TEST_H_

int lcd_spi_init();

//-maze--//
void drawMazeBrd();
void move_ball(uint16_t tox,uint16_t toy);
void display_maze();
void start_screen(int mode);
void end_screen();
void victory_screen();

//Blue color for ball
#define BALL_R 0
#define BALL_G 0
#define BALL_B 255

//Yellow Color for Destination
#define DEST_R 255
#define DEST_G 255
#define DEST_B 0

//Sienna brown Wall Color
#define WALL_R 160
#define WALL_G 82
#define WALL_B 45

// Star Green Color
#define STAR_R 0
#define STAR_G 150
#define STAR_B 0

// Hole Red Color
#define HOLE_R 200
#define HOLE_G 0
#define HOLE_B 0

//WAY Black Color
#define WAY_COL 0

//Border Black Color
#define BRD_COL 0

//End screen Color
#define ST_END_R 0
#define ST_END_G 255
#define ST_END_B 0

//Start and end screen font color
#define ST_SCR_FONT_R 0
#define ST_SCR_FONT_G 255
#define ST_SCR_FONT_B 0
#define END_SCR_FONT_R 255
#define END_SCR_FONT_G 255
#define END_SCR_FONT_B 255

//Victory screen and font color
#define VIC_SCR_FONT_R 255
#define VIC_SCR_FONT_G 255
#define VIC_SCR_FONT_B 255
#define VIC_SCR_R 255
#define VIC_SCR_G 0
#define VIC_SCR_B 255

//Start screen color
#define ST_SCR_R 255
#define ST_SCR_G 153
#define ST_SCR_B 153

//---

#endif
