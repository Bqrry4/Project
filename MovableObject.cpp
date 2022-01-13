#include "MovableObject.h"
#include "SystemTimer.h"

void MovableObject::Movement()
{
	float dt = SystemTimer::GetInstance()->GetDt();

	if (!collide.Below)
	{
		vy += Gravity * dt;
		hitbox.y += vy * dt;
	}
	else
	{
		vy = 0.f;
	}

	if (vx > 0 && !(direction == Direction::Left && collide.Left) && !(direction == Direction::Right && collide.Right))
	{
		vx *= 0.9;
		hitbox.x += (int)direction * vx * dt;
	}
	else
	{
		vx = 0.f;
	}
}

void MovableObject::Move(Direction direction)
{
	vx = 150.f;
	this->direction = direction;
}

void MovableObject::Update()
{
	Movement();
}