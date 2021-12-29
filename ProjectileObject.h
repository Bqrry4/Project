#pragma once
#include "AnimatedObj.h"
#include "Entity.h"


class ProjectileObject : public AnimatedObj
{
	float vx;
	int lifespan; //deplacement in pixels

	int Damage;

	Looking direction;

public:
	ProjectileObject() : AnimatedObj(), vx(0.0f), lifespan(0), Damage(0), direction(Looking::Left)
	{}
	//ProjectileObject(const Hitbox& hitbox)
	//{
	//	this->hitbox.x = hitbox.x + hitbox.w;
	//	this->hitbox.y = hitbox.h / 3 + hitbox.y;
	//	vx = 200;
	//	lifespan = 5000;
	//}
	ProjectileObject(const ProjectileObject& projectile) = default;

	bool Parse(XMLElement* root, int iObject = 0, XMLElement* xmlElem = nullptr);

	void Movement();
	void Update();

	void SetDirection(Looking direction) { this->direction = direction; }
};

