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

#include "player.h"
#include "defs.h"

extern int distance2(int, int, int, int);
extern void move_dot(dot *);
extern void clearInput();

extern sun *sun_list;
extern dot *dot_ptr_grid[SCREEN_HEIGHT / GRID_ROW_WIDTH][SCREEN_WIDTH / GRID_COL_WIDTH][NO_OF_COLORS / 2];
extern struct player_select_ring select_ring;

void doPlayer()
{
	sun *s;
	dot *d;
	static sun *initial_sun = NULL, *final_sun = NULL;
	int i, j;
	if(
		(input.mouse_down_x >= 0) &&
		(input.mouse_down_y >= 0) &&
		(input.mouse_up_x >= 0) &&
		(input.mouse_up_x >= 0)
	) { /* this means mouse was pressed and also released */
		if(distance2(input.mouse_down_x, input.mouse_down_y, input.mouse_up_x, input.mouse_up_y) <= MAX_CLICK_DIST2) {
			/* mouse motion was a click */
			if(!initial_sun) {
				/* initial sun being selected */
				s = sun_list;
				while(s) {
					if(distance2(input.mouse_down_x, input.mouse_down_y, s->x, s->y) <= (SUN_LEVEL3_RADIUS * SUN_LEVEL3_RADIUS)) {
						initial_sun = s;
						break;
					}
					s = s->next;
				}
				if(initial_sun) {
					select_ring.x = initial_sun->x;
					select_ring.y = initial_sun->y;
					select_ring.target_sun = NULL;
					select_ring.active = 1;
				}
			}
			else {
				/* final sun being selected */
				s = sun_list;
				while(s) {
					if(distance2(input.mouse_down_x, input.mouse_down_y, s->x, s->y) <= (SUN_LEVEL3_RADIUS * SUN_LEVEL3_RADIUS)) {
						final_sun = s;
						break;
					}
					s = s->next;
				}
				if(!final_sun) {
					return;
				}
				select_ring.target_sun = final_sun;
				for(i = 0; i < SCREEN_HEIGHT / GRID_ROW_WIDTH; i++) {
					for(j = 0; j < SCREEN_WIDTH / GRID_COL_WIDTH; j++) {
						d = dot_ptr_grid[i][j][BLUE / 2];
						while(d) {
							if(d->target_sun == initial_sun) {
								d->target_sun = final_sun;
								d->target_x = final_sun->x;
								d->target_y = final_sun->y;
								d->reached_sun = 0;
								d->move_dot = move_dot;
							}
							d = d->next;
						}
					}
				}
				initial_sun = NULL;
				final_sun = NULL;
			}
			clearInput();
		}
		else { /* mouse motion was a drag */
			return;
		}
	}
}
