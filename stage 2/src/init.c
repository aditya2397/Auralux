#include "init.h"
#include <math.h>		
#include "defs.h"

extern void freeSprites(void);
extern void freeSounds(void);

extern void generate_dots(sun *);
extern SDL_Surface *getSprite(int);
void init(char *title)
{
	/* Initialise SDL Video and Audio */
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		
		exit(1);
	}
	
	/* Open a screen */
	
	game.screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, SDL_HWPALETTE | SDL_DOUBLEBUF);
	
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

void initLevel() {
	sun *s;
	int i;
	for(i = 0; i < NO_OF_COLORS / 2; i++) {
		active_sun_colors[i] = 0;
	}
	/* first sun */
	s = (sun *)malloc(sizeof(sun));
	s->color = BLUE;
	s->health = 100;
	s->upgrade = 0;
	s->x = SCREEN_WIDTH / 4;
	s->y = SCREEN_HEIGHT / 4;
	
	s->curr_level = 3;
	s->max_level = 5;
	s->flock_count = 0;
	s->generate_dots = generate_dots;
	s->next = sun_list;
	sun_list = s;
	s->sprite = getSprite(BLUE_SUN);
	s->bar_small = getSprite(BLUE_BAR_SMALL);
		
	/* second sun */
	s = (sun *)malloc(sizeof(sun));
	s->color = ORANGE;
	s->health = 100;
	s->upgrade = 0;
	s->x = 3 * SCREEN_WIDTH / 4;
	s->y = SCREEN_HEIGHT / 4;
	
	s->curr_level = 1;
	s->max_level = 5;
	s->flock_count = 0;
	s->generate_dots = generate_dots;
	s->next = sun_list;
	sun_list = s;
	s->sprite = getSprite(ORANGE_SUN);
	s->bar_small = getSprite(ORANGE_BAR_SMALL);

	/* third sun */
	s = (sun *)malloc(sizeof(sun));
	s->color = GREEN;
	s->health = 100;
	s->upgrade = 0;
	s->x = SCREEN_WIDTH / 4;
	s->y = 3 * SCREEN_HEIGHT / 4;
	
	s->curr_level = 1;
	s->max_level = 5;
	s->flock_count = 0;
	s->generate_dots = generate_dots;
	s->next = sun_list;
	sun_list = s;
	s->sprite = getSprite(GREEN_SUN);
	s->bar_small = getSprite(GREEN_BAR_SMALL);
		
	/* fourth sun */
	s = (sun *)malloc(sizeof(sun));
	s->color = PURPLE;
	s->health = 100;
	s->upgrade = 0;
	s->x = 3 * SCREEN_WIDTH / 4;
	s->y = 3 * SCREEN_HEIGHT / 4;
	
	s->curr_level = 1;
	s->max_level = 5;
	s->flock_count = 0;
	s->generate_dots = generate_dots;
	s->next = sun_list;
	sun_list = s;
	s->sprite = getSprite(PURPLE_SUN);
	s->bar_small = getSprite(PURPLE_BAR_SMALL);

}
