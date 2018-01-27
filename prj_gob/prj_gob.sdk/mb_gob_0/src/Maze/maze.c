//Maze Generated from: Jacek Wieczorek's depth-first search maze generator
//Code by Jacek Wieczorek;
//Modified by Gokul Prasath and Barath Kumar;

#include "maze.h"

typedef struct
{
	uint16_t x, y; //Node position - little waste of memory, but it allows faster generation
	void *parent; //Pointer to parent node
	uint8_t c; //Character to be displayed
	uint8_t dirs; //Directions that still haven't been explored
} Node;

static Node nodes[MAZE_X_SIZE*MAZE_Y_SIZE]; //Nodes array
static uint16_t width, height; //Maze dimensions
static uint16_t difficulty;

int maze_init();
void maze_update_arr();

int maze_init()
{
	uint16_t i, j;
	Node *n;

	//Allocate memory for maze
	//nodes = calloc(width * height, sizeof(Node));
	if (nodes == NULL) return 1;

	//Setup crucial nodes
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			n = nodes + i + j * width;
			if (i * j % 2)
			{
				n->x = i;
				n->y = j;
				n->dirs = 15; //Assume that all directions can be explored (4 youngest bits set)
				n->c = WAY;
				n->parent = NULL;
			}
			else n->c = WALL; //Add walls between nodes
		}
	}
	return 0;
}

Node *maze_link(Node *n)
{
	//Connects node to random neighbor (if possible) and returns
	//address of next node that should be visited
	uint16_t x, y;
	uint16_t dir;
	Node *dest;

	//Nothing can be done if null pointer is given - return
	if (n == NULL) return NULL;

	//While there are directions still unexplored
	while (n->dirs)
	{
		//xil_printf("%d\n",n->dirs);
		//Randomly pick one direction
		dir = (1 << (rand() % 4));


		//If it has already been explored - try again
		if (~n->dirs & dir)
		continue;
		//xil_printf("L1");


		//Mark direction as explored
		n->dirs &= ~dir;

		//Depending on chosen direction
		switch (dir)
		{
			//Check if it's possible to go right
		case 1:
			if (n->x + 2 < width)
			{
				x = n->x + 2;
				y = n->y;
			}
			else continue;
			break;

			//Check if it's possible to go down
		case 2:
			if (n->y + 2 < height)
			{
				x = n->x;
				y = n->y + 2;
			}
			else continue;
			break;

			//Check if it's possible to go left 
		case 4:
			if (n->x - 2 >= 0)
			{
				x = n->x - 2;
				y = n->y;
			}
			else continue;
			break;

			//Check if it's possible to go up
		case 8:
			if (n->y - 2 >= 0)
			{
				x = n->x;
				y = n->y - 2;
			}
			else continue;
			break;
		}

		//Get destination node into pointer (makes things a tiny bit faster)
		dest = nodes + x + y * width;
		//xil_printf("L2");
		//Make sure that destination node is not a wall
		if (dest->c == WAY)
		{
			//If destination is a linked node already - abort
			if (dest->parent != NULL) continue;

			//Otherwise, adopt node
			dest->parent = n;

			//Remove wall between nodes
			nodes[n->x + (x - n->x) / 2 + (n->y + (y - n->y) / 2) * width].c = WAY;

			//Return address of the child node
			return dest;
		}
	}

	//If nothing more can be done here - return parent's address
	return n->parent;
}

void maze_update_arr()
{
	uint16_t i, j, a, b, x, y;
	uint16_t c, d;

	switch(difficulty)
	{
		case EASY:
		{
			//flush the first height
			for (b = 0; b < MAZE_Y_SIZE; b++)
			    maze_arr[0][b] = NOWAY;	//use way for unused width

			for (i = 0, a = 1; i < height; i++, a = a+2)
			{
				maze_arr[a][0] = NOWAY;	//use way for unused width
				for (j = 0, b = 1; j < width; j++, b = b+2)
				{
				    maze_arr[a][b] = nodes[j + i * width].c;

				    if(maze_arr[a][b] == WAY)
				    {
				        maze_arr[a][b+1] = WAY;
				        maze_arr[a+1][b] = WAY;
				        maze_arr[a+1][b+1] = WAY;
				    }
				    else if(maze_arr[a][b] == WALL)
				    {
		    			maze_arr[a][b+1] = (j != width-1) ? nodes[j+1 + i * width].c : NOWAY;
		    			maze_arr[a+1][b] = (i != height-1) ? nodes[j + (i+1) * width].c : NOWAY;
		    			maze_arr[a+1][b+1] = ((j != width-1) && (i != height-1)) ? (nodes[j+1 +(i+1) * width].c || nodes[j+1 + i * width].c || nodes[j + (i+1) * width].c): NOWAY;
				    }
				}
				maze_arr[a][b] = NOWAY;	//use way for unused width
			}

			break;
		}
		case HARD:
		{
			for (i = 0; i < height; i++)
			{
				for (j = 0; j < width; j++)
				{
					maze_arr[i][j] = nodes[j + i * width].c;
				}
			}
			break;
		}
	}


	//Code to find Random start position
	do
	{
		x=random()%MAZE_X_SIZE;	//searching in width only
		//xil_printf("%d",x);
	} while(maze_arr[START_OFFSET+2][x]!=WAY); //Compare the row generated with random number and assign to player
	maze_arr[START_OFFSET+2][x]=PLAYER;

	//Code to find Random end position
	do
	{
		x=random()%MAZE_X_SIZE;	//searching in width only
		//xil_printf("%d",x);
	} while(maze_arr[STOP_OFFSET-3][x]!=WAY); //Compare the row generated with random number and assign to destination
	maze_arr[STOP_OFFSET-3][x]=DESTINATION;

	//For generating Life bars
	c= PLAYER_STARS_COUNT; // Copy player life count to local variable and compare it
	for(j=LIFE_BASE; (j<MAZE_X_SIZE && c>0); j+=LIFE_OFFSET) // Choose three columns and generate 3 stars in them
	{
		do
		{
			y=random()%MAZE_Y_SIZE;
		}while(maze_arr[y][j]!=WAY);

		if(c!=0)
		{
			maze_arr[y][j]= STAR;
			c--;
		}
	}

	////For generating Hole
	d= PLAYER_HOLES_COUNT; //Copy player stars count to local wariable
	while(d>0) // choose random point and check if there are two walls and two ways near to it and if it is a way place hole
	{
		do
		{
			x=random()%MAZE_X_SIZE;
			y=random()%MAZE_Y_SIZE;
		}while(maze_arr[y][x]!=WAY);

		if(d!=0)
		{
			if(((maze_arr[y][x+1]==WAY && maze_arr[y][x-1]==WALL &&\
				maze_arr[y-1][x]==WALL && maze_arr[y+1][x]==WAY) ||\
				(maze_arr[y][x+1]==WALL && maze_arr[y][x-1]==WAY &&\
				maze_arr[y-1][x]==WAY && maze_arr[y+1][x]==WALL) ||\
				(maze_arr[y][x+1]==WAY && maze_arr[y][x-1]==WALL &&\
				maze_arr[y-1][x]==WAY && maze_arr[y+1][x]==WALL) ||\
				(maze_arr[y][x+1]==WALL && maze_arr[y][x-1]==WAY &&\
				maze_arr[y-1][x]==WALL && maze_arr[y+1][x]==WAY)) ||\
				(maze_arr[y][x+1]==WAY && maze_arr[y][x-1]==WAY &&\
				maze_arr[y-1][x]==WAY && maze_arr[y+1][x]==WAY))
			{
				//xil_printf("hole: %d %d",y,x);
				maze_arr[y][x]=HOLE;
				d--;
			}
		}
	}
}

int maze_gen(uint16_t maze_width, uint16_t maze_height, uint16_t seed, uint16_t maze_difficulty)
{
	//srand( clock() );
	srand(seed);
	Node *start, *last;

	//update globals
	difficulty = maze_difficulty;

	//update maze w&h based on difficulty
	switch(maze_difficulty)
	{
		case EASY:
			width = (maze_width+1)/2 - 1;
			height = (maze_height+1)/2 - 1;
			break;
		case HARD:
			width = maze_width;
			height = maze_height;
			break;
	}

	//Do not allow negative dimensions
	if (width <= 0 || height <= 0)
	{
		//fprintf(stderr, "%s: dimensions must be greater than 0!\n", "S");
		return(1);
	}

	//Seed random generator
	srand(rand());

    //Initialize maze
	if (maze_init())
	{
		//fprintf(stderr, "%s: out of memory!\n", "S");
		return(1);
	}

	//Setup start node
	start = nodes + 1 + width;
	start->parent = start;
	last = start;

	//Connect nodes until start node is reached and can't be left
	while ((last = maze_link(last)) != start);

	maze_update_arr();

	return 0;
}
