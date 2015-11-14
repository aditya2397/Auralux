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

#include "init.h"
#include <math.h>		
#include "defs.h"

extern SDL_Surface *getSprite(int);
extern int my_random();
char *get_timestamp();
extern unsigned int sun_radii[];
	
void give_user_info();
void init_level_names();

void init(char *title) {
	/* Initialise SDL Video */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	/* Open a screen */
	game.screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWPALETTE | SDL_DOUBLEBUF | SDL_FULLSCREEN/*remove if incompatible*/);
	if (game.screen == NULL) {
		printf("Couldn't set screen mode to %d x %d: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}
	/* Set the screen title */
	SDL_WM_SetCaption(title, NULL);
}

void cleanup() {
	/* Shut down SDL */
	SDL_Quit();
}

extern int no_of_suns;
extern sun *sun_list;
extern int active_sun_colors[NO_OF_COLORS / 2];
extern int **edge_matrix;
extern struct player_select_ring select_ring;

void initLevel(char *level_index_str) {
	sun *s;
	int i, j;
	int color, curr_level, max_level;
	char input_system;
	char level_txt_file[100] = "config/levels/";
	int level_index = atoi(level_index_str);
	float x_part, y_part;
	float radius, theta;
	int x_offset, y_offset;
	FILE *fp;
	
	/* init select ring */
	select_ring.x = SCREEN_WIDTH / 2;
	select_ring.y = SCREEN_HEIGHT / 2;
	select_ring.active = 0;
	select_ring.target_sun = NULL;
	/* read file and generate level */
	if(level_index < 0 || level_index >= NO_OF_LEVELS) {
		give_user_info();
		exit(1);
	}
	strcat(level_txt_file, levels[level_index]);
	strcat(level_txt_file, ".txt");
	fp = fopen(level_txt_file, "r");
	if(!fp) {
		perror("fp didn't open file for reading level");
	}
	for(i = 0; i < NO_OF_COLORS / 2; i++) {
		active_sun_colors[i] = 0;
	}
	fscanf(fp, "%d", &no_of_suns);
	fscanf(fp, " %c", &input_system);
	if(input_system == 'P') {
		fscanf(fp, "%d%d", &x_offset, &y_offset);
	}
	for(i = 0; i < no_of_suns; i++) {
		s = (sun *)malloc(sizeof(sun));
		if(!s) {
			perror("init level : s");
		}
		if(input_system == 'R') { /* rectangular representation */
			fscanf(fp, "%d%f%f%d%d", &color, &x_part, &y_part, &curr_level, &max_level);
			s->x = SCREEN_WIDTH * x_part;
			s->y = SCREEN_HEIGHT * y_part;
		}
		else { /* polar representation */
			fscanf(fp, "%d%f%f%d%d", &color, &radius, &theta, &curr_level, &max_level);
			theta = RADIAN(theta);
			s->x = SCREEN_WIDTH / 2 + x_offset + radius * cos(theta);
			s->y = SCREEN_HEIGHT / 2 + y_offset + radius * sin(theta);
		}
		/* to provide randomness at the beginning */
		s->x += (-1 * MAX_SUN_POSITION_ERROR + 1 + my_random() % (2 * MAX_SUN_POSITION_ERROR));
		s->y += (-1 * MAX_SUN_POSITION_ERROR + 1 + my_random() % (2 * MAX_SUN_POSITION_ERROR));
		s->color = color;
		s->health = 100;
		s->upgrade = 0;
		s->curr_level = curr_level;
		s->max_level = max_level;
		s->next = sun_list;
		s->nearby_enemies = 0;
		sun_list = s;
		s->graph_index = i;
		if(color) {
			active_sun_colors[color / 2]++;
		}
		else { /* GRAY sun */
			s->health = 0;
		}
	}
	edge_matrix = (int **)malloc(no_of_suns * sizeof(int *));
	if(!edge_matrix) {
		perror("init level : edge matrix");
	}
	for(i = 0; i < no_of_suns; i++) {
		edge_matrix[i] = (int *)malloc(no_of_suns * sizeof(int));
		if(!edge_matrix[i]) {
			perror("init level : edge matrix [i]");
		}
		for(j = 0; j < no_of_suns; j++) {
			fscanf(fp, "%d", &edge_matrix[i][j]);
		}
	}
	fclose(fp);
	fp = fopen("config/colors.txt", "r");
	if(!fp) {
		perror("fp didn't open file for reading colors");
	}
	i = 0;
	i += fscanf(fp, "%x", &sun_rgb[(BLUE + 1)/ 2]);
	i += fscanf(fp, "%x", &dot_rgb[(BLUE + 1)/ 2]);
	i += fscanf(fp, "%x", &sun_rgb[(ORANGE + 1)/ 2]);
	i += fscanf(fp, "%x", &dot_rgb[(ORANGE + 1)/ 2]);
	i += fscanf(fp, "%x", &sun_rgb[(GREEN + 1)/ 2]);
	i += fscanf(fp, "%x", &dot_rgb[(GREEN + 1)/ 2]);
	i += fscanf(fp, "%x", &sun_rgb[(PURPLE + 1)/ 2]);
	i += fscanf(fp, "%x", &dot_rgb[(PURPLE + 1)/ 2]);
	sun_rgb[0] = 0x7f7f7f;
	dot_rgb[0] = 0;
	fclose(fp);
	if(i < 8) {
		printf("%d\n", i);
		printf("Colors not scanned correctly\n");
		exit(1);
	}
	/* set sun radius for each level */
	sun_radii[0] = GRAY_SUN_RADIUS;
	sun_radii[1] = SUN_LEVEL1_RADIUS;
	sun_radii[2] = SUN_LEVEL2_RADIUS;
	sun_radii[3] = SUN_LEVEL3_RADIUS;
}

char *get_timestamp() {
	time_t t;
	int i;
	char *timestamp = (char *)malloc(100);
	time(&t);
	strcpy(timestamp, ctime(&t));
	for(i = 0; timestamp[i] != '\0'; i++) {
		if(timestamp[i] == ' ' || timestamp[i] == '\n') {
			timestamp[i] = '_';
		}
		else if(timestamp[i] == ':') {
			timestamp[i] = '-';
		}
	}
	return timestamp;
}

void init_level_names() {
	int i;
	levels = (char **)malloc(NO_OF_LEVELS * sizeof(char *));
	if(!levels) {
		perror("init level names : levels");
	}
	for(i = 0; i < NO_OF_LEVELS; i++) {
		levels[i] = (char *)malloc(100);
		if(!levels[i]) {
			perror("init level names : levels[i]");
		}
	}
	strcpy(levels[CLASH], "clash");
	strcpy(levels[COCYTUS], "cocytus");
	strcpy(levels[HEX], "hex");
	strcpy(levels[RING], "ring");
	strcpy(levels[ROME], "rome");
	strcpy(levels[SPLIT], "split");
	strcpy(levels[TRIAD], "triad");
}

void give_user_info() {
	int i;
	printf("Available levels :\n");
	for(i = 0; i < NO_OF_LEVELS; i++) {
		printf("%d : %s\n", i, levels[i]);
	}
	printf("Enter corresponding number of level as argument to main\n");
	printf("To change colors of player and CPU opponents, change RGB hex values in \"config/colors.txt\"\n");
}
