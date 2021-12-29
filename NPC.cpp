#include "NPC.h"
#include "SystemTimer.h"


const float Height = 70;
const float TimetoApex = 0.4;

float SpeedY = 2.0f * Height / TimetoApex;
float Gravity = 2.0f * Height / TimetoApex / TimetoApex;

float SpeedX = 50;

void NPC::Movement()
{
	float dt = SystemTimer::GetInstance()->GetDt();

	if (!collide.Below)
	{
		vy += Gravity * dt;
		hitbox.y += vy * dt;
	}

	if (deplDt < 500)
	{
		vx = 0.0f;
		ObjState = (Uint16)NPCState::Repaos;
		deplDt += SystemTimer::GetInstance()->GetDt() * 1000;
	}
	else
	{
		vx = (int)direction * SpeedX;
		ObjState = (Uint16)NPCState::Run;
		if (abs(LastXPosition - hitbox.x) > deplasament || (collide.Right && direction == Looking::Right) || (collide.Left && direction == Looking::Left))
		{
			deplDt = 0;
			vx *= -1;
			RandDeplasament();
			LastXPosition = hitbox.x;
		}
	}


	if (vx > 0.0f)
	{
		flip = SDL_FLIP_NONE;
		direction = Looking::Right;
	}
	if(vx < 0.0f)
	{
		flip = SDL_FLIP_HORIZONTAL;
		direction = Looking::Left;
	}

	if (collide.Left && collide.Right)
	{
		vx = 0.0f;
		ObjState = (Uint16)NPCState::Repaos;
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