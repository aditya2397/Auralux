#include "graphics.h"

SDL_Surface *loadImage(char *name)
{
	/* Load the image using SDL Image */
	
	SDL_Surface *temp = IMG_Load(name);
	SDL_Surface *image;
	
	if (temp == NULL)
	{
		printf("Failed to load image %s\n", name);
		
		return NULL;
	}
	
	/* Make the background transparent */
	
	SDL_SetColorKey(temp, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(temp->format, 0, 0, 0));
	
	/* Convert the image to the screen's native format */
	
	image = SDL_DisplayFormat(temp);
	
	SDL_FreeSurface(temp);
	
	if (image == NULL)
	{
		printf("Failed to convert image %s to native format\n", name);
		
		return NULL;
	}
	
	/* Return the processed image */
	
	return image;
}

void drawImage(SDL_Surface *image, int x, int y)
{
	SDL_Rect dest;
	
	/* Set the blitting rectangle to the size of the src image */
	
	dest.x = x;
	dest.y = y;
	dest.w = image->w;
	dest.h = image->h;
	
	/* Blit the entire image onto the screen at coordinates x and y */
	
	SDL_BlitSurface(image, NULL, game.screen, &dest);
}

void loadSprite(int index, char *name)
{
	/* Load the image into the next slot in the sprite bank */
	
	if (index >= MAX_SPRITES || index < 0)
	{
		printf("Invalid index for sprite! Index: %d Maximum: %d\n", index, MAX_SPRITES);
		
		exit(1);
	}
	
	sprite[index].image = loadImage(name);
	
	if (sprite[index].image == NULL)
	{
		exit(1);
	}
}

SDL_Surface *getSprite(int index)
{
	if (index >= MAX_SPRITES || index < 0)
	{
		printf("Invalid index for sprite! Index: %d Maximum: %d\n", index, MAX_SPRITES);
		
		exit(1);
	}
	
	return sprite[index].image;
}

void freeSprites()
{
	int i;
	
	/* Loop through the sprite bank and clear the images */
	
	for (i=0;i<MAX_SPRITES;i++)
	{
		if (sprite[i].image != NULL)
		{
			SDL_FreeSurface(sprite[i].image);
		}
	}
}

void loadAllSprites()
{
	/* dots */
	loadSprite(BLUE_DOT, "gfx/BLUE_DOT.png");
	loadSprite(ORANGE_DOT, "gfx/ORANGE_DOT.png");
	loadSprite(GREEN_DOT, "gfx/GREEN_DOT.png");
	loadSprite(PURPLE_DOT, "gfx/PURPLE_DOT.png");
	/* suns */
	loadSprite(BLUE_SUN, "gfx/BLUE_SUN.png");
	loadSprite(ORANGE_SUN, "gfx/ORANGE_SUN.png");
	loadSprite(GREEN_SUN, "gfx/GREEN_SUN.png");
	loadSprite(PURPLE_SUN, "gfx/PURPLE_SUN.png");
	loadSprite(GRAY_SUN, "gfx/GRAY_SUN.png");
	/* status bars */
	loadSprite(HEALTH_BAR_HOLLOW, "gfx/HEALTH_BAR_HOLLOW.png");
	loadSprite(UPGRADE_BAR_HOLLOW, "gfx/UPGRADE_BAR_HOLLOW.png");
	loadSprite(HEALTH_BAR_SMALL, "gfx/HEALTH_BAR_SMALL.png");
	loadSprite(BLUE_BAR_SMALL, "gfx/BLUE_BAR_SMALL.png");
	loadSprite(ORANGE_BAR_SMALL, "gfx/ORANGE_BAR_SMALL.png");
	loadSprite(GREEN_BAR_SMALL, "gfx/GREEN_BAR_SMALL.png");
	loadSprite(PURPLE_BAR_SMALL, "gfx/PURPLE_BAR_SMALL.png");
}
