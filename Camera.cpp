#include "Camera.h"
#include "Game.h"

void Camera::Update(Hitbox* target)
{
	int w = Game::GetInstance()->ScreenWidth();
	int h = Game::GetInstance()->ScreenHeigth();

	//Check colision with target and update Position
	if (target->x < Position.x + w / 3 && Position.x > 0)
	{      
		Position.x -= Position.x + w / 3 - target->x;
	}

	if (target->x + target->w > Position.x + 2*w / 3 && Position.x < borderW - w)
	{

		Position.x += target->x + target->w - Position.x - 2 * w / 3;
	}
}