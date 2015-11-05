#include "cpu.h"

extern sun *sun_list;
extern dot *dot_ptr_grid[SCREEN_HEIGHT / GRID_ROW_WIDTH][SCREEN_WIDTH / GRID_COL_WIDTH][NO_OF_COLORS / 2];
extern int no_of_suns;
extern void move_dot(dot *);
extern void dot_reached_sun(dot *);
extern int distance2(int, int, int, int);
extern int **edge_matrix;
extern sun *nearest_empty_sun(int, int, sun *);
extern void move_to_annulus(dot *);
extern sun *weakest_nearby_sun(float, float, int, sun *);
extern int distance2(int, int, int, int);

/*
   pass color of CPU
   function will return an integer corresponding to one of the following actions

   1 - target pointer points to his sun to be healed - urgent
   2 - target pointer points to a GRAY sun to occupy - only if reasonably close to its own suns
   3 - target pointer points to his sun to upgrade - not urgent
   4 - target pointer points to enemy's sun to attack - only if reasonably close to its own suns
*/

void cpuThink() {
	static int redistribute = 0;
	dot *d;
	int i, j, k;
	int dist;
	sun *s, *s2;
	int flag;
	/*
	sun *s = sun_list;
	printf("cpu think in\n");
	*/
	/* reseting nearby_enemies and min_enemy_dist of all suns */
	s = sun_list;
	while(s) {
		s->nearby_enemies = 0;
		s->min_enemy_dist = INT_MAX;
		s = s->next;
	}
	/* updating nearby_enemies and min_enemy_dist of each sun */
	s = sun_list;
	while(s) {
		s2 = s->next;
		while(s2) {
			if(
				s->color && s2->color && /* both not GRAY */
				(s->color != s2->color) &&
				(((s->color + 1) >> 1) != ((s2->color + 1) >> 1)) /* suns are not friendly towards each other */
			) {
				if(edge_matrix[s->graph_index][s2->graph_index]) { /* edge exists */
					s->nearby_enemies++;
					s2->nearby_enemies++;
				}
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
	/* sort the sun_list based on min_enemy_dist
	s = sun_list;
	while(s) {
		s2 = s->next;
		while(s2) {
			if(s->min_enemy_dist > s2->min_enemy_dist) {
				
			}
		}
		s = s->next;
	}*/
	/* instructing dots */
	for(i = 0; i < SCREEN_HEIGHT / GRID_ROW_WIDTH; i++) {
		for(j = 0; j < SCREEN_WIDTH / GRID_COL_WIDTH; j++) {
			for(k = 0; k < NO_OF_COLORS / 2; k++) {
				d = dot_ptr_grid[i][j][k];
				while(d) {
					/* no further cpu instruction required, first reach target (or attack target dot) and then ask for further instructions */
					/*if((!(d->reached_sun)) || d->target_dot) {*/
					if(
						d->target_dot ||/* target is a dot */
						(/* you havent reached sun and sun isn't friendly */
							!d->reached_sun /*&&
							(d->color != d->target_sun->color) &&
							(d->color + 1 != d->target_sun->color)*/
						)
					) {
						d = d->next;
						continue;
					}
					/* checks if home sun needs to recover */
					if(
						(
							(d->color == d->target_sun->color) ||
							(d->color + 1 == d->target_sun->color)
						) &&
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
						d = d->next;
						continue;
					}
					if(k == 0) { /* player dots also protect home suns, no automation for player beyond that */
						break;
					}
					/* don't leave home sun open to attack */
					if(
						/*d->target_sun->nearby_enemies && remove this condition if you want to make CPU dumber and initially more hesitant to conquer GRAY suns */
						(
							/*(d->target_sun->flock_count < SAFE_FLOCK_COUNT * d->target_sun->curr_level) ||*/
							(d->target_sun->flock_count < d->target_sun->nearby_enemies * PER_ENEMY_FLOCK_COUNT)
						)
					) {
						if(redistribute % 5 == 0) {
							d->move_dot = move_to_annulus;
						}
						d = d->next;
						continue;
					}
					/* NEW CONDITION
					if(!d->reached_sun) {
						d = d->next;
						continue;					
					}*/
					/* check if friends need to recover (color and GRAY-myColor) */
					s = sun_list;
					flag = 1;
					while(s && flag) { /* if friend sun urgently needs help */
						if(
							(d->target_sun != s) &&
							(/* sun is friendly */
								(d->color == s->color) ||
								(d->color + 1 == s->color)
							) &&
							edge_matrix[d->target_sun->graph_index][s->graph_index] && /* edge exists */
							(
								(s->health < HEALTH_MAX) || /* needs to heal */ 
								(
									(!d->target_sun->nearby_enemies)  &&
									(s->nearby_enemies) /* neighbour has enemies but you do not */
								) ||
								(s->flock_count < SAFE_FLOCK_COUNT)
							)
						) {
							d->target_sun->flock_count--; /* leaving friendly home sun */
							s->flock_count++; /* adding to flock count of a friendly sun */
							d->target_sun = s;
							d->target_x = s->x;
							d->target_y = s->y;
							flag = 0;
							d->reached_sun = 0;
							continue;
						}
						s = s->next;
					}
					if(!flag) {
						d = d->next;
						continue;
					}
					/* (new) if friend sun slightly needs help */
					/* check if friends need to recover (color and GRAY-myColor) */
					s = sun_list;
					flag = 1;
					while(s && flag) { /* if friend sun urgently needs help */
						if(
							(d->target_sun != s) &&
							(/* sun is friendly */
								(d->color == s->color) ||
								(d->color + 1 == s->color)
							) &&
							edge_matrix[d->target_sun->graph_index][s->graph_index] && /* edge exists */
							(
								(/* neither of you have enemies, but friend is nearer to enemies */
									(!s->nearby_enemies) &&
									(!d->target_sun->nearby_enemies) &&
									(s->min_enemy_dist < d->target_sun->min_enemy_dist)
								)
							)
						) {
							d->target_sun->flock_count--; /* leaving friendly home sun */
							s->flock_count++; /* adding to flock count of a friendly sun */
							d->target_sun = s;
							d->target_x = s->x;
							d->target_y = s->y;
							flag = 0;
							d->reached_sun = 0;
							continue;
						}
						s = s->next;
					}
					if(!flag) {
						d = d->next;
						continue;
					}
					/* attack nearest GRAY or GRAY-color sun (excluding GRAY-myColor suns) */
					/* s = nearest_empty_sun(d->x, d->y); */
					s = NULL;
					s = nearest_empty_sun(d->target_sun->x, d->target_sun->y, d->target_sun);
					if(s) {
						d->target_sun->flock_count--; /* leaving friendly home sun */
						s->flock_count--; /* subtracting from flock count of an non-friendly GRAY sun */
						d->target_sun = s;
						d->target_x = s->x;
						d->target_y = s->y;
						d->reached_sun = 0;
						d = d->next;
						continue;
					}
					/* upgrade home sun */
					if((d->target_sun-> color == d->color) && (d->target_sun->curr_level < d->target_sun->max_level)) {
						d->target_sun->flock_count--;
						d->target_x = d->target_sun->x;
						d->target_y = d->target_sun->y;
						d->reached_sun = 0;
						d = d->next;
						continue;
					}
					/* attack enemy suns */
					/* s = weakest_nearby_sun(d->x, d->y, d->color); */
					s = NULL;
					s = weakest_nearby_sun(d->target_sun->x, d->target_sun->y, d->color, d->target_sun);
					if(s) {
						d->target_sun->flock_count--; /* leaving friendly home sun */
						s->flock_count--; /* subtracting from flock count of an enemy sun */
						d->target_sun = s;
						d->target_x = s->x;
						d->target_y = s->y;
						d->reached_sun = 0;
						d = d->next;
						continue;
					}
					/* if dot has no job, get redistributed at regular intervals */
					if(redistribute % 15 == 0) {
						d->move_dot = move_to_annulus;
						/*d->reached_sun = 0;*/
					}
					d = d->next;
				}
			}
		}
	}
	redistribute++;
	/*
	printf("cpu think out\n");
	*/
}
