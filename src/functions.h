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
