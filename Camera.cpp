#include "Camera.h"
#include "Game.h"

void Camera::Update(Hitbox* target)
{
	int w = Game::ScreenWidth();
	int h = Game::ScreenHeigth();

	//Check colision with target and update Position
	if (target->x < Position.x + w / 3 && Position.x > 0)
	{      
		Position.x -= Position.x + w / 3 - target->x;
	}

	if (target->x + target->w > Position.x + 2*w / 3 && Position.x < borderW - w)
	{
		Position.x += target->x + target->w - Position.x - 2 * w / 3;
	}

	if (Position.x < 0) Position.x = 0;

	if (target->y < Position.y + h / 3 && Position.y > 0)
	{
		Position.y -= Position.y + h / 3 - target->y;
	}

	if (target->y + target->h > Position.y + 2 * h / 3 && Position.y < borderH - h)
	{
		Position.y += target->y + target->h - Position.y - 2 * h / 3;
	}

	if (Position.y < 0) Position.y = 0;
}