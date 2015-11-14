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

#include "cpu.h"

extern sun *sun_list;
extern dot *dot_ptr_grid[SCREEN_HEIGHT / GRID_ROW_WIDTH][SCREEN_WIDTH / GRID_COL_WIDTH][NO_OF_COLORS / 2];
extern int no_of_suns;
extern int heuristic[NO_OF_COLORS / 2];
extern void move_dot(dot *);
extern void dot_reached_sun(dot *);
extern int distance2(int, int, int, int);
extern int **edge_matrix;
extern sun *nearest_empty_sun(int, int, sun *);
extern void move_to_annulus(dot *);
extern sun *weakest_nearby_sun(float, float, int, sun *);
extern int distance2(int, int, int, int);
extern void update_nearby_suns();

int heal_home_sun(dot *d);
int guard_home_sun(dot *d);
int guard_home_sun_timid(dot *d);
int help_urgent_friends(dot *d);
int help_relaxed_friends(dot *d);
int attack_empty_sun(dot *d);
int upgrade_home_sun(dot *d);
int attack_enemy_sun(dot *d);

void cpuThink() {
	static int redistribute = 0;
	dot *d;
	int i, j, k;
	int field_coverage;

	update_nearby_suns(); /* update nearby and enemy suns */
	for(i = 0; i < NO_OF_COLORS / 2; i++) { /* update heuristics */
		field_coverage = active_sun_colors[i] * 100 / no_of_suns;
		if(field_coverage > 50) {
			heuristic[i] = AGGRESSIVE;
		}
		else if(field_coverage > 20) {
			heuristic[i] = DEVELOPING;
		}
		else {
			heuristic[i] = TIMID;
		}
	}
	/* instructing dots */
	for(i = 0; i < SCREEN_HEIGHT / GRID_ROW_WIDTH; i++) {
		for(j = 0; j < SCREEN_WIDTH / GRID_COL_WIDTH; j++) {
			for(k = 0; k < NO_OF_COLORS / 2; k++) {
				d = dot_ptr_grid[i][j][k];
				while(d) {
					/* no further cpu instruction required, first reach target (or attack target dot) and then ask for further instructions */
					if(d->target_dot || !d->reached_sun) {
						d = d->next;
						continue;
					}
					if(heal_home_sun(d)) { /* checks if home sun needs to recover */
						d = d->next;
						continue;
					}
					if(k == 0) { /* player dots also protect home suns, no automation for player beyond that */
						break;
					}
					switch(heuristic[d->color / 2]) {
						case TIMID :
							if(
									guard_home_sun(d) ||
									help_urgent_friends(d) ||
									help_relaxed_friends(d) ||
									attack_empty_sun(d) ||
									upgrade_home_sun(d) ||
									attack_enemy_sun(d)
							  ) {
								d = d->next;
								continue;
							}
							break;
						case DEVELOPING :
							if(
									guard_home_sun(d) ||
									help_urgent_friends(d) ||
									upgrade_home_sun(d) ||
									attack_empty_sun(d) ||
									help_relaxed_friends(d) ||
									attack_enemy_sun(d)
							  ) {
								d = d->next;
								continue;
							}
							break;
						case AGGRESSIVE :
							if(
									guard_home_sun(d) ||
									help_urgent_friends(d) ||
									attack_enemy_sun(d) ||
									help_relaxed_friends(d) ||
									attack_empty_sun(d) ||
									upgrade_home_sun(d)
							  ) {
								d = d->next;
								continue;
							}
							break;
					}
					if(redistribute % 15 == 0) { /* if dot has no job, get redistributed at regular intervals */
						d->move_dot = move_to_annulus;
					}
					d = d->next;
				}
			}
		}
	}
	redistribute++;
}

int heal_home_sun(dot *d) { /* checks if home sun needs to recover */
	if(
			(((d->color + 1) >> 1) == (d->target_sun->color + 1) >> 1) &&
			(d->target_sun->health < HEALTH_MAX)
	  ) {
		if(distance2(d->x, d->y, d->target_sun->x, d->target_sun->y) <= SUN_REASONABLY_CLOSE * SUN_REASONABLY_CLOSE) {
			dot_reached_sun(d);
		}
		else {
			d->target_x = d->target_sun->x;
			d->target_y = d->target_sun->y;
			d->move_dot = move_dot;
			d->reached_sun = 0;
		}
		return 1;
	}
	return 0;
}

int guard_home_sun(dot *d) { /* don't leave home sun open to attack */
	if((d->target_sun->flock_count < d->target_sun->nearby_enemies * PER_ENEMY_FLOCK_COUNT)) {
		return 1;
	}
	return 0;
}

int help_urgent_friends(dot *d) { /* check if friends need to recover (color and GRAY-myColor) */
	sun *curr_sun = sun_list, *urgent_friend = NULL;

	while(curr_sun) {
		if(
				(d->target_sun != curr_sun) &&
				(((d->color + 1) >> 1) == (curr_sun->color + 1) >> 1) && /* sun is friendly */
				edge_matrix[d->target_sun->graph_index][curr_sun->graph_index] /* edge exists */
		  ) {
			if(!urgent_friend) {
				if(curr_sun->health < HEALTH_MAX || curr_sun->flock_count < 0) {
					urgent_friend = curr_sun;
					break;
				}

			}
		}
		curr_sun = curr_sun->next;
	}
	if(urgent_friend) {
		d->target_sun->flock_count--; /* leaving friendly home sun */
		urgent_friend->flock_count++; /* adding to flock count of a friendly sun */
		d->target_sun = urgent_friend;
		d->target_x = urgent_friend->x;
		d->target_y = urgent_friend->y;
		d->reached_sun = 0;
		return 1;
	}
	return 0;
}

int help_relaxed_friends(dot *d) {
	sun *curr_sun = sun_list, *urgent_friend = NULL;

	while(curr_sun) {
		if(
				(d->target_sun != curr_sun) &&
				(((d->color + 1) >> 1) == (curr_sun->color + 1) >> 1) && /* sun is friendly */
				edge_matrix[d->target_sun->graph_index][curr_sun->graph_index] /* edge exists */
		  ) {
			if(!urgent_friend) {
				if(curr_sun->min_enemy_dist < d->target_sun->min_enemy_dist) {
					urgent_friend = curr_sun;
				}
				if(curr_sun->nearby_enemies && !d->target_sun->nearby_enemies) {
					urgent_friend = curr_sun;
				}
			}
			else if(curr_sun->nearby_enemies > urgent_friend->nearby_enemies) { /* has more nearby enemies */
				urgent_friend = curr_sun;
			}
			else if(curr_sun->min_enemy_dist < urgent_friend->min_enemy_dist) { /* friend is nearer to enemies */
				urgent_friend = curr_sun;
			}
		}
		curr_sun = curr_sun->next;
	}
	if(urgent_friend) {
		d->target_sun->flock_count--;
		urgent_friend->flock_count++;
		d->target_sun = urgent_friend;
		d->target_x = urgent_friend->x;
		d->target_y = urgent_friend->y;
		d->reached_sun = 0;
		return 1;
	}
	return 0;
}

int attack_empty_sun(dot *d) {
	sun *s = nearest_empty_sun(d->target_sun->x, d->target_sun->y, d->target_sun);

	if(s) {
		d->target_sun->flock_count--;
		s->flock_count--;
		d->target_sun = s;
		d->target_x = s->x;
		d->target_y = s->y;
		d->reached_sun = 0;
		return 1;
	}
	return 0;
}

int upgrade_home_sun(dot *d) {
	if((d->target_sun-> color == d->color) && (d->target_sun->curr_level < d->target_sun->max_level)) {
		d->target_sun->flock_count--;
		d->target_x = d->target_sun->x;
		d->target_y = d->target_sun->y;
		d->reached_sun = 0;
		return 1;
	}
	return 0;
}

int attack_enemy_sun(dot *d) {
	sun *s = weakest_nearby_sun(d->target_sun->x, d->target_sun->y, d->color, d->target_sun);

	if(s) {
		d->target_sun->flock_count--;
		s->flock_count--;
		d->target_sun = s;
		d->target_x = s->x;
		d->target_y = s->y;
		d->reached_sun = 0;
		return 1;
	}
	return 0;
}
