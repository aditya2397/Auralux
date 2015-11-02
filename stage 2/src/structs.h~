#ifndef STRUCTS
#define STRUCTS

#include "defs.h"

typedef struct Game
{
	SDL_Surface *screen;
} Game;

typedef struct Sprite
{
	SDL_Surface *image;
} Sprite;

typedef struct Sound
{
	Mix_Chunk *effect;
} Sound;

typedef struct Entity
{
	int active;
	int x, y, thinkTime;
	SDL_Surface *sprite;
	void (*action)(void);
	void (*draw)(void);
} Entity;

typedef struct Control
{
	int up, down, left, right, fire, mouse_down_x, mouse_down_y, mouse_up_x, mouse_up_y, mouse_down;
} Control;

typedef struct sun {
	int color;
	int health, upgrade;
	int x, y;
	int curr_level, max_level; /* curr_level decides size of sun's picture and max_level - curr_level decides number of rings around the sun's picture */
	int flock_count;
	void (*generate_dots)(); /* write this function and fill in parameters later */
	struct sun *next;
	SDL_Surface *sprite;
	SDL_Surface *bar_small;
}sun;

typedef struct dot {
	int color;
	int active;
	int x, y;
	int target_x, target_y;
	sun *target_sun; /* can never be NULL */
	int reached_sun; /* when 1, means sun reached, check if target is dot */
	struct dot *target_dot; /* when NULL, then is no target */
	/* float curr_speed; for smooth motion of dot, slowly speeding up and finally reaching a steady speed */
	void (*move_dot)(struct dot *); /* this points to move_dot() */
	struct dot *next;
	SDL_Surface *sprite;
}dot;

int no_of_suns; /* set after level is selected and before suns are malloced and initialised  */

sun *sun_list; 
/* pointer to first sun in the list */
/* malloc to an array of sun pointers of size no_of_suns */

dot *dot_ptr_grid[SCREEN_HEIGHT / GRID_ROW_WIDTH][SCREEN_WIDTH / GRID_COL_WIDTH][NO_OF_COLORS / 2];
/* 2d grid, each a[x][y] containing an array of dot pointers of size = number of possible dot colors */
/* if you try to allocate it dynamically, it will be a dot ****. so allocate it statically */
/* a little memory is wasted, but it ensures simplicity	*/

int active_sun_colors[NO_OF_COLORS / 2];

#endif
