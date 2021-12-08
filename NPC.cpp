#include "NPC.h"
#include "SystemTimer.h"

float SpeedX = 50;

void NPC::Movement()
{
	float dt = SystemTimer::GetInstance()->GetDt();

	if (ObjState == (Uint16)NPCState::Atack) //If collide with player, vx = 0
	{
		vx = 0;
	}
	else
	{
		if(vx == 0)
		vx = SpeedX;
		ObjState = (Uint16)NPCState::Run;
	}

	if (vx > 0)
	{
		flip = SDL_FLIP_NONE;
	}
	else
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
	if (!collide.Below || abs(LastXPosition - hitbox.x) > deplasament || collide.Left || collide.Right)
	{
		vx *= -1;
		RandDeplasament();
	}

	hitbox.x += vx * dt;
}

void NPC::Atack()
{

}

void NPC::IsDiyng()
{
	if (HP < 0)
	{
		ObjState = (Uint16)NPCState::Dying;
		collide.WithOthers = false;
	}
}

void NPC::Update()
{
	IsDiyng();
	if(ObjState != (Uint16)NPCState::Dying)
	{ 
		Movement();
	}
}