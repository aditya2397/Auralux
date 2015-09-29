#include <math.h>

enum colors {GRAY, BLUE, GRAY_BLUE, ORANGE, GRAY_ORANGE, PURPLE, GRAY_PURPLE, GREEN, GRAY_GREEN};

#define SCREEN_WIDTH 1200;
#define SCREEN_HEIGHT 800;
#define UPGRADE_MAX 100;
#define HEALTH_MAX 100;
#define SUN_REASONABLY_CLOSE (10.0);
#define DOT_REASONABLY_CLOSE (2.0);

#define DOT_SPEED (10.0);
#define ANNULUS_INNER_RADIUS (30.0);
#define ANNULUS_OUTER_RADIUS (45.0);
#define RADIAN(x) (3.1415 * x / 180.0);
#define LEVEL1_HEXAGON_RADIUS (200.0);
#define ITCHING_TO_FIGHT_RADIUS (5.0);
#define ITCHING_TO_FIGHT_SPEED (DOT_SPEED * 1.5)

typedef struct sun {
	int color;
	int health, upgrade;
	float x, y;
	int curr_level, max_level; // curr_level decides size of sun's picture and max_level - curr_level decides number of rings around the sun's picture
	int flock_count;
	void (*generate_dots)(); // write this function and fill in parameters later
}sun;

typedef struct dot {
	int color;
	float x, y;
	float target_x, target_y; // used when target is empty space or target is sun
	// cannot be used when target is dot, because dot may not be stationary
	struct sun *target_sun; // when ptr is NULL, then target is an empty space
	struct dot *target_dot; // when ptr is NULL, then is no target
	// float curr_speed; // for smooth motion of dot, slowly speeding up and finally reaching a steady speed
	void (*move)(struct dot *); // this points to move_dot()
}dot;

float distance(float x1, float y1, float x2, float y2);

// accepts coordinates of a sun and returns a pointer to the weakest nearby sun
sun *weakest_nearby_sun(float x, float y, int friendly_color);

sun *nearest_empty_sun(int x, int y);

// initialise sun_list, place suns and initialise their values
// return value equal to input level if successful operation and -1 in case of failure
int initialise_suns(int level);

// cycles through all the suns, generates new dots and updates flock_count
void update_suns();

void cpu_action(int color);

// if dot is used, make it disappear before function return
void dot_reached_sun(dot *d);

void move_dot(dot *d);

void random_init();

void generate_dots(sun *s);

void move_to_annulus(dot *d, sun *s);

int update_flock_count(sun *s);
