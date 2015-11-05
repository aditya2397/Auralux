#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"
#include "defs.h"
#include "functions.h"

extern SDL_Surface *getSprite(int);

unsigned int no_of_dots[NO_OF_COLORS / 2];

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
			/*
			printf("dots have fought\n");
			*/
		}
		/* printf("dot at target\n"); */
		return;
	}
	/* move towards target */
	shift_x = (d->target_x - d->x) * DOT_SPEED / sq_dist;
	shift_y = (d->target_y - d->y) * DOT_SPEED / sq_dist;
/*
	if(d->target_dot) {
		shift_x /= 2;
		shift_y /= 2;
	}
*/
	d->x += shift_x / 2;
	d->y += shift_y / 2;
	/*
	printf("d->x : %d\nd->y : %d\n", d->x, d->y);
	*/
	if(d->x >= SCREEN_WIDTH) {
		d->x = SCREEN_WIDTH - 1;
	}
	else if(d->x < 0) {
		d->x = 0;
	}
	if(d->y >= SCREEN_HEIGHT) {
		d->y = SCREEN_HEIGHT - 1;
	}
	else if(d->y < 0) {
		d->y = 0;
	}
	/*
	if(shift_x / 2) {
		d->x += shift_x / 2;
	}
	else {
		if(shift_x > 0) {
			d->x += 1;
		}
		else if(shift_x < 0) {
			d->x -= 1;
		}
	}
	
	if(shift_y / 2) {
		d->y += shift_y / 2;
	}
	else {
		if(shift_y > 0) {
			d->y += 1;
		}
		else if(shift_y < 0) {
			d->y -= 1;
		}
	}
	*/
	if(!d->reached_sun && distance2(d->x, d->y, d->target_x, d->target_y) <= SUN_REASONABLY_CLOSE) {
		/*
		printf("dot reached sun after going through movedot function\n");
		*/
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
	/*
	printf("generate dots in\n");
	*/
	/* no_of_dots[s->color / 2] += dots_per_cycle; */
	while(dots_per_cycle) { /* gray suns have curr_level zero, so loop won't execute */
		if(s->health < HEALTH_MAX) { /* injured sun will prefer healing itself before generating dots */
			s->health++;
			dots_per_cycle--;
			continue;
		}
		temp = (dot *)malloc(sizeof(dot));
		no_of_dots[s->color / 2]++;
		if(!temp) {
			perror("generate dots : temp");
		}
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
	/*
	printf("generate dots out\n");
	*/
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
	/*
	printf("move to annulus in\n");
	*/
	d->target_x = s->x + radius * cos(theta);
	d->target_y = s->y + radius * sin(theta);
	d->move_dot = move_dot;
	/*
	printf("move to annulus out\n");
	*/
}

/* COMPLETED FUNCTIONS */

/* distance squared */
int distance2(int x1, int y1, int x2, int y2) {
	return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1); /* no need to take square root */
}

/* if dot is used, make it disappear before function return */
void dot_reached_sun(dot *d) {
	/*
	printf("dot reached sun in\n");
	*/
	d->reached_sun = 1;
	if(d->target_sun->color == d->color) { /* sun is a friendly */
		if(d->target_sun->health < HEALTH_MAX) { /* restore health */
			(d->target_sun->health)+= HEALTH_STEP;
			d->active = 0;
			/*
			printf("%d health : %d\n", d->target_sun->color, d->target_sun->health);			
			printf("dot reached sun out\n");
			*/
			return;
		}
		if(d->target_sun->curr_level < d->target_sun->max_level) { /* upgrade */
			(d->target_sun->upgrade) += UPGRADE_STEP;
			if(d->target_sun->upgrade >= UPGRADE_MAX) {
				d->target_sun->upgrade = 0;
				(d->target_sun->curr_level)++;
				switch(d->color) {
					case BLUE:
						switch(d->target_sun->curr_level) {
							case 1:
								d->target_sun->sprite = getSprite(BLUE_SUN_1);
								break;
							case 2:
								d->target_sun->sprite = getSprite(BLUE_SUN_2);
								break;
							case 3:
								d->target_sun->sprite = getSprite(BLUE_SUN_3);
								break;
						}
						break;
					case ORANGE:
						switch(d->target_sun->curr_level) {
							case 1:
								d->target_sun->sprite = getSprite(ORANGE_SUN_1);
								break;
							case 2:
								d->target_sun->sprite = getSprite(ORANGE_SUN_2);
								break;
							case 3:
								d->target_sun->sprite = getSprite(ORANGE_SUN_3);
								break;
						}
						break;
					case PURPLE:
						switch(d->target_sun->curr_level) {
							case 1:
								d->target_sun->sprite = getSprite(PURPLE_SUN_1);
								break;
							case 2:
								d->target_sun->sprite = getSprite(PURPLE_SUN_2);
								break;
							case 3:
								d->target_sun->sprite = getSprite(PURPLE_SUN_3);
								break;
						}
						break;
					case GREEN:
						switch(d->target_sun->curr_level) {
							case 1:
								d->target_sun->sprite = getSprite(GREEN_SUN_1);
								break;
							case 2:
								d->target_sun->sprite = getSprite(GREEN_SUN_2);
								break;
							case 3:
								d->target_sun->sprite = getSprite(GREEN_SUN_3);
								break;
						}
						break;
				}

				/* change the image of the sun here, add a new ring */
			}
			d->active = 0;
			/*
			printf("%d upgrade : %d\n", d->target_sun->color, d->target_sun->upgrade);			
			printf("dot reached sun out\n");
			*/
			return;
		}
		d->reached_sun = 1;
		move_to_annulus(d); /* else move to annulus */
		/*
		printf("dot reached sun out\n");
		*/
		return;
	}
	else if(d->target_sun->color == d->color + 1) { /* + 1 means it is in GRAY_mycolor state, about-to-be-friendly sun */
		(d->target_sun->health) += HEALTH_STEP; /* restore health */
		/*
		printf("%d health : %d\n", d->target_sun->color, d->target_sun->health);			
		*/
		if(d->target_sun->health >= HEALTH_MAX) {
			d->target_sun->upgrade = 0;
			d->target_sun->curr_level = 1;
			d->target_sun->health = HEALTH_MAX;
			d->target_sun->color = d->color;
			switch(d->color) {
				case BLUE:
					switch(d->target_sun->curr_level) {
						case 1:
							d->target_sun->sprite = getSprite(BLUE_SUN_1);
							break;
						case 2:
							d->target_sun->sprite = getSprite(BLUE_SUN_2);
							break;
						case 3:
							d->target_sun->sprite = getSprite(BLUE_SUN_3);
							break;
					}
					break;
				case ORANGE:
					switch(d->target_sun->curr_level) {
						case 1:
							d->target_sun->sprite = getSprite(ORANGE_SUN_1);
							break;
						case 2:
							d->target_sun->sprite = getSprite(ORANGE_SUN_2);
							break;
						case 3:
							d->target_sun->sprite = getSprite(ORANGE_SUN_3);
							break;
					}
					break;
				case PURPLE:
					switch(d->target_sun->curr_level) {
						case 1:
							d->target_sun->sprite = getSprite(PURPLE_SUN_1);
							break;
						case 2:
							d->target_sun->sprite = getSprite(PURPLE_SUN_2);
							break;
						case 3:
							d->target_sun->sprite = getSprite(PURPLE_SUN_3);
							break;
					}
					break;
				case GREEN:
					switch(d->target_sun->curr_level) {
						case 1:
							d->target_sun->sprite = getSprite(GREEN_SUN_1);
							break;
						case 2:
							d->target_sun->sprite = getSprite(GREEN_SUN_2);
							break;
						case 3:
							d->target_sun->sprite = getSprite(GREEN_SUN_3);
							break;
					}
					break;
			}
			active_sun_colors[d->color / 2]++;
			/* change the image of the sun here, to color of the dot */
		}
		d->active = 0;
		/*
		printf("dot reached sun out\n");
		*/
		return;
	}
	else if(d->target_sun->color == GRAY) { /* neutral sun */
		(d->target_sun->health) += HEALTH_STEP;
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
		/*
		printf("dot reached sun out\n");
		*/
		return;
	}
	else { /* enemy sun or gray-enemy sun */
		(d->target_sun->health) -= HEALTH_STEP;
		/*
		printf("%d health : %d\n", d->target_sun->color, d->target_sun->health);			
		*/
		if(d->target_sun->health <= 0) {
			if(d->target_sun->color % 2) {
				active_sun_colors[d->target_sun->color / 2]--;
			}
			d->target_sun->health = 0;
			d->target_sun->curr_level = 0;
			d->target_sun->color = GRAY;
			d->target_sun->upgrade = 0;
			d->target_sun->sprite = getSprite(GRAY_SUN);
		}
		d->active = 0;
		/*
		printf("dot reached sun out\n");
		*/
		return;
	}
}

sun *nearest_empty_sun(int x, int y, sun *s) {
	sun *nearest = NULL, *curr_sun;
	/*
	printf("nearest empty sun in\n");
	*/
	curr_sun = sun_list;
	while(curr_sun) {
		if(curr_sun->color % 2 == 0) {
			if(
				!edge_matrix[s->graph_index][curr_sun->graph_index] ||
				(curr_sun->flock_count < (-1 * OVERBEARING_FLOCK_COUNT))
			) { /* don't attack suns that are already being attacked by a lot of dots */
				curr_sun = curr_sun->next;
				continue;
			}
			else if(!nearest) {
				nearest = curr_sun;
			}
			else if(distance2(x, y, curr_sun->x, curr_sun->y) < distance2(x, y, nearest->x, nearest->y)) {
				nearest = curr_sun;
			}
		}
		curr_sun = curr_sun->next;
	}
	/*
	printf("nearest empty sun out\n");
	*/
	return nearest;
}

/* accepts coordinates of a dot and returns a pointer to the weakest nearby sun */
sun *weakest_nearby_sun(float x, float y, int friendly_color, sun *s) {
	sun *weakest = NULL, *curr_sun;
	/*
	printf("weakest nearby sun in\n");
	*/
	curr_sun = sun_list;
	while(curr_sun) {
		if((curr_sun->color != friendly_color) && (curr_sun->color != (friendly_color + 1))) {
			if(
				!edge_matrix[s->graph_index][curr_sun->graph_index] ||
				(curr_sun->flock_count + s->flock_count/**/ < (-1 * OVERBEARING_FLOCK_COUNT)) || /* don't attack suns that are already being attacked by a lot of dots */
				(curr_sun->flock_count > 1.5 * s->flock_count) /* pick on somebody your own size (or smaller) */
			) {
				curr_sun = curr_sun->next;
				continue;
			}
			else if(!weakest) {
				weakest = curr_sun;
			}
			else if((curr_sun->flock_count * distance2(x, y, curr_sun->x, curr_sun->y)) < (weakest->flock_count * distance2(x, y, weakest->x, weakest->y))) {
				/* divide flock count by some number if value of product goes out of integer range */
				weakest = curr_sun;
			}
		}
		curr_sun = curr_sun->next;
	}
	/*
	printf("weakest nearby sun out\n");
	*/
	return weakest;
}

