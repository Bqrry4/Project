#include "NPC.h"
#include "SystemTimer.h"

float SpeedX = 50;

void NPC::Movement()
{
	float dt = SystemTimer::GetInstance()->GetDt();

	if(vx == 0)
	vx = SpeedX;
	ObjState = (Uint16)NPCState::Run;

	if (vx > 0)
	{
		flip = SDL_FLIP_NONE;
		direction = Looking::Right;
	}
	if(vx < 0)
	{
		flip = SDL_FLIP_HORIZONTAL;
		direction = Looking::Left;
	}
	if (!collide.Below || abs(LastXPosition - hitbox.x) > deplasament || collide.Left || collide.Right)
	{
		vx *= -1;
		RandDeplasament();
	}
	hitbox.x += vx * dt;
}



void NPC::IsDiyng()
{
	if (HP <= 0)
	{
		if (ObjState != (Uint16)NPCState::Dying)
		{
			ObjState = (Uint16)NPCState::Dying;
			collide.WithOthers = false;
			AMode = true;
			frame.aFrame = 0;
			SDL_Log("%d", frame.aFrame);
		}
		else
		{
			//Delete the obj or not?
		}
	}

}

void NPC::Update()
{


	IsDiyng();
	if (ObjState != (Uint16)NPCState::Atack && ObjState != (Uint16)NPCState::Dying) {
		Movement();
	}
}