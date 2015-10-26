#include <math.h>
#include <stdlib.h>

#include "types.h"
#include "defn.h"
#include "functions.h"


// initialise sun_list, place suns and initialise their values
// return value equal to input level if successful operation and -1 in case of failure
int initialise_suns(int level) {
	// first level will have 6 suns arranged in a hexagon with one sun controlled by player, one sun controlled by each of two computer opponents and 3 GRAY suns. vacant and occupied suns will be alternately placed
	int i;
	int origin_x = SCREEN_WIDTH/2, origin_y = SCREEN_HEIGHT/2; // these are the coordinates of the centre of the screen
	no_of_suns = 6;
	sun_list = (sun **)malloc(no_of_suns * sizoeof(sun *));
	float theta = RADIAN(360.0 / no_of_suns);
	/* CHANGE ALL THE SUN INITIALISATIONS AFTER YOU WRITE REMAINING FUNCTIONS */
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
	// add switch case for levels after finishing one level completely
}

/* cycles through all the suns and :
	1) generates new dots
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


// vector system used in XY plane
void move_dot(dot *d) {
	int sq_dist;
	if(d->reached_sun) { // dot has already reached target sun, we should check for ITCHING_TO_FIGHT
		return;
	}
	//move towards target
	sq_dist = sqrt(distance2(x, y, target_sun->x, target_sun->y));
	dot->x += ((target_sun->x - x)/ sq_dist) * DOT_SPEED;
	dot->y += ((target_sun->y - y)/ sq_dist) * DOT_SPEED;
	if(distance2(x, y, target_sun->x, target_sun->y) <= SUN_REASONABLY_CLOSE) {
		dot_reached_sun(d);
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
	dot *p, *temp;
	while(dots_per_cycle) { // gray suns have curr_level zero, so loop won't execute
		temp = (dot *)malloc(sizeof(dot));
		temp->color = s->color;
		temp->active = 1;
		temp->reached_sun = 1; // already near the parent sun
		temp->x = s->x;
		temp->y = s->y;
		temp->target_sun = s;
		temp->target_dot = NULL;
		temp->move = move_to_annulus;
		temp->next = dot_ptr_array[s->color];
		dot_ptr_array[s->color] = temp;
		dots_per_cycle--;
	}
}

void move_to_annulus(dot *d, sun *s) {
	int radius = (int)random() % (int)(ANNULUS_OUTER_RADIUS - ANNULUS_INNER_RADIUS);
	float theta = RADIAN((int)random() % 360); // theta will be in radians
	d->x_target = s->x + radius * cos(theta);
	d->y_target = s->y + radius * sin(theta);
	d->target_sun = NULL;
}



/* COMPLETED FUNCTIONS */

void remove_inactive_dots(dot **dot_ptr_array) {
	int i;
	dot *p, *q;
	for(i = 0; i < no_of_colors; i++) {
		p = dot_ptr_array[i];
		// if all the first 'n' dots are inactive, then dot_ptr_arrry must also by shifted
		while(p && !p->active) {
			dot_ptr_array[i] = p->next;
			free(p);
			p = dot_ptr_array[i];
		}
		if(!p) {
			continue;
		}
		q = p;
		p = p->next;
		while(p) {
			if(!p->active) {
				q->next = p->next;
				free(p);
				p = q->next;
			}
			else {
				q = p;
				p = p->next;
			}
		}
	}
}

/* set all flock counts to zero and increment them */
void update_flock_count() {
	int i;
	dot *d;
	sun *s;
	// set all flock counts to zero
	for(i = 0; i < no_of_colors; i++) {
		s =  sun_ptr_array[i];
		while(s) {
			s->flock_count = 0;
			s = s->next;
		}
	}
	// update the flock counts
	for(i = 0; i < no_of_colors, i++) {
		d = dot_ptr_array[i];
		while(d) {
			if(d->color == d->target_sun->color || d->color + 1 == d->target_sun->color)
				d->target_sun->flock_count++;
			else
				d->target_sun->flock_count--;
			d = d->next;
		}	
	}
}

// distance squared
int distance2(int x1, int y1, int x2, int y2) {
	return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1); // no need to take square root
}

// accepts coordinates of a sun and returns a pointer to the weakest nearby sun
sun *weakest_nearby_sun(int x, int y, int friendly_color) {
	// traverse all suns in the sun_list and return pointer of the weakest non-friendly sun
	sun *weakest = NULL, *curr_sun;
	int i;
	for(i = 0; i < no_of_suns; i++) {
		curr_sun = sun_list[i];
		if(curr_sun->color != friendly_color) {
			if(!weakest) {
				weakest = curr_sun;
			}
			else if(curr_sun->flock_count * distance2(x, y, curr_sun->x, curr_sun->y) < weakest->flock_count * distance2(x, y, weakest->x, weakest->y)) {
				/* divide flock count by some number if value of product goes out of integer range */
				weakest = curr_sun;
			}
		}
	}
	return weakest;
}

sun *nearest_empty_sun(int x, int y) {
	sun *nearest = NULL, *curr_sun;
	int i;
	for(i = 0; i < no_of_suns; i++) {
		curr_sun = sun_list[i];
		if(curr_sun->color == GRAY) {
			if(!nearest) {
				nearest = curr_sun;
			}
			else if(distance2(x, y, curr_sun->x, curr_sun->y) < distance2(x, y, nearest->x, nearest->y)) {
				nearest = curr_sun;
			}
		}
	}
	return nearest;
}

// if dot is used, make it disappear before function return
void dot_reached_sun(dot *d) {
	int radius;
	float theta;
	if(d->target_sun.color == d->color) { // sun is a friendly
		if(d->target_sun.health < HEALTH_MAX) { // restore health
			(d->target_sun.health)++;
			free(d);
			return;
		}
		if(d->target_sun.curr_level < d->target_sun.max_level) { // upgrade
			(d->target_sun.upgrade)++;
			if(d->target_sun.upgrade >= UPGRADE_MAX) {
				d->target_sun.upgrade = 0;
				(d->target_sun.curr_level)++;
				// change the image of the sun here, add a new ring
			}
			free(d);
			return;
		}
		d->reached_sun = 1;
		move_to_annulus(d, d->target_sun); // else move to annulus
		return;
	}
	else if(d->target_sun.color == d->color + 1) { // + 1 means it is in GRAY_mycolor state, about-to-be-friendly sun
		(d->target_sun.health)++; // restore health
		if(d->target_sun.health >= HEALTH_MAX) {
			d->target_sun.upgrade = 0;
			d->target_sun.curr_level = 1;
			d->target_sun.health = HEALTH_MAX;
			d->target_sun.color = d->color;
			// change the image of the sun here, to color of the dot
		}
		free(d);
		return;
	}
	else if(d->target_sun.color == GRAY) { // neutral sun
		(d->target_sun.health)++;
		d->target_sun.color = dot color + 1;
		free(d);
		return;
	}
	else { // enemy sun or gray-enemy sun
		(d->target_sun.health)--;
		if(d->target_sun.health <= 0) {
			d->target_sun.health = 0;
			d->target_sun.curr_level = 0;
			d->target_sun.color = GRAY;
			d->target_sun.upgrade = 0;
		}
		free(d);
		return
	}
}

void dots_init() {
	dot_ptr_array = (dot **)calloc(no_of_colors, sizeof(dot *)); // will set all pointers to NULL
}