#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"
#include "defs.h"
#include "functions.h"

extern SDL_Surface *getSprite(int);

/* vector system used in XY plane */
void move_dot(dot *d) {
	int sq_dist;
	int shift_x, shift_y;
	/*
	printf("move dot activated\n");
	*/
	/* check whether target is a dot */
	if(d->target_dot) {
		d->target_x = d->target_dot->x;
		d->target_y = d->target_dot->y;
	}
	sq_dist = sqrt(distance2(d->x, d->y, d->target_x, d->target_y));
	if(sq_dist <= DOT_REASONABLY_CLOSE) { /* dot has already reached target sun, we should check for ITCHING_TO_FIGHT */
		if(d->target_dot) { /* dots have fought, make both inactive */
			d->active = 0;
			d->target_dot->active = 0;
			printf("dots have fought\n");
		}
		/* printf("dot at target\n"); */
		return;
	}
	/* move towards target */
	shift_x = (d->target_x - d->x) * DOT_SPEED / sq_dist / 2;
	shift_y = (d->target_y - d->y) * DOT_SPEED / sq_dist / 2;
	if(d->target_dot) {
		shift_x /= 2;
		shift_y /= 2;
	}
	d->x += shift_x;
	d->y += shift_y;
	if(!d->reached_sun && distance2(d->x, d->y, d->target_x, d->target_y) <= SUN_REASONABLY_CLOSE) {
		printf("dot reached sun after going through movedot function\n");
		dot_reached_sun(d);
	}
	return;
}

int my_random() {
	int x;
	srand(clock());
	x = rand();
	return x;
}

void generate_dots(sun *s) {
	int dots_per_cycle = s->curr_level;
	int i = 0;
	int radius = ANNULUS_INNER_RADIUS + my_random() % (int)(ANNULUS_OUTER_RADIUS - ANNULUS_INNER_RADIUS);
	int angle = my_random() % 360;
	float theta = RADIAN(angle);
	dot *temp;
	while(dots_per_cycle) { /* gray suns have curr_level zero, so loop won't execute */
		temp = (dot *)malloc(sizeof(dot));
		temp->color = s->color;
		temp->active = 1;
		temp->reached_sun = 1; /* already near the parent sun */
		temp->x = s->x;
		temp->y = s->y;
		temp->target_sun = s;
		temp->target_dot = NULL;
		temp->move_dot = move_dot;
		temp->target_x = s->x + radius * cos(theta + 2 * PI * i / s->curr_level);
		temp->target_y = s->y + radius * sin(theta + 2 * PI * i / s->curr_level);
		i++;
		switch(s->color) {
			case BLUE:
				temp->sprite = getSprite(BLUE_DOT);
				break;
			case ORANGE:
				temp->sprite = getSprite(ORANGE_DOT);
				break;
			case GREEN:
				temp->sprite = getSprite(GREEN_DOT);
				break;
			case PURPLE:
				temp->sprite = getSprite(PURPLE_DOT);
				break;
		}
		temp->next = dot_ptr_grid[s->y / GRID_ROW_WIDTH][s->x / GRID_COL_WIDTH][s->color / 2];
		dot_ptr_grid[s->y / GRID_ROW_WIDTH][s->x / GRID_COL_WIDTH][s->color / 2] = temp;
		dots_per_cycle--;
	}
}

void move_to_annulus(dot *d) {
	sun *s = d->target_sun;
	int radius = ANNULUS_INNER_RADIUS + my_random() % (int)(ANNULUS_OUTER_RADIUS - ANNULUS_INNER_RADIUS);
	float theta = RADIAN(my_random()); /* theta will be in radians */
	/* printf("move to annulus activated\n"); */
	/*
	printf("%f\n", theta);
	printf("radius : %d\n", radius);
	*/
	d->target_x = s->x + radius * cos(theta);
	d->target_y = s->y + radius * sin(theta);
	d->move_dot = move_dot;
}

/* COMPLETED FUNCTIONS */

/* distance squared */
int distance2(int x1, int y1, int x2, int y2) {
	return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1); /* no need to take square root */
}

/* if dot is used, make it disappear before function return */
void dot_reached_sun(dot *d) {
	if(d->target_sun->color == d->color) { /* sun is a friendly */
		if(d->target_sun->health < HEALTH_MAX) { /* restore health */
			(d->target_sun->health)++;
			printf("%d health : %d\n", d->target_sun->color, d->target_sun->health);			
			d->active = 0;
			return;
		}
		if(d->target_sun->curr_level < d->target_sun->max_level) { /* upgrade */
			(d->target_sun->upgrade)++;
			if(d->target_sun->upgrade >= UPGRADE_MAX) {
				d->target_sun->upgrade = 0;
				printf("%d upgrade : %d\n", d->target_sun->color, d->target_sun->upgrade);			
				(d->target_sun->curr_level)++;	
				/* change the image of the sun here, add a new ring */
			}
			d->active = 0;
			return;
		}
		d->reached_sun = 1;
		move_to_annulus(d); /* else move to annulus */
		return;
	}
	else if(d->target_sun->color == d->color + 1) { /* + 1 means it is in GRAY_mycolor state, about-to-be-friendly sun */
		(d->target_sun->health)++; /* restore health */
		printf("%d health : %d\n", d->target_sun->color, d->target_sun->health);			
		if(d->target_sun->health >= HEALTH_MAX) {
			d->target_sun->upgrade = 0;
			d->target_sun->curr_level = 1;
			d->target_sun->health = HEALTH_MAX;
			d->target_sun->color = d->color;
			switch(d->color) {
				case BLUE:
					d->target_sun->sprite = getSprite(BLUE_SUN);
					break;
				case ORANGE:
					d->target_sun->sprite = getSprite(ORANGE_SUN);
					break;
				case GREEN:
					d->target_sun->sprite = getSprite(GREEN_SUN);
					break;
				case PURPLE:
					d->target_sun->sprite = getSprite(PURPLE_SUN);
					break;
			}
			/* change the image of the sun here, to color of the dot */
		}
		d->active = 0;
		return;
	}
	else if(d->target_sun->color == GRAY) { /* neutral sun */
		(d->target_sun->health)++;
		d->target_sun->color = d->color + 1;
		switch(d->color) {
			case BLUE :
				d->target_sun->bar_small = getSprite(BLUE_BAR_SMALL);
				break;
			case ORANGE :
				d->target_sun->bar_small = getSprite(ORANGE_BAR_SMALL);
				break;
			case PURPLE :
				d->target_sun->bar_small = getSprite(PURPLE_BAR_SMALL);
				break;
			case GREEN :
				d->target_sun->bar_small = getSprite(GREEN_BAR_SMALL);
				break;				
		}		
		d->active = 0;
		return;
	}
	else { /* enemy sun or gray-enemy sun */
		(d->target_sun->health)--;
		printf("%d health : %d\n", d->target_sun->color, d->target_sun->health);			
		if(d->target_sun->health <= 0) {
			d->target_sun->health = 0;
			d->target_sun->curr_level = 0;
			d->target_sun->color = GRAY;
			d->target_sun->upgrade = 0;
			d->target_sun->sprite = getSprite(GRAY_SUN);
		}
		d->active = 0;
		return;
	}
}