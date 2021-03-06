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

#include "input.h"

void clearInput() {
	input.mouse_down = 0;
	input.mouse_down_x = -1;
	input.mouse_down_y = -1;
	input.mouse_up_x = -1;
	input.mouse_up_y = -1;

}

void getInput()
{
	SDL_Event event;
	
	/* Loop through waiting messages and process them */
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				if((event.button.button == SDL_BUTTON_LEFT) && (!input.mouse_down)) {
					input.mouse_down = 1;
					input.mouse_down_x = event.button.x;
					input.mouse_down_y = event.button.y;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if((event.button.button == SDL_BUTTON_LEFT) && (input.mouse_down)) {
					input.mouse_down = 0;
					input.mouse_up_x = event.button.x;
					input.mouse_up_y = event.button.y;
				}
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						exit(0);
						break;
					default:
						break;
				}
				break;
			case SDL_QUIT:
				exit(0);
				break;
		}
	}
}
