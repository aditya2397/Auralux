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

#include "entity.h"

extern sun *sun_list;
extern dot *dot_ptr_grid[SCREEN_HEIGHT / GRID_ROW_WIDTH][SCREEN_WIDTH / GRID_COL_WIDTH][NO_OF_COLORS / 2];
extern void generate_dots(sun *);
extern void move_dot(dot *);
extern int active_sun_colors[NO_OF_COLORS / 2];
extern SDL_Surface *getSprite(int);
extern int distance2(int, int, int, int);

unsigned int no_of_dots[NO_OF_COLORS / 2];
extern Game game;

void doEntities(int step) {
	int i, j, k;
	dot *p, *q;
	dot *temp; /* use to shift dots to new grid boxes */
	sun *s;
	if(select_ring.active && select_ring.target_sun) { /* move select_ring, if active and has a target sun */
		int sqdist = sqrt(distance2(select_ring.x, select_ring.y, select_ring.target_sun->x, select_ring.target_sun->y));
		int shift_x, shift_y;
		if(sqdist <= RING_REASONABLY_CLOSE) {
			select_ring.active = 0;
			select_ring.target_sun = NULL;
		}
		else {
			shift_x = (select_ring.target_sun->x - select_ring.x) * SELECT_RING_SPEED / sqdist;
			shift_y = (select_ring.target_sun->y - select_ring.y) * SELECT_RING_SPEED / sqdist;
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
					}
				}
			}
		}
	}
	/* reset flock counts */
	s = sun_list;
	while(s) {
		s->flock_count = 0;
		s = s->next;
	}
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
					if(!p->active || !active_sun_colors[p->color / 2]) { /* remove inactive dots */
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
					}
					else {
						p->move_dot(p); /* move dots */
						/* update flock counts */
						if(((p->color + 1) >> 1) == ((p->target_sun->color + 1) >> 1)) {
							p->target_sun->flock_count++;
						}
						else {
							p->target_sun->flock_count--;
						}
						if( /* put dot in correct grid box */
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
	if(step % ((1000 / MIN_TICK_DELAY) / UNIT_DOT_PRODUCTION_RATE) == 0) {
		/* generate dots */
		s = sun_list;
		while(s) {
			if(s->color % 2) { /* non_gray color suns generate dots */
				generate_dots(s);
			}
			s = s->next;
		}
	}
}


void drawEntities() {
	int i, j, k;
	dot *d;
	sun *s;	
	
	/* Draw select_ring if active */
	if(select_ring.active) {
		filledEllipseRGBA(game.screen, 
			select_ring.x, 
			select_ring.y, 
			1.5 * SUN_LEVEL3_RADIUS, 
			1.5 * SUN_LEVEL3_RADIUS, 
			255, 
			255, 
			255, 
			255);
		filledEllipseRGBA(game.screen, 
			select_ring.x, 
			select_ring.y, 
			1.4 * SUN_LEVEL3_RADIUS, 
			1.4 * SUN_LEVEL3_RADIUS, 
			0, 
			0, 
			0, 
			255);
	}
	/* Draw all suns */
	s = sun_list;
	while(s) {
		int upgrade_bar_offset = 0;
		/* draw rings */
		for(i = s->max_level; i > 1; i--) {
			filledEllipseRGBA(game.screen, 
				s->x, 
				s->y, 
				sun_radii[i], 
				sun_radii[i], 
				127, 
				127, 
				127, 
				255);
			filledEllipseRGBA(game.screen, 
				s->x, 
				s->y, 
				0.93 * sun_radii[i], 
				0.93 * sun_radii[i], 
				0, 
				0, 
				0, 
				255);
		}
		/* draw sun */
		filledEllipseRGBA(game.screen, 
			s->x, 
			s->y, 
			sun_radii[s->curr_level], 
			sun_radii[s->curr_level], 
			(sun_rgb[(s->color + 1)/ 2] >> 16) & 0xff, 
			(sun_rgb[(s->color + 1)/ 2] >> 8) & 0xff, 
			(sun_rgb[(s->color + 1)/ 2] >> 0) & 0xff, 
			255);
		/* draw status bars */
		if((s->health > 0) && (s->health < HEALTH_MAX)) { /* health bar */
			upgrade_bar_offset = 2 * BAR_SMALL_H;
			/* hollow health box */
			boxRGBA(game.screen, 
				s->x - HOLLOW_BAR_WIDTH / 2 - 2, 
				s->y + STATUS_BAR_Y_OFFSET, 
				s->x + HOLLOW_BAR_WIDTH / 2 + 2, 
				s->y + STATUS_BAR_Y_OFFSET + 2 * BAR_SMALL_H, 
				255, 
				255, 
				255, 
				255);
			boxRGBA(game.screen, 
				s->x - HOLLOW_BAR_WIDTH / 2, 
				s->y + STATUS_BAR_Y_OFFSET + 1, 
				s->x + HOLLOW_BAR_WIDTH / 2, 
				s->y + STATUS_BAR_Y_OFFSET + 2 * BAR_SMALL_H - 1, 
				0, 
				0, 
				0, 
				255);
			if(s->color % 2) {
				for(i = 0; 2 * i < s->health; i++) {
					rectangleRGBA(game.screen, 
						s->x - HOLLOW_BAR_WIDTH / 2 + i, 
						s->y + STATUS_BAR_Y_OFFSET + 1, 
						s->x - HOLLOW_BAR_WIDTH / 2 + i + BAR_SMALL_W, 
						s->y + STATUS_BAR_Y_OFFSET + 2 * BAR_SMALL_H - 1, 
						255, 
						0, 
						0, 
						255);					
				}
			}
			else {
				for(i = 0; 2 * i < s->health; i++) {
					rectangleRGBA(game.screen, 
						s->x - HOLLOW_BAR_WIDTH / 2 + i, 
						s->y + STATUS_BAR_Y_OFFSET + 1, 
						s->x - HOLLOW_BAR_WIDTH / 2 + i + BAR_SMALL_W, 
						s->y + STATUS_BAR_Y_OFFSET + 2 * BAR_SMALL_H - 1, 
						(dot_rgb[(s->color + 1) / 2] >> 16) & 0xff, 
						(dot_rgb[(s->color + 1) / 2] >> 8) & 0xff, 
						(dot_rgb[(s->color + 1) / 2] >> 0) & 0xff, 
						255);					
				}
			}
		}
		if(s->upgrade) { /* upgrade bar */
			/* hollow upgrade box */
			boxRGBA(game.screen, 
				s->x - HOLLOW_BAR_WIDTH / 2 - 2, 
				s->y + STATUS_BAR_Y_OFFSET + upgrade_bar_offset, 
				s->x + HOLLOW_BAR_WIDTH / 2 + 2, 
				s->y + STATUS_BAR_Y_OFFSET + 2 * BAR_SMALL_H + upgrade_bar_offset, 
				255, 
				255, 
				255, 
				255);
			boxRGBA(game.screen, 
				s->x - HOLLOW_BAR_WIDTH / 2, 
				s->y + STATUS_BAR_Y_OFFSET + upgrade_bar_offset + 1, 
				s->x + HOLLOW_BAR_WIDTH / 2, 
				s->y + STATUS_BAR_Y_OFFSET + 2 * BAR_SMALL_H + upgrade_bar_offset - 1, 
				0, 
				0, 
				0, 
				255);
			for(i = 0; 2 * i < s->upgrade; i++) {
				rectangleRGBA(game.screen, 
					s->x - HOLLOW_BAR_WIDTH / 2 + i, 
					s->y + STATUS_BAR_Y_OFFSET + upgrade_bar_offset + 1, 
					s->x - HOLLOW_BAR_WIDTH / 2 + i + BAR_SMALL_W, 
					s->y + STATUS_BAR_Y_OFFSET + 2 * BAR_SMALL_H + upgrade_bar_offset  - 1, 
					(dot_rgb[(s->color + 1)/ 2] >> 16) & 0xff, 
					(dot_rgb[(s->color + 1)/ 2] >> 8) & 0xff, 
					(dot_rgb[(s->color + 1)/ 2] >> 0) & 0xff, 
					255);
			}
		}
		s = s->next;
	}
	/* Draw all dots */
	for(i = 0; i < SCREEN_HEIGHT / GRID_ROW_WIDTH; i++) {
		for(j = 0; j < SCREEN_WIDTH / GRID_COL_WIDTH; j++) {
			for(k = 0; k < NO_OF_COLORS / 2; k++) {
				d = dot_ptr_grid[i][j][k];
				while(d) {
					filledEllipseRGBA(game.screen, 
						d->x, 
						d->y, 
						DOT_RADIUS, 
						DOT_RADIUS, 
						(dot_rgb[(d->color + 1)/ 2] >> 16) & 0xff, 
						(dot_rgb[(d->color + 1)/ 2] >> 8) & 0xff, 
						(dot_rgb[(d->color + 1)/ 2] >> 0) & 0xff, 
						255);				
					d = d->next;
				}
			}
		}
	}
}

