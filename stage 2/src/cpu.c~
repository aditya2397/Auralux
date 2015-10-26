#include "cpu.h"

extern sun *sun_list;
extern dot *dot_ptr_grid[SCREEN_HEIGHT / GRID_ROW_WIDTH][SCREEN_WIDTH / GRID_COL_WIDTH][NO_OF_COLORS / 2];
extern void move_dot(dot *);
extern void dot_reached_sun(dot *);
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
	dot *d;
	int i, j, k;
	/*
	sun *s = sun_list;
	*/
	for(i = 0; i < SCREEN_HEIGHT / GRID_ROW_WIDTH; i++) {
		for(j = 0; j < SCREEN_WIDTH / GRID_COL_WIDTH; j++) {
			for(k = 0; k < NO_OF_COLORS / 2; k++) {
				d = dot_ptr_grid[i][j][k];
				while(d) {
					/* checks if home sun needs to recover */
					if(!d->target_dot && (d->target_sun->health < HEALTH_MAX)) {
						if(distance2(d->x, d->y, d->target_sun->x, d->target_sun->y) <= SUN_REASONABLY_CLOSE * SUN_REASONABLY_CLOSE) {
							dot_reached_sun(d);
						}
						else {
							d->reached_sun = 0;
							d->target_x = d->target_sun->x;
							d->target_y = d->target_sun->y;
							d->move_dot = move_dot;
						}
					}
					d = d->next;
				}
			}
		}
	}
}
