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

#include "structs.h"
#include "defs.h"
#include "functions.h"

extern SDL_Surface *getSprite(int);

void update_nearby_suns();

unsigned int no_of_dots[NO_OF_COLORS / 2];

/* vector system used in XY plane */
void move_dot(dot *d) {
	int sq_dist;
	int shift_x, shift_y;
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
		}
		return;
	}
	/* move towards target */
	shift_x = (d->target_x - d->x) * DOT_SPEED / sq_dist;
	shift_y = (d->target_y - d->y) * DOT_SPEED / sq_dist;
	d->x += shift_x / 2;
	d->y += shift_y / 2;
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
	if(!d->reached_sun && distance2(d->x, d->y, d->target_x, d->target_y) <= SUN_REASONABLY_CLOSE) {
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
		temp->next = dot_ptr_grid[s->y / GRID_ROW_WIDTH][s->x / GRID_COL_WIDTH][s->color / 2];
		dot_ptr_grid[s->y / GRID_ROW_WIDTH][s->x / GRID_COL_WIDTH][s->color / 2] = temp;
		dots_per_cycle--;
	}
}

void move_to_annulus(dot *d) {
	sun *s = d->target_sun;
	int radius = ANNULUS_INNER_RADIUS + my_random() % (int)(ANNULUS_OUTER_RADIUS - ANNULUS_INNER_RADIUS);
	float theta = RADIAN(my_random()); /* theta will be in radians */
	d->target_x = s->x + radius * cos(theta);
	d->target_y = s->y + radius * sin(theta);
	d->move_dot = move_dot;
}

/* distance squared */
int distance2(int x1, int y1, int x2, int y2) {
	return (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
}

void update_nearby_suns() {
	sun *s = sun_list, *s2;
	int dist;
	/* resetting all values */
	while(s) {
		s->nearby_enemies = 0;
		s->min_enemy_dist = INT_MAX;
		s = s->next;
	}
	/* updating all values */
	s = sun_list;
	while(s) {
		s2 = s->next;
		while(s2) {
			if(edge_matrix[s->graph_index][s2->graph_index] && s->color && s2->color) {
				if(((s->color + 1) >> 1) != ((s2->color + 1) >> 1)) {
						s->nearby_enemies++;
						s2->nearby_enemies++;
				}
			}
			/* update min_enemy_dist */
			if(
				s->color && s2->color && /* both not GRAY */
				(s->color != s2->color) &&
				(((s->color + 1) >> 1) != ((s2->color + 1) >> 1)) /* suns are not friendly towards each other */
			) {
				dist = sqrt(distance2(s->x, s->y, s2->x, s2->y));
				if(dist < s->min_enemy_dist) {
					s->min_enemy_dist = dist;
				}
				if(dist < s2->min_enemy_dist) {
					s2->min_enemy_dist = dist;
				}
				
			}			
			s2 = s2->next;
		}
		s = s->next;
	}

}

void dot_reached_sun(dot *d) {
	d->reached_sun = 1;
	if(d->target_sun->color == d->color) { /* sun is a friendly */
		if(d->target_sun->health < HEALTH_MAX) { /* restore health */
			(d->target_sun->health)+= HEALTH_STEP;
			d->active = 0;
			return;
		}
		if(d->target_sun->curr_level < d->target_sun->max_level) { /* upgrade */
			(d->target_sun->upgrade) += UPGRADE_STEP;
			if(d->target_sun->upgrade >= UPGRADE_MAX) {
				d->target_sun->upgrade = 0;
				(d->target_sun->curr_level)++;
			}
			d->active = 0;
			return;
		}
		d->reached_sun = 1;
		move_to_annulus(d); /* else move to annulus */
		return;
	}
	else if(d->target_sun->color == d->color + 1) { /* + 1 means it is in GRAY_mycolor state, about-to-be-friendly sun */
		(d->target_sun->health) += HEALTH_STEP; /* restore health */
		if(d->target_sun->health >= HEALTH_MAX) {
			d->target_sun->upgrade = 0;
			d->target_sun->curr_level = 1;
			d->target_sun->health = HEALTH_MAX;
			d->target_sun->color = d->color;
			active_sun_colors[d->color / 2]++;
		}
		d->active = 0;
		return;
	}
	else if(d->target_sun->color == GRAY) { /* neutral sun */
		(d->target_sun->health) += HEALTH_STEP;
		d->target_sun->color = d->color + 1;
		d->active = 0;
		return;
	}
	else { /* enemy sun or gray-enemy sun */
		(d->target_sun->health) -= HEALTH_STEP;
		if(d->target_sun->health <= 0) {
			if(d->target_sun->color % 2) {
				active_sun_colors[d->target_sun->color / 2]--;
			}
			d->target_sun->health = 0;
			d->target_sun->curr_level = 0;
			d->target_sun->color = GRAY;
			d->target_sun->upgrade = 0;
		}
		d->active = 0;
		return;
	}
}

sun *nearest_empty_sun(int x, int y, sun *s) {
	sun *nearest = NULL, *curr_sun;
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
	return nearest;
}

/* accepts coordinates of a dot and returns a pointer to the weakest nearby sun */
sun *weakest_nearby_sun(float x, float y, int friendly_color, sun *s) {
	sun *weakest = NULL, *curr_sun;
	curr_sun = sun_list;
	while(curr_sun) {
		if((curr_sun->color != friendly_color) && (curr_sun->color != (friendly_color + 1))) {
			if(
				!edge_matrix[s->graph_index][curr_sun->graph_index] ||
				(curr_sun->flock_count + s->flock_count < (-1 * OVERBEARING_FLOCK_COUNT)) || /* don't attack suns that are already being attacked by a lot of dots */
				(curr_sun->flock_count > 1.5 * s->flock_count) /* pick on somebody your own size (or smaller) */
			) {
				curr_sun = curr_sun->next;
				continue;
			}
			else if(!weakest) {
				weakest = curr_sun;
			}
			else if((curr_sun->flock_count * distance2(x, y, curr_sun->x, curr_sun->y)) < (weakest->flock_count * distance2(x, y, weakest->x, weakest->y))) {
				weakest = curr_sun;
			}
		}
		curr_sun = curr_sun->next;
	}
	return weakest;
}
