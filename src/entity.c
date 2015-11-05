#include "entity.h"

extern void drawImage(SDL_Surface *, int, int);

extern sun *sun_list;
extern dot *dot_ptr_grid[SCREEN_HEIGHT / GRID_ROW_WIDTH][SCREEN_WIDTH / GRID_COL_WIDTH][NO_OF_COLORS / 2];
extern void generate_dots(sun *);
extern void move_dot(dot *);
extern int active_sun_colors[NO_OF_COLORS / 2];
extern SDL_Surface *getSprite(int);
extern int distance2(int, int, int, int);

unsigned int no_of_dots[NO_OF_COLORS / 2];

void initEntities()
{
/*
	int i, j;
	sun_ptr_array = (sun **)calloc(NO_OF_COLORS, sizeof(sun *));
	dot_ptr_grid = (dot ***)malloc(SCREEN_HEIGHT / GRID_ROW_WIDTH * sizeof(dot **));
	for(i = 0; i < SCREEN_HEIGHT / GRID_ROW_WIDTH; i++) {
		dot_ptr_grid[i] = (dot **)malloc(SCREEN_WIDTH / GRID_COL_WIDTH * sizeof(dot *));
		for(j = 0; j < SCREEN_WIDTH / GRID_COL_WIDTH; j++) {
			dot_ptr_grid[i][j] = (dot *)calloc((NO_OF_COLORS / 2), sizeof(dot *));
		}
	}
*/
}

void doEntities(int step)
{
	int i, j, k;
	dot *p, *q; /* *r, *s;  4 pointers because 4 possible dot colors */
	dot *temp; /* use to shift dots to new grid boxes */
	sun *s;
	/*
	printf("do entities in\n");
	for(i = 0; i < NO_OF_COLORS / 2; i++) {
		printf("%d : %d\n", i, active_sun_colors[i]);
	}
	*/
	
	/* move select_ring, if active and has a target sun */
	/*
	printf("target sun : %p\n", (void *)select_ring.target_sun);
	*/
	if(select_ring.active && select_ring.target_sun) {
		int sqdist = sqrt(distance2(select_ring.x, select_ring.y, select_ring.target_sun->x, select_ring.target_sun->y));
		int shift_x, shift_y;
		if(sqdist <= RING_REASONABLY_CLOSE) {
			select_ring.active = 0;
			select_ring.target_sun = NULL;
		}
		else {
			shift_x = (select_ring.target_sun->x - select_ring.x) * SELECT_RING_SPEED / sqdist;
			shift_y = (select_ring.target_sun->y - select_ring.y) * SELECT_RING_SPEED / sqdist;
			/*printf("sun x & y : %d : %d\nring x & y : %d : %d\nshift x & y : %d : %d\n", select_ring.target_sun->x, select_ring.target_sun->y, select_ring.x, select_ring.y, shift_x, shift_y);
			*/
			select_ring.x += shift_x;
			select_ring.y += shift_y;
		}
	}
	
	/* set up fights */
	p = NULL;
	q = NULL;
	for(i = 0; i < SCREEN_HEIGHT / GRID_ROW_WIDTH; i++) {
		for(j = 0; j < SCREEN_WIDTH / GRID_COL_WIDTH; j++) {
			p = dot_ptr_grid[i][j][0];
			for(k = 1; k < NO_OF_COLORS / 2; k++) {
				if(!p) {
					p = dot_ptr_grid[i][j][k];
				}
				else {
					q = dot_ptr_grid[i][j][k];
				}
				while(p && q) {
					while(p && p->target_dot) {
						p = p->next;
					}
					while(q && q->target_dot) {
						q = q->next;
					}
					if(p && q) {
						p->target_dot = q;
						q->target_dot = p;
						p = p->next;
						q = q->next;
						/*
						printf("targets assigned\n");
						*/
					}
				}
			}
		}
	}
	/* reset flock counts */
	s = sun_list;
	/* printf("%d BLUE %d s : %p\n", BLUE, i, (void *)s); */
	while(s) {
		/*
		printf("flock count : %d\n", s->flock_count);
		*/
		s->flock_count = 0;
		s = s->next;
	}
	/*
	printf("flock counts reset\n");
	*/
	for(i = 0; i < SCREEN_HEIGHT / GRID_ROW_WIDTH; i++) {
		for(j = 0; j < SCREEN_WIDTH / GRID_COL_WIDTH; j++) {
			for(k = 0; k < NO_OF_COLORS / 2; k++) {
				p = dot_ptr_grid[i][j][k];
				/* if all the first 'n' dots are inactive, then dot_ptr_arrry must also by shifted */
				while(p && (!p->active || !active_sun_colors[p->color / 2])) {
					dot_ptr_grid[i][j][k] = p->next;
					if(p->target_dot) {
						p->target_dot->target_dot = NULL;
						p->target_dot->target_x = p->target_dot->target_sun->x;
						p->target_dot->target_y = p->target_dot->target_sun->y;
					}
					no_of_dots[p->color / 2]--;
					free(p);
					p = dot_ptr_grid[i][j][k];
				}
				if(!p) {
					continue;
				}
				q = NULL;
				while(p) {
					/* remove inactive dots */
					if(!p->active || !active_sun_colors[p->color / 2]) {
						/*
						printf("%p\n", (void *)p);
						*/
						if(q) {
							q->next = p->next;
							no_of_dots[p->color / 2]--;
							free(p);
							p = q->next;
						}
						else { /* first dot in the list is being moved */
							dot_ptr_grid[i][j][k] = p->next;
							no_of_dots[p->color / 2]--;
							free(p);
							p = dot_ptr_grid[i][j][k];
						}
						/*
						printf("exit remove dot code\n");
						*/
					}
					else {
						/*
						printf("dot is active\n");
						*/
						/* move dot */
						p->move_dot(p);
						/* update flock counts */
						if(p->color == p->target_sun->color || p->color + 1 == p->target_sun->color)
							p->target_sun->flock_count++;
						else
							p->target_sun->flock_count--;
						/* put dot in correct grid box */
						if(
							(p->y < i * GRID_ROW_WIDTH) ||
							(p->y >= (i + 1) * GRID_ROW_WIDTH) ||
							(p->x < j * GRID_COL_WIDTH) ||
							(p->x >= (j + 1) * GRID_COL_WIDTH)
						) {
							if(q) {
								q->next = p->next;
							}
							else { /* first dot in the list is being moved */
								dot_ptr_grid[i][j][k] = p->next;
							}
							/* store a pointer to the next dot to be checked */
							temp = p->next;
							/* move the dot pointed to by p to the appropriate grid box */
							p->next = dot_ptr_grid[p->y / GRID_ROW_WIDTH][p->x / GRID_COL_WIDTH][p->color / 2];
							dot_ptr_grid[p->y / GRID_ROW_WIDTH][p->x / GRID_COL_WIDTH][p->color / 2] = p;
							/* point p back to the next dot to be checked */
							p = temp;
						}
						else { /* q will change only when checked point is not moved to another grid box */
							q = p;
							p = p->next;
						}
					}
				}
			}
		}
	}
	/*
	printf("dots moved and flock counts updated\n");
	*/
	/* PRINTING STATS OF EACH SUN
	s = sun_list;
	printf("index\tc_l\tm_l\tnear_en\tflock\tmin_en_dis\n");
	while(s) {
		printf("%d\t%d\t%d\t%d\t%d\t%d\n", s->graph_index, s->curr_level, s->max_level, s->nearby_enemies, s->flock_count, s->min_enemy_dist);
		s = s->next;
	}
	*/

	if(step % (100 / MIN_TICK_DELAY) == 0) {
		/* generate dots */
		s = sun_list;
		while(s) {
			/*
			printf("%d health : %d\n", s->color, s->health);
			printf("%d upgrade : %d\n", s->color, s->upgrade);
			*/
			if(s->color % 2) { /* non_gray color suns generate dots */
				generate_dots(s);
			}
			s = s->next;
		}
		/*
		printf("dots generated\n");
		*/
	}
	/*
	printf("do entities out\n");
	*/
}

extern Sprite sprite[MAX_SPRITES];

void drawEntities()
{
	int i, j, k;
	dot *d;
	sun *s;
	/*
	printf("draw entities in\n");
	*/
	
	/* Draw all suns */
	s = sun_list;
	while(s) {
		if(s->color % 2) {
			switch(s->curr_level) {
				case 1:
					drawImage(s->sprite, s->x - 60 / 2, s->y - 60 / 2);
					switch(s->max_level) {
						case 3:
							drawImage(getSprite(RING_2), s->x - 100 / 2, s->y - 100 / 2);
						case 2:
							drawImage(getSprite(RING_1), s->x - 80 / 2, s->y - 80 / 2);
					}
					break;
				case 2:
					drawImage(s->sprite, s->x - 80 / 2, s->y - 80 / 2);
					switch(s->max_level) {
						case 3:
							drawImage(getSprite(RING_2), s->x - 100 / 2, s->y - 100 / 2);
					}
					break;
				case 3:
					drawImage(s->sprite, s->x - 100 / 2, s->y - 100 / 2);
					break;
			}
			if(s->health < HEALTH_MAX) {
				/* health bar */
				drawImage(sprite[HEALTH_BAR_HOLLOW].image, s->x - 11, s->y + SUN_IMG_L / 2 + 5);
				for(i = 0; 5 * i < s->health; i++) {
					drawImage(sprite[HEALTH_BAR_SMALL].image, s->x - 10 + i, s->y + SUN_IMG_L / 2 + 6);
				}
				if(s->upgrade) {
					/* upgrade bar */
					drawImage(sprite[UPGRADE_BAR_HOLLOW].image, s->x - 11, s->y + SUN_IMG_L / 2 + 11);
					for(i = 0; 5 * i < s->upgrade; i++) {
						drawImage(s->bar_small, s->x - 10 + i, s->y + SUN_IMG_L / 2 + 12);
					}
				}
			}
			else {
				if(s->upgrade) {
					/* upgrade bar only */
					drawImage(sprite[UPGRADE_BAR_HOLLOW].image, s->x - 11, s->y + SUN_IMG_L / 2 + 5);
					for(i = 0; 5 * i < s->upgrade; i++) {
						drawImage(s->bar_small, s->x - 10 + i, s->y + SUN_IMG_L / 2 + 6);
					}
				}
			}
		}
		else {
			drawImage(s->sprite, s->x - 50 / 2, s->y - 50 / 2);
			switch(s->max_level) {
				case 3:
					drawImage(getSprite(RING_2), s->x - 100 / 2, s->y - 100 / 2);
				case 2:
					drawImage(getSprite(RING_1), s->x - 80 / 2, s->y - 80 / 2);
			}
			drawImage(sprite[HEALTH_BAR_HOLLOW].image, s->x - 11, s->y + SUN_IMG_L / 2 + 5);
			for(i = 0; 5 * i < s->health; i++) {
				drawImage(s->bar_small, s->x - 10 + i, s->y + SUN_IMG_L / 2 + 6);
			}
		}
		/*
		printf("draw a sun here\n");
		*/
		s = s->next;
	}
	/*
	printf("drew suns\n");
	*/
	/* Draw all dots */
	for(i = 0; i < SCREEN_HEIGHT / GRID_ROW_WIDTH; i++) {
		for(j = 0; j < SCREEN_WIDTH / GRID_COL_WIDTH; j++) {
			for(k = 0; k < NO_OF_COLORS / 2; k++) {
				d = dot_ptr_grid[i][j][k];
				while(d) {
					drawImage(d->sprite, d->x - DOT_IMG_W / 2, d->y - DOT_IMG_L / 2);
					/*
					printf("%d draw dot here\n", d->color);
					*/
					d = d->next;
				}
			}
		}
	}
	/* Draw select_ring if active */
	if(select_ring.active) {
		/* image size of select ring must be 2 * ANNULUS_OUTER_RADIUS X 2 * ANNULUS_OUTER_RADIUS */
		drawImage(select_ring.sprite, select_ring.x - ANNULUS_OUTER_RADIUS, select_ring.y - ANNULUS_OUTER_RADIUS);
	}
	/*
	printf("drew dots\n");
	printf("draw entities out\n");
	*/
}

