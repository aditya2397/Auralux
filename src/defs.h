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

#ifndef DEFS
#define DEFS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <time.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_gfxPrimitives.h"

enum sun_colors {GRAY, BLUE, GRAY_BLUE, ORANGE, GRAY_ORANGE, GREEN, GRAY_GREEN, PURPLE, GRAY_PURPLE, NO_OF_COLORS};
enum CPU_heuristics {TIMID, DEVELOPING, AGGRESSIVE};
enum level_names {CLASH, COCYTUS, HEX, RING, ROME, SPLIT, TRIAD, NO_OF_LEVELS};

/*
 * maintain order of the enum colors as color followed by corresponding gray_color
 * hence if (color % 2) is 1, then it means color sun, and is 0 means gray type sun
 * GRAY_color is when the sun is currently GRAY / defeated by another color and is in the process of being converted to that color
 */
#define SCREEN_WIDTH /*640*/1000
#define SCREEN_HEIGHT /*480*/700

#define DOT_SPEED 5
#define SELECT_RING_SPEED 20

#define UPGRADE_MAX 100
#define UPGRADE_STEP 1
#define HEALTH_MAX 100
#define HEALTH_STEP 1
#define SUN_REASONABLY_CLOSE 3.0
#define DOT_REASONABLY_CLOSE 1.0
#define RING_REASONABLY_CLOSE 20

#define ANNULUS_INNER_RADIUS 55
#define ANNULUS_OUTER_RADIUS 60
/*
#define ANNULUS_INNER_RADIUS 65
#define ANNULUS_OUTER_RADIUS 70
*/
#define PI 3.1415
#define RADIAN(x) (PI * x / 180.0)

#define GRAY_SUN_RADIUS (30 / 2)
#define SUN_LEVEL1_RADIUS (60 / 2)
#define SUN_LEVEL2_RADIUS (80 / 2)
#define SUN_LEVEL3_RADIUS (100 /2)
#define STATUS_BAR_Y_OFFSET 50
#define BAR_SMALL_W 1
#define BAR_SMALL_H 4
#define HOLLOW_BAR_WIDTH 50
#define DOT_RADIUS 2
#define MAX_SUN_POSITION_ERROR 5


#define SUN_FLOCK_RADIUS (50 * 50)
#define SAFE_FLOCK_COUNT 50
#define OVERBEARING_FLOCK_COUNT 110
#define PER_ENEMY_FLOCK_COUNT 40

#define UNIT_DOT_PRODUCTION_RATE 10 /* this is the number of dots produced per second by a level 1 sun */

#define MIN_TICK_DELAY 64 /* currently at 15 fps */
#define MAX_CLICK_DIST2 (10 * 10)
#define GRID_ROW_WIDTH 10/*(SCREEN_HEIGHT / 60)*/
#define GRID_COL_WIDTH 10/*(SCREEN_WIDTH / 75)*/

#endif
