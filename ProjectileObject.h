#pragma once
#include "AnimatedObj.h"
#include "Entity.h"

class ProjectileObject : public AnimatedObj
{
	float vx;
	//float vy;
	int lifespan; //deplacement in pixels

	int Damage;

	Direction direction;

public:
	ProjectileObject() : AnimatedObj(), vx(10.0f), lifespan(0), Damage(0), direction(Direction::Right)
	{
		Interact = true;
		collide.Is = true;
		//collide.WithOthers = true;
	}
	~ProjectileObject() = default;
	ProjectileObject(const ProjectileObject& projectile) = default;

	bool Parse(XMLElement* root, int iObject = 0, XMLElement* xmlElem = nullptr);

	void Movement();
	void Update();

	void AdaptPosition(SDL_Point, Direction);
	int DoDamage() { return Damage; }
	Direction FlyDirection() { return direction; }
	void SetDirection(Direction direction) { this->direction = direction; }
};

