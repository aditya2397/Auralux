#include "entity.h"

extern void drawImage(SDL_Surface *, int, int);

extern sun *sun_list;
extern dot *dot_ptr_grid[SCREEN_HEIGHT / GRID_ROW_WIDTH][SCREEN_WIDTH / GRID_COL_WIDTH][NO_OF_COLORS / 2];
extern void generate_dots(sun *);
extern void move_dot(dot *);

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

int removed[NO_OF_COLORS] = {0};

void doEntities(int step)
{
	int i, j, k;
	dot *p, *q; /* *r, *s;  4 pointers because 4 possible dot colors */
	dot *temp; /* use to shift dots to new grid boxes */
	sun *s;

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
				q = NULL;
				p = dot_ptr_grid[i][j][k];
				/* if all the first 'n' dots are inactive, then dot_ptr_arrry must also by shifted */
				while(p && !p->active) {
					removed[p->color]++;
					printf("%p\n", (void *)p);
					/*
					printf("%d dot removed\n", p->color);
					*/
					dot_ptr_grid[i][j][k] = p->next;
					free(p);
					p = dot_ptr_grid[i][j][k];
				}
				if(!p) {
					continue;
				}
				/*
				printf("inactive dots removed\n");
				*/
				while(p) {
					/* remove inactive dots */
					if(!p->active) {
						removed[p->color]++;
						printf("%p\n", (void *)p);
						/*
						printf("%d dot removed haha\n", p->color);
						*/
						if(q) {
							q->next = p->next;
							free(p);
							p = q->next;
						}
						else { /* first dot in the list is being moved */
							dot_ptr_grid[i][j][k] = p->next;
							free(p);
							p = dot_ptr_grid[i][j][k];
						}
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
	if(step % (100 / MIN_TICK_DELAY) == 0) {
		/* generate dots */
		s = sun_list;
		while(s) {
			printf("%d health : %d\n", s->color, s->health);
			printf("%d upgrade : %d\n", s->color, s->upgrade);
			if(s->color % 2) { /* non_gray color suns generate dots */
				if(s->color == GREEN) {
					printf("I am generating dots\n");
				}
				generate_dots(s);
			}
			s = s->next;
		}
		/*
		printf("dots generated\n");
		*/
	}
/*	printf("removed Blue : %d\n", removed[1]);
	printf("removed Orange : %d\n", removed[3]);
*/
}

extern Sprite sprite[MAX_SPRITES];

void drawEntities()
{
	int i, j, k;
	dot *d;
	sun *s;
	
	/*
	printf("entered draw entities\n");
	*/
	/* Draw all suns */
	s = sun_list;
	while(s) {
		if(s->color % 2) {
			drawImage(s->sprite, s->x - SUN_IMG_W / 2, s->y - SUN_IMG_L / 2);
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
			drawImage(s->sprite, s->x - SUN_IMG_W / 2, s->y - SUN_IMG_L / 2);
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
	/*
	printf("drew dots\n");
	printf("drew entities\n");
	*/
}
