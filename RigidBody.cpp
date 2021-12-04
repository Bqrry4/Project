#include "RigidBody.h"
#include "SystemTimer.h"
#include <SDL.h>

//const float Gravity = 41.322;

void RigidBody::ApplyForceX(float Force)
{
	int dt = SystemTimer::GetInstance()->GetDt();

	vx = (Force / mass) * dt;
	SDL_Log("%f", vx);
	dx = vx * dt;
	SDL_Log("%f", dx);
}
//void RigidBody::ApplyForceY(float Force)
//{
//	int dt = SystemTimer::GetInstance()->GetDt();
//	vy = ((Force-Gravity) / mass) * dt;
//	dy = vy * dt;
//}

