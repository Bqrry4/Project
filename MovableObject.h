#pragma once
#include "GObject.h"
#include "Entity.h"

class MovableObject : public GObject
{
	float vx, vy, MovableSpeed;
	Direction direction;

	void Movement();
public:
	MovableObject() : vy(0.f), vx(0.f), MovableSpeed(0.f), direction(Direction::Left)
	{
		Interact = true;
		collide.Is = true;
		collide.WithOthers = true;
	}
	~MovableObject() = default;
	void Update();
	bool Parse(XMLElement* root, int iObject = 0, XMLElement* xmlElem = nullptr);

	void Move(Direction);
};

