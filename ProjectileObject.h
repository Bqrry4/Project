#pragma once
#include "AnimatedObj.h"

class ProjectileObject : public AnimatedObj
{
	float vx;
	int lifespan;
public:
	ProjectileObject() : vx(0), lifespan(0)
	{}
	ProjectileObject(const Hitbox& hitbox)
	{	
		this->hitbox.x = hitbox.x + hitbox.w;
		this->hitbox.y = hitbox.h / 3 + hitbox.y;
		vx = 200;
		lifespan = 5000;
	}

	bool Parse(XMLElement* root, int iObject);

};

