#include "init.h"
#include <math.h>		
#include "defs.h"

extern void freeSprites(void);
extern void freeSounds(void);

extern void generate_dots(sun *);
extern SDL_Surface *getSprite(int);
char *get_timestamp();

void init(char *title)
{
	/* Initialise SDL Video and Audio */
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		
		exit(1);
	}
	
	/* Open a screen */
	
	game.screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWPALETTE | SDL_DOUBLEBUF/* | SDL_FULLSCREEN*/);
	
	if (game.screen == NULL)
	{
		printf("Couldn't set screen mode to %d x %d: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());

		exit(1);
	}
	
	/* Set the audio rate to 22050, 16 bit stereo, 2 channels and a 4096 byte buffer
	
	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0)
	{
		printf("Could not open audio: %s\n", Mix_GetError());
		
		exit(1);
	}*/
	
	/* Set the screen title */
	
	SDL_WM_SetCaption(title, NULL);
}

void cleanup()
{
	/* Free the sprites */
	
	freeSprites();
	
	/* Free the sounds
	
	freeSounds();*/
	
	/* Close the mixer
	
	Mix_CloseAudio();*/
	
	/* Shut down SDL */
	
	SDL_Quit();
}

extern int no_of_suns;
extern sun *sun_list;
extern int active_sun_colors[NO_OF_COLORS / 2];
extern int **edge_matrix;
extern struct player_select_ring select_ring;

void initLevel() { /* later on, pass an integer to this function to indicate level, strcat this int to "level" and open the corresponding .txt config file */
	sun *s, *s2;
	int i, j;
	int color, curr_level, max_level;
	char input_system;
	float x_part, y_part;
	float radius, theta;
	int x_offset;
	int y_offset;
	FILE *fp;
	/*
	printf("init level in\n");
	*/
	select_ring.x = SCREEN_WIDTH / 2;
	select_ring.y = SCREEN_HEIGHT / 2;
	select_ring.active = 0;
	select_ring.target_sun = NULL;
	select_ring.sprite = getSprite(SELECT_RING);
	for(i = 0; i < NO_OF_COLORS / 2; i++) {
		active_sun_colors[i] = 0;
	}
	fp = fopen("config_levels/triad(original).txt", "r");
	if(!fp) {
		perror("fp didn't open file for reading");
	}
	fscanf(fp, "%d", &no_of_suns);
	fscanf(fp, " %c", &input_system);
	if(input_system == 'P') {
		fscanf(fp, "%d%d", &x_offset, &y_offset);
	}
	for(i = 0; i < no_of_suns; i++) {
		s = (sun *)malloc(sizeof(sun));
		if(!s) {
			perror("init sun malloc failed");
		}
		if(input_system == 'R') { /* rectangular representation */
			fscanf(fp, "%d%f%f%d%d", &color, &x_part, &y_part, &curr_level, &max_level);
			s->x = SCREEN_WIDTH * x_part;
			s->y = SCREEN_HEIGHT * y_part;
		}
		else { /* polar representation */
			fscanf(fp, "%d%f%f%d%d", &color, &radius, &theta, &curr_level, &max_level);
			/*printf("r, theta : %f , %f\n", radius, theta);*/
			theta = RADIAN(theta);
			s->x = SCREEN_WIDTH / 2 + x_offset + radius * cos(theta);
			s->y = SCREEN_HEIGHT / 2 + y_offset + radius * sin(theta);
			/*
			printf("rad theta : %f\n", theta);
			printf("cos, sin : %f, %f\n", cos(theta), sin(theta));
			printf("r * cos, r * sin : %f, %f\n", radius * cos(theta), radius * sin(theta));
			printf("x , y : %d , %d\n", s->x, s->y);
			*/
		}
		if(!s) {
			perror("init level : s");
		}
		s->color = color;
		s->health = 100;
		s->upgrade = 0;
		s->curr_level = curr_level;
		s->max_level = max_level;
		s->generate_dots = generate_dots;
		s->next = sun_list;
		s->nearby_enemies = 0;
		sun_list = s;
		s->graph_index = i;
		if(color) {
			active_sun_colors[color / 2]++;
		}
		switch(color) {
			case BLUE:
				s->bar_small = getSprite(BLUE_BAR_SMALL);
				switch(curr_level) {
					case 1:
						s->sprite = getSprite(BLUE_SUN_1);
						break;
					case 2:
						s->sprite = getSprite(BLUE_SUN_2);
						break;
					case 3:
						s->sprite = getSprite(BLUE_SUN_3);
						break;
				}
				break;
			case ORANGE:
				s->bar_small = getSprite(ORANGE_BAR_SMALL);
				switch(curr_level) {
					case 1:
						s->sprite = getSprite(ORANGE_SUN_1);
						break;
					case 2:
						s->sprite = getSprite(ORANGE_SUN_2);
						break;
					case 3:
						s->sprite = getSprite(ORANGE_SUN_3);
						break;
				}
				break;
			case PURPLE:
				s->bar_small = getSprite(PURPLE_BAR_SMALL);
				switch(curr_level) {
					case 1:
						s->sprite = getSprite(PURPLE_SUN_1);
						break;
					case 2:
						s->sprite = getSprite(PURPLE_SUN_2);
						break;
					case 3:
						s->sprite = getSprite(PURPLE_SUN_3);
						break;
				}				break;
			case GREEN:
				s->bar_small = getSprite(GREEN_BAR_SMALL);
				switch(curr_level) {
					case 1:
						s->sprite = getSprite(GREEN_SUN_1);
						break;
					case 2:
						s->sprite = getSprite(GREEN_SUN_2);
						break;
					case 3:
						s->sprite = getSprite(GREEN_SUN_3);
						break;
				}				break;
			case GRAY:
				s->sprite = getSprite(GRAY_SUN);
				s->bar_small = getSprite(GREEN_BAR_SMALL);
				s->health = 0;
				break;
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
	/* set nearby_enemies of each sun */
	s = sun_list;
	while(s) {
		s2 = s->next;
		while(s2) {
			if(edge_matrix[s->graph_index][s2->graph_index] && s->color && s2->color && (s->color != s2->color)) {
			/* edge exists, both suns are not GRAY and color of both suns is not same */
				s->nearby_enemies++;
				s2->nearby_enemies++;
			}
			s2 = s2->next;
		}
		s = s->next;
	}
	/*
	s = sun_list;
	while(s) {
		printf("graph_index : %d \tno of enemies : %d\n", s->graph_index, s->nearby_enemies);
		s = s->next;
	}
	*/
	/*
	for(i = 0; i < no_of_suns; i++) {
		for(j = 0; j < no_of_suns; j++) {
			printf("%d ", edge_matrix[i][j]);
		}
		printf("\n");
	}
	*/
	fclose(fp);
	/*
	printf("init level out\n");
	*/
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
	printf("%s\n", timestamp);
	return timestamp;
}
