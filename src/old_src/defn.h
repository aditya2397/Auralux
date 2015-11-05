#include <math.h>

enum sun_colors {GRAY, BLUE, GRAY_BLUE, ORANGE, GRAY_ORANGE, PURPLE, GRAY_PURPLE, GREEN, GRAY_GREEN, NO_OF_COLORS};

// maintain order of the enum colors as color followed by corresponding gray_color
// hence if (color % 2) is 1, then it means color sun, and is 0 means gray type sun
// GRAY_color is when the sun is currently GRAY / defeated by another color and is in the process of being converted to that color

// to convert a neutral sun into a color sun, increment health till HEALTH_MAX
// curr_level must be set back to zero once a sun is defeated, hence every gray and gray_color sun has curr_level = 0

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define UPGRADE_MAX 100
#define HEALTH_MAX 100
#define SUN_REASONABLY_CLOSE 10.0
#define DOT_REASONABLY_CLOSE 2.0

#define DOT_SPEED 10.0
#define ANNULUS_INNER_RADIUS 30.0
#define PI 3.1415
#define RADIAN(x) (PI * x / 180.0)
#define ANNULUS_OUTER_RADIUS 45.0
#define LEVEL1_HEXAGON_RADIUS 200.0
#define ITCHING_TO_FIGHT_RADIUS 5.0
#define ITCHING_TO_FIGHT_SPEED (DOT_SPEED * 1.5)
// when a dot is already at target location i.e. target_x = target_y = -1, it will check whether there is an enemy dot in the nearby ITCHING_TO_FIGHT_RADIUS and move towards that dot with ITCHING_TO_FIGHT_SPEED (> DOT_SPEED)
#define SUN_FLOCK_RADIUS (50 * 50)

/*
   target priority:
   1) sun
   2) nearby dot

   corresponding checks:
   1) if target_dot != NULL, pursue target_dot
   3) else you are free to find nearby enemies
*/

int no_of_suns; // set after level is selected and before suns are malloced and initialised  

sun **sun_ptr_array; // pointer to pointer to sun
// malloc to an array of sun pointers of size no_of_suns

dot **dot_ptr_array; // pointer to pointer to dot
// a little memory is wasted, but it ensures simplicity	
