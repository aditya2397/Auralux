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

#ifndef FUNCTIONS
#define FUNCTIONS

int distance2(int x1, int y1, int x2, int y2);
sun *weakest_nearby_sun(float x, float y, int friendly_color, sun *s);
sun *nearest_empty_sun(int x, int y, sun *s);
int initialise_suns(int level);
void update_suns();
void cpu_action(int color);
void dot_reached_sun(dot *d);
void move_dot(dot *d);
void random_init();
void generate_dots(sun *s);
void move_to_annulus(dot *d);
void update_flock_count(sun *s);
int my_random();

#endif
