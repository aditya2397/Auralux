#include "bullet.h"

extern int getFreeEntity(void);
extern void drawStandardEntity(void);
extern SDL_Surface *getSprite(int);
extern void playSound(int);

static void moveStandardBullet(void);

void addBullet(int x, int y)
{
	int i = getFreeEntity();
	
	if (i == -1)
	{
		printf("Couldn't get a free slot for a bullet!\n");
		
		return;
	}
	
	entity[i].x = x;
	entity[i].y = y;
	entity[i].action = &moveStandardBullet;
	entity[i].draw = &drawStandardEntity;
	entity[i].sprite = getSprite(BULLET_SPRITE);
	
	/* Play a sound when the shot is fired */
	
	playSound(BULLET_SOUND);
}

static void moveStandardBullet()
{
	/* Move the bullet across the screen */
	
	self->x += BULLET_SPEED;
	
	/* Kill the bullet if it moves off the screen */
	
	if (self->x >= SCREEN_WIDTH)
	{
		self->active = 0;
	}
}
