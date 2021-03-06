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

#include "draw.h"

extern void drawPlayer(void);
extern void drawEntities(void);

void draw() {
	/* Fill the screen */
	SDL_FillRect(game.screen, NULL, 0);
	/* Draw the entities */
	drawEntities();
	/* Update the buffer */
	SDL_Flip(game.screen);
	/* Sleep briefly */
	SDL_Delay(1);
}

void delay(unsigned int frameLimit) {
	unsigned int ticks = SDL_GetTicks();
	if (frameLimit < ticks)	{
		return;
	}
	if (frameLimit > ticks + MIN_TICK_DELAY) {
		SDL_Delay(MIN_TICK_DELAY);
	}	
	else {
		SDL_Delay(frameLimit - ticks);
	}
}
