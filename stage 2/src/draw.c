#include "draw.h"

extern void drawPlayer(void);
extern void drawEntities(void);

void draw()
{
	/* Blank the screen */
	
	SDL_FillRect(game.screen, NULL, 0);
	/* Draw the entities */
	
	drawEntities();
	
	/* Update the buffer */

	SDL_Flip(game.screen);
	
	/* Sleep briefly */
	
	SDL_Delay(1);
}

void delay(unsigned int frameLimit)
{
	unsigned int ticks = SDL_GetTicks();

	if (frameLimit < ticks)
	{
		return;
	}
	
	if (frameLimit > ticks + MIN_TICK_DELAY)
	{
		SDL_Delay(MIN_TICK_DELAY);
	}
	
	else
	{
		SDL_Delay(frameLimit - ticks);
	}
}
