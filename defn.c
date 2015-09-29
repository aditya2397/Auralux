#include <math.h>
#include <stdio.h>
#include "defn.h"
#include <stdlib.h>

sun **sun_list; // pointer to pointer to sun

int no_of_suns; // set after level is selected and before suns are malloced and initialised  

/* How are you going to store the dots?
   options of storing the dots:
   1) individual linked lists for each color
   2) commmon hashtable
   3) individual hashtable for each color

   hash function will segregate members based on their position in XY plane
   it will split playing field into rows and columns and create hash bucket for each of the rectangular subregions of the playing arena

   the CPUs will always send their dots to other suns
   can we use this fact to simplify storage of CPU dots?
 */

float distance(float x1, float y1, float x2, float y2) {
	return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1); // no need to take square root
}

// accepts coordinates of a sun and returns a pointer to the weakest nearby sun
sun *weakest_nearby_sun(float x, float y, int friendly_color) {
	// traverse all suns in the sun_list and return pointer of the weakest non-friendly sun
	sun *weakest = NULL, *curr_sun;
	int i;
	for(i = 0; i < no_of_suns; i++) {
		curr_sun = sun_list[i];
		if(curr_sun->color != friendly_color) {
			if(!weakest) {
				weakest = curr_sun;
			}
			else if(curr_sun->flock_count/* divide flock count by some number if value of product goes out of integer range */ * distance(x, y, curr_sun->x, curr_sun->y) < weakest->flock_count * distance(x, y, weakest->x, weakest->y)) {
				weakest = curr_sun;
			}
		}
	}
	return curr_sun;
}

sun *nearest_empty_sun(int x, int y) {
	sun *nearest = NULL, *curr_sun;
	int i;
	for(i = 0; i < no_of_suns; i++) {
		curr_sun = sun_list[i];
		if(curr_sun->color == GRAY) {
			if(nearest && (distance(x, y, curr_sun->x, curr_sun->y) < distance(x, y, nearest->x, nearest->y))) {
				nearest = curr_sun;
			}
			else {
				nearest = curr_sun;
			}
		}
	}
	return nearest;
}

// initialise sun_list, place suns and initialise their values
// return value equal to input level if successful operation and -1 in case of failure
int initialise_suns(int level) {
	// first level will have 6 suns arranged in a hexagon with one sun controlled by player, one sun controlled by each of two computer opponents and 3 GRAY suns. vacant and occupied suns will be alternately placed
	int i;
	float origin_x = SCREEN_WIDTH/2, origin_y = SCREEN_HEIGHT/2; // these are the coordinates of the centre of the screen
	sun_list = (sun **)malloc(sizeof(sun *));
	no_of_suns = 6;	
	float theta = RADIAN(360.0 / no_of_suns);
	for(i = 0; i < no_of_suns; i++) {
		sun_list[i] = (sun *)malloc(sizeof(sun));
		sun_list[i]->upgrade = 0;
		sun_list[i]->max_level = 2;
		sun_list[i]->flock_count = 0;
		sun_list[i]->generate_dots = &//here, write name of function used to generate dots1
			sun_list[i]->x = SCREEN_WIDTH / 2 + LEVEL1_HEXAGON_RADIUS * cos(i * theta);
		sun_list[i]->y = SCREEN_HEIGHT / 2 + LEVEL1_HEXAGON_RADIUS * sin(i * theta);
		if(i % 2) { // if i is odd, consider color suns
			sun_list[i]->curr_level = 1;
			sun_list[i]->health = 100;
		}
		else { // consider gray suns
			sun_list[i]->curr_level = 0;
			sun_list[i]->health = 0;
			sun_list[i]->color = GRAY;
		}
	}
	sun_list[1]->color = BLUE;
	sun_list[3]->color = ORANGE;
	sun_list[5]->color = GREEN;
	return level;
	// add switch for level after finishing one level completely
}

// cycles through all the suns, generates new dots and updates flock_count
void update_suns() {
	// check first sun's level
	// generate (malloc) and initialise that many dots. set target x and y to make the dots symmetrically disperse from the centre of the sun to the annular disc around the sun	
	// count number of friendly dots in a small radius around the sun update flock_count
}

// pass color of CPU
// function will return an integer corresponding to one of the following actions
/*
   1 - target pointer points to his sun to be healed - urgent
   2 - target pointer points to a GRAY sun to occupy - only if reasonably close to its own suns
   3 - target pointer points to his sun to upgrade - not urgent
   4 - target pointer points to enemy's sun to attack - only if reasonably close to its own suns
*/
cpu_action(int color) {
}

// if dot is used, make it disappear before function return
void dot_reached_sun(dot *d) {
	float radius, theta;
	//sun is a friendly
	if(d->target_sun->color == d->color) {
		//restore health
		if(d->target_sun->health < HEALTH_MAX) {
			(d->target_sun->health)++;
			free(d);
			return
		}
		//upgrade
		if(d->target_sun.curr_level < d->target_sun.max_level) {
			(d->target_sun->upgrade)++;
			if(d->target_sun->upgrade >= UPGRADE_MAX) {
				d->target_sun->upgrade = 0;
				(d->target_sun->curr_level)++;
				// change the image of the sun here, add a new ring
			}
			free(d);
			return
		}
		move_to_annulus(d, d->target_sun);
		return;
	}
	// about-to-be-friendly sun
	else if(d->target_sun.color == d->color + 1) { // + 1 means it is in GRAY_mycolor state
		// restore health
		(d->target_sun.health)++;
		if(d->target_sun.health >= HEALTH_MAX) {
			d->target_sun->upgrade = 0;
			d->target_sun->curr_level = 1;
			d->target_sun->health = HEALTH_MAX;
			d->target_sun->color = d->color;
			// change the image of the sun here, add a new ring
		}
		free(d);
		return
	}
	// neutral sun
	else if(d->target_sun->color == GRAY) {
		(d->target_sun->health)++; d->target_sun->color = dot color + 1;
		free(d);
		return
	}
	// enemy sun or gray-enemy sun
	else {
		(d->target_sun->health)--;
		if(d->target_sun->health == 0) {
			d->target_sun->curr_level = 0;
			d->target_sun->color = GRAY;
			d->target_sun->upgrade = 0;
		}
		free(d);
		return
	}
}

void move_dot(dot *d) {
	float sq_dist;
	if(d->target_x < 0 || d->target_y < 0) {
		return;
	}
	//move towards target
	sq_dist = sqrt(distance(x, y, target_x, target_y));
	dot->x += ((target_x - x)/ sq_diff) * DOT_SPEED;
	dot->y += ((target_y - y)/ sq_diff) * DOT_SPEED;
	if(distance(x, y, target_x, target_y) <= SUN_REASONABLY_CLOSE) {
		target_x = target_y = -1;
		if(d->target_sun) {
			dot_reached_sun(d);
		}
	}
	return;
}

void random_init() {
	// for creating uniform distribution of the dots in the annulus
	time_t tt;
	srandom(time(&tt));
	return;
}

void generate_dots(sun *s) {
	int dots_per_cycle = s->curr_level;
	while(dots_per_cycle) {
		// how do you store a dot?
		dots_per_cycle--;
	}
}

void move_to_annulus(dot *d, sun *s) {
	float radius = random() % (int)(ANNULUS_OUTER_RADIUS - ANNULUS_INNER_RADIUS);
	float theta = RADIAN(random() % 360); // theta will be in radians
	d->x_target = s->x + radius * cos(theta);
	d->y_target = s->y + radius * sin(theta);
	//d->x_target = x_target + radius * cos(theta);
	//d->y_target = y_target + radius * sin(theta);
	d->target_sun = NULL; // to specify that target is empty space and not a sun
}

int update_flock_count(sun *s) {
	//main challenge arises once again, how are you going to store the dots?
	return 0;
}
