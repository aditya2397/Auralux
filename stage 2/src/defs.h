#ifndef DEFS
#define DEFS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define DOT_SPEED 5

enum
{
	BLUE_DOT,
	ORANGE_DOT,
	PURPLE_DOT,
	GREEN_DOT,

	BLUE_SUN,
	ORANGE_SUN,
	PURPLE_SUN,
	GREEN_SUN,
	GRAY_SUN,

	HEALTH_BAR_HOLLOW,
	HEALTH_BAR_SMALL,
	UPGRADE_BAR_HOLLOW,
	BLUE_BAR_SMALL,
	ORANGE_BAR_SMALL,
	GREEN_BAR_SMALL,
	PURPLE_BAR_SMALL,
	MAX_SPRITES
};

enum
{
	BULLET_SOUND,
 	MAX_SOUNDS
};

#include <math.h>

enum sun_colors {GRAY, BLUE, GRAY_BLUE, ORANGE, GRAY_ORANGE, PURPLE, GRAY_PURPLE, GREEN, GRAY_GREEN, NO_OF_COLORS};

/*
// maintain order of the enum colors as color followed by corresponding gray_color
// hence if (color % 2) is 1, then it means color sun, and is 0 means gray type sun
// GRAY_color is when the sun is currently GRAY / defeated by another color and is in the process of being converted to that color

// to convert a neutral sun into a color sun, increment health till HEALTH_MAX
// curr_level must be set back to zero once a sun is defeated, hence every gray and gray_color sun has curr_level = 0
*/
/*
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
*/
#define UPGRADE_MAX 100
#define HEALTH_MAX 100
#define SUN_REASONABLY_CLOSE 3.0
#define DOT_REASONABLY_CLOSE 1.0

#define ANNULUS_INNER_RADIUS 50
#define ANNULUS_OUTER_RADIUS 70
#define PI 3.1415
#define RADIAN(x) (PI * x / 180.0)
#define LEVEL1_HEXAGON_RADIUS 200.0
#define ITCHING_TO_FIGHT_RADIUS 5.0
#define ITCHING_TO_FIGHT_SPEED (DOT_SPEED * 2)
/* when a dot is already at target location i.e. target_x = target_y = -1, it will check whether there is an enemy dot in the nearby ITCHING_TO_FIGHT_RADIUS and move towards that dot with ITCHING_TO_FIGHT_SPEED (> DOT_SPEED) */
#define SUN_FLOCK_RADIUS (50 * 50)

#define SUN_IMG_L 50
#define SUN_IMG_W SUN_IMG_L
#define DOT_IMG_L 6
#define DOT_IMG_W DOT_IMG_L

#define MIN_TICK_DELAY 64 /* currently at 15 fps */
#define MAX_CLICK_DIST2 (10 * 10)
#define GRID_ROW_WIDTH 4/*(SCREEN_HEIGHT / 60)*/
#define GRID_COL_WIDTH 4/*(SCREEN_WIDTH / 75)*/

/*
   target priority:
   1) sun
   2) nearby dot

   corresponding checks:
   1) if target_dot != NULL, pursue target_dot
   3) else you are free to find nearby enemies
*/

#endif
