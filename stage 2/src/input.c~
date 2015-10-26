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
			/*
			** Closing the Window or pressing Escape will exit the program
			** Pressing the arrow keys will move the player
			** Pressing space will fire a bullet
			*/
			case SDL_MOUSEBUTTONDOWN:
				if((event.button.button == SDL_BUTTON_LEFT) && (!input.mouse_down)) {
					input.mouse_down = 1;
					input.mouse_down_x = event.button.x;
					input.mouse_down_y = event.button.y;
					printf("mouse down\n x : %d\ny : %d\n", event.button.x, event.button.y);
				}
				break;
				
			case SDL_MOUSEBUTTONUP:
				if((event.button.button == SDL_BUTTON_LEFT) && (input.mouse_down)) {
					input.mouse_down = 0;
					input.mouse_up_x = event.button.x;
					input.mouse_up_y = event.button.y;
					printf("mouse up\n x : %d\ny : %d\n", event.button.x, event.button.y);
				}
				break;
			
			case SDL_QUIT:
				exit(0);
				break;
			
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_UP:
						input.up = 1;
						break;
					
					case SDLK_DOWN:
						input.down = 1;
						break;
						
					case SDLK_LEFT:
						input.left = 1;
						break;
						
					case SDLK_RIGHT:
						input.right = 1;
						break;
						
					case SDLK_SPACE:
						input.fire = 1;
						break;
					
					case SDLK_ESCAPE:
						exit(0);
						break;
					
					default:
					break;
				}
				break;
			
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
					case SDLK_UP:
						input.up = 0;
						break;
					
					case SDLK_DOWN:
						input.down = 0;
						break;
						
					case SDLK_LEFT:
						input.left = 0;
						break;
						
					case SDLK_RIGHT:
						input.right = 0;
						break;
						
					case SDLK_SPACE:
						input.fire = 0;
						break;
					
					default:
						break;
				}
				break;
		}
	}
}
