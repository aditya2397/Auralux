typedef struct sun {
	int color;
	int health, upgrade;
	float x, y;
	int curr_level, max_level; // curr_level decides size of sun's picture and max_level - curr_level decides number of rings around the sun's picture
	int flock_count;
	void (*generate_dots)(); // write this function and fill in parameters later
	struct sun *next;
}sun;

typedef struct dot {
	int color;
	char active;
	float x, y;
	float x_target, y_target;
	sun *target_sun; // can never be NULL
	char reached_sun; // when 1, means sun reached, check if target is dot
	struct dot *target_dot; // when NULL, then is no target
	// float curr_speed; // for smooth motion of dot, slowly speeding up and finally reaching a steady speed
	void (*move)(struct dot *); // this points to move_dot()
	struct dot *next;
}dot;
