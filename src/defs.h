#ifndef DEFS
#define DEFS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <time.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"

#define SCREEN_WIDTH /*640*/1000
#define SCREEN_HEIGHT /*480*/700

#define DOT_SPEED 5
#define SELECT_RING_SPEED (4 * DOT_SPEED)

enum
{
	BLUE_DOT,
	ORANGE_DOT,
	PURPLE_DOT,
	GREEN_DOT,

	BLUE_SUN_1,
	BLUE_SUN_2,
	BLUE_SUN_3,
	ORANGE_SUN_1,
	ORANGE_SUN_2,
	ORANGE_SUN_3,
	PURPLE_SUN_1,
	PURPLE_SUN_2,
	PURPLE_SUN_3,
	GREEN_SUN_1,
	GREEN_SUN_2,
	GREEN_SUN_3,
	GRAY_SUN,
	
	RING_1,
	RING_2,
	SELECT_RING,

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

enum sun_colors {GRAY, BLUE, GRAY_BLUE, ORANGE, GRAY_ORANGE, GREEN, GRAY_GREEN, PURPLE, GRAY_PURPLE, NO_OF_COLORS};

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
#define UPGRADE_STEP 2
#define HEALTH_MAX 100
#define HEALTH_STEP 2
#define SUN_REASONABLY_CLOSE 3.0
#define DOT_REASONABLY_CLOSE 1.0
#define RING_REASONABLY_CLOSE (3 * SUN_REASONABLY_CLOSE)

#define ANNULUS_INNER_RADIUS 60
#define ANNULUS_OUTER_RADIUS 70
#define PI 3.1415
#define RADIAN(x) (PI * x / 180.0)
#define LEVEL1_HEXAGON_RADIUS 200.0
#define ITCHING_TO_FIGHT_RADIUS 5.0
#define ITCHING_TO_FIGHT_SPEED (DOT_SPEED * 2)
/* when a dot is already at target location i.e. target_x = target_y = -1, it will check whether there is an enemy dot in the nearby ITCHING_TO_FIGHT_RADIUS and move towards that dot with ITCHING_TO_FIGHT_SPEED (> DOT_SPEED) */
#define SUN_FLOCK_RADIUS (50 * 50)
#define SAFE_FLOCK_COUNT 20
#define OVERBEARING_FLOCK_COUNT 30
#define PER_ENEMY_FLOCK_COUNT 40

#define SUN_IMG_L 50
#define SUN_IMG_W SUN_IMG_L
#define DOT_IMG_L 6
#define DOT_IMG_W DOT_IMG_L

#define MIN_TICK_DELAY 64 /* currently at 15 fps */
#define MAX_CLICK_DIST2 (10 * 10)
#define GRID_ROW_WIDTH 10/*(SCREEN_HEIGHT / 60)*/
#define GRID_COL_WIDTH 10/*(SCREEN_WIDTH / 75)*/

/*
   target priority:
   1) sun
   2) nearby dot

   corresponding checks:
   1) if target_dot != NULL, pursue target_dot
   3) else you are free to find nearby enemies
*/

#endif
