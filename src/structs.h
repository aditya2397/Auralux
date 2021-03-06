/*  Auralux game
    Copyright (C) 2015 Aditya Barve
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STRUCTS
#define STRUCTS

#include "defs.h"

typedef struct Game {
	SDL_Surface *screen;
} Game;

typedef struct Control
{
	int mouse_down_x, mouse_down_y, mouse_up_x, mouse_up_y, mouse_down;
} Control;

typedef struct sun {
	int color;
	int health, upgrade;
	int x, y;
	int curr_level, max_level;
	int flock_count;
	struct sun *next;
	int graph_index;
	int nearby_enemies;
	int min_enemy_dist;
} sun;

typedef struct dot {
	int color;
	int active;
	int x, y;
	int target_x, target_y;
	sun *target_sun;
	int reached_sun;
	struct dot *target_dot;
	void (*move_dot)(struct dot *);
	struct dot *next;
} dot;

struct player_select_ring {
	int x, y;
	int active;
	sun *target_sun;
} select_ring; /* select ring is a variable of this struct type */

int no_of_suns;
int active_sun_colors[NO_OF_COLORS / 2];
int heuristic[NO_OF_COLORS / 2];
unsigned int sun_rgb[NO_OF_COLORS / 2 + 1];
unsigned int dot_rgb[NO_OF_COLORS / 2 + 1];
unsigned int no_of_dots[NO_OF_COLORS / 2];
unsigned int sun_radii[4];
int **edge_matrix;
char **levels;

sun *sun_list; /* pointer to first sun in the list */

dot *dot_ptr_grid[SCREEN_HEIGHT / GRID_ROW_WIDTH][SCREEN_WIDTH / GRID_COL_WIDTH][NO_OF_COLORS / 2];
/* 2d grid, each a[x][y] containing an array of dot pointers of size = number of possible dot colors */
/* if you try to allocate it dynamically, it will be a dot ****. so allocate it statically */
/* a little memory is wasted, but it ensures simplicity	*/

#endif
