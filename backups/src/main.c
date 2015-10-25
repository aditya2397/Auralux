/* This tutorial will open a 640 x 480 screen,
** load an image and allow the user to control it
** using the arrow keys. Pressing Space will fire
** a bullet. Pressing escape
** or closing the window will exit the program
*/

#include "main.h"

extern void init(char *);
extern void cleanup(void);
extern void getInput(void);
extern void draw(void);
extern void initPlayer(void);
extern void doPlayer(void);
extern void doEntities(int);
extern void loadAllSprites(void);
extern void loadAllSounds(void);
extern void delay(unsigned int);
extern void clearEntities(void);

extern void initEntities();
extern void initLevel();
extern void clearInput();
extern void cpuThink();

int main(int argc, char *argv[])
{
	unsigned int frameLimit = SDL_GetTicks() + MIN_TICK_DELAY;
	int step = 2500;
	/* Start up SDL */
	
	init("Auralux 1");
	printf("init\n");
	/* Call the cleanup function when the program exits */
	
	atexit(cleanup);
	printf("atexit\n");
		
	/* Load all the sprites */
	
	loadAllSprites();
	printf("load all sprites\n");
	
	/* Load all the sounds
	
	loadAllSounds();*/
	
	/* initialise sun */
	
	clearInput();
	initEntities();
	printf("init entities\n");
	initLevel();
	printf("init level\n");
	
	/* Loop indefinitely for messages */
	
	while (step)
	{
		printf("step : %d\n", step);
		/*
		*/
		/* Get the input */
		
		getInput();
		printf("get input\n");
		/*
		*/
		/* Update the entities */
		
		doEntities(step);
		printf("do entities\n");
		doPlayer();
		printf("do player\n");
		/*
		*/
		/* CPU opponents decide strategy */
		if(step % (100 / MIN_TICK_DELAY) == 0) {
			cpuThink();
			printf("CPU has thought about it\n");
		}
		
		/* Draw everything */
		
		draw();
		printf("draw\n");
		/*
		*/
		/* Sleep briefly to stop sucking up all the CPU time */
		
		delay(frameLimit);
		
		frameLimit = SDL_GetTicks() + MIN_TICK_DELAY;
		step--;
	}
	
	/* Exit the program */
	
	exit(0);
}