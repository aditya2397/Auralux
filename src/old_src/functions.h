float distance2(float x1, float y1, float x2, float y2);
sun *weakest_nearby_sun(float x, float y, int friendly_color);
float distance2(x, y, curr_sun.x, curr_sun.y) < weakest.flock_count * distance2(x, y, weakest.x, weakest.y));
sun *nearest_empty_sun(int x, int y);
int initialise_suns(int level);
void update_suns();
void cpu_action(int color);
void dot_reached_sun(dot *d);
void move_dot(dot *d);
void random_init();
void generate_dots(sun *s);
void move_to_annulus(dot *d, sun *s);
void update_flock_count(sun *s);
void update_dots(dot_list *l);
